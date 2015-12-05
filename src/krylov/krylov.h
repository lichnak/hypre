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
 * $Revision: 2.26 $
 ***********************************************************************EHEADER*/


#include "HYPRE_krylov.h"

#ifndef hypre_KRYLOV_HEADER
#define hypre_KRYLOV_HEADER

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "_hypre_utilities.h"

#define hypre_CTAllocF(type, count, funcs) ( (type *)(*(funcs->CAlloc))((unsigned int)(count), (unsigned int)sizeof(type)) )

#define hypre_TFreeF( ptr, funcs ) ( (*(funcs->Free))((char *)ptr), ptr = NULL )

#ifdef __cplusplus
extern "C" {
#endif

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
 * $Revision: 2.26 $
 ***********************************************************************EHEADER*/



/******************************************************************************
 *
 * BiCGSTAB bicgstab
 *
 *****************************************************************************/

#ifndef hypre_KRYLOV_BiCGSTAB_HEADER
#define hypre_KRYLOV_BiCGSTAB_HEADER

/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------*/

/**
 * @name Generic BiCGSTAB Interface
 *
 * A general description of the interface goes here...
 *
 * @memo A generic BiCGSTAB linear solver interface
 * @version 0.1
 * @author Jeffrey F. Painter
 **/
/*@{*/

/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * hypre_BiCGSTABData and hypre_BiCGSTABFunctions
 *--------------------------------------------------------------------------*/


/**
 * @name BiCGSTAB structs
 *
 * Description...
 **/
/*@{*/

/**
 * The {\tt hypre\_BiCGSTABSFunctions} object ...
 **/

/* functions in pcg_struct.c which aren't used here:
char *hypre_ParKrylovCAlloc( int count , int elt_size );
int hypre_ParKrylovFree( char *ptr );
void *hypre_ParKrylovCreateVectorArray( int n , void *vvector );
int hypre_ParKrylovMatvecT( void *matvec_data , double alpha , void *A , void *x , double beta , void *y );
int hypre_ParKrylovClearVector( void *x );
*/
/* functions in pcg_struct.c which are used here:
  void *hypre_ParKrylovCreateVector( void *vvector );
  int hypre_ParKrylovDestroyVector( void *vvector );
  void *hypre_ParKrylovMatvecCreate( void *A , void *x );
  int hypre_ParKrylovMatvec( void *matvec_data , double alpha , void *A , void *x , double beta , void *y );
  int hypre_ParKrylovMatvecDestroy( void *matvec_data );
  double hypre_ParKrylovInnerProd( void *x , void *y );
  int hypre_ParKrylovCopyVector( void *x , void *y );
  int hypre_ParKrylovScaleVector( double alpha , void *x );
  int hypre_ParKrylovAxpy( double alpha , void *x , void *y );
  int hypre_ParKrylovCommInfo( void *A , int *my_id , int *num_procs );
  int hypre_ParKrylovIdentitySetup( void *vdata , void *A , void *b , void *x );
  int hypre_ParKrylovIdentity( void *vdata , void *A , void *b , void *x );
*/

typedef struct
{
  void *(*CreateVector)( void *vvector );
  int (*DestroyVector)( void *vvector );
  void *(*MatvecCreate)( void *A , void *x );
  int (*Matvec)( void *matvec_data , double alpha , void *A , void *x , double beta , void *y );
  int (*MatvecDestroy)( void *matvec_data );
  double (*InnerProd)( void *x , void *y );
  int (*CopyVector)( void *x , void *y );
  int (*ScaleVector)( double alpha , void *x );
  int (*Axpy)( double alpha , void *x , void *y );
  int (*CommInfo)( void *A , int *my_id , int *num_procs );
  int (*precond_setup)();
  int (*precond)();

} hypre_BiCGSTABFunctions;

/**
 * The {\tt hypre\_BiCGSTABData} object ...
 **/

typedef struct
{
   int      min_iter;
   int      max_iter;
   int      stop_crit;
   int      converged;
   double   tol;
   double   cf_tol;
   double   rel_residual_norm;

   void  *A;
   void  *r;
   void  *r0;
   void  *s;
   void  *v;
   void  *p;
   void  *q;

   void  *matvec_data;
   void    *precond_data;

   hypre_BiCGSTABFunctions * functions;

   /* log info (always logged) */
   int      num_iterations;
 
   /* additional log info (logged when `logging' > 0) */
   int      logging;
   int      print_level;
   double  *norms;
   char    *log_file_name;

} hypre_BiCGSTABData;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name generic BiCGSTAB Solver
 *
 * Description...
 **/
/*@{*/

/**
 * Description...
 *
 * @param param [IN] ...
 **/

   hypre_BiCGSTABFunctions *
   hypre_BiCGSTABFunctionsCreate(
      void *(*CreateVector)( void *vvector ),
      int (*DestroyVector)( void *vvector ),
      void *(*MatvecCreate)( void *A , void *x ),
      int (*Matvec)( void *matvec_data , double alpha , void *A , void *x , double beta , void *y ),
      int (*MatvecDestroy)( void *matvec_data ),
      double (*InnerProd)( void *x , void *y ),
      int (*CopyVector)( void *x , void *y ),
      int (*ScaleVector)( double alpha , void *x ),
      int (*Axpy)( double alpha , void *x , void *y ),
      int (*CommInfo)( void *A , int *my_id , int *num_procs ),
      int (*PrecondSetup) (void *vdata, void *A, void *b, void *x ),
      int (*Precond)  ( void *vdata, void *A, void *b, void *x )
      );


/**
 * Description...
 *
 * @param param [IN] ...
 **/

void *
hypre_BiCGSTABCreate( hypre_BiCGSTABFunctions * bicgstab_functions );


#ifdef __cplusplus
}
#endif

#endif
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
 * $Revision: 2.26 $
 ***********************************************************************EHEADER*/




/******************************************************************************
 *
 * cgnr (conjugate gradient on the normal equations A^TAx = A^Tb) functions
 *
 *****************************************************************************/

#ifndef hypre_KRYLOV_CGNR_HEADER
#define hypre_KRYLOV_CGNR_HEADER

/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------*/

/**
 * @name Generic CGNR Interface
 *
 * A general description of the interface goes here...
 *
 * @memo A generic CGNR linear solver interface
 * @version 0.1
 * @author Jeffrey F. Painter
 **/
/*@{*/

/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * hypre_CGNRData and hypre_CGNRFunctions
 *--------------------------------------------------------------------------*/


/**
 * @name CGNR structs
 *
 * Description...
 **/
/*@{*/

/**
 * The {\tt hypre\_CGNRSFunctions} object ...
 **/

typedef struct
{
   int    (*CommInfo)      ( void  *A, int   *my_id, int   *num_procs );
   void * (*CreateVector)  ( void *vector );
   int    (*DestroyVector) ( void *vector );
   void * (*MatvecCreate)  ( void *A, void *x );
   int    (*Matvec)        ( void *matvec_data, double alpha, void *A,
                             void *x, double beta, void *y );
   int    (*MatvecT)       ( void *matvec_data, double alpha, void *A,
                             void *x, double beta, void *y );
   int    (*MatvecDestroy) ( void *matvec_data );
   double (*InnerProd)     ( void *x, void *y );
   int    (*CopyVector)    ( void *x, void *y );
   int    (*ClearVector)   ( void *x );
   int    (*ScaleVector)   ( double alpha, void *x );
   int    (*Axpy)          ( double alpha, void *x, void *y );
   int    (*precond_setup) ( void *vdata, void *A, void *b, void *x );
   int    (*precond)       ( void *vdata, void *A, void *b, void *x );
   int    (*precondT)       ( void *vdata, void *A, void *b, void *x );
} hypre_CGNRFunctions;

/**
 * The {\tt hypre\_CGNRData} object ...
 **/

typedef struct
{
   double   tol;
   double   rel_residual_norm;
   int      min_iter;
   int      max_iter;
   int      stop_crit;

   void    *A;
   void    *p;
   void    *q;
   void    *r;
   void    *t;

   void    *matvec_data;
   void    *precond_data;

   hypre_CGNRFunctions * functions;

   /* log info (always logged) */
   int      num_iterations;

   /* additional log info (logged when `logging' > 0) */
   int      logging;
   double  *norms;
   char    *log_file_name;

} hypre_CGNRData;


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @name generic CGNR Solver
 *
 * Description...
 **/
/*@{*/


/**
 * Description...
 *
 * @param param [IN] ...
 **/
hypre_CGNRFunctions *
hypre_CGNRFunctionsCreate(
   int    (*CommInfo)      ( void  *A, int   *my_id, int   *num_procs ),
   void * (*CreateVector)  ( void *vector ),
   int    (*DestroyVector) ( void *vector ),
   void * (*MatvecCreate)  ( void *A, void *x ),
   int    (*Matvec)        ( void *matvec_data, double alpha, void *A,
                             void *x, double beta, void *y ),
   int    (*MatvecT)        ( void *matvec_data, double alpha, void *A,
                             void *x, double beta, void *y ),
   int    (*MatvecDestroy) ( void *matvec_data ),
   double (*InnerProd)     ( void *x, void *y ),
   int    (*CopyVector)    ( void *x, void *y ),
   int    (*ClearVector)   ( void *x ),
   int    (*ScaleVector)   ( double alpha, void *x ),
   int    (*Axpy)          ( double alpha, void *x, void *y ),
   int    (*PrecondSetup)  ( void *vdata, void *A, void *b, void *x ),
   int    (*Precond)       ( void *vdata, void *A, void *b, void *x ),
   int    (*PrecondT)       ( void *vdata, void *A, void *b, void *x )
   );

/**
 * Description...
 *
 * @param param [IN] ...
 **/

void *
hypre_CGNRCreate( hypre_CGNRFunctions *cgnr_functions );

#ifdef __cplusplus
}
#endif

#endif
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
 * $Revision: 2.26 $
 ***********************************************************************EHEADER*/



/******************************************************************************
 *
 * GMRES gmres
 *
 *****************************************************************************/

#ifndef hypre_KRYLOV_GMRES_HEADER
#define hypre_KRYLOV_GMRES_HEADER

/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------*/

/**
 * @name Generic GMRES Interface
 *
 * A general description of the interface goes here...
 *
 * @memo A generic GMRES linear solver interface
 * @version 0.1
 * @author Jeffrey F. Painter
 **/
/*@{*/

/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * hypre_GMRESData and hypre_GMRESFunctions
 *--------------------------------------------------------------------------*/


/**
 * @name GMRES structs
 *
 * Description...
 **/
/*@{*/

/**
 * The {\tt hypre\_GMRESFunctions} object ...
 **/

typedef struct
{
   char * (*CAlloc)        ( int count, int elt_size );
   int    (*Free)          ( char *ptr );
   int    (*CommInfo)      ( void  *A, int   *my_id, int   *num_procs );
   void * (*CreateVector)  ( void *vector );
   void * (*CreateVectorArray)  ( int size, void *vectors );
   int    (*DestroyVector) ( void *vector );
   void * (*MatvecCreate)  ( void *A, void *x );
   int    (*Matvec)        ( void *matvec_data, double alpha, void *A,
                             void *x, double beta, void *y );
   int    (*MatvecDestroy) ( void *matvec_data );
   double (*InnerProd)     ( void *x, void *y );
   int    (*CopyVector)    ( void *x, void *y );
   int    (*ClearVector)   ( void *x );
   int    (*ScaleVector)   ( double alpha, void *x );
   int    (*Axpy)          ( double alpha, void *x, void *y );

   int    (*precond)();
   int    (*precond_setup)();

} hypre_GMRESFunctions;

/**
 * The {\tt hypre\_GMRESData} object ...
 **/

/* rel_change!=0 means: if pass the other stopping criteria,
 also check the relative change in the solution x.
   stop_crit!=0 means: absolute error tolerance rather than
 the usual relative error tolerance on the residual.  Never
 applies if rel_change!=0.
*/

typedef struct
{
   int      k_dim;
   int      min_iter;
   int      max_iter;
   int      rel_change;
   int      stop_crit;
   int      converged;
   double   tol;
   double   cf_tol;
   double   rel_residual_norm;

   void  *A;
   void  *r;
   void  *w;
   void  **p;

   void    *matvec_data;
   void    *precond_data;

   hypre_GMRESFunctions * functions;

   /* log info (always logged) */
   int      num_iterations;
 
   int     print_level; /* printing when print_level>0 */
   int     logging;  /* extra computations for logging when logging>0 */
   double  *norms;
   char    *log_file_name;

} hypre_GMRESData;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name generic GMRES Solver
 *
 * Description...
 **/
/*@{*/

/**
 * Description...
 *
 * @param param [IN] ...
 **/

hypre_GMRESFunctions *
hypre_GMRESFunctionsCreate(
   char * (*CAlloc)        ( int count, int elt_size ),
   int    (*Free)          ( char *ptr ),
   int    (*CommInfo)      ( void  *A, int   *my_id, int   *num_procs ),
   void * (*CreateVector)  ( void *vector ),
   void * (*CreateVectorArray)  ( int size, void *vectors ),
   int    (*DestroyVector) ( void *vector ),
   void * (*MatvecCreate)  ( void *A, void *x ),
   int    (*Matvec)        ( void *matvec_data, double alpha, void *A,
                             void *x, double beta, void *y ),
   int    (*MatvecDestroy) ( void *matvec_data ),
   double (*InnerProd)     ( void *x, void *y ),
   int    (*CopyVector)    ( void *x, void *y ),
   int    (*ClearVector)   ( void *x ),
   int    (*ScaleVector)   ( double alpha, void *x ),
   int    (*Axpy)          ( double alpha, void *x, void *y ),
   int    (*PrecondSetup)  ( void *vdata, void *A, void *b, void *x ),
   int    (*Precond)       ( void *vdata, void *A, void *b, void *x )
   );

/**
 * Description...
 *
 * @param param [IN] ...
 **/

void *
hypre_GMRESCreate( hypre_GMRESFunctions *gmres_functions );

#ifdef __cplusplus
}
#endif
#endif
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
 * $Revision: 2.26 $
 ***********************************************************************EHEADER*/




/******************************************************************************
 *
 * Preconditioned conjugate gradient (Omin) headers
 *
 *****************************************************************************/

#ifndef hypre_KRYLOV_PCG_HEADER
#define hypre_KRYLOV_PCG_HEADER

/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------*/

/**
 * @name Generic PCG Interface
 *
 * A general description of the interface goes here...
 *
 * @memo A generic PCG linear solver interface
 * @version 0.1
 * @author Jeffrey F. Painter
 **/
/*@{*/

/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * hypre_PCGData and hypre_PCGFunctions
 *--------------------------------------------------------------------------*/


/**
 * @name PCG structs
 *
 * Description...
 **/
/*@{*/

/**
 * The {\tt hypre\_PCGSFunctions} object ...
 **/

typedef struct
{
   char * (*CAlloc)        ( int count, int elt_size );
   int    (*Free)          ( char *ptr );
   int    (*CommInfo)      ( void  *A, int   *my_id, int   *num_procs );
   void * (*CreateVector)  ( void *vector );
   int    (*DestroyVector) ( void *vector );
   void * (*MatvecCreate)  ( void *A, void *x );
   int    (*Matvec)        ( void *matvec_data, double alpha, void *A,
                             void *x, double beta, void *y );
   int    (*MatvecDestroy) ( void *matvec_data );
   double (*InnerProd)     ( void *x, void *y );
   int    (*CopyVector)    ( void *x, void *y );
   int    (*ClearVector)   ( void *x );
   int    (*ScaleVector)   ( double alpha, void *x );
   int    (*Axpy)          ( double alpha, void *x, void *y );

   int    (*precond)();
   int    (*precond_setup)();
} hypre_PCGFunctions;

/**
 * The {\tt hypre\_PCGData} object ...
 **/

/*
 Summary of Parameters to Control Stopping Test:
 - Standard (default) error tolerance: |delta-residual|/|right-hand-side|<tol
 where the norm is an energy norm wrt preconditioner, |r|=sqrt(<Cr,r>).
 - two_norm!=0 means: the norm is the L2 norm, |r|=sqrt(<r,r>)
 - rel_change!=0 means: if pass the other stopping criteria, also check the
 relative change in the solution x.
 - stop_crit!=0 means: pure absolute error tolerance rather than a pure relative
 error tolerance on the residual.  Never applies if rel_change!=0 or atolf!=0.
 - atolf = absolute error tolerance factor to be used _together_ with the
 relative error tolerance, |delta-residual| / ( atolf + |right-hand-side| ) < tol
 - tol = relative error tolerance, as above
 - cf_tol = convergence factor tolerance; if >0 used for special test
 for slow convergence
*/

typedef struct
{
   double   tol;
   double   atolf;
   double   cf_tol;
   int      max_iter;
   int      two_norm;
   int      rel_change;
   int      stop_crit;
   int      converged;

   void    *A;
   void    *p;
   void    *s;
   void    *r; /* ...contains the residual.  This is currently kept permanently.
                  If that is ever changed, it still must be kept if logging>1 */

   int      owns_matvec_data;  /* normally 1; if 0, don't delete it */
   void    *matvec_data;
   void    *precond_data;

   hypre_PCGFunctions * functions;

   /* log info (always logged) */
   int      num_iterations;
   double   rel_residual_norm;

   int     print_level; /* printing when print_level>0 */
   int     logging;  /* extra computations for logging when logging>0 */
   double  *norms;
   double  *rel_norms;

} hypre_PCGData;

#define hypre_PCGDataOwnsMatvecData(pcgdata)  ((pcgdata) -> owns_matvec_data)

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @name generic PCG Solver
 *
 * Description...
 **/
/*@{*/

/**
 * Description...
 *
 * @param param [IN] ...
 **/

hypre_PCGFunctions *
hypre_PCGFunctionsCreate(
   char * (*CAlloc)        ( int count, int elt_size ),
   int    (*Free)          ( char *ptr ),
   int    (*CommInfo)      ( void  *A, int   *my_id, int   *num_procs ),
   void * (*CreateVector)  ( void *vector ),
   int    (*DestroyVector) ( void *vector ),
   void * (*MatvecCreate)  ( void *A, void *x ),
   int    (*Matvec)        ( void *matvec_data, double alpha, void *A,
                             void *x, double beta, void *y ),
   int    (*MatvecDestroy) ( void *matvec_data ),
   double (*InnerProd)     ( void *x, void *y ),
   int    (*CopyVector)    ( void *x, void *y ),
   int    (*ClearVector)   ( void *x ),
   int    (*ScaleVector)   ( double alpha, void *x ),
   int    (*Axpy)          ( double alpha, void *x, void *y ),
   int    (*PrecondSetup)  ( void *vdata, void *A, void *b, void *x ),
   int    (*Precond)       ( void *vdata, void *A, void *b, void *x )
   );

/**
 * Description...
 *
 * @param param [IN] ...
 **/

void *
hypre_PCGCreate( hypre_PCGFunctions *pcg_functions );

#ifdef __cplusplus
}
#endif

#endif

/* bicgstab.c */
hypre_BiCGSTABFunctions *hypre_BiCGSTABFunctionsCreate ( void *(*CreateVector )(void *vvector ), int (*DestroyVector )(void *vvector ), void *(*MatvecCreate )(void *A ,void *x ), int (*Matvec )(void *matvec_data ,double alpha ,void *A ,void *x ,double beta ,void *y ), int (*MatvecDestroy )(void *matvec_data ), double (*InnerProd )(void *x ,void *y ), int (*CopyVector )(void *x ,void *y ), int (*ScaleVector )(double alpha ,void *x ), int (*Axpy )(double alpha ,void *x ,void *y ), int (*CommInfo )(void *A ,int *my_id ,int *num_procs ), int (*PrecondSetup )(void *vdata ,void *A ,void *b ,void *x ), int (*Precond )(void *vdata ,void *A ,void *b ,void *x ));
void *hypre_BiCGSTABCreate ( hypre_BiCGSTABFunctions *bicgstab_functions );
int hypre_BiCGSTABDestroy ( void *bicgstab_vdata );
int hypre_BiCGSTABSetup ( void *bicgstab_vdata , void *A , void *b , void *x );
int hypre_BiCGSTABSolve ( void *bicgstab_vdata , void *A , void *b , void *x );
int hypre_BiCGSTABSetTol ( void *bicgstab_vdata , double tol );
int hypre_BiCGSTABSetConvergenceFactorTol ( void *bicgstab_vdata , double cf_tol );
int hypre_BiCGSTABSetMinIter ( void *bicgstab_vdata , int min_iter );
int hypre_BiCGSTABSetMaxIter ( void *bicgstab_vdata , int max_iter );
int hypre_BiCGSTABSetStopCrit ( void *bicgstab_vdata , int stop_crit );
int hypre_BiCGSTABSetPrecond ( void *bicgstab_vdata , int (*precond )(), int (*precond_setup )(), void *precond_data );
int hypre_BiCGSTABGetPrecond ( void *bicgstab_vdata , HYPRE_Solver *precond_data_ptr );
int hypre_BiCGSTABSetLogging ( void *bicgstab_vdata , int logging );
int hypre_BiCGSTABSetPrintLevel ( void *bicgstab_vdata , int print_level );
int hypre_BiCGSTABGetConverged ( void *bicgstab_vdata , int *converged );
int hypre_BiCGSTABGetNumIterations ( void *bicgstab_vdata , int *num_iterations );
int hypre_BiCGSTABGetFinalRelativeResidualNorm ( void *bicgstab_vdata , double *relative_residual_norm );
int hypre_BiCGSTABGetResidual ( void *bicgstab_vdata , void **residual );

/* cgnr.c */
hypre_CGNRFunctions *hypre_CGNRFunctionsCreate ( int (*CommInfo )(void *A ,int *my_id ,int *num_procs ), void *(*CreateVector )(void *vector ), int (*DestroyVector )(void *vector ), void *(*MatvecCreate )(void *A ,void *x ), int (*Matvec )(void *matvec_data ,double alpha ,void *A ,void *x ,double beta ,void *y ), int (*MatvecT )(void *matvec_data ,double alpha ,void *A ,void *x ,double beta ,void *y ), int (*MatvecDestroy )(void *matvec_data ), double (*InnerProd )(void *x ,void *y ), int (*CopyVector )(void *x ,void *y ), int (*ClearVector )(void *x ), int (*ScaleVector )(double alpha ,void *x ), int (*Axpy )(double alpha ,void *x ,void *y ), int (*PrecondSetup )(void *vdata ,void *A ,void *b ,void *x ), int (*Precond )(void *vdata ,void *A ,void *b ,void *x ), int (*PrecondT )(void *vdata ,void *A ,void *b ,void *x ));
void *hypre_CGNRCreate ( hypre_CGNRFunctions *cgnr_functions );
int hypre_CGNRDestroy ( void *cgnr_vdata );
int hypre_CGNRSetup ( void *cgnr_vdata , void *A , void *b , void *x );
int hypre_CGNRSolve ( void *cgnr_vdata , void *A , void *b , void *x );
int hypre_CGNRSetTol ( void *cgnr_vdata , double tol );
int hypre_CGNRSetMinIter ( void *cgnr_vdata , int min_iter );
int hypre_CGNRSetMaxIter ( void *cgnr_vdata , int max_iter );
int hypre_CGNRSetStopCrit ( void *cgnr_vdata , int stop_crit );
int hypre_CGNRSetPrecond ( void *cgnr_vdata , int (*precond )(), int (*precondT )(), int (*precond_setup )(), void *precond_data );
int hypre_CGNRGetPrecond ( void *cgnr_vdata , HYPRE_Solver *precond_data_ptr );
int hypre_CGNRSetLogging ( void *cgnr_vdata , int logging );
int hypre_CGNRGetNumIterations ( void *cgnr_vdata , int *num_iterations );
int hypre_CGNRGetFinalRelativeResidualNorm ( void *cgnr_vdata , double *relative_residual_norm );

/* gmres.c */
hypre_GMRESFunctions *hypre_GMRESFunctionsCreate ( char *(*CAlloc )(int count ,int elt_size ), int (*Free )(char *ptr ), int (*CommInfo )(void *A ,int *my_id ,int *num_procs ), void *(*CreateVector )(void *vector ), void *(*CreateVectorArray )(int size ,void *vectors ), int (*DestroyVector )(void *vector ), void *(*MatvecCreate )(void *A ,void *x ), int (*Matvec )(void *matvec_data ,double alpha ,void *A ,void *x ,double beta ,void *y ), int (*MatvecDestroy )(void *matvec_data ), double (*InnerProd )(void *x ,void *y ), int (*CopyVector )(void *x ,void *y ), int (*ClearVector )(void *x ), int (*ScaleVector )(double alpha ,void *x ), int (*Axpy )(double alpha ,void *x ,void *y ), int (*PrecondSetup )(void *vdata ,void *A ,void *b ,void *x ), int (*Precond )(void *vdata ,void *A ,void *b ,void *x ));
void *hypre_GMRESCreate ( hypre_GMRESFunctions *gmres_functions );
int hypre_GMRESDestroy ( void *gmres_vdata );
int hypre_GMRESGetResidual ( void *gmres_vdata , void **residual );
int hypre_GMRESSetup ( void *gmres_vdata , void *A , void *b , void *x );
int hypre_GMRESSolve ( void *gmres_vdata , void *A , void *b , void *x );
int hypre_GMRESSetKDim ( void *gmres_vdata , int k_dim );
int hypre_GMRESGetKDim ( void *gmres_vdata , int *k_dim );
int hypre_GMRESSetTol ( void *gmres_vdata , double tol );
int hypre_GMRESGetTol ( void *gmres_vdata , double *tol );
int hypre_GMRESSetConvergenceFactorTol ( void *gmres_vdata , double cf_tol );
int hypre_GMRESGetConvergenceFactorTol ( void *gmres_vdata , double *cf_tol );
int hypre_GMRESSetMinIter ( void *gmres_vdata , int min_iter );
int hypre_GMRESGetMinIter ( void *gmres_vdata , int *min_iter );
int hypre_GMRESSetMaxIter ( void *gmres_vdata , int max_iter );
int hypre_GMRESGetMaxIter ( void *gmres_vdata , int *max_iter );
int hypre_GMRESSetRelChange ( void *gmres_vdata , int rel_change );
int hypre_GMRESGetRelChange ( void *gmres_vdata , int *rel_change );
int hypre_GMRESSetStopCrit ( void *gmres_vdata , int stop_crit );
int hypre_GMRESGetStopCrit ( void *gmres_vdata , int *stop_crit );
int hypre_GMRESSetPrecond ( void *gmres_vdata , int (*precond )(), int (*precond_setup )(), void *precond_data );
int hypre_GMRESGetPrecond ( void *gmres_vdata , HYPRE_Solver *precond_data_ptr );
int hypre_GMRESSetPrintLevel ( void *gmres_vdata , int level );
int hypre_GMRESGetPrintLevel ( void *gmres_vdata , int *level );
int hypre_GMRESSetLogging ( void *gmres_vdata , int level );
int hypre_GMRESGetLogging ( void *gmres_vdata , int *level );
int hypre_GMRESGetNumIterations ( void *gmres_vdata , int *num_iterations );
int hypre_GMRESGetConverged ( void *gmres_vdata , int *converged );
int hypre_GMRESGetFinalRelativeResidualNorm ( void *gmres_vdata , double *relative_residual_norm );

/* HYPRE_bicgstab.c */
int HYPRE_BiCGSTABDestroy ( HYPRE_Solver solver );
int HYPRE_BiCGSTABSetup ( HYPRE_Solver solver , HYPRE_Matrix A , HYPRE_Vector b , HYPRE_Vector x );
int HYPRE_BiCGSTABSolve ( HYPRE_Solver solver , HYPRE_Matrix A , HYPRE_Vector b , HYPRE_Vector x );
int HYPRE_BiCGSTABSetTol ( HYPRE_Solver solver , double tol );
int HYPRE_BiCGSTABSetConvergenceFactorTol ( HYPRE_Solver solver , double cf_tol );
int HYPRE_BiCGSTABSetMinIter ( HYPRE_Solver solver , int min_iter );
int HYPRE_BiCGSTABSetMaxIter ( HYPRE_Solver solver , int max_iter );
int HYPRE_BiCGSTABSetStopCrit ( HYPRE_Solver solver , int stop_crit );
int HYPRE_BiCGSTABSetPrecond ( HYPRE_Solver solver , HYPRE_PtrToSolverFcn precond , HYPRE_PtrToSolverFcn precond_setup , HYPRE_Solver precond_solver );
int HYPRE_BiCGSTABGetPrecond ( HYPRE_Solver solver , HYPRE_Solver *precond_data_ptr );
int HYPRE_BiCGSTABSetLogging ( HYPRE_Solver solver , int logging );
int HYPRE_BiCGSTABSetPrintLevel ( HYPRE_Solver solver , int print_level );
int HYPRE_BiCGSTABGetNumIterations ( HYPRE_Solver solver , int *num_iterations );
int HYPRE_BiCGSTABGetFinalRelativeResidualNorm ( HYPRE_Solver solver , double *norm );
int HYPRE_BiCGSTABGetResidual ( HYPRE_Solver solver , void **residual );

/* HYPRE_cgnr.c */
int HYPRE_CGNRDestroy ( HYPRE_Solver solver );
int HYPRE_CGNRSetup ( HYPRE_Solver solver , HYPRE_Matrix A , HYPRE_Vector b , HYPRE_Vector x );
int HYPRE_CGNRSolve ( HYPRE_Solver solver , HYPRE_Matrix A , HYPRE_Vector b , HYPRE_Vector x );
int HYPRE_CGNRSetTol ( HYPRE_Solver solver , double tol );
int HYPRE_CGNRSetMinIter ( HYPRE_Solver solver , int min_iter );
int HYPRE_CGNRSetMaxIter ( HYPRE_Solver solver , int max_iter );
int HYPRE_CGNRSetStopCrit ( HYPRE_Solver solver , int stop_crit );
int HYPRE_CGNRSetPrecond ( HYPRE_Solver solver , HYPRE_PtrToSolverFcn precond , HYPRE_PtrToSolverFcn precondT , HYPRE_PtrToSolverFcn precond_setup , HYPRE_Solver precond_solver );
int HYPRE_CGNRGetPrecond ( HYPRE_Solver solver , HYPRE_Solver *precond_data_ptr );
int HYPRE_CGNRSetLogging ( HYPRE_Solver solver , int logging );
int HYPRE_CGNRGetNumIterations ( HYPRE_Solver solver , int *num_iterations );
int HYPRE_CGNRGetFinalRelativeResidualNorm ( HYPRE_Solver solver , double *norm );

/* HYPRE_gmres.c */
int HYPRE_GMRESSetup ( HYPRE_Solver solver , HYPRE_Matrix A , HYPRE_Vector b , HYPRE_Vector x );
int HYPRE_GMRESSolve ( HYPRE_Solver solver , HYPRE_Matrix A , HYPRE_Vector b , HYPRE_Vector x );
int HYPRE_GMRESSetKDim ( HYPRE_Solver solver , int k_dim );
int HYPRE_GMRESGetKDim ( HYPRE_Solver solver , int *k_dim );
int HYPRE_GMRESSetTol ( HYPRE_Solver solver , double tol );
int HYPRE_GMRESGetTol ( HYPRE_Solver solver , double *tol );
int HYPRE_GMRESSetConvergenceFactorTol ( HYPRE_Solver solver , double cf_tol );
int HYPRE_GMRESGetConvergenceFactorTol ( HYPRE_Solver solver , double *cf_tol );
int HYPRE_GMRESSetMinIter ( HYPRE_Solver solver , int min_iter );
int HYPRE_GMRESGetMinIter ( HYPRE_Solver solver , int *min_iter );
int HYPRE_GMRESSetMaxIter ( HYPRE_Solver solver , int max_iter );
int HYPRE_GMRESGetMaxIter ( HYPRE_Solver solver , int *max_iter );
int HYPRE_GMRESSetStopCrit ( HYPRE_Solver solver , int stop_crit );
int HYPRE_GMRESGetStopCrit ( HYPRE_Solver solver , int *stop_crit );
int HYPRE_GMRESSetRelChange ( HYPRE_Solver solver , int rel_change );
int HYPRE_GMRESGetRelChange ( HYPRE_Solver solver , int *rel_change );
int HYPRE_GMRESSetPrecond ( HYPRE_Solver solver , HYPRE_PtrToSolverFcn precond , HYPRE_PtrToSolverFcn precond_setup , HYPRE_Solver precond_solver );
int HYPRE_GMRESGetPrecond ( HYPRE_Solver solver , HYPRE_Solver *precond_data_ptr );
int HYPRE_GMRESSetPrintLevel ( HYPRE_Solver solver , int level );
int HYPRE_GMRESGetPrintLevel ( HYPRE_Solver solver , int *level );
int HYPRE_GMRESSetLogging ( HYPRE_Solver solver , int level );
int HYPRE_GMRESGetLogging ( HYPRE_Solver solver , int *level );
int HYPRE_GMRESGetNumIterations ( HYPRE_Solver solver , int *num_iterations );
int HYPRE_GMRESGetConverged ( HYPRE_Solver solver , int *converged );
int HYPRE_GMRESGetFinalRelativeResidualNorm ( HYPRE_Solver solver , double *norm );
int HYPRE_GMRESGetResidual ( HYPRE_Solver solver , void **residual );

/* HYPRE_pcg.c */
int HYPRE_PCGSetup ( HYPRE_Solver solver , HYPRE_Matrix A , HYPRE_Vector b , HYPRE_Vector x );
int HYPRE_PCGSolve ( HYPRE_Solver solver , HYPRE_Matrix A , HYPRE_Vector b , HYPRE_Vector x );
int HYPRE_PCGSetTol ( HYPRE_Solver solver , double tol );
int HYPRE_PCGGetTol ( HYPRE_Solver solver , double *tol );
int HYPRE_PCGSetAbsoluteTolFactor ( HYPRE_Solver solver , double abstolf );
int HYPRE_PCGGetAbsoluteTolFactor ( HYPRE_Solver solver , double *abstolf );
int HYPRE_PCGSetConvergenceFactorTol ( HYPRE_Solver solver , double cf_tol );
int HYPRE_PCGGetConvergenceFactorTol ( HYPRE_Solver solver , double *cf_tol );
int HYPRE_PCGSetMaxIter ( HYPRE_Solver solver , int max_iter );
int HYPRE_PCGGetMaxIter ( HYPRE_Solver solver , int *max_iter );
int HYPRE_PCGSetStopCrit ( HYPRE_Solver solver , int stop_crit );
int HYPRE_PCGGetStopCrit ( HYPRE_Solver solver , int *stop_crit );
int HYPRE_PCGSetTwoNorm ( HYPRE_Solver solver , int two_norm );
int HYPRE_PCGGetTwoNorm ( HYPRE_Solver solver , int *two_norm );
int HYPRE_PCGSetRelChange ( HYPRE_Solver solver , int rel_change );
int HYPRE_PCGGetRelChange ( HYPRE_Solver solver , int *rel_change );
int HYPRE_PCGSetPrecond ( HYPRE_Solver solver , HYPRE_PtrToSolverFcn precond , HYPRE_PtrToSolverFcn precond_setup , HYPRE_Solver precond_solver );
int HYPRE_PCGGetPrecond ( HYPRE_Solver solver , HYPRE_Solver *precond_data_ptr );
int HYPRE_PCGSetLogging ( HYPRE_Solver solver , int level );
int HYPRE_PCGGetLogging ( HYPRE_Solver solver , int *level );
int HYPRE_PCGSetPrintLevel ( HYPRE_Solver solver , int level );
int HYPRE_PCGGetPrintLevel ( HYPRE_Solver solver , int *level );
int HYPRE_PCGGetNumIterations ( HYPRE_Solver solver , int *num_iterations );
int HYPRE_PCGGetConverged ( HYPRE_Solver solver , int *converged );
int HYPRE_PCGGetFinalRelativeResidualNorm ( HYPRE_Solver solver , double *norm );
int HYPRE_PCGGetResidual ( HYPRE_Solver solver , void **residual );

/* pcg.c */
hypre_PCGFunctions *hypre_PCGFunctionsCreate ( char *(*CAlloc )(int count ,int elt_size ), int (*Free )(char *ptr ), int (*CommInfo )(void *A ,int *my_id ,int *num_procs ), void *(*CreateVector )(void *vector ), int (*DestroyVector )(void *vector ), void *(*MatvecCreate )(void *A ,void *x ), int (*Matvec )(void *matvec_data ,double alpha ,void *A ,void *x ,double beta ,void *y ), int (*MatvecDestroy )(void *matvec_data ), double (*InnerProd )(void *x ,void *y ), int (*CopyVector )(void *x ,void *y ), int (*ClearVector )(void *x ), int (*ScaleVector )(double alpha ,void *x ), int (*Axpy )(double alpha ,void *x ,void *y ), int (*PrecondSetup )(void *vdata ,void *A ,void *b ,void *x ), int (*Precond )(void *vdata ,void *A ,void *b ,void *x ));
void *hypre_PCGCreate ( hypre_PCGFunctions *pcg_functions );
int hypre_PCGDestroy ( void *pcg_vdata );
int hypre_PCGGetResidual ( void *pcg_vdata , void **residual );
int hypre_PCGSetup ( void *pcg_vdata , void *A , void *b , void *x );
int hypre_PCGSolve ( void *pcg_vdata , void *A , void *b , void *x );
int hypre_PCGSetTol ( void *pcg_vdata , double tol );
int hypre_PCGGetTol ( void *pcg_vdata , double *tol );
int hypre_PCGSetAbsoluteTolFactor ( void *pcg_vdata , double atolf );
int hypre_PCGGetAbsoluteTolFactor ( void *pcg_vdata , double *atolf );
int hypre_PCGSetConvergenceFactorTol ( void *pcg_vdata , double cf_tol );
int hypre_PCGGetConvergenceFactorTol ( void *pcg_vdata , double *cf_tol );
int hypre_PCGSetMaxIter ( void *pcg_vdata , int max_iter );
int hypre_PCGGetMaxIter ( void *pcg_vdata , int *max_iter );
int hypre_PCGSetTwoNorm ( void *pcg_vdata , int two_norm );
int hypre_PCGGetTwoNorm ( void *pcg_vdata , int *two_norm );
int hypre_PCGSetRelChange ( void *pcg_vdata , int rel_change );
int hypre_PCGGetRelChange ( void *pcg_vdata , int *rel_change );
int hypre_PCGSetStopCrit ( void *pcg_vdata , int stop_crit );
int hypre_PCGGetStopCrit ( void *pcg_vdata , int *stop_crit );
int hypre_PCGGetPrecond ( void *pcg_vdata , HYPRE_Solver *precond_data_ptr );
int hypre_PCGSetPrecond ( void *pcg_vdata , int (*precond )(), int (*precond_setup )(), void *precond_data );
int hypre_PCGSetPrintLevel ( void *pcg_vdata , int level );
int hypre_PCGGetPrintLevel ( void *pcg_vdata , int *level );
int hypre_PCGSetLogging ( void *pcg_vdata , int level );
int hypre_PCGGetLogging ( void *pcg_vdata , int *level );
int hypre_PCGGetNumIterations ( void *pcg_vdata , int *num_iterations );
int hypre_PCGGetConverged ( void *pcg_vdata , int *converged );
int hypre_PCGPrintLogging ( void *pcg_vdata , int myid );
int hypre_PCGGetFinalRelativeResidualNorm ( void *pcg_vdata , double *relative_residual_norm );

#ifdef __cplusplus
}
#endif

#endif

