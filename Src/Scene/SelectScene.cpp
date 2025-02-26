#include <DxLib.h>

#include "../Application.h"

#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"

#include "../Utility/AsoUtility.h"

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
}
void SelectScene::Init(void)
{
	//�񓯊��ǂݍ��݂�L���ɂ���
	SetUseASyncLoadFlag(false);

	// �J�������[�h�F��_�J����
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);


	for (int ii = 0; ii < PLAYER_MAX; ii++)
	{
		pos[ii] = AsoUtility::VECTOR_ZERO;

		// �J�[�\���̏������W
		pos[ii].x = Application::SCREEN_SIZE_X / 4 + ((Application::SCREEN_SIZE_X / 4 * 2) * ii);
		pos[ii].y = Application::SCREEN_SIZE_Y / 2;
	}

	// �����������̏�Ԃɏ�����
	for (int ii = 0; ii < PLAYER_MAX; ii++)start[ii] = false;
	
	// �L���������I����Ԃɏ�����
	for(int ii = 0; ii < PLAYER_MAX; ii++)chara[ii] = CHARA::E_CHARA_NON;
}

void SelectScene::Update(void)
{
	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	for (int ii = 0; ii < PLAYER_MAX; ii++)
	{
		// �v���C���[�P�ƃv���C���[�Q�����������{�^���������ăX�y�[�X�������ƃQ�[���V�[���Ɉڍs
		if (start[ii] && ins.IsTrgDown(KEY_INPUT_SPACE)) {
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		}
	}

	// BACKSPACE
	if (ins.IsTrgDown(KEY_INPUT_BACK))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	// �J�[�\���ړ�
	GetMove(pos[0], pos[1]);

	// �L�����I�����̓����蔻��
	Collision();

}

void SelectScene::Draw(void)
{
	//-----------------------------------------------------
	//�f�o�b�N�p

	//�w�i
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x00ff00, true);

	//�L����
	DrawBox(0, 0, Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0xfff000, true);
	DrawBox(Application::SCREEN_SIZE_X / 2, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y / 2, 0x000fff, true);


	//�v���C���[�P�����������������ǂ���

	//�v���C���[�P�̃J�[�\���i���j
	if (start[0] == false)
	{
		DrawBox(pos[0].x - SIZE, pos[0].y - SIZE, pos[0].x + SIZE, pos[0].y + SIZE, 0x000000, true);
	}
	else 
	{
		DrawBox(pos[0].x - SIZE, pos[0].y - SIZE, pos[0].x + SIZE, pos[0].y + SIZE, 0xffffff, true);
	}

	//�v���C���[�Q�̃J�[�\���i���j
	if (start[1] == false)
	{
		DrawBox(pos[1].x - SIZE, pos[1].y - SIZE, pos[1].x + SIZE, pos[1].y + SIZE, 0x000000, true);
	}
	else
	{
		DrawBox(pos[1].x - SIZE, pos[1].y - SIZE, pos[1].x + SIZE, pos[1].y + SIZE, 0xffffff, true);
	}

	SetFontSize(25);

	// �v���C���[�P�̃L�����̏��
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

	// �v���C���[�Q�̃L�����̏��
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

	//-----------------------------------------------------
}

void SelectScene::Release(void)
{

}

//�L�����I����ʂ̃J�[�\���ړ��p(��)
void SelectScene::GetMove(VECTOR& P1, VECTOR& P2)
{
	//------------------------------------------

	if (start[0] == false)
	{
		if (CheckHitKey(KEY_INPUT_W))
		{
			P1.y -= MOVE;
		}
		if (CheckHitKey(KEY_INPUT_S))
		{
			P1.y += MOVE;
		}
		if (CheckHitKey(KEY_INPUT_D))
		{
			P1.x += MOVE;
		}
		if (CheckHitKey(KEY_INPUT_A))
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

	for (int ii = 0; ii < CHARACTER_MAX; ii++)
	{
		if (pos[0].x < Application::SCREEN_SIZE_X / 2 + (ii * Application::SCREEN_SIZE_X / 2) &&
			pos[0].x > ii * (Application::SCREEN_SIZE_X / 2) &&
			pos[0].y < Application::SCREEN_SIZE_Y / 2 &&
			pos[0].y > 0 &&
			ins.IsTrgDown(KEY_INPUT_1))
		{
			// �V�[���J��
			// �v���C���[�P�������������Ă��邩�ǂ���
			// true == �������� / false == ������
			if (start[0] == false)
			{
				start[0] = true;

				// �ǂ���̃L������I���������ǂ���
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
				// �����������Ă��Ȃ����
				start[0] = false;
				chara[0] = E_CHARA_NON;
			}
			
		}
	}


	for (int ii = 0; ii < CHARACTER_MAX; ii++)
	{
		if (pos[1].x < Application::SCREEN_SIZE_X / 2 + (ii * Application::SCREEN_SIZE_X / 2) &&
			pos[1].x > ii * (Application::SCREEN_SIZE_X / 2) &&
			pos[1].y < Application::SCREEN_SIZE_Y / 2 &&
			pos[1].y > 0 &&
			ins.IsTrgDown(KEY_INPUT_2))
		{
			// �v���C���[�Q�������������Ă��邩�ǂ���
			// true == �������� / false == ������
			if (start[1] == false)
			{
				start[1] = true;

				// �ǂ���̃L������I���������ǂ���
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
				// �����������Ă��Ȃ����
				start[1] = false;
				chara[1] = E_CHARA_NON;
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
