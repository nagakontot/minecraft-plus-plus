#include "Global.h"
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
#define BLOCK_GENERIC BLOCK_TOP BLOCK_SIDE1 BLOCK_SIDE2 BLOCK_SIDE3 BLOCK_SIDE4 BLOCK_BOTTOM

void Block::Draw(int x, int y, int z) {
	BlockType t = BlockTypes[type];
	switch(type){
	case 0:
		break;
	case 1:
		glBindTexture(GL_TEXTURE_2D,t.texture[0]);
		glBegin(GL_QUADS);
		BLOCK_GENERIC
		glEnd();
		break;
	case 2:
		glBindTexture(GL_TEXTURE_2D,t.texture[0]);
		glBegin(GL_QUADS);
		BLOCK_GENERIC
		glEnd();
		break;
	}
}

vector<BlockType> BlockTypes;

BlockType::BlockType() {
	solid = false;
	porosity = 0;
}

BlockType::BlockType(bool _solid, uint8_t _porosity, bool _mineable, uint8_t _opacity, GLuint* _texture) {
	solid = _solid;
	porosity = _porosity;
	mineable = _mineable;
	opacity = _opacity;
	texture = _texture;
}

void InitBlocks() {
	GLuint* tex;
	//Air
	BlockTypes.push_back(BlockType(true, 255, false, 0, 0));
	//Dirt
	tex = new GLuint;
	*tex = GetTexture("textures/dirt.png");
	BlockTypes.push_back(BlockType(true, 100, true, 255, tex));
	//Stone
	tex = new GLuint;
	*tex = GetTexture("textures/stone.png");
	BlockTypes.push_back(BlockType(true, 20, true, 255, tex));
}