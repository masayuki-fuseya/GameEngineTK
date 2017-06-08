//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡
//!	@file	FolloCamera.h
//!
//!	@brief	自機に追従するカメラのヘッダファイル
//!
//!	@date	2017/05/15
//!
//!	@author	Masayuki Fuseya
//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡

// 多重インクルードの防止 ==================================================
#pragma once

// ヘッダファイルの読み込み ================================================
#include "Camera.h"
#include "KeyboardUtil.h"
#include "Player.h"

#include <SimpleMath.h>


class FollowCamera :public Camera
{
private:
	// 追従対象の座標
	DirectX::SimpleMath::Vector3 m_targetPos;
	// 追従対象の角度
	float m_targetAngle;
	// 追従対象
	Player* m_player;
	KeyboardUtil* m_keyboard;
	bool m_isTPSViewPoint;		// true:TPS　false:FPS
public:
	// カメラと自機の距離
	static const float CAMERA_DISTANCE;

	FollowCamera(int width, int height);
	~FollowCamera();

	void InitializeTPS();

	void Update() override;
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetPos);
	void SetTargetAngle(const float targetAngle);
	void SetKeyboard(KeyboardUtil* keyboard);
};

