//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡
//!	@file	Camera.h
//!
//!	@brief	カメラクラスのヘッダファイル
//!
//!	@date	2017/05/11
//!
//!	@author	Masayuki Fuseya
//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡

#pragma once

#include <d3d11.h>
#include <SimpleMath.h>

class Camera
{
protected:
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	// カメラの位置
	DirectX::SimpleMath::Vector3 m_eyepos;
	// どこをみるか
	DirectX::SimpleMath::Vector3 m_refpos;
	// 上方向ベクトル
	DirectX::SimpleMath::Vector3 m_upvec;
	// 垂直方向視野角(上下それぞれ何度まで写すか)
	float m_fovY;
	// 画面サイズの比率
	float m_aspect;
	// 手前の表示限界
	float m_nearClip;
	// 奥の表示限界
	float m_farClip;
public:
	Camera(int width, int height);
	virtual ~Camera();

	void Update();
	// ビュー座標を取得
	DirectX::SimpleMath::Matrix GetViewMatrix();
	// 射影座標を取得
	DirectX::SimpleMath::Matrix GetProjectionMatrix();
	void SetEyePos(DirectX::SimpleMath::Vector3 eyePos);
	void SetRefPos(DirectX::SimpleMath::Vector3 refPos);
	void SetUpVec(DirectX::SimpleMath::Vector3 upVec);
	void SetFovY(float fovY);
	void SetAspect(float aspect);
	void SetNearClip(float nearClip);
	void SetFarClip(float farClip);
};
