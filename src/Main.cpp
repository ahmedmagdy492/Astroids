#include "Models.h"

int main() {
	InitWindow(WIDTH, HEIGHT, "Astroids");

	SetTargetFPS(60);

	SceneManager sceneManager;

	while (!WindowShouldClose()) {
		ClearBackground(BLACK);

		BeginDrawing();

		sceneManager.ProcessInput();
		sceneManager.RenderActiveScene();

		EndDrawing();
	}

	CloseWindow();
}