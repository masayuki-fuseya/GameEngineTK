//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡
//!	@file	Collision.cpp
//!
//!	@brief	衝突判定ライブラリのソースファイル
//!
//!	@date	2017/06/15
//!
//!	@author	Masayuki Fuseya
//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡

// ヘッダファイルの読み込み ================================================
#include "Collision.h"


using namespace DirectX::SimpleMath;

//**********************************************************************
//!	@brief		球同士の当たり判定
//!
//!	@param[in]	球A,　球B
//!
//!	@return		true:当たっている　false:当たってない
//**********************************************************************
bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	Vector3 sub = sphereB.m_center - sphereA.m_center;

	//sub.Length();
	// ベクトルの長さを計算する
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;
	// 半径の和を計算する
	float radiusSquare = sphereA.m_radius + sphereB.m_radius;
	radiusSquare *= radiusSquare;

	if (distanceSquare > radiusSquare)
	{
		return false;
	}

	return true;
}