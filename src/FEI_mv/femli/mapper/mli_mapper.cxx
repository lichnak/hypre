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
 * $Revision: 1.4 $
 ***********************************************************************EHEADER*/




#include <string.h>
#include <assert.h>
#include "utilities/_hypre_utilities.h"
#include "HYPRE.h"
#include "mapper/mli_mapper.h"
#include "util/mli_utils.h"

/***************************************************************************
 * constructor function for the MLI_Mapper 
 *--------------------------------------------------------------------------*/

MLI_Mapper::MLI_Mapper()
{
   nEntries  = 0;
   tokenList = NULL;
   tokenMap  = NULL;
}

/***************************************************************************
 * destructor function for the MLI_Mapper 
 *--------------------------------------------------------------------------*/

MLI_Mapper::~MLI_Mapper()
{
   if ( tokenList != NULL ) delete [] tokenList;
   if ( tokenMap  != NULL ) delete [] tokenMap;
}

/***************************************************************************
 * set map
 *--------------------------------------------------------------------------*/

int MLI_Mapper::setMap(int nItems, int *itemList, int *mapList)
{
   int i, *tokenAux;

   if ( nItems <= 0 ) return -1;
   nEntries  = nItems;
   tokenList = new int[nItems];
   for ( i = 0; i < nItems; i++ ) tokenList[i] = itemList[i];
   tokenAux = new int[nItems];
   for ( i = 0; i < nItems; i++ ) tokenAux[i] = i;
   MLI_Utils_IntQSort2( tokenList, tokenAux, 0, nItems-1 );
   tokenMap = new int[nItems];
   for ( i = 0; i < nItems; i++ ) tokenMap[i] = mapList[tokenAux[i]];
   delete [] tokenAux;
   return 0;
}

/***************************************************************************
 * adjust map offset (This is used for slide surface reduction)
 *--------------------------------------------------------------------------*/

int MLI_Mapper::adjustMapOffset(MPI_Comm comm, int *procNRows, 
                                int *procOffsets)
{
   int i, j, nprocs;

   if ( nEntries <= 0 ) return -1;
   MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
   for ( i = 0; i < nEntries; i++ ) 
   {
      for ( j = 0; j < nprocs; j++ ) 
         if ( tokenList[i] < procNRows[j] ) break;
      tokenMap[i] -= procOffsets[j-1];
   }
   return 0;
}

/***************************************************************************
 * get map
 *--------------------------------------------------------------------------*/

int MLI_Mapper::getMap(int nItems, int *itemList, int *mapList)
{
   int i, *itemTemp, *itemAux, counter;

   if ( nItems <= 0 ) return -1;
   itemTemp = new int[nItems];
   for ( i = 0; i < nItems; i++ ) itemTemp[i] = itemList[i];
   itemAux = new int[nItems];
   for ( i = 0; i < nItems; i++ ) itemAux[i] = i;
   MLI_Utils_IntQSort2( itemTemp, itemAux, 0, nItems-1 );

   counter = 0;
   for ( i = 0; i < nItems; i++ ) 
   {
      if ( itemTemp[i] == tokenList[counter] )
         mapList[itemAux[i]] = tokenMap[counter];
      else
      {
         counter++;
         while ( counter < nEntries )
         {
            if ( itemTemp[i] == tokenList[counter] )
            {
               mapList[itemAux[i]] = tokenMap[counter];
               break;
            }
            else counter++;
         }
      }
      if ( counter >= nEntries )
      {
         printf("MLI_Mapper::getMap - item not found %d.\n", itemList[i]);
         exit(1);
      } 
   }
   delete [] itemTemp;
   delete [] itemAux;
   return 0;
}

/***************************************************************************
 * setParams 
 *--------------------------------------------------------------------------*/

int MLI_Mapper::setParams(char *param_string, int argc, char **argv)
{
   int nItems, *itemList, *mapList;

   if ( !strcmp(param_string, "setMap") )
   {
      if ( argc == 3 )
      {
         nItems   = *(int *) argv[0];
         itemList = (int *) argv[1];
         mapList  = (int *) argv[2];
         setMap(nItems, itemList, mapList);
      }
      else
      {
         printf("MLI_Mapper::setParams : setMap requires 3 arguments.\n");
         exit(1);
      }
   }
   else
   {
      printf("MLI_Mapper::setParams : command not recognized %s.\n",
             param_string);
      return 1;
   }
   return 0;
}

