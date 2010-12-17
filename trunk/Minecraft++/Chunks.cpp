#include "Global.h"

Chunk::Chunk(int64_t _x, int64_t _y, int64_t _z) {
	x = _x;
	y = _y;
	z = _z;
	xp = 0;
	xn = 0;
	yp = 0;
	yn = 0;
	zp = 0;
	zn = 0;
	//Map generation goes here =D
	for(int i = 0; i < 4096; i++){
		if(z>=0){
			Blocks[i].type = rand()%2+1;
		} else {
			Blocks[i].type = 0;
		}
	}
	Chunk* c;
	c = GetChunk(x-1,y,z,false);
	if(c!=0){c->xp = this; xn = c;}
	c = GetChunk(x+1,y,z,false);
	if(c!=0){c->xn = this; xp = c;}
	c = GetChunk(x,y-1,z,false);
	if(c!=0){c->yp = this; yn = c;}
	c = GetChunk(x,y+1,z,false);
	if(c!=0){c->yn = this; yp = c;}
	c = GetChunk(x,y,z-1,false);
	if(c!=0){c->zp = this; zn = c;}
	c = GetChunk(x,y,z+1,false);
	if(c!=0){c->zn = this; zp = c;}
	Chunks.push_back(this);
}

void Chunk::Draw() {
	glLoadIdentity();
	glTranslated(-16*(player.pos.cx-x), -16*(player.pos.cy-y), -16*(player.pos.cz-z));
	for(int i = 0; i < 4096; i++){
		Blocks[i].Draw(int(double(i)/256), int(double(i)/16)%16, i%16, this);
	}
}

Chunk* GetChunk(int64_t x, int64_t y, int64_t z, bool generate) {
	unsigned int s = Chunks.size();
	for(unsigned int i = 0; i < s; i++){
		Chunk* c = Chunks[i];
		if(c->x==x && c->y==y && c->z==z) {
			return Chunks[i];
		}
	}
	if(generate){
		return new Chunk(x, y, z);
	} else {
		return 0;
	}
}

vector<Chunk*> Chunks;