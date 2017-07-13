//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�
//!	@file	Player.h
//!
//!	@brief	�v���C���[�N���X�̃w�b�_�t�@�C��
//!
//!	@date	2017/06/01
//!
//!	@author	Masayuki Fuseya
//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�

// ���d�C���N���[�h�̖h�~ ==================================================
#pragma once

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "Obj3d.h"
#include "KeyboardUtil.h"
#include "CollisionNode.h"

#include <SimpleMath.h>
#include <vector>

class Player :public Obj3d
{
private:
	std::vector<Obj3d> m_parts;
	KeyboardUtil* m_keyboard;
	DirectX::SimpleMath::Vector3 m_starAngle;
	// �^���N�̑��x
	DirectX::SimpleMath::Vector3 m_batteryVel;
	// ���x�x�N�g��
	DirectX::SimpleMath::Vector3 m_velocity;
	float m_sinAngle;
	float m_sinScale;
	bool m_shootFlag;		// ���˒���
	bool m_isJump;
	int m_timer;
	// �����蔻�苅
	SphereNode m_collisionNodeBattery;
	SphereNode m_collisionNodeTank;
public:
	// �d�͉����x
	const float GRAVITY_ACC = 0.03f;
	// �W�����v�̏����x
	const float JUMP_SPEED_FIRST = 0.5f;
	// �W�����v�̍ō����x
	const float JUMP_SPEED_MAX = 0.3f;
public:
	enum PLAYER_PARTS
	{
		PLAYER_PARTS_TANK,
		PLAYER_PARTS_BATTERY,
		PLAYER_PARTS_SHIELD,
		PLAYER_PARTS_DRILL,
		PLAYER_PARTS_STAR,

		PLAYER_PARTS_NUM	// �S�̂̐�
	};

	Player();

	void Update();
	void Calc();
	void Render();

	void SetKeyboard(KeyboardUtil* keyboard)
	{
		m_keyboard = keyboard;
	}
	void SetTranslation(DirectX::SimpleMath::Vector3& translation)
	{
		m_parts[0].SetTranslation(translation);
	}

	const DirectX::SimpleMath::Vector3& GetRotation()
	{
		return m_parts[0].GetRotation();
	}
	const DirectX::SimpleMath::Vector3& GetTranslation()
	{
		return m_parts[0].GetTranslation();
	}
	const SphereNode& GetCollisionNodeBattery()
	{
		return m_collisionNodeBattery;
	}
	const SphereNode& GetCollisionNodeTank()
	{
		return m_collisionNodeTank;
	}
	const DirectX::SimpleMath::Vector3& GetVelocity()
	{
		return m_velocity;
	}

	void StartJump();
	void StartFall();
	void StopJump();
private:
	void ShootBattery();
	void ResetBattery();
	void MoveParts(DirectX::SimpleMath::Vector3 moveV);
};
