
#include <DxLib.h>

#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"

#include "../Utility/AsoUtility.h"
#include "SpaceDome.h"

SpaceDome::SpaceDome(const Transform& follow) : follow_(follow)
{
	//state_ = STATE::NONE;
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

	float scale = 1.0f;
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
	
	
	/*if (input.IsNew(KEY_INPUT_Q))	{		movePow.x = 1.0f;	}
	if (input.IsNew(KEY_INPUT_E))	{		movePow.x = -1.0f;	}
	if (input.IsNew(KEY_INPUT_B))	{		movePow.z = 1.0f;	}
	if (input.IsNew(KEY_INPUT_F))   {		movePow.z = -1.0f;}
	if (input.IsNew(KEY_INPUT_U))	{	movePow.y = 1.0f;	}
	if (input.IsNew(KEY_INPUT_I))   {	movePow.y = -1.0f;	}

	transform_.pos = VAdd(transform_.pos, movePow);*/


	//transform_.pos = follow_.pos;

	movePow = VSub(follow_.pos, transform_.pos);

	transform_.pos = VAdd(transform_.pos,movePow);

	transform_.pos = follow_.pos;

	/*transform_.pos.z = follow_.localPos.x;
	transform_.pos.x = follow_.pos.y;
	transform_.pos.y = follow_.pos.z;*/

	// 回転

	// ラジアンからクォータニオン
	float rot = 10.0f;

	/*Quaternion qua = Quaternion::Euler
	(AsoUtility::Deg2RadF(rot)
		, AsoUtility::Deg2RadF(rot)
		, AsoUtility::Deg2RadF(rot));

	transform_.quaRot = transform_.quaRot.Mult(qua);*/




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
