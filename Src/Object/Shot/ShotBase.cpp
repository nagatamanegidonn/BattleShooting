#include <EffekseerForDXLib.h>

#include "../../Utility/AsoUtility.h"

#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"

#include "ShotBase.h"


ShotBase::ShotBase(void)
{
	// ������
	timeAlive_ = 0.0f;
	dir_ = AsoUtility::VECTOR_ZERO;

	// �p�����[�^�ݒ�
	//SetParam();
	
	// ���f������̊�{���
	ResourceManager& rem = ResourceManager::GetInstance();
	transform_.modelId =
		rem.LoadModelDuplicate(ResourceManager::SRC::SHOT_MODEL);
	//float scale = 0.2f;
	float scale = 0.85f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = AsoUtility::VECTOR_ZERO;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler(AsoUtility::Deg2RadF(90.0f), AsoUtility::Deg2RadF(-90.0f), 0.0f);
	transform_.Update();

	// �����G�t�F�N�g
	effectBlastResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::SHOT_EXPLOSION).handleId_;
}

ShotBase::~ShotBase(void)
{
}

void ShotBase::Create(VECTOR birthPos, VECTOR dir,int modelId)
{
	//���f���̐ݒ�
	transform_.modelId = MV1DuplicateModel(modelId);
	float scale = 0.85f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = AsoUtility::VECTOR_ZERO;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler(AsoUtility::Deg2RadF(90.0f), AsoUtility::Deg2RadF(-90.0f), 0.0f);
	transform_.Update();

	// �p�����[�^�ݒ�
	SetParam();
	// �ė��p�\�Ȃ悤�ɂ���
	// �w������ɒe���΂�
	dir_ = dir;
	// �e�̔����ʒu
	transform_.pos = birthPos;

	// �e���f���̌���(�p�x)���w������ɍ��킹��
	transform_.quaRot = Quaternion::LookRotation(dir_);

	// �����t���O�A���Ԃ̏�����
	timeAlive_ = 2.0f;
	// ���f������̊�{���X�V
	transform_.Update();
	// ��ԑJ��
	ChangeState(STATE::SHOT);
}

void ShotBase::Blast(void)
{
	ChangeState(STATE::BLAST);
}

bool ShotBase::IsShot(void)
{
	if (state_ == STATE::SHOT)
	{
		return true;
	}
	return false;
}

void ShotBase::Update(void)
{
	switch (state_)
	{
	case ShotBase::STATE::NONE:
		break;
	case ShotBase::STATE::SHOT:
		UpdateShot();
		break;
	case ShotBase::STATE::BLAST:
		UpdateBlast();
		break;
	case ShotBase::STATE::END:
		break;
	}
	// ���f������̊�{���X�V
	transform_.Update();
}

void ShotBase::CheckAlive(void)
{
	// �������Ԃ����炷
	timeAlive_ -= SceneManager::GetInstance().GetDeltaTime();
	if (timeAlive_ < 0.0f)
	{
		ChangeState(STATE::END);
	}
}

void ShotBase::Draw(void)
{
	// �J�����N���b�v�O�ɂȂ�����`�悵�Ȃ�
	if (!CheckCameraViewClip(transform_.pos))
	{
		switch (state_)
		{
		case ShotBase::STATE::NONE:
			break;
		case ShotBase::STATE::SHOT:
			MV1DrawModel(transform_.modelId);
			//DrawSphere3D(transform_.pos, collisionRadius_, 10, 0x00ff00, 0x00ff00, true);

			break;
		case ShotBase::STATE::BLAST:
			MV1DrawModel(transform_.modelId);
			break;
		case ShotBase::STATE::END:
			break;
		}
	}
}

void ShotBase::Release(void)
{
}

VECTOR ShotBase::GetPos(void) const
{
	return transform_.pos;
}

float ShotBase::GetSpeed(void) const
{
	return speed_;
}

float ShotBase::GetTimeAlive(void) const
{
	return timeAlive_;
}

float ShotBase::GetCollisionRadius(void) const
{
	return collisionRadius_;
}

void ShotBase::Move(void)
{
	// �ړ�
	VECTOR velocity = VScale(dir_, speed_);
	transform_.pos = VAdd(transform_.pos, velocity);
}
void ShotBase::UpdateShot(void)
{
	// �����`�F�b�N
	CheckAlive();
	if (state_ != STATE::SHOT)
	{
		// �������f
		return;
	}
	// �ړ�����
	Move();
}
void ShotBase::UpdateBlast(void)
{
	// �G�t�F�N�g�Đ��`�F�b�N
	// 0:�Đ����A-1:�Đ�����Ă��Ȃ��A�������͍Đ��I��
	int ret = IsEffekseer3DEffectPlaying(effectBlastPlayId_);
	if (ret == -1)
	{
		// �I����Ԃ�
		ChangeState(STATE::END);
	}
}

void ShotBase::ChangeState(STATE state)
{
	if (state_ == state)
	{
		// ������ԂȂ珈�����Ȃ�
		return;
	}
	// ��ԍX�V
	state_ = state;
	switch (state)
	{
	case ShotBase::STATE::NONE:
		break;
	case ShotBase::STATE::SHOT:
		break;
	case ShotBase::STATE::BLAST:
		// �����G�t�F�N�g���Đ�����
		PlayBlastEffect();
		break;
	case ShotBase::STATE::END:
		break;
	}
}

void ShotBase::PlayBlastEffect(void)
{
	// �����G�t�F�N�g���Đ�����
	effectBlastPlayId_ = PlayEffekseer3DEffect(effectBlastResId_);
	SetScalePlayingEffekseer3DEffect(
		effectBlastPlayId_, BLAST_SCALE, BLAST_SCALE, BLAST_SCALE);
	SetRotationPlayingEffekseer3DEffect(effectBlastPlayId_, 0.0f, 0.0f, 0.0f);
	VECTOR pos = transform_.pos;
	SetPosPlayingEffekseer3DEffect(effectBlastPlayId_, pos.x, pos.y, pos.z);
}