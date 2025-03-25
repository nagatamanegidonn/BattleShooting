#pragma once
#include "SceneBase.h"

class Stage;

class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	void AsyncPreLoad(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// タイトルロゴ
	int imgTitleLogo_;

	//背景インスタンス
	Stage* stage_;

	// ロゴ描画
	void DrawLogo(void);

};
