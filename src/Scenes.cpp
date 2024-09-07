#include "Models.h"

GameScene::GameScene(SceneManager* sceneManager) : Scene("GameScene") {
	this->sceneManager = sceneManager;

	fireSound = LoadSound("resources/fire.wav");
	thrustSound = LoadSound("resources/thrust.wav");
	bangSmallSound = LoadSound("resources/bangSmall.wav");
	bangMediumSound = LoadSound("resources/bangMedium.wav");

	for (int i = 0; i < ASTROIDS_POOL_SIZE; ++i) {
		int x = GetRandomValue(MAX_ASTROID_RADIUS - 100.0f, WIDTH + 100.0f);
		int y = GetRandomValue(-1050.0f, MAX_ASTROID_RADIUS);
		Vector3 shapeCenterPoint = { (float)x, (float)y, 0.0f };
		Astroid* astroid = new Astroid(shapeCenterPoint);
		astroids.push_back(astroid);
	}

	for (int i = 0; i < BULLET_POOL_SIZE; ++i) {
		/*Color color = { 
			GetRandomValue(100, 255), GetRandomValue(100, 255), GetRandomValue(100, 255),
			GetRandomValue(50, 255)
		};*/
		Color color = {
			255, 255, 255, 255
		};
		shootingAngle = player.GetRotationAngle();
		bulletsPool.push(new Bullet({-100.0f, -100.0f}, shootingAngle, color));
	}
}

const std::string& GameScene::GetName() const {
	return name;
}

void GameScene::Render() {
	while (bulletsToRemove.size() > 0) {
		Bullet* bullet = bulletsToRemove.top();
		bulletsToRemove.pop();
		std::vector<Bullet*>::iterator result = std::find(bullets.begin(), bullets.end(), bullet);
		if (result != bullets.end()) {
			bullets.erase(result);
		}
	}

	while (astroidsToRemove.size() > 0) {
		Astroid* astroid = astroidsToRemove.top();
		astroidsToRemove.pop();
		std::vector<Astroid*>::iterator result = std::find(astroids.begin(), astroids.end(), astroid);
		if (result != astroids.end()) {
			astroids.erase(result);
		}
	}

	for (int i = 0; i < bullets.size(); ++i) {
		Vector3 position = bullets[i]->GetPosition();
		if (!Utils::IsPositionOffScreen(position, WIDTH, HEIGHT)) {
			bullets[i]->Draw();
		}
	}

	player.Draw();

	for (int i = 0; i < ASTROIDS_SIZE; ++i) {
		astroids[i]->Draw();
	}

	elapsedSeconds += GetFrameTime();

	DrawUI();

	if (isGameOver) {
		const char* gameOverText = "Game Over ";
		Font font = *(sceneManager->GetFont());
		Vector2 size = MeasureTextEx(font, gameOverText, 70.0f, 0.0f);
		DrawTextEx(font, gameOverText, { (WIDTH - size.x)/2, (HEIGHT - size.y)/2}, 70.0f, 0.0f, WHITE);
		
		const char* pressEnterText = "Press Enter to Reset";
		Vector2 pressEnterSize = MeasureTextEx(font, pressEnterText, 50.0f, 0.0f);
		DrawTextEx(font, pressEnterText, { (WIDTH - pressEnterSize.x) / 2, ((HEIGHT - size.y) / 2) + size.y + 20.0f }, 50.0f, 0.0f, WHITE);
	}
}

void GameScene::ResetScene() {
	elapsedSeconds = 0.0;
	isPlayerCollided = false;
	shootingAngle = 90.0f;
	player.ResetState(); 

	for (int i = 0; i < astroids.size(); ++i) {
		int x = GetRandomValue(MAX_ASTROID_RADIUS - 100.0f, WIDTH + 100.0f);
		int y = GetRandomValue(-1050.0f, MAX_ASTROID_RADIUS);
		Vector3 shapeCenterPoint = { (float)x, (float)y, 0.0f };
		astroids[i]->ResetCenterPoint(shapeCenterPoint);
	}

	for (int i = 0; i < bullets.size(); ++i) {
		Bullet* bullet = bullets[bullets.size() - 1];
		if (bullet) {
			bullets.pop_back();
			bullet->ResetPosition({ -100.0f, -100.0f, 0.0f });
			bullet->SetRotationAngle(shootingAngle);
			bulletsPool.push(bullet);
		}
	}

	while (bulletsToRemove.size() > 0) {
		bulletsToRemove.pop();
	}

	while (astroidsToRemove.size() > 0) {
		astroidsToRemove.pop();
	}
}

void GameScene::Update() {
	if (!isGameOver) {
		float curRotAngle = player.GetRotationAngle();

		if (IsKeyDown(KEY_RIGHT)) {
			player.RotatePlayer(PLAYER_ROT_DEG);
			shootingAngle = player.GetRotationAngle();
		}
		if (IsKeyDown(KEY_LEFT)) {
			player.RotatePlayer(-PLAYER_ROT_DEG);
			shootingAngle = player.GetRotationAngle();
		}
		if (IsKeyDown(KEY_UP)) {
			PlaySound(thrustSound);
			player.ShowFuel();
			float angleInRadians = Utils::ConvertDegreesToRadians(curRotAngle);
			Vector3 curPlayerVelocity = player.GetVelocity();
			if (curPlayerVelocity.x < PLYAER_MAX_SPEED && curPlayerVelocity.y < PLYAER_MAX_SPEED) {
				curPlayerVelocity.x += PLAYER_SPEED;
				curPlayerVelocity.y += PLAYER_SPEED;
			}
			player.SetVelocity(curPlayerVelocity);

			Vector3 dirVec = {
				cosf(angleInRadians) * curPlayerVelocity.x,
				sinf(angleInRadians) * curPlayerVelocity.y,
				0.0f
			};

			player.Move({ dirVec.x * -1, dirVec.y * -1, 0.0f });

			PlayerOffScreenDirection playerOffCondition = player.IsPlayerOffScreen(WIDTH, HEIGHT);

			if (playerOffCondition == PlayerOffScreenDirection::PlayerOffHeight || playerOffCondition == PlayerOffScreenDirection::PlayerOffZeroY) {
				unsigned char playerLives = player.GetLivesNo();
				if (playerLives > 0) {
					PlaySound(bangMediumSound);
					isPlayerCollided = true;
					--playerLives;
					player.SetLivesNo(playerLives);
					ResetScene();
				}
			}
			else {
				player.Reposition(playerOffCondition);
			}
		}
		else if (IsKeyUp(KEY_UP)) {
			player.HideFuel();
			float angleInRadians = Utils::ConvertDegreesToRadians(curRotAngle);
			Vector3 curPlayerVelocity = player.GetVelocity();

			if (curPlayerVelocity.x > 0 && curPlayerVelocity.y > 0) {
				curPlayerVelocity.x -= PLAYER_SPEED;
				curPlayerVelocity.y -= PLAYER_SPEED;
				player.SetVelocity(curPlayerVelocity);

				Vector3 dirVec = {
					cosf(angleInRadians) * curPlayerVelocity.x,
					sinf(angleInRadians) * curPlayerVelocity.y,
					0.0f
				};

				player.Move({ dirVec.x * -1, dirVec.y * -1, 0.0f });
			}
		}
		if (IsKeyPressed(KEY_SPACE)) {
			PlaySound(fireSound);
			Vector3 playerPos = player.GetPosition();
		 	Bullet* bullet = bulletsPool.top();
			bulletsPool.pop();
			bullet->ResetPosition({playerPos.x, playerPos.y - 10, 0.0f});
			bullet->SetRotationAngle(shootingAngle);
			bullets.push_back(bullet);
		}

		// astroids moving
		for (int i = 0; i < ASTROIDS_SIZE; ++i) {
			Vector3* points = astroids[i]->GetPosition();
			int noOfPointsOutOfRange = 0;
			astroids[i]->Move({ 0.0f, ASTROID_SPEED, 0.0f });
			for (int j = 0; j < NO_OF_POINTS_IN_ASTROID; ++j) {
				if (points[j].y >= HEIGHT) {
					++noOfPointsOutOfRange;
				}
			}

			if (noOfPointsOutOfRange == NO_OF_POINTS_IN_ASTROID) {
				// the astroid is totally out of sight
				int x = GetRandomValue(MAX_ASTROID_RADIUS, WIDTH);
				int y = GetRandomValue(-50.0f, MAX_ASTROID_RADIUS);
				astroids[i]->ResetCenterPoint({ (float)x, (float)y, 0.0f });
			}

			if (!isPlayerCollided && astroids[i]->IsCollidingWithPlayer(player)) {
				unsigned char playerLives = player.GetLivesNo();
				if (playerLives > 0) {
					PlaySound(bangMediumSound);
					isPlayerCollided = true;
					--playerLives;
					player.SetLivesNo(playerLives);
					ResetScene();
				}
				else {
					isGameOver = true;
				}
			}
		}

		// bullets moving
		for (int i = 0; i < bullets.size(); ++i) {
			float angleInRadians = Utils::ConvertDegreesToRadians(bullets[i]->GetRotationAngle());
			Vector3 position = bullets[i]->GetPosition();
			Vector3 dirVec = { cosf(angleInRadians) * BULLET_X_SPEED, sinf(angleInRadians) * BULLET_Y_SPEED, 0.0f };
			bullets[i]->Move({ dirVec.x * -1, dirVec.y * -1, dirVec.z });

			// check for collision with any astroid
			bool hasBulletCollidedWithAstroid = false;
			for (int j = 0; j < ASTROIDS_SIZE; ++j) {
				if (astroids[j]->IsCollidingWithBullet(bullets[i])) {
					PlaySound(bangSmallSound);
					player.SetScore(player.GetScore() + 1);
					bulletsToRemove.push(bullets[i]);
					astroidsToRemove.push(astroids[j]);
					hasBulletCollidedWithAstroid = true;
					break;
				}
			}

			if (!hasBulletCollidedWithAstroid) {
				// check if the bullet is off the screen and if so remove it from
				// bullets list and add it to bulletsPool
				Vector3 bulletPos = bullets[i]->GetPosition();
				if (
					Utils::IsPositionOffScreen(bulletPos, WIDTH, HEIGHT) ||
					Utils::IsPositionOffScreen({ bulletPos.x + BULLET_SIZE, bulletPos.y, bulletPos.z }, WIDTH, HEIGHT) ||
					Utils::IsPositionOffScreen({ bulletPos.x, bulletPos.y + BULLET_SIZE, bulletPos.z }, WIDTH, HEIGHT) ||
					Utils::IsPositionOffScreen({ bulletPos.x + BULLET_SIZE, bulletPos.y + BULLET_SIZE, bulletPos.z }, WIDTH, HEIGHT)
					) {
					bullets[i]->ResetPosition({ -100.0f, -100.0f, 0.0f });
					bulletsToRemove.push(bullets[i]);
				}
			}
		}
	}
	else {
		if (IsKeyPressed(KEY_ENTER)) {
			isGameOver = false;
			ResetScene();
			player.SetScore(0);
			player.SetLivesNo(PLAYER_LIVES);
		}
	}

	if (IsKeyPressed(KEY_ESCAPE)) {
		sceneManager->SetActiveSceneByName("MenuScene");
	}
}

void GameScene::DrawUI() const {
	float x = 40.0f, y = 50.0f;
	for (int i = 0; i < player.GetLivesNo(); ++i) {
		Vector2 middlePos = { x + PLAYER_BASE_WIDTH * i, y};
		DrawTriangle(middlePos, { middlePos.x - (PLAYER_BASE_WIDTH / 2), middlePos.y + PLAYER_TRI_HEIGHT }, { middlePos.x + (PLAYER_BASE_WIDTH / 2), middlePos.y + PLAYER_TRI_HEIGHT }, GRAY);
		x += PLAYER_BASE_WIDTH + 10.0f;
	}

	// Drawing Score
	const char* scoreText = TextFormat("Score: %i", player.GetScore());
	Font font = *(sceneManager->GetFont());
	Vector2 size = MeasureTextEx(font, scoreText, 45.0f, 0.0f);
	DrawTextEx(font, scoreText, { (WIDTH - size.x) / 2, 50.0f }, 45.0f, 0.0f, WHITE);
}

GameScene::~GameScene() {

	for (int i = 0; i < astroids.size(); ++i) {
		delete astroids[i];
	}
	astroids.clear();

	for (int i = 0; i < bulletsPool.size(); ++i) {
		delete bulletsPool.top();
		bulletsPool.pop();
	}

	for (int i = 0; i < bullets.size(); ++i) {
		delete bullets[i];
	}

	UnloadSound(fireSound);
	UnloadSound(thrustSound);
	UnloadSound(bangSmallSound);
	UnloadSound(bangMediumSound);
}

MenuScene::MenuScene(SceneManager* sceneManager) : Scene("MenuScene"), sceneManager(sceneManager) {
}

const std::string& MenuScene::GetName() const {
	return name;
}

void MenuScene::Render() {
	const char* astroids = "Astroids";
	Font font = *(sceneManager->GetFont());
	Vector2 size = MeasureTextEx(font, astroids, 100.0f, 0.0f);
	DrawTextEx(font, astroids, { (WIDTH - size.x) / 2, 200.0f }, 100.0f, 0.0f, WHITE);

	const char* startGame = "Start Game";
	Vector2 startGameSize = MeasureTextEx(font, startGame, 70.0f, 0.0f);
	DrawTextEx(font, startGame, { (WIDTH - startGameSize.x) / 2, (HEIGHT - startGameSize.y) /2}, 70.0f, 0.0f, WHITE);

	const char* exit = "Exit";
	Vector2 exitSize = MeasureTextEx(font, exit, 70.0f, 0.0f);
	DrawTextEx(font, exit, { (WIDTH - exitSize.x) / 2, ((HEIGHT - startGameSize.y) / 2) + startGameSize.y + 50.0f}, 70.0f, 0.0f, WHITE);
}

void MenuScene::Update() {
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		// start game button
		Vector2 mousePos = GetMousePosition();
		const char* startGame = "Start Game";
		Font font = *(sceneManager->GetFont());
		Vector2 startGameSize = MeasureTextEx(font, startGame, 70.0f, 0.0f);
		if ((mousePos.x >= ((WIDTH - startGameSize.x) / 2) && mousePos.x <= (((WIDTH - startGameSize.x) / 2) + startGameSize.x)) &&
			(mousePos.y >= ((HEIGHT - startGameSize.y) / 2) && mousePos.y <= ((HEIGHT - startGameSize.y) / 2) + startGameSize.y)
			) {
			sceneManager->SetActiveSceneByName("GameScene");
		}

		// exit button
		const char* exit = "Exit";
		Vector2 exitSize = MeasureTextEx(font, exit, 70.0f, 0.0f);
		if ((mousePos.x >= ((WIDTH - exitSize.x) / 2) && mousePos.x <= (((WIDTH - exitSize.x) / 2) + exitSize.x)) &&
			(mousePos.y >= (((HEIGHT - startGameSize.y) / 2) + startGameSize.y + 50.0f) && mousePos.y <= (((HEIGHT - startGameSize.y) / 2) + startGameSize.y + 50.0f) + exitSize.y)) {
			sceneManager->shouldExit = true;
		}
	}
}

MenuScene::~MenuScene() {

}

SceneManager::SceneManager(Font* font) : font(font) {
	scenes.insert({ "MenuScene", new MenuScene(this) });
	scenes.insert({ "GameScene", new GameScene(this) });
	currentActiveScene = scenes["MenuScene"];
}

Font* SceneManager::GetFont() const {
	return font;
}

void SceneManager::SetActiveSceneByName(const std::string& name) {
	if (scenes.find(name) != scenes.end()) {
		currentActiveScene = scenes[name];
	}
}

void SceneManager::ProcessInput() {
	currentActiveScene->Update();
}

void SceneManager::RenderActiveScene() {
	currentActiveScene->Render();
}

SceneManager::~SceneManager() {
	delete scenes["MenuScene"];
	delete scenes["GameScene"];
}