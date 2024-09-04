#include "Models.h"

GameScene::GameScene(SceneManager* sceneManager) : Scene("GameScene") {
	this->sceneManager = sceneManager;

	for (int i = 0; i < ASTROIDS_POOL_SIZE; ++i) {
		int x = GetRandomValue(MAX_ASTROID_RADIUS - 100.0f, WIDTH + 100.0f);
		int y = GetRandomValue(-1050.0f, MAX_ASTROID_RADIUS);
		Vector3 shapeCenterPoint = { (float)x, (float)y, 0.0f };
		Astroid* astroid = new Astroid(shapeCenterPoint);
		astroids.push_back(astroid);
	}
}

const std::string& GameScene::GetName() const {
	return name;
}

void GameScene::Render() {
	player.Draw();

	for (int i = 0; i < ASTROIDS_SIZE; ++i) {
		astroids[i]->Draw();
	}

	elapsedSeconds += GetFrameTime();
}

void GameScene::Update() {
	float curRotAngle = player.GetRotationAngle();

	if (IsKeyPressed(KEY_RIGHT)) {
		player.RotatePlayer(PLAYER_ROT_DEG);
	}
	if (IsKeyPressed(KEY_LEFT)) {
		player.RotatePlayer(-PLAYER_ROT_DEG);
	}
	if (IsKeyDown(KEY_UP)) {
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
	}
	else if(IsKeyUp(KEY_UP)) {
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

			PlayerOffScreenDirection playerOffCondition = player.IsPlayerOffScreen(WIDTH, HEIGHT);
			switch (playerOffCondition) {
			case PlayerOffScreenDirection::PlayerOffWidth:
				break;
			case PlayerOffScreenDirection::PlayerOffZeroX:
				break;
			case PlayerOffScreenDirection::PlayerOffHeight:
				break;
			case PlayerOffScreenDirection::PlayerOffZeroY:
				break;
			}
		}
	}

	

	// astroids moving
	for (int i = 0; i < ASTROIDS_SIZE; ++i) {
		if (astroids[i]->IsMoving()) {
			Vector3* points = astroids[i]->GetPosition();
			int noOfPointsOutOfRange = 0;
			astroids[i]->Move({ ASTROID_SPEED, ASTROID_SPEED, 0.0f });
			for (int j = 0; j < NO_OF_POINTS_IN_ASTROID; ++j) {
				if ((points[j].x <= 0 || points[j].x >= WIDTH) && (points[j].y <= 0 || points[j].y >= HEIGHT)) {
					++noOfPointsOutOfRange;
				}
			}

			if (noOfPointsOutOfRange == NO_OF_POINTS_IN_ASTROID) {
				// the astroid is totally out of sight
				int x = GetRandomValue(MAX_ASTROID_RADIUS, WIDTH);
				int y = GetRandomValue(-50.0f, MAX_ASTROID_RADIUS);
				astroids[i]->ResetCenterPoint({ (float)x, (float)y, 0.0f });
			}
		}
	}
}

GameScene::~GameScene() {
	for (int i = 0; i < astroids.size(); ++i) {
		delete astroids[i];
	}
	astroids.clear();
}

SceneManager::SceneManager() {
	scenes.insert({ "GameScene", new GameScene(this) });
	currentActiveScene = scenes["GameScene"];
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
	delete scenes["GameScene"];
}