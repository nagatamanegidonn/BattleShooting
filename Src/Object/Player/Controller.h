#pragma once
#include <map>
#include <vector>

class Controller
{

public:

	// ����̎��
	enum class MODE
	{
		FORWARD,//�O�i
		BACK,	//���
		LEFT,	//���؂�ւ�
		RIGHT,	//�E�؂�ւ�
		ATTACK,	//�U��
		XBUTTUN,	//�����[�h
		JATTACK,
		MAX
	};

	Controller();  // �R���X�g���N�^
	~Controller(); // �f�X�g���N�^

	void Init(int i);  // ������
	void Update();  // �X�V
	void Draw();  // �`��

	void Reset();//���͂����Z�b�g

	void SetisControl(MODE mode, bool isBool);
	bool GetisControl(MODE mode);

private:

	//�R���g���[���֌W
	//std::map<MODE, std::vector<bool>> isControl_;
	bool isControl_[static_cast<int>(MODE::MAX)];

	int modeId_;
	void IsCommnd(void);
};

