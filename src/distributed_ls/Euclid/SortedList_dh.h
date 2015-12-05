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



#ifndef SORTEDLIST_DH_H
#define SORTEDLIST_DH_H

/* for private use by mpi factorization algorithms */

#include "euclid_common.h"

typedef struct _srecord {
    int    col;
    int    level;
    double val;
    int next;
} SRecord;


extern void SortedList_dhCreate(SortedList_dh *sList);
extern void SortedList_dhDestroy(SortedList_dh sList);
extern void SortedList_dhInit(SortedList_dh sList, SubdomainGraph_dh sg);
extern void SortedList_dhEnforceConstraint(SortedList_dh sList, SubdomainGraph_dh sg);

extern void SortedList_dhReset(SortedList_dh sList, int row);

extern int SortedList_dhReadCount(SortedList_dh sList);
  /* returns number of records inserted since last reset */

extern void SortedList_dhResetGetSmallest(SortedList_dh sList);
  /* resets index used for SortedList_dhGetSmallestLowerTri().
   */

extern SRecord * SortedList_dhGetSmallest(SortedList_dh sList);
  /* returns record with smallest column value that hasn't been
     retrieved via this method since last call to SortedList_dhReset()
     or SortedList_dhResetGetSmallest().
     If all records have been retrieved, returns NULL.
   */

extern SRecord * SortedList_dhGetSmallestLowerTri(SortedList_dh sList);
  /* returns record with smallest column value that hasn't been
     retrieved via this method since last call to reset.  
     Only returns records where SRecord sr.col < row (per Init).
     If all records have been retrieved, returns NULL.
   */

extern void SortedList_dhInsert(SortedList_dh sList, SRecord *sr);
  /* unilateral insert (does not check to see if item is already
     in list); does not permute sr->col; used in numeric
     factorization routines.
   */

extern void SortedList_dhInsertOrUpdateVal(SortedList_dh sList, SRecord *sr);
  /* unilateral insert: does not check to see if already
     inserted; does not permute sr->col; used in numeric 
     factorization routines.
   */

extern bool SortedList_dhPermuteAndInsert(SortedList_dh sList, SRecord *sr, double thresh);
  /* permutes sr->col, and inserts record in sorted list.
     Note: the contents of the passed variable "sr" may be changed.
     Note: this performs sparsification 
  */


extern void SortedList_dhInsertOrUpdate(SortedList_dh sList, SRecord *sr);
  /* if a record with identical sr->col was inserted, updates sr->level
     to smallest of the two values; otherwise, inserts the record.
     Unlike SortedList_dhPermuteAndInsert, does not permute sr->col.
     Note: the contents of the passed variable "sr" may be changed.
     Warning: do not call SortedList_dhGetSmallestLowerTri() again
     until reset is called.
  */

extern SRecord * SortedList_dhFind(SortedList_dh sList, SRecord *sr);
  /* returns NULL if no record is found containing sr->col 
   */

extern void SortedList_dhUpdateVal(SortedList_dh sList, SRecord *sr);

#endif