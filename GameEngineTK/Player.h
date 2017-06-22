//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡
//!	@file	Player.h
//!
//!	@brief	プレイヤークラスのヘッダファイル
//!
//!	@date	2017/06/01
//!
//!	@author	Masayuki Fuseya
//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡

// 多重インクルードの防止 ==================================================
#pragma once

// ヘッダファイルの読み込み ================================================
#include "Obj3d.h"
#include "KeyboardUtil.h"
#include "CollisionNode.h"

#include <SimpleMath.h>
#include <vector>

class Player :public Obj3d
{
private:
	std::vector<Obj3d> m_parts;
	KeyboardUtil* m_keyboard;
	DirectX::SimpleMath::Vector3 m_starAngle;
	// タンクの速度
	DirectX::SimpleMath::Vector3 m_batteryVel;
	float m_sinAngle;
	float m_sinScale;
	bool m_shootFlag;		// 発射中か
	int m_timer;
	SphereNode m_collisionNodeBattery;
public:
	enum PLAYER_PARTS
	{
		PLAYER_PARTS_TANK,
		PLAYER_PARTS_BATTERY,
		PLAYER_PARTS_SHIELD,
		PLAYER_PARTS_DRILL,
		PLAYER_PARTS_STAR,

		PLAYER_PARTS_NUM	// 全体の数
	};

	Player();

	void Update();
	void Render();

	void SetKeyboard(KeyboardUtil* keyboard)
	{
		m_keyboard = keyboard;
	}

	const DirectX::SimpleMath::Vector3& GetRotation()
	{
		return m_parts[0].GetRotation();
	}
	const DirectX::SimpleMath::Vector3& GetTranslation()
	{
		return m_parts[0].GetTranslation();
	}
	const SphereNode& GetCollisionNodeBattery()
	{
		return m_collisionNodeBattery;
	}
private:
	void ShootBattery();
	void ResetBattery();
	void MoveParts(DirectX::SimpleMath::Vector3 moveV);
};
