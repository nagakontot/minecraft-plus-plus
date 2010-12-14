struct Chunk {
	Block Blocks[16*16*16];
};

map<int64_t, map<int64_t, map<int64_t, Chunk*>>> Chunks;