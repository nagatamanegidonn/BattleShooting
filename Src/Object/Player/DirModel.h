#pragma once


#include "../Common/Transform.h"

class DirModel
{
public:

	DirModel();  // �R���X�g���N�^
	~DirModel(); // �f�X�g���N�^

	void Init(int i);  // ������
	void Update();  // �X�V
	void Draw();  // �`��

	void SyncModel(VECTOR setPos, Quaternion Qut);

private:
	Transform transform_;

};

