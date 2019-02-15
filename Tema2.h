#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include "LabCamera.h"

#include<queue>

class Ball;
class Player;

enum TypePocket {StSus, StCentru, StJos, DrSus, DrCentru, DrJos};

class Pocket
{
public:
	Pocket(TypePocket type, glm::vec3 position, float radius, Mesh* mesh);
	TypePocket type;
	glm::vec3 position;
	float radius;
	Mesh *mesh;
};

class Tac : public Tema2Camera::Camera
{
public:
	Tac(Mesh* mesh);
	Mesh* mesh;

	glm::vec3 oldPosition;
	glm::mat4 rotation = glm::mat4(1.0f);

	glm::mat4 oldRotation;

	glm::mat4 modelMatrixTac = glm::mat4(1);

	glm::vec3 lastValid;
	float lastTheta = 0;

	float radius1;
	float radius2;
	float height;

	float angleX;
	float angleY;
};

class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, bool axis);

	void CreateBalls();
protected:
	float tableSizeX = 8.0f;
	float tableSizeY = 0.5f;
	float tableSizeZ = 5.0f;
	glm::vec3 tableColor = glm::vec3(0, 0.6f, 0);

	float marginIncrease = 0.15f;
	float marginHeight = 0.9f;
	glm::vec3 marginColor = glm::vec3(0, 0.3f, 0);

	float interBall = 0.2f; //din diametrul bilei
	float ballX = 2.0f / 3; // cat ocupa bilele din ultima treime
	float radius;
	Ball* balls[16];

	Pocket* pockets[6];
	float radiusIncrease = 2.0f;
	float pocketRadius;

	//Ball* bila;

	float lowerTacRadius1 = 2.0f;
	float lowerTacRadius2 = 4.0f;
	float tacRadius1;
	float tacRadius2;
	float tacHeight = tableSizeX / 3;
	glm::vec3 tacColor = glm::vec3(0.823f, 0.411f, 0.117f);
	Tac* tac;

	bool viewMode = false;
	bool start = false;

	float maxDist = 1.0f;

	void FindPInt(Ball* ball, int *res, glm::vec3* pInt);

	void FindPExt(Tac* tac, Ball* ball, glm::vec3* cameraPos);

	glm::mat4 View;

	Tema2Camera::Camera* camera;
	float distCameraN = 2.0f;
	float distCamereP = 1.0f;
	float heightCamera = 8.0f;

	glm::mat4 projectionMatrix;
	float FOV = RADIANS(60);

	float minTheta;

	bool animation = false;
	bool lovitura = false;

	float time;
	float time0 = -1.0f;
	bool begin = false;

	bool puneAlba = true;

	bool normal = false;


	void TratareCiocniriBile();
	void TratareCiocniri();
	bool TratareIntrareBile(Ball *ball);

	float dif0;

	float coefFrecare = 0.005f;


	int tura = 0;

	bool primaBilaJoc = true;
	bool primaBilaLovitura = false;

	bool fault = false;

	Player* players[2];

	void AfiStatisticiJoc();

	bool EvitareColiziuniAlba();

	float raportOrtho = 0.20f;

	bool finish = true;
	void NewGame();

	void FindP(Tac* tac, Ball* ball, int* res, glm::vec3* pInt);
	Mesh* CreateAxis(glm::vec3 tac, glm::vec3 bila, glm::vec3 color, float gros);

	float grosime = 0.025f;
	float lungime = 2 * tableSizeX;

	float precision = 0.0001f;
	glm::vec3 colorAxis = glm::vec3(0, 0, 0);

	glm::vec3 colorAxisOptional = glm::vec3(0.501f, 0, 0.501f);

	float interAxis = 0.2f;

	void CreateOptionalAxis(Tac* tac, Ball* bila);

	glm::vec3 Inters2Circles(glm::vec3 Ball1, glm::vec3 Ball2, glm::vec3 direction);
};

class Ball
{
public:
	Ball(int init_id, glm::vec3 init_color, glm::vec3 init_position, Mesh* mesh);
	int id;
	glm::vec3 color;
	glm::vec3 position;
	glm::vec3 positionInit;
	bool out = false;
	Mesh* mesh;

	float intensitate = 0;
	glm::vec3 directie = glm::vec3(0, 0,0);

	float coefVelocity = 0.05f;
	void reset() {
		intensitate = 0;
		directie = glm::vec3(0, 0, 0);
		position = positionInit;
		out = false;
	};

	glm::vec3 Ball::getVelocity() {
		return directie * intensitate * coefVelocity;

	}
	void Ball::setVelocity(glm::vec3 velocity) {
		intensitate = length(velocity) / coefVelocity;
		if (intensitate != 0) {
			directie = normalize(velocity);
		}
	}

	bool ciocnitDeMargine = false;

};

enum TipPlayer {Rosu, Galben};
class Player {
public:
	Player(int paritateTura) {
		this->paritateTura = paritateTura;
	}
	int bileBagate = 0;
	int nrFaulturi = 0;
	TipPlayer tip;
	int paritateTura;

	void reset() {
		bileBagate = 0;
		nrFaulturi = 0;
	};
};
