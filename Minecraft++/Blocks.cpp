#include "Global.h"

map<uint16_t,BlockType> BlockTypes;

BlockType::BlockType() {
	solid = false;
	porosity = 0;
	texture = 0;
}

BlockType::BlockType(bool _solid, uint8_t _porosity, GLuint _texture, bool _mineable, uint8_t _opacity) {
	solid = _solid;
	porosity = _porosity;
	texture = _texture;
	mineable = _mineable;
	opacity = _opacity;
}

void InitBlocks() {
	//Air
	BlockTypes[0] = BlockType(true, 255, 0, false, 0);
	//Dirt
	BlockTypes[0] = BlockType(true, 100, 0, true, 255);
}