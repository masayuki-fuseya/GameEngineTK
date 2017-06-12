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
	: m_scale(1.0f)
	, m_pObjParent(nullptr)
	, m_useQuaternion(false)
{
}



//**********************************************************************
//!	@brief		モデルを読み込む
//!
//!	@param[in]	ファイル名
//!
//!	@return		なし
//**********************************************************************
void Obj3d::LoadModel(const wchar_t* fileName)
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
	Matrix scalemat = Matrix::CreateScale(m_scale);

	Matrix rotmat;
	if (m_useQuaternion)
	{
		// クォータニオンから回転行列を計算
		rotmat = Matrix::CreateFromQuaternion(m_quaternion);
	}
	else
	{
		// オイラー角から回転行列を計算(Z-X-Y)
		Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);
		Matrix rotmatX = Matrix::CreateRotationX(m_rotation.x);
		Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);
		rotmat = rotmatZ * rotmatX * rotmatY;
	}

	Matrix transmat = Matrix::CreateTranslation(m_translation);

	m_world = scalemat * rotmat * transmat;
	// 親がいれば親の行列分移動
	if (m_pObjParent)
	{
		m_world *= m_pObjParent->GetWorld();
	}
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
	if (m_model)
	{
		m_model->Draw(m_d3dContext.Get(), *m_states, m_world, m_pCamera->GetView(), m_pCamera->GetProjection());
	}
}