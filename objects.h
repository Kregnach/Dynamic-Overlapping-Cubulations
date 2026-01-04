#pragma once
#ifndef OBJECTS_H
#define OBJECTS_H

#include "ball.h"



Cube* Ball::createCube() {
    if (nextCubeId >= AbsMaxCubexId) return nullptr;

    int id = availableCubeIds[nextCubeId];
    Cube* cube = nullptr;
    if (!freeCubes.empty()) {
        cube = freeCubes.back();
        freeCubes.pop_back();
        cube->Initialize();
    } else {
        cube = new Cube();
        allocatedCubes.push_back(cube);
    }
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

    // Clear the old last slot and recycle the allocation.
    cubeMap[nextCubeId] = nullptr;
    freeCubes.push_back(cube);
}


Face* Ball::createFace() {
    if (nextFaceId >= AbsMaxFacexId) return nullptr;

    int id = availableFaceIds[nextFaceId];
    Face* face = nullptr;
    if (!freeFaces.empty()) {
        face = freeFaces.back();
        freeFaces.pop_back();
        face->Initialize();
    } else {
        face = new Face();
        allocatedFaces.push_back(face);
    }
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

    faceMap[nextFaceId] = nullptr;
    freeFaces.push_back(face);
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
