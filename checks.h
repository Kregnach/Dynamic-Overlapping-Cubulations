
#pragma once
#ifndef CHECKS_H
#define CHECKS_H

#include "ball.h"
#include "cube.h"
#include <cassert>  // For assert



void Ball::performAllChecks() {

   validateBoundaryFaces();
   validateAllCubeFaces();
   validateCubeNeighbors();
   validateBoundaryCube();
   validateBoundaryFaceNeighbors();

}


void Ball::validateBoundaryCube() {
    printf("Check if a boundary cube has no possible cube on the boundary\n");

    Vector3 directions[6] = {
        Vector3(1, 0, 0),  // px
        Vector3(0, 1, 0),  // py
        Vector3(0, 0, 1),  // pz
        Vector3(-1, 0, 0), // mx
        Vector3(0, -1, 0), // my
        Vector3(0, 0, -1)  // mz
    };

    for (size_t i = 0; i < nextFaceBId; i++) {
        Face* face = BoundaryFaces[i];
        Cube* cube = face->getCube();
        Vector3 direction = face->getVector();
        

		 auto orthogonals = direction.getOrthogonal(); // 4 orthogonal directions (no allocation)
		 
		 		
        // Check the cube at the boundary direction
        assert( cube->getNeighbor(direction) == nullptr); // There should be no cube in the boundary direction

		Cube * neighbor = nullptr;
			
		for(int j = 0 ; j < 4 ; j++) {
			neighbor = cube->getNeighbor(orthogonals[j]); //sideCubes_below
			if(neighbor) {
				if(neighbor->getNeighbor(orthogonals[j]*-1 + direction)) {
					printf(" ASSERT : cube:%d\t nb:%d->%d\n",cube->getId(),neighbor->getId(),neighbor->getNeighbor(orthogonals[j]*-1 + direction)->getId());
					direction.printCoord();
					orthogonals[j].printCoord();
					assert(!neighbor->getNeighbor(orthogonals[j]*-1 + direction));
				}
			}
						
			neighbor = cube->getNeighbor(orthogonals[j]+orthogonals[(j+1)%4]); //cornerCubes_below
			if(neighbor) assert(!neighbor->getNeighbor(orthogonals[(j+1)%4]*-1 + orthogonals[j]*-1 + direction));
			
			neighbor = cube->getNeighbor(orthogonals[j] + direction); //sideCubes_layer
			if(neighbor) assert(!neighbor->getNeighbor(orthogonals[j]*-1));
			
			neighbor = cube->getNeighbor(orthogonals[j]+orthogonals[(j+1)%4] + direction); //cornerCubes_layers
			if(neighbor) {
				if(neighbor->getNeighbor(orthogonals[j]*-1+orthogonals[(j+1)%4]*-1)) {
					
					printf(" ASSERT : cube:%d\t nb:%d->%d\n",cube->getId(),neighbor->getId(),neighbor->getNeighbor(orthogonals[j]*-1+orthogonals[(j+1)%4]*-1)->getId());
					

					
					assert(!neighbor->getNeighbor(orthogonals[j]*-1+orthogonals[(j+1)%4]*-1));
				}
			}
			

			
		} 
    }

    printf(" FREE spots Checked \n");
}

void Ball::validateCubeNeighbors() {
    printf("Validating cube neighbors...\n");

    Vector3 directions[6] = {
        Vector3(1, 0, 0),  // px
        Vector3(0, 1, 0),  // py
        Vector3(0, 0, 1),  // pz
        Vector3(-1, 0, 0), // mx
        Vector3(0, -1, 0), // my
        Vector3(0, 0, -1)  // mz
    };

    for (int i = 0; i < nextCubeId; i++) {
        Cube* cube = cubeMap[i];
        assert(cube); // Ensure the cube exists

        for (int j = 0; j < 6; j++) {
            Cube* neighbor = cube->getNeighbor(directions[j]);
            if (neighbor) { // If a neighbor exists in this direction
                // Check reverse relationship
                Cube* reverseNeighbor = neighbor->getNeighbor(directions[j] * -1);
                assert(reverseNeighbor == cube); // The neighbor's neighbor in the opposite direction should be the original cube
            }
        }
    }

    printf("Neighbor validation complete.\n");
}

void Ball::validateAllCubeFaces() {
    printf("Validating all cube faces...\n");

		Vector3 px(1, 0, 0); // create the base vector set
		Vector3 py(0, 1, 0);
		Vector3 pz(0, 0, 1);
		Vector3 mx(-1, 0, 0);
		Vector3 my(0, -1, 0);
		Vector3 mz(0, 0, -1); // create the base vector set
		
		    
    Vector3 directions[6] = {px,py,pz,mx,my,mz};
    

    for (int i = 0; i < nextCubeId; i++) {
        Cube* cube = cubeMap[i];
        assert(cube);
        
        for(int j = 0 ; j < 6 ; j++) {
	        Face * face = cube->getFace(directions[j]);
	        
	         if (face->getIsBoundary()) face->getVector().validateNonZero();
             else face->getVector().validateZero();
        }
    }

	printf("DONE\n");
}

void Ball::validateBoundaryFaces() {
//	printf("##################### VALIDATE BOUNDARY FACES #####################\n");
    for (size_t i = 0; i < nextFaceBId; i++) {
        Face* face = BoundaryFaces[i];
        
        Cube * cube = face->getCube();
        Vector3 direction = face->getVector();
  //      printf("faceid boundary id: %d %d\n",face->getId(),face->getBId());
//		printf(" sits with boundary %d\t \n",face->getIsBoundary());
		
	//	if(face->getIsBoundary() == 0) printf("on Cube: %d \t",cube->getId());
        
        assert(face->getIsBoundary() != 0);
        
        direction.validateNonZero();
        
        assert(cube->getFace(direction) == face);


        // Assert that each face marked as a boundary actually has the isBoundary flag set to true
//        assert(face->isBoundary && "Boundary face does not have isBoundary set to true");
    }
    
//    printf("##################### ALL DONE #####################\n");
}


void Ball::validateBoundaryFaceNeighbors() {
    Face* neighborFace = nullptr;
    Face* backFace = nullptr;
    int faceBId = 0, faceId;
    Face* face = nullptr;
    Cube * cube = nullptr;
    
   for (size_t i = 0; i < nextFaceBId; i++) {
	//    printf(" I = %ld\n",i);
        face = BoundaryFaces[i];
        
        faceBId = face->getBId();
        faceId = face->getId();
        face->printNeighbors();

        Vector3 direction = face->getVector();
		face->printNeighbors();
		
        auto orthogonals = direction.getOrthogonal();
        
        

        assert(face->getIsBoundary() == 1);
		
		//direction.printCoord();        

        direction.validateNonZero();
        
        face->checkNeighborDirections();
        
        
        cube = face->getCube();
        assert(cube->getFace(direction) == face);
        
       // printf("i:%ld/%d faceID: %d\tfaceBID:  %d\t cubeID: %d\tj:\n",i,nextFaceBId-1,faceId,faceBId,cube->getId());
        for(int j = 0 ; j < 4 ; j++) {
        
        //	printf(" in Loop %d\t",j);
        	
        	neighborFace = face->getAdjacent(orthogonals[j]);
        	
        	//printf("cubeID: %d\t",neighborFace->getCube()->getId());
        	
        	assert(neighborFace);
        	
        	//neighborFace->printNeighbors();
        	
        	//printf("after assert \n");
        	
    		
    		if (neighborFace->getVector() == direction) backFace =  neighborFace->getAdjacent(orthogonals[j] * -1);  
	        else if(neighborFace->getVector() == orthogonals[j]) backFace = neighborFace->getAdjacent(direction);  
        	else backFace = neighborFace->getAdjacent(direction * -1);  
        	
        	//printf("before next assert \n");
        	
        	//if(!backFace) {
        	//	printf(" WUT ?\n");
        		
        		//face->printNeighbors();
        	//	}
        	
        	//if(backFace->getBId() != faceBId) {printf(" BID: %d %d\n",backFace->getBId(),faceBId);}
        	
        	assert(backFace->getBId() == faceBId);
        	
        	//printf("this failed? \n");
        }
       // printf("\n");
      
	}
	
//	printf("  ################# validateBoundaryFaceNeighbors VALIDATION FINISHED!! #################  \n");
}





#endif
