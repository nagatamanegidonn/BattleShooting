#pragma once

#include "Common/Transform.h"

class SpaceDome;

class Stage
{

public:

#pragma region �f�o�b�O�p�萔

	// ���̒���
	static constexpr float LEN = 1200.0f;

	// ���̒����̔���
	static constexpr float HLEN = LEN / 2.0f;

	// ���̊Ԋu
	static constexpr float TERM = 50.0f;

	// ���̐�
	static const int NUM = static_cast<int>(LEN / TERM);

	// ���̐��̔���
	static const int HNUM = NUM / 2;

#pragma endregion

	//�傫��
	//static constexpr float BACKGROUND_SCALE = 10.0f;
	static constexpr float BACKGROUND_SCALE = 7.5f;

	//Y���W�����l
	//static constexpr float BACKGROUND_POS_Y = -1000.0f;
	static constexpr float BACKGROUND_POS_Y = -100.0f;

	//��]X
	static constexpr float BACKGROUND_ANGLE_X = 180.0f;

	//��]Y
	static constexpr float BACKGROUND_ANGLE_Y = 90.0f;

	//���ā[���̔��a
	static constexpr float STAGE_RADIUS = 800.0f;

	// �R���X�g���N�^
	Stage(void);

	// �f�X�g���N�^
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void DrawDebug(void);
	void Release(void);

private:

	// ���f������̊�{���
	Transform transform_;

	SpaceDome* spaceDome_;

};