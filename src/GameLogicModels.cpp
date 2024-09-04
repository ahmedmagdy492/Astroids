#include "Models.h"

Player::Player() : rotationAngle(90) {
	middlePosition = { (WIDTH - PLAYER_BASE_WIDTH) / 2, HEIGHT / 2, 0.0f };
	rightPosition = { middlePosition.x + (PLAYER_BASE_WIDTH / 2), middlePosition.y + PLAYER_TRI_HEIGHT, 0.0f };
	leftPosition = { middlePosition.x - (PLAYER_BASE_WIDTH / 2), middlePosition.y + PLAYER_TRI_HEIGHT, 0.0f };
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