#include <chrono>
#include <thread>
#include <DxLib.h>

#include <EffekseerForDXLib.h>

#include "../Common/Fader.h"

#include "../Scene/TitleScene.h"
#include "../Scene/SelectScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/ResultScene.h"

#include "ResourceManager.h"
#include "Camera.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{

	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	fader_ = new Fader();
	fader_->Init();

	// カメラ
	camera_ = new Camera();
	camera_->Init();

	//ゲームの勝敗等変数の初期化
	ResetGame();

	scene_ = new TitleScene();
	scene_->Init();

	isSceneChanging_ = false;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	// 3D用の設定
	Init3D();

	// 初期シーンの設定
	//ChangeScene(SCENE_ID::TITLE);

}

void SceneManager::Init3D(void)
{

	// 背景色設定
	SetBackgroundColor(0, 0, 0);

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	SetUseLighting(true);

	// 正面から斜め下に向かったライト
	ChangeLightTypeDir({ 0.00f, -1.00f, 1.00f });

	// フォグ設定(ステージ導入時)
	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(3000.0f, 15000.0f);

}

void SceneManager::Update(void)
{

	if (scene_ == nullptr)
	{
		return;
	}

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	fader_->Update();
	if (isSceneChanging_)
	{
		Fade();
	}


	scene_->Update();

	// カメラ更新
	camera_->Update();

}

void SceneManager::Draw(void)
{
	
	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	// カメラ設定
	camera_->SetBeforeDraw();

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	// 描画
	scene_->Draw();

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();

	// 暗転・明転
	fader_->Draw();
	
	if (fader_->GetState() == Fader::STATE::LOADING)
	{
		scene_->LoadingDraw();
	}

	// デバック用描画
#ifdef _DEBUG
	SetFontSize(16);
	 // 非同期読み込みの数を描画
	DrawFormatString(0, 0, GetColor(255, 255, 255), "非同期読み込みの数 %d", GetASyncLoadNum());
#endif

}

void SceneManager::Destroy(void)
{

	scene_->Release();
	delete scene_;

	delete fader_;

	camera_->Release();
	delete camera_;

	delete instance_;

}

void SceneManager::ChangeScene(SCENE_ID nextId)
{
	// フェード処理が終わってからシーンを変える場合もあるため、
	// 遷移先シーンをメンバ変数に保持
	waitSceneId_ = nextId;

	// フェードアウト(暗転)を開始する
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	return deltaTime_;
}


Camera* SceneManager::GetCamera(void) const
{
	return camera_;
}

bool SceneManager::IsLoading(void) const
{
	if (fader_->GetState() == Fader::STATE::LOADING)
	{
		return true;
	}

	return false;
}
//
//bool SceneManager::IsLoading(void) const
//{
//	return (fader_->GetState() == Fader::STATE::LOADING);
//}


void SceneManager::ResetGame(void)
{
	isWinner_ = WINNER::NONE;
}
//勝者関係
SceneManager::WINNER SceneManager::GetWinner(void) const
{
	return isWinner_;
}
void SceneManager::SetWinner(WINNER win)
{
	isWinner_ = win;
}


SceneManager::SceneManager(void)
{

	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	scene_ = nullptr;
	fader_ = nullptr;

	isSceneChanging_ = false;

	// デルタタイム
	deltaTime_ = 1.0f / 60.0f;

	camera_ = nullptr;
	
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{

	// リソースの解放
	ResourceManager::GetInstance().Release();

	// シーンを変更する
	sceneId_ = sceneId;

	// 現在のシーンを解放
	if (scene_ != nullptr)
	{
		scene_->Release();
		delete scene_;
	}

	//受け取ったsceneId_でシーンを変更する
	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		scene_ = new TitleScene();
		break;
	case SCENE_ID::SELECT:
		scene_ = new SelectScene();
		break;
	case SCENE_ID::GAME:
		scene_ = new GameScene();
		break;
	case SCENE_ID::RESULT:
		scene_ = new ResultScene();
		break;
	}

	//Initの変わりにAsyncPreLoad
	scene_->AsyncPreLoad();

	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// 明転中
		if (fader_->IsEnd())
		{
			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// 暗転中
		if (fader_->IsEnd())
		{
			// 完全に暗転してからシーン遷移
			DoChangeScene(waitSceneId_);
			// 暗転から  へ
			fader_->SetFade(Fader::STATE::LOADING);
		}
		break;
	case Fader::STATE::LOADING:
		// 暗転中

		//シーンがすべて読み込めたか確認
		if (scene_->IsLoad())
		{

			//初期化
			scene_->Init();

			// 暗転から明転へ
			fader_->SetFade(Fader::STATE::FADE_IN);

		}
		break;
	}

}


