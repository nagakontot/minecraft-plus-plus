#include "Global.h"

Chunk::Chunk(int64_t _x, int64_t _y, int64_t _z) {
	x = _x;
	y = _y;
	z = _z;
	//Map generation goes here =D
	for(int i = 0; i < 4096; i++){
		if(rand()%4==0) {
			Blocks[i].type = 1;
		} else {
			Blocks[i].type = 0;
		}
	}
}

Chunk* GetChunk(int64_t x, int64_t y, int64_t z) {
	for(unsigned int i = 0; i < Chunks.size(); i++){
		if(Chunks[i]->x==x && Chunks[i]->y==y && Chunks[i]->z==z) {
			return Chunks[i];
		}
	}
	return new Chunk(x, y, z);
}

vector<Chunk*> Chunks;