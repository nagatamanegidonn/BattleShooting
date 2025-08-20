#pragma once
#include <map>
#include <functional>
#include <memory>  // shared_ptrを使うために必要(Controll)
#include <DxLib.h>

#include "../../Scene/SelectScene.h"

class InputController;

class SelectPlayer
{
public:

	static constexpr int SIZE = 20;				// カーソルの半径
	static constexpr int MOVE = 4;				// カーソルの移動速度

	SelectPlayer();  // コンストラクタ
	~SelectPlayer(); // デストラクタ

	void Init(const VECTOR startPos, int playerNo, int pryId);  // 初期化
	void Update();  // 更新
	void Draw();  // 描画

	const VECTOR GetPos(void)const { return cursorPos_; }
	const bool IsReady(void)const { return isReady_; }

private:

	//コントローラ
	std::unique_ptr<InputController> inputController_;

	int selectCharId_ = -1;
	int charImg_[BattleManager::CHARACTER_MAX];

	// カーソル座標
	VECTOR cursorPos_;
	int CursorImg_;

	//
	int playerNo_;
	int chrColor_;
	bool isReady_;

	//更新処理
	std::function<void(void)> selectUpdate_;
	void CursorUpdate(void);
	void StartUpdate(void);

	//描画処理
	VECTOR DrawPos_;
	bool DrawDir_;
	//std::function<void(void)> selectDraw_;
};

