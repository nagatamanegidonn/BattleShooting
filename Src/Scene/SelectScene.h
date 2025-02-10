#pragma once
#include "SceneBase.h"

class SelectScene : public SceneBase
{
public:

	// コンストラクタ
	SelectScene(void);

	// デストラクタ
	~SelectScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

};
