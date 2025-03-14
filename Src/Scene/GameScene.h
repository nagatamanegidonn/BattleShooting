#pragma once
#include <memory>  // shared_ptrを使うために必要
#include<vector>

#include "SceneBase.h"

class Player;
class BackGround;
class Grid;
class Camera;


class GameScene : public SceneBase
{
public:

	static constexpr int PLAYER_SIZE = 2;


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


	std::vector<std::shared_ptr<Player>> players_;
	Camera* camera_[PLAYER_SIZE];
	int screenH[PLAYER_SIZE];

	Grid* stage_;
	
	//背景インスタンス
	BackGround* backGround_;


	float hitStop_;


	void Collision(void);

	//デバッグ用の描画
	void DrawDebug(void);

	int* handleIds_;
	int* handleIds_2;
};
