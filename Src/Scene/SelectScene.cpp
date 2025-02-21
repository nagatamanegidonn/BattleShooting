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

	//カーソル移動
	GetMove(pos[0], pos[1]);

	// キャラ選択時の当たり判定
	Collision();

}

void SelectScene::Draw(void)
{
	//-----------------------------------------------------
	//デバック用

	//背景
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x00ff00, true);

	//キャラ選択
	for (int ii = 1; ii <= 4; ii++) {
		DrawBox(
			180 * (1 * ii),
			Application::SCREEN_SIZE_Y / 2 - 50,
			(180 * (1 * ii)) + 100,
			(Application::SCREEN_SIZE_Y / 2) + 50, 0x000fff, true);
	}

	//プレイヤー１が準備完了したかどうか
	SetFontSize(25);
	if (start[0]) {
		DrawString(0, 0, "p1", RGB(0, 0, 255), true);
	}

	//プレイヤー１が準備完了したかどうか
	if (start[1]) {
		DrawString(0, 0, "p2", RGB(0, 0, 255), true);
	}

	//プレイヤー１のカーソル（仮）
	DrawBox(pos[0].x - SIZE, pos[0].y - SIZE, pos[0].x + SIZE, pos[0].y + SIZE, 0x000000, true);

	//プレイヤー２のカーソル（仮）
	DrawBox(pos[1].x - SIZE, pos[1].y - SIZE, pos[1].x + SIZE, pos[1].y + SIZE, 0x000000, true);
	//-----------------------------------------------------

}

void SelectScene::Release(void)
{

}

//キャラ選択画面のカーソル移動用(仮)
void SelectScene::GetMove(VECTOR& P1, VECTOR& P2)
{
	//------------------------------------------

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

	for (int ii = 1; ii <= 4; ii++)
	{
		if (pos[0].x < (180 * (1 * ii)) + 100 &&
			pos[0].x > 180 * (1 * ii) &&
			pos[0].y < (Application::SCREEN_SIZE_Y / 2) + 50 &&
			pos[0].y > Application::SCREEN_SIZE_Y / 2 - 50)
		{
			// シーン遷移
			// プレイヤー１が準備完了しているかどうか
			// true == 準備完了 / false == 準備中
			if (ins.IsTrgDown(KEY_INPUT_1))
			{
				if (start[0] == false)
				{
					start[0] = true;
				}
				else
				{
					start[0] = false;
				}
			}
		}
	}


	for (int ii = 1; ii <= 4; ii++)
	{
		if (pos[1].x < (180 * (1 * ii)) + 100 &&
			pos[1].x > 180 * (1 * ii) &&
			pos[1].y < (Application::SCREEN_SIZE_Y / 2) + 50 &&
			pos[1].y > Application::SCREEN_SIZE_Y / 2 - 50)
		{
			// プレイヤー２が準備完了しているかどうか
			// true == 準備完了 / false == 準備中
			if (ins.IsTrgDown(KEY_INPUT_2))
			{
				if (start[1] == false)
				{
					start[1] = true;
				}
				else
				{
					start[1] = false;
				}
			}
		}
	}
}