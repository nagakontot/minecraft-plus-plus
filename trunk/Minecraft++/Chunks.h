struct Chunk {
	int64_t x, y, z;
	Chunk *xp, *xn, *yp, *yn, *zp, *zn;
	Block Blocks[4096];
	GLfloat *model, *tex;
	uint16_t verts;
	boost::mutex lock;
	bool generated;
	bool updated;
	Chunk(int64_t x, int64_t y, int64_t z);
	const void Draw();
	void Update();
	void Generate();
};

Chunk* GetChunk(int64_t x, int64_t y, int64_t z, bool generate = true);

extern vector<Chunk*> Chunks;
extern map<int64_t,map<int64_t,map<int64_t,Chunk*>>> ChunkPos;

void ChunkUpdateThread();
void AddChunkUpdate(Chunk* c);
extern deque<Chunk*> ChunksToUpdate;
extern boost::mutex ChunkUpdate;