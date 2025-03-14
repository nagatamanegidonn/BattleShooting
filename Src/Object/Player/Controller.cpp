#include "../../Manager/InputManager.h"

#include "Controller.h"


Controller::Controller()
{
	modeId_ = 0;
}
Controller::~Controller()
{
}

void Controller::Init(int i)
{
	modeId_ = i;

	auto& input = InputManager::GetInstance();

	IsCommnd();
}

void Controller::Update()
{
	auto& input = InputManager::GetInstance();

	IsCommnd();
}

void Controller::Draw()
{
}


void Controller::Reset()
{
}

void Controller::SetisControl(MODE mode, bool isBool)
{
	//isControl_[mode].push_back(isBool);
}

bool Controller::GetisControl(MODE mode)
{
	return isControl_[static_cast<int>(mode)];
}

void Controller::IsCommnd(void)
{
	auto& input = InputManager::GetInstance();

	switch (modeId_)
	{
	case 0:
	{
		InputManager::JOYPAD_NO jno = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD1);

		// 左スティックの横軸
		auto leftStickX = input.GetJPadInputState(jno).AKeyLX;
		// 左スティックの縦軸
		auto leftStickY = input.GetJPadInputState(jno).AKeyLY;

		isControl_[static_cast<int>(MODE::FORWARD)] = ((input.IsNew(KEY_INPUT_W)) || (input.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::RIGHT)));
		isControl_[static_cast<int>(MODE::BACK)] = ((input.IsNew(KEY_INPUT_S)) || (input.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::DOWN)));
		isControl_[static_cast<int>(MODE::RIGHT)] = ((leftStickX > 0.0f) || (input.IsNew(KEY_INPUT_D)) || (input.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::R_BUTTON)));
		isControl_[static_cast<int>(MODE::LEFT)] = ((leftStickX < 0.0f) || (input.IsNew(KEY_INPUT_A)) || (input.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::L_BUTTON)));
		isControl_[static_cast<int>(MODE::ATTACK)] = ((input.IsNew(KEY_INPUT_SPACE)) || (input.IsNew(KEY_INPUT_V)) || (input.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::R_TRIGGER)));
		isControl_[static_cast<int>(MODE::JATTACK)] = ((input.IsNew(KEY_INPUT_F)) || (input.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::L_TRIGGER)));
	}
		break;
	case 1:
	{
		InputManager::JOYPAD_NO jno = static_cast<InputManager::JOYPAD_NO>(InputManager::JOYPAD_NO::PAD2);

		// 左スティックの横軸
		auto leftStickX = input.GetJPadInputState(jno).AKeyLX;
		// 左スティックの縦軸
		auto leftStickY = input.GetJPadInputState(jno).AKeyLY;

		isControl_[static_cast<int>(MODE::FORWARD)] = ((input.IsNew(KEY_INPUT_UP)) || (input.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::RIGHT)));
		isControl_[static_cast<int>(MODE::BACK)] = ((input.IsNew(KEY_INPUT_DOWN)) || (input.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::DOWN)));
		isControl_[static_cast<int>(MODE::RIGHT)] = ((leftStickX > 0.0f) || (input.IsNew(KEY_INPUT_RIGHT)) || (input.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::R_BUTTON)));
		isControl_[static_cast<int>(MODE::LEFT)] = ((leftStickX < 0.0f) || (input.IsNew(KEY_INPUT_LEFT)) || (input.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::L_BUTTON)));
		isControl_[static_cast<int>(MODE::ATTACK)] = ((input.IsNew(KEY_INPUT_RETURN)) || (input.IsNew(KEY_INPUT_V)) || (input.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::R_TRIGGER)));
		isControl_[static_cast<int>(MODE::JATTACK)] = ((input.IsNew(KEY_INPUT_RSHIFT)) || (input.IsPadBtnNew(jno, InputManager::JOYPAD_BTN::L_TRIGGER)));
	}
		break;
	case 2:
		isControl_[static_cast<int>(MODE::FORWARD)] = (input.IsNew(KEY_INPUT_T));
		isControl_[static_cast<int>(MODE::BACK)] = (input.IsNew(KEY_INPUT_G));
		isControl_[static_cast<int>(MODE::RIGHT)] = (input.IsNew(KEY_INPUT_H));
		isControl_[static_cast<int>(MODE::LEFT)] = (input.IsNew(KEY_INPUT_F));
		isControl_[static_cast<int>(MODE::ATTACK)] = ((input.IsNew(KEY_INPUT_Z)) || (input.IsNew(KEY_INPUT_N)));
		break;
	case 3:
		isControl_[static_cast<int>(MODE::FORWARD)] = (input.IsNew(KEY_INPUT_I));
		isControl_[static_cast<int>(MODE::BACK)] = (input.IsNew(KEY_INPUT_K));
		isControl_[static_cast<int>(MODE::RIGHT)] = (input.IsNew(KEY_INPUT_L));
		isControl_[static_cast<int>(MODE::LEFT)] = (input.IsNew(KEY_INPUT_J));
		isControl_[static_cast<int>(MODE::ATTACK)] = ((input.IsNew(KEY_INPUT_Z)) || (input.IsNew(KEY_INPUT_M)));
		break;
	}
}
