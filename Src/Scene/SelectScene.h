#pragma once
#include<memory>  // shared_ptrを使うために必要c

#include "../Application.h"
#include "../Manager/BattleManager.h"

#include "SceneBase.h"

class ViewPlayer;
class SelectPlayer;

class SelectScene : public SceneBase
{
public:

	static constexpr float LengeSpaceX = Application::SCREEN_SIZE_X / 3;
	static constexpr float LengeSpaceY = Application::SCREEN_SIZE_Y / 3;


	static constexpr float DISTANCE = 500.0f;

	//プレイヤーの設定
	static constexpr VECTOR VIEW_STATRT_POS[4] = {
		{-DISTANCE,0.0f,DISTANCE / 2}			//左上
		,{DISTANCE,0.0f,DISTANCE / 2 + 70.0f}	//右上
		,{-DISTANCE,0.0f,-DISTANCE}				//左下
		,{DISTANCE,0.0f,-DISTANCE}				//右上 
	};

	static constexpr VECTOR cursorStartPos[BattleManager::PLAYER_SIZE] = {
		{LengeSpaceX,LengeSpaceY ,0.0f},
		{LengeSpaceX * 2,LengeSpaceY ,0.0f}
	};
	
	// コンストラクタ
	SelectScene(void);

	// デストラクタ
	~SelectScene(void);

	void AsyncPreLoad(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;


private:

	std::vector<std::unique_ptr<ViewPlayer>> playerViews_;
	std::unique_ptr<SelectPlayer> players_[BattleManager::PLAYER_SIZE];

	// カーソル座標
	bool isStart_;


	int CursorImg_;
	int FrameImg_;

	int startImg_;
	
};
