#pragma once
#include "SceneBase.h"

class SelectScene : public SceneBase
{
public:
	static constexpr int PLAYER_MAX = 2;		// プレイヤーの最大数
	static constexpr int SIZE = 20;				// カーソルの半径
	static constexpr int MOVE = 6;				// カーソルの移動速度
	static constexpr int CHARACTER_MAX = 2;		// キャラの最大数

	enum CHARA
	{
		E_CHARA_NON,
		E_CHARA1,
		E_CHARA2,
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

	// カーソルの移動処理
	void GetMove(VECTOR& P1, VECTOR& P2);

	// 当たり判定
	void Collision(void);

	CHARA GetP1Character(void) { return chara[0]; }
	CHARA GetP2Character(void) { return chara[1]; }

private:

	CHARA chara[PLAYER_MAX];

	// カーソル座標
	VECTOR kPos[PLAYER_MAX];

	VECTOR pPos[PLAYER_MAX];

	// キャラクター選択が完了しているかどうかの確認用
	bool start[PLAYER_MAX];

};
