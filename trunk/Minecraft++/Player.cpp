#include "Global.h"

Player player;

Player::Player() {
	width = 0.3;
	height = 1.78;
	eyeh = 1.62;
	pos.x = 8;
	pos.y = 8;
	pos.z = 8;
	vx = 0;
	vy = 0;
	vz = 0;
	onground = 0;
	accel = 0.01;
	gravity = 9.8;
	speed = 8;
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
	const sf::Input& input = Window.GetInput();
	if(Game::Active){
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
			vz = -6;
		}
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
	double nx = pos.x+rx;
	double ny = pos.y+ry;
	double nz = pos.z+rz;
	pos.x = nx;
	pos.y = ny;
	pos.z = nz;
#define HANDLE\
	Block* b = GetBlock(x,y,z,ch);\
	if(b==0){return;}\
	BlockType t = BlockTypes[b->type];\
	if(t.solid){\
		cout << "collision!" << endl;\
		pos.x = ox;\
		pos.y = oy;\
		pos.z = oz;\
	}
#define CHECK\
	if(ox-width>x+1 || ox+width<x){\
		if(oy-width>y+1 || oy+width<y){\
			if(oz-height>z+1 || oz<z){\
				if(nx-width<x+1 && nx+width>x){\
					if(ny-width<y+1 && ny+width>y){\
						if(nz-height<z+1 && nz>z){\
							cout << "ohai" << endl;\
							HANDLE;\
						}\
					}\
				}\
			}\
		}\
	}
#define LOOP_Z\
	if(rz>0){\
		for(int8_t z=floor(oz);z<=oz+rz;z++){\
			CHECK;\
		}\
	} else if(rz<0){\
		for(int8_t z=floor(oz);z>=oz+rz;z--){\
			CHECK;\
		}\
	} else {\
		int8_t z=floor(oz);\
		CHECK;\
	}
#define LOOP_Y\
	if(oy>0){\
		for(int8_t y=floor(oy);y<=oy+ry;y++){\
			LOOP_Z;\
		}\
	} else if(oy<0){\
		for(int8_t y=floor(oy);y>=oy+ry;y--){\
			LOOP_Z;\
		}\
	} else {\
		int8_t y=floor(oy);\
		LOOP_Z;\
	}
#define LOOP_X\
	if(ox>0){\
		for(int8_t x=floor(ox);x<=ox+rx;x++){\
			LOOP_Y;\
		}\
	} else if(oy<0){\
		for(int8_t x=floor(ox);x>=ox+rx;x--){\
			LOOP_Y;\
		}\
	} else {\
		int8_t x=floor(ox);\
		LOOP_Y;\
	}
	LOOP_X;
	/*double dis = sqrt(sqr(rx)+sqr(ry)+sqr(rz));
	double d = pdir(0,0,rx,ry);
	double p = pdir(0,0,sqrt(sqr(rx)+sqr(ry)),rz);
	while(dis>0){
		int8_t nx, ny, nz;
		double dx, dy, dz;
		if(rx>=0){
			nx = floor(pos.x)+1;
		} else {
			nx = floor(pos.x)-1;
		}
		if(rx==0){
			dx = 0;
		} else {
			dx = abs((nx-pos.x)/ldx(1,d));
		}
		if(ry>=0){
			ny = floor(pos.y)+1;
		} else {
			ny = floor(pos.y)-1;
		}
		if(ry==0){
			dy = 0;
		} else {
			dy = abs((ny-pos.y)/ldy(1,d));
		}
		if(rz>=0){
			nz = floor(pos.z)+1;
		} else {
			nz = floor(pos.z)-1;
		}
		if(rz==0){
			dz = 0;
		} else {
			dz = abs((nz-pos.z)/ldy(1,p));
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
			if(c.value<=0.000001){
				continue;
			}
			double tx = pos.x+ldx(c.value-0.0001,d)*ldx(1,p);
			double ty = pos.y+ldy(c.value-0.0001,d)*ldx(1,p);
			double tz = pos.z+ldy(c.value-0.0001,p);
			Block* b = GetBlock(floor(tx),floor(ty),floor(tz),ch);
			if(b==0){
				dis = 0;
				break;
			}
			BlockType t = BlockTypes[b->type];
			if(t.solid){
				ndis = dis-c.value;
				if(c.type=="x"){
					vx = 0;
					cout << "x";
				}
				if(c.type=="y"){
					vy = 0;
					cout << "y";
				}
				if(c.type=="z"){
					vz = 0;
				}
				pos.x = tx;
				pos.y = ty;
				pos.z = tz;
				break;
			}
		}
		if(ndis>dis){
			pos.x += ldx(dis,d)*ldx(1,p);
			pos.y += ldy(dis,d)*ldx(1,p);
			pos.z += ldy(dis,p);
			break;
		}
		dis = ndis;
	}*/
	pos.Update();
}