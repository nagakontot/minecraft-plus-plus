struct Block{
	uint16_t type;
	uint16_t info;
	uint8_t temperature;
	uint8_t saturation;
	uint16_t light;
};

struct BlockType{
	BlockType();
	BlockType(bool _solid, uint8_t _porosity, GLuint _texture, bool _mineable, uint8_t _opacity);
	bool solid;
	uint8_t porosity;
	GLuint texture;
	bool mineable;
	uint8_t opacity;
};

extern map<uint16_t,BlockType> BlockTypes;

void InitBlocks();