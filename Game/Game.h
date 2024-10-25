
#include "SDL.h"

#pragma comment(lib,"C:\\Users\\woobu\\OneDrive\\πŸ≈¡ »≠∏È\\SDL2-2.30.8\\lib\\x64\\SDL2")
#pragma comment(lib,"C:\\Users\\woobu\\OneDrive\\πŸ≈¡ »≠∏È\\SDL2-2.30.8\\lib\\x64\\SDL2main")

#include <vector>
using namespace std;

struct Vector2
{
	float x;
	float y;
};

struct Ball
{
	Vector2 mBallPos;
	Vector2 mBallVel;
};

class Game
{
public:
	Game();

	bool Initialize();

	void RunLoop();

	void Shutdown();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

private:
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	vector<Ball*> mBalls;
	bool mIsRunning;
	Vector2 mPaddlePos;
	Vector2 mRightPaddlePos;
	Vector2 mBallPos;
	Uint32 mTicksCount;
	int mPaddleDir;
	int mRightPaddleDir;
	Vector2 mBallVel;
	int ballCount;
};
