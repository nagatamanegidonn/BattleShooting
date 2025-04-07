#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// ���\�[�X��
	enum class SRC
	{
		TITLE_LOGO,
		START_LOGO,
		PLAY_LOGO,
		EXIT_LOGO,

		CURSOR,
		FRAME,

		ALERT_MSG,
		LIGHT,
		SKYDOME,
		DUNGEON,
		SHOT_EXPLOSION,
		ALPHABET,
		SPEECH_BALLOON,

		P1_IMAGE, // P1�L�m�R�摜
		P2_IMAGE, // P2�L�m�R�摜
		P3_IMAGE, // P3�摜
		P4_IMAGE, // P4�摜

		SHOT_MODEL,
		P1_SHOT_MODEL, // P1�L�m�R�e���f��
		P2_SHOT_MODEL, // P2�L�m�R�e���f��

		ROCK01,
		ROCK02,
		BACKGROUNDCITY,
		BACKGROUNDFOREST,

		JET,
		BOOST,
		PLAYER_SHIP_EXPLOSION,
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	Resource Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, Resource> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource*> loadedMap_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);

	// �f�X�g���N�^�����l
	~ResourceManager(void) = default;

	// �������[�h
	Resource* _Load(SRC src);

};

