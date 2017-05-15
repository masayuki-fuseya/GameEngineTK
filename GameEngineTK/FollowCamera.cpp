//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡
//!	@file	FollowCamera.cpp
//!
//!	@brief	自機に追従するカメラのソースファイル
//!
//!	@date	2017/05/15
//!
//!	@author	Masayuki Fuseya
//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡

// ヘッダファイルの読み込み ================================================
#include "FollowCamera.h"

using namespace DirectX::SimpleMath;

// 静的メンバ変数の初期化
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

//**********************************************************************
//!	@brief		コンストラクタ
//!
//!	@param[in]	横幅,　縦幅
//!
//!	@return		なし
//**********************************************************************
FollowCamera::FollowCamera(int width, int height)
	: Camera(width, height)
{
	m_targetPos = Vector3::Zero;
	m_targetAngle = 0.0f;
}



//**********************************************************************
//!	@brief		デストラクタ
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
FollowCamera::~FollowCamera()
{
}



//**********************************************************************
//!	@brief		更新処理
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void FollowCamera::Update()
{
	Vector3 eyePos, refPos;

	refPos = m_targetPos + Vector3(0.0f, 2.0f, 0.0f);
	Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);

	Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
	// ベクトルの回転
	cameraV = Vector3::TransformNormal(cameraV, rotmat);

	eyePos = refPos + cameraV;

	SetEyePos(eyePos);
	SetRefPos(refPos);

	Camera::Update();
}



//**********************************************************************
//!	@brief		追従対象の座標を設定する
//!
//!	@param[in]	追従対象の座標
//!
//!	@return		なし
//**********************************************************************
void FollowCamera::SetTargetPos(const Vector3& targetPos)
{
	m_targetPos = targetPos;
}



//**********************************************************************
//!	@brief		追従対象の角度を設定する
//!
//!	@param[in]	追従対象の角度
//!
//!	@return		なし
//**********************************************************************
void FollowCamera::SetTargetAngle(const float targetAngle)
{
	m_targetAngle = targetAngle;
}
