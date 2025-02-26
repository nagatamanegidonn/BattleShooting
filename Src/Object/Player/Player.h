#pragma once

#include<vector>
#include <map>
#include <functional>
#include <memory>  // shared_ptrを使うために必要(Controll)

#include "../Common/Transform.h"

//#include "Controller.h"

class ShotPlayer;
class Controller;

class Camera;

class Player
{
public:

	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.1f;

	// スピード
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float ROT_POW = 3.0f;


	// 弾の発射間隔
	static constexpr float TIME_DELAY_SHOT = 0.2f;


	
	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
		JUMP,
		DEAD,
		VICTORY,
		END
	};


    Player(Camera& camera);  // コンストラクタ
    ~Player(); // デストラクタ

    void Init(VECTOR startPos,int playerNo);  // 初期化
    void Update();  // 更新
    void Draw();  // 描画

	//プレイヤーのTransform情報
	const Transform& GetTransform(void) const { return transform_; }
	const int& GetRideMaxHp(void) const { return ridesMaxHp_; }
	const int& GetRideHp(void) const { return ridesHp_; }
	const void RideDamage(int damage);
	const void SetJump(VECTOR vec);
	VECTOR& GetPos(int id);

	// 弾
	const std::vector<ShotPlayer*>& GetShots(void) const { return shots_; }


private:
	//自身のカメラ
	Camera& camera_;


	std::shared_ptr<Controller> controller_;

	// 状態管理
	STATE state_;

	// 状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void(void)>> stateChanges_;
	// 状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);
	void ChangeStateJump(void);
	void ChangeStateDead(void);
	void ChangeStateEnd(void);
	void ChangeStateVictory(void);


	// 更新ステップ
	void UpdateNone(void);
	void UpdatePlay(void);
	void UpdateJump(void);
	void UpdateDead(void);
	void UpdateEnd(void);
	void UpdateVictory(void);


	//	最終的な移動量
	VECTOR movedPos_;
	VECTOR movePow_;
	//操作：移動
	void ProcessMove(void);
	void Move(void);


	VECTOR direction_;
	//操作：回転
	void ProcessTurn(void);
	void Turn(VECTOR axis);


	// 操作：移動ブースト
	void ProcessBoost(void);


	// 弾
	std::vector<ShotPlayer*> shots_;
	// 弾の発射間隔
	float deleyShot_;
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
	int ridesHp_;
	int ridesMaxHp_;

    // モデル制御の基本情報
    Transform transform_;

	// 頂点情報
	VERTEX3D vertexs_[6];
	// 描画用の四角頂点を作成
	void MakeSquereVertex(void);

};

