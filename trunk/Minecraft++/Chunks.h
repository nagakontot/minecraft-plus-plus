struct Chunk {
	int64_t x, y, z;
	Block Blocks[4096];
	Chunk(int64_t x, int64_t y, int64_t z);
	void Draw();
};

Chunk* GetChunk(int64_t x, int64_t y, int64_t z);

extern vector<Chunk*> Chunks;