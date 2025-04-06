#pragma once
#include<memory>  // shared_ptr���g�����߂ɕK�vc

#include "SceneBase.h"

class ViewPlayer;

class SelectScene : public SceneBase
{
public:
	static constexpr int PLAYER_MAX = 2;		// �v���C���[�̍ő吔
	static constexpr int SIZE = 20;				// �J�[�\���̔��a
	static constexpr int MOVE = 4;				// �J�[�\���̈ړ����x
	static constexpr int CHARACTER_MAX = 2;

	
	// �R���X�g���N�^
	SelectScene(void);

	// �f�X�g���N�^
	~SelectScene(void);

	void AsyncPreLoad(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	// �J�[�\���̈ړ�����
	void GetMove(VECTOR& P1, VECTOR& P2);


private:

	std::vector<std::shared_ptr<ViewPlayer>> players_;

	// �J�[�\�����W
	VECTOR pos[PLAYER_MAX];

	// �L�����N�^�[�I�����������Ă��邩�ǂ����̊m�F�p
	bool isReady_[PLAYER_MAX];
	bool isStart_;

	int playerImg_[CHARACTER_MAX];

	int Cursor_;

	// �����蔻��
	void Collision(void);


	void CharacthrSelect(int playerId);
};
