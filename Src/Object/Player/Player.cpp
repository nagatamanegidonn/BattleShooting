
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
#include "Player.h"
#include "../../Manager/SoundManager.h"


//Player::Player(Camera& camera) :camera_(camera)
Player::Player()
{
	// ��ԊǗ�
	stateChanges_.emplace(STATE::NONE, std::bind(&Player::ChangeStateNone, this));
	stateChanges_.emplace(STATE::PLAY, std::bind(&Player::ChangeStatePlay, this));
	stateChanges_.emplace(STATE::JUMP, std::bind(&Player::ChangeStateJump, this));
	stateChanges_.emplace(STATE::FALL_DEAD, std::bind(&Player::ChangeStateFallDead, this));
	stateChanges_.emplace(STATE::DEAD, std::bind(&Player::ChangeStateDead, this));
	stateChanges_.emplace(STATE::END, std::bind(&Player::ChangeStateEnd, this));
	stateChanges_.emplace(STATE::VICTORY, std::bind(&Player::ChangeStateVictory, this));


	//�ϐ��F�ړ��֌W
	movedPos_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;

	//�ϐ��F��]�֌W
	turnTime_ = 2.0f;
	direction_ = AsoUtility::VECTOR_ZERO;
	direction_.y = ROT_POW;

}
Player::~Player()
{
}

void Player::Init(VECTOR startPos, int playerNo, int pryId)
{

	controller_ = std::make_shared<Controller>();
	controller_->Init(playerNo);

	dirModel_ = std::make_shared<DirModel>();
	dirModel_->Init(playerNo);

	// ���f������̊�{���
	/*transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_SHIP));*/

	//�v���C�L�������Ƃɑ���
	if (pryId == 0)
	{
		transform_.SetModel(MV1LoadModel("Data/Model/P1/P1.mv1"));
		shotModel_= ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::P1_SHOT_MODEL);

		MV1SetMaterialDifColor(transform_.modelId, 3, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		MV1SetMaterialEmiColor(transform_.modelId, 3, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		//�A�j���[�V�����̐ݒ�
		InitAnimation(Application::PATH_MODEL + "P1/P1.mv1");
		playerIconH_= LoadGraph("Data/Image/P1Image.png");

	}
	else
	{
		transform_.SetModel(MV1LoadModel("Data/Model/P2/P2.mv1"));
		shotModel_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::P2_SHOT_MODEL);

		MV1SetMaterialDifColor(transform_.modelId, 3, GetColorF(0.5f, 0.5f, 1.0f, 1.0f));
		MV1SetMaterialEmiColor(transform_.modelId, 3, GetColorF(0.5f, 0.5f, 1.0f, 1.0f));
		//�A�j���[�V�����̐ݒ�
		InitAnimation(Application::PATH_MODEL + "P2/P2.mv1");

		// �}�e���A���̎��Ȕ����F��ݒ�
		MV1SetMaterialEmiColor(transform_.modelId, 4, GetColorF(0.2f, 0.2f, 0.2f, 1.0f));
		playerIconH_ = LoadGraph("Data/Image/P2Image.png");

	}

	//float scale = 10.0f;
	float scale = 0.3f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = startPos;
	//transform_.pos = { 10.0f, 20.0f, 30.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(-90.0f),
		AsoUtility::Deg2RadF(0.0f)
	);
	transform_.Update();

	//�ϐ��F�ړ��֌W
	movedPos_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;

	//�ϐ��F��]�֌W
	turnTime_ = 2.0f;
	direction_= AsoUtility::VECTOR_ZERO;
	direction_.y = ROT_POW;

	//�ӂ���΂��֌W
	jumpTime_ = 0.0f;
	jumpDir_ = AsoUtility::VECTOR_ZERO;

	//�X�e�[�^�X
	damageTime_ = 0.0f;
	invincibleTime_ = 0.0f;

	rideAttrckPos_ = transform_.pos;
	rideDamagePos_ = transform_.pos;

	//�X�e�[�^�X�ϐ�
	//HP���Ƃ͂P�O
	playerMaxHp_ = playerHp_ = 5;
	hpGaugeH_ = LoadGraph("Data/Image/HPGauge.png");
	hpFreamH_ = LoadGraph("Data/Image/HPFream.png");
	shotGaugeH_ = LoadGraph("Data/Image/shotGauge.png");
	shotFreamH_ = LoadGraph("Data/Image/shotFream.png");

	//�ϐ��F�U���֌W
	//�e���ˌ�̍d�����ԃZ�b�g
	deleyShot_ = TIME_DELAY_SHOT;
	magazineMaxShot_ = magazineShot_ = 20;
	reloadTime_ = 0.0f;
	reloadSet_ = TIME_RELOAD;
	// �������
	ChangeState(STATE::PLAY);

	InitEffect();

}

void Player::Update()
{
	controller_->Update();

	
	//�����[�h�d��
	if (reloadTime_ > 0.0f)
	{
		reloadTime_ -= SceneManager::GetInstance().GetDeltaTime();
		for (int i = 0; i < static_cast<int>(magazineMaxShot_); i++)
		{
			if (reloadTime_ <= reloadSet_)
			{
				reloadSet_ -= 0.1f;
				magazineShot_ += 1;
				if (magazineShot_ % 2 == 0)
				{
					SoundManager::GetInstance().Play(SoundManager::SRC::RELOAD, Sound::TIMES::ONCE, true);
				}
			}
		}
	}
	else
	{
		// �X�V�X�e�b�v
		stateUpdate_();
	}

	transform_.Update();

	/*size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		shots_[i]->Update();
	}*/

	rideDamagePos_ = VAdd(transform_.pos, VScale(transform_.quaRot.GetBack(), 0));
	rideAttrckPos_ = VAdd(transform_.pos, VScale(transform_.quaRot.GetForward(), 40));

	// �A�j���[�V�����Đ�
	animationController_->Update();
}

void Player::Draw()
{
	//�Q�[�����łȂ���΃��f���݂̂�`��
	if (state_ == STATE::END)
	{
		return;
	}

	// ���f���̕`��
	MV1DrawModel(transform_.modelId);
	
	if (state_ == STATE::DEAD|| state_ == STATE::FALL_DEAD)
	{
		return;
	}
	if (state_ == STATE::VICTORY)
	{
		SetFontSize(28);//�����̃T�C�Y��ݒ�

		std::string msg = "Result WIN";

		SceneManager& Sns = SceneManager::GetInstance();

		switch (Sns.GetWinner())
		{
			//�v���C���[�P�̏���
		case SceneManager::WINNER::PLAYER_ONE:
			msg = "PLAYER1 WIN";
			break;
			//�v���C���[�Q�̏���
		case SceneManager::WINNER::PLAYER_TWO:
			msg = "PLAYER2 WIN";
			break;
		case SceneManager::WINNER::DRAW:
			msg = "DRAW";
			break;
		}

		int cx = Application::SCREEN_SIZE_X / 2;
		int cy = Application::SCREEN_SIZE_Y / 2;

		int len = (int)strlen(msg.c_str());
		int width = GetDrawStringWidth(msg.c_str(), len);
		DrawFormatString(cx - (width / 2), cy, 0xffffff, msg.c_str());

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetFontSize(16);
		return;
	}

#pragma region ���̕`��i���j

	VECTOR addAxis = VScale(direction_, 10);


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


	// ���b�V���̕`��
	VECTOR zero = VAdd(transform_.pos, VScale(trnQut.GetLeft(), 40));
	VECTOR one = VAdd(transform_.pos, VScale(trnQut.GetForward(), 80));
	VECTOR two = VAdd(transform_.pos, VScale(trnQut.GetForward(), 20));
	VECTOR three = VAdd(transform_.pos, VScale(trnQut.GetRight(), 40));

	dirModel_->SyncModel(VAdd(transform_.pos, VScale(trnQut.GetForward(), 40)), trnQut);
	dirModel_->Draw();


	//DrawTriangle3D(zero, one, two, 0x0000ff, true);
	//DrawTriangle3D( two,one, three, 0x0000ff, true);

#pragma endregion

#ifdef _DEBUG
	DrawSphere3D(rideAttrckPos_, ATTRCK_RADIUS, 10, 0xff0000, 0xff0000, false);
	DrawSphere3D(rideDamagePos_, DAMAGE_RADIUS, 10, 0x0000ff, 0x0000ff, false);
#endif

	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		shots_[i]->Draw();
	}
}
void Player::DrawPram(int plyarNo)
{
	int plyNum = -1;
	bool plyFlag = true;

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	int imageSize = 68;
	int cx2 = (400 * (plyNum * -1)) + cx;

	if (plyarNo == 0)
	{
		plyNum = 1;
		plyFlag = true;
		cx2 = (400 * (plyNum * -1)) + cx;
		//�v���[���[�A�C�R���̕`��
		DrawRotaGraph(cx2 + ((plyNum * -1) * 70), 50, 0.1f, 0.0f, playerIconH_, true, false);

	}
	else
	{
		plyNum = -1;
		plyFlag = false;
		cx2 = (400 * (plyNum * -1)) + cx;
		//�v���[���[�A�C�R���̕`��
		DrawRotaGraph(cx2 + ((plyNum * -1) * 70), 50, 0.1f, 0.0f, playerIconH_, true, true);
	}

	//HP�̕`��
	for (int i = 0; i < playerMaxHp_; i++)
	{
		DrawRotaGraph(cx2 + (plyNum * imageSize * i), 30, 1.0f, 0.0f, hpFreamH_, true, plyFlag);
	}
	for (int i = 0; i < playerHp_; i++)
	{
		DrawRotaGraph(cx2 + (plyNum * imageSize * i), 30, 1.0f, 0.0f, hpGaugeH_, true, plyFlag);
	}

	//�e�̕`��
	for (int i = 0; i < magazineMaxShot_; i++)
	{
		DrawRotaGraph(cx2 + (plyNum * 15 * i), 50, 0.75f, 0.0f, shotFreamH_, true, plyFlag);
	}
	for (int i = 0; i < magazineShot_; i++)
	{
		DrawRotaGraph(cx2 + (plyNum * 15 * i), 50, 0.65f, 0.0f, shotGaugeH_, true, plyFlag);
	}

	if (magazineShot_ <= 0)
	{
		DrawFormatString(cx - 120, 80, 0xffffff, "Y�{�^������Q�A�_�L�[�Ń����[�h");
	}

#ifdef _DEBUG
	const VECTOR pos = transform_.pos;
	if (plyNum == 1)
	{
		DrawFormatString(0, 16, 0xff0000, "%2.f,%2.f,%2.f", pos.x, pos.y, pos.z);
	}
	else
	{
		DrawFormatString(0, 32, 0xff0000, "%2.f,%2.f,%2.f", pos.x, pos.y, pos.z);
	}
	plyNum *= -1;
#endif

}

//�_���[�W
const void Player::Damage(int damage)
{
	reloadTime_ = 0.0f;
	if (invincibleTime_ > 0.0f)
	{
		return;
	}

	//�̗͂����炷
	playerHp_ -= damage;
	SoundManager::GetInstance().Play(SoundManager::SRC::DAMAGE, Sound::TIMES::ONCE, true);

	if (playerHp_ <= 0)
	{
		playerHp_ = 0;
	}
	//���G���Ԃ�ݒ�
	invincibleTime_ = 1.0f;
}
const void Player::SetJump(VECTOR vec)
{
	//����\��ԂȂ琁����΂�

	ChangeState(STATE::JUMP);
	jumpDir_ = vec;
	jumpTime_ = 1.0f;
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

//�A�j���[�V�����̐ݒ�
void Player::InitAnimation(std::string path)
{

	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path, 20.0f, 0);
	animationController_->Add((int)ANIM_TYPE::RUN, path, 20.0f, 1);

	animationController_->Play((int)ANIM_TYPE::IDLE);

}

void Player::InitEffect(void)
{


}

#pragma region �ϐ�state_�ɂ��֐�stateUpdate_�̕ύX

void Player::ChangeState(STATE state)
{
	if (state_ == STATE::END)
	{
		return;
	}
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
void Player::ChangeStateJump(void)
{
	stateUpdate_ = std::bind(&Player::UpdateJump, this);
}
void Player::ChangeStateDead()
{
	playerHp_ = 0;
	stateUpdate_ = std::bind(&Player::UpdateDead, this);
}
void Player::ChangeStateFallDead(void)
{
	playerHp_ = 0;
	stateUpdate_ = std::bind(&Player::UpdateFallDead, this);
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

	ProcessBoost();

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
void Player::UpdateJump(void)
{
	if (invincibleTime_ > 0.0f)
	{
		invincibleTime_ -= SceneManager::GetInstance().GetDeltaTime();
	}
	if (jumpTime_ > 0.0f)
	{
		jumpTime_ -= SceneManager::GetInstance().GetDeltaTime();
		if (jumpTime_ <= 0.0f)
		{
			jumpTime_ = 0.0f;
			//�̗͂��O�Ȃ玀�S��Ԃɂ���
			if (playerHp_ <= 0)
			{
				ChangeState(STATE::DEAD);
				return;
			}
			ChangeState(STATE::PLAY);
			return;
		}
	}

	// �ړ�
	movePow_ =
		VScale(jumpDir_, SPEED_MOVE * jumpTime_);

	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);
	// �ړ���̈ʒu�ɖ�肪�Ȃ���Έړ�
	transform_.pos = movedPos_;
}
void Player::UpdateFallDead(void)
{
	if (jumpTime_ > 0.0f)
	{
		jumpTime_ -= SceneManager::GetInstance().GetDeltaTime();
		// �ړ�
		movePow_ =
			VScale(jumpDir_, SPEED_MOVE * jumpTime_);
		// ���ݍ��W���N�_�Ɉړ�����W�����߂�
		movedPos_ = VAdd(transform_.pos, movePow_);
		// �ړ���̈ʒu�ɖ�肪�Ȃ���Έړ�
		transform_.pos = movedPos_;
		if (jumpTime_ <= 0.0f)
		{
			if (turnTime_ > 5.0f)
			{
				ChangeState(STATE::END);
				return;
			}
			turnTime_ = 2.0f;
			jumpTime_ = 0.0f;
		}
	}
	else
	{
		//�����]����������]�ʂ��N�H�[�^�j�I���ō��
		Quaternion rotPow = Quaternion();
		float dir = 1.0f;

		if (turnTime_ < 0.5f)
		{
			dir = 1.0f;
		}
		else if (turnTime_ < 1.5f)
		{
			dir = -1;
		}

		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(dir), AsoUtility::AXIS_Y
			));

		// ��]�Ȃ�������(����)
		//transform_.quaRot = transform_.quaRot.Mult(rotPow);
		transform_.quaRotLocal = transform_.quaRotLocal.Mult(rotPow);
		transform_.Update();

		turnTime_ -= SceneManager::GetInstance().GetDeltaTime();

		if (turnTime_ <= 0.0f)
		{
			turnTime_ = 10.0f;
			jumpTime_ = 3.0f;
			jumpDir_ = DOWN_DIR;
		}
	}
}
void Player::UpdateDead()
{
	if (jumpTime_ > 0.0f)
	{
		jumpTime_ -= SceneManager::GetInstance().GetDeltaTime();
		// �ړ�
		movePow_ =
			VScale(jumpDir_, SPEED_MOVE * jumpTime_);
		// ���ݍ��W���N�_�Ɉړ�����W�����߂�
		movedPos_ = VAdd(transform_.pos, movePow_);
		// �ړ���̈ʒu�ɖ�肪�Ȃ���Έړ�
		transform_.pos = movedPos_;
		if (jumpTime_ <= 0.0f)
		{
			ChangeState(STATE::END);//�{���Ȃ炱���Ŕ����G�t�F�N�g����
			return;
		}
		return;
	}
	ChangeState(STATE::END);//�{���Ȃ炱���ŃG�t�F�N�g���ЂƂ��Ȃ������甚���G�t�F�N�g����

	//���̐�̏����A�G�t�F�N�g���I��������ChangeState���鏈��

}
void Player::UpdateEnd()
{

}
void Player::UpdateVictory()
{
	InputManager& ins = InputManager::GetInstance();
	InputManager::JOYPAD_NO jno = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD1);

	if (controller_->GetisControl(Controller::MODE::FORWARD))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	if (controller_->GetisControl(Controller::MODE::BACK))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);
	}
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

void Player::ProcessBoost(void)
{
	auto& ins = InputManager::GetInstance();

	//�^�b�N���U��
	if ((controller_->GetisControl(Controller::MODE::JATTACK)
		&& jumpTime_ <= 0.0f))
	{
		VECTOR dir = VScale(transform_.GetForward(), 2.0f);
		SetJump(dir);
	}
}

void Player::ProcessShot(void)
{
	auto& ins = InputManager::GetInstance();

	//�ˌ��U��
	if (controller_->GetisControl(Controller::MODE::ATTACK)
		&& deleyShot_ <= 0.0f && magazineShot_ > 0)
	{
		// �e�𐶐�(�����͉��Ő��ʕ���)
		//shot->CreateShot(barrelPos_, { 0.0f, 0.0f, 1.0f });
		// �e���w��ʒu����A�w������ɔ��˂�����
		CreateShot();
		SoundManager::GetInstance().Play(SoundManager::SRC::ATTRCK, Sound::TIMES::ONCE, true);

		//�e���[�P
		magazineShot_ -= 1;
		//�e���ˌ�̍d�����ԃZ�b�g
		deleyShot_ = TIME_DELAY_SHOT;
	}
	//�e�̃����[�h
	if (controller_->GetisControl(Controller::MODE::XBUTTUN) && (magazineShot_ < magazineMaxShot_))
	{
		reloadTime_ = 0.1f * (magazineMaxShot_ - magazineShot_);
		reloadSet_ = 0.1f * (magazineMaxShot_ - magazineShot_) - 0.1f;
		//reloadTime_ = TIME_RELOAD;
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
			v->Create(transform_.pos, transform_.GetForward(),shotModel_);
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
		newShot->Create(transform_.pos, transform_.GetForward(), shotModel_);

		// �e�̊Ǘ��z��ɒǉ�
		shots_.push_back(newShot);
	}
}
