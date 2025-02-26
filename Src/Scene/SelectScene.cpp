#include <DxLib.h>

#include "../Application.h"

#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"

#include "../Utility/AsoUtility.h"

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
}
void SelectScene::Init(void)
{
	//非同期読み込みを有効にする
	SetUseASyncLoadFlag(false);

	// カメラモード：定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);


	for (int ii = 0; ii < PLAYER_MAX; ii++)
	{
		kPos[ii] = AsoUtility::VECTOR_ZERO;

		// カーソルの初期座標
		kPos[ii].x = Application::SCREEN_SIZE_X / 4 + ((Application::SCREEN_SIZE_X / 4 * 2) * ii);
		kPos[ii].y = Application::SCREEN_SIZE_Y / 2;

		pPos[ii].x = Application::SCREEN_SIZE_X / 4 + ((Application::SCREEN_SIZE_X / 4 * 2) * ii);
		pPos[ii].y = Application::SCREEN_SIZE_Y / 4 * 3;

		start[ii] = false;
		chara[ii] = CHARA::E_CHARA_NON;
	}
}

void SelectScene::Update(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	for (int ii = 0; ii < PLAYER_MAX; ii++)
	{
		// プレイヤー１とプレイヤー２が準備完了ボタンを押してスペースを押すとゲームシーンに移行
		if (start[ii] && ins.IsTrgDown(KEY_INPUT_SPACE)) {
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		}
	}

	// BACKSPACE
	if (ins.IsTrgDown(KEY_INPUT_BACK))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	// カーソル移動
	GetMove(kPos[0], kPos[1]);

	// キャラ選択時の当たり判定
	Collision();

}

void SelectScene::Draw(void)
{
	//-----------------------------------------------------
	//デバック用

	//背景
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x00ff00, true);

	//キャラ
	DrawBox(0, 0, Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 0xfff000, true);
	DrawBox(Application::SCREEN_SIZE_X / 2, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y / 2, 0x000fff, true);

	for (int ii = 0; ii < PLAYER_MAX; ii++)
	{
		DrawBox(pPos[ii].x - 150, pPos[ii].y - 150, pPos[ii].x + 150, pPos[ii].y + 150, 0x0ffff0, true);
		if (start[ii])
		{
			DrawBox(pPos[ii].x - 140, pPos[ii].y - 140, pPos[ii].x + 140, pPos[ii].y + 140, 0x0f0f0f, true);
		}
	}

	//プレイヤー１のカーソル（仮）
	if (start[0] == false)
	{
		DrawBox(kPos[0].x - SIZE, kPos[0].y - SIZE, kPos[0].x + SIZE, kPos[0].y + SIZE, 0x000000, true);
	}
	else 
	{
		DrawBox(kPos[0].x - SIZE, kPos[0].y - SIZE, kPos[0].x + SIZE, kPos[0].y + SIZE, 0xffffff, true);
	}

	//プレイヤー２のカーソル（仮）
	if (start[1] == false)
	{
		DrawBox(kPos[1].x - SIZE, kPos[1].y - SIZE, kPos[1].x + SIZE, kPos[1].y + SIZE, 0x000000, true);
	}
	else
	{
		DrawBox(kPos[1].x - SIZE, kPos[1].y - SIZE, kPos[1].x + SIZE, kPos[1].y + SIZE, 0xffffff, true);
	}

	SetFontSize(25);

	// プレイヤー１のキャラの状態
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

	// プレイヤー２のキャラの状態
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

	//-----------------------------------------------------
}

void SelectScene::Release(void)
{

}

//キャラ選択画面のカーソル移動用(仮)
void SelectScene::GetMove(VECTOR& P1, VECTOR& P2)
{
	//------------------------------------------

	InputManager& ins = InputManager::GetInstance();
	if (start[0] == false)
	{
		if (CheckHitKey(KEY_INPUT_W))
		{
			P1.y -= MOVE;
		}
		if (CheckHitKey(KEY_INPUT_S))
		{
			P1.y += MOVE;
		}
		if (CheckHitKey(KEY_INPUT_D))
		{
			P1.x += MOVE;
		}
		if (CheckHitKey(KEY_INPUT_A))
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

	for (int ii = 0; ii < CHARACTER_MAX; ii++)
	{
		if (kPos[0].x < Application::SCREEN_SIZE_X / 2 + (ii * Application::SCREEN_SIZE_X / 2) &&
			kPos[0].x > ii * (Application::SCREEN_SIZE_X / 2) &&
			kPos[0].y < Application::SCREEN_SIZE_Y / 2 &&
			kPos[0].y > 0 &&
			ins.IsTrgDown(KEY_INPUT_1))
		{
			// シーン遷移
			// プレイヤー１が準備完了しているかどうか
			// true == 準備完了 / false == 準備中
			if (start[0] == false)
			{
				start[0] = true;

				// どちらのキャラを選択したかどうか
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
				// 準備完了していない状態
				start[0] = false;
				chara[0] = E_CHARA_NON;
			}
			
		}
	}


	for (int ii = 0; ii < CHARACTER_MAX; ii++)
	{
		if (kPos[1].x < Application::SCREEN_SIZE_X / 2 + (ii * Application::SCREEN_SIZE_X / 2) &&
			kPos[1].x > ii * (Application::SCREEN_SIZE_X / 2) &&
			kPos[1].y < Application::SCREEN_SIZE_Y / 2 &&
			kPos[1].y > 0 &&
			ins.IsTrgDown(KEY_INPUT_2))
		{
			// プレイヤー２が準備完了しているかどうか
			// true == 準備完了 / false == 準備中
			if (start[1] == false)
			{
				start[1] = true;

				// どちらのキャラを選択したかどうか
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
				// 準備完了していない状態
				start[1] = false;
				chara[1] = E_CHARA_NON;
			}
			
		}
	}

	//-------------------------------------------------------------------
	// カーソルと画面端の当たり判定
	for (int ii = 0; ii < PLAYER_MAX; ii++)
	{
		// 左
		if (kPos[ii].x - SIZE < 0)
		{
			kPos[ii].x += MOVE;
		}

		// 右
		if (kPos[ii].x + SIZE > Application::SCREEN_SIZE_X)
		{
			kPos[ii].x -= MOVE;
		}

		// 上
		if (kPos[ii].y - SIZE < 0)
		{
			kPos[ii].y += MOVE;
		}

		// 下
		if (kPos[ii].y + SIZE > Application::SCREEN_SIZE_Y)
		{
			kPos[ii].y -= MOVE;
		}
	}
}
