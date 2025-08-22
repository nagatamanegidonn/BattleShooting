#include <DxLib.h>

#include "../Manager/SoundManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"

#include "../Utility/AsoUtility.h"

#include "../Object/Player/SelectPlayer.h"
#include "../Object/Player/ViewPlayer.h"

#include "SelectScene.h"
#include "../Manager/FadeManager.h"

namespace {

	
}

SelectScene::SelectScene(void)
{
	CursorImg_ = -1;
	FrameImg_ = -1;
	startImg_ = -1;

}

SelectScene::~SelectScene(void)
{
	playerViews_.clear();
	players_->reset();
}

void SelectScene::AsyncPreLoad(void)
{
	//非同期読み込みを有効にする
	SetUseASyncLoadFlag(true);


	int CharId[BattleManager::CHAR_SIZE] = {
		0,
		2
	};
	// 初期化: i = 1、条件式: i <= 5、更新: i++
	for (int i = 0; i < BattleManager::CHAR_SIZE; i++) {

		auto  player = std::make_unique<ViewPlayer>();
		playerViews_.push_back(std::move(player));

		playerViews_[i]->Init(VIEW_STATRT_POS[i], i, CharId[i]);
		playerViews_[i]->ChangeState(ViewPlayer::STATE::PLAY);

	}

	CursorImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CURSOR).handleId_;
	FrameImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::FRAME).handleId_;

	startImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::START_IMAGE).handleId_;

}
void SelectScene::Init(void)
{
	//非同期読み込みを有効にする
	SetUseASyncLoadFlag(false);

	// カメラモード：定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);


	//ゲーム開始準備確認用フラグ
	for (int i = 0; i < BattleManager::PLAYER_SIZE; i++)
	{
		players_[i] = std::make_unique<SelectPlayer>();
		players_[i]->Init(cursorStartPos[i], i, -1);
	}
	isStart_ = false;
	

	
	SoundManager::GetInstance().Play(SoundManager::SRC::SELECT_BGM, Sound::TIMES::LOOP);

}

void SelectScene::Update(void)
{
	//ロードが完了したか判断
	if (GetASyncLoadNum() != 0 || SceneManager::GetInstance().IsLoading())
	{
		return;
	}

	InputManager& ins = InputManager::GetInstance();

	
	InputManager::JOYPAD_NO jno = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD1);
	InputManager::JOYPAD_NO jno2 = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD2);

	// 左スティックの横軸
	auto leftStickX = ins.GetJPadInputState(jno).AKeyLX;
	// 左スティックの縦軸
	auto leftStickY = ins.GetJPadInputState(jno).AKeyLY;

	// シーン遷移
	// プレイヤー１とプレイヤー２が準備完了ボタンを押してスペースを押すとゲームシーンに移行
	if (isStart_ && (ins.IsTrgDown(KEY_INPUT_SPACE)
		|| (ins.IsPadBtnTrgDown(jno, InputManager::JOYPAD_BTN::START))
		|| (ins.IsPadBtnTrgDown(jno2, InputManager::JOYPAD_BTN::START))))

	{
		FadeManager::GetInstance().ChangeState(FadeManager::SCENE::GAME);

		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		return;
	}

	// BACKSPACE
	if (ins.IsTrgDown(KEY_INPUT_BACK))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	//プレイヤーの更新
	for (auto& p : playerViews_)
	{
		p->Update();
	}

	bool isStart = true;
	for (auto& p : players_)
	{
		p->Update();
		if (!p->IsReady())
		{
			isStart = false;
		}
	}
	isStart_ = isStart;

}

void SelectScene::Draw(void)
{
	//ロードが完了したか判断
	if (GetASyncLoadNum() != 0 || SceneManager::GetInstance().IsLoading())
	{
		return;
	}
	//-----------------------------------------------------
	//デバック用


	const int cx = Application::SCREEN_SIZE_X / 2;
	const int cy = Application::SCREEN_SIZE_Y / 2;

	//背景
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x5562bf, true);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y / 2 - 28, 0x191970, true);

	//キャラ選択
	int distanceX = 270;
	DrawRotaGraph(distanceX, 150, 1.0f, 0.0f, FrameImg_, true);
	DrawRotaGraph(Application::SCREEN_SIZE_X - distanceX, 150, 1.0f, 0.0f, FrameImg_, true);


	BattleManager& BattleSns = BattleManager::GetInstance();


	//プレイヤーの更新
	for (auto& p : playerViews_)
	{
		p->Draw();
	}

	int strx = Application::SCREEN_SIZE_X / 2;


	



	AsoUtility::DrawCenterString("キャラ決定", SceneManager::GetInstance().GetFont(), cx, 400);
	AsoUtility::DrawCenterString("カラー変更", SceneManager::GetInstance().GetFont(), cx, 475);

	for (auto& p : players_)
	{
		p->Draw();
	}

	if (isStart_)
	{
		DrawRotaGraph(cx, cy, 1.0f, 0.0f, startImg_, true);
		AsoUtility::DrawCenterString("PUSH  START", SceneManager::GetInstance().GetFont(), cx, Application::SCREEN_SIZE_Y - 50);
	}

#ifdef _DEBUG
	DrawLine(cx, 0, cx, Application::SCREEN_SIZE_Y, 0xf0000);
#endif // DEBUG


}

void SelectScene::Release(void)
{
	SoundManager::GetInstance().AllStop();
}
