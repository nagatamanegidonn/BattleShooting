#pragma once

#include<vector>
#include <map>
#include <functional>
#include <memory>  // shared_ptrを使うために必要(Controll)

#include "../Common/Transform.h"

//#include "Controller.h"

class AnimationController;
class ShotPlayer;
class Controller;
class DirModel;


class Player
{
public:

	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.1f;

	// スピード
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float ROT_POW = 3.25f;


	// 弾の発射間隔
	static constexpr float TIME_DELAY_SHOT = 0.2f;
	static constexpr float TIME_RELOAD = 2.0f;

	//半径
	static constexpr float ATTRCK_RADIUS = 25.0f;
	static constexpr float DAMAGE_RADIUS = 35.0f;

	static constexpr VECTOR DOWN_DIR = { 0.0f,-0.3f,0.0f };
	

	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
		JUMP,
		FALL_DEAD,
		DEAD,
		VICTORY,
		END
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
	};

    //Player(Camera& camera);  // コンストラクタ
    Player();  // コンストラクタ
    ~Player(); // デストラクタ

    void Init(VECTOR startPos,int playerNo,int pryId);  // 初期化
    void Update();  // 更新
    void Draw();  // 描画
    void DrawPram(int plyarNo);  // 描画

	// 状態遷移
	void ChangeState(STATE state);

	//プレイヤーのTransform情報
	const Transform& GetTransform(void) const { return transform_; }
	const int& GetModelId(void) const { return transform_.modelId; }

	const STATE& GetState(void) const { return state_; }
	const bool IsDead(void) const { return state_ == STATE::DEAD; }

	const VECTOR& GetJumpDir(void) const { return jumpDir_; }
	const float& GetJumpTime(void) const { return jumpTime_; }
	const float& GetTrunTime(void) const { return turnTime_; }

	const int& GetMaxHp(void) const { return playerMaxHp_; }
	const int& GetHp(void) const { return playerHp_; }
	const void Damage(int damage);
	const void SetJump(VECTOR vec);
	VECTOR& GetPos(int id);

	// 弾
	const std::vector<ShotPlayer*>& GetShots(void) const { return shots_; }


private:
	//自身のカメラ
	//Camera& camera_;

	std::shared_ptr<Controller> controller_;
	std::shared_ptr<DirModel> dirModel_;

	// 状態管理
	STATE state_;

	// 状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void(void)>> stateChanges_;
	// 状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;


	//爆発エフェクト
	int effectDestroyResId_;
	int effectDestroyPlayId_;

	// アニメーション
	std::unique_ptr<AnimationController> animationController_;
	//アニメーションの初期化
	void InitAnimation(std::string path);

	// エフェクト初期化
	void InitEffect(void);
	
	// 状態遷移
	void ChangeStateNone(void);
	void ChangeStatePlay(void);
	void ChangeStateJump(void);
	void ChangeStateDead(void);
	void ChangeStateFallDead(void);
	void ChangeStateEnd(void);
	void ChangeStateVictory(void);


	// 更新ステップ
	void UpdateNone(void);
	void UpdatePlay(void);
	void UpdateJump(void);
	void UpdateFallDead(void);//落下死亡をする更新
	void UpdateDead(void);//死亡爆発をする更新
	void UpdateEnd(void);
	void UpdateVictory(void);


	//	最終的な移動量
	VECTOR movedPos_;
	VECTOR movePow_;
	//操作：移動
	void ProcessMove(void);
	void Move(void);

	float turnTime_;
	VECTOR direction_;
	//操作：回転
	void ProcessTurn(void);
	void Turn(VECTOR axis);


	// 操作：移動ブースト
	void ProcessBoost(void);


	// 弾
	std::vector<ShotPlayer*> shots_;
	//弾のモデル
	int shotModel_;
	// 弾の発射間隔
	float deleyShot_;
	//弾数
	int magazineShot_;
	int magazineMaxShot_;
	float reloadSet_;//弾込めの間隔
	float reloadTime_;
	// 操作：弾発射
	void ProcessShot(void);
	// 自機の弾を発射
	void CreateShot(void);

	//ふっ飛ばし関係
	float jumpTime_;
	VECTOR jumpDir_;

	//ステータス
	int playerId_;	//弾の種類やモデルを決める変数
	int playerHp_;
	int playerMaxHp_;
	//ステータス画像
	int hpGaugeH_;
	int hpFreamH_;
	int shotGaugeH_;
	int shotFreamH_;
	int playerIconH_;

	float damageTime_;
	float invincibleTime_;

	VECTOR rideDamagePos_;
	VECTOR rideAttrckPos_;

    // モデル制御の基本情報
    Transform transform_;


};

