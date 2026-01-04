#pragma once
#ifndef CUBE_H
#define CUBE_H


#include <functional>
#include <unordered_map>
#include <array>
#include <vector>
#include <string>
#include <cmath> // For std::sqrt
#include <cassert>
struct Vector3 {
    int x, y, z;

	// Constructor for convenience
    Vector3(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}
        
    // Equality comparison
    bool operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z; }
    
    bool operator!=(const Vector3& other) const { return !(x == other.x && y == other.y && z == other.z); }

    // Vector addition
    Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }

    // Vector subtraction
    Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
    
    // Scalar multiplication
    Vector3 operator*(int scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
    
    // Scalar division
    Vector3 operator/(int scalar) const { return Vector3(x / scalar, y / scalar, z / scalar); }

    // Dot product
    int dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }
    
    double det() const { return std::sqrt(x * x + y * y + z * z); }
    
    Vector3 * getVector() { return this; };
    
    
    // For axis-aligned vectors, return the 4 orthogonal directions without allocating.
    std::array<Vector3, 4> getOrthogonal() const {
        // This codebase only calls getOrthogonal() for axis-aligned directions (faces).
        assert((x != 0 && y == 0 && z == 0) || (y != 0 && x == 0 && z == 0) || (z != 0 && x == 0 && y == 0));

        // x-axis (±x): orthogonal are ±y and ±z
        if (x != 0) {
            return {Vector3(0, 1, 0), Vector3(0, 0, 1), Vector3(0, -1, 0), Vector3(0, 0, -1)};
        }
        // y-axis (±y): orthogonal are ±x and ±z
        if (y != 0) {
            return {Vector3(1, 0, 0), Vector3(0, 0, 1), Vector3(-1, 0, 0), Vector3(0, 0, -1)};
        }
        // z-axis (±z): orthogonal are ±x and ±y
        return {Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(-1, 0, 0), Vector3(0, -1, 0)};
    }
    
	 std::vector<Vector3> decompose() const {
        return {
            Vector3(x, 0, 0),  // Component along X
            Vector3(0, y, 0),  // Component along Y
            Vector3(0, 0, z)   // Component along Z
        };
    }

    std::string getStr() const {
        if (x == 1) return "x";
        else if (x == -1) return "-x";
        else if (y == 1) return "y";
        else if (y == -1) return "-y";
        else if (z == 1) return "z";
        else if (z == -1) return "-z";
        else return "0"; // Return "0" for the zero vector or non-axis-aligned vectors
    }
    
	void printCoord() const { printf("Direction: (%d, %d, %d)\n",x,y,z); }
//    void printCoord() { printf("Direction: (x,y,z): (%d, %d, %d)\n",x,y,z); }

	void validateNonZero() const { assert(x != 0 || y != 0 || z != 0);}
	void validateZero() const { assert((x == 0 && y==0 && z == 0)) ;}
	
};

// Custom hash function for Vector3
namespace std { template<> struct hash<Vector3> { size_t operator()(const Vector3& v) const { return hash<int>()(v.x) ^ hash<int>()(v.y) << 1 ^ hash<int>()(v.z) << 2; } }; }


class Cube; // Forward declaration to resolve circular dependency


class Face {
public:
    int id;
    int bId;
    bool isBoundary;
    std::unordered_map<Vector3, Cube*> cubes;
    std::unordered_map<Vector3, Face*> neighbors;
	Vector3 coordinate;
	
    Face( ) { Initialize(); }
    
    void Initialize() {
        id = -1;
        bId = -1;
        coordinate = {0, 0, 0};
        isBoundary = 1;
        // These maps are tiny and fairly stable; reserving reduces rehash churn.
        cubes.reserve(2);
        neighbors.reserve(4);
    }
    
    int getId() { return id;}
    int getBId() { return bId;}
    void setId(int setid) { id = setid;}
    void setBId(int setBId) { bId = setBId;}
    
    void setVector(int x, int y, int z) { coordinate = Vector3(x, y, z); }
	void setVector(Vector3 vector) { coordinate = vector; }
	void unsetVector() { coordinate = {0,0,0}; }
	
	const Vector3& getVector() const { return coordinate ; }
	std::string getVectorStr() const { return coordinate.getStr() ; }

	
	void setCube(const Vector3& direction, Cube* cube) {
        cubes[direction] = cube;
        
        if(cubes.size() == 1) { isBoundary = 1; }
		else {
			coordinate = {0,0,0};
			isBoundary =  0;
		}
        // Automatically determine if this face is a boundary element.
        // If it has only one neighbor, it's a boundary; more than one, it's not.
    }
	
	void printCoordStr() { std::cout << " " << coordinate.getStr() << " "; }// Using std::cout for C++ style output
	std::string getCoordStr() { return coordinate.getStr(); }
    


    void unsetCube(const Vector3& direction) {
        auto it = cubes.find(direction);
        if (it != cubes.end()) {
            cubes.erase(it); // Remove the cube from the map
            
            // Update isBoundary status after removal.
            // This simple example marks the face as a boundary if no neighbors are left.
            
            if(cubes.size() == 1) isBoundary = 1;
            
            //isBoundary = cubes.size() == 1;
        }
     }
     
     int getIsBoundary() {return isBoundary;}
     
     Face *  getAdjacent(const Vector3& direction) { return neighbors[direction]; }

     void setAdjacent(const Vector3& direction, Face * face) { assert(face->getId() != this->getId()) ; neighbors[direction] = face; }
     
     void unsetAdjacent(const Vector3& direction) {
     	auto it = neighbors.find(direction);
        if (it != neighbors.end()) neighbors.erase(it); // Remove the face from the map
     }
     
     void removeBoundary() { neighbors.clear(); }
     
     Cube* getCube(const Vector3& direction) {
        auto it = cubes.find(direction);
        if (it != cubes.end()) return it->second; // Return the found Cube*
        
        return nullptr; // Return nullptr if no Cube* is found
    }
    
    Cube* getCube() {
        // Check if the face is a boundary face with exactly one associated cube
        if (isBoundary && cubes.size() == 1) return cubes.begin()->second; // cubes.begin() returns an iterator to the first element, ->second accesses the Cube* value
        
        // If not a boundary face or no cubes are associated, return nullptr
        return nullptr;
    }
    
    const std::unordered_map<Vector3, Face*>& getNeighbors() const { return neighbors; }

	void printInfo() {
		printf("FaceId: %d ",id);
		printf("IsBoundary: %d ",isBoundary);
		if(isBoundary) printf("%d\n",bId);
		else printf("\n");
		coordinate.printCoord();

		printf("\n");
	}
	
	
    void printNeighbors() {
    	printf("FaceId/BId: [%d,%d]:%s ",id,bId,getCoordStr().c_str()	);
        for (const auto& pair : neighbors) {
            const Vector3& dir = pair.first;
            Face* neighborFace = pair.second;
            //Cube* neighborCube = neighborFace->getCube();
            std::string dirStr = dir.getStr();
            //int neighborCubeId = neighborCube->getId();
            std::string neighborFaceDirStr = neighborFace->getCoordStr();
			printf("[%d,%d]: ",neighborFace->getId(),neighborFace->getBId());
			printf("[%s %s] ",dirStr.c_str(),neighborFaceDirStr.c_str());
			
			assert(neighborFaceDirStr.c_str() != "0");
		}
        std::cout << std::endl;
    }
		
	void checkNeighborDirections() const {
		for (const auto& pair : neighbors) {
		    Face* neighborFace = pair.second;
		    std::string neighborFaceDirStr = neighborFace->getVectorStr();

		    // Using != "0" to compare the string value rather than the C-style string comparison
		    assert(neighborFaceDirStr != "0");
		}
	}
    

};

class Cube {
public:
    int id;
    Vector3 coordinate;
    std::unordered_map<Vector3, Face*> faces;
	std::unordered_map<Vector3, Cube*> neighbors; // Store neighboring cubes
	    
    Cube() { Initialize() ; }
    
	
	void Initialize() {
        id = -1;
        coordinate = {0, 0, 0};
        faces.reserve(6);
        neighbors.reserve(26); // offsets in {-1,0,1}^3 excluding (0,0,0)
    }

	void setVector(int x, int y, int z) { coordinate = Vector3(x, y, z); }
	void setVector(Vector3 vector) { coordinate = vector; }
	
	const Vector3& getVector() const { return coordinate ; }
	
	void setFace(const Vector3& direction, Face* face) { faces[direction] = face; }

    // Method to remove the association of a face with the cube in a specified direction
    void unsetFace(const Vector3& direction) {
        auto it = faces.find(direction);
        if (it != faces.end()) { faces.erase(it); }
    }

    
    int getId() { return id;}
    void setId(int setid) { id = setid;}
    
    Face* getFace(const Vector3& vector) {
    	if (faces.find(vector) != faces.end()) { return faces[vector]; }
        return nullptr;
    }

    void setNeighbor(const Vector3& vector, Cube* neighbor) {
    	assert(!neighbors[vector]);
    	neighbors[vector] = neighbor; 
    }

    Cube* getNeighbor(const Vector3& vector) {
    	if (neighbors.find(vector) != neighbors.end()) { return neighbors[vector]; }
        return nullptr;
    }
    
    void unsetNeighbor(const Vector3& direction) {
        auto it = neighbors.find(direction);
        if (it != neighbors.end()) { neighbors.erase(it); }
    }
    
    const std::unordered_map<Vector3, Cube*>& getNeighbors() const { return neighbors; }
    
    
    Cube * getDiagonalCube(const Vector3& x1, const Vector3& x2) {
    	std::vector<std::vector<Vector3>> permutations = { {x1, x2}, {x1, x2} };
    	
    	for (const auto& perm : permutations) {
		    Cube* current = this; // Start from the current cube
		    bool pathValid = true;
		    
		    for (const auto& dir : perm) {
		        current = current->getNeighbor(dir);
		        if (!current) {
		            pathValid = false; // Path is invalid if any step fails
		            break;
		        }
		    }
		    if (pathValid) { return current; }// Found a valid corner cube
		}

		return nullptr; // No corner cube found
    }
    
	Cube * getCornerCube(const Vector3& x1, const Vector3& x2, const Vector3& x3) {
		// Manually define all six permutations for three items
		std::vector<std::vector<Vector3>> permutations = {
		    {x1, x2, x3}, {x1, x3, x2},
		    {x2, x1, x3}, {x2, x3, x1},
		    {x3, x1, x2}, {x3, x2, x1}
		};

		// Try each permutation to find a corner neighbor
		for (const auto& perm : permutations) {
		    Cube* current = this; // Start from the current cube
		    bool pathValid = true;
		    
		    for (const auto& dir : perm) {
		        current = current->getNeighbor(dir);
		        if (!current) {
		            pathValid = false; // Path is invalid if any step fails
		            break;
		        }
		    }
		    if (pathValid) { return current; }// Found a valid corner cube
		}

		return nullptr; // No corner cube found
	}


    
    Cube * getSecondNeighbor(const Vector3& vector1, const Vector3& vector2) {
    	Cube * cube = this->getNeighbor(vector1);
    	
    	if(cube) return cube->getNeighbor(vector2);
    	
    	return nullptr;
    }
    
    Cube * getThirdNeighbor(const Vector3& vector1, const Vector3& vector2,const Vector3& vector3) {
    	Cube * cube = this->getNeighbor(vector1);
    	
    	if(cube) cube = cube->getNeighbor(vector2);
    	if(cube) return cube->getNeighbor(vector3);
    	
    	return nullptr;
    }
    
    
    void printFaceIDs() const {
        // Define the directions in the order you wish to print
        std::vector<Vector3> directions = {
            Vector3(1, 0, 0),   // x
            Vector3(0, 1, 0),   // y
            Vector3(0, 0, 1),   // z
            Vector3(-1, 0, 0),  // -x
            Vector3(0, -1, 0),  // -y
            Vector3(0, 0, -1)   // -z
        };

        printf("CUBE COORD ID: %d\t",this->id);
        for (const auto& dir : directions) {
            auto it = faces.find(dir);
            if (it != faces.end() && it->second != nullptr) printf("%s: %d, ", dir.getStr().c_str(), it->second->getId());
            else printf("%s: None, ", dir.getStr().c_str());
            
        }
        printf("\n");
    }
    
    void printNeighbors() const {
        printf("Cube ID %d neighbors:\n", id);
        for (const auto& pair : neighbors) {
            const Vector3& dir = pair.first;
            const Cube* neighbor = pair.second;
            if (neighbor) {
                printf("Direction (%d, %d, %d): Neighbor Cube ID %d\n", dir.x, dir.y, dir.z, neighbor->id);
            } else {
                printf("Direction (%d, %d, %d): No Neighbor\n", dir.x, dir.y, dir.z);
            }
        }
    }

};


#endif
