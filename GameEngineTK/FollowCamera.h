//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�
//!	@file	FolloCamera.h
//!
//!	@brief	���@�ɒǏ]����J�����̃w�b�_�t�@�C��
//!
//!	@date	2017/05/15
//!
//!	@author	Masayuki Fuseya
//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�

// ���d�C���N���[�h�̖h�~ ==================================================
#pragma once

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "Camera.h"
#include "KeyboardUtil.h"
#include "Player.h"

#include <SimpleMath.h>


class FollowCamera :public Camera
{
private:
	// �Ǐ]�Ώۂ̍��W
	DirectX::SimpleMath::Vector3 m_targetPos;
	// �Ǐ]�Ώۂ̊p�x
	float m_targetAngle;
	// �Ǐ]�Ώ�
	Player* m_player;
	KeyboardUtil* m_keyboard;
	bool m_isTPSViewPoint;		// true:TPS�@false:FPS
public:
	// �J�����Ǝ��@�̋���
	static const float CAMERA_DISTANCE;

	FollowCamera(int width, int height);
	~FollowCamera();

	void InitializeTPS();

	void Update() override;
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetPos);
	void SetTargetAngle(const float targetAngle);
	void SetKeyboard(KeyboardUtil* keyboard);
};

