#include "Global.h"

noise::module::Perlin gen2d;
noise::module::Perlin gen3d;

void InitGen() {
	gen2d.SetFrequency(0.001);
	gen2d.SetLacunarity(2);
	gen2d.SetNoiseQuality(noise::QUALITY_STD);
	gen2d.SetOctaveCount(10);
	gen2d.SetPersistence(0.5);
}

Chunk::Chunk(uint64_t _x, uint64_t _y, uint64_t _z) {
	generated = false;
	updated = false;
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
	ChunkPos[_x][_y][_z] = this;
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
				c->AddUpdate(15,0,0,15,15,15);
			}
		}
		c = GetChunk(x+1,y,z,false);
		if(c!=0){
			c->xn = this;
			xp = c;
			if(c->generated){
				c->AddUpdate(0,0,0,0,15,15);
			}
		}
		c = GetChunk(x,y-1,z,false);
		if(c!=0){
			c->yp = this;
			yn = c;
			if(c->generated){
				c->AddUpdate(0,15,0,15,15,15);
			}
		}
		c = GetChunk(x,y+1,z,false);
		if(c!=0){
			c->yn = this;
			yp = c;
			if(c->generated){
				c->AddUpdate(0,0,0,15,0,15);
			}
		}
		c = GetChunk(x,y,z-1,false);
		if(c!=0){
			c->zp = this;
			zn = c;
			if(c->generated){
				c->AddUpdate(0,0,15,15,15,15);
			}
		}
		c = GetChunk(x,y,z+1,false);
		if(c!=0){
			c->zn = this;
			zp = c;
			if(c->generated){
				c->AddUpdate(0,0,0,15,15,0);
			}
		}
	}
	generated = true;
	AddChunkUpdate(this);
	lock.unlock();
}

#define append(x,y) for(int j=0;j<4;j++){\
						int k = 3*i;\
						int r = 3*j;\
						nmodel[k] = x[r]+a;\
						nmodel[k+1] = x[r+1]+b;\
						nmodel[k+2] = x[r+2]+c;\
						ntex[k] = T_QUAD[j*2];\
						ntex[k+1] = T_QUAD[j*2+1];\
						ntex[k+2] = (GLfloat(t.tex[y])+0.5)/MAX_TEXTURES;\
						i++;\
					}

void Chunk::Update() {
	lock.lock();
	verts = 0;
	for(int a=0;a<16;a++){
		for(int b=0;b<16;b++){
			for(int c=0;c<16;c++){
				Block* bl = &Blocks[a*256+b*16+c];
				bl->Update(a,b,c,this);
				BlockType t = BlockTypes[bl->type];
				if(bl->extra&1){
					if(t.model==0 && t.tex!=0){
						if(bl->extra&0x40){
							verts += 4;
						}
						if(bl->extra&0x20){
							verts += 4;
						}
						if(bl->extra&0x10){
							verts += 4;
						}
						if(bl->extra&0x8){
							verts += 4;
						}
						if(bl->extra&0x4){
							verts += 4;
						}
						if(bl->extra&0x2){
							verts += 4;
						}
					} else {
						verts += BlockTypes[bl->type].verts;
					}
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
					if(t.model==0 && t.tex!=0){
						if(bl->extra&0x40){
							append(M_XP,0);
						}
						if(bl->extra&0x20){
							append(M_XN,1);
						}
						if(bl->extra&0x10){
							append(M_YP,2);
						}
						if(bl->extra&0x8){
							append(M_YN,3);
						}
						if(bl->extra&0x4){
							append(M_ZP,4);
						}
						if(bl->extra&0x2){
							append(M_ZN,5);
						}
					} else {
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
	}
	delete[] model;
	delete[] tex;
	model = nmodel;
	tex = ntex;
	updated = true;
	lock.unlock();
}

void Chunk::AddUpdate(uint8_t x1, uint8_t y1, uint8_t z1, uint8_t x2, uint8_t y2, uint8_t z2) {
	for(uint8_t a=x1;a<=x2;a++){
		for(uint8_t b=y1;b<=y2;b++){
			for(uint8_t c=z1;c<=z2;c++){
				Blocks[a*256+b*16+c].extra &= 0x7f;
			}
		}
	}
	AddChunkUpdate(this);
}

const void Chunk::Draw() {
	if(generated && updated && verts>0){
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
	while(!Game::Done){
		if(ChunksToUpdate.empty()){
			sf::Sleep(0.01);
		} else {
			ChunkUpdate.lock();
			auto it = ChunksToUpdate.begin();
			Chunk* c = *it;
			ChunksToUpdate.erase(it);
			ChunkUpdate.unlock();
			if(!c->generated){
				c->Generate();
			} else {
				c->Update();
			}
		}
	}
	ChunkThreadDone = true;
}

void AddChunkUpdate(Chunk* c) {
	ChunkUpdate.lock();
	ChunksToUpdate.insert(c);
	ChunkUpdate.unlock();
}

unordered_set<Chunk*> ChunksToUpdate;
boost::mutex ChunkUpdate;
bool ChunkThreadDone = false;