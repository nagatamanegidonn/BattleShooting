#include <DxLib.h>

#include "../Application.h"

#include "../Manager/SoundManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"

#include "../Utility/AsoUtility.h"

#include "../Object/Player/ViewPlayer.h"

#include "SelectScene.h"

SelectScene::SelectScene(void)
{
}

SelectScene::~SelectScene(void)
{
}

void SelectScene::AsyncPreLoad(void)
{
	//非同期読み込みを有効にする
	SetUseASyncLoadFlag(true);

	// 初期化: i = 1、条件式: i <= 5、更新: i++
	for (int i = 0; i < PLAYER_MAX; i++) {

		auto  player = std::make_shared<ViewPlayer>();
		players_.push_back(player);
	}
}
void SelectScene::Init(void)
{
	//非同期読み込みを有効にする
	SetUseASyncLoadFlag(false);

	// カメラモード：定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);


	playerImg_[0] = ResourceManager::GetInstance().Load(ResourceManager::SRC::P1_IMAGE).handleId_;
	playerImg_[1] = ResourceManager::GetInstance().Load(ResourceManager::SRC::P2_IMAGE).handleId_;

	Cursor_ = LoadGraph("Data/Image/Cursor.png");

	pos[0] = AsoUtility::VECTOR_ZERO;
	pos[1] = AsoUtility::VECTOR_ZERO;

	pos[0].x = Application::SCREEN_SIZE_X / 3;
	pos[0].y = Application::SCREEN_SIZE_Y / 3;

	pos[1].x = (Application::SCREEN_SIZE_X / 3) * 2;
	pos[1].y = Application::SCREEN_SIZE_Y / 3;

	//ゲーム開始準備確認用フラグ
	for (int ii = 0; ii < PLAYER_MAX; ii++)
	{
		isReady_[ii] = false;
	}
	isStart_ = false;
	

	float size = 500.0f;

	//プレイヤーの設定
	VECTOR sPos[4] = {
		{-size,0.0f,size / 2}//左上
		,{size,0.0f,size / 2}//右上
		,{-size,0.0f,-size}//左下
		,{size,0.0f,-size}//右上 
	};

	// 初期化: i = 1、条件式: i <= 5、更新: i++
	for (int i = 0; i < PLAYER_MAX; i++) {
		players_[i]->Init(sPos[i], i, i);
		players_[i]->ChangeState(ViewPlayer::STATE::PLAY);
	}

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

	(ins.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::RIGHT));
	
	// シーン遷移
	for (int ii = 0; ii < PLAYER_MAX; ii++)
	{
		// プレイヤー１とプレイヤー２が準備完了ボタンを押してスペースを押すとゲームシーンに移行
		if (isStart_ && (ins.IsTrgDown(KEY_INPUT_SPACE)
			|| (ins.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::START))
			|| (ins.IsPadBtnNew(jno2, InputManager::JOYPAD_BTN::START))))
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
			return;
		}
	}

	// BACKSPACE
	if (ins.IsTrgDown(KEY_INPUT_BACK))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	//カーソル移動
	GetMove(pos[0], pos[1]);

	// キャラ選択時の当たり判定
	Collision();

	//プレイヤーの更新
	for (auto p : players_)
	{
		p->Update();
	}
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

	DrawFormatString(0, 16, 0xffffff, "1,2で決定");
	DrawFormatString(0, 16, 0xffffff, "Spaceキーでスタート");


	//背景
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x00ff00, true);

	//キャラ選択

	DrawBox(0, 0, Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0xfff000, true);
	DrawBox(Application::SCREEN_SIZE_X / 2, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y / 2, 0x000fff, true);

	SceneManager& sns = SceneManager::GetInstance();

	//プレイヤー１が準備完了したかどうか
	SetFontSize(25);
	if (isReady_[0] && sns.GetPlayerId(0) > -1) {
		DrawString(0, Application::SCREEN_SIZE_Y / 2, "p1_OK", RGB(0, 0, 255), true);

		DrawRotaGraph(Application::SCREEN_SIZE_X / 4, Application::SCREEN_SIZE_Y / 4 * 3
			, 0.3f, 0.0f, playerImg_[sns.GetPlayerId(0)], true);
	}

	//プレイヤー2が準備完了したかどうか
	if (isReady_[1] && sns.GetPlayerId(1) > -1) {
		DrawString(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, "p2_OK", RGB(0, 0, 255), true);

		DrawRotaGraph(Application::SCREEN_SIZE_X / 4 * 3, Application::SCREEN_SIZE_Y / 4 * 3
			, 0.3f, 0.0f, playerImg_[sns.GetPlayerId(1)], true, true);

	}

	//プレイヤーの更新
	for (auto p : players_)
	{
		p->Draw();
	}
	
	//プレイヤー１のカーソル（仮）
	//DrawBox(pos[0].x - SIZE, pos[0].y - SIZE, pos[0].x + SIZE, pos[0].y + SIZE, 0x000000, true);
	DrawRotaGraph(pos[0].x, pos[0].y, 0.03f, 0.0f, Cursor_, true);

	//プレイヤー２のカーソル（仮）
	//DrawBox(pos[1].x - SIZE, pos[1].y - SIZE, pos[1].x + SIZE, pos[1].y + SIZE, 0x000000, true);
	DrawRotaGraph(pos[1].x, pos[1].y, 0.03f, 0.0f, Cursor_, true);
	//-----------------------------------------------------

	
}

void SelectScene::Release(void)
{
	SoundManager::GetInstance().AllStop();

}

//キャラ選択画面のカーソル移動用(仮)
void SelectScene::GetMove(VECTOR& P1, VECTOR& P2)
{
	//------------------------------------------
	InputManager& ins = InputManager::GetInstance();


	InputManager::JOYPAD_NO jno = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD1);
	InputManager::JOYPAD_NO jno2 = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD2);

	// 左スティックの横軸
	auto leftStickX = ins.GetJPadInputState(jno).AKeyLX;
	// 左スティックの縦軸
	auto leftStickY = ins.GetJPadInputState(jno).AKeyLY;


	if (isReady_[0] == false)
	{
		if (CheckHitKey(KEY_INPUT_W) || (leftStickY < 0))
		{
			P1.y -= MOVE;
		}
		if (CheckHitKey(KEY_INPUT_S) || (leftStickY > 0))
		{
			P1.y += MOVE;
		}
		if (CheckHitKey(KEY_INPUT_D) || (leftStickX > 0))
		{
			P1.x += MOVE;
		}
		if (CheckHitKey(KEY_INPUT_A) || (leftStickX < 0))
		{
			P1.x -= MOVE;
		}
	}


	//------------------------------------------

	// 左スティックの横軸
	auto leftStick2X = ins.GetJPadInputState(jno2).AKeyLX;
	// 左スティックの縦軸
	auto leftStick2Y = ins.GetJPadInputState(jno2).AKeyLY;

	if (isReady_[1] == false)
	{
		if (CheckHitKey(KEY_INPUT_UP) || (leftStick2Y < 0))
		{
			P2.y -= MOVE;
		}
		if (CheckHitKey(KEY_INPUT_DOWN) || (leftStick2Y > 0))
		{
			P2.y += MOVE;
		}
		if (CheckHitKey(KEY_INPUT_RIGHT) || (leftStick2X > 0))
		{
			P2.x += MOVE;
		}
		if (CheckHitKey(KEY_INPUT_LEFT) || (leftStick2X < 0))
		{
			P2.x -= MOVE;
		}
	}


	//------------------------------------------
}

// キャラ選択時の当たり判定
void SelectScene::Collision(void)
{
	InputManager& ins = InputManager::GetInstance();

	//キャラクターを決定
	CharacthrSelect(0);
	CharacthrSelect(1);

	//全プレイヤーが準備完了なら
	if (isReady_[0] && isReady_[1])
	{
		isStart_ = true;
	}
	else {
		isStart_ = false;
	}

	//-------------------------------------------------------------------
	// カーソルと画面端の当たり判定
	for (int ii = 0; ii < PLAYER_MAX; ii++)
	{
		// 左
		if (pos[ii].x - SIZE < 0)
		{
			pos[ii].x += MOVE;
		}

		// 右
		if (pos[ii].x + SIZE > Application::SCREEN_SIZE_X)
		{
			pos[ii].x -= MOVE;
		}

		// 上
		if (pos[ii].y - SIZE < 0)
		{
			pos[ii].y += MOVE;
		}

		// 下
		if (pos[ii].y + SIZE > Application::SCREEN_SIZE_Y)
		{
			pos[ii].y -= MOVE;
		}
	}
}

void SelectScene::CharacthrSelect(int playerId)
{
	InputManager& ins = InputManager::GetInstance();

	InputManager::JOYPAD_NO jno = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD1);
	bool isTrg = ins.IsTrgDown(KEY_INPUT_1);
	if (playerId == 1)//プレイヤー2なら
	{
		jno = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD2);
		isTrg = ins.IsTrgDown(KEY_INPUT_2);
	}

	for (int ii = 0; ii < CHARACTER_MAX; ii++)
	{
		if (pos[playerId].x < Application::SCREEN_SIZE_X / 2 + (ii * Application::SCREEN_SIZE_X / 2) &&
			pos[playerId].x > ii * (Application::SCREEN_SIZE_X / 2) &&
			pos[playerId].y < Application::SCREEN_SIZE_Y / 2 &&
			pos[playerId].y > 0)
		{
			// プレイヤー２が準備完了しているかどうか
			// true == 準備完了 / false == 準備中
			if (isTrg || ins.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::RIGHT))
			{
				if (isReady_[playerId] == false)
				{
					isReady_[playerId] = true;

					//プレイヤー選択をSceneManagerの設定
					SceneManager::GetInstance().SetPlayerId(playerId, ii);
				}
				else
				{
					SceneManager::GetInstance().SetPlayerId(playerId, -1);
					isReady_[playerId] = false;
				}
			}
		}
	}
}
