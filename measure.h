
#pragma once
#ifndef MEASURE_H
#define MEASURE_H

void Ball::measure() {
    
    char filename[256];
    		
    sprintf(filename, "cube-%s.out", name.c_str());
      
    FILE* out = fopen(filename, "a");
    if (!out) {
        perror("Failed to open file for output");
        return;
    }

    // Measure the area of the cubulation
    fprintf(out, "%d\t%d\t", nextCubeId,nextFaceBId);

    // Compute the average coordinate
    Vector3 avg = {0, 0, 0};
    Vector3 vec = {0, 0, 0};
    Vector3 sum = {0, 0, 0};   
    double R = 0, r = 0;
    double R2 = 0;
    double R3 = 0;
    double R4 = 0;
    
    for (int i = 0; i < nextCubeId; i++) {
        vec = cubeMap[i]->getVector();
        sum = sum+vec;        
    }
    avg = sum*(1/nextCubeId);
    

    for (int i = 0; i < nextCubeId; i++) {
       
        Vector3 vec = cubeMap[i]->getVector();
        r = (avg-vec).det();
        
        R += r;
        R2 += r*r;
        R3 += r*r*r;
        R4 += r*r*r*r;
         
    }
    R /= nextCubeId;


    fprintf(out, "%g\t", R);
	fprintf(out, "%g\t", R2);
	fprintf(out, "%g\t", R3);
	fprintf(out, "%g\t", R4);

    fprintf(out,"%g\t",lambda);
    fprintf(out,"%g\n",alpha);

    // Close the file
    fclose(out);
}






#endif
