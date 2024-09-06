#include "Models.h"

int main() {
	InitWindow(WIDTH, HEIGHT, "Astroids");

	SetTargetFPS(60);

	Texture2D bgTexture = LoadTexture("resources/bg.png");
	Font font = LoadFont("resources/game-font.ttf");

	SceneManager sceneManager(&font);
	
	while (!sceneManager.shouldExit) {
		ClearBackground(BLACK);

		BeginDrawing();

		DrawTextureEx(bgTexture, { 0.0f, 0.0f }, 0.0f, 6.0f, RAYWHITE);

		sceneManager.ProcessInput();
		sceneManager.RenderActiveScene();

		EndDrawing();
	}

	UnloadFont(font);
	UnloadTexture(bgTexture);

	CloseWindow();
}