class Player {
public:
	Position pos;
	Rotation rot;
	double width;
	double height;
	double eyeh;
	double vx, vy, vz;
	double accel;
	double gravity;
	double speed;
	double jump;
	bool respawned;
	bool onground;
	Player();
	void Step();
} extern player;