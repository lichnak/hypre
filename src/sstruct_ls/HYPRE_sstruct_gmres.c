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
 * $Revision: 2.3 $
 ***********************************************************************EHEADER*/




/******************************************************************************
 *
 * HYPRE_SStructGMRES interface
 *
 *****************************************************************************/

#include "headers.h"

/*--------------------------------------------------------------------------
 * HYPRE_SStructGMRESCreate
 *--------------------------------------------------------------------------*/

int
HYPRE_SStructGMRESCreate( MPI_Comm             comm,
                          HYPRE_SStructSolver *solver )
{
   hypre_GMRESFunctions * gmres_functions =
      hypre_GMRESFunctionsCreate(
         hypre_CAlloc, hypre_SStructKrylovFree, hypre_SStructKrylovCommInfo,
         hypre_SStructKrylovCreateVector,
         hypre_SStructKrylovCreateVectorArray,
         hypre_SStructKrylovDestroyVector, hypre_SStructKrylovMatvecCreate,
         hypre_SStructKrylovMatvec, hypre_SStructKrylovMatvecDestroy,
         hypre_SStructKrylovInnerProd, hypre_SStructKrylovCopyVector,
         hypre_SStructKrylovClearVector,
         hypre_SStructKrylovScaleVector, hypre_SStructKrylovAxpy,
         hypre_SStructKrylovIdentitySetup, hypre_SStructKrylovIdentity );

   *solver = ( (HYPRE_SStructSolver) hypre_GMRESCreate( gmres_functions ) );

   return 0;
}

/*--------------------------------------------------------------------------
 * HYPRE_SStructGMRESDestroy
 *--------------------------------------------------------------------------*/

int 
HYPRE_SStructGMRESDestroy( HYPRE_SStructSolver solver )
{
   return( hypre_GMRESDestroy( (void *) solver ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_SStructGMRESSetup
 *--------------------------------------------------------------------------*/

int 
HYPRE_SStructGMRESSetup( HYPRE_SStructSolver solver,
                         HYPRE_SStructMatrix A,
                         HYPRE_SStructVector b,
                         HYPRE_SStructVector x )
{
   return( HYPRE_GMRESSetup( (HYPRE_Solver) solver,
                             (HYPRE_Matrix) A,
                             (HYPRE_Vector) b,
                             (HYPRE_Vector) x ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_SStructGMRESSolve
 *--------------------------------------------------------------------------*/

int 
HYPRE_SStructGMRESSolve( HYPRE_SStructSolver solver,
                         HYPRE_SStructMatrix A,
                         HYPRE_SStructVector b,
                         HYPRE_SStructVector x )
{
   return( HYPRE_GMRESSolve( (HYPRE_Solver) solver,
                             (HYPRE_Matrix) A,
                             (HYPRE_Vector) b,
                             (HYPRE_Vector) x ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_SStructGMRESSetKDim
 *--------------------------------------------------------------------------*/

int
HYPRE_SStructGMRESSetKDim( HYPRE_SStructSolver solver,
                           int                 k_dim )
{
   return( HYPRE_GMRESSetKDim( (HYPRE_Solver) solver, k_dim ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_SStructGMRESSetTol
 *--------------------------------------------------------------------------*/

int
HYPRE_SStructGMRESSetTol( HYPRE_SStructSolver solver,
                          double              tol )
{
   return( HYPRE_GMRESSetTol( (HYPRE_Solver) solver, tol ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_SStructGMRESSetMinIter
 *--------------------------------------------------------------------------*/

int
HYPRE_SStructGMRESSetMinIter( HYPRE_SStructSolver solver,
                              int                 min_iter )
{
   return( HYPRE_GMRESSetMinIter( (HYPRE_Solver) solver, min_iter ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_SStructGMRESSetMaxIter
 *--------------------------------------------------------------------------*/

int
HYPRE_SStructGMRESSetMaxIter( HYPRE_SStructSolver solver,
                              int                 max_iter )
{
   return( HYPRE_GMRESSetMaxIter( (HYPRE_Solver) solver, max_iter ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_SStructGMRESSetStopCrit
 *--------------------------------------------------------------------------*/

int
HYPRE_SStructGMRESSetStopCrit( HYPRE_SStructSolver solver,
                               int                 stop_crit )
{
   return( HYPRE_GMRESSetStopCrit( (HYPRE_Solver) solver, stop_crit ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_SStructGMRESSetPrecond
 *--------------------------------------------------------------------------*/

int
HYPRE_SStructGMRESSetPrecond( HYPRE_SStructSolver          solver,
                              HYPRE_PtrToSStructSolverFcn  precond,
                              HYPRE_PtrToSStructSolverFcn  precond_setup,
                              void *          precond_data )
{
   return( HYPRE_GMRESSetPrecond( (HYPRE_Solver) solver,
                                  (HYPRE_PtrToSolverFcn) precond,
                                  (HYPRE_PtrToSolverFcn) precond_setup,
                                  (HYPRE_Solver) precond_data ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_SStructGMRESSetLogging
 *--------------------------------------------------------------------------*/

int
HYPRE_SStructGMRESSetLogging( HYPRE_SStructSolver solver,
                              int                 logging )
{
   return( HYPRE_GMRESSetLogging( (HYPRE_Solver) solver, logging ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_SStructGMRESSetPrintLevel
 *--------------------------------------------------------------------------*/

int
HYPRE_SStructGMRESSetPrintLevel( HYPRE_SStructSolver solver,
                              int                 level )
{
   return( HYPRE_GMRESSetPrintLevel( (HYPRE_Solver) solver, level ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_SStructGMRESGetNumIterations
 *--------------------------------------------------------------------------*/

int
HYPRE_SStructGMRESGetNumIterations( HYPRE_SStructSolver  solver,
                                    int                 *num_iterations )
{
   return( HYPRE_GMRESGetNumIterations( (HYPRE_Solver) solver, num_iterations ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_SStructGMRESGetFinalRelativeResidualNorm
 *--------------------------------------------------------------------------*/

int
HYPRE_SStructGMRESGetFinalRelativeResidualNorm( HYPRE_SStructSolver  solver,
                                                double              *norm )
{
   return( HYPRE_GMRESGetFinalRelativeResidualNorm( (HYPRE_Solver) solver, norm ) );
}

/*--------------------------------------------------------------------------
 * HYPRE_SStructGMRESGetResidual
 *--------------------------------------------------------------------------*/

int
HYPRE_SStructGMRESGetResidual( HYPRE_SStructSolver  solver,
                                void              **residual )
{
   return( HYPRE_GMRESGetResidual( (HYPRE_Solver) solver, residual ) );
}