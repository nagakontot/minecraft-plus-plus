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

const void Block::Draw(int x, int y, int z) {
	if(!extra&1){return;}
	const BlockType& t = BlockTypes[type];
	if(t.verts==0){return;}
	glPushMatrix();
	glTranslated(x,y,z);
	glTexCoordPointer(3,GL_FLOAT,0,t.tex);
	glVertexPointer(3,GL_FLOAT,0,t.model);
	glDrawArrays(GL_QUADS,0,t.verts);
	glPopMatrix();
}

bool BlockVisible(int x, int y, int z, Chunk* chunk) {
	Block* b = GetBlock(x, y, z, chunk);
	if(b==0){return false;}
	return BlockTypes[b->type].opacity<1;
}

void Block::Update(int x, int y, int z, Chunk* chunk) {
	extra = extra>>1<<1;
	extra |= BlockVisible(x+1,y,z,chunk);
	extra |= BlockVisible(x-1,y,z,chunk);
	extra |= BlockVisible(x,y+1,z,chunk);
	extra |= BlockVisible(x,y-1,z,chunk);
	extra |= BlockVisible(x,y,z+1,chunk);
	extra |= BlockVisible(x,y,z-1,chunk);
}

BlockType BlockTypes[100];

BlockType::BlockType() {
	solid = false;
	porosity = 0;
}

BlockType::BlockType(bool _solid, uint8_t _porosity, bool _mineable, double _opacity, const GLfloat* _model, const GLfloat* _tex, uint16_t _verts) {
	solid = _solid;
	porosity = _porosity;
	mineable = _mineable;
	opacity = _opacity;
	model = _model;
	tex = _tex;
	verts = _verts;
}
// BlockType(Visible, Porosity, Mineable, Opacity, Model, Texture, Vertices);
void InitBlocks() {
	//Air
	BlockTypes[0] = BlockType(true, 255, false, 0, 0, 0, 0);
	//Dirt
	BlockTypes[1] = BlockType(true, 100, true, 1, &M_BLOCK[0], &T_DIRT[0], 24);
	//Stone
	BlockTypes[2] = BlockType(true, 20, true, 1, &M_BLOCK[0], &T_STONE[0], 24);
	//Grass
	BlockTypes[3] = BlockType(true, 100, true, 1, &M_BLOCK[0], &T_GRASS[0], 24);
	//Cobblestone
	BlockTypes[4] = BlockType(true, 0, true, 1, &M_BLOCK[0], &T_COBBLE[0], 24);
	//Leaves
	BlockTypes[5] = BlockType(true, 200, false, 1, &M_BLOCK[0], &T_LEAVES[0], 24);
	//Logs
	BlockTypes[6] = BlockType(true, 50, true, 1, &M_BLOCK[0], &T_LOGS[0], 24);
	//Planks
	BlockTypes[7] = BlockType(true, 100, true, 1, &M_BLOCK[0], &T_PLANKS[0], 24);
	//Gravel
	BlockTypes[8] = BlockType(true, 175, true, 1, &M_BLOCK[0], &T_GRAVEL[0], 24);
	//Sand
	BlockTypes[9] = BlockType(true, 125, true, 1, &M_BLOCK[0], &T_SAND[0], 24);
	//Glass
	BlockTypes[10] = BlockType(true, 0, false, 1, &M_BLOCK[0], &T_GLASS[0], 24);
}