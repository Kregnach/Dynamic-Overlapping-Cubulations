#pragma once
#ifndef OBJECTS_H
#define OBJECTS_H

#include "ball.h"



Cube* Ball::createCube() {
    if (nextCubeId >= AbsMaxCubexId) return nullptr;

    int id = availableCubeIds[nextCubeId];
    Cube* cube = new Cube();
    cube->setId(id);
    cubeMap[id] = cube;
    nextCubeId++; // Move to the next available ID
    
    return cube;
}

void Ball::deleteCube(Cube * cube) {
	int id = cube->getId();
	cubeMap[id] = cubeMap[nextCubeId-1];
	nextCubeId--;
	
	cubeMap[id]->setId(id);
	
	delete cube;
	cube = nullptr;
}


Face* Ball::createFace() {
    if (nextFaceId >= AbsMaxFacexId) return nullptr;

    int id = availableFaceIds[nextFaceId];
    Face* face = new Face();
    face->setId(id);
    faceMap[id] = face;
    nextFaceId++; // Move to the next available ID
    
    AddFaceBoundary(face);
    
    return face;
}

void Ball::deleteFace(Face * face) {
	int id = face->getId();
	//printf("delete ID: %d/%d\n",id,nextFaceId);
	faceMap[id] = faceMap[nextFaceId-1];
	nextFaceId--;
		
	faceMap[id]->setId(id);
	
	delete face;
	face = nullptr;
}



Cube* Ball::getCube(int id) const {return cubeMap[id];}
Face* Ball::getFace(int id) const {return faceMap[id];}
Face* Ball::getBFace(int id) const {return BoundaryFaces[id];}

void Ball::AddFaceBoundary(Face * boundaryFace) {

	BoundaryFaces[nextFaceBId] = boundaryFace;
	boundaryFace->setBId(nextFaceBId);
	
	nextFaceBId++;
}

void Ball::RestoreFaceBoundary(Face * boundaryFace, Vector3 direction) {

	BoundaryFaces[nextFaceBId] = boundaryFace;
	
	//printf("RESTORED BID IS : %d \n",nextFaceBId);
	boundaryFace->setBId(nextFaceBId);
	
	boundaryFace->setVector(direction);
	
	nextFaceBId++;
}



void Ball::RemoveFaceBoundary(Face * boundaryFace) {

	int bId = boundaryFace->getBId(); 

	//printf("REMOVED BID IS : %d next is: %d \n",bId,nextFaceBId - 1);	
	
	BoundaryFaces[bId] = BoundaryFaces[nextFaceBId-1];
	BoundaryFaces[bId]->setBId(bId);
		
	BoundaryFaces[nextFaceBId-1] = nullptr;
	
	nextFaceBId--;
}




	
#endif
