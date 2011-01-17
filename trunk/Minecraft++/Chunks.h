struct Chunk {
	uint64_t x, y, z;
	Chunk *xp, *xn, *yp, *yn, *zp, *zn;
	Block Blocks[4096];
	GLuint vbo;
	uint16_t verts;
	bool generated;
	bool updated;
	bool modified;
	Chunk(uint64_t x, uint64_t y, uint64_t z);
	~Chunk();
	const void Draw();
	void Update();
	void Generate();
	void AddUpdate(uint8_t x1, uint8_t y1, uint8_t z1, uint8_t x2, uint8_t y2, uint8_t z2);
};

Chunk* GetChunk(uint64_t x, uint64_t y, uint64_t z, bool generate = true);

extern set<Chunk*> Chunks;
extern map<uint64_t,map<uint64_t,map<uint64_t,Chunk*>>> ChunkPos;

extern uint16_t GenSpeed;
void InitGen();
void UpdateChunks();
void GenChunks();
void UnloadChunks();
void AddChunkUnload(Chunk* c);
void AddChunkUpdate(Chunk* c);

extern unordered_set<Chunk*> ChunksToUpdate;
extern deque<Chunk*> ChunksToGen;
extern unordered_set<Chunk*> ChunksToUnload;

bool ChunkComp(Chunk* a, Chunk* b);