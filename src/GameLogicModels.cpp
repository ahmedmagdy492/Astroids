#include "Models.h"

Player::Player() : rotationAngle(90), curVelocity({ PLAYER_VEL_X, PLAYER_VEL_Y, 0.0f}) {
	middlePosition = { (WIDTH - PLAYER_BASE_WIDTH) / 2, HEIGHT / 2, 0.0f };
	rightPosition = { middlePosition.x + (PLAYER_BASE_WIDTH / 2), middlePosition.y + PLAYER_TRI_HEIGHT, 0.0f };
	leftPosition = { middlePosition.x - (PLAYER_BASE_WIDTH / 2), middlePosition.y + PLAYER_TRI_HEIGHT, 0.0f };
}

Vector3 Player::GetVelocity() const {
	return curVelocity;
}

void Player::SetVelocity(Vector3 newVel) {
	curVelocity = newVel;
}

float Player::GetRotationAngle() const {
	return rotationAngle;
}

void Player::SetRotationAngle(float newAngle) {
	rotationAngle = newAngle;
}

Vector3 Player::GetPosition() const {
	return middlePosition;
}

void Player::SetPosition(Vector3 newPos) {
	middlePosition = newPos;
}

void Player::RotatePlayer(float angle) {
	SetRotationAngle(GetRotationAngle() + angle);

	// translating the object to the screen origin
	/*Vector3 playerCenterVector = { middlePosition.x, middlePosition.y + (PLAYER_TRI_HEIGHT / 2), 0.0f };*/
	Vector3 playerCenterVector = { 
		(leftPosition.x + middlePosition.x + rightPosition.x) / 3, 
		(leftPosition.y + middlePosition.y + rightPosition.y) / 3, 
		0.0f };
	Matrix transMat = MatrixTranslate(-playerCenterVector.x, -playerCenterVector.y, 0.0f);

	middlePosition = Vector3Transform(middlePosition, transMat);
	leftPosition = Vector3Transform(leftPosition, transMat);
	rightPosition = Vector3Transform(rightPosition, transMat);

	// rotating the vector at the origin
	float angleInRad = Utils::ConvertDegreesToRadians(angle);
	
	Matrix rotationMat = MatrixRotateZ(angleInRad);
	middlePosition = Vector3Transform(middlePosition, rotationMat);
	rightPosition = Vector3Transform(rightPosition, rotationMat);
	leftPosition = Vector3Transform(leftPosition, rotationMat);

	// translating back the vector
	Matrix transBackMat = MatrixTranslate(playerCenterVector.x, playerCenterVector.y, 0);
	middlePosition = Vector3Transform(middlePosition, transBackMat);
	leftPosition = Vector3Transform(leftPosition, transBackMat);
	rightPosition = Vector3Transform(rightPosition, transBackMat);
}

void Player::Move(Vector3 translateVector) {
	Matrix transMat = MatrixTranslate(translateVector.x, translateVector.y, 0.0f);

	middlePosition = Vector3Transform(middlePosition, transMat);
	leftPosition = Vector3Transform(leftPosition, transMat);
	rightPosition = Vector3Transform(rightPosition, transMat);
}

void Player::Draw() {
	DrawTriangle({ rightPosition.x, rightPosition.y }, { middlePosition.x, middlePosition.y }, { leftPosition.x, leftPosition.y }, GRAY);
}

PlayerOffScreenDirection Player::IsPlayerOffScreen(int width, int height) {
	if ((middlePosition.x > width && leftPosition.x > width && rightPosition.x > width)) {
		return PlayerOffScreenDirection::PlayerOffWidth;
	}
	if ((middlePosition.x < 0 && leftPosition.x < 0 && rightPosition.x < 0)) {
		return PlayerOffScreenDirection::PlayerOffZeroX;
	}

	if ((middlePosition.y > height && leftPosition.y > height && rightPosition.y > height)) {
		return PlayerOffScreenDirection::PlayerOffHeight;
	}
	if ((middlePosition.y < 0 && leftPosition.y < 0 && rightPosition.y < 0)) {
		return PlayerOffScreenDirection::PlayerOffZeroY;
	}
	return PlayerOffScreenDirection::PlayerOffNone;
}

Astroid::Astroid(Vector3 centerPoint) : color(WHITE), centerPoint(centerPoint), maxShapeRadius(MAX_ASTROID_RADIUS) {
	ArrangePointsPositionsBasedOnCenterPoint();
}

void Astroid::ToggleIsMoving() {
	isMoving = !isMoving;
}

bool Astroid::IsMoving() const {
	return isMoving;
}

void Astroid::ArrangePointsPositionsBasedOnCenterPoint() {
	float angleToRotateWith = 0.0f;
	for (int i = 0; i < NO_OF_POINTS_IN_ASTROID; ++i) {
		float randShapeRadius = GetRandomValue(10, (int)maxShapeRadius);
		pointsRadiuses[i] = randShapeRadius;
		points[i] = {
			(cosf(Utils::ConvertDegreesToRadians(angleToRotateWith)) * randShapeRadius) + centerPoint.x,
			(sinf(Utils::ConvertDegreesToRadians(angleToRotateWith)) * randShapeRadius) + centerPoint.y,
			0.0f
		};
		angleToRotateWith += 45.0f;
	}
}

Vector3* Astroid::GetPosition() const {
	return (Vector3*)points;
}

void Astroid::Draw() {
	int i = 0;
	for (; i < NO_OF_POINTS_IN_ASTROID - 1; ++i) {
		DrawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, color);
	}
	DrawLine(points[i].x, points[i].y, points[0].x, points[0].y, color);
}

void Astroid::Move(Vector3 translateVec) {
	Matrix translateMat = MatrixTranslate(translateVec.x, translateVec.y, translateVec.z);
	for (int i = 0; i < NO_OF_POINTS_IN_ASTROID; ++i) {
		Vector3 translatedVec = Vector3Transform(points[i], translateMat);
		points[i] = translatedVec;
	}
}

void Astroid::ResetCenterPoint(Vector3 newCenterPoint) {
	this->centerPoint = newCenterPoint;
	ArrangePointsPositionsBasedOnCenterPoint();
}

Astroid::~Astroid() {

}