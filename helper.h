#pragma once
#ifndef HELPER_H
#define HELPER_H


void Ball::setCubeFaceNeighbor(Cube * cube1, Cube * cube2, Face * face,Vector3 direction) {
		
	cube1->setNeighbor(direction, cube2);
	cube2->setNeighbor(direction * -1, cube1);
	cube1->setFace(direction,face);
	face->setCube(direction * -1, cube1);
	
	//assert(face->getIsBoundary() == 1);
	
}

void Ball::setCubeCubeNeighbor(Cube * cube1, Cube * cube2, Vector3 direction) {
	cube1->setNeighbor(direction, cube2);
	cube2->setNeighbor(direction * -1, cube1);
}

void Ball::setNewCubeBFacePair(Cube * cube, Face * face, Vector3 direction) {
// Set the Cube-Face pairs
	face->setVector(direction); //set the direction of the new face
	cube->setFace(direction, face);  //add the new ace to the new cube
	face->setCube(direction * -1, cube); // add the new cube to the new face
	// glue the new side to the boundary (no neighbor here), rest of the boundaries are done later
	
	//assert(face->getIsBoundary() == 1);

}

void Ball::setFaceFaceAdjacent(Face * face1, Face * face2, Vector3 direction1,Vector3 direction2) {
	face1->setAdjacent(direction1,face2); //new face to ort direction is adjacent to top
    face2->setAdjacent(direction2,face1); //top is adjacent to the new face
}



// WIP HERE
void Ball::unsetNewCubeBFacePair(Cube * cube, Face * face, Vector3 direction) {
// Set the Cube-Face pairs
	face->unsetVector(); //set the direction of the new face
	cube->unsetFace(direction);  //add the new ace to the new cube
	face->unsetCube(direction * -1); // add the new cube to the new face
	// glue the new side to the boundary (no neighbor here), rest of the boundaries are done later

}

void Ball::unsetCubeCubeNeighbor(Cube * cube1, Cube * cube2, Vector3 direction) {
	cube1->unsetNeighbor(direction);
	cube2->unsetNeighbor(direction * -1);
}
void Ball::unsetCubeFaceNeighbor(Cube * cube1, Cube * cube2, Vector3 direction) {

	cube1->unsetNeighbor(direction);
	cube2->unsetNeighbor(direction * -1);
	
	cube1->unsetFace(direction);
	cube2->getFace(direction * -1)->unsetCube(direction * -1);

}
void Ball::unsetFaceFaceAdjacent(Face * face1, Face * face2, Vector3 direction1,Vector3 direction2) {
	face1->unsetAdjacent(direction1); //new face to ort direction is adjacent to top
    face2->unsetAdjacent(direction2); //top is adjacent to the new face
}

#endif
