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
#include <wchar.h>
#include <Windows.h>
#include <wrl.h>


class Obj3d
{
public:
	static void InitializeStatic(Camera* pCamera, Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext);
	// 減算描画設定をセット
	static void SetSubtractive();
private:
	static Camera*											s_pCamera;
	static Microsoft::WRL::ComPtr<ID3D11Device>				s_d3dDevice;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>		s_d3dContext;
	static std::unique_ptr<DirectX::CommonStates>			s_states;
	static std::unique_ptr<DirectX::EffectFactory>			s_factory;
	static ID3D11BlendState*								s_pBlendStateSubtract;
protected:
	static bool s_displayFlag;
private:
	// モデル
	std::unique_ptr<DirectX::Model> m_model;
	// スケール
	DirectX::SimpleMath::Vector3 m_scale;
	// 回転(オイラー角)
	DirectX::SimpleMath::Vector3 m_rotation;
	// 回転(クォータニオン)
	DirectX::SimpleMath::Quaternion m_quaternion;
	// 平行移動
	DirectX::SimpleMath::Vector3 m_translation;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// 親のポインタ
	Obj3d* m_pObjParent;
	// 回転をクォータニオンで持つか
	bool m_useQuaternion;
public:
	Obj3d();
	//virtual ~Obj3d();

	void LoadModel(const wchar_t* fileName);
	virtual void Update();
	virtual void Render();

	// 減算描画での描画（影用）
	void DrawSubtractive();
	// オブジェクトのライティングを無効にする
	void DisableLighting();

	void SetScale(const DirectX::SimpleMath::Vector3& scale)
	{
		m_scale = scale;
	}
	void SetQuaternion(const DirectX::SimpleMath::Quaternion& quaternion)
	{
		m_quaternion = quaternion;
		m_useQuaternion = true;
	}
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation)
	{
		m_rotation = rotation;
		m_useQuaternion = false;
	}
	void SetTranslation(const DirectX::SimpleMath::Vector3& translation)
	{
		m_translation = translation;
	}
	void SetWorld(const DirectX::SimpleMath::Matrix& world)
	{
		m_world = world;
	}
	void SetObjParent(Obj3d* pObjParent)
	{
		m_pObjParent = pObjParent;
	}

	const DirectX::SimpleMath::Vector3& GetScale()
	{
		return m_scale;
	}
	const DirectX::SimpleMath::Vector3& GetRotation()
	{
		return m_rotation;
	}
	const DirectX::SimpleMath::Vector3& GetTranslation()
	{
		return m_translation;
	}
	const DirectX::SimpleMath::Matrix& GetWorld()
	{
		return m_world;
	}
	Obj3d* GetObjParent()
	{
		return m_pObjParent;
	}
};
