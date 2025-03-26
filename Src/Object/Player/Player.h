#pragma once

#include<vector>
#include <map>
#include <functional>
#include <memory>  // shared_ptr���g�����߂ɕK�v(Controll)

#include "../Common/Transform.h"

//#include "Controller.h"

class AnimationController;
class ShotPlayer;
class Controller;
class DirModel;

class Camera;

class Player
{
public:

	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 0.1f;

	// �X�s�[�h
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float ROT_POW = 3.25f;


	// �e�̔��ˊԊu
	static constexpr float TIME_DELAY_SHOT = 0.2f;

	//���a
	static constexpr float ATTRCK_RADIUS = 25.0f;
	static constexpr float DAMAGE_RADIUS = 35.0f;
	
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

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
	};

    Player(Camera& camera);  // �R���X�g���N�^
    ~Player(); // �f�X�g���N�^

    void Init(VECTOR startPos,int playerNo);  // ������
    void Update();  // �X�V
    void Draw();  // �`��

	//�v���C���[��Transform���
	const Transform& GetTransform(void) const { return transform_; }
	const int& GetModelId(void) const { return transform_.modelId; }

	const STATE& GetState(void) const { return state_; }

	const int& GetMaxHp(void) const { return playerMaxHp_; }
	const int& GetHp(void) const { return playerHp_; }
	const void Damage(int damage);
	const void SetJump(VECTOR vec);
	VECTOR& GetPos(int id);

	// �e
	const std::vector<ShotPlayer*>& GetShots(void) const { return shots_; }


private:
	//���g�̃J����
	Camera& camera_;


	std::shared_ptr<Controller> controller_;
	std::shared_ptr<DirModel> dirModel_;

	// ��ԊǗ�
	STATE state_;

	// ��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void(void)>> stateChanges_;
	// ��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)> stateUpdate_;


	//�����G�t�F�N�g
	int effectDestroyResId_;
	int effectDestroyPlayId_;

	// �A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;
	//�A�j���[�V�����̏�����
	void InitAnimation(std::string path);

	// �G�t�F�N�g������
	void InitEffect(void);
	// �G�t�F�N�g����
	void SyncJetEffect(void);
	void SyncBoostEffct(void);

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

	float damageTime_;
	float invincibleTime_;

	VECTOR rideDamagePos_;
	VECTOR rideAttrckPos_;

    // ���f������̊�{���
    Transform transform_;

	// ���_���
	VERTEX3D vertexs_[6];
	// �`��p�̎l�p���_���쐬
	void MakeSquereVertex(void);

};

