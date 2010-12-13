#include "Global.h"

map<uint16_t,BlockType> BlockTypes;

BlockType::BlockType() {
	solid = false;
	porosity = 0;
	texture = 0;
}

BlockType::BlockType(bool _solid, uint8_t _porosity, GLuint _texture) {
	solid = _solid;
	porosity = _porosity;
	texture = _texture;
}

void InitBlocks() {
	//Air
	BlockTypes[0] = BlockType(true, 255, 0);
}