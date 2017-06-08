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
	, m_timer(60)
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
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		rnd *= 180.0f;
		m_distAngle += rnd;
	}

	// �ڕW�p�x�Ɍ������āA�@�̂̊p�x�����킶����
	{
		Vector3 rotv = m_parts[PLAYER_PARTS_TANK].GetRotation();
		float angle = GetShortAngleRad(rotv.y, XMConvertToRadians(m_distAngle));
		rotv.y += angle * 0.01f;
		m_parts[PLAYER_PARTS_TANK].SetRotation(rotv);
	}

	// �@�̂̌����Ă�������ɐi��
	{
		Vector3 trans = m_parts[PLAYER_PARTS_TANK].GetTranslation();

		Vector3 move(0.0f, 0.0f, -0.02f);
		Vector3 rotv = m_parts[PLAYER_PARTS_TANK].GetRotation();
		Matrix rotm = Matrix::CreateRotationY(rotv.y);
		move = Vector3::TransformNormal(move, rotm);
		trans += move;
		m_parts[PLAYER_PARTS_TANK].SetTranslation(trans);
	}


	//Vector3 moveV = Vector3::Zero;

	//// �p�[�c�𓮂���
	//MoveParts(moveV);

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