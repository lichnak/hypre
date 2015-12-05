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
 * $Revision: 1.8 $
 ***********************************************************************EHEADER*/





/*
 * -- SuperLU routine (version 2.0) --
 * Univ. of California Berkeley, Xerox Palo Alto Research Center,
 * and Lawrence Berkeley National Lab.
 * November 15, 1997
 *
 * Changes made to this file corresponding to calls to blas/lapack functions
 * in Nov 2003 at LLNL
 */
/*
 * File name:	dlangs.c
 * History:     Modified from lapack routine DLANGE
 */
#include <math.h>
#include "dsp_defs.h"
#include "superlu_util.h"

double dlangs(char *norm, SuperMatrix *A)
{
/* 
    Purpose   
    =======   

    DLANGS returns the value of the one norm, or the Frobenius norm, or 
    the infinity norm, or the element of largest absolute value of a 
    real matrix A.   

    Description   
    ===========   

    DLANGE returns the value   

       DLANGE = ( max(abs(A(i,j))), NORM = 'M' or 'm'   
                (   
                ( norm1(A),         NORM = '1', 'O' or 'o'   
                (   
                ( normI(A),         NORM = 'I' or 'i'   
                (   
                ( normF(A),         NORM = 'F', 'f', 'E' or 'e'   

    where  norm1  denotes the  one norm of a matrix (maximum column sum), 
    normI  denotes the  infinity norm  of a matrix  (maximum row sum) and 
    normF  denotes the  Frobenius norm of a matrix (square root of sum of 
    squares).  Note that  max(abs(A(i,j)))  is not a  matrix norm.   

    Arguments   
    =========   

    NORM    (input) CHARACTER*1   
            Specifies the value to be returned in DLANGE as described above.   
    A       (input) SuperMatrix*
            The M by N sparse matrix A. 

   ===================================================================== 
*/
    
    /* Local variables */
    NCformat *Astore;
    double   *Aval;
    int      i, j, irow;
    double   value, sum;
    double   *rwork;

    Astore = A->Store;
    Aval   = Astore->nzval;
    
    if ( MIN(A->nrow, A->ncol) == 0) {
	value = 0.;
	
    } else if (superlu_lsame(norm, "M")) {
	/* Find max(abs(A(i,j))). */
	value = 0.;
	for (j = 0; j < A->ncol; ++j)
	    for (i = Astore->colptr[j]; i < Astore->colptr[j+1]; i++)
		value = MAX( value, fabs( Aval[i]) );
	
    } else if (superlu_lsame(norm, "O") || *(unsigned char *)norm == '1') {
	/* Find norm1(A). */
	value = 0.;
	for (j = 0; j < A->ncol; ++j) {
	    sum = 0.;
	    for (i = Astore->colptr[j]; i < Astore->colptr[j+1]; i++) 
		sum += fabs(Aval[i]);
	    value = MAX(value,sum);
	}
	
    } else if (superlu_lsame(norm, "I")) {
	/* Find normI(A). */
	if ( !(rwork = (double *) SUPERLU_MALLOC(A->nrow * sizeof(double))) )
	    ABORT("SUPERLU_MALLOC fails for rwork.");
	for (i = 0; i < A->nrow; ++i) rwork[i] = 0.;
	for (j = 0; j < A->ncol; ++j)
	    for (i = Astore->colptr[j]; i < Astore->colptr[j+1]; i++) {
		irow = Astore->rowind[i];
		rwork[irow] += fabs(Aval[i]);
	    }
	value = 0.;
	for (i = 0; i < A->nrow; ++i)
	    value = MAX(value, rwork[i]);
	
	SUPERLU_FREE (rwork);
	
    } else if (superlu_lsame(norm, "F") || superlu_lsame(norm, "E")) {
	/* Find normF(A). */
	ABORT("Not implemented.");
    } else
	ABORT("Illegal norm specified.");

    return (value);

} /* dlangs */
