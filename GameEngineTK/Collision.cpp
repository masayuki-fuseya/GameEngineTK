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



//**********************************************************************
//!	@brief		3�_�̍��W����O�p�`���v�Z����(�����v���ɒ��_���w��)
//!
//!	@param[in]	3�_�̍��W,�@�O�p�`(���ʏo�͗p)
//!
//!	@return		�Ȃ�
//**********************************************************************
void ComputeTriangle(const Vector3& _p0, const Vector3& _p1, const Vector3& _p2, Triangle* _triangle)
{
	_triangle->P0 = _p0;
	_triangle->P1 = _p1;
	_triangle->P2 = _p2;

	// �@���x�N�g�����O�ς��g���Čv�Z
	Vector3 P0_P1 = _p1 - _p0;
	Vector3 P1_P2 = _p2 - _p1;
	_triangle->Normal = P0_P1.Cross(P1_P2);
	_triangle->Normal.Normalize();
}