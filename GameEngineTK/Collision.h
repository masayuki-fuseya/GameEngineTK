//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�
//!	@file	Collision.h
//!
//!	@brief	�Փ˔��胉�C�u�����̃w�b�_�t�@�C��
//!
//!	@date	2017/06/15
//!
//!	@author	Masayuki Fuseya
//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�

// ���d�C���N���[�h�̖h�~ ==================================================
#pragma once

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include <d3d11.h>
#include <d3d11_1.h>
#include <SimpleMath.h>


class Sphere
{
public:
	DirectX::SimpleMath::Vector3 m_center;
	float m_radius;
public:
	Sphere()
	{
		m_radius = 1.0f;
	}
};

// ����
class Segment
{
public:
	// �n�_���W
	DirectX::SimpleMath::Vector3 m_start;
	// �I�_���W
	DirectX::SimpleMath::Vector3 m_end;
};

bool CheckSphere2Sphere(const Sphere & sphereA, const Sphere & sphereB);
