////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include <random>


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;

	m_Player = 0;
	m_PlayerColl = 0;
	m_PlayerPos = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_LightShader = 0;
	m_Light = 0;

	m_Plane = 0;
	m_SkyBox = 0;

	m_StreetLamp = 0;
	m_Wall = 0;
	m_WallColl = 0;

	spawnTime = 0;
	DontSpawn = false;
	m_Zombie = 0;
	for (int i = 0; i < 10; ++i)
	{
		m_ZombieSurivival[i] = false;
		m_ZombieColl[i] = 0;
		m_ZombiePos[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	DontShoot = true;
	coolTime = 30.0f;
	m_Cannon = 0;
	for (int i = 0; i < 2; ++i)
	{
		m_CannonSurvival[i] = false;
		m_CannonColl[i] = 0;
		m_CannonPos[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	m_TextureShader = 0;

	m_Text = 0;

	for (int i = 0; i < 3; ++i)
	{
		m_Title[i] = 0;
	}

	m_ParticleSystem = 0;
	m_ParticleShader = 0;

	m_ObjCount = 0;
	m_PolyCount = 0;

	m_PlayerPolyCount = 0;
	m_PlanPolyCount = 0;

	m_LampCount = 0;
	m_LampPolyCount = 0;

	m_WallCount = 0;
	m_WallPolyCount = 0;

	m_ZombieCount = 0;
	m_ZombiePolyCount = 0;

	m_CannonCount = 0;
	m_CannonPolyCount = 0;

	m_BGM = 0;
	m_AttackSound = 0;

	for (int i = 0; i < 4; ++i)
	{
		m_Tree[i] = 0;
	}
	m_fpMode = false;
	hp = 5;

	timer = 60;
	time_fps = 0;

	gameOver = false;
	gameClear = false;

	m_stage = EStage::Title;
	titleNum = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	m_fpMode = false;
	hp = 5;
	timer = 60;
	time_fps = 0;
	gameOver = false;
	gameClear = false;
	fristChange = false;
	speedUp = 1.0f;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D) { return false; }

	// Initialize the Direct3D object.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera) { return false; }

	// Set the initial position of the camera.
	//m_Camera->SetPosition(85.0f, 30.0f, -180.0f);	// for cube
	m_Camera->SetPosition(0.0f, 30.0f, -300.0f);
	m_Camera->Render();

	XMMATRIX matrix;
	m_Camera->GetViewMatrix(matrix);

	//Text
	{
		m_Text = new TextClass;
		if (!m_Text) return false;
		result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, matrix);
		if (!result) return false;

		m_TextureShader = new TextureShaderClass;
		if (!m_TextureShader) return false;
		result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
		if (!result) return false;
	}

	// Player
	{
		m_Player = new ModelClass;
		if (!m_Player) { return false; }

		// Initialize the model object.
		result = m_Player->Initialize(m_D3D->GetDevice(), L"./data/Tank/tank.obj", L"./data/Tank/tank.dds");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_PlayerPos = XMFLOAT3(0.0f, 0.0f, -145.0f);
		m_PlayerPolyCount = m_Player->GetIndexCount();


		m_PlayerColl = new Collision;
		if (!m_PlayerColl) { return false; }

		m_PlayerColl->InitAABB(XMFLOAT3(0.0f, 0.0f, -140.0f), XMFLOAT3(10.0f, 20.0f, 10.0f));
	}

	//SkyBox
	{
		m_SkyBox = new SkyBox;
		if (!m_SkyBox) { return false; }

		result = m_SkyBox->Initialize(m_D3D);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
	}

	//Plane
	{
		m_Plane = new ModelClass;

		result = m_Plane->Initialize(m_D3D->GetDevice(), L"./data/plane.obj", L"./data/seafloor.dds");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_PlanPolyCount = m_Plane->GetIndexCount();
	}

	//Lamp
	{
		m_StreetLamp = new ModelClass;
		if (!m_StreetLamp) return false;

		result = m_StreetLamp->Initialize(m_D3D->GetDevice(), L"./data/Lamp/StreetLamp.obj", L"./data/Lamp/StreetLamp.dds");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_LampPolyCount = m_StreetLamp->GetIndexCount();

		m_LampCount = 4;
	}

	//Wall
	{
		m_Wall = new ModelClass;
		if (!m_Wall) return false;

		result = m_Wall->Initialize(m_D3D->GetDevice(), L"./data/Wall/Wall.obj", L"./data/Wall/Wall.dds");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_WallPolyCount = m_Wall->GetIndexCount();

		m_WallColl = new Collision;
		if (!m_WallColl) return false;
		m_WallColl->InitAABB(XMFLOAT3(0.0f, 0.0f, -100.0f), XMFLOAT3(125.0f, 10.0f, 2.0f));

		m_WallCount = 10;
	}

	//Zombie
	{
		m_Zombie = new ModelClass;
		if (!m_Zombie) return false;

		result = m_Zombie->Initialize(m_D3D->GetDevice(), L"./data/Zombie/Zombie.obj", L"./data/Zombie/Zombie.dds");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_ZombiePolyCount = m_Zombie->GetIndexCount();

		//Zombie Initialize
		for (int i = 0; i < 10; ++i)
		{
			m_ZombieSurivival[i] = false;

			m_ZombieColl[i] = new Collision;
			//XMFLOAT3(5.0f, 20.0f, 5.0f)
			m_ZombiePos[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
			m_ZombieColl[i]->InitAABB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(5.0f, 22.0f, 10.0f));
		}
	}

	//Cannon
	{
		m_Cannon = new ModelClass;
		if (!m_Cannon) return false;

		result = m_Cannon->Initialize(m_D3D->GetDevice(), L"./data/Cannon/Cannon.obj", L"./data/Cannon/Cannon.dds");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_CannonPolyCount = m_Cannon->GetIndexCount();

		//Cannon Initialize
		for (int i = 0; i < 2; ++i)
		{
			m_CannonSurvival[i] = false;

			m_CannonColl[i] = new Collision;
			m_CannonColl[i]->InitAABB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f));
			m_CannonPos[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
	}

	//Light
	{	// Create the light shader object.
		m_LightShader = new LightShaderClass;
		if (!m_LightShader) { return false; }

		// Initialize the light shader object.
		result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
			return false;
		}

		// Create the light object.
		m_Light = new LightClass;
		if (!m_Light) { return false; }

		// Initialize the light object.
		float ambient = 0.01f;
		float diffuse = 0.01f;
		m_Light->SetAmbientColor(ambient, ambient, ambient, 1.0f);
		m_Light->SetDiffuseColor(diffuse, diffuse, diffuse, 1.0f);
		m_Light->SetDirection(-0.1f, -0.25f, -0.10f);
		m_Light->SetSpecularColor(0.15f, 0.15f, 0.15f, 1.0f);
		m_Light->SetSpecularPower(20.0f);
	}

	//Sound
	{
		m_BGM = new SoundClass;
		if (!m_BGM) return false;
		result = m_BGM->InitializeBGM(hwnd, "./data/Sound/BGM.wav");
		if (!result) return false;
		m_BGM->SetVolume(-100.0f);

		m_AttackSound = new SoundClass;
		if (!m_AttackSound) return false;
		result = m_AttackSound->Initialize(hwnd, "./data/Sound/Shoot.wav");
		if (!result) return false;

		m_GameClearSound = new SoundClass;
		if (!m_GameClearSound) return false;
		result = m_GameClearSound->Initialize(hwnd, "./data/Sound/GameClear.wav");
		if (!result) return false;

		m_GameOverSound = new SoundClass;
		if (!m_GameOverSound) return false;
		result = m_GameOverSound->Initialize(hwnd, "./data/Sound/GameOver.wav");
		if (!result) return false;

		m_AttackedSound = new SoundClass;
		if (!m_AttackedSound) return false;
		result = m_AttackedSound->Initialize(hwnd, "./data/Sound/Attacked.wav");
		if (!result) return false;

		m_MonsterDeadSound = new SoundClass;
		if (!m_MonsterDeadSound) return false;
		result = m_MonsterDeadSound->Initialize(hwnd, "./data/Sound/MonsterDead.wav");
		if (!result) return false;
	}

	//Billboard
	{
		for (int i = 0; i < 4; ++i)
		{
			m_Tree[i] = new Billboard;
			if (!m_Tree[i]) return false;
			result = m_Tree[i]->Initialize(m_D3D->GetDevice(), "./data/BillboardModel.txt", L"./data/Tree/Tree1.dds");
			if (!result) return false;
		}
	}

	//Bitmap
	{
		for (int i = 0; i < 3; ++i)
		{
			m_Title[i] = new BitmapClass;
			if (!m_Title[i]) return false;
		}

		result = m_Title[0]->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/Title/Title1.dds", 800, 600);
		if (!result) return false;

		result = m_Title[1]->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/Title/Title2.dds", 800, 600);
		if (!result) return false;

		result = m_Title[2]->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/Title/Title3.dds", 800, 600);
		if (!result) return false;

		m_GameClear = new BitmapClass;
		if (!m_GameClear) return false;

		result = m_GameClear->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/Title/GameClear.dds", 800, 600);
		if (!result) return false;

		m_GameOver = new BitmapClass;
		if (!m_GameOver) return false;

		result = m_GameOver->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/Title/GameOver.dds", 800, 600);
		if (!result) return false;
	}

	//Particle
	{
		m_ParticleShader = new ParticleShaderClass;
		if (!m_ParticleShader) { return false; }

		result = m_ParticleShader->Initialize(m_D3D->GetDevice(), hwnd);
		if (!result) { return false; }


		m_ParticleSystem = new ParticleSystemClass;
		if (!m_ParticleSystem) return false;

		result = m_ParticleSystem->Initialize(m_D3D->GetDevice(), L"./data/Particle/star.dds");
		if (!result) { return false; }
	}

	return true;
}

void GraphicsClass::Shutdown()
{ 
	// Release the model object.
	if (m_Player)
	{
		m_Player->Shutdown();
		delete m_Player;
		m_Player = 0;
	}

	if (m_PlayerColl)
	{
		delete m_PlayerColl;
		m_PlayerColl = 0;
	}

	if (m_Plane)
	{
		m_Plane->Shutdown();
		delete m_Plane;
		m_Plane = 0;
	}

	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	if (m_SkyBox) {
		m_SkyBox->Shutdown();
		delete m_SkyBox;
		m_SkyBox = 0;
	}

	if (m_StreetLamp)
	{
		m_StreetLamp->Shutdown();
		delete m_StreetLamp;
		m_StreetLamp = 0;
	}

	if (m_Wall)
	{
		m_Wall->Shutdown();
		delete m_Wall;
		m_Wall = 0;
	}

	if (m_Zombie)
	{
		m_Zombie->Shutdown();
		delete m_Zombie;
		m_Zombie = 0;
	}


	for (int i = 0; i < 10; ++i)
	{

		if (m_ZombieColl[i])
		{
			delete m_ZombieColl[i];
			m_ZombieColl[i] = 0;
		}
	}

	if (m_Cannon)
	{
		m_Cannon->Shutdown();
		delete m_Cannon;
		m_Cannon = 0;
	}

	for (int i = 0; i < 2; ++i)
	{

		if (m_CannonColl[i])
		{
			delete m_CannonColl[i];
			m_CannonColl[i] = 0;
		}
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	for (int i = 0; i < 4; ++i) {
		if (m_Tree[i])
		{
			m_Tree[i]->Shutdown();
			delete m_Tree[i];
			m_Tree[i] = 0;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		if (m_Title[i])
		{
			m_Title[i]->Shutdown();
			delete m_Title[i];
			m_Title[i] = 0;
		}
	}

	if (m_BGM)
	{
		m_BGM->Shutdown();
		delete m_BGM;
		m_BGM = 0;
	}

	if (m_AttackSound)
	{
		m_AttackSound->Shutdown();
		delete m_AttackSound;
		m_AttackSound = 0;
	}

	if (m_GameClearSound)
	{
		m_GameClearSound->Shutdown();
		delete m_GameClearSound;
		m_GameClearSound = 0;
	}

	if (m_GameOverSound)
	{
		m_GameOverSound->Shutdown();
		delete m_GameOverSound;
		m_GameOverSound = 0;
	}

	if (m_AttackedSound)
	{
		m_AttackedSound->Shutdown();
		delete m_AttackedSound;
		m_AttackedSound = 0;
	}

	if (m_MonsterDeadSound)
	{
		m_MonsterDeadSound->Shutdown();
		delete m_MonsterDeadSound;
		m_MonsterDeadSound = 0;
	}

	if (m_GameClear)
	{
		m_GameClear->Shutdown();
		delete m_GameClear;
		m_GameClear = 0;
	}

	if (m_GameOver)
	{
		m_GameOver->Shutdown();
		delete m_GameOver;
		m_GameOver = 0;
	}

	if (m_ParticleShader)
	{
		m_ParticleShader->Shutdown();
		delete m_ParticleShader;
		m_ParticleShader = 0;
	}

	if (m_ParticleSystem)
	{
		m_ParticleSystem->Shutdown();
		delete m_ParticleSystem;
		m_ParticleSystem = 0;
	}

	return;
}

bool GraphicsClass::Frame(int fps, int cpu, float frameTime)
{
	bool result;

	switch (m_stage)
	{
	case GraphicsClass::Title:
	case GraphicsClass::Clear:
	case GraphicsClass::Over:

		break;
	case GraphicsClass::Main:

		if (!m_fpMode && !gameOver && !gameClear) Timer(frameTime);

		if (DontShoot) ShootCoolTime();

		if (DontSpawn)
			SpawnCoolTime();
		else
			SpawnZombie();

		CheckCollision();

		result = UIFrame(fps, cpu, frameTime);
		if (!result) return false;

		break;
	default:

		break;
	}

	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}
	//result = UIRender(fps, frameTime);
	if (!result) return false;

	return true;
}

bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	XMMATRIX matScale, matRot, matTrans;

	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	switch (m_stage)
	{
	case EStage::Title:
	{
		m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
		m_Camera->Render();

		// Get the world, view, and projection matrices from the camera and d3d objects.
		m_Camera->GetViewMatrix(viewMatrix);
		m_D3D->GetWorldMatrix(worldMatrix);
		m_D3D->GetProjectionMatrix(projectionMatrix);

		m_D3D->GetOrthoMatrix(orthoMatrix);

		// Turn off the Z buffer to begin all 2D rendering.
		m_D3D->TurnZBufferOff();

		result = m_Title[titleNum]->Render(m_D3D->GetDeviceContext(), 0, 0);
		if (!result) return false;

		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Title[titleNum]->GetIndexCount(),
			worldMatrix, viewMatrix, orthoMatrix, m_Title[titleNum]->GetTexture());
		if (!result) { return false; }

		m_D3D->TurnZBufferOn();

		break;
	}
	case EStage::Main:
	{
		if (!m_fpMode) {
			m_Camera->SetPosition(m_PlayerPos.x, m_PlayerPos.y + 27.0f, m_PlayerPos.z - 30.0f);
			//m_Camera->SetPosition(m_PlayerPos.x, m_PlayerPos.y + 27.0f, m_PlayerPos.z - 15.0f);
			// Generate the view matrix based on the camera's position.
			m_Camera->Render();
		}
		else
			m_Camera->Update();

		// Get the world, view, and projection matrices from the camera and d3d objects.
		m_Camera->GetViewMatrix(viewMatrix);
		m_D3D->GetWorldMatrix(worldMatrix);
		m_D3D->GetProjectionMatrix(projectionMatrix);

		////////////////////////////////////////////////////////////////////////////////////////////////
		///                                                                                          ///
		///                                        Sky Box                                           ///
		///                                                                                          ///
		////////////////////////////////////////////////////////////////////////////////////////////////

		XMFLOAT3 camPos3 = m_Camera->GetPosition();
		XMVECTOR camPos;
		camPos = XMLoadFloat3(&camPos3);

		m_SkyBox->Update(m_D3D, viewMatrix, projectionMatrix, camPos);

		////////////////////////////////////////////////////////////////////////////////////////////////
		///                                                                                          ///
		///                                       Player                                             ///
		///                                                                                          ///
		////////////////////////////////////////////////////////////////////////////////////////////////

		matScale = XMMatrixScaling(3.0f, 3.5f, 3.0f);
		matRot = XMMatrixRotationY(float(XMConvertToRadians(180.0f)));
		matTrans = XMMatrixTranslation(m_PlayerPos.x, m_PlayerPos.y, m_PlayerPos.z);
		worldMatrix = matScale * matRot * matTrans;

		m_Player->Render(m_D3D->GetDeviceContext());
		result = LightShaderRender(m_Player, worldMatrix, viewMatrix, projectionMatrix);
		if (!result) { return false; }


		////////////////////////////////////////////////////////////////////////////////////////////////
		///                                                                                          ///
		///                                        Plane                                             ///
		///                                                                                          ///
		////////////////////////////////////////////////////////////////////////////////////////////////

		matScale = XMMatrixScaling(4.5f, 6.0f, 1.0f);
		matRot = XMMatrixRotationX(float(XMConvertToRadians(90.0f)));
		matTrans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		worldMatrix = matScale * matRot * matTrans;

		m_Plane->Render(m_D3D->GetDeviceContext());
		result = LightShaderRender(m_Plane, worldMatrix, viewMatrix, projectionMatrix);
		if (!result) { return false; }

		////////////////////////////////////////////////////////////////////////////////////////////////
		///                                                                                          ///
		///                                     Street Lamp                                          ///
		///                                                                                          ///
		////////////////////////////////////////////////////////////////////////////////////////////////

		result = SetLamp();
		if (!result) { return false; }

		////////////////////////////////////////////////////////////////////////////////////////////////
		///                                                                                          ///
		///                                         WALL                                             ///
		///                                                                                          ///
		////////////////////////////////////////////////////////////////////////////////////////////////

		result = SetWall();
		if (!result) return false;

		////////////////////////////////////////////////////////////////////////////////////////////////
		///                                                                                          ///
		///                                        Zombie                                            ///
		///                                                                                          ///
		////////////////////////////////////////////////////////////////////////////////////////////////

		result = ZombieRender();
		if (!result) return false;

		////////////////////////////////////////////////////////////////////////////////////////////////
		///                                                                                          ///
		///                                        Cannon                                            ///
		///                                                                                          ///
		////////////////////////////////////////////////////////////////////////////////////////////////

		result = CannonRender();
		if (!result) return false;

		////////////////////////////////////////////////////////////////////////////////////////////////
		///                                                                                          ///
		///                                        Billboard                                         ///
		///                                                                                          ///
		////////////////////////////////////////////////////////////////////////////////////////////////

		result = SetTree();
		if (!result) return false;

		m_D3D->GetWorldMatrix(worldMatrix);
		m_D3D->GetOrthoMatrix(orthoMatrix);

		m_D3D->TurnZBufferOff();

		m_D3D->TurnOnAlphaBlending();

		result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
		if (!result) return false;

		m_D3D->TurnOffAlphaBlending();

		// Turn the Z buffer back on now that all 2D rendering has completed.
		m_D3D->TurnZBufferOn();

		break;
	}
	case EStage::Clear:
	{
		m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
		m_Camera->Render();

		// Get the world, view, and projection matrices from the camera and d3d objects.
		m_Camera->GetViewMatrix(viewMatrix);
		m_D3D->GetWorldMatrix(worldMatrix);
		m_D3D->GetProjectionMatrix(projectionMatrix);

		m_D3D->GetOrthoMatrix(orthoMatrix);

		// Turn off the Z buffer to begin all 2D rendering.
		m_D3D->TurnZBufferOff();

		result = m_GameClear->Render(m_D3D->GetDeviceContext(), 0, 0);
		if (!result) return false;

		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_GameClear->GetIndexCount(),
			worldMatrix, viewMatrix, orthoMatrix, m_GameClear->GetTexture());
		if (!result) { return false; }

		m_D3D->TurnZBufferOn();

		break;
	}
	case EStage::Over:
	{
		m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
		m_Camera->Render();

		// Get the world, view, and projection matrices from the camera and d3d objects.
		m_Camera->GetViewMatrix(viewMatrix);
		m_D3D->GetWorldMatrix(worldMatrix);
		m_D3D->GetProjectionMatrix(projectionMatrix);

		m_D3D->GetOrthoMatrix(orthoMatrix);

		// Turn off the Z buffer to begin all 2D rendering.
		m_D3D->TurnZBufferOff();

		result = m_GameOver->Render(m_D3D->GetDeviceContext(), 0, 0);
		if (!result) return false;

		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_GameOver->GetIndexCount(),
			worldMatrix, viewMatrix, orthoMatrix, m_GameOver->GetTexture());
		if (!result) { return false; }

		m_D3D->TurnZBufferOn();

		break;
	}
	default:
		break;
	}

	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::LightShaderRender(ModelClass* model, XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
	bool result;

	XMFLOAT4 pointDuffusecolor[4];
	XMFLOAT4 lightPosition[4];

	pointDuffusecolor[0] = XMFLOAT4(0.2f, 0.0f, 0.0f, 1.0f);
	pointDuffusecolor[1] = XMFLOAT4(0.2f, 0.0f, 0.0f, 1.0f);
	pointDuffusecolor[2] = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	pointDuffusecolor[3] = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);

	lightPosition[0] = XMFLOAT4(85.0f, 50.0f, 90.0f, 1.0f);
	lightPosition[1] = XMFLOAT4(-85.0f, 50.0f, 90.0f, 1.0f);
	lightPosition[2] = XMFLOAT4(85.0f, 50.0f, -130.0f, 1.0f);
	lightPosition[3] = XMFLOAT4(-85.0f, 50.0f, -130.0f, 1.0f);

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), model->GetIndexCount(),
		world, view, projection, model->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(),
		pointDuffusecolor, lightPosition);

	return result;
}

bool GraphicsClass::SetLamp()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMMATRIX matScale, matTrans;

	bool result;

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	matScale = XMMatrixScaling(0.12f, 0.12f, 0.12f);
	matTrans = XMMatrixTranslation(85.0f, 0.0f, 100.0f);
	worldMatrix = matScale * matTrans;

	m_StreetLamp->Render(m_D3D->GetDeviceContext());
	result = LightShaderRender(m_StreetLamp, worldMatrix, viewMatrix, projectionMatrix);
	if (!result) { return false; }

	//matScale = XMMatrixScaling(0.12f, 0.12f, 0.102f);
	matTrans = XMMatrixTranslation(-85.0f, 0.0f, 100.0f);
	worldMatrix = matScale * matTrans;

	m_StreetLamp->Render(m_D3D->GetDeviceContext());
	LightShaderRender(m_StreetLamp, worldMatrix, viewMatrix, projectionMatrix);
	if (!result) { return false; }

	//matScale = XMMatrixScaling(0.12f, 0.12f, 0.12f);
	matTrans = XMMatrixTranslation(85.0f, 0.0f, -125.0f);
	//matTrans = XMMatrixTranslation(0.0f, 10.0f, -100.0f);
	worldMatrix = matScale * matTrans;

	m_StreetLamp->Render(m_D3D->GetDeviceContext());
	LightShaderRender(m_StreetLamp, worldMatrix, viewMatrix, projectionMatrix);
	if (!result) { return false; }

	//matScale = XMMatrixScaling(0.12f, 0.12f, 0.12f);
	matTrans = XMMatrixTranslation(-85.0f, 0.0f, -125.0f);
	worldMatrix = matScale * matTrans;

	m_StreetLamp->Render(m_D3D->GetDeviceContext());
	LightShaderRender(m_StreetLamp, worldMatrix, viewMatrix, projectionMatrix);
	if (!result) { return false; }

	return true;
}

bool GraphicsClass::SetWall()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMMATRIX matScale, matRot, matTrans;

	bool result;

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	float trans = -120.0f;
	for (int i = 0; i < 5; ++i)
	{
		matScale = XMMatrixScaling(3.0f, 3.0f, 3.0f);
		matRot = XMMatrixRotationY(float(XMConvertToRadians(90.0f)));
		matTrans = XMMatrixTranslation(-100.0f, 0.0f, trans);
		worldMatrix = matScale * matRot * matTrans;

		m_Wall->Render(m_D3D->GetDeviceContext());
		result = LightShaderRender(m_Wall, worldMatrix, viewMatrix, projectionMatrix);
		if (!result) return false;

		trans += 60.0f;
	}

	trans = -120.0f;
	for (int i = 0; i < 5; ++i)
	{
		matScale = XMMatrixScaling(3.0f, 3.0f, 3.0f);
		matRot = XMMatrixRotationY(float(XMConvertToRadians(90.0f)));
		matTrans = XMMatrixTranslation(100.0f, 0.0f, trans);
		worldMatrix = matScale * matRot * matTrans;

		m_Wall->Render(m_D3D->GetDeviceContext());
		result = LightShaderRender(m_Wall, worldMatrix, viewMatrix, projectionMatrix);
		if (!result) return false;

		trans += 60.0f;
	}

	trans = -80.0f;
	for (int i = 0; i < 5; ++i)
	{
		matScale = XMMatrixScaling(2.0f, 0.8f, 2.0f);
		matTrans = XMMatrixTranslation(trans, 0.0f, -100.0f);
		worldMatrix = matScale * matTrans;

		m_Wall->Render(m_D3D->GetDeviceContext());
		result = LightShaderRender(m_Wall, worldMatrix, viewMatrix, projectionMatrix);
		if (!result) return false;

		trans += 40.0f;
	}

	return true;

}

bool GraphicsClass::ZombieRender()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMMATRIX matScale, matTrans;

	bool result;

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	for (int i = 0; i < 10; ++i)
	{
		if (!m_ZombieSurivival[i]) continue;
		++m_ZombieCount;

		matScale = XMMatrixScaling(1.2f, 1.2f, 1.2f);
		matTrans = XMMatrixTranslation(m_ZombiePos[i].x, m_ZombiePos[i].y, m_ZombiePos[i].z);
		worldMatrix = matScale * matTrans;

		//m_Zombie[i]->Render(m_D3D->GetDeviceContext());
		m_Zombie->Render(m_D3D->GetDeviceContext());
		result = LightShaderRender(m_Zombie, worldMatrix, viewMatrix, projectionMatrix);
		if (!result) return false;

		m_ZombieColl[i]->UpdateAABB(m_ZombiePos[i]);

		if (!m_fpMode)
			m_ZombiePos[i].z -= 0.5f * speedUp;
	}

	return true;
}

bool GraphicsClass::CannonRender()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMMATRIX matScale, matTrans;

	bool result;

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	for (int i = 0; i < 2; ++i)
	{
		if (!m_CannonSurvival[i]) continue;
		++m_CannonCount;

		matScale = XMMatrixScaling(3.0f, 3.0f, 3.0f);
		matTrans = XMMatrixTranslation(m_CannonPos[i].x, m_CannonPos[i].y, m_CannonPos[i].z);
		worldMatrix = matScale * matTrans;

		m_Cannon->Render(m_D3D->GetDeviceContext());
		result = LightShaderRender(m_Cannon, worldMatrix, viewMatrix, projectionMatrix);
		if (!result) return false;

		m_CannonColl[i]->UpdateAABB(m_CannonPos[i].x, m_CannonPos[i].y - 2.0f, m_CannonPos[i].z);


		if (m_fpMode) continue;

		if (m_CannonPos[i].z > 160.0f)
			m_CannonSurvival[i] = false;
		else
			m_CannonPos[i].z += 7.5f;


	}

	return true;
}

bool GraphicsClass::SetTree()
{

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	XMMATRIX matScale, matRot, matTrans, matBill;
	XMFLOAT3 modelPos, camPos;
	bool result;

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	camPos = m_Camera->GetPosition();
	matScale = XMMatrixScaling(25.0f, 25.0f, 25.0f);

	m_D3D->TurnOnAlphaBlending();

	for (int i = 0; i < 4; ++i)
	{
		modelPos.x = -75.0f + i * 50.0f;
		modelPos.y = 23.0f;
		modelPos.z = 150.0f;

		double angle = atan2(modelPos.x - camPos.x, modelPos.z - camPos.z) * (180.0 / XM_PI);

		float rotation = (float)angle * 0.0174532925f;

		matRot = XMMatrixRotationY(rotation);

		worldMatrix = matScale * matRot;

		matBill = XMMatrixTranslation(modelPos.x, modelPos.y, modelPos.z);

		worldMatrix = XMMatrixMultiply(worldMatrix, matBill);

		m_Tree[i]->Render(m_D3D->GetDeviceContext());


		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Tree[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_Tree[i]->GetTexture());
		if (!result) return false;

	}

	m_D3D->TurnOffAlphaBlending();

	return true;
}

void GraphicsClass::adMoveMent(float x)
{
	m_PlayerPos.x += x;
	if (m_PlayerPos.x < -80.0f)
		m_PlayerPos.x = -80.0f;
	if (m_PlayerPos.x > 80.0f)
		m_PlayerPos.x = 80.0f;
}

void GraphicsClass::Shooting()
{
	if (DontShoot) return ;

	for (int i = 0; i < 2; ++i)
	{
		if (m_CannonSurvival[i]) ;

		printf("Shoot\n");

		m_CannonSurvival[i] = true;

		m_CannonPos[i] = XMFLOAT3(m_PlayerPos.x, m_PlayerPos.y + 15.0f, m_PlayerPos.z + 20.0f);
		m_CannonColl[i]->UpdateAABB(m_CannonPos[i].x, m_CannonPos[i].y - 2.0f, m_CannonPos[i].z);

		DontShoot = true;

		m_AttackSound->PlayGameSound();

		return ;
	}

	return ;
}

void GraphicsClass::CheckCollision()
{
	for (int i = 0; i < 10; ++i)
	{
		if (!m_ZombieSurivival[i]) continue;

		for (int j = 0; j < 2; ++j) {
			if (!m_CannonSurvival[j]) continue;

			if (m_CannonColl[j]->CheckAABB(m_ZombieColl[i]))
			{
				printf("Crush\n");
				m_ZombieSurivival[i] = false;
				m_CannonSurvival[j] = false;

				m_MonsterDeadSound->PlayGameSound();

				break;
			}
		}

		if (m_WallColl->CheckAABB(m_ZombieColl[i]))
		{
			hp -= 1;
			m_AttackedSound->PlayGameSound();
			if (hp <= 0)
			{
				m_stage = EStage::Over;
				m_BGM->StopBGM();
				m_GameOverSound->PlayGameSound();
			}
			m_ZombieSurivival[i] = false;
		}
	}
}

void GraphicsClass::ShootCoolTime()
{
	if (m_fpMode) return;
	if (coolTime > 60.0f)
	{
		DontShoot = false;
		coolTime = 0.0f;
	}
	else
		coolTime += 0.5f;

}

void GraphicsClass::SpawnCoolTime()
{
	if (m_fpMode) return;
	if (spawnTime > 70.0f)
	{
		DontSpawn = false;
		spawnTime = 0.0f;
	}
	else
		spawnTime += 0.5f;
}

void GraphicsClass::SpawnZombie()
{
	for (int i = 0; i < 10; ++i)
	{
		if (m_ZombieSurivival[i]) continue;

		++m_ZombieCount;

		m_ZombieSurivival[i] = true;

		std::random_device rd;
		std::mt19937 gen(rd());

		std::uniform_int_distribution<int> dis(0, 4);

		int xPos = dis(gen);

		m_ZombiePos[i] = XMFLOAT3(-50.0f + (float)(xPos * 25), 0.0f, 150.0f);
		m_ZombieColl[i]->UpdateAABB(m_ZombiePos[i]);

		DontSpawn = true;

		break;
	}
}

void GraphicsClass::SetObjPolyCount()
{
	// PlayerCount + PlanCount + LampCount + WallCount + ZombieCount + CannonCount
	m_ObjCount = 2 + m_LampCount + m_WallCount + m_ZombieCount + m_CannonCount;

	m_PolyCount = m_PlayerPolyCount / 3 + m_PlanPolyCount / 3 + (m_LampCount * m_LampPolyCount / 3) +
		(m_WallCount * m_WallPolyCount / 3) + (m_ZombieCount * m_ZombiePolyCount / 3) + (m_CannonCount * m_CannonPolyCount / 3);

	m_ZombieCount = 0;
	m_CannonCount = 0;
}

bool GraphicsClass::UIFrame(int fps, int cpu, float framTime)
{
	bool result;

	SetObjPolyCount();

	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result) return false;

	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result) return false;

	result = m_Text->SetPolyCount(m_PolyCount, m_D3D->GetDeviceContext());
	if (!result) return false;

	result = m_Text->SetObjCount(m_ObjCount, m_D3D->GetDeviceContext());
	if (!result) return false;

	result = m_Text->SetScreenSize(m_screenWidth, m_screenHeight, m_D3D->GetDeviceContext());
	if (!result) return false;

	result = m_Text->SetHP(hp, m_D3D->GetDeviceContext());
	if (!result) return false;

	result = m_Text->SetTimer(timer, m_D3D->GetDeviceContext());
	if (!result) return false;


	return true;
}

void GraphicsClass::Timer(float frameTime)
{
	timer -= frameTime / 1000.0f;

	if (timer <= 30.0f && !fristChange)
	{
		fristChange = true;
		speedUp = 2.0f;
	}

	if (timer <= 0.0f)
	{
		m_stage = EStage::Clear;
		m_BGM->StopBGM();
		m_GameClearSound->PlayGameSound();
	}
}

void GraphicsClass::AddTitleNum()
{
	titleNum += 1;

	if (titleNum == 3)
		m_stage = EStage::Main;
}

void GraphicsClass::ChangeFPmode()
{
	m_fpMode = !m_fpMode;

	if (!m_fpMode)
		CameraReset();
}

void GraphicsClass::Reset()
{
	m_stage = EStage::Main;
	m_fpMode = false;

	timer = 60.0f;

	DontShoot = true;
	coolTime = 30.0f;
	spawnTime = 30.0f;

	hp = 5;

	for (int i = 0; i < 2; ++i)
	{
		m_CannonSurvival[i] = false;
	}
	m_CannonCount = 0;

	for (int i = 0; i < 10; ++i)
	{
		m_ZombieSurivival[i] = false;
	}
	m_ZombieCount = 0;

	gameOver = false;
	gameClear = false;

	m_PlayerPos = XMFLOAT3(0.0f, 0.0f, -145.0f);
	m_PlayerColl->InitAABB(XMFLOAT3(0.0f, 0.0f, -140.0f), XMFLOAT3(10.0f, 20.0f, 10.0f));

	m_BGM->PlayBGM();
}