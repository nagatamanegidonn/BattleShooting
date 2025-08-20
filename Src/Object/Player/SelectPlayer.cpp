
#include "../../Manager/ResourceManager.h"
#include "../../Manager/BattleManager.h"

#include "../../Application.h"

#include "../Common/InputController.h"

#include "SelectPlayer.h"

SelectPlayer::SelectPlayer()
{
	selectUpdate_ = std::bind(&SelectPlayer::CursorUpdate, this);

	isReady_ = false;
}

SelectPlayer::~SelectPlayer()
{
	DeleteGraph(CursorImg_);
	for (int i = 0; i < BattleManager::CHARACTER_MAX; i++)
	{
		DeleteGraph(charImg_[i]);
	}
}

void SelectPlayer::Init(const VECTOR startPos, int playerNo, int pryId)
{
	playerNo_ = playerNo;
	chrColor_ = BattleManager::GetInstance().GetColorId(playerNo_);

	CursorImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CURSOR).handleId_;
	charImg_[0] = ResourceManager::GetInstance().Load(ResourceManager::SRC::P1_IMAGE).handleId_;
	charImg_[1] = ResourceManager::GetInstance().Load(ResourceManager::SRC::P2_IMAGE).handleId_;
	charImg_[2] = ResourceManager::GetInstance().Load(ResourceManager::SRC::P3_IMAGE).handleId_;
	charImg_[3] = ResourceManager::GetInstance().Load(ResourceManager::SRC::P4_IMAGE).handleId_;


	//１番目のプレイヤー
	if (playerNo_ == 0)
	{
		DrawPos_ = { Application::SCREEN_SIZE_X / 4 ,(Application::SCREEN_SIZE_Y / 4) * 3 ,0.0f};
		DrawDir_ = false;
	}
	//２番目のプレイヤー
	else if (playerNo_ == 1)
	{
		DrawPos_ = { (Application::SCREEN_SIZE_X / 4) * 3 ,(Application::SCREEN_SIZE_Y / 4) * 3 ,0.0f };
		DrawDir_ = true;
	}

	//コントローラーの登録
	inputController_ = std::make_unique<InputController>(playerNo + 1);
	inputController_->SetConrolPlayer(playerNo);

	//カーソル
	cursorPos_ = startPos;
}

void SelectPlayer::Update()
{
	inputController_->Update();

	selectUpdate_();
}

void SelectPlayer::Draw()
{
	if (BattleManager::GetInstance().GetCharId(playerNo_) != -1)
	{
		DrawRotaGraph(DrawPos_.x, DrawPos_.y
			, 0.3f, 0.0f, charImg_[BattleManager::GetInstance().GetCharId(playerNo_)], true, DrawDir_);
	}

	if (!isReady_)
	{
		DrawRotaGraph(cursorPos_.x, cursorPos_.y, 1.0f, 0.0f, CursorImg_, true);
	}
}

void SelectPlayer::CursorUpdate(void)
{
	if (inputController_->IsNew(InputController::KEY::N_UP))
	{
		cursorPos_.y -= MOVE;
	}
	if (inputController_->IsNew(InputController::KEY::N_DOWN))
	{
		cursorPos_.y += MOVE;
	}
	if (inputController_->IsNew(InputController::KEY::N_RIGHT))
	{
		cursorPos_.x += MOVE;
	}
	if (inputController_->IsNew(InputController::KEY::N_LEFT))
	{
		cursorPos_.x -= MOVE;
	}

	// 左
	if (cursorPos_.x - SIZE < 0)
	{
		cursorPos_.x += MOVE;
	}

	// 右
	if (cursorPos_.x + SIZE > Application::SCREEN_SIZE_X)
	{
		cursorPos_.x -= MOVE;
	}

	// 上
	if (cursorPos_.y - SIZE < 0)
	{
		cursorPos_.y += MOVE;
	}

	// 下
	if (cursorPos_.y + SIZE > Application::SCREEN_SIZE_Y)
	{
		cursorPos_.y -= MOVE;
	}

	BattleManager& BattleSns = BattleManager::GetInstance();
	
	if (inputController_->IsTriggered(InputController::KEY::N_CHANGE))
	{
		chrColor_ = (chrColor_ + 1) % BattleManager::COLOR_SIZE;
		BattleSns.SetColorId(playerNo_, chrColor_);
	}

	if (cursorPos_.x < Application::SCREEN_SIZE_X / 2 &&
		cursorPos_.x > 0  &&
		cursorPos_.y < Application::SCREEN_SIZE_Y / 2 &&
		cursorPos_.y > 0)
	{
		BattleSns.SetCharcterId(playerNo_, 0 + BattleSns.GetColorId(playerNo_));
	}
	else if (cursorPos_.x < Application::SCREEN_SIZE_X &&
		cursorPos_.x > Application::SCREEN_SIZE_X / 2 &&
		cursorPos_.y < Application::SCREEN_SIZE_Y / 2 &&
		cursorPos_.y > 0)
	{
		BattleSns.SetCharcterId(playerNo_, 2 + BattleSns.GetColorId(playerNo_));
	}
	else
	{
		BattleSns.SetCharcterId(playerNo_, -1);
	}


	if (inputController_->IsNew(InputController::KEY::OK) && BattleManager::GetInstance().GetCharId(playerNo_) != -1)
	{
		isReady_ = true;
		selectUpdate_ = std::bind(&SelectPlayer::StartUpdate, this);
	}
}
void SelectPlayer::StartUpdate(void)
{
	//BattleManagerの状態によってはここから
}
