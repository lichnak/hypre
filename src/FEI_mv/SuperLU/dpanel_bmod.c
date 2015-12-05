/*BHEADER**********************************************************************
 * Copyright (c) 2006   The Regents of the University of California.
 * Produced at the Lawrence Livermore National Laboratory.
 * Written by the HYPRE team. UCRL-CODE-222953.
 * All rights reserved.
 *
 * This file is part of HYPRE (see http://www.llnl.gov/CASC/hypre/).
 * Please see the COPYRIGHT_and_LICENSE file for the copyright notice, 
 * disclaimer, contact information and the GNU Lesser General Public License.
 *
 * HYPRE is free software; you can redistribute it and/or modify it under the 
 * terms of the GNU General Public License (as published by the Free Software
 * Foundation) version 2.1 dated February 1999.
 *
 * HYPRE is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the IMPLIED WARRANTY OF MERCHANTABILITY or FITNESS 
 * FOR A PARTICULAR PURPOSE.  See the terms and conditions of the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * $Revision: 1.9 $
 ***********************************************************************EHEADER*/





/*
 * -- SuperLU routine (version 2.0) --
 * Univ. of California Berkeley, Xerox Palo Alto Research Center,
 * and Lawrence Berkeley National Lab.
 * November 15, 1997
 *
 */
/*
  Copyright (c) 1994 by Xerox Corporation.  All rights reserved.
 
  THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY
  EXPRESSED OR IMPLIED.  ANY USE IS AT YOUR OWN RISK.
 
  Permission is hereby granted to use or copy this program for any
  purpose, provided the above notices are retained on all copies.
  Permission to modify the code and to distribute modified code is
  granted, provided the above notices are retained, and a notice that
  the code was modified is included with the above copyright notice.

  Changes made to this file corresponding to calls to blas/lapack functions
  in Nov 2003 at LLNL
*/

#include <stdio.h>
#include <stdlib.h>
#include "dsp_defs.h"
#include "superlu_util.h"

#ifndef HYPRE_USING_HYPRE_BLAS
#define USE_VENDOR_BLAS
#endif

/* 
 * Function prototypes 
 */
void sludlsolve(int, int, double *, double *);
void sludmatvec(int, int, int, double *, double *, double *);
extern void dcheck_tempv();

void
dpanel_bmod (
	    const int  m,          /* in - number of rows in the matrix */
	    const int  w,          /* in */
	    const int  jcol,       /* in */
	    const int  nseg,       /* in */
	    double     *dense,     /* out, of size n by w */
	    double     *tempv,     /* working array */
	    int        *segrep,    /* in */
	    int        *repfnz,    /* in, of size n by w */
	    GlobalLU_t *Glu        /* modified */
	    )
{
/* 
 * Purpose
 * =======
 *
 *    Performs numeric block updates (sup-panel) in topological order.
 *    It features: col-col, 2cols-col, 3cols-col, and sup-col updates.
 *    Special processing on the supernodal portion of L\U[*,j]
 *
 *    Before entering this routine, the original nonzeros in the panel 
 *    were already copied into the spa[m,w].
 *
 *    Updated/Output parameters-
 *	dense[0:m-1,w]: L[*,j:j+w-1] and U[*,j:j+w-1] are returned 
 *      collectively in the m-by-w vector dense[*]. 
 *
 */

#ifdef USE_VENDOR_BLAS
#ifdef _CRAY
    _fcd ftcs1 = _cptofcd("L", strlen("L")),
         ftcs2 = _cptofcd("N", strlen("N")),
         ftcs3 = _cptofcd("U", strlen("U"));
#endif
    int          incx = 1, incy = 1;
    double       alpha, beta;
#endif

    register int k, ksub;
    int          fsupc, nsupc, nsupr, nrow;
    int          krep, krep_ind;
    double       ukj, ukj1, ukj2;
    int          luptr, luptr1, luptr2;
    int          segsze;
    int          block_nrow;  /* no of rows in a block row */
    register int lptr;	      /* Points to the row subscripts of a supernode */
    int          kfnz, irow, no_zeros; 
    register int isub, isub1, i;
    register int jj;	      /* Index through each column in the panel */
    int          *xsup, *supno;
    int          *lsub, *xlsub;
    double       *lusup;
    int          *xlusup;
    int          *repfnz_col; /* repfnz[] for a column in the panel */
    double       *dense_col;  /* dense[] for a column in the panel */
    double       *tempv1;             /* Used in 1-D update */
    double       *TriTmp, *MatvecTmp; /* used in 2-D update */
    double      zero = 0.0;
#ifdef USE_VENDOR_BLAS
    double      one = 1.0;
#endif
    register int ldaTmp;
    register int r_ind, r_hi;
    static   int first = 1, maxsuper, rowblk, colblk;
    extern SuperLUStat_t SuperLUStat;
    flops_t  *ops = SuperLUStat.ops;
    
    xsup    = Glu->xsup;
    supno   = Glu->supno;
    lsub    = Glu->lsub;
    xlsub   = Glu->xlsub;
    lusup   = Glu->lusup;
    xlusup  = Glu->xlusup;
    
    if ( first ) {
	maxsuper = sp_ienv(3);
	rowblk   = sp_ienv(4);
	colblk   = sp_ienv(5);
	first = 0;
    }
    ldaTmp = maxsuper + rowblk;

    /* 
     * For each nonz supernode segment of U[*,j] in topological order 
     */
    k = nseg - 1;
    for (ksub = 0; ksub < nseg; ksub++) { /* for each updating supernode */

	/* krep = representative of current k-th supernode
	 * fsupc = first supernodal column
	 * nsupc = no of columns in a supernode
	 * nsupr = no of rows in a supernode
	 */
        krep = segrep[k--];
	fsupc = xsup[supno[krep]];
	nsupc = krep - fsupc + 1;
	nsupr = xlsub[fsupc+1] - xlsub[fsupc];
	nrow = nsupr - nsupc;
	lptr = xlsub[fsupc];
	krep_ind = lptr + nsupc - 1;

	repfnz_col = repfnz;
	dense_col = dense;
	
	if ( nsupc >= colblk && nrow > rowblk ) { /* 2-D block update */

	    TriTmp = tempv;
	
	    /* Sequence through each column in panel -- triangular solves */
	    for (jj = jcol; jj < jcol + w; jj++,
		 repfnz_col += m, dense_col += m, TriTmp += ldaTmp ) {

		kfnz = repfnz_col[krep];
		if ( kfnz == EMPTY ) continue;	/* Skip any zero segment */
	    
		segsze = krep - kfnz + 1;
		luptr = xlusup[fsupc];

		ops[TRSV] += segsze * (segsze - 1);
		ops[GEMV] += 2 * nrow * segsze;
	
		/* Case 1: Update U-segment of size 1 -- col-col update */
		if ( segsze == 1 ) {
		    ukj = dense_col[lsub[krep_ind]];
		    luptr += nsupr*(nsupc-1) + nsupc;

		    for (i = lptr + nsupc; i < xlsub[fsupc+1]; i++) {
			irow = lsub[i];
			dense_col[irow] -= ukj * lusup[luptr];
			++luptr;
		    }

		} else if ( segsze <= 3 ) {
		    ukj = dense_col[lsub[krep_ind]];
		    ukj1 = dense_col[lsub[krep_ind - 1]];
		    luptr += nsupr*(nsupc-1) + nsupc-1;
		    luptr1 = luptr - nsupr;

		    if ( segsze == 2 ) {
			ukj -= ukj1 * lusup[luptr1];
			dense_col[lsub[krep_ind]] = ukj;
			for (i = lptr + nsupc; i < xlsub[fsupc+1]; ++i) {
			    irow = lsub[i];
			    luptr++; luptr1++;
			    dense_col[irow] -= (ukj*lusup[luptr]
						+ ukj1*lusup[luptr1]);
			}
		    } else {
			ukj2 = dense_col[lsub[krep_ind - 2]];
			luptr2 = luptr1 - nsupr;
			ukj1 -= ukj2 * lusup[luptr2-1];
			ukj = ukj - ukj1*lusup[luptr1] - ukj2*lusup[luptr2];
			dense_col[lsub[krep_ind]] = ukj;
			dense_col[lsub[krep_ind-1]] = ukj1;
			for (i = lptr + nsupc; i < xlsub[fsupc+1]; ++i) {
			    irow = lsub[i];
			    luptr++; luptr1++; luptr2++;
			    dense_col[irow] -= ( ukj*lusup[luptr]
                             + ukj1*lusup[luptr1] + ukj2*lusup[luptr2] );
			}
		    }

		} else  {	/* segsze >= 4 */
		    
		    /* Copy U[*,j] segment from dense[*] to TriTmp[*], which
		       holds the result of triangular solves.    */
		    no_zeros = kfnz - fsupc;
		    isub = lptr + no_zeros;
		    for (i = 0; i < segsze; ++i) {
			irow = lsub[isub];
			TriTmp[i] = dense_col[irow]; /* Gather */
			++isub;
		    }
		    
		    /* start effective triangle */
		    luptr += nsupr * no_zeros + no_zeros;

#ifdef USE_VENDOR_BLAS
#ifdef _CRAY
		    STRSV( ftcs1, ftcs2, ftcs3, &segsze, &lusup[luptr], 
			   &nsupr, TriTmp, &incx );
#else
		    hypre_F90_NAME_BLAS(dtrsv,DTRSV)( "L", "N", "U", &segsze, &lusup[luptr], 
			   &nsupr, TriTmp, &incx );
#endif
#else		
		    sludlsolve ( nsupr, segsze, &lusup[luptr], TriTmp );
#endif
		    

		} /* else ... */
	    
	    }  /* for jj ... end tri-solves */

	    /* Block row updates; push all the way into dense[*] block */
	    for ( r_ind = 0; r_ind < nrow; r_ind += rowblk ) {
		
		r_hi = MIN(nrow, r_ind + rowblk);
		block_nrow = MIN(rowblk, r_hi - r_ind);
		luptr = xlusup[fsupc] + nsupc + r_ind;
		isub1 = lptr + nsupc + r_ind;
		
		repfnz_col = repfnz;
		TriTmp = tempv;
		dense_col = dense;
		
		/* Sequence through each column in panel -- matrix-vector */
		for (jj = jcol; jj < jcol + w; jj++,
		     repfnz_col += m, dense_col += m, TriTmp += ldaTmp) {
		    
		    kfnz = repfnz_col[krep];
		    if ( kfnz == EMPTY ) continue; /* Skip any zero segment */
		    
		    segsze = krep - kfnz + 1;
		    if ( segsze <= 3 ) continue;   /* skip unrolled cases */
		    
		    /* Perform a block update, and scatter the result of
		       matrix-vector to dense[].		 */
		    no_zeros = kfnz - fsupc;
		    luptr1 = luptr + nsupr * no_zeros;
		    MatvecTmp = &TriTmp[maxsuper];
		    
#ifdef USE_VENDOR_BLAS
		    alpha = one; 
                    beta = zero;
#ifdef _CRAY
		    SGEMV(ftcs2, &block_nrow, &segsze, &alpha, &lusup[luptr1], 
			   &nsupr, TriTmp, &incx, &beta, MatvecTmp, &incy);
#else
		    hypre_F90_NAME_BLAS(dgemv,DGEMV)("N", &block_nrow, &segsze, &alpha, &lusup[luptr1], 
			   &nsupr, TriTmp, &incx, &beta, MatvecTmp, &incy);
#endif
#else
		    sludmatvec(nsupr, block_nrow, segsze, &lusup[luptr1],
			   TriTmp, MatvecTmp);
#endif
		    
		    /* Scatter MatvecTmp[*] into SPA dense[*] temporarily
		     * such that MatvecTmp[*] can be re-used for the
		     * the next blok row update. dense[] will be copied into 
		     * global store after the whole panel has been finished.
		     */
		    isub = isub1;
		    for (i = 0; i < block_nrow; i++) {
			irow = lsub[isub];
			dense_col[irow] -= MatvecTmp[i];
			MatvecTmp[i] = zero;
			++isub;
		    }
		    
		} /* for jj ... */
		
	    } /* for each block row ... */
	    
	    /* Scatter the triangular solves into SPA dense[*] */
	    repfnz_col = repfnz;
	    TriTmp = tempv;
	    dense_col = dense;
	    
	    for (jj = jcol; jj < jcol + w; jj++,
		 repfnz_col += m, dense_col += m, TriTmp += ldaTmp) {
		kfnz = repfnz_col[krep];
		if ( kfnz == EMPTY ) continue; /* Skip any zero segment */
		
		segsze = krep - kfnz + 1;
		if ( segsze <= 3 ) continue; /* skip unrolled cases */
		
		no_zeros = kfnz - fsupc;		
		isub = lptr + no_zeros;
		for (i = 0; i < segsze; i++) {
		    irow = lsub[isub];
		    dense_col[irow] = TriTmp[i];
		    TriTmp[i] = zero;
		    ++isub;
		}
		
	    } /* for jj ... */
	    
	} else { /* 1-D block modification */
	    
	    
	    /* Sequence through each column in the panel */
	    for (jj = jcol; jj < jcol + w; jj++,
		 repfnz_col += m, dense_col += m) {
		
		kfnz = repfnz_col[krep];
		if ( kfnz == EMPTY ) continue;	/* Skip any zero segment */
		
		segsze = krep - kfnz + 1;
		luptr = xlusup[fsupc];

		ops[TRSV] += segsze * (segsze - 1);
		ops[GEMV] += 2 * nrow * segsze;
		
		/* Case 1: Update U-segment of size 1 -- col-col update */
		if ( segsze == 1 ) {
		    ukj = dense_col[lsub[krep_ind]];
		    luptr += nsupr*(nsupc-1) + nsupc;

		    for (i = lptr + nsupc; i < xlsub[fsupc+1]; i++) {
			irow = lsub[i];
			dense_col[irow] -= ukj * lusup[luptr];
			++luptr;
		    }

		} else if ( segsze <= 3 ) {
		    ukj = dense_col[lsub[krep_ind]];
		    luptr += nsupr*(nsupc-1) + nsupc-1;
		    ukj1 = dense_col[lsub[krep_ind - 1]];
		    luptr1 = luptr - nsupr;

		    if ( segsze == 2 ) {
			ukj -= ukj1 * lusup[luptr1];
			dense_col[lsub[krep_ind]] = ukj;
			for (i = lptr + nsupc; i < xlsub[fsupc+1]; ++i) {
			    irow = lsub[i];
			    ++luptr;  ++luptr1;
			    dense_col[irow] -= (ukj*lusup[luptr]
						+ ukj1*lusup[luptr1]);
			}
		    } else {
			ukj2 = dense_col[lsub[krep_ind - 2]];
			luptr2 = luptr1 - nsupr;
			ukj1 -= ukj2 * lusup[luptr2-1];
			ukj = ukj - ukj1*lusup[luptr1] - ukj2*lusup[luptr2];
			dense_col[lsub[krep_ind]] = ukj;
			dense_col[lsub[krep_ind-1]] = ukj1;
			for (i = lptr + nsupc; i < xlsub[fsupc+1]; ++i) {
			    irow = lsub[i];
			    ++luptr; ++luptr1; ++luptr2;
			    dense_col[irow] -= ( ukj*lusup[luptr]
                             + ukj1*lusup[luptr1] + ukj2*lusup[luptr2] );
			}
		    }

		} else  { /* segsze >= 4 */
		    /* 
		     * Perform a triangular solve and block update,
		     * then scatter the result of sup-col update to dense[].
		     */
		    no_zeros = kfnz - fsupc;
		    
		    /* Copy U[*,j] segment from dense[*] to tempv[*]: 
		     *    The result of triangular solve is in tempv[*];
		     *    The result of matrix vector update is in dense_col[*]
		     */
		    isub = lptr + no_zeros;
		    for (i = 0; i < segsze; ++i) {
			irow = lsub[isub];
			tempv[i] = dense_col[irow]; /* Gather */
			++isub;
		    }
		    
		    /* start effective triangle */
		    luptr += nsupr * no_zeros + no_zeros;
		    
#ifdef USE_VENDOR_BLAS
#ifdef _CRAY
		    STRSV( ftcs1, ftcs2, ftcs3, &segsze, &lusup[luptr], 
			   &nsupr, tempv, &incx );
#else
		    hypre_F90_NAME_BLAS(dtrsv,DTRSV)( "L", "N", "U", &segsze, &lusup[luptr], 
			   &nsupr, tempv, &incx );
#endif
		    
		    luptr += segsze;	/* Dense matrix-vector */
		    tempv1 = &tempv[segsze];
                    alpha = one;
                    beta = zero;
#ifdef _CRAY
		    SGEMV( ftcs2, &nrow, &segsze, &alpha, &lusup[luptr], 
			   &nsupr, tempv, &incx, &beta, tempv1, &incy );
#else
		    hypre_F90_NAME_BLAS(dgemv,DGEMV)( "N", &nrow, &segsze, &alpha, &lusup[luptr], 
			   &nsupr, tempv, &incx, &beta, tempv1, &incy );
#endif
#else
		    sludlsolve ( nsupr, segsze, &lusup[luptr], tempv );
		    
		    luptr += segsze;        /* Dense matrix-vector */
		    tempv1 = &tempv[segsze];
		    sludmatvec (nsupr,nrow,segsze,&lusup[luptr],tempv,tempv1);
#endif
		    
		    /* Scatter tempv[*] into SPA dense[*] temporarily, such
		     * that tempv[*] can be used for the triangular solve of
		     * the next column of the panel. They will be copied into 
		     * ucol[*] after the whole panel has been finished.
		     */
		    isub = lptr + no_zeros;
		    for (i = 0; i < segsze; i++) {
			irow = lsub[isub];
			dense_col[irow] = tempv[i];
			tempv[i] = zero;
			isub++;
		    }
		    
		    /* Scatter the update from tempv1[*] into SPA dense[*] */
		    /* Start dense rectangular L */
		    for (i = 0; i < nrow; i++) {
			irow = lsub[isub];
			dense_col[irow] -= tempv1[i];
			tempv1[i] = zero;
			++isub;	
		    }
		    
		} /* else segsze>=4 ... */
		
	    } /* for each column in the panel... */
	    
	} /* else 1-D update ... */

    } /* for each updating supernode ... */

}


