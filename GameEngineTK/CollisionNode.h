//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡
//!	@file	CollisionNode.h
//!
//!	@brief	当たり判定ノードのヘッダファイル
//!
//!	@date	2017/06/19
//!
//!	@author	Masayuki Fuseya
//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡

// 多重インクルードの防止 ==================================================
#pragma once

#include "Collision.h"
#include "Obj3d.h"

#include <SimpleMath.h>

class CollisionNode
{
protected:
	// デバッグ表示オブジェクト
	Obj3d m_obj;
	// 親からのオフセット
	DirectX::SimpleMath::Vector3 m_translation;
public:
	static bool GetDebugVisible()
	{
		return m_debugVisible;
	}

	static void SetDebugVisible(bool debugVisible)
	{
		m_debugVisible = debugVisible;
	}
protected:
	static bool m_debugVisible;
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	void SetParent(Obj3d* parent);
	void SetTranslation(const DirectX::SimpleMath::Vector3& translation)
	{
		m_translation = translation;
	}
};

// 球用の当たり判定ノード
class SphereNode :public CollisionNode, public Sphere
{
private:
	// ローカル半径
	float m_localRadius;
public:
	SphereNode();

	void Initialize();
	void Update();
	void Render();

	void SetLocalRadius(float localRadius)
	{
		m_localRadius = localRadius;
	}
};