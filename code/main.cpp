#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include "vstd/vmath.h"
constexpr auto SCR_WIDTH = 800;
constexpr auto SCR_HEIGHT = 800;

constexpr auto GRID_SIZE = 32;
constexpr auto N_ROW = SCR_HEIGHT / GRID_SIZE;
constexpr auto N_COLUMNS = SCR_HEIGHT / GRID_SIZE;
constexpr auto RECT_COLOR = RAYWHITE;

int LastIndexMarked = -1;

void ProcessInput();
void DrawGrid();
void DrawCorners();

static std::vector<bool> Grid;
static bool Running = true;

constexpr auto MOUSE_COLOR = GREEN;
constexpr auto KEYBOARD_COLOR = BLUE;
constexpr auto MOUSE_RADIUS = 20;
static Vector2 MousePos = { 0, 0 };
static Vector2 KeyboardPos = { 0, 0 };
static double TimeElapsed = 0;


int main(void)
{
	InitWindow(SCR_WIDTH, SCR_HEIGHT, "DDA");

	Grid.resize(int(N_ROW * N_COLUMNS), false);

	while(Running)
	{
		ProcessInput();
		TimeElapsed = GetTime();

		Vector2 RayStart = KeyboardPos;
		RayStart.x = RayStart.x / GRID_SIZE;
		RayStart.y = RayStart.y / GRID_SIZE;

		Vector2 MouseUnitSpace = MousePos;
		MouseUnitSpace.x = MouseUnitSpace.x / GRID_SIZE;
		MouseUnitSpace.y = MouseUnitSpace.y / GRID_SIZE;

		Vector2 RayDir = Vector2Normalize(Vector2Subtract(MouseUnitSpace, RayStart));
		
		Vector2 RayUnitStepSize = {
			sqrt(1 + (RayDir.y / RayDir.x) * (RayDir.y / RayDir.x)),
			sqrt(1 + (RayDir.x / RayDir.y) * (RayDir.x / RayDir.y))
		};

		Vector2 MapCheck = RayStart;

		Vector2 RayLength1D;
		Vector2 Step;
		
		if (RayDir.x < 0)
		{
			Step.x = -1;
			RayLength1D.x = (RayStart.x - (float)MapCheck.x) * RayUnitStepSize.x;
		}
		else
		{
			Step.x = 1;
			RayLength1D.x = (float(MapCheck.x + 1) - RayStart.x) * RayUnitStepSize.x;
		}

		if (RayDir.y < 0)
		{
			Step.y = -1;
			RayLength1D.y = (RayStart.y - (float)MapCheck.y) * RayUnitStepSize.y;
		}
		else
		{
			Step.y = 1;
			RayLength1D.y = (float(MapCheck.y + 1) - RayStart.y)* RayUnitStepSize.y;
		}

		bool TileFound = false;
		float Distance = 0.0f;
		float MaxDistance = 100.0f;
		while (!TileFound && Distance < MaxDistance)
		{
			if (RayLength1D.x < RayLength1D.y)
			{
				MapCheck.x += Step.x;
				Distance = RayLength1D.x;
				RayLength1D.x += RayUnitStepSize.x;
			}
			else
			{
				MapCheck.y += Step.y;
				Distance = RayLength1D.y;
				RayLength1D.y += RayUnitStepSize.y;
			}

			if (MapCheck.x >= 0 && MapCheck.x < N_COLUMNS &&
				MapCheck.y >= 0 && MapCheck.y < N_ROW)
			{
				int Index = MapCheck.y * N_COLUMNS + MapCheck.x;
				
				if (Grid[Index])
				{
					TileFound = true;
				}
			}
		}
		Vector2 Intersection;
		if (TileFound)
		{
			Intersection = RayStart + RayDir * Distance;
		}

		BeginDrawing();
			ClearBackground(GRAY);
			DrawGrid();
			DrawCorners();
			if (TileFound)
			{
				DrawCircle(Intersection.x * GRID_SIZE, 
					Intersection.y * GRID_SIZE,
					MOUSE_RADIUS / 2.0f, YELLOW);
			}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}


void DrawCorners()
{
	DrawCircle(MousePos.x, MousePos.y, MOUSE_RADIUS, MOUSE_COLOR);
	DrawCircle(KeyboardPos.x, KeyboardPos.y, MOUSE_RADIUS, KEYBOARD_COLOR);
	DrawLine(MousePos.x, MousePos.y, KeyboardPos.x, KeyboardPos.y, RED);
}

void DrawGrid()
{
	for (int i = 0; i < Grid.size(); i++)
	{
		if (Grid[i])
		{
			int Row = i / N_COLUMNS;
			int Column = i % N_ROW;
			DrawRectangle(Column * GRID_SIZE, Row * GRID_SIZE, GRID_SIZE, GRID_SIZE, RECT_COLOR);
		}

	}


	for (int i = 0; i < SCR_WIDTH; i+=GRID_SIZE)
	{
		DrawLine(i, 0, i, SCR_HEIGHT, RED);
		for (int j = 0; j < SCR_HEIGHT; j+=GRID_SIZE)
		{
			DrawLine(0, j, SCR_WIDTH, j, RED);
		}
	}
}

void ProcessInput()
{
	if (IsKeyReleased(KEY_ESCAPE) || WindowShouldClose()) Running = false;
	
	if (IsKeyDown(KEY_W)) {
		KeyboardPos.y -= 0.1;
	} else if (IsKeyDown(KEY_S)) {
		KeyboardPos.y += 0.1;
	}

	if (IsKeyDown(KEY_A)) {
		KeyboardPos.x -= 0.1;
	} else if (IsKeyDown(KEY_D)) {
		KeyboardPos.x += 0.1;
	}
	
	KeyboardPos.x = Clampf32(KeyboardPos.x, 0, SCR_WIDTH);
	KeyboardPos.y = Clampf32(KeyboardPos.y, 0, SCR_HEIGHT);

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
	{
		int Row = MousePos.y / GRID_SIZE;
		int Column = MousePos.x / GRID_SIZE;
		int Index = Row * N_COLUMNS + Column;
		if (Index != LastIndexMarked)
		{
			Grid[Index] = !Grid[Index];
			LastIndexMarked = Index;
		}
	}
	if (IsKeyReleased(KEY_C))
	{
		for (int i = 0; i < Grid.size(); i++) Grid[i] = false;
	}
	MousePos = GetMousePosition();
}