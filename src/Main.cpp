#include "Models.h"

int main() {
	InitWindow(WIDTH, HEIGHT, "Astroids");

	SetTargetFPS(60);

	Font font = LoadFont("resources/game-font.otf");

	SceneManager sceneManager(&font);
	
	while (!sceneManager.shouldExit) {
		ClearBackground(BLACK);

		BeginDrawing();

		sceneManager.ProcessInput();
		sceneManager.RenderActiveScene();

		EndDrawing();
	}

	UnloadFont(font);

	CloseWindow();
}