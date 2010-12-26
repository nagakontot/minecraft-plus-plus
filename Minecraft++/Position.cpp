#include "Global.h"

void Position::Update() {
	int8_t dx = floor(x/16);
	int8_t dy = floor(y/16);
	int8_t dz = floor(z/16);
	cx += dx;
	cy += dy;
	cz += dz;
	x = fmod(fmod(x,16)+16,16);
	y = fmod(fmod(y,16)+16,16);
	z = fmod(fmod(z,16)+16,16);
}