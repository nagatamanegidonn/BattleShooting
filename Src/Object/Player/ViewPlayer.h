
#pragma once

#include<vector>
#include <map>
#include <functional>
#include <memory>  // shared_ptr���g�����߂ɕK�v(Controll)

#include "../Common/Transform.h"


class AnimationController;

class ViewPlayer
{
public:

	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 0.1f;

	// �X�s�[�h
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float ROT_POW = 3.25f;


	// �e�̔��ˊԊu
	static constexpr float TIME_DELAY_SHOT = 0.2f;
	static constexpr float TIME_RELOAD = 2.0f;

	//���a
	static constexpr float ATTRCK_RADIUS = 25.0f;
	static constexpr float DAMAGE_RADIUS = 35.0f;


	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		END
	};

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
	};

	//Player(Camera& camera);  // �R���X�g���N�^
	ViewPlayer();  // �R���X�g���N�^
	~ViewPlayer(); // �f�X�g���N�^

	void Init(VECTOR startPos, int playerNo, int pryId);  // ������
	void Update();  // �X�V
	void Draw();  // �`��

	// ��ԑJ��
	void ChangeState(STATE state);
	
	
private:
	
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

	// ��ԑJ��
	void ChangeStateNone(void);
	void ChangeStatePlay(void);
	void ChangeStateEnd(void);

	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdatePlay(void);
	void UpdateEnd(void);

	//����F��]
	void Turn(VECTOR axis);


	// ���f������̊�{���
	Transform transform_;


};

