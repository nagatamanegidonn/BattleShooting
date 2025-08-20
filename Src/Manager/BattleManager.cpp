#include "BattleManager.h"


BattleManager* BattleManager::instance_ = nullptr;

void BattleManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new BattleManager();
	}
	instance_->Init();
}

BattleManager& BattleManager::GetInstance(void)
{
	// TODO: return ƒXƒe[ƒgƒƒ“ƒg‚ğ‚±‚±‚É‘}“ü‚µ‚Ü‚·
	return *instance_;
}


BattleManager::BattleManager(void)
{

}


void BattleManager::Init(void)
{
	ResetGame();
}

void BattleManager::Release(void)
{
}

void BattleManager::Destroy(void)
{
}


void BattleManager::ResetGame(void)
{

	for (int i = 0; i < BattleManager::PLAYER_SIZE; i++) {
		charColorId_[i] = 0;
		charcterId_[i] = -1;
	}

	isWinner_ = RESULT::NONE;
}
void BattleManager::SetCharcterId(int i, int setNum)
{
	charcterId_[i] = setNum;
}
void BattleManager::SetColorId(int i, int setNum)
{
	charColorId_[i] = setNum;
}
//Ÿ”sŠÖŒW
void BattleManager::SetResult(RESULT win)
{
	isWinner_ = win;
}
