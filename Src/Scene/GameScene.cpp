#include <DxLib.h>

#include <EffekseerForDXLib.h>

#include "../Application.h"

#include "../Utility/AsoUtility.h"//Quaternion等を扱う関数が入っている

#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"

#include "../Object/BackGround.h"
#include "../Object/Grid.h"
#include "../Object/Shot/ShotPlayer.h"
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

	backGround_ = new BackGround;
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

	//背景初期化
	backGround_->Init();

	Camera* camera = SceneManager::GetInstance().GetCamera();
	// カメラモード：定点カメラ
	camera->ChangeMode(Camera::MODE::FOLLOW_POINT);

	int c = 0;
	for (auto p : players_)
	{
		//本体カメラの設定
		if (c == 0)
		{
			camera->SetFollow(&p->GetTransform());
		}
		else//2つ目以降なら
		{
			camera->SetSubFollow(&p->GetTransform());
		}

		// カメラ
		camera_[c]->Init();

		camera_[c]->ChangeMode(Camera::MODE::FOLLOW);
		camera_[c]->SetFollow(&p->GetTransform());
		c++;
	}
	
	// 自機破壊エフェクト
	effectHitResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::SHOT_EXPLOSION).handleId_;


	hitStop_ = 0.0f;
}

void GameScene::Update(void)
{
	//ロードが完了したか判断
	if (GetASyncLoadNum() != 0 || SceneManager::GetInstance().IsLoading())
	{
		return;
	}

	if (hitStop_ > 0)
	{
		hitStop_ -= SceneManager::GetInstance().GetDeltaTime();
	}
	else
	{
		//エフェクト再生
		effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
		hitStop_ = 2;
	}
	//位置
	SetPosPlayingEffekseer3DEffect(effectHitPlayId_,0, 0, 0);
	// 大きさ
	float SCALE = 0.1f;
	SetScalePlayingEffekseer3DEffect(effectHitPlayId_, SCALE, SCALE, SCALE);
	//回転
	SetRotationPlayingEffekseer3DEffect(effectHitPlayId_, 0, 0, 0);



	// シーン遷移
	/*InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
		return;
	}*/

	/*stage_->Update();*/

	

	//プレイヤーの更新
	for (auto p : players_)
	{
		p->Update();
	}

	// 衝突判定
	Collision();

	//プレイヤーのカメラ更新
	for (auto& camera : camera_)
	{
		camera->Update();
	}

	// カメラ衝突チェック
	Camera* camera = SceneManager::GetInstance().GetCamera();
	// カメラモード：
	//camera->ChangeMode(Camera::MODE::FIXED_POINT);


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
	int screenSize = 100;
	int mx = Application::SCREEN_SIZE_X - screenSize;
	int my = Application::SCREEN_SIZE_Y - screenSize;

	int cx = Application::SCREEN_SIZE_X - screenSize;
	int cy = Application::SCREEN_SIZE_Y - screenSize;


	//背景描画
	//backGround_->Draw();

	//プレイヤーの描画
	for (auto& p : players_)
	{
		p->Draw();

		VECTOR pos2D = ConvWorldPosToScreenPos(p->GetTransform().pos);
		DrawCircle(pos2D.x, pos2D.y, 10, 0x0000ff);

		if ((mx < pos2D.x) || (my < pos2D.y) || (screenSize > pos2D.x) || (screenSize > pos2D.y))
		{
			SceneManager::GetInstance().GetCamera()->FadeOut();
		}
	}

	//デバッグ描画
	DrawDebug();

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

	//背景解放処理
	backGround_->Release();
	delete backGround_;

	//エフェクトの開放
	StopEffekseer3DEffect(effectHitPlayId_);

}


void GameScene::Collision(void)
{
	//プレイヤーの更新
	for (auto& plyer : players_)
	{
		auto shots = plyer->GetShots();
		for (auto& s : shots)
		{
			s->Update();
		}


		for (auto& vsPlyer : players_)
		{
			//戦っているプレイヤーが自身だったらコンテニュー
			if (plyer == vsPlyer) { continue; }


			VECTOR pos2D = ConvWorldPosToScreenPos(plyer->GetTransform().pos);
			VECTOR posVS2D = ConvWorldPosToScreenPos(vsPlyer->GetTransform().pos);
			int screenSize = 100;
			int cx = Application::SCREEN_SIZE_X / 2;
			int cy = Application::SCREEN_SIZE_Y / 2;

			//カメラの判定
			if (((cx + screenSize > pos2D.x) && (cy + screenSize > pos2D.y)
				&& (cx - screenSize < pos2D.x) && (cy - screenSize < pos2D.y))
				&& ((cx + screenSize > posVS2D.x) && (cy + screenSize > posVS2D.y)
					&& (cx - screenSize < posVS2D.x) && (cy - screenSize < posVS2D.y)))
			{
				SceneManager::GetInstance().GetCamera()->FadeIn();
			}



			// playerとの衝突判定
			VECTOR diff = VSub(plyer->GetPos(0), vsPlyer->GetPos(1));

			float disPow = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
			if (disPow < Player::DAMAGE_RADIUS * Player::ATTRCK_RADIUS)//ダメージ半径×攻撃半径
			{
				//plyerに相手のマシンの攻撃が当たっている
				plyer->Damage(1);
				//ふっ飛ばし処理
				VECTOR dir = VNorm(VSub(plyer->GetTransform().pos, vsPlyer->GetTransform().pos));
				plyer->SetJump(dir);
				dir = VScale(dir, -1);//吹っ飛ばしの方向を反転
				vsPlyer->SetJump(dir);

				//エフェクト再生
				effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
				//位置
				SetPosPlayingEffekseer3DEffect(effectHitPlayId_, plyer->GetTransform().pos.x, plyer->GetTransform().pos.y, plyer->GetTransform().pos.z);
			}

			//攻撃箇所同士が衝突
			diff = VSub(plyer->GetPos(1), vsPlyer->GetPos(1));

			disPow = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
			if (disPow < Player::ATTRCK_RADIUS * Player::ATTRCK_RADIUS)//攻撃半径×攻撃半径
			{
				plyer->Damage(1);
				vsPlyer->Damage(1);

				//ふっ飛ばし処理
				VECTOR dir = VNorm(VSub(plyer->GetTransform().pos, vsPlyer->GetTransform().pos));
				plyer->SetJump(dir);
				dir = VScale(dir, -1);
				vsPlyer->SetJump(dir);

				//エフェクト再生
				effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
				//位置
				SetPosPlayingEffekseer3DEffect(effectHitPlayId_, plyer->GetTransform().pos.x, plyer->GetTransform().pos.y, plyer->GetTransform().pos.z);
			}

			// 大きさ
			float SCALE = 0.1f;
			SetScalePlayingEffekseer3DEffect(effectHitPlayId_, SCALE, SCALE, SCALE);
			//回転
			SetRotationPlayingEffekseer3DEffect(effectHitPlayId_, plyer->GetTransform().rot.x, plyer->GetTransform().rot.y, plyer->GetTransform().rot.z);


			auto shots = plyer->GetShots();
			for (auto& shot : shots)
			{
				if (shot->IsShot() && vsPlyer->GetState() == Player::STATE::PLAY)
				{
					
					//弾の当たり判定
#pragma region 没案

					//auto info = MV1CollCheck_Sphere(vsPlyer->GetModelId(), -1,
					//	shot->GetPos(), shot->GetCollisionRadius());
					//if (info.HitNum >= 1)
					//{
					//	//plyerに攻撃が当たっている
					//	vsPlyer->RideDamage(1);

					//	//ふっ飛ばし処理
					//	VECTOR dir = VScale(VNorm(VSub(shot->GetPos(), vsPlyer->GetTransform().pos)),-1);
					//	vsPlyer->SetJump(dir);

					//	shot->Blast();
					//}
					//// 当たり判定結果ポリゴン配列の後始末をする
					//MV1CollResultPolyDimTerminate(info);
#pragma endregion
					diff = VSub(shot->GetPos(), vsPlyer->GetPos(2));
					disPow = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
					if (disPow < shot->GetCollisionRadius() * Player::DAMAGE_RADIUS)//攻撃半径×攻撃半径
					{
						//plyerに攻撃が当たっている
						vsPlyer->Damage(1);
						//hitStop_ = 0.5f;

						//ふっ飛ばし処理
						VECTOR dir = VScale(VScale(VNorm(VSub(shot->GetPos(), vsPlyer->GetTransform().pos)), -1), 0.5f);
						vsPlyer->SetJump(dir);

						shot->Blast();
					}
				}
			}
		}
	}

	int winid = 0;
	SceneManager& Sns = SceneManager::GetInstance();
	for (auto& plyer : players_)
	{
		//勝敗判定
		if (plyer->GetState() == Player::STATE::DEAD)
		{
			switch (winid)
			{
			case 0:
				Sns.SetWinner(SceneManager::WINNER::PLAYER_TWO);
				break;
			case 1:
				Sns.SetWinner(SceneManager::WINNER::PLAYER_ONE);
				break;
			}
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
			return;
		}
		winid++;
	}

}

//デバッグ描画
void GameScene::DrawDebug(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;


	int plyNum= -1;
	//プレイヤーの更新
	for (auto& plyer : players_)
	{
		int cx2 = cx + (20 * plyNum);
		DrawBox(cx2, 20, cx2 + (20 * plyNum * plyer->GetMaxHp()), 50, 0x000000, true);
		DrawBox(cx2, 20, cx2 + (20 * plyNum * plyer->GetHp()), 50, 0x00ff00, true);

		VECTOR pos = plyer->GetTransform().pos;
		if (plyNum == 1)
		{
			DrawFormatString(0, 16, 0xff0000, "%2.f,%2.f,%2.f", pos.x, pos.y, pos.z);
		}
		else
		{
			DrawFormatString(0, 32, 0xff0000, "%2.f,%2.f,%2.f", pos.x, pos.y, pos.z);
		}
		plyNum *= -1;
	}



	return;

	//メッセージ
	std::string msg = "Game Scene";
	SetFontSize(28);//文字のサイズを設定

	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), 550, 0xffffff, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(16);

}
