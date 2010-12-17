#include "Global.h"

Chunk::Chunk(int64_t _x, int64_t _y, int64_t _z) {
	Chunks.push_back(this);
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
	int c1 = random(x,y)%32-16;
	int c2 = random(x+1,y)%32-16;
	int c3 = random(x,y+1)%32-16;
	int c4 = random(x+1,y+1)%32-16;
	for(int a=0;a<16;a++){
		for(int b=0;b<16;b++){
			int h = (c1*(16-a)*(16-b)+c2*a*(16-b)+c3*(16-a)*b+c4*a*b)/512;
			for(int c=0;c<16;c++){
				int i = a*256+b*16+c;
				int hh = h+random(a,b)%2;
				if(c+z*16<hh){
					Blocks[i].type = 0;
				} else if(c+z*16<hh+1){
					Blocks[i].type = 3;
				} else if(c+z*16<hh+4){
					Blocks[i].type = 1;
				} else {
					Blocks[i].type = 2;
				}
			}
		}
	}
	Chunk* c;
	c = GetChunk(x-1,y,z,false);
	if(c!=0){
		c->xp = this;
		xn = c;
		c->Update();
	}
	c = GetChunk(x+1,y,z,false);
	if(c!=0){
		c->xn = this;
		xp = c;
		c->Update();
	}
	c = GetChunk(x,y-1,z,false);
	if(c!=0){
		c->yp = this;
		yn = c;
		c->Update();
	}
	c = GetChunk(x,y+1,z,false);
	if(c!=0){
		c->yn = this;
		yp = c;
		c->Update();
	}
	c = GetChunk(x,y,z-1,false);
	if(c!=0){
		c->zp = this;
		zn = c;
		c->Update();
	}
	c = GetChunk(x,y,z+1,false);
	if(c!=0){
		c->zn = this;
		zp = c;
		c->Update();
	}
	Update();
}

void Chunk::Update() {
	for(int a=0;a<16;a++){
		for(int b=0;b<16;b++){
			for(int c=0;c<16;c++){
				Blocks[a*256+b*16+c].Update(a,b,c,this);
			}
		}
	}
}

void Chunk::Draw() {
	glLoadIdentity();
	glTranslated(-16*(player.pos.cx-x), -16*(player.pos.cy-y), -16*(player.pos.cz-z));
	//glPushMatrix();
	for(short i = 0; i < 4096; i++){
		Blocks[i].Draw(i/256, (i/16)%16, i%16, this);
	}
	//glPopMatrix();
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