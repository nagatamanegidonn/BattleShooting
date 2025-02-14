#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"

#include "../Object/Common/Transform.h"

#include "Camera.h"

Camera::Camera(void)
{
	mode_ = MODE::NONE;
	pos_ = { 0.0f, 0.0f, 0.0f };
	targetPos_ = { 0.0f, 0.0f, 0.0f };
	rot_ = Quaternion::Identity();
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	// �J�����̏����ݒ�
	SetDefault();
}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	}

	// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

}

#pragma region mode_�ɂ��`����@

void Camera::SetBeforeDrawFixedPoint(void)
{
	// �������Ȃ�
}
void Camera::SetBeforeDrawFree(void)
{
}
void Camera::SetBeforeDrawFollow(void)
{
	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = followTransform_->pos;
	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = followTransform_->quaRot;
	// �Ǐ]�Ώۂ���J�����܂ł̑��΍��W
	VECTOR relativeCPos = followRot.PosAxis(RELATIVE_F2C_POS_FOLLOW);
	// �J�����ʒu�̍X�V
	pos_ = VAdd(followPos, relativeCPos);
	// �J�����ʒu���璍���_�܂ł̑��΍��W
	VECTOR relativeTPos = followRot.PosAxis(RELATIVE_C2T_POS);
	// �����_�̍X�V
	targetPos_ = VAdd(pos_, relativeTPos);
	// �J�����̏����
	cameraUp_ = followRot.PosAxis(rot_.GetUp());
}


#pragma endregion



void Camera::Draw(void)
{
}
void Camera::Release(void)
{
}


void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

void Camera::ChangeMode(MODE mode)
{

	// �J�����̏����ݒ�
	SetDefault();

	// �J�������[�h�̕ύX
	mode_ = mode;

	// �ύX���̏���������
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FOLLOW:
		break;
	}

}

void Camera::SetDefault(void)
{

	// �J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;

	// �����_
	targetPos_ = VAdd(pos_, RELATIVE_C2T_POS);

	// �J�����̏����
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	// �J������X���ɌX���Ă��邪�A
	// ���̌X������Ԃ��p�x�[���A�X�������Ƃ���
	rot_ = Quaternion::Identity();

}
