#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include "Tema2.h"

/*
	Fisierul defineste functiile de construire a obiectelor si de detectare a coliziunilor.
*/
namespace Tema2Object3D
{
	Mesh* CreateParaleliped(std::string name, glm::vec3 corner, float height, float depth, float width, glm::vec3 color []);

	Mesh* CreateSphere(std::string name, glm::vec3 center, float radius, glm::vec3 color);

	Mesh* CreateCylinder(std::string name, glm::vec3 bottomCenter, float radius1, float radius2, float height, glm::vec3 color);

	float findTheta(Tac* tac, glm::vec3 upperRightCorner, Ball* ball);

	Mesh* CreateRect(std::string name, glm::vec3 leftBottomCorner, glm::vec3 normal, glm::vec3 tang, float lengthN, float lengthT, glm::vec3 color);
}

