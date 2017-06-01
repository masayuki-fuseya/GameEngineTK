//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

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
	m_camera = std::make_unique<FollowCamera>(m_outputWidth, m_outputHeight);
	// �J�����ɃL�[�{�[�h���Z�b�g
	m_camera->SetKeyboard(m_keyboard.get());

	m_debugCamera = std::make_unique<DebugCamera>(m_outputWidth, m_outputHeight);

	Obj3d::InitializeStatic(m_camera.get(), m_d3dDevice, m_d3dContext);

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

	//�t�@�C���̓ǂݍ��݃p�X���w��
	m_factory->SetDirectory(L"Resources");

	// ���f���̓ǂݍ���
	m_objSkydome.LoadModel(L"Resources\\skydome.cmo");
	m_objGround.LoadModel(L"Resources\\ground200m.cmo");

	m_objPlayer.resize(PLAYER_PARTS_NUM);
	m_objPlayer[PLAYER_PARTS_TANK].LoadModel(L"Resources\\tank.cmo");
	m_objPlayer[PLAYER_PARTS_BATTERY].LoadModel(L"Resources\\battery.cmo");
	m_objPlayer[PLAYER_PARTS_STAR].LoadModel(L"Resources\\star.cmo");
	m_objPlayer[PLAYER_PARTS_SHIELD].LoadModel(L"Resources\\shield.cmo");
	m_objPlayer[PLAYER_PARTS_DRILL].LoadModel(L"Resources\\drill.cmo");

	// �e�q�֌W�̐ݒ�
	m_objPlayer[PLAYER_PARTS_BATTERY].SetObjParent(&m_objPlayer[PLAYER_PARTS_TANK]);
	m_objPlayer[PLAYER_PARTS_STAR].SetObjParent(&m_objPlayer[PLAYER_PARTS_BATTERY]);
	m_objPlayer[PLAYER_PARTS_SHIELD].SetObjParent(&m_objPlayer[PLAYER_PARTS_TANK]);
	m_objPlayer[PLAYER_PARTS_DRILL].SetObjParent(&m_objPlayer[PLAYER_PARTS_TANK]);

	m_modelSphere = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources\\sphere.cmo", *m_factory);
	m_modelTeapot = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources\\teapot.cmo", *m_factory);

	m_sphereAngle = 0.0f;
	m_teapotAngle = 0.0f;
	m_tankAngle = 0.0f;
	m_starAngle = Vector3::Zero;
	m_sinAngle = 0.0f;
	m_sinScale = 1.0f;

	//for (int i = 0; i < 40000; i++)
	//{
	//	// �n�ʂ̃��[���h�s��̌v�Z
	//	m_worldGround[i] = Matrix::CreateTranslation(i % 200 - 100, 0, i / 200 - 100);
	//}

	// �e�B�[�|�b�g ///////////////////////////////////////////////

	//for (int i = 0; i < 20; i++)
	//{
	//	m_posTeapot[i].x = static_cast<float>(rand() % 100);
	//	m_posTeapot[i].z = static_cast<float>(rand() % 100);
	//	int angle = rand() % static_cast<int>(XM_2PI);

	//	m_posTeapot[i].x = static_cast<float>(cos(angle)) * m_posTeapot[i].x;
	//	m_posTeapot[i].z = static_cast<float>(sin(angle)) * m_posTeapot[i].z;

	//	Matrix transmat = Matrix::CreateTranslation(m_posTeapot[i].x, 0, m_posTeapot[i].z);

	//	// 10�b�Ō��_�Ɍ������悤�Ɍv�Z����
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
	// �Q�[���̖��t���[������
	m_debugCamera->Update();
	
	// ���̉�] ////////////////////////////////////////////////////

	//// ���̃��[���h�s��̌v�Z
	//Matrix scalemat = Matrix::CreateScale(0.5f);
	//Matrix transmat;
	//Matrix rotmatY;

	//// �p�x��ς���
	//m_angleSphere += 1.0f;

	//for (int i = 0; i < 20; i++)
	//{
	//	transmat = Matrix::CreateTranslation(10.0f * (i / 10 + 1), 0.0f, 0.0f);
	//	// ��������鋅
	//	if (i < 10)
	//	{
	//		// �����v���̉�]
	//		rotmatY = Matrix::CreateRotationY(XMConvertToRadians(36.0f * i + m_angleSphere));
	//	}
	//	// �O������鋅
	//	else
	//	{
	//		// ���v���̉�]
	//		rotmatY = Matrix::CreateRotationY(XMConvertToRadians(36.0f * i + -m_angleSphere));
	//	}
	//	m_worldSphere[i] = scalemat * transmat * rotmatY;
	//}

	////////////////////////////////////////////////////////////////

	// �e�B�[�|�b�g�̉�]�ƈړ� ////////////////////////////////////

	//m_angleTeapot += 0.5f;
	//m_time += 0.5f / 60.0f;
	//for (int i = 0; i < 20; i++)
	//{
	//	// ���_�Ɍ�����
	//	m_posTeapot[i].x += m_moveTeapot[i].x;
	//	m_posTeapot[i].z += m_moveTeapot[i].z;
	//	// �s���߂����猴�_�ɖ߂�
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

	// �^���N //////////////////////////////////////////////////////
	m_keyboard->Update();

	Vector3 moveV = Vector3::Zero;
	if (m_keyboard->IsPressed(Keyboard::Keys::A))
	{
		// ����]
		//m_angleTank += 0.02f;
		// ���ƂȂ�I�u�W�F�N�g����]������
		float angle = m_objPlayer[0].GetRotation().y + 0.02f;
		m_objPlayer[0].SetRotation(Vector3(0.0f, angle, 0.0f));
	}
	if (m_keyboard->IsPressed(Keyboard::Keys::D))
	{
		// �E��]
		//m_angleTank -= 0.02f;
		float angle = m_objPlayer[0].GetRotation().y - 0.02f;
		m_objPlayer[0].SetRotation(Vector3(0.0f, angle, 0.0f));
	}
	if (m_keyboard->IsPressed(Keyboard::Keys::Q))
	{
		float angle = m_objPlayer[PLAYER_PARTS_BATTERY].GetRotation().y + 0.02f;
		m_objPlayer[PLAYER_PARTS_BATTERY].SetRotation(Vector3(0.0f, angle, 0.0f));
	}
	if (m_keyboard->IsPressed(Keyboard::Keys::E))
	{
		float angle = m_objPlayer[PLAYER_PARTS_BATTERY].GetRotation().y - 0.02f;
		m_objPlayer[PLAYER_PARTS_BATTERY].SetRotation(Vector3(0.0f, angle, 0.0f));
	}
	if (m_keyboard->IsPressed(Keyboard::Keys::W))
	{
		moveV = Vector3(0.0f, 0.0f, -0.1f);
		// �ړ��x�N�g������]������
		moveV = Vector3::TransformNormal(moveV, m_objPlayer[PLAYER_PARTS_TANK].GetWorld());

		// ��ɑO�i
		//moveV = Vector3(sin(m_angleTank) * -0.1f, 0.0f, cos(m_angleTank) * -0.1f);
	}
	if (m_keyboard->IsPressed(Keyboard::Keys::S))
	{
		moveV = Vector3(0.0f, 0.0f, 0.1f);
		// �ړ��x�N�g������]������
		moveV = Vector3::TransformNormal(moveV, m_objPlayer[PLAYER_PARTS_TANK].GetWorld());

		// ��Ɍ�i
		//moveV = Vector3(sin(m_angleTank) * 0.1f, 0.0f, cos(m_angleTank) * 0.1f);
	}
	Vector3 pos = m_objPlayer[0].GetTranslation();
	pos += moveV;

	// �^���N
	m_objPlayer[PLAYER_PARTS_TANK].SetTranslation(pos);

	// �C��
	m_objPlayer[PLAYER_PARTS_BATTERY].SetTranslation(Vector3(0.0f, 0.4f, 0.0f));

	// ��
	//m_starAngle = m_objPlayer[PLAYER_PARTS_STAR].GetRotation() + Vector3(0.0f, 0.0f, 0.1f);
	m_starAngle += Vector3(0.0f, 0.0f, 0.02f);
	Vector3 starAngle = Vector3::Zero;
	starAngle.z = (1 - cos(m_starAngle.z * XM_PI)) * XM_PI;
	m_objPlayer[PLAYER_PARTS_STAR].SetRotation(starAngle);
	m_objPlayer[PLAYER_PARTS_STAR].SetTranslation(Vector3(0.0f, 1.0f, 0.0f));

	// ��
	m_sinAngle += 0.1f;
	m_objPlayer[PLAYER_PARTS_SHIELD].SetRotation(Vector3(0.0f, m_sinAngle - XM_PIDIV2, 0.0f));
	m_objPlayer[PLAYER_PARTS_SHIELD].SetTranslation(Vector3(sin(m_sinAngle), 0.8f, cos(m_sinAngle)));

	// �h����
	m_objPlayer[PLAYER_PARTS_DRILL].SetTranslation(Vector3(0.0f, 0.2f, -0.7f));
	
	////////////////////////////////////////////////////////////////
	 
	// �J�����̈ʒu�̍X�V //////////////////////////////////////////

	// �J���������@�ɂ��Ă���
	//Vector3 moveCamera(0, 1, 3);
	//moveCamera = Vector3::TransformNormal(moveCamera, m_worldTank);
	//m_camera->SetEyePos(m_posTank + moveCamera);
	//m_camera->SetRefPos(m_posTank);

	//m_camera->Update();
	//m_view = m_camera->GetView();
	//m_proj = m_camera->GetProjection();
	
	Vector3 target_pos = m_objPlayer[PLAYER_PARTS_TANK].GetTranslation();
	m_camera->SetTargetPos(target_pos);
	float target_angle = m_objPlayer[PLAYER_PARTS_TANK].GetRotation().y;
	m_camera->SetTargetAngle(target_angle);

	m_camera->Update();
	m_view = m_camera->GetView();
	m_proj = m_camera->GetProjection();

	////////////////////////////////////////////////////////////////

	// 3D�I�u�W�F�N�g�̍X�V ////////////////////////////////////////

	m_objSkydome.Update();
	m_objGround.Update();

	for (std::vector<Obj3d>::iterator it = m_objPlayer.begin(); it != m_objPlayer.end() ; it++)
	{
		it->Update();
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
	// �`��͂����ɏ���
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
	
	//// �J�����̈ʒu
	//Vector3 eyepos(0.0f, 0.0f, 5.0f);
	//// �ǂ����݂邩
	//Vector3 refpos = Vector3::Zero;
	//// ������x�N�g��
	//Vector3 upvec(1, -1, 0);
	//upvec.Normalize();
	//m_view = Matrix::CreateLookAt(eyepos, refpos, upvec);
	//m_view = m_debugCamera->GetCameraMatrix();

	//// ������������p(�㉺���ꂼ�ꉽ�x�܂Ŏʂ���)
	//float fovY = XMConvertToRadians(60.0f);
	//// ��ʃT�C�Y�̔䗦
	//float aspect = (float)m_outputWidth / m_outputHeight;
	//// ��O�̕\�����E
	//float nearClip = 0.1f;
	//// ���̕\�����E
	//float farClip = 1000.0f;
	//m_proj = Matrix::CreatePerspectiveFieldOfView(fovY, aspect, nearClip, farClip);

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

	m_effect->Apply(m_d3dContext.Get());
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	// �V���̕`��
	m_objSkydome.Render();
	
	// �n�ʂ̕`��
	//for (int i = 0; i < 40000; i++)
	//{
	//	m_modelGround->Draw(m_d3dContext.Get(), *m_states, m_worldGround[i], m_view, m_proj);
	//}
	m_objGround.Render();
	
	//for (int i = 0; i < 20; i++)
	//{
	//	// ���̕`��
	//	m_modelSphere->Draw(m_d3dContext.Get(), *m_states, m_worldSphere[i], m_view, m_proj);
	//}

	//for (int i = 0; i < 20; i++)
	//{
	//	// �e�B�[�|�b�g�̕`��
	//	m_modelTeapot->Draw(m_d3dContext.Get(), *m_states, m_worldTeapot[i], m_view, m_proj);
	//}

	// �^���N�̕`��
	for (std::vector<Obj3d>::iterator it = m_objPlayer.begin(); it != m_objPlayer.end(); it++)
	{
		it->Render();
	}

	m_batch->Begin();

	//m_batch->DrawLine(VertexPositionColor(Vector3(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f)),
	//	VertexPositionColor(Vector3(100.0f, 80.0f, 0.0f), Color(1.0f, 1.0f, 1.0f)));
	// 2D���W
	//VertexPositionColor v1(Vector3(400.f, 150.f, 0.f), Colors::Chocolate);
	//VertexPositionColor v2(Vector3(600.f, 450.f, 0.f), Colors::Blue);
	//VertexPositionColor v3(Vector3(200.f, 450.f, 0.f), Colors::Yellow);
	// 3D���W
	//VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::Yellow);
	//VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::Yellow);
	//VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::Yellow);

	//m_batch->DrawTriangle(v1, v2, v3);

	m_batch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, 6, vertices, 4);

	m_batch->End();

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