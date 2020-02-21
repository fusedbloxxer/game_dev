#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h" 
#include "SceneObject.h"
#include "Collidable.h"
#include "Controls.h"
#include "Globals.h"
#include <ostream>

class Camera : public Collidable
{
public:
	// Camera Type
	enum class Type {
		FIRST_PERSON,
		THIRD_PERSON
	};

private:
	// Flag if perspective was modified
	GLboolean perspModified;

	// Screen size
	GLfloat width, height;

	Type type; GLint id;
	Vector3 target, position, up, xAxis, yAxis, zAxis;
	Matrix viewMatrix, worldMatrix, projectionMatrix;
	GLfloat moveSpeed, rotateSpeed, nearP, farP, fov, deltaTime;

	friend std::ostream& operator<<(std::ostream& os, const Camera& camera);

public:
	static Type atot(const char* str);
	static const char* ttoa(const Type& type);

	// Has default values.
	Camera(GLfloat width, GLfloat height, GLint id, const Vector3& position = Vector3{ 0.0f, 0.0f, 0.0f }, const Vector3& target = Vector3{ 0.0f, 0.0f, 0.0f }, const Vector3& up = Vector3{ 0.0f, 1.0f, 0.0f }, GLfloat moveSpeed = 3.0f, GLfloat rotateSpeed = 3.0f, GLfloat nearP = 0.2f, GLfloat farP = 10000.0f, GLfloat fov = 45.0f, GLfloat deltaTime = 0.0f, Type type = Type::FIRST_PERSON);

	// Move functions
	void moveOz(GLfloat directie);
	void moveOx(GLfloat directie);
	void moveOy(GLfloat directie);

	// Rotate functions
	void rotateOy(GLfloat directie);
	void rotateOx(GLfloat directie);
	void rotateOz(GLfloat directie);
	
	// Execute control operations
	void execute(Controls::Type key);

	// Getters and setters.
	void updateWorldView();

	Matrix& getViewMatrix();
	Matrix& getWorldMatrix();
	Matrix& getProjMatrix();
	
	Vector3 getTarget();
	void setTarget(Vector3&);

	Vector3 getPosition();
	void setPosition(Vector3&);

	Vector3 getUp();
	void setUp(Vector3&);

	GLfloat getRotateSpeed() const;
	void setRotateSpeed(GLfloat);

	GLfloat getDeltaTime() const;
	void setDeltaTime(GLfloat);

	GLfloat getMoveSpeed() const;
	void setMoveSpeed(GLfloat);

	GLfloat getNear() const;
	void setNear(GLfloat);

	GLfloat getFar() const;
	void setFar(GLfloat);

	GLfloat getFov() const;
	void setFov(GLfloat);

	Type getType() const;
	void setType(Type type);

	GLint getCameraId() const;

private:
	void refreshAxis();

	// Inherited via Collidable
	virtual bool collides(Collidable* object) const override;
};