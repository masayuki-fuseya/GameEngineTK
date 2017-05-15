//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡
//!	@file	Camera.h
//!
//!	@brief	カメラクラスのヘッダファイル
//!
//!	@date	2017/05/11
//!
//!	@author	Masayuki Fuseya
//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡

// 多重インクルードの防止 ==================================================
#pragma once

// ヘッダファイルの読み込み ================================================
#include <d3d11.h>
#include <SimpleMath.h>

class Camera
{
protected:
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	// カメラの位置
	DirectX::SimpleMath::Vector3 m_eyePos;
	// どこをみるか
	DirectX::SimpleMath::Vector3 m_refPos;
	// 上方向ベクトル
	DirectX::SimpleMath::Vector3 m_upVec;
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

	virtual void Update();
	// ビュー座標を取得
	// 書き換えられない参照を返す
	const DirectX::SimpleMath::Matrix& GetView();
	// 射影座標を取得
	const DirectX::SimpleMath::Matrix& GetProjection();
	void SetEyePos(const DirectX::SimpleMath::Vector3& eyePos);
	void SetRefPos(const DirectX::SimpleMath::Vector3& refPos);
	void SetUpVec(const DirectX::SimpleMath::Vector3& upVec);
	void SetFovY(const float fovY);
	void SetAspect(const float aspect);
	void SetNearClip(const float nearClip);
	void SetFarClip(const float farClip);
private:
	void CalcMatrix();
};
