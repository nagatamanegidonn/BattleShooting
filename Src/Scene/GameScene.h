#pragma once
#include<memory>  // shared_ptrを使うために必要c
#include<vector>

#include "SceneBase.h"
#include "../Manager/SceneManager.h"

class Player;
class Stage;
class Grid;
class Camera;


class GameScene : public SceneBase
{
public:

	static constexpr float BLAST_SCALE = 30.0f;


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

	std::vector<std::shared_ptr<Player>> players_;
	Camera* camera_[SceneManager::PLAYER_SIZE];
	int screenH[SceneManager::PLAYER_SIZE];

	//背景インスタンス
	Stage* stage_;
	//Stage* backGround_;


	//爆発エフェクト
	int effectHitResId_;
	int effectHitPlayId_;


	float hitStop_;


	void Collision(void);

	void GameDraw(void);
	void EventDraw(void);

	//デバッグ用の描画
	void DrawDebug(void);

};
