//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡
//!	@file	CollisionNode.cpp
//!
//!	@brief	当たり判定ノードのソースファイル
//!
//!	@date	2017/06/19
//!
//!	@author	Masayuki Fuseya
//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡

// ヘッダファイルの読み込み ================================================
#include "CollisionNode.h"

using namespace DirectX::SimpleMath;

//**********************************************************************
//!	@brief		親を設定する
//!
//!	@param[in]	親
//!
//!	@return		なし
//**********************************************************************
void CollisionNode::SetParent(Obj3d* parent)
{
	m_obj.SetObjParent(parent);
}



//**********************************************************************
//!	@brief		コンストラクタ
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
SphereNode::SphereNode()
{
	m_localRadius = 1.0f;
}



//**********************************************************************
//!	@brief		初期化処理
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void SphereNode::Initialize()
{
	m_obj.LoadModel(L"Resources\\SphereNode.cmo");
}



//**********************************************************************
//!	@brief		更新処理
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void SphereNode::Update()
{
	m_obj.SetTranslation(m_translation);
	m_obj.SetScale(Vector3(m_localRadius));

	m_obj.Update();

	{// 当たり判定球の要素を計算
		const Matrix& worldm = m_obj.GetWorld();

		// モデル座標系での中心点
		Vector3 center = Vector3::Zero;
		Vector3 right(1.0f, 0.0f, 0.0f);

		// ワールド座標に変換
		center = Vector3::Transform(center, worldm);
		right = Vector3::Transform(right, worldm);

		Sphere::m_center = center;
		Sphere::m_radius = Vector3::Distance(center, right);
	}
}



//**********************************************************************
//!	@brief		描画処理
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void SphereNode::Render()
{
	m_obj.Render();
}
