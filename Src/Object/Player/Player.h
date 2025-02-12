#pragma once

#include<vector>
#include <map>
#include <functional>
#include "../Common/Transform.h"

//#include "Controller.h"

class Player
{
public:

	// �X�s�[�h
	static constexpr float SPEED_MOVE = 5.0f;

	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		DEAD,
		VICTORY,
		END
	};


    Player();  // �R���X�g���N�^
    ~Player(); // �f�X�g���N�^

    void Init();  // ������
    void Update();  // �X�V
    void Draw();  // �`��

private:

	// ��ԊǗ�
	STATE state_;

	// ��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void(void)>> stateChanges_;
	// ��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)> stateUpdate_;


	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);
	void ChangeStateDead(void);
	void ChangeStateEnd(void);
	void ChangeStateVictory(void);


	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdatePlay(void);
	void UpdateDead(void);
	void UpdateEnd(void);
	void UpdateVictory(void);


	//	�ŏI�I�Ȉړ���
	VECTOR movedPos_;
	VECTOR movePow_;
	//����F�ړ�
	void ProcessMove(void);
	void Move(void);

	//����F��]
	void ProcessTurn(void);
	void Turn(VECTOR axis);

	// ����F�ړ��u�[�X�g
	void ProcessBoost(void);

	// ����F�e����
	//void ProcessShot(void);
	// ���@�̒e���쐬
	//void CreateShot(void);


    // ���f������̊�{���
    Transform transform_;


};

