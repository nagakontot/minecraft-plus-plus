

namespace Game {
	bool Init();
	bool Loop();
	void Unload();
	extern bool Active;
	extern boost::thread Thread;
};