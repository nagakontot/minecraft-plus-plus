#include "Global.h"

Block* GetBlock(int x, int y, int z, Chunk*& chunk) {
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

#define BLOCK_ZN glTexCoord2i(0,0);\
		glVertex3i(x,y,z);\
		glTexCoord2i(1,0);\
		glVertex3i(x+1,y,z);\
		glTexCoord2i(1,1);\
		glVertex3i(x+1,y+1,z);\
		glTexCoord2i(0,1);\
		glVertex3i(x,y+1,z);
#define BLOCK_XN glTexCoord2i(0,0);\
		glVertex3i(x,y,z);\
		glTexCoord2i(1,0);\
		glVertex3i(x,y+1,z);\
		glTexCoord2i(1,1);\
		glVertex3i(x,y+1,z+1);\
		glTexCoord2i(0,1);\
		glVertex3i(x,y,z+1);
#define BLOCK_YP glTexCoord2i(0,0);\
		glVertex3i(x,y+1,z);\
		glTexCoord2i(1,0);\
		glVertex3i(x+1,y+1,z);\
		glTexCoord2i(1,1);\
		glVertex3i(x+1,y+1,z+1);\
		glTexCoord2i(0,1);\
		glVertex3i(x,y+1,z+1);
#define BLOCK_XP glTexCoord2i(0,0);\
		glVertex3i(x+1,y+1,z);\
		glTexCoord2i(1,0);\
		glVertex3i(x+1,y,z);\
		glTexCoord2i(1,1);\
		glVertex3i(x+1,y,z+1);\
		glTexCoord2i(0,1);\
		glVertex3i(x+1,y+1,z+1);
#define BLOCK_YN glTexCoord2i(0,0);\
		glVertex3i(x+1,y,z);\
		glTexCoord2i(1,0);\
		glVertex3i(x,y,z);\
		glTexCoord2i(1,1);\
		glVertex3i(x,y,z+1);\
		glTexCoord2i(0,1);\
		glVertex3i(x+1,y,z+1);
#define BLOCK_ZP glTexCoord2i(0,0);\
		glVertex3i(x,y,z+1);\
		glTexCoord2i(1,0);\
		glVertex3i(x+1,y,z+1);\
		glTexCoord2i(1,1);\
		glVertex3i(x+1,y+1,z+1);\
		glTexCoord2i(0,1);\
		glVertex3i(x,y+1,z+1);

#define BLOCK_GENERIC	if(extra&1){BLOCK_XP}\
						if(extra&2){BLOCK_XN}\
						if(extra&4){BLOCK_YP}\
						if(extra&8){BLOCK_YN}\
						if(extra&16){BLOCK_ZP}\
						if(extra&32){BLOCK_ZN}

void Block::Draw(int x, int y, int z, Chunk* chunk) {
	BlockType t = BlockTypes[type];
	switch(type){
	case 0:
		break;
	case 1:
		if(!(extra<<2>>2)){break;}
		BindTexture(t.texture[0]);
		glBegin(GL_QUADS);
		BLOCK_GENERIC;
		glEnd();
		break;
	case 2:
		if(!(extra<<2>>2)){break;}
		BindTexture(t.texture[0]);
		glBegin(GL_QUADS);
		BLOCK_GENERIC;
		glEnd();
		break;
	}
}

bool BlockVisible(int x, int y, int z, Chunk* chunk) {
	Block* b = GetBlock(x, y, z, chunk);
	if(b==0){return true;}
	return BlockTypes[b->type].opacity<1;
}

void Block::Update(int x, int y, int z, Chunk* chunk) {
	extra = extra>>6<<6;
	extra |= uint8_t(BlockVisible(x+1,y,z,chunk))<<0;
	extra |= uint8_t(BlockVisible(x-1,y,z,chunk))<<1;
	extra |= uint8_t(BlockVisible(x,y+1,z,chunk))<<2;
	extra |= uint8_t(BlockVisible(x,y-1,z,chunk))<<3;
	extra |= uint8_t(BlockVisible(x,y,z+1,chunk))<<4;
	extra |= uint8_t(BlockVisible(x,y,z-1,chunk))<<5;
}

BlockType BlockTypes[100];

BlockType::BlockType() {
	solid = false;
	porosity = 0;
}

BlockType::BlockType(bool _solid, uint8_t _porosity, bool _mineable, double _opacity, GLuint* _texture) {
	solid = _solid;
	porosity = _porosity;
	mineable = _mineable;
	opacity = _opacity;
	texture = _texture;
}

void InitBlocks() {
	GLuint* tex;
	//Air
	BlockTypes[0] = BlockType(true, 255, false, 0, 0);
	//Dirt
	tex = new GLuint;
	*tex = GetTexture("textures/dirt.png");
	BlockTypes[1] = BlockType(true, 100, true, 1, tex);
	//Stone
	tex = new GLuint;
	*tex = GetTexture("textures/stone.png");
	BlockTypes[2] = BlockType(true, 20, true, 1, tex);
}