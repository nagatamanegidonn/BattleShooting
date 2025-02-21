#include <DxLib.h>

#include "../Application.h"

#include "../Utility/AsoUtility.h"//Quaternion等を扱う関数が入っている

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

void GameScene::AsyncPreLoad(void)
{
	//非同期読み込みを有効にする
	SetUseASyncLoadFlag(true);

	// 初期化: i = 1、条件式: i <= 5、更新: i++
	for (int i = 0; i < PLAYER_SIZE; i++) {
		camera_[i] = new Camera();

		auto  player = std::make_shared<Player>(*camera_[i]);
		players_.push_back(player);
	}

	// 複数画像
	handleIds_ = new int[10 * 6];
	LoadDivGraph(
		"Alphabet.png",
		10 * 6,
		10, 6,
		32, 192 / 6,
		&handleIds_[0]);
	// 複数画像
	handleIds_2 = new int[10 * 6];
	LoadDivGraph(
		"Alphabet.png",
		10 * 6,
		10, 6,
		32, 192 / 6,
		&handleIds_2[0]);

	stage_ = new Grid;
}
void GameScene::Init(void)
{
	//非同期処理を無効にする
	SetUseASyncLoadFlag(false);

	float size = 100.0f;

	VECTOR sPos[4] = {
		{-size,0.0f,size}//左上
		,{size,0.0f,size}//右上
		,{-size,0.0f,-size}//左下
		,{size,0.0f,-size}//右上 
	};


	// 初期化: i = 1、条件式: i <= 5、更新: i++
	for (int i = 0; i < PLAYER_SIZE; i++) {
		players_[i]->Init(sPos[i], i);

		//各プレイヤーのスクリーンの作成
		screenH[i] = MakeScreen(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y, true);

	}

	stage_->Init();

	Camera* camera = SceneManager::GetInstance().GetCamera();
	// カメラモード：定点カメラ
	camera->ChangeMode(Camera::MODE::FIXED_POINT);

	int c = 0;
	for (auto p : players_)
	{
		// カメラ
		camera_[c]->Init();

		camera_[c]->ChangeMode(Camera::MODE::FOLLOW);
		camera_[c]->SetFollow(&p->GetTransform());
		c++;
	}

}

void GameScene::Update(void)
{
	//ロードが完了したか判断
	if (GetASyncLoadNum() != 0 || SceneManager::GetInstance().IsLoading())
	{
		return;
	}

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	}

	stage_->Update();

	//プレイヤーの更新
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
	//ロードが完了したか判断
	if (GetASyncLoadNum() != 0 || SceneManager::GetInstance().IsLoading())
	{	
		return;
	}


#pragma region ゲームシーンの描画

	// 描画
	stage_->Draw();

	//プレイヤーの描画
	for (auto& p : players_)
	{
		p->Draw();
	}

	//デバッグ描画
	//DrawDebug();

#pragma endregion
	return;


//	for (int i = 0; i < PLAYER_SIZE; i++) 
//	{
//		// 設定したいスクリーンを作成する
//		SetDrawScreen(screenH[i]);
//
//		// 画面を初期化
//		ClearDrawScreen();
//
//		// カメラ設定
//		camera_[i]->SetBeforeDraw();
////
//#pragma region ゲームシーンの描画
//
//		// 描画
//		stage_->Draw();
//
//		//プレイヤーの描画
//		for (auto& p : players_)
//		{
//			p->Draw();
//		}
//
//		//デバッグ描画
//		//DrawDebug();
//
//#pragma endregion
//
//		//つなぎ
//		SetDrawScreen(DX_SCREEN_BACK);
//
//	}
	
	SetDrawScreen(DX_SCREEN_BACK);
	// 画面を初期化
	ClearDrawScreen();

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	int x = 0;
	int y = 0;

	//描画場所
	/*for (auto screen : screenH)
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
	}*/


	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

}

void GameScene::Release(void)
{
	int num = 10 * 6;
	for (int i = 0; i < num; i++)
	{
		DeleteGraph(handleIds_[i]);
	}
	delete[] handleIds_;
	
	num = 10 * 6;
	for (int i = 0; i < num; i++)
	{
		DeleteGraph(handleIds_2[i]);
	}
	delete[] handleIds_2;



	for (int i = 0; i < PLAYER_SIZE; i++) {
		camera_[i]->Release();
		delete camera_[i];
	}


	stage_->Release();
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
