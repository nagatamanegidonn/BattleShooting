/* ---------------------
　フレームレート制御処理
 ----------------------- */

#include "FrameRate.h"
#include "../Application.h"
#include <DxLib.h>

/*　デフォルトコンストラクタ　*/
FrameRate::FrameRate(void)
{
	curTime_  = lateTime_ = 0;

	counter_	= 0;
	updateTime_ = 0;

	viewFramelate_ = 0;
}


/*　更新処理　*/
void FrameRate::Update(void)
{
	Sleep(1); //システムに処理を返す

	//現在時刻を取得する
	curTime_ = GetNowCount();
}

/*　描画処理　*/
void FrameRate::Draw(void)
{
	// 平均フレームレート 描画
	DrawFormatString((Application::SCREEN_SIZE_X - TEXT_POS_X),
					 (Application::SCREEN_SIZE_Y - TEXT_POS_Y),
					 0xFFFFFF, "%.1ffps", viewFramelate_);
}


/*　フレームレート割り当て　*/
void FrameRate::SetFrameRate(void)
{
	lateTime_ = curTime_; // 前フレームの時間 割り当て

	counter_++; // フレームカウント増加

	int nDifTime = curTime_ - updateTime_;

	if (nDifTime > 1000)
	{
		// フレームレート単位変更(ミリ秒 → 秒)
		float fFrameCount = (float)(counter_ * 1000);

		// 描画フレーム単位 取得
		viewFramelate_ = fFrameCount / nDifTime;

		counter_ = 0; // フレームカウント 初期化

		// フレームレート 更新
		updateTime_ = curTime_;
	}
}

/*　フレームレート制限 判定　*/
bool FrameRate::GetLimitFrameRate(void)
{
	if (curTime_ - lateTime_ >= FRAME_RATE)
	{
		return true;
	}
	return false;
}