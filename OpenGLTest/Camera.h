#pragma once

//GLEW
#include <GL/glew.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

using namespace glm;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//Default camera values
const GLfloat YAW        = -90.0f;//
const GLfloat PITCH      = 0.0f;
const GLfloat SPEED		 = 3.0f;
const GLfloat SENSITIVTY = 0.25;
const GLfloat ZOOM		 = 45.0f;


class Camera
{
public:
	//Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	//Eular Angles
	GLfloat Yaw;
	GLfloat Pitch;

	//Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	//Constructor with vectors
	Camera(vec3 position = vec3(0.0f, 0.0f, 0.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH)
	{		
		this->MovementSpeed = SPEED;
		this->MouseSensitivity = SENSITIVTY;
		this->Zoom = ZOOM;

		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
	
		//this->Front = glm::vec3(0.0f, 0.0f, -3.0f);
		//Update
		this->UpdateCameraVectors();
	}
	
	//Get view matrix
	mat4 GetViewMatrix()
	{
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	//Keyboard event
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->MovementSpeed * deltaTime; //deltaTime: save the costing time of previours frame
		if (direction == FORWARD)
			this->Position += this->Front * velocity; //Scene to backward
		if (direction == BACKWARD)
			this->Position -= this->Front * velocity; //Scene to forward
		if (direction == LEFT)
			this->Position -= this->Right * velocity; //Scene to right
		if (direction == RIGHT)
			this->Position += this->Right * velocity; //Scene to left
	}

	//Mouse event
	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true)
	{
		xOffset *= this->MouseSensitivity;
		yOffset *= this->MouseSensitivity;

		this->Yaw += xOffset;
		this->Pitch += yOffset;

		//Limit pitch
		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
		}

		//Update Front, Right and Up Vectors using the update Eular angles
		this->UpdateCameraVectors();
	}

	//Scroll event
	void ProcessMouseScroll(GLfloat yOffset)
	{
		if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
			this->Zoom -= yOffset;
		if (this->Zoom <= 1.0f)
			this->Zoom = 1.0f;
		if (this->Zoom >= 45.0f)
			this->Zoom = 45.0f;
	}

private:
	//Calulates the front vector from the Camera's Eular Angles
	void UpdateCameraVectors()
	{
		//Calulate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->Pitch)) * cos(glm::radians(this->Yaw));
		front.y = sin(glm::radians(this->Pitch));
		front.z = cos(glm::radians(this->Pitch)) * sin(glm::radians(this->Yaw));

		this->Front = glm::normalize(front);

		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));

		this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	}
};