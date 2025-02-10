#include <DxLib.h>

#include "../Application.h"

#include "../Utility/AsoUtility.h"//Quaternion���������֐��������Ă���

#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"

#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{

	// �J�������[�h�F��_�J����
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

}

void GameScene::Update(void)
{

	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	}

}

void GameScene::Draw(void)
{
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0xff0000, true);
	
	//�f�o�b�O�`��
	DrawDebug();

}

void GameScene::Release(void)
{
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
