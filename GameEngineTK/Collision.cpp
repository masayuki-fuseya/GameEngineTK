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



//**********************************************************************
//!	@brief		3点の座標から三角形を計算する(反時計回りに頂点を指定)
//!
//!	@param[in]	3点の座標,　三角形(結果出力用)
//!
//!	@return		なし
//**********************************************************************
void ComputeTriangle(const Vector3& _p0, const Vector3& _p1, const Vector3& _p2, Triangle* _triangle)
{
	_triangle->P0 = _p0;
	_triangle->P1 = _p1;
	_triangle->P2 = _p2;

	// 法線ベクトルを外積を使って計算
	Vector3 P0_P1 = _p1 - _p0;
	Vector3 P1_P2 = _p2 - _p1;
	_triangle->Normal = P0_P1.Cross(P1_P2);
	_triangle->Normal.Normalize();
}