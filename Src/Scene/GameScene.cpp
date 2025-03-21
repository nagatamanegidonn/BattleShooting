#include <DxLib.h>

#include <EffekseerForDXLib.h>

#include "../Application.h"

#include "../Utility/AsoUtility.h"//Quaternion���������֐��������Ă���

#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"

#include "../Object/BackGround.h"
#include "../Object/Grid.h"
#include "../Object/Shot/ShotPlayer.h"
#include "../Object/Player/Player.h"

#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::AsyncPreLoad(void)
{
	//�񓯊��ǂݍ��݂�L���ɂ���
	SetUseASyncLoadFlag(true);

	// ������: i = 1�A������: i <= 5�A�X�V: i++
	for (int i = 0; i < PLAYER_SIZE; i++) {
		camera_[i] = new Camera();

		auto  player = std::make_shared<Player>(*camera_[i]);
		players_.push_back(player);
	}

	// �����摜
	handleIds_ = new int[10 * 6];
	LoadDivGraph(
		"Alphabet.png",
		10 * 6,
		10, 6,
		32, 192 / 6,
		&handleIds_[0]);
	// �����摜
	handleIds_2 = new int[10 * 6];
	LoadDivGraph(
		"Alphabet.png",
		10 * 6,
		10, 6,
		32, 192 / 6,
		&handleIds_2[0]);

	stage_ = new Grid;

	backGround_ = new BackGround;
}
void GameScene::Init(void)
{
	//�񓯊������𖳌��ɂ���
	SetUseASyncLoadFlag(false);

	float size = 100.0f;

	VECTOR sPos[4] = {
		{-size,0.0f,size}//����
		,{size,0.0f,size}//�E��
		,{-size,0.0f,-size}//����
		,{size,0.0f,-size}//�E�� 
	};


	// ������: i = 1�A������: i <= 5�A�X�V: i++
	for (int i = 0; i < PLAYER_SIZE; i++) {
		players_[i]->Init(sPos[i], i);

		//�e�v���C���[�̃X�N���[���̍쐬
		screenH[i] = MakeScreen(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y, true);

	}

	stage_->Init();

	//�w�i������
	backGround_->Init();

	Camera* camera = SceneManager::GetInstance().GetCamera();
	// �J�������[�h�F��_�J����
	camera->ChangeMode(Camera::MODE::FOLLOW_POINT);

	int c = 0;
	for (auto p : players_)
	{
		//�{�̃J�����̐ݒ�
		if (c == 0)
		{
			camera->SetFollow(&p->GetTransform());
		}
		else//2�ڈȍ~�Ȃ�
		{
			camera->SetSubFollow(&p->GetTransform());
		}

		// �J����
		camera_[c]->Init();

		camera_[c]->ChangeMode(Camera::MODE::FOLLOW);
		camera_[c]->SetFollow(&p->GetTransform());
		c++;
	}
	
	// ���@�j��G�t�F�N�g
	effectHitResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::SHOT_EXPLOSION).handleId_;


	hitStop_ = 0.0f;
}

void GameScene::Update(void)
{
	//���[�h���������������f
	if (GetASyncLoadNum() != 0 || SceneManager::GetInstance().IsLoading())
	{
		return;
	}

	if (hitStop_ > 0)
	{
		hitStop_ -= SceneManager::GetInstance().GetDeltaTime();
	}
	else
	{
		//�G�t�F�N�g�Đ�
		effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
		hitStop_ = 2;
	}
	//�ʒu
	SetPosPlayingEffekseer3DEffect(effectHitPlayId_,0, 0, 0);
	// �傫��
	float SCALE = 0.1f;
	SetScalePlayingEffekseer3DEffect(effectHitPlayId_, SCALE, SCALE, SCALE);
	//��]
	SetRotationPlayingEffekseer3DEffect(effectHitPlayId_, 0, 0, 0);



	// �V�[���J��
	/*InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
		return;
	}*/

	/*stage_->Update();*/

	

	//�v���C���[�̍X�V
	for (auto p : players_)
	{
		p->Update();
	}

	// �Փ˔���
	Collision();

	//�v���C���[�̃J�����X�V
	for (auto& camera : camera_)
	{
		camera->Update();
	}

	// �J�����Փ˃`�F�b�N
	Camera* camera = SceneManager::GetInstance().GetCamera();
	// �J�������[�h�F
	//camera->ChangeMode(Camera::MODE::FIXED_POINT);


}

void GameScene::Draw(void)
{
	//���[�h���������������f
	if (GetASyncLoadNum() != 0 || SceneManager::GetInstance().IsLoading())
	{	
		return;
	}

#pragma region �Q�[���V�[���̕`��

	// �`��
	stage_->Draw();
	int screenSize = 100;
	int mx = Application::SCREEN_SIZE_X - screenSize;
	int my = Application::SCREEN_SIZE_Y - screenSize;

	int cx = Application::SCREEN_SIZE_X - screenSize;
	int cy = Application::SCREEN_SIZE_Y - screenSize;


	//�w�i�`��
	//backGround_->Draw();

	//�v���C���[�̕`��
	for (auto& p : players_)
	{
		p->Draw();

		VECTOR pos2D = ConvWorldPosToScreenPos(p->GetTransform().pos);
		DrawCircle(pos2D.x, pos2D.y, 10, 0x0000ff);

		if ((mx < pos2D.x) || (my < pos2D.y) || (screenSize > pos2D.x) || (screenSize > pos2D.y))
		{
			SceneManager::GetInstance().GetCamera()->FadeOut();
		}
	}

	//�f�o�b�O�`��
	DrawDebug();

#pragma endregion
	return;


//	for (int i = 0; i < PLAYER_SIZE; i++) 
//	{
//		// �ݒ肵�����X�N���[�����쐬����
//		SetDrawScreen(screenH[i]);
//
//		// ��ʂ�������
//		ClearDrawScreen();
//
//		// �J�����ݒ�
//		camera_[i]->SetBeforeDraw();
////
//#pragma region �Q�[���V�[���̕`��
//
//		// �`��
//		stage_->Draw();
//
//		//�v���C���[�̕`��
//		for (auto& p : players_)
//		{
//			p->Draw();
//		}
//
//		//�f�o�b�O�`��
//		//DrawDebug();
//
//#pragma endregion
//
//		//�Ȃ�
//		SetDrawScreen(DX_SCREEN_BACK);
//
//	}
	
	SetDrawScreen(DX_SCREEN_BACK);
	// ��ʂ�������
	ClearDrawScreen();

	int x = 0;
	int y = 0;

	//�`��ꏊ
	/*for (auto screen : screenH)
	{
		DrawGraph(x, y, screen, true);

		if (x >= cx && y >= cy)
		{
			x += cx;
		}
		else if (x >= cx && y >= 0)
		{
			x -= cx;
			y += cy;
		}
		else if (x >= 0 && y >= 0)
		{
			x += cx;
		}
	}*/


	// (�R�c�`��Ŏg�p����J�����̐ݒ�Ȃǂ����Z�b�g�����)
	SetDrawScreen(DX_SCREEN_BACK);

}

void GameScene::Release(void)
{
	int num = 10 * 6;
	for (int i = 0; i < num; i++)
	{
		DeleteGraph(handleIds_[i]);
	}
	delete[] handleIds_;
	
	num = 10 * 6;
	for (int i = 0; i < num; i++)
	{
		DeleteGraph(handleIds_2[i]);
	}
	delete[] handleIds_2;



	for (int i = 0; i < PLAYER_SIZE; i++) {
		camera_[i]->Release();
		delete camera_[i];
	}


	stage_->Release();

	//�w�i�������
	backGround_->Release();
	delete backGround_;

	//�G�t�F�N�g�̊J��
	StopEffekseer3DEffect(effectHitPlayId_);

}


void GameScene::Collision(void)
{
	//�v���C���[�̍X�V
	for (auto& plyer : players_)
	{
		auto shots = plyer->GetShots();
		for (auto& s : shots)
		{
			s->Update();
		}


		for (auto& vsPlyer : players_)
		{
			//����Ă���v���C���[�����g��������R���e�j���[
			if (plyer == vsPlyer) { continue; }


			VECTOR pos2D = ConvWorldPosToScreenPos(plyer->GetTransform().pos);
			VECTOR posVS2D = ConvWorldPosToScreenPos(vsPlyer->GetTransform().pos);
			int screenSize = 100;
			int cx = Application::SCREEN_SIZE_X / 2;
			int cy = Application::SCREEN_SIZE_Y / 2;

			//�J�����̔���
			if (((cx + screenSize > pos2D.x) && (cy + screenSize > pos2D.y)
				&& (cx - screenSize < pos2D.x) && (cy - screenSize < pos2D.y))
				&& ((cx + screenSize > posVS2D.x) && (cy + screenSize > posVS2D.y)
					&& (cx - screenSize < posVS2D.x) && (cy - screenSize < posVS2D.y)))
			{
				SceneManager::GetInstance().GetCamera()->FadeIn();
			}



			// player�Ƃ̏Փ˔���
			VECTOR diff = VSub(plyer->GetPos(0), vsPlyer->GetPos(1));

			float disPow = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
			if (disPow < Player::DAMAGE_RADIUS * Player::ATTRCK_RADIUS)//�_���[�W���a�~�U�����a
			{
				//plyer�ɑ���̃}�V���̍U�����������Ă���
				plyer->Damage(1);
				//�ӂ���΂�����
				VECTOR dir = VNorm(VSub(plyer->GetTransform().pos, vsPlyer->GetTransform().pos));
				plyer->SetJump(dir);
				dir = VScale(dir, -1);//������΂��̕����𔽓]
				vsPlyer->SetJump(dir);

				//�G�t�F�N�g�Đ�
				effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
				//�ʒu
				SetPosPlayingEffekseer3DEffect(effectHitPlayId_, plyer->GetTransform().pos.x, plyer->GetTransform().pos.y, plyer->GetTransform().pos.z);
			}

			//�U���ӏ����m���Փ�
			diff = VSub(plyer->GetPos(1), vsPlyer->GetPos(1));

			disPow = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
			if (disPow < Player::ATTRCK_RADIUS * Player::ATTRCK_RADIUS)//�U�����a�~�U�����a
			{
				plyer->Damage(1);
				vsPlyer->Damage(1);

				//�ӂ���΂�����
				VECTOR dir = VNorm(VSub(plyer->GetTransform().pos, vsPlyer->GetTransform().pos));
				plyer->SetJump(dir);
				dir = VScale(dir, -1);
				vsPlyer->SetJump(dir);

				//�G�t�F�N�g�Đ�
				effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
				//�ʒu
				SetPosPlayingEffekseer3DEffect(effectHitPlayId_, plyer->GetTransform().pos.x, plyer->GetTransform().pos.y, plyer->GetTransform().pos.z);
			}

			// �傫��
			float SCALE = 0.1f;
			SetScalePlayingEffekseer3DEffect(effectHitPlayId_, SCALE, SCALE, SCALE);
			//��]
			SetRotationPlayingEffekseer3DEffect(effectHitPlayId_, plyer->GetTransform().rot.x, plyer->GetTransform().rot.y, plyer->GetTransform().rot.z);


			auto shots = plyer->GetShots();
			for (auto& shot : shots)
			{
				if (shot->IsShot() && vsPlyer->GetState() == Player::STATE::PLAY)
				{
					
					//�e�̓����蔻��
#pragma region �v��

					//auto info = MV1CollCheck_Sphere(vsPlyer->GetModelId(), -1,
					//	shot->GetPos(), shot->GetCollisionRadius());
					//if (info.HitNum >= 1)
					//{
					//	//plyer�ɍU�����������Ă���
					//	vsPlyer->RideDamage(1);

					//	//�ӂ���΂�����
					//	VECTOR dir = VScale(VNorm(VSub(shot->GetPos(), vsPlyer->GetTransform().pos)),-1);
					//	vsPlyer->SetJump(dir);

					//	shot->Blast();
					//}
					//// �����蔻�茋�ʃ|���S���z��̌�n��������
					//MV1CollResultPolyDimTerminate(info);
#pragma endregion
					diff = VSub(shot->GetPos(), vsPlyer->GetPos(2));
					disPow = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
					if (disPow < shot->GetCollisionRadius() * Player::DAMAGE_RADIUS)//�U�����a�~�U�����a
					{
						//plyer�ɍU�����������Ă���
						vsPlyer->Damage(1);
						//hitStop_ = 0.5f;

						//�ӂ���΂�����
						VECTOR dir = VScale(VScale(VNorm(VSub(shot->GetPos(), vsPlyer->GetTransform().pos)), -1), 0.5f);
						vsPlyer->SetJump(dir);

						shot->Blast();
					}
				}
			}
		}
	}

	int winid = 0;
	SceneManager& Sns = SceneManager::GetInstance();
	for (auto& plyer : players_)
	{
		//���s����
		if (plyer->GetState() == Player::STATE::DEAD)
		{
			switch (winid)
			{
			case 0:
				Sns.SetWinner(SceneManager::WINNER::PLAYER_TWO);
				break;
			case 1:
				Sns.SetWinner(SceneManager::WINNER::PLAYER_ONE);
				break;
			}
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
			return;
		}
		winid++;
	}

}

//�f�o�b�O�`��
void GameScene::DrawDebug(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;


	int plyNum= -1;
	//�v���C���[�̍X�V
	for (auto& plyer : players_)
	{
		int cx2 = cx + (20 * plyNum);
		DrawBox(cx2, 20, cx2 + (20 * plyNum * plyer->GetMaxHp()), 50, 0x000000, true);
		DrawBox(cx2, 20, cx2 + (20 * plyNum * plyer->GetHp()), 50, 0x00ff00, true);

		VECTOR pos = plyer->GetTransform().pos;
		if (plyNum == 1)
		{
			DrawFormatString(0, 16, 0xff0000, "%2.f,%2.f,%2.f", pos.x, pos.y, pos.z);
		}
		else
		{
			DrawFormatString(0, 32, 0xff0000, "%2.f,%2.f,%2.f", pos.x, pos.y, pos.z);
		}
		plyNum *= -1;
	}



	return;

	//���b�Z�[�W
	std::string msg = "Game Scene";
	SetFontSize(28);//�����̃T�C�Y��ݒ�

	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), 550, 0xffffff, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	SetFontSize(16);

}
