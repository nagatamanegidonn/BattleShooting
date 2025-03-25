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
	//�����f���̃e�N�X�`���̎Q�Ɛ悪���������̔����`�悳��܂�

	//���f���̏������\�[�X�}�l�[�W���[����������Ă���
	transform_.modelId = ResourceManager::GetInstance().LoadModelDuplicate(
		ResourceManager::SRC::BACKGROUNDCITY);
	//�傫��
	float scale = BACKGROUND_SCALE;
	transform_.scl = { scale, scale, scale };
	//���W
	transform_.pos = AsoUtility::VECTOR_ZERO;
	transform_.pos.y = BACKGROUND_POS_Y;
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

void BackGround::Update(void)
{
}

void BackGround::Draw(void)
{
	//3D���f���̕`��
	MV1DrawModel(transform_.modelId);
}

void BackGround::Release(void)
{
}
