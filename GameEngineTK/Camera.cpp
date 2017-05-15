//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�
//!	@file	Camera.cpp
//!
//!	@brief	�J�����N���X�̃\�[�X�t�@�C��
//!
//!	@date	2017/05/11
//!
//!	@author	Masayuki Fuseya
//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�

#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


//**********************************************************************
//!	@brief		�R���X�g���N�^
//!
//!	@param[in]	����,�@�c��
//!
//!	@return		�Ȃ�
//**********************************************************************
Camera::Camera(int width, int height)
{
	m_eyePos = Vector3(0.0f, 0.0f, 5.0f);
	m_refPos = Vector3::Zero;
	m_upVec = Vector3::UnitY;
	// �J�������W, �J�����̌���, �J�����̉�]�i�Q�[�����[���h�̏�������R�c��̂ǂ�����ɂ��邩�j
	m_view = Matrix::CreateLookAt(m_eyePos, m_refPos, m_upVec);

	m_fovY = XMConvertToRadians(60.0f);
	m_aspect = (float)width / height;
	m_nearClip = 0.1f;
	m_farClip = 1000.0f;
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearClip, m_farClip);
}



//**********************************************************************
//!	@brief		�f�X�g���N�^
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
Camera::~Camera()
{
}



//**********************************************************************
//!	@brief		�X�V����
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
void Camera::Update()
{
	// �s��̌v�Z
	CalcMatrix();
}



//**********************************************************************
//!	@brief		�r���[�s����擾����
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�r���[�s��
//**********************************************************************
const Matrix& Camera::GetView()
{
	return m_view;
}



//**********************************************************************
//!	@brief		�v���W�F�N�V�����s����擾����
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�v���W�F�N�V�����s��
//**********************************************************************
const Matrix& Camera::GetProjection()
{
	return m_proj;
}



//**********************************************************************
//!	@brief		�J�����̈ʒu��ݒ肷��
//!
//!	@param[in]	�J�����̈ʒu
//!
//!	@return		�Ȃ�
//**********************************************************************
void Camera::SetEyePos(const Vector3& eyePos)
{
	m_eyePos = eyePos;
}



//**********************************************************************
//!	@brief		�J�����̌���������ݒ肷��
//!
//!	@param[in]	�J�����̌�������
//!
//!	@return		�Ȃ�
//**********************************************************************
void Camera::SetRefPos(const Vector3& refPos)
{
	m_refPos = refPos;
}



//**********************************************************************
//!	@brief		������x�N�g����ݒ肷��
//!
//!	@param[in]	������x�N�g��
//!
//!	@return		�Ȃ�
//**********************************************************************
void Camera::SetUpVec(const Vector3& upVec)
{
	m_upVec = upVec;
}



//**********************************************************************
//!	@brief		������������p��ݒ肷��
//!
//!	@param[in]	������������p
//!
//!	@return		�Ȃ�
//**********************************************************************
void Camera::SetFovY(const float fovY)
{
	m_fovY = fovY;
}



//**********************************************************************
//!	@brief		��ʃT�C�Y�̔䗦��ݒ肷��
//!
//!	@param[in]	��ʃT�C�Y�̔䗦
//!
//!	@return		�Ȃ�
//**********************************************************************
void Camera::SetAspect(const float aspect)
{
	m_aspect = aspect;
}



//**********************************************************************
//!	@brief		��O�̕\�����E��ݒ肷��
//!
//!	@param[in]	��O�̕\�����E�̈ʒu
//!
//!	@return		�Ȃ�
//**********************************************************************
void Camera::SetNearClip(const float nearClip)
{
	m_nearClip = nearClip;
}



//**********************************************************************
//!	@brief		���̕\�����E��ݒ肷��
//!
//!	@param[in]	���̕\�����E�̈ʒu
//!
//!	@return		�Ȃ�
//**********************************************************************
void Camera::SetFarClip(const float farClip)
{
	m_farClip = farClip;
}



//**********************************************************************
//!	@brief		�s����v�Z����
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
void Camera::CalcMatrix()
{
	// �r���[�s��̍X�V
	m_view = Matrix::CreateLookAt(m_eyePos, m_refPos, m_upVec);
	// �ˉe�s��̍X�V
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearClip, m_farClip);
}
