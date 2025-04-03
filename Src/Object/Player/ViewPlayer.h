
#pragma once

#include<vector>
#include <map>
#include <functional>
#include <memory>  // shared_ptrを使うために必要(Controll)

#include "../Common/Transform.h"


class AnimationController;

class ViewPlayer
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


	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
		END
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
	};

	//Player(Camera& camera);  // コンストラクタ
	ViewPlayer();  // コンストラクタ
	~ViewPlayer(); // デストラクタ

	void Init(VECTOR startPos, int playerNo, int pryId);  // 初期化
	void Update();  // 更新
	void Draw();  // 描画

	// 状態遷移
	void ChangeState(STATE state);
	
	
private:
	
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

	// 状態遷移
	void ChangeStateNone(void);
	void ChangeStatePlay(void);
	void ChangeStateEnd(void);

	// 更新ステップ
	void UpdateNone(void);
	void UpdatePlay(void);
	void UpdateEnd(void);

	//操作：回転
	void Turn(VECTOR axis);


	// モデル制御の基本情報
	Transform transform_;


};

