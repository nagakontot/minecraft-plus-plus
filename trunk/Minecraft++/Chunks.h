struct Chunk {
	uint64_t x, y, z;
	Chunk *xp, *xn, *yp, *yn, *zp, *zn;
	Block Blocks[4096];
	GLfloat *model, *tex;
	uint16_t verts;
	boost::mutex lock;
	bool generated;
	bool updated;
	Chunk(uint64_t x, uint64_t y, uint64_t z);
	const void Draw();
	void Update();
	void Generate();
};

Chunk* GetChunk(uint64_t x, uint64_t y, uint64_t z, bool generate = true);

extern vector<Chunk*> Chunks;
extern map<uint64_t,map<uint64_t,map<uint64_t,Chunk*>>> ChunkPos;

void InitGen();
void ChunkUpdateThread();
void AddChunkUpdate(Chunk* c);
extern deque<Chunk*> ChunksToUpdate;
extern boost::mutex ChunkUpdate;