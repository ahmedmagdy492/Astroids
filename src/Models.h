#pragma once

#include <unordered_map>
#include <iostream>
#include <cmath>

#ifdef __cplusplus
extern "C" {
	#include <raylib.h>
	#include <raymath.h>
}
#endif

#include "Shared.h"

class Utils {
public:
	static double ConvertDegreesToRadians(int deg)
	{
		return deg * (PI / 180.0f);
	}

	static float ConvertRadiansToDegrees(float angleInRad) {
		return RAD2DEG * angleInRad;
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

	static Vector3 MultiplyVectorWithMatrix(Vector3 vec, Matrix mat) {
		Quaternion quat = { vec.x, vec.y, vec.z, 1 };
		Quaternion result = {};

		result.x = quat.x * mat.m0 + quat.y * mat.m4 + quat.z * mat.m8 + quat.w * mat.m12;
		result.y = quat.x * mat.m1 + quat.y * mat.m5 + quat.z * mat.m9 + quat.w * mat.m13;
		result.z = quat.x * mat.m2 + quat.y * mat.m6 + quat.z * mat.m10 + quat.w * mat.m14;
		result.w = quat.x * mat.m3 + quat.y * mat.m7 + quat.z * mat.m11 + quat.w * mat.m15;

		return { result.x, result.y, result.z };
	}
};

class Player {
private:
	Vector3 middlePosition, leftPosition, rightPosition;
	float rotationAngle;

public:
	Player();

	float GetRotationAngle() const;
	void SetRotationAngle(float newAngle);

	Vector3 GetPosition() const;
	void SetPosition(Vector3 newPos);

	void RotatePlayer(float angle);
	void Move(Vector3 translateVector);

	void Draw();
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

public:
	SceneManager();
	void SetActiveSceneByName(const std::string& name);
	void ProcessInput();
	void RenderActiveScene();
	~SceneManager();
};

class GameScene : public Scene {
private:
	Player player;
	SceneManager* sceneManager;
public:
	GameScene(SceneManager* sceneManager);

	const std::string& GetName() const;

	void Render();
	void Update();
};