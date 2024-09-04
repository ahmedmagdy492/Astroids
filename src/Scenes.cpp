#include "Models.h"

GameScene::GameScene(SceneManager* sceneManager) : Scene("GameScene") {
	this->sceneManager = sceneManager;
}

const std::string& GameScene::GetName() const {
	return name;
}

void GameScene::Render() {
	player.Draw();
}

void GameScene::Update() {
	float curRotAngle = player.GetRotationAngle();

	if (IsKeyPressed(KEY_RIGHT)) {
		player.RotatePlayer(PLAYER_ROT_DEG);
	}
	if (IsKeyPressed(KEY_LEFT)) {
		player.RotatePlayer(-PLAYER_ROT_DEG);
	}
	if (IsKeyPressed(KEY_UP)) {
		float angleInRadians = Utils::ConvertDegreesToRadians(curRotAngle);
		Vector3 dirVec = { 
			cosf(angleInRadians) * PLAYER_SPEED, 
			sinf(angleInRadians) * PLAYER_SPEED, 
			0.0f
		};

		player.Move({ dirVec.x * -1, dirVec.y * -1, 0.0f });
	}
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