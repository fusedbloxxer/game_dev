#include "stdafx.h"
#include "Controls.h"
#include "Camera.h"
#include <iostream>

Camera::Type Camera::atot(const char* str)
{
	if (strcmp(str, "firstPerson") == 0) {
		return Camera::Type::FIRST_PERSON;
	}
	else if (strcmp(str, "thirdPerson") == 0) {
		return Camera::Type::THIRD_PERSON;
	}
	else {
		throw std::runtime_error{ "INVALID STRING-CAMERA::TYPE CONVERSION" };
	}
}

const char* Camera::ttoa(const Type& type)
{
	switch (type)
	{
	case Camera::Type::FIRST_PERSON: return "FIRST_PERSON";
	case Camera::Type::THIRD_PERSON: return "THIRD_PERSON";
	default: throw std::invalid_argument{ "Invalid Camera-string::Type conversion" };
	}
}

Camera::Camera(GLfloat width, GLfloat height, GLint id, const Vector3& position, const Vector3& target, const Vector3& up, GLfloat moveSpeed, GLfloat rotateSpeed, GLfloat nearP, GLfloat farP, GLfloat fov, GLfloat deltaTime, Type type)
	:width{ width }, height{ height }, id{ id }, target{ target }, position{ position }, up{ up }, moveSpeed{ moveSpeed }, rotateSpeed{ rotateSpeed }, nearP{ nearP }, farP{ farP }, fov{ fov }, deltaTime{ deltaTime }, type{ type },
	perspModified{ true }
{
	refreshAxis();
}

void Camera::moveOz(GLfloat directie)
{
	Vector3 vectorDeplasare = zAxis * directie * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	refreshAxis();
}

void Camera::moveOx(GLfloat directie)
{
	Vector3 vectorDeplasare = xAxis * directie * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	refreshAxis();
}

void Camera::moveOy(GLfloat directie)
{
	Vector3 vectorDeplasare = yAxis * directie * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	refreshAxis();
}

void Camera::rotateOy(GLfloat directie)
{
	Matrix mRotateOy;
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOy.SetRotationY(TO_RAD(directie * rotateSpeed * deltaTime));
	Vector4 result = rotatedTarget * worldMatrix;

	target = Vector3(result.x, result.y, result.z);
	refreshAxis();
}

void Camera::rotateOx(GLfloat directie)
{
	Matrix mRotateOx; mRotateOx.SetRotationX(TO_RAD(directie * rotateSpeed * deltaTime));

	Vector4 rotatedLocalUp = Vector4(0.0f, 1.0f, 0.0f, 0.0f) * mRotateOx;
	Vector4 resultUp = rotatedLocalUp * worldMatrix;
	up = Vector3(resultUp.x, resultUp.y, resultUp.z);
	up.Normalize();

	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotateLocalTarget = localTarget * mRotateOx;
	Vector4 result = rotateLocalTarget * worldMatrix;
	target = Vector3(result.x, result.y, result.z);

	refreshAxis();
}

void Camera::rotateOz(GLfloat directie)
{
	Matrix mRotateOz; mRotateOz.SetRotationZ(TO_RAD(directie * rotateSpeed * deltaTime));

	Vector4 rotateLocalUp = Vector4{ 0.0f, 1.0f, 0.0f, 0.0f } *mRotateOz;
	Vector4 resultUp = rotateLocalUp * worldMatrix;
	up = Vector3(resultUp.x, resultUp.y, resultUp.z);

	refreshAxis();
}

void Camera::updateWorldView()
{
	Matrix R, T;

	R.m[0][0] = xAxis.x; R.m[0][1] = xAxis.y; R.m[0][2] = xAxis.z; R.m[0][3] = 0;
	R.m[1][0] = yAxis.x; R.m[1][1] = yAxis.y; R.m[1][2] = yAxis.z; R.m[1][3] = 0;
	R.m[2][0] = zAxis.x; R.m[2][1] = zAxis.y; R.m[2][2] = zAxis.z; R.m[2][3] = 0;
	R.m[3][0] = 0; R.m[3][1] = 0; R.m[3][2] = 0; R.m[3][3] = 1;

	T.SetTranslation(position.x, position.y, position.z);

	worldMatrix = R * T;
	viewMatrix = T.SetTranslation(-position.x, -position.y, -position.z) * R.Transpose();
}

Matrix& Camera::getViewMatrix()
{
	return viewMatrix;
}

Matrix& Camera::getWorldMatrix()
{
	return worldMatrix;
}

Matrix& Camera::getProjMatrix()
{
	if (perspModified) {
		projectionMatrix.SetPerspective(TO_RAD(fov), (GLfloat)width / (GLfloat)height, nearP, farP);
		perspModified = false;
	}
	return projectionMatrix;
}

Vector3 Camera::getTarget()
{
	return target;
}

void Camera::setTarget(Vector3& target)
{
	this->target = target;
	refreshAxis();
}

Vector3 Camera::getPosition()
{
	return position;
}

void Camera::setPosition(Vector3& position)
{
	this->position = position;
	refreshAxis();
}

Vector3 Camera::getUp()
{
	return up;
}

void Camera::setUp(Vector3& up)
{
	this->up = up;
	refreshAxis();
}

GLfloat Camera::getRotateSpeed() const
{
	return rotateSpeed;
}

void Camera::setRotateSpeed(GLfloat rotateSpeed)
{
	this->rotateSpeed = rotateSpeed;
}

GLfloat Camera::getDeltaTime() const
{
	return deltaTime;
}

void Camera::setDeltaTime(GLfloat deltaTime)
{
	this->deltaTime = deltaTime;
}

GLfloat Camera::getMoveSpeed() const
{
	return moveSpeed;
}

void Camera::setMoveSpeed(GLfloat moveSpeed)
{
	this->moveSpeed = moveSpeed;
}

GLfloat Camera::getNear() const
{
	return nearP;
}

void Camera::setNear(GLfloat nearP)
{
	this->nearP = nearP;
	perspModified = true;
}

GLfloat Camera::getFar() const
{
	return farP;
}

void Camera::setFar(GLfloat farP)
{
	this->farP = farP;
	perspModified = true;
}

GLfloat Camera::getFov() const
{
	return fov;
}

void Camera::setFov(GLfloat fov)
{
	this->fov = fov;
	perspModified = true;
}

Camera::Type Camera::getType() const
{
	return type;
}

void Camera::setType(Type type)
{
	this->type = type;
}

GLint Camera::getCameraId() const
{
	return id;
}

void Camera::execute(Controls::Type key)
{
	switch (key)
	{
	case Controls::Type::MOVE_CAMERA_POSITIVE_X:
		this->moveOx(1);
		break;
	case Controls::Type::MOVE_CAMERA_NEGATIVE_X:
		this->moveOx(-1);
		break;
	case Controls::Type::MOVE_CAMERA_POSITIVE_Y:
		this->moveOy(1);
		break;
	case Controls::Type::MOVE_CAMERA_NEGATIVE_Y:
		this->moveOy(-1);
		break;
	case Controls::Type::MOVE_CAMERA_POSITIVE_Z:
		this->moveOz(-1);
		break;
	case Controls::Type::MOVE_CAMERA_NEGATIVE_Z:
		this->moveOz(1);
		break;
	case Controls::Type::ROTATE_CAMERA_POSITIVE_X:
		this->rotateOx(1);
		break;
	case Controls::Type::ROTATE_CAMERA_NEGATIVE_X:
		this->rotateOx(-1);
		break;
	case Controls::Type::ROTATE_CAMERA_POSITIVE_Y:
		this->rotateOy(-1);
		break;
	case Controls::Type::ROTATE_CAMERA_NEGATIVE_Y:
		this->rotateOy(1);
		break;
	case Controls::Type::ROTATE_CAMERA_POSITIVE_Z:
		this->rotateOz(-1);
		break;
	case Controls::Type::ROTATE_CAMERA_NEGATIVE_Z:
		this->rotateOz(1);
		break;
	default:
		break;
	}
}

void Camera::refreshAxis()
{
	zAxis = -(target - position).Normalize();
	yAxis = up.Normalize();
	xAxis = zAxis.Cross(yAxis).Normalize();
	updateWorldView();
}

std::ostream& operator<<(std::ostream& os, const Camera& camera)
{
	os << "Camera: [id = " << camera.id << ", type = " << Camera::ttoa(camera.type) << ", "
		<< "position = (" << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << "), "
		<< "target = (" << camera.target.x << ", " << camera.target.y << ", " << camera.target.z << "), "
		<< "up = (" << camera.up.x << ", " << camera.up.y << ", " << camera.up.z << "), "
		<< "translationSpeed = " << camera.moveSpeed << ", "
		<< "rotationSpeed = " << camera.rotateSpeed << ", "
		<< "fov = " << camera.fov << ", "
		<< "near = " << camera.nearP << ", "
		<< "far = " << camera.farP << "]";

	return os;
}