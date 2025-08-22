#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"

#include "../../Application.h"

#include "../../Utility/AsoUtility.h"

#include "../Common/AnimationController.h"

#include "../../Manager/SoundManager.h"

#include "ViewPlayer.h"

//Player::Player(Camera& camera) :camera_(camera)
ViewPlayer::ViewPlayer()
{
	// 状態管理
	stateChanges_.emplace(STATE::NONE, std::bind(&ViewPlayer::ChangeStateNone, this));
	stateChanges_.emplace(STATE::PLAY, std::bind(&ViewPlayer::ChangeStatePlay, this));
	stateChanges_.emplace(STATE::END, std::bind(&ViewPlayer::ChangeStateEnd, this));


	
}
ViewPlayer::~ViewPlayer()
{
}

void ViewPlayer::Init(VECTOR startPos, int playerNo, int pryId)
{
	charId_ = pryId;
	MV1DrawModel(transform_.modelId);

	// モデル制御の基本情報
	//プレイキャラごとに代わる
	if (pryId == 0)
	{
		transform_.SetModel(ResourceManager::GetInstance().Load(ResourceManager::SRC::P1_MODEL).handleId_);

		MV1SetMaterialDifColor(transform_.modelId, 3, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		MV1SetMaterialEmiColor(transform_.modelId, 3, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		//アニメーションの設定
		InitAnimation(Application::PATH_MODEL + "P1/P1.mv1");

	}
	else if (pryId == 1)
	{
		transform_.SetModel(ResourceManager::GetInstance().Load(ResourceManager::SRC::P2_MODEL).handleId_);

		MV1SetMaterialDifColor(transform_.modelId, 3, GetColorF(0.5f, 0.5f, 1.0f, 1.0f));
		MV1SetMaterialEmiColor(transform_.modelId, 3, GetColorF(0.5f, 0.5f, 1.0f, 1.0f));
		//アニメーションの設定
		InitAnimation(Application::PATH_MODEL + "P2/P2.mv1");

		// マテリアルの自己発光色を設定
		MV1SetMaterialEmiColor(transform_.modelId, 4, GetColorF(0.2f, 0.2f, 0.2f, 1.0f));

	}
	else if (pryId == 2)
	{
		transform_.SetModel(ResourceManager::GetInstance().Load(ResourceManager::SRC::P3_MODEL).handleId_);

		//アニメーションの設定
		InitAnimation(Application::PATH_MODEL + "P3/P3.mv1");


		int num = MV1GetMaterialNum(transform_.modelId);

		std::string a = "";
		std::string tex = "";
		for (int i = 0; i < num; i++)
		{
			a += MV1GetMaterialName(transform_.modelId, i);
			a += ":";
			tex += MV1GetMaterialDifMapTexture(transform_.modelId, i);
			tex += ":";
		}

		std::string s1 = a;
		std::string s2 = tex;

	}
	else if (pryId == 3)
	{
		transform_.SetModel(ResourceManager::GetInstance().Load(ResourceManager::SRC::P4_MODEL).handleId_);

		//アニメーションの設定
		InitAnimation(Application::PATH_MODEL + "P4/P4.mv1");
	}

	//トランスフォームの設定
	float scale = 1.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = startPos;
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(90.0f),
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(-10.0f)
	);
	transform_.Update();

	
	// 初期状態
	ChangeState(STATE::PLAY);

}

void ViewPlayer::Update()
{
	//controller_->Update();


	// 更新ステップ
	stateUpdate_();

	transform_.Update();

	// アニメーション再生
	animationController_->Update();
}

void ViewPlayer::Draw()
{
	//ゲーム中でなければモデルのみを描画
	if (SceneManager::GetInstance().GetSceneID() != SceneManager::SCENE_ID::GAME)
	{
		MV1DrawModel(transform_.modelId);



		const int cx = Application::SCREEN_SIZE_X / 2;
		const int cy = Application::SCREEN_SIZE_Y / 2;

		int RATE_GAGE = 20;

		float sy = cy / 2 - 100;
		float sxP1 = cx - (cx / 2) + 100;
		float sxP2 = cx + (cx / 2) + 100;

		if (charId_ >= 2)
		{
			//
			DrawParm("装填数", { sxP2,sy,0.0f }, RATE_GAGE * 3, RATE_GAGE * 6, 0xf0f000);

			//ATKバー
			DrawParm("吹っ飛ばし力", { sxP2,sy + 80,0.0f }, RATE_GAGE * 5, RATE_GAGE * 6, 0xff0000);

			//DEFバー
			DrawParm("吹き飛び耐性", { sxP2,sy + 160,0.0f }, RATE_GAGE * 4, RATE_GAGE * 6, 0x0000ff);
		}
		else if (charId_ >= 0)
		{
			//
			DrawParm("装填数", { sxP1,sy,0.0f }, RATE_GAGE * 5, RATE_GAGE * 6, 0xf0f000);

			//ATKバー
			DrawParm("吹っ飛ばし力", { sxP1,sy + 80,0.0f }, RATE_GAGE * 3, RATE_GAGE * 6, 0xff0000);

			//DEFバー
			DrawParm("吹き飛び耐性", { sxP1,sy + 160,0.0f }, RATE_GAGE * 3, RATE_GAGE * 6, 0x0000ff);
		}
		return;
	}

}

//アニメーションの設定
void ViewPlayer::InitAnimation(std::string path)
{

	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path, 20.0f, 0);
	animationController_->Add((int)ANIM_TYPE::RUN, path, 20.0f, 0);

	animationController_->Play((int)ANIM_TYPE::IDLE);

}


#pragma region 変数state_による関数stateUpdate_の変更

void ViewPlayer::ChangeState(STATE state)
{

	// 状態変更
	state_ = state;

	// 各状態遷移の初期処理
	stateChanges_[state_]();

}
void ViewPlayer::ChangeStateNone()
{
	stateUpdate_ = std::bind(&ViewPlayer::UpdateNone, this);

	float scale = 1.0f;
	transform_.scl = { scale, scale, scale };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(90.0f),
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(-10.0f)
	);
}
void ViewPlayer::ChangeStatePlay()
{
	stateUpdate_ = std::bind(&ViewPlayer::UpdatePlay, this);
}
void ViewPlayer::ChangeStateEnd()
{
	stateUpdate_ = std::bind(&ViewPlayer::UpdateEnd, this);
}

#pragma endregion

// 更新ステップ
#pragma region state_ごとによる更新処理

void ViewPlayer::UpdateNone()
{
	//今回回転させたい回転量をクォータニオンで作る
	Quaternion rotPow = Quaternion();

	rotPow = rotPow.Mult(
		Quaternion::AngleAxis(
			AsoUtility::Deg2RadF(1.0f), AsoUtility::AXIS_Z
		));

	// 回転諒を加える(合成)
	transform_.quaRot = transform_.quaRot.Mult(rotPow);
	transform_.Update();

}
void ViewPlayer::UpdatePlay()
{

	Turn({ 0.0f,0.0f,1.0f });
	
}
void ViewPlayer::UpdateEnd()
{


}

#pragma endregion


void ViewPlayer::Turn(VECTOR axis)
{

	VECTOR addAxis = axis;

	
	//左回転
	//addAxis.y = -1.0f;
	//右回転
	//addAxis.y = 1.0f;

	if (!AsoUtility::EqualsVZero(addAxis))
	{
		//今回回転させたい回転量をクォータニオンで作る
		Quaternion rotPow = Quaternion();

		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(addAxis.z), AsoUtility::AXIS_Z
			));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(addAxis.x), AsoUtility::AXIS_X
			));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(addAxis.y), AsoUtility::AXIS_Y
			));

		// 回転諒を加える(合成)
		transform_.quaRot = transform_.quaRot.Mult(rotPow);

	}

}



void ViewPlayer::DrawParm(std::string pramName, const VECTOR sPos, int lenge, int freaLen, unsigned int color)
{
	//HPバー
	int hight = 32;
	DrawBox(sPos.x - 1, sPos.y - 1, sPos.x + freaLen + 1, sPos.y + hight + 1, 0x000000, true);
	DrawBox(sPos.x, sPos.y, sPos.x + lenge, sPos.y + hight, color, true);

	int strHight = 32;
	int outLineScl = 2;
	DrawStringToHandle(sPos.x, sPos.y - strHight + outLineScl, pramName.c_str(), 0x000000, SceneManager::GetInstance().GetFont());
	DrawStringToHandle(sPos.x, sPos.y - strHight - outLineScl, pramName.c_str(), 0x000000, SceneManager::GetInstance().GetFont());
	DrawStringToHandle(sPos.x + outLineScl, sPos.y - strHight, pramName.c_str(), 0x000000, SceneManager::GetInstance().GetFont());
	DrawStringToHandle(sPos.x - outLineScl, sPos.y - strHight, pramName.c_str(), 0x000000, SceneManager::GetInstance().GetFont());

	DrawStringToHandle(sPos.x, sPos.y - strHight, pramName.c_str(), 0xffffff, SceneManager::GetInstance().GetFont());


}
