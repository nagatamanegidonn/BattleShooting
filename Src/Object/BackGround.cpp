#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/AsoUtility.h"
#include "Common/Transform.h"
#include "BackGround.h"

BackGround::BackGround(void)
{
}

BackGround::~BackGround(void)
{
}

void BackGround::Init(void)
{
	//※モデルのテクスチャの参照先がおかしいの白く描画されます

	//モデルの情報をリソースマネージャーから引っ張てくる
	transform_.modelId = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::BACKGROUNDCITY);
	//大きさ
	float scale = BACKGROUND_SCALE;
	transform_.scl = { scale, scale, scale };
	//座標
	transform_.pos = AsoUtility::VECTOR_ZERO;
	transform_.pos.y = BACKGROUND_POS_Y;
	//回転(X、Y、Z）
	transform_.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(BACKGROUND_ANGLE_Y),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	//トランスフォーム更新
	//変更する場合は必須
	transform_.Update();

}

void BackGround::Update(void)
{
}

void BackGround::Draw(void)
{
	//3Dモデルの描画
	MV1DrawModel(transform_.modelId);
}

void BackGround::Release(void)
{
}
