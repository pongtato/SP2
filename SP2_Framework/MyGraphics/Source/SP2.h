#ifndef _SP2_H
#define _SP2_H

#include "Scene.h"
#include "Mtx44.h"
#include "Camera2.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Utility.h"
#include "ReadFromText.h"
#include "RenderPos.h"
#include "NPC.h"
#include "Item.h"
#include "Character.h"
#include<algorithm>

using namespace std;

class SP2: public Scene
{
private:
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_TYPE,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_POSITION,
		U_LIGHT1_TYPE,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHT2_POSITION,
		U_LIGHT2_TYPE,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		U_LIGHT3_POSITION,
		U_LIGHT3_TYPE,
		U_LIGHT3_COLOR,
		U_LIGHT3_POWER,
		U_LIGHT3_KC,
		U_LIGHT3_KL,
		U_LIGHT3_KQ,
		U_LIGHT3_SPOTDIRECTION,
		U_LIGHT3_COSCUTOFF,
		U_LIGHT3_COSINNER,
		U_LIGHT3_EXPONENT,

		U_LIGHT4_POSITION,
		U_LIGHT4_TYPE,
		U_LIGHT4_COLOR,
		U_LIGHT4_POWER,
		U_LIGHT4_KC,
		U_LIGHT4_KL,
		U_LIGHT4_KQ,
		U_LIGHT4_SPOTDIRECTION,
		U_LIGHT4_COSCUTOFF,
		U_LIGHT4_COSINNER,
		U_LIGHT4_EXPONENT,

		U_NUMLIGHTS,
		//add these enum in UNIFORM_TYPE before U_TOTAL
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,


		U_TOTAL,
	};

	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_QUAD,
		GEO_LIGHTBALL,
		GEO_LIGHTBALL2,
		GEO_CYLINDER,

		GEO_MODEL1,
		GEO_MODEL_DOORMAN,
		GEO_MODEL_MART,
		GEO_MODEL_DOOR,
		GEO_MODEL_FLOOR,
		GEO_MODEL_CASHIER,
		GEO_MODEL_CHAR1,
		GEO_MODEL_FRIDGE,
		GEO_MODEL_SHELF,
		GEO_MODEL_CEREAL_1,
		GEO_MODEL_CEREAL_2,
		GEO_MODEL_CEREAL_3,
		GEO_MODEL_WATERCAN,
		GEO_MODEL_MTDEW,
		GEO_MODEL_COKE,
		GEO_MODEL_PEPSI,
		GEO_MODEL_HERSHEY,
		GEO_MODEL_ROCHER,
		GEO_MODEL_BBCAN,
		GEO_MODEL_PUMPKINCAN,
		GEO_MODEL_TOMATOSOUPCAN,
		GEO_MODEL_PEASNCARROTCAN,
		GEO_MODEL_LAYSCHIPS,
		GEO_MODEL_PIZZA,
		GEO_MODEL_ICECREAM,
		GEO_MODEL_MCNCHEESE,
		GEO_MODEL_SARDINES,
		GEO_MODEL_AYAMCAN,
		GEO_MODEL_CHICKENSTOCK,
		GEO_MODEL_TOBLERONE,
		GEO_MODEL_VEGCAN,
		GEO_MODEL_MILO,
		GEO_MODEL_SARDCAN,
		GEO_MODEL_CACTUS,
		GEO_MODEL_REDITOS,
		GEO_MODEL_DEWITOS,
		GEO_MODEL_DIABETOS,
		GEO_MODEL_MOATIES,
		GEO_MODEL_SWBOX,
		GEO_MODEL_DOSH,
		GEO_MODEL_PISTOL,
		GEO_MODEL_RIFLE,
		GEO_TEXT,
		GEO_XHAIR,
		GEO_HP,
		GEO_STAM,
		GEO_HPI,
		GEO_STAMI,
		GEO_ALERT,
		GEO_UI,
		GEO_MODEL_CAMERA,
		GEO_MODEL_STAFFDOOR,
		GEO_MODEL_SHUTTER,
		GEO_MODEL_ROAD,
		GEO_MODEL_FENCE,
		GEO_MODEL_BOARD1,
		GEO_MODEL_BOARD2,
		GEO_MODEL_BOARD3,
		GEO_MODEL_BOARD4,
		GEO_MODEL_LIGHTS,
		GEO_MODEL_GUARDHOUSE,
		GEO_MODEL_GUARDCONTROL,



		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,

		NUM_GEOMETRY,
	};

public:
	SP2();
	~SP2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	CReadFromText data;
	CReadFromText shelve;
	CReadFromText cashier;
	CReadFromText fridge;
	CReadFromText character;
	CReadFromText FNB;

	CNPC npc;
	CNPC shopper;
	CNPC patroler;
	CCharacter player;

	bool police;
	int Lightswitch;

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float rotateAngle;
	float FPS;
	std::string FPS_count;
	std::string XPos;
	std::string ZPos;
	std::string Target;
	std::string HP;
	std::string STAM;
	std::string MONEY;
	std::string ItemName;
	std::string ShopTut;
	std::string GuardTut;
	std::string TheifTut;

	float EntranceDoorSlide;
	float ExitDoorSlide;
	float ShutterDoorOpen;
	float RightStaffDoorSlide;
	float LeftStaffDoorSlide;

	//Camera2 camera;
	Camera3 camera;
	Camera3 cameraDupe;

	MS modelStack, viewStack, projectionStack;

	Light lights[10];

	void RenderWorld();
	void RenderSkybox();
	void RenderCashier();
	void RenderFridge();
	void RenderShelves();
	void RenderCharacter();
	void RenderFNB();
	void RenderPlayer();
	void RenderScreenUI();

	void SetPrevPos(void);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderUI(Mesh* mesh, Color color, float sizeX,float sizeY,float sizeZ, float x, float y);
	void CheckItem();
	void CharacterCrouch();
	void DoorSlide();
	void ShutterOpen();
	void BoundsCheck();
	void UIupdates(double dt);
	void CheckOut();
	void NPCwalk();
	void UnpaidItems();
	void Stealing();
};

#endif