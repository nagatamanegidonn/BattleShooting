#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/BattleManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/Camera.h"

#include "../Object/Stage.h"
#include "../Object/Player/GameClass/Player.h"

#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	imgTitleLogo_ = -1;
	pushImg_ = -1;

	sceneTotalTime_ = 0.0f;
	sizeRate_ = (std::sin(sceneTotalTime_) * 0.5f + 0.5f) * (1.2f - 0.8f) + 0.8f;  // 0.8～1.2 を往復
	sizeRate_ *= 0.25f;

	cntSound_ = 0;
	stage_ = nullptr;

	isSceneChange_ = false;
	sceneChangeTime_ = 0.0f;


}

TitleScene::~TitleScene(void)
{
	DeleteGraph(pushImg_);
}

void TitleScene::AsyncPreLoad(void)
{
	//非同期読み込みを有効にする
	SetUseASyncLoadFlag(true);

}
void TitleScene::Init(void)
{
	//非同期読み込みを無効にする
	SetUseASyncLoadFlag(false);

	// カメラモード：定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// タイトルロゴ
	imgTitleLogo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TITLE_LOGO).handleId_;

	pushImg_ = LoadGraph((Application::PATH_IMAGE + "Button.png").c_str());

	//背景初期化
	stage_ = new Stage;
	stage_->Init();

	cntSound_ = 0;


	SoundManager::GetInstance().Play(SoundManager::SRC::TITLE_BGM, Sound::TIMES::LOOP);

}

void TitleScene::Update(void)
{


	//ロードが完了したか判断
	if (GetASyncLoadNum() != 0 || SceneManager::GetInstance().IsLoading())
	{
		return;
	}


	InputManager::JOYPAD_NO jno = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD1);
	InputManager::JOYPAD_NO jno2 = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD2);

	if (sceneChangeTime_ <= 0.0f && isSceneChange_)
	{
		BattleManager::GetInstance().ResetGame();
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);
		return;
	}
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if ((ins.IsTrgDown(KEY_INPUT_SPACE)
		|| ins.IsPadBtnTrgDown(jno, InputManager::JOYPAD_BTN::START)
		|| ins.IsPadBtnTrgDown(jno2, InputManager::JOYPAD_BTN::START)) && !isSceneChange_)
	{
		sceneChangeTime_ = FLASH_TIME;
		isSceneChange_ = true;
	}

	
	if (sceneChangeTime_ > 0.0f)
	{
		sceneChangeTime_ -= SceneManager::GetInstance().GetDeltaTime();
	}
	if (!isSceneChange_)
	{
		//ボタンの拡大縮小処理// 例：TitleScene::Update()
		sizeRate_ = (std::sin(sceneTotalTime_) * 0.5f + 0.5f) * (1.2f - 0.8f) + 0.8f;  // 0.8～1.2 を往復
		sizeRate_ *= 0.25f;
		sceneTotalTime_ += SceneManager::GetInstance().GetDeltaTime() * 3.0f;
	}
}

void TitleScene::Draw(void)
{
	//ロードが完了したか判断
	if (GetASyncLoadNum() != 0 || SceneManager::GetInstance().IsLoading())
	{
		return;
	}

	stage_->Draw();


	// ロゴ描画
	DrawLogo();

}

void TitleScene::Release(void)
{
	SoundManager::GetInstance().AllStop();

	stage_->Release();
	delete stage_;
}

void TitleScene::DrawLogo(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// タイトルロゴ
	DrawRotaGraph(
		cx, cy,
		1.0f, 0.0f, imgTitleLogo_, true);


	const float BLINK_INTERVAL = 0.1f; // 点滅の間隔（秒）

	//1.0f-のところが点滅する時間
	bool isVisible = static_cast<int>((FLASH_TIME - sceneChangeTime_) / BLINK_INTERVAL) % 2 == 0;

	if (isVisible)
	{
		// Pushメッセージ
		DrawRotaGraph(cx, 550, sizeRate_, 0.0f, pushImg_, true);
	}

	
}
