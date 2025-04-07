#include <DxLib.h>

#include <EffekseerForDXLib.h>

#include "../Application.h"

#include "../Utility/AsoUtility.h"//Quaternion等を扱う関数が入っている

#include "../Manager/SoundManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"

#include "../Object/Stage.h"
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
	for (int i = 0; i < SceneManager::PLAYER_SIZE; i++) {
		camera_[i] = new Camera();

		auto  player = std::make_shared<Player>();
		players_.push_back(player);
	}

	eventFlag_ = false;
	eventId_ = -1;//０か1か


	stage_ = new Stage;
}
void GameScene::Init(void)
{
	//非同期処理を無効にする
	SetUseASyncLoadFlag(false);

	float size = 100.0f;

	VECTOR sPos[4] = {
		{-size,50.0f,size}//左上
		,{size,50.0f,size}//右上
		,{-size,0.0f,-size}//左下
		,{size,0.0f,-size}//右上 
	};


	// 初期化: i = 1、条件式: i <= 5、更新: i++
	for (int i = 0; i < SceneManager::PLAYER_SIZE; i++) {
		players_[i]->Init(sPos[i], i, SceneManager::GetInstance().GetPlayerId(i));

		//各プレイヤーのスクリーンの作成
		screenH[i] = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	}

	//背景初期化
	stage_->Init();
	//backGround_->Init();

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

		camera_[c]->ChangeMode(Camera::MODE::NONE);
		camera_[c]->SetFollow(&p->GetTransform());
		c++;
	}
	
	// 自機破壊エフェクト
	effectHitResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::SHOT_EXPLOSION).handleId_;


	hitStop_ = 0.0f;

	SoundManager::GetInstance().Play(SoundManager::SRC::BTTLE_BGM_3, Sound::TIMES::LOOP);

}

void GameScene::Update(void)
{
	//ロードが完了したか判断
	if (GetASyncLoadNum() != 0 || SceneManager::GetInstance().IsLoading())
	{
		return;
	}

	//エフェクトを再生してるか
	int ret = IsEffekseer3DEffectPlaying(effectHitPlayId_);
	if (ret == -1)
	{
		//再生できてない
		int t = 0;
	}

#ifdef _DEBUG
	// シーン遷移
	/*InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
		return;
	}*/
#endif

	SceneManager& Sns = SceneManager::GetInstance();

	//勝敗判定
	if (players_[0]->GetHp() <= 0 && players_[1]->GetHp() <= 0)
	{
		Sns.SetWinner(SceneManager::WINNER::DRAW);

		if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_SPACE)
			&& (players_[0]->GetState() == Player::STATE::END && players_[1]->GetState() == Player::STATE::END))
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
			return;
		}
	}
	else if (players_[0]->GetHp() <= 0)
	{
		Sns.SetWinner(SceneManager::WINNER::PLAYER_TWO);
		eventFlag_ = true;
		eventId_ = 0;

		const VECTOR& dir = players_[eventId_]->GetJumpDir();
		if (dir.x == Player::DOWN_DIR.x
			&& dir.y == Player::DOWN_DIR.y
			&& dir.z == Player::DOWN_DIR.z)
		{
			camera_[eventId_]->ChangeMode(Camera::MODE::NONE);
		}
		//カメラを揺らす
		if (players_[eventId_]->GetJumpTime() <= Camera::TIME_SHAKE && players_[eventId_]->GetTrunTime() > 5.0f)
		{
			camera_[eventId_]->ChangeMode(Camera::MODE::SHAKE);
		}
	}
	else if (players_[1]->GetHp() <= 0)
	{
		Sns.SetWinner(SceneManager::WINNER::PLAYER_ONE);
		eventFlag_ = true;
		eventId_ = 1;

		const VECTOR& dir = players_[eventId_]->GetJumpDir();
		if (dir.x == Player::DOWN_DIR.x
			&& dir.y == Player::DOWN_DIR.y
			&& dir.z == Player::DOWN_DIR.z)
		{
			camera_[eventId_]->ChangeMode(Camera::MODE::NONE);
		}
		//カメラを揺らす
		if (players_[eventId_]->GetJumpTime() <= Camera::TIME_SHAKE && players_[eventId_]->GetTrunTime() > 5.0f)
		{
			camera_[eventId_]->ChangeMode(Camera::MODE::SHAKE);
		}
	}

	if (players_[0]->GetState() == Player::STATE::END && players_[1]->GetState() == Player::STATE::END)
	{
	}
	else if (players_[0]->GetState() == Player::STATE::END && eventFlag_)
	{
		players_[1]->ChangeState(Player::STATE::VICTORY);
		eventId_ = 1;
	}
	else if (players_[1]->GetState() == Player::STATE::END && eventFlag_)
	{
		players_[0]->ChangeState(Player::STATE::VICTORY);
		eventId_ = 0;
	}


	stage_->Update();



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

	if (eventFlag_)
	{
		EventDraw();
	}
	else
	{
		GameDraw();


	}

}

void GameScene::Release(void)
{
	


	for (int i = 0; i < SceneManager::PLAYER_SIZE; i++) {
		camera_[i]->Release();
		delete camera_[i];
	}

	//背景解放処理
	stage_->Release();
	delete stage_;

	//backGround_->Release();
	//delete backGround_;

	//エフェクトの開放
	StopEffekseer3DEffect(effectHitPlayId_);

	SoundManager::GetInstance().AllStop();
}

//ゲームの衝突処理
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

			//プレイヤー同士の中心
			VECTOR cPos = VScale(VAdd(plyer->GetTransform().pos, vsPlyer->GetTransform().pos), 0.5f);

			float disPow = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
			if (disPow < Player::DAMAGE_RADIUS * Player::ATTRCK_RADIUS)//ダメージ半径×攻撃半径
			{
				//plyerに相手のマシンの攻撃が当たっている
				plyer->Damage(1);
				//ふっ飛ばし処理
				VECTOR dir = VNorm(VSub(plyer->GetTransform().pos, vsPlyer->GetTransform().pos));
				plyer->SetJump(VScale(dir, 1.5f));
				dir = VScale(dir, -1);//吹っ飛ばしの方向を反転&×５
				vsPlyer->SetJump(dir);

				//音の再生
				SoundManager::GetInstance().Play(SoundManager::SRC::IMPACT, Sound::TIMES::ONCE, true);

				// 爆発エフェクトを再生する
				effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
				SetScalePlayingEffekseer3DEffect(effectHitPlayId_, BLAST_SCALE, BLAST_SCALE, BLAST_SCALE);
				SetRotationPlayingEffekseer3DEffect(effectHitPlayId_, AsoUtility::Deg2RadF(90.0f), 0.0f, 0.0f);
				SetPosPlayingEffekseer3DEffect(effectHitPlayId_, cPos.x, cPos.y, cPos.z);
			}

			//攻撃箇所同士が衝突
			diff = VSub(plyer->GetPos(1), vsPlyer->GetPos(1));

			disPow = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
			if (disPow < Player::ATTRCK_RADIUS * Player::ATTRCK_RADIUS + Player::ATTRCK_RADIUS * Player::ATTRCK_RADIUS)//攻撃半径×攻撃半径
			{
				plyer->Damage(1);
				vsPlyer->Damage(1);

				//ふっ飛ばし処理
				VECTOR dir = VNorm(VSub(plyer->GetTransform().pos, vsPlyer->GetTransform().pos));
				plyer->SetJump(dir);
				dir = VScale(dir, -1);
				vsPlyer->SetJump(dir);

				//音の再生
				SoundManager::GetInstance().Play(SoundManager::SRC::IMPACT, Sound::TIMES::ONCE, true);

				// 爆発エフェクトを再生する
				effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
				SetScalePlayingEffekseer3DEffect(effectHitPlayId_, BLAST_SCALE, BLAST_SCALE, BLAST_SCALE);
				SetRotationPlayingEffekseer3DEffect(effectHitPlayId_, AsoUtility::Deg2RadF(90.0f), 0.0f, 0.0f);
				SetPosPlayingEffekseer3DEffect(effectHitPlayId_, cPos.x, cPos.y, cPos.z);
			}

			//弾の当たり判定
			auto shots = plyer->GetShots();
			for (auto& shot : shots)
			{
				if (shot->IsShot() &&
					(vsPlyer->GetState() == Player::STATE::PLAY || vsPlyer->GetState() == Player::STATE::JUMP))
				{

					//弾の当たり判定
					diff = VSub(shot->GetPos(), vsPlyer->GetPos(2));
					disPow = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
					if (disPow < shot->GetCollisionRadius() * shot->GetCollisionRadius() + Player::DAMAGE_RADIUS * Player::DAMAGE_RADIUS)//攻撃半径×攻撃半径
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

		VECTOR diff = VSub(AsoUtility::VECTOR_ZERO, plyer->GetTransform().pos);

		float disPow = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
		float distance = Player::DAMAGE_RADIUS * Player::DAMAGE_RADIUS + Stage::STAGE_RADIUS * Stage::STAGE_RADIUS;
		if (disPow > distance)//ダメージ半径×攻撃半径
		{
			plyer->ChangeState(Player::STATE::FALL_DEAD);
		}
	}
	

}

void GameScene::GameDraw(void)
{

#pragma region ゲームシーンの描画

	// 描画
	//背景描画
	//backGround_->Draw();
	stage_->Draw();
	int screenSize = 100;
	int mx = Application::SCREEN_SIZE_X - screenSize;
	int my = Application::SCREEN_SIZE_Y - screenSize;

	int cx = Application::SCREEN_SIZE_X - screenSize;
	int cy = Application::SCREEN_SIZE_Y - screenSize;


	//プレイヤーの描画
	int plyNum = 0;

	for (auto& plyer : players_)
	{
		plyer->Draw();

		//HPバーの表示
		plyer->DrawPram(plyNum);
		plyNum += 1;

		//メインカメラ更新
		VECTOR pos2D = ConvWorldPosToScreenPos(plyer->GetTransform().pos);
		//DrawCircle(pos2D.x, pos2D.y, 10, 0x0000ff);

		if ((mx < pos2D.x) || (my < pos2D.y) || (screenSize > pos2D.x) || (screenSize > pos2D.y))
		{
			SceneManager::GetInstance().GetCamera()->FadeOut();
		}
	}

	if (players_[0]->GetState() == Player::STATE::END && players_[1]->GetState() == Player::STATE::END)
	{
		SetFontSize(28);//文字のサイズを設定

		std::string msg = "Result WIN";

		SceneManager& Sns = SceneManager::GetInstance();

		switch (Sns.GetWinner())
		{
			//プレイヤー１の勝利
		case SceneManager::WINNER::PLAYER_ONE:
			msg = "PLAYER1 WIN";
			break;
			//プレイヤー２の勝利
		case SceneManager::WINNER::PLAYER_TWO:
			msg = "PLAYER2 WIN";
			break;
		case SceneManager::WINNER::DRAW:
			msg = "DRAW";
			break;
		}

		int cx = Application::SCREEN_SIZE_X / 2;
		int cy = Application::SCREEN_SIZE_Y / 2;

		int len = (int)strlen(msg.c_str());
		int width = GetDrawStringWidth(msg.c_str(), len);
		DrawFormatString(cx - (width / 2), cy, 0xffffff, msg.c_str());

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetFontSize(16);
	}

	//デバッグ描画
#ifdef _DEBUG
	DrawDebug();
#endif

#pragma endregion

}
void GameScene::EventDraw(void)
{
	int cx = Application::SCREEN_SIZE_X/2;
	int cy = Application::SCREEN_SIZE_Y/2;


	for (int i = 0; i < SceneManager::PLAYER_SIZE; i++)
	{
		// 設定したいスクリーンを作成する
		SetDrawScreen(screenH[i]);
		// 画面を初期化
		ClearDrawScreen();
		// カメラ設定
		camera_[i]->SetBeforeDraw();
		//

#pragma region ゲームシーンの描画

	// 描画
	//背景描画
	//backGround_->Draw();
		stage_->Draw();
		int screenSize = 100;
		int mx = Application::SCREEN_SIZE_X - screenSize;
		int my = Application::SCREEN_SIZE_Y - screenSize;

		//プレイヤーの描画
		for (auto& plyer : players_)
		{
			plyer->Draw();

			//メインカメラ更新
			VECTOR pos2D = ConvWorldPosToScreenPos(plyer->GetTransform().pos);

			if ((mx < pos2D.x) || (my < pos2D.y) || (screenSize > pos2D.x) || (screenSize > pos2D.y))
			{
				SceneManager::GetInstance().GetCamera()->FadeOut();
			}
		}


		//デバッグ描画
#ifdef _DEBUG
		DrawDebug();
#endif

#pragma endregion

	}
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
	if (eventId_ != -1)
	{
		DrawGraph(0, 0, screenH[eventId_], true);
	}

	//プレイヤーパラメーターの描画
	int plyNum = 0;

	bool eventFlag = false;
	for (auto& plyer : players_)
	{
		if (plyer->GetState()==Player::STATE::END)
		{
			eventFlag = true;
		}
	}
	if (eventFlag)
	{
		if (players_[0]->GetHp() <= 0 && players_[1]->GetHp() <= 0)
		{
			SetFontSize(28);//文字のサイズを設定

			std::string msg = "Result WIN";

			SceneManager& Sns = SceneManager::GetInstance();

			switch (Sns.GetWinner())
			{
				//プレイヤー１の勝利
			case SceneManager::WINNER::PLAYER_ONE:
				msg = "PLAYER1 WIN";
				break;
				//プレイヤー２の勝利
			case SceneManager::WINNER::PLAYER_TWO:
				msg = "PLAYER2 WIN";
				break;
			case SceneManager::WINNER::DRAW:
				msg = "DRAW";
				break;
			}

			int cx = Application::SCREEN_SIZE_X / 2;
			int cy = Application::SCREEN_SIZE_Y / 2;

			int len = (int)strlen(msg.c_str());
			int width = GetDrawStringWidth(msg.c_str(), len);
			DrawFormatString(cx - (width / 2), cy, 0xffffff, msg.c_str());

			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			SetFontSize(16);
		}

		for (auto& plyer : players_)
		{
			//HPバーの表示
			plyer->DrawPram(plyNum);
			plyNum += 1;
		}
	}

	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

}

//デバッグ描画
void GameScene::DrawDebug(void)
{
	//グリッド線の描画
	stage_->DrawDebug();

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;


	
	printf("Effect ID: %d, Position: (0, 0, 0), Scale: %f\n", effectHitPlayId_, BLAST_SCALE);

	DrawFormatString(0, 64, 0xff0000, "Effect ID: %d, Position: (0, 0, 0), Scale: %f\n", effectHitPlayId_, BLAST_SCALE);


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
