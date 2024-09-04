#include "Models.h"

int main() {
	InitWindow(WIDTH, HEIGHT, "Astroids");

	SetTargetFPS(60);

	SceneManager sceneManager;

	Texture2D bgTexture = LoadTexture("resources/bg.png");

	
	while (!WindowShouldClose()) {
		ClearBackground(BLACK);

		BeginDrawing();

		DrawTextureEx(bgTexture, { 0.0f, 0.0f }, 0.0f, 6.0f, RAYWHITE);

		sceneManager.ProcessInput();
		sceneManager.RenderActiveScene();

		EndDrawing();
	}

	UnloadTexture(bgTexture);

	CloseWindow();
}