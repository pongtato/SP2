/****************************************************************************/
/*!
\file SP2.h
\Author SP2 Group 4
\par
\brief
Main Header File for SP2.
*/
/****************************************************************************/
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
#include "Trolley.h"
#include "CameraSecurity.h"
#include<algorithm>
#include "Bullet.h"
#include "CollisionBounds.h"
#include "Police.h"
#include <iomanip>
#include <time.h>
#include <stdlib.h>
#include "Csounds.h"

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
		//Character Parts
		//Basic Guy
		GEO_MODEL_HEAD,
		GEO_MODEL_BODY,
		GEO_MODEL_LEFTARM,
		GEO_MODEL_RIGHTARM,
		GEO_MODEL_LEFTLEG,
		GEO_MODEL_RIGHTLEG,
		//Shopper 2
		GEO_MODEL_SHOPPER2_HEAD,
		GEO_MODEL_SHOPPER2_BODY,
		GEO_MODEL_SHOPPER2_LEFTARM,
		GEO_MODEL_SHOPPER2_RIGHTARM,
		GEO_MODEL_SHOPPER2_LEFTLEG,
		GEO_MODEL_SHOPPER2_RIGHTLEG,
		//Cashier
		GEO_MODEL_CASHIER_HEAD,
		GEO_MODEL_CASHIER_BODY,
		GEO_MODEL_CASHIER_LEFTARM,
		GEO_MODEL_CASHIER_RIGHTARM,
		GEO_MODEL_CASHIER_LEFTLEG,
		GEO_MODEL_CASHIER_RIGHTLEG,
		//Security Guard
		GEO_MODEL_GUARD_HEAD,
		GEO_MODEL_GUARD_BODY,
		GEO_MODEL_GUARD_LEFTARM,
		GEO_MODEL_GUARD_RIGHTARM,
		GEO_MODEL_GUARD_LEFTLEG,
		GEO_MODEL_GUARD_RIGHTLEG,
		//Default Doorman
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
		GEO_MODEL_SUPPLIES,
		GEO_MODEL_SFFOOD,
		GEO_MODEL_SCHIPS,
		GEO_MODEL_SCANS,
		GEO_MODEL_SCVEG,
		GEO_MODEL_SCEREAL,
		GEO_MODEL_SGUNS,
		GEO_MODEL_STFOOD,
		GEO_TEXT,
		GEO_XHAIR,
		GEO_HP,
		GEO_STAM,
		GEO_HPI,
		GEO_STAMI,
		GEO_ALERT,
		GEO_UI,
		GEO_DIALOGUEBOX,
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
		GEO_MODEL_TROLLEY,
		GEO_MENUBOX,
		GEO_MENUSELECT,
		GEO_PLAYER_ARMSL,
		GEO_PLAYER_ARMSR,
		GEO_MODEL_BULLET,
		GEO_MODEL_POLICEVAN,
		GEO_MODEL_POLICEVANWHEEL,
		GEO_MODEL_POLICEMAN,
		GEO_MODEL_POLICEMAN_DAMAGED,
		GEO_MODEL_BROKEN,
		GEO_EXPLODE,
		GEO_STARTBTN,
		GEO_INSTRUCTIONBTN,
		GEO_ACHIEVEBTN,
		GEO_EXITBTN,
		GEO_LOGO,
		GEO_TINT,
		GEO_ENDSCREEN,
		GEO_WINSCREEN,
		GEO_ACHIEVESCREEN,
		GEO_INSTRUCTIONSSCREEN,
		GEO_MODEL_ESCAPE,
		GEO_MAIN_INSTRUCT,
		GEO_PINSTRUCTBOX,
		GEO_SINSTRUCTBOX,

		GEO_BUILDING01,
		GEO_BUILDING02,
		GEO_BUILDING03,
		GEO_BUILDING04,
		GEO_LAMP,
		GEO_FLAG,
		GEO_MISSINGITEM,
		GEO_LOCKER,

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
	CReadFromText cashiermodel;
	CReadFromText FNB;
	CReadFromText Police;
	CReadFromText PoliceCol;
	CReadFromText PoliceMan;
	CReadFromText Building01;
	CReadFromText Building02;
	CReadFromText Building03;
	CReadFromText Building04;
	CReadFromText Lamp;
	CReadFromText Locker;
	vector<CBullet*> bullet;
	vector<CBullet*> Policebullet;
	CBullet shoot;
	CBullet Trolleyz;

	vector<CCollisionBounds*> colBounds;
	vector<CCollisionBounds*> VanBounds;
	vector<CCollisionBounds*> LockerBounds;
	


	CNPC npc;
	CNPC guard;
	CNPC shopper;
	CNPC shopperTwo;
	CNPC patroler;
	CCharacter player;
	CTrolley trolley;

	CPolice police1;
	CPolice police2;
	CPolice police3;
	CPolice police4;
	CPolice Escape;


	bool police;
	int Lightswitch;
	bool CheckoutActive;
	float EscapeCarMove;
	bool EscapeEnd;
	float ExplosionScale;
	int MenuState;
	float MenuLimit;
	float GunOffset;
	float GunSwing;
	float DupeRot;
	float DupeRotArms;
	float DupeRotArmsY;
	Csounds MusicPlayer;
	int RandomItem;

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float rotateAngle;
	float FPS;
	float angle;
	float translate;
	float translateY;
	bool walk;
	bool playeridle;
	bool NPC1idle;
	bool NPC2idle;
	bool NPC3idle;
	bool paying;
	bool paid;
	bool ShopStaff;
	bool GameEndStaff;
	bool GameEndSteal;
	bool GameEndStealLose;
	double DTimer;
	string DTtime;
	bool UpdateItemMissing;
	string missinglist;
	string nameofitem[10];
	int noofitem[10];
	float CashierOffetX;
	float CashierOffetY;
	float CashierOffetZ;
	float PoliceInstructTimer;
	float StaffInstructTimer;
	bool PInstruct;
	bool SInstruct;
	bool Instruct;
	std::string FPS_count;
	std::string XPos;
	std::string ZPos;
	std::string Target;
	std::string HP;
	std::string STAM;
	std::string MONEY;
	std::string ROLE;
	std::string INSTRUCTIONS1;
	std::string INSTRUCTIONS2;
	std::string INSTRUCTIONS3;
	std::string INSTRUCTIONS4;
	std::string TITLE;
	std::string ItemName;
	std::string BulletCount;
	std::string ItemPrice;
	std::string TheifTut;
	std::string ShopTut;
	std::string GuardTut;
	std::string EndGame;
	std::string EndGameSteal;
	std::string EndGameStealL;

	float EntranceDoorSlide;
	float ExitDoorSlide;
	float ShutterDoorOpen;
	float RightStaffDoorSlide;
	float LeftStaffDoorSlide;
	float ArmSwing;
	bool MenuKey;
	void EscapeSteal();
	float LengthY;

	//Camera2 camera;
	Camera3 camera;
	CCameraSecurity EndScene;
	CCameraSecurity Security1;
	CCameraSecurity Security2;
	Camera3 cameraDupe;

	MS modelStack, viewStack, projectionStack;

	Light lights[10];

	void RenderWorld();
	void RenderSkybox();
	void RenderCashier();
	void RenderCashierModel();
	void RenderFridge();
	void RenderShelves();
	void RenderCharacter();
	void RenderFNB();
	void RenderPlayer();
	void RenderScreenUI();
	void RenderItemMissing();
	void RenderAnimate();
	void RenderAnimate2();
	void RenderAnimateGuard();
	void RenderPolice();
	void PoliceShoot(double dt);
	void RenderBuilding();
	void ItemMissing();

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
	void BoundsCheckTrolley(float x,float y, float z);
	void UIupdates(double dt);
	void Animate(double dt);
	void CheckOut();
	void NPCwalk();
	void UnpaidItems();
	void Stealing();
	void RenderBasicModel();
	void Trolley();
	void TrolleyUpdate();
	void NPCShop();
	bool BulletCollision(float x,float y,float z,double dt);
	bool BulletCollisionEnemy(float x,float y,float z);
	void MainMenu();
	void MenuUpdate(double dt);
	void LightsReset();
};

#endif