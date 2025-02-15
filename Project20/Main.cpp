#include "raylib.h"

struct Ball
{
	float x, y;
	float speedX, speedY;
	float radius;

	void Draw()
	{
		DrawCircle((int)x, (int)y, radius, WHITE);
	}
};

struct Paddle
{
	float x, y;
	float speed;
	float width, height;

	Rectangle GetRect()
	{
		return Rectangle{ x - width / 2, y - height / 2, 10, 100 };
	}

	void Draw()
	{
		DrawRectangleRec(GetRect(), WHITE);
	}
};

int main()
{
	InitWindow(800, 600, "Window");
	SetTargetFPS(60);

	Ball ball;
	Sound tick;

	InitAudioDevice();
	tick = LoadSound("tick.mp3");

	ball.x = GetScreenWidth() / 2.0f;
	ball.y = GetScreenHeight() / 2.0f;
	ball.radius = 5.0f;
	ball.speedX = 450;
	ball.speedY = 450;

	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2;
	leftPaddle.width = 10;
	leftPaddle.height = 100;
	leftPaddle.speed = 600;

	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 50;
	rightPaddle.y = GetScreenHeight() / 2;
	rightPaddle.width = 10;
	rightPaddle.height = 100;
	rightPaddle.speed = 600;

	const char* winnerText = nullptr;

	while (!WindowShouldClose())
	{

		ball.x += ball.speedX * GetFrameTime();
		ball.y += ball.speedY * GetFrameTime();

		if (ball.y < 0) // for top
		{
			ball.y = 0;
			ball.speedY *= -1;
		}
		if (ball.y > GetScreenHeight()) // for bottom
		{
			ball.y = GetScreenHeight();
			ball.speedY *= -1;
		}

		// movement of paddles
		if (IsKeyDown(KEY_W))
		{
			leftPaddle.y -= leftPaddle.speed * GetFrameTime();
			if ((leftPaddle.y - leftPaddle.height / 2) < 0)
			{
				leftPaddle.y = leftPaddle.height / 2;
			}
		}
		if (IsKeyDown(KEY_S))
		{
			leftPaddle.y += leftPaddle.speed * GetFrameTime();
			if ((leftPaddle.y + leftPaddle.height / 2) > GetScreenHeight())
			{
				leftPaddle.y = GetScreenHeight() - leftPaddle.height / 2;
			}
		}
		if (IsKeyDown(KEY_UP))
		{
			rightPaddle.y -= leftPaddle.speed * GetFrameTime();
			if (rightPaddle.y - rightPaddle.height / 2 < 0)
			{
				rightPaddle.y = rightPaddle.height / 2;
			}
		}
		if (IsKeyDown(KEY_DOWN))
		{
			rightPaddle.y += leftPaddle.speed * GetFrameTime();
			if (rightPaddle.y + rightPaddle.height / 2 > GetScreenHeight())
			{
				rightPaddle.y = GetScreenHeight() - rightPaddle.height / 2;
			}
		}


		// collision
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, leftPaddle.GetRect()))
		{
			if (ball.speedX < 0)
			{
				PlaySound(tick);
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - leftPaddle.y) / (leftPaddle.height / 2) * ball.speedX;
			}
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, rightPaddle.GetRect()))
		{
			if (ball.speedX > 0)
			{
				ball.speedX *= -1.1f;
				ball.speedY = (ball.y - rightPaddle.y) / (rightPaddle.height / 2) * -ball.speedX;
				PlaySound(tick);
			}
		}

		if (ball.x < 0) // check who won
		{
			winnerText = "Right Player Wins!";
		}
		if (ball.x > GetScreenWidth())
		{
			winnerText = "Left Player Wins!";
		}

		if (winnerText != nullptr && IsKeyPressed(KEY_SPACE))
		{
			ball.x = GetScreenWidth() / 2;
			ball.y = GetScreenHeight() / 2;
			ball.speedX = 300;
			ball.speedY = 300;
			winnerText = nullptr;
		}

		BeginDrawing();
			ClearBackground(BLACK);

			ball.Draw();
			leftPaddle.Draw();
			rightPaddle.Draw();

			if (winnerText)
			{
				int textWidth = MeasureText(winnerText, 60);
				DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 2 - 30, 60, YELLOW);
			}

			DrawFPS(10, 10);
		EndDrawing();
	}
	UnloadSound(tick);
	CloseAudioDevice();
	CloseWindow();
	return 0;
}