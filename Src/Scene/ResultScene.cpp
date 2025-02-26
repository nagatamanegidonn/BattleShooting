#include <DxLib.h>

#include "../Application.h"

#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"

#include "ResultScene.h"

ResultScene::ResultScene(void)
{
}

ResultScene::~ResultScene(void)
{
}

void ResultScene::AsyncPreLoad(void)
{
	//非同期読み込みを有効にする
	SetUseASyncLoadFlag(true);

}
void ResultScene::Init(void)
{
	//非同期読み込みを有効にする
	SetUseASyncLoadFlag(false);

	// カメラモード：定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	next = false;
	select = 0;

}

void ResultScene::Update(void)
{

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE) && next == false)
	{
		next = true;
	}
	if (next)
	{
		if (ins.IsTrgDown(KEY_INPUT_LEFT))
		{
			select = 1;
		}
		if (ins.IsTrgDown(KEY_INPUT_RIGHT))
		{
			select = 2;
		}
	}

	if (select == 1)
	{
		if (ins.IsTrgDown(KEY_INPUT_SPACE))
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}
	if(select == 2)
	{
		if (ins.IsTrgDown(KEY_INPUT_SPACE))
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);
		}
	}
}

void ResultScene::Draw(void)
{
	if (next == true)
	{
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0xffffff, true);
		SetFontSize(25);
		if (select == 1) {
			DrawString(Application::SCREEN_SIZE_X / 4, Application::SCREEN_SIZE_Y / 2, "タイトル", RGB(0, 0, 0), true);
		}
		else if(select == 2)
		{
			DrawString(Application::SCREEN_SIZE_X / 4 * 3, Application::SCREEN_SIZE_Y / 2, "キャラ選択に戻る", RGB(0, 0, 0), true);
		}
		SetFontSize(16);
	}
	else if(next == false)
	{
		DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x0ff0ff, true);
	}

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	//メッセージ
	std::string msg = "Result Scene";
	SetFontSize(28);//文字のサイズを設定

	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), 550, 0xffffff, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(16);

}

void ResultScene::Release(void)
{
}
