#include "SP2.h"
#include "GL\glew.h"
#include <sstream>

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "Vertex.h"

#include "LoadTGA.h"


SP2::SP2()
{
}

SP2::~SP2()
{
}

void SP2::Init()
{

	data.ReadTextFile( "OBJ-Pos/Martpos.txt" );
	shelve.ReadTextFile( "OBJ-Pos/Shelves.txt" );
	cashier.ReadTextFile( "OBJ-Pos/Cashierpos.txt" );
	fridge.ReadTextFile( "OBJ-Pos/Fridgepos.txt" );
	character.ReadTextFile( "OBJ-Pos/NPC/Characterpos.txt" );
	shopper.ReadTextFilePath( "OBJ-Pos/NPC/CharacterPath.txt");
	patroler.ReadTextFilePath( "OBJ-Pos/NPC/CharacterPath.txt");
	FNB.ReadTextFileItem( "OBJ-Pos/Food/Cans/FNB.txt");

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

	//variable to rotate geometry
	rotateAngle = 0;

	//Initialize camera settings
	camera.Init(Vector3(0, 6, 100), Vector3(0, 0, 0), Vector3(0, 1, 0));
	
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
			lights[1].position.Set(0, 20, 15);
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
			lights[2].position.Set(0, 20, -15);
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
			lights[3].position.Set(-35, 20, 15);
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
			lights[4].position.Set(-35, 20, -15);
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

	meshList[GEO_STAM] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_STAM]->textureID = LoadTGA("Image//stamina.tga");

	meshList[GEO_HPI] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_HPI]->textureID = LoadTGA("Image//hpicon.tga");

	meshList[GEO_STAMI] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_STAMI]->textureID = LoadTGA("Image//staminaicon.tga");

	meshList[GEO_UI] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_UI]->textureID = LoadTGA("Image//UI.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//()Town_bk.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//()Town_ft.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//()Town_dn.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//()Town2_Up.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//()Town_rt.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//()Town_lf.tga");

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
	meshList[GEO_MODEL_RIFLE]->textureID = LoadTGA("Image//DOOR2.tga");

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

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//comic.tga");

	EntranceDoorSlide = data.GetRenderPos(1)->getTranslationX();
	ExitDoorSlide = data.GetRenderPos(2)->getTranslationX();
	npc.setName(character.GetRenderPos(0)->getName());
	npc.setPosX(character.GetRenderPos(0)->getTranslationX());
	npc.setPosZ(character.GetRenderPos(0)->getTranslationZ());
	shopper.setName(character.GetRenderPos(1)->getName());
	shopper.setPosX(character.GetRenderPos(1)->getTranslationX());
	shopper.setPosZ(character.GetRenderPos(1)->getTranslationZ());
	patroler.setName(character.GetRenderPos(2)->getName());
	patroler.setPosX(character.GetRenderPos(2)->getTranslationX());
	patroler.setPosZ(character.GetRenderPos(2)->getTranslationZ());
}



static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;
bool Lightsssss = false;

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
	if(Application::IsKeyPressed('5'))
	{
		Vector3 getOrigin;
		getOrigin.x = data.GetRenderPos(0)->getTranslationX();
		getOrigin.y = data.GetRenderPos(0)->getTranslationY();
		getOrigin.z = data.GetRenderPos(0)->getTranslationZ();
		camera.setCameraState(1);
		float tempX = data.GetRenderPos(10)->getTranslationX();
		float tempY = data.GetRenderPos(10)->getTranslationY();
		float tempZ = data.GetRenderPos(10)->getTranslationZ();
		camera.target.x = 0;
		camera.target.y = 1;
		camera.target.z = 0;
		camera.up.y = 0.98;
		camera.up.x = 0.11;
		camera.up.z = -0.07;
		camera.position.y = getOrigin.y + tempY;
		camera.position.x = getOrigin.x - tempX + 1;
		camera.position.z = getOrigin.z - tempZ - 1;
	}

	if(Application::IsKeyPressed('6'))
	{
		Vector3 getOrigin;
		getOrigin.x = data.GetRenderPos(0)->getTranslationX();
		getOrigin.y = data.GetRenderPos(0)->getTranslationY();
		getOrigin.z = data.GetRenderPos(0)->getTranslationZ();
		camera.setCameraState(1);
		float tempX = data.GetRenderPos(9)->getTranslationX();
		float tempY = data.GetRenderPos(9)->getTranslationY();
		float tempZ = data.GetRenderPos(9)->getTranslationZ();
		camera.target.x = 0;
		camera.target.y = 0;
		camera.target.z = 0;
		camera.up.y = 0.98;
		camera.up.x = 0.11;
		camera.up.z = 0.07;
		
		camera.position.y = getOrigin.y + tempY;
		camera.position.x = getOrigin.x - tempX +1;
		camera.position.z = getOrigin.z - tempZ +1;
	}
	if(Application::IsKeyPressed('7'))
	{
		camera.position = cameraDupe.position;
		camera.target = cameraDupe.target;
		camera.up = cameraDupe.up;
		camera.setCameraState(0);
	}

	CheckItem();
	DoorSlide();
	NPCwalk();

	if (camera.getCameraState() == 0)
	{
	CharacterCrouch();
	UIupdates(dt);
	BoundsCheck();
	SetPrevPos();
	}
	camera.Update(dt);


}

void SP2::NPCwalk()
{
	npc.TestAIPath();
	shopper.ShopPathing(0);
	patroler.ShopPathing(1);
}

void SP2::UIupdates(double dt)
{
	std::stringstream dd;
	dd << camera.position.x;
	XPos = dd.str();

	std::stringstream ff;
	ff << camera.position.z;
	ZPos = ff.str();

	FPS = 1/dt;
	std::stringstream ss;
	ss << FPS;
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
}

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
		}
}

void SP2::SetPrevPos()
{
	cameraDupe.position = camera.position;
	cameraDupe.target = camera.target;
	cameraDupe.up = camera.up;
}

void SP2::BoundsCheck()
{
	Vector3 tempMart;
	tempMart.x = data.GetRenderPos(0)->getTranslationX();
	tempMart.z = data.GetRenderPos(0)->getTranslationZ();

	//fence check
	//leftfence
	if( camera.position.x <= data.GetRenderPos(7)->getTranslationX()-100)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
	}
	//right fence
	if( camera.position.x >= data.GetRenderPos(7)->getTranslationX()+90)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
	}
	if( camera.position.z <= data.GetRenderPos(7)->getTranslationZ()-205)
	{
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
	}
	if( camera.position.z >= data.GetRenderPos(7)->getTranslationZ()+180)
	{
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
	}
	


	////mart outer bound check
	////left wall
	if( camera.position.x >= tempMart.x - 78 && camera.position.x <= tempMart.x - 71 && camera.position.z <= tempMart.z + 54 && camera.position.z >= tempMart.z - 54)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
	}
	////Back Wall Left
	if( camera.position.x >= tempMart.x -78 && camera.position.x <= tempMart.x - 14 && camera.position.z <= tempMart.z + -45  && camera.position.z >= tempMart.z - 54)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
	}

	////Back Wall Right
	if( camera.position.x >= tempMart.x + 14 && camera.position.x <= tempMart.x + 78 && camera.position.z <= tempMart.z + -45  && camera.position.z >= tempMart.z - 54)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
	}

	////Right Wall
	if( camera.position.x >= tempMart.x + 71 && camera.position.x <= tempMart.x + 78 && camera.position.z <= tempMart.z + 54  && camera.position.z >= tempMart.z - 54)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
	}

	////Right Wall
	if( camera.position.x >= tempMart.x - 62 && camera.position.x <= tempMart.x + 62 && camera.position.z <= tempMart.z + 54  && camera.position.z >= tempMart.z + 46)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
	}

	////Middle Wall
	if( camera.position.x >= tempMart.x - 62 && camera.position.x <= tempMart.x + 62 && camera.position.z <= tempMart.z - 22  && camera.position.z >= tempMart.z  - 30)
	{
		camera.position.x = cameraDupe.position.x;
		camera.target.x = cameraDupe.target.x;
		camera.position.z = cameraDupe.position.z;
		camera.target.z = cameraDupe.target.z;
	}
}

void SP2::DoorSlide()
{
		if ( camera.EntranceDoor == true)
	{
		if ( EntranceDoorSlide >= 53)
		{
				EntranceDoorSlide-= 0.4;
		}
	}
	else if ( camera.EntranceDoor == false)
	{
		if ( EntranceDoorSlide <= 66.4)
		{
				EntranceDoorSlide+= 0.4;
		}
	}
	

	if ( camera.ExitDoor == true)
	{
		if ( ExitDoorSlide <= -53)
		{
				ExitDoorSlide+= 0.4;
		}
	}
	else if ( camera.ExitDoor == false)
	{
		if ( ExitDoorSlide >= -66.4)
		{
				ExitDoorSlide-= 0.4;
		}
	}
}

void SP2::Render()
{
	//bool toggleLight = false;

	//clear depth and color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Temp variables
	Mtx44 MVP;

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
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

	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(lights[1].position.x, lights[1].position.y, lights[1].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL2], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(lights[2].position.x, lights[2].position.y, lights[2].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL2], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(lights[3].position.x, lights[3].position.y, lights[3].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL2], false);
	modelStack.PopMatrix();

	RenderMesh(meshList[GEO_AXES], false);

	RenderWorld();
	RenderPlayer();
	RenderSkybox();
	RenderFNB();
	if ( camera.getCameraState() == 0 )
		RenderScreenUI();
}

void SP2::RenderScreenUI()
{
	double tempStamCalc = camera.getStaminaDupe();
	double Calc = (tempStamCalc/200) * 54 ;

	RenderTextOnScreen(meshList[GEO_TEXT], "FPS: " , Color(0, 1, 0), 3, 14, 18);
	RenderTextOnScreen(meshList[GEO_TEXT], FPS_count , Color(0, 1, 0), 3, 18, 18);

	RenderTextOnScreen(meshList[GEO_TEXT], XPos , Color(0, 1, 0), 3, 0, 18);
	RenderTextOnScreen(meshList[GEO_TEXT], ZPos , Color(0, 1, 0), 3, 0, 17);

	//RenderTextOnScreen(meshList[GEO_TEXT], Target, Color(0, 1, 0), 2, 0, 16);
	RenderUI(meshList[GEO_XHAIR], Color(0, 1, 0), 15, 15, 15, 40, 34.25);
	RenderUI(meshList[GEO_HP], Color(0, 1, 0), 54, 1.5, 15, 12.5, 9);
	RenderUI(meshList[GEO_STAM], Color(0, 1, 0), Calc, 1.5, 15, 45, 9);
	RenderUI(meshList[GEO_HPI], Color(0, 1, 0), 4, 4, 4, 12, 9);
	RenderUI(meshList[GEO_STAMI], Color(0, 1, 0), 4, 4, 4, 45, 9);
	//RenderTextOnScreen(meshList[GEO_TEXT], HP, Color(1, 1, 1), 4, 3.2, 1.5);
	//RenderTextOnScreen(meshList[GEO_TEXT], STAM, Color(1, 1, 1), 4, 12.2, 1.5);
	//RenderUI(meshList[GEO_ALERT], Color(0, 1, 0), 15, 2.5, 15,1, 1);
	for ( int i = 0; i < player.getInventoryCap() ; ++i)
	{
	RenderUI(meshList[GEO_UI], Color(0, 1, 0), 5, 5, 5,15+(i*5.5), 4);
	}

	for ( int i = 0; i < player.returnInvenSize(); ++i)
	{
		if ( player.returnInvenSize() != 0 )
		{
			if ( player.getInventory(i)->getItemName() == "Coke")
			{
				RenderUI(meshList[GEO_MODEL_COKE], Color(0, 1, 0), 5, 5, 5,15+(i*5.5), 4);
			}
			else if ( player.getInventory(i)->getItemName() == "Pepsi")
			{
				RenderUI(meshList[GEO_MODEL_PEPSI], Color(0, 1, 0), 5, 5, 5,15+(i*5.5), 4);
			}
		}
	}
}

void SP2::CheckItem()
{
	if(Application::IsKeyPressed('E'))
	{
		for ( int i = 0; i < FNB.ReturnListSize();  ++i)
		{
			if ( camera.target.x >= FNB.GetRenderPosItem(i)->getItemTranslationX()-0.2 && camera.target.x <= FNB.GetRenderPosItem(i)->getItemTranslationX()+0.2 
				&& camera.target.y >= FNB.GetRenderPosItem(i)->getItemTranslationY()-1 && camera.target.y <= FNB.GetRenderPosItem(i)->getItemTranslationY()+1 
				&& camera.target.z >= FNB.GetRenderPosItem(i)->getItemTranslationZ()-1 && camera.target.z <= FNB.GetRenderPosItem(i)->getItemTranslationZ()+1)
			{
				if (player.returnInvenSize() < 5 && FNB.GetRenderPosItem(i)->getItemAvailability() == true)
				{
					FNB.GetRenderPosItem(i)->setItemAvailable(0);
					player.setInventory(FNB.GetRenderPosItem(i)->getItemName(),FNB.GetRenderPosItem(i)->getItemPrice());
					cout << player.returnInvenSize()<< endl;

					for (int i = 0; i < player.returnInvenSize(); ++i)
					{
						if (player.returnInvenSize() != 0 )
						{
							cout << i + 1 << player.getInventory(i)->getItemName() << endl;
							cout << i + 1 << player.getInventory(i)->getItemPrice() << endl;
						}
					}
				}
			}
		}
	}
	if(Application::IsKeyPressed('G'))
	{
		for ( int i = 0; i < FNB.ReturnListSize();  ++i)
		{
			if ( camera.target.x >= FNB.GetRenderPosItem(i)->getItemTranslationX()-0.2 && camera.target.x <= FNB.GetRenderPosItem(i)->getItemTranslationX()+0.2 
				&& camera.target.y >= FNB.GetRenderPosItem(i)->getItemTranslationY()-1 && camera.target.y <= FNB.GetRenderPosItem(i)->getItemTranslationY()+1 
				&& camera.target.z >= FNB.GetRenderPosItem(i)->getItemTranslationZ()-1 && camera.target.z <= FNB.GetRenderPosItem(i)->getItemTranslationZ()+1)
			{
				if (player.returnInvenSize() > 0 && FNB.GetRenderPosItem(i)->getItemAvailability() == false)
				{
					FNB.GetRenderPosItem(i)->setItemAvailable(1);
					//system("CLS");
					player.dropItem(FNB.GetRenderPosItem(i)->getItemName());
					cout << "Deleted "<< endl;
					cout << player.returnInvenSize()<< endl;
					/*for (int i = 0; i < player.returnInvenSize(); ++i)
					{
						if (player.returnInvenSize() != 0 )
						{
							cout << i + 1 << player.getInventory(i)->getItemName() << endl;
							cout << i + 1 << player.getInventory(i)->getItemPrice() << endl;
						}
					}*/
				}
			}
		}
	}
}

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

void SP2::RenderCashier()
{
		for(int i = 0; i < 4; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(cashier.GetRenderPos(0)->getTranslationX(),cashier.GetRenderPos(0)->getTranslationY(),cashier.GetRenderPos(0)->getTranslationZ()+(i*16));
		modelStack.Rotate(cashier.GetRenderPos(0)->getRotation(),cashier.GetRenderPos(0)->getRX(),cashier.GetRenderPos(0)->getRY(),cashier.GetRenderPos(0)->getRZ());
		modelStack.Scale(cashier.GetRenderPos(0)->getScaleX(),cashier.GetRenderPos(0)->getScaleY(),cashier.GetRenderPos(0)->getScaleZ());
		RenderMesh(meshList[GEO_MODEL_CASHIER], true);
		modelStack.PopMatrix();
	}
}

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

	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(4)->getTranslationX(),data.GetRenderPos(4)->getTranslationY(),data.GetRenderPos(4)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(4)->getRotation(),data.GetRenderPos(4)->getRX(),data.GetRenderPos(4)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(4)->getScaleX(),data.GetRenderPos(4)->getScaleY(),data.GetRenderPos(4)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_STAFFDOOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(5)->getTranslationX(),data.GetRenderPos(5)->getTranslationY(),data.GetRenderPos(5)->getTranslationZ());
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
	RenderMesh(meshList[GEO_MODEL_FENCE], true);
	modelStack.PopMatrix();

			modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(8)->getTranslationX(),data.GetRenderPos(8)->getTranslationY(),data.GetRenderPos(8)->getTranslationZ());
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
	modelStack.Translate(-5,0,-13.2);
	modelStack.Rotate(180,0,1,0);
	RenderCashier();
	RenderFridge();
	RenderShelves();
	RenderCharacter();
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void SP2::RenderCharacter()
{
	//Bob
	modelStack.PushMatrix();
	modelStack.Translate(npc.getPosX(),character.GetRenderPos(0)->getTranslationY(),npc.getPosZ());
	modelStack.Rotate(npc.getRot(),character.GetRenderPos(0)->getRX(),character.GetRenderPos(0)->getRY(),character.GetRenderPos(0)->getRZ());
	modelStack.Scale(character.GetRenderPos(0)->getScaleX(),character.GetRenderPos(0)->getScaleY(),character.GetRenderPos(0)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_DOORMAN], true);
	modelStack.PopMatrix();
	//Dan
	modelStack.PushMatrix();
	modelStack.Translate(shopper.getPosX(),character.GetRenderPos(1)->getTranslationY(),shopper.getPosZ());
	modelStack.Rotate(shopper.getRot(),character.GetRenderPos(1)->getRX(),character.GetRenderPos(1)->getRY(),character.GetRenderPos(1)->getRZ());
	modelStack.Scale(character.GetRenderPos(1)->getScaleX(),character.GetRenderPos(1)->getScaleY(),character.GetRenderPos(1)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_DOORMAN], true);
	modelStack.PopMatrix();
	//Tom
	modelStack.PushMatrix();
	modelStack.Translate(patroler.getPosX(),character.GetRenderPos(2)->getTranslationY(),patroler.getPosZ());
	modelStack.Rotate(patroler.getRot(),character.GetRenderPos(2)->getRX(),character.GetRenderPos(2)->getRY(),character.GetRenderPos(2)->getRZ());
	modelStack.Scale(character.GetRenderPos(2)->getScaleX(),character.GetRenderPos(2)->getScaleY(),character.GetRenderPos(2)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_DOORMAN], true);
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(character.GetRenderPos(0)->getTranslationX(),character.GetRenderPos(0)->getTranslationY(),character.GetRenderPos(0)->getTranslationZ());
	//modelStack.Rotate(character.GetRenderPos(0)->getRotation(),character.GetRenderPos(0)->getRX(),character.GetRenderPos(0)->getRY(),character.GetRenderPos(0)->getRZ());
	//modelStack.Scale(character.GetRenderPos(0)->getScaleX(),character.GetRenderPos(0)->getScaleY(),character.GetRenderPos(0)->getScaleZ());
	//RenderMesh(meshList[GEO_MODEL_CHAR1], true);
	//modelStack.PopMatrix();
}

void SP2::RenderPlayer()
{

}

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
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Cereal1")
			{
			RenderMesh(meshList[GEO_MODEL_MOATIES], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Cereal2")
			{
			RenderMesh(meshList[GEO_MODEL_CEREAL_2], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Cereal3")
			{
			RenderMesh(meshList[GEO_MODEL_CHICKENSTOCK], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Pistol")
			{
			RenderMesh(meshList[GEO_MODEL_PISTOL], true);
			}
			else if ( FNB.GetRenderPosItem(i)->getItemName() == "Rifle")
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
}

