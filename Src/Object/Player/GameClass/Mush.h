#pragma once
#include "Player.h"

class Mush :
    public Player
{
public:

	// コンストラクタ
	Mush(void);
	// デストラクタ
	~Mush(void);

protected:

	// パラメータ設定
	void SetParam(void) override;



};

