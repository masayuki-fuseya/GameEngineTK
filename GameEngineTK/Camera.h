//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�
//!	@file	Camera.h
//!
//!	@brief	�J�����N���X�̃w�b�_�t�@�C��
//!
//!	@date	2017/05/11
//!
//!	@author	Masayuki Fuseya
//�߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁߁�

#pragma once

#include <d3d11.h>
#include <SimpleMath.h>

class Camera
{
protected:
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;
	// �J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_eyepos;
	// �ǂ����݂邩
	DirectX::SimpleMath::Vector3 m_refpos;
	// ������x�N�g��
	DirectX::SimpleMath::Vector3 m_upvec;
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

	void Update();
	// �r���[���W���擾
	DirectX::SimpleMath::Matrix GetViewMatrix();
	// �ˉe���W���擾
	DirectX::SimpleMath::Matrix GetProjectionMatrix();
	void SetEyePos(DirectX::SimpleMath::Vector3 eyePos);
	void SetRefPos(DirectX::SimpleMath::Vector3 refPos);
	void SetUpVec(DirectX::SimpleMath::Vector3 upVec);
	void SetFovY(float fovY);
	void SetAspect(float aspect);
	void SetNearClip(float nearClip);
	void SetFarClip(float farClip);
};
