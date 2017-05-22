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

Camera* Obj3d::m_pCamera;
std::unique_ptr<CommonStates> Obj3d::m_states;
Microsoft::WRL::ComPtr<ID3D11Device> Obj3d::m_d3dDevice;
Microsoft::WRL::ComPtr<ID3D11DeviceContext> Obj3d::m_d3dContext;
std::unique_ptr<EffectFactory> Obj3d::m_factory;


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
	m_pCamera = pCamera;
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	//�t�@�C���̓ǂݍ��݃p�X���w��
	m_factory->SetDirectory(L"Resources");
}



//**********************************************************************
//!	@brief		�R���X�g���N�^
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
Obj3d::Obj3d()
	: m_scale(0.0f)
	, m_rotate(Vector3::Zero)
	, m_trans(Vector3::Zero)
	, m_world(Matrix::Identity)
{
}



//**********************************************************************
//!	@brief		�f�X�g���N�^
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�Ȃ�
//**********************************************************************
Obj3d::~Obj3d()
{
}



//**********************************************************************
//!	@brief		���f����ǂݍ���
//!
//!	@param[in]	�t�@�C����
//!
//!	@return		�Ȃ�
//**********************************************************************
void Obj3d::LoadModel(const wchar_t * fileName)
{
	m_model = Model::CreateFromCMO(m_d3dDevice.Get(), fileName, *m_factory);
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
	m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	m_d3dContext->RSSetState(m_states->CullNone());
}



//**********************************************************************
//!	@brief		�X�P�[����ݒ肷��
//!
//!	@param[in]	�X�P�[��
//!
//!	@return		�Ȃ�
//**********************************************************************
void Obj3d::SetScale(float scale)
{
	m_scale = scale;
}



//**********************************************************************
//!	@brief		��]�p��ݒ肷��
//!
//!	@param[in]	��]�p
//!
//!	@return		�Ȃ�
//**********************************************************************
void Obj3d::SetRotate(DirectX::SimpleMath::Vector3 rotate)
{
	m_rotate = rotate;
}



//**********************************************************************
//!	@brief		���s�ړ���ݒ肷��
//!
//!	@param[in]	���s�ړ�
//!
//!	@return		�Ȃ�
//**********************************************************************
void Obj3d::SetTrans(DirectX::SimpleMath::Vector3 trans)
{
	m_trans = trans;
}



//**********************************************************************
//!	@brief		�X�P�[�����擾����
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�X�P�[��
//**********************************************************************
float Obj3d::GetScale()
{
	return m_scale;
}



//**********************************************************************
//!	@brief		��]�p���擾����
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		��]�p
//**********************************************************************
Vector3 Obj3d::GetRotate()
{
	return m_rotate;
}



//**********************************************************************
//!	@brief		�X�P�[�����擾����
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		�X�P�[��
//**********************************************************************
Vector3 Obj3d::GetTrans()
{
	return m_trans;
}



//**********************************************************************
//!	@brief		���[���h�s����擾����
//!
//!	@param[in]	�Ȃ�
//!
//!	@return		���[���h�s��
//**********************************************************************
Matrix Obj3d::GetWorld()
{
	return m_world;
}
