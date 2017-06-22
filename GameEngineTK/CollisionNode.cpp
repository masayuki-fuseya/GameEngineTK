//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�
//!	@file	CollisionNode.cpp
//!
//!	@brief	�����蔻��m�[�h�̃\�[�X�t�@�C��
//!
//!	@date	2017/06/19
//!
//!	@author	Masayuki Fuseya
//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "CollisionNode.h"

using namespace DirectX::SimpleMath;

//**********************************************************************
//!	@brief		�e��ݒ肷��
//!
//!	@param[in]	�e
//!
//!	@return		�Ȃ�
//**********************************************************************
void CollisionNode::SetParent(Obj3d* parent)
{
	m_obj.SetObjParent(parent);
}



//**********************************************************************
//!	@brief		�R���X�g���N�^
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
SphereNode::SphereNode()
{
	m_localRadius = 1.0f;
}



//**********************************************************************
//!	@brief		����������
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
void SphereNode::Initialize()
{
	m_obj.LoadModel(L"Resources\\SphereNode.cmo");
}



//**********************************************************************
//!	@brief		�X�V����
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
void SphereNode::Update()
{
	m_obj.SetTranslation(m_translation);
	m_obj.SetScale(Vector3(m_localRadius));

	m_obj.Update();

	{// �����蔻�苅�̗v�f���v�Z
		const Matrix& worldm = m_obj.GetWorld();

		// ���f�����W�n�ł̒��S�_
		Vector3 center = Vector3::Zero;
		Vector3 right(1.0f, 0.0f, 0.0f);

		// ���[���h���W�ɕϊ�
		center = Vector3::Transform(center, worldm);
		right = Vector3::Transform(right, worldm);

		Sphere::m_center = center;
		Sphere::m_radius = Vector3::Distance(center, right);
	}
}



//**********************************************************************
//!	@brief		�`�揈��
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
void SphereNode::Render()
{
	m_obj.Render();
}
