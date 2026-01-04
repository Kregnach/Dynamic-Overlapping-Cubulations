
#pragma once
#ifndef MEASURE_H
#define MEASURE_H

void Ball::measure() {
    
    char filename[256];
    		
    sprintf(filename, "cube-%s.out", name.c_str());
    
    // Opening/closing per call is expensive; keep a buffered handle.
    static FILE* out = nullptr;
    static std::string openedFor;
    static int flushCounter = 0;
    if (!out || openedFor != filename) {
        if (out) fclose(out);
        out = fopen(filename, "a");
        if (!out) {
            perror("Failed to open file for output");
            return;
        }
        openedFor = filename;
        setvbuf(out, nullptr, _IOFBF, 1 << 20); // 1MB buffer
    }

    // Measure the area of the cubulation
    fprintf(out, "%d\t%d\t", nextCubeId,nextFaceBId);

    // Compute the average coordinate (centroid)
    double sumx = 0.0, sumy = 0.0, sumz = 0.0;
    double R = 0.0, r = 0.0;
    double R2 = 0;
    double R3 = 0;
    double R4 = 0;
    
    for (int i = 0; i < nextCubeId; i++) {
        const Vector3& vec = cubeMap[i]->getVector();
        sumx += vec.x;
        sumy += vec.y;
        sumz += vec.z;
    }
    const double invN = 1.0 / static_cast<double>(nextCubeId);
    const double avgx = sumx * invN;
    const double avgy = sumy * invN;
    const double avgz = sumz * invN;
    

    for (int i = 0; i < nextCubeId; i++) {
       
        const Vector3& vec = cubeMap[i]->getVector();
        const double dx = avgx - static_cast<double>(vec.x);
        const double dy = avgy - static_cast<double>(vec.y);
        const double dz = avgz - static_cast<double>(vec.z);
        r = std::sqrt(dx*dx + dy*dy + dz*dz);
        
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

    // Periodically flush (avoid paying the cost every call).
    if ((++flushCounter & 1023) == 0) fflush(out);
}






#endif
