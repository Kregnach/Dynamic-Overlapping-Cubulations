#pragma once
#ifndef BALL_CUBE_H
#define BALL_CUBE_H

#include <unordered_map>
#include <set>
#include "cube.h"



    

class Ball {
private:
    std::vector< Cube* > cubeMap;
    std::vector< Face* > faceMap;
    std::vector< Face* > BoundaryFaces;
    
    int availableCubeIds[AbsMaxCubexId], availableFaceIds[AbsMaxFacexId], availableFaceBIds[AbsMaxFacexId];
    int nextCubeId,nextFaceId,nextFaceBId;
    
    std::vector<std::pair<int, std::string>> growthChain;

	
public:
    Ball() { Initialize(); }
    ~Ball() { }
	
	void Initialize();
	
    Cube* createCube();
    Face* createFace();

    void deleteCube(Cube *cube);
    void deleteFace(Face * face);

    // Optional: Getters for retrieving objects by ID
    Cube* getCube(int id) const;
    Face* getFace(int id) const;
    Face* getBFace(int id) const;
    
    Face * GetBoundaryFace(int bId) { return BoundaryFaces[bId]; }
    
    int getNextCubeId() { return nextCubeId; }
	int getNextFaceId() { return nextFaceId; }
	int getBNextFaceId() { return nextFaceBId; }

	void growCube(Face * BoundaryFace);
	void shrinkCube(Face* boundaryFace);
	
	void printCubulation();

	bool performGrow();	
	bool performShrink();		

	std::vector<int> analyzeGrow(Face* boundaryFace);

	std::pair<int, Face*>  CheckValidGrow(Face* boundaryFace);
	std::pair<int, Face*>  CheckValidShrink(Face* boundaryFace);

	void AddFaceBoundary(Face * face);
	void RestoreFaceBoundary(Face * face, Vector3 direction);
	
	void RemoveFaceBoundary(Face * face);

	void addGrowthStep(int cubeId, const Vector3& dir) { growthChain.push_back({cubeId, dir.getStr()}); }

	void printGrowthChain() {
		std::cout << "Growth Chain:" << std::endl;
		for (const auto& step : growthChain) {
		    std::cout << "Cube ID: " << step.first << " Direction: "
		              << step.second << std::endl;
		}
	}

	void printCubeNeighbors(const char* filename);
	void printBoundaryFaceNeighbors(const char* filename);
	void printCubeDensity(const char* filename);


	void setCubeFaceNeighbor(Cube * cube1, Cube * cube2, Face * face, Vector3 direction);
	void setNewCubeBFacePair(Cube * cube, Face * face, Vector3 direction);
	void setFaceFaceAdjacent(Face * face1, Face * face2, Vector3 direction1,Vector3 direction2);
	void setCubeCubeNeighbor(Cube * cube1, Cube * cube2, Vector3 direction);


	void unsetCubeFaceNeighbor(Cube * cube1, Cube * cube2, Vector3 direction);
	void unsetNewCubeBFacePair(Cube * cube, Face * face, Vector3 direction);
	void unsetFaceFaceAdjacent(Face * face1, Face * face2, Vector3 direction1,Vector3 direction2);
	void unsetCubeCubeNeighbor(Cube * cube1, Cube * cube2, Vector3 direction);

	
	void initializeCubicStructure(int N);
	
	void validateBoundaryFaces();
	void validateBoundaryFaceNeighbors();
	void validateAllCubeFaces();
	void validateCubeNeighbors();
	void validateBoundaryCube();
	void performAllChecks();


	// ACTION : S = alpha * A + lambda * V + epsilon*(V-Vfix)^2	
	// GROW/SHRINK 
	// V--> V+1 || : V--> V-1 
	// A--> A + dNB
	//
	// Sgrow = alpha * (A+dNB) + lambda * (V+1) + epsilon*(V+1-Vfix)^2	
	// Sshrink = alpha * (A+dNB) + lambda * (V-1) + epsilon*(V+1-Vfix)^2	
	//
	// Z = exp(-S) = exp(alpha*A - lambda*V -epsilon(V-Vfix)^2)
	// 
	// 
	// Choose a random boundary with probability: 1/A * factor1 , the boundary changes by dNB (-4 -2 0 2 4)
	// The probability backwards: 1/(A+dNB) * factor2 
	// factor1 and factor2 counts the number of ways a grow/shrink can happen. 
	// e.g. : grow a cube on  flat surface, number of ways: 1 ; but it increases the number of boundaries by 4; there will be 5 ways to shrink
	// The factor is the ratio 
	//
	
	double GetProbN(double dNB) { 
		if(dNB == 4) return 5.0;
		else if(dNB == 2) return 2.;
		else if(dNB == 0) return 1.0;
		else if(dNB == -2) return 0.5;
		else return 0.2;
	}
	
	bool getActionDiffGrow(double dNB) {

		double probA = (double)nextFaceBId/((double)nextFaceBId+dNB);
		double delta_S = -alpha*dNB -(double)lambda + epsilon*(double)(2*(V-nextCubeId)-1);
		double probN = GetProbN(dNB);
		
		double moveprob = probN*probA* std::exp(delta_S);
				
		if(moveprob > 1) return true;
		else if(moveprob > uniform_real()) return true;
			
		return false;
	}
	
	
	bool getActionDiffShrink(double dNB) {
		
		double probA = (double)nextFaceBId/((double)nextFaceBId+dNB);

		double delta_S = -alpha*(double)dNB + (double)lambda + epsilon *(double)(2*(nextCubeId-V)+1);
		
		double probN = GetProbN(dNB);
		
		double moveprob = probN*probA*std::exp(delta_S);

		if(moveprob > 1) return true;
		else if(moveprob > uniform_real()) return true;

		
		return false;
	}
	
	void measure();
	
	void tuneV();
	void tuneA();
	
	void printConfigs();
};






#endif
