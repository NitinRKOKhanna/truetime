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

#ifndef GET_MSG
#define GET_MSG

#include "getnetwork.cpp"

// do the dirty work: poke around inside nwsys of network block 
NWmsg *nwGetMsg(NetworkInterface* nwi) {
  NWmsg *m;
  if ((m = (NWmsg *)nwi->nwsys->nwnodes[nwi->nodeNbr]->postprocQ->getFirst()) != NULL) {
    nwi->nwsys->nwnodes[nwi->nodeNbr]->postprocQ->removeNode(m);
  }
  return m;
}

void *ttGetMsg(int networkNbr, double* signalPower)
{
  void *data;
  NetworkInterface* nwi = getNetwork(networkNbr);

  if (nwi == NULL) {
    // is there no network?
    char buf[200];
    sprintf(buf, "ttGetMsg: Network #%d not present!", networkNbr);
    TT_MEX_ERROR(buf);
    return NULL;
  }

  NWmsg *nwmsg = nwGetMsg(nwi); 
  if (nwmsg == NULL) {
    *signalPower = 0;
    return NULL;
  }
  data = nwmsg->data;
  *signalPower = nwmsg->signalPower;

  // Delete message 
  delete nwmsg;

  return data;
}

void *ttGetMsg(int networkNbr) // returns data (void *) or NULL if no message
{
  void *data;
  NetworkInterface* nwi = getNetwork(networkNbr);

  if (nwi == NULL) {
    // is there no network?
    char buf[200];
    sprintf(buf, "ttGetMsg: Network #%d not present!", networkNbr);
    TT_MEX_ERROR(buf);
    return NULL;
  }

  NWmsg *nwmsg = nwGetMsg(nwi); 
  if (nwmsg == NULL) {
    return NULL;
  }
  data = nwmsg->data;
  // Delete message 
  delete nwmsg;

  return data;
}

void *ttGetMsg() {
  return ttGetMsg(1);
}

// For use from Matlab mex function
mxArray *ttGetMsgMATLAB(int networkNbr, double* signalPower)
{
  NetworkInterface* nwi = getNetwork(networkNbr);
  
  if (nwi == NULL) {
    // is there no network?
    char buf[200];
    sprintf(buf, "ttGetMsg: Network #%d not present!", networkNbr);
    TT_MEX_ERROR(buf);
    return NULL;
  }

  NWmsg *nwmsg = nwGetMsg(nwi);
  if (nwmsg == NULL) {
    *signalPower = 0;
    return NULL;
  }
  *signalPower = nwmsg->signalPower;
  mxArray *data = mxDuplicateArray(nwmsg->dataMATLAB);

  // Delete message
  mxDestroyArray(nwmsg->dataMATLAB);
  delete nwmsg;

  return data;
}


#endif
