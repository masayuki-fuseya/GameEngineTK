//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡
//!	@file	Player.cpp
//!
//!	@brief	プレイヤークラスのソースファイル
//!
//!	@date	2017/06/01
//!
//!	@author	Masayuki Fuseya
//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡

// ヘッダファイルの読み込み ================================================
#include "Player.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//**********************************************************************
//!	@brief		コンストラクタ
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
Player::Player()
	: m_keyboard(nullptr)
	, m_starAngle(Vector3::Zero)
	, m_batteryVel(Vector3::Zero)
	, m_velocity(Vector3::Zero)
	, m_sinAngle(0.0f)
	, m_sinScale(1.0f)
	, m_shootFlag(false)
	, m_isJump(false)
	, m_timer(0)
{
	m_parts.resize(PLAYER_PARTS_NUM);
	m_parts[PLAYER_PARTS_TANK].LoadModel(L"Resources\\tank.cmo");
	m_parts[PLAYER_PARTS_BATTERY].LoadModel(L"Resources\\battery.cmo");
	m_parts[PLAYER_PARTS_SHIELD].LoadModel(L"Resources\\shield.cmo");
	m_parts[PLAYER_PARTS_DRILL].LoadModel(L"Resources\\drill.cmo");
	m_parts[PLAYER_PARTS_STAR].LoadModel(L"Resources\\star.cmo");

	// 親子関係の設定
	m_parts[PLAYER_PARTS_BATTERY].SetObjParent(&m_parts[PLAYER_PARTS_TANK]);
	m_parts[PLAYER_PARTS_SHIELD].SetObjParent(&m_parts[PLAYER_PARTS_TANK]);
	m_parts[PLAYER_PARTS_DRILL].SetObjParent(&m_parts[PLAYER_PARTS_TANK]);
	m_parts[PLAYER_PARTS_STAR].SetObjParent(&m_parts[PLAYER_PARTS_BATTERY]);

	// 各パーツの初期位置の設定
	// 砲台
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
		m_collisionNodeBattery.Initialize();
		m_collisionNodeBattery.SetParent(&m_parts[PLAYER_PARTS_BATTERY]);
		m_collisionNodeBattery.SetTranslation(Vector3(0.0f, 0.0f, 0.0f));
		m_collisionNodeBattery.SetLocalRadius(0.5f);

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
void Player::Update()
{
	Vector3 moveV = Vector3::Zero;
	if (m_keyboard->IsPressed(Keyboard::Keys::A))
	{
		// 元となるオブジェクトを回転させる
		float angle = m_parts[0].GetRotation().y + 0.02f;
		m_parts[0].SetRotation(Vector3(0.0f, angle, 0.0f));
	}
	if (m_keyboard->IsPressed(Keyboard::Keys::D))
	{
		float angle = m_parts[0].GetRotation().y - 0.02f;
		m_parts[0].SetRotation(Vector3(0.0f, angle, 0.0f));
	}
	if (m_keyboard->IsPressed(Keyboard::Keys::Q))
	{
		float angle = m_parts[PLAYER_PARTS_BATTERY].GetRotation().y + 0.02f;
		m_parts[PLAYER_PARTS_BATTERY].SetRotation(Vector3(0.0f, angle, 0.0f));
	}
	if (m_keyboard->IsPressed(Keyboard::Keys::E))
	{
		float angle = m_parts[PLAYER_PARTS_BATTERY].GetRotation().y - 0.02f;
		m_parts[PLAYER_PARTS_BATTERY].SetRotation(Vector3(0.0f, angle, 0.0f));
	}
	if (m_keyboard->IsPressed(Keyboard::Keys::W))
	{
		moveV = Vector3(0.0f, 0.0f, -0.1f);
		// 移動ベクトルを回転させる
		moveV = Vector3::TransformNormal(moveV, m_parts[PLAYER_PARTS_TANK].GetWorld());
	}
	if (m_keyboard->IsPressed(Keyboard::Keys::S))
	{
		moveV = Vector3(0.0f, 0.0f, 0.1f);
		// 移動ベクトルを回転させる
		moveV = Vector3::TransformNormal(moveV, m_parts[PLAYER_PARTS_TANK].GetWorld());
	}
	if (m_keyboard->IsTriggered(Keyboard::Keys::Z))
	{
		if (m_timer == 0)
		{
			ShootBattery();
		}
	}
	if (m_keyboard->IsTriggered(Keyboard::Keys::D1))
	{
		s_displayFlag = !s_displayFlag;
	}
	if (m_keyboard->IsTriggered(Keyboard::Keys::Space))
	{
		StartJump();
	}

	if (m_isJump)
	{
		// 重力をかける
		m_velocity.y -= GRAVITY_ACC;
		if (m_velocity.y <= -JUMP_SPEED_MAX)
		{
			m_velocity.y = -JUMP_SPEED_MAX;
		}
	}

	Vector3 trans = GetTranslation();
	trans += m_velocity;
	SetTranslation(trans);

	// パーツを動かす
	MoveParts(moveV);

	Calc();
}



//**********************************************************************
//!	@brief		行列更新
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void Player::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_parts.begin(); it != m_parts.end(); it++)
	{
		it->Obj3d::Update();
	}

	m_collisionNodeBattery.Update();
	m_collisionNodeTank.Update();
}



//**********************************************************************
//!	@brief		描画処理
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void Player::Render()
{
	for (std::vector<Obj3d>::iterator it = m_parts.begin(); it != m_parts.end(); it++)
	{
		it->Obj3d::Render();
	}

	if (s_displayFlag)
	{
		m_collisionNodeBattery.Render();
		m_collisionNodeTank.Render();
	}
}



//**********************************************************************
//!	@brief		ジャンプし始める
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void Player::StartJump()
{
	if (!m_isJump)
	{
		m_velocity.y = JUMP_SPEED_FIRST;
		m_isJump = true;
	}
}



//**********************************************************************
//!	@brief		落下し始める
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void Player::StartFall()
{
	if (!m_isJump)
	{
		m_velocity.y = 0.0f;
		m_isJump = true;
	}
}



//**********************************************************************
//!	@brief		ジャンプを終了する
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void Player::StopJump()
{
	m_isJump = false;
	m_velocity = Vector3::Zero;
}



//**********************************************************************
//!	@brief		砲台を発射する
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void Player::ShootBattery()
{
	if (m_shootFlag)
	{
		return;
	}

	Matrix worldm = m_parts[PLAYER_PARTS_BATTERY].GetWorld();

	Vector3 scale;
	Quaternion quaternion;
	Vector3 trans;

	// スケール行列、回転行列、平行行列を取り出す
	worldm.Decompose(scale, quaternion, trans);

	// 親子関係を切り離して独立させる
	m_parts[PLAYER_PARTS_BATTERY].SetObjParent(nullptr);
	m_parts[PLAYER_PARTS_BATTERY].SetScale(scale);
	m_parts[PLAYER_PARTS_BATTERY].SetQuaternion(quaternion);
	m_parts[PLAYER_PARTS_BATTERY].SetTranslation(trans);

	// 星の位置をそのままに
	m_parts[PLAYER_PARTS_STAR].SetObjParent(&m_parts[PLAYER_PARTS_TANK]);
	trans = Vector3(0.0f, 1.4f, 0.0f);
	m_parts[PLAYER_PARTS_STAR].SetTranslation(trans);

	m_batteryVel = Vector3(0.0f, 0.0f, -0.2f);
	// ベクトルを回転
	m_batteryVel = Vector3::Transform(m_batteryVel, quaternion);

	m_shootFlag = true;
}



//**********************************************************************
//!	@brief		砲台を元の位置に戻す
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void Player::ResetBattery()
{
	if (!m_shootFlag)
	{
		return;
	}

	m_parts[PLAYER_PARTS_BATTERY].SetObjParent(&m_parts[PLAYER_PARTS_TANK]);
	m_parts[PLAYER_PARTS_BATTERY].SetScale(Vector3(1.0f, 1.0f, 1.0f));
	m_parts[PLAYER_PARTS_BATTERY].SetRotation(Vector3(0.0f, 0.0f, 0.0f));
	m_parts[PLAYER_PARTS_BATTERY].SetTranslation(Vector3(0.0f, 0.4f, 0.0f));
	// ワールド行列の更新
	m_parts[PLAYER_PARTS_BATTERY].Update();

	// 星の位置を修正
	m_parts[PLAYER_PARTS_STAR].SetObjParent(&m_parts[PLAYER_PARTS_BATTERY]);
	m_parts[PLAYER_PARTS_STAR].SetTranslation(Vector3(0.0f, 1.0f, 0.0f));

	// 速度を初期化
	m_batteryVel = Vector3(0.0f, 0.0f, 0.0f);

	m_shootFlag = false;
}



//**********************************************************************
//!	@brief		プレイヤーパーツを動かす
//!
//!	@param[in]	移動ベクトル
//!
//!	@return		なし
//**********************************************************************
void Player::MoveParts(DirectX::SimpleMath::Vector3 moveV)
{
	// タンク
	Vector3 pos = m_parts[0].GetTranslation();
	pos += moveV;
	m_parts[PLAYER_PARTS_TANK].SetTranslation(pos);

	if (m_shootFlag)
	{
		// 砲台
		pos = m_parts[PLAYER_PARTS_BATTERY].GetTranslation();
		pos += m_batteryVel;
		m_parts[PLAYER_PARTS_BATTERY].SetTranslation(pos);
		m_timer++;
		if (m_timer == 120)
		{
			ResetBattery();
			m_timer = 0;
		}
	}

	// 盾
	m_sinAngle += 0.1f;
	m_parts[PLAYER_PARTS_SHIELD].SetRotation(Vector3(0.0f, m_sinAngle - XM_PIDIV2, 0.0f));
	m_parts[PLAYER_PARTS_SHIELD].SetTranslation(Vector3(sin(m_sinAngle), 0.8f, cos(m_sinAngle)));

	// ドリル
	m_sinScale += 0.05f;
	Vector3 drillScall(1.0f, 1.0f, 1.0f);
	drillScall.z = (1 - cos(m_sinScale * XM_PI)) * 3.0f + 1.0f;
	m_parts[PLAYER_PARTS_DRILL].SetScale(drillScall);

	// 星
	//m_starAngle = m_objPlayer[PLAYER_PARTS_STAR].GetRotation() + Vector3(0.0f, 0.0f, 0.1f);
	m_starAngle += Vector3(0.0f, 0.0f, 0.02f);
	Vector3 starAngle = Vector3::Zero;
	starAngle.z = (1 - cos(m_starAngle.z * XM_PI)) * XM_PI;
	m_parts[PLAYER_PARTS_STAR].SetRotation(starAngle);
}
