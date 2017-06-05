//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�
//!	@file	Enemy.h
//!
//!	@brief	�G�l�~�[�N���X�̃w�b�_�t�@�C��
//!
//!	@date	2017/06/05
//!
//!	@author	Masayuki Fuseya
//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�

// ���d�C���N���[�h�̖h�~ ==================================================
#pragma once

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "Obj3d.h"
#include "KeyboardUtil.h"

#include <SimpleMath.h>
#include <vector>

class Enemy :public Obj3d
{
private:
	std::vector<Obj3d> m_parts;
	DirectX::SimpleMath::Vector3 m_starAngle;
	float m_sinAngle;
	float m_sinScale;
	int m_timer;
	float m_distAngle;
public:
	enum PLAYER_PARTS
	{
		PLAYER_PARTS_TANK,
		PLAYER_PARTS_BATTERY,
		PLAYER_PARTS_STAR,
		PLAYER_PARTS_SHIELD,
		PLAYER_PARTS_DRILL,

		PLAYER_PARTS_NUM	// �S�̂̐�
	};

	Enemy();

	void Update();
	void Render();
private:
	float GetShortAngleRad(float targetAngle, float radian);
	void MoveParts(DirectX::SimpleMath::Vector3 moveV);
};