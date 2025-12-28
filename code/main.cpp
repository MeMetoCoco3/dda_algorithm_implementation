#include <iostream>
#include <raylib.h>
#include <vector>
constexpr auto SCR_WIDTH = 800;
constexpr auto SCR_HEIGHT = 800;

constexpr auto GRID_SIZE = 32;
constexpr auto N_ROW = SCR_HEIGHT / GRID_SIZE;
constexpr auto N_COLUMNS = SCR_HEIGHT / GRID_SIZE;
constexpr auto MOUSE_COLOR = GREEN;
constexpr auto MOUSE_RADIUS = 20;
constexpr auto RECT_COLOR = RAYWHITE;

int LastIndexMarked = -1;

void ProcessInput();
void DrawGrid();
void DrawCursor();

static std::vector<bool> Grid;
int main(void)
{
	InitWindow(SCR_WIDTH, SCR_HEIGHT, "DDA");

	Grid.resize(int(N_ROW * N_COLUMNS), false);

	for (!WindowShouldClose();;)
	{
		ProcessInput();

		BeginDrawing();
			ClearBackground(GRAY);
			DrawGrid();
			DrawCursor();
		EndDrawing();
	}
	return 0;
}


void DrawCursor()
{
	Vector2 MousePos = GetMousePosition();
	DrawCircle(MousePos.x, MousePos.y, MOUSE_RADIUS, MOUSE_COLOR);
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




	for (int i = 0; i < 800; i+=GRID_SIZE)
	{
		DrawLine(i, 0, i, SCR_HEIGHT, RED);
		for (int j = 0; j < 800; j+=GRID_SIZE)
		{
			DrawLine(0, j, SCR_WIDTH, j, RED);
		}
	}
}

void ProcessInput()
{
	if (IsKeyReleased(KEY_ESCAPE))
	{
		CloseWindow();
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
	{
		Vector2 MousePos = GetMousePosition();
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
}