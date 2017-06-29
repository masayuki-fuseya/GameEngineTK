//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�
//!	@file	Obj3d.cpp
//!
//!	@brief	3D�I�u�W�F�N�g�N���X�̃\�[�X�t�@�C��
//!
//!	@date	2017/05/22
//!
//!	@author	Masayuki Fuseya
//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
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
//!	@brief		�ÓI�����o�ϐ�������������
//!
//!	@param[in]	�J�����̃|�C���^,�@�f�o�C�X,�@�R���e�L�X�g
//!
//!	@return		�Ȃ�
//**********************************************************************
void Obj3d::InitializeStatic(Camera* pCamera,
	Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext)
{
	s_pCamera = pCamera;
	s_d3dDevice = d3dDevice;
	s_d3dContext = d3dContext;
	s_states = std::make_unique<CommonStates>(s_d3dDevice.Get());
	s_factory = std::make_unique<EffectFactory>(s_d3dDevice.Get());
	//�t�@�C���̓ǂݍ��݃p�X���w��
	s_factory->SetDirectory(L"Resources");

	// ���Z�`��p�̃u�����h�X�e�[�g���쐬
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
//!	@brief		���Z�`���ݒ�
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
void Obj3d::SetSubtractive()
{
	s_d3dContext->OMSetBlendState(s_pBlendStateSubtract, nullptr, 0xffffff);
}



//**********************************************************************
//!	@brief		�R���X�g���N�^
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
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
//!	@brief		���f����ǂݍ���
//!
//!	@param[in]	�t�@�C����
//!
//!	@return		�Ȃ�
//**********************************************************************
void Obj3d::LoadModel(const wchar_t* fileName)
{
	m_model = Model::CreateFromCMO(s_d3dDevice.Get(), fileName, *s_factory);
}



//**********************************************************************
//!	@brief		�X�V����
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
void Obj3d::Update()
{
	Matrix scalemat = Matrix::CreateScale(m_scale);

	Matrix rotmat;
	if (m_useQuaternion)
	{
		// �N�H�[�^�j�I�������]�s����v�Z
		rotmat = Matrix::CreateFromQuaternion(m_quaternion);
	}
	else
	{
		// �I�C���[�p�����]�s����v�Z(Z-X-Y)
		Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);
		Matrix rotmatX = Matrix::CreateRotationX(m_rotation.x);
		Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);
		rotmat = rotmatZ * rotmatX * rotmatY;
	}

	Matrix transmat = Matrix::CreateTranslation(m_translation);

	m_world = scalemat * rotmat * transmat;
	// �e������ΐe�̍s�񕪈ړ�
	if (m_pObjParent)
	{
		m_world *= m_pObjParent->GetWorld();
	}
}



//**********************************************************************
//!	@brief		�`�揈��
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
void Obj3d::Render()
{
	if (m_model)
	{
		m_model->Draw(s_d3dContext.Get(), *s_states, m_world, s_pCamera->GetView(), s_pCamera->GetProjection());
	}
}



//**********************************************************************
//!	@brief		���Z�`��ŕ`��
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
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

		// ���Z�`��p�̐ݒ�֐���n���ĕ`��
		m_model->Draw(s_d3dContext.Get(), *s_states, m_world, view, projection, false, Obj3d::SetSubtractive);
	}
}



//**********************************************************************
//!	@brief		���C�e�B���O�𖳌��ɂ���
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
void Obj3d::DisableLighting()
{
	if (m_model)
	{
		// ���f�����̑S���b�V������
		ModelMesh::Collection::const_iterator it_mesh = m_model->meshes.begin();
		for (; it_mesh != m_model->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// ���b�V�����̑S���b�V���p�[�c����
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// ���b�V���p�[�c�ɃZ�b�g���ꂽ�G�t�F�N�g��BasicEffect�Ƃ��Ď擾
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());
				if (eff != nullptr)
				{
					// ���Ȕ������ő�l��
					eff->SetEmissiveColor(Vector3(1, 1, 1));

					// �G�t�F�N�g�Ɋ܂ޑS�Ă̕��s�������ɂ��ď�������
					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)
					{
						// ���C�g�𖳌��ɂ���
						eff->SetLightEnabled(i, false);
					}
				}
			}
		}
	}
}
