#pragma once
#include "ShotBase.h"

class ShotPlayer : public ShotBase
{
public:

	// �R���X�g���N�^
	ShotPlayer(void);
	// �f�X�g���N�^
	~ShotPlayer(void);

protected:

	// �p�����[�^�ݒ�
	void SetParam(void) override;


};