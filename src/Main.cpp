#include "Models.h"

int main() {
	InitWindow(WIDTH, HEIGHT, "Astroids");

	SetTargetFPS(60);

	Font font = LoadFont("resources/game-font.otf");

	BeginDrawing();
	ClearBackground(BLACK);
	const char* loading = "Loading...";
	Vector2 size = MeasureTextEx(font, loading, 50.0f, 0.0f);
	DrawTextEx(font, loading, {(WIDTH - size.x)/2, (HEIGHT - size.y)/2}, 50.0f, 0.0f, WHITE);
	EndDrawing();

	InitAudioDevice();

	SceneManager sceneManager(&font);
	
	while (!sceneManager.shouldExit) {
		ClearBackground(BLACK);

		BeginDrawing();

		sceneManager.ProcessInput();
		sceneManager.RenderActiveScene();

		EndDrawing();
	}

	UnloadFont(font);

	CloseAudioDevice();

	CloseWindow();
}