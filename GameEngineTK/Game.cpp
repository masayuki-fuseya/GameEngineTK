//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "ModelEffect.h"

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game() :
    m_window(0),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

	m_keyboard = std::make_unique<KeyboardUtil>();

	//m_player = std::make_unique<Player>();
	//m_player->SetKeyboard(m_keyboard.get());

	m_camera = std::make_unique<FollowCamera>(m_outputWidth, m_outputHeight);
	//m_camera->SetPlayer(m_player.get());
	m_camera->SetKeyboard(m_keyboard.get());

	m_debugCamera = std::make_unique<DebugCamera>(m_outputWidth, m_outputHeight);

	Obj3d::InitializeStatic(m_camera.get(), m_d3dDevice, m_d3dContext);

	// 地形の初期化
	LandShapeCommonDef def;
	def.pDevice = m_d3dDevice.Get();
	def.pDeviceContext = m_d3dContext.Get();
	def.pCamera = m_camera.get();
	LandShape::InitializeCommon(def);

	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
	//m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(m_d3dContext.Get());
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(m_d3dContext.Get());
	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());

	m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0.0f,
		static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight), 0.0f, 0.0f, 1.0f));
	m_effect->SetVertexColorEnabled(true);

	void const* shaderBateCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderBateCode, &byteCodeLength);

	m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
		shaderBateCode, byteCodeLength, m_inputLayout.GetAddressOf());

	m_world = Matrix::Identity;

	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());

	//ファイルの読み込みパスを指定
	m_factory->SetDirectory(L"Resources");

	// モデルの読み込み
	m_objSkydome.LoadModel(L"Resources\\skydome.cmo");
	//m_objGround.LoadModel(L"Resources\\ground200m.cmo");

	// 地形の読み込み
	m_landShape.Initialize(L"ground200m", L"ground200m");

	// ライティングを無効にする
	m_objSkydome.DisableLighting();
	//m_objGround.DisableLighting();
	m_landShape.DisableLighting();

	m_player = std::make_unique<Player>();
	m_player->SetKeyboard(m_keyboard.get());

	m_camera->SetPlayer(m_player.get());

	// 敵の生成
	int enemyNum = rand() % ENEMY_NUM + 1;
	m_enemies.resize(enemyNum);
	for (int i = 0; i < enemyNum; i++)
	{
		m_enemies[i] = std::make_unique<Enemy>();
	}

	m_modelSphere = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources\\sphere.cmo", *m_factory);
	m_modelTeapot = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources\\teapot.cmo", *m_factory);

	m_sphereAngle = 0.0f;
	m_teapotAngle = 0.0f;

	//for (int i = 0; i < 40000; i++)
	//{
	//	// 地面のワールド行列の計算
	//	m_worldGround[i] = Matrix::CreateTranslation(i % 200 - 100, 0, i / 200 - 100);
	//}

	// ティーポット ///////////////////////////////////////////////

	//for (int i = 0; i < 20; i++)
	//{
	//	m_posTeapot[i].x = static_cast<float>(rand() % 100);
	//	m_posTeapot[i].z = static_cast<float>(rand() % 100);
	//	int angle = rand() % static_cast<int>(XM_2PI);

	//	m_posTeapot[i].x = static_cast<float>(cos(angle)) * m_posTeapot[i].x;
	//	m_posTeapot[i].z = static_cast<float>(sin(angle)) * m_posTeapot[i].z;

	//	Matrix transmat = Matrix::CreateTranslation(m_posTeapot[i].x, 0, m_posTeapot[i].z);

	//	// 10秒で原点に向かうように計算する
	//	m_moveTeapot[i] = Vector3(-m_posTeapot[i].x / 600.0f, 0.0f, -m_posTeapot[i].z / 600.0f);

	//	m_worldTeapot[i] = transmat;
	//}

	///////////////////////////////////////////////////////////////

	m_time = 0.0f;
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;
	// ゲームの毎フレーム処理
	m_debugCamera->Update();

	m_keyboard->Update();

	// 球の回転 ////////////////////////////////////////////////////

	//// 球のワールド行列の計算
	//Matrix scalemat = Matrix::CreateScale(0.5f);
	//Matrix transmat;
	//Matrix rotmatY;

	//// 角度を変える
	//m_angleSphere += 1.0f;

	//for (int i = 0; i < 20; i++)
	//{
	//	transmat = Matrix::CreateTranslation(10.0f * (i / 10 + 1), 0.0f, 0.0f);
	//	// 内側を回る球
	//	if (i < 10)
	//	{
	//		// 反時計回りの回転
	//		rotmatY = Matrix::CreateRotationY(XMConvertToRadians(36.0f * i + m_angleSphere));
	//	}
	//	// 外側を回る球
	//	else
	//	{
	//		// 時計回りの回転
	//		rotmatY = Matrix::CreateRotationY(XMConvertToRadians(36.0f * i + -m_angleSphere));
	//	}
	//	m_worldSphere[i] = scalemat * transmat * rotmatY;
	//}

	////////////////////////////////////////////////////////////////

	// ティーポットの回転と移動 ////////////////////////////////////

	//m_angleTeapot += 0.5f;
	//m_time += 0.5f / 60.0f;
	//for (int i = 0; i < 20; i++)
	//{
	//	// 原点に向かう
	//	m_posTeapot[i].x += m_moveTeapot[i].x;
	//	m_posTeapot[i].z += m_moveTeapot[i].z;
	//	// 行き過ぎたら原点に戻す
	//	if (m_moveTeapot[i].x < 0.0f && m_posTeapot[i].x < 0.0f ||
	//		m_moveTeapot[i].x > 0.0f && m_posTeapot[i].x > 0.0f)
	//	{
	//		m_posTeapot[i].x = 0.0f;
	//	}
	//	if (m_moveTeapot[i].z < 0.0f && m_posTeapot[i].z < 0.0f ||
	//		m_moveTeapot[i].z > 0.0f && m_posTeapot[i].z > 0.0f)
	//	{
	//		m_posTeapot[i].z = 0.0f;
	//	}

	//	Matrix transmat = Matrix::CreateTranslation(m_posTeapot[i].x, 0.0f, m_posTeapot[i].z);

	//	Matrix rotmatY = Matrix::CreateRotationY(XMConvertToRadians(m_angleTeapot));
	//	
	//	Matrix scalemat = Lerp(1.0f, 5.0f, m_time);
	//	
	//	m_worldTeapot[i] = rotmatY * transmat;
	//}

	////////////////////////////////////////////////////////////////
	 
	// 3Dオブジェクトの更新 ////////////////////////////////////////

	m_objSkydome.Update();
	//m_objGround.Update();
	m_landShape.Update();
	m_player->Update();
	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemies.begin(); it != m_enemies.end(); it++)
	{
		Enemy* enemy = it->get();
		enemy->Update();
	}

	////////////////////////////////////////////////////////////////

	// 弾丸と敵の当たり判定 ////////////////////////////////////////

	const Sphere& batterySphere = m_player->GetCollisionNodeBattery();

	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemies.begin(); it != m_enemies.end(); )
	{
		Enemy* enemy = it->get();

		const Sphere& enemySphere = enemy->GetCollisionNodeTank();

		// 砲台と敵が当たっていたら
		if (CheckSphere2Sphere(batterySphere, enemySphere))
		{
			// エフェクトを出す
			ModelEffectManager::getInstance()->Entry(L"Resources\\effect.cmo", 20,
				enemy->GetTranslation(Enemy::PLAYER_PARTS_TANK) + Vector3(0.0f, 0.5f, 0.0f), Vector3::Zero, Vector3::Zero,
				Vector3::Zero, Vector3::Zero, Vector3::Zero, Vector3(4.0f));

			// 敵を消す
			// 消した次の要素のイテレータを返す
			it = m_enemies.erase(it);
		}
		else
		{
			it++;
		}
	}

	////////////////////////////////////////////////////////////////
	
	ModelEffectManager::getInstance()->Update();

	// カメラの位置の更新 //////////////////////////////////////////

	// カメラが自機についてくる
	//Vector3 moveCamera(0, 1, 3);
	//moveCamera = Vector3::TransformNormal(moveCamera, m_worldTank);
	//m_camera->SetEyePos(m_posTank + moveCamera);
	//m_camera->SetRefPos(m_posTank);

	//m_camera->Update();
	//m_view = m_camera->GetView();
	//m_proj = m_camera->GetProjection();

	m_camera->Update();
	m_view = m_camera->GetView();
	m_proj = m_camera->GetProjection();

	////////////////////////////////////////////////////////////////

	// 自機の地形へのめり込みを検出して押し出す ////////////////////

	{
		Sphere sphere = m_player->GetCollisionNodeTank();
		Vector3 trans = m_player->GetTranslation();
		// 当たり判定球の中心から自機の足元へのベクトル
		Vector3 sphere2player = trans - sphere.m_center;

		// 排斥ベクトル
		Vector3 reject;

		// 地形と球の当たり判定
		if (m_landShape.IntersectSphere(sphere, &reject))
		{
			// めり込み分押し出す
			sphere.m_center += reject;
		}

		// 押し出された座標をセット
		m_player->SetTranslation(sphere.m_center += sphere2player);

		// ワールド行列だけずらす
		m_player->Calc();
	}

	////////////////////////////////////////////////////////////////

	// 地面に乗る処理 //////////////////////////////////////////////

	{
		// 落下中か着地中のみ
		if (m_player->GetVelocity().y <= 0.0f)
		{
			// プレイヤーの上から下へのベクトル
			Segment playerSegment;
			Vector3 trans = m_player->GetTranslation();
			playerSegment.m_start = trans + Vector3(0.0f, 1.0f, 0.0f);
			// 少し長めに判定を取る
			playerSegment.m_end = trans + Vector3(0.0f, -0.5f, 0.0f);

			Vector3 inter;

			// 地形と線分の当たり判定
			if (m_landShape.IntersectSegment(playerSegment, &inter))
			{
				trans.y = inter.y;
				m_player->SetTranslation(trans);

				m_player->Calc();

				m_player->StopJump();
			}
			else
			{
				m_player->StartFall();
			}
		}
	}

	////////////////////////////////////////////////////////////////

	/// ゲームクリア ///////////////////////////////////////////////

	if (m_enemies.size() == 0)
	{
		m_gameClear = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources\\gameclear.cmo", *m_factory);
	}

	////////////////////////////////////////////////////////////////
}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    // TODO: Add your rendering code here.
	// 描画はここに書く
	uint16_t indices[] =
	{
		0, 1, 2,
		2, 1, 3
	};

	VertexPositionNormal vertices[] =
	{
		{ Vector3(-1.0f,  1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) },
		{ Vector3(-1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) },
		{ Vector3( 1.0f,  1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) },
		{ Vector3( 1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) }
	};

	m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	m_d3dContext->RSSetState(m_states->CullNone());
	
	//// カメラの位置
	//Vector3 eyepos(0.0f, 0.0f, 5.0f);
	//// どこをみるか
	//Vector3 refpos = Vector3::Zero;
	//// 上方向ベクトル
	//Vector3 upvec(1, -1, 0);
	//upvec.Normalize();
	//m_view = Matrix::CreateLookAt(eyepos, refpos, upvec);
	//m_view = m_debugCamera->GetCameraMatrix();

	//// 垂直方向視野角(上下それぞれ何度まで写すか)
	//float fovY = XMConvertToRadians(60.0f);
	//// 画面サイズの比率
	//float aspect = (float)m_outputWidth / m_outputHeight;
	//// 手前の表示限界
	//float nearClip = 0.1f;
	//// 奥の表示限界
	//float farClip = 1000.0f;
	//m_proj = Matrix::CreatePerspectiveFieldOfView(fovY, aspect, nearClip, farClip);

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

	m_effect->Apply(m_d3dContext.Get());
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	// 天球の描画
	m_objSkydome.Render();
	
	// 地面の描画
	//for (int i = 0; i < 40000; i++)
	//{
	//	m_modelGround->Draw(m_d3dContext.Get(), *m_states, m_worldGround[i], m_view, m_proj);
	//}
	//m_objGround.Render();
	m_landShape.Draw();
	
	//for (int i = 0; i < 20; i++)
	//{
	//	// 球の描画
	//	m_modelSphere->Draw(m_d3dContext.Get(), *m_states, m_worldSphere[i], m_view, m_proj);
	//}

	//for (int i = 0; i < 20; i++)
	//{
	//	// ティーポットの描画
	//	m_modelTeapot->Draw(m_d3dContext.Get(), *m_states, m_worldTeapot[i], m_view, m_proj);
	//}
	
	m_player->Render();

	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemies.begin(); it != m_enemies.end(); it++)
	{
		Enemy* enemy = it->get();
		enemy->Render();
	}

	// エフェクトの描画
	ModelEffectManager::getInstance()->Draw();

	if (m_gameClear)
	{
		Matrix transmat = Matrix::CreateTranslation(m_player->GetTranslation() + Vector3(0.0f, 3.0f, -5.0f));
		Matrix rotmat = Matrix::CreateRotationY(m_player->GetRotation().y);
		m_gameClear->Draw(m_d3dContext.Get(), *m_states, rotmat * transmat, m_view, m_proj);
	}

	//m_batch->Begin();

	//m_batch->DrawLine(VertexPositionColor(Vector3(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f)),
	//	VertexPositionColor(Vector3(100.0f, 80.0f, 0.0f), Color(1.0f, 1.0f, 1.0f)));
	// 2D座標
	//VertexPositionColor v1(Vector3(400.f, 150.f, 0.f), Colors::Chocolate);
	//VertexPositionColor v2(Vector3(600.f, 450.f, 0.f), Colors::Blue);
	//VertexPositionColor v3(Vector3(200.f, 450.f, 0.f), Colors::Yellow);
	// 3D座標
	//VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::Yellow);
	//VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::Yellow);
	//VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::Yellow);

	//m_batch->DrawTriangle(v1, v2, v3);

	//m_batch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, 6, vertices, 4);

	//m_batch->End();

    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    HRESULT hr = D3D11CreateDevice(
        nullptr,                                // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        m_d3dDevice.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
        &m_featureLevel,                        // returns feature level of device created
        m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
        );

    if (hr == E_INVALIDARG)
    {
        // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
        hr = D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creationFlags,
            &featureLevels[1],
            _countof(featureLevels) - 1,
            D3D11_SDK_VERSION,
            m_d3dDevice.ReleaseAndGetAddressOf(),
            &m_featureLevel,
            m_d3dContext.ReleaseAndGetAddressOf()
            );
    }

    DX::ThrowIfFailed(hr);

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    // DirectX 11.1 if present
    if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
        (void)m_d3dContext.As(&m_d3dContext1);

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory1> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        ComPtr<IDXGIFactory2> dxgiFactory2;
        if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
        {
            // DirectX 11.1 or later

            // Create a descriptor for the swap chain.
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
            swapChainDesc.Width = backBufferWidth;
            swapChainDesc.Height = backBufferHeight;
            swapChainDesc.Format = backBufferFormat;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = backBufferCount;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
            fsSwapChainDesc.Windowed = TRUE;

            // Create a SwapChain from a Win32 window.
            DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
                m_d3dDevice.Get(),
                m_window,
                &swapChainDesc,
                &fsSwapChainDesc,
                nullptr,
                m_swapChain1.ReleaseAndGetAddressOf()
                ));

            DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
        }
        else
        {
            DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
            swapChainDesc.BufferCount = backBufferCount;
            swapChainDesc.BufferDesc.Width = backBufferWidth;
            swapChainDesc.BufferDesc.Height = backBufferHeight;
            swapChainDesc.BufferDesc.Format = backBufferFormat;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.OutputWindow = m_window;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.Windowed = TRUE;

            DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
        }

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain1.Reset();
    m_swapChain.Reset();
    m_d3dContext1.Reset();
    m_d3dContext.Reset();
    m_d3dDevice1.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}

Matrix Game::Lerp(float startScale, float targetScale, float t)
{
	Matrix lerpScale = Matrix::Identity;

	lerpScale = Matrix::CreateScale((1 - cosCurve(t)) * startScale + cosCurve(t) * targetScale);

	return lerpScale;
}

float Game::cosCurve(float time)
{
	float vt = 0.0f;

	vt = (1 - cos(time * XM_PI) / 2.0f);

	return vt;
}