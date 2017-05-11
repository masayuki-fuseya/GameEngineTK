//
// Game.h
//

#pragma once

#include "Camera.h"
#include "DebugCamera.h"
#include "KeyboardUtil.h"
#include "StepTimer.h"
#include <CommonStates.h>
#include <Effects.h>
#include <Model.h>
#include <PrimitiveBatch.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include <VertexTypes.h>


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

	Game();

	// Initialization and management
	void Initialize(HWND window, int width, int height);

	// Basic game loop
	void Tick();

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged(int width, int height);

	// Properties
	void GetDefaultSize(int& width, int& height) const;

private:

	void Update(DX::StepTimer const& timer);
	void Render();

	void Clear();
	void Present();

	void CreateDevice();
	void CreateResources();

	void OnDeviceLost();

	// cos補間
	DirectX::SimpleMath::Matrix Lerp(float startScale, float targetScale, float t);
	float cosCurve(float time);

	// Device resources.
	HWND                                            m_window;
	int                                             m_outputWidth;
	int                                             m_outputHeight;

	D3D_FEATURE_LEVEL                               m_featureLevel;
	Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

	Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
	Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

	// Rendering loop timer.
	DX::StepTimer                                   m_timer;

	std::unique_ptr<DirectX::CommonStates> m_states;
	//std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>> m_batch;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<DebugCamera> m_debugCamera;

	std::unique_ptr<DirectX::EffectFactory> m_factory;
	std::unique_ptr<DirectX::Model> m_modelSkydome;
	std::unique_ptr<DirectX::Model> m_modelGround;
	std::unique_ptr<DirectX::Model> m_modelSphere;
	std::unique_ptr<DirectX::Model> m_modelTeapot;
	std::unique_ptr<DirectX::Model> m_modelTank;
	// 球用のワールド行列
	DirectX::SimpleMath::Matrix m_worldSphere[20];
	// 球用の角度
	float m_angleSphere;
	// ティーポット用の角度
	float m_angleTeapot;
	// タンク用の角度
	float m_angleTank;
	// ティーポットの座標
	DirectX::SimpleMath::Vector3 m_posTeapot[20];
	// ティーポットの移動用変数
	DirectX::SimpleMath::Vector3 m_moveTeapot[20];
	// ティーポットのワールド行列
	DirectX::SimpleMath::Matrix m_worldTeapot[20];
	// 地面用のワールド行列
	//DirectX::SimpleMath::Matrix m_worldGround[40000];
	// タンクの座標
	DirectX::SimpleMath::Vector3 m_posTank;
	// タンクのワールド行列
	DirectX::SimpleMath::Matrix m_worldTank;
	// キーボード
	std::unique_ptr<KeyboardUtil> m_keyboard;
	
	// 3次補間用の時間
	float m_time;
};