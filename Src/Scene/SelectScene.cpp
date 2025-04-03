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

	p1Img_ = LoadGraph("Data/Image/P1MushImage.png");
	p2Img_ = LoadGraph("Data/Image/P2MushImage.png");

	Cursor_ = LoadGraph("Data/Image/Cursor.png");

	pos[0] = AsoUtility::VECTOR_ZERO;
	pos[1] = AsoUtility::VECTOR_ZERO;

	pos[0].x = Application::SCREEN_SIZE_X / 3;
	pos[0].y = Application::SCREEN_SIZE_Y / 3;

	pos[1].x = (Application::SCREEN_SIZE_X / 3) * 2;
	pos[1].y = Application::SCREEN_SIZE_Y / 3;

	//�Q�[���J�n�����m�F�p�t���O
	for (int ii = 0; ii < PLAYER_MAX; ii++)
	{
		isReady_[ii] = false;
	}
	isStart_ = false;
	

	float size = 500.0f;

	//�v���C���[�̐ݒ�
	VECTOR sPos[4] = {
		{-size,0.0f,size / 2}//����
		,{size,0.0f,size / 2}//�E��
		,{-size,0.0f,-size}//����
		,{size,0.0f,-size}//�E�� 
	};

	// ������: i = 1�A������: i <= 5�A�X�V: i++
	for (int i = 0; i < PLAYER_MAX; i++) {
		players_[i]->Init(sPos[i], i, i);
		players_[i]->ChangeState(Player::STATE::NONE);
	}

	for (int ii = 0; ii > PLAYER_MAX; ii++)
	{
		select[ii] = SELECT_TYPE_NON;
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
	for (int ii = 0; ii < PLAYER_MAX; ii++)
	{
		// �v���C���[�P�ƃv���C���[�Q�����������{�^���������ăX�y�[�X�������ƃQ�[���V�[���Ɉڍs
		if (isStart_ && (ins.IsTrgDown(KEY_INPUT_SPACE)
			|| (ins.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::START))
			|| (ins.IsPadBtnNew(jno2, InputManager::JOYPAD_BTN::START))))
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

	DrawFormatString(0, 16, 0xffffff, "1,2�Ō���");
	DrawFormatString(0, 16, 0xffffff, "Space�L�[�ŃX�^�[�g");


	//�w�i
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x00ff00, true);

	//�L�����I��

	DrawBox(0, 0, Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0xfff000, true);
	DrawBox(Application::SCREEN_SIZE_X / 2, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y / 2, 0x000fff, true);


	//�v���C���[�P�����������������ǂ���
	SetFontSize(25);
	if (isReady_[0]) {
		DrawString(0, Application::SCREEN_SIZE_Y / 2, "p1_OK", RGB(0, 0, 255), true);

		if (select[0] == SELECT_TYPE_1)
		{
			DrawRotaGraph(Application::SCREEN_SIZE_X / 4, Application::SCREEN_SIZE_Y / 4 * 3, 0.3f, 0.0f, p1Img_, true);
		}
		else if (select[0] == SELECT_TYPE_2)
		{
			DrawRotaGraph(Application::SCREEN_SIZE_X / 4, Application::SCREEN_SIZE_Y / 4 * 3, 0.3f, 0.0f, p2Img_, true);
		}
	}

	//�v���C���[�P�����������������ǂ���
	if (isReady_[1]) {
		DrawString(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, "p2_OK", RGB(0, 0, 255), true);
		if (select[1] == SELECT_TYPE_1)
		{
			DrawRotaGraph(Application::SCREEN_SIZE_X / 4 * 3, Application::SCREEN_SIZE_Y / 4 * 3, 0.3f, 0.0f, p1Img_, true, true);
		}
		else if (select[1] == SELECT_TYPE_2)
		{
			DrawRotaGraph(Application::SCREEN_SIZE_X / 4 * 3, Application::SCREEN_SIZE_Y / 4 * 3, 0.3f, 0.0f, p2Img_, true ,true);
		}

		//
	
	}

	//�v���C���[�̍X�V
	for (auto p : players_)
	{
		p->Draw();
	}
	
	//�v���C���[�P�̃J�[�\���i���j
	//DrawBox(pos[0].x - SIZE, pos[0].y - SIZE, pos[0].x + SIZE, pos[0].y + SIZE, 0x000000, true);
	DrawRotaGraph(pos[0].x, pos[0].y, 0.03f, 0.0f, Cursor_, true);

	//�v���C���[�Q�̃J�[�\���i���j
	//DrawBox(pos[1].x - SIZE, pos[1].y - SIZE, pos[1].x + SIZE, pos[1].y + SIZE, 0x000000, true);
	DrawRotaGraph(pos[1].x, pos[1].y, 0.03f, 0.0f, Cursor_, true);
	//-----------------------------------------------------

	
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
	InputManager::JOYPAD_NO jno2 = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD2);

	// ���X�e�B�b�N�̉���
	auto leftStickX = ins.GetJPadInputState(jno).AKeyLX;
	// ���X�e�B�b�N�̏c��
	auto leftStickY = ins.GetJPadInputState(jno).AKeyLY;


	if (isReady_[0] == false)
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

	// ���X�e�B�b�N�̉���
	auto leftStick2X = ins.GetJPadInputState(jno2).AKeyLX;
	// ���X�e�B�b�N�̏c��
	auto leftStick2Y = ins.GetJPadInputState(jno2).AKeyLY;

	if (isReady_[1] == false)
	{
		if (CheckHitKey(KEY_INPUT_UP) || (leftStick2Y < 0))
		{
			P2.y -= MOVE;
		}
		if (CheckHitKey(KEY_INPUT_DOWN) || (leftStick2Y > 0))
		{
			P2.y += MOVE;
		}
		if (CheckHitKey(KEY_INPUT_RIGHT) || (leftStick2X > 0))
		{
			P2.x += MOVE;
		}
		if (CheckHitKey(KEY_INPUT_LEFT) || (leftStick2X < 0))
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
			if (ins.IsTrgDown(KEY_INPUT_1) || ins.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::RIGHT))
			{
				if (isReady_[0] == false)
				{
					isReady_[0] = true;

					if (ii == 0)
					{
						select[0] = SELECT_TYPE_1;
					}
					else if (ii == 1)
					{
						select[0] = SELECT_TYPE_2;
					}
					//
					SceneManager::GetInstance().SetPlayerId(0, ii);
				}
				else
				{
					SceneManager::GetInstance().SetPlayerId(0, -1);
					isReady_[0] = false;
					select[0] = SELECT_TYPE_NON;
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
			if (ins.IsTrgDown(KEY_INPUT_2) || ins.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::RIGHT))
			{
				if (isReady_[1] == false)
				{
					isReady_[1] = true;

					if (ii == 0)
					{
						select[1] = SELECT_TYPE_1;
					}
					else if (ii == 1)
					{
						select[1] = SELECT_TYPE_2;
					}

					//
					SceneManager::GetInstance().SetPlayerId(1, ii);

				}
				else
				{
					SceneManager::GetInstance().SetPlayerId(1, -1);
					isReady_[1] = false;
					select[0] = SELECT_TYPE_NON;
				}
			}
		}
	}

	//�S�v���C���[�����������Ȃ�
	if (isReady_[0] && isReady_[1])
	{
		isStart_ = true;
	}
	else {
		isStart_ = false;
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