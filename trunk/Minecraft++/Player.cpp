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
	jump = 5.5;
	respawned = false;
	pos.cx = (uint64_t(random(rand()))<<32)+random(rand());
	pos.cy = (uint64_t(random(rand()))<<32)+random(rand());
	pos.cz = UINT64_HALF;
	Window.SetCursorPosition(Window.GetWidth()/2,Window.GetHeight()/2);
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
	if(input.IsKeyDown(sf::Key::Space) && onground){
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
	double tdis = sqrt(sqr(rx)+sqr(ry)+sqr(rz));
	double dis = tdis;
	double dxy = pdir(0,0,rx,ry);
	double dyz = pdir(0,0,ry,rz);
	double dzx = pdir(0,0,rz,rx);
	while(dis>0){
		bool finished = false;
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
			dy = abs((ny-(pos.y+width))/ry*tdis);
		} else if(ry<0){
			ny = floor(pos.y-width)-1;
			dy = abs((ny-(pos.y-width))/ry*tdis);
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
				finished = true;
				break;
			}
			if(c.value==0){
				if(choices.empty()){
					finished = true;
				}
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
						Chunk* Ch = ch;
						Block* b = GetBlock(nx,y,z,Ch);
						if(b==0){
							return;
						}
						BlockType t = BlockTypes[b->type];
						if(t.solid){
							vx = 0;
							if(rx>0){
								pos.x = nx-0.0001-width;
							}
							if(rx<0){
								pos.x = nx+0.0001+width;
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
						Chunk* Ch = ch;
						Block* b = GetBlock(x,ny,z,Ch);
						if(b==0){
							return;
						}
						BlockType t = BlockTypes[b->type];
						if(t.solid){
							vy = 0;
							if(ry>0){
								pos.y = ny-0.0001-width;
							}
							if(ry<0){
								pos.y = ny+0.0001+width;
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
						Chunk* Ch = ch;
						Block* b = GetBlock(x,y,nz,Ch);
						if(b==0){
							return;
						}
						BlockType t = BlockTypes[b->type];
						if(t.solid){
							vz = 0;
							if(rz>0){
								onground = true;
								pos.z = nz-0.0001;
							}
							if(rz<0){
								pos.z = nz+0.0001;
							}
							pos.x += rx/tdis*c.value;
							pos.y += ry/tdis*c.value;
							ndis = dis-c.value;
							goto done;
						}
					}
				}
			}
		}
		finished = true;
		done:;
		if(finished){
			pos.x += rx/tdis*ndis;
			pos.y += ry/tdis*ndis;
			pos.z += rz/tdis*ndis;
			break;
		}
		dis = ndis;
		continue;
	}
	pos.Update();
}