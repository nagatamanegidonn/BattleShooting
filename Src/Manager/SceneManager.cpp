#include <chrono>
#include <thread>
#include <DxLib.h>

#include <EffekseerForDXLib.h>

#include "../Common/Fader.h"

#include "../Scene/TitleScene.h"
#include "../Scene/SelectScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/ResultScene.h"

#include "ResourceManager.h"
#include "Camera.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{

	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	fader_ = new Fader();
	fader_->Init();

	// �J����
	camera_ = new Camera();
	camera_->Init();

	//�Q�[���̏��s���ϐ��̏�����
	ResetGame();

	scene_ = new TitleScene();
	scene_->Init();

	isSceneChanging_ = false;

	// �f���^�^�C��
	preTime_ = std::chrono::system_clock::now();

	// 3D�p�̐ݒ�
	Init3D();

	// �����V�[���̐ݒ�
	//ChangeScene(SCENE_ID::TITLE);

}

void SceneManager::Init3D(void)
{

	// �w�i�F�ݒ�
	SetBackgroundColor(0, 0, 0);

	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	// �o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);

	// ���C�g�̐ݒ�
	SetUseLighting(true);

	// ���ʂ���΂߉��Ɍ����������C�g
	ChangeLightTypeDir({ 0.00f, -1.00f, 1.00f });

	// �t�H�O�ݒ�(�X�e�[�W������)
	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(3000.0f, 15000.0f);

}

void SceneManager::Update(void)
{

	if (scene_ == nullptr)
	{
		return;
	}

	// �f���^�^�C��
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	fader_->Update();
	if (isSceneChanging_)
	{
		Fade();
	}


	scene_->Update();

	// �J�����X�V
	camera_->Update();

}

void SceneManager::Draw(void)
{
	
	// �`���O���t�B�b�N�̈�̎w��
	// (�R�c�`��Ŏg�p����J�����̐ݒ�Ȃǂ����Z�b�g�����)
	SetDrawScreen(DX_SCREEN_BACK);

	// ��ʂ�������
	ClearDrawScreen();

	// �J�����ݒ�
	camera_->SetBeforeDraw();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();

	// �`��
	scene_->Draw();

	// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
	DrawEffekseer3D();

	// �Ó]�E���]
	fader_->Draw();
	
	if (fader_->GetState() == Fader::STATE::LOADING)
	{
		scene_->LoadingDraw();
	}

	// �f�o�b�N�p�`��
#ifdef _DEBUG
	SetFontSize(16);
	 // �񓯊��ǂݍ��݂̐���`��
	DrawFormatString(0, 0, GetColor(255, 255, 255), "�񓯊��ǂݍ��݂̐� %d", GetASyncLoadNum());
#endif

}

void SceneManager::Destroy(void)
{

	scene_->Release();
	delete scene_;

	delete fader_;

	camera_->Release();
	delete camera_;

	delete instance_;

}

void SceneManager::ChangeScene(SCENE_ID nextId)
{
	// �t�F�[�h�������I����Ă���V�[����ς���ꍇ�����邽�߁A
	// �J�ڐ�V�[���������o�ϐ��ɕێ�
	waitSceneId_ = nextId;

	// �t�F�[�h�A�E�g(�Ó])���J�n����
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	return deltaTime_;
}


Camera* SceneManager::GetCamera(void) const
{
	return camera_;
}

bool SceneManager::IsLoading(void) const
{
	if (fader_->GetState() == Fader::STATE::LOADING)
	{
		return true;
	}

	return false;
}
//
//bool SceneManager::IsLoading(void) const
//{
//	return (fader_->GetState() == Fader::STATE::LOADING);
//}


void SceneManager::ResetGame(void)
{
	isWinner_ = WINNER::NONE;
}
//���Ҋ֌W
SceneManager::WINNER SceneManager::GetWinner(void) const
{
	return isWinner_;
}
void SceneManager::SetWinner(WINNER win)
{
	isWinner_ = win;
}


SceneManager::SceneManager(void)
{

	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	scene_ = nullptr;
	fader_ = nullptr;

	isSceneChanging_ = false;

	// �f���^�^�C��
	deltaTime_ = 1.0f / 60.0f;

	camera_ = nullptr;
	
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{

	// ���\�[�X�̉��
	ResourceManager::GetInstance().Release();

	// �V�[����ύX����
	sceneId_ = sceneId;

	// ���݂̃V�[�������
	if (scene_ != nullptr)
	{
		scene_->Release();
		delete scene_;
	}

	//�󂯎����sceneId_�ŃV�[����ύX����
	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		scene_ = new TitleScene();
		break;
	case SCENE_ID::SELECT:
		scene_ = new SelectScene();
		break;
	case SCENE_ID::GAME:
		scene_ = new GameScene();
		break;
	case SCENE_ID::RESULT:
		scene_ = new ResultScene();
		break;
	}

	//Init�̕ς���AsyncPreLoad
	scene_->AsyncPreLoad();

	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// ���]��
		if (fader_->IsEnd())
		{
			// ���]���I��������A�t�F�[�h�����I��
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// �Ó]��
		if (fader_->IsEnd())
		{
			// ���S�ɈÓ]���Ă���V�[���J��
			DoChangeScene(waitSceneId_);
			// �Ó]����  ��
			fader_->SetFade(Fader::STATE::LOADING);
		}
		break;
	case Fader::STATE::LOADING:
		// �Ó]��

		//�V�[�������ׂēǂݍ��߂����m�F
		if (scene_->IsLoad())
		{

			//������
			scene_->Init();

			// �Ó]���疾�]��
			fader_->SetFade(Fader::STATE::FADE_IN);

		}
		break;
	}

}


