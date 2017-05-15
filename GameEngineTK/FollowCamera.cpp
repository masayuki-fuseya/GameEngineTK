//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�
//!	@file	FollowCamera.cpp
//!
//!	@brief	���@�ɒǏ]����J�����̃\�[�X�t�@�C��
//!
//!	@date	2017/05/15
//!
//!	@author	Masayuki Fuseya
//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "FollowCamera.h"

using namespace DirectX::SimpleMath;

// �ÓI�����o�ϐ��̏�����
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

//**********************************************************************
//!	@brief		�R���X�g���N�^
//!
//!	@param[in]	����,�@�c��
//!
//!	@return		�Ȃ�
//**********************************************************************
FollowCamera::FollowCamera(int width, int height)
	: Camera(width, height)
{
	m_targetPos = Vector3::Zero;
	m_targetAngle = 0.0f;
}



//**********************************************************************
//!	@brief		�f�X�g���N�^
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
FollowCamera::~FollowCamera()
{
}



//**********************************************************************
//!	@brief		�X�V����
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
void FollowCamera::Update()
{
	Vector3 eyePos, refPos;

	refPos = m_targetPos + Vector3(0.0f, 2.0f, 0.0f);
	Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);

	Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
	// �x�N�g���̉�]
	cameraV = Vector3::TransformNormal(cameraV, rotmat);

	eyePos = refPos + cameraV;

	SetEyePos(eyePos);
	SetRefPos(refPos);

	Camera::Update();
}



//**********************************************************************
//!	@brief		�Ǐ]�Ώۂ̍��W��ݒ肷��
//!
//!	@param[in]	�Ǐ]�Ώۂ̍��W
//!
//!	@return		�Ȃ�
//**********************************************************************
void FollowCamera::SetTargetPos(const Vector3& targetPos)
{
	m_targetPos = targetPos;
}



//**********************************************************************
//!	@brief		�Ǐ]�Ώۂ̊p�x��ݒ肷��
//!
//!	@param[in]	�Ǐ]�Ώۂ̊p�x
//!
//!	@return		�Ȃ�
//**********************************************************************
void FollowCamera::SetTargetAngle(const float targetAngle)
{
	m_targetAngle = targetAngle;
}
