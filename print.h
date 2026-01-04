#pragma once
#ifndef PRINT_H
#define PRINT_H

#include "ball.h"







void Ball::printCubulation() {
    printf("##################################################################\n");
    printf("######################## PRINT CUBULATION ########################\n");
    printf("##################################################################\n\n");
    
    printf("###################   Cubes Information    #####################:\n");
    for (size_t i = 0; i < nextCubeId; ++i) {
        Cube* cube = cubeMap[i];
        printf("Cube ID: %zu ", i);
        cube->getVector().printCoord();
        printf("Faces: ");
        for (Face* f : cube->faces) {
            if (f) printf("%d ", f->getId());
        }
        printf("\n");
        
        // New section for printing neighbor information
        printf("Neighbors: ");
        for (int idx = 0; idx < 27; idx++) {
            if (idx == 13) continue;
            Cube* neighborCube = cube->neighbors[idx];
            if (!neighborCube) continue;
            const Vector3 dir = Vector3::neighborFromIndex(idx);
            printf("- %d (%d,%d,%d) ", neighborCube->getId(), dir.x, dir.y, dir.z);
        }
        printf("\n");
    }

    printf("###################   Face Information    #####################:\n");
    for (size_t i = 0; i < nextFaceId; ++i) {
        Face* face = faceMap[i];
        printf("Face ID: %zu Cubes: ", i);
        for (Cube* c : face->cubes) {
            if (c) printf("%d ", c->getId());
        }
        printf("Is Boundary: %s ", face->isBoundary ? "Yes" : "No");
        printf("Neighbors: ");
        for (Face* nf : face->neighbors) {
            if (nf) printf("%d ", nf->getId());
        }
        face->getVector().printCoord();
    }

    printf("\n");
    
    printf("###################   Boundary Face Information   ###################:\n");
	for (size_t i = 0; i < nextFaceBId; ++i) {
    Face* face = BoundaryFaces[i];
    if (!face) continue; // Safety check to skip over null pointers
    
    
    face->printNeighbors();
     
    printf(" %s ", face->isBoundary ? "Boundary" : "");
    printf("Face ID: %zu ", i);

    printf("Cubes: ");
    for (Cube* c : face->cubes) {
        if (c) printf("%d ", c->getId());
    }
   
    printf("Neighbors: [ ");
    for (Face* neighborFace : face->neighbors) {
        if (neighborFace) printf("%d ", neighborFace->getId());
    }
    printf("] ");
    face->getVector().printCoord();
}

	printf("\n");
    
    printf("##################################################################\n");
    printf("######################## CUBULATION PRINTED ######################\n");
    printf("##################################################################\n\n");
}



void Ball::printCubeNeighbors(const char* filename) {
    FILE* file = fopen(filename, "w");

	Vector3 directions[6] = {
        Vector3(1, 0, 0),  // px
        Vector3(0, 1, 0),  // py
        Vector3(0, 0, 1),  // pz
        Vector3(-1, 0, 0), // mx
        Vector3(0, -1, 0), // my
        Vector3(0, 0, -1)  // mz
    };
    
    Cube * neighborCube = nullptr;
    
    for (int i = 0; i < nextCubeId; ++i) {
        Cube* cube = cubeMap[i];
        fprintf(file, "%d\t", cube->getId());
        
        for(int j = 0 ; j < 6 ; j++) {
        	neighborCube = cube->getNeighbor(directions[j]);
        	if(neighborCube)
        	fprintf(file, "%d\t", neighborCube->getId());
        }

        fprintf(file,"\n");
    }

    fclose(file);
}



void Ball::printBoundaryFaceNeighbors(const char* filename) {
    FILE* file = fopen(filename, "w");

    for (size_t i = 0; i < nextFaceBId; ++i) {
        Face* face = BoundaryFaces[i];
        fprintf(file, "%d ", face->getBId());
        for (Face* neighborFace : face->neighbors) {
            if (neighborFace) fprintf(file, "%d ", neighborFace->getBId());
        }
        fprintf(file,"\n");
    }

    fclose(file);
}


#include <cstdio>  // For file operations

void Ball::printCubeDensity(const char* filename) {
    FILE* file = fopen(filename, "w");  // Open the file for writing
    if (!file) {
        perror("Failed to open file");
        return;
    }

    // Write the header line
    //fprintf(file, "ID x y z\n");

    // Iterate through all cubes and print their coordinates
    for (size_t i = 0; i < nextCubeId; ++i) {
        Cube* cube = cubeMap[i];
        if (cube) {  // Make sure the cube pointer is not null
            const Vector3& coord = cube->getVector();  // Assuming getVector() returns a reference to the Vector3 of the cube's position
            fprintf(file, "%d %d %d %d\n", cube->getId(), coord.x, coord.y, coord.z);
        }
    }

    fclose(file);  // Close the file
}


	void Ball::printConfigs() {
	
		char Bafilename[256];
        char Cafilename[256];
        char CDfilename[256];

    		
		sprintf(Bafilename, "Boundary-%s.out", name.c_str());
		sprintf(Cafilename, "Cubulation-%s.out", name.c_str());
		sprintf(CDfilename, "CubeDensity-%s.out", name.c_str());
	
		printBoundaryFaceNeighbors(Bafilename);
		printCubeNeighbors(Cafilename);
		printCubeDensity(CDfilename);
	}
    

	
#endif
