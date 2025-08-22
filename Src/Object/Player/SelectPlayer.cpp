
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
	DeleteGraph(freamImg_);
	for (int i = 0; i < BattleManager::CHARACTER_MAX; i++)
	{
		DeleteGraph(charImg_[i]);
	}


	for (int i = 0; i < 2; i++)
	{
		DeleteGraph(padImgs_[i]);
		DeleteGraph(keyImgs_[i]);
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


	freamImg_ = LoadGraph((Application::PATH_IMAGE + "SelectScene/Fream.png").c_str());

	padImgs_[0] = LoadGraph((Application::PATH_IMAGE + "SelectScene/xbox_button_color_y.png").c_str());
	padImgs_[1] = LoadGraph((Application::PATH_IMAGE + "SelectScene/xbox_button_color_b.png").c_str());


	//１番目のプレイヤー
	if (playerNo_ == 0)
	{
		DrawPos_ = { Application::SCREEN_SIZE_X / 5 ,(Application::SCREEN_SIZE_Y / 4) * 3 ,0.0f};
		DrawDir_ = false;

		keyImgs_[0] = LoadGraph((Application::PATH_IMAGE + "SelectScene/keyboard_q.png").c_str());
		keyImgs_[1] = LoadGraph((Application::PATH_IMAGE + "SelectScene/keyboard_space.png").c_str());

	}
	//２番目のプレイヤー
	else if (playerNo_ == 1)
	{
		DrawPos_ = { Application::SCREEN_SIZE_X - (Application::SCREEN_SIZE_X / 5) ,(Application::SCREEN_SIZE_Y / 4) * 3 ,0.0f };
		DrawDir_ = true;

		keyImgs_[0] = LoadGraph((Application::PATH_IMAGE + "SelectScene/keyboard_slash_back.png").c_str());
		keyImgs_[1] = LoadGraph((Application::PATH_IMAGE + "SelectScene/keyboard_shift.png").c_str());

	}

	//コントローラーの登録
	inputController_ = std::make_unique<InputController>(playerNo + 1);
	inputController_->SetConrolPlayer(playerNo);

	//カーソル
	cursorPos_ = startPos;

	selectDraw_ = std::bind(&SelectPlayer::DrawKeyBord, this);

}

void SelectPlayer::Update()
{
	inputController_->Update();

	selectUpdate_();

	if (inputController_->IsPeripheralTriggered(InputController::PeripheralType::GAME_PAD)
		|| inputController_->IsPeripheralTriggered(InputController::PeripheralType::GAME_PAD_INS)
		|| inputController_->IsPeripheralTriggered(InputController::PeripheralType::GAME_PAD_STICK)
		)
	{
		selectDraw_ = std::bind(&SelectPlayer::DrawPad, this);
	}
	else if(inputController_->IsPeripheralTriggered(InputController::PeripheralType::KEYBOARD))
	{
		selectDraw_ = std::bind(&SelectPlayer::DrawKeyBord, this);
	}

}

void SelectPlayer::Draw()
{
	DrawRotaGraph(DrawPos_.x, DrawPos_.y
		, 0.3f, 0.0f, freamImg_, true, DrawDir_);

	if (BattleManager::GetInstance().GetCharId(playerNo_) != -1)
	{
		DrawRotaGraph(DrawPos_.x, DrawPos_.y
			, 0.3f, 0.0f, charImg_[BattleManager::GetInstance().GetCharId(playerNo_)], true, DrawDir_);

	}

	selectDraw_();

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

	if (cursorPos_.x < static_cast<float>(Application::SCREEN_SIZE_X) / 2 &&
		cursorPos_.x > 0  &&
		cursorPos_.y < static_cast<float>(Application::SCREEN_SIZE_Y) / 2 &&
		cursorPos_.y > 0)
	{
		BattleSns.SetCharcterId(playerNo_, 0 + BattleSns.GetColorId(playerNo_));
	}
	else if (cursorPos_.x < Application::SCREEN_SIZE_X &&
		cursorPos_.x > static_cast<float>(Application::SCREEN_SIZE_X) / 2 &&
		cursorPos_.y < static_cast<float>(Application::SCREEN_SIZE_Y) / 2 &&
		cursorPos_.y > 0)
	{
		BattleSns.SetCharcterId(playerNo_, 2 + BattleSns.GetColorId(playerNo_));
	}
	else
	{
		BattleSns.SetCharcterId(playerNo_, -1);
	}


	if (inputController_->IsTriggered(InputController::KEY::OK) && BattleManager::GetInstance().GetCharId(playerNo_) != -1)
	{
		isReady_ = true;
		selectUpdate_ = std::bind(&SelectPlayer::StartUpdate, this);
	}
}
void SelectPlayer::StartUpdate(void)
{
	if (inputController_->IsTriggered(InputController::KEY::BACK) && BattleManager::GetInstance().GetCharId(playerNo_) != -1)
	{
		isReady_ = false;
		selectUpdate_ = std::bind(&SelectPlayer::CursorUpdate, this);
	}
	//BattleManagerの状態によってはここから
}

void SelectPlayer::DrawKeyBord(void)
{
	int plyNum = -1;//右か左か
	
	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	int cx2 = Application::SCREEN_SIZE_X / 2;

	if (playerNo_ == 0)
	{
		plyNum = -1;
	}
	else
	{
		plyNum = 1;
	}
	//プレーヤーアイコンの描画
	DrawRotaGraph(cx + (plyNum * 120), 400 + 25, 1.0f, 0.0f, keyImgs_[1], true);
	DrawRotaGraph(cx + (plyNum * 120), 475 + 25, 1.0f, 0.0f, keyImgs_[0], true);

}

void SelectPlayer::DrawPad(void)
{
	int plyNum = -1;//右か左か

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	int cx2 = Application::SCREEN_SIZE_X / 2;

	if (playerNo_ == 0)
	{
		plyNum = -1;
	}
	else
	{
		plyNum = 1;
	}
	//プレーヤーアイコンの描画
	DrawRotaGraph(cx + (plyNum * 120), 400 + 25, 1.0f, 0.0f, padImgs_[1], true);
	DrawRotaGraph(cx + (plyNum * 120), 475 + 25, 1.0f, 0.0f, padImgs_[0], true);


}
