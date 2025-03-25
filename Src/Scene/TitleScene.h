#pragma once
#include "SceneBase.h"

class Stage;

class TitleScene : public SceneBase
{

public:

	// �R���X�g���N�^
	TitleScene(void);

	// �f�X�g���N�^
	~TitleScene(void);

	void AsyncPreLoad(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// �^�C�g�����S
	int imgTitleLogo_;

	//�w�i�C���X�^���X
	Stage* stage_;

	// ���S�`��
	void DrawLogo(void);

};
