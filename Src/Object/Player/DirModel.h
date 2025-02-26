#pragma once


#include "../Common/Transform.h"

class DirModel
{
public:

	DirModel();  // コンストラクタ
	~DirModel(); // デストラクタ

	void Init(int i);  // 初期化
	void Update();  // 更新
	void Draw();  // 描画

	void SyncModel(VECTOR setPos, Quaternion Qut);

private:
	Transform transform_;

};

