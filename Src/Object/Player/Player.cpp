
#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/ResourceManager.h"

#include "../../Utility/AsoUtility.h"

#include "../Shot/ShotPlayer.h"

#include "Controller.h"
#include "Player.h"

Player::Player()
{
	// ��ԊǗ�
	stateChanges_.emplace(STATE::NONE, std::bind(&Player::ChangeStateNone, this));
	stateChanges_.emplace(STATE::PLAY, std::bind(&Player::ChangeStatePlay, this));
	stateChanges_.emplace(STATE::DEAD, std::bind(&Player::ChangeStateDead, this));
	stateChanges_.emplace(STATE::END, std::bind(&Player::ChangeStateEnd, this));
	stateChanges_.emplace(STATE::VICTORY, std::bind(&Player::ChangeStateVictory, this));



}
Player::~Player()
{
}

void Player::Init(VECTOR startPos, int playerNo)
{

	controller_ = std::make_shared<Controller>();
	controller_->Init(playerNo);

	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_SHIP));

	float scale = 10.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = startPos;
	//transform_.pos = { 10.0f, 20.0f, 30.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

	//�ϐ��F�ړ��֌W
	movedPos_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;

	//�ϐ��F��]�֌W
	direction_= AsoUtility::VECTOR_ZERO;
	direction_.y = ROT_POW;

	//�ϐ��F�U���֌W
	//�e���ˌ�̍d�����ԃZ�b�g
	deleyShot_ = TIME_DELAY_SHOT;


	// �������
	ChangeState(STATE::PLAY);


}

void Player::Update()
{
	controller_->Update();

	// �X�V�X�e�b�v
	stateUpdate_();

	transform_.Update();

	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		shots_[i]->Update();
	}
}

void Player::Draw()
{

	// ���f���̕`��
	MV1DrawModel(transform_.modelId);

	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		shots_[i]->Draw();
	}
}


#pragma region �ϐ�state_�ɂ��֐�stateUpdate_�̕ύX

void Player::ChangeState(STATE state)
{

	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[state_]();

}
void Player::ChangeStateNone()
{
	stateUpdate_ = std::bind(&Player::UpdateNone, this);
}
void Player::ChangeStatePlay()
{
	stateUpdate_ = std::bind(&Player::UpdatePlay, this);
}
void Player::ChangeStateDead()
{
	stateUpdate_ = std::bind(&Player::UpdateDead, this);
}
void Player::ChangeStateEnd()
{
	stateUpdate_ = std::bind(&Player::UpdateEnd, this);
}
void Player::ChangeStateVictory()
{
	stateUpdate_ = std::bind(&Player::UpdateVictory, this);
}

#pragma endregion


// �X�V�X�e�b�v
#pragma region state_���Ƃɂ��X�V����

void Player::UpdateNone()
{

}
void Player::UpdatePlay()
{


	ProcessTurn();
	ProcessMove();

	ProcessShot();

	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);

	//�ړ���̈ʒu�ɖ�肪����m�F
	// �Փ�(�J�v�Z��)
	//CollisionCapsule();
	// �Փ�(�d��)
	//CollisionGravity();
	
	// �ړ���̈ʒu�ɖ�肪�Ȃ���Έړ�
	transform_.pos = movedPos_;

}
void Player::UpdateDead()
{

}
void Player::UpdateEnd()
{

}
void Player::UpdateVictory()
{

}

#pragma endregion


void Player::ProcessMove(void)
{
	Move();
}
void Player::Move(void)
{
	auto& input = InputManager::GetInstance();

	movePow_ = AsoUtility::VECTOR_ZERO;
	
	// �O�������擾(��)
	VECTOR forward = AsoUtility::VECTOR_ZERO;

	

	if (controller_->GetisControl(Controller::MODE::FORWARD)) { forward = transform_.GetForward(); }
	if (controller_->GetisControl(Controller::MODE::BACK)) { forward = transform_.GetBack(); }

	/*for (auto& key : controlKey_[DIR::UP])
	{
		if (input.IsNew(key)) {forward = transform_.GetForward(); }
	}
	for (auto& key : controlKey_[DIR::DOWN])
	{
		if (input.IsNew(key)) { forward = transform_.GetBack(); }
	}*/

	// �ړ�
	movePow_ =
		VScale(forward, SPEED_MOVE);

}

void Player::ProcessTurn(void)
{
	Turn(AsoUtility::VECTOR_ONE);
}
void Player::Turn(VECTOR axis)
{

	auto& input = InputManager::GetInstance();

	bool isMove = (controller_->GetisControl(Controller::MODE::FORWARD) 
		|| controller_->GetisControl(Controller::MODE::BACK));
	
	if (controller_->GetisControl(Controller::MODE::LEFT)) { direction_.y = -ROT_POW; }
	if (controller_->GetisControl(Controller::MODE::RIGHT)) { direction_.y = ROT_POW; }

	VECTOR addAxis = direction_;

	if (controller_->GetisControl(Controller::MODE::BACK))
	{
		addAxis.y *= -1;
	}


	//����]
	//addAxis.y = -1.0f;
	//�E��]
	//addAxis.y = 1.0f;

	if (!AsoUtility::EqualsVZero(addAxis)&& isMove)
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

void Player::ProcessShot(void)
{
	auto& ins = InputManager::GetInstance();


	if ((controller_->GetisControl(Controller::MODE::ATTACK)
		&& deleyShot_ <= 0.0f))
	{

		// �e�𐶐�(�����͉��Ő��ʕ���)
		//shot->CreateShot(barrelPos_, { 0.0f, 0.0f, 1.0f });
		// �e���w��ʒu����A�w������ɔ��˂�����
		CreateShot();


		// �e���ˌ�̍d�����ԃZ�b�g
		deleyShot_ = TIME_DELAY_SHOT;
	}


	// �e���ˌ�̍d�����Ԃ����炵�Ă���
	if (deleyShot_ > 0.0f)
	{
		deleyShot_ -= SceneManager::GetInstance().GetDeltaTime();
	}
}
void Player::CreateShot(void)
{
	// �e�̐����t���O
	bool isCreate = false;

	for (auto v : shots_)
	{
		if (v->GetState() == ShotPlayer::STATE::END)
		{
			// �ȑO�ɐ��������C���X�^���X���g����
			v->Create(transform_.pos, transform_.GetForward());
			isCreate = true;
			break;
		}
	}
	if (!isCreate)
	{
		// ���@�̑O������
		auto dir = transform_.GetForward();
		// �V�����C���X�^���X�𐶐�
		ShotPlayer* newShot = new ShotPlayer();
		newShot->Create(transform_.pos, transform_.GetForward());

		// �e�̊Ǘ��z��ɒǉ�
		shots_.push_back(newShot);
	}
}
