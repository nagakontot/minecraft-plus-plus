struct Block{
	uint16_t type;
	uint16_t info;
	uint8_t temperature;
	uint8_t saturation;
	uint16_t light;
};

struct BlockType{
	bool solid;
	void* model;//To be dealt with later
};

extern map<uint16_t,BlockType> BlockTypes;

void InitBlocks();