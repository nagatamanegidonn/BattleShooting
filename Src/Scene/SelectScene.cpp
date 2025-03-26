#include <DxLib.h>

#include "../Application.h"

#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"

#include "../Utility/AsoUtility.h"

#include "../Object/Player/Player.h"

#include "SelectScene.h"

SelectScene::SelectScene(void)
{
}

SelectScene::~SelectScene(void)
{
}

void SelectScene::AsyncPreLoad(void)
{
	//�񓯊��ǂݍ��݂�L���ɂ���
	SetUseASyncLoadFlag(true);

	// ������: i = 1�A������: i <= 5�A�X�V: i++
	for (int i = 0; i < PLAYER_MAX; i++) {

		auto  player = std::make_shared<Player>();
		players_.push_back(player);
	}
}
void SelectScene::Init(void)
{
	//�񓯊��ǂݍ��݂�L���ɂ���
	SetUseASyncLoadFlag(false);

	// �J�������[�h�F��_�J����
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);


	pos[0] = AsoUtility::VECTOR_ZERO;
	pos[1] = AsoUtility::VECTOR_ZERO;

	pos[0].x = Application::SCREEN_SIZE_X / 3;
	pos[0].y = Application::SCREEN_SIZE_Y / 2;

	pos[1].x = (Application::SCREEN_SIZE_X / 3) * 2;
	pos[1].y = Application::SCREEN_SIZE_Y / 2;

	for (int ii = 0; ii < PLAYER_MAX; ii++)
	{
		start[ii] = false;
	}

	for(int ii = 0; ii < PLAYER_MAX; ii++)chara[ii] = CHARA::E_CHARA_NON;

	float size = 100.0f;

	//�v���C���[�̐ݒ�
	VECTOR sPos[4] = {
		{-size,0.0f,size}//����
		,{size,0.0f,size}//�E��
		,{-size,0.0f,-size}//����
		,{size,0.0f,-size}//�E�� 
	};

	// ������: i = 1�A������: i <= 5�A�X�V: i++
	for (int i = 0; i < PLAYER_MAX; i++) {
		players_[i]->Init(sPos[i], i);
		players_[i]->ChangeState(Player::STATE::NONE);
	}
}

void SelectScene::Update(void)
{
	//���[�h���������������f
	if (GetASyncLoadNum() != 0 || SceneManager::GetInstance().IsLoading())
	{
		return;
	}

	InputManager& ins = InputManager::GetInstance();

	
	InputManager::JOYPAD_NO jno = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD1);
	InputManager::JOYPAD_NO jno2 = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD2);

	// ���X�e�B�b�N�̉���
	auto leftStickX = ins.GetJPadInputState(jno).AKeyLX;
	// ���X�e�B�b�N�̏c��
	auto leftStickY = ins.GetJPadInputState(jno).AKeyLY;

	(ins.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::RIGHT));
	
	// �V�[���J��
	if (ins.IsTrgDown(KEY_INPUT_SPACE)||(ins.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::RIGHT)))
	{
	//	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}
	
	for (int ii = 0; ii < PLAYER_MAX; ii++)
	{
		// �v���C���[�P�ƃv���C���[�Q�����������{�^���������ăX�y�[�X�������ƃQ�[���V�[���Ɉڍs
		if (start[ii] && ins.IsTrgDown(KEY_INPUT_SPACE)
			|| (start[ii] && (ins.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::START)))
			|| (start[ii] && (ins.IsPadBtnNew(jno2, InputManager::JOYPAD_BTN::START))))
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
			return;
		}
	}

	// BACKSPACE
	if (ins.IsTrgDown(KEY_INPUT_BACK))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	//�J�[�\���ړ�
	GetMove(pos[0], pos[1]);

	// �L�����I�����̓����蔻��
	Collision();

	//�v���C���[�̍X�V
	for (auto p : players_)
	{
		p->Update();
	}
}

void SelectScene::Draw(void)
{
	//���[�h���������������f
	if (GetASyncLoadNum() != 0 || SceneManager::GetInstance().IsLoading())
	{
		return;
	}
	//-----------------------------------------------------
	//�f�o�b�N�p

	//�w�i
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x00ff00, true);

	//�L�����I��

	DrawBox(0, 0, Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0xfff000, true);
	DrawBox(Application::SCREEN_SIZE_X / 2, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y / 2, 0x000fff, true);


	//�v���C���[�P�����������������ǂ���
	SetFontSize(25);
	if (start[0]) {
		DrawString(0, 0, "p1", RGB(0, 0, 255), true);
	}

	//�v���C���[�P�����������������ǂ���
	if (start[1]) {
		DrawString(0, 0, "p2", RGB(0, 0, 255), true);
	}

	//�v���C���[�̍X�V
	for (auto p : players_)
	{
		p->Draw();
	}
	
	//�v���C���[�P�̃J�[�\���i���j
	DrawBox(pos[0].x - SIZE, pos[0].y - SIZE, pos[0].x + SIZE, pos[0].y + SIZE, 0x000000, true);

	//�v���C���[�Q�̃J�[�\���i���j
	DrawBox(pos[1].x - SIZE, pos[1].y - SIZE, pos[1].x + SIZE, pos[1].y + SIZE, 0x000000, true);
	//-----------------------------------------------------

	switch (chara[0])
	{
	case E_CHARA_NON:
		break;
	case E_CHARA1:
		DrawString(0, Application::SCREEN_SIZE_Y / 2, "�v�C�����[�P/�L�����N�^�[�P", RGB(0, 0, 255), true);
		break;
	case E_CHARA2:
		DrawString(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, "�v�C�����[�P/�L�����N�^�[2", RGB(0, 0, 255), true);
		break;
	}

	switch (chara[1])
	{
	case E_CHARA_NON:
		break;
	case E_CHARA1:
		DrawString(0, Application::SCREEN_SIZE_Y / 2 + 50 , "�v�C�����[�Q/�L�����N�^�[�P", RGB(0, 0, 255), true);
		break;
	case E_CHARA2:
		DrawString(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2 + 50, "�v�C�����[�Q/�L�����N�^�[2", RGB(0, 0, 255), true);
		break;
	}
}

void SelectScene::Release(void)
{

}

//�L�����I����ʂ̃J�[�\���ړ��p(��)
void SelectScene::GetMove(VECTOR& P1, VECTOR& P2)
{
	//------------------------------------------
	InputManager& ins = InputManager::GetInstance();


	InputManager::JOYPAD_NO jno = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD1);

	// ���X�e�B�b�N�̉���
	auto leftStickX = ins.GetJPadInputState(jno).AKeyLX;
	// ���X�e�B�b�N�̏c��
	auto leftStickY = ins.GetJPadInputState(jno).AKeyLY;


	if (start[0] == false)
	{
		if (CheckHitKey(KEY_INPUT_W) || (leftStickY < 0))
		{
			P1.y -= MOVE;
		}
		if (CheckHitKey(KEY_INPUT_S) || (leftStickY > 0))
		{
			P1.y += MOVE;
		}
		if (CheckHitKey(KEY_INPUT_D) || (leftStickX > 0))
		{
			P1.x += MOVE;
		}
		if (CheckHitKey(KEY_INPUT_A) || (leftStickX < 0))
		{
			P1.x -= MOVE;
		}
	}


	//------------------------------------------

	if (start[1] == false)
	{
		if (CheckHitKey(KEY_INPUT_UP))
		{
			P2.y -= MOVE;
		}
		if (CheckHitKey(KEY_INPUT_DOWN))
		{
			P2.y += MOVE;
		}
		if (CheckHitKey(KEY_INPUT_RIGHT))
		{
			P2.x += MOVE;
		}
		if (CheckHitKey(KEY_INPUT_LEFT))
		{
			P2.x -= MOVE;
		}
	}


	//------------------------------------------
}

// �L�����I�����̓����蔻��
void SelectScene::Collision(void)
{
	InputManager& ins = InputManager::GetInstance();

	InputManager::JOYPAD_NO jno = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD1);
	InputManager::JOYPAD_NO jno2 = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD2);

	for (int ii = 0; ii < CHARACTER_MAX; ii++)
	{
		if (pos[0].x < Application::SCREEN_SIZE_X / 2 + (ii * Application::SCREEN_SIZE_X / 2) &&
			pos[0].x > ii * (Application::SCREEN_SIZE_X / 2) &&
			pos[0].y < Application::SCREEN_SIZE_Y / 2 &&
			pos[0].y > 0)
		{
			// �V�[���J��
			// �v���C���[�P�������������Ă��邩�ǂ���
			// true == �������� / false == ������
			if (ins.IsTrgDown(KEY_INPUT_1))
			{
				if (start[0] == false)
				{
					start[0] = true;


					if (ii == 0)
					{
						chara[0] = E_CHARA1;
					}
					else if (ii == 1)
					{
						chara[0] = E_CHARA2;
					}
				}
				else
				{
					start[0] = false;
					chara[0] = E_CHARA_NON;
				}
			}
		}
	}


	for (int ii = 0; ii < CHARACTER_MAX; ii++)
	{
		if (pos[1].x < Application::SCREEN_SIZE_X / 2 + (ii * Application::SCREEN_SIZE_X / 2) &&
			pos[1].x > ii * (Application::SCREEN_SIZE_X / 2) &&
			pos[1].y < Application::SCREEN_SIZE_Y / 2 &&
			pos[1].y > 0)
		{
			// �v���C���[�Q�������������Ă��邩�ǂ���
			// true == �������� / false == ������
			if (ins.IsTrgDown(KEY_INPUT_2))
			{
				if (start[1] == false)
				{
					start[1] = true;

					if (ii == 0)
					{
						chara[1] = E_CHARA1;
					}
					else if (ii == 1)
					{
						chara[1] = E_CHARA2;
					}
				}
				else
				{
					start[1] = false;
					chara[1] = E_CHARA_NON;
				}
			}
		}
	}

	//-------------------------------------------------------------------
	// �J�[�\���Ɖ�ʒ[�̓����蔻��
	for (int ii = 0; ii < PLAYER_MAX; ii++)
	{
		// ��
		if (pos[ii].x - SIZE < 0)
		{
			pos[ii].x += MOVE;
		}

		// �E
		if (pos[ii].x + SIZE > Application::SCREEN_SIZE_X)
		{
			pos[ii].x -= MOVE;
		}

		// ��
		if (pos[ii].y - SIZE < 0)
		{
			pos[ii].y += MOVE;
		}

		// ��
		if (pos[ii].y + SIZE > Application::SCREEN_SIZE_Y)
		{
			pos[ii].y -= MOVE;
		}
	}
}