//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�
//!	@file	Enemy.cpp
//!
//!	@brief	�G�l�~�[�N���X�̃\�[�X�t�@�C��
//!
//!	@date	2017/06/05
//!
//!	@author	Masayuki Fuseya
//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//**********************************************************************
//!	@brief		�R���X�g���N�^
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
Enemy::Enemy()
	: m_starAngle(Vector3::Zero)
	, m_sinAngle(0.0f)
	, m_sinScale(1.0f)
	, m_timer(0)
	, m_distAngle(0.0f)
{
	m_parts.resize(PLAYER_PARTS_NUM);
	m_parts[PLAYER_PARTS_TANK].LoadModel(L"Resources\\tank.cmo");
	m_parts[PLAYER_PARTS_BATTERY].LoadModel(L"Resources\\battery.cmo");
	m_parts[PLAYER_PARTS_STAR].LoadModel(L"Resources\\star.cmo");
	m_parts[PLAYER_PARTS_SHIELD].LoadModel(L"Resources\\shield.cmo");
	m_parts[PLAYER_PARTS_DRILL].LoadModel(L"Resources\\drill.cmo");

	// �e�q�֌W�̐ݒ�
	m_parts[PLAYER_PARTS_BATTERY].SetObjParent(&m_parts[PLAYER_PARTS_TANK]);
	m_parts[PLAYER_PARTS_STAR].SetObjParent(&m_parts[PLAYER_PARTS_BATTERY]);
	m_parts[PLAYER_PARTS_SHIELD].SetObjParent(&m_parts[PLAYER_PARTS_TANK]);
	m_parts[PLAYER_PARTS_DRILL].SetObjParent(&m_parts[PLAYER_PARTS_TANK]);

	// �e�p�[�c�̏����ʒu�̐ݒ�
	// �C��
	Vector3 pos;
	pos.x = rand() % 20 - 10;
	pos.z = rand() % 20 - 10;
 	m_parts[PLAYER_PARTS_TANK].SetTranslation(pos);
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

	{
		m_collisionNodeTank.Initialize();
		m_collisionNodeTank.SetParent(&m_parts[PLAYER_PARTS_TANK]);
		m_collisionNodeTank.SetTranslation(Vector3(0.0f, 0.4f, 0.0f));
		m_collisionNodeTank.SetLocalRadius(0.7f);
	}
}



//**********************************************************************
//!	@brief		�X�V����
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
void Enemy::Update()
{
	m_timer--;
	if (m_timer < 0)
	{
		m_timer = 60;

		// �ڕW�p�x��ύX
		// -5 �` +5
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		// -90 �` +90
		rnd *= 180.0f;
		rnd = XMConvertToRadians(rnd);
		m_distAngle += rnd;
	}

	{
		// ���ƂȂ�I�u�W�F�N�g����]������
		Vector3 rot = GetRotation();

		float angle = m_distAngle - rot.y;

		// �ŒZ�p�x��ڎw��
		if (angle > XM_PI)
		{
			angle -= XM_2PI;
		}
		if (angle < -XM_PI)
		{
			angle += XM_2PI;
		}

		rot.y += angle * 0.01f;

		SetRotation(rot);
	}
	
	// �@�̂̌����Ă�������ɐi��
	Vector3 moveV = Vector3::Zero;
	{
		moveV = Vector3(0.0f, 0.0f, -0.1f);
		float angle = m_parts[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		// �ړ��x�N�g������]������
		moveV = Vector3::TransformNormal(moveV, rotmat);
	}

	// �p�[�c�𓮂���
	MoveParts(moveV);

	Calc();

	m_collisionNodeTank.Update();
}



//**********************************************************************
//!	@brief		�s��X�V
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
void Enemy::Calc()
{
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
void Enemy::Render()
{
	for (std::vector<Obj3d>::iterator it = m_parts.begin(); it != m_parts.end(); it++)
	{
		it->Obj3d::Render();
	}

	if (s_displayFlag)
	{
		m_collisionNodeTank.Render();
	}
}



//**********************************************************************
//!	@brief		�ڕW�p�x�ւ̍ŒZ�p�x���擾����
//!
//!	@param[in]	�ڕW�p�x,�@�p�x
//!
//!	@return		�ŒZ�p�x
//**********************************************************************
float Enemy::GetShortAngleRad(float targetAngle, float radian)
{
	

	return 0.0f;
}



//**********************************************************************
//!	@brief		�G�l�~�[�p�[�c�𓮂���
//!
//!	@param[in]	�ړ��x�N�g��
//!
//!	@return		�Ȃ�
//**********************************************************************
void Enemy::MoveParts(DirectX::SimpleMath::Vector3 moveV)
{
	Vector3 pos = m_parts[0].GetTranslation();
	pos += moveV;

	// �^���N
	m_parts[PLAYER_PARTS_TANK].SetTranslation(pos);

	// �C��
	m_parts[PLAYER_PARTS_BATTERY].SetTranslation(Vector3(0.0f, 0.4f, 0.0f));

	// ��
	//m_starAngle = m_objPlayer[PLAYER_PARTS_STAR].GetRotation() + Vector3(0.0f, 0.0f, 0.1f);
	m_starAngle += Vector3(0.0f, 0.0f, 0.02f);
	Vector3 starAngle = Vector3::Zero;
	starAngle.z = (1 - cos(m_starAngle.z * XM_PI)) * XM_PI;
	m_parts[PLAYER_PARTS_STAR].SetRotation(starAngle);
	m_parts[PLAYER_PARTS_STAR].SetTranslation(Vector3(0.0f, 1.0f, 0.0f));

	// ��
	m_sinAngle += 0.1f;
	m_parts[PLAYER_PARTS_SHIELD].SetRotation(Vector3(0.0f, m_sinAngle - XM_PIDIV2, 0.0f));
	m_parts[PLAYER_PARTS_SHIELD].SetTranslation(Vector3(sin(m_sinAngle), 0.8f, cos(m_sinAngle)));

	// �h����
	m_sinScale += 0.05f;
	Vector3 drillScall(1.0f, 1.0f, 1.0f);
	drillScall.z = (1 - cos(m_sinScale * XM_PI)) * 3.0f + 1.0f;
	m_parts[PLAYER_PARTS_DRILL].SetScale(drillScall);
	m_parts[PLAYER_PARTS_DRILL].SetTranslation(Vector3(0.0f, 0.2f, -0.7f));
}