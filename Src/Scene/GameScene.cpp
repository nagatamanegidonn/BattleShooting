#include <DxLib.h>

#include "../Application.h"

#include "../Utility/AsoUtility.h"//Quaternion���������֐��������Ă���

#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"

#include "../Object/Grid.h"
#include "../Object/Player/Player.h"

#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	float size = 100.0f;

	VECTOR sPos[4] = {
		{-size,0.0f,size}//����
		,{size,0.0f,size}//�E��
		,{-size,0.0f,-size}//����
		,{size,0.0f,-size}//�E�� 
	};


	// ������: i = 1�A������: i <= 5�A�X�V: i++
	for (int i = 0; i < PLAYER_SIZE; i++) {
		auto  player = std::make_shared<Player>();
		player->Init(sPos[i], i);

		players_.push_back(player);

		//�e�v���C���[�̃X�N���[���̍쐬
		screenH[i] = MakeScreen(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y/2, true);

	}

	stage_ = new Grid;
	stage_->Init();

	Camera* camera = SceneManager::GetInstance().GetCamera();

	// �J�������[�h�F��_�J����
	camera->ChangeMode(Camera::MODE::FIXED_POINT);

	int c = 0;
	for (auto p : players_)
	{
		// �J����
		camera_[c] = new Camera();
		camera_[c]->Init();

		camera_[c]->ChangeMode(Camera::MODE::FOLLOW);
		camera_[c]->SetFollow(&p->GetTransform());
		c++;
	}

}

void GameScene::Update(void)
{

	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	}

	stage_->Update();

	//�v���C���[�̍X�V
	for (auto p : players_)
	{
		p->Update();
	}

	for (auto& camera : camera_)
	{
		camera->Update();
	}
}

void GameScene::Draw(void)
{
	for (int i = 0; i < PLAYER_SIZE; i++) 
	{
		// �ݒ肵�����X�N���[�����쐬����
		SetDrawScreen(screenH[i]);

		// ��ʂ�������
		ClearDrawScreen();

		// �J�����ݒ�
		camera_[i]->SetBeforeDraw();

#pragma region �Q�[���V�[���̕`��

		// �`��
		stage_->Draw();

		//�v���C���[�̕`��
		for (auto& p : players_)
		{
			p->Draw();
		}

		//�f�o�b�O�`��
		//DrawDebug();

#pragma endregion

		//�Ȃ�
		SetDrawScreen(DX_SCREEN_BACK);

	}

	// ��ʂ�������
	ClearDrawScreen();

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	int x = 0;
	int y = 0;

	for (auto screen : screenH)
	{
		DrawGraph(x, y, screen, true);

		if (x >= cx && y >= cy)
		{
			x += cx;
		}
		else if (x >= cx && y >= 0)
		{
			x -= cx;
			y += cy;
		}
		else if (x >= 0 && y >= 0)
		{
			x += cx;
		}
	}

	// (�R�c�`��Ŏg�p����J�����̐ݒ�Ȃǂ����Z�b�g�����)
	SetDrawScreen(DX_SCREEN_BACK);

}

void GameScene::Release(void)
{
	for (int i = 0; i < PLAYER_SIZE; i++) {
		camera_[i]->Release();
		delete camera_[i];
	}


	stage_->Release();
}


//�f�o�b�O�`��
void GameScene::DrawDebug(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	//���b�Z�[�W
	std::string msg = "Game Scene";
	SetFontSize(28);//�����̃T�C�Y��ݒ�

	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), 550, 0xffffff, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(16);

}
