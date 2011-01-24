#pragma once

#include <math.h>
#include <vector>

using namespace std;

class Chunk3D {

public:

	enum direction {E, W, N, S, T, B};

	static const int center_id = 13;

	static vector<Chunk3D*> chunks;

	int id;
	int level;
	float x, y, z;

	Chunk3D* parent;
	Chunk3D* children[27];
	Chunk3D* neighbours[8];

	/// ALONE CHUNK
	Chunk3D(void):id(0),
		parent(0),
		level(0),
		x(0), y(0), z(0) {

			for (int i=0; i< 8; i++) neighbours[i] = 0;
			for (int i=0; i<27; i++) children  [i] = 0;

		chunks.push_back(this);
	}

	~Chunk3D(void) {
		for (int i=0; i< 27; i++) delete [] children[i];
		delete [] &children;
		delete [] &neighbours;

	};

	static Chunk3D* getChunkAt(Chunk3D* chunk, direction dir) {
		
		if(!chunk->parent) return 0;

		int lx = chunk->id % 3, ly = chunk->id / 3 % 3, lz = chunk->id / 9 % 3, nx, ny, nz; 
		nx = dir == E ? lx+1 : dir == W ? lx-1 : lx ;
		ny = dir == N ? ly+1 : dir == S ? ly-1 : ly ;
		nz = dir == T ? lz+1 : dir == B ? lz-1 : lz ;	
	
		int nid;
		/// LOCAL NEIGHBOR
		if (nx>=0 && nx<=2 && ny>=0 && ny<=2 && nz>=0 && nz<=2) {
			nid = nx + ny*3 + nz*9;
			return chunk->parent->children[nid];
		}

		if(!chunk->parent->parent) return 0;

		Chunk3D* parentsNeighbor = Chunk3D::getChunkAt(chunk->parent, dir);

		if(!parentsNeighbor) return 0;
	
		nx = dir == E ? nx-3 : dir == W ? nx+3 : lx ;
		ny = dir == N ? ny-3 : dir == S ? ny+3 : ly ;
		nz = dir == T ? nz-3 : dir == B ? nz+3 : lz ;
	
		if (nx>=0 && nx<=2 && ny>=0 && ny<=2 && nz>=0 && nz<=2) {
			nid = nx + ny*3 + nz*9;
			return parentsNeighbor->children[nid];
		}

		return 0;

	
	};


	static Chunk3D* createChunkAt(Chunk3D* chunk, direction dir) {
		if(!chunk->parent) {
			chunk->parent = new Chunk3D();
			chunk->parent->children[center_id] = chunk;
			chunk->parent->level = chunk->level + 1;
			chunk->parent->x = chunk->x;
			chunk->parent->y = chunk->y;
			chunk->parent->z = chunk->z;
			chunk->id = center_id;
		}
		
		Chunk3D* neighbor = Chunk3D::getChunkAt(chunk, dir);

		if(neighbor) return neighbor;
	
		/// local x y z of chunk
		int lx = chunk->id % 3, ly = chunk->id / 3 % 3, lz = chunk->id / 9 % 3, nx, ny, nz; 
		nx = dir == E ? lx+1 : dir == W ? lx-1 : lx ;
		ny = dir == N ? ly+1 : dir == S ? ly-1 : ly ;
		nz = dir == T ? lz+1 : dir == B ? lz-1 : lz ;
	
		int nid;
		/// LOCAL NEIGHBOR
		if (nx>=0 && nx<=2 && ny>=0 && ny<=2 && nz>=0 && nz<=2) {
			neighbor = new Chunk3D();
			nid = nx + ny*3 + nz*9;
			neighbor->parent = chunk->parent;
			neighbor->id = nid;
			neighbor->level = chunk->level;
			float step = pow(3.0f, neighbor->level);
			neighbor->x = neighbor->parent->x+(nx-1)*step;
			neighbor->y = neighbor->parent->y+(ny-1)*step;
			neighbor->z = neighbor->parent->z+(nz-1)*step;
			chunk->parent->children[nid] = neighbor;
			return neighbor;
		}

		Chunk3D* parentsNeighbor = Chunk3D::createChunkAt(chunk->parent, dir);

		nx = dir == E ? nx-3 : dir == W ? nx+3 : lx ;
		ny = dir == N ? ny-3 : dir == S ? ny+3 : ly ;
		nz = dir == T ? nz-3 : dir == B ? nz+3 : lz ;
	
		if (nx>=0 && nx<=2 && ny>=0 && ny<=2 && nz>=0 && nz<=2) {
			neighbor = new Chunk3D();
			nid = nx + ny*3 + nz*9;
			neighbor->parent = parentsNeighbor;
			neighbor->id = nid;
			neighbor->level = chunk->level;
			float step = pow(3.0f, neighbor->level);
			neighbor->x = neighbor->parent->x+(nx-1)*step;
			neighbor->y = neighbor->parent->y+(ny-1)*step;
			neighbor->z = neighbor->parent->z+(nz-1)*step;
			parentsNeighbor->children[nid] = neighbor;
			return neighbor;
		}

		return 0;

	
	};

};

vector<Chunk3D*> Chunk3D::chunks;

