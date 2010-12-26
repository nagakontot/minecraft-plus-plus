struct Block{
	uint16_t type;
	uint16_t info;
	uint8_t temperature;
	uint8_t saturation;
	uint8_t light;
	uint8_t extra;
	Block();
	const void Draw(int8_t x, int8_t y, int8_t z);
	void Update(int8_t x, int8_t y, int8_t z, struct Chunk* chunk);
};

struct BlockType{
	BlockType();
	BlockType(bool solid, uint8_t porosity, bool mineable, double opacity, const GLfloat* model, const GLfloat* tex, uint16_t verts);
	bool solid;
	uint8_t porosity;
	bool mineable;
	double opacity;
	const GLfloat* model;
	const GLfloat* tex;
	uint16_t verts;
};

extern BlockType BlockTypes[100];

void InitBlocks();

Block* GetBlock(int8_t x, int8_t y, int8_t z, Chunk*& chunk);

bool BlockVisible(int8_t x, int8_t y, int8_t z, Chunk* chunk);