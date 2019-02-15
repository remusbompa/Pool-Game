#include "Tema2Object3D.h"
#include <Core/Engine.h>
#include "Tema2.h"

#include <vector>
#include <string>

#include <iostream>

//contruire paralelipiped cu fetele colorate diferit
Mesh* Tema2Object3D::CreateParaleliped(std::string name, glm::vec3 corner, float height, float depth, float width, glm::vec3 color[]){
	std::vector<VertexFormat> vertices = {};

	for (int i = 0; i < 6;i++) {
		vertices.push_back(VertexFormat(corner + glm::vec3(0, 0, width), color[i]));
		vertices.push_back(VertexFormat(corner + glm::vec3(height, 0, width), color[i]));
		vertices.push_back(VertexFormat(corner + glm::vec3(0, depth, width), color[i]));
		vertices.push_back(VertexFormat(corner + glm::vec3(height, depth, width), color[i]));
		vertices.push_back(VertexFormat(corner + glm::vec3(0, 0, 0), color[i]));
		vertices.push_back(VertexFormat(corner + glm::vec3(height, 0, 0), color[i]));
		vertices.push_back(VertexFormat(corner + glm::vec3(0, depth, 0), color[i]));
		vertices.push_back(VertexFormat(corner + glm::vec3(height, depth, 0), color[i]));
	}

	std::vector<unsigned short> indices =
	{
		8 * 0 + 0, 8 * 0 + 1, 8 * 0 + 2,		8 * 0 + 1, 8 * 0 + 3, 8 * 0 + 2,
		8 * 1 + 2, 8 * 1 + 3, 8 * 1 + 7,		8 * 1 + 2, 8 * 1 + 7, 8 * 1 + 6,
		8 * 2 + 1, 8 * 2 + 7, 8 * 2 + 3,		8 * 2 + 1, 8 * 2 + 5, 8 * 2 + 7,
		8 * 3 + 6, 8 * 3 + 7, 8 * 3 + 4,		8 * 3 + 7, 8 * 3 + 5, 8 * 3 + 4,
		8 * 4 + 0, 8 * 4 + 4, 8 * 4 + 1,		8 * 4 + 1, 8 * 4 + 4, 8 * 4 + 5,
		8 * 5 + 2, 8 * 5 + 6, 8 * 5 + 4,		8 * 5 + 0, 8 * 5 + 2, 8 * 5 + 4,
	};

	Mesh * mesh = new Mesh(name);
	mesh->InitFromData(vertices, indices);
	return mesh;
}

//construire sfera
Mesh* Tema2Object3D::CreateSphere(std::string name, glm::vec3 center, float radius, glm::vec3 color) {
	int nrFragments = 100;
	std::vector<VertexFormat> vertices = { VertexFormat(center, color) };
	for (int i = 0; i < nrFragments; i++) {
		float theta = RADIANS((float)(360 * i) / nrFragments);
		
		for (int j = 0; j < nrFragments; j++) {
			float phi = RADIANS((float)(360 * j) / nrFragments);
			float x = radius * cos(phi) * cos(theta);
			float y = radius * cos(phi) * sin(theta);
			float z = radius * sin(phi);
			vertices.push_back(VertexFormat(glm::vec3(center.x + x, center.y + y, center.z + z), color));
		}
		
	}

	std::vector<unsigned short> indices = {};

	//fetele cu acelasi theta
	for (int i = 1; i <= nrFragments; i++) {
		for (int j = 1; j < nrFragments; j++) {
			int nr = (i - 1) * nrFragments + j;
			indices.push_back(0);
			indices.push_back(nr);
			indices.push_back(nr + 1);
		}

		indices.push_back(0);
		indices.push_back(i * nrFragments);
		indices.push_back((i-1) * nrFragments + 1);
	}

	//fetele cu acelasi phi
	for (int j = 1; j <= nrFragments; j++) {
		for (int i = 1; i < nrFragments; i++) {
			int nr = (i - 1) * nrFragments + j;
			indices.push_back(0);
			indices.push_back(nr);
			indices.push_back(nr + nrFragments);
		}

		indices.push_back(0);
		indices.push_back(nrFragments * (nrFragments-1) +j);
		indices.push_back(j);
	}

	Mesh * mesh = new Mesh(name);
	mesh->InitFromData(vertices, indices);
	return mesh;
}

//construire cilindru, cu bottomCenter centrul bazei mici iar radius1 raza mare si radius2 baza mica
Mesh* Tema2Object3D::CreateCylinder(std::string name, glm::vec3 bottomCenter, float radius1, float radius2, float height, glm::vec3 color) {
	int nrFragments = 100;

	//cercul de jos
	std::vector<VertexFormat> vertices = { VertexFormat(bottomCenter, color) };
	for (int i = 0; i < nrFragments; i++) {
		float theta = RADIANS((float)(360 * i) / nrFragments);
		float y = radius2 * cos(theta), z = radius2 * sin(theta);
		vertices.push_back(VertexFormat(glm::vec3(bottomCenter.x, bottomCenter.y + y, bottomCenter.z + z), color));
	}
	//cercul de sus
	glm::vec3 upperCenter = bottomCenter + glm::vec3(-height,0,0);

	for (int i = 0; i < nrFragments; i++) {
		float theta = RADIANS((float)(360 * i) / nrFragments);
		float y = radius1 * cos(theta), z = radius1 * sin(theta);
		vertices.push_back(VertexFormat(glm::vec3(upperCenter.x, upperCenter.y + y, upperCenter.z + z), color));
	}

	vertices.push_back(VertexFormat(upperCenter, color));

	std::vector<unsigned short> indices = {};

	//primul cerc
	for (int i = 1; i < nrFragments; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	indices.push_back(0);
	indices.push_back(nrFragments);
	indices.push_back(1);

	//al doilea cerc
	for (int i = 1; i < nrFragments; i++) {
		indices.push_back(2 * nrFragments + 1);
		indices.push_back(nrFragments + i);
		indices.push_back(nrFragments + i + 1);
	}

	indices.push_back(2 * nrFragments + 1);
	indices.push_back(2 * nrFragments);
	indices.push_back(nrFragments + 1);

	//fete laterale

	for (int i = 1; i < nrFragments; i++) {
		indices.push_back(nrFragments + i);
		indices.push_back(i);
		indices.push_back(i + 1);

		indices.push_back(i+1);
		indices.push_back(nrFragments + i + 1);
		indices.push_back(nrFragments + i );
	}

	indices.push_back(2 * nrFragments);
	indices.push_back(nrFragments);
	indices.push_back(1);

	indices.push_back(1);
	indices.push_back(nrFragments + 1);
	indices.push_back(2 * nrFragments );

	Mesh * mesh = new Mesh(name);
	mesh->InitFromData(vertices, indices);
	return mesh;
}

//determinare unghi intre coltul din dreapta sus al marginii upperRightCorner, bila si tac, pentru care dreapta
//bila->tac e tangenta la margine in upperRightCorner
float Tema2Object3D::findTheta(Tac* tac, glm::vec3 upperRightCorner, Ball* ball) {
	float r1 = tac->radius1, r2 = tac->radius2, H = tac->height;
	float D = tac->distanceToTarget;
	float PO = glm::distance(upperRightCorner, ball->position);
	float a = 1 + glm::pow(H, 2) / glm::pow(r1 - r2, 2);
	float b = 2 * D * H / (r1 - r2) - 2 * glm::pow(H, 2) * r2 / glm::pow(r1 - r2, 2);
	float c = glm::pow(D, 2) - 2 * D * H * r2 / (r1 - r2) + glm::pow(H * r2 / (r1 - r2), 2) - glm::pow(PO, 2);

	float delta = glm::pow(b, 2) - 4 * a * c;
	if (delta < 0) {
		std::cout << "Nu intersecteaza!\n";
		return -1.0f;
	}

	float rdelta = glm::sqrt(delta);
	float h = (-b + rdelta) / (2 * a);

	float d = H * (h - r2) / (r1 - r2);
	float theta = asin(h / PO);
	return theta;
}

//creare dreptunghi, de-a lungul axelor: normal si tangent
Mesh* Tema2Object3D::CreateRect(std::string name, glm::vec3 leftBottomCorner, glm::vec3 normal, glm::vec3 tang, float lengthN, float lengthT, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + tang * lengthT, color),
		VertexFormat(corner + tang * lengthT + normal * lengthN, color),
		VertexFormat(corner - tang * lengthT + normal * lengthN, color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };


	// draw 2 triangles. Add the remaining 2 indices
	indices.push_back(0);
	indices.push_back(2);

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}