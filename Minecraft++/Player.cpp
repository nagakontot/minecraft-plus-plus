#include "Global.h"

Player player;

Player::Player() {
	width = 0.3;
	height = 1.7;
	eyeh = 1.62;
	pos.x = 7.5;
	pos.y = 7.5;
	pos.z = 7.5;
	vx = 1;
	vy = 1;
	vz = 1;
	onground = 0;
	accel = 0.004;
	gravity = 9.8;
	speed = 8;
	jump = 6;
	respawned = false;
	pos.cx = (uint64_t(random(rand()))<<32)+random(rand());
	pos.cy = (uint64_t(random(rand()))<<32)+random(rand());
	pos.cz = UINT64_HALF;
	Window.SetCursorPosition(Window.GetWidth()/2,Window.GetHeight()/2);
}

#define handle(w)	if(d##w!=0 && d##w<dis){\
						v##w = 0;\
						pos.##w = o##w;\
						ndis = dis-d##w;\
					}

void Player::Step() {
	if(!Game::Active){
		return;
	}
	const sf::Input& input = Window.GetInput();
	if(input.IsKeyDown(sf::Key::R)){
		if(!respawned){
			pos.cx = (uint64_t(random(rand()))<<32)+random(rand());
			pos.cy = (uint64_t(random(rand()))<<32)+random(rand());
		}
		respawned = true;
	} else {
		respawned = false;
	}
	int mx = input.GetMouseX()-Window.GetWidth()/2;
	int my = input.GetMouseY()-Window.GetHeight()/2;
	Window.SetCursorPosition(Window.GetWidth()/2,Window.GetHeight()/2);
	rot.d += 0.2*mx;
	rot.p = min((double)89,max((double)-89,rot.p+0.2*my));
	bool kw = input.IsKeyDown(sf::Key::W);
	bool ka = input.IsKeyDown(sf::Key::A);
	bool ks = input.IsKeyDown(sf::Key::S);
	bool kd = input.IsKeyDown(sf::Key::D);
	double d = pow(accel,delta);
	if(kw){
		vx = (vx-ldx(speed,rot.d))*d+ldx(speed,rot.d);
		vy = (vy-ldy(speed,rot.d))*d+ldy(speed,rot.d);
	}
	if(ka){
		vx = (vx-ldx(speed,rot.d-90))*d+ldx(speed,rot.d-90);
		vy = (vy-ldy(speed,rot.d-90))*d+ldy(speed,rot.d-90);
	}
	if(ks){
		vx = (vx-ldx(speed,rot.d+180))*d+ldx(speed,rot.d+180);
		vy = (vy-ldy(speed,rot.d+180))*d+ldy(speed,rot.d+180);
	}
	if(kd){
		vx = (vx-ldx(speed,rot.d+90))*d+ldx(speed,rot.d+90);
		vy = (vy-ldy(speed,rot.d+90))*d+ldy(speed,rot.d+90);
	}
	if(!(kw && ka && ks && kd)){
		vx *= d;
		vy *= d;
	}
	if(input.IsKeyDown(sf::Key::Space)){
		vz = -jump;
	}
	Chunk* ch = GetChunk(pos.cx,pos.cy,pos.cz);
	if(ch==0 || ch->generated==false){
		return;
	}
	vz += gravity*delta;
	double ox = pos.x;
	double oy = pos.y;
	double oz = pos.z;
	double rx = vx*delta;
	double ry = vy*delta;
	double rz = vz*delta;
	onground = false;
/*#define HANDLE\
	Block* b = GetBlock(x,y,z,ch);\
	if(b==0){return;}\
	BlockType t = BlockTypes[b->type];\
	if(t.solid){\
		double ax, ay, az, bx, by, bz;\
		if(rx>0){\
			ax = ox+width;\
			bx = x;\
		} else {\
			ax = ox-width;\
			bx = x+1;\
		}\
		if(ry>0){\
			ay = oy+width;\
			by = y;\
		} else {\
			ay = oy-width;\
			by = y+1;\
		}\
		if(rz>0){\
			az = oz;\
			bz = z;\
		} else {\
			az = oz-height;\
			bz = z+1;\
		}\
		double axy, ayz, azx, vxy, vyz, vzx;\
		axy = pdir(ax,ay,bx,by);\
		vxy = pdir(0,0,vx,vy);\
		ayz = pdir(ay,az,by,bz);\
		vyz = pdir(0,0,vy,vz);\
		azx = pdir(az,ax,bz,bx);\
		vzx = pdir(0,0,vz,vx);\
		double dxy, dyz, dzx;\
		dxy = sign(vx)*sign(vy)*angdif(axy,vxy);\
		dyz = sign(vy)*sign(vz)*angdif(ayz,vyz);\
		dzx = sign(vz)*sign(vx)*angdif(azx,vzx);\
		if(vz>0 && dyz>0 && dzx<0){\
			pos.z = z-0.001;\
			vz = 0;\
			onground = true;\
		} else {\
			cout << dxy << ", " << dyz << ", " << dzx << endl;\
		}\
	}
#define CHECK\
	if(ox-width>x+1 || ox+width<x || oy-width>y+1 || oy+width<y || oz-height>z+1 || oz<z){\
		if(nx-width<x+1 && nx+width>x && ny-width<y+1 && ny+width>y && nz-height<z+1 && nz>z){\
			HANDLE;\
		}\
	}
#define LOOP_Z\
	if(rz>0){\
		for(int8_t z=floor(oz);z<=oz+rz;z++){\
			CHECK;\
		}\
	} else if(rz<0){\
		for(int8_t z=floor(oz);z>=oz+rz-height-1;z--){\
			CHECK;\
		}\
	} else {\
		int8_t z=floor(oz);\
		CHECK;\
	}
#define LOOP_Y\
	if(oy>0){\
		for(int8_t y=floor(oy);y<=oy+ry+width;y++){\
			LOOP_Z;\
		}\
	} else if(oy<0){\
		for(int8_t y=floor(oy);y>=oy+ry-width-1;y--){\
			LOOP_Z;\
		}\
	} else {\
		int8_t y=floor(oy);\
		LOOP_Z;\
	}
#define LOOP_X\
	if(ox>0){\
		for(int8_t x=floor(ox);x<=ox+rx+width;x++){\
			LOOP_Y;\
		}\
	} else if(oy<0){\
		for(int8_t x=floor(ox);x>=ox+rx-width-1;x--){\
			LOOP_Y;\
		}\
	} else {\
		int8_t x=floor(ox);\
		LOOP_Y;\
	}
	LOOP_X;*/
	double tdis = sqrt(sqr(rx)+sqr(ry)+sqr(rz));
	double dis = tdis;
	double dxy = pdir(0,0,rx,ry);
	double dyz = pdir(0,0,ry,rz);
	double dzx = pdir(0,0,rz,rx);
	while(dis>0){
		rx = vx*delta;
		ry = vy*delta;
		rz = vz*delta;
		double nx, ny, nz;
		double dx, dy, dz;
		if(rx>0){
			nx = floor(pos.x+width)+1;
			dx = abs((nx-(pos.x+width))/rx*tdis);
		} else if(rx<0){
			nx = floor(pos.x-width)-1;
			dx = abs((nx-(pos.x-width))/rx*tdis);
		} else {
			dx = 0;
		}
		if(ry>0){
			ny = floor(pos.y+width)+1;
			dy = abs((ny-pos.y-width)/ry*tdis);
		} else if(ry<0){
			ny = floor(pos.y-width)-1;
			dy = abs((ny-pos.y+width)/ry*tdis);
		} else {
			dy = 0;
		}
		if(rz>0){
			nz = floor(pos.z)+1;
			dz = abs((nz-pos.z)/rz*tdis);
		} else if(rz<0){
			nz = floor(pos.z-height)-1;
			dz = abs((nz-pos.z+height)/rz*tdis);
		} else {
			dz = 0;
		}
		double ndis = dis;
		struct choice {
			string type;
			double value;
			choice() {
				type = "";
				value = 0;
			}
			choice(string t, double v) {
				type = t;
				value = v;
			}
			bool operator() (const choice a, const choice b) {
				return a.value>b.value;
			}
		};
		priority_queue<choice,vector<choice>,choice> choices;
		choices.push(choice("x",dx));
		choices.push(choice("y",dy));
		choices.push(choice("z",dz));
		for(int i=0;i<3;i++){
			choice c = choices.top();
			choices.pop();
			if(c.value>dis){
				ndis = dis+1;
				break;
			}
			if(c.value==0){
				continue;
			}
			int8_t sx, sy, sz;
			int8_t fx, fy, fz;
			sx = floor(pos.x+rx/tdis*c.value-width);
			fx = floor(pos.x+rx/tdis*c.value+width);
			sy = floor(pos.y+ry/tdis*c.value-width);
			fy = floor(pos.y+ry/tdis*c.value+width);
			sz = floor(pos.z+rz/tdis*c.value-height);
			fz = floor(pos.z+rz/tdis*c.value);
			if(c.type=="x"){
				for(int8_t y = sy; y<=fy; y++){
					for(int8_t z = sz; z<=fz; z++){
						Block* b = GetBlock(nx,y,z,ch);
						if(b==0){
							return;
						}
						BlockType t = BlockTypes[b->type];
						if(t.solid){
							cout << "x collision!" << endl;
							vx = 0;
							if(rx>0){
								pos.x = nx-0.001;
							}
							if(rx<0){
								pos.x = nx+0.001;
							}
							pos.y += ry/tdis*c.value;
							pos.z += rz/tdis*c.value;
							ndis = dis-c.value;
							goto done;
						}
					}
				}
			}
			if(c.type=="y"){
				for(int8_t x = sx; x<=fx; x++){
					for(int8_t z = sz; z<=fz; z++){
						Block* b = GetBlock(x,ny,z,ch);
						if(b==0){
							return;
						}
						BlockType t = BlockTypes[b->type];
						if(t.solid){
							cout << "y collision!" << endl;
							vy = 0;
							if(ry>0){
								pos.y = ny-0.001;
							}
							if(rx<0){
								pos.y = ny+0.001;
							}
							pos.x += rx/tdis*c.value;
							pos.z += rz/tdis*c.value;
							ndis = dis-c.value;
							goto done;
						}
					}
				}
			}
			if(c.type=="z"){
				for(int8_t x = sx; x<=fx; x++){
					for(int8_t y = sy; y<=fy; y++){
						Block* b = GetBlock(x,y,nz,ch);
						if(b==0){
							return;
						}
						BlockType t = BlockTypes[b->type];
						if(t.solid){
							cout << "z collision!" << endl;
							vz = 0;
							if(rz>0){
								pos.z = nz-0.1;
							}
							if(rz<0){
								pos.z = nz+0.1;
							}
							pos.x += rx/tdis*c.value;
							pos.y += ry/tdis*c.value;
							ndis = dis-c.value;
							goto done;
						}
					}
				}
			}
			done:;
		}
		if(ndis>dis){
			pos.x += rx/tdis*dis;
			pos.y += ry/tdis*dis;
			pos.z += rz/tdis*dis;
			break;
		}
		dis = ndis;
	}
	pos.Update();
}