#pragma once
#ifndef MC_H
#define MC_H

#include <random>
#include <ball.h>

void Ball::tuneV() {
    // Calculate the difference between the current number of cubes and the desired volume
    double diff = V - meanV/window;

	//printf("diff: %g V: %d meanV: %g \n",diff, V, meanV/window);
    // Decide the change in lambda based on the magnitude of the difference
    double adjustment = 0;
    if (abs(diff) > 100) adjustment = 0.2; // Large adjustment
    else if (abs(diff) > 40) adjustment = 0.02; // Moderate adjustment
    else if (abs(diff) > 10) adjustment = 0.002; // Moderate adjustment
     

    // Adjust lambda based on whether we have too many or too few cubes
    if (diff > 0) lambda -= adjustment; // Decrease Lambda if there are too many cubes 
    else          lambda += adjustment; // Increase Lambda if there are too few cubes
    

    // Optionally print the new value of Lambda to track changes
//    printf("Adjusted Lambda to: %f\n", Lambda);
}

void Ball::tuneA() {
    // Calculate the difference between the current number of cubes and the desired volume
    int diff = A-nextFaceBId;

    // Decide the change in Lambda based on the magnitude of the difference
    double adjustment = 0;
    if (abs(diff) > 100) adjustment = 1.0; // Large adjustment 
    else if (abs(diff) > 10) adjustment = 0.1; // Moderate adjustment
    else adjustment = 0.01; // Fine adjustment
    

    // Adjust Lambda based on whether we have too many or too few cubes
    if (diff > 0) alpha -= adjustment; // Decrease Lambda if there are too many cubes
    else if (diff < 0) alpha += adjustment; // Increase Lambda if there are too few cubes
    

    // Optionally print the new value of Lambda to track changes
//    printf("Adjusted Lambda to: %f\n", Lambda);
}



//var = rand() % 10;



	
#endif
