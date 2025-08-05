#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

#ifndef RNG_SEED
#define RNG_SEED 1 // Default seed
#endif


#define AbsMaxFacexId 100000
#define AbsMaxCubexId 100000


#include <numeric>

#include <iostream>
#include <cstdio>

#include "random.h"


int seed;
int A;
int V;

double epsilon;
double lambda ;
double alpha;

int steps ;
int thermal;
int sweeps ;

double meanV;
int window;

int startsize;
   

std::string name;

#include "config.h"
#include "ball.h"
//#include "action.h"
#include "initialize.h"

#include "helper.h"
#include "grow_cube.h"
#include "shrink_cube.h"

#include "boundary.h"

#include "objects.h"
#include "print.h"

#include "checks.h"

#include "measure.h"
#include "mc.h"


#endif
