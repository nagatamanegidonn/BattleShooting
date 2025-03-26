
#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"

#include "../../Application.h"

#include "../../Utility/AsoUtility.h"

#include "../Common/AnimationController.h"

#include "../Shot/ShotPlayer.h"

#include "Controller.h"
#include "DirModel.h"
#include "Player.h"


//Player::Player(Camera& camera) :camera_(camera)
Player::Player()
{
	// 状態管理
	stateChanges_.emplace(STATE::NONE, std::bind(&Player::ChangeStateNone, this));
	stateChanges_.emplace(STATE::PLAY, std::bind(&Player::ChangeStatePlay, this));
	stateChanges_.emplace(STATE::JUMP, std::bind(&Player::ChangeStateJump, this));
	stateChanges_.emplace(STATE::DEAD, std::bind(&Player::ChangeStateDead, this));
	stateChanges_.emplace(STATE::END, std::bind(&Player::ChangeStateEnd, this));
	stateChanges_.emplace(STATE::VICTORY, std::bind(&Player::ChangeStateVictory, this));


	//変数：移動関係
	movedPos_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;

	//変数：回転関係
	direction_ = AsoUtility::VECTOR_ZERO;
	direction_.y = ROT_POW;

}
Player::~Player()
{
}

void Player::Init(VECTOR startPos, int playerNo)
{

	controller_ = std::make_shared<Controller>();
	controller_->Init(playerNo);

	dirModel_ = std::make_shared<DirModel>();
	dirModel_->Init(playerNo);

	// モデル制御の基本情報
	/*transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_SHIP));*/

	if (playerNo == 0)
	{
		transform_.SetModel(MV1LoadModel("Data/Model/P1/P1.mv1"));
		shotModel_= ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::P1_SHOT_MODEL);

		MV1SetMaterialDifColor(transform_.modelId, 3, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		MV1SetMaterialEmiColor(transform_.modelId, 3, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		//アニメーションの設定
		InitAnimation(Application::PATH_MODEL + "P1/P1.mv1");

	}
	else
	{
		transform_.SetModel(MV1LoadModel("Data/Model/P2/P2.mv1"));
		shotModel_ = ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::P2_SHOT_MODEL);

		MV1SetMaterialDifColor(transform_.modelId, 3, GetColorF(0.5f, 0.5f, 1.0f, 1.0f));
		MV1SetMaterialEmiColor(transform_.modelId, 3, GetColorF(0.5f, 0.5f, 1.0f, 1.0f));
		//アニメーションの設定
		InitAnimation(Application::PATH_MODEL + "P2/P2.mv1");

		// マテリアルの自己発光色を設定
		MV1SetMaterialEmiColor(transform_.modelId, 4, GetColorF(0.2f, 0.2f, 0.2f, 1.0f));
	}

	//float scale = 10.0f;
	float scale = 0.3f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = startPos;
	//transform_.pos = { 10.0f, 20.0f, 30.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(-90.0f),
		AsoUtility::Deg2RadF(0.0f)
	);
	transform_.Update();

	//変数：移動関係
	movedPos_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;

	//変数：回転関係
	direction_= AsoUtility::VECTOR_ZERO;
	direction_.y = ROT_POW;

	//ふっ飛ばし関係
	jumpTime_ = 0.0f;
	jumpDir_ = AsoUtility::VECTOR_ZERO;

	//ステータス
	damageTime_ = 0.0f;
	invincibleTime_ = 0.0f;

	rideAttrckPos_ = transform_.pos;
	rideDamagePos_ = transform_.pos;

	//ステータス変数
	//HPもとは１０
	playerMaxHp_ = playerHp_ = 5;

	//矢印の作成
	MakeSquereVertex();

	//変数：攻撃関係
	//弾発射後の硬直時間セット
	deleyShot_ = TIME_DELAY_SHOT;


	// 初期状態
	ChangeState(STATE::PLAY);

	InitEffect();

}

void Player::Update()
{
	controller_->Update();

	// 更新ステップ
	stateUpdate_();


	transform_.Update();

	/*size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		shots_[i]->Update();
	}*/

	rideDamagePos_ = VAdd(transform_.pos, VScale(transform_.quaRot.GetBack(), 0));
	rideAttrckPos_ = VAdd(transform_.pos, VScale(transform_.quaRot.GetForward(), 40));

	// アニメーション再生
	animationController_->Update();
}

void Player::Draw()
{
	//ゲーム中でなければモデルのみを描画
	if (SceneManager::GetInstance().GetSceneID() != SceneManager::SCENE_ID::GAME)
	{
		MV1DrawModel(transform_.modelId);
		return;
	}
	int temp = static_cast<int>(state_);
	DrawFormatString(0, 64, 0x00FF, "PlayerParam: %d",temp);

	// モデルの描画
	// 視野範囲内：ディフューズカラーを赤色にする

	MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(1.0f, 1.0f, 1.0f, 1.0f));
	MV1SetMaterialEmiColor(transform_.modelId, 0, GetColorF(1.0f, 1.0f, 1.0f, 1.0f));
	if (invincibleTime_ > 0.0f)
	{
		if (invincibleTime_ >= 0.1f)
		{
			MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
			MV1SetMaterialEmiColor(transform_.modelId, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}
	MV1DrawModel(transform_.modelId);
	

#pragma region 矢印の描画（仮）

	VECTOR addAxis = VScale(direction_, 10);


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
	auto trnQut = transform_.quaRot;
	trnQut = trnQut.Mult(rotPow);


	// メッシュの描画
	VECTOR zero = VAdd(transform_.pos, VScale(trnQut.GetLeft(), 40));
	VECTOR one = VAdd(transform_.pos, VScale(trnQut.GetForward(), 80));
	VECTOR two = VAdd(transform_.pos, VScale(trnQut.GetForward(), 20));
	VECTOR three = VAdd(transform_.pos, VScale(trnQut.GetRight(), 40));

	dirModel_->SyncModel(VAdd(transform_.pos, VScale(trnQut.GetForward(), 40)), trnQut);
	dirModel_->Draw();


	//DrawTriangle3D(zero, one, two, 0x0000ff, true);
	//DrawTriangle3D( two,one, three, 0x0000ff, true);

#pragma endregion


	DrawSphere3D(rideAttrckPos_, ATTRCK_RADIUS, 10, 0xff0000, 0xff0000, false);
	DrawSphere3D(rideDamagePos_, DAMAGE_RADIUS, 10, 0x0000ff, 0x0000ff, false);

	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		shots_[i]->Draw();
	}
}

//ダメージ
const void Player::Damage(int damage)
{
	if (invincibleTime_ > 0.0f)
	{
		return;
	}
	//体力を減らす
	playerHp_ -= damage;
	if (playerHp_ <= 0)
	{
		playerHp_ = 0;
	}
	//無敵時間を設定
	invincibleTime_ = 1.0f;
}
const void Player::SetJump(VECTOR vec)
{
	//操作可能状態なら吹っ飛ばす

	ChangeState(STATE::JUMP);
	jumpDir_ = vec;
	jumpTime_ = 1.0f;
}

VECTOR& Player::GetPos(int id)
{
	switch (id)
	{
	case 0://乗り物のdamage位置
		return rideDamagePos_;
		break;
	case 1://乗り物の攻撃地点
		return rideAttrckPos_;
		break;
	case 2:
		return transform_.pos;
		break;
	}

	return transform_.pos;
	// TODO: return ステートメントをここに挿入します
}

//アニメーションの設定
void Player::InitAnimation(std::string path)
{

	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path, 20.0f, 0);
	animationController_->Add((int)ANIM_TYPE::RUN, path, 20.0f, 1);

	animationController_->Play((int)ANIM_TYPE::IDLE);

}

void Player::InitEffect(void)
{


}

#pragma region 変数state_による関数stateUpdate_の変更

void Player::ChangeState(STATE state)
{
	if (state_ == STATE::DEAD)
	{
		return;
	}

	// 状態変更
	state_ = state;

	// 各状態遷移の初期処理
	stateChanges_[state_]();

}
void Player::ChangeStateNone()
{
	stateUpdate_ = std::bind(&Player::UpdateNone, this);

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
	transform_.Update();
}
void Player::ChangeStatePlay()
{
	stateUpdate_ = std::bind(&Player::UpdatePlay, this);
}
void Player::ChangeStateJump(void)
{
	stateUpdate_ = std::bind(&Player::UpdateJump, this);
}
void Player::ChangeStateDead()
{
	stateUpdate_ = std::bind(&Player::UpdateDead, this);
}
void Player::ChangeStateEnd()
{
	stateUpdate_ = std::bind(&Player::UpdateEnd, this);
}
void Player::ChangeStateVictory()
{
	stateUpdate_ = std::bind(&Player::UpdateVictory, this);
}

#pragma endregion


// 更新ステップ
#pragma region state_ごとによる更新処理

void Player::UpdateNone()
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
void Player::UpdatePlay()
{


	ProcessTurn();
	ProcessMove();



	ProcessShot();

	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);

	//移動後の位置に問題がある確認
	// 衝突(カプセル)
	//CollisionCapsule();
	// 衝突(重力)
	//CollisionGravity();
	
	// 移動後の位置に問題がなければ移動
	transform_.pos = movedPos_;

}
void Player::UpdateJump(void)
{
	if (invincibleTime_ > 0.0f)
	{
		invincibleTime_ -= SceneManager::GetInstance().GetDeltaTime();
	}
	if (jumpTime_ > 0.0f)
	{
		jumpTime_ -= SceneManager::GetInstance().GetDeltaTime();
		if (jumpTime_ <= 0.0f)
		{
			//体力が０なら死亡状態にする
			if (playerHp_ <= 0)
			{
				ChangeState(STATE::DEAD);
				return;
			}
			ChangeState(STATE::PLAY);
			return;
		}
	}

	// 移動
	movePow_ =
		VScale(jumpDir_, SPEED_MOVE * jumpTime_);

	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);
	// 移動後の位置に問題がなければ移動
	transform_.pos = movedPos_;
}
void Player::UpdateDead()
{

}
void Player::UpdateEnd()
{

}
void Player::UpdateVictory()
{

}

#pragma endregion

void Player::ProcessMove(void)
{
	Move();

	InputManager& ins = InputManager::GetInstance();
	InputManager::JOYPAD_NO jno = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD1);

	// 左スティックの横軸
	auto leftStickX = ins.GetJPadInputState(jno).AKeyLX;
	// 左スティックの縦軸
	auto leftStickY = ins.GetJPadInputState(jno).AKeyLY;



	VECTOR dir = AsoUtility::VECTOR_ZERO;
	float rotRad = 0.0f;

	if ((leftStickY < 0.0f) || (ins.IsNew(KEY_INPUT_W))) { dir = VAdd(dir, AsoUtility::DIR_F); }
	if ((leftStickX < 0.0f) || (ins.IsNew(KEY_INPUT_A))) { dir = VAdd(dir, AsoUtility::DIR_L); }
	if ((leftStickY > 0.0f) || (ins.IsNew(KEY_INPUT_S))) { dir = VAdd(dir, AsoUtility::DIR_B); }
	if ((leftStickX > 0.0f) || (ins.IsNew(KEY_INPUT_D))) { dir = VAdd(dir, AsoUtility::DIR_R); }


	//movePow_ = AsoUtility::VECTOR_ZERO;


}
void Player::Move(void)
{
	auto& input = InputManager::GetInstance();

	movePow_ = AsoUtility::VECTOR_ZERO;
	
	// 前方向を取得(仮)
	VECTOR forward = AsoUtility::VECTOR_ZERO;

	

	if (controller_->GetisControl(Controller::MODE::FORWARD)) { forward = transform_.GetForward(); }
	if (controller_->GetisControl(Controller::MODE::BACK)) { forward = transform_.GetBack(); }

	/*for (auto& key : controlKey_[DIR::UP])
	{
		if (input.IsNew(key)) {forward = transform_.GetForward(); }
	}
	for (auto& key : controlKey_[DIR::DOWN])
	{
		if (input.IsNew(key)) { forward = transform_.GetBack(); }
	}*/

	// 移動
	movePow_ =
		VScale(forward, SPEED_MOVE);

}


void Player::ProcessTurn(void)
{
	Turn(AsoUtility::VECTOR_ONE);
}
void Player::Turn(VECTOR axis)
{

	auto& input = InputManager::GetInstance();

	bool isMove = (controller_->GetisControl(Controller::MODE::FORWARD) 
		|| controller_->GetisControl(Controller::MODE::BACK));
	

	if (controller_->GetisControl(Controller::MODE::LEFT)) { direction_.y = -ROT_POW; }
	if (controller_->GetisControl(Controller::MODE::RIGHT)) { direction_.y = ROT_POW; }

	VECTOR addAxis = direction_;

	if (controller_->GetisControl(Controller::MODE::BACK))
	{
		addAxis.y *= -1;
	}


	//左回転
	//addAxis.y = -1.0f;
	//右回転
	//addAxis.y = 1.0f;

	if (!AsoUtility::EqualsVZero(addAxis) && isMove)
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

void Player::ProcessShot(void)
{
	auto& ins = InputManager::GetInstance();

	//タックル攻撃
	if ((controller_->GetisControl(Controller::MODE::JATTACK)
		&& jumpTime_ <= 0.0f))
	{
		VECTOR dir = VScale(transform_.GetForward(), 2.0f);
		SetJump(dir);
	}

	//射撃攻撃
	if ((controller_->GetisControl(Controller::MODE::ATTACK)
		&& deleyShot_ <= 0.0f))
	{

		// 弾を生成(方向は仮で正面方向)
		//shot->CreateShot(barrelPos_, { 0.0f, 0.0f, 1.0f });
		// 弾を指定位置から、指定方向に発射させる
		CreateShot();


		// 弾発射後の硬直時間セット
		deleyShot_ = TIME_DELAY_SHOT;
	}


	// 弾発射後の硬直時間を減らしていく
	if (deleyShot_ > 0.0f)
	{
		deleyShot_ -= SceneManager::GetInstance().GetDeltaTime();
	}
}
void Player::CreateShot(void)
{
	// 弾の生成フラグ
	bool isCreate = false;

	for (auto v : shots_)
	{
		if (v->GetState() == ShotPlayer::STATE::END)
		{
			// 以前に生成したインスタンスを使い回し
			v->Create(transform_.pos, transform_.GetForward(),shotModel_);
			isCreate = true;
			break;
		}
	}
	if (!isCreate)
	{
		// 自機の前方方向
		auto dir = transform_.GetForward();
		// 新しいインスタンスを生成
		ShotPlayer* newShot = new ShotPlayer();
		newShot->Create(transform_.pos, transform_.GetForward(), shotModel_);

		// 弾の管理配列に追加
		shots_.push_back(newShot);
	}
}

void Player::MakeSquereVertex(void)
{
	
}
