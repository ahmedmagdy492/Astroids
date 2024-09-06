#include "Models.h"

Player::Player() : rotationAngle(90), curVelocity({ PLAYER_VEL_X, PLAYER_VEL_Y, 0.0f}) {
	middlePosition = { (WIDTH - PLAYER_BASE_WIDTH) / 2, HEIGHT / 2, 0.0f };
	rightPosition = { middlePosition.x + (PLAYER_BASE_WIDTH / 2), middlePosition.y + PLAYER_TRI_HEIGHT, 0.0f };
	leftPosition = { middlePosition.x - (PLAYER_BASE_WIDTH / 2), middlePosition.y + PLAYER_TRI_HEIGHT, 0.0f };
}

void Player::ResetState() {
	rotationAngle = 90;
	curVelocity = { PLAYER_VEL_X, PLAYER_VEL_Y, 0.0f };
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

Vector3 Player::GetLeftPosition() const {
	return leftPosition;
}

Vector3 Player::GetRightPosition() const {
	return rightPosition;
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

int Player::GetLivesNo() const {
	return lives;
}

void Player::SetLivesNo(unsigned char newLives) {
	lives = newLives;
}

int Player::GetScore() const {
	return score;
}

void Player::SetScore(unsigned int newScore) {
	score = newScore;
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

Astroid::Astroid(Vector3 centerPoint) : color(WHITE), centerPoint(centerPoint) {
	ArrangePointsPositionsBasedOnCenterPoint();
}

void Astroid::ToggleIsMoving() {
	isMoving = !isMoving;
}

bool Astroid::IsMoving() const {
	return isMoving;
}

bool Astroid::IsPointInsidePolygon(Vector3 middlePosition, Vector3* points) {
	bool inside = false;

	for (int i = 0, j = NO_OF_POINTS_IN_ASTROID - 1; i < NO_OF_POINTS_IN_ASTROID; j = i++) {
		if (((points[i].y > middlePosition.y) != (points[j].y > middlePosition.y)) &&
			(middlePosition.x < (points[j].x - points[i].x) * (middlePosition.y - points[i].y) / (points[j].y - points[i].y) + points[i].x)) {
			inside = !inside;
		}
	}

	return inside;
}

bool Astroid::IsCollidingWithPlayer(const Player& player) {
	Vector3 middlePosition = player.GetPosition(), leftPosition = player.GetLeftPosition(), rightPosition = player.GetRightPosition();

	float longestAstroidRadius = INT_MIN;
	for (int i = 0; i < NO_OF_POINTS_IN_ASTROID; ++i) {
		if (pointsRadiuses[i] > longestAstroidRadius) {
			longestAstroidRadius = pointsRadiuses[i];
		}
	}

	float distBetweenCPAndMP = Utils::DistanceBetween(centerPoint, middlePosition);
	//DrawLine(centerPoint.x, centerPoint.y, middlePosition.x, middlePosition.y, YELLOW);
	/*DrawText(TextFormat("r: %02i", (int)longestAstroidRadius), centerPoint.x, centerPoint.y, 22, BLUE);*/
	if (distBetweenCPAndMP < longestAstroidRadius) {
		return true;
	}
	float distBetweenCPAndLP = Utils::DistanceBetween(centerPoint, leftPosition);
	//DrawLine(centerPoint.x, centerPoint.y, leftPosition.x, leftPosition.y, YELLOW);
	if (distBetweenCPAndLP < longestAstroidRadius) {
		return true;
	}
	float distBetweenCPAndrRP = Utils::DistanceBetween(centerPoint, rightPosition);
	//DrawLine(centerPoint.x, centerPoint.y, rightPosition.x, rightPosition.y, YELLOW);
	if (distBetweenCPAndrRP < longestAstroidRadius) {
		return true;
	}

	return false;
}

void Astroid::ArrangePointsPositionsBasedOnCenterPoint() {
	float angleToRotateWith = 0.0f;
	for (int i = 0; i < NO_OF_POINTS_IN_ASTROID; ++i) {
		float randShapeRadius = GetRandomValue(10, MAX_ASTROID_RADIUS);
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
	float longestRadius = 0.0f;
	for (; i < NO_OF_POINTS_IN_ASTROID - 1; ++i) {
		if (pointsRadiuses[i] > longestRadius) {
			longestRadius = pointsRadiuses[i];
		}
		DrawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, color);
	}
	DrawLine(points[i].x, points[i].y, points[0].x, points[0].y, color);
	//DrawCircleLines(centerPoint.x, centerPoint.y, longestRadius, RED);
}

void Astroid::Move(Vector3 translateVec) {
	Matrix translateMat = MatrixTranslate(translateVec.x, translateVec.y, translateVec.z);
	for (int i = 0; i < NO_OF_POINTS_IN_ASTROID; ++i) {
		Vector3 translatedVec = Vector3Transform(points[i], translateMat);
		points[i] = translatedVec;
	}
	centerPoint = Vector3Transform(centerPoint, translateMat);
}

void Astroid::ResetCenterPoint(Vector3 newCenterPoint) {
	this->centerPoint = newCenterPoint;
	ArrangePointsPositionsBasedOnCenterPoint();
}

bool Astroid::IsCollidingWithBullet(Bullet* bullet) {
	float longestAstroidRadius = INT_MIN;
	for (int i = 0; i < NO_OF_POINTS_IN_ASTROID; ++i) {
		if (pointsRadiuses[i] > longestAstroidRadius) {
			longestAstroidRadius = pointsRadiuses[i];
		}
	}

	Vector3 bulletPos = bullet->GetPosition();
	Vector3 topLeftBulletPoint = { bulletPos.x, bulletPos.y, 0.0f };
	Vector3 topRightBulletPoint = { bulletPos.x + BULLET_SIZE, bulletPos.y, 0.0f };
	Vector3 bottomLeftBulletPoint = { bulletPos.x, bulletPos.y + BULLET_SIZE, 0.0f };
	Vector3 bottomRightBulletPoint = { bulletPos.x + BULLET_SIZE, bulletPos.y + BULLET_SIZE, 0.0f };

	float distBetweenCPAndTLP = Utils::DistanceBetween(centerPoint, topLeftBulletPoint);
	if (distBetweenCPAndTLP < longestAstroidRadius) {
		return true;
	}
	float distBetweenCPAndTRP = Utils::DistanceBetween(centerPoint, topRightBulletPoint);
	if (distBetweenCPAndTRP < longestAstroidRadius) {
		return true;
	}
	float distBetweenCPAndrBLP = Utils::DistanceBetween(centerPoint, bottomLeftBulletPoint);
	if (distBetweenCPAndrBLP < longestAstroidRadius) {
		return true;
	}
	float distBetweenCPAndrBRP = Utils::DistanceBetween(centerPoint, bottomRightBulletPoint);
	if (distBetweenCPAndrBRP < longestAstroidRadius) {
		return true;
	}

	return false;
}

Astroid::~Astroid() {

}

Bullet::Bullet(Vector3 position, float rotationAngle, Color color) : color(color), rotationAngle(rotationAngle), position(position) {
}

float Bullet::GetRotationAngle() {
	return rotationAngle;
}

void Bullet::SetRotationAngle(float newAngle) {
	rotationAngle = newAngle;
}

Vector3 Bullet::GetPosition() {
	return position;
}

void Bullet::Move(Vector3 velocity) {
	Matrix translateMat = MatrixTranslate(velocity.x, velocity.y, velocity.z);
	position = Vector3Transform(position, translateMat);
}

void Bullet::Draw() {
	DrawRectangle(position.x, position.y, BULLET_SIZE, BULLET_SIZE, color);
}

void Bullet::ResetPosition(Vector3 position) {
	this->position = position;
}