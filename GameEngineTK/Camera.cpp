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

Camera::Camera(int width, int height)
{
	m_eyepos = Vector3(0.0f, 0.0f, 5.0f);
	m_refpos = Vector3::Zero;
	m_upvec = Vector3::UnitY;
	// �J�������W, �J�����̌���, �J�����̉�]�i�Q�[�����[���h�̏�������R�c��̂ǂ�����ɂ��邩�j
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);

	m_fovY = XMConvertToRadians(60.0f);
	m_aspect = (float)width / height;
	m_nearClip = 0.1f;
	m_farClip = 1000.0f;
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearClip, m_farClip);
}



Camera::~Camera()
{
}



void Camera::Update()
{
	// �r���[�s��̍X�V
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);
	// �ˉe�s��̍X�V
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearClip, m_farClip);
}



Matrix Camera::GetViewMatrix()
{
	return m_view;
}



Matrix Camera::GetProjectionMatrix()
{
	return m_proj;
}



void Camera::SetEyePos(Vector3 eyePos)
{
	m_eyepos = eyePos;
}



void Camera::SetRefPos(Vector3 refPos)
{
	m_refpos = refPos;
}



void Camera::SetUpVec(Vector3 upVec)
{
	m_upvec = upVec;
}



void Camera::SetFovY(float fovY)
{
	m_fovY = fovY;
}



void Camera::SetAspect(float aspect)
{
	m_aspect = aspect;
}



void Camera::SetNearClip(float nearClip)
{
	m_nearClip = nearClip;
}



void Camera::SetFarClip(float farClip)
{
	m_farClip = farClip;
}
