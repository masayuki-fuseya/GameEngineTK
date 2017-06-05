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
	float m_sinAngle;
	float m_sinScale;
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

	Player();

	void Update();
	void Render();

	void SetKeyboard(KeyboardUtil* keyboard);
private:
	void MoveParts(DirectX::SimpleMath::Vector3 moveV);
};
