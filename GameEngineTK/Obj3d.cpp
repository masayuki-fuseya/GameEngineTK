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

Camera* Obj3d::s_pCamera;
std::unique_ptr<CommonStates> Obj3d::s_states;
Microsoft::WRL::ComPtr<ID3D11Device> Obj3d::s_d3dDevice;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> Obj3d::s_d3dContext;
std::unique_ptr<EffectFactory> Obj3d::s_factory;
ID3D11BlendState* Obj3d::s_pBlendStateSubtract;
bool Obj3d::s_displayFlag;


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
	s_pCamera = pCamera;
	s_d3dDevice = d3dDevice;
	s_d3dContext = d3dContext;
	s_states = std::make_unique<CommonStates>(s_d3dDevice.Get());
	s_factory = std::make_unique<EffectFactory>(s_d3dDevice.Get());
	//ファイルの読み込みパスを指定
	s_factory->SetDirectory(L"Resources");

	// 減算描画用のブレンドステートを作成
	D3D11_BLEND_DESC desc;
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_REV_SUBTRACT;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HRESULT ret = s_d3dDevice->CreateBlendState(&desc, &s_pBlendStateSubtract);

	s_displayFlag = false;
}



//**********************************************************************
//!	@brief		減算描画を設定
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void Obj3d::SetSubtractive()
{
	s_d3dContext->OMSetBlendState(s_pBlendStateSubtract, nullptr, 0xffffff);
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

//Obj3d::~Obj3d()
//{
//}



//**********************************************************************
//!	@brief		モデルを読み込む
//!
//!	@param[in]	ファイル名
//!
//!	@return		なし
//**********************************************************************
void Obj3d::LoadModel(const wchar_t* fileName)
{
	m_model = Model::CreateFromCMO(s_d3dDevice.Get(), fileName, *s_factory);
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
		m_model->Draw(s_d3dContext.Get(), *s_states, m_world, s_pCamera->GetView(), s_pCamera->GetProjection());
	}
}



//**********************************************************************
//!	@brief		減算描画で描画
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void Obj3d::DrawSubtractive()
{
	if (m_model)
	{
		assert(s_pCamera);
		const Matrix& view = s_pCamera->GetView();
		const Matrix& projection = s_pCamera->GetProjection();

		assert(s_d3dContext);
		assert(s_states);

		// 減算描画用の設定関数を渡して描画
		m_model->Draw(s_d3dContext.Get(), *s_states, m_world, view, projection, false, Obj3d::SetSubtractive);
	}
}



//**********************************************************************
//!	@brief		ライティングを無効にする
//!
//!	@param[in]	なし
//!
//!	@return		なし
//**********************************************************************
void Obj3d::DisableLighting()
{
	if (m_model)
	{
		// モデル内の全メッシュ分回す
		ModelMesh::Collection::const_iterator it_mesh = m_model->meshes.begin();
		for (; it_mesh != m_model->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// メッシュ内の全メッシュパーツ分回す
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// メッシュパーツにセットされたエフェクトをBasicEffectとして取得
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());
				if (eff != nullptr)
				{
					// 自己発光を最大値に
					eff->SetEmissiveColor(Vector3(1, 1, 1));

					// エフェクトに含む全ての平行光源分について処理する
					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)
					{
						// ライトを無効にする
						eff->SetLightEnabled(i, false);
					}
				}
			}
		}
	}
}
