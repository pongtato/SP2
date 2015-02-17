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
		GEO_TEXT,

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
	CReadFromText cereal;
	CReadFromText pizza;
	CReadFromText icecream;
	CReadFromText mcncheese;
	CReadFromText sardines;
	CReadFromText ayamcan;
	CReadFromText chickenstock;
	CReadFromText toblerone;
	CReadFromText milo;
	CReadFromText vegcan;
	CReadFromText sardcan;
	CReadFromText cactus;
	CReadFromText reditos;
	CReadFromText dewitos;
	CReadFromText diabetos;
	CReadFromText moaties;

	CNPC npc;

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

	float EntranceDoorSlide;

	//Camera2 camera;
	Camera3 camera;

	MS modelStack, viewStack, projectionStack;

	Light lights[2];

	void RenderWorld();
	void RenderSkybox();
	void RenderCashier();
	void RenderFridge();
	void RenderShelves();
	void RenderCharacter();
	void RenderCereal();
	void RenderPizza();
	void RenderIceCream();
	void RenderMcNCheese();
	void RenderSardines();
	void RenderAyamCan();
	void RenderChickenStock();
	void RenderToblerone();
	void RenderMilo();
	void RenderVegCan();
	void RenderSardCan();
	void RenderCactus();
	void RenderReditos();
	void RenderDewitos();
	void RenderDiabetos();
	void RenderMOaties();

	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);

};

#endif