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

		isControl_[static_cast<int>(MODE::FORWARD)] = ((leftStickY < 0.0f) || (input.IsNew(KEY_INPUT_W)));
		isControl_[static_cast<int>(MODE::BACK)] = ((leftStickY > 0.0f) || (input.IsNew(KEY_INPUT_S)));
		isControl_[static_cast<int>(MODE::RIGHT)] = ((leftStickX > 0.0f) || (input.IsNew(KEY_INPUT_D)));
		isControl_[static_cast<int>(MODE::LEFT)] = ((leftStickX < 0.0f) || (input.IsNew(KEY_INPUT_A)));
		isControl_[static_cast<int>(MODE::ATTACK)] = ((input.IsNew(KEY_INPUT_Z)) || (input.IsNew(KEY_INPUT_V)));
	}
		break;
	case 1:
		isControl_[static_cast<int>(MODE::FORWARD)] = (input.IsNew(KEY_INPUT_UP));
		isControl_[static_cast<int>(MODE::BACK)] = (input.IsNew(KEY_INPUT_DOWN));
		isControl_[static_cast<int>(MODE::RIGHT)] = (input.IsNew(KEY_INPUT_RIGHT));
		isControl_[static_cast<int>(MODE::LEFT)] = (input.IsNew(KEY_INPUT_LEFT));
		isControl_[static_cast<int>(MODE::ATTACK)] = ((input.IsNew(KEY_INPUT_Z)) || (input.IsNew(KEY_INPUT_B)));
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
