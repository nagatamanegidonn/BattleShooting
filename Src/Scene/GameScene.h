#pragma once
#include <memory>  // shared_ptr���g�����߂ɕK�v
#include<vector>

#include "SceneBase.h"

class Player;
class BackGround;
class Grid;
class Camera;


class GameScene : public SceneBase
{
public:

	static constexpr int PLAYER_SIZE = 2;


	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void AsyncPreLoad(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:


	std::vector<std::shared_ptr<Player>> players_;
	Camera* camera_[PLAYER_SIZE];
	int screenH[PLAYER_SIZE];

	Grid* stage_;
	
	//�w�i�C���X�^���X
	BackGround* backGround_;


	float hitStop_;


	void Collision(void);

	//�f�o�b�O�p�̕`��
	void DrawDebug(void);

	int* handleIds_;
	int* handleIds_2;
};
