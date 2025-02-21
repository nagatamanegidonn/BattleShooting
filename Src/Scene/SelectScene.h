#pragma once
#include "SceneBase.h"

class SelectScene : public SceneBase
{
public:
	static constexpr int PLAYER_MAX = 2;
	static constexpr int SIZE = 20;
	static constexpr int MOVE = 4;

	// コンストラクタ
	SelectScene(void);

	// デストラクタ
	~SelectScene(void);

	void AsyncPreLoad(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	void GetMove(VECTOR& P1, VECTOR& P2);

	void Collision(void);


private:
	VECTOR pos[PLAYER_MAX];

	bool start[PLAYER_MAX];

};
