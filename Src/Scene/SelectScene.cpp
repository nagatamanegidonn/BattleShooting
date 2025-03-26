#include <DxLib.h>

#include "../Application.h"

#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"

#include "../Utility/AsoUtility.h"

#include "../Object/Player/Player.h"

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

		auto  player = std::make_shared<Player>();
		players_.push_back(player);
	}
}
void SelectScene::Init(void)
{
	//非同期読み込みを有効にする
	SetUseASyncLoadFlag(false);

	// カメラモード：定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);


	pos[0] = AsoUtility::VECTOR_ZERO;
	pos[1] = AsoUtility::VECTOR_ZERO;

	pos[0].x = Application::SCREEN_SIZE_X / 3;
	pos[0].y = Application::SCREEN_SIZE_Y / 2;

	pos[1].x = (Application::SCREEN_SIZE_X / 3) * 2;
	pos[1].y = Application::SCREEN_SIZE_Y / 2;

	for (int ii = 0; ii < PLAYER_MAX; ii++)
	{
		start[ii] = false;
	}

	for(int ii = 0; ii < PLAYER_MAX; ii++)chara[ii] = CHARA::E_CHARA_NON;

	float size = 100.0f;

	//プレイヤーの設定
	VECTOR sPos[4] = {
		{-size,0.0f,size}//左上
		,{size,0.0f,size}//右上
		,{-size,0.0f,-size}//左下
		,{size,0.0f,-size}//右上 
	};

	// 初期化: i = 1、条件式: i <= 5、更新: i++
	for (int i = 0; i < PLAYER_MAX; i++) {
		players_[i]->Init(sPos[i], i);
		players_[i]->ChangeState(Player::STATE::NONE);
	}
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
	if (ins.IsTrgDown(KEY_INPUT_SPACE)||(ins.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::RIGHT)))
	{
	//	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}
	
	for (int ii = 0; ii < PLAYER_MAX; ii++)
	{
		// プレイヤー１とプレイヤー２が準備完了ボタンを押してスペースを押すとゲームシーンに移行
		if (start[ii] && ins.IsTrgDown(KEY_INPUT_SPACE)
			|| (start[ii] && (ins.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::START)))
			|| (start[ii] && (ins.IsPadBtnNew(jno2, InputManager::JOYPAD_BTN::START))))
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

	//背景
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x00ff00, true);

	//キャラ選択

	DrawBox(0, 0, Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0xfff000, true);
	DrawBox(Application::SCREEN_SIZE_X / 2, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y / 2, 0x000fff, true);


	//プレイヤー１が準備完了したかどうか
	SetFontSize(25);
	if (start[0]) {
		DrawString(0, 0, "p1", RGB(0, 0, 255), true);
	}

	//プレイヤー１が準備完了したかどうか
	if (start[1]) {
		DrawString(0, 0, "p2", RGB(0, 0, 255), true);
	}

	//プレイヤーの更新
	for (auto p : players_)
	{
		p->Draw();
	}
	
	//プレイヤー１のカーソル（仮）
	DrawBox(pos[0].x - SIZE, pos[0].y - SIZE, pos[0].x + SIZE, pos[0].y + SIZE, 0x000000, true);

	//プレイヤー２のカーソル（仮）
	DrawBox(pos[1].x - SIZE, pos[1].y - SIZE, pos[1].x + SIZE, pos[1].y + SIZE, 0x000000, true);
	//-----------------------------------------------------

	switch (chara[0])
	{
	case E_CHARA_NON:
		break;
	case E_CHARA1:
		DrawString(0, Application::SCREEN_SIZE_Y / 2, "プイレヤー１/キャラクター１", RGB(0, 0, 255), true);
		break;
	case E_CHARA2:
		DrawString(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, "プイレヤー１/キャラクター2", RGB(0, 0, 255), true);
		break;
	}

	switch (chara[1])
	{
	case E_CHARA_NON:
		break;
	case E_CHARA1:
		DrawString(0, Application::SCREEN_SIZE_Y / 2 + 50 , "プイレヤー２/キャラクター１", RGB(0, 0, 255), true);
		break;
	case E_CHARA2:
		DrawString(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2 + 50, "プイレヤー２/キャラクター2", RGB(0, 0, 255), true);
		break;
	}
}

void SelectScene::Release(void)
{

}

//キャラ選択画面のカーソル移動用(仮)
void SelectScene::GetMove(VECTOR& P1, VECTOR& P2)
{
	//------------------------------------------
	InputManager& ins = InputManager::GetInstance();


	InputManager::JOYPAD_NO jno = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD1);

	// 左スティックの横軸
	auto leftStickX = ins.GetJPadInputState(jno).AKeyLX;
	// 左スティックの縦軸
	auto leftStickY = ins.GetJPadInputState(jno).AKeyLY;


	if (start[0] == false)
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

	if (start[1] == false)
	{
		if (CheckHitKey(KEY_INPUT_UP))
		{
			P2.y -= MOVE;
		}
		if (CheckHitKey(KEY_INPUT_DOWN))
		{
			P2.y += MOVE;
		}
		if (CheckHitKey(KEY_INPUT_RIGHT))
		{
			P2.x += MOVE;
		}
		if (CheckHitKey(KEY_INPUT_LEFT))
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

	InputManager::JOYPAD_NO jno = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD1);
	InputManager::JOYPAD_NO jno2 = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD2);

	for (int ii = 0; ii < CHARACTER_MAX; ii++)
	{
		if (pos[0].x < Application::SCREEN_SIZE_X / 2 + (ii * Application::SCREEN_SIZE_X / 2) &&
			pos[0].x > ii * (Application::SCREEN_SIZE_X / 2) &&
			pos[0].y < Application::SCREEN_SIZE_Y / 2 &&
			pos[0].y > 0)
		{
			// シーン遷移
			// プレイヤー１が準備完了しているかどうか
			// true == 準備完了 / false == 準備中
			if (ins.IsTrgDown(KEY_INPUT_1))
			{
				if (start[0] == false)
				{
					start[0] = true;


					if (ii == 0)
					{
						chara[0] = E_CHARA1;
					}
					else if (ii == 1)
					{
						chara[0] = E_CHARA2;
					}
				}
				else
				{
					start[0] = false;
					chara[0] = E_CHARA_NON;
				}
			}
		}
	}


	for (int ii = 0; ii < CHARACTER_MAX; ii++)
	{
		if (pos[1].x < Application::SCREEN_SIZE_X / 2 + (ii * Application::SCREEN_SIZE_X / 2) &&
			pos[1].x > ii * (Application::SCREEN_SIZE_X / 2) &&
			pos[1].y < Application::SCREEN_SIZE_Y / 2 &&
			pos[1].y > 0)
		{
			// プレイヤー２が準備完了しているかどうか
			// true == 準備完了 / false == 準備中
			if (ins.IsTrgDown(KEY_INPUT_2))
			{
				if (start[1] == false)
				{
					start[1] = true;

					if (ii == 0)
					{
						chara[1] = E_CHARA1;
					}
					else if (ii == 1)
					{
						chara[1] = E_CHARA2;
					}
				}
				else
				{
					start[1] = false;
					chara[1] = E_CHARA_NON;
				}
			}
		}
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