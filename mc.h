#pragma once
#ifndef MC_H
#define MC_H

/*
 * Performance optimizations applied: 2026-01-07
 * Author: Dániel Németh
 * 
 * Changes:
 * - Added caching of member variables in tuneV() and tuneA()
 * - Precomputed inverse window value to replace division
 * - Used std::abs() for better performance
 * - Reduced redundant member variable access
 */

#include <random>
#include <cmath>
#include "ball.h"

void Ball::tuneV() {
    // Cache window and V to avoid repeated member access
    const int cachedWindow = window;
    const int cachedV = V;
    const double cachedMeanV = meanV;
    
    // Calculate the difference between the current number of cubes and the desired volume
    const double invWindow = 1.0 / static_cast<double>(cachedWindow);
    const double diff = static_cast<double>(cachedV) - cachedMeanV * invWindow;

	//printf("diff: %g V: %d meanV: %g \n",diff, V, meanV/window);
    // Decide the change in lambda based on the magnitude of the difference
    const double absDiff = std::abs(diff);
    double adjustment = 0;
    if (absDiff > 100) adjustment = 0.2; // Large adjustment
    else if (absDiff > 40) adjustment = 0.02; // Moderate adjustment
    else if (absDiff > 10) adjustment = 0.002; // Moderate adjustment
     

    // Adjust lambda based on whether we have too many or too few cubes
    if (diff > 0) lambda -= adjustment; // Decrease Lambda if there are too many cubes 
    else          lambda += adjustment; // Increase Lambda if there are too few cubes
    

    // Optionally print the new value of Lambda to track changes
//    printf("Adjusted Lambda to: %f\n", Lambda);
}

void Ball::tuneA() {
    // Cache nextFaceBId and A to avoid repeated member access
    const int cachedNextFaceBId = nextFaceBId;
    const int cachedA = A;
    
    // Calculate the difference between the current number of cubes and the desired volume
    const int diff = cachedA - cachedNextFaceBId;

    // Decide the change in Lambda based on the magnitude of the difference
    const int absDiff = std::abs(diff);
    double adjustment = 0;
    if (absDiff > 100) adjustment = 1.0; // Large adjustment 
    else if (absDiff > 10) adjustment = 0.1; // Moderate adjustment
    else adjustment = 0.01; // Fine adjustment
    

    // Adjust Lambda based on whether we have too many or too few cubes
    if (diff > 0) alpha -= adjustment; // Decrease Lambda if there are too many cubes
    else if (diff < 0) alpha += adjustment; // Increase Lambda if there are too few cubes
    

    // Optionally print the new value of Lambda to track changes
//    printf("Adjusted Lambda to: %f\n", Lambda);
}



//var = rand() % 10;



	
#endif
