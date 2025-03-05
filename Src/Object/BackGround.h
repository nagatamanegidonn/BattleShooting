#pragma once

#include "Common/Transform.h"

class BackGround
{

public:
	//�傫��
	static constexpr float BACKGROUND_SCALE = 2.0f;

	//Y���W�����l
	static constexpr float BACKGROUND_POS_Y = -11300.0f;

	//��]X
	static constexpr float BACKGROUND_ANGLE_X = 180.0f;

	//��]Y
	static constexpr float BACKGROUND_ANGLE_Y = 90.0f;


	// �R���X�g���N�^
	BackGround(void);

	// �f�X�g���N�^
	~BackGround(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// ���f������̊�{���
	Transform transform_;

};