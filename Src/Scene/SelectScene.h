#pragma once
#include<memory>  // shared_ptrを使うために必要c

#include "SceneBase.h"

class ViewPlayer;

class SelectScene : public SceneBase
{
public:
	static constexpr int PLAYER_MAX = 2;		// プレイヤーの最大数
	static constexpr int SIZE = 20;				// カーソルの半径
	static constexpr int MOVE = 4;				// カーソルの移動速度
	static constexpr int CHARACTER_MAX = 2;

	
	// コンストラクタ
	SelectScene(void);

	// デストラクタ
	~SelectScene(void);

	void AsyncPreLoad(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	// カーソルの移動処理
	void GetMove(VECTOR& P1, VECTOR& P2);


private:

	std::vector<std::shared_ptr<ViewPlayer>> players_;

	// カーソル座標
	VECTOR pos[PLAYER_MAX];

	// キャラクター選択が完了しているかどうかの確認用
	bool isReady_[PLAYER_MAX];
	bool isStart_;

	int playerImg_[CHARACTER_MAX];

	int Cursor_;

	// 当たり判定
	void Collision(void);


	void CharacthrSelect(int playerId);
};
