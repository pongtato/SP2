/************************************************************************/
/*!
\file SP2.cpp
\Author SP2 Group 4
\par
\brief
Codes to update and render the application.
*/
/************************************************************************/
#include "SP2.h"
#include "GL\glew.h"
#include <sstream>

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "Vertex.h"

#include "LoadTGA.h"

/***************************************************************/
/*!
/brief
Default Constructor
*/
/***************************************************************/
SP2::SP2()
{
	Lightswitch = 1;
}
/***************************************************************/
/*!
/brief
Default Destructor
*/
/***************************************************************/
SP2::~SP2()
{
}
/***************************************************************/
/*!
/brief
Initiation codes for text files, parameters, meshLists, etc.
*/
/***************************************************************/
void SP2::Init()
{
	data.ReadTextFile( "OBJ-Pos/Martpos.txt" );
	shelve.ReadTextFile( "OBJ-Pos/Shelves.txt" );
	cashier.ReadTextFile( "OBJ-Pos/Cashierpos.txt" );
	cashiermodel.ReadTextFile( "OBJ-Pos/CashierModelpos.txt" );
	fridge.ReadTextFile( "OBJ-Pos/Fridgepos.txt" );
	character.ReadTextFile( "OBJ-Pos/NPC/Characterpos.txt" );
	guard.ReadTextFilePath( "OBJ-Pos/NPC/CharacterPath.txt");
	shopper.ReadTextFilePath( "OBJ-Pos/NPC/CharacterPath.txt");
	shopperTwo.ReadTextFilePath( "OBJ-Pos/NPC/CharacterPath.txt");
	patroler.ReadTextFilePath( "OBJ-Pos/NPC/CharacterPath.txt");
	FNB.ReadTextFileItem( "OBJ-Pos/FNB.txt");
	Police.ReadTextFileItem("OBJ-Pos/PoliceInit.txt");
	PoliceCol.ReadTextFileItem("OBJ-Pos/PoliceCol.txt");
	PoliceMan.ReadTextFilePolice("OBJ-Pos/Police.txt");
	Building01.ReadTextFile("OBJ-Pos/Building/Building01Pos.txt");
	Building02.ReadTextFile("OBJ-Pos/Building/Building02Pos.txt");
	Building03.ReadTextFile("OBJ-Pos/Building/Building03Pos.txt");
	Building04.ReadTextFile("OBJ-Pos/Building/Building04Pos.txt");
	Lamp.ReadTextFile("OBJ-Pos/Building/LampPos.txt");
	Locker.ReadTextFile("OBJ-Pos/Building/LockerPos.txt");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);
	//Enable back face culling
	glEnable(GL_CULL_FACE);
	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	
	//Load vertex and fragment shaders
	m_programID = LoadShaders( "Shader//Texture.vertexshader", "Shader//Text.fragmentshader" );
	// Use our shader
	//glUseProgram(m_programID);
	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

	//variable init
	rotateAngle = 0;
	angle = 0;
	walk = false;
	playeridle = false;
	NPC1idle = false;
	NPC2idle = false;
	NPC3idle = false;
	translate = 0;
	translateY = 0;
	LengthY = 0;
	UpdateItemMissing = false;
	GameEndStaff = false;
	GameEndSteal = false;
	GameEndStealLose = false;
	ShopStaff = false;
	float tempX = data.GetRenderPos(10)->getTranslationX();
	srand((unsigned) time(NULL));
	RandomItem = rand() % 1000 + 1;

	//Initialize camera settings
	camera.Init(Vector3(0, 6, 100), Vector3(0, 0, 0), Vector3(0, 1, 0));
	Security1.Init(Vector3(-73,14.5,33), Vector3(0, 0, 0), Vector3(0, 1, 0));
	Security2.Init(Vector3(67,14.5,-35), Vector3(0, 0, 0), Vector3(0, 1, 0));
	EndScene.Init(Vector3(-5,20,-80), Vector3(-5, 0, -140), Vector3(0, 1, 0));

	//Initialize all meshes to NULL
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 2000.f);
	projectionStack.LoadMatrix(projection);

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID,"lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID,"lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID,"lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID,"lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID,"lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID,"lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID,"lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID,"lights[1].exponent");

	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID,"lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID,"lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID,"lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID,"lights[2].exponent");

	m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID, "lights[3].position_cameraspace");
	m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
	m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
	m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID, "lights[3].power");
	m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
	m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
	m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");
	m_parameters[U_LIGHT3_SPOTDIRECTION] = glGetUniformLocation(m_programID,"lights[3].spotDirection");
	m_parameters[U_LIGHT3_COSCUTOFF] = glGetUniformLocation(m_programID,"lights[3].cosCutoff");
	m_parameters[U_LIGHT3_COSINNER] = glGetUniformLocation(m_programID,"lights[3].cosInner");
	m_parameters[U_LIGHT3_EXPONENT] = glGetUniformLocation(m_programID,"lights[3].exponent");

	m_parameters[U_LIGHT4_POSITION] = glGetUniformLocation(m_programID, "lights[4].position_cameraspace");
	m_parameters[U_LIGHT4_TYPE] = glGetUniformLocation(m_programID, "lights[4].type");
	m_parameters[U_LIGHT4_COLOR] = glGetUniformLocation(m_programID, "lights[4].color");
	m_parameters[U_LIGHT4_POWER] = glGetUniformLocation(m_programID, "lights[4].power");
	m_parameters[U_LIGHT4_KC] = glGetUniformLocation(m_programID, "lights[4].kC");
	m_parameters[U_LIGHT4_KL] = glGetUniformLocation(m_programID, "lights[4].kL");
	m_parameters[U_LIGHT4_KQ] = glGetUniformLocation(m_programID, "lights[4].kQ");
	m_parameters[U_LIGHT4_SPOTDIRECTION] = glGetUniformLocation(m_programID,"lights[4].spotDirection");
	m_parameters[U_LIGHT4_COSCUTOFF] = glGetUniformLocation(m_programID,"lights[4].cosCutoff");
	m_parameters[U_LIGHT4_COSINNER] = glGetUniformLocation(m_programID,"lights[4].cosInner");
	m_parameters[U_LIGHT4_EXPONENT] = glGetUniformLocation(m_programID,"lights[4].exponent");

	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID,"numLights");
	
		// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");


			lights[0].type = Light::LIGHT_DIRECTIONAL;
			lights[0].position.Set(0, 100, 50);
			lights[0].color.Set(1, 1, 1);
			lights[0].power = 1;
			lights[0].kC = 1.f;
			lights[0].kL = 0.01f;
			lights[0].kQ = 0.001f;
			lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
			lights[0].cosInner = cos(Math::DegreeToRadian(30));
			lights[0].exponent = 3.f;
			lights[0].spotDirection.Set(0.f, 1.f, 0.f);

			lights[1].type = Light::LIGHT_SPOT;
			lights[1].position.Set(0, 14.5, 15);
			lights[1].color.Set(1, 1, 1);
			lights[1].power = 0.2;
			lights[1].kC = 1.f;
			lights[1].kL = 0.01f;
			lights[1].kQ = 0.001f;
			lights[1].cosCutoff = cos(Math::DegreeToRadian(65));
			lights[1].cosInner = cos(Math::DegreeToRadian(30));
			lights[1].exponent = 3.f;
			lights[1].spotDirection.Set(0.f, 1.f, 0.f);

			lights[2].type = Light::LIGHT_SPOT;
			lights[2].position.Set(0, 14.5, -15);
			lights[2].color.Set(1, 1, 1);
			lights[2].power = 0.2;
			lights[2].kC = 1.f;
			lights[2].kL = 0.01f;
			lights[2].kQ = 0.001f;
			lights[2].cosCutoff = cos(Math::DegreeToRadian(65));
			lights[2].cosInner = cos(Math::DegreeToRadian(30));
			lights[2].exponent = 3.f;
			lights[2].spotDirection.Set(0.f, 1.f, 0.f);

			lights[3].type = Light::LIGHT_SPOT;
			lights[3].position.Set(-35, 14.5, 15);
			lights[3].color.Set(1, 1, 1);
			lights[3].power = 0.2;
			lights[3].kC = 1.f;
			lights[3].kL = 0.01f;
			lights[3].kQ = 0.001f;
			lights[3].cosCutoff = cos(Math::DegreeToRadian(65));
			lights[3].cosInner = cos(Math::DegreeToRadian(30));
			lights[3].exponent = 3.f;
			lights[3].spotDirection.Set(0.f, 1.f, 0.f);

			lights[4].type = Light::LIGHT_SPOT;
			lights[4].position.Set(-35, 14.5, -15);
			lights[4].color.Set(1, 1, 1);
			lights[4].power = 0.2;
			lights[4].kC = 1.f;
			lights[4].kL = 0.01f;
			lights[4].kQ = 0.001f;
			lights[4].cosCutoff = cos(Math::DegreeToRadian(65));
			lights[4].cosInner = cos(Math::DegreeToRadian(30));
			lights[4].exponent = 3.f;
			lights[4].spotDirection.Set(0.f, 1.f, 0.f);

	glUseProgram(m_programID);

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform1i(m_parameters[U_NUMLIGHTS], 2);
	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	glUniform1i(m_parameters[U_NUMLIGHTS], 3);
	glUniform1i(m_parameters[U_LIGHT2_TYPE], lights[2].type);
	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &lights[2].color.r);
	glUniform1f(m_parameters[U_LIGHT2_POWER], lights[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], lights[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], lights[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], lights[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], lights[2].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], lights[2].cosInner);
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], lights[2].exponent);

	glUniform1i(m_parameters[U_NUMLIGHTS], 4);
	glUniform1i(m_parameters[U_LIGHT3_TYPE], lights[3].type);
	glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &lights[3].color.r);
	glUniform1f(m_parameters[U_LIGHT3_POWER], lights[3].power);
	glUniform1f(m_parameters[U_LIGHT3_KC], lights[3].kC);
	glUniform1f(m_parameters[U_LIGHT3_KL], lights[3].kL);
	glUniform1f(m_parameters[U_LIGHT3_KQ], lights[3].kQ);
	glUniform1f(m_parameters[U_LIGHT3_COSCUTOFF], lights[3].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT3_COSINNER], lights[3].cosInner);
	glUniform1f(m_parameters[U_LIGHT3_EXPONENT], lights[3].exponent);

	glUniform1i(m_parameters[U_NUMLIGHTS], 5);
	glUniform1i(m_parameters[U_LIGHT4_TYPE], lights[4].type);
	glUniform3fv(m_parameters[U_LIGHT4_COLOR], 1, &lights[4].color.r);
	glUniform1f(m_parameters[U_LIGHT4_POWER], lights[4].power);
	glUniform1f(m_parameters[U_LIGHT4_KC], lights[4].kC);
	glUniform1f(m_parameters[U_LIGHT4_KL], lights[4].kL);
	glUniform1f(m_parameters[U_LIGHT4_KQ], lights[4].kQ);
	glUniform1f(m_parameters[U_LIGHT4_COSCUTOFF], lights[4].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT4_COSINNER], lights[4].cosInner);
	glUniform1f(m_parameters[U_LIGHT4_EXPONENT], lights[4].exponent);
	
	//Light ball
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 10, 10, 1);
	meshList[GEO_LIGHTBALL2] = MeshBuilder::GenerateSphere("lightball2", Color(1, 1, 1), 10, 10, 1);

	meshList[GEO_XHAIR] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_XHAIR]->textureID = LoadTGA("Image//xhair.tga");

	meshList[GEO_ALERT] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_ALERT]->textureID = LoadTGA("Image//alert.tga");

	meshList[GEO_HP] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_HP]->textureID = LoadTGA("Image//hp.tga");

	meshList[GEO_EXPLODE] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_EXPLODE]->textureID = LoadTGA("Image//boom.tga");

	meshList[GEO_STAM] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_STAM]->textureID = LoadTGA("Image//stamina.tga");

	meshList[GEO_HPI] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_HPI]->textureID = LoadTGA("Image//hpicon.tga");

	meshList[GEO_STAMI] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_STAMI]->textureID = LoadTGA("Image//staminaicon.tga");

	meshList[GEO_UI] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_UI]->textureID = LoadTGA("Image//UI.tga");

	meshList[GEO_STARTBTN] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_STARTBTN]->textureID = LoadTGA("Image//Startbtn.tga");

	meshList[GEO_INSTRUCTIONBTN] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_INSTRUCTIONBTN]->textureID = LoadTGA("Image//Instructions.tga");

	meshList[GEO_ACHIEVEBTN] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_ACHIEVEBTN]->textureID = LoadTGA("Image//Achievements.tga");

	meshList[GEO_LOGO] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_LOGO]->textureID = LoadTGA("Image//Logo.tga");

	meshList[GEO_ENDSCREEN] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_ENDSCREEN]->textureID = LoadTGA("Image//losescreen.tga");

	meshList[GEO_WINSCREEN] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_WINSCREEN]->textureID = LoadTGA("Image//winscreen.tga");

	meshList[GEO_INSTRUCTIONSSCREEN] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_INSTRUCTIONSSCREEN]->textureID = LoadTGA("Image//Tint.tga");

	meshList[GEO_ACHIEVESCREEN] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_ACHIEVESCREEN]->textureID = LoadTGA("Image//Tint.tga");

	meshList[GEO_TINT] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_TINT]->textureID = LoadTGA("Image//Tint.tga");

	meshList[GEO_EXITBTN] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_EXITBTN]->textureID = LoadTGA("Image//Exit.tga");

	meshList[GEO_DIALOGUEBOX] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_DIALOGUEBOX]->textureID = LoadTGA("Image//dialoguebox.tga");

	meshList[GEO_PINSTRUCTBOX] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_PINSTRUCTBOX]->textureID = LoadTGA("Image//Pinstruct.tga");

	meshList[GEO_SINSTRUCTBOX] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_SINSTRUCTBOX]->textureID = LoadTGA("Image//Sinstruct.tga");

	meshList[GEO_MENUBOX] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_MENUBOX]->textureID = LoadTGA("Image//menubox.tga");

	meshList[GEO_MAIN_INSTRUCT] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_MAIN_INSTRUCT]->textureID = LoadTGA("Image//Instruct.tga");

	meshList[GEO_MENUSELECT] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_MENUSELECT]->textureID = LoadTGA("Image//menuselect.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//()airBK.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//()airFT.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//()airDN.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//()airUP.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//()airRT.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//()airLT.tga");

	meshList[GEO_MODEL_DOORMAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//doorman.obj");
	meshList[GEO_MODEL_DOORMAN]->textureID = LoadTGA("Image//doorman.tga");

	meshList[GEO_MODEL_MART] = MeshBuilder::GenerateOBJ("model1", "OBJ//Mart.obj");
	meshList[GEO_MODEL_MART]->textureID = LoadTGA("Image//supermart.tga");

	meshList[GEO_MODEL_DOOR] = MeshBuilder::GenerateOBJ("model1", "OBJ//Door.obj");
	meshList[GEO_MODEL_DOOR]->textureID = LoadTGA("Image//Door.tga");

	meshList[GEO_MODEL_CASHIER] = MeshBuilder::GenerateOBJ("model1", "OBJ//cashier table.obj");
	meshList[GEO_MODEL_CASHIER]->textureID = LoadTGA("Image//cashier table.tga");

	meshList[GEO_MODEL_CHAR1] = MeshBuilder::GenerateOBJ("model1", "OBJ//char1.obj");
	meshList[GEO_MODEL_CHAR1]->textureID = LoadTGA("Image//Door.tga");

	meshList[GEO_MODEL_FRIDGE] = MeshBuilder::GenerateOBJ("model1", "OBJ//fridge.obj");
	meshList[GEO_MODEL_FRIDGE]->textureID = LoadTGA("Image//fridge.tga");

	meshList[GEO_MODEL_SHELF] = MeshBuilder::GenerateOBJ("model1", "OBJ//shelf.obj");
	meshList[GEO_MODEL_SHELF]->textureID = LoadTGA("Image//Shelf_Texture2.tga");

	meshList[GEO_MODEL_FLOOR] = MeshBuilder::GenerateOBJ("model1", "OBJ//Floor.obj");
	meshList[GEO_MODEL_FLOOR]->textureID = LoadTGA("Image//Floor.tga");

	meshList[GEO_MODEL_CEREAL_1] = MeshBuilder::GenerateOBJ("model1", "OBJ//cerealbox1.obj");
	meshList[GEO_MODEL_CEREAL_1]->textureID = LoadTGA("Image//cereal box 1.tga");

	meshList[GEO_MODEL_CEREAL_2] = MeshBuilder::GenerateOBJ("model1", "OBJ//cerealbox2.obj");
	meshList[GEO_MODEL_CEREAL_2]->textureID = LoadTGA("Image//cereal box 2.tga");

	meshList[GEO_MODEL_CEREAL_3] = MeshBuilder::GenerateOBJ("model1", "OBJ//cerealbox3.obj");
	meshList[GEO_MODEL_CEREAL_3]->textureID = LoadTGA("Image//cereal box 3.tga");

	meshList[GEO_MODEL_WATERCAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//water.obj");
	meshList[GEO_MODEL_WATERCAN]->textureID = LoadTGA("Image//water.tga");

	meshList[GEO_MODEL_MTDEW] = MeshBuilder::GenerateOBJ("model1", "OBJ//mountainDew.obj");
	meshList[GEO_MODEL_MTDEW]->textureID = LoadTGA("Image//Mountain Dew Texture.tga");

	meshList[GEO_MODEL_COKE] = MeshBuilder::GenerateOBJ("model1", "OBJ//coke.obj");
	meshList[GEO_MODEL_COKE]->textureID = LoadTGA("Image//Coke.tga");

	meshList[GEO_MODEL_PEPSI] = MeshBuilder::GenerateOBJ("model1", "OBJ//pepsi.obj");
	meshList[GEO_MODEL_PEPSI]->textureID = LoadTGA("Image//Pepsi.tga");

	meshList[GEO_MODEL_HERSHEY] = MeshBuilder::GenerateOBJ("model1", "OBJ//hershey.obj");
	meshList[GEO_MODEL_HERSHEY]->textureID = LoadTGA("Image//hershey.tga");

	meshList[GEO_MODEL_ROCHER] = MeshBuilder::GenerateOBJ("model1", "OBJ//FerreroRocher.obj");
	meshList[GEO_MODEL_ROCHER]->textureID = LoadTGA("Image//rocher.tga");

	meshList[GEO_MODEL_BBCAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//bbCan.obj");
	meshList[GEO_MODEL_BBCAN]->textureID = LoadTGA("Image//bbCan.tga");

	meshList[GEO_MODEL_PUMPKINCAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//pumpkinCan.obj");
	meshList[GEO_MODEL_PUMPKINCAN]->textureID = LoadTGA("Image//pumpkinCan.tga");

	meshList[GEO_MODEL_TOMATOSOUPCAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//tomatoSoupCan.obj");
	meshList[GEO_MODEL_TOMATOSOUPCAN]->textureID = LoadTGA("Image//tomatoSoupCan.tga");

	meshList[GEO_MODEL_PEASNCARROTCAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//peasNcarrot.obj");
	meshList[GEO_MODEL_PEASNCARROTCAN]->textureID = LoadTGA("Image//peasNcarrot.tga");

	meshList[GEO_MODEL_LAYSCHIPS] = MeshBuilder::GenerateOBJ("model1", "OBJ//LaysChips.obj");
	meshList[GEO_MODEL_LAYSCHIPS]->textureID = LoadTGA("Image//LaysChips.tga");

	meshList[GEO_MODEL_PIZZA] = MeshBuilder::GenerateOBJ("model1", "OBJ//pizza.obj");
	meshList[GEO_MODEL_PIZZA]->textureID = LoadTGA("Image//frozen_pizza.tga");
	
	meshList[GEO_MODEL_ICECREAM] = MeshBuilder::GenerateOBJ("model1", "OBJ//icecream.obj");
	meshList[GEO_MODEL_ICECREAM]->textureID = LoadTGA("Image//ice_cream_box.tga");

	meshList[GEO_MODEL_MCNCHEESE] = MeshBuilder::GenerateOBJ("model1", "OBJ//mcncheese.obj");
	meshList[GEO_MODEL_MCNCHEESE]->textureID = LoadTGA("Image//Box_Tex.tga");

	meshList[GEO_MODEL_SARDINES] = MeshBuilder::GenerateOBJ("model1", "OBJ//sardines.obj");
	meshList[GEO_MODEL_SARDINES]->textureID = LoadTGA("Image//Can_Tex.tga");

	meshList[GEO_MODEL_AYAMCAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//ayamcan.obj");
	meshList[GEO_MODEL_AYAMCAN]->textureID = LoadTGA("Image//Can2_Tex.tga");

	meshList[GEO_MODEL_CHICKENSTOCK] = MeshBuilder::GenerateOBJ("model1", "OBJ//chickenstock.obj");
	meshList[GEO_MODEL_CHICKENSTOCK]->textureID = LoadTGA("Image//Packet_Tex.tga");

	meshList[GEO_MODEL_TOBLERONE] = MeshBuilder::GenerateOBJ("model1", "OBJ//toblerone.obj");
	meshList[GEO_MODEL_TOBLERONE]->textureID = LoadTGA("Image//Packet2_Tex.tga");

	meshList[GEO_MODEL_VEGCAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//vegcan.obj");
	meshList[GEO_MODEL_VEGCAN]->textureID = LoadTGA("Image//vegcan.tga");

	meshList[GEO_MODEL_MILO] = MeshBuilder::GenerateOBJ("model1", "OBJ//milo.obj");
	meshList[GEO_MODEL_MILO]->textureID = LoadTGA("Image//milo.tga");

	meshList[GEO_MODEL_SARDCAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//sardcan.obj");
	meshList[GEO_MODEL_SARDCAN]->textureID = LoadTGA("Image//sardcan.tga");

	meshList[GEO_MODEL_CACTUS] = MeshBuilder::GenerateOBJ("model1", "OBJ//cactus.obj");
	meshList[GEO_MODEL_CACTUS]->textureID = LoadTGA("Image//cactus.tga");

	meshList[GEO_MODEL_REDITOS] = MeshBuilder::GenerateOBJ("model1", "OBJ//reditos.obj");
	meshList[GEO_MODEL_REDITOS]->textureID = LoadTGA("Image//reditos.tga");

	meshList[GEO_MODEL_DEWITOS] = MeshBuilder::GenerateOBJ("model1", "OBJ//dewitos.obj");
	meshList[GEO_MODEL_DEWITOS]->textureID = LoadTGA("Image//dewitos.tga");

	meshList[GEO_MODEL_DIABETOS] = MeshBuilder::GenerateOBJ("model1", "OBJ//diabetos.obj");
	meshList[GEO_MODEL_DIABETOS]->textureID = LoadTGA("Image//diabetos.tga");

	meshList[GEO_MODEL_MOATIES] = MeshBuilder::GenerateOBJ("model1", "OBJ//moaties.obj");
	meshList[GEO_MODEL_MOATIES]->textureID = LoadTGA("Image//moaties.tga");

	meshList[GEO_MODEL_SWBOX] = MeshBuilder::GenerateOBJ("model1", "OBJ//SWBox.obj");
	meshList[GEO_MODEL_SWBOX]->textureID = LoadTGA("Image//SWBox.tga");

	meshList[GEO_MODEL_DOSH] = MeshBuilder::GenerateOBJ("model1", "OBJ//dosh.obj");
	meshList[GEO_MODEL_DOSH]->textureID = LoadTGA("Image//Door.tga");

	meshList[GEO_MODEL_PISTOL] = MeshBuilder::GenerateOBJ("model1", "OBJ//handgun.obj");
	meshList[GEO_MODEL_PISTOL]->textureID = LoadTGA("Image//DOOR2.tga");

	meshList[GEO_MODEL_RIFLE] = MeshBuilder::GenerateOBJ("model1", "OBJ//rifle.obj");
	meshList[GEO_MODEL_RIFLE]->textureID = LoadTGA("Image//rifle.tga");

	meshList[GEO_MODEL_SFFOOD] = MeshBuilder::GenerateOBJ("model1", "OBJ//supplyfrozen.obj");
	meshList[GEO_MODEL_SFFOOD]->textureID = LoadTGA("Image//supplies.tga");

	meshList[GEO_MODEL_SCANS] = MeshBuilder::GenerateOBJ("model1", "OBJ//supplycan.obj");
	meshList[GEO_MODEL_SCANS]->textureID = LoadTGA("Image//supplies.tga");

	meshList[GEO_MODEL_SCVEG] = MeshBuilder::GenerateOBJ("model1", "OBJ//supplyveg.obj");
	meshList[GEO_MODEL_SCVEG]->textureID = LoadTGA("Image//supplies.tga");

	meshList[GEO_MODEL_SCHIPS] = MeshBuilder::GenerateOBJ("model1", "OBJ//supplychips.obj");
	meshList[GEO_MODEL_SCHIPS]->textureID = LoadTGA("Image//supplies.tga");

	meshList[GEO_MODEL_SCEREAL] = MeshBuilder::GenerateOBJ("model1", "OBJ//supplycereal.obj");
	meshList[GEO_MODEL_SCEREAL]->textureID = LoadTGA("Image//supplies.tga");

	meshList[GEO_MODEL_SGUNS] = MeshBuilder::GenerateOBJ("model1", "OBJ//supplygun.obj");
	meshList[GEO_MODEL_SGUNS]->textureID = LoadTGA("Image//supplies.tga");

	meshList[GEO_MODEL_STFOOD] = MeshBuilder::GenerateOBJ("model1", "OBJ//supplytfood.obj");
	meshList[GEO_MODEL_STFOOD]->textureID = LoadTGA("Image//supplies.tga");

	meshList[GEO_MODEL_CAMERA] = MeshBuilder::GenerateOBJ("model1", "OBJ//camera.obj");
	meshList[GEO_MODEL_CAMERA]->textureID = LoadTGA("Image//CAMERA.tga");

	meshList[GEO_MODEL_SHUTTER] = MeshBuilder::GenerateOBJ("model1", "OBJ//shutter.obj");
	meshList[GEO_MODEL_SHUTTER]->textureID = LoadTGA("Image//shutter.tga");

	meshList[GEO_MODEL_STAFFDOOR] = MeshBuilder::GenerateOBJ("model1", "OBJ//Door.obj");
	meshList[GEO_MODEL_STAFFDOOR]->textureID = LoadTGA("Image//DOOR2.tga");

	meshList[GEO_MODEL_ROAD] = MeshBuilder::GenerateOBJ("model1", "OBJ//road.obj");
	meshList[GEO_MODEL_ROAD]->textureID = LoadTGA("Image//road.tga");

	meshList[GEO_MODEL_FENCE] = MeshBuilder::GenerateOBJ("model1", "OBJ//fence.obj");
	meshList[GEO_MODEL_FENCE]->textureID = LoadTGA("Image//fence.tga");

	meshList[GEO_MODEL_BOARD1] = MeshBuilder::GenerateOBJ("model1", "OBJ//Board.obj");
	meshList[GEO_MODEL_BOARD1]->textureID = LoadTGA("Image//board1.tga");

	meshList[GEO_MODEL_BOARD2] = MeshBuilder::GenerateOBJ("model1", "OBJ//Board.obj");
	meshList[GEO_MODEL_BOARD2]->textureID = LoadTGA("Image//board2.tga");

	meshList[GEO_MODEL_BOARD3] = MeshBuilder::GenerateOBJ("model1", "OBJ//Board.obj");
	meshList[GEO_MODEL_BOARD3]->textureID = LoadTGA("Image//board3.tga");

	meshList[GEO_MODEL_BOARD4] = MeshBuilder::GenerateOBJ("model1", "OBJ//Board.obj");
	meshList[GEO_MODEL_BOARD4]->textureID = LoadTGA("Image//board4.tga");

	meshList[GEO_MODEL_LIGHTS] = MeshBuilder::GenerateOBJ("model1", "OBJ//Lights.obj");
	meshList[GEO_MODEL_LIGHTS]->textureID = LoadTGA("Image//supermart.tga");

	meshList[GEO_MODEL_GUARDHOUSE] = MeshBuilder::GenerateOBJ("model1", "OBJ//GuardHouse.obj");
	meshList[GEO_MODEL_GUARDHOUSE]->textureID = LoadTGA("Image//guardhouse.tga");

	meshList[GEO_MODEL_GUARDCONTROL] = MeshBuilder::GenerateOBJ("model1", "OBJ//GuardControl.obj");
	meshList[GEO_MODEL_GUARDCONTROL]->textureID = LoadTGA("Image//guardhouse.tga");

	meshList[GEO_MODEL_TROLLEY] = MeshBuilder::GenerateOBJ("model1", "OBJ//Cart.obj");
	meshList[GEO_MODEL_TROLLEY]->textureID = LoadTGA("Image//Cart.tga");
	
	//Character(basic)
	meshList[GEO_MODEL_HEAD] = MeshBuilder::GenerateOBJ("model1", "OBJ//head.obj");
	meshList[GEO_MODEL_HEAD]->textureID = LoadTGA("Image//characterskin.tga");

	meshList[GEO_MODEL_BODY] = MeshBuilder::GenerateOBJ("model1", "OBJ//body.obj");
	meshList[GEO_MODEL_BODY]->textureID = LoadTGA("Image//characterskin.tga");

	meshList[GEO_MODEL_LEFTARM] = MeshBuilder::GenerateOBJ("model1", "OBJ//leftarm.obj");
	meshList[GEO_MODEL_LEFTARM]->textureID = LoadTGA("Image//characterskin.tga");

	meshList[GEO_MODEL_RIGHTARM] = MeshBuilder::GenerateOBJ("model1", "OBJ//leftarm.obj");
	meshList[GEO_MODEL_RIGHTARM]->textureID = LoadTGA("Image//characterskin.tga");

	meshList[GEO_MODEL_LEFTLEG] = MeshBuilder::GenerateOBJ("model1", "OBJ//leftleg.obj");
	meshList[GEO_MODEL_LEFTLEG]->textureID = LoadTGA("Image//characterskin.tga");

	meshList[GEO_MODEL_RIGHTLEG] = MeshBuilder::GenerateOBJ("model1", "OBJ//leftleg.obj");
	meshList[GEO_MODEL_RIGHTLEG]->textureID = LoadTGA("Image//characterskin.tga");

	//Character(shopper2)
	meshList[GEO_MODEL_SHOPPER2_HEAD] = MeshBuilder::GenerateOBJ("model1", "OBJ//head.obj");
	meshList[GEO_MODEL_SHOPPER2_HEAD]->textureID = LoadTGA("Image//shopper2skin.tga");

	meshList[GEO_MODEL_SHOPPER2_BODY] = MeshBuilder::GenerateOBJ("model1", "OBJ//body.obj");
	meshList[GEO_MODEL_SHOPPER2_BODY]->textureID = LoadTGA("Image//shopper2skin.tga");

	meshList[GEO_MODEL_SHOPPER2_LEFTARM] = MeshBuilder::GenerateOBJ("model1", "OBJ//leftarm.obj");
	meshList[GEO_MODEL_SHOPPER2_LEFTARM]->textureID = LoadTGA("Image//shopper2skin.tga");

	meshList[GEO_MODEL_SHOPPER2_RIGHTARM] = MeshBuilder::GenerateOBJ("model1", "OBJ//leftarm.obj");
	meshList[GEO_MODEL_SHOPPER2_RIGHTARM]->textureID = LoadTGA("Image//shopper2skin.tga");

	meshList[GEO_MODEL_SHOPPER2_LEFTLEG] = MeshBuilder::GenerateOBJ("model1", "OBJ//leftleg.obj");
	meshList[GEO_MODEL_SHOPPER2_LEFTLEG]->textureID = LoadTGA("Image//shopper2skin.tga");

	meshList[GEO_MODEL_SHOPPER2_RIGHTLEG] = MeshBuilder::GenerateOBJ("model1", "OBJ//leftleg.obj");
	meshList[GEO_MODEL_SHOPPER2_RIGHTLEG]->textureID = LoadTGA("Image//shopper2skin.tga");

	//Character(cashier person)
	meshList[GEO_MODEL_CASHIER_HEAD] = MeshBuilder::GenerateOBJ("model1", "OBJ//head.obj");
	meshList[GEO_MODEL_CASHIER_HEAD]->textureID = LoadTGA("Image//cashierskin.tga");

	meshList[GEO_MODEL_CASHIER_BODY] = MeshBuilder::GenerateOBJ("model1", "OBJ//body.obj");
	meshList[GEO_MODEL_CASHIER_BODY]->textureID = LoadTGA("Image//cashierskin.tga");

	meshList[GEO_MODEL_CASHIER_LEFTARM] = MeshBuilder::GenerateOBJ("model1", "OBJ//leftarm.obj");
	meshList[GEO_MODEL_CASHIER_LEFTARM]->textureID = LoadTGA("Image//cashierskin.tga");

	meshList[GEO_MODEL_CASHIER_RIGHTARM] = MeshBuilder::GenerateOBJ("model1", "OBJ//leftarm.obj");
	meshList[GEO_MODEL_CASHIER_RIGHTARM]->textureID = LoadTGA("Image//cashierskin.tga");

	meshList[GEO_MODEL_CASHIER_LEFTLEG] = MeshBuilder::GenerateOBJ("model1", "OBJ//leftleg.obj");
	meshList[GEO_MODEL_CASHIER_LEFTLEG]->textureID = LoadTGA("Image//cashierskin.tga");

	meshList[GEO_MODEL_CASHIER_RIGHTLEG] = MeshBuilder::GenerateOBJ("model1", "OBJ//leftleg.obj");
	meshList[GEO_MODEL_CASHIER_RIGHTLEG]->textureID = LoadTGA("Image//cashierskin.tga");

	//player
	meshList[GEO_PLAYER_ARMSL] = MeshBuilder::GenerateOBJ("model1", "OBJ//Arm.obj");
	meshList[GEO_PLAYER_ARMSL]->textureID = LoadTGA("Image//POLICE.tga");

	meshList[GEO_PLAYER_ARMSR] = MeshBuilder::GenerateOBJ("model1", "OBJ//Arm.obj");
	meshList[GEO_PLAYER_ARMSR]->textureID = LoadTGA("Image//POLICE.tga");

	//bullet
	meshList[GEO_MODEL_BULLET] = MeshBuilder::GenerateOBJ("model1", "OBJ//bullet.obj");
	meshList[GEO_MODEL_BULLET]->textureID = LoadTGA("Image//stamina.tga");

	//police car
	meshList[GEO_MODEL_POLICEVAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//PoliceVan.obj");
	meshList[GEO_MODEL_POLICEVAN]->textureID = LoadTGA("Image//PoliceVan.tga");

	//police car
	meshList[GEO_MODEL_ESCAPE] = MeshBuilder::GenerateOBJ("model1", "OBJ//PoliceVan.obj");
	meshList[GEO_MODEL_ESCAPE]->textureID = LoadTGA("Image//Escape.tga");

	meshList[GEO_MODEL_POLICEVANWHEEL] = MeshBuilder::GenerateOBJ("model1", "OBJ//PoliceVanWheel.obj");
	meshList[GEO_MODEL_POLICEVANWHEEL]->textureID = LoadTGA("Image//Cart.tga");
	//Character(security guard)
	meshList[GEO_MODEL_GUARD_HEAD] = MeshBuilder::GenerateOBJ("model1", "OBJ//head.obj");
	meshList[GEO_MODEL_GUARD_HEAD]->textureID = LoadTGA("Image//guardskin.tga");

	meshList[GEO_MODEL_GUARD_BODY] = MeshBuilder::GenerateOBJ("model1", "OBJ//body.obj");
	meshList[GEO_MODEL_GUARD_BODY]->textureID = LoadTGA("Image//guardskin.tga");

	meshList[GEO_MODEL_GUARD_LEFTARM] = MeshBuilder::GenerateOBJ("model1", "OBJ//leftarm.obj");
	meshList[GEO_MODEL_GUARD_LEFTARM]->textureID = LoadTGA("Image//guardskin.tga");

	meshList[GEO_MODEL_GUARD_RIGHTARM] = MeshBuilder::GenerateOBJ("model1", "OBJ//leftarm.obj");
	meshList[GEO_MODEL_GUARD_RIGHTARM]->textureID = LoadTGA("Image//guardskin.tga");

	meshList[GEO_MODEL_GUARD_LEFTLEG] = MeshBuilder::GenerateOBJ("model1", "OBJ//leftleg.obj");
	meshList[GEO_MODEL_GUARD_LEFTLEG]->textureID = LoadTGA("Image//guardskin.tga");

	meshList[GEO_MODEL_GUARD_RIGHTLEG] = MeshBuilder::GenerateOBJ("model1", "OBJ//leftleg.obj");
	meshList[GEO_MODEL_GUARD_RIGHTLEG]->textureID = LoadTGA("Image//guardskin.tga");

	meshList[GEO_MODEL_POLICEMAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//Policeman.obj");
	meshList[GEO_MODEL_POLICEMAN]->textureID = LoadTGA("Image//POLICE.tga");

	meshList[GEO_MODEL_POLICEMAN_DAMAGED] = MeshBuilder::GenerateOBJ("model1", "OBJ//Policeman.obj");
	meshList[GEO_MODEL_POLICEMAN_DAMAGED]->textureID = LoadTGA("Image//POLICE_HIT.tga");

	meshList[GEO_MODEL_BROKEN] = MeshBuilder::GenerateOBJ("model1", "OBJ//Broken_fence.obj");
	meshList[GEO_MODEL_BROKEN]->textureID = LoadTGA("Image//fence.tga");

	meshList[GEO_BUILDING01] = MeshBuilder::GenerateOBJ("model1", "OBJ//Building01.obj");
	meshList[GEO_BUILDING01]->textureID = LoadTGA("Image//Building01.tga");

	meshList[GEO_BUILDING02] = MeshBuilder::GenerateOBJ("model1", "OBJ//Building02.obj");
	meshList[GEO_BUILDING02]->textureID = LoadTGA("Image//Building02.tga");

	meshList[GEO_BUILDING03] = MeshBuilder::GenerateOBJ("model1", "OBJ//Building03.obj");
	meshList[GEO_BUILDING03]->textureID = LoadTGA("Image//Building03.tga");

	meshList[GEO_BUILDING04] = MeshBuilder::GenerateOBJ("model1", "OBJ//Building04.obj");
	meshList[GEO_BUILDING04]->textureID = LoadTGA("Image//Building04.tga");

	meshList[GEO_LAMP] = MeshBuilder::GenerateOBJ("model1", "OBJ//Lamp.obj");
	meshList[GEO_LAMP]->textureID = LoadTGA("Image//Lamp.tga");

	meshList[GEO_FLAG] = MeshBuilder::GenerateOBJ("model1", "OBJ//Flag.obj");
	meshList[GEO_FLAG]->textureID = LoadTGA("Image//Flag.tga");

	meshList[GEO_LOCKER] = MeshBuilder::GenerateOBJ("model1", "OBJ//Locker.obj");
	meshList[GEO_LOCKER]->textureID = LoadTGA("Image//Locker.tga");

	meshList[GEO_MISSINGITEM] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_MISSINGITEM]->textureID = LoadTGA("Image//MissingItem.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//comic.tga");

	EntranceDoorSlide = data.GetRenderPos(1)->getTranslationX();
	ExitDoorSlide = data.GetRenderPos(2)->getTranslationX();
	ShutterDoorOpen = data.GetRenderPos(8)->getTranslationY();
	LeftStaffDoorSlide = data.GetRenderPos(4)->getTranslationX();
	RightStaffDoorSlide = data.GetRenderPos(5)->getTranslationX();

	npc.setName(character.GetRenderPos(0)->getName());
	npc.setPosX(character.GetRenderPos(0)->getTranslationX());
	npc.setPosZ(character.GetRenderPos(0)->getTranslationZ());
	guard.setName(character.GetRenderPos(1)->getName());
	guard.setPosX(character.GetRenderPos(1)->getTranslationX());
	guard.setPosZ(character.GetRenderPos(1)->getTranslationZ());
	shopper.setName(character.GetRenderPos(2)->getName());
	shopper.setPosX(character.GetRenderPos(2)->getTranslationX());
	shopper.setPosZ(character.GetRenderPos(2)->getTranslationZ());
	shopperTwo.setName(character.GetRenderPos(3)->getName());
	shopperTwo.setPosX(character.GetRenderPos(3)->getTranslationX());
	shopperTwo.setPosZ(character.GetRenderPos(3)->getTranslationZ());
	police = false;
	EscapeCarMove = 0;
	ArmSwing = 0;
	MenuKey = true;
	ExplosionScale = 50;
	camera.setCameraState(2);
	EscapeEnd = false;
	MenuState = 1;
	GunOffset = 0;
	GunSwing = -30.f;
	DupeRot = 0;
	DupeRotArms = 0;
	DupeRotArmsY = 0;
	GameEndStaff = false;
	GameEndSteal = false;
	ROLE = "Shopper";
	Instruct = false;
	PInstruct = false;
	SInstruct = false;
	//Play Music
	MusicPlayer.MainMusic();
	//Init Police bounds
	for ( int i = 0; i < PoliceMan.ReadTextFilePoliceSize(); ++i)
	{
		if ( PoliceMan.GetRenderPosPolice(i)->getItemAvailability() == true )
		{
			CCollisionBounds* temp =  new CCollisionBounds();
			temp->SetCollisionBounds(PoliceMan.GetRenderPosPolice(i)->getItemTranslationX(),PoliceMan.GetRenderPosPolice(i)->getItemTranslationY(),PoliceMan.GetRenderPosPolice(i)->getItemTranslationZ(),2,7,2);
			colBounds.push_back(temp);
		}
	}

	//Init Police van bounds
	for ( int i =0; i < PoliceCol.ReturnListSize(); ++i)
	{
		if ( i < 4)
		{
			if ( PoliceCol.GetRenderPosItem(i)->getItemAvailability() == true )
			{
				CCollisionBounds* temp =  new CCollisionBounds();
				temp->SetCollisionBounds(PoliceCol.GetRenderPosItem(i)->getItemTranslationX(),PoliceCol.GetRenderPosItem(i)->getItemTranslationY(),PoliceCol.GetRenderPosItem(i)->getItemTranslationZ(),12,5,5);
				VanBounds.push_back(temp);
			}
		}
		else
		{
			if ( PoliceCol.GetRenderPosItem(i)->getItemAvailability() == true )
			{
				CCollisionBounds* temp =  new CCollisionBounds();
				temp->SetCollisionBounds(PoliceCol.GetRenderPosItem(i)->getItemTranslationX(),PoliceCol.GetRenderPosItem(i)->getItemTranslationY(),PoliceCol.GetRenderPosItem(i)->getItemTranslationZ(),5,5,12);
				VanBounds.push_back(temp);
			}
		}
	}

	//Init Locker bounds
	for ( int i =0; i < Locker.ReturnReadListSize(); ++i)
	{
		CCollisionBounds* temp =  new CCollisionBounds();
		temp->SetCollisionBounds(Locker.GetRenderPos(i)->getTranslationX(),Locker.GetRenderPos(i)->getTranslationY(),Locker.GetRenderPos(i)->getTranslationZ(),2,7,2);
		LockerBounds.push_back(temp);
	}
}

//Animate walk speed
static float WSPEED = 50.0f;
/***************************************************************/
/*!
/brief
Codes for the numerical controls and police updating
*/
/***************************************************************/
void SP2::Update(double dt)
{

	float LSPEED = 10.f;
	if(Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if(Application::IsKeyPressed('4')) 
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	
	//Main menu update or exited main menu
	if(MenuKey == true)
	{
		MenuUpdate(dt);
	}
	else
		LightsReset();

	if ( Application::IsKeyPressed('E') && police != true && ShopStaff != true)
		{
			if ( camera.target.x >= LockerBounds[0]->TTL.x && camera.target.x <= LockerBounds[0]->TTR.x
				&& camera.target.y >= LockerBounds[0]->BTL.y && camera.target.y <= LockerBounds[0]->TTL.y
				&& camera.target.z >= LockerBounds[0]->TTL.z && camera.target.z <= LockerBounds[0]->TBL.z)
			{
				ShopStaff = true;
				ItemMissing();
			}
		}

	//Activate security camera
	if ( ItemName == "Camera")
	{
		if(Application::IsKeyPressed('5'))
		{
			camera.setCameraState(1);
			camera.CameraMode = true;
		}

		if(Application::IsKeyPressed('6'))
		{
			camera.setCameraState(2);
			camera.CameraMode = true;
		}
		if(Application::IsKeyPressed('7'))
		{
			camera.setCameraState(0);
			camera.CameraMode = false;
		}
	}

	CheckItem();
	DoorSlide();
	ShutterOpen();
	NPCwalk();
	NPCShop();
	if (camera.getCameraState() == 0)
	{
	CharacterCrouch();
	UIupdates(dt);
	BoundsCheck();
	SetPrevPos();
	UnpaidItems();
	camera.Update(dt);
	Stealing();
	}
	else if ( camera.getCameraState() == 1)
	{
	Security1.Update(dt);
	}
	else if ( camera.getCameraState() == 2)
	{
	Security2.Update(dt);
	}

	if ( EscapeEnd == true)
	{
		ExplosionScale++;
	}

	if ( ShopStaff == true)
	{
		SInstruct = true;
		StaffInstructTimer += (1*dt);
	}
	
	//Police spawned alarm rung
	if ( police == true)
	{
		PInstruct = true;
		PoliceInstructTimer += (1*dt);
			
		ROLE = "Thief";
		ShopStaff = false;

		/*if ( Application::IsKeyPressed(VK_NUMPAD8))
		{
			player.setHealth(1000);
		}*/

		if (player.getHealth() <= 0 )
		{
			GameEndStealLose = true;
		}

		if ( EscapeEnd == true)
		{
			EscapeCarMove++;
		}
		if ( EscapeCarMove >=  70 )
		{
			GameEndSteal = true;
			keybd_event( VK_NUMPAD9,
				0X69,
				KEYEVENTF_EXTENDEDKEY | 0,
				0 );
		}

		if ( GunOffset < 0.06)
		{
			GunOffset += 0.01;
		}

		if ( GunSwing < 0)
		{
			GunSwing +=3;
		}

		//Zoom In and out
		if(Application::IsKeyPressed(VK_RBUTTON))
		{
			Mtx44 projection;
			projection.SetToPerspective(15.f, 4.f / 3.f, 0.1f, 2000.f);
			projectionStack.LoadMatrix(projection);
			camera.downSight = true;
		}
		else
		{
			Mtx44 projection;
			projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 2000.f);
			projectionStack.LoadMatrix(projection);
			camera.downSight = false;
		}

		//Damage animation reset
		for ( int i =0; i<PoliceMan.ReadTextFilePoliceSize(); ++i)
		{
			if (PoliceMan.GetRenderPosPolice(i)->DamageReset < 0.5 && PoliceMan.GetRenderPosPolice(i)->Damaged == true)
			{
				PoliceMan.GetRenderPosPolice(i)->DamageReset += (2*dt);
			}
			else if (PoliceMan.GetRenderPosPolice(i)->DamageReset >=0.5 )
			{
				PoliceMan.GetRenderPosPolice(i)->Damaged = false;
				PoliceMan.GetRenderPosPolice(i)->DamageReset = 0;
			}
		}

		//Police respawn
		for ( int i =0; i<PoliceMan.ReadTextFilePoliceSize(); ++i)
		{
			if ( PoliceMan.GetRenderPosPolice(i)->getItemPrice() <= 0 )
			{
				PoliceMan.GetRenderPosPolice(i)->setItemAvailable(false);
			}
			if ( PoliceMan.GetRenderPosPolice(i)->getItemAvailability() == false)
			{
				if ( PoliceMan.GetRenderPosPolice(i)->RespawnCD < 20)
				{
					PoliceMan.GetRenderPosPolice(i)->RespawnCD += (1*dt);
				}
				else if ( PoliceMan.GetRenderPosPolice(i)->RespawnCD > 20)
				{
					PoliceMan.GetRenderPosPolice(i)->resethealth();
					PoliceMan.GetRenderPosPolice(i)->RespawnCD = 0;
					PoliceMan.GetRenderPosPolice(i)->setItemAvailable(true);
				}
			}
		}

		PoliceShoot(dt);

		//Police van roll in animation
		for (int i = 0; i < Police.ReturnListSize(); ++i)
		{
			if (Police.GetRenderPosItem(i)->getItemName() == "PoliceVan")
			{
				police1.PoliceDrive(Police.GetRenderPosItem(i)->getItemTranslationX(),
					Police.GetRenderPosItem(i)->getItemRotation(),
					Police.GetRenderPosItem(i)->getItemTranslationZ(),
					80,80,270);
			}
			if (Police.GetRenderPosItem(i)->getItemName() == "PoliceVan2")
			{
				police2.PoliceDrive(Police.GetRenderPosItem(i)->getItemTranslationX(),
					Police.GetRenderPosItem(i)->getItemRotation(),
					Police.GetRenderPosItem(i)->getItemTranslationZ(),
					-140,80,270);
			}
			if (Police.GetRenderPosItem(i)->getItemName() == "PoliceVan3")
			{
				police3.PoliceDriveL(Police.GetRenderPosItem(i)->getItemTranslationX(),
					Police.GetRenderPosItem(i)->getItemRotation(),
					Police.GetRenderPosItem(i)->getItemTranslationZ(),
					80,-90,90);
			}
			if (Police.GetRenderPosItem(i)->getItemName() == "PoliceVan4")
			{
				police4.PoliceDriveL(Police.GetRenderPosItem(i)->getItemTranslationX(),
					Police.GetRenderPosItem(i)->getItemRotation(),
					Police.GetRenderPosItem(i)->getItemTranslationZ(),
					-140,-90,90);
			}
		}
	}

	//Trolley mode activated
	if (player.trolley == true)
	{
		Trolleyz.Position = Vector3(camera.position.x,camera.position.y,camera.position.z);
		Trolleyz.Direction = Vector3(camera.target.x-camera.position.x,camera.target.y-camera.position.y,camera.target.z-camera.position.z);
		Trolleyz.TrolleyLimiter(camera.position.x,camera.position.y,camera.position.z,dt);
		BoundsCheckTrolley(Trolleyz.Position.x,Trolleyz.Position.y,Trolleyz.Position.z);
	}

	//Shooting
	if (shoot.bullet > 0  && police == true)
	{
		if(Application::IsKeyPressed(VK_LBUTTON))
		{
			if (shoot.ShootInterval < 0.3)
			{
				shoot.ShootInterval += (2*dt);
			}

			if (shoot.ShootInterval >= 0.3)
			{
			CBullet* temp =  new CBullet();
			temp->Position = Vector3(camera.position.x,camera.position.y,camera.position.z);
			temp->Direction = Vector3(camera.target.x-camera.position.x,camera.target.y-camera.position.y,camera.target.z-camera.position.z);
			bullet.push_back( temp );
			shoot.bullet--;
			shoot.ShootInterval = 0;
			MusicPlayer.Bullet();
			}
		}
	}
	//Bullet check
	else if  (shoot.bullet <= 0  && police == true)
	{
		if ( shoot.clip >0)
		{
			if (shoot.RTime < 2)
				shoot.RTime += (1*dt);
			if ( shoot.RTime >= 2)
			{
				//play reloading music
				MusicPlayer.Reload();
				shoot.clip--;
				shoot.bullet=30;
				shoot.RTime = 0;
			}
		}
	}
	//Reload
	else if  (shoot.bullet <= 0  && police == true)
	{
		if ( shoot.clip >0)
			shoot.clip--;
		shoot.bullet=30;
	}

	//Bullet check for delete
	for( int i=0; i < bullet.size(); i ++ )
	{
		BulletCollision(bullet[i]->Position.x,bullet[i]->Position.y,bullet[i]->Position.z,dt);
		if ( !bullet[i]->BulletUpdate(dt) || BulletCollision(bullet[i]->Position.x,bullet[i]->Position.y,bullet[i]->Position.z,dt) == true )
		{
			bullet.erase( bullet.begin() + i );
		}
	}

	//Enemy bullet check for delete
	for( int i=0; i < Policebullet.size(); i ++ )
	{
		BulletCollisionEnemy(Policebullet[i]->Position.x,Policebullet[i]->Position.y,Policebullet[i]->Position.z);
		if ( !Policebullet[i]->BulletUpdatePolice(dt) || BulletCollisionEnemy(Policebullet[i]->Position.x,Policebullet[i]->Position.y,Policebullet[i]->Position.z) == true )
		{
			Policebullet.erase( Policebullet.begin() + i );
		}
	}

	//Local calculation for rotation
	if ( camera.isCollide == false && player.trolley == true && player.trolleyDrop == false)
	{
			if(camera.MouseX < 960)
			{
				DupeRot += (float)(camera.CAMERA_SPEED * dt);
			}

			if(camera.MouseX < 958  && camera.downSight == false)
			{
				DupeRot += (float)(18*camera.CAMERA_SPEED * dt);
			}

			if(camera.MouseX > 960)
			{
				DupeRot -= (float)(camera.CAMERA_SPEED * dt);
			}

			if(camera.MouseX > 962  && camera.downSight == false)
			{
				DupeRot -= (float)(18*camera.CAMERA_SPEED * dt);
			}
	}

	if ( camera.isCollide == false && MenuKey != true )
	{
		if(camera.MouseX < 960)
		{
			DupeRotArms += (float)(camera.CAMERA_SPEED * dt);
		}

		if(camera.MouseX < 958  && camera.downSight == false)
		{
			DupeRotArms += (float)(18*camera.CAMERA_SPEED * dt);
		}

		if(camera.MouseX > 960)
		{
			DupeRotArms -= (float)(camera.CAMERA_SPEED * dt);
		}

		if(camera.MouseX > 962  && camera.downSight == false)
		{
			DupeRotArms -= (float)(18*camera.CAMERA_SPEED * dt);
		}

		if (camera.CameraLock < 40)
		{
			if (camera.getCameraState() == 0 )
			{
				if(camera.MouseY < 540 && camera.MouseY >=538)
				{
					DupeRotArmsY += (float)(camera.CAMERA_SPEED * dt);
				}
				if(camera.MouseY < 538 && camera.downSight == false)
				{
					DupeRotArmsY += (float)(12*camera.CAMERA_SPEED * dt);
				}
			}
		}
		if (camera.CameraLock > -40)
		{
			if (camera.getCameraState() == 0 )
			{
				if(camera.MouseY > 540 && camera.MouseY <= 542 )
				{
					DupeRotArmsY -= (float)(camera.CAMERA_SPEED * dt);
				}
				if(camera.MouseY > 542  && camera.downSight == false)
				{
					DupeRotArmsY -= (float)(12*camera.CAMERA_SPEED * dt);
				}
			}
		}
	}
	Animate(dt);
}
/***************************************************************/
/*!
/brief
A function for NPCs to walk a assigned path
*/
/***************************************************************/
void SP2::NPCwalk()
{
	npc.TestAIPath();
	guard.ShopPathing(0,playeridle);
	shopper.ShopPathing(1,playeridle);
	shopperTwo.ShopPathing(2,playeridle);
}
/***************************************************************/
/*!
/brief
Code to check if items were unpaid or not.
*/
/***************************************************************/
void SP2::UnpaidItems()
{
	if (player.returnInvenSize() == 0)
	{
		player.unpaiditems = false;
	}
	else
	{
		player.unpaiditems = true;
	}
	if (player.unpaiditems == true)
	{
		if ( camera.position.z <= -65 || camera.position.z >= 40)
		{
			police = true;
		}
	}
}
/***************************************************************/
/*!
/brief
Code to set the mart to an 'alert' state.
*/
/***************************************************************/
void SP2::Stealing()
{
	if (police == true)
	{
		Lightswitch = 0;
		lights[1].color.Set(1,Lightswitch,Lightswitch);
		lights[2].color.Set(1,Lightswitch,Lightswitch);
		lights[3].color.Set(1,Lightswitch,Lightswitch);
		lights[4].color.Set(1,Lightswitch,Lightswitch);
		lights[0].power = 0.1f;
		lights[1].power = 1.0f;
		lights[2].power = 1.0f;
		lights[3].power = 1.0f;
		lights[4].power = 1.0f;


		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
		glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
		glUniform1f(m_parameters[U_LIGHT2_POWER], lights[2].power);
		glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &lights[2].color.r);
		glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &lights[3].color.r);
		glUniform1f(m_parameters[U_LIGHT3_POWER], lights[3].power);
		glUniform3fv(m_parameters[U_LIGHT4_COLOR], 1, &lights[4].color.r);
		glUniform1f(m_parameters[U_LIGHT4_POWER], lights[4].power);
		glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	}
}
/***************************************************************/
/*!
/brief
Code to set back to the previous position for the collision codes.
*/
/***************************************************************/
void SP2::SetPrevPos()
{
	cameraDupe.position = camera.position;
	cameraDupe.target = camera.target;
	cameraDupe.up = camera.up;
}
/***************************************************************/
/*!
/brief
Code to update the User Interface.
*/
/***************************************************************/
void SP2::UIupdates(double dt)
{
	std::stringstream dd;
	dd << camera.position.x;
	XPos = dd.str();

	std::stringstream ff;
	ff << camera.position.z;
	ZPos = ff.str();

	//FPS = 1/dt;
	std::stringstream ss;
	ss << 1/dt;;
	FPS_count = ss.str();

	std::stringstream hh;
	hh << camera.target;
	Target = hh.str();

	std::stringstream gg;
	gg << player.getHealth();
	HP = gg.str();

	std::stringstream jj;
	jj << player.getStamina();
	STAM = jj.str();

	std::stringstream kk;
	kk << "$" << player.getMoney();
	MONEY = kk.str();

	if ( ShopStaff == true && GameEndStaff != true)
	{	
	DTimer += dt;
	std::stringstream DeltaTime;
	DeltaTime << setprecision(2) << fixed << DTimer;
	DTtime = DeltaTime.str();
	}

	if ( GameEndStaff == true)
	{
		std::stringstream Final;
		Final << "Your time :" << setprecision(2) << fixed << DTimer;
		EndGame = Final.str();
	}
	

	std::stringstream Final2;
	Final2 << "You stole: $" << player.getMoney() ;
	EndGameSteal = Final2.str();

	std::stringstream Final3;
	Final3  <<  "Vehicle Destroyed";
	EndGameStealL = Final3.str();
	
	//Crosshair updates
	if ( police == true)
	{
		std::stringstream ll;
		if ( shoot.bullet != 0 )
			ll << shoot.bullet;
		else if ( shoot.clip > 0 && shoot.RTime == 0)
			ll << "Reload";
		else if (shoot.RTime > 0)
			ll << "Reloading";
		else 
			ll << "Empty";
		BulletCount = ll.str();
	}

	//Scan for item 
	for ( int i = 0; i < FNB.ReturnListSize();  ++i)
	{
		if ( camera.target.x >= FNB.GetRenderPosItem(i)->getItemTranslationX()-0.2 && camera.target.x <= FNB.GetRenderPosItem(i)->getItemTranslationX()+0.2 
			&& camera.target.y >= FNB.GetRenderPosItem(i)->getItemTranslationY()-1 && camera.target.y <= FNB.GetRenderPosItem(i)->getItemTranslationY()+1 
			&& camera.target.z >= FNB.GetRenderPosItem(i)->getItemTranslationZ()-1 && camera.target.z <= FNB.GetRenderPosItem(i)->getItemTranslationZ()+1)
		{
			ItemName = FNB.GetRenderPosItem(i)->getItemName();
			std::stringstream pp;
			if ( FNB.GetRenderPosItem(i)->getItemName() != "Rifle" && FNB.GetRenderPosItem(i)->getItemName() != "Pistol")
			{
			pp << "$" << FNB.GetRenderPosItem(i)->getItemPrice();
			}
			else
			{
			pp << "Pick Ammo";
			}
			ItemPrice = pp.str();
			break;
		}
		else
		{
			ItemName = "";
			ItemPrice = "";
		}
	}

	//scan for cashier
	for ( int i = 0; i < 4;  ++i)
	{
		if ( camera.target.x >= cashier.GetRenderPos(i)->getTranslationX()-4 && camera.target.x <= cashier.GetRenderPos(i)->getTranslationX()+4 
			&& camera.target.y >= cashier.GetRenderPos(i)->getTranslationY()-10 && camera.target.y <= cashier.GetRenderPos(i)->getTranslationY()+10 
			&& camera.target.z >= cashier.GetRenderPos(i)->getTranslationZ()-3 && camera.target.z <= cashier.GetRenderPos(i)->getTranslationZ()+3)
		{
			ItemName = "Checkout";
		}
	}

	//scan for security camera panel
	if ( camera.target.x >= data.GetRenderPos(20)->getTranslationX()-2 && camera.target.x <= data.GetRenderPos(20)->getTranslationX()+13.5 
			&& camera.target.y >= data.GetRenderPos(20)->getTranslationY()-10 && camera.target.y <= data.GetRenderPos(20)->getTranslationY()+10
			&& camera.target.z >= data.GetRenderPos(20)->getTranslationZ()-10 && camera.target.z <= data.GetRenderPos(20)->getTranslationZ())
		{
			ItemName = "Camera";
		}

	//scan for trolley
	if ( camera.target.x >= trolley.LastX-5 && camera.target.x <= trolley.LastX+5
		&& camera.target.y >= data.GetRenderPos(21)->getTranslationY()-2 && camera.target.y <= data.GetRenderPos(21)->getTranslationY()+5
		&& camera.target.z >= trolley.LastZ-5 && camera.target.z <= trolley.LastZ+5 && player.trolley == false)
		{
			ItemName = "Trolley";
		}

	//scan for fill truck
	if ( camera.target.x >= VanBounds[4]->TTL.x && camera.target.x <= VanBounds[4]->TTR.x
		&& camera.target.y >= VanBounds[4]->BTL.y && camera.target.y <= VanBounds[4]->TTL.y
		&& camera.target.z >= VanBounds[4]->TBL.z-2 && camera.target.z <= VanBounds[4]->TBL.z+2 && police == true)
		{
			ItemName = "$$$";
		}

	//scan for escape 
		if ( camera.target.x >= VanBounds[4]->TTL.x && camera.target.x <= VanBounds[4]->TTR.x
		&& camera.target.y >= VanBounds[4]->BTL.y && camera.target.y <= VanBounds[4]->TTL.y
		&& camera.target.z >= VanBounds[4]->TTL.z && camera.target.z <= VanBounds[4]->TBL.z-15 && police == true)
		{
			ItemName = "Escape";
		}

		if ( camera.target.x >= LockerBounds[0]->TTL.x && camera.target.x <= LockerBounds[0]->TTR.x
		&& camera.target.y >= LockerBounds[0]->BTL.y && camera.target.y <= LockerBounds[0]->TTL.y
		&& camera.target.z >= LockerBounds[0]->TTL.z && camera.target.z <= LockerBounds[0]->TBL.z)
		{
			ItemName = "Shop Staff";
		}
}
/***************************************************************/
/*!
/brief
Codes to animate the NPCs.
*/
/***************************************************************/
void SP2::Animate(double dt)
{
	 if(walk == false)
	 {
		 angle += (float)(WSPEED * dt);
		 translate += (float)(dt);
		 if(translate < 0)
		 {
			 translateY = translate * -1;
		 }
		 else
		 {
			 translateY = translate;
		 }
		 if(angle >= 30)
		 {
			 walk = true;
		 }
	 }
	 else if(walk == true)
	 {
		 angle -= (float)(WSPEED * dt);
		 translate -= (float)(dt);
		 if(translate < 0)
		 {
			 translateY = translate * - 1;
		 }
		 else
		 {
			 translateY = translate;
		 }
		 if(angle <= -30)
		 {
			 walk = false;
		 }
	 }
}
/***************************************************************/
/*!
/brief
Codes for crouching controls and definitions.
*/
/***************************************************************/
void SP2::CharacterCrouch()
{
		if(Application::IsKeyPressed(VK_CONTROL) && camera.position.y != 4)
		{
			camera.position.y -= 0.5;
			camera.target.y -= 0.5;
		}
		else if (Application::IsKeyReleased(VK_CONTROL) && camera.position.y != 6 )
		{
			camera.position.y = 6;
			camera.target.y = 6;
			camera.CameraLock = 0;
			camera.CamRotationY = 0;
		}
}
/***************************************************************/
/*!
/brief
Codes to check from a text file where the bounds are for collision checking.
*/
/***************************************************************/
void SP2::BoundsCheck()
{
	Vector3 tempMart, tempShelves, tempCashier, tempFridge, tempshopper;
	tempMart.x = data.GetRenderPos(0)->getTranslationX();
	tempMart.z = data.GetRenderPos(0)->getTranslationZ();
	tempShelves.x = shelve.GetRenderPos(0)->getTranslationX();
	tempShelves.z = shelve.GetRenderPos(0)->getTranslationZ();
	tempCashier.x = cashier.GetRenderPos(0)->getTranslationX();
	tempCashier.z = cashier.GetRenderPos(0)->getTranslationZ();
	tempFridge.x = fridge.GetRenderPos(0)->getTranslationX();
	tempFridge.z = fridge.GetRenderPos(0)->getTranslationZ();
	tempshopper.x = shopper.getPosX();
	tempshopper.z = shopper.getPosZ();

	//PoliceCar
	for ( int i = 0; i< VanBounds.size(); ++i)
	{
		if ( camera.position.x  >= VanBounds[i]->TTL.x && camera.position.x  <= VanBounds[i]->TTR.x 
			&& camera.position.y >= VanBounds[i]->BTL.y && camera.position.y <= VanBounds[i]->TTL.y
			&& camera.position.z >= VanBounds[i]->TTL.z && camera.position.z <= VanBounds[i]->TBL.z && police == true)
		{
			camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
		}
	}

	
	//Locker
	for ( int i = 0; i< LockerBounds.size(); ++i)
	{
		if ( camera.position.x  >= LockerBounds[i]->TTL.x && camera.position.x  <= LockerBounds[i]->TTR.x 
			&& camera.position.y >= LockerBounds[i]->BTL.y && camera.position.y <= LockerBounds[i]->TTL.y
			&& camera.position.z >= LockerBounds[i]->TTL.z && camera.position.z <= LockerBounds[i]->TBL.z)
		{
			camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
		}
	}

	//fence check
	//leftfence
	if( camera.position.x <= data.GetRenderPos(7)->getTranslationX()-100)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.isCollide = true;
	}
	//right fence
	else if( camera.position.x >= data.GetRenderPos(7)->getTranslationX()+90)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.isCollide = true;
	}
	else if( camera.position.z <= data.GetRenderPos(7)->getTranslationZ()-205)
	{
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
		camera.isCollide = true;
	}
	else if( camera.position.z >= data.GetRenderPos(7)->getTranslationZ()+180)
	{
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
		camera.isCollide = true;
	}

	////mart outer bound check
	////left wall
	else if( camera.position.x >= tempMart.x - 78 && camera.position.x <= tempMart.x - 71 && camera.position.z <= tempMart.z + 54 && camera.position.z >= tempMart.z - 54)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
		camera.isCollide = true;
	}
	////Back Wall Left
	else if( camera.position.x >= tempMart.x -78 && camera.position.x <= tempMart.x - 14 && camera.position.z <= tempMart.z + -45  && camera.position.z >= tempMart.z - 54)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
		camera.isCollide = true;
	}

	////Back Wall Right
	else if( camera.position.x >= tempMart.x + 14 && camera.position.x <= tempMart.x + 78 && camera.position.z <= tempMart.z + -45  && camera.position.z >= tempMart.z - 54)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
		camera.isCollide = true;
	}

	////Right Wall
	else if( camera.position.x >= tempMart.x + 71 && camera.position.x <= tempMart.x + 78 && camera.position.z <= tempMart.z + 54  && camera.position.z >= tempMart.z - 54)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
		camera.isCollide = true;
	}

	////Right Wall
	else if( camera.position.x >= tempMart.x - 62 && camera.position.x <= tempMart.x + 62 && camera.position.z <= tempMart.z + 54  && camera.position.z >= tempMart.z + 46)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
		camera.isCollide = true;
	}

	////Middle Wall
	else if( camera.position.x >= tempMart.x - 62 && camera.position.x <= tempMart.x + 62 && camera.position.z <= tempMart.z - 22  && camera.position.z >= tempMart.z  - 30)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
		camera.isCollide = true;
	}

	//Fridge Bounds Check
	else if(camera.position.x >= tempFridge.x -92 && camera.position.x <= tempFridge.x +8 && camera.position.z <= tempFridge.z + 2 && camera.position.z >= tempFridge.z)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
		camera.isCollide = true;
	}

	//patroler collision
	else if(camera.position.x >= patroler.getPosX() - 2 && camera.position.x <= patroler.getPosX() +2 && camera.position.z >= patroler.getPosZ() - 2 && camera.position.z <= patroler.getPosZ() +2)
	{
		if(camera.position.x != cameraDupe.position.x && camera.position.z != cameraDupe.position.z)//player not idle
		{
			camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
		}
	}
	else
	{
		camera.isCollide = false;
	}

	//Shelves Bounds Check
	for(int i = 0; i<3; ++i)
	{
		for(int j = 0; j<3; ++j)
		{
			if(camera.position.x >= ((tempShelves.x - 4) + (j*34)) && camera.position.x <= ((tempShelves.x + 22) + (j*34))  && camera.position.z <= (tempShelves.z +48 -(i*15)) && camera.position.z >= (tempShelves.z +43-(i*15)))
			{
				camera.position.x = cameraDupe.position.x;
				camera.target.x = cameraDupe.target.x;
				camera.position.z = cameraDupe.position.z;
				camera.target.z = cameraDupe.target.z;
				camera.isCollide = true;
			}
		}
	}
	//Cashier Bounds Check
	for(int i = 0; i < 4; ++i)
	{
		if(camera.position.x >= tempCashier.x -1 && camera.position.x <= tempCashier.x +2 && camera.position.z <= tempCashier.z + 53 -(i*16) && camera.position.z >= tempCashier.z + 44-(i*16))
		{
			camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
		}
	}

	//Fridge Bounds Check
	if(camera.position.x >= tempFridge.x -92 && camera.position.x <= tempFridge.x +8 && camera.position.z <= tempFridge.z + 2 && camera.position.z >= tempFridge.z)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
		camera.isCollide = true;
	}

	//GuardHouse Collison
	//left wall
	if(camera.position.x >= data.GetRenderPos(19)->getTranslationX() + 141 && camera.position.x <= data.GetRenderPos(19)->getTranslationX() + 145 && camera.position.z >= 87 && camera.position.z <= 118)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
		camera.isCollide = true;
	}
	//right wall
	if(camera.position.x <= data.GetRenderPos(19)->getTranslationX() + 169 && camera.position.x >= data.GetRenderPos(19)->getTranslationX() + 168 && camera.position.z >= 87 && camera.position.z <= 118)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
		camera.isCollide = true;
	}
	//back wall
	if(camera.position.z >= data.GetRenderPos(19)->getTranslationZ() + 230 && camera.position.z <= data.GetRenderPos(19)->getTranslationZ() + 235 && camera.position.x >= 60 && camera.position.x <= 90)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
		camera.isCollide = true;
	}
	//Front wall
	if(camera.position.z >= data.GetRenderPos(19)->getTranslationZ() + 203 && camera.position.z <= data.GetRenderPos(19)->getTranslationZ() + 206 && camera.position.x >= 71 && camera.position.x <= 90)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
		camera.isCollide = true;
	}
	//Guard Control
	if(camera.position.x >= data.GetRenderPos(20)->getTranslationX() - 2.8 && camera.position.x <= data.GetRenderPos(20)->getTranslationX() + 14.2 && camera.position.z >= data.GetRenderPos(20)->getTranslationZ() - 12.5 && camera.position.z <= data.GetRenderPos(20)->getTranslationZ() - 10.5)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
		camera.isCollide = true;

	}
	//Trolley Collision(player only)
	if(camera.position.x >= trolley.LastXMove-3 && camera.position.x <= trolley.LastXMove+3 && camera.position.z >= trolley.LastZMove - 3 && camera.position.z <= trolley.LastZMove + 3 && player.trolley == false)// need fix
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
	}
	//Cashier NPC Collision
	for(int i = 0; i < 4; ++i)
	{
		if(camera.position.x >= cashiermodel.GetRenderPos(i)->getTranslationX() - 2 && camera.position.x <= cashiermodel.GetRenderPos(i)->getTranslationX() + 2 && camera.position.z >= cashiermodel.GetRenderPos(i)->getTranslationZ() - 2 && camera.position.z <= cashiermodel.GetRenderPos(i)->getTranslationZ() + 2)
		{
			camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
		}

	}
	//shopper AI collision
	if(playeridle == false)
	{
		if(camera.position.x >= shopper.getPosX() - 4 && camera.position.x <= shopper.getPosX() + 4 && camera.position.z >= shopper.getPosZ() - 4 && camera.position.z <= shopper.getPosZ() + 4)
		{
			if(camera.position.x != cameraDupe.position.x && camera.position.z != cameraDupe.position.z)//player not idle
			{
				camera.position.x = cameraDupe.position.x;
				camera.target.x = cameraDupe.target.x;
				camera.position.z = cameraDupe.position.z;
				camera.target.z = cameraDupe.target.z;
				playeridle = false;

			}
			else if(camera.position.x == cameraDupe.position.x && camera.position.z == cameraDupe.position.z)//idle player/camera
			{
				//prevent AI movement
				playeridle = true;
				NPC1idle = true;
			}
		}
	}
	//player break idle for AI shopper
	if(playeridle == true && NPC1idle == true)
	{
		//player X Coord > AI X Coord
		if(camera.position.x > shopper.getPosX() && camera.position.x < shopper.getPosX() + 2)
		{
			if(camera.position.x < cameraDupe.position.x && camera.position.z >= shopper.getPosZ() - 3 && camera.position.z <= shopper.getPosZ() + 3)
			{
				camera.position.x = cameraDupe.position.x;
				camera.target.x = cameraDupe.target.x;

			}

		}
		//player X Coord < AI X Coord
		if(camera.position.x < shopper.getPosX() && camera.position.x > shopper.getPosX() - 2)
		{
			if(camera.position.x > cameraDupe.position.x && camera.position.z >= shopper.getPosZ() - 2 && camera.position.z <= shopper.getPosZ() + 2)
			{
				camera.position.x = cameraDupe.position.x;
				camera.target.x = cameraDupe.target.x;
			}
		}
		if(((camera.position.z > shopper.getPosZ() + 3 || camera.position.z < shopper.getPosZ() - 3) && (camera.position.x > shopper.getPosX() + 3 || camera.position.x < shopper.getPosX() - 3)) || (camera.position.x > shopper.getPosX() + 5) || (camera.position.x < shopper.getPosX() - 5))
		{
			playeridle = false;
			NPC1idle = false;
		}
	}
	//shopper 2  AI collision
	if(playeridle == false)
	{
		if(camera.position.x >= shopperTwo.getPosX() - 4 && camera.position.x <= shopperTwo.getPosX() + 4 && camera.position.z >= shopperTwo.getPosZ() - 4 && camera.position.z <= shopperTwo.getPosZ() + 4)
		{
			if(camera.position.x != cameraDupe.position.x && camera.position.z != cameraDupe.position.z)//player not idle
			{
				camera.position.x = cameraDupe.position.x;
				camera.target.x = cameraDupe.target.x;
				camera.position.z = cameraDupe.position.z;
				camera.target.z = cameraDupe.target.z;
				playeridle = false;

			}
			else if(camera.position.x == cameraDupe.position.x && camera.position.z == cameraDupe.position.z)//idle player/camera
			{
				//prevent AI movement
				playeridle = true;
				NPC2idle = true;
			}
		}
	}
	//player break idle for AI shopper 2
	if(playeridle == true && NPC2idle == true)
	{
		//player X Coord > AI X Coord
		if(camera.position.x > shopperTwo.getPosX() && camera.position.x < shopperTwo.getPosX() + 2)
		{
			if(camera.position.x < cameraDupe.position.x && camera.position.z >= shopperTwo.getPosZ() - 3 && camera.position.z <= shopperTwo.getPosZ() + 3)
			{
				camera.position.x = cameraDupe.position.x;
				camera.target.x = cameraDupe.target.x;

			}

		}
		//player X Coord < AI X Coord
		if(camera.position.x < shopperTwo.getPosX() && camera.position.x > shopperTwo.getPosX() - 2)
		{
			if(camera.position.x > cameraDupe.position.x && camera.position.z >= shopperTwo.getPosZ() - 2 && camera.position.z <= shopperTwo.getPosZ() + 2)
			{
				camera.position.x = cameraDupe.position.x;
				camera.target.x = cameraDupe.target.x;
			}
		}
		if(((camera.position.z > shopperTwo.getPosZ() + 3 || camera.position.z < shopperTwo.getPosZ() - 3) && (camera.position.x > shopperTwo.getPosX() + 3 || camera.position.x < shopperTwo.getPosX() - 3)) || (camera.position.x > shopperTwo.getPosX() + 5) || (camera.position.x < shopperTwo.getPosX() - 5))
		{
			playeridle = false;
			NPC2idle = false;
		}
	}

	//guard/patroler AI collision
	if(playeridle == false)
	{
		if(camera.position.x >= guard.getPosX() - 4 && camera.position.x <= guard.getPosX() + 4 && camera.position.z >= guard.getPosZ() - 4 && camera.position.z <= guard.getPosZ() + 4)
		{
			if(camera.position.x != cameraDupe.position.x && camera.position.z != cameraDupe.position.z)//player not idle
			{
				camera.position.x = cameraDupe.position.x;
				camera.target.x = cameraDupe.target.x;
				camera.position.z = cameraDupe.position.z;
				camera.target.z = cameraDupe.target.z;
				playeridle = false;

			}
			else if(camera.position.x == cameraDupe.position.x && camera.position.z == cameraDupe.position.z)//idle player/camera
			{
				//prevent AI movement
				playeridle = true;
				NPC3idle = true;
			}
		}
	}
	//player break idle for AI guard/patroler
	if(playeridle == true && NPC3idle == true)
	{
		//player X Coord > AI X Coord
		if(camera.position.x > guard.getPosX() && camera.position.x < guard.getPosX() + 2)
		{
			if(camera.position.x < cameraDupe.position.x && camera.position.z >= guard.getPosZ() - 3 && camera.position.z <= guard.getPosZ() + 3)
			{
				camera.position.x = cameraDupe.position.x;
				camera.target.x = cameraDupe.target.x;

			}

		}
		//player X Coord < AI X Coord
		if(camera.position.x < guard.getPosX() && camera.position.x > guard.getPosX() - 2)
		{
			if(camera.position.x > cameraDupe.position.x && camera.position.z >= guard.getPosZ() - 2 && camera.position.z <= guard.getPosZ() + 2)
			{
				camera.position.x = cameraDupe.position.x;
				camera.target.x = cameraDupe.target.x;
			}
		}
		if(((camera.position.z > guard.getPosZ() + 3 || camera.position.z < guard.getPosZ() - 3) && (camera.position.x > guard.getPosX() + 3 || camera.position.x < guard.getPosX() - 3)) || (camera.position.x > guard.getPosX() + 5) || (camera.position.x < guard.getPosX() - 5))
		{
			playeridle = false;
			NPC3idle = false;
		}
	}
	
}

void SP2::BoundsCheckTrolley(float x,float y, float z)
{
	Vector3 tempMart, tempShelves, tempCashier, tempFridge, tempshopper;
	tempMart.x = data.GetRenderPos(0)->getTranslationX();
	tempMart.z = data.GetRenderPos(0)->getTranslationZ();
	tempShelves.x = shelve.GetRenderPos(0)->getTranslationX();
	tempShelves.z = shelve.GetRenderPos(0)->getTranslationZ();
	tempCashier.x = cashier.GetRenderPos(0)->getTranslationX();
	tempCashier.z = cashier.GetRenderPos(0)->getTranslationZ();
	tempFridge.x = fridge.GetRenderPos(0)->getTranslationX();
	tempFridge.z = fridge.GetRenderPos(0)->getTranslationZ();
	tempshopper.x = shopper.getPosX();
	tempshopper.z = shopper.getPosZ();

	//PoliceCar
	for ( int i = 0; i< VanBounds.size(); ++i)
	{
		if ( x  >= VanBounds[i]->TTL.x && x  <= VanBounds[i]->TTR.x 
			&& y >= VanBounds[i]->BTL.y && y <= VanBounds[i]->TTL.y
			&& z >= VanBounds[i]->TTL.z && z <= VanBounds[i]->TBL.z && police == true)
		{
			camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
		}
	}

	//fence check
	//leftfence
	if( x <= data.GetRenderPos(7)->getTranslationX()-100)
	{
		camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
		
			camera.isCollide = true;
	}
	//right fence
	else if( x >= data.GetRenderPos(7)->getTranslationX()+90)
	{
		camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			
			camera.isCollide = true;
	}
	else if( z <= data.GetRenderPos(7)->getTranslationZ()-205)
	{
		
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
	}
	else if( z >= data.GetRenderPos(7)->getTranslationZ()+180)
	{
	
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
	}

	////mart outer bound check
	////left wall
	else if( x >= tempMart.x - 78 && x <= tempMart.x - 71 && z <= tempMart.z + 54 && z >= tempMart.z - 54)
	{
		camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
	}
	////Back Wall Left
	else if( x >= tempMart.x -78 && x <= tempMart.x - 14 && z <= tempMart.z + -45  && z >= tempMart.z - 54)
	{
		camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
	}

	////Back Wall Right
	else if( x >= tempMart.x + 14 && x <= tempMart.x + 78 && z <= tempMart.z + -45  && z >= tempMart.z - 54)
	{
		camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
	}

	////Right Wall
	else if( x >= tempMart.x + 71 && x <= tempMart.x + 78 && z <= tempMart.z + 54  && z >= tempMart.z - 54)
	{
		camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
	}

	////Right Wall
	else if( x >= tempMart.x - 62 && x <= tempMart.x + 62 && z <= tempMart.z + 54  && z >= tempMart.z + 46)
	{
		camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
	}

	////Middle Wall
	else if( x >= tempMart.x - 62 && x <= tempMart.x + 62 && z <= tempMart.z - 22  && z >= tempMart.z  - 30)
	{
		camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
	}

	//Fridge Bounds Check
	else if(x >= tempFridge.x -92 && x <= tempFridge.x +8 && z <= tempFridge.z + 2 && z >= tempFridge.z)
	{
		camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
	}

	//patroler collision
	else if(x >= patroler.getPosX() - 2 && x <= patroler.getPosX() +2 && z >= patroler.getPosZ() - 2 && z <= patroler.getPosZ() +2)
	{
		if(x != cameraDupe.position.x && z != cameraDupe.position.z)//player not idle
		{
			camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
		}
	}
	else
	{
		camera.isCollide = false;
	}

	//Shelves Bounds Check
	for(int i = 0; i<3; ++i)
	{
		for(int j = 0; j<3; ++j)
		{
			if(x >= ((tempShelves.x - 4) + (j*34)) && x <= ((tempShelves.x + 22) + (j*34))  && z <= (tempShelves.z +48 -(i*15)) && z >= (tempShelves.z +43-(i*15)))
			{
				camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;

			}
		}
	}
	//Cashier Bounds Check
	for(int i = 0; i < 4; ++i)
	{
		if(x >= tempCashier.x -1 && x <= tempCashier.x +2 && z <= tempCashier.z + 53 -(i*16) && z >= tempCashier.z + 44-(i*16))
		{
			camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
		}
	}

	//Fridge Bounds Check
	if(x >= tempFridge.x -92 && x <= tempFridge.x +8 && z <= tempFridge.z + 2 && z >= tempFridge.z)
	{
		camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
	}

	//GuardHouse Collison
	//left wall
	if(x >= data.GetRenderPos(19)->getTranslationX() + 141 && x <= data.GetRenderPos(19)->getTranslationX() + 145 && z >= 87 && z <= 118)
	{
		camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
	}
	//right wall
	if(x <= data.GetRenderPos(19)->getTranslationX() + 169 && x >= data.GetRenderPos(19)->getTranslationX() + 168 && z >= 87 && z <= 118)
	{
		camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
	}
	//back wall
	if(z >= data.GetRenderPos(19)->getTranslationZ() + 230 && z <= data.GetRenderPos(19)->getTranslationZ() + 235 && x >= 60 && x <= 90)
	{
		camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
	}
	//Front wall
	if(z >= data.GetRenderPos(19)->getTranslationZ() + 203 && z <= data.GetRenderPos(19)->getTranslationZ() + 206 && x >= 71 && x <= 90)
	{
		camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
	}
	//Guard Control
	if(x >= data.GetRenderPos(20)->getTranslationX() - 2.8 && x <= data.GetRenderPos(20)->getTranslationX() + 14.2 && z >= data.GetRenderPos(20)->getTranslationZ() - 12.5 && z <= data.GetRenderPos(20)->getTranslationZ() - 10.5)
	{
		camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
	}
	//Cashier NPC Collision
	for(int i = 0; i < 4; ++i)
	{
		if(x >= cashiermodel.GetRenderPos(i)->getTranslationX() - 2 && x <= cashiermodel.GetRenderPos(i)->getTranslationX() + 2 && z >= cashiermodel.GetRenderPos(i)->getTranslationZ() - 2 && z <= cashiermodel.GetRenderPos(i)->getTranslationZ() + 2)
		{
			camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;
		}

	}
	//shopper AI collision
	if(playeridle == false)
	{
		if(x >= shopper.getPosX() - 4 && x <= shopper.getPosX() + 4 && z >= shopper.getPosZ() - 4 && z <= shopper.getPosZ() + 4)
		{
			if(x != cameraDupe.position.x && z != cameraDupe.position.z)//player not idle
			{
				camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			camera.position.z = cameraDupe.position.z;
			camera.target.z = cameraDupe.target.z;
			camera.isCollide = true;

			}
			else if(x == cameraDupe.position.x && z == cameraDupe.position.z)//idle player/camera
			{
				//prevent AI movement
				playeridle = true;
			}
		}
	}
	//player break idle for AI
	if(playeridle == true)
	{
		//player X Coord > AI X Coord
		if(x > shopper.getPosX() && x < shopper.getPosX() + 2)
		{
			if(x < cameraDupe.position.x && z >= shopper.getPosZ() - 3 && z <= shopper.getPosZ() + 3)
			{
				if(x < cameraDupe.position.x && z >= shopper.getPosZ() - 2 && z <= shopper.getPosZ() + 2)
				{
					camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			
			camera.isCollide = true;

				}

			}
			//player X Coord < AI X Coord
			if(x < shopper.getPosX())
			{
				if(x > cameraDupe.position.x && z >= shopper.getPosZ() - 2 && z <= shopper.getPosZ() + 2)
				{
					camera.position.x = cameraDupe.position.x;
			camera.target.x = cameraDupe.target.x;
			
			camera.isCollide = true;
				}
			}
			if(((z > shopper.getPosZ() + 3 || z < shopper.getPosZ() - 3) && (x > shopper.getPosX() + 3 || x < shopper.getPosX() - 3)) || (x > shopper.getPosX() + 5) || (x < shopper.getPosX() - 5))
			{
				playeridle = false;
			}
			/*if(x > shopper.getPosX() + 5 && z > shopper.getPosZ() + 5 )
			{
			playeridle = false;
			}*/
		}
	}
}
/***************************************************************/
/*!
/brief
Codes to open the shutter door.
*/
/***************************************************************/
void SP2::ShutterOpen()
{
	if (camera.ShutterDoor == true && ShutterDoorOpen < 22)
	{
		ShutterDoorOpen += 0.4;
	}
	else if (camera.ShutterDoor == false)
	{
		if(ShutterDoorOpen > 8)
		{
			ShutterDoorOpen -= 0.4;
		}
	}
}
/***************************************************************/
/*!
/brief
Codes to animate the door sliding.
*/
/***************************************************************/
void SP2::DoorSlide()
{
	//Main Mart Doors
		if ( camera.EntranceDoor == true || shopper.enterMart == true)
	{
		if ( EntranceDoorSlide >= 53)
		{
				EntranceDoorSlide-= 0.4;
		}
	}
	else if ( camera.EntranceDoor == false || shopper.enterMart == false)
	{
		if ( EntranceDoorSlide <= 66.4)
		{
				EntranceDoorSlide+= 0.4;
		}
	}
	

	if ( camera.ExitDoor == true || shopper.exitMart == true)
	{
		if ( ExitDoorSlide <= -53)
		{
				ExitDoorSlide+= 0.4;
		}
	}
	else if ( camera.ExitDoor == false|| shopper.exitMart == false)
	{
		if ( ExitDoorSlide >= -66.4)
		{
				ExitDoorSlide-= 0.4;
		}
	}

	//Staff Doors
	if ( camera.RightStaffDoor == true && RightStaffDoorSlide < -50)
	{
		RightStaffDoorSlide += 0.4;
	}
	else if ( camera.RightStaffDoor == false)
	{
		if ( RightStaffDoorSlide > -66.4)
		{
				RightStaffDoorSlide -= 0.4;
		}
	}
	if ( camera.LeftStaffDoor == true && LeftStaffDoorSlide >50)
	{
		LeftStaffDoorSlide -= 0.4;
	}
	else if ( camera.LeftStaffDoor == false)
	{
		if ( LeftStaffDoorSlide < 66.4)
		{
				LeftStaffDoorSlide += 0.4;
		}
	}
}
/***************************************************************/
/*!
/brief
Codes for the trolley controls and rendering.
*/
/***************************************************************/
void SP2::Trolley()
{
	if(Application::IsKeyPressed('E'))
	{
		if ( camera.target.x >= trolley.LastX-5 && camera.target.x <= trolley.LastX+5
			&& camera.target.y >= data.GetRenderPos(21)->getTranslationY()-2 && camera.target.y <= data.GetRenderPos(21)->getTranslationY()+5
			&& camera.target.z >= trolley.LastZ-5 && camera.target.z <= trolley.LastZ+5)
		{
			player.trolley = true;
			player.trolleyDrop = false;
			camera.TrolleyMode = true;
		}
	}

	if ( player.trolley == true && player.trolleyDrop == false)
		trolley.DetectBulletPos = Trolleyz.Position;

	if ( player.trolley == true)
	{
		player.setInventoryCap(trolley.ExpansionSize);
		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x,data.GetRenderPos(21)->getTranslationY(),camera.position.z);
		modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
		modelStack.PushMatrix();
		modelStack.Translate(3,0,0);
		modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
		RenderMesh(meshList[GEO_MODEL_TROLLEY], true);
		modelStack.PopMatrix();	
		modelStack.PopMatrix();	
		trolley.SetList(camera.position.x,camera.position.z,trolley.DetectBulletPos.x,trolley.DetectBulletPos.z,DupeRot,10);
	}

	else if ( player.trolley == false && player.trolleyDrop == false)
	{
		modelStack.PushMatrix();
		modelStack.Translate(data.GetRenderPos(21)->getTranslationX(),data.GetRenderPos(21)->getTranslationY(),data.GetRenderPos(21)->getTranslationZ());
		modelStack.Rotate(data.GetRenderPos(21)->getRotation(),data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
		modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
		modelStack.PushMatrix();
		RenderMesh(meshList[GEO_MODEL_TROLLEY], true);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		trolley.SetList(data.GetRenderPos(21)->getTranslationX(),data.GetRenderPos(21)->getTranslationZ(),data.GetRenderPos(21)->getTranslationX(),data.GetRenderPos(21)->getTranslationZ(),camera.CamRotation,10);
	}

	if(Application::IsKeyPressed('R') && player.trolley == true)
	{
		player.trolley = false;
		player.trolleyDrop = true;
	}

	if ( player.trolleyDrop == true)
	{
		modelStack.PushMatrix();
		modelStack.Translate(trolley.LastXMove,data.GetRenderPos(21)->getTranslationY(),trolley.LastZMove);
		modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
		modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
		modelStack.PushMatrix();
		modelStack.Translate(-3,0,0);
		RenderMesh(meshList[GEO_MODEL_TROLLEY], true);
		modelStack.PopMatrix();	
		modelStack.PopMatrix();	
	}
}
/***************************************************************/
/*!
/brief
Codes to show the items in the trolley, and for updating it in general.
*/
/***************************************************************/
void SP2::TrolleyUpdate()
{
	float tempOffsetY = 2;
	float offSetZ = 0;
	float offSetX = 0;
	float StartPosZ = -0.5;
	float StartPosX = 1.5;
	float PerX = 0.7;
	if ( player.trolley == true || player.trolleyDrop == true)
	{
			for ( int i = 0; i < player.returnInvenSize(); ++i)
			{
				if ( i >= 5 )
				{
					offSetZ = 1;
					offSetX = 3.5;
				}
				if ( player.getInventory(i)->getItemName() == "Coke")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,0,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_COKE], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
				else if ( player.getInventory(i)->getItemName() == "Pepsi")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,0,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_PEPSI], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
				else if ( player.getInventory(i)->getItemName() == "MtDew")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,0,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_MTDEW], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
				else if ( player.getInventory(i)->getItemName() == "Pizza")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,0.2,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_PIZZA], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
				else if ( player.getInventory(i)->getItemName() == "IceCream")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,-0.41,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_ICECREAM], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	

				}
				else if ( player.getInventory(i)->getItemName() == "McNCheese")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,0,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_MCNCHEESE], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
				else if ( player.getInventory(i)->getItemName() == "Toblerone")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,-0.3,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_TOBLERONE], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
				else if ( player.getInventory(i)->getItemName() == "Rocher")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,0.3,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_ROCHER], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
				else if ( player.getInventory(i)->getItemName() == "Reditos")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,0,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_REDITOS], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
				else if ( player.getInventory(i)->getItemName() == "Dewitos")
				{						modelStack.PushMatrix();
				modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
				modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
				modelStack.PushMatrix();
				modelStack.Translate(StartPosX+(i*PerX)-offSetX,0,StartPosZ+offSetZ);
				modelStack.Rotate(90,0,1,0);
				modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_DEWITOS], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
				}
				else if ( player.getInventory(i)->getItemName() == "Lays")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,0.39,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();
				}
				else if ( player.getInventory(i)->getItemName() == "Cactus")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,0,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_CACTUS], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
				else if ( player.getInventory(i)->getItemName() == "Vegcan")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,0,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_VEGCAN], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
				else if ( player.getInventory(i)->getItemName() == "PCan")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,0,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_PUMPKINCAN], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
				else if ( player.getInventory(i)->getItemName() == "MOaties")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,-0.4,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_MOATIES], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
				else if ( player.getInventory(i)->getItemName() == "OBox")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,0,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_CEREAL_2], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
				else if ( player.getInventory(i)->getItemName() == "Chickenstock")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,0,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_CHICKENSTOCK], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
			/*	else if ( player.getInventory(i)->getItemName() == "Pistol")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,0,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_PISTOL], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
				else if ( player.getInventory(i)->getItemName() == "Rifle" )
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,0,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_RIFLE], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}*/
				else if ( player.getInventory(i)->getItemName() == "Milo")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,0,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_MILO], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
				else if ( player.getInventory(i)->getItemName() == "SardCan")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,-0.2,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_SARDCAN], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
				else if ( player.getInventory(i)->getItemName() == "SoupCan")
				{
					modelStack.PushMatrix();
					modelStack.Translate(trolley.LastX,data.GetRenderPos(21)->getTranslationY()+tempOffsetY,trolley.LastZ);
					modelStack.Rotate(DupeRot,data.GetRenderPos(21)->getRX(),data.GetRenderPos(21)->getRY(),data.GetRenderPos(21)->getRZ());
					modelStack.PushMatrix();
					modelStack.Translate(StartPosX+(i*PerX)-offSetX,-0.1,StartPosZ+offSetZ);
					modelStack.Rotate(90,0,1,0);
					modelStack.Scale(data.GetRenderPos(21)->getScaleX(),data.GetRenderPos(21)->getScaleY(),data.GetRenderPos(21)->getScaleZ());
					RenderMesh(meshList[GEO_MODEL_TOMATOSOUPCAN], true);
					modelStack.PopMatrix();	
					modelStack.PopMatrix();	
				}
			}
	}
}
/***************************************************************/
/*!
/brief
Codes for general render functions.
*/
/***************************************************************/
void SP2::Render()
{
	//bool toggleLight = false;

	//clear depth and color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Temp variables
	Mtx44 MVP;

	viewStack.LoadIdentity();
	if ( camera.getCameraState() == 0 )
	{
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	}
	else if ( camera.getCameraState() == 1 )
	{
	viewStack.LookAt(Security1.position.x, Security1.position.y, Security1.position.z, Security1.target.x, Security1.target.y, Security1.target.z, Security1.up.x, Security1.up.y, Security1.up.z);
	}
	else if ( camera.getCameraState() == 2 )
	{
	viewStack.LookAt(Security2.position.x, Security2.position.y, Security2.position.z, Security2.target.x, Security2.target.y, Security2.target.z, Security2.up.x, Security2.up.y, Security2.up.z);
	}
	else if ( camera.getCameraState() == 3 )
	{
	viewStack.LookAt(EndScene.position.x, EndScene.position.y, EndScene.position.z, EndScene.target.x, EndScene.target.y, EndScene.target.z, EndScene.up.x, EndScene.up.y, EndScene.up.z);
	}
	modelStack.LoadIdentity();

	if(lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	if(lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	if(lights[2].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[2].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	if(lights[3].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[3].position;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[3].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT3_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	if(lights[4].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[4].position;
		glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[4].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT4_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}	

	RenderMesh(meshList[GEO_AXES], false);
	RenderWorld();
	RenderPlayer();
	RenderSkybox();
	RenderFNB();
	Trolley();
	TrolleyUpdate();
	CheckOut();
	EscapeSteal();

	modelStack.PushMatrix();
	modelStack.Translate(-5,20,-190);
	modelStack.Rotate(0,1,0,0);
	modelStack.Scale(ExplosionScale,ExplosionScale,ExplosionScale);
	if (ExplosionScale < 100 && EscapeEnd == true)
	{
	RenderMesh(meshList[GEO_EXPLODE], true);
	}
	modelStack.PopMatrix();

	if ( police == true)
	{
		RenderPolice();
	}

		for( int i=0; i < bullet.size(); i ++ )
		{
			modelStack.PushMatrix();
			modelStack.Translate(bullet[i]->Position.x,bullet[i]->Position.y,bullet[i]->Position.z);
			RenderMesh(meshList[GEO_MODEL_BULLET], true);
			modelStack.PopMatrix();
		}

			for( int i=0; i < Policebullet.size(); i ++ )
		{
			modelStack.PushMatrix();
			modelStack.Translate(Policebullet[i]->Position.x,Policebullet[i]->Position.y,Policebullet[i]->Position.z);
			RenderMesh(meshList[GEO_MODEL_BULLET], true);
			modelStack.PopMatrix();
		}

			if ( player.trolley == true)
			{
				modelStack.PushMatrix();
				modelStack.Translate(Trolleyz.Position.x,Trolleyz.Position.y,Trolleyz.Position.z);
				//RenderMesh(meshList[GEO_MODEL_BULLET], true);
				modelStack.PopMatrix();
			}

			if ( camera.getCameraState() == 0 )
				RenderScreenUI();
			else if (MenuKey == true)
				MainMenu();

			if(police == false && Application::IsKeyPressed(VK_TAB))
			{
				//ShopStaff = true;
				RenderItemMissing();
				if(UpdateItemMissing == true)
				{
					UpdateItemMissing = false;
				}
			}


	if ( player.returnInvenSize() == 0 && ShopStaff == true && police != true)
	{
		RenderUI(meshList[GEO_WINSCREEN], Color(0, 1, 0), 80,60, 0, 40, 30);
		GameEndStaff = true;
	}

	if ( GameEndSteal == true )
	{
		RenderUI(meshList[GEO_WINSCREEN], Color(0, 1, 0), 80,60, 0, 40, 30);
		police = false;
	}
	if ( GameEndStealLose == true )
	{
		RenderUI(meshList[GEO_ENDSCREEN], Color(0, 1, 0), 80,60, 0, 40, 30);
		police = false;
	}

	if ( ShopStaff == true && GameEndStaff != true)
		RenderTextOnScreen(meshList[GEO_TEXT], DTtime, Color(0,1,0), 5,5,10);

	if ( GameEndStaff == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], EndGame, Color(1,1,1), 3,5,10);
	}

	if (GameEndSteal == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], EndGameSteal, Color(1,1,1), 3,5,10);
	}

	if (GameEndStealLose == true)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], EndGameStealL, Color(1,1,1), 3,5,10);
	}
}
/***************************************************************/
/*!
/brief
Codes to render a basic character model
*/
/***************************************************************/
void SP2::RenderBasicModel()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, 1, 0);
	RenderMesh(meshList[GEO_MODEL_HEAD], false);
	RenderMesh(meshList[GEO_MODEL_BODY], false);
	RenderMesh(meshList[GEO_MODEL_LEFTARM], false);
	modelStack.PushMatrix();
	modelStack.Translate(-3, 0, 0);
	RenderMesh(meshList[GEO_MODEL_RIGHTARM], false);
	modelStack.PopMatrix();
	RenderMesh(meshList[GEO_MODEL_LEFTLEG], false);
	modelStack.PushMatrix();
	modelStack.Translate(-1, 0, 0);
	RenderMesh(meshList[GEO_MODEL_RIGHTLEG], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}
/***************************************************************/
/*!
/brief
Codes to render the UI on screen.
*/
/***************************************************************/
void SP2::RenderScreenUI()
{
	double tempStamCalc = camera.getStaminaDupe();
	double Calc = (tempStamCalc/200) * 54 ;

	double tempHpCalc = player.getHealth();
	double HpCalc = (tempHpCalc/100) * 54 ;

	if ( Application::IsKeyPressed(VK_TAB))
	{
	RenderTextOnScreen(meshList[GEO_TEXT], "FPS: " , Color(0, 1, 0), 3, 14, 18);
	RenderTextOnScreen(meshList[GEO_TEXT], FPS_count , Color(0, 1, 0), 3, 18, 18);
	}

	//RenderTextOnScreen(meshList[GEO_TEXT], XPos , Color(0, 1, 0), 3, 0, 18);
	//RenderTextOnScreen(meshList[GEO_TEXT], ZPos , Color(0, 1, 0), 3, 0, 17);
	RenderTextOnScreen(meshList[GEO_TEXT], MONEY , Color(0, 1, 0), 3, 0, 14);
	RenderTextOnScreen(meshList[GEO_TEXT], ItemName , Color(0, 1, 0), 3, 13, 11);
	RenderTextOnScreen(meshList[GEO_TEXT], ItemPrice , Color(0, 1, 0), 3, 13, 10.2);
	RenderTextOnScreen(meshList[GEO_TEXT], ROLE , Color(0,1,0), 3, 0, 16);
	RenderTextOnScreen(meshList[GEO_TEXT], BulletCount , Color(0, 1, 0), 2, 20, 13);
	RenderTextOnScreen(meshList[GEO_TEXT], INSTRUCTIONS1 , Color(0,1,0), 3, 4, 13);
	RenderTextOnScreen(meshList[GEO_TEXT], INSTRUCTIONS2 , Color(0,1,0), 3, 4, 12);
	RenderTextOnScreen(meshList[GEO_TEXT], INSTRUCTIONS3 , Color(0,1,0), 3, 4, 11);
	RenderTextOnScreen(meshList[GEO_TEXT], INSTRUCTIONS4 , Color(0,1,0), 3, 4, 10);

	//RenderTextOnScreen(meshList[GEO_TEXT], Target, Color(0, 1, 0), 2, 0, 16);
	if (camera.downSight == false)
		RenderUI(meshList[GEO_XHAIR], Color(0, 1, 0), 15, 15, 15, 40, 30);
	RenderUI(meshList[GEO_HP], Color(0, 1, 0), HpCalc, 1.5, 15, 12.5, 9);
	RenderUI(meshList[GEO_STAM], Color(0, 1, 0), Calc, 1.5, 15, 45, 9);
	RenderUI(meshList[GEO_HPI], Color(0, 1, 0), 5, 4, 4, 12, 9);
	RenderUI(meshList[GEO_STAMI], Color(0, 1, 0), 4, 4, 4, 45, 9);

	if (PInstruct == true && PoliceInstructTimer < 5)
	{
	RenderUI(meshList[GEO_PINSTRUCTBOX], Color(0, 1, 0), 60, 50, 1.5, 40, 20);
	//RenderTextOnScreen(meshList[GEO_TEXT], "Load items on to your truck at the back of the store before its destroyed!" , Color(0, 1, 0), 1, 6.1, 9.2);
	}

	if (SInstruct == true && StaffInstructTimer < 5)
	{
	RenderUI(meshList[GEO_SINSTRUCTBOX], Color(0, 1, 0), 60, 50, 1.5, 40, 20);
	//RenderTextOnScreen(meshList[GEO_TEXT], "Restock items as fast as possible!" , Color(0, 1, 0), 2, 6.1, 9.2);
	}

	//Render Cashier Dialogue
	for(int i = 0; i<4; ++i)
	{
		if(camera.position.x >= cashier.GetRenderPos(i)->getTranslationX()-5 && camera.position.x <= cashier.GetRenderPos(i)->getTranslationX()-1 && camera.position.z >= cashier.GetRenderPos(i)->getTranslationZ()-2 && camera.position.z <= cashier.GetRenderPos(i)->getTranslationZ()+2)
		{
			RenderUI(meshList[GEO_DIALOGUEBOX], Color(0, 1, 0), 60, 50, 1.5, 40, 20);
			RenderTextOnScreen(meshList[GEO_TEXT], "Welcome to UNFAIRPRICE" , Color(0, 1, 0), 2, 6.1, 9.2);
		}
	}
	//Render shopper/guard Dialogue
	for(int i = 0; i<4; ++i)
	{
		if(camera.position.x >= shopper.getPosX()-8 && camera.position.x <= shopper.getPosX()+2 && camera.position.z >= shopper.getPosZ()-2 && camera.position.z <= shopper.getPosZ()+2)
		{
			RenderUI(meshList[GEO_DIALOGUEBOX], Color(0, 1, 0), 60, 50, 1.5, 40, 20);
			RenderTextOnScreen(meshList[GEO_TEXT], "What should i get?" , Color(0, 1, 0), 2, 6.1, 9.2);
		}
	}
	//RenderTextOnScreen(meshList[GEO_TEXT], HP, Color(1, 1, 1), 4, 3.2, 1.5);
	//RenderTextOnScreen(meshList[GEO_TEXT], STAM, Color(1, 1, 1), 4, 12.2, 1.5);
	//RenderUI(meshList[GEO_ALERT], Color(0, 1, 0), 15, 2.5, 15,1, 1);
	for ( int i = 0; i < player.getInventoryCap() ; ++i)
	{
		int temp = 0;
		if ( i >= 5)
		{
			temp = 4;
		}
	RenderUI(meshList[GEO_UI], Color(0, 1, 0), 5, 5, 5,15+(i*5.5)+temp, 4);
	}

	if (Instruct == true)
	{
		RenderUI(meshList[GEO_MAIN_INSTRUCT], Color(0, 1, 0), 80,60, 0, 40, 30);
	}

	for ( int i = 0; i < player.returnInvenSize(); ++i)
	{
		int temp = 0;
		if ( i >= 5)
		{
			temp = 4;
		}
		if ( player.returnInvenSize() != 0 )
		{
			if ( player.getInventory(i)->getItemName() == "Coke")
			{
				RenderUI(meshList[GEO_MODEL_COKE], Color(0, 1, 0), 5, 5, 5,15+(i*5.5)+temp, 4);
			}
			else if ( player.getInventory(i)->getItemName() == "Pepsi")
			{
				RenderUI(meshList[GEO_MODEL_PEPSI], Color(0, 1, 0), 5, 5, 5,15+(i*5.5)+temp, 4);
			}
			else if ( player.getInventory(i)->getItemName() == "MtDew")
			{
				RenderUI(meshList[GEO_MODEL_MTDEW], Color(0, 1, 0), 5, 5, 5,15+(i*5.5)+temp, 4);
			}
			else if ( player.getInventory(i)->getItemName() == "Pizza")
			{
				RenderUI(meshList[GEO_MODEL_PIZZA], Color(0, 1, 0), 3, 3, 3,15+(i*5.5)+temp, 4);
			}
			else if ( player.getInventory(i)->getItemName() == "IceCream")
			{
				RenderUI(meshList[GEO_MODEL_ICECREAM], Color(0, 1, 0), 2, 2, 2,15+(i*5.5)+temp, 2.5);
			}
			else if ( player.getInventory(i)->getItemName() == "McNCheese")
			{
				RenderUI(meshList[GEO_MODEL_MCNCHEESE], Color(0, 1, 0), 3, 3, 3,15+(i*5.5)+temp, 4);
			}
			else if ( player.getInventory(i)->getItemName() == "Toblerone")
			{
				RenderUI(meshList[GEO_MODEL_TOBLERONE], Color(0, 1, 0), 2, 4, 4,15+(i*5.5)+temp, 3.5);
			}
			else if ( player.getInventory(i)->getItemName() == "Rocher")
			{
				RenderUI(meshList[GEO_MODEL_ROCHER], Color(0, 1, 0), 3, 3, 3,15+(i*5.5)+temp, 4);
			}
			else if ( player.getInventory(i)->getItemName() == "Reditos")
			{
				RenderUI(meshList[GEO_MODEL_REDITOS], Color(0, 1, 0), 4, 4, 4,15+(i*5.5)+temp, 3);
			}
			else if ( player.getInventory(i)->getItemName() == "Dewitos")
			{
				RenderUI(meshList[GEO_MODEL_DEWITOS], Color(0, 1, 0), 3, 3, 3,15+(i*5.5)+temp, 3.5);
			}
			else if ( player.getInventory(i)->getItemName() == "Lays")
			{
				RenderUI(meshList[GEO_MODEL_LAYSCHIPS], Color(0, 1, 0), 2.5, 2.5, 2.5,15+(i*5.5)+temp, 4);
			}
			else if ( player.getInventory(i)->getItemName() == "Cactus")
			{
				RenderUI(meshList[GEO_MODEL_CACTUS], Color(0, 1, 0), 9.5, 8.5, 9.5,15+(i*5.5)+temp, 4);
			}
			else if ( player.getInventory(i)->getItemName() == "Vegcan")
			{
				RenderUI(meshList[GEO_MODEL_VEGCAN], Color(0, 1, 0), 6, 6, 6,15+(i*5.5)+temp, 1.5);
			}
			else if ( player.getInventory(i)->getItemName() == "PCan")
			{
				RenderUI(meshList[GEO_MODEL_PUMPKINCAN], Color(0, 1, 0), 4.5, 4.5, 4.5,15+(i*5.5)+temp, 4);
			}
			else if ( player.getInventory(i)->getItemName() == "MOaties")
			{
				RenderUI(meshList[GEO_MODEL_MOATIES], Color(0, 1, 0), 3, 3, 3,15+(i*5.5)+temp, 2);
			}
			else if ( player.getInventory(i)->getItemName() == "OBox")
			{
				RenderUI(meshList[GEO_MODEL_CEREAL_2], Color(0, 1, 0), 2.5, 2.5, 2.5,15+(i*5.5)+temp, 4);
			}
			else if ( player.getInventory(i)->getItemName() == "Chickenstock")
			{
				RenderUI(meshList[GEO_MODEL_CHICKENSTOCK], Color(0, 1, 0), 3, 3, 3,15+(i*5.5)+temp,3.5);
			}
			/*else if ( player.getInventory(i)->getItemName() == "Pistol")
			{
				RenderUI(meshList[GEO_MODEL_PISTOL], Color(0, 1, 0), 3.5, 3.5, 3.5,15+(i*5.5)+temp, 3);
			}
			else if ( player.getInventory(i)->getItemName() == "Rifle")
			{
				RenderUI(meshList[GEO_MODEL_RIFLE], Color(0, 1, 0), 2.5, 3.5, 3.5,15+(i*5.5)+temp, 3);
			}*/
			else if ( player.getInventory(i)->getItemName() == "Milo")
			{
				RenderUI(meshList[GEO_MODEL_MILO], Color(0, 1, 0), 3.5, 3.5, 3.5,15+(i*5.5)+temp, 4);
			}
			else if ( player.getInventory(i)->getItemName() == "SardCan")
			{
				RenderUI(meshList[GEO_MODEL_SARDCAN], Color(0, 1, 0), 10, 10, 10,15+(i*5.5)+temp, 4);
			}
			else if ( player.getInventory(i)->getItemName() == "SoupCan")
			{
				RenderUI(meshList[GEO_MODEL_TOMATOSOUPCAN], Color(0, 1, 0), 6, 6, 6,15+(i*5.5)+temp, 4);
			}
		}
	}
}

void SP2::RenderItemMissing()
{
	RenderUI(meshList[GEO_MISSINGITEM], Color(0, 1, 0), 45, 32, 1, 65, 35);
	LengthY = 0;
	int offsetlist1 = 0, offsetlist2 = 0, offsetlist3 = 0, offsetlist4 = 0, offsetlist5 = 0, offsetlist6 = 0, offsetlist7 = 0, offsetlist8 = 0, offsetlist9 = 0, offsetlist10 = 0, offsetlist11 = 0, offsetlist12 = 0, offsetlist13 = 0, offsetlist14 = 0, offsetlist15 = 0, offsetlist16 = 0, offsetlist17 = 0, offsetlist18 = 0, offsetlist19 = 0, offsetlist20 = 0, offsetlist21 = 0, offsetlist22 = 0;
	float offsetx = 14;
	float offsety = 15;

	for(int i = 0; i < player.returnInvenSize(); i++)
	{
			RenderTextOnScreen(meshList[GEO_TEXT],player.getInventory(i)->getItemName(),Color(0,1,0), 3, offsetx, offsety - LengthY);
			LengthY+= 1;
	}
	/*
	for(int i = 0 ; i < FNB.ReturnListSize(); ++i)
	{
		if(LengthY > 10)
		{
			offsetx = 10;
			LengthY = 0;
		}
		if(FNB.GetRenderPosItem(i)->getItemAvailability() == false)
		{
			if (FNB.GetRenderPosItem(i)->getItemName() == "Coke" && offsetlist1 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"Coke",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist1++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "Pepsi" && offsetlist2 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"Pepsi",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist2++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "MtDew" && offsetlist3 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"MtDew",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist3++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "Pizza" && offsetlist4 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"Pizza",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist4++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "IceCream" && offsetlist5 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"IceCream",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist5++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "McNCheese" && offsetlist6 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"McNCheese",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist6++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "Toblerone" && offsetlist7 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"Toblerone",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist7++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "Rocher" && offsetlist8 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"Rocher",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist8++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "Reditos" && offsetlist9 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"Reditos",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist9++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "Dewitos" && offsetlist10 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"Dewitos",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist10++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "Lays" && offsetlist11 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"Lays",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist11++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "Cactus" && offsetlist12 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"Cactus",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist12++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "Vegcan" && offsetlist13 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"Vegcan",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist13++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "PCan" && offsetlist14 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"PCan",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist14++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "MOaties" && offsetlist15 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"MOaties",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist15++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "OBox" && offsetlist16 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"OBox",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist16++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "Chickenstock" && offsetlist17 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"Chickenstock",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist17++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "Pistol" && offsetlist18 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"Pistol",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist18++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "Rifle" && offsetlist19 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"Rifle",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist19++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "Milo" && offsetlist20 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"Milo",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist20++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "SardCan" && offsetlist21 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"SardCan",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist21++;
			}
			else if (FNB.GetRenderPosItem(i)->getItemName() == "SoupCan" && offsetlist22 < 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT],"SoupCan",Color(0,1,0), 3, offsetx, offsety - LengthY);
				LengthY += 1;
				offsetlist22++;
			}
		}
	}
	*/
}

void SP2::MainMenu()
{
	if(MenuKey == true)
	{	float buttonsize = 20;
	//RenderUI(meshList[GEO_TINT], Color(0, 1, 0), 80, 60, 1.5, 40, 30);
	RenderUI(meshList[GEO_LOGO], Color(0, 1, 0), 80, 80, 1.5, 40, 50);
	//RenderTextOnScreen(meshList[GEO_TEXT], TITLE, Color(0,1,0), 6, 1, 7);
	if (MenuState == 1)
	{
		float middle = 40;
		float offset = 25;
		RenderUI(meshList[GEO_INSTRUCTIONBTN], Color(0,1,0), 25, 25, 1.5, middle, 25);
		RenderUI(meshList[GEO_STARTBTN], Color(0,1,0), buttonsize, buttonsize, 1.5, middle + offset, 25);
	}

	else if (MenuState == 2)
	{
		float middle = 40;
		float offset = 25;
		RenderUI(meshList[GEO_STARTBTN], Color(0,1,0), 25, 25, 1.5, middle, 25);
		RenderUI(meshList[GEO_INSTRUCTIONBTN], Color(0,1,0), buttonsize, buttonsize, 1.5, middle - offset, 25);
		RenderUI(meshList[GEO_EXITBTN], Color(0,1,0), buttonsize, buttonsize, 1.5, middle + offset, 25);
	}
	else if (MenuState == 3)
	{
		float middle = 40;
		float offset = 25;
		RenderUI(meshList[GEO_EXITBTN], Color(0,1,0), 25, 25, 1.5, middle, 25);
		RenderUI(meshList[GEO_STARTBTN], Color(0,1,0), buttonsize, buttonsize, 1.5, middle - offset, 25);
	}
	}
}

void SP2::MenuUpdate(double dt)
{
	if (MenuLimit > 0 )
	{
		MenuLimit -= (3*dt);
	}
	if(Application::IsKeyPressed(VK_LEFT))
	{
		if ( MenuState > 1 && MenuLimit <= 0 )
		{
			MenuState--;
			MenuLimit = true;
			MusicPlayer.MetalClank();
		}
	}	
	if(Application::IsKeyPressed(VK_RIGHT))
	{
		if ( MenuState <3 && MenuLimit <= 0)
		{
			MenuState++;
			MenuLimit = true;
			MusicPlayer.MetalClank();
		}
	}
	if(Application::IsKeyPressed(VK_BACK) && Instruct == true)
	{
		camera.setCameraState(2);
		Instruct = false;
	}

	if(Application::IsKeyPressed(VK_RETURN))
	{
		if ( MenuState == 1 && MenuLimit <= 0)
		{
			Instruct = true;
			MenuLimit = true;
			camera.setCameraState(0);
			//MenuKey = false;
		}
		
		else if ( MenuState == 2 && MenuLimit <= 0)
		{
			camera.setCameraState(0);
			MenuLimit = true;
			MenuKey = false;
		}

		else if ( MenuState == 3 && MenuLimit <= 0)
		{
				keybd_event( VK_ESCAPE,
				0X1B,
				KEYEVENTF_EXTENDEDKEY | 0,
				0 );

					keybd_event( VK_ESCAPE,
				0X1B,
				KEYEVENTF_EXTENDEDKEY | 0,
				0 );
		}
	}	

		if (MenuKey == true)
	{
		Lightswitch = 0;
		lights[0].power = 0;
		lights[1].power = 0;
		lights[2].power = 0;
		lights[3].power = 0;
		lights[4].power = 0;


		glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
		glUniform1f(m_parameters[U_LIGHT2_POWER], lights[2].power);
		glUniform1f(m_parameters[U_LIGHT3_POWER], lights[3].power);
		glUniform1f(m_parameters[U_LIGHT4_POWER], lights[4].power);
		glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	}
}

void SP2::LightsReset()
{
	if (police != true)
	{
		Lightswitch = 1;
		lights[1].color.Set(Lightswitch,Lightswitch,Lightswitch);
		lights[2].color.Set(Lightswitch,Lightswitch,Lightswitch);
		lights[3].color.Set(Lightswitch,Lightswitch,Lightswitch);
		lights[4].color.Set(Lightswitch,Lightswitch,Lightswitch);
		lights[0].power = 1.0f;
		lights[1].power = 0.2f;
		lights[2].power = 0.2f;
		lights[3].power = 0.2f;
		lights[4].power = 0.2f;


		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
		glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
		glUniform1f(m_parameters[U_LIGHT2_POWER], lights[2].power);
		glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &lights[2].color.r);
		glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &lights[3].color.r);
		glUniform1f(m_parameters[U_LIGHT3_POWER], lights[3].power);
		glUniform3fv(m_parameters[U_LIGHT4_COLOR], 1, &lights[4].color.r);
		glUniform1f(m_parameters[U_LIGHT4_POWER], lights[4].power);
		glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	}
}

void SP2::ItemMissing()
{
	for(int i = 0; i < 5; i++)
	{
		std::stringstream listitem;
		while(RandomItem >= FNB.ReturnListSize())
		{
			RandomItem = rand() % 1000;
		}
	
		if (FNB.GetRenderPosItem(RandomItem)->getItemAvailability() == true)
		{
			FNB.GetRenderPosItem(RandomItem)->setItemAvailable(0);
			listitem << FNB.GetRenderPosItem(RandomItem)->getItemName();
			missinglist = listitem.str();
			nameofitem[i] = missinglist;
			noofitem[i] = RandomItem;
			player.setInventory(FNB.GetRenderPosItem(RandomItem)->getItemName(),FNB.GetRenderPosItem(RandomItem)->getItemPrice());
			RandomItem = rand() % 1000;
		}
		else
		{
			while(FNB.GetRenderPosItem(RandomItem)->getItemAvailability() == false)
			{
				RandomItem = rand() % 1000;
				while(RandomItem >= FNB.ReturnListSize())
				{
					RandomItem = rand() % 1000;
				}
				FNB.GetRenderPosItem(RandomItem)->setItemAvailable(0);
				listitem << FNB.GetRenderPosItem(RandomItem)->getItemName();
				missinglist = listitem.str();
				nameofitem[i] = missinglist;
				noofitem[i] = RandomItem;
				player.setInventory(FNB.GetRenderPosItem(RandomItem)->getItemName(),FNB.GetRenderPosItem(RandomItem)->getItemPrice());
				RandomItem = rand() % 1000;
			}
		}
	}
}
/***************************************************************/
/*!
/brief
Code for item collection and replacement controls.
*/
/***************************************************************/
void SP2::CheckItem()
{
	if(Application::IsKeyPressed('E'))
	{
		player.Takeitems = true;
	}

	if ( player.Takeitems == true)
	{
		for ( int i = 0; i < FNB.ReturnListSize();  ++i)
		{
			if ( camera.target.x >= FNB.GetRenderPosItem(i)->getItemTranslationX()-0.4 && camera.target.x <= FNB.GetRenderPosItem(i)->getItemTranslationX()+0.4 
				&& camera.target.y >= FNB.GetRenderPosItem(i)->getItemTranslationY()-1.2 && camera.target.y <= FNB.GetRenderPosItem(i)->getItemTranslationY()+0.8
				&& camera.target.z >= FNB.GetRenderPosItem(i)->getItemTranslationZ()-1 && camera.target.z <= FNB.GetRenderPosItem(i)->getItemTranslationZ()+1)
			{
				if (player.returnInvenSize() < player.getInventoryCap() && FNB.GetRenderPosItem(i)->getItemAvailability() == true && FNB.GetRenderPosItem(i)->getItemName() == ItemName)
				{
					if (ArmSwing < 140 )
					{
						ArmSwing+=10;
					}	

					if ( FNB.GetRenderPosItem(i)->getItemName() != "Rifle" && FNB.GetRenderPosItem(i)->getItemName() != "Pistol")
					{
						if (ArmSwing >= 140)
						{
							FNB.GetRenderPosItem(i)->setItemAvailable(0);
							player.setInventory(FNB.GetRenderPosItem(i)->getItemName(),FNB.GetRenderPosItem(i)->getItemPrice());
							player.Takeitems = false;
							break;
						}
					}
					else
					{
						FNB.GetRenderPosItem(i)->setItemAvailable(0);
						shoot.clip++;
						player.Takeitems = false;
						break;
					}
				}
			}
		}
	}
	if ( player.Takeitems == false)
	{
		if (ArmSwing > 0 )
			ArmSwing-=2;
	}

	if(Application::IsKeyPressed('G'))
	{
		for ( int i = 0; i < FNB.ReturnListSize();  ++i)
		{
			if ( camera.target.x >= FNB.GetRenderPosItem(i)->getItemTranslationX()-0.4 && camera.target.x <= FNB.GetRenderPosItem(i)->getItemTranslationX()+0.4 
				&& camera.target.y >= FNB.GetRenderPosItem(i)->getItemTranslationY()-1.2 && camera.target.y <= FNB.GetRenderPosItem(i)->getItemTranslationY()+0.8 
				&& camera.target.z >= FNB.GetRenderPosItem(i)->getItemTranslationZ()-1 && camera.target.z <= FNB.GetRenderPosItem(i)->getItemTranslationZ()+1)
			{
				if (player.returnInvenSize() > 0 && FNB.GetRenderPosItem(i)->getItemAvailability() == false)
				{
					for ( int j = 0; j < player.returnInvenSize(); ++j)
					{
						if ( player.getInventory(j)->getItemName() == FNB.GetRenderPosItem(i)->getItemName())
						{
							FNB.GetRenderPosItem(i)->setItemAvailable(1);
							player.dropItem(FNB.GetRenderPosItem(i)->getItemName());
							break;
						}
					}
				}
			}
		}
	}
}
/***************************************************************/
/*!
/brief
Codes for checking out items.
*/
/***************************************************************/
void SP2::CheckOut()
{ 
	float tempOffsetY = 3.8;
	float offSetZ = 0;
	float offSetX = 0;
	float StartPosZ = -2.3;
	float StartPosX = 1.8;
	float PerZ = 1;
	if(Application::IsKeyPressed('E') && player.returnInvenSize() != 0 && ShopStaff != true && police != true)
	{
		for ( int i = 0; i < 4;  ++i)
		{
			if ( camera.target.x >= cashier.GetRenderPos(i)->getTranslationX()-4 && camera.target.x <= cashier.GetRenderPos(i)->getTranslationX()+4 
				&& camera.target.y >= cashier.GetRenderPos(i)->getTranslationY()-10 && camera.target.y <= cashier.GetRenderPos(i)->getTranslationY()+10 
				&& camera.target.z >= cashier.GetRenderPos(i)->getTranslationZ()-3 && camera.target.z <= cashier.GetRenderPos(i)->getTranslationZ()+3)
			{
				paying = true;
				ArmSwing = 0;
				CashierOffetX = cashier.GetRenderPos(i)->getTranslationX();
				CashierOffetY = cashier.GetRenderPos(i)->getTranslationY();
				CashierOffetZ = cashier.GetRenderPos(i)->getTranslationZ();
				//Play CheckOut music
				MusicPlayer.CheckOut();
			}
		}
	}

	if ( paying == false)
		{
			if (ArmSwing > 0 )
				ArmSwing-=2;
		}

	if ( paying == true && player.returnInvenSize() != 0)
	{

		for ( int j = 0; j < player.returnInvenSize(); ++j)
		{
			if ( j >= 5)
			{
				offSetX = 1.5;
				offSetZ = 5;
			}
			if ( player.getInventory(j)->getItemName() == "Coke")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);
				//modelStack.Rotate(player.getInventory(j)->getItemRotation(),player.getInventory(j)->getItemRX(),player.getInventory(j)->getItemRY(),player.getInventory(j)->getItemRZ());
				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,0,StartPosZ+(j*PerZ)-offSetZ);
				RenderMesh(meshList[GEO_MODEL_COKE], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "Pepsi")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,0,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_PEPSI], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "MtDew")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,0,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_MTDEW], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "Pizza")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,0.2,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_PIZZA], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "IceCream")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,-0.41,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_ICECREAM], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	

			}
			else if ( player.getInventory(j)->getItemName() == "McNCheese")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,0,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_MCNCHEESE], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "Toblerone")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,-0.3,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_TOBLERONE], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "Rocher")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,0.3,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_ROCHER], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "Reditos")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,0,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_REDITOS], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "Dewitos")
			{						modelStack.PushMatrix();
			modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

			modelStack.PushMatrix();
			modelStack.Translate(StartPosX-offSetX,0,StartPosZ+(j*PerZ)-offSetZ);

			RenderMesh(meshList[GEO_MODEL_DEWITOS], true);
			modelStack.PopMatrix();	
			modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "Lays")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,0.39,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "Cactus")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,-0.2,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_CACTUS], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "Vegcan")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,0,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_VEGCAN], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "PCan")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,0,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_PUMPKINCAN], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "MOaties")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,-0.4,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_MOATIES], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "OBox")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,0,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_CEREAL_2], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "Chickenstock")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,0,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_CHICKENSTOCK], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
		/*	else if ( player.getInventory(j)->getItemName() == "Pistol")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,0,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_PISTOL], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "Rifle")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,0,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_RIFLE], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}*/
			else if ( player.getInventory(j)->getItemName() == "Milo")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,0,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_MILO], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "SardCan")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,-0.2,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_SARDCAN], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			else if ( player.getInventory(j)->getItemName() == "SoupCan")
			{
				modelStack.PushMatrix();
				modelStack.Translate(CashierOffetX,CashierOffetY+tempOffsetY,CashierOffetZ);

				modelStack.PushMatrix();
				modelStack.Translate(StartPosX-offSetX,-0.1,StartPosZ+(j*PerZ)-offSetZ);

				RenderMesh(meshList[GEO_MODEL_TOMATOSOUPCAN], true);
				modelStack.PopMatrix();	
				modelStack.PopMatrix();	
			}
			if (ArmSwing < 140 && paying == true )
			{
				ArmSwing+=5;
			}
			else if (ArmSwing >= 140 && paying == true )
			{
				player.setMoney(player.getInventory(j)->getItemPrice());
				player.sellItems(j);
			}
		}
		if (player.returnInvenSize() == 0)
			{
				paying = false;
			}
	}
}
/***************************************************************/
/*!
/brief
Codes for escaping from the mart.
*/
/***************************************************************/
void SP2::EscapeSteal()
{
	if(Application::IsKeyPressed('E') && player.returnInvenSize() != 0)
	{
		if ( camera.target.x >= VanBounds[4]->TTL.x && camera.target.x <= VanBounds[4]->TTR.x
			&& camera.target.y >= VanBounds[4]->BTL.y && camera.target.y <= VanBounds[4]->TTL.y
			&& camera.target.z >= VanBounds[4]->TBL.z-2 && camera.target.z <= VanBounds[4]->TBL.z+2 && police == true)
		{
			for ( int i = 0; i <player.returnInvenSize(); ++i)
			{
				player.setMoney(player.getInventory(i)->getItemPrice()*-1);
				player.sellItems(i);
			}
		}
	}
		if(Application::IsKeyPressed('E') && player.getMoney() > 0 )
		{
			if ( camera.target.x >= VanBounds[4]->TTL.x && camera.target.x <= VanBounds[4]->TTR.x
				&& camera.target.y >= VanBounds[4]->BTL.y && camera.target.y <= VanBounds[4]->TTL.y
				&& camera.target.z >= VanBounds[4]->TTL.z && camera.target.z <= VanBounds[4]->TBL.z-15 && police == true)
			{
				//End scene code
				EscapeEnd = true;
				MusicPlayer.Explosion();
				camera.setCameraState(3);
				
			}
		}
}
/***************************************************************/
/*!
/brief
Codes to render the Skybox.
*/
/***************************************************************/
void SP2::RenderSkybox()
{
	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Scale(1000,1000,1000);
	modelStack.Translate(0,0.1,-0.497);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Scale(1000,1000,1000);
	modelStack.Translate(0.497,0.1,0);
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Scale(1000,1000,1000);
	modelStack.Translate(-0.497,0.1,0);
	modelStack.Rotate(90,0,1,0);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Scale(1000,1000,1000);
	modelStack.Translate(0,0.1,0.497);
	modelStack.Rotate(180,0,1,0);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Scale(1000,1000,1000);
	modelStack.Translate(0,0.597,0);
	modelStack.Rotate(90,1,0,0);
	modelStack.PushMatrix();
	modelStack.Rotate(90,0,0,1);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Scale(1000,1000,1000);
	modelStack.Translate(0,0,0);
	modelStack.Rotate(-90,1,0,0);
	modelStack.PushMatrix();
	modelStack.Rotate(90,0,0,1);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();


}
/***************************************************************/
/*!
/brief
Codes to render the cashier NPC.
*/
/***************************************************************/
void SP2::RenderCashierModel()
{
	for(int i = 0; i < 4; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(cashiermodel.GetRenderPos(i)->getTranslationX(),cashiermodel.GetRenderPos(i)->getTranslationY(),cashiermodel.GetRenderPos(i)->getTranslationZ());
		modelStack.Rotate(cashiermodel.GetRenderPos(i)->getRotation(),cashiermodel.GetRenderPos(i)->getRX(),cashiermodel.GetRenderPos(i)->getRY(),cashiermodel.GetRenderPos(i)->getRZ());
		modelStack.Scale(cashiermodel.GetRenderPos(i)->getScaleX(),cashiermodel.GetRenderPos(i)->getScaleY(),cashiermodel.GetRenderPos(i)->getScaleZ());
		RenderMesh(meshList[GEO_MODEL_CASHIER_HEAD], false);
		RenderMesh(meshList[GEO_MODEL_CASHIER_BODY], false);
		RenderMesh(meshList[GEO_MODEL_CASHIER_LEFTARM], false);
		modelStack.PushMatrix();
		modelStack.Translate(-3, 0, 0);
		RenderMesh(meshList[GEO_MODEL_CASHIER_RIGHTARM], false);
		modelStack.PopMatrix();
		RenderMesh(meshList[GEO_MODEL_CASHIER_LEFTLEG], false);
		modelStack.PushMatrix();
		modelStack.Translate(-1, 0, 0);
		RenderMesh(meshList[GEO_MODEL_CASHIER_RIGHTLEG], false);
		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}
}
/***************************************************************/
/*!
/brief
Codes to render a policeman model.
*/
/***************************************************************/
void SP2::RenderPolice()
{
	for(int i=0; i< Police.ReturnListSize(); ++i)
	{
		if  ( Police.GetRenderPosItem(i)->getItemAvailability() == true && Police.GetRenderPosItem(i)->getItemName() == "PoliceVan")
		{
			modelStack.PushMatrix();
			modelStack.Translate(Police.GetRenderPosItem(i)->getItemTranslationX()+police1.PoliceXMove,Police.GetRenderPosItem(i)->getItemTranslationY(),Police.GetRenderPosItem(i)->getItemTranslationZ()+police1.PoliceZMove);
			modelStack.Rotate(Police.GetRenderPosItem(i)->getItemRotation()+police1.PoliceRotate,Police.GetRenderPosItem(i)->getItemRX(),Police.GetRenderPosItem(i)->getItemRY(),Police.GetRenderPosItem(i)->getItemRZ());
			modelStack.Scale(Police.GetRenderPosItem(i)->getItemScaleX(),Police.GetRenderPosItem(i)->getItemScaleY(),Police.GetRenderPosItem(i)->getItemScaleZ());
			RenderMesh(meshList[GEO_MODEL_POLICEVAN], true);
			//front Left wheel
			modelStack.PushMatrix();
			modelStack.Translate(-3.2,-1,5.7);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(police1.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			//front right wheel
			modelStack.PushMatrix();
			modelStack.Translate(3.2,-1,5.7);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(police1.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			//back left wheel
			modelStack.PushMatrix();
			modelStack.Translate(-3.2,-1,-6);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(police1.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			//back right wheel
			modelStack.PushMatrix();
			modelStack.Translate(3.2,-1,-6);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(police1.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			modelStack.PopMatrix();
		}

		if  ( Police.GetRenderPosItem(i)->getItemAvailability() == true && Police.GetRenderPosItem(i)->getItemName() == "PoliceVan2")
		{
			modelStack.PushMatrix();
			modelStack.Translate(Police.GetRenderPosItem(i)->getItemTranslationX()+police2.PoliceXMove,Police.GetRenderPosItem(i)->getItemTranslationY(),Police.GetRenderPosItem(i)->getItemTranslationZ()+police2.PoliceZMove);
			modelStack.Rotate(Police.GetRenderPosItem(i)->getItemRotation()+police2.PoliceRotate,Police.GetRenderPosItem(i)->getItemRX(),Police.GetRenderPosItem(i)->getItemRY(),Police.GetRenderPosItem(i)->getItemRZ());
			modelStack.Scale(Police.GetRenderPosItem(i)->getItemScaleX(),Police.GetRenderPosItem(i)->getItemScaleY(),Police.GetRenderPosItem(i)->getItemScaleZ());
			RenderMesh(meshList[GEO_MODEL_POLICEVAN], true);
			//front Left wheel
			modelStack.PushMatrix();
			modelStack.Translate(-3.2,-1,5.7);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(police2.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			//front right wheel
			modelStack.PushMatrix();
			modelStack.Translate(3.2,-1,5.7);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(police2.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			//back left wheel
			modelStack.PushMatrix();
			modelStack.Translate(-3.2,-1,-6);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(police2.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			//back right wheel
			modelStack.PushMatrix();
			modelStack.Translate(3.2,-1,-6);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(police2.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			modelStack.PopMatrix();
		}

		if  ( Police.GetRenderPosItem(i)->getItemAvailability() == true && Police.GetRenderPosItem(i)->getItemName() == "PoliceVan3")
		{
			modelStack.PushMatrix();
			modelStack.Translate(Police.GetRenderPosItem(i)->getItemTranslationX()+police3.PoliceXMove,Police.GetRenderPosItem(i)->getItemTranslationY(),Police.GetRenderPosItem(i)->getItemTranslationZ()+police3.PoliceZMove);
			modelStack.Rotate(Police.GetRenderPosItem(i)->getItemRotation()+police3.PoliceRotate,Police.GetRenderPosItem(i)->getItemRX(),Police.GetRenderPosItem(i)->getItemRY(),Police.GetRenderPosItem(i)->getItemRZ());
			modelStack.Scale(Police.GetRenderPosItem(i)->getItemScaleX(),Police.GetRenderPosItem(i)->getItemScaleY(),Police.GetRenderPosItem(i)->getItemScaleZ());
			RenderMesh(meshList[GEO_MODEL_POLICEVAN], true);
			//front Left wheel
			modelStack.PushMatrix();
			modelStack.Translate(-3.2,-1,5.7);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(police3.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			//front right wheel
			modelStack.PushMatrix();
			modelStack.Translate(3.2,-1,5.7);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(police3.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			//back left wheel
			modelStack.PushMatrix();
			modelStack.Translate(-3.2,-1,-6);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(police3.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			//back right wheel
			modelStack.PushMatrix();
			modelStack.Translate(3.2,-1,-6);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(police3.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			modelStack.PopMatrix();
		}

		if  ( Police.GetRenderPosItem(i)->getItemAvailability() == true && Police.GetRenderPosItem(i)->getItemName() == "PoliceVan4")
		{
			modelStack.PushMatrix();
			modelStack.Translate(Police.GetRenderPosItem(i)->getItemTranslationX()+police4.PoliceXMove,Police.GetRenderPosItem(i)->getItemTranslationY(),Police.GetRenderPosItem(i)->getItemTranslationZ()+police4.PoliceZMove);
			modelStack.Rotate(Police.GetRenderPosItem(i)->getItemRotation()+police4.PoliceRotate,Police.GetRenderPosItem(i)->getItemRX(),Police.GetRenderPosItem(i)->getItemRY(),Police.GetRenderPosItem(i)->getItemRZ());
			modelStack.Scale(Police.GetRenderPosItem(i)->getItemScaleX(),Police.GetRenderPosItem(i)->getItemScaleY(),Police.GetRenderPosItem(i)->getItemScaleZ());
			RenderMesh(meshList[GEO_MODEL_POLICEVAN], true);
			//front Left wheel
			modelStack.PushMatrix();
			modelStack.Translate(-3.2,-1,5.7);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(police4.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			//front right wheel
			modelStack.PushMatrix();
			modelStack.Translate(3.2,-1,5.7);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(police4.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			//back left wheel
			modelStack.PushMatrix();
			modelStack.Translate(-3.2,-1,-6);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(police4.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			//back right wheel
			modelStack.PushMatrix();
			modelStack.Translate(3.2,-1,-6);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(police4.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			modelStack.PopMatrix();
		}

		if  ( Police.GetRenderPosItem(i)->getItemAvailability() == true && Police.GetRenderPosItem(i)->getItemName() == "Escape")
		{
			modelStack.PushMatrix();
			modelStack.Translate(Police.GetRenderPosItem(i)->getItemTranslationX()+Escape.PoliceXMove,Police.GetRenderPosItem(i)->getItemTranslationY(),Police.GetRenderPosItem(i)->getItemTranslationZ()+Escape.PoliceZMove-EscapeCarMove);
			modelStack.Rotate(Police.GetRenderPosItem(i)->getItemRotation()+Escape.PoliceRotate,Police.GetRenderPosItem(i)->getItemRX(),Police.GetRenderPosItem(i)->getItemRY(),Police.GetRenderPosItem(i)->getItemRZ());
			modelStack.Scale(Police.GetRenderPosItem(i)->getItemScaleX(),Police.GetRenderPosItem(i)->getItemScaleY(),Police.GetRenderPosItem(i)->getItemScaleZ());
			RenderMesh(meshList[GEO_MODEL_ESCAPE], true);
			//front Left wheel
			modelStack.PushMatrix();
			modelStack.Translate(-3.2,-1,5.7);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(Escape.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			//front right wheel
			modelStack.PushMatrix();
			modelStack.Translate(3.2,-1,5.7);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(Escape.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			//back left wheel
			modelStack.PushMatrix();
			modelStack.Translate(-3.2,-1,-6);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(Escape.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			//back right wheel
			modelStack.PushMatrix();
			modelStack.Translate(3.2,-1,-6);
			modelStack.Rotate(90,1,0,0);
			modelStack.PushMatrix();
			modelStack.Rotate(Escape.WheelRot,1,0,0);
			modelStack.Scale(1,1,1);
			RenderMesh(meshList[GEO_MODEL_POLICEVANWHEEL], true);
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			modelStack.PopMatrix();
		}
	}
	for ( int i = 0;i < PoliceMan.ReadTextFilePoliceSize(); ++i)
	{
		if (police1.arrived == true && PoliceMan.GetRenderPosPolice(i)->getItemAvailability() == true && PoliceMan.GetRenderPosPolice(i)->getItemName() == "Police")
		{
			modelStack.PushMatrix();
			modelStack.Translate(PoliceMan.GetRenderPosPolice(i)->getItemTranslationX(),PoliceMan.GetRenderPosPolice(i)->getItemTranslationY(),PoliceMan.GetRenderPosPolice(i)->getItemTranslationZ());
			modelStack.Rotate(PoliceMan.GetRenderPosPolice(i)->getItemRotation(),PoliceMan.GetRenderPosPolice(i)->getItemRX(),PoliceMan.GetRenderPosPolice(i)->getItemRY(),PoliceMan.GetRenderPosPolice(i)->getItemRZ());
			modelStack.Scale(PoliceMan.GetRenderPosPolice(i)->getItemScaleX(),PoliceMan.GetRenderPosPolice(i)->getItemScaleY(),PoliceMan.GetRenderPosPolice(i)->getItemScaleZ());
			if ( PoliceMan.GetRenderPosPolice(i)->Damaged == false)
				RenderMesh(meshList[GEO_MODEL_POLICEMAN], true);
			else if ( PoliceMan.GetRenderPosPolice(i)->Damaged == true)
				RenderMesh(meshList[GEO_MODEL_POLICEMAN_DAMAGED], true);
			modelStack.PopMatrix();
		}
	}
}
/***************************************************************/
/*!
/brief
Codes for policemen to shoot at the escape van.
*/
/***************************************************************/
void SP2::PoliceShoot(double dt)
{
	for ( int i = 0; i < PoliceMan.ReadTextFilePoliceSize(); ++i)
	{
		if ( i == 2)
		{
			if(police1.arrived == true && PoliceMan.GetRenderPosPolice(i)->getItemAvailability() == true)
			{
				if ( PoliceMan.GetRenderPosPolice(i)->ShootCD < 3)
				{
					PoliceMan.GetRenderPosPolice(i)->ShootCD += (1*dt);
				}

				if (PoliceMan.GetRenderPosPolice(i)->ShootCD >= 3)
				{
					CBullet* temp =  new CBullet();
					temp->Position = 
						Vector3(PoliceMan.GetRenderPosPolice(i)->getItemTranslationX(),
						PoliceMan.GetRenderPosPolice(i)->getItemTranslationY()+5,
						PoliceMan.GetRenderPosPolice(i)->getItemTranslationZ());
					temp->Direction = 
						Vector3(-0.999,0,0);
					Policebullet.push_back( temp );
					PoliceMan.GetRenderPosPolice(i)->ShootCD = 0;
				}
			}
		}
		else if ( i == 3)
		{
			if(police1.arrived == true && PoliceMan.GetRenderPosPolice(i)->getItemAvailability() == true)
			{
				if ( PoliceMan.GetRenderPosPolice(i)->ShootCD < 3)
				{
					PoliceMan.GetRenderPosPolice(i)->ShootCD += (1*dt);
				}

				if (PoliceMan.GetRenderPosPolice(i)->ShootCD >= 3)
				{
					CBullet* temp =  new CBullet();
					temp->Position = 
						Vector3(PoliceMan.GetRenderPosPolice(i)->getItemTranslationX(),
						PoliceMan.GetRenderPosPolice(i)->getItemTranslationY()+5,
						PoliceMan.GetRenderPosPolice(i)->getItemTranslationZ());
					temp->Direction = 
						Vector3(-0.994,0,-0.1);
					Policebullet.push_back( temp );
					PoliceMan.GetRenderPosPolice(i)->ShootCD = 0;
				}
			}
		}
		else if ( i == 6)
		{
			if(police1.arrived == true && PoliceMan.GetRenderPosPolice(i)->getItemAvailability() == true)
			{
				if ( PoliceMan.GetRenderPosPolice(i)->ShootCD < 3)
				{
					PoliceMan.GetRenderPosPolice(i)->ShootCD += (1*dt);
				}

				if (PoliceMan.GetRenderPosPolice(i)->ShootCD >= 3)
				{
					CBullet* temp =  new CBullet();
					temp->Position = 
						Vector3(PoliceMan.GetRenderPosPolice(i)->getItemTranslationX(),
						PoliceMan.GetRenderPosPolice(i)->getItemTranslationY()+5,
						PoliceMan.GetRenderPosPolice(i)->getItemTranslationZ());
					temp->Direction = 
						Vector3(0.999,0,0.1);
					Policebullet.push_back( temp );
					PoliceMan.GetRenderPosPolice(i)->ShootCD = 0;
				}
			}
		}

		else if ( i == 7)
		{
			if(police1.arrived == true && PoliceMan.GetRenderPosPolice(i)->getItemAvailability() == true)
			{
				if ( PoliceMan.GetRenderPosPolice(i)->ShootCD < 3)
				{
					PoliceMan.GetRenderPosPolice(i)->ShootCD += (1*dt);
				}

				if (PoliceMan.GetRenderPosPolice(i)->ShootCD >= 3)
				{
					CBullet* temp =  new CBullet();
					temp->Position = 
						Vector3(PoliceMan.GetRenderPosPolice(i)->getItemTranslationX(),
						PoliceMan.GetRenderPosPolice(i)->getItemTranslationY()+5,
						PoliceMan.GetRenderPosPolice(i)->getItemTranslationZ());
					temp->Direction = 
						Vector3(0.992,0,-0.1);
					Policebullet.push_back( temp );
					PoliceMan.GetRenderPosPolice(i)->ShootCD = 0;
				}
			}
		}
	}
}

/***************************************************************/
/*!
/brief
Codes for your bullet collision.
*/
/***************************************************************/
bool SP2::BulletCollision(float x,float y,float z,double dt)
{
	Vector3 tempMart, tempShelves, tempCashier, tempFridge, tempPatroler;
	tempMart.x = data.GetRenderPos(0)->getTranslationX();
	tempMart.z = data.GetRenderPos(0)->getTranslationZ();
	tempShelves.x = shelve.GetRenderPos(0)->getTranslationX();
	tempShelves.z = shelve.GetRenderPos(0)->getTranslationZ();
	tempCashier.x = cashier.GetRenderPos(0)->getTranslationX();
	tempCashier.z = cashier.GetRenderPos(0)->getTranslationZ();
	tempFridge.x = fridge.GetRenderPos(0)->getTranslationX();
	tempFridge.z = fridge.GetRenderPos(0)->getTranslationZ();

	for ( int i = 0; i < colBounds.size(); ++i)
	{
		if ( x >= colBounds[i]->TTL.x && x <= colBounds[i]->TTR.x 
			&& y >= colBounds[i]->BTL.y && y <= colBounds[i]->TTL.y
			&& z >= colBounds[i]->TTL.z && z <= colBounds[i]->TBL.z
			)
		{
			if ( PoliceMan.GetRenderPosPolice(i)->getItemName() == "Police")
			{
				PoliceMan.GetRenderPosPolice(i)->sethealth(30);
				PoliceMan.GetRenderPosPolice(i)->Damaged = true;
				//colBounds.erase(colBounds.begin() +i );0
			}
			return true;
		}
		//fence check
		//leftfence
		else if( x <= data.GetRenderPos(7)->getTranslationX()-100)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
			}
			return true;
		}
		//right fence
		else if( x >= data.GetRenderPos(7)->getTranslationX()+90)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
			}
			return true;
		}
		else if( z <= data.GetRenderPos(7)->getTranslationZ()-205)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
			}
			return true;
		}
		else if( z >= data.GetRenderPos(7)->getTranslationZ()+180)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
			}
			return true;
		}

		////mart outer bound check
		////left wall
		else if( x >= tempMart.x - 78 && x <= tempMart.x - 71 && z <= tempMart.z + 54 && z >= tempMart.z - 54)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}
		////Back Wall Left
		else if( x >= tempMart.x -78 && x <= tempMart.x - 14 && z <= tempMart.z + -45  && z >= tempMart.z - 54)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		////Back Wall Right
		else if( x >= tempMart.x + 14 && x <= tempMart.x + 78 && z <= tempMart.z + -45  && z >= tempMart.z - 54)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		////Right Wall
		else if( x >= tempMart.x + 71 && x <= tempMart.x + 78 && z <= tempMart.z + 54  && z >= tempMart.z - 54)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		////Right Wall
		else if( x >= tempMart.x - 62 && x <= tempMart.x + 62 && z <= tempMart.z + 54  && z >= tempMart.z + 46)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		////Middle Wall
		else if( x >= tempMart.x - 62 && x <= tempMart.x + 62 && z <= tempMart.z - 22  && z >= tempMart.z  - 30)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		//Fridge Bounds Check
		else if(x >= tempFridge.x -92 && x <= tempFridge.x +8 && z <= tempFridge.z + 2 && z >= tempFridge.z)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		//Shelves Bounds Check
		for(int i = 0; i<3; ++i)
		{
			for(int j = 0; j<3; ++j)
			{
				if(x >= ((tempShelves.x - 4) + (j*34)) && x <= ((tempShelves.x + 22) + (j*34))  && z <= (tempShelves.z +48 -(i*15)) && z >= (tempShelves.z +43-(i*15)))
				{
					if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
					{
						Police.GetRenderPosItem(i)->setItemAvailable(false);
						
					}
					return true;
				}
			}
		}
		//Cashier Bounds Check
		for(int i = 0; i < 4; ++i)
		{
			if(x >= tempCashier.x -1 && x <= tempCashier.x +2 && z <= tempCashier.z + 53 -(i*16) && z >= tempCashier.z + 44-(i*16))
			{
				if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
				{
					Police.GetRenderPosItem(i)->setItemAvailable(false);
					
				}
				return true;
			}
		}

		//Fridge Bounds Check
		if(x >= tempFridge.x -92 && x <= tempFridge.x +8 && z <= tempFridge.z + 2 && z >= tempFridge.z)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}
	}
	return false;
}
/***************************************************************/
/*!
/brief
Codes for the enemy's bullets to hit things.
*/
/***************************************************************/
bool SP2::BulletCollisionEnemy(float x,float y,float z)
{
	Vector3 tempMart, tempShelves, tempCashier, tempFridge, tempPatroler;
	tempMart.x = data.GetRenderPos(0)->getTranslationX();
	tempMart.z = data.GetRenderPos(0)->getTranslationZ();
	tempShelves.x = shelve.GetRenderPos(0)->getTranslationX();
	tempShelves.z = shelve.GetRenderPos(0)->getTranslationZ();
	tempCashier.x = cashier.GetRenderPos(0)->getTranslationX();
	tempCashier.z = cashier.GetRenderPos(0)->getTranslationZ();
	tempFridge.x = fridge.GetRenderPos(0)->getTranslationX();
	tempFridge.z = fridge.GetRenderPos(0)->getTranslationZ();

	for ( int i = 0; i < VanBounds.size(); ++i)
	{
		if ( x >= VanBounds[i]->TTL.x && x <= VanBounds[i]->TTR.x 
			&& y >= VanBounds[i]->BTL.y && y <= VanBounds[i]->TTL.y
			&& z >= VanBounds[i]->TTL.z && z <= VanBounds[i]->TBL.z
			)
		{
			if ( PoliceCol.GetRenderPosItem(i)->getItemName() == "Escape")
			{
				player.setHealth(1);
				//colBounds.erase(colBounds.begin() +i );
			}
			return true;
		}
		//fence check
		//leftfence
		else if( x <= data.GetRenderPos(7)->getTranslationX()-100)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
			}
			return true;
		}
		//right fence
		else if( x >= data.GetRenderPos(7)->getTranslationX()+90)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
			}
			return true;
		}
		else if( z <= data.GetRenderPos(7)->getTranslationZ()-205)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
			}
			return true;
		}
		else if( z >= data.GetRenderPos(7)->getTranslationZ()+180)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
			}
			return true;
		}

		////mart outer bound check
		////left wall
		else if( x >= tempMart.x - 78 && x <= tempMart.x - 71 && z <= tempMart.z + 54 && z >= tempMart.z - 54)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}
		////Back Wall Left
		else if( x >= tempMart.x -78 && x <= tempMart.x - 14 && z <= tempMart.z + -45  && z >= tempMart.z - 54)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		////Back Wall Right
		else if( x >= tempMart.x + 14 && x <= tempMart.x + 78 && z <= tempMart.z + -45  && z >= tempMart.z - 54)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		////Right Wall
		else if( x >= tempMart.x + 71 && x <= tempMart.x + 78 && z <= tempMart.z + 54  && z >= tempMart.z - 54)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		////Right Wall
		else if( x >= tempMart.x - 62 && x <= tempMart.x + 62 && z <= tempMart.z + 54  && z >= tempMart.z + 46)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		////Middle Wall
		else if( x >= tempMart.x - 62 && x <= tempMart.x + 62 && z <= tempMart.z - 22  && z >= tempMart.z  - 30)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		//Fridge Bounds Check
		else if(x >= tempFridge.x -92 && x <= tempFridge.x +8 && z <= tempFridge.z + 2 && z >= tempFridge.z)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		//Shelves Bounds Check
		for(int i = 0; i<3; ++i)
		{
			for(int j = 0; j<3; ++j)
			{
				if(x >= ((tempShelves.x - 4) + (j*34)) && x <= ((tempShelves.x + 22) + (j*34))  && z <= (tempShelves.z +48 -(i*15)) && z >= (tempShelves.z +43-(i*15)))
				{
					if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
					{
						Police.GetRenderPosItem(i)->setItemAvailable(false);
						
					}
					return true;
				}
			}
		}
		//Cashier Bounds Check
		for(int i = 0; i < 4; ++i)
		{
			if(x >= tempCashier.x -1 && x <= tempCashier.x +2 && z <= tempCashier.z + 53 -(i*16) && z >= tempCashier.z + 44-(i*16))
			{
				if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
				{
					Police.GetRenderPosItem(i)->setItemAvailable(false);
					
				}
				return true;
			}
		}

		//Fridge Bounds Check
		if(x >= tempFridge.x -92 && x <= tempFridge.x +8 && z <= tempFridge.z + 2 && z >= tempFridge.z)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				//colBounds.erase(colBounds.begin() +i );
			}
			return true;
		}
		//fence check
		//leftfence
		else if( x <= data.GetRenderPos(7)->getTranslationX()-100)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
			}
			return true;
		}
		//right fence
		else if( x >= data.GetRenderPos(7)->getTranslationX()+90)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
			}
			return true;
		}
		else if( z <= data.GetRenderPos(7)->getTranslationZ()-205)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
			}
			return true;
		}
		else if( z >= data.GetRenderPos(7)->getTranslationZ()+180)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
			}
			return true;
		}

		////mart outer bound check
		////left wall
		else if( x >= tempMart.x - 78 && x <= tempMart.x - 71 && z <= tempMart.z + 54 && z >= tempMart.z - 54)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}
		////Back Wall Left
		else if( x >= tempMart.x -78 && x <= tempMart.x - 14 && z <= tempMart.z + -45  && z >= tempMart.z - 54)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		////Back Wall Right
		else if( x >= tempMart.x + 14 && x <= tempMart.x + 78 && z <= tempMart.z + -45  && z >= tempMart.z - 54)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		////Right Wall
		else if( x >= tempMart.x + 71 && x <= tempMart.x + 78 && z <= tempMart.z + 54  && z >= tempMart.z - 54)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		////Right Wall
		else if( x >= tempMart.x - 62 && x <= tempMart.x + 62 && z <= tempMart.z + 54  && z >= tempMart.z + 46)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		////Middle Wall
		else if( x >= tempMart.x - 62 && x <= tempMart.x + 62 && z <= tempMart.z - 22  && z >= tempMart.z  - 30)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		//Fridge Bounds Check
		else if(x >= tempFridge.x -92 && x <= tempFridge.x +8 && z <= tempFridge.z + 2 && z >= tempFridge.z)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);
				
			}
			return true;
		}

		//Shelves Bounds Check
		for(int i = 0; i<3; ++i)
		{
			for(int j = 0; j<3; ++j)
			{
				if(x >= ((tempShelves.x - 4) + (j*34)) && x <= ((tempShelves.x + 22) + (j*34))  && z <= (tempShelves.z +48 -(i*15)) && z >= (tempShelves.z +43-(i*15)))
				{
					if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
					{
						Police.GetRenderPosItem(i)->setItemAvailable(false);
						
					}
					return true;
				}
			}
		}
		//Cashier Bounds Check
		for(int i = 0; i < 4; ++i)
		{
			if(x >= tempCashier.x -1 && x <= tempCashier.x +2 && z <= tempCashier.z + 53 -(i*16) && z >= tempCashier.z + 44-(i*16))
			{
				if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
				{
					Police.GetRenderPosItem(i)->setItemAvailable(false);
					
				}
				return true;
			}
		}

		//Fridge Bounds Check
		if(x >= tempFridge.x -92 && x <= tempFridge.x +8 && z <= tempFridge.z + 2 && z >= tempFridge.z)
		{
			if ( Police.GetRenderPosItem(i)->getItemName() != "PoliceVan")
			{
				Police.GetRenderPosItem(i)->setItemAvailable(false);		
			}
			return true;
		}
	}
	return false;
}
/***************************************************************/
/*!
/brief
Codes to render the cashier counter.
*/
/***************************************************************/
void SP2::RenderCashier()
{
	for(int i = 0; i < 4; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(cashier.GetRenderPos(i)->getTranslationX(),cashier.GetRenderPos(i)->getTranslationY(),cashier.GetRenderPos(i)->getTranslationZ());
		modelStack.Rotate(cashier.GetRenderPos(i)->getRotation(),cashier.GetRenderPos(i)->getRX(),cashier.GetRenderPos(i)->getRY(),cashier.GetRenderPos(i)->getRZ());
		modelStack.Scale(cashier.GetRenderPos(i)->getScaleX(),cashier.GetRenderPos(i)->getScaleY(),cashier.GetRenderPos(i)->getScaleZ());
		RenderMesh(meshList[GEO_MODEL_CASHIER], true);
		modelStack.PopMatrix();
	}
}
/***************************************************************/
/*!
/brief
Codes to render the fridge shelves.
*/
/***************************************************************/
void SP2::RenderFridge()
{
	for(int i = 0; i < 7; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(fridge.GetRenderPos(0)->getTranslationX()-(i*14),fridge.GetRenderPos(0)->getTranslationY(),fridge.GetRenderPos(0)->getTranslationZ());
		modelStack.Rotate(fridge.GetRenderPos(0)->getRotation(),fridge.GetRenderPos(0)->getRX(),fridge.GetRenderPos(0)->getRY(),fridge.GetRenderPos(0)->getRZ());
		modelStack.Scale(fridge.GetRenderPos(0)->getScaleX(),fridge.GetRenderPos(0)->getScaleY(),fridge.GetRenderPos(0)->getScaleZ());
		RenderMesh(meshList[GEO_MODEL_FRIDGE], true);
		modelStack.PopMatrix();
	}
}
/***************************************************************/
/*!
/brief
Codes to render the shelves.
*/
/***************************************************************/
void SP2::RenderShelves()
{
	int temp = 0;
	for(int i = 1; i <= 3; ++ i )
	{
		for(int j = 0; j < 12; ++ j )
		{
			if ( j >= 4 && j < 8 )
			{
				temp = 10;
			}
			else  if ( j >= 8 )
			{
				temp = 20;
			}
			else
			{
				temp = 0;
			}
			
			modelStack.PushMatrix();
			modelStack.Translate(shelve.GetRenderPos(0)->getTranslationX()+(j*6)+temp,shelve.GetRenderPos(0)->getTranslationY(),shelve.GetRenderPos(0)->getTranslationZ()+(i*15));
			modelStack.Rotate(shelve.GetRenderPos(0)->getRotation(),shelve.GetRenderPos(0)->getRX(),shelve.GetRenderPos(0)->getRY(),shelve.GetRenderPos(0)->getRZ());
			modelStack.Scale(shelve.GetRenderPos(0)->getScaleX(),shelve.GetRenderPos(0)->getScaleY(),shelve.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_SHELF], true);
			modelStack.PopMatrix();
		}
	}
}
/***************************************************************/
/*!
/brief
Codes to render the world objects.
*/
/***************************************************************/
void SP2::RenderWorld()
{	
	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(0)->getTranslationX(),data.GetRenderPos(0)->getTranslationY(),data.GetRenderPos(0)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(0)->getRotation(),data.GetRenderPos(0)->getRX(),data.GetRenderPos(0)->getRY(),data.GetRenderPos(0)->getRZ());
	modelStack.Scale(data.GetRenderPos(0)->getScaleX(),data.GetRenderPos(0)->getScaleY(),data.GetRenderPos(0)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_MART], true);

	modelStack.PushMatrix();
	modelStack.Translate(EntranceDoorSlide,data.GetRenderPos(1)->getTranslationY(),data.GetRenderPos(1)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(1)->getRotation(),data.GetRenderPos(1)->getRX(),data.GetRenderPos(1)->getRY(),data.GetRenderPos(1)->getRZ());
	modelStack.Scale(data.GetRenderPos(1)->getScaleX(),data.GetRenderPos(1)->getScaleY(),data.GetRenderPos(1)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_DOOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(ExitDoorSlide,data.GetRenderPos(2)->getTranslationY(),data.GetRenderPos(2)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(2)->getRotation(),data.GetRenderPos(2)->getRX(),data.GetRenderPos(2)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(2)->getScaleX(),data.GetRenderPos(2)->getScaleY(),data.GetRenderPos(2)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_DOOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(3)->getTranslationX(),data.GetRenderPos(3)->getTranslationY(),data.GetRenderPos(3)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(3)->getRotation(),data.GetRenderPos(3)->getRX(),data.GetRenderPos(3)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(3)->getScaleX(),data.GetRenderPos(3)->getScaleY(),data.GetRenderPos(3)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_FLOOR], true);
	modelStack.PopMatrix();

	//Left Staff Door
	modelStack.PushMatrix();
	modelStack.Translate(LeftStaffDoorSlide,data.GetRenderPos(4)->getTranslationY(),data.GetRenderPos(4)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(4)->getRotation(),data.GetRenderPos(4)->getRX(),data.GetRenderPos(4)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(4)->getScaleX(),data.GetRenderPos(4)->getScaleY(),data.GetRenderPos(4)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_STAFFDOOR], true);
	modelStack.PopMatrix();

	//Right Staff Door
	modelStack.PushMatrix();
	modelStack.Translate(RightStaffDoorSlide,data.GetRenderPos(5)->getTranslationY(),data.GetRenderPos(5)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(5)->getRotation(),data.GetRenderPos(5)->getRX(),data.GetRenderPos(5)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(5)->getScaleX(),data.GetRenderPos(5)->getScaleY(),data.GetRenderPos(5)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_STAFFDOOR], true);
	modelStack.PopMatrix();

		modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(6)->getTranslationX(),data.GetRenderPos(6)->getTranslationY(),data.GetRenderPos(6)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(6)->getRotation(),data.GetRenderPos(6)->getRX(),data.GetRenderPos(6)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(6)->getScaleX(),data.GetRenderPos(6)->getScaleY(),data.GetRenderPos(6)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_ROAD], true);
	modelStack.PopMatrix();

		modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(7)->getTranslationX(),data.GetRenderPos(7)->getTranslationY(),data.GetRenderPos(7)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(7)->getRotation(),data.GetRenderPos(7)->getRX(),data.GetRenderPos(7)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(7)->getScaleX(),data.GetRenderPos(7)->getScaleY(),data.GetRenderPos(7)->getScaleZ());
	if (EscapeEnd == true && ExplosionScale >= 100)
	{
	RenderMesh(meshList[GEO_MODEL_BROKEN], true);
	}
	else
	{
	RenderMesh(meshList[GEO_MODEL_FENCE], true);
	}
	modelStack.PopMatrix();

			modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(8)->getTranslationX(),ShutterDoorOpen,data.GetRenderPos(8)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(8)->getRotation(),data.GetRenderPos(8)->getRX(),data.GetRenderPos(8)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(8)->getScaleX(),data.GetRenderPos(8)->getScaleY(),data.GetRenderPos(8)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_SHUTTER], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(9)->getTranslationX(),data.GetRenderPos(9)->getTranslationY(),data.GetRenderPos(9)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(9)->getRotation(),data.GetRenderPos(9)->getRX(),data.GetRenderPos(9)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(9)->getScaleX(),data.GetRenderPos(9)->getScaleY(),data.GetRenderPos(9)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_CAMERA], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(10)->getTranslationX(),data.GetRenderPos(10)->getTranslationY(),data.GetRenderPos(10)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(10)->getRotation(),data.GetRenderPos(10)->getRX(),data.GetRenderPos(10)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(10)->getScaleX(),data.GetRenderPos(10)->getScaleY(),data.GetRenderPos(10)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_CAMERA], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(11)->getTranslationX(),data.GetRenderPos(11)->getTranslationY(),data.GetRenderPos(11)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(11)->getRotation(),data.GetRenderPos(11)->getRX(),data.GetRenderPos(11)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(11)->getScaleX(),data.GetRenderPos(11)->getScaleY(),data.GetRenderPos(11)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_BOARD1], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(12)->getTranslationX(),data.GetRenderPos(12)->getTranslationY(),data.GetRenderPos(12)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(12)->getRotation(),data.GetRenderPos(12)->getRX(),data.GetRenderPos(12)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(12)->getScaleX(),data.GetRenderPos(12)->getScaleY(),data.GetRenderPos(12)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_BOARD2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(13)->getTranslationX(),data.GetRenderPos(13)->getTranslationY(),data.GetRenderPos(13)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(13)->getRotation(),data.GetRenderPos(13)->getRX(),data.GetRenderPos(13)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(13)->getScaleX(),data.GetRenderPos(13)->getScaleY(),data.GetRenderPos(13)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_BOARD3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(14)->getTranslationX(),data.GetRenderPos(14)->getTranslationY(),data.GetRenderPos(14)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(14)->getRotation(),data.GetRenderPos(14)->getRX(),data.GetRenderPos(14)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(14)->getScaleX(),data.GetRenderPos(14)->getScaleY(),data.GetRenderPos(14)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_BOARD4], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(15)->getTranslationX(),data.GetRenderPos(15)->getTranslationY(),data.GetRenderPos(15)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(15)->getRotation(),data.GetRenderPos(15)->getRX(),data.GetRenderPos(15)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(15)->getScaleX(),data.GetRenderPos(15)->getScaleY(),data.GetRenderPos(15)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_LIGHTS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(16)->getTranslationX(),data.GetRenderPos(16)->getTranslationY(),data.GetRenderPos(16)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(16)->getRotation(),data.GetRenderPos(16)->getRX(),data.GetRenderPos(16)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(16)->getScaleX(),data.GetRenderPos(16)->getScaleY(),data.GetRenderPos(16)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_LIGHTS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(17)->getTranslationX(),data.GetRenderPos(17)->getTranslationY(),data.GetRenderPos(17)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(17)->getRotation(),data.GetRenderPos(17)->getRX(),data.GetRenderPos(17)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(17)->getScaleX(),data.GetRenderPos(17)->getScaleY(),data.GetRenderPos(17)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_LIGHTS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(18)->getTranslationX(),data.GetRenderPos(18)->getTranslationY(),data.GetRenderPos(18)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(18)->getRotation(),data.GetRenderPos(18)->getRX(),data.GetRenderPos(18)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(18)->getScaleX(),data.GetRenderPos(18)->getScaleY(),data.GetRenderPos(18)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_LIGHTS], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(19)->getTranslationX(),data.GetRenderPos(19)->getTranslationY(),data.GetRenderPos(19)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(19)->getRotation(),data.GetRenderPos(19)->getRX(),data.GetRenderPos(19)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(19)->getScaleX(),data.GetRenderPos(19)->getScaleY(),data.GetRenderPos(19)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_GUARDHOUSE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-5,0,-13.2);
	modelStack.Rotate(180,0,1,0);
	RenderCashier();
	RenderFridge();
	RenderShelves();
	RenderCharacter();
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(20)->getTranslationX(),data.GetRenderPos(20)->getTranslationY(),data.GetRenderPos(20)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(20)->getRotation(),data.GetRenderPos(20)->getRX(),data.GetRenderPos(20)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(20)->getScaleX(),data.GetRenderPos(10)->getScaleY(),data.GetRenderPos(20)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_GUARDCONTROL], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	RenderBuilding();
	modelStack.PopMatrix();
}
/***************************************************************/
/*!
/brief
Codes to render the Buildings outside the fence.
*/
/***************************************************************/
void SP2::RenderBuilding()
{
	for(int i = 0; i < Building01.ReturnReadListSize(); ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(Building01.GetRenderPos(i)->getTranslationX(),Building01.GetRenderPos(i)->getTranslationY(),Building01.GetRenderPos(i)->getTranslationZ());
		modelStack.Rotate(Building01.GetRenderPos(i)->getRotation(),Building01.GetRenderPos(i)->getRX(),Building01.GetRenderPos(i)->getRY(),Building01.GetRenderPos(i)->getRZ());
		modelStack.Scale(Building01.GetRenderPos(i)->getScaleX(),Building01.GetRenderPos(i)->getScaleY(),Building01.GetRenderPos(i)->getScaleZ());
		RenderMesh(meshList[GEO_BUILDING01],false);
		modelStack.PopMatrix();
	}
	for(int i = 0; i < Building02.ReturnReadListSize(); ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(Building02.GetRenderPos(i)->getTranslationX(),Building02.GetRenderPos(i)->getTranslationY(),Building02.GetRenderPos(i)->getTranslationZ());
		modelStack.Rotate(Building02.GetRenderPos(i)->getRotation(),Building02.GetRenderPos(i)->getRX(),Building02.GetRenderPos(i)->getRY(),Building02.GetRenderPos(i)->getRZ());
		modelStack.Scale(Building02.GetRenderPos(i)->getScaleX(),Building02.GetRenderPos(i)->getScaleY(),Building02.GetRenderPos(i)->getScaleZ());
		RenderMesh(meshList[GEO_BUILDING02],false);
		modelStack.PopMatrix();
	}
	for(int i = 0; i < Building03.ReturnReadListSize(); ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(Building03.GetRenderPos(i)->getTranslationX(),Building03.GetRenderPos(i)->getTranslationY(),Building03.GetRenderPos(i)->getTranslationZ());
		modelStack.Rotate(Building03.GetRenderPos(i)->getRotation(),Building03.GetRenderPos(i)->getRX(),Building03.GetRenderPos(i)->getRY(),Building03.GetRenderPos(i)->getRZ());
		modelStack.Scale(Building03.GetRenderPos(i)->getScaleX(),Building03.GetRenderPos(i)->getScaleY(),Building03.GetRenderPos(i)->getScaleZ());
		RenderMesh(meshList[GEO_BUILDING03],false);
		modelStack.PopMatrix();
	}
	for(int i = 0; i < Building04.ReturnReadListSize(); ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(Building04.GetRenderPos(i)->getTranslationX(),Building04.GetRenderPos(i)->getTranslationY(),Building04.GetRenderPos(i)->getTranslationZ());
		modelStack.Rotate(Building04.GetRenderPos(i)->getRotation(),Building04.GetRenderPos(i)->getRX(),Building04.GetRenderPos(i)->getRY(),Building04.GetRenderPos(i)->getRZ());
		modelStack.Scale(Building04.GetRenderPos(i)->getScaleX(),Building04.GetRenderPos(i)->getScaleY(),Building04.GetRenderPos(i)->getScaleZ());
		RenderMesh(meshList[GEO_BUILDING04],false);
		modelStack.PopMatrix();
	}
	for(int i = 0;i < Lamp.ReturnReadListSize(); ++i)
	{
		if(i < 4)
		{
			modelStack.PushMatrix();
			modelStack.Translate(Lamp.GetRenderPos(i)->getTranslationX(),Lamp.GetRenderPos(i)->getTranslationY(),Lamp.GetRenderPos(i)->getTranslationZ());
			modelStack.Rotate(Lamp.GetRenderPos(i)->getRotation(),Lamp.GetRenderPos(i)->getRX(),Lamp.GetRenderPos(i)->getRY(),Lamp.GetRenderPos(i)->getRZ());
			modelStack.Scale(Lamp.GetRenderPos(i)->getScaleX(),Lamp.GetRenderPos(i)->getScaleY(),Lamp.GetRenderPos(i)->getScaleZ());
			RenderMesh(meshList[GEO_LAMP],false);
			modelStack.PopMatrix();
		}
		else
		{
			modelStack.PushMatrix();
			modelStack.Translate(Lamp.GetRenderPos(i)->getTranslationX(),Lamp.GetRenderPos(i)->getTranslationY(),Lamp.GetRenderPos(i)->getTranslationZ());
			modelStack.Rotate(Lamp.GetRenderPos(i)->getRotation(),Lamp.GetRenderPos(i)->getRX(),Lamp.GetRenderPos(i)->getRY(),Lamp.GetRenderPos(i)->getRZ());
			modelStack.Scale(Lamp.GetRenderPos(i)->getScaleX(),Lamp.GetRenderPos(i)->getScaleY(),Lamp.GetRenderPos(i)->getScaleZ());
			RenderMesh(meshList[GEO_FLAG],false);
			modelStack.PopMatrix();
		}
	}
	
	for(int i = 0; i < Locker.ReturnReadListSize(); i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(Locker.GetRenderPos(i)->getTranslationX(),Locker.GetRenderPos(i)->getTranslationY(),Locker.GetRenderPos(i)->getTranslationZ());
		modelStack.Rotate(Locker.GetRenderPos(i)->getRotation(),Locker.GetRenderPos(i)->getRX(),Locker.GetRenderPos(i)->getRY(),Locker.GetRenderPos(i)->getRZ());
		modelStack.Scale(Locker.GetRenderPos(i)->getScaleX(),Locker.GetRenderPos(i)->getScaleY(),Locker.GetRenderPos(i)->getScaleZ());
		RenderMesh(meshList[GEO_LOCKER],false);
		modelStack.PopMatrix();
	}
}
/***************************************************************/
/*!
/brief
Codes to render the NPCs.
*/
/***************************************************************/
void SP2::RenderCharacter()
{
	modelStack.PushMatrix();
	modelStack.Translate(guard.getPosX(), character.GetRenderPos(1)->getTranslationY(), guard.getPosZ());
	modelStack.Rotate(guard.getRot(),character.GetRenderPos(1)->getRX(),character.GetRenderPos(1)->getRY(),character.GetRenderPos(1)->getRZ());
	modelStack.Scale(character.GetRenderPos(1)->getScaleX(),character.GetRenderPos(1)->getScaleY(),character.GetRenderPos(1)->getScaleZ());
	RenderAnimateGuard();
	modelStack.PopMatrix();

	//Render Shopper
	modelStack.PushMatrix();
	modelStack.Translate(shopper.getPosX(), character.GetRenderPos(2)->getTranslationY(), shopper.getPosZ());
	modelStack.Rotate(shopper.getRot(),character.GetRenderPos(2)->getRX(),character.GetRenderPos(2)->getRY(),character.GetRenderPos(2)->getRZ());
	modelStack.Scale(character.GetRenderPos(2)->getScaleX(),character.GetRenderPos(2)->getScaleY(),character.GetRenderPos(2)->getScaleZ());
	RenderAnimate();
	modelStack.PopMatrix();

	//Render Shopper 2
	modelStack.PushMatrix();
	modelStack.Translate(shopperTwo.getPosX(), character.GetRenderPos(3)->getTranslationY(), shopperTwo.getPosZ());
	modelStack.Rotate(shopperTwo.getRot(),character.GetRenderPos(3)->getRX(),character.GetRenderPos(3)->getRY(),character.GetRenderPos(3)->getRZ());
	modelStack.Scale(character.GetRenderPos(3)->getScaleX(),character.GetRenderPos(3)->getScaleY(),character.GetRenderPos(3)->getScaleZ());
	RenderAnimate2();
	modelStack.PopMatrix();

	//All Cashier personnel
	modelStack.PushMatrix();
	RenderCashierModel();
	modelStack.PopMatrix();
}
/***************************************************************/
/*!
/brief
Codes to render a guard NPC with walking animation
*/
/***************************************************************/
void SP2::RenderAnimateGuard()
{
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_MODEL_GUARD_HEAD], false);
	RenderMesh(meshList[GEO_MODEL_GUARD_BODY], false);

	modelStack.PushMatrix();
	modelStack.Translate(0, translateY - 0.1, -translate * 4);
	modelStack.Rotate(angle, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL_GUARD_LEFTARM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-3, translateY - 0.1, 0 + translate * 4);
	modelStack.Rotate(-angle, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL_GUARD_RIGHTARM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0.2, translate * 2);
	modelStack.Rotate(-angle, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL_GUARD_LEFTLEG], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(-1, 0.2, 0 - translate * 2);
	modelStack.Rotate(angle, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL_GUARD_RIGHTLEG], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}
/***************************************************************/
/*!
/brief
Codes to animate 1st shopper with walking animation
*/
/***************************************************************/
void SP2::RenderAnimate()
{
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_MODEL_HEAD], false);
	RenderMesh(meshList[GEO_MODEL_BODY], false);

	modelStack.PushMatrix();
	modelStack.Translate(0, translateY - 0.1, -translate * 4);
	modelStack.Rotate(angle, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL_LEFTARM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-3, translateY - 0.1, 0 + translate * 4);
	modelStack.Rotate(-angle, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL_RIGHTARM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0.2, translate * 2);
	modelStack.Rotate(-angle, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL_LEFTLEG], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(-1, 0.2, 0 - translate * 2);
	modelStack.Rotate(angle, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL_RIGHTLEG], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}
/***************************************************************/
/*!
/brief
Codes to animate 2nd shopper with walking animation
*/
/***************************************************************/
void SP2::RenderAnimate2()
{
	modelStack.PushMatrix();
	RenderMesh(meshList[GEO_MODEL_SHOPPER2_HEAD], false);
	RenderMesh(meshList[GEO_MODEL_SHOPPER2_BODY], false);

	modelStack.PushMatrix();
	modelStack.Translate(0, translateY - 0.1, -translate * 4);
	modelStack.Rotate(angle, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL_SHOPPER2_LEFTARM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-3, translateY - 0.1, 0 + translate * 4);
	modelStack.Rotate(-angle, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL_SHOPPER2_RIGHTARM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0.2, translate * 2);
	modelStack.Rotate(-angle, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL_SHOPPER2_LEFTLEG], false);
	modelStack.PopMatrix();
	
	modelStack.PushMatrix();
	modelStack.Translate(-1, 0.2, 0 - translate * 2);
	modelStack.Rotate(angle, 1, 0, 0);
	RenderMesh(meshList[GEO_MODEL_SHOPPER2_RIGHTLEG], false);
	modelStack.PopMatrix();

	modelStack.PopMatrix();
}
/***************************************************************/
/*!
/brief
Codes to render the player charater.
*/
/***************************************************************/
void SP2::RenderPlayer()
{
	if (police != true && EscapeEnd != true)
	{
	//Origin
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
	//Left Right to camera
	if ( player.trolley == false)
		modelStack.Rotate(DupeRotArms,0,1,0);
	else if ( player.trolley == true)
		modelStack.Rotate(DupeRot,0,1,0);
	modelStack.PushMatrix();
	//Up Down to camera
	if ( player.trolley == false)
		modelStack.Rotate(DupeRotArmsY,1,0,0);
	/*else if ( player.trolley == true)
		modelStack.Rotate(-60,1,0,0);*/
	//Right Arm
	modelStack.PushMatrix();
	modelStack.Translate(0.05, -0.2, -0.1);
	//modelStack.Translate(0.05, -0.2, -0.1);
	modelStack.Rotate(-10,0,1,0);
	modelStack.PushMatrix();
	modelStack.Rotate(ArmSwing,1,0,0);
	if ( player.trolley == false)
	RenderMesh(meshList[GEO_PLAYER_ARMSR], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	//left Arm
	modelStack.PushMatrix();
	modelStack.Translate(-0.05, -0.2, -0.1);
	modelStack.Rotate(10,0,1,0);
	modelStack.PushMatrix();
	if ( player.trolley == true)
		modelStack.Rotate(ArmSwing,1,0,0);
	else if ( player.trolley == false)
		modelStack.Rotate(0,1,0,0);
	//RenderMesh(meshList[GEO_PLAYER_ARMSL], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	}

	if ( police == true  && EscapeEnd != true && camera.downSight == false)
	{
		//Origin
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
	//Left Right to camera
	modelStack.Rotate(DupeRotArms,0,1,0);
	modelStack.PushMatrix();
	//Up Down to camera
	modelStack.Rotate(DupeRotArmsY,1,0,0);
	//Right Arm
	modelStack.PushMatrix();
	modelStack.Translate(0.07, -0.24, -0.25);
	//modelStack.Rotate(-10,0,1,0);
	modelStack.PushMatrix();
	modelStack.Translate(0.,GunOffset,0);
	modelStack.Rotate(GunSwing,1,0,0);
	modelStack.PushMatrix();
	//modelStack.Rotate(ArmSwing,1,0,0);
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(meshList[GEO_MODEL_RIFLE], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	}
	else if ( police == true  && EscapeEnd != true && camera.downSight == true)
	{
		//Origin
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
	//Left Right to camera
	modelStack.Rotate(DupeRotArms,0,1,0);
	modelStack.PushMatrix();
	//Up Down to camera
	modelStack.Rotate(DupeRotArmsY,1,0,0);
	//Right Arm
	modelStack.PushMatrix();
	modelStack.Translate(0.0, -0.20, -0.3);
	//modelStack.Translate(0, -0.6, -0.25);
	//modelStack.Rotate(-10,0,1,0);
	modelStack.PushMatrix();
	modelStack.Translate(0.,GunOffset,0);
	modelStack.Rotate(-5,1,0,0);
	modelStack.PushMatrix();
	//modelStack.Rotate(ArmSwing,1,0,0);
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(meshList[GEO_MODEL_RIFLE], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	}
}
/***************************************************************/
/*!
/brief
Codes to render the items.
*/
/***************************************************************/
void SP2::RenderFNB()
{
	for ( int i = 0; i < FNB.ReturnListSize();  ++i)
	{
		if (FNB.GetRenderPosItem(i)->getItemAvailability() == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(FNB.GetRenderPosItem(i)->getItemTranslationX(),FNB.GetRenderPosItem(i)->getItemTranslationY(),FNB.GetRenderPosItem(i)->getItemTranslationZ());
			modelStack.Rotate(FNB.GetRenderPosItem(i)->getItemRotation(),FNB.GetRenderPosItem(i)->getItemRX(),FNB.GetRenderPosItem(i)->getItemRY(),FNB.GetRenderPosItem(i)->getItemRZ());
			modelStack.Scale(FNB.GetRenderPosItem(i)->getItemScaleX(),FNB.GetRenderPosItem(i)->getItemScaleY(),FNB.GetRenderPosItem(i)->getItemScaleZ());
			if ( FNB.GetRenderPosItem(i)->getItemName() == "Pepsi")
			{
			RenderMesh(meshList[GEO_MODEL_PEPSI], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Coke")
			{
			RenderMesh(meshList[GEO_MODEL_COKE], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Pizza")
			{
			RenderMesh(meshList[GEO_MODEL_PIZZA], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "IceCream")
			{
			RenderMesh(meshList[GEO_MODEL_ICECREAM], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "McNCheese")
			{
			RenderMesh(meshList[GEO_MODEL_MCNCHEESE], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Toblerone")
			{
			RenderMesh(meshList[GEO_MODEL_TOBLERONE], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Rocher")
			{
			RenderMesh(meshList[GEO_MODEL_ROCHER], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "MtDew")
			{
			RenderMesh(meshList[GEO_MODEL_MTDEW], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Reditos")
			{
			RenderMesh(meshList[GEO_MODEL_REDITOS], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Dewitos")
			{
			RenderMesh(meshList[GEO_MODEL_DEWITOS], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Lays")
			{
			RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Cactus")
			{
			RenderMesh(meshList[GEO_MODEL_CACTUS], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Vegcan")
			{
			RenderMesh(meshList[GEO_MODEL_VEGCAN], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "PCan")
			{
			RenderMesh(meshList[GEO_MODEL_PUMPKINCAN], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "MOaties")
			{
			RenderMesh(meshList[GEO_MODEL_MOATIES], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "OBox")
			{
			RenderMesh(meshList[GEO_MODEL_CEREAL_2], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Chickenstock")
			{
			RenderMesh(meshList[GEO_MODEL_CHICKENSTOCK], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Pistol" && police == true)
			{
			RenderMesh(meshList[GEO_MODEL_PISTOL], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Rifle" && police == true)
			{
			RenderMesh(meshList[GEO_MODEL_RIFLE], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Milo")
			{
			RenderMesh(meshList[GEO_MODEL_MILO], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "SardCan")
			{
			RenderMesh(meshList[GEO_MODEL_SARDCAN], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "SoupCan")
			{
			RenderMesh(meshList[GEO_MODEL_TOMATOSOUPCAN], true);
			}
			modelStack.PopMatrix();
		}
	}
}
/***************************************************************/
/*!
/brief
Codes for an NPC to shop.
*/
/***************************************************************/
void SP2::NPCShop()
{
	//For shopper 1
	if(playeridle == false)
	{
		//shopper shop(scripted)
		if(shopper.getPosX() > -60 && shopper.getPosX() < -35 && shopper.getPosZ() <= -25)//getting 1 pepsi
		{
			for ( int i = 0; i < FNB.ReturnListSize();  ++i)
			{
				if (FNB.GetRenderPosItem(i)->getItemAvailability() == true)
				{
					if ( FNB.GetRenderPosItem(i)->getItemName() == "Pepsi" && shopper.maxItem == 0)
					{
						shopper.setShop(true);
						if(shopper.getRot() == 0)
						{
							FNB.GetRenderPosItem(i)->setItemAvailable(0);
							shopper.maxItem = 1;
						}
					}

				}
			}
		}
		if(shopper.getRot() == 0)//reset for next shop
		{
			shopper.setShop(false);
		}
		if(shopper.getPosX() > -25 && shopper.getPosX() < 0 && shopper.getPosZ() <= -25)//getting 1 coke
		{
			for ( int i = 0; i < FNB.ReturnListSize();  ++i)
			{
				if (FNB.GetRenderPosItem(i)->getItemAvailability() == true)
				{
					if ( FNB.GetRenderPosItem(i)->getItemName() == "Coke" && shopper.maxItem == 1)
					{
						shopper.setShop(true);
						if(shopper.getRot() == 0)
						{
							FNB.GetRenderPosItem(i)->setItemAvailable(0);
							shopper.maxItem = 2;
						}
					}

				}
			}
		}
		if(shopper.getRot() == 0)//reset for next shop
		{
			shopper.setShop(false);
		}
		if(shopper.getPosX() > 10 && shopper.getPosX() < 35 && shopper.getPosZ() <= -25)//getting 1 MtDew
		{
			for ( int i = 0; i < FNB.ReturnListSize();  ++i)
			{
				if (FNB.GetRenderPosItem(i)->getItemAvailability() == true)
				{
					if ( FNB.GetRenderPosItem(i)->getItemName() == "MtDew" && shopper.maxItem == 2)
					{
						shopper.setShop(true);
						if(shopper.getRot() == 0)
						{
							FNB.GetRenderPosItem(i)->setItemAvailable(0);
							shopper.maxItem = 3;
						}
					}

				}
			}
		}
		if(shopper.getRot() == 0)//reset for next shop
		{
			shopper.setShop(false);
		}
		if(shopper.getPosX() > -60 && shopper.getPosX() < -35 && shopper.getPosZ() <= -25)//getting 2nd pepsi
		{
			for ( int i = 0; i < FNB.ReturnListSize();  ++i)
			{
				if (FNB.GetRenderPosItem(i)->getItemAvailability() == true)
				{
					if ( FNB.GetRenderPosItem(i)->getItemName() == "Pepsi" && shopper.maxItem == 3)
					{
						shopper.setShop(true);
						if(shopper.getRot() == 0)
						{
							FNB.GetRenderPosItem(i)->setItemAvailable(0);
							shopper.maxItem = 4;
						}
					}

				}
			}
		}
		if(shopper.getRot() == 0)//reset for next shop
		{
			shopper.setShop(false);
		}
		if(shopper.maxItem == 4)
		{
			shopper.cashIn = true;
			shopper.Checkout();
		}
		// respawn AI
		if(shopper.getPosX() >= 150 )
		{
			shopper.respawn();
		}
		//open entrance door
		if(shopper.getPosZ() <= 60 && shopper.getPosZ() >= 30 && shopper.maxItem == 0)
		{
			shopper.enterMart = true;
		}
		if(shopper.getPosZ() < 30)
		{
			shopper.enterMart = false;
		}
	}
	//for shopper 2
	if(playeridle == false)
	{
		//shopperTwo shop(scripted)
		if(shopperTwo.getPosX() > -25 && shopperTwo.getPosX() < 0 && shopperTwo.getPosZ() <= -8)//getting 1 Vegcan
		{
			for ( int i = 0; i < FNB.ReturnListSize();  ++i)
			{
				if (FNB.GetRenderPosItem(i)->getItemAvailability() == true)
				{
					if ( FNB.GetRenderPosItem(i)->getItemName() == "Vegcan" && shopperTwo.maxItem == 0)
					{
						shopperTwo.setShop(true);
						if(shopperTwo.getRot() == 0)
						{
							FNB.GetRenderPosItem(i)->setItemAvailable(0);
							shopperTwo.maxItem = 1;
						}
					}

				}
			}
		}
		if(shopperTwo.getRot() == 0)//reset for next shop
		{
			shopperTwo.setShop(false);
		}
		if(shopperTwo.getPosX() > 0 && shopperTwo.getPosX() < 25 && shopperTwo.getPosZ() <= -8)//getting 1 PCan
		{
			for ( int i = 0; i < FNB.ReturnListSize();  ++i)
			{
				if (FNB.GetRenderPosItem(i)->getItemAvailability() == true)
				{
					if ( FNB.GetRenderPosItem(i)->getItemName() == "PCan" && shopperTwo.maxItem == 1)
					{
						shopperTwo.setShop(true);
						if(shopperTwo.getRot() == 0)
						{
							FNB.GetRenderPosItem(i)->setItemAvailable(0);
							shopperTwo.maxItem = 2;
						}
					}

				}
			}
		}
		if(shopperTwo.getRot() == 0)//reset for next shop
		{
			shopperTwo.setShop(false);
		}
		if(shopperTwo.getPosX() > -25 && shopperTwo.getPosX() < 0 && shopperTwo.getPosZ() <= -8)//getting 2nd VegCan
		{
			for ( int i = 0; i < FNB.ReturnListSize();  ++i)
			{
				if (FNB.GetRenderPosItem(i)->getItemAvailability() == true)
				{
					if ( FNB.GetRenderPosItem(i)->getItemName() == "Vegcan" && shopperTwo.maxItem == 2)
					{
						shopperTwo.setShop(true);
						if(shopperTwo.getRot() == 0)
						{
							FNB.GetRenderPosItem(i)->setItemAvailable(0);
							shopperTwo.maxItem = 3;
						}
					}

				}
			}
		}
		if(shopperTwo.getRot() == 0)//reset for next shop
		{
			shopperTwo.setShop(false);
		}
		if(shopperTwo.getPosX() > 0 && shopperTwo.getPosX() < 25 && shopperTwo.getPosZ() <= -8)//getting 2nd PCan
		{
			for ( int i = 0; i < FNB.ReturnListSize();  ++i)
			{
				if (FNB.GetRenderPosItem(i)->getItemAvailability() == true)
				{
					if ( FNB.GetRenderPosItem(i)->getItemName() == "PCan" && shopperTwo.maxItem == 3)
					{
						shopperTwo.setShop(true);
						if(shopperTwo.getRot() == 0)
						{
							FNB.GetRenderPosItem(i)->setItemAvailable(0);
							shopperTwo.maxItem = 4;
						}
					}

				}
			}
		}
		if(shopperTwo.getRot() == 0)//reset for next shop
		{
			shopperTwo.setShop(false);
		}
		if(shopperTwo.maxItem == 3)
		{
			shopperTwo.cashIn = true;
			shopperTwo.Checkout();
		}
		// respawn AI
		if(shopperTwo.getPosX() >= 150 )
		{
			shopperTwo.respawn();
		}
		//open entrance door
		if(shopperTwo.getPosZ() <= 60 && shopperTwo.getPosZ() >= 30 && shopperTwo.maxItem == 0)
		{
			shopperTwo.enterMart = true;
		}
		if(shopperTwo.getPosZ() < 30)
		{
			shopperTwo.enterMart = false;
		}
	}
}
/***************************************************************/
/*!
/brief
Codes for rendering meshes from the meshlist.
*/
/***************************************************************/
void SP2::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	//mesh->Render();

	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once 
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
/***************************************************************/
/*!
/brief
Codes for rendering text.
*/
/***************************************************************/
void SP2::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}
/***************************************************************/
/*!
/brief
Codes for rendering text on the screen
*/
/***************************************************************/
void SP2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;
	
	
	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f+1.5f, 1.0f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}
/***************************************************************/
/*!
/brief
Codes for rendering the UI
*/
/***************************************************************/
void SP2::RenderUI(Mesh* mesh, Color color, float sizeX,float sizeY,float sizeZ, float x, float y)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;
	
	
	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);	
	modelStack.Scale(sizeX, sizeY, sizeZ);

	RenderMesh(mesh, false);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}
/***************************************************************/
/*!
/brief
Codes to close the application and cleanup
*/
/***************************************************************/
void SP2::Exit()
{
	// Cleanup here
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i] != NULL)
			delete meshList[i];
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);

	MusicPlayer.~Csounds();
}

