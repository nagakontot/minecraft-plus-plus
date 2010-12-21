#include "Global.h"

Chunk::Chunk(int64_t _x, int64_t _y, int64_t _z) {
	generated = false;
	updated = false;
	//Chunks.push_back(this);
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

int64_t GetSubGen(int64_t x, int64_t y, int64_t l, int64_t r) {
	int64_t h = 0;
	int64_t mx = x%l;
	int64_t my = y%l;
	int64_t s = 8;
	if(x<0){mx=(x+1)%l+l-1;x-=l-1;}
	if(y<0){my=(y+1)%l+l-1;y-=l-1;}
	h += (random(x/l,y/l,r)%(2*s*l)-s*l)*(l-mx)*(l-my);
	h += (random(x/l+1,y/l,r)%(2*s*l)-s*l)*(mx)*(l-my);
	h += (random(x/l+1,y/l+1,r)%(2*s*l)-s*l)*(mx)*(my);
	h += (random(x/l,y/l+1,r)%(2*s*l)-s*l)*(l-mx)*(my);
	h /= l*l*2;
	return h;
}

int64_t GetTotalGen(int64_t x, int64_t y) {
	int64_t h = 0;
	h += GetSubGen(x,y,1,1)+GetSubGen(x,y,1,6);
	h += GetSubGen(x,y,2,2)+GetSubGen(x,y,2,7);
	h += GetSubGen(x,y,4,3)+GetSubGen(x,y,4,8);
	h += GetSubGen(x,y,8,4)+GetSubGen(x,y,8,9);
	h += GetSubGen(x,y,16,5)+GetSubGen(x,y,16,10);
	return h;
}

void Chunk::Generate() {
	lock.lock();
	//Map generation goes here =D
	int64_t h1 = GetTotalGen(x,y);
	int64_t h2 = GetTotalGen(x+1,y);
	int64_t h3 = GetTotalGen(x+1,y+1);
	int64_t h4 = GetTotalGen(x,y+1);
	for(int a=0;a<16;a++){
		for(int b=0;b<16;b++){
			for(int c=0;c<16;c++){
				int i = a*256+b*16+c;
				int64_t h = (h1*(16-a)*(16-b)+h2*a*(16-b)+h3*a*b+h4*(16-a)*b)/256;
				h += (random(a,b,x,y,1)%4+random(a,b,x,y,2)%4+random(a,b,x,y,3)%4)/4;
				if(c+z*16<h){
					Blocks[i].type = 0;
				} else if(c+z*16<h+1){
					Blocks[i].type = 3;
				} else if(c+z*16<h+4){
					Blocks[i].type = 1;
				} else {
					Blocks[i].type = 2;
				}
			}
		}
	}
	//Add updates for all neighboring chunks
	{
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
	}
	generated = true;
	lock.unlock();
}

void Chunk::Update() {
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
	GLfloat *nmodel = new GLfloat[verts*3];
	GLfloat *ntex = new GLfloat[verts*3];
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
						nmodel[l] = t.model[r]+a;
						ntex[l] = t.tex[r];
						nmodel[l+1] = t.model[r+1]+b;
						ntex[l+1] = t.tex[r+1];
						nmodel[l+2] = t.model[r+2]+c;
						ntex[l+2] = t.tex[r+2];
						i++;
					}
				}
			}
		}
	}
	lock.lock();
	delete[] model;
	delete[] tex;
	model = nmodel;
	tex = ntex;
	updated = true;
	lock.unlock();
}

const void Chunk::Draw() {
	if(lock.try_lock()){
		if(generated && updated){
			glLoadIdentity();
			glTranslated(-16*(player.pos.cx-x), -16*(player.pos.cy-y), -16*(player.pos.cz-z));
			glTexCoordPointer(3,GL_FLOAT,0,tex);
			glVertexPointer(3,GL_FLOAT,0,model);
			glDrawArrays(GL_QUADS,0,verts);
		}
		lock.unlock();
	}
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
			ChunksToUpdate.pop_front();
			remove(ChunksToUpdate.begin(),ChunksToUpdate.end(),c);
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
	ChunksToUpdate.push_back(c);
	ChunkUpdate.unlock();
}

deque<Chunk*> ChunksToUpdate;
boost::mutex ChunkUpdate;