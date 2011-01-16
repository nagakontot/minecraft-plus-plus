struct Chunk {
	uint64_t x, y, z;
	Chunk *xp, *xn, *yp, *yn, *zp, *zn;
	Block Blocks[4096];
	GLfloat *model, *tex;
	GLuint vbo;
	uint16_t verts;
	boost::mutex lock;
	bool generated;
	bool updated;
	bool vboupdated;
	Chunk(uint64_t x, uint64_t y, uint64_t z);
	const void Draw();
	void Update();
	void Generate();
	void AddUpdate(uint8_t x1, uint8_t y1, uint8_t z1, uint8_t x2, uint8_t y2, uint8_t z2);
};

Chunk* GetChunk(uint64_t x, uint64_t y, uint64_t z, bool generate = true);

extern set<Chunk*> Chunks;
extern map<uint64_t,map<uint64_t,map<uint64_t,Chunk*>>> ChunkPos;

void InitGen();
void ChunkUpdateThread();
void ChunkGenThread();
void ChunkUnloadThread();
void AddChunkUnload(Chunk* c);
void AddChunkUpdate(Chunk* c);

extern unordered_set<Chunk*> ChunksToUpdate;
extern boost::mutex ChunkUpdate;
extern bool ChunkThreadDone;

extern unordered_set<Chunk*> ChunksToGen;
extern boost::mutex ChunkGen;
extern bool GenThreadDone;

extern unordered_set<Chunk*> ChunksToUnload;
extern boost::mutex ChunkUnload;
extern bool UnloadThreadDone;
extern unordered_set<GLuint> BuffersToUnload;