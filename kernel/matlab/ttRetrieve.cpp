/*
 * Copyright (c) 2009 Lund University
 *
 * Written by Anton Cervin, Dan Henriksson and Martin Ohlin,
 * Department of Automatic Control LTH, Lund University, Sweden.
 *   
 * This file is part of Truetime 2.0 beta.
 *
 * Truetime 2.0 beta is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Truetime 2.0 beta is distributed in the hope that it will be useful, but
 * without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Truetime 2.0 beta. If not, see <http://www.gnu.org/licenses/>
 */

#define KERNEL_MATLAB
#include "../ttkernel.h" 



#include "../retrieve.cpp"
#include "getrtsys.cpp"

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[] )
{
  rtsys = getrtsys() ; // Get pointer to rtsys 

  if (rtsys==NULL) {
    return;
  }

  // Check number and type of arguments. 
  if (nrhs != 1) {
    TT_MEX_ERROR("ttRetrieve: Wrong number of input arguments!\nUsage: ttRetrieve(mailboxname)");
    return;
  }

  if (mxIsChar(prhs[0]) != 1) {
    TT_MEX_ERROR("ttRetrieve: mailboxname must be a string");
    return;
  }
  
  char mailboxname[MAXCHARS];
  mxGetString(prhs[0], mailboxname, MAXCHARS);
  
  mxArray* data;
  mxArray* msg = (mxArray *) ttRetrieve(mailboxname);
  if ( msg!=NULL ){
    data = mxDuplicateArray((mxArray *) msg);
    plhs[0] = data;
    // Delete message
    mxDestroyArray((mxArray *) msg); 
  } else {
    // A message was already fetched but not retrieved!
    TT_MEX_ERROR("ttRetrieve: No message to retrieve!\n");
    plhs[0] = mxCreateDoubleMatrix(0,0,mxREAL); // Return empty matrix (error)
  }

}
