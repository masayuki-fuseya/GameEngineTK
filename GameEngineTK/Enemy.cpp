//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡
//!	@file	Enemy.cpp
//!
//!	@brief	エネミークラスのソースファイル
//!
//!	@date	2017/06/05
//!
//!	@author	Masayuki Fuseya
//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡

// ヘッダファイルの読み込み ================================================
#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//**********************************************************************
//!	@brief		コンストラクタ
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
Enemy::Enemy()
	: m_starAngle(Vector3::Zero)
	, m_sinAngle(0.0f)
	, m_sinScale(1.0f)
	, m_timer(0)
	, m_distAngle(0.0f)
{
	m_parts.resize(PLAYER_PARTS_NUM);
	m_parts[PLAYER_PARTS_TANK].LoadModel(L"Resources\\tank.cmo");
	m_parts[PLAYER_PARTS_BATTERY].LoadModel(L"Resources\\battery.cmo");
	m_parts[PLAYER_PARTS_STAR].LoadModel(L"Resources\\star.cmo");
	m_parts[PLAYER_PARTS_SHIELD].LoadModel(L"Resources\\shield.cmo");
	m_parts[PLAYER_PARTS_DRILL].LoadModel(L"Resources\\drill.cmo");

	// 親子関係の設定
	m_parts[PLAYER_PARTS_BATTERY].SetObjParent(&m_parts[PLAYER_PARTS_TANK]);
	m_parts[PLAYER_PARTS_STAR].SetObjParent(&m_parts[PLAYER_PARTS_BATTERY]);
	m_parts[PLAYER_PARTS_SHIELD].SetObjParent(&m_parts[PLAYER_PARTS_TANK]);
	m_parts[PLAYER_PARTS_DRILL].SetObjParent(&m_parts[PLAYER_PARTS_TANK]);

	// 各パーツの初期位置の設定
	// 砲台
	Vector3 pos;
	pos.x = rand() % 20 - 10;
	pos.z = rand() % 20 - 10;
 	m_parts[PLAYER_PARTS_TANK].SetTranslation(pos);
	m_parts[PLAYER_PARTS_BATTERY].SetTranslation(Vector3(0.0f, 0.4f, 0.0f));

	// 盾
	m_parts[PLAYER_PARTS_SHIELD].SetRotation(Vector3(0.0f, m_sinAngle - XM_PIDIV2, 0.0f));
	m_parts[PLAYER_PARTS_SHIELD].SetTranslation(Vector3(sin(m_sinAngle), 0.8f, cos(m_sinAngle)));

	// ドリル
	m_parts[PLAYER_PARTS_DRILL].SetTranslation(Vector3(0.0f, 0.2f, -0.7f));

	// 星
	Vector3 starAngle = Vector3::Zero;
	starAngle.z = (1 - cos(m_starAngle.z * XM_PI)) * XM_PI;
	m_parts[PLAYER_PARTS_STAR].SetRotation(starAngle);
	m_parts[PLAYER_PARTS_STAR].SetTranslation(Vector3(0.0f, 1.0f, 0.0f));

	{
		m_collisionNodeTank.Initialize();
		m_collisionNodeTank.SetParent(&m_parts[PLAYER_PARTS_TANK]);
		m_collisionNodeTank.SetTranslation(Vector3(0.0f, 0.4f, 0.0f));
		m_collisionNodeTank.SetLocalRadius(0.7f);
	}
}



//**********************************************************************
//!	@brief		更新処理
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void Enemy::Update()
{
	m_timer--;
	if (m_timer < 0)
	{
		m_timer = 60;

		// 目標角度を変更
		// -5 ～ +5
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		// -90 ～ +90
		rnd *= 180.0f;
		rnd = XMConvertToRadians(rnd);
		m_distAngle += rnd;
	}

	{
		// 元となるオブジェクトを回転させる
		Vector3 rot = GetRotation();

		float angle = m_distAngle - rot.y;

		// 最短角度を目指す
		if (angle > XM_PI)
		{
			angle -= XM_2PI;
		}
		if (angle < -XM_PI)
		{
			angle += XM_2PI;
		}

		rot.y += angle * 0.01f;

		SetRotation(rot);
	}
	
	// 機体の向いている方向に進む
	Vector3 moveV = Vector3::Zero;
	{
		moveV = Vector3(0.0f, 0.0f, -0.1f);
		float angle = m_parts[0].GetRotation().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		// 移動ベクトルを回転させる
		moveV = Vector3::TransformNormal(moveV, rotmat);
	}

	// パーツを動かす
	MoveParts(moveV);

	Calc();

	m_collisionNodeTank.Update();
}



//**********************************************************************
//!	@brief		行列更新
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void Enemy::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_parts.begin(); it != m_parts.end(); it++)
	{
		it->Obj3d::Update();
	}
}



//**********************************************************************
//!	@brief		描画処理
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void Enemy::Render()
{
	for (std::vector<Obj3d>::iterator it = m_parts.begin(); it != m_parts.end(); it++)
	{
		it->Obj3d::Render();
	}

	if (s_displayFlag)
	{
		m_collisionNodeTank.Render();
	}
}



//**********************************************************************
//!	@brief		目標角度への最短角度を取得する
//!
//!	@param[in]	目標角度,　角度
//!
//!	@return		最短角度
//**********************************************************************
float Enemy::GetShortAngleRad(float targetAngle, float radian)
{
	

	return 0.0f;
}



//**********************************************************************
//!	@brief		エネミーパーツを動かす
//!
//!	@param[in]	移動ベクトル
//!
//!	@return		なし
//**********************************************************************
void Enemy::MoveParts(DirectX::SimpleMath::Vector3 moveV)
{
	Vector3 pos = m_parts[0].GetTranslation();
	pos += moveV;

	// タンク
	m_parts[PLAYER_PARTS_TANK].SetTranslation(pos);

	// 砲台
	m_parts[PLAYER_PARTS_BATTERY].SetTranslation(Vector3(0.0f, 0.4f, 0.0f));

	// 星
	//m_starAngle = m_objPlayer[PLAYER_PARTS_STAR].GetRotation() + Vector3(0.0f, 0.0f, 0.1f);
	m_starAngle += Vector3(0.0f, 0.0f, 0.02f);
	Vector3 starAngle = Vector3::Zero;
	starAngle.z = (1 - cos(m_starAngle.z * XM_PI)) * XM_PI;
	m_parts[PLAYER_PARTS_STAR].SetRotation(starAngle);
	m_parts[PLAYER_PARTS_STAR].SetTranslation(Vector3(0.0f, 1.0f, 0.0f));

	// 盾
	m_sinAngle += 0.1f;
	m_parts[PLAYER_PARTS_SHIELD].SetRotation(Vector3(0.0f, m_sinAngle - XM_PIDIV2, 0.0f));
	m_parts[PLAYER_PARTS_SHIELD].SetTranslation(Vector3(sin(m_sinAngle), 0.8f, cos(m_sinAngle)));

	// ドリル
	m_sinScale += 0.05f;
	Vector3 drillScall(1.0f, 1.0f, 1.0f);
	drillScall.z = (1 - cos(m_sinScale * XM_PI)) * 3.0f + 1.0f;
	m_parts[PLAYER_PARTS_DRILL].SetScale(drillScall);
	m_parts[PLAYER_PARTS_DRILL].SetTranslation(Vector3(0.0f, 0.2f, -0.7f));
}