#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	Resource res;

	// �X�^�[�g���S
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "StartLogo.png");
	resourcesMap_.emplace(SRC::START_LOGO, res);

	// �G���h���S
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "EndLogo.png");
	resourcesMap_.emplace(SRC::END_LOGO, res);

	// �A���[�g
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Alert.png");
	resourcesMap_.emplace(SRC::ALERT_MSG, res);

	// �p�[�e�B�N���G�t�F�N�g�p
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Light.png");
	resourcesMap_.emplace(SRC::LIGHT, res);

	// �X�J�C�h�[��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKYDOME, res);

	
	// �_���W����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Rock/Dungeon.mv1");
	resourcesMap_.emplace(SRC::DUNGEON, res);

	// �����摜
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Alphabet.png", 10, 6, 32, 32);
	resourcesMap_.emplace(SRC::ALPHABET, res);

	// �����o��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SpeechBalloon.png");
	resourcesMap_.emplace(SRC::SPEECH_BALLOON, res);
	
	
	//P1�̃C���[�W�摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "P1MushImage.png");
	resourcesMap_.emplace(SRC::P1_MUSH_IMAGE, res);
	//P2�̃C���[�W�摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "P2MushImage.png");
	resourcesMap_.emplace(SRC::P2_MUSH_IMAGE, res);


	// �e
	//res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Shot/Shot.mv1");
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "P1Mush/P1Bullet.mv1");
	resourcesMap_.emplace(SRC::SHOT_MODEL, res);

	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "P1Mush/P1Bullet.mv1");
	resourcesMap_.emplace(SRC::P1_MUSH_SHOT_MODEL, res);

	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "P2Mush/P2Bullet.mv1");
	resourcesMap_.emplace(SRC::P2_MUSH_SHOT_MODEL, res);


	
	// �w�i��P
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Rock/Rock21.mv1");
	resourcesMap_.emplace(SRC::ROCK01, res);

	// �w�i��Q
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Rock/Rock23.mv1");
	resourcesMap_.emplace(SRC::ROCK02, res);

	//�w�i��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "BackGround/City/City.mv1");
	resourcesMap_.emplace(SRC::BACKGROUNDCITY, res);

	//�w�i�X
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "BackGround/Forest/Forest.mv1");
	resourcesMap_.emplace(SRC::BACKGROUNDFOREST, res);

	// ���@����
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Jet.efkefc");
	resourcesMap_.emplace(SRC::JET, res);

	// ���@�u�[�X�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Boost.efkefc");
	resourcesMap_.emplace(SRC::BOOST, res);

	// ���@�j��
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Explosion.efkefc");
	resourcesMap_.emplace(SRC::PLAYER_SHIP_EXPLOSION, res);

	// �e�̔���
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Blast.efkefc");
	resourcesMap_.emplace(SRC::SHOT_EXPLOSION, res);

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return nullptr;
	}

	rPair->second.Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
