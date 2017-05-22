//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡
//!	@file	Obj3d.cpp
//!
//!	@brief	3Dオブジェクトクラスのソースファイル
//!
//!	@date	2017/05/22
//!
//!	@author	Masayuki Fuseya
//≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡≡

// ヘッダファイルの読み込み ================================================
#include "Obj3d.h"


using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera* Obj3d::m_pCamera;
std::unique_ptr<CommonStates> Obj3d::m_states;
Microsoft::WRL::ComPtr<ID3D11Device> Obj3d::m_d3dDevice;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> Obj3d::m_d3dContext;
std::unique_ptr<EffectFactory> Obj3d::m_factory;


//**********************************************************************
//!	@brief		静的メンバ変数を初期化する
//!
//!	@param[in]	カメラのポインタ,　デバイス,　コンテキスト
//!
//!	@return		なし
//**********************************************************************
void Obj3d::InitializeStatic(Camera* pCamera,
	Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext)
{
	m_pCamera = pCamera;
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	//ファイルの読み込みパスを指定
	m_factory->SetDirectory(L"Resources");
}



//**********************************************************************
//!	@brief		コンストラクタ
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
Obj3d::Obj3d()
	: m_scale(0.0f)
	, m_rotate(Vector3::Zero)
	, m_trans(Vector3::Zero)
	, m_world(Matrix::Identity)
{
}



//**********************************************************************
//!	@brief		デストラクタ
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
Obj3d::~Obj3d()
{
}



//**********************************************************************
//!	@brief		モデルを読み込む
//!
//!	@param[in]	ファイル名
//!
//!	@return		なし
//**********************************************************************
void Obj3d::LoadModel(const wchar_t * fileName)
{
	m_model = Model::CreateFromCMO(m_d3dDevice.Get(), fileName, *m_factory);
}



//**********************************************************************
//!	@brief		更新処理
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void Obj3d::Update()
{
}



//**********************************************************************
//!	@brief		描画処理
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void Obj3d::Render()
{
	m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	m_d3dContext->RSSetState(m_states->CullNone());
}



//**********************************************************************
//!	@brief		スケールを設定する
//!
//!	@param[in]	スケール
//!
//!	@return		なし
//**********************************************************************
void Obj3d::SetScale(float scale)
{
	m_scale = scale;
}



//**********************************************************************
//!	@brief		回転角を設定する
//!
//!	@param[in]	回転角
//!
//!	@return		なし
//**********************************************************************
void Obj3d::SetRotate(DirectX::SimpleMath::Vector3 rotate)
{
	m_rotate = rotate;
}



//**********************************************************************
//!	@brief		平行移動を設定する
//!
//!	@param[in]	平行移動
//!
//!	@return		なし
//**********************************************************************
void Obj3d::SetTrans(DirectX::SimpleMath::Vector3 trans)
{
	m_trans = trans;
}



//**********************************************************************
//!	@brief		スケールを取得する
//!
//!	@param[in]	なし
//!
//!	@return		スケール
//**********************************************************************
float Obj3d::GetScale()
{
	return m_scale;
}



//**********************************************************************
//!	@brief		回転角を取得する
//!
//!	@param[in]	なし
//!
//!	@return		回転角
//**********************************************************************
Vector3 Obj3d::GetRotate()
{
	return m_rotate;
}



//**********************************************************************
//!	@brief		スケールを取得する
//!
//!	@param[in]	なし
//!
//!	@return		スケール
//**********************************************************************
Vector3 Obj3d::GetTrans()
{
	return m_trans;
}



//**********************************************************************
//!	@brief		ワールド行列を取得する
//!
//!	@param[in]	なし
//!
//!	@return		ワールド行列
//**********************************************************************
Matrix Obj3d::GetWorld()
{
	return m_world;
}
