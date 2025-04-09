#pragma once

#include "../Object/Common/Transform.h"



class SpaceDome
{
public:

	// コンストラクタ
	SpaceDome(const Transform& follow);
	// デストラクタ
	~SpaceDome(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:
	// 追従対象
	const Transform& follow_;

	// モデル制御の基本情報
	Transform transform_;
};
