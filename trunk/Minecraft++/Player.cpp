#include "Global.h"

Player player;

Player::Player() {
	width = 0.3;
	height = 1.7;
	eyeh = 1.6;
	pos.x = 7.5;
	pos.y = 7.5;
	pos.z = 7.5;
	vx = 1;
	vy = 1;
	vz = 1;
	onground = 0;
	accel = 0.0005;
	gravity = 9.8;
	speed = 8;
	jump = 5.5;
	respawned = false;
	safespot = false;
	pos.cx = (uint64_t(random(rand()))<<32)+random(rand());
	pos.cy = (uint64_t(random(rand()))<<32)+random(rand());
	pos.cz = UINT64_HALF-1;
	Window.SetCursorPosition(Window.GetWidth()/2,Window.GetHeight()/2);
}

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

void Player::Step() {
	if(!Game::Active){
		return;
	}
	const sf::Input& input = Window.GetInput();
	if(input.IsKeyDown(sf::Key::R)){
		if(!respawned){
			pos.cx = (uint64_t(random(rand()))<<32)+random(rand());
			pos.cy = (uint64_t(random(rand()))<<32)+random(rand());
			pos.cz = UINT64_HALF;
		}
		respawned = true;
		safespot = false;
	} else {
		respawned = false;
	}
	static bool flying(false);
	static bool lflying(false);
	if(input.IsKeyDown(sf::Key::F)){
		if(!lflying){
			flying = !flying;
			if(flying){
				speed = 32;
				jump = 16;
			} else {
				speed = 8;
				jump = 5.5;
			}
		}
		lflying = true;
	} else {
		lflying = false;
	}
	//Mouse stuff
	int mx = input.GetMouseX()-Window.GetWidth()/2;
	int my = input.GetMouseY()-Window.GetHeight()/2;
	Window.SetCursorPosition(Window.GetWidth()/2,Window.GetHeight()/2);
	rot.d += 0.2*mx;
	rot.p = min((double)89,max((double)-89,rot.p+0.2*my));
	//Moving the player up until he is no longer inside a solid block
	if(!safespot){
		Chunk* cs = GetChunk(pos.cx,pos.cy,pos.cz);
		if(cs==0 || !cs->generated){
			return;
		}
		Block* bh1 = GetBlock(pos.x,pos.y,pos.z,cs);
		Block* bh2 = GetBlock(pos.x,pos.y,pos.z-1,cs);
		if(bh1==0 || bh2==0){
			return;
		}
		BlockType t1 = BlockTypes[bh1->type];
		BlockType t2 = BlockTypes[bh2->type];
		if(t1.solid || t2.solid){
			pos.z -= 2;
			pos.Update();
			return;
		}
		safespot = true;
	}
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
		vx = 0;
		vy = 0;
		vz = 0;
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
			dx = (nx-(pos.x+width))/rx*tdis;
		} else if(rx<0){
			nx = floor(pos.x-width)-1;
			dx = ((nx+1)-(pos.x-width))/rx*tdis;
		} else {
			dx = 0;
		}
		if(ry>0){
			ny = floor(pos.y+width)+1;
			dy = (ny-(pos.y+width))/ry*tdis;
		} else if(ry<0){
			ny = floor(pos.y-width)-1;
			dy = ((ny+1)-(pos.y-width))/ry*tdis;
		} else {
			dy = 0;
		}
		if(rz>0){
			nz = floor(pos.z)+1;
			dz = (nz-pos.z)/rz*tdis;
		} else if(rz<0){
			nz = floor(pos.z-height)-1;
			dz = ((nz+1)-pos.z+height)/rz*tdis;
		} else {
			dz = 0;
		}
		double ndis = dis;
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
								pos.x = nx+1.0001+width;
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
								pos.y = ny+1.0001+width;
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
							if(rz>0){
								vz = 0;
								onground = true;
								pos.z = nz-0.0001;
							}
							if(rz<0){
								if(flying){
									finished = true;
									goto done;
								}
								vz = 0;
								pos.z = nz+1.0001+height;
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

void Player::EditBlocks(int button) {
	Block* bo = 0;
	int8_t bx, by, bz;
	string bd = "";
	double tdis = 10;//Maximum block distance
	double dis = tdis;
	double rx = ldx(1,player.rot.d)*ldx(1,player.rot.p)*tdis;
	double ry = ldy(1,player.rot.d)*ldx(1,player.rot.p)*tdis;
	double rz = ldy(1,player.rot.p)*tdis;
	double ox = pos.x;
	double oy = pos.y;
	double oz = pos.z-eyeh;
	Chunk* ch = GetChunk(pos.cx,pos.cy,pos.cz);
	while(dis>0){
		bool finished = false;
		double nx, ny, nz;
		double dx, dy, dz;
		if(rx>0){
			nx = floor(ox)+1;
			dx = (nx-ox)/rx*tdis;
		} else if(rx<0){
			nx = floor(ox-0.0001)-1;
			dx = ((nx+1)-ox)/rx*tdis;
		} else {
			dx = 0;
		}
		if(ry>0){
			ny = floor(oy)+1;
			dy = (ny-oy)/ry*tdis;
		} else if(ry<0){
			ny = floor(oy-0.0001)-1;
			dy = ((ny+1)-oy)/ry*tdis;
		} else {
			dy = 0;
		}
		if(rz>0){
			nz = floor(oz)+1;
			dz = (nz-oz)/rz*tdis;
		} else if(rz<0){
			nz = floor(oz-0.0001)-1;
			dz = ((nz+1)-oz)/rz*tdis;
		} else {
			dz = 0;
		}
		priority_queue<choice,vector<choice>,choice> choices;
		choices.push(choice("x",dx));
		choices.push(choice("y",dy));
		choices.push(choice("z",dz));

		choice c;
		bool done = false;
		while(!choices.empty()){
			c = choices.top();
			choices.pop();
			if(c.value>0.00001 && c.value<dis){
				done = true;
				break;
			}
		}
		if(!done){
			break;
		}
		double ndis = dis-c.value;
		ox += rx/tdis*c.value;
		oy += ry/tdis*c.value;
		oz += rz/tdis*c.value;
		int8_t cx = floor(ox);
		int8_t cy = floor(oy);
		int8_t cz = floor(oz);
		Chunk* Ch = ch;
		if(c.type=="x"){
			Block* b = GetBlock(nx,cy,cz,Ch);
			if(b==0){
				return;
			}
			BlockType t = BlockTypes[b->type];
			if(t.mineable){
				bo = b;
				bx = nx;
				by = cy;
				bz = cz;
				if(rx>0){
					bd = "xn";
				} else {
					bd = "xp";
				}
			}
		}
		if(c.type=="y"){
			Block* b = GetBlock(cx,ny,cz,Ch);
			if(b==0){
				return;
			}
			BlockType t = BlockTypes[b->type];
			if(t.mineable){
				bo = b;
				bx = cx;
				by = ny;
				bz = cz;
				if(ry>0){
					bd = "yn";
				} else {
					bd = "yp";
				}
			}
		}
		if(c.type=="z"){
			Block* b = GetBlock(cx,cy,nz,Ch);
			if(b==0){
				return;
			}
			BlockType t = BlockTypes[b->type];
			if(t.mineable){
				bo = b;
				bx = cx;
				by = cy;
				bz = nz;
				if(rz>0){
					bd = "zn";
				} else {
					bd = "zp";
				}
			}
		}
		dis = ndis;
		if(bo){
			glLoadIdentity();
			glTranslated(bx,by,bz);
			glColor4f(1,1,1,0.5);
			glBegin(GL_QUADS);
			if(bd=="xp"){
				glVertex3f(1.001,0,0);
				glVertex3f(1.001,1,0);
				glVertex3f(1.001,1,1);
				glVertex3f(1.001,0,1);
			}
			if(bd=="xn"){
				glVertex3f(-0.001,0,0);
				glVertex3f(-0.001,0,1);
				glVertex3f(-0.001,1,1);
				glVertex3f(-0.001,1,0);
			}
			if(bd=="yp"){
				glVertex3f(0,1.001,0);
				glVertex3f(0,1.001,1);
				glVertex3f(1,1.001,1);
				glVertex3f(1,1.001,0);
			}
			if(bd=="yn"){
				glVertex3f(0,-0.001,0);
				glVertex3f(1,-0.001,0);
				glVertex3f(1,-0.001,1);
				glVertex3f(0,-0.001,1);
			}
			if(bd=="zp"){
				glVertex3f(0,0,1.001);
				glVertex3f(1,0,1.001);
				glVertex3f(1,1,1.001);
				glVertex3f(0,1,1.001);
			}
			if(bd=="zn"){
				glVertex3f(0,0,-0.001);
				glVertex3f(0,1,-0.001);
				glVertex3f(1,1,-0.001);
				glVertex3f(1,0,-0.001);
			}
			glEnd();
			glColor4f(1,1,1,1);
			if(button==1){
				bo->type = 0;
				AddBlockUpdate(bx,by,bz,ch);
			}
			if(button==2){
				if(bd=="xp"){
					GetBlock(bx+1,by,bz,ch)->type = Inv.Selected;
					AddBlockUpdate(bx+1,by,bz,ch);
				}
				if(bd=="xn"){
					GetBlock(bx-1,by,bz,ch)->type = Inv.Selected;
					AddBlockUpdate(bx-1,by,bz,ch);
				}
				if(bd=="yp"){
					GetBlock(bx,by+1,bz,ch)->type = Inv.Selected;
					AddBlockUpdate(bx,by+1,bz,ch);
				}
				if(bd=="yn"){
					GetBlock(bx,by-1,bz,ch)->type = Inv.Selected;
					AddBlockUpdate(bx,by-1,bz,ch);
				}
				if(bd=="zp"){
					GetBlock(bx,by,bz+1,ch)->type = Inv.Selected;
					AddBlockUpdate(bx,by,bz+1,ch);
				}
				if(bd=="zn"){
					GetBlock(bx,by,bz-1,ch)->type = Inv.Selected;
					AddBlockUpdate(bx,by,bz-1,ch);
				}
			}
			break;
		}
	}
	//Handle stuff
}