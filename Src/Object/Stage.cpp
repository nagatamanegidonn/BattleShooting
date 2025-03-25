#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/AsoUtility.h"
#include "Common/Transform.h"
#include "Stage.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
	//�����f���̃e�N�X�`���̎Q�Ɛ悪���������̔����`�悳��܂�

	//���f���̏������\�[�X�}�l�[�W���[����������Ă���
	transform_.modelId = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::BACKGROUNDFOREST);
	//�傫��
	float scale = BACKGROUND_SCALE;
	transform_.scl = { scale, scale, scale };
	//���W
	transform_.pos = AsoUtility::VECTOR_ZERO;
	transform_.pos.y = BACKGROUND_POS_Y * scale;
	//��](X�AY�AZ�j
	transform_.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(BACKGROUND_ANGLE_Y),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	//�g�����X�t�H�[���X�V
	//�ύX����ꍇ�͕K�{
	transform_.Update();

}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	//3D���f���̕`��
	MV1DrawModel(transform_.modelId);
}
void Stage::DrawDebug(void)
{

	// �y���K�z�ŏ��̂P�{
	//VECTOR sPos = { 0.0f, 0.0f, 0.0f };
	//VECTOR ePos = { HLEN, 0.0f, 0.0f };
	//VECTOR sPos = { -HLEN, 0.0f, 0.0f };
	//VECTOR ePos = {  HLEN, 0.0f, 0.0f };
	//DrawLine3D(sPos, ePos, 0xff0000);

	// XZ��{��(�O���b�h)
	float num;

	//�����]����������]�ʂ��N�H�[�^�j�I���ō��
	Quaternion rotPow = Quaternion();

	float dirPow= 360.0f / (HNUM * 2);
	VECTOR dir = AsoUtility::DIR_R; 

	VECTOR sPos = AsoUtility::VECTOR_ZERO;
	VECTOR ePos;
	VECTOR eAgoPos = AsoUtility::VECTOR_ZERO;

	DrawSphere3D(sPos, 20.0f, 10, 0xff0000, 0xff0000, true);

	//�Z
	for (int i = -HNUM; i < HNUM; i++)
	{

		float rad = AsoUtility::Deg2RadF(dir.y);
		float cosA = cosf(rad);
		float sinA = sinf(rad);

		// Y����]�s����g���ĐV�������W���v�Z
		float newX = dir.x * cosA + dir.z * sinA;
		float newZ = -dir.x * sinA + dir.z * cosA;

		ePos = { newX, 0.0f, newZ };
		ePos = VScale(ePos, HLEN / 2);

		DrawLine3D(sPos, ePos, 0xff0000);
		DrawSphere3D(ePos, 20.0f, 10, 0x0000ff, 0x0000ff, true);

		if (!AsoUtility::EqualsVZero(eAgoPos))
		{
			DrawLine3D(ePos, eAgoPos, 0x0000ff);
		}
		eAgoPos = ePos;

		dir.y += dirPow;
	}

	// Y��(��)
	sPos = { 0.0f, -HLEN, 0.0f };
	ePos = { 0.0f,  HLEN, 0.0f };
	DrawLine3D(sPos, ePos, 0x00ff00);
	DrawSphere3D(ePos, 20.0f, 10, 0x00ff00, 0x00ff00, true);

	
	return;

	//��
	for (int i = -HNUM; i < HNUM; i++)
	{

		num = static_cast<float>(i);

		// X��(��)
		sPos = { -HLEN, 0.0f, num * TERM };
		ePos = { HLEN, 0.0f, num * TERM };
		DrawLine3D(sPos, ePos, 0xff0000);
		DrawSphere3D(ePos, 20.0f, 10, 0xff0000, 0xff0000, true);

		// Z��(��)
		sPos = { num * TERM, 0.0f, -HLEN };
		ePos = { num * TERM, 0.0f,  HLEN };
		DrawLine3D(sPos, ePos, 0x0000ff);
		DrawSphere3D(ePos, 20.0f, 10, 0x0000ff, 0x0000ff, true);

	}

	// Y��(��)
	sPos = { 0.0f, -HLEN, 0.0f };
	ePos = { 0.0f,  HLEN, 0.0f };
	DrawLine3D(sPos, ePos, 0x00ff00);
	DrawSphere3D(ePos, 20.0f, 10, 0x00ff00, 0x00ff00, true);

}

void Stage::Release(void)
{
}
