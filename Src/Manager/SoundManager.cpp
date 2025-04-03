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
	soundMap_.emplace(SRC::RULE_BGM, res);

	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "GameBGM.mp3");
	//res.ChengeMaxVolume(0.7);
	soundMap_.emplace(SRC::GAME_BGM, res);
	
	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "BttleBGM.mp3");
	res.ChengeMaxVolume(0.9);
	soundMap_.emplace(SRC::BTTLE_BGM, res);

	
	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "VictoryBGM.mp3");
	res.ChengeMaxVolume(0.9);
	soundMap_.emplace(SRC::VICTORY_BGM , res);
	


	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "Attrck.mp3");
	soundMap_.emplace(SRC::ATTRCK, res);

	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "Damege.mp3");
	res.ChengeMaxVolume(1.3);
	soundMap_.emplace(SRC::DAMAGE , res);



	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "Keen.mp3");
	res.ChengeMaxVolume(2.0);
	soundMap_.emplace(SRC::KEEN, res);


	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "WinBGM.mp3");
	soundMap_.emplace(SRC::WIN_BGM, res);

	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "DrawBGM.mp3");
	soundMap_.emplace(SRC::DRAW_BGM, res);

	res = Sound(Sound::TYPE::SOUND_2D, Application::PATH_SOUND + "LoseBGM.mp3");
	soundMap_.emplace(SRC::LOSE_BGM, res);

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
bool SoundManager::Play(SRC src, Sound::TIMES times)
{
	const auto& lPair = soundMap_.find(src);
	if (lPair != soundMap_.end())
	{
		if (!lPair->second.CheckLoad())
		{
			lPair->second.Load();
		}
		return lPair->second.Play(times);
	}
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

bool SoundManager::CheckMove(SRC src)
{
	const auto& lPair = soundMap_.find(src);
	if (lPair != soundMap_.end())
	{
		return lPair->second.CheckMove();
	}
}

void SoundManager::ChengeVolume(SRC src, float per)
{
	const auto& lPair = soundMap_.find(src);
	if (lPair != soundMap_.end())
	{
		return lPair->second.ChengeVolume(per);
	}
}
