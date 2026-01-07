#pragma once
#ifndef SHRINK_CUBE_H
#define SHRINK_CUBE_H

/*
 * Performance optimizations applied: 2026-01-07
 * Author: Dániel Németh
 * 
 * Changes:
 * - Added caching of nextCubeId, nextFaceBId, and orthogonals arrays
 * - Optimized CheckValidShrink() and shrinkCube() functions with cached values
 * - Reduced redundant calculations in tight loops
 */

#include "ball.h"


bool Ball:: performShrink() {
	// Cache nextCubeId and nextFaceBId to avoid repeated member access
	const int cachedNextCubeId = nextCubeId;
	if(cachedNextCubeId == 1) return false;
	
	std::pair<int, Face*> deltaNB;
	
	const int cachedNextFaceBId = nextFaceBId;
	deltaNB = CheckValidShrink(GetBoundaryFace(uniform_int(cachedNextFaceBId)));
	
	if(deltaNB.first == -1) return false;
	
	if(getActionDiffShrink((double)deltaNB.first)) shrinkCube(deltaNB.second);	
	
	
	return true;

}

		
std::pair<int, Face*> Ball::CheckValidShrink(Face* boundaryFace) {	
	Cube * cube = boundaryFace->getCube();
	
	Vector3 direction = boundaryFace->getVector();
	    
    // Cache orthogonals array - reused multiple times
    auto orthogonals = direction.getOrthogonal(); // 4 orthogonal directions (no allocation)
      
    Cube * tempCube0 = nullptr;
    Cube * tempCube1 = nullptr;
    Cube * tempCube2 = nullptr;
    Cube * tempCube3 = nullptr;

   // ROTATE THE POINT OF VIEW If no bottom
   
    if(!(cube->getNeighbor(direction * -1))) {
		tempCube0 = cube->getNeighbor(orthogonals[0]);
		tempCube1 = cube->getNeighbor(orthogonals[1]);
		tempCube2 = cube->getNeighbor(orthogonals[2]);
		tempCube3 = cube->getNeighbor(orthogonals[3]);

		if(tempCube0 && !tempCube2) boundaryFace = cube->getFace(orthogonals[2]);
		else if(!tempCube0 && tempCube2) boundaryFace = cube->getFace(orthogonals[0]);
		else if(tempCube1 && !tempCube3) boundaryFace = cube->getFace(orthogonals[3]);
		else if(!tempCube1 && tempCube3) boundaryFace = cube->getFace(orthogonals[1]);
		else return std::make_pair(-1, boundaryFace); // invalid config-->changes topology
		
		direction = boundaryFace->getVector();
		orthogonals = direction.getOrthogonal(); // recompute orthogonals for new direction
    }
	
	//assert(cube->getNeighbor(direction * -1));
    
    Cube * sideCubes_below[4] =  {nullptr};
	Cube * cornerCubes_below[4] =  {nullptr};
	Cube * sideCubes_layer[4] =  {nullptr};
	Cube * cornerCubes_layer[4] =  {nullptr};
	Cube * sideCubes_above[4] =  {nullptr};
	Cube * cornerCubes_above[4] =  {nullptr};
	Cube * bottomCube = nullptr;
    
	Cube * tempCube = nullptr;
	
	for(int i = 0 ; i < 4 ; i++) {
			
		sideCubes_layer[i] = cube->getNeighbor(orthogonals[i]); 
		
		cornerCubes_layer[i] = cube->getNeighbor(orthogonals[i]+orthogonals[(i+1)%4]);

		sideCubes_above[i] = cube->getNeighbor(orthogonals[i]+direction);
		cornerCubes_above[i] = cube->getNeighbor(orthogonals[i]+orthogonals[(i+1)%4]+direction);
		
		sideCubes_below[i] = cube->getNeighbor(orthogonals[i] + direction * -1); 
		cornerCubes_below[i] = cube->getNeighbor(orthogonals[i]+orthogonals[(i+1)%4]+ direction * -1);
		
		sideCubes_above[i] = cube->getNeighbor(orthogonals[i]+direction);
		cornerCubes_above[i] = cube->getNeighbor(orthogonals[i]+orthogonals[(i+1)%4]+direction);
			
	}
	/*
	printf(" in Shrink CHECK  \n");
	printf("Cube ID : %d \t below id: %d \n",cube->getId(),cube->getNeighbor(direction * -1)->getId());
		
	for(int i = 0 ; i < 4 ; i++) {
   		if(sideCubes_below[i]) printf("sb %d\t%d\n",i,sideCubes_below[i]->getId());
   		if(sideCubes_layer[i]) printf("sl %d\t%d\n",i,sideCubes_layer[i]->getId());
   		if(sideCubes_above[i]) printf("sa %d\t%d\n",i,sideCubes_above[i]->getId());
   		if(cornerCubes_below[i]) printf("cb %d\t%d\n",i,cornerCubes_below[i]->getId());
   		if(cornerCubes_layer[i]) printf("cl %d\t%d\n",i,cornerCubes_layer[i]->getId());
   		if(cornerCubes_above[i]) printf("ca %d\t%d\n",i,cornerCubes_above[i]->getId());
   	}

	*/
	
	
	bottomCube = cube->getNeighbor(direction * -1);
	
	
	int sumACA = 0;
	
	for(int i = 0 ; i < 4 ; i++) if(sideCubes_layer[i]) sumACA++;
	
	
	for(int i = 0 ;i < 4 ; i++) {
		if( sideCubes_layer[i] && !sideCubes_below[i] ) return  std::make_pair(-1, boundaryFace); //avoid edge connection
		if( (sideCubes_layer[i] && sideCubes_layer[(i+2)%4]) && (sumACA == 2 && (sideCubes_below[i] && sideCubes_below[(i+2)%4]) ) ) return std::make_pair(-1, boundaryFace); //avoid changing the topology
		if( (sideCubes_layer[i] && sideCubes_layer[(i+1)%4]) && !cornerCubes_layer[i] ) return  std::make_pair(-1, boundaryFace); //avoid edge connection
	}
	
	int dNB  = -4 + 2*sumACA;
	
	
       
	return std::make_pair(dNB, boundaryFace);

}



void Ball::shrinkCube(Face* boundaryFace) {
//	printf(" ################################################# \n");
//	printf(" ################### START SHRINK ############## \n");
//	printf(" ################################################# \n");
				
				
	Cube * cube = boundaryFace->getCube();
	Vector3 direction = boundaryFace->getVector();
	// Cache orthogonals array - reused multiple times
	const auto orthogonals = direction.getOrthogonal(); // 4 orthogonal directions (no allocation)
    
    Cube * tempCube0 = nullptr;
    Cube * tempCube1 = nullptr;
    Cube * tempCube2 = nullptr;
    Cube * tempCube3 = nullptr;

/*    if(!(cube->getNeighbor(direction * -1))) {
		tempCube0 = cube->getNeighbor(orthogonals[0]);
		tempCube1 = cube->getNeighbor(orthogonals[1]);
		tempCube2 = cube->getNeighbor(orthogonals[2]);
		tempCube3 = cube->getNeighbor(orthogonals[3]);
		
		if(tempCube0 && !tempCube2) direction = orthogonals[2];
		else if(!tempCube0 && tempCube2) direction = orthogonals[0];
		else if(tempCube1 && !tempCube3) direction = orthogonals[3];
		else if(!tempCube1 && tempCube3) direction = orthogonals[1]; 

		orthogonals = direction.getOrthogonal();
		boundaryFace = cube->getFace(direction);
    	
    }
 
	assert(cube->getNeighbor(direction * -1));
 */   
      
    Cube * sideCubes_below[4] =  {nullptr};
	Cube * cornerCubes_below[4] =  {nullptr};
	Cube * sideCubes_layer[4] =  {nullptr};
	Cube * cornerCubes_layer[4] =  {nullptr};
	Cube * sideCubes_above[4] =  {nullptr};
	Cube * cornerCubes_above[4] =  {nullptr};
	Cube * bottomCube = nullptr;
    Face * bottomFace = nullptr;
	
	Face * adjacentFaces[4] = {nullptr};
	Face * sideFaces[4] = {nullptr};
	
	bottomFace = cube->getFace(direction * -1);
	bottomCube = cube->getNeighbor(direction * -1);
	
	for(int i = 0 ; i < 4 ; i++) {
		sideCubes_layer[i] = cube->getNeighbor(orthogonals[i]); 
		cornerCubes_layer[i] = cube->getNeighbor(orthogonals[i]+orthogonals[(i+1)%4]);

		sideCubes_above[i] = cube->getNeighbor(orthogonals[i]+direction);
		cornerCubes_above[i] = cube->getNeighbor(orthogonals[i]+orthogonals[(i+1)%4]+direction);
		
		sideCubes_below[i] = cube->getNeighbor(orthogonals[i] + direction * -1); 
		cornerCubes_below[i] = cube->getNeighbor(orthogonals[i]+orthogonals[(i+1)%4]+ direction * -1);
			
		sideFaces[i] = cube->getFace(orthogonals[i]);
	}
	
	for(int i = 0 ; i < 4 ; i++) adjacentFaces[i] = boundaryFace->getAdjacent(orthogonals[i]);
	
	for(int i = 0 ; i < 4 ; i++) {
		
		if(sideCubes_above[i]) unsetCubeCubeNeighbor(cube,sideCubes_above[i],orthogonals[i]+direction);
		if(cornerCubes_above[i]) unsetCubeCubeNeighbor(cube,cornerCubes_above[i],orthogonals[i]+orthogonals[(i+1)%4]+direction);
		
		if(sideCubes_layer[i]) unsetCubeFaceNeighbor(cube,sideCubes_layer[i],orthogonals[i]);
		else unsetNewCubeBFacePair(cube,sideFaces[i],orthogonals[i]);

		if(cornerCubes_layer[i]) unsetCubeCubeNeighbor(cube,cornerCubes_layer[i],orthogonals[i]+orthogonals[(i+1)%4]);

		if(sideCubes_below[i]) unsetCubeCubeNeighbor(cube,sideCubes_below[i],orthogonals[i]+direction*-1);
		if(cornerCubes_below[i]) unsetCubeCubeNeighbor(cube,cornerCubes_below[i],orthogonals[i]+orthogonals[(i+1)%4]+direction*-1);

	}
	
	
	unsetCubeFaceNeighbor(cube,bottomCube,direction*-1);
	
	RemoveFaceBoundary(boundaryFace);
	
	for(int i = 0 ; i < 4 ; i++)  {
		if(!sideCubes_layer[i]) {	
			RemoveFaceBoundary(sideFaces[i]);
			
			deleteFace(sideFaces[i]);
		}
		else RestoreFaceBoundary(sideFaces[i],orthogonals[i] * -1); 
		
	}
	RestoreFaceBoundary(bottomFace,direction);

	
	for(int i = 0 ; i <4 ; i++) {
	
		if(sideCubes_above[i]) setFaceFaceAdjacent(sideFaces[i],adjacentFaces[i],direction,direction * -1);
		else if(sideCubes_layer[i]) setFaceFaceAdjacent(sideFaces[i],adjacentFaces[i],direction,orthogonals[i]*-1);

		if(sideCubes_layer[i] && sideCubes_layer[(i+1)%4]) setFaceFaceAdjacent(sideFaces[i],sideFaces[(i+1)%4],orthogonals[(i+1)%4],orthogonals[i]);
			
		else if(sideCubes_layer[i]) {
			if(cornerCubes_layer[i]) setFaceFaceAdjacent(sideFaces[i],cornerCubes_layer[i]->getFace(orthogonals[i]*-1),orthogonals[(i+1)%4],orthogonals[(i+1)%4]*-1);		
			else setFaceFaceAdjacent(sideFaces[i],sideCubes_layer[i]->getFace(orthogonals[(i+1)%4]),orthogonals[(i+1)%4],orthogonals[i]*-1);
			
		}
		else if(sideCubes_layer[(i+1)%4]) {
			
			if(cornerCubes_layer[i]) setFaceFaceAdjacent(sideFaces[(i+1)%4],cornerCubes_layer[i]->getFace(orthogonals[(i+1)%4]*-1),orthogonals[i],orthogonals[i]*-1);			
			else setFaceFaceAdjacent(sideFaces[(i+1)%4],sideCubes_layer[(i+1)%4]->getFace(orthogonals[i]),orthogonals[i],orthogonals[(i+1)%4]*-1);

		}
		else {;}	
	
		if(sideCubes_layer[i]) setFaceFaceAdjacent(bottomFace,sideFaces[i],orthogonals[i], direction*-1);
		else if(sideCubes_below[i]) setFaceFaceAdjacent(bottomFace,sideCubes_below[i]->getFace(direction),orthogonals[i], orthogonals[i] * -1);
		else setFaceFaceAdjacent(bottomFace,bottomCube->getFace(orthogonals[i]),orthogonals[i], direction);

	}

	
	deleteFace(boundaryFace);

	deleteCube(cube);
	
}


















    
    
    
    
    
    

	
#endif
