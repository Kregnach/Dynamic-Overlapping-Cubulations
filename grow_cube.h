#pragma once
#ifndef GROW_CUBE_H
#define GROW_CUBE_H




#include "ball.h"
#include <numeric>



bool Ball:: performGrow() {

	std::pair<int, Face*> deltaNB;

	if(nextFaceBId-1 == 100000-2) return false;
	
	deltaNB = CheckValidGrow(GetBoundaryFace(uniform_int(nextFaceBId)));
	
	if(deltaNB.first == -1) return false;
		
	if(getActionDiffGrow((double)deltaNB.first)) growCube(deltaNB.second);


	return true;

}




std::pair<int, Face*> Ball::CheckValidGrow(Face* boundaryFace) {

//	printf("####################### CHECK VALIDATE GROW #######################\n");	

	
    Cube* oldCube = boundaryFace->getCube();

    Vector3 direction = boundaryFace->getVector(); // Get the direction of the boundary cube
    
    auto orthogonals = direction.getOrthogonal(); // 4 orthogonal directions (no allocation)
	
//	printf("BoundaryFace of CUBE %d: \n",oldCube->getId());
//	direction.printCoord();
//	printf("OCD good\n");

    Cube * sideCubes_below[4] =  {nullptr};
	Cube * cornerCubes_below[4] =  {nullptr};
	Cube * sideCubes_layer[4] =  {nullptr};
	Cube * cornerCubes_layer[4] =  {nullptr};
	Cube * sideCubes_above[4] =  {nullptr};
	Cube * cornerCubes_above[4] =  {nullptr};
	Cube * topCube = nullptr;
    
	Cube * tempCube = nullptr;
	Face * tempFace = nullptr;
	Face * sideFace = nullptr;
    

    for(int i = 0 ; i <4 ; i++) {
		sideCubes_below[i] = oldCube->getNeighbor(orthogonals[i]);
		sideCubes_layer[i] = oldCube->getNeighbor(orthogonals[i]+direction);
		
		if(sideCubes_layer[i] && !topCube) topCube = sideCubes_layer[i]->getNeighbor(direction+orthogonals[i]*-1);	
			
    }
	
//	########################################################
	// ROTATE THE POINT OF VIEW If topCube
//	########################################################
   	
	if(topCube) {
		
		for(int i = 0 ; i < 4 ; i++) if(!oldCube->getNeighbor(direction+orthogonals[i]) && topCube->getNeighbor(direction*-1+orthogonals[i])) return std::make_pair(-1, boundaryFace);
		
		if(sideCubes_layer[0] && !sideCubes_layer[2]) boundaryFace = sideCubes_layer[0]->getFace(orthogonals[2]);
		else if(sideCubes_layer[2] && !sideCubes_layer[0]) boundaryFace = sideCubes_layer[2]->getFace(orthogonals[0]);
		else if(sideCubes_layer[1] && !sideCubes_layer[3]) boundaryFace = sideCubes_layer[1]->getFace(orthogonals[3]);
		else if(sideCubes_layer[3] && !sideCubes_layer[1]) boundaryFace = sideCubes_layer[3]->getFace(orthogonals[1]);
		else return std::make_pair(-1, boundaryFace);
    	
    	direction = boundaryFace->getVector(); 
    	
		orthogonals = direction.getOrthogonal(); // reset the orthogonals

		oldCube = boundaryFace->getCube(); // reset the oldCube

	}
    
    // Reset local neighbor caches (avoid out-of-bounds writes on [4]).
    for (int i = 0; i < 4; i++) {
        sideCubes_below[i] = nullptr;
        sideCubes_layer[i] = nullptr;
        sideCubes_above[i] = nullptr;
        cornerCubes_below[i] = nullptr;
        cornerCubes_layer[i] = nullptr;
        cornerCubes_above[i] = nullptr;
    }
    
    
    Face * adjacentFaces[4] = {nullptr};
    topCube = nullptr;
	
//	########################################################
// ROTATE THE POINT OF VIEW If topCube
//	########################################################
//	########################################################
// SET UP THE NEIGHBOUR GRID
//	########################################################

	for(int i = 0 ; i < 4 ; i++) {
		sideCubes_below[i] = oldCube->getNeighbor(orthogonals[i]);
		sideCubes_layer[i] = oldCube->getNeighbor(orthogonals[i]+direction);
		cornerCubes_below[i] = oldCube->getNeighbor(orthogonals[i]+orthogonals[(i+1)%4]);
		cornerCubes_layer[i] = oldCube->getNeighbor(orthogonals[i]+orthogonals[(i+1)%4]+direction);
		
		if(sideCubes_layer[i]) sideCubes_above[i] = sideCubes_layer[i]->getNeighbor(direction);
		if(cornerCubes_layer[i]) cornerCubes_above[i] = cornerCubes_layer[i]->getNeighbor(direction);	
		
    } // simple adjacencies

	//Find non-simple connections
	for(int i = 0 ; i < 4 ; i++) {
    	if(sideCubes_layer[i]) {
    		
    		if(!cornerCubes_above[i]) cornerCubes_above[i] = sideCubes_layer[i]->getNeighbor(orthogonals[(i+1)%4]+ direction);
    		if(!cornerCubes_above[(i+3)%4]) cornerCubes_above[(i+3)%4] = sideCubes_layer[i]->getNeighbor(orthogonals[(i+3)%4] + direction);

    		if(!sideCubes_above[(i+1)%4]) sideCubes_above[(i+1)%4] = sideCubes_layer[i]->getNeighbor(orthogonals[(i+1)%4] + orthogonals[i]*-1 + direction);
      		if(!sideCubes_above[(i+3)%4]) sideCubes_above[(i+3)%4] = sideCubes_layer[i]->getNeighbor(orthogonals[(i+3)%4] + orthogonals[i]*-1 + direction);

//    		if(!cornerCubes_above[i]) cornerCubes_above[i] = sideCubes_layer[i]->getNeighbor(orthogonals[(i+1)%4]);
    	}    
    }
    
    
	
	for(int i = 0 ; i <4 ; i++) {	

		if(sideCubes_above[i] && !cornerCubes_above[i]) {
			cornerCubes_above[i] = sideCubes_above[i]->getNeighbor(orthogonals[(i+1)%4]);
			
			if(cornerCubes_above[i] && !sideCubes_above[(i+1)%4]) if(cornerCubes_above[i]->getNeighbor(orthogonals[i]*-1)) return std::make_pair(-1, boundaryFace); //disagreement in connection, don't grow
		}

		if(sideCubes_above[i] && !cornerCubes_above[(i+3)%4]) {
			cornerCubes_above[(i+3)%4] = sideCubes_above[i]->getNeighbor(orthogonals[(i+3)%4]);
			
			if(cornerCubes_above[(i+3)%4] && !sideCubes_above[(i+3)%4]) if(cornerCubes_above[(i+3)%4]->getNeighbor(orthogonals[i]*-1)) return std::make_pair(-1, boundaryFace); //disagreement in connection, don't grow
				
		}
	
		if(cornerCubes_above[i] && !sideCubes_above[i]) if(cornerCubes_above[i]->getNeighbor(orthogonals[(i+1)%4]*-1)) return std::make_pair(-1, boundaryFace);  //disagreement in connection, don't grow
		if(cornerCubes_above[i] && !sideCubes_above[(i+1)%4]) if(cornerCubes_above[i]->getNeighbor(orthogonals[i]*-1)) return std::make_pair(-1, boundaryFace);  //disagreement in connection, don't grow
		
	}	

	for(int i = 0 ; i < 4 ; i++) {				
		if(sideCubes_layer[i]) adjacentFaces[i] = sideCubes_layer[i]->getFace(orthogonals[i]*-1);
		else if(sideCubes_below[i]) adjacentFaces[i] = sideCubes_below[i]->getFace(direction);
		else adjacentFaces[i] = oldCube->getFace(orthogonals[i]); //adjacent Boundary Faces
	}
	

//	########################################################
// THE NEIGHBOUR GRID SET UP 
//	########################################################
	

	
	for(int i = 0 ; i < 4 ; i++) {	
		if(sideCubes_above[i] && !sideCubes_layer[i]) return std::make_pair(-1, boundaryFace); // avoid edge connection
		
//		if(cornerCubes_layer[i] && !sideCubes_layer[i] ) return std::make_pair(-1, boundaryFace); // avoid edge connection	
//		if(cornerCubes_layer[i] && !sideCubes_layer[(i+1)%4] ) return std::make_pair(-1, boundaryFace); // avoid edge connection
		
		if(cornerCubes_layer[i] && !(sideCubes_layer[i] || sideCubes_layer[(i+1)%4]) ) return std::make_pair(-1, boundaryFace); // avoid edge connection

//		if(cornerCubes_above[i] && !( (sideCubes_layer[i]) && (sideCubes_above[i]) ) ) return std::make_pair(-1, boundaryFace);
//		if(cornerCubes_above[i] && !( (sideCubes_layer[(i+1)%4]) && (sideCubes_above[(i+1)%4]) ) ) return std::make_pair(-1, boundaryFace);
		
		if(cornerCubes_above[i] && (!( (sideCubes_layer[(i+1)%4]) && (sideCubes_above[(i+1)%4]) ) ||!( (sideCubes_layer[i]) && (sideCubes_above[i]) ) ) ) return std::make_pair(-1, boundaryFace);
		
		
		
		if( sideCubes_layer[i] &&  sideCubes_layer[(i+2)%4] && !(sideCubes_below[i] && sideCubes_below[(i+2)%4]) )  return std::make_pair(-1, boundaryFace);
	}
  	
  
	//printf(" ############ in Validate CONNECTIONS after checking ###############  \n");
	 
/*
	for(int i = 0 ; i < 4 ; i++) {
   		if(sideCubes_below[i]) printf("sb %d\t%d\n",i,sideCubes_below[i]->getId());
   		if(sideCubes_layer[i]) printf("sl %d\t%d\n",i,sideCubes_layer[i]->getId());
   		if(sideCubes_above[i]) printf("sa %d\t%d\n",i,sideCubes_above[i]->getId());
   		if(cornerCubes_below[i]) printf("cb %d\t%d\n",i,cornerCubes_below[i]->getId());
   		if(cornerCubes_layer[i]) printf("cl %d\t%d\n",i,cornerCubes_layer[i]->getId());
   		if(cornerCubes_above[i]) printf("ca %d\t%d\n",i,cornerCubes_above[i]->getId());
   	}
 */ 			


 	int sumACA = 0;
 	for(int i = 0 ; i < 4 ; i++) if(sideCubes_layer[i]) sumACA++;
 	
 	int dNB  = 4 - 2*sumACA;
 	
 	
    return std::make_pair(dNB, boundaryFace); // Return the number of new faces and sum of adjacent cubes above
}



void Ball::growCube(Face* boundaryFace) {


    Cube* oldCube = boundaryFace->getCube(); //cube of the boundary Face
    Cube* newCube = createCube(); // create a new cube
    
	Cube * sideCubes_below[4] =  {nullptr};
	Cube * cornerCubes_below[4] =  {nullptr};
	Cube * sideCubes_layer[4] =  {nullptr};
	Cube * cornerCubes_layer[4] =  {nullptr};
	Cube * sideCubes_above[4] =  {nullptr};
	Cube * cornerCubes_above[4] =  {nullptr};

	Face* newFaces[5] = {nullptr};
	Face * adjacentFaces[4] = {nullptr};
	Face * sideFaces[4] = {nullptr};
	Face * topFace = nullptr;
	
	Face * tempFace = nullptr;
	
	int newFaceCounter = 0;
	int dNB = 5;
	
	Vector3 direction = boundaryFace->getVector(); // Set the new cube adjacent to the boundary face
	newCube->setVector(oldCube->getVector() + direction); // set the coordinate of the old cube
	auto orthogonals = direction.getOrthogonal(); // 4 orthogonal directions (no allocation)
	
	for(int i = 0 ; i < 4 ; i++) {
				
		sideCubes_below[i] = oldCube->getNeighbor(orthogonals[i]);
		sideCubes_layer[i] = oldCube->getNeighbor(orthogonals[i]+direction);
		
		if(sideCubes_layer[i]) dNB--; 
		
		cornerCubes_below[i] = oldCube->getNeighbor(orthogonals[i]+orthogonals[(i+1)%4]);
		cornerCubes_layer[i] = oldCube->getNeighbor(orthogonals[i]+orthogonals[(i+1)%4]+direction);
		
		if(sideCubes_layer[i]) sideCubes_above[i] = sideCubes_layer[i]->getNeighbor(direction);
		if(cornerCubes_layer[i]) cornerCubes_above[i] = cornerCubes_layer[i]->getNeighbor(direction);	
		
    } // simple adjacencies
	
	for(int i = 0 ; i < dNB ; i++) newFaces[i] = createFace(); // create deltaNB new faces
	
	for(int i = 0 ; i <4 ; i++) {
		if(sideCubes_above[i] && !cornerCubes_above[i]) {cornerCubes_above[i] = sideCubes_above[i]->getNeighbor(orthogonals[(i+1)%4]);}
		if(sideCubes_above[i] && !cornerCubes_above[(i+3)%4]) {cornerCubes_above[(i+3)%4] = sideCubes_above[i]->getNeighbor(orthogonals[(i+1)%4]*-1);};
	}	
	
	
	for(int i = 0 ; i < 4 ; i++) {				
		if(sideCubes_layer[i]) adjacentFaces[i] = sideCubes_layer[i]->getFace(orthogonals[i]*-1);
		else if(sideCubes_below[i]) adjacentFaces[i] = sideCubes_below[i]->getFace(direction);
		else adjacentFaces[i] = oldCube->getFace(orthogonals[i]); //adjacent Boundary Faces
	}	

	newCube->setFace(direction * -1, boundaryFace); //glue the boundary to the new cube
	boundaryFace->setCube(direction, newCube); //glue the new Cube to the boundary
	setCubeCubeNeighbor(oldCube,newCube,direction); //set them adjacent
	setNewCubeBFacePair(newCube,newFaces[dNB-1],direction); // set top face


	for(int i = 0 ; i < 4 ; i++) {

		if(cornerCubes_below[i]) setCubeCubeNeighbor(newCube,cornerCubes_below[i],orthogonals[i]+orthogonals[(i+1)%4]+direction*-1);
		if(cornerCubes_layer[i]) setCubeCubeNeighbor(newCube,cornerCubes_layer[i],orthogonals[i]+orthogonals[(i+1)%4]);
		if(cornerCubes_above[i]) setCubeCubeNeighbor(newCube,cornerCubes_above[i],orthogonals[i]+orthogonals[(i+1)%4]+direction);
		if(sideCubes_above[i]) setCubeCubeNeighbor(newCube,sideCubes_above[i],orthogonals[i]+direction);
		if(sideCubes_below[i]) setCubeCubeNeighbor(newCube,sideCubes_below[i],orthogonals[i]+direction*-1);
		
		if(sideCubes_layer[i]) setCubeFaceNeighbor(newCube,sideCubes_layer[i],adjacentFaces[i],orthogonals[i]); // set new cube and side cube adjacent
		else setNewCubeBFacePair(newCube,newFaces[newFaceCounter++],orthogonals[i]); // set a new face to the new cube
		
	}
	for(int i = 0 ; i <4 ; i++) sideFaces[i] = newCube->getFace(orthogonals[i]); // get side Faces (new or old)
	topFace = newCube->getFace(direction); //get topFace
		
	// ADJACENCIES are SET, now SET BOUNDARIES
	
	for(int i = 0 ; i < 4 ; i++) adjacentFaces[i] = boundaryFace->getAdjacent(orthogonals[i]);
	
	//Treat the down direction (in case no adjacent cube)
	for(int i = 0 ; i < 4 ; i++) {
		if(!sideCubes_layer[i] && !sideCubes_below[i]) setFaceFaceAdjacent(sideFaces[i],adjacentFaces[i],direction * - 1,direction);
		else if(!sideCubes_layer[i]) setFaceFaceAdjacent(sideFaces[i],adjacentFaces[i],direction * - 1,orthogonals[i] * -1);
		//else{}	
	}
	
	//Treat the side direction
	for(int i = 0 ; i < 4 ; i++) {
		if(!sideCubes_layer[i]) { //if no adjacentCUBE
			 
			if(!sideCubes_layer[(i+1)%4]) setFaceFaceAdjacent(sideFaces[i],sideFaces[(i+1)%4],orthogonals[(i+1)%4],orthogonals[i]);
			else if(sideCubes_layer[(i+1)%4]) {
				tempFace = adjacentFaces[(i+1)%4]->getAdjacent(orthogonals[i]);
				
				if(cornerCubes_layer[i]) setFaceFaceAdjacent(sideFaces[i],tempFace,orthogonals[(i+1)%4],orthogonals[i] *-1);
				else setFaceFaceAdjacent(sideFaces[i],tempFace,orthogonals[(i+1)%4],orthogonals[(i+1)%4]*-1);
			}
			//else {}
		}
		else if(!sideCubes_layer[(i+1)%4]) {
			tempFace = adjacentFaces[i]->getAdjacent(orthogonals[(i+1)%4]);
			
			if(cornerCubes_layer[i]) setFaceFaceAdjacent(sideFaces[(i+1)%4],tempFace,orthogonals[i],orthogonals[(i+1)%4]*-1);
			else setFaceFaceAdjacent(sideFaces[(i+1)%4],tempFace,orthogonals[i],orthogonals[i]*-1);		
		}
		//else {}
	}
	
	//treat the top direction
	
	for(int i = 0 ; i < 4 ; i++) {
		if(!sideCubes_layer[i]) setFaceFaceAdjacent(topFace,sideFaces[i],orthogonals[i],direction);
		else {
			tempFace = sideFaces[i]->getAdjacent(direction);
			
			if(sideCubes_above[i]) setFaceFaceAdjacent(topFace,tempFace,orthogonals[i],direction * -1);
			else setFaceFaceAdjacent(topFace,tempFace,orthogonals[i],orthogonals[i] * -1);
		}
	}
		
	RemoveFaceBoundary(boundaryFace);
	for(int i = 0 ; i < 4 ; i++) if(sideCubes_layer[i]) RemoveFaceBoundary(sideFaces[i]);
	
} 

#endif
