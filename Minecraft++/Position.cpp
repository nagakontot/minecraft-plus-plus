#include "Global.h"

void Position::Update() {
	cx += floor(x/16);
	cy += floor(y/16);
	cz += floor(z/16);
	x = fmod(fmod(x,16)+16,16);
	y = fmod(fmod(y,16)+16,16);
	z = fmod(fmod(z,16)+16,16);
}