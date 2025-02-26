#pragma once

#include<vector>
#include <map>
#include <functional>
#include <memory>  // shared_ptr���g�����߂ɕK�v(Controll)

#include "../Common/Transform.h"

//#include "Controller.h"

class ShotPlayer;
class Controller;

class Camera;

class Player
{
public:

	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 0.1f;

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
		JUMP,
		DEAD,
		VICTORY,
		END
	};


    Player(Camera& camera);  // �R���X�g���N�^
    ~Player(); // �f�X�g���N�^

    void Init(VECTOR startPos,int playerNo);  // ������
    void Update();  // �X�V
    void Draw();  // �`��

	//�v���C���[��Transform���
	const Transform& GetTransform(void) const { return transform_; }
	const int& GetRideMaxHp(void) const { return ridesMaxHp_; }
	const int& GetRideHp(void) const { return ridesHp_; }
	const void RideDamage(int damage);
	const void SetJump(VECTOR vec);
	VECTOR& GetPos(int id);

	// �e
	const std::vector<ShotPlayer*>& GetShots(void) const { return shots_; }


private:
	//���g�̃J����
	Camera& camera_;


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
	void ChangeStateJump(void);
	void ChangeStateDead(void);
	void ChangeStateEnd(void);
	void ChangeStateVictory(void);


	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdatePlay(void);
	void UpdateJump(void);
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

	//�ӂ���΂��֌W
	float jumpTime_;
	VECTOR jumpDir_;

	//�X�e�[�^�X
	int playerId_;	//�e�̎�ނ⃂�f�������߂�ϐ�
	int playerHp_;
	int playerMaxHp_;
	int ridesHp_;
	int ridesMaxHp_;

    // ���f������̊�{���
    Transform transform_;

	// ���_���
	VERTEX3D vertexs_[6];
	// �`��p�̎l�p���_���쐬
	void MakeSquereVertex(void);

};

