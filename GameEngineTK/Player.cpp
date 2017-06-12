//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�
//!	@file	Player.cpp
//!
//!	@brief	�v���C���[�N���X�̃\�[�X�t�@�C��
//!
//!	@date	2017/06/01
//!
//!	@author	Masayuki Fuseya
//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//**********************************************************************
//!	@brief		�R���X�g���N�^
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
Player::Player()
	: m_keyboard(nullptr)
	, m_starAngle(Vector3::Zero)
	, m_batteryVel(Vector3::Zero)
	, m_sinAngle(0.0f)
	, m_sinScale(1.0f)
	, m_shootFlag(false)
{
	m_parts.resize(PLAYER_PARTS_NUM);
	m_parts[PLAYER_PARTS_TANK].LoadModel(L"Resources\\tank.cmo");
	m_parts[PLAYER_PARTS_BATTERY].LoadModel(L"Resources\\battery.cmo");
	m_parts[PLAYER_PARTS_SHIELD].LoadModel(L"Resources\\shield.cmo");
	m_parts[PLAYER_PARTS_DRILL].LoadModel(L"Resources\\drill.cmo");
	m_parts[PLAYER_PARTS_STAR].LoadModel(L"Resources\\star.cmo");

	// �e�q�֌W�̐ݒ�
	m_parts[PLAYER_PARTS_BATTERY].SetObjParent(&m_parts[PLAYER_PARTS_TANK]);
	m_parts[PLAYER_PARTS_SHIELD].SetObjParent(&m_parts[PLAYER_PARTS_TANK]);
	m_parts[PLAYER_PARTS_DRILL].SetObjParent(&m_parts[PLAYER_PARTS_TANK]);
	m_parts[PLAYER_PARTS_STAR].SetObjParent(&m_parts[PLAYER_PARTS_BATTERY]);

	// �e�p�[�c�̏����ʒu�̐ݒ�
	// �C��
	m_parts[PLAYER_PARTS_BATTERY].SetTranslation(Vector3(0.0f, 0.4f, 0.0f));

	// ��
	m_parts[PLAYER_PARTS_SHIELD].SetRotation(Vector3(0.0f, m_sinAngle - XM_PIDIV2, 0.0f));
	m_parts[PLAYER_PARTS_SHIELD].SetTranslation(Vector3(sin(m_sinAngle), 0.8f, cos(m_sinAngle)));

	// �h����
	m_parts[PLAYER_PARTS_DRILL].SetTranslation(Vector3(0.0f, 0.2f, -0.7f));

	// ��
	Vector3 starAngle = Vector3::Zero;
	starAngle.z = (1 - cos(m_starAngle.z * XM_PI)) * XM_PI;
	m_parts[PLAYER_PARTS_STAR].SetRotation(starAngle);
	m_parts[PLAYER_PARTS_STAR].SetTranslation(Vector3(0.0f, 1.0f, 0.0f));
}



//**********************************************************************
//!	@brief		�X�V����
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
void Player::Update()
{
	Vector3 moveV = Vector3::Zero;
	if (m_keyboard->IsPressed(Keyboard::Keys::A))
	{
		// ���ƂȂ�I�u�W�F�N�g����]������
		float angle = m_parts[0].GetRotation().y + 0.02f;
		m_parts[0].SetRotation(Vector3(0.0f, angle, 0.0f));
	}
	if (m_keyboard->IsPressed(Keyboard::Keys::D))
	{
		float angle = m_parts[0].GetRotation().y - 0.02f;
		m_parts[0].SetRotation(Vector3(0.0f, angle, 0.0f));
	}
	if (m_keyboard->IsPressed(Keyboard::Keys::Q))
	{
		float angle = m_parts[PLAYER_PARTS_BATTERY].GetRotation().y + 0.02f;
		m_parts[PLAYER_PARTS_BATTERY].SetRotation(Vector3(0.0f, angle, 0.0f));
	}
	if (m_keyboard->IsPressed(Keyboard::Keys::E))
	{
		float angle = m_parts[PLAYER_PARTS_BATTERY].GetRotation().y - 0.02f;
		m_parts[PLAYER_PARTS_BATTERY].SetRotation(Vector3(0.0f, angle, 0.0f));
	}
	if (m_keyboard->IsPressed(Keyboard::Keys::W))
	{
		moveV = Vector3(0.0f, 0.0f, -0.1f);
		// �ړ��x�N�g������]������
		moveV = Vector3::TransformNormal(moveV, m_parts[PLAYER_PARTS_TANK].GetWorld());
	}
	if (m_keyboard->IsPressed(Keyboard::Keys::S))
	{
		moveV = Vector3(0.0f, 0.0f, 0.1f);
		// �ړ��x�N�g������]������
		moveV = Vector3::TransformNormal(moveV, m_parts[PLAYER_PARTS_TANK].GetWorld());
	}
	if (m_keyboard->IsTriggered(Keyboard::Keys::Space))
	{
		if (m_shootFlag)
		{
			ResetBattery();
		}
		else
		{
			ShootBattery();
		}
	}

	// �p�[�c�𓮂���
	MoveParts(moveV);

	for (std::vector<Obj3d>::iterator it = m_parts.begin(); it != m_parts.end(); it++)
	{
		it->Obj3d::Update();
	}
}



//**********************************************************************
//!	@brief		�`�揈��
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
void Player::Render()
{
	for (std::vector<Obj3d>::iterator it = m_parts.begin(); it != m_parts.end(); it++)
	{
		it->Obj3d::Render();
	}
}



//**********************************************************************
//!	@brief		�C��𔭎˂���
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
void Player::ShootBattery()
{
	if (m_shootFlag)
	{
		return;
	}

	Matrix worldm = m_parts[PLAYER_PARTS_BATTERY].GetWorld();

	Vector3 scale;
	Quaternion quaternion;
	Vector3 trans;

	// �X�P�[���s��A��]�s��A���s�s������o��
	worldm.Decompose(scale, quaternion, trans);

	// �e�q�֌W��؂藣���ēƗ�������
	m_parts[PLAYER_PARTS_BATTERY].SetObjParent(nullptr);
	m_parts[PLAYER_PARTS_BATTERY].SetScale(scale);
	m_parts[PLAYER_PARTS_BATTERY].SetQuaternion(quaternion);
	m_parts[PLAYER_PARTS_BATTERY].SetTranslation(trans);

	// ���̈ʒu�����̂܂܂�
	m_parts[PLAYER_PARTS_STAR].SetObjParent(&m_parts[PLAYER_PARTS_TANK]);
	trans = Vector3(0.0f, 1.4f, 0.0f);
	m_parts[PLAYER_PARTS_STAR].SetTranslation(trans);

	m_batteryVel = Vector3(0.0f, 0.0f, -0.2f);
	// �x�N�g������]
	m_batteryVel = Vector3::Transform(m_batteryVel, quaternion);

	m_shootFlag = true;
}



//**********************************************************************
//!	@brief		�C������̈ʒu�ɖ߂�
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
void Player::ResetBattery()
{
	if (!m_shootFlag)
	{
		return;
	}

	m_parts[PLAYER_PARTS_BATTERY].SetObjParent(&m_parts[PLAYER_PARTS_TANK]);
	m_parts[PLAYER_PARTS_BATTERY].SetScale(Vector3(1.0f, 1.0f, 1.0f));
	m_parts[PLAYER_PARTS_BATTERY].SetRotation(Vector3(0.0f, 0.0f, 0.0f));
	m_parts[PLAYER_PARTS_BATTERY].SetTranslation(Vector3(0.0f, 0.4f, 0.0f));
	// ���[���h�s��̍X�V
	m_parts[PLAYER_PARTS_BATTERY].Update();

	// ���̈ʒu���C��
	m_parts[PLAYER_PARTS_STAR].SetObjParent(&m_parts[PLAYER_PARTS_BATTERY]);
	m_parts[PLAYER_PARTS_STAR].SetTranslation(Vector3(0.0f, 1.0f, 0.0f));

	// ���x��������
	m_batteryVel = Vector3(0.0f, 0.0f, 0.0f);

	m_shootFlag = false;
}



//**********************************************************************
//!	@brief		�v���C���[�p�[�c�𓮂���
//!
//!	@param[in]	�ړ��x�N�g��
//!
//!	@return		�Ȃ�
//**********************************************************************
void Player::MoveParts(DirectX::SimpleMath::Vector3 moveV)
{
	// �^���N
	Vector3 pos = m_parts[0].GetTranslation();
	pos += moveV;
	m_parts[PLAYER_PARTS_TANK].SetTranslation(pos);

	// �C��
	pos = m_parts[PLAYER_PARTS_BATTERY].GetTranslation();
	pos += m_batteryVel;
	m_parts[PLAYER_PARTS_BATTERY].SetTranslation(pos);

	// ��
	m_sinAngle += 0.1f;
	m_parts[PLAYER_PARTS_SHIELD].SetRotation(Vector3(0.0f, m_sinAngle - XM_PIDIV2, 0.0f));
	m_parts[PLAYER_PARTS_SHIELD].SetTranslation(Vector3(sin(m_sinAngle), 0.8f, cos(m_sinAngle)));

	// �h����
	m_sinScale += 0.05f;
	Vector3 drillScall(1.0f, 1.0f, 1.0f);
	drillScall.z = (1 - cos(m_sinScale * XM_PI)) * 3.0f + 1.0f;
	m_parts[PLAYER_PARTS_DRILL].SetScale(drillScall);

	// ��
	//m_starAngle = m_objPlayer[PLAYER_PARTS_STAR].GetRotation() + Vector3(0.0f, 0.0f, 0.1f);
	m_starAngle += Vector3(0.0f, 0.0f, 0.02f);
	Vector3 starAngle = Vector3::Zero;
	starAngle.z = (1 - cos(m_starAngle.z * XM_PI)) * XM_PI;
	m_parts[PLAYER_PARTS_STAR].SetRotation(starAngle);
}
