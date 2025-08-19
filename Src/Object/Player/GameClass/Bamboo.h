#pragma once
#include "Player.h"

class Bamboo :
    public Player
{

public:

	// コンストラクタ
	Bamboo(void);
	// デストラクタ
	~Bamboo(void);

protected:

	// パラメータ設定
	void SetParam(void) override;



};

