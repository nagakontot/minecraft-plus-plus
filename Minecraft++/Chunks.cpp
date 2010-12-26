#include "Global.h"
#include <ciso646>

noise::module::Perlin gen2d;
noise::module::Perlin gen3d;

void InitGen() {
	gen2d.SetFrequency(0.001);
	gen2d.SetLacunarity(2);
	gen2d.SetNoiseQuality(noise::QUALITY_BEST);
	gen2d.SetOctaveCount(20);
	gen2d.SetPersistence(0.5);
}

Chunk::Chunk(uint64_t _x, uint64_t _y, uint64_t _z) {
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

void Chunk::Generate() {
	lock.lock();
	//Map generation goes here =D
	for(uint8_t a=0;a<16;a++){
		for(uint8_t b=0;b<16;b++){
			double cx, cy;
			if(x/0x10000000%2==1){
				cx = ((x&0xfffffff)<<4)+a;
			} else {
				cx = 0x100000000-(((x&0xfffffff)<<4)+a);
			}
			if(y/0x10000000%2==1){
				cy = ((y&0xfffffff)<<4)+b;
			} else {
				cy = 0x100000000-(((y&0xfffffff)<<4)+b);
			}
			double hd = gen2d.GetValue(cx,cy,0);
			double o = gen2d.GetValue(x>>16,y>>16,1000);
			hd += o;
			int64_t r = hd*0x80;
			int64_t rz;
			if(z>=INT64_MAX){
				rz = z-INT64_MAX;
			} else {
				rz = -(INT64_MAX-z);
			}
			for(uint8_t c=0;c<16;c++){
				int64_t bz = rz*16+c;
				int64_t d = bz-r;
				uint16_t i = a*256+b*16+c;
				if(d>0){
					Blocks[i].type = 0;
				} else if(d<0){
					Blocks[i].type = 1;
				} else {
					Blocks[i].type = 3;
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
			if(c->generated){
				c->Update();
			}
		}
		c = GetChunk(x+1,y,z,false);
		if(c!=0){
			c->xn = this;
			xp = c;
			if(c->generated){
				c->Update();
			}
		}
		c = GetChunk(x,y-1,z,false);
		if(c!=0){
			c->yp = this;
			yn = c;
			if(c->generated){
				c->Update();
			}
		}
		c = GetChunk(x,y+1,z,false);
		if(c!=0){
			c->yn = this;
			yp = c;
			if(c->generated){
				c->Update();
			}
		}
		c = GetChunk(x,y,z-1,false);
		if(c!=0){
			c->zp = this;
			zn = c;
			if(c->generated){
				c->Update();
			}
		}
		c = GetChunk(x,y,z+1,false);
		if(c!=0){
			c->zn = this;
			zp = c;
			if(c->generated){
				c->Update();
			}
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
	ChunkUpdate.lock();
	remove(ChunksToUpdate.begin(),ChunksToUpdate.end(),this);
	ChunkUpdate.unlock();
}

const void Chunk::Draw() {
	if(generated && updated){
		if(lock.try_lock()){
			glLoadIdentity();
			int64_t dx, dy, dz;
			if(player.pos.cx>x){
				dx = player.pos.cx-x;
			} else {
				dx = x-player.pos.cx;
				dx = -dx;
			}
			if(player.pos.cy>y){
				dy = player.pos.cy-y;
			} else {
				dy = y-player.pos.cy;
				dy = -dy;
			}
			if(player.pos.cz>z){
				dz = player.pos.cz-z;
			} else {
				dz = z-player.pos.cz;
				dz = -dz;
			}
			glTranslated(-16*dx, -16*dy, -16*dz);
			glTexCoordPointer(3,GL_FLOAT,0,tex);
			glVertexPointer(3,GL_FLOAT,0,model);
			glDrawArrays(GL_QUADS,0,verts);
			glFlush();
			lock.unlock();
		}
	}
}

Chunk* GetChunk(uint64_t x, uint64_t y, uint64_t z, bool generate) {
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
map<uint64_t,map<uint64_t,map<uint64_t,Chunk*>>> ChunkPos;

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