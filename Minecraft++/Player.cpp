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
		if(input.IsKeyDown(sf::Key::Space) && onground){
			vz = -6;
		}
	}
	Chunk* c = GetChunk(pos.cx,pos.cy,pos.cz);
	if(c==0 || c->generated==false){
		return;
	}
	vz += gravity*delta;
	double ox = pos.x;
	double oy = pos.y;
	double oz = pos.z;
	pos.x += vx*delta;
	pos.y += vy*delta;
	pos.z += vz*delta;
#define CHECK_Z	if(floor(pos.z)>floor(oz)){\
					for(int8_t z=floor(oz)+1;z<=floor(pos.z);z++){\
						Block* b = GetBlock(ox,oy,z,c);\
						if(b==0){\
							vz = 0;\
							pos.z = oz;\
						} else {\
							BlockType t = BlockTypes[b->type];\
							if(t.solid){\
								vz = 0;\
								pos.z = oz;\
								onground = true;\
							}\
						}\
					}\
				} else if(floor(pos.z-height)<floor(oz-height)){\
					for(int8_t z=floor(oz-height)-1;z>=floor(pos.z-height);z--){\
						Block* b = GetBlock(ox,oy,z,c);\
						if(b==0){\
							vz = 0;\
							pos.z = oz;\
						} else {\
							BlockType t = BlockTypes[b->type];\
							if(t.solid){\
								vz = 0;\
								pos.z = oz;\
							}\
						}\
					}\
				}
#define CHECK_Y	if(floor(pos.y+width)>floor(oy+width)){\
					for(int8_t y=floor(oy+width)+1;y<=floor(pos.y+width);y++){\
						Block* b = GetBlock(ox,y,oz,c);\
						if(b==0){\
							vy = 0;\
							pos.y = oy;\
						} else {\
							BlockType t = BlockTypes[b->type];\
							if(t.solid){\
								vy = 0;\
								pos.y = oy;\
							}\
						}\
					}\
				} else if(floor(pos.y-width)<floor(oy-width)){\
					for(int8_t y=floor(oy-width)-1;y>=floor(pos.y-width);y--){\
						Block* b = GetBlock(ox,y,oz,c);\
						if(b==0){\
							vy = 0;\
							pos.y = oy;\
						} else {\
							BlockType t = BlockTypes[b->type];\
							if(t.solid){\
								vy = 0;\
								pos.y = oy;\
							}\
						}\
					}\
				}
#define CHECK_X	if(floor(pos.x+width)>floor(ox+width)){\
					for(int8_t x=floor(ox+width)+1;x<=floor(pos.x+width);x++){\
						Block* b = GetBlock(x,oy,oz,c);\
						if(b==0){\
							vx = 0;\
							pos.x = ox;\
						} else {\
							BlockType t = BlockTypes[b->type];\
							if(t.solid){\
								vx = 0;\
								pos.x = ox;\
							}\
						}\
					}\
				} else if(floor(pos.x-width)<floor(ox-width)){\
					for(int8_t x=floor(ox-width)-1;x>=floor(pos.x-width);x--){\
						Block* b = GetBlock(x,oy,oz,c);\
						if(b==0){\
							vx = 0;\
							pos.x = ox;\
						} else {\
							BlockType t = BlockTypes[b->type];\
							if(t.solid){\
								vx = 0;\
								pos.x = ox;\
							}\
						}\
					}\
				}
	onground = false;
	CHECK_X;
	CHECK_Y;
	CHECK_Z;
	pos.Update();
}