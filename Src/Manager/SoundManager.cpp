#include <DxLib.h>
#include "../Application.h"
#include "SoundManager.h"

SoundManager* SoundManager::instance_ = nullptr;

void SoundManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new SoundManager();
	}
	instance_->Init();
}

SoundManager& SoundManager::GetInstance(void)
{
	return *instance_;
}

void SoundManager::Init(void)
{
	Sound res;

	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "TitleBGM.mp3");
	res.ChengeMaxVolume(0.9);
	soundMap_.emplace(SRC::TITLE_BGM, res);

	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "SelectBGM.mp3");
	res.ChengeMaxVolume(1.1);
	soundMap_.emplace(SRC::SELECT_BGM, res);

	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "GameBGM1.mp3");
	res.ChengeMaxVolume(0.6);
	soundMap_.emplace(SRC::BTTLE_BGM_1, res);
	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "GameBGM2.mp3");
	res.ChengeMaxVolume(0.6);
	soundMap_.emplace(SRC::BTTLE_BGM_2, res);
	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "GameBGM3.mp3");
	res.ChengeMaxVolume(0.6);
	soundMap_.emplace(SRC::BTTLE_BGM_3, res);

	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "GameBGM.mp3");
	//res.ChengeMaxVolume(0.7);
	soundMap_.emplace(SRC::RESULT_BGM, res);


	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "SE/Lobot_Hit.mp3");
	res.ChengeMaxVolume(1.1);
	soundMap_.emplace(SRC::IMPACT, res);


	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "SE/shot.mp3");
	res.ChengeMaxVolume(0.9);
	soundMap_.emplace(SRC::ATTRCK, res);

	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "SE/Damege.mp3");
	res.ChengeMaxVolume(1.1);
	soundMap_.emplace(SRC::DAMAGE, res);

	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "SE/Reload.mp3");
	res.ChengeMaxVolume(1.1);
	soundMap_.emplace(SRC::RELOAD, res);
	
	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "SE/MoveStrat.mp3");
	res.ChengeMaxVolume(1.1);
	soundMap_.emplace(SRC::DASH, res);
	
	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "SE/TurnMove.mp3");
	res.ChengeMaxVolume(1.1);
	soundMap_.emplace(SRC::TURN, res);
	
	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "SE/Fall.mp3");
	res.ChengeMaxVolume(1.1);
	soundMap_.emplace(SRC::FALL, res);

	//res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "TitleBGM.mp3");
	//soundMap_.emplace(SRC::TITLE_BGM, res);
}

void SoundManager::Release(void)
{
	for (auto& p : soundMap_)
	{
		p.second.Release();  // �T�E���h�̃��\�[�X�����

		// p.second �����I�Ɋm�ۂ��ꂽ�I�u�W�F�N�g�ł���΁A�ȉ��̂悤�Ƀ��������
		// delete p.second; // �K�v�ɉ����āA���������������

	}

	soundMap_.clear();  // �}�b�v���N���A
}

void SoundManager::Destroy(void)
{
	Release();
	soundMap_.clear();
	delete instance_;
}

//���̍Đ�
bool SoundManager::Play(SRC src, Sound::TIMES times, bool isForce)
{
	const auto& lPair = soundMap_.find(src);
	if (lPair != soundMap_.end())
	{
		if (!lPair->second.CheckLoad())
		{
			lPair->second.Load();
		}
		return lPair->second.Play(times, isForce);
	}
	return false;
}
bool SoundManager::Play(SRC src, Sound::TIMES times, VECTOR pos, float radius)
{
	const auto& lPair = soundMap_.find(src);
	if (lPair != soundMap_.end())
	{
		if (!lPair->second.CheckLoad())
		{
			lPair->second.Load();
		}
		return lPair->second.Play(pos, radius, times);
	}
	return false;
}

void SoundManager::Stop(SRC src)
{
	const auto& lPair = soundMap_.find(src);
	if (lPair != soundMap_.end())//�w��̂��̂����݂��邩�H
	{
		return lPair->second.Stop();
	}
}
void SoundManager::AllStop(void)
{
	// soundMap_ �̂��ׂĂ̗v�f�ɑ΂��� Stop() ���Ăяo��
	for (auto& lPair : soundMap_)
	{
		lPair.second.Stop();
	}
}
//�Đ�����?
bool SoundManager::CheckMove(SRC src)
{
	const auto& lPair = soundMap_.find(src);
	if (lPair != soundMap_.end())
	{
		return lPair->second.CheckMove();
	}
	return false;
}
//����
void SoundManager::ChengeVolume(SRC src, float per)
{
	const auto& lPair = soundMap_.find(src);
	if (lPair != soundMap_.end())
	{
		return lPair->second.ChengeVolume(per);
	}
}
