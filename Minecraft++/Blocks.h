struct Block{
	uint16_t type;
	uint16_t info;
	uint8_t temperature;
	uint8_t saturation;
	uint8_t light;
	uint8_t extra;
	Block();
	const void Draw(int x, int y, int z);
	void Update(int x, int y, int z, struct Chunk* chunk);
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

Block* GetBlock(int x, int y, int z, Chunk*& chunk);

bool BlockVisible(int x, int y, int z, Chunk* chunk);