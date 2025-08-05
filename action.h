#pragma once

#ifndef ACTION_h
#define ACTION_h

#include "globals.h"


double getActionDiff(int delta_Nb, int Nb) { return epsilon*2*delta_Nb*(A - Nb) - epsilon*delta_Nb*delta_Nb - Lambda; }

double getActionA() {return epsilon*(A-(nextFaceBId-1))*(A-(nextFaceBId-1))  + Lambda * (nextCubeId-1);}
double getActionB(int delta_Nb) {return epsilon*(A+Nb-(nextFaceBId-1))*(A+Nb-(nextFaceBId-1))  + Lambda * (nextCubeId);}


#endif
