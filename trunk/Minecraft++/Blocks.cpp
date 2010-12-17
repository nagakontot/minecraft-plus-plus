#include "Global.h"


inline uint16_t NeighborType(int x, int y, int z, Chunk* chunk) {
	if(x<0){
		chunk = chunk->xn;
		x += 16;
	}
	if(y<0){
		chunk = chunk->yn;
		y += 16;
	}
	if(z<0){
		chunk = chunk->zn;
		z += 16;
	}
	if(x>15){
		chunk = chunk->xp;
		x -= 16;
	}
	if(y>15){
		chunk = chunk->yp;
		y -= 16;
	}
	if(z>15){
		chunk = chunk->zp;
		z -= 16;
	}
	if(chunk==0){
		return 0;
	}
	return chunk->Blocks[x*256+y*16+z].type;
}
inline double NeighborOpacity(int x, int y, int z, Chunk* chunk) {
	return BlockTypes[NeighborType(x, y, z, chunk)].opacity;
}

#define BLOCK_TOP glTexCoord2i(0,0);\
		glVertex3i(x,y,z);\
		glTexCoord2i(1,0);\
		glVertex3i(x+1,y,z);\
		glTexCoord2i(1,1);\
		glVertex3i(x+1,y+1,z);\
		glTexCoord2i(0,1);\
		glVertex3i(x,y+1,z);
#define BLOCK_SIDE1 glTexCoord2i(0,0);\
		glVertex3i(x,y,z);\
		glTexCoord2i(1,0);\
		glVertex3i(x,y+1,z);\
		glTexCoord2i(1,1);\
		glVertex3i(x,y+1,z+1);\
		glTexCoord2i(0,1);\
		glVertex3i(x,y,z+1);
#define BLOCK_SIDE2 glTexCoord2i(0,0);\
		glVertex3i(x,y+1,z);\
		glTexCoord2i(1,0);\
		glVertex3i(x+1,y+1,z);\
		glTexCoord2i(1,1);\
		glVertex3i(x+1,y+1,z+1);\
		glTexCoord2i(0,1);\
		glVertex3i(x,y+1,z+1);
#define BLOCK_SIDE3 glTexCoord2i(0,0);\
		glVertex3i(x+1,y+1,z);\
		glTexCoord2i(1,0);\
		glVertex3i(x+1,y,z);\
		glTexCoord2i(1,1);\
		glVertex3i(x+1,y,z+1);\
		glTexCoord2i(0,1);\
		glVertex3i(x+1,y+1,z+1);
#define BLOCK_SIDE4 glTexCoord2i(0,0);\
		glVertex3i(x+1,y,z);\
		glTexCoord2i(1,0);\
		glVertex3i(x,y,z);\
		glTexCoord2i(1,1);\
		glVertex3i(x,y,z+1);\
		glTexCoord2i(0,1);\
		glVertex3i(x+1,y,z+1);
#define BLOCK_BOTTOM glTexCoord2i(0,0);\
		glVertex3i(x,y,z+1);\
		glTexCoord2i(1,0);\
		glVertex3i(x+1,y,z+1);\
		glTexCoord2i(1,1);\
		glVertex3i(x+1,y+1,z+1);\
		glTexCoord2i(0,1);\
		glVertex3i(x,y+1,z+1);
#define VISIBLE(x,y,z)	NeighborOpacity(x,y,z,chunk)<1
#define BLOCK_GENERIC	if(zn){BLOCK_TOP}\
						if(xn){BLOCK_SIDE1}\
						if(yp){BLOCK_SIDE2}\
						if(xp){BLOCK_SIDE3}\
						if(yn){BLOCK_SIDE4}\
						if(zp){BLOCK_BOTTOM}
#define CHECK_VISIBLE	xp = VISIBLE(x+1,y,z);\
						xn = VISIBLE(x-1,y,z);\
						yp = VISIBLE(x,y+1,z);\
						yn = VISIBLE(x,y-1,z);\
						zp = VISIBLE(x,y,z+1);\
						zn = VISIBLE(x,y,z-1);
void Block::Draw(int x, int y, int z, Chunk* chunk) {
	BlockType t = BlockTypes[type];
	bool xp;
	bool xn;
	bool yp;
	bool yn;
	bool zp;
	bool zn;
	switch(type){
	case 0:
		break;
	case 1:
		CHECK_VISIBLE;
		if(!xp && !xn && !yp && !yn && !zp && !zn){break;}
		BindTexture(t.texture[0]);
		glBegin(GL_QUADS);
		BLOCK_GENERIC;
		glEnd();
		break;
	case 2:
		CHECK_VISIBLE;
		if(!xp && !xn && !yp && !yn && !zp && !zn){break;}
		BindTexture(t.texture[0]);
		glBegin(GL_QUADS);
		BLOCK_GENERIC;
		glEnd();
		break;
	}
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