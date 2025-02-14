#pragma once
#include "SceneBase.h"

class SelectScene : public SceneBase
{
public:
	static constexpr int PLAYER_MAX = 2;
	static constexpr int SIZE = 20;

	// コンストラクタ
	SelectScene(void);

	// デストラクタ
	~SelectScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	void GetMove(VECTOR& P1, VECTOR& P2);

	void Collision(void);


private:
	VECTOR pos[PLAYER_MAX];

	bool startP1;
	bool startP2;

};
