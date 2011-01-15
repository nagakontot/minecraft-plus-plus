#include "Global.h"

Block::Block() {
	type = 0;
	info = 0;
	temperature = 0;
	saturation = 0;
	light = 0;
	extra = 0;
}

Block* GetBlock(int8_t x, int8_t y, int8_t z, Chunk*& chunk) {
	if(chunk==0){return 0;}
	if(x<0){
		chunk = chunk->xn;
		x += 16;
		if(chunk==0){return 0;}
	}
	if(y<0){
		chunk = chunk->yn;
		y += 16;
		if(chunk==0){return 0;}
	}
	if(z<0){
		chunk = chunk->zn;
		z += 16;
		if(chunk==0){return 0;}
	}
	if(x>15){
		chunk = chunk->xp;
		x -= 16;
		if(chunk==0){return 0;}
	}
	if(y>15){
		chunk = chunk->yp;
		y -= 16;
		if(chunk==0){return 0;}
	}
	if(z>15){
		chunk = chunk->zp;
		z -= 16;
		if(chunk==0){return 0;}
	}
	return &(chunk->Blocks[x*256+y*16+z]);
}

bool BlockVisible(int8_t x, int8_t y, int8_t z, Chunk* chunk) {
	Block* b = GetBlock(x, y, z, chunk);
	if(b==0){return false;}
	if(!chunk->generated){return false;}
	return !(BlockTypes[b->type].opaque);
}

void Block::Update(int8_t x, int8_t y, int8_t z, Chunk* chunk) {
	if(!(extra>>7)){
		extra >>= 8;
		extra = 1;
		extra <<= 1;
		extra |= BlockVisible(x+1,y,z,chunk);
		extra <<= 1;
		extra |= BlockVisible(x-1,y,z,chunk);
		extra <<= 1;
		extra |= BlockVisible(x,y+1,z,chunk);
		extra <<= 1;
		extra |= BlockVisible(x,y-1,z,chunk);
		extra <<= 1;
		extra |= BlockVisible(x,y,z+1,chunk);
		extra <<= 1;
		extra |= BlockVisible(x,y,z-1,chunk);
		extra <<= 1;
		extra |= bool(extra<<1);
	}
}

BlockType BlockTypes[100];

BlockType::BlockType() {
	solid = false;
	porosity = 0;
}

BlockType::BlockType(bool _solid, uint8_t _porosity, bool _mineable, bool _opaque, const GLfloat* _model, const GLfloat* _tex, uint16_t _verts) {
	solid = _solid;
	porosity = _porosity;
	mineable = _mineable;
	opaque = _opaque;
	model = _model;
	tex = _tex;
	verts = _verts;
}
// BlockType(Solid, Porosity, Mineable, Opaque, Model (leave 0 for standard block), Texture, Vertices);
void InitBlocks() {
	//Air
	BlockTypes[0] = BlockType(false, 255, false, false, 0, 0, 0);
	//Dirt
	BlockTypes[1] = BlockType(true, 100, true, true, 0, &T_DIRT[0], 24);
	//Stone
	BlockTypes[2] = BlockType(true, 20, true, true, 0, &T_STONE[0], 24);
	//Grass
	BlockTypes[3] = BlockType(true, 100, true, true, 0, &T_GRASS[0], 24);
	//Cobblestone
	BlockTypes[4] = BlockType(true, 0, true, true, 0, &T_COBBLE[0], 24);
	//Leaves
	BlockTypes[5] = BlockType(true, 200, true, false, 0, &T_LEAVES[0], 24);
	//Logs
	BlockTypes[6] = BlockType(true, 50, true, true, 0, &T_LOGS[0], 24);
	//Planks
	BlockTypes[7] = BlockType(true, 100, true, true, 0, &T_PLANKS[0], 24);
	//Gravel
	BlockTypes[8] = BlockType(true, 175, true, true, 0, &T_GRAVEL[0], 24);
	//Sand
	BlockTypes[9] = BlockType(true, 125, true, true, 0, &T_SAND[0], 24);
	//Glass
	BlockTypes[10] = BlockType(true, 0, true, false, 0, &T_GLASS[0], 24);
}
#define BUPDATE(X,Y,Z)	c = chunk;\
						b = GetBlock(x+X,y+Y,z+Z,c);\
						if(b!=0){\
							b->extra = 0;\
							AddChunkUpdate(c);\
						}
void AddBlockUpdate(int8_t x, int8_t y, int8_t z, Chunk* chunk) {
	Block* b;
	Chunk* c;
	BUPDATE(0,0,0);
	BUPDATE(1,0,0);
	BUPDATE(-1,0,0);
	BUPDATE(0,1,0);
	BUPDATE(0,-1,0);
	BUPDATE(0,0,1);
	BUPDATE(0,0,-1);
}