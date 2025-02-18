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


	pos[0] = AsoUtility::VECTOR_ZERO;
	pos[1] = AsoUtility::VECTOR_ZERO;

	pos[0].x = Application::SCREEN_SIZE_X / 3;
	pos[0].y = Application::SCREEN_SIZE_Y / 2;

	pos[1].x = (Application::SCREEN_SIZE_X / 3) * 2;
	pos[1].y = Application::SCREEN_SIZE_Y / 2;

	startP1 = startP2 = false;
}

void SelectScene::Update(void)
{
	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	// �v���C���[�P�ƃv���C���[�Q�����������{�^���������ăX�y�[�X�������ƃQ�[���V�[���Ɉڍs
	if (startP1 && startP2 && ins.IsTrgDown(KEY_INPUT_SPACE)) {
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	//�J�[�\���ړ�
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

	//�L�����I��
	for (int ii = 1; ii <= 4; ii++) {
		DrawBox(
			180 * (1 * ii),
			Application::SCREEN_SIZE_Y / 2 - 50,
			(180 * (1 * ii)) + 100,
			(Application::SCREEN_SIZE_Y / 2) + 50, 0x000fff, true);
	}

	//�v���C���[�P�����������������ǂ���
	SetFontSize(25);
	if (startP1) {
		DrawString(0, 0, "p1", RGB(0, 0, 255), true);
	}

	//�v���C���[�P�����������������ǂ���
	if (startP2) {
		DrawString(0, 0, "p2", RGB(0, 0, 255), true);
	}

	//�v���C���[�P�̃J�[�\���i���j
	DrawBox(pos[0].x - SIZE, pos[0].y - SIZE, pos[0].x + SIZE, pos[0].y + SIZE, 0x000000, true);

	//�v���C���[�Q�̃J�[�\���i���j
	DrawBox(pos[1].x - SIZE, pos[1].y - SIZE, pos[1].x + SIZE, pos[1].y + SIZE, 0x000000, true);
	//-----------------------------------------------------

}

void SelectScene::Release(void)
{

}

//�L�����I����ʂ̃J�[�\���ړ��p(��)
void SelectScene::GetMove(VECTOR& P1, VECTOR& P2)
{
	//------------------------------------------

	if (startP1 == false)
	{
		if (CheckHitKey(KEY_INPUT_W))
		{
			P1.y -= 5;
		}
		if (CheckHitKey(KEY_INPUT_S))
		{
			P1.y += 5;
		}
		if (CheckHitKey(KEY_INPUT_D))
		{
			P1.x += 5;
		}
		if (CheckHitKey(KEY_INPUT_A))
		{
			P1.x -= 5;
		}
	}


	//------------------------------------------

	if (startP2 == false)
	{
		if (CheckHitKey(KEY_INPUT_UP))
		{
			P2.y -= 5;
		}
		if (CheckHitKey(KEY_INPUT_DOWN))
		{
			P2.y += 5;
		}
		if (CheckHitKey(KEY_INPUT_RIGHT))
		{
			P2.x += 5;
		}
		if (CheckHitKey(KEY_INPUT_LEFT))
		{
			P2.x -= 5;
		}
	}


	//------------------------------------------
}

// �L�����I�����̓����蔻��
void SelectScene::Collision(void)
{
	InputManager& ins = InputManager::GetInstance();

	for (int ii = 1; ii <= 4; ii++)
	{
		if (pos[0].x < (180 * (1 * ii)) + 100 &&
			pos[0].x > 180 * (1 * ii) &&
			pos[0].y < (Application::SCREEN_SIZE_Y / 2) + 50 &&
			pos[0].y > Application::SCREEN_SIZE_Y / 2 - 50)
		{
			// �V�[���J��
			// �v���C���[�P�������������Ă��邩�ǂ���
			// true == �������� / false == ������
			if (ins.IsTrgDown(KEY_INPUT_1))
			{
				if (startP1 == false)
				{
					startP1 = true;
				}
				else
				{
					startP1 = false;
				}
			}
		}
	}


	for (int ii = 1; ii <= 4; ii++)
	{
		if (pos[1].x < (180 * (1 * ii)) + 100 &&
			pos[1].x > 180 * (1 * ii) &&
			pos[1].y < (Application::SCREEN_SIZE_Y / 2) + 50 &&
			pos[1].y > Application::SCREEN_SIZE_Y / 2 - 50)
		{
			// �v���C���[�Q�������������Ă��邩�ǂ���
			// true == �������� / false == ������
			if (ins.IsTrgDown(KEY_INPUT_2))
			{
				if (startP2 == false)
				{
					startP2 = true;
				}
				else
				{
					startP2 = false;
				}
			}
		}
	}
}