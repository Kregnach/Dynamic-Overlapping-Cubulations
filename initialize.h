#pragma once
#ifndef INITIALIZE_H
#define INITIALIZE_H

#include "ball.h"

void Ball::Initialize() {

	for (int i = 0; i < AbsMaxCubexId; i++) { availableCubeIds[i] = i ; }
	for (int i = 0; i < AbsMaxFacexId; i++) { availableFaceIds[i] = i ; }
	for (int i = 0; i < AbsMaxFacexId; i++) { availableFaceBIds[i] = i ; }
	
	
	cubeMap.resize(AbsMaxCubexId);
	faceMap.resize(AbsMaxFacexId);
	BoundaryFaces.resize(AbsMaxFacexId);

	nextCubeId = 0;
    nextFaceId = 0;
    nextFaceBId = 0;
	
	
    Cube* cube = createCube();
    cube->setVector(0,0,0);
    
    // CREATE FIRST CUBE //
    
    Vector3 px(1, 0, 0); // create the base vector set
    Vector3 py(0, 1, 0);
    Vector3 pz(0, 0, 1);
    Vector3 mx(-1, 0, 0);
    Vector3 my(0, -1, 0);
    Vector3 mz(0, 0, -1); // create the base vector set
    
    Face* face_px = createFace(); // Create faces
    Face* face_py = createFace();
    Face* face_pz = createFace();
    Face* face_mx = createFace();
    Face* face_my = createFace();
    Face* face_mz = createFace(); // Create faces

    cube->setFace(px,face_px); //add faces to the cube
    cube->setFace(py,face_py);
    cube->setFace(pz,face_pz);
    cube->setFace(mx,face_mx);
    cube->setFace(my,face_my);
    cube->setFace(mz,face_mz); //add faces to the cube
    
    face_px->setCube(px * -1, cube); //add cube to the faces (opposite direction)
    face_py->setCube(py * -1, cube); 
    face_pz->setCube(pz * -1, cube); 
    face_mx->setCube(mx * -1, cube); 
    face_my->setCube(my * -1, cube); 
    face_mz->setCube(mz * -1, cube); //add cube to the faces (opposite direction)
    
    face_px->setVector(px); //set direction of boundary faces
    face_py->setVector(py);
    face_pz->setVector(pz);
    face_mx->setVector(mx);
    face_my->setVector(my);
    face_mz->setVector(mz); //set direction of boundary faces
   
    face_px->setAdjacent(py,face_py); //make faces to be adjacent
    face_px->setAdjacent(my,face_my);
    face_px->setAdjacent(pz,face_pz);
    face_px->setAdjacent(mz,face_mz);
    
    face_py->setAdjacent(px,face_px);
    face_py->setAdjacent(mx,face_mx);
    face_py->setAdjacent(pz,face_pz);
    face_py->setAdjacent(mz,face_mz);
    
    face_pz->setAdjacent(py,face_py);
    face_pz->setAdjacent(my,face_my);
    face_pz->setAdjacent(px,face_px);
    face_pz->setAdjacent(mx,face_mx); //make faces to be adjacent

	face_mx->setAdjacent(my,face_my); //make faces to be adjacent
    face_mx->setAdjacent(py,face_py);
    face_mx->setAdjacent(mz,face_mz);
    face_mx->setAdjacent(pz,face_pz);
    
    face_my->setAdjacent(mx,face_mx);
    face_my->setAdjacent(px,face_px);
    face_my->setAdjacent(mz,face_mz);
    face_my->setAdjacent(pz,face_pz);
    
    face_mz->setAdjacent(my,face_my);
    face_mz->setAdjacent(py,face_py);
    face_mz->setAdjacent(mx,face_mx);
    face_mz->setAdjacent(px,face_px); //make faces to be adjacent
    
     
    // Initialize larger cube if startsize > 1 //
        
    initializeCubicStructure(startsize);
    
    printCubulation();
    
    
    
}
void Ball::initializeCubicStructure(int N) {
    if (N <= 1) return;

    // Assuming cubeMap[0] is already placed as the first cube at the origin.
    for (int z = 0; z < N; z++) {
        for (int y = 0; y < N; y++) {
            for (int x = 0; x < N; x++) {
                if (x == 0 && y == 0 && z == 0) continue; // The first cube is already placed.
				
				printf("(x y z): (%d %d %d) ALL CUBES: %d: \n",x,y,z, nextCubeId);
				

                Vector3 direction;
                // Determine direction based on layer (z), row (y), and column (x).
                if (x == 0) {
                    if (y == 0) {
                        // Start of a new layer (excluding the first layer).
                        if (z > 0) {
                            int baseCubeIndex = (z - 1) * N * N; // Base cube of the previous layer.
                            direction = Vector3(0, 0, 1); // Moving up to start a new layer.

                            growCube(cubeMap[baseCubeIndex]->getFace(direction));
                        }
                    } 
                    else {
                        // Start of a new row (excluding the first row of each layer).
                        int baseCubeIndex = z * N * N + (y - 1) * N; // End cube of the previous row in the current layer.
                       
                        direction = Vector3(0, 1, 0); // Moving forward to start a new row.
                 
                        growCube(cubeMap[baseCubeIndex]->getFace(direction));
                    }
                } 
                else {
                    // Continuing a row.
                    int baseCubeIndex = z * N * N + y * N + (x - 1); // Previous cube in the current row.
                   
                    direction = Vector3(1, 0, 0); // Moving right in the current row.
 
                    growCube(cubeMap[baseCubeIndex]->getFace(direction));
                }
            }
        }
    }
}


	
#endif
