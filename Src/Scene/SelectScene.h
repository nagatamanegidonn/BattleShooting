#pragma once
#include "SceneBase.h"

class SelectScene : public SceneBase
{
public:
	static constexpr int PLAYER_MAX = 2;		// �v���C���[�̍ő吔
	static constexpr int SIZE = 20;				// �J�[�\���̔��a
	static constexpr int MOVE = 4;				// �J�[�\���̈ړ����x
	static constexpr int CHARACTER_MAX = 2;

	enum CHARA
	{
		E_CHARA_NON,
		E_CHARA1,
		E_CHARA2,
	};


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

	// �����蔻��
	void Collision(void);


private:

	int chara[PLAYER_MAX];

	// �J�[�\�����W
	VECTOR pos[PLAYER_MAX];

	// �L�����N�^�[�I�����������Ă��邩�ǂ����̊m�F�p
	bool start[PLAYER_MAX];

};
