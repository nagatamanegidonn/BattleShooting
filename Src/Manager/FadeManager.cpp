#include "../Application.h"
#include "SceneManager.h"
#include "FadeManager.h"


FadeManager* FadeManager::instance_ = nullptr;

void FadeManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new FadeManager();
	}

	// 状態管理（インスタンス作成後に）
	instance_->sceneChanges_.emplace(SCENE::NORMAL, std::bind(&FadeManager::ChangeSceneNone, instance_));
	instance_->sceneChanges_.emplace(SCENE::GAME, std::bind(&FadeManager::ChangeSceneGame, instance_));

	instance_->Init();
}

FadeManager& FadeManager::GetInstance(void)
{
	// TODO: return ステートメントをここに挿入します
	return *instance_;
}

FadeManager::FadeManager(void)
{
	
}

void FadeManager::Init(void)
{
	ChangeState(SCENE::NORMAL);

	manualImg_ = LoadGraph((Application::PATH_IMAGE + "Manual.png").c_str());
}

void FadeManager::Draw(void)
{
	sceneDraw_();
}


void FadeManager::Release(void)
{
}

void FadeManager::Destroy(void)
{
	DeleteGraph(manualImg_);
}

void FadeManager::ChangeState(const SCENE scene)
{
	// 各状態遷移の初期処理
	sceneChanges_[scene]();

}

// 状態遷移
void FadeManager::ChangeSceneNone(void)
{
	sceneDraw_ = std::bind(&FadeManager::FadeNormalDraw, instance_);
	loadMinCnt_ = 3;

}
void FadeManager::ChangeSceneGame(void)
{
	sceneDraw_ = std::bind(&FadeManager::FadeGameDraw, instance_);
	loadMinCnt_ = 8;

}


//描画処理
void FadeManager::FadeNormalDraw(void)
{
	DrawBox(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		0x000000, true);
}
void FadeManager::FadeGameDraw(void)
{
	const int cx = Application::SCREEN_SIZE_X / 2;
	const int cy = Application::SCREEN_SIZE_Y / 2;

	DrawBox(
		0, 0,
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y,
		0x000000, true);

	if (SceneManager::GetInstance().IsLoading())
	{
		DrawRotaGraph(cx, cy, 1.0f, 0.0f, manualImg_, true);
	}
}
