#pragma once
#include "Player.h"

class Mush :
    public Player
{
public:

	static constexpr float ATK_RATE = 1.5f;
	static constexpr float DEF_RATE = 0.5f;

	// コンストラクタ
	Mush(void);
	// デストラクタ
	~Mush(void);

protected:

	// パラメータ設定
	void SetParam(void) override;



};

