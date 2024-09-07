#pragma once

#include <unordered_map>
#include <stack>
#include <iostream>
#include <cmath>

#ifdef __cplusplus
extern "C" {
	#include <raylib.h>
	#include <raymath.h>
}
#endif

#include "Shared.h"

enum PlayerOffScreenDirection : unsigned char {
	PlayerOffNone,
	PlayerOffWidth,
	PlayerOffZeroX,
	PlayerOffHeight,
	PlayerOffZeroY
};

class Utils {
public:
	static bool IsPositionOffScreen(Vector3 position, int width, int height) {
		if (position.x < 0 || position.x > width) {
			return true;
		}
		if (position.y < 0 || position.y > height) {
			return true;
		}

		return false;
	}

	static double ConvertDegreesToRadians(int deg)
	{
		return deg * (PI / 180.0f);
	}

	static float ConvertRadiansToDegrees(float angleInRad) {
		return RAD2DEG * angleInRad;
	}

	static float DistanceBetween(Vector3 point1, Vector3 point2) {
		return sqrtf((point2.x - point1.x) * (point2.x - point1.x) + (point2.y - point1.y) * (point2.y - point1.y));
	}

	static float GetVectorLength(Vector3 v) {
		return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	}

	static Vector3 NormalizeVector(Vector3 v) {
		float len = GetVectorLength(v);
		if (len == 0) {
			return { 0.0f, 0.0f, 0.0f };
		}
		return { v.x / len, v.y / len, v.z / len };
	}

	static Vector3 UnNormalizeVector(Vector3 vec, Vector3 originalVec) {
		float len = GetVectorLength(originalVec);
		return { vec.x * len, vec.y * len, vec.z * len };
	}
};

class Player {
private:
	Vector3 middlePosition, leftPosition, rightPosition;
	float rotationAngle;
	Vector3 curVelocity;
	unsigned char lives = PLAYER_LIVES;
	unsigned short score = 0;
	bool showFuel = false;

public:
	Player();

	float GetRotationAngle() const;
	void SetRotationAngle(float newAngle);

	int GetScore() const;
	void SetScore(unsigned int newScore);

	void ShowFuel();
	void HideFuel();

	int GetLivesNo() const;
	void SetLivesNo(unsigned char newLives);

	Vector3 GetPosition() const;
	void SetPosition(Vector3 newPos);

	void Reposition(PlayerOffScreenDirection playerOffCond);

	Vector3 GetLeftPosition() const;
	Vector3 GetRightPosition() const;

	Vector3 GetVelocity() const;
	void SetVelocity(Vector3 newVel);

	void RotatePlayer(float angle);
	void Move(Vector3 translateVector);
	PlayerOffScreenDirection IsPlayerOffScreen(int width, int height);

	void ResetState();

	void Draw();
};

class Bullet {
private:
	Vector3 position;
	Color color;
	float rotationAngle;

public:
	Bullet(Vector3 position, float rotationAngle, Color color);

	Vector3 GetPosition();
	float GetRotationAngle();
	void SetRotationAngle(float newAngle);

	void Move(Vector3 velocity);
	void Draw();

	void ResetPosition(Vector3 position);
};

class Astroid {
private:
	float pointsRadiuses[NO_OF_POINTS_IN_ASTROID] = {0.0f};
	Vector3 centerPoint;
	Vector3 points[NO_OF_POINTS_IN_ASTROID] = { 0.0f };
	Color color;
	bool isMoving = false;

	void ArrangePointsPositionsBasedOnCenterPoint();

public:
	Astroid(Vector3 centerPoint);

	Vector3* GetPosition() const;

	void ToggleIsMoving();

	bool IsPointInsidePolygon(Vector3 middlePosition, Vector3* points);

	bool IsCollidingWithBullet(Bullet* bullet);

	bool IsMoving() const;

	bool IsCollidingWithPlayer(const Player& player);

	void Draw();
	void Move(Vector3 translateVector);

	void ResetCenterPoint(Vector3 newCenterPoint);

	~Astroid();
};

class Scene {
protected:
	std::string name;

public:
	Scene(const std::string& name) : name(name) {
	}

	virtual void Render() = 0;
	virtual void Update() = 0;
};

class SceneManager {
private:
	Scene* currentActiveScene = nullptr;
	std::unordered_map<std::string, Scene*> scenes;
	Font* font;

public:
	bool shouldExit = false;

	SceneManager(Font* font);
	void SetActiveSceneByName(const std::string& name, bool hideCursor = false);
	Font* GetFont() const;
	void ProcessInput();
	void RenderActiveScene();
	~SceneManager();
};

class GameScene : public Scene {
private:
	Player player;
	SceneManager* sceneManager;
	std::vector<Astroid*> astroids;
	std::stack<Astroid*> astroidsToRemove;
	std::stack<Bullet*> bulletsPool;
	std::stack<Bullet*> bulletsToRemove;
	std::vector<Bullet*> bullets;
	double elapsedSeconds = 0.0;
	bool isPlayerCollided = false;
	bool isGameOver = false;
	float shootingAngle;

	Sound fireSound;
	Sound thrustSound;
	Sound bangSmallSound;
	Sound bangMediumSound;

public:
	GameScene(SceneManager* sceneManager);

	const std::string& GetName() const;

	void DrawUI() const;

	void Render();
	void Update();

	void ResetScene();

	~GameScene();
};


class MenuScene : public Scene {
private:
	SceneManager* sceneManager;

public:
	MenuScene(SceneManager* sceneManager);

	const std::string& GetName() const;


	void Render();
	void Update();

	~MenuScene();
};