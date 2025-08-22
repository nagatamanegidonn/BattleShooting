#pragma once
#include<memory>  // shared_ptrを使うために必要c
#include<vector>

#include "SceneBase.h"
#include "../Manager/SceneManager.h"

class Player;
class Stage;
class Camera;


class GameScene : public SceneBase
{
public:

	static constexpr float BLAST_SCALE = 90.0f;


	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void AsyncPreLoad(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	bool eventFlag_;
	int eventId_;

	std::vector<std::unique_ptr<Player>> players_;
	Camera* camera_[BattleManager::PLAYER_SIZE];
	int screenH[BattleManager::PLAYER_SIZE];

	//背景インスタンス
	Stage* stage_;


	//爆発エフェクト
	int effectHitResId_;
	int effectHitPlayId_;


	float hitStop_;

	//勝敗関係
	int winnerImg_;;
	bool isVictory_;
	float vTime_;
	float goalRot_;

	void Collision(void);

	void GameDraw(void);
	void EventDraw(void);

	//デバッグ用の描画
	void DrawDebug(void);

	void PlayImpacEffect(const VECTOR& cPos);
};
