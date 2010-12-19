#include "Global.h"

Chunk::Chunk(int64_t _x, int64_t _y, int64_t _z) {
	generated = false;
	updated = false;
	Chunks.push_back(this);
	ChunkPos[_x][_y][_z] = this;
	x = _x;
	y = _y;
	z = _z;
	xp = 0;
	xn = 0;
	yp = 0;
	yn = 0;
	zp = 0;
	zn = 0;
	model = 0;
	tex = 0;
	verts = 0;
	AddChunkUpdate(this);
}

void Chunk::Generate() {
	lock.lock();
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
	//Add updates for all neighboring chunks
	Chunk* c;
	c = GetChunk(x-1,y,z,false);
	if(c!=0){
		c->xp = this;
		xn = c;
		AddChunkUpdate(c);
	}
	c = GetChunk(x+1,y,z,false);
	if(c!=0){
		c->xn = this;
		xp = c;
		AddChunkUpdate(c);
	}
	c = GetChunk(x,y-1,z,false);
	if(c!=0){
		c->yp = this;
		yn = c;
		AddChunkUpdate(c);
	}
	c = GetChunk(x,y+1,z,false);
	if(c!=0){
		c->yn = this;
		yp = c;
		AddChunkUpdate(c);
	}
	c = GetChunk(x,y,z-1,false);
	if(c!=0){
		c->zp = this;
		zn = c;
		AddChunkUpdate(c);
	}
	c = GetChunk(x,y,z+1,false);
	if(c!=0){
		c->zn = this;
		zp = c;
		AddChunkUpdate(c);
	}
	generated = true;
	lock.unlock();
}

void Chunk::Update() {
	lock.lock();
	delete[] model;
	delete[] tex;
	verts = 0;
	for(int a=0;a<16;a++){
		for(int b=0;b<16;b++){
			for(int c=0;c<16;c++){
				Block* bl = &Blocks[a*256+b*16+c];
				bl->Update(a,b,c,this);
				if(bl->extra&1){
					verts += BlockTypes[bl->type].verts;
				}
			}
		}
	}
	model = new GLfloat[verts*3];
	tex = new GLfloat[verts*3];
	int i = 0;
	for(int a=0;a<16;a++){
		for(int b=0;b<16;b++){
			for(int c=0;c<16;c++){
				Block* bl = &Blocks[a*256+b*16+c];
				BlockType t = BlockTypes[bl->type];
				if(bl->extra&1){
					for(int j=0; j<t.verts; j++){
						int l = 3*i;
						int r = 3*j;
						model[l] = t.model[r]+a;
						tex[l] = t.tex[r];
						model[l+1] = t.model[r+1]+b;
						tex[l+1] = t.tex[r+1];
						model[l+2] = t.model[r+2]+c;
						tex[l+2] = t.tex[r+2];
						i++;
					}
				}
			}
		}
	}
	updated = true;
	lock.unlock();
}

const void Chunk::Draw() {
	if(!lock.try_lock()){
		return;
	}
	if(generated && updated){
		glLoadIdentity();
		glTranslated(-16*(player.pos.cx-x), -16*(player.pos.cy-y), -16*(player.pos.cz-z));
		glTexCoordPointer(3,GL_FLOAT,0,tex);
		glVertexPointer(3,GL_FLOAT,0,model);
		glDrawArrays(GL_QUADS,0,verts);
	}
	lock.unlock();
}

Chunk* GetChunk(int64_t x, int64_t y, int64_t z, bool generate) {
	Chunk* c = ChunkPos[x][y][z];
	if(c!=0){
		return c;
	}
	if(generate){
		return new Chunk(x, y, z);
	}
	return 0;
}

vector<Chunk*> Chunks;
map<int64_t,map<int64_t,map<int64_t,Chunk*>>> ChunkPos;

void ChunkUpdateThread() {
	while(true){
		if(ChunksToUpdate.empty()){
			sf::Sleep(0.1);
		} else {
			ChunkUpdate.lock();
			Chunk* c = ChunksToUpdate.front();
			ChunksToUpdate.pop();
			ChunkUpdate.unlock();
			if(!c->generated){
				c->Generate();
			}
			c->Update();
		}
	}
}

void AddChunkUpdate(Chunk* c) {
	ChunkUpdate.lock();
	ChunksToUpdate.push(c);
	ChunkUpdate.unlock();
}

queue<Chunk*> ChunksToUpdate;
boost::mutex ChunkUpdate;