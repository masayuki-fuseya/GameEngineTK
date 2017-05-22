//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡
//!	@file	Obj3d.h
//!
//!	@brief	3Dオブジェクトクラスのヘッダファイル
//!
//!	@date	2017/05/22
//!
//!	@author	Masayuki Fuseya
//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡

// 多重インクルードの防止 ==================================================
#pragma once

// ヘッダファイルの読み込み ================================================
#include "Camera.h"

#include <CommonStates.h>
#include <Effects.h>
#include <memory>
#include <Model.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include <Windows.h>
#include <wrl.h>


class Obj3d
{
public:
	static void InitializeStatic(Camera* pCamera, Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext);
private:
	static Camera* m_pCamera;
	static Microsoft::WRL::ComPtr<ID3D11Device>				m_d3dDevice;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_d3dContext;
	static std::unique_ptr<DirectX::CommonStates>			m_states;
	static std::unique_ptr<DirectX::EffectFactory>			m_factory;
private:
	// モデル
	std::unique_ptr<DirectX::Model> m_model;
	// スケール
	float m_scale;
	// 回転
	DirectX::SimpleMath::Vector3 m_rotate;
	// 平行移動
	DirectX::SimpleMath::Vector3 m_trans;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ポインタ
	Obj3d* m_pObjParent;
public:
	Obj3d();
	~Obj3d();

	void LoadModel(const wchar_t* fileName);
	void Update();
	void Render();

	void SetScale(float scale);
	void SetRotate(DirectX::SimpleMath::Vector3 rotate);
	void SetTrans(DirectX::SimpleMath::Vector3 trans);

	float GetScale();
	DirectX::SimpleMath::Vector3 GetRotate();
	DirectX::SimpleMath::Vector3 GetTrans();
	DirectX::SimpleMath::Matrix GetWorld();
};
