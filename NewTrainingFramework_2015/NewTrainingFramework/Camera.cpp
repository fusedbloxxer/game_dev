#include "stdafx.h"
#include "Camera.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, Vector3& vec) {
	os << vec.x << ' ' << vec.y << ' ' << vec.z << '\n';
	return os;
}

Camera::Type Camera::atot(const char* str)
{
	if (strcmp(str, "firstPerson") == 0) {
		return Camera::Type::FIRST_PERSON;
	}
	else if (strcmp(str, "thirdPerson") == 0) {
		return Camera::Type::THIRD_PERSON;
	}
	else {
		std::cerr << "INVALID STRING-CAMERA::TYPE CONVERSION: " << str << std::endl;
		abort();
	}
}

Camera::Camera(GLfloat width, GLfloat height, GLint id, Vector3& position, Vector3& target, Vector3& up, GLfloat moveSpeed, GLfloat rotateSpeed, GLfloat nearP, GLfloat farP, GLfloat fov, GLfloat deltaTime, Type type)
	:id{ id }, target{ target }, position{ position }, up{ up }, moveSpeed{ moveSpeed }, rotateSpeed{ rotateSpeed }, nearP{ nearP }, farP{ farP }, fov{ fov }, deltaTime{ deltaTime }, type{ type }
{
	projectionMatrix.SetPerspective(RAD(fov), (GLfloat)width / (GLfloat)height, nearP, farP);
	refreshAxis();
}

void Camera::moveOz(GLint directie)
{
	Vector3 vectorDeplasare = zAxis * directie * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	refreshAxis();
}

void Camera::moveOx(GLint directie)
{
	Vector3 vectorDeplasare = xAxis * directie * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	refreshAxis();
}

void Camera::moveOy(GLint directie)
{
	Vector3 vectorDeplasare = yAxis * directie * moveSpeed * deltaTime;
	position += vectorDeplasare;
	target += vectorDeplasare;
	refreshAxis();
}

void Camera::rotateOy(GLint directie)
{
	Matrix mRotateOy;
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOy.SetRotationY(directie * rotateSpeed * deltaTime);
	Vector4 result = rotatedTarget * worldMatrix;

	target = Vector3(result.x, result.y, result.z);
	refreshAxis();
}

void Camera::rotateOx(GLint directie)
{
	Matrix mRotateOx; mRotateOx.SetRotationX(directie * rotateSpeed * deltaTime);

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

void Camera::rotateOz(GLint directie)
{
	Matrix mRotateOz; mRotateOz.SetRotationZ(directie * rotateSpeed * deltaTime);
	
	Vector4 rotateLocalUp = Vector4{ 0.0f, 1.0f, 0.0f, 0.0f } * mRotateOz;
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

	// M
	worldMatrix = R * T;
	// V - se returneaza
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
}

GLfloat Camera::getFar() const
{
	return farP;
}

void Camera::setFar(GLfloat farP)
{
	this->farP = farP;
}

GLfloat Camera::getFov() const
{
	return fov;
}

void Camera::setFov(GLfloat fov)
{
	this->fov = fov;
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

void Camera::refreshAxis()
{
	zAxis = -(target - position).Normalize();
	yAxis = up.Normalize();
	xAxis = zAxis.Cross(yAxis).Normalize();
	updateWorldView();
}

std::ostream& operator<<(std::ostream& os, const Camera& camera)
{
	os << "Camera: [id = " << camera.id << ", type = " << camera.type << ", "
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
