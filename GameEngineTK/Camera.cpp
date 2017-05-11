//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡
//!	@file	Camera.cpp
//!
//!	@brief	カメラクラスのソースファイル
//!
//!	@date	2017/05/11
//!
//!	@author	Masayuki Fuseya
//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡

#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera(int width, int height)
{
	m_eyepos = Vector3(0.0f, 0.0f, 5.0f);
	m_refpos = Vector3::Zero;
	m_upvec = Vector3::UnitY;
	// カメラ座標, カメラの向き, カメラの回転（ゲームワールドの上向きが３Ｄ上のどこを上にするか）
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
	// ビュー行列の更新
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);
	// 射影行列の更新
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
