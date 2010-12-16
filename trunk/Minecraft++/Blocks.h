struct Block{
	uint16_t type;
	uint16_t info;
	uint8_t temperature;
	uint8_t saturation;
	uint16_t light;
	void Draw(int x, int y, int z, struct Chunk* chunk);
};

struct BlockType{
	BlockType();
	BlockType(bool solid, uint8_t porosity, bool mineable, double opacity, GLuint* texture);
	bool solid;
	uint8_t porosity;
	bool mineable;
	double opacity;
	GLuint* texture;
};

extern vector<BlockType> BlockTypes;

void InitBlocks();