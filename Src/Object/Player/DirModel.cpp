#include "../../Utility/AsoUtility.h"

#include "DirModel.h"

DirModel::DirModel()
{
}

DirModel::~DirModel()
{
}

void DirModel::Init(int i)
{

	// ÉÇÉfÉãêßå‰ÇÃäÓñ{èÓïÒ
	transform_.SetModel(MV1LoadModel("Data/Model/Dir/Dir.mv1"));
	/*transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_SHIP));*/

			//float scale = 10.0f;
	float scale = 0.1f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = AsoUtility::VECTOR_ZERO;
	//transform_.pos = { 10.0f, 20.0f, 30.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);
	transform_.Update();

	if (i==0)
	{
		MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		MV1SetMaterialEmiColor(transform_.modelId, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
		MV1SetMaterialEmiColor(transform_.modelId, 0, GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
	}
}

void DirModel::Update()
{
}

void DirModel::Draw()
{

	MV1DrawModel(transform_.modelId);
}

void DirModel::SyncModel(VECTOR setPos, Quaternion Qut)
{

	transform_.pos = setPos;
	transform_.quaRot = Qut;
	transform_.Update();
}
