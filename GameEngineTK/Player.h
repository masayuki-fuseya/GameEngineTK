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
	float m_sinAngle;
	float m_sinScale;
	bool m_shootFlag;		// ���˒���
	int m_timer;
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
	void Render();

	void SetKeyboard(KeyboardUtil* keyboard)
	{
		m_keyboard = keyboard;
	}

	const DirectX::SimpleMath::Vector3& GetRotation()
	{
		return m_parts[0].GetRotation();
	}
	const DirectX::SimpleMath::Vector3& GetTranslation()
	{
		return m_parts[0].GetTranslation();
	}

private:
	void ShootBattery();
	void ResetBattery();
	void MoveParts(DirectX::SimpleMath::Vector3 moveV);
};
