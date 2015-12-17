enum IDS{PLAYER,BULLET,ENEMY,TOWER};


struct Tower
{
	int ID;
	int x;
	int y;
	bool live;
	int boundx;
	int boundy;
	int hp;
};
struct Player
{
	int ID;
	int x;
	int y;
	int speed;
	int score;
};
struct Bullet
{
		int ID;
		int x;
		int y;
		bool live;
		int speed;
};
struct Enemy
{
	int ID;
	int x;
	int y;
	bool live;
	int speed;
	int boundx;
	int boundy;
	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationDirection;
};
