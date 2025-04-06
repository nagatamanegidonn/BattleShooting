/* ---------------------
�@�t���[�����[�g���䏈��
 ----------------------- */

#include "FrameRate.h"
#include "../Application.h"
#include <DxLib.h>

/*�@�f�t�H���g�R���X�g���N�^�@*/
FrameRate::FrameRate(void)
{
	curTime_  = lateTime_ = 0;

	counter_	= 0;
	updateTime_ = 0;

	viewFramelate_ = 0;
}


/*�@�X�V�����@*/
void FrameRate::Update(void)
{
	Sleep(1); //�V�X�e���ɏ�����Ԃ�

	//���ݎ������擾����
	curTime_ = GetNowCount();
}

/*�@�`�揈���@*/
void FrameRate::Draw(void)
{
	// ���σt���[�����[�g �`��
	DrawFormatString((Application::SCREEN_SIZE_X - TEXT_POS_X),
					 (Application::SCREEN_SIZE_Y - TEXT_POS_Y),
					 0xFFFFFF, "%.1ffps", viewFramelate_);
}


/*�@�t���[�����[�g���蓖�ā@*/
void FrameRate::SetFrameRate(void)
{
	lateTime_ = curTime_; // �O�t���[���̎��� ���蓖��

	counter_++; // �t���[���J�E���g����

	int nDifTime = curTime_ - updateTime_;

	if (nDifTime > 1000)
	{
		// �t���[�����[�g�P�ʕύX(�~���b �� �b)
		float fFrameCount = (float)(counter_ * 1000);

		// �`��t���[���P�� �擾
		viewFramelate_ = fFrameCount / nDifTime;

		counter_ = 0; // �t���[���J�E���g ������

		// �t���[�����[�g �X�V
		updateTime_ = curTime_;
	}
}

/*�@�t���[�����[�g���� ����@*/
bool FrameRate::GetLimitFrameRate(void)
{
	if (curTime_ - lateTime_ >= FRAME_RATE)
	{
		return true;
	}
	return false;
}