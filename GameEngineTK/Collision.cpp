//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�
//!	@file	Collision.cpp
//!
//!	@brief	�Փ˔��胉�C�u�����̃\�[�X�t�@�C��
//!
//!	@date	2017/06/15
//!
//!	@author	Masayuki Fuseya
//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "Collision.h"


using namespace DirectX::SimpleMath;

//**********************************************************************
//!	@brief		�����m�̓����蔻��
//!
//!	@param[in]	��A,�@��B
//!
//!	@return		true:�������Ă���@false:�������ĂȂ�
//**********************************************************************
bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	Vector3 sub = sphereB.m_center - sphereA.m_center;

	//sub.Length();
	// �x�N�g���̒������v�Z����
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;
	// ���a�̘a���v�Z����
	float radiusSquare = sphereA.m_radius + sphereB.m_radius;
	radiusSquare *= radiusSquare;

	if (distanceSquare > radiusSquare)
	{
		return false;
	}

	return true;
}