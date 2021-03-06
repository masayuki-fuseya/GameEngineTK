//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
//!	@file	FolloCamera.h
//!
//!	@brief	©@ÉÇ]·éJÌwb_t@C
//!
//!	@date	2017/05/15
//!
//!	@author	Masayuki Fuseya
//ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

// ½dCN[hÌh~ ==================================================
#pragma once

// wb_t@CÌÇÝÝ ================================================
#include "Camera.h"
#include "KeyboardUtil.h"
#include "Player.h"

#include <SimpleMath.h>


class FollowCamera :public Camera
{
private:
	// Ç]ÎÛÌÀW
	DirectX::SimpleMath::Vector3 m_targetPos;
	// Ç]ÎÛÌpx
	float m_targetAngle;
	// Ç]ÎÛ
	Player* m_player;
	KeyboardUtil* m_keyboard;
	bool m_isTPSViewPoint;		// true:TPS@false:FPS
public:
	// JÆ©@Ì£
	static const float CAMERA_DISTANCE;

	FollowCamera(int width, int height);
	~FollowCamera();

	void InitializeTPS();

	void Update() override;
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetPos);
	void SetTargetAngle(const float targetAngle);
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	void SetKeyboard(KeyboardUtil* keyboard);
};

