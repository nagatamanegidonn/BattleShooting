
#include "../../Application.h"

#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"

#include "../../Utility/AsoUtility.h"

#include "../Shot/ShotPlayer.h"

#include "Controller.h"
#include "Player.h"


Player::Player(Camera& camera) :camera_(camera)
{
	// ��ԊǗ�
	stateChanges_.emplace(STATE::NONE, std::bind(&Player::ChangeStateNone, this));
	stateChanges_.emplace(STATE::PLAY, std::bind(&Player::ChangeStatePlay, this));
	stateChanges_.emplace(STATE::DEAD, std::bind(&Player::ChangeStateDead, this));
	stateChanges_.emplace(STATE::END, std::bind(&Player::ChangeStateEnd, this));
	stateChanges_.emplace(STATE::VICTORY, std::bind(&Player::ChangeStateVictory, this));

	//�ϐ��F�ړ��֌W
	movedPos_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;

	//�ϐ��F��]�֌W
	direction_ = AsoUtility::VECTOR_ZERO;
	direction_.y = ROT_POW;

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
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();


	// ���f������̊�{���
	transDir_.SetModel(MV1LoadModel("Data/Model/Dir/Dir.mv1"));
	if (playerNo == 0)
	{
		MV1SetMaterialDifColor(transDir_.modelId, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		MV1SetMaterialEmiColor(transDir_.modelId, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		MV1SetMaterialDifColor(transDir_.modelId, 0, GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
		MV1SetMaterialEmiColor(transDir_.modelId, 0, GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
	}
	transDir_.scl = { 0.1f, 0.1f, 0.1f };
	transDir_.pos = startPos;

	transDir_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	transDir_.quaRotLocal = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	transDir_.Update();

	//�ϐ��F�ړ��֌W
	movedPos_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;

	//�ϐ��F��]�֌W
	direction_= AsoUtility::VECTOR_ZERO;
	direction_.y = ROT_POW;

	//�X�e�[�^�X
	damageTime_ = 0.0f;
	invincibleTime_ = 0.0f;

	rideAttrckPos_ = transform_.pos;
	rideDamagePos_ = transform_.pos;

	//�X�e�[�^�X�ϐ�
	ridesMaxHp_ = ridesHp_ = 10;

	//���̍쐬
	MakeSquereVertex();

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
	transDir_.Update();

	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		shots_[i]->Update();
	}

	rideDamagePos_ = VAdd(transform_.pos, VScale(transform_.quaRot.GetBack(), 20));
	rideAttrckPos_ = VAdd(transform_.pos, VScale(transform_.quaRot.GetForward(), 20));
}

void Player::Draw()
{

	// ���f���̕`��
	// ����͈͓��F�f�B�t���[�Y�J���[��ԐF�ɂ���

	MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
	if (invincibleTime_ > 0.0f)
	{
		if (static_cast<int>(invincibleTime_) % 2 == 0)
		{
			MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}
	MV1DrawModel(transform_.modelId);

	DrawSphere3D(rideAttrckPos_, 10, 10, 0xff0000, 0xffff00, false);
	DrawSphere3D(rideDamagePos_, 20, 10, 0xff0000, 0x00ffff, false);


#pragma region ���̕`��i���j

	VECTOR addAxis = VScale(direction_, 10);


	if (controller_->GetisControl(Controller::MODE::BACK))
	{
		//addAxis.y *= -1;
	}

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
	auto trnQut = transform_.quaRot;
	trnQut = trnQut.Mult(rotPow);

	transDir_.pos = VAdd(transform_.pos, VScale(trnQut.GetForward(), 40));
	transDir_.quaRot = trnQut;
	transDir_.Update();
	// �R�c���f���̕`��
	MV1DrawModel(transDir_.modelId);


	auto c = MV1GetMaterialDifColor(transDir_.modelId, 0);

	// ���b�V���̕`��
	/*VECTOR zero = VAdd(transform_.pos, VScale(trnQut.GetLeft(), 40));
	VECTOR one = VAdd(transform_.pos, VScale(trnQut.GetForward(), 80));
	VECTOR two = VAdd(transform_.pos, VScale(trnQut.GetForward(), 20));
	VECTOR three = VAdd(transform_.pos, VScale(trnQut.GetRight(), 40));*/

	/*DrawSphere3D(zero, 10, 10, 0xff0000, 0xff0000, false);
	DrawSphere3D(one, 10, 10, 0xff0000, 0xff0000, false);
	DrawSphere3D(two, 10, 10, 0xff0000, 0xff0000, false);
	DrawSphere3D(three, 10, 10, 0xff0000, 0xff0000, false);*/

	//DrawTriangle3D(zero, one, two, 0x0000ff, true);
	//DrawTriangle3D( two,one, three, 0x0000ff, true);

#pragma endregion


	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		shots_[i]->Draw();
	}
}

const void Player::RideDamage(int damage)
{
	if (invincibleTime_ > 0.0f)
	{
		return;
	}
	//�̗͂����炷
	ridesHp_ -= damage;
	//���G���Ԃ�ݒ�
	invincibleTime_ = 3.0f;
}

VECTOR& Player::GetPos(int id)
{
	switch (id)
	{
	case 0://��蕨��damage�ʒu
		return rideDamagePos_;
		break;
	case 1://��蕨�̍U���n�_
		return rideAttrckPos_;
		break;
	case 2:
		return transform_.pos;
		break;
	}

	return transform_.pos;
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
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
	if (invincibleTime_ > 0.0f)
	{
		invincibleTime_ -= SceneManager::GetInstance().GetDeltaTime();
	}


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

	InputManager& ins = InputManager::GetInstance();
	InputManager::JOYPAD_NO jno = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD1);

	// ���X�e�B�b�N�̉���
	auto leftStickX = ins.GetJPadInputState(jno).AKeyLX;
	// ���X�e�B�b�N�̏c��
	auto leftStickY = ins.GetJPadInputState(jno).AKeyLY;



	VECTOR dir = AsoUtility::VECTOR_ZERO;
	float rotRad = 0.0f;

	if ((leftStickY < 0.0f) || (ins.IsNew(KEY_INPUT_W))) { dir = VAdd(dir, AsoUtility::DIR_F); }
	if ((leftStickX < 0.0f) || (ins.IsNew(KEY_INPUT_A))) { dir = VAdd(dir, AsoUtility::DIR_L); }
	if ((leftStickY > 0.0f) || (ins.IsNew(KEY_INPUT_S))) { dir = VAdd(dir, AsoUtility::DIR_B); }
	if ((leftStickX > 0.0f) || (ins.IsNew(KEY_INPUT_D))) { dir = VAdd(dir, AsoUtility::DIR_R); }


	//movePow_ = AsoUtility::VECTOR_ZERO;


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
	

	if (controller_->GetisControl(Controller::MODE::LEFT)) { direction_.y = ROT_POW; }
	if (controller_->GetisControl(Controller::MODE::RIGHT)) { direction_.y = -ROT_POW; }

	VECTOR addAxis = direction_;

	if (controller_->GetisControl(Controller::MODE::BACK))
	{
		addAxis.y *= -1;
	}


	//����]
	//addAxis.y = -1.0f;
	//�E��]
	//addAxis.y = 1.0f;

	if (!AsoUtility::EqualsVZero(addAxis) && isMove)
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

void Player::MakeSquereVertex(void)
{
	
}
