struct Block{
	uint16_t type;
	uint16_t info;
	uint8_t temperature;
	uint8_t saturation;
	uint8_t light;
	uint8_t extra;
	Block();
	void Draw(int x, int y, int z, struct Chunk* chunk);
	void Update(int x, int y, int z, struct Chunk* chunk);
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

extern BlockType BlockTypes[100];

void InitBlocks();

Block* GetBlock(int x, int y, int z, Chunk*& chunk);

bool BlockVisible(int x, int y, int z, Chunk* chunk);