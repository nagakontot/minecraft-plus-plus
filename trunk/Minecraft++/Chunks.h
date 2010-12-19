struct Chunk {
	int64_t x, y, z;
	Chunk *xp, *xn, *yp, *yn, *zp, *zn;
	Block Blocks[4096];
	GLfloat *model, *tex;
	uint16_t verts;
	Chunk(int64_t x, int64_t y, int64_t z);
	const void Draw();
	void Update();
};

Chunk* GetChunk(int64_t x, int64_t y, int64_t z, bool generate = true);

extern vector<Chunk*> Chunks;