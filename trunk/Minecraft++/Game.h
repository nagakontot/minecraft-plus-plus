extern double delta;
extern uint64_t ticks;

namespace Game {
	bool Init();
	bool Loop();
	void Unload();
	extern bool Active;
	extern bool Done;
	extern boost::thread Thread;
	extern uint16_t Range;
	extern bool Online;
};