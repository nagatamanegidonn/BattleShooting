#include "../../Utility/AsoUtility.h"
#include "Mush.h"

Mush::Mush(void)
{
}

Mush::~Mush(void)
{
}

void Mush::SetParam(void)
{
	//ステータス
	damageTime_ = 0.0f;
	invincibleTime_ = 0.0f;

	rideAttrckPos_ = transform_.pos;
	rideDamagePos_ = transform_.pos;

	//ステータス変数
	//HPもとは１０
	playerMaxHp_ = playerHp_ = MAX_HP;

	//変数：攻撃関係
	//弾発射後の硬直時間セット
	deleyShot_ = TIME_DELAY_SHOT;
	shotMagazineMax_ = shotMagazine_ = 20;
	reloadTime_ = 0.0f;
	reloadSet_ = 0.0f;

	//ふっ飛ばし関係
	jumpTime_ = 0.0f;
	jumpDir_ = AsoUtility::VECTOR_ZERO;
	jumpAttrckRate_ = 1.5f;
	jumpDefenseRate_ = 0.5f;
}
