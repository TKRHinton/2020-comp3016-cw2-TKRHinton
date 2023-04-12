#pragma once
#include <string>  
#include <glm/glm.hpp> //includes GLM
#include <iostream>

#ifndef TETRIS_H
#define TETRIS_H
static int newShape;

class Tetris {
public:
	//Default Constructor 
	Tetris();

	std::string getType() {
		return this->type;
	}
	glm::vec3 getPosition() {
		return this->currentPosition;
	}
	glm::vec3 getOldPosition() {
		return this->oldPosition;
	}
	glm::vec3 getRotation() {
		return this->currentRotation;
	}
	glm::vec3 getOldRotation() {
		return this->oldRotation;
	}

	void setType() {
		newShape = std::rand() % 5;

		switch (newShape) {
		case 0:
			this->type = "L";
			break;
		case 1:
			this->type = "S";
			break;
		case 2:
			this->type = "T";
			break;
		case 3:
			this->type = "Z";
			break;
		case 4:
			this->type = "I";
			break;
		}	
	}
	void setPosition(glm::vec3 position) {
		this->currentPosition = position;
	}
	void setOldPosition(glm::vec3 position) {
		this->oldPosition = position;
	}
	void setRotation(glm::vec3 rotation) {
		this->currentRotation = rotation;
	}
	void setOldRotation(glm::vec3 rotation) {
		this->oldRotation = rotation;
	}

private:
	std::string type; //L I Squre the other one
	glm::vec3 oldPosition;
	glm::vec3 currentPosition;
	glm::vec3 oldRotation;
	glm::vec3 currentRotation;
};
#endif // !TETRIS_H