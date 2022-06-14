////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "Collision.h"
#include "TimerClass.h"
#include "FpsClass.h"
#include "TextClass.h"
#include "SkyBox.h"
#include "textureshaderclass.h"
#include "SoundClass.h"
#include "Billboard.h"
#include "bitmapclass.h"
#include "ParticleSystem.h"
#include "ParticleShaderClass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public :
	enum EStage {Title, Main, Clear, Over};

	EStage GetStage() { return m_stage; }
	void SetStage(EStage stage) { m_stage = stage; }

public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int, float);

	void adMoveMent(float);
	void Shooting();

	float GetCamYaw() { return m_Camera->GetCamYaw(); }
	float GetCamPitch() { return m_Camera->GetCamPitch(); }

	void CameraReset() { m_Camera->Reset(); }
	void SetCamYaw(float yaw) { m_Camera->SetCamYaw(yaw); }
	void SetCamPitch(float pitch) { m_Camera->SetCamPitch(pitch); }
	void SetMoveLeftRight(float LR) { m_Camera->SetMoveLeftRight(LR); }
	void SetMoveBackForward(float BF) { m_Camera->SetMoveBackForward(BF); }

	void ChangeFPmode();
	bool GetFPMode() { return m_fpMode; }

	bool GameOver() { return gameOver; }
	bool GameClear() { return gameClear; }

	void AddTitleNum();
	void Reset();

private:
	bool Render();

	bool LightShaderRender(ModelClass*, XMMATRIX, XMMATRIX, XMMATRIX);
	bool SetLamp();
	bool SetWall();
	bool SetTree();

	void SpawnZombie();

	bool ZombieRender();
	bool CannonRender();

	void CheckCollision();
	void ShootCoolTime();
	void SpawnCoolTime();

	void SetObjPolyCount();
	bool UIFrame(int, int, float);
	bool UIRender(int, int, float);

	void Timer(float);

private:
	float coolTime;
	float spawnTime;

	D3DClass* m_D3D;
	CameraClass* m_Camera;

	ModelClass* m_Player;
	Collision* m_PlayerColl;
	XMFLOAT3 m_PlayerPos;

	//ModelClass* m_SkyBox;
	SkyBox* m_SkyBox;

	ModelClass* m_Plane;
	ModelClass* m_StreetLamp;
	
	ModelClass* m_Wall;
	Collision* m_WallColl;

	bool DontSpawn;
	ModelClass* m_Zombie;
	bool m_ZombieSurivival[10];
	Collision* m_ZombieColl[10];
	XMFLOAT3 m_ZombiePos[10];

	bool DontShoot;
	ModelClass* m_Cannon;
	bool m_CannonSurvival[2];
	Collision* m_CannonColl[2];
	XMFLOAT3 m_CannonPos[2];

	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	TextClass* m_Text;
	FpsClass* m_FPS;

	TextureShaderClass* m_TextureShader;

	SoundClass* m_BGM;
	SoundClass* m_AttackSound;
	SoundClass* m_GameClearSound;
	SoundClass* m_GameOverSound;
	SoundClass* m_AttackedSound;
	SoundClass* m_MonsterDeadSound;

	Billboard* m_Tree[4];

	BitmapClass* m_Title[3];
	BitmapClass* m_GameClear;
	BitmapClass* m_GameOver;

	ParticleShaderClass* m_ParticleShader = nullptr;
	ParticleSystemClass* m_ParticleSystem = nullptr;

private:
	EStage m_stage;
	int titleNum;

	bool m_fpMode;

	int hp;

	float timer;
	int time_fps;

	bool gameOver;
	bool gameClear;

	bool fristChange;
	float speedUp;

private:
	int m_screenWidth;
	int m_screenHeight;

	int m_ObjCount;
	int m_PolyCount;

	int m_PlayerPolyCount;
	int m_PlanPolyCount;

	int m_LampCount;
	int m_LampPolyCount;

	int m_WallCount;
	int m_WallPolyCount;

	int m_ZombieCount;
	int m_ZombiePolyCount;

	int m_CannonCount;
	int m_CannonPolyCount;

};

#endif