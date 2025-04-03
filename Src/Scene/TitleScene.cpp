#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/Camera.h"

#include "../Object/Stage.h"
#include "../Object/Player/Player.h"

#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	imgTitleLogo_ = -1;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::AsyncPreLoad(void)
{
	//非同期読み込みを有効にする
	SetUseASyncLoadFlag(true);

}
void TitleScene::Init(void)
{
	//非同期読み込みを有効にする
	SetUseASyncLoadFlag(false);

	// カメラモード：定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// タイトルロゴ
	imgTitleLogo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::START_LOGO).handleId_;

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


	/*cntSound_++;
	if (cntSound_ % 30 == 0 || cntSound_ % 45 == 0)
	{
		SoundManager::GetInstance().Play(SoundManager::SRC::DAMAGE, Sound::TIMES::ONCE, true);
	}*/
	

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);
		return;
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
	//SoundManager::GetInstance().Stop(SoundManager::SRC::TITLE_BGM);
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
		cx, cy - 200,
		1.0f, 0.0f, imgTitleLogo_, true);

	// Pushメッセージ
	std::string msg = "Push Space";
	SetFontSize(28);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), 200, 0x87cefa, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);

}
