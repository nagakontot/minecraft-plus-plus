struct Block{
	uint16_t type;
	uint16_t info;
	uint8_t temperature;
	uint8_t saturation;
	uint16_t light;
	void Draw(int x, int y, int z);
};

struct BlockType{
	BlockType();
	BlockType(bool solid, uint8_t porosity, bool mineable, uint8_t opacity);
	bool solid;
	uint8_t porosity;
	bool mineable;
	uint8_t opacity;
};

extern map<uint16_t,BlockType> BlockTypes;

void InitBlocks();