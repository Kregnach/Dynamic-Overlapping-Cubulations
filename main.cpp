
#include "globals.h"

/*
 * Performance optimizations applied: 2026-01-07
 * Author: Dániel Németh
 * 
 * Changes:
 * - Cached steps/window calculation in thermal loop to avoid repeated division
 * - Optimized main simulation loop structure
 */

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config_filename>\n";
        return 1; // Return error if no config file is specified
    }

    std::string fname(argv[1]);
    
    printf("###### USING CONFIG FILE: %s\n",fname.c_str());

    ConfigReader cfr;
    cfr.read(fname);

    seed = cfr.getInt("seed");
    A = cfr.getInt("A");
    V = cfr.getInt("V");
    
    startsize = cfr.getInt("startsize");

    lambda = cfr.getDouble("lambda");
    alpha = cfr.getDouble("alpha");    
    epsilon = cfr.getDouble("epsilon");


    steps = cfr.getInt("steps");
    thermal = cfr.getInt("thermal");
    sweeps = cfr.getInt("sweeps");

    name = cfr.getString("name");
    
    
    printf("seed: %d\n",seed);
    printf("A: %d\n",A);
    printf("V: %d\n",V);
    printf("startSize: %d\n",startsize);
    printf("epsilon: %g\n",epsilon);
    printf("Lambda: %g\n",lambda);
    printf("steps: %d\n",steps);
    printf("thermal: %d\n",thermal);
    printf("sweeps: %d\n",sweeps);
    printf("name: %s\n",name.c_str());
    
  
	setGlobalRNGSeed(seed); // Example seed value
	
	printf("######## Create a Ball ############\n");
	
    Ball ball; // Assuming Ball's constructor initializes at least one cube.
    
    
    
    printf("###### START THERMAL: ######\n");
    
    
  	
  	for(int i = 0 ; i < V; i++) {
		ball.performGrow();
		ball.measure();
    }
    
    for(int i = 0 ; i < V; i++) {
		if(0.5 > uniform_real()) ball.performGrow();
			else ball.performShrink();	
		ball.measure();
    }
    
	window = 10;
    
    // Cache window division result
    const int stepsPerWindow = int(steps/window);
    
    for(int i = 0 ; i < thermal; i++) {
		for(int j = 0 ; j < stepsPerWindow; j++) {
			meanV = 0;
			for(int k = 0 ; k < window ; k++) {
				if(0.5 > uniform_real()) ball.performGrow();
				else ball.performShrink();
				meanV+=ball.getNextCubeId();
			}
		}
		
		ball.measure();
		
		ball.tuneV();
    }


    printf("###### PRINT CONFIGS: ######\n");
    
    ball.printConfigs();
    
		
    printf("###### FINITO ######\n");

    return 0;
}

