#include <DxLib.h>

#include "../Application.h"

#include "../Utility/AsoUtility.h"//Quaternion等を扱う関数が入っている

#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"

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

	// 初期化: i = 1、条件式: i <= 5、更新: i++
	for (int i = 1; i <= 1; i++) {
		auto  player = std::make_shared<Player>();
		player->Init();

		players_.push_back(player);

	}

	// カメラモード：定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);
}

void GameScene::Update(void)
{

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	}

	//プレイヤーの更新
	for (auto p : players_)
	{
		p->Update();
	}

}

void GameScene::Draw(void)
{
	//DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0xff0000, true);
	
	//プレイヤーの描画
	for (auto p : players_)
	{
		p->Draw();
	}

	//デバッグ描画
	//DrawDebug();

}

void GameScene::Release(void)
{
	
}


//デバッグ描画
void GameScene::DrawDebug(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	//メッセージ
	std::string msg = "Game Scene";
	SetFontSize(28);//文字のサイズを設定

	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), 550, 0xffffff, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(16);

}
