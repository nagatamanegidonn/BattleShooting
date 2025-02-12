
#include "../../Manager/InputManager.h"
#include "../../Manager/ResourceManager.h"

#include "../../Utility/AsoUtility.h"

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

void Player::Init()
{
	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_SHIP));

	float scale = 10.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	//transform_.pos = { 10.0f, 20.0f, 30.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();


	movedPos_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;


	// �������
	ChangeState(STATE::PLAY);


}

void Player::Update()
{
	// �X�V�X�e�b�v
	stateUpdate_();

	transform_.Update();
}

void Player::Draw()
{

	// ���f���̕`��
	MV1DrawModel(transform_.modelId);

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


	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);

	// �Փ�(�J�v�Z��)
	//CollisionCapsule();
	// �Փ�(�d��)
	//CollisionGravity();
	
	// �ړ�
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
	movePow_ = AsoUtility::VECTOR_ZERO;


	// �O�������擾
	VECTOR forward = transform_.GetForward();
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
	VECTOR addAxis = AsoUtility::VECTOR_ZERO;

	//����]
	//addAxis.y = -1.0f;
	//�E��]
	addAxis.y = 1.0f;

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
