#pragma once
#ifndef CUBE_H
#define CUBE_H

#include <array>
#include <string>
#include <cstdio>
#include <iostream>
#include <cmath> // For std::sqrt
#include <cassert>
struct Vector3 {
    int x, y, z;

	// Constructor for convenience
    constexpr Vector3(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}
        
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
    
    static inline bool isAxisAligned(const Vector3& v) {
        const int nonZero = (v.x != 0) + (v.y != 0) + (v.z != 0);
        return nonZero == 1 && ((v.x == 1 || v.x == -1) || (v.y == 1 || v.y == -1) || (v.z == 1 || v.z == -1));
    }

    static inline int axisIndex(const Vector3& v) {
        // +x,+y,+z,-x,-y,-z
        if (v.x == 1 && v.y == 0 && v.z == 0) return 0;
        if (v.x == 0 && v.y == 1 && v.z == 0) return 1;
        if (v.x == 0 && v.y == 0 && v.z == 1) return 2;
        if (v.x == -1 && v.y == 0 && v.z == 0) return 3;
        if (v.x == 0 && v.y == -1 && v.z == 0) return 4;
        if (v.x == 0 && v.y == 0 && v.z == -1) return 5;
        assert(false && "Expected axis-aligned unit vector");
        return 0;
    }

    static inline Vector3 axisFromIndex(int idx) {
        switch (idx) {
            case 0: return Vector3(1, 0, 0);
            case 1: return Vector3(0, 1, 0);
            case 2: return Vector3(0, 0, 1);
            case 3: return Vector3(-1, 0, 0);
            case 4: return Vector3(0, -1, 0);
            case 5: return Vector3(0, 0, -1);
            default: assert(false && "axisFromIndex out of range"); return Vector3(0, 0, 0);
        }
    }

    static inline int neighborIndex(const Vector3& v) {
        // v components must be in [-1, 1] and v != (0,0,0)
        assert(v.x >= -1 && v.x <= 1);
        assert(v.y >= -1 && v.y <= 1);
        assert(v.z >= -1 && v.z <= 1);
        assert(!(v.x == 0 && v.y == 0 && v.z == 0));
        return (v.x + 1) * 9 + (v.y + 1) * 3 + (v.z + 1);
    }

    static inline Vector3 neighborFromIndex(int idx) {
        assert(idx >= 0 && idx < 27);
        const int dx = (idx / 9) - 1;
        const int dy = ((idx % 9) / 3) - 1;
        const int dz = (idx % 3) - 1;
        return Vector3(dx, dy, dz);
    }
    
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

class Cube; // Forward declaration to resolve circular dependency


class Face {
public:
    int id;
    int bId;
    bool isBoundary;
    std::array<Cube*, 6> cubes{};
    std::array<Face*, 6> neighbors{};
    int cubeCount;
	Vector3 coordinate;
	
    Face( ) { Initialize(); }
    
    void Initialize() {
        id = -1;
        bId = -1;
        coordinate = {0, 0, 0};
        isBoundary = 1;
        cubes.fill(nullptr);
        neighbors.fill(nullptr);
        cubeCount = 0;
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
        const int idx = Vector3::axisIndex(direction);
        if (!cubes[idx]) cubeCount++;
        cubes[idx] = cube;

        if (cubeCount == 1) {
            isBoundary = 1;
        } else {
            coordinate = {0, 0, 0};
            isBoundary = 0;
        }
        // Automatically determine if this face is a boundary element.
        // If it has only one neighbor, it's a boundary; more than one, it's not.
    }
	
	void printCoordStr() { std::cout << " " << coordinate.getStr() << " "; }// Using std::cout for C++ style output
	std::string getCoordStr() { return coordinate.getStr(); }
    


    void unsetCube(const Vector3& direction) {
        const int idx = Vector3::axisIndex(direction);
        if (cubes[idx]) {
            cubes[idx] = nullptr;
            cubeCount--;
            if (cubeCount == 1) isBoundary = 1;
        }
     }
     
     int getIsBoundary() {return isBoundary;}
     
     Face *  getAdjacent(const Vector3& direction) { return neighbors[Vector3::axisIndex(direction)]; }

     void setAdjacent(const Vector3& direction, Face * face) {
        assert(face);
        assert(face->getId() != this->getId());
        neighbors[Vector3::axisIndex(direction)] = face;
     }
     
     void unsetAdjacent(const Vector3& direction) {
     	neighbors[Vector3::axisIndex(direction)] = nullptr;
     }
     
     void removeBoundary() { neighbors.fill(nullptr); }
     
     Cube* getCube(const Vector3& direction) {
        return cubes[Vector3::axisIndex(direction)];
    }
    
    Cube* getCube() {
        // Check if the face is a boundary face with exactly one associated cube
        if (isBoundary && cubeCount == 1) {
            for (Cube* c : cubes) if (c) return c;
        }
        
        // If not a boundary face or no cubes are associated, return nullptr
        return nullptr;
    }

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
        for (int i = 0; i < 6; i++) {
            Face* neighborFace = neighbors[i];
            if (!neighborFace) continue;
            const Vector3 dir = Vector3::axisFromIndex(i);
            const std::string dirStr = dir.getStr();
            const std::string neighborFaceDirStr = neighborFace->getCoordStr();
            printf("[%d,%d]: ", neighborFace->getId(), neighborFace->getBId());
            printf("[%s %s] ", dirStr.c_str(), neighborFaceDirStr.c_str());
            assert(neighborFaceDirStr != "0");
        }
        std::cout << std::endl;
    }
		
	void checkNeighborDirections() const {
        for (Face* neighborFace : neighbors) {
            if (!neighborFace) continue;
            const std::string neighborFaceDirStr = neighborFace->getVectorStr();
            assert(neighborFaceDirStr != "0");
        }
	}
    

};

class Cube {
public:
    int id;
    Vector3 coordinate;
    std::array<Face*, 6> faces{};
	std::array<Cube*, 27> neighbors{}; // offsets in {-1,0,1}^3 including diagonals
	    
    Cube() { Initialize() ; }
    
	
	void Initialize() {
        id = -1;
        coordinate = {0, 0, 0};
        faces.fill(nullptr);
        neighbors.fill(nullptr);
    }

	void setVector(int x, int y, int z) { coordinate = Vector3(x, y, z); }
	void setVector(Vector3 vector) { coordinate = vector; }
	
	const Vector3& getVector() const { return coordinate ; }
	
	void setFace(const Vector3& direction, Face* face) { faces[Vector3::axisIndex(direction)] = face; }

    // Method to remove the association of a face with the cube in a specified direction
    void unsetFace(const Vector3& direction) {
        faces[Vector3::axisIndex(direction)] = nullptr;
    }

    
    int getId() { return id;}
    void setId(int setid) { id = setid;}
    
    Face* getFace(const Vector3& vector) {
        return faces[Vector3::axisIndex(vector)];
    }

    void setNeighbor(const Vector3& vector, Cube* neighbor) {
        const int idx = Vector3::neighborIndex(vector);
    	assert(!neighbors[idx]);
    	neighbors[idx] = neighbor;
    }

    Cube* getNeighbor(const Vector3& vector) {
        const int idx = Vector3::neighborIndex(vector);
        return neighbors[idx];
    }
    
    void unsetNeighbor(const Vector3& direction) {
        neighbors[Vector3::neighborIndex(direction)] = nullptr;
    }
    
    
    Cube * getDiagonalCube(const Vector3& x1, const Vector3& x2) {
        const std::array<std::array<Vector3, 2>, 2> perms = {{
            {x1, x2},
            {x2, x1},
        }};
        for (const auto& perm : perms) {
            Cube* current = this;
            current = current->getNeighbor(perm[0]);
            if (!current) continue;
            current = current->getNeighbor(perm[1]);
            if (current) return current;
        }
        return nullptr;
    }
    
	Cube * getCornerCube(const Vector3& x1, const Vector3& x2, const Vector3& x3) {
        const std::array<std::array<Vector3, 3>, 6> perms = {{
            {x1, x2, x3}, {x1, x3, x2},
            {x2, x1, x3}, {x2, x3, x1},
            {x3, x1, x2}, {x3, x2, x1},
        }};

        for (const auto& perm : perms) {
            Cube* current = this;
            current = current->getNeighbor(perm[0]);
            if (!current) continue;
            current = current->getNeighbor(perm[1]);
            if (!current) continue;
            current = current->getNeighbor(perm[2]);
            if (current) return current;
        }
        return nullptr;
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
        const std::array<Vector3, 6> directions = {
            Vector3(1, 0, 0),   // x
            Vector3(0, 1, 0),   // y
            Vector3(0, 0, 1),   // z
            Vector3(-1, 0, 0),  // -x
            Vector3(0, -1, 0),  // -y
            Vector3(0, 0, -1)   // -z
        };

        printf("CUBE COORD ID: %d\t",this->id);
        for (const auto& dir : directions) {
            Face* f = faces[Vector3::axisIndex(dir)];
            if (f) printf("%s: %d, ", dir.getStr().c_str(), f->getId());
            else printf("%s: None, ", dir.getStr().c_str());
            
        }
        printf("\n");
    }
    
    void printNeighbors() const {
        printf("Cube ID %d neighbors:\n", id);
        for (int idx = 0; idx < 27; idx++) {
            if (idx == 13) continue; // (0,0,0)
            const Cube* neighbor = neighbors[idx];
            if (!neighbor) continue;
            const Vector3 dir = Vector3::neighborFromIndex(idx);
            printf("Direction (%d, %d, %d): Neighbor Cube ID %d\n", dir.x, dir.y, dir.z, neighbor->id);
        }
    }

};


#endif
