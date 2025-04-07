#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"

#include "../../Application.h"

#include "../../Utility/AsoUtility.h"

#include "../Common/AnimationController.h"

#include "../Shot/ShotPlayer.h"

#include "Controller.h"
#include "DirModel.h"
#include "ViewPlayer.h"
#include "../../Manager/SoundManager.h"


//Player::Player(Camera& camera) :camera_(camera)
ViewPlayer::ViewPlayer()
{
	// ��ԊǗ�
	stateChanges_.emplace(STATE::NONE, std::bind(&ViewPlayer::ChangeStateNone, this));
	stateChanges_.emplace(STATE::PLAY, std::bind(&ViewPlayer::ChangeStatePlay, this));
	stateChanges_.emplace(STATE::END, std::bind(&ViewPlayer::ChangeStateEnd, this));


	
}
ViewPlayer::~ViewPlayer()
{
}

void ViewPlayer::Init(VECTOR startPos, int playerNo, int pryId)
{

	// ���f������̊�{���
	/*transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_SHIP));*/

	//�v���C�L�������Ƃɑ���
	if (pryId == 0)
	{
		transform_.SetModel(MV1LoadModel("Data/Model/P1/P1.mv1"));

		MV1SetMaterialDifColor(transform_.modelId, 3, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		MV1SetMaterialEmiColor(transform_.modelId, 3, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		//�A�j���[�V�����̐ݒ�
		InitAnimation(Application::PATH_MODEL + "P1/P1.mv1");

	}
	else if (pryId == 1)
	{
		transform_.SetModel(MV1LoadModel("Data/Model/P2/P2.mv1"));

		MV1SetMaterialDifColor(transform_.modelId, 3, GetColorF(0.5f, 0.5f, 1.0f, 1.0f));
		MV1SetMaterialEmiColor(transform_.modelId, 3, GetColorF(0.5f, 0.5f, 1.0f, 1.0f));
		//�A�j���[�V�����̐ݒ�
		InitAnimation(Application::PATH_MODEL + "P2/P2.mv1");

		// �}�e���A���̎��Ȕ����F��ݒ�
		MV1SetMaterialEmiColor(transform_.modelId, 4, GetColorF(0.2f, 0.2f, 0.2f, 1.0f));

	}
	else if (pryId == 2)
	{
		transform_.SetModel(MV1LoadModel("Data/Model/P3/P3.mv1"));

		//�A�j���[�V�����̐ݒ�
		InitAnimation(Application::PATH_MODEL + "P3/P3.mv1");


		int num = MV1GetMaterialNum(transform_.modelId);

		std::string a = "";
		std::string tex = "";
		for (int i = 0; i < num; i++)
		{
			a += MV1GetMaterialName(transform_.modelId, i);
			a += ":";
			tex += MV1GetMaterialDifMapTexture(transform_.modelId, i);
			tex += ":";
		}

		std::string s1 = a;
		std::string s2 = tex;

	}
	else if (pryId == 3)
	{
		transform_.SetModel(MV1LoadModel("Data/Model/P4/P4.mv1"));

		//�A�j���[�V�����̐ݒ�
		InitAnimation(Application::PATH_MODEL + "P4/P4.mv1");
	}

	//�g�����X�t�H�[���̐ݒ�
	float scale = 1.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = startPos;
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(90.0f),
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(-10.0f)
	);
	transform_.Update();

	
	// �������
	ChangeState(STATE::PLAY);

}

void ViewPlayer::Update()
{
	//controller_->Update();


	// �X�V�X�e�b�v
	stateUpdate_();

	transform_.Update();

	// �A�j���[�V�����Đ�
	animationController_->Update();
}

void ViewPlayer::Draw()
{
	//�Q�[�����łȂ���΃��f���݂̂�`��
	if (SceneManager::GetInstance().GetSceneID() != SceneManager::SCENE_ID::GAME)
	{
		MV1DrawModel(transform_.modelId);
		return;
	}

	// ���f���̕`��
	// ����͈͓��F�f�B�t���[�Y�J���[��ԐF�ɂ���

	
	MV1DrawModel(transform_.modelId);
	

}

//�A�j���[�V�����̐ݒ�
void ViewPlayer::InitAnimation(std::string path)
{

	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path, 20.0f, 0);
	animationController_->Add((int)ANIM_TYPE::RUN, path, 20.0f, 0);

	animationController_->Play((int)ANIM_TYPE::IDLE);

}


#pragma region �ϐ�state_�ɂ��֐�stateUpdate_�̕ύX

void ViewPlayer::ChangeState(STATE state)
{

	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[state_]();

}
void ViewPlayer::ChangeStateNone()
{
	stateUpdate_ = std::bind(&ViewPlayer::UpdateNone, this);

	float scale = 1.0f;
	transform_.scl = { scale, scale, scale };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(90.0f),
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(-10.0f)
	);
}
void ViewPlayer::ChangeStatePlay()
{
	stateUpdate_ = std::bind(&ViewPlayer::UpdatePlay, this);
}
void ViewPlayer::ChangeStateEnd()
{
	stateUpdate_ = std::bind(&ViewPlayer::UpdateEnd, this);
}

#pragma endregion

// �X�V�X�e�b�v
#pragma region state_���Ƃɂ��X�V����

void ViewPlayer::UpdateNone()
{
	//�����]����������]�ʂ��N�H�[�^�j�I���ō��
	Quaternion rotPow = Quaternion();

	rotPow = rotPow.Mult(
		Quaternion::AngleAxis(
			AsoUtility::Deg2RadF(1.0f), AsoUtility::AXIS_Z
		));

	// ��]�Ȃ�������(����)
	transform_.quaRot = transform_.quaRot.Mult(rotPow);
	transform_.Update();

}
void ViewPlayer::UpdatePlay()
{

	Turn({ 0.0f,0.0f,1.0f });
	
}
void ViewPlayer::UpdateEnd()
{

}

#pragma endregion


void ViewPlayer::Turn(VECTOR axis)
{

	VECTOR addAxis = axis;

	
	//����]
	//addAxis.y = -1.0f;
	//�E��]
	//addAxis.y = 1.0f;

	if (!AsoUtility::EqualsVZero(addAxis))
	{
		//�����]����������]�ʂ��N�H�[�^�j�I���ō��
		Quaternion rotPow = Quaternion();

		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(addAxis.z), AsoUtility::AXIS_Z
			));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(addAxis.x), AsoUtility::AXIS_X
			));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(addAxis.y), AsoUtility::AXIS_Y
			));

		// ��]�Ȃ�������(����)
		transform_.quaRot = transform_.quaRot.Mult(rotPow);

	}

}