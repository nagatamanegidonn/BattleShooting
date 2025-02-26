
#include "../../Application.h"

#include "../../Manager/SceneManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"

#include "../../Utility/AsoUtility.h"

#include "../Shot/ShotPlayer.h"

#include "Controller.h"
#include "Player.h"


Player::Player(Camera& camera) :camera_(camera)
{
	// 状態管理
	stateChanges_.emplace(STATE::NONE, std::bind(&Player::ChangeStateNone, this));
	stateChanges_.emplace(STATE::PLAY, std::bind(&Player::ChangeStatePlay, this));
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

	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_SHIP));

	float scale = 10.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = startPos;
	//transform_.pos = { 10.0f, 20.0f, 30.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();


	// モデル制御の基本情報
	transDir_.SetModel(MV1LoadModel("Data/Model/Dir/Dir.mv1"));
	if (playerNo == 0)
	{
		MV1SetMaterialDifColor(transDir_.modelId, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		MV1SetMaterialEmiColor(transDir_.modelId, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		MV1SetMaterialDifColor(transDir_.modelId, 0, GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
		MV1SetMaterialEmiColor(transDir_.modelId, 0, GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
	}
	transDir_.scl = { 0.1f, 0.1f, 0.1f };
	transDir_.pos = startPos;

	transDir_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	transDir_.quaRotLocal = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	transDir_.Update();

	//変数：移動関係
	movedPos_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;

	//変数：回転関係
	direction_= AsoUtility::VECTOR_ZERO;
	direction_.y = ROT_POW;

	//ステータス
	damageTime_ = 0.0f;
	invincibleTime_ = 0.0f;

	rideAttrckPos_ = transform_.pos;
	rideDamagePos_ = transform_.pos;

	//ステータス変数
	ridesMaxHp_ = ridesHp_ = 10;

	//矢印の作成
	MakeSquereVertex();

	//変数：攻撃関係
	//弾発射後の硬直時間セット
	deleyShot_ = TIME_DELAY_SHOT;


	// 初期状態
	ChangeState(STATE::PLAY);


}

void Player::Update()
{
	controller_->Update();

	// 更新ステップ
	stateUpdate_();

	transform_.Update();
	transDir_.Update();

	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		shots_[i]->Update();
	}

	rideDamagePos_ = VAdd(transform_.pos, VScale(transform_.quaRot.GetBack(), 20));
	rideAttrckPos_ = VAdd(transform_.pos, VScale(transform_.quaRot.GetForward(), 20));
}

void Player::Draw()
{

	// モデルの描画
	// 視野範囲内：ディフューズカラーを赤色にする

	MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(0.0f, 0.0f, 0.0f, 1.0f));
	if (invincibleTime_ > 0.0f)
	{
		if (static_cast<int>(invincibleTime_) % 2 == 0)
		{
			MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}
	MV1DrawModel(transform_.modelId);

	DrawSphere3D(rideAttrckPos_, 10, 10, 0xff0000, 0xffff00, false);
	DrawSphere3D(rideDamagePos_, 20, 10, 0xff0000, 0x00ffff, false);


#pragma region 矢印の描画（仮）

	VECTOR addAxis = VScale(direction_, 10);


	if (controller_->GetisControl(Controller::MODE::BACK))
	{
		//addAxis.y *= -1;
	}

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

	transDir_.pos = VAdd(transform_.pos, VScale(trnQut.GetForward(), 40));
	transDir_.quaRot = trnQut;
	transDir_.Update();
	// ３Ｄモデルの描画
	MV1DrawModel(transDir_.modelId);


	auto c = MV1GetMaterialDifColor(transDir_.modelId, 0);

	// メッシュの描画
	/*VECTOR zero = VAdd(transform_.pos, VScale(trnQut.GetLeft(), 40));
	VECTOR one = VAdd(transform_.pos, VScale(trnQut.GetForward(), 80));
	VECTOR two = VAdd(transform_.pos, VScale(trnQut.GetForward(), 20));
	VECTOR three = VAdd(transform_.pos, VScale(trnQut.GetRight(), 40));*/

	/*DrawSphere3D(zero, 10, 10, 0xff0000, 0xff0000, false);
	DrawSphere3D(one, 10, 10, 0xff0000, 0xff0000, false);
	DrawSphere3D(two, 10, 10, 0xff0000, 0xff0000, false);
	DrawSphere3D(three, 10, 10, 0xff0000, 0xff0000, false);*/

	//DrawTriangle3D(zero, one, two, 0x0000ff, true);
	//DrawTriangle3D( two,one, three, 0x0000ff, true);

#pragma endregion


	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		shots_[i]->Draw();
	}
}

const void Player::RideDamage(int damage)
{
	if (invincibleTime_ > 0.0f)
	{
		return;
	}
	//体力を減らす
	ridesHp_ -= damage;
	//無敵時間を設定
	invincibleTime_ = 3.0f;
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


#pragma region 変数state_による関数stateUpdate_の変更

void Player::ChangeState(STATE state)
{

	// 状態変更
	state_ = state;

	// 各状態遷移の初期処理
	stateChanges_[state_]();

}
void Player::ChangeStateNone()
{
	stateUpdate_ = std::bind(&Player::UpdateNone, this);
}
void Player::ChangeStatePlay()
{
	stateUpdate_ = std::bind(&Player::UpdatePlay, this);
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

}
void Player::UpdatePlay()
{
	if (invincibleTime_ > 0.0f)
	{
		invincibleTime_ -= SceneManager::GetInstance().GetDeltaTime();
	}


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
	

	if (controller_->GetisControl(Controller::MODE::LEFT)) { direction_.y = ROT_POW; }
	if (controller_->GetisControl(Controller::MODE::RIGHT)) { direction_.y = -ROT_POW; }

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
			v->Create(transform_.pos, transform_.GetForward());
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
		newShot->Create(transform_.pos, transform_.GetForward());

		// 弾の管理配列に追加
		shots_.push_back(newShot);
	}
}

void Player::MakeSquereVertex(void)
{
	
}
