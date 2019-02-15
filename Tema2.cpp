#include "Tema2.h"
#include "Tema2Object3D.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	camera =  new Tema2Camera::Camera();
	camera->Set(glm::vec3(0, heightCamera, 0), glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));

	projectionMatrix = glm::perspective(FOV, window->props.aspectRatio, 0.01f, 200.0f);

	players[0] = new Player(0);
	players[1] = new Player(1);

	{
		glm::vec3 color[6] = { tableColor, tableColor, tableColor, tableColor, tableColor, tableColor };
		Mesh* table = Tema2Object3D::CreateParaleliped("table", glm::vec3(-tableSizeX / 2, 0, -tableSizeZ / 2),
		tableSizeX, tableSizeY, tableSizeZ, color);
		meshes[table->GetMeshID()] = table;
	}
	//creare margini
	{
		glm::vec3 corner = { -tableSizeX / 2 * (1 + marginIncrease), 0, -tableSizeZ * (1 + marginIncrease) / 2 };
		float dx = tableSizeX * (1 + marginIncrease);
		float dy = tableSizeY * (1 + marginHeight);
		float dz = tableSizeZ * marginIncrease / 2;
		glm::vec3 color[6] = { marginColor, marginColor, tableColor, tableColor, tableColor, tableColor };
		Mesh* margSt = Tema2Object3D::CreateParaleliped("margSt", corner, dx, dy, dz, color);
		meshes[margSt->GetMeshID()] = margSt;
	}

	{
		glm::vec3 corner = { -tableSizeX / 2 * (1 + marginIncrease), 0, tableSizeZ / 2};
		float dx = tableSizeX * (1 + marginIncrease);
		float dy = tableSizeY * (1 + marginHeight);
		float dz = tableSizeZ * marginIncrease / 2;
		glm::vec3 color[6] = { tableColor, marginColor, tableColor, marginColor, tableColor, tableColor };
		Mesh* margDr = Tema2Object3D::CreateParaleliped("margDr", corner, dx, dy, dz, color);
		meshes[margDr->GetMeshID()] = margDr;
	}

	{
		glm::vec3 corner = { -tableSizeX / 2 * (1 + marginIncrease), 0, -tableSizeZ * (1 + marginIncrease) / 2 };
		float dx = tableSizeX * marginIncrease / 2;
		float dy = tableSizeY * (1 + marginHeight);
		float dz = tableSizeZ * (1 + marginIncrease) ;
		glm::vec3 color[6] = { tableColor, marginColor, marginColor, tableColor, tableColor, tableColor };
		Mesh* margJos = Tema2Object3D::CreateParaleliped("margJos", corner, dx, dy, dz, color);
		meshes[margJos->GetMeshID()] = margJos;
	}

	{
		glm::vec3 corner = { tableSizeX / 2, 0, -tableSizeZ * (1 + marginIncrease) / 2 };
		float dx = tableSizeX * marginIncrease / 2;
		float dy = tableSizeY * (1 + marginHeight);
		float dz = tableSizeZ * (1 + marginIncrease);
		glm::vec3 color[6] = { tableColor, marginColor, tableColor, tableColor, tableColor, marginColor };
		Mesh* margSus = Tema2Object3D::CreateParaleliped("margSus", corner, dx, dy, dz, color);
		meshes[margSus->GetMeshID()] = margSus;
	}

	radius = tableSizeX * ballX / (5 + 4 * interBall) / 6;
	pocketRadius = radius * radiusIncrease;
	CreateBalls();

	//creare buzunare
	{
		glm::vec3 position;
		glm::vec3 negru = glm::vec3(0, 0, 0);

		position = glm::vec3(tableSizeX / 2, tableSizeY, -tableSizeZ / 2);
		Mesh* mesh = Tema2Object3D::CreateSphere("PocketStSus", position, pocketRadius, negru);
		meshes[mesh->GetMeshID()] = mesh;
		pockets[0] = new Pocket(TypePocket::StSus, position, pocketRadius,mesh);

		position = glm::vec3(tableSizeX / 2, tableSizeY, tableSizeZ / 2);
		mesh = Tema2Object3D::CreateSphere("PocketDrSus", position, pocketRadius, negru);
		meshes[mesh->GetMeshID()] = mesh;
		pockets[1] = new Pocket(TypePocket::DrSus, position, pocketRadius,mesh);

		position = glm::vec3(0, tableSizeY, -tableSizeZ / 2);
		mesh = Tema2Object3D::CreateSphere("PocketStC", position, pocketRadius, negru);
		meshes[mesh->GetMeshID()] = mesh;
		pockets[2] = new Pocket(TypePocket::StCentru, position, pocketRadius, mesh);

		position = glm::vec3(0, tableSizeY, tableSizeZ / 2);
		mesh = Tema2Object3D::CreateSphere("PocketDrC", position, pocketRadius, negru);
		meshes[mesh->GetMeshID()] = mesh;
		pockets[3] = new Pocket(TypePocket::DrCentru, position, pocketRadius,mesh);

		position = glm::vec3(-tableSizeX / 2, tableSizeY, -tableSizeZ / 2);
		mesh = Tema2Object3D::CreateSphere("PocketStJos", position, pocketRadius, negru);
		meshes[mesh->GetMeshID()] = mesh;
		pockets[4] = new Pocket(TypePocket::StJos, position, pocketRadius,mesh);

		position = glm::vec3(-tableSizeX / 2, tableSizeY, tableSizeZ / 2);
		mesh = Tema2Object3D::CreateSphere("PocketDrJos", position, pocketRadius, negru);
		meshes[mesh->GetMeshID()] = mesh;
		pockets[5] = new Pocket(TypePocket::DrJos, position, pocketRadius,mesh);

	}
	//creare bila alba
	glm::vec3 positionInit = glm::vec3(-tableSizeX / 3, tableSizeY + radius, 0);
	Mesh* mesh = Tema2Object3D::CreateSphere("bila", positionInit, radius, glm::vec3(1, 1, 1));
	meshes[mesh->GetMeshID()] = mesh;
	balls[15] = new Ball(15, glm::vec3(1, 1, 1), positionInit, mesh);

	//creare tac
	tacRadius1 = radius / lowerTacRadius1;
	tacRadius2 = radius / lowerTacRadius2;
	
	mesh = Tema2Object3D::CreateCylinder("tac", glm::vec3(0,0,0), tacRadius1, tacRadius2, tacHeight, tacColor);
	meshes[mesh->GetMeshID()] = mesh;

	tac = new Tac(mesh);
	tac->distanceToTarget = radius + maxDist;
	tac->radius1 = tacRadius1;
	tac->radius2 = tacRadius2;
	tac->height = tacHeight;
	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderTema2");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{
	
	glm::mat4 modelMatrix = glm::mat4(1);
	//afisare masa
	RenderSimpleMesh(meshes["table"], shaders["ShaderTema2"], modelMatrix, false);
	//afisare margini
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		RenderSimpleMesh(meshes["margSt"], shaders["ShaderTema2"], modelMatrix, false);

		RenderSimpleMesh(meshes["margDr"], shaders["ShaderTema2"], modelMatrix, false);

		RenderSimpleMesh(meshes["margSus"], shaders["ShaderTema2"], modelMatrix, false);

		RenderSimpleMesh(meshes["margJos"], shaders["ShaderTema2"], modelMatrix, false);
	}
	//afisare bile
	for (int nr = 0; nr <= 15; nr++) {
		Ball* bila = balls[nr];
		glm::mat4 modelMatrixBila = glm::mat4(1);
		modelMatrixBila *= Transform3D::Translate(bila->position.x - bila->positionInit.x, bila->position.y -
			bila->positionInit.y, bila->position.z - bila->positionInit.z);
		if (!bila->out ) {
			RenderSimpleMesh(bila->mesh, shaders["ShaderTema2"], modelMatrixBila, false);
		}	
	}
	//afisare buzunare
	for (int nr = 0; nr <= 5; nr++) {
		RenderSimpleMesh(pockets[nr]->mesh, shaders["ShaderTema2"], modelMatrix, false);
	}
	
	//cazul vederii perspective
	if (viewMode ) {
		Ball* bila = balls[15];
		//initializarea vederii perespective
		if (start) {
			if (!primaBilaJoc) primaBilaLovitura = true;
			cout << "Incepe tura " << tura << "\n";

			float theta;
			glm::vec3 posCamera;

			//pozitionare tac langa cea mai apropiata margine
			int res;
			glm::vec3 pInt;
			FindPInt(bila, &res, &pInt);
			int rotateAxis = 0;
			
			if (res == 1) {
				glm::vec3 position = { bila->position.x - tac->distanceToTarget, bila->position.y, bila->position.z };
				tac->Set(position, bila->position, glm::vec3(0, 1, 0));
				theta = atan((tableSizeY * (1 + marginHeight) - bila->position.y) / abs(abs(bila->position.x) - tableSizeX / 2));
				rotateAxis = 3;
			}else if (res == 2) {
				glm::vec3 position = { bila->position.x + tac->distanceToTarget, bila->position.y, bila->position.z };
				tac->Set(position, bila->position, glm::vec3(0, 1, 0));
				theta = atan((tableSizeY * (1 + marginHeight) - bila->position.y) / abs(abs(bila->position.x) - tableSizeX / 2));
				rotateAxis = 3;
			}
			else if (res == 3) {
				glm::vec3 position = { bila->position.x, bila->position.y, bila->position.z + tac->distanceToTarget };
				tac->Set(position, bila->position, glm::vec3(0, 1, 0));
				theta = atan((tableSizeY * (1 + marginHeight) - bila->position.y) / abs(abs(bila->position.z) - tableSizeZ / 2));
				rotateAxis = 1;
			}
			else if (res == 4) {
				glm::vec3 position = { bila->position.x, bila->position.y, bila->position.z - tac->distanceToTarget };
				tac->Set(position, bila->position, glm::vec3(0, 1, 0));
				theta = atan((tableSizeY * (1 + marginHeight) - bila->position.y) / abs(abs(bila->position.z) - tableSizeZ / 2));
				rotateAxis = 1;
			}

			float dtheta = Tema2Object3D::findTheta(tac, glm::vec3(pInt.x, tableSizeY * (1 + marginHeight), pInt.z), bila);
			theta += dtheta;
			if (res == 1) {
				theta = -theta;
			}
			else if (res == 2) {

			}
			else if (res == 3) {
				theta = -theta;
			}
			else if (res == 4) {

			}

			tac->modelMatrixTac = Transform3D::Translate(bila->position.x, bila->position.y, bila->position.z);
			if (rotateAxis == 1) {
				tac->modelMatrixTac *= Transform3D::RotateOX(theta);
			}
			else if (rotateAxis == 3) {
				tac->modelMatrixTac *= Transform3D::RotateOZ(theta);
			}
			tac->modelMatrixTac *= Transform3D::Translate(-bila->position.x, -bila->position.y, -bila->position.z);
			tac->modelMatrixTac *= Transform3D::Translate(tac->position.x, tac->position.y, tac->position.z);

			if (res == 1) {
			}
			else if(res == 2){
				tac->modelMatrixTac *= Transform3D::RotateOY(RADIANS(180));
			}
			else if (res == 3) {
				tac->modelMatrixTac *= Transform3D::RotateOY(RADIANS(90));
			}
			else if (res == 4) {
				tac->modelMatrixTac *= Transform3D::RotateOY(RADIANS(-90));
			}
			
			tac->Set(glm::vec3(tac->modelMatrixTac * glm::vec4(0, 0, 0, 1)), bila->position, glm::vec3(0, 1, 0));
			tac->oldRotation = tac->modelMatrixTac;

			//pozitionare camera
			FindPExt(tac, bila, &posCamera);

			camera->Set(posCamera, bila->position, glm::vec3(0, 1, 0));
			camera->distanceToTarget = distance(posCamera, bila->position);
			start = false;

			tac->angleX = 0;
			tac->angleY = 0;
		}
		//cazul vederii perspectiva
		else 
		{
			//aflare punct P de colt dreapta sus a marginii, in care dreapta bila->tac e tangenta
			glm::vec3 P;
			int res;
			FindP(tac, bila, &res, &P);
			/*
			aflare unghi minim minTheta pentru actuala pozitie a tacului (obtinuta prin miscare mouse ului fata de pozitia initiala), 
			pentru care tacul nu taie marginea
			*/
			minTheta = 0;
			minTheta = atan((tableSizeY * (1 + marginHeight) - bila->position.y) / distance(glm::vec3(P.x,bila->position.y,P.z),bila->position));
			float dtheta = Tema2Object3D::findTheta(tac, P, bila);
			minTheta += dtheta;

			glm::vec3 bila_tac = glm::normalize(tac->position - bila->position);
			float compl = acos(glm::dot(bila_tac, glm::vec3(0, 1, 0)));
			float theta = RADIANS(90) - compl;

			glm::vec3 bottomCenter = glm::vec3(tac->modelMatrixTac * glm::vec4(-tac->height, 0, 0, 1));
			//cazul in care unghiul e mai mic decat minTheta, deci tacul taie marginea sau cazu in care baza tacului intra pe masa
			if (theta < minTheta || ((bottomCenter.x >= - tableSizeX / 2 && bottomCenter.x <= tableSizeX / 2)
				&& (bottomCenter.z >= -tableSizeZ / 2 && bottomCenter.z <= tableSizeZ / 2))) {
				tac->modelMatrixTac = tac->oldRotation;
				//refac pozitia tac ului (in caz ca rotatie nu e valida)
				tac->RotateThirdPerson_OX(-tac->angleY);
				tac->RotateThirdPerson_OY(-tac->angleX);

				tac->angleX = 0;
				tac->angleY = 0;

				tac->position = glm::vec3(tac->modelMatrixTac * glm::vec4(0, 0, 0, 1));
			}
			//cazul in care nu s-au intalnit problemele anterioare iar tacul si camera pot fi rotite
			else {
				tac->oldRotation = tac->modelMatrixTac;

				camera->RotateThirdPerson_OY(tac->angleX);
				camera->RotateThirdPerson_OX(tac->angleY);


				tac->angleX = 0;
				tac->angleY = 0;

				tac->position = glm::vec3(tac->modelMatrixTac * glm::vec4(0, 0, 0, 1));
			}
		}
		RenderSimpleMesh(tac->mesh, shaders["ShaderTema2"], tac->modelMatrixTac, false);

		//afisare axe ajutatoare

		//axa tacului
		Mesh* mesh = CreateAxis(tac->position, bila->position, colorAxis, grosime);
		RenderSimpleMesh(mesh, shaders["ShaderTema2"], glm::mat4(1), true);
		//axele bilelor lovite de tac
		CreateOptionalAxis(tac, bila);
	}
	//cazul in care se executa lovitura, dupa iesirea din modul animatie
	else if (lovitura){
		Ball* bila = balls[15];
		glm::vec3 difPoz = bila->position - tac->position;

		glm::vec3 depl = glm::normalize(difPoz) * (length(difPoz) - radius);
		time = (float)Engine::GetElapsedTime() - time0;
		glm::vec3 dif = -sin(2 * time) * depl;

		//initializare parametri necesari acestei etape: timpul la care s-a ajuns in animatie, directie si intensitate tac
		if (begin) {
			bila->intensitate = length(tac->position + dif - (bila->position));
			bila->directie = normalize(glm::vec3(difPoz.x, 0, difPoz.z));
			begin = false;
			dif0 = time;
		}
		else {
			dif = -sin(2 * (dif0 + (time - dif0)* bila->intensitate)) * depl;
		}

		glm::mat4 modelMatrixTac_reserved = Transform3D::Translate(dif.x, dif.y, dif.z) * tac->modelMatrixTac;

		RenderSimpleMesh(tac->mesh, shaders["ShaderTema2"], modelMatrixTac_reserved, false);
		
		glm::vec3 newTacPos = glm::vec3(Transform3D::Translate(dif.x, dif.y, dif.z) * glm::vec4(tac->position, 1.0));

		//cazul in care se termina aceasta faza: tacul atinge bila
		if (distance(newTacPos, bila->position) <= radius + deltaTimeSeconds) {
			lovitura = false;
			tac->position = newTacPos;
			tac->modelMatrixTac = modelMatrixTac_reserved;
			normal = true;
			time0 = -1.0f;
		}
		
	}
	//faza in care se urmareste de sus miscare bilelor
	else if(normal){
		//tratare cicniri cu margini si intrare in buzunar
		TratareCiocniri();
		//tratare ciocniri intre bile
		TratareCiocniriBile();
		
		//daca a intrat bila neagra
		if (finish) {
			finish = false;
			return;
		}

		bool ok = true;
		for (int i = 0; i <= 15; i++) {
			Ball* bila = balls[i];
			if (!bila->out) {
				//calcul pozitie bile
				glm::vec3 v = bila->getVelocity();
				bila->position.x += v.x;
				bila->position.y += v.y;
				bila->position.z += v.z;
				bila->intensitate -= coefFrecare;
				if (bila->intensitate <= 0) {
					bila->intensitate = 0;
				}
				else {
					ok = false;
				}
			}
		}

		//daca bilele s-au terminat de miscat 
		if (ok) {
			//daca s-a constatat un fault, se pune bila alba
			if (fault) {
				puneAlba = true;
				balls[15]->reset();
				fault = false;
			}
			//daca totul e in regula, se revine in ViewMode
			else {
				viewMode = true;
				start = true;
			}
			normal = false;
			tura++;
		}
	}

	/*in totate fazele mai putin viewMode se se foloseste o proiectie ortografica iar camera
	se afla deasupra */
	if (!viewMode) {
		float ratio = tableSizeX / tableSizeZ;

		float top = tableSizeX / 2 * (1 + raportOrtho);
		float bottom = -top;
		float right = top * window->props.aspectRatio;
		float left = -right;
		projectionMatrix = glm::ortho(left, right, bottom, top , 0.01f, 200.0f);
		camera->Set(glm::vec3(0,heightCamera,0), glm::vec3(0,0,0), glm::vec3(1, 0, 0));
	}
	/*in viewMode se foloseste o proiectie perspectiva iar camera este cea pozitionata in Update, in cazul ViewMode*/
	else {
		projectionMatrix = glm::perspective(FOV, window->props.aspectRatio, 0.01f, 200.0f);

	}
	
	
}

void Tema2::FrameEnd()
{
	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	//faza in care se pune bila alba in prima treime a scenei
	if (puneAlba) {
		Ball* bila = balls[15];
		glm::vec3 oldPos = bila->position;
		if (window->KeyHold(GLFW_KEY_W)) {
			bila->position += glm::vec3(deltaTime, 0, 0);
			if (bila->position.x > -tableSizeX / 6 - radius) bila->position.x = -tableSizeX / 6 - radius;
		}
		else if (window->KeyHold(GLFW_KEY_S)) {
			bila->position += glm::vec3(-deltaTime, 0, 0);
			if (bila->position.x < -tableSizeX/2 + radius) bila->position.x = -tableSizeX / 2 + radius;
		}
		else if (window->KeyHold(GLFW_KEY_A)) {
			bila->position += glm::vec3(0, 0, -deltaTime);
			if (bila->position.z < -tableSizeZ / 2 + radius) bila->position.z = -tableSizeZ / 2 + radius;
		}
		else if (window->KeyHold(GLFW_KEY_D)) {
			bila->position += glm::vec3(0, 0, deltaTime);
			if (bila->position.z > tableSizeZ / 2 - radius) bila->position.z = tableSizeZ / 2 - radius;
		}

		//daca exista coliziuni cu alte bile, se revine la vechea pozitie
		if (!EvitareColiziuniAlba()) {
			bila->position = oldPos;
		}
	}
	
}

void Tema2::OnKeyPress(int key, int mods)
{
	//la apasarea tastei space, se trece in faza ViewMode
	if (key == GLFW_KEY_SPACE && puneAlba) {
		viewMode = true;
		start = true;
		puneAlba = false;
		//View = 
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	//la mutarea mouse-ului cu click dreapta apasat, se roteste atat camera cat si tacul
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) && viewMode )
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		float angleX = deltaX * sensivityOX;
		float angleY = deltaY * sensivityOY;
			
		tac->angleX = angleX;
		tac->angleY = angleY;

		glm::vec3 dif = -balls[15]->position ;
		//rotatie pe OY cu unghiul angleX
		glm::mat4 rot1 = glm::rotate(glm::mat4(1.0f), angleX, glm::vec3(0, 1, 0));
		tac->RotateThirdPerson_OY(angleX);
		//rotatie pe OX cu unghiul angleY
		glm::mat4 rot2 = glm::rotate(glm::mat4(1.0f), angleY, tac->right);
		tac->RotateThirdPerson_OX(angleY);

		tac->rotation = Transform3D::Translate(-dif.x, -dif.y, -dif.z);
		tac->rotation *= rot2;
			
		tac->rotation *= rot1;
		tac->rotation *= Transform3D::Translate(dif.x, dif.y, dif.z);

		tac->modelMatrixTac = tac->rotation * tac->modelMatrixTac;
			
	}
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event

	//la apasarea click stanga, se activeaza modul animation, unde tacul oscileaza in vertexShader
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && viewMode) {
		animation = true;
	}

}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event

	//la eliberarea click stanga, se trece in faza lovitura, unde se vizioneaza lovitura top-down
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && animation) {
		lovitura = true;
		animation = false;
		begin = true;
		viewMode = false;
	}
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}

void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, bool axis)
{

	if (!mesh || !shader || !shader->GetProgramID()) {
		if (!mesh) std::cout << "no mesh \n";
		if (!shader) std::cout << "no shader \n";
		return;
	}


	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// TODO : get shader location for uniform mat4 "Model"
	int location = glGetUniformLocation(shader->GetProgramID(), "Model");


	// TODO : set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// TODO : get shader location for uniform mat4 "View"
	location = glGetUniformLocation(shader->GetProgramID(), "View");

	// TODO : set shader uniform "View" to viewMatrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// TODO : get shader location for uniform mat4 "Projection"
	location = glGetUniformLocation(shader->GetProgramID(), "Projection");

	// TODO : set shader uniform "Projection" to projectionMatrix
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	location = glGetUniformLocation(shader->GetProgramID(), "radius");
	glUniform1f(location, radius);

	int object = -1;
	if (mesh == tac->mesh) object = 0;
	else if (axis) object = 1;
	location = glGetUniformLocation(shader->GetProgramID(), "object");
	glUniform1i(location, object);

	location = glGetUniformLocation(shader->GetProgramID(), "animation");
	int anim = (int)animation;
	glUniform1i(location, anim);

	glm::vec3 difPoz = balls[15]->position - tac->position;
	location = glGetUniformLocation(shader->GetProgramID(), "difPoz");
	glUniform3fv(location, 1, glm::value_ptr(difPoz));

	location = glGetUniformLocation(shader->GetProgramID(), "time");
	if (animation) {
		if (time0 == -1.0f) {
			time0 = (float)Engine::GetElapsedTime();
			time = 0;
		}
		else {
			time = (float)Engine::GetElapsedTime() - time0;
		}
		glUniform1f(location, time);
	}

	location = glGetUniformLocation(shader->GetProgramID(), "tableSizeX");
	glUniform1f(location, tableSizeX);

	location = glGetUniformLocation(shader->GetProgramID(), "tableSizeZ");
	glUniform1f(location, tableSizeZ);

	location = glGetUniformLocation(shader->GetProgramID(), "ball");
	glUniform3fv(location, 1, glm::value_ptr(balls[15]->position));

	location = glGetUniformLocation(shader->GetProgramID(), "interAxis");
	glUniform1f(location, interAxis);
	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

Ball::Ball(int init_id, glm::vec3 init_color, glm::vec3 init_position, Mesh* mesh)
{
	out = false;
	id = init_id;
	color = init_color;
	position = init_position;
	positionInit = init_position;
	this->mesh = mesh;
}

Pocket::Pocket(TypePocket type, glm::vec3 position, float radius, Mesh* mesh) {
	this->type = type;
	this->position = position;
	this->radius = radius;
	this->mesh = mesh;
}

Tac::Tac(Mesh* mesh) {
	this->mesh = mesh;
}

//creaza obiecte de tipul Ball avand setata o mesha si pozitia, pentru fiecare bila
void Tema2::CreateBalls()
{

	float iter = (interBall + 1) * 2 * radius;

	float x0 = tableSizeX / 6.0f + (tableSizeX / 3.0f - ballX * tableSizeX / 3.0f) * 1.0f / 2 ;
	float y0 = tableSizeY + radius;
	float z0 = 0;
	glm::vec3 first = glm::vec3(x0, y0, z0);

	glm::vec3 rosu = glm::vec3(1, 0, 0);
	glm::vec3 galben = glm::vec3(1, 1, 0);
	glm::vec3 negru = glm::vec3(0, 0, 0);

	glm::vec3 position;
	glm::vec3 color;

	
	for (int nr = 0;nr <= 14; nr++) {
		if (nr == 0) {
			color = galben;
			position = first;
		}
		else if (nr >= 1 && nr <= 2) {
			if (nr == 1) color = rosu;
			else color = galben;

			position = first + glm::vec3(iter, 0, 0) +
				(1.5f - (float)nr) * glm::vec3(0, 0, iter);
		}
		else if (nr >= 3 && nr <= 5) {
			if (nr == 3) color = galben;
			else if (nr == 4) color = negru;
			else if (nr == 5) color = rosu;

			position = first + 2.0f * glm::vec3(iter, 0, 0) +
				(4.0f - nr) * glm::vec3(0, 0, iter);
		}
		else if (nr >= 6 && nr <= 9) {
			if (nr == 6 || nr == 9) color = galben;
			else color = rosu;

			position = first + 3.0f * glm::vec3(iter, 0, 0) +
				(7.5f - (float)nr) * glm::vec3(0, 0, iter);
		}
		else if (nr >= 10 && nr <= 14) {
			if (nr == 10 || nr == 11 || nr == 14) color = rosu;
			else color = galben;

			position = first + 4.0f * glm::vec3(iter, 0, 0) +
				(12.0f - nr) * glm::vec3(0, 0, iter);
		}

		char name[20];
		sprintf(name, "b%d", nr);
		Mesh* mesh = Tema2Object3D::CreateSphere(name, position, radius, color);
		meshes[mesh->GetMeshID()] = mesh;
		balls[nr] = new Ball(nr, color, position, mesh);
	}
}

//determinare colt din dreapta sus a marginii in care dreapta tac->bila este tangenta la margine
void Tema2::FindP(Tac* tac, Ball* ball,	int* res, glm::vec3* pInt) {
	float tX = tac->position.x, tZ = tac->position.z;
	float bX = ball->position.x, bZ = ball->position.z;

	float y = tableSizeY * (1 + marginHeight);

	*res = 0;

	float x = -tableSizeX / 2;
	float z = tZ + (x - tX) * (bZ - tZ) / (bX - tX);
	//cout << "1: " << "(" << x << "," << z << ")\n";
	if (z >= -tableSizeZ / 2 && z <= tableSizeZ / 2 ) {
		glm::vec3 bt = glm::normalize(glm::vec3(tac->position.x, ball->position.y, tac->position.z) - ball->position);
		glm::vec3 bp = glm::normalize(glm::vec3(x, ball->position.y, z) - ball->position);
		if (abs(bt.x - bp.x) < precision && abs(bt.z - bp.z) < precision) {
			*pInt = glm::vec3(x, y, z);
			*res = 1;
		}
	}
	z = -tableSizeZ / 2;
	x = tX + (z - tZ) * (bX - tX) / (bZ - tZ);
	if (x >= -tableSizeX / 2 && x <= tableSizeX / 2 ) {
		glm::vec3 bt = glm::normalize(glm::vec3(tac->position.x, ball->position.y, tac->position.z) - ball->position);
		glm::vec3 bp = glm::normalize(glm::vec3(x, ball->position.y, z) - ball->position);
		if (abs(bt.x - bp.x) < precision && abs(bt.z - bp.z) < precision) {
			*pInt = glm::vec3(x, y, z);
			*res = 4;
		}
	}

	x = tableSizeX / 2;
	z = tZ + (x - tX) * (bZ - tZ) / (bX - tX);
	if (z >= -tableSizeZ / 2 && z <= tableSizeZ / 2 ) {
		glm::vec3 bt = glm::normalize(glm::vec3(tac->position.x, ball->position.y, tac->position.z) - ball->position);
		glm::vec3 bp = glm::normalize(glm::vec3(x, ball->position.y, z) - ball->position);
		if (abs(bt.x - bp.x) < precision && abs(bt.z - bp.z) < precision) {
			*pInt = glm::vec3(x, y, z);
			*res = 2;
		}
	}

	z = tableSizeZ / 2;
	x = tX + (z - tZ) * (bX - tX) / (bZ - tZ);
	if (x >= -tableSizeX / 2 && x <= tableSizeX / 2 ) {
		glm::vec3 bt = glm::normalize(glm::vec3(tac->position.x, ball->position.y, tac->position.z) - ball->position);
		glm::vec3 bp = glm::normalize(glm::vec3(x, ball->position.y, z) - ball->position);
		if (abs(bt.x - bp.x) < precision && abs(bt.z - bp.z) < precision) {
			*pInt = glm::vec3(x, y, z);
			*res = 3;
		}
	}

}

//tratare ciocnire intre bile 2 cate 2: se transfera componenta normala de la una la alta
void Tema2::TratareCiocniriBile() {

	for (int i = 0; i <= 15; i++) {
		if (!balls[i]->out) {
			for (int j = 0; j <= 15; j++) {
				if (i != j && balls[i]->intensitate > 0 && !balls[j]->out) {
					Ball* bila1 = balls[i];
					Ball* bila2 = balls[j];
					if (distance(bila1->position, bila2->position) <= 2 * radius) {
						glm::vec3 N = normalize(bila2->position - bila1->position);
						float spre2 = glm::dot(bila1->getVelocity(), N);
						if (spre2 > 0) {
							glm::vec3 dif2 = N * spre2;
							bila2->setVelocity(bila2->getVelocity() + dif2);
							bila1->setVelocity(bila1->getVelocity() - dif2);
						}
					}
				}

			}
		}
	}
}

//tratare ciocniri bile cu margini si intrare bile in buzunare
void Tema2::TratareCiocniri() {

	for (int i = 0; i <= 15; i++) {
		Ball* bila = balls[i];
		if (!bila->out) {
			if (TratareIntrareBile(bila)) continue;

			glm::vec3 newBilaPos = bila->position + bila->getVelocity();

			//tratare ciocniri cu margini: in cazul unei ciocniri cu o margine,
			//componenta normala se inverseaza iar cea tangenta ramane constanta 
			bool b1 = newBilaPos.x <= -tableSizeX / 2.0f + radius;
			bool b2 = newBilaPos.x >= tableSizeX / 2.0f - radius;
			bool b3 = newBilaPos.z <= -tableSizeZ / 2.0f + radius;
			bool b4 = newBilaPos.z >= tableSizeZ / 2.0f - radius;
			
			if (b1 || b2) {
				glm::vec3 v = bila->getVelocity();
				bila->setVelocity(glm::vec3(-v.x, v.y, v.z));
			}
			else if (b3 || b4) {
				glm::vec3 v = bila->getVelocity();
				bila->setVelocity(glm::vec3(v.x, v.y, -v.z));
			}
		}
	}
}

//tratare intrare bila in buzunar
bool Tema2::TratareIntrareBile(Ball *ball) {
	for (int i = 0; i <= 5; i++) {
		if (!ball->out && distance(ball->position, pockets[i]->position) <= pocketRadius + radius) {
			ball->out = true;
			Player* player = players[tura % 2];

		 if (ball->color == glm::vec3(0, 0, 0)) {
			if (player->bileBagate == 7) {
				cout << "Jucatorul " << tura % 2 << " a castigat!\n -- A bagat bila neagra!\n";
			}
			else if (player->bileBagate < 7) {
				cout << "Jucatorul " << tura % 2 << " a pierdut!\n  -- A bagat bila neagra!\n";
			}
			NewGame();
			finish = true;
			return true;
		}
		else if (ball->color == glm::vec3(1, 1, 1)) {
			cout << "Jucatorul " << tura % 2 << " a bagat bila alba!\n";
			player->nrFaulturi++;
			fault = true;
		}
		else if (primaBilaJoc) {
			if (ball->color == glm::vec3(1, 0, 0)) {
				cout << "Jucatorul " << tura%2 << " si-a ales bilele rosii!\n";
				player->tip = TipPlayer::Rosu;
				players[(tura + 1) % 2]->tip = TipPlayer::Galben;
			}
			else if (ball->color == glm::vec3(1, 1, 0)) {
				cout << "Jucatorul " << tura % 2 << " si-a ales bilele galbene!\n";
				player->tip = TipPlayer::Galben;
				players[(tura + 1) % 2]->tip = TipPlayer::Rosu;
			}
			player->bileBagate++;
			primaBilaJoc = false;
		}
		else if (primaBilaLovitura) {
			if ((ball->color == glm::vec3(1, 0, 0) && player->tip == TipPlayer::Rosu) ||
				(ball->color == glm::vec3(1, 1, 0) && player->tip == TipPlayer::Galben)) {
				cout << "Jucatorul " << tura % 2 << " si-a bagat o bila!\n";
				player->bileBagate++;
			}
			else if ((ball->color == glm::vec3(1, 0, 0) && player->tip == TipPlayer::Galben) ||
				(ball->color == glm::vec3(1, 1, 0) && player->tip == TipPlayer::Rosu)) {
				cout << "Jucatorul " << tura % 2 << " a bagat o bila adversarului!\n";
				players[(tura + 1) % 2]->bileBagate++;
				player->nrFaulturi++;
				fault = true;
			}

			primaBilaLovitura = false;
		}
		else if ((ball->color == glm::vec3(1, 0, 0) && player->tip == TipPlayer::Rosu) ||
			 (ball->color == glm::vec3(1, 1, 0) && player->tip == TipPlayer::Galben)) {
			 cout << "Jucatorul " << tura % 2 << " si-a bagat o bila!\n";
			 player->bileBagate++;
		 }
		else if ((ball->color == glm::vec3(1, 0, 0) && player->tip == TipPlayer::Galben) ||
			 (ball->color == glm::vec3(1, 1, 0) && player->tip == TipPlayer::Rosu)) {
			 cout << "Jucatorul " << tura % 2 << " a bagat o bila adversarului!\n";
			 players[(tura + 1) % 2]->bileBagate++;
			 player->nrFaulturi++;
		 }
			
		AfiStatisticiJoc();
		return true;
		}
	}
	return false;
}

//aflare colt dreapta sus a marginii cele mai apropiate de bila (inceputul fazei ViewMode)
//pe directia careia se va pozitiona tacul si camera
void Tema2::FindPInt(Ball* ball, int* res, glm::vec3* pInt) {
	float x = ball->position.x;
	float y = ball->position.y;
	float z = ball->position.z;
	float mst = -tableSizeX / 2;
	float mdr = tableSizeX / 2;
	float msus = tableSizeZ / 2;
	float mjos = -tableSizeZ / 2;

	float dif = 1000;
	*res = 0;

	float difSt = x - mst;
	if (difSt < dif) {
		*pInt = glm::vec3(mst, y, z);
		dif = difSt;
		*res = 1;
	}
	float difDr = mdr - x;
	if (difDr < dif) {
		*pInt = glm::vec3(mdr, y, z);
		dif = difDr;
		*res = 2;
	}
	float difSus = msus - z;
	if (difSus < dif) {
		*pInt = glm::vec3(x, y, msus);
		dif = difSus;
		*res = 3;
	}
	float difJos = z - mjos;
	if (difJos < dif) {
		*pInt = glm::vec3(x, y, mjos);
		dif = difJos;
		*res = 4;
	}
}

//determinare punct in care va pozitiona camera, pe directia bila->tac
void Tema2::FindPExt(Tac* tac, Ball* ball, glm::vec3* cameraPos) {
	glm::vec3 dir = glm::normalize(tac->position - ball->position);
	float len = glm::length(tac->position - ball->position);

	glm::vec3 oy = glm::vec3(0, 1, 0);
	glm::vec3 tang = glm::normalize(oy - dir * glm::dot(oy, dir));

	*cameraPos = tac->position + dir * distCameraN + tang * distCamereP;
}

//afisare statistici despre fiecare jucator
void Tema2::AfiStatisticiJoc() {
	cout << "\nJucatorul:\tNr bile bagate:\tNr faulturi comise:\n";
	cout << "\t"<<0<<"\t\t"<<players[0]->bileBagate<<"\t\t"<<players[0]->nrFaulturi <<"\n";
	cout << "\t" << 1 << "\t\t" << players[1]->bileBagate << "\t\t" << players[1]->nrFaulturi << "\n";
}

//pentru faza in care se pune bila alba, evitare coliziuni cu alte bile
bool Tema2::EvitareColiziuniAlba() {
	Ball* alba = balls[15];
	for (int i = 0; i <= 14; i++) {
		if (distance(alba->position, balls[i]->position) <= 2 * radius) {
			return false;
		}
	}
	return true;
}

//setare parametri pentru inceputul unui nou joc
void Tema2::NewGame() {
	cout << "STARTED NEW GAME!\n";
	players[0]->reset();
	players[1]->reset();
	tura = 0;
	normal = false;
	puneAlba = true;
	for (int i = 0; i <= 15; i++) {
		balls[i]->reset();
	}
	time0 = -1.0f;
	primaBilaJoc = true;
	primaBilaLovitura = false;
	fault = false;
}

//creare axa de la tac la bila pentru a creste precizia loviturii 
Mesh* Tema2::CreateAxis(glm::vec3 tac, glm::vec3 bila, glm::vec3 color, float gros) {
	glm::vec3 prTac = glm::vec3(tac.x, bila.y, tac.z);
	glm::vec3 dir = glm::normalize(bila - prTac);

	glm::vec3 ox = glm::vec3(1, 0, 0);
	glm::vec3 tang = ox - dir * glm::dot(ox, dir);
	if (tang.x < precision && tang.z < precision) {
		tang = glm::vec3(0, 0, -1);
	}
	else {
		tang = glm::normalize(tang);
	}
	
	glm::vec3 leftBottomCorner = bila - grosime / 2 * tang;
	Mesh* mesh = Tema2Object3D::CreateRect("axa", leftBottomCorner, dir, tang, lungime, gros, color);
	return mesh;
}

//creare axe cu posibilele directii ale bilelor ce ar putea fi lovite de bila alba, pentru actuala directie a loviturii
void Tema2::CreateOptionalAxis(Tac* tac, Ball* bila) {
	glm::vec3 prTac1 = glm::vec3(tac->position.x, bila->position.y, tac->position.z);
	glm::vec3 dir1 = glm::normalize(bila->position - prTac1);

	glm::vec3 ox1 = glm::vec3(1, 0, 0);
	glm::vec3 tang1 = ox1 - dir1 * glm::dot(ox1, dir1);
	if (tang1.x < precision && tang1.z < precision) {
		tang1 = glm::vec3(0, 0, -1);
	}
	else {
		tang1 = glm::normalize(tang1);
	}


	for (int i = 0; i <= 14; i++) {
		
		Ball* ball = balls[i];
		if (!ball->out) {
			glm::vec3 dir = glm::normalize(ball->position - bila->position);

			glm::vec3 ox = glm::vec3(1, 0, 0);
			glm::vec3 tang = ox - dir * glm::dot(ox, dir);
			if (tang.x < precision && tang.z < precision) {
				tang = glm::vec3(0, 0, -1);
			}
			else {
				tang = glm::normalize(tang);
			}

			glm::vec3 bila_ball = ball->position - bila->position;
			float distPr = glm::dot(bila_ball, dir1);
			if (distPr >= 0) {
				glm::vec3 prTang = bila_ball - distPr * dir1;
				if (length(prTang) <= 2 * radius) {
					glm::vec3 intP = Inters2Circles(bila->position, ball->position, dir1);

					glm::vec3 normala = glm::normalize(ball->position - intP);
					intP = intP + 2 * radius * normala;

					Mesh* mesh = CreateAxis(ball->position, intP, colorAxisOptional, grosime);
					RenderSimpleMesh(mesh, shaders["ShaderTema2"], glm::mat4(1), true);
				}
			}
		}
	}
}

//determinare punct de intersectie a doua bile, primul miscandu-se pe directia dir1 
glm::vec3 Tema2::Inters2Circles(glm::vec3 Ball1, glm::vec3 Ball2, glm::vec3 dir1) {
	glm::vec3 dir2 = glm::normalize(Ball2 - Ball1);

	glm::vec3 prD2 = dir1 * glm::dot(glm::vec3(Ball2 - Ball1), dir1);
	glm::vec3 tgD2 = glm::vec3(Ball2 - Ball1) - prD2;
	float delta = glm::sqrt(4 * radius * radius - length(tgD2) * length(tgD2));

	float D1 = length(prD2);
	glm::vec3 newBall1 = Ball1 + dir1 * (D1 - delta);

	glm::vec3 newD2 = glm::normalize(Ball2 - newBall1);

	return newBall1 + newD2 * radius;
}