#pragma once
#include <memory>  // shared_ptr���g�����߂ɕK�v
#include<vector>

#include "SceneBase.h"

class Player;

class GameScene : public SceneBase
{
public:

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	std::vector < std::shared_ptr<Player>> players_;


	//�f�o�b�O�p�̕`��
	void DrawDebug(void);

};
