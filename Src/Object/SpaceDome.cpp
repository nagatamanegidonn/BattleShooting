
#include <DxLib.h>

#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"

#include "../Utility/AsoUtility.h"
#include "SpaceDome.h"
#include "Stage.h"

SpaceDome::SpaceDome(const Transform& follow) : follow_(follow)
{
}

SpaceDome::~SpaceDome(void)
{
}

void SpaceDome::Init(void)
{
	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::SKYDOME));

	float scale = Stage::BACKGROUND_SCALE * 3;
	//scale = 0.01f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

	
}

void SpaceDome::Update(void)
{
	auto& input = InputManager::GetInstance();

	VECTOR movePow = AsoUtility::VECTOR_ZERO;

	movePow = VSub(follow_.pos, transform_.pos);
	transform_.pos = VAdd(transform_.pos,movePow);
	transform_.pos = follow_.pos;

	// 回転
	// ラジアンからクォータニオン
	float rot = 10.0f;


	transform_.Update();
}

void SpaceDome::Draw(void)
{
	MV1SetWriteZBuffer(transform_.modelId, false);

	MV1DrawModel(transform_.modelId);

}

void SpaceDome::Release(void)
{
	// ロードされた３Ｄモデルをメモリから解放
	MV1DeleteModel(transform_.modelId);
}
