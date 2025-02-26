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

		//�J�����̏�����X�V
		cameraUp_ = rot_.GetForward();
		pos_ = { 0.0f, 1000.0f, 0.0f };
		targetPos_ = { 0.0f, -100.0f, 0.0f };

		/*rot_ = Quaternion::Identity();

		Quaternion rotPow = Quaternion();

		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(90.0f), AsoUtility::AXIS_X
			));
		
		 ��]�Ȃ�������(����)
		rot_ = rot_.Mult(rotPow);
		SetBeforeDrawFixedPoint();
		pos_ = { 0.0f, 1000.0f , 0.0f};
		targetPos_ = { 0.0f, 0.0f, 0.0f };*/
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
	auto& ins = InputManager::GetInstance();
	// WASD�ŃJ�����̈ʒu��ς���
	float movePow = 3.0f;

	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }

	//��]���Ɨʌ���
	const float ROT_POW = 1.0f;
	VECTOR axisDeg = AsoUtility::VECTOR_ZERO;

	if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x = ROT_POW; }
	if (ins.IsNew(KEY_INPUT_UP)) { axisDeg.x = -ROT_POW; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y = ROT_POW; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { axisDeg.y = -ROT_POW; }

	if (!AsoUtility::EqualsVZero(axisDeg))
	{
		//�����]����������]�ʂ��N�H�[�^�j�I���ō��
		Quaternion rotPow = Quaternion();

		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.z), AsoUtility::AXIS_Z
			));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.x), AsoUtility::AXIS_X
			));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.y), AsoUtility::AXIS_Y
			));

		// ��]�Ȃ�������(����)
		rot_ = rot_.Mult(rotPow);

		// �����_�̑��΍��W����]������
		VECTOR rotLocalPos = rot_.PosAxis(RELATIVE_C2T_POS);
		//���΍��W�����[���h���W�ɂ���i�����_�̍X�V�j
		targetPos_ = VAdd(pos_, rotLocalPos);

		//�J�����̏�����X�V
		cameraUp_ = rot_.GetUp();
	}

	if (!AsoUtility::EqualsVZero(moveDir))
	{
		//�@�ړ������W�{�ړ���
		//�ړ��ʁ������~�X�s�[�h

		//���͂��ꂽ���������߂�̉�]�����g����
		//�J�����̐i�s�����ɕϊ�����
		VECTOR direction = rot_.PosAxis(moveDir);
		//�ړ���
		const float speed = SPEED / 2;
		VECTOR movePow = VScale(direction, speed);
		//
		pos_ = VAdd(pos_, movePow);
		targetPos_ = VAdd(targetPos_, movePow);

	}
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

	//�J�����̏�����X�V
	cameraUp_ = rot_.GetForward();

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

//�J�����̃Y�[��
void Camera::FadeIn(void)
{	
	if (pos_.y < 200.0f)
	{
		return;
	}
	pos_.y--;
}
void Camera::FadeOut(void)
{
	pos_.y++;
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
	case Camera::MODE::FOLLOW_POINT:
		pos_ = { 0.0f, 500.0f, 0.0f };
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
