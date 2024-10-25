
#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game() : mWindow(nullptr), mRenderer(nullptr), mIsRunning(true)
, mBallPos{ 1024 / 2.f,768 / 2.f } 
, mPaddlePos{10.0f,768/2.f},mTicksCount(0)
, mPaddleDir(0),mBallVel{-200.0f,235.0f}
, mRightPaddlePos{1004.0f,768/2.f}, mRightPaddleDir(0)
{
}

bool Game::Initialize()
{
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    
    //mTicksCount = SDL_GetTicks();
    
    if (sdlResult != 0)
    {
        SDL_Log("Unable to initialize SDL : %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 1)",
        100,
        100,
        1024,
        768,
        0);

    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer : %s ", SDL_GetError());
        return false;
    }


    for (int i = 0; i < 5; i++)
    {
        Ball* b = new Ball; 
        int dir = 1;
        b->mBallPos = { 1024 / 2.f + 30 * i, 768 / 2.f + 30 * i };
        b->mBallVel = { -200.0f * dir + 10 * i, 135.0f + 10 * i};
        dir *= -1;
        mBalls.push_back(b);
    }
    ballCount = mBalls.size();
    return true;
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown()
{
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
    SDL_Quit();
}

void Game::ProcessInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            mIsRunning = false;
            break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }

    mPaddleDir = 0;
    if (state[SDL_SCANCODE_W])
        mPaddleDir -= 1;
    if (state[SDL_SCANCODE_S])
        mPaddleDir += 1;
    
    mRightPaddleDir = 0;
    if (state[SDL_SCANCODE_I])
        mRightPaddleDir -= 1;
    if (state[SDL_SCANCODE_K])
        mRightPaddleDir += 1;

}

void Game::UpdateGame()
{
    // 마지막 프레임이후 16ms가 경과할 때까지 대기
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

    // 최대 델타 시간값으로 고정
    if (deltaTime > 0.05f)
        deltaTime = 0.05f;
    mTicksCount = SDL_GetTicks();

    
    for (size_t i = 0; i < mBalls.size(); i++)
    {
        if (!mBalls[i])
            continue;
        Ball*& b =  mBalls[i];
        if (mPaddleDir != 0)
        {
            mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;

            if (mPaddlePos.y < (paddleH / 2.0f + thickness))
            {
                mPaddlePos.y = paddleH / 2.0f + thickness;
            }
            else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
            {
                mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
            }
        }

        if (mRightPaddleDir != 0)
        {
            mRightPaddlePos.y += mRightPaddleDir * 300.0F * deltaTime;

            if (mRightPaddlePos.y < (paddleH / 2.0f + thickness))
            {
                mRightPaddlePos.y = paddleH / 2.0f + thickness;
            }
            else if (mRightPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
            {
                mRightPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
            }

        }

        b->mBallPos.x += b->mBallVel.x * deltaTime;
        b->mBallPos.y += b->mBallVel.y * deltaTime;

        

        if (b->mBallPos.y <= thickness && b->mBallVel.y < 0.0f)
        {
            b->mBallVel.y *= -1;
        }

        float diff = mPaddlePos.y - b->mBallPos.y;
        // Take absolute value of difference
        diff = (diff > 0.0f) ? diff : -diff;
        if (
            // Our y-difference is small enough
            diff <= paddleH / 2.0f &&
            // We are in the correct x-position
            b->mBallPos.x <= 25.0f && b->mBallPos.x >= 20.0f &&
            // The ball is moving to the left
            b->mBallVel.x < 0.0f)
        {
            b->mBallVel.x *= -1.0f;
        }
        // Did the ball go off the screen? (if so, end game)
        else if (b->mBallPos.x <= 0.0f)
        {
            ballCount--;
            if (mBalls[i])
            {
                delete mBalls[i];
                mBalls[i] = nullptr;
            }
            if(ballCount==0)
                mIsRunning = false;
        }

        if (!mBalls[i])
            continue;
        // Did the ball collide with the top wall?
        if (b->mBallPos.y <= thickness && b->mBallVel.y < 0.0f)
        {
            b->mBallVel.y *= -1;
        }
        // Did the ball collide with the bottom wall?
        else if (b->mBallPos.y >= (768 - thickness) &&
            b->mBallVel.y > 0.0f)
        {
            b->mBallVel.y *= -1;
        }

        if (!mBalls[i])
            continue;

        float rightDiff = mRightPaddlePos.y - b->mBallPos.y;
        // Take absolute value of difference
        rightDiff = (rightDiff > 0.0f) ? rightDiff : -rightDiff;
        if (
            // Our y-difference is small enough
            rightDiff <= paddleH / 2.0f &&
            // We are in the correct x-position
            b->mBallPos.x <= 1011.5f && b->mBallPos.x >= 996.5f &&
            // The ball is moving to the left
            b->mBallVel.x > 0.0f)
        {
            b->mBallVel.x *= -1.0f;
        }
        // Did the ball go off the screen? (if so, end game)
        else if (b->mBallPos.x >= 1024.f)
        {
            ballCount--;
            if (mBalls[i])
            {
                delete mBalls[i];
                mBalls[i] = nullptr;
            }
            if (ballCount == 0)
                mIsRunning = false;
        }
        if (!mBalls[i])
            continue;
        // Did the ball collide with the top wall?
        if (b->mBallPos.y <= thickness && b->mBallVel.y < 0.0f)
        {
            b->mBallVel.y *= -1;
        }
        // Did the ball collide with the bottom wall?
        else if (b->mBallPos.y >= (768 - thickness) &&
            b->mBallVel.y > 0.0f)
        {
            b->mBallVel.y *= -1;
        }
    }
}

void Game::GenerateOutput()
{
    SDL_SetRenderDrawColor(
        mRenderer,
        0,
        0,
        255,
        255
    );
    SDL_RenderClear(mRenderer);

    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

    SDL_Rect wall{ 0,0,1024,thickness };

    SDL_RenderFillRect(mRenderer, &wall);
    
    wall.y = 768 - thickness;
    
    SDL_RenderFillRect(mRenderer, &wall);

   /* wall.x = 1024 - thickness;
    wall.y = 0;
    wall.w = thickness;
    wall.h = 768;
    SDL_RenderFillRect(mRenderer, &wall);*/

    SDL_Rect rightPaddle
    {
        static_cast<int>(mRightPaddlePos.x),
        static_cast<int>(mRightPaddlePos.y - paddleH/2),
        thickness,
        static_cast<int>(paddleH)
    };
    SDL_RenderFillRect(mRenderer, &rightPaddle);

    // Draw paddle
    SDL_Rect paddle{
        static_cast<int>(mPaddlePos.x),
        static_cast<int>(mPaddlePos.y - paddleH / 2),
        thickness,
        static_cast<int>(paddleH)
    };
    SDL_RenderFillRect(mRenderer, &paddle);

    for (size_t i = 0; i < mBalls.size(); i++)
    {
        if (!mBalls[i])
            continue;
        Ball*& b = mBalls[i];
        SDL_Rect ball{
           static_cast<int>(b->mBallPos.x - thickness / 2),
           static_cast<int>(b->mBallPos.y - thickness / 2),
           thickness,
           thickness
        };

        SDL_RenderFillRect(mRenderer, &ball);
    }
    SDL_RenderPresent(mRenderer);
}
