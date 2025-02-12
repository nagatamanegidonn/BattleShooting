#pragma once
#include <memory>  // shared_ptrを使うために必要
#include<vector>

#include "SceneBase.h"

class Player;

class GameScene : public SceneBase
{
public:

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	std::vector < std::shared_ptr<Player>> players_;


	//デバッグ用の描画
	void DrawDebug(void);

};
