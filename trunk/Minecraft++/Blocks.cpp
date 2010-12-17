#include "Global.h"

Block::Block() {
	type = 0;
	info = 0;
	temperature = 0;
	saturation = 0;
	light = 0;
	extra = 0;
}

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

#define BLOCK_GENERIC	if(extra&1){glVertexPointer(3,GL_SHORT,0,M_XP);glDrawArrays(GL_QUADS,0,4);}\
						if(extra&2){glVertexPointer(3,GL_SHORT,0,M_XN);glDrawArrays(GL_QUADS,0,4);}\
						if(extra&4){glVertexPointer(3,GL_SHORT,0,M_YP);glDrawArrays(GL_QUADS,0,4);}\
						if(extra&8){glVertexPointer(3,GL_SHORT,0,M_YN);glDrawArrays(GL_QUADS,0,4);}\
						if(extra&16){glVertexPointer(3,GL_SHORT,0,M_ZP);glDrawArrays(GL_QUADS,0,4);}\
						if(extra&32){glVertexPointer(3,GL_SHORT,0,M_ZN);glDrawArrays(GL_QUADS,0,4);}

void Block::Draw(int x, int y, int z, Chunk* chunk) {
	if(!(extra<<2>>2)){
		return;
	}
	const BlockType& t = BlockTypes[type];
	glPushMatrix();
	glTranslated(x,y,z);
	switch(type){
	case 0:
		break;
	case 1:
		BindTexture(t.texture[0]);
		glTexCoordPointer(2,GL_SHORT,0,T_QUAD);
		BLOCK_GENERIC;
		break;
	case 2:
		BindTexture(t.texture[0]);
		glTexCoordPointer(2,GL_SHORT,0,T_QUAD);
		BLOCK_GENERIC;
		break;
	case 3:
		glTexCoordPointer(2,GL_SHORT,0,T_QUAD);
		if(extra&32){
			BindTexture(t.texture[0]);
			glVertexPointer(3,GL_SHORT,0,M_ZN);glDrawArrays(GL_QUADS,0,4);
		}
		BindTexture(t.texture[1]);
		if(extra&1){glVertexPointer(3,GL_SHORT,0,M_XP);glDrawArrays(GL_QUADS,0,4);}
		if(extra&2){glVertexPointer(3,GL_SHORT,0,M_XN);glDrawArrays(GL_QUADS,0,4);}
		if(extra&4){glVertexPointer(3,GL_SHORT,0,M_YP);glDrawArrays(GL_QUADS,0,4);}
		if(extra&8){glVertexPointer(3,GL_SHORT,0,M_YN);glDrawArrays(GL_QUADS,0,4);}
		if(extra&16){glVertexPointer(3,GL_SHORT,0,M_ZP);glDrawArrays(GL_QUADS,0,4);}
		break;
	}
	glPopMatrix();
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
	//Grass
	tex = new GLuint[2];
	tex[0] = GetTexture("textures/grass.png");
	tex[1] = GetTexture("textures/grass side.png");
	BlockTypes[3] = BlockType(true, 100, true, 1, tex);
}