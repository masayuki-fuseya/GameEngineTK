//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�
//!	@file	Obj3d.h
//!
//!	@brief	3D�I�u�W�F�N�g�N���X�̃w�b�_�t�@�C��
//!
//!	@date	2017/05/22
//!
//!	@author	Masayuki Fuseya
//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�

// ���d�C���N���[�h�̖h�~ ==================================================
#pragma once

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "Camera.h"

#include <CommonStates.h>
#include <Effects.h>
#include <memory>
#include <Model.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include <wchar.h>
#include <Windows.h>
#include <wrl.h>


class Obj3d
{
public:
	static void InitializeStatic(Camera* pCamera, Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext);
private:
	static Camera* m_pCamera;
	static Microsoft::WRL::ComPtr<ID3D11Device>				m_d3dDevice;
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_d3dContext;
	static std::unique_ptr<DirectX::CommonStates>			m_states;
	static std::unique_ptr<DirectX::EffectFactory>			m_factory;
private:
	// ���f��
	std::unique_ptr<DirectX::Model> m_model;
	// �X�P�[��
	DirectX::SimpleMath::Vector3 m_scale;
	// ��](�I�C���[�p)
	DirectX::SimpleMath::Vector3 m_rotation;
	// ��](�N�H�[�^�j�I��)
	DirectX::SimpleMath::Quaternion m_quaternion;
	// ���s�ړ�
	DirectX::SimpleMath::Vector3 m_translation;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	// �e�̃|�C���^
	Obj3d* m_pObjParent;
	// ��]���N�H�[�^�j�I���Ŏ���
	bool m_useQuaternion;
public:
	Obj3d();

	void LoadModel(const wchar_t* fileName);
	virtual void Update();
	virtual void Render();

	void SetScale(const DirectX::SimpleMath::Vector3& scale)
	{
		m_scale = scale;
	}
	void SetQuaternion(const DirectX::SimpleMath::Quaternion& quaternion)
	{
		m_quaternion = quaternion;
		m_useQuaternion = true;
	}
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation)
	{
		m_rotation = rotation;
		m_useQuaternion = false;
	}
	void SetTranslation(const DirectX::SimpleMath::Vector3& translation)
	{
		m_translation = translation;
	}
	void SetObjParent(Obj3d* pObjParent)
	{
		m_pObjParent = pObjParent;
	}

	const DirectX::SimpleMath::Vector3& GetScale()
	{
		return m_scale;
	}
	const DirectX::SimpleMath::Vector3& GetRotation()
	{
		return m_rotation;
	}
	const DirectX::SimpleMath::Vector3& GetTranslation()
	{
		return m_translation;
	}
	const DirectX::SimpleMath::Matrix& GetWorld()
	{
		return m_world;
	}
	Obj3d* GetObjParent()
	{
		return m_pObjParent;
	}
};
