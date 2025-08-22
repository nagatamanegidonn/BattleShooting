#pragma once
#include "Player.h"

class Bamboo :
    public Player
{

public:

	static constexpr float ATK_RATE = 2.0f;
	static constexpr float DEF_RATE = 1.0f;

	// コンストラクタ
	Bamboo(void);
	// デストラクタ
	~Bamboo(void);

protected:

	// パラメータ設定
	void SetParam(void) override;



};

