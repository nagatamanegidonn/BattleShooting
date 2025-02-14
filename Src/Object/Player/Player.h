#pragma once

#include<vector>
#include <map>
#include <functional>
#include <memory>  // shared_ptr���g�����߂ɕK�v(Controll)

#include "../Common/Transform.h"

//#include "Controller.h"

class ShotPlayer;
class Controller;

class Player
{
public:

	// �X�s�[�h
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float ROT_POW = 3.0f;


	// �e�̔��ˊԊu
	static constexpr float TIME_DELAY_SHOT = 0.2f;


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

    void Init(VECTOR startPos,int playerNo);  // ������
    void Update();  // �X�V
    void Draw();  // �`��

	//�v���C���[��Transform���
	const Transform& GetTransform(void) const { return transform_; }
	// �e
	const std::vector<ShotPlayer*>& GetShots(void) const { return shots_; }


private:

	std::shared_ptr<Controller> controller_;

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


	VECTOR direction_;
	//����F��]
	void ProcessTurn(void);
	void Turn(VECTOR axis);

	// ����F�ړ��u�[�X�g
	void ProcessBoost(void);


	// �e
	std::vector<ShotPlayer*> shots_;
	// �e�̔��ˊԊu
	float deleyShot_;
	// ����F�e����
	void ProcessShot(void);
	// ���@�̒e�𔭎�
	void CreateShot(void);


    // ���f������̊�{���
    Transform transform_;


};

