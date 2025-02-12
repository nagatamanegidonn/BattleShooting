
#include "../../Manager/InputManager.h"
#include "../../Manager/ResourceManager.h"

#include "../../Utility/AsoUtility.h"

#include "Player.h"

Player::Player()
{
	// 状態管理
	stateChanges_.emplace(STATE::NONE, std::bind(&Player::ChangeStateNone, this));
	stateChanges_.emplace(STATE::PLAY, std::bind(&Player::ChangeStatePlay, this));
	stateChanges_.emplace(STATE::DEAD, std::bind(&Player::ChangeStateDead, this));
	stateChanges_.emplace(STATE::END, std::bind(&Player::ChangeStateEnd, this));
	stateChanges_.emplace(STATE::VICTORY, std::bind(&Player::ChangeStateVictory, this));



}
Player::~Player()
{
}

void Player::Init()
{
	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_SHIP));

	float scale = 10.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	//transform_.pos = { 10.0f, 20.0f, 30.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();


	movedPos_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;


	// 初期状態
	ChangeState(STATE::PLAY);


}

void Player::Update()
{
	// 更新ステップ
	stateUpdate_();

	transform_.Update();
}

void Player::Draw()
{

	// モデルの描画
	MV1DrawModel(transform_.modelId);

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


	ProcessTurn();
	ProcessMove();


	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);

	// 衝突(カプセル)
	//CollisionCapsule();
	// 衝突(重力)
	//CollisionGravity();
	
	// 移動
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
}
void Player::Move(void)
{
	movePow_ = AsoUtility::VECTOR_ZERO;


	// 前方向を取得
	VECTOR forward = transform_.GetForward();
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
	VECTOR addAxis = AsoUtility::VECTOR_ZERO;

	//左回転
	//addAxis.y = -1.0f;
	//右回転
	addAxis.y = 1.0f;

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
