#pragma once
#include<memory>  // shared_ptr���g�����߂ɕK�vc
#include<vector>

#include "SceneBase.h"
#include "../Manager/SceneManager.h"

class Player;
class Stage;
class Grid;
class Camera;


class GameScene : public SceneBase
{
public:

	static constexpr float BLAST_SCALE = 30.0f;


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

	bool eventFlag_;
	int eventId_;

	std::vector<std::shared_ptr<Player>> players_;
	Camera* camera_[SceneManager::PLAYER_SIZE];
	int screenH[SceneManager::PLAYER_SIZE];

	//�w�i�C���X�^���X
	Stage* stage_;
	//Stage* backGround_;


	//�����G�t�F�N�g
	int effectHitResId_;
	int effectHitPlayId_;


	float hitStop_;


	void Collision(void);

	void GameDraw(void);
	void EventDraw(void);

	//�f�o�b�O�p�̕`��
	void DrawDebug(void);

};
