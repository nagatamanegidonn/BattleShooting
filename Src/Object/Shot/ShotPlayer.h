#pragma once
#include "ShotBase.h"

class ShotPlayer : public ShotBase
{
public:

	// コンストラクタ
	ShotPlayer(void);
	// デストラクタ
	~ShotPlayer(void);

protected:

	// パラメータ設定
	void SetParam(void) override;


};