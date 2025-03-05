#pragma once

#include "Common/Transform.h"

class BackGround
{

public:
	//大きさ
	static constexpr float BACKGROUND_SCALE = 2.0f;

	//Y座標初期値
	static constexpr float BACKGROUND_POS_Y = -11300.0f;

	//回転X
	static constexpr float BACKGROUND_ANGLE_X = 180.0f;

	//回転Y
	static constexpr float BACKGROUND_ANGLE_Y = 90.0f;


	// コンストラクタ
	BackGround(void);

	// デストラクタ
	~BackGround(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// モデル制御の基本情報
	Transform transform_;

};