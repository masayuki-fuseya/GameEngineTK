//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡
//!	@file	Collision.h
//!
//!	@brief	衝突判定ライブラリのヘッダファイル
//!
//!	@date	2017/06/15
//!
//!	@author	Masayuki Fuseya
//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡

// 多重インクルードの防止 ==================================================
#pragma once

// ヘッダファイルの読み込み ================================================
#include <d3d11.h>
#include <d3d11_1.h>
#include <SimpleMath.h>


class Sphere
{
public:
	DirectX::SimpleMath::Vector3 m_center;
	float m_radius;
public:
	Sphere()
	{
		m_radius = 1.0f;
	}
};

// 線分
class Segment
{
public:
	// 始点座標
	DirectX::SimpleMath::Vector3 m_start;
	// 終点座標
	DirectX::SimpleMath::Vector3 m_end;
};

class Triangle
{
public:
	DirectX::SimpleMath::Vector3	P0;
	DirectX::SimpleMath::Vector3	P1;
	DirectX::SimpleMath::Vector3	P2;
	DirectX::SimpleMath::Vector3	Normal;	// 法線ベクトル
};

bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB);

void ComputeTriangle(const DirectX::SimpleMath::Vector3& _p0, const DirectX::SimpleMath::Vector3& _p1,
	const DirectX::SimpleMath::Vector3& _p2, Triangle* _triangle);

bool CheckSphere2Triangle(const Sphere& _sphere, const Triangle& _triangle, DirectX::SimpleMath::Vector3* _inter);

bool CheckSegment2Triangle(const Segment& _segment, const Triangle& _triangle, DirectX::SimpleMath::Vector3* _inter);
