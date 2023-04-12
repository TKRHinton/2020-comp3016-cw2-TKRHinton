#include "Tetris.h"
#include <algorithm>
#include <vector>
#include <string>  
#include <iostream>

Tetris::Tetris()
{
	this->type = "T";
	this->currentPosition = glm::vec3(0.f,6.f,-19.f);
	this->oldPosition = glm::vec3(0.f, 8.f, 0.f);
	this->currentRotation = glm::vec3(0.f, 0.f, 0.f);
	this->oldRotation = glm::vec3(0.f, 0.f, 0.f);
}
