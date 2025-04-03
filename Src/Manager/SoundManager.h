#pragma once
#include<map>
#include"Sound.h"

class SoundManager
{
public:
	//���\�[�X��
	enum class SRC
	{
		TITLE_BGM,		//�^�C�g���V�[����BGM
		RULE_BGM,		//���[���V�[����BGM
		GAME_BGM,		//�Q�[���V�[����BGM
		BTTLE_BGM,		//�Q�[���V�[����BGM

		VICTORY_BGM,

		ATTRCK,
		DAMAGE,

		KEEN,//������

		WIN_BGM,		//����������BGM
		DRAW_BGM,		//���������̎���BGM
		LOSE_BGM,		//����������BGM

		ENTER_SOUND,	//���艹
		SELECT_SOUND,	//�I�����Ă���Ƃ��̌��ʉ�
		DAMAGE_SOUND,	//�_���[�W���󂯂����̌��ʉ�
		HEAL_SOUND,		//�񕜂������̌��ʉ�
		POISON_SOUND,	//�łɂȂ������̌��ʉ�

	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static SoundManager& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	bool Play(SRC src, Sound::TIMES times);	//�񎟌������p
	bool Play(SRC src, Sound::TIMES times, VECTOR pos, float radius);	//�O���������p

	void Stop(SRC src);	//�������~����
	void AllStop(void);	//�������~����

	bool CheckMove(SRC src);

	void ChengeVolume(SRC src, float per);	//per 0.0�`1.0�Ńp�[�Z���g��ݒ肷��
private:
	// �ÓI�C���X�^���X
	static SoundManager* instance_;

	std::map<SRC, Sound> soundMap_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	SoundManager(void) = default;

	// �f�X�g���N�^�����l
	~SoundManager(void) = default;
};
