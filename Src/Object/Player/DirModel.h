#pragma once


#include "../Common/Transform.h"

class DirModel
{
public:


	static constexpr FLOAT4 COLOR_CLEAR_RED = { 1.0f, 0.0f, 0.0f, 0.5f };
	static constexpr FLOAT4 COLOR_CLEAR_BULE = { 0.0f, 0.0f, 1.0f, 0.5f };

	DirModel();  // コンストラクタ
	~DirModel(); // デストラクタ

	void Init(int i);  // 初期化
	void Update();  // 更新
	void Draw();  // 描画

	void SyncModel(VECTOR setPos, Quaternion Qut);

private:
	Transform transform_;

};

