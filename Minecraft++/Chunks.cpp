#include "Global.h"

uint16_t GenSpeed;

void InitGen() {
}

Chunk::Chunk(uint64_t _x, uint64_t _y, uint64_t _z) {
	generated = false;
	updated = false;
	modified = false;
	x = _x;
	y = _y;
	z = _z;
	xp = 0;
	xn = 0;
	yp = 0;
	yn = 0;
	zp = 0;
	zn = 0;
	glGenBuffers(1,&vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	verts = 0;
	ChunkPos[_x][_y][_z] = this;
	AddChunkUpdate(this);
}

Chunk::~Chunk() {
	if(!Game::Done){
		auto it2 = find(ChunksToGen.begin(),ChunksToGen.end(),this);
		if(it2!=ChunksToGen.end()){
			ChunksToGen.erase(it2);
		}
		auto it3 = find(ChunksToUpdate.begin(),ChunksToUpdate.end(),this);
		if(it3!=ChunksToUpdate.end()){
			ChunksToUpdate.erase(it3);
		}
		auto it4 = find(Chunks.begin(),Chunks.end(),this);
		if(it4!=Chunks.end()){
			Chunks.erase(it4);
		}
		ChunkPos[x][y][z] = 0;
		if(xp!=0){
			xp->xn = 0;
		}
		if(xn!=0){
			xn->xp = 0;
		}
		if(yp!=0){
			yp->yn = 0;
		}
		if(yn!=0){
			yn->yp = 0;
		}
		if(zp!=0){
			zp->zn = 0;
		}
		if(zn!=0){
			zn->zp = 0;
		}
		glDeleteBuffers(1,&vbo);
	}
	if(modified){
		ofstream file("save/"+tostring(x)+"."+tostring(y)+"."+tostring(z)+".imd", ios_base::binary);
		file.write((char*)Blocks,32768);
		file.close();
	}
}

struct rand2d {
public:
	double nn, np, pn, pp;
	uint64_t x, y;
	rand2d(uint64_t x, uint64_t y, uint8_t depth, uint64_t type) {
		this->x = x;
		this->y = y;
		uint64_t xt = x;
		uint64_t yt = y;
		nn = 0;
		np = 0;
		pn = 0;
		pp = 0;
		for(uint8_t i=0;i<depth;i++){
			uint64_t inn, inp, ipn, ipp;
			inn = random(xt,yt,type,i)>>(depth-i);
			inp = random(xt,yt+1,type,i)>>(depth-i);
			ipn = random(xt+1,yt,type,i)>>(depth-i);
			ipp = random(xt+1,yt+1,type,i)>>(depth-i);
			double xn = 1-double(x-(xt<<i))/(1<<i);
			double yn = 1-double(y-(yt<<i))/(1<<i);
			double xp = double(((xt+1)<<i)-x-1)/(1<<i);
			double yp = double(((yt+1)<<i)-y-1)/(1<<i);
			xn = xn*xn*(3-2*xn);
			yn = yn*yn*(3-2*yn);
			xp = xp*xp*(3-2*xp);
			yp = yp*yp*(3-2*yp);
			nn += (inn*xn*yn+inp*xn*(1-yn)+ipn*(1-xn)*yn+ipp*(1-xn)*(1-yn))/0x100000000;
			np += (inn*xn*yp+inp*xn*(1-yp)+ipn*(1-xn)*yp+ipp*(1-xn)*(1-yp))/0x100000000;
			pn += (inn*xp*yn+inp*xp*(1-yn)+ipn*(1-xp)*yn+ipp*(1-xp)*(1-yn))/0x100000000;
			pp += (inn*xp*yp+inp*xp*(1-yp)+ipn*(1-xp)*yp+ipp*(1-xp)*(1-yp))/0x100000000;
			xt >>= 1;
			yt >>= 1;
		}
	}
	double get(uint8_t bx, uint8_t by) {
		return (nn*(16-bx)*(16-by)+np*(16-bx)*by+pn*bx*(16-by)+pp*bx*by)/(16*16)-0.5;
	}
};
struct rand3d {
public:
	double nnn, npn, pnn, ppn, nnp, npp, pnp, ppp;
	uint64_t x, y, z;
	rand3d(uint64_t x, uint64_t y, uint64_t z, uint8_t depth, uint64_t type) {
		this->x = x;
		this->y = y;
		this->z = z;
		uint64_t xt = x;
		uint64_t yt = y;
		uint64_t zt = z;
		nnn = 0;
		npn = 0;
		pnn = 0;
		ppn = 0;
		nnp = 0;
		npp = 0;
		pnp = 0;
		ppp = 0;
		for(uint8_t i=0;i<depth;i++){
			uint64_t innn, inpn, ipnn, ippn, innp, inpp, ipnp, ippp;
			innn = random(xt,yt,zt,type,i)>>(depth-i);
			inpn = random(xt,yt+1,zt,type,i)>>(depth-i);
			ipnn = random(xt+1,yt,zt,type,i)>>(depth-i);
			ippn = random(xt+1,yt+1,zt,type,i)>>(depth-i);
			innp = random(xt,yt,zt+1,type,i)>>(depth-i);
			inpp = random(xt,yt+1,zt+1,type,i)>>(depth-i);
			ipnp = random(xt+1,yt,zt+1,type,i)>>(depth-i);
			ippp = random(xt+1,yt+1,zt+1,type,i)>>(depth-i);
			double xn = 1-double(x-(xt<<i))/(1<<i);
			double yn = 1-double(y-(yt<<i))/(1<<i);
			double zn = 1-double(z-(zt<<i))/(1<<i);
			double xp = double(((xt+1)<<i)-x-1)/(1<<i);
			double yp = double(((yt+1)<<i)-y-1)/(1<<i);
			double zp = double(((zt+1)<<i)-z-1)/(1<<i);
			xn = xn*xn*(3-2*xn);
			yn = yn*yn*(3-2*yn);
			zn = zn*zn*(3-2*zn);
			xp = xp*xp*(3-2*xp);
			yp = yp*yp*(3-2*yp);
			zp = zp*zp*(3-2*zp);
			nnn += (innn*xn*yn*zn+inpn*xn*(1-yn)*zn+ipnn*(1-xn)*yn*zn+ippn*(1-xn)*(1-yn)*zn+innp*xn*yn*(1-zn)+inpp*xn*(1-yn)*(1-zn)+ipnp*(1-xn)*yn*(1-zn)+ippp*(1-xn)*(1-yn)*(1-zn))/0x100000000;
			npn += (innn*xn*yp*zn+inpn*xn*(1-yp)*zn+ipnn*(1-xn)*yp*zn+ippn*(1-xn)*(1-yp)*zn+innp*xn*yp*(1-zn)+inpp*xn*(1-yp)*(1-zn)+ipnp*(1-xn)*yp*(1-zn)+ippp*(1-xn)*(1-yp)*(1-zn))/0x100000000;
			pnn += (innn*xp*yn*zn+inpn*xp*(1-yn)*zn+ipnn*(1-xp)*yn*zn+ippn*(1-xp)*(1-yn)*zn+innp*xp*yn*(1-zn)+inpp*xp*(1-yn)*(1-zn)+ipnp*(1-xp)*yn*(1-zn)+ippp*(1-xp)*(1-yn)*(1-zn))/0x100000000;
			ppn += (innn*xp*yp*zn+inpn*xp*(1-yp)*zn+ipnn*(1-xp)*yp*zn+ippn*(1-xp)*(1-yp)*zn+innp*xp*yp*(1-zn)+inpp*xp*(1-yp)*(1-zn)+ipnp*(1-xp)*yp*(1-zn)+ippp*(1-xp)*(1-yp)*(1-zn))/0x100000000;
			nnp += (innn*xn*yn*zp+inpn*xn*(1-yn)*zp+ipnn*(1-xn)*yn*zp+ippn*(1-xn)*(1-yn)*zp+innp*xn*yn*(1-zp)+inpp*xn*(1-yn)*(1-zp)+ipnp*(1-xn)*yn*(1-zp)+ippp*(1-xn)*(1-yn)*(1-zp))/0x100000000;
			npp += (innn*xn*yp*zp+inpn*xn*(1-yp)*zp+ipnn*(1-xn)*yp*zp+ippn*(1-xn)*(1-yp)*zp+innp*xn*yp*(1-zp)+inpp*xn*(1-yp)*(1-zp)+ipnp*(1-xn)*yp*(1-zp)+ippp*(1-xn)*(1-yp)*(1-zp))/0x100000000;
			pnp += (innn*xp*yn*zp+inpn*xp*(1-yn)*zp+ipnn*(1-xp)*yn*zp+ippn*(1-xp)*(1-yn)*zp+innp*xp*yn*(1-zp)+inpp*xp*(1-yn)*(1-zp)+ipnp*(1-xp)*yn*(1-zp)+ippp*(1-xp)*(1-yn)*(1-zp))/0x100000000;
			ppp += (innn*xp*yp*zp+inpn*xp*(1-yp)*zp+ipnn*(1-xp)*yp*zp+ippn*(1-xp)*(1-yp)*zp+innp*xp*yp*(1-zp)+inpp*xp*(1-yp)*(1-zp)+ipnp*(1-xp)*yp*(1-zp)+ippp*(1-xp)*(1-yp)*(1-zp))/0x100000000;
			xt >>= 1;
			yt >>= 1;
			zt >>= 1;
		}
	}
	double get(uint8_t bx, uint8_t by, uint8_t bz) {
		return (nnn*(16-bx)*(16-by)*(16-bz)+npn*(16-bx)*by*(16-bz)+pnn*bx*(16-by)*(16-bz)+ppn*bx*by*(16-bz)+nnp*(16-bx)*(16-by)*bz+npp*(16-bx)*by*bz+pnp*bx*(16-by)*bz+ppp*bx*by*bz)/(16*16*16)-0.5;
	}
};

void Chunk::Generate() {
	//Map generation goes here =D
	ifstream file("save/"+tostring(x)+"."+tostring(y)+"."+tostring(z)+".imd", ios_base::binary);
	if(file.is_open()){
		file.read((char*)Blocks,32768);
		file.close();
	} else {
		rand2d hill(x,y,1,0);
		rand3d cave(x,y,z,0,0);
		rand3d strange(x,y,z,1,2);
		for(uint8_t a=0;a<16;a++){
			for(uint8_t b=0;b<16;b++){
				if(z>UINT64_HALF && z-UINT64_HALF>10000){
					for(uint8_t c=0;c<16;c++){
						Blocks[a*256+b*16+c].type = 2;
					}
				} else if(z<UINT64_HALF && UINT64_HALF-z>10000){
					for(uint8_t c=0;c<16;c++){
						Blocks[a*256+b*16+c].type = 0;
					}
				} else {
					double hf = hill.get(a,b);
					int64_t dh = sdif(z,UINT64_HALF);
					for(uint8_t c=0;c<16;c++){
						double hc = hf+strange.get(a,b,c)*0.2;
						int64_t h = hc*1024;
						int64_t bh = int8_t(c)+dh*16;
						int64_t d = bh-h;
						uint16_t i = a*256+b*16+c;
						if(d<0 || abs(cave.get(a,b,c))<0.01){
							Blocks[i].type = 0;
						} else if(d>4){
							Blocks[i].type = 2;
						} else if(d>0){
							Blocks[i].type = 1;
						} else {
							Blocks[i].type = 3;
						}
					}
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
	AddChunkUpdate(this);
	generated = true;
}

void Chunk::Update() {
	verts = 0;
	for(int a=0;a<16;a++){
		for(int b=0;b<16;b++){
			for(int c=0;c<16;c++){
				Block* bl = &Blocks[a*256+b*16+c];
				BlockType t = BlockTypes[bl->type];
				if(t.tex!=0){
					bl->Update(a,b,c,this);
					if(bl->extra&1){
						if(t.model==0){
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
	}
	if(verts==0){
		updated = true;
		return;
	}
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,verts*6*sizeof(GLfloat),NULL,GL_STATIC_DRAW);
	void* ptr = glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
	GLfloat* model = (GLfloat*)ptr;
	GLfloat* tex = ((GLfloat*)ptr)+verts*3;
	int i = 0;
	for(int a=0;a<16;a++){
		for(int b=0;b<16;b++){
			for(int c=0;c<16;c++){
				Block* bl = &Blocks[a*256+b*16+c];
				BlockType t = BlockTypes[bl->type];
				if(bl->extra&1){
					if(t.model==0 && t.tex!=0){
						if(bl->extra&0x40){
							for(int j=0;j<4;j++){
								int k = 3*i;
								int r = 3*j;
								model[k] = M_XP[r]+a;
								model[k+1] = M_XP[r+1]+b;
								model[k+2] = M_XP[r+2]+c;
								tex[k] = (7*a+15-b+T_QUAD[j*2])/16;
								tex[k+1] = (7*a+c+T_QUAD[j*2+1])/16;
								tex[k+2] = (GLfloat(t.tex[0])+0.5)/MAX_TEXTURES;
								i++;
							}
						}
						if(bl->extra&0x20){
							for(int j=0;j<4;j++){
								int k = 3*i;
								int r = 3*j;
								model[k] = M_XN[r]+a;
								model[k+1] = M_XN[r+1]+b;
								model[k+2] = M_XN[r+2]+c;
								tex[k] = (7*a+b+T_QUAD[j*2])/16;
								tex[k+1] = (7*a+c+T_QUAD[j*2+1])/16;
								tex[k+2] = (GLfloat(t.tex[1])+0.5)/MAX_TEXTURES;
								i++;
							}
						}
						if(bl->extra&0x10){
							for(int j=0;j<4;j++){
								int k = 3*i;
								int r = 3*j;
								model[k] = M_YP[r]+a;
								model[k+1] = M_YP[r+1]+b;
								model[k+2] = M_YP[r+2]+c;
								tex[k] = (7*b+a+T_QUAD[j*2])/16;
								tex[k+1] = (7*b+c+T_QUAD[j*2+1])/16;
								tex[k+2] = (GLfloat(t.tex[2])+0.5)/MAX_TEXTURES;
								i++;
							}
						}
						if(bl->extra&0x8){
							for(int j=0;j<4;j++){
								int k = 3*i;
								int r = 3*j;
								model[k] = M_YN[r]+a;
								model[k+1] = M_YN[r+1]+b;
								model[k+2] = M_YN[r+2]+c;
								tex[k] = (7*b+15-a+T_QUAD[j*2])/16;
								tex[k+1] = (7*b+c+T_QUAD[j*2+1])/16;
								tex[k+2] = (GLfloat(t.tex[3])+0.5)/MAX_TEXTURES;
								i++;
							}
						}
						if(bl->extra&0x4){
							for(int j=0;j<4;j++){
								int k = 3*i;
								int r = 3*j;
								model[k] = M_ZP[r]+a;
								model[k+1] = M_ZP[r+1]+b;
								model[k+2] = M_ZP[r+2]+c;
								tex[k] = (7*c+15-a+T_QUAD[j*2])/16;
								tex[k+1] = (7*c+b+T_QUAD[j*2+1])/16;
								tex[k+2] = (GLfloat(t.tex[4])+0.5)/MAX_TEXTURES;
								i++;
							}
						}
						if(bl->extra&0x2){
							for(int j=0;j<4;j++){
								int k = 3*i;
								int r = 3*j;
								model[k] = M_ZN[r]+a;
								model[k+1] = M_ZN[r+1]+b;
								model[k+2] = M_ZN[r+2]+c;
								tex[k] = (7*c+15-a+T_QUAD[j*2])/16;
								tex[k+1] = (7*c+15-b+T_QUAD[j*2+1])/16;
								tex[k+2] = (GLfloat(t.tex[5])+0.5)/MAX_TEXTURES;
								i++;
							}
						}
					} else {
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
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	updated = true;
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
	//View culling. Thereby speeding up the rendering, suppose the culling itself isn't too intensive
	//This needs a lot of work.
	/*double vfov = 60*degtorad;
	double wfov = atan(tan(vfov)*Window.GetWidth()/Window.GetHeight());
	double diag = sqrt(sqr(tan(vfov))+sqr(tan(wfov)));
	double dfov = atan(diag);
	double px, py, pz;
	if(x>player.pos.cx){
		px = x-player.pos.cx;
	} else {
		px = player.pos.cx-x;
		px = -px;
	}
	px -= player.pos.x/16-0.5;
	if(y>player.pos.cy){
		py = y-player.pos.cy;
	} else {
		py = player.pos.cy-y;
		py = -py;
	}
	py -= player.pos.y/16-0.5;
	if(z>player.pos.cz){
		pz = z-player.pos.cz;
	} else {
		pz = player.pos.cz-z;
		pz = -pz;
	}
	pz -= player.pos.z/16-0.5;
	double dis = sqrt(sqr(px)+sqr(py)+sqr(pz));
	double ad = 2*acos(sqrt(3.0)/2/dis);
	double da = acos((ldx(1,player.rot.d)*ldx(1,player.rot.p)*px+ldy(1,player.rot.d)*ldx(1,player.rot.p)*py+ldy(1,player.rot.p)*pz)/dis);
	if(dis<2 || da<dfov+ad){*/
		glLoadIdentity();
		double dx, dy, dz;
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
		glBindBuffer(GL_ARRAY_BUFFER,vbo);
		glVertexPointer(3,GL_FLOAT,0,0);
		glTexCoordPointer(3,GL_FLOAT,0,(char*)NULL+verts*3*sizeof(GLfloat));
		glDrawArrays(GL_QUADS,0,verts);
	//}
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

set<Chunk*> Chunks;
map<uint64_t,map<uint64_t,map<uint64_t,Chunk*>>> ChunkPos;

void UpdateChunks() {
	while(!ChunksToUpdate.empty()){
		auto it = ChunksToUpdate.begin();
		Chunk* c = *it;
		ChunksToUpdate.erase(it);
		c->Update();
	}
}

bool ChunkComp(Chunk* a, Chunk* b){
	double da = pdis(player.pos.cx,player.pos.cy,player.pos.cz,a->x,a->y,a->z);
	double db = pdis(player.pos.cx,player.pos.cy,player.pos.cz,b->x,b->y,b->z);
	return da<db;
}
void GenChunks() {
	for(int f=0;f<GenSpeed && !ChunksToGen.empty(); f++){
		Chunk* c = ChunksToGen.front();
		ChunksToGen.pop_front();
		c->Generate();
		AddChunkUpdate(c);
	}
}

void UnloadChunks() {
	if(Game::Done){
		for(auto it=ChunksToUnload.begin();it!=ChunksToUnload.end();it++){
			Chunk* c = *it;
			if(c->modified){
				ofstream file("save/"+tostring(c->x)+"."+tostring(c->y)+"."+tostring(c->z)+".imd", ios_base::binary);
				file.write((char*)c->Blocks,32768);
				file.close();
			}
		}
	} else {
		while(!ChunksToUnload.empty()){
			auto it = ChunksToUnload.begin();
			Chunk* c = *it;
			ChunksToUnload.erase(it);
			delete c;
		}
	}
}

void AddChunkUpdate(Chunk* c) {
	if(c->generated){
		ChunksToUpdate.insert(c);
	} else {
		ChunksToGen.push_back(c);
	}
}

unordered_set<Chunk*> ChunksToUpdate;
deque<Chunk*> ChunksToGen;
unordered_set<Chunk*> ChunksToUnload;

void AddChunkUnload(Chunk* c) {
	ChunksToUnload.insert(c);
}