//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�
//!	@file	Camera.h
//!
//!	@brief	�J�����N���X�̃w�b�_�t�@�C��
//!
//!	@date	2017/05/11
//!
//!	@author	Masayuki Fuseya
//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�

// ���d�C���N���[�h�̖h�~ ==================================================
#pragma once

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include <d3d11.h>
#include <SimpleMath.h>

class Camera
{
protected:
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	// �J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_eyePos;
	// �ǂ����݂邩
	DirectX::SimpleMath::Vector3 m_refPos;
	// ������x�N�g��
	DirectX::SimpleMath::Vector3 m_upVec;
	// ������������p(�㉺���ꂼ�ꉽ�x�܂Ŏʂ���)
	float m_fovY;
	// ��ʃT�C�Y�̔䗦
	float m_aspect;
	// ��O�̕\�����E
	float m_nearClip;
	// ���̕\�����E
	float m_farClip;
public:
	Camera(int width, int height);
	virtual ~Camera();

	virtual void Update();
	// �r���[���W���擾
	// �����������Ȃ��Q�Ƃ�Ԃ�
	const DirectX::SimpleMath::Matrix& GetView();
	// �ˉe���W���擾
	const DirectX::SimpleMath::Matrix& GetProjection();
	void SetEyePos(const DirectX::SimpleMath::Vector3& eyePos);
	void SetRefPos(const DirectX::SimpleMath::Vector3& refPos);
	void SetUpVec(const DirectX::SimpleMath::Vector3& upVec);
	void SetFovY(const float fovY);
	void SetAspect(const float aspect);
	void SetNearClip(const float nearClip);
	void SetFarClip(const float farClip);
private:
	void CalcMatrix();
};
