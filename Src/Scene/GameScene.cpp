#include <DxLib.h>

#include <EffekseerForDXLib.h>

#include "../Application.h"

#include "../Utility/AsoUtility.h"//Quaternion���������֐��������Ă���

#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"

#include "../Object/Stage.h"
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

		auto  player = std::make_shared<Player>();
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

	stage_ = new Stage;
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

	//�w�i������
	stage_->Init();
	//backGround_->Init();

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

	//�G�t�F�N�g���Đ����Ă邩
	int ret = IsEffekseer3DEffectPlaying(effectHitPlayId_);
	if (ret == -1)
	{
		//�Đ��ł��ĂȂ�
		int t = 0;
	}

	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
		return;
	}

	SceneManager& Sns = SceneManager::GetInstance();

	//���s����
	if (players_[0]->IsDead() || players_[1]->IsDead())
	{
		if (players_[0]->IsDead() && players_[1]->IsDead())
		{
			Sns.SetWinner(SceneManager::WINNER::DRAW);
		}
		else if (players_[0]->IsDead())
		{
			Sns.SetWinner(SceneManager::WINNER::PLAYER_TWO);
		}
		else if (players_[1]->IsDead())
		{
			Sns.SetWinner(SceneManager::WINNER::PLAYER_ONE);
		}

		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
		return;
	}


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
	//�w�i�`��
	//backGround_->Draw();
	stage_->Draw();
	int screenSize = 100;
	int mx = Application::SCREEN_SIZE_X - screenSize;
	int my = Application::SCREEN_SIZE_Y - screenSize;

	int cx = Application::SCREEN_SIZE_X - screenSize;
	int cy = Application::SCREEN_SIZE_Y - screenSize;


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
#ifdef _DEBUG
	DrawDebug();
#endif

#pragma endregion
	
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

	//�w�i�������
	stage_->Release();
	delete stage_;

	//backGround_->Release();
	//delete backGround_;

	//�G�t�F�N�g�̊J��
	StopEffekseer3DEffect(effectHitPlayId_);
}

//�Q�[���̏Փˏ���
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

			//�v���C���[���m�̒��S
			VECTOR cPos = VScale(VAdd(plyer->GetTransform().pos, vsPlyer->GetTransform().pos), 0.5f);

			float disPow = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
			if (disPow < Player::DAMAGE_RADIUS * Player::ATTRCK_RADIUS)//�_���[�W���a�~�U�����a
			{
				//plyer�ɑ���̃}�V���̍U�����������Ă���
				plyer->Damage(1);
				//�ӂ���΂�����
				VECTOR dir = VNorm(VSub(plyer->GetTransform().pos, vsPlyer->GetTransform().pos));
				plyer->SetJump(VScale(dir, 1.5f));
				dir = VScale(dir, -1);//������΂��̕����𔽓]&�~�T
				vsPlyer->SetJump(dir);

				// �����G�t�F�N�g���Đ�����
				effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
				SetScalePlayingEffekseer3DEffect(effectHitPlayId_, BLAST_SCALE, BLAST_SCALE, BLAST_SCALE);
				SetRotationPlayingEffekseer3DEffect(effectHitPlayId_, AsoUtility::Deg2RadF(90.0f), 0.0f, 0.0f);
				SetPosPlayingEffekseer3DEffect(effectHitPlayId_, cPos.x, cPos.y, cPos.z);
			}

			//�U���ӏ����m���Փ�
			diff = VSub(plyer->GetPos(1), vsPlyer->GetPos(1));

			disPow = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
			if (disPow < Player::ATTRCK_RADIUS * Player::ATTRCK_RADIUS + Player::ATTRCK_RADIUS * Player::ATTRCK_RADIUS)//�U�����a�~�U�����a
			{
				plyer->Damage(1);
				vsPlyer->Damage(1);

				//�ӂ���΂�����
				VECTOR dir = VNorm(VSub(plyer->GetTransform().pos, vsPlyer->GetTransform().pos));
				plyer->SetJump(dir);
				dir = VScale(dir, -1);
				vsPlyer->SetJump(dir);

				// �����G�t�F�N�g���Đ�����
				effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
				SetScalePlayingEffekseer3DEffect(effectHitPlayId_, BLAST_SCALE, BLAST_SCALE, BLAST_SCALE);
				SetRotationPlayingEffekseer3DEffect(effectHitPlayId_, AsoUtility::Deg2RadF(90.0f), 0.0f, 0.0f);
				SetPosPlayingEffekseer3DEffect(effectHitPlayId_, cPos.x, cPos.y, cPos.z);
			}

			//�e�̓����蔻��
			auto shots = plyer->GetShots();
			for (auto& shot : shots)
			{
				if (shot->IsShot() &&
					(vsPlyer->GetState() == Player::STATE::PLAY || vsPlyer->GetState() == Player::STATE::JUMP))
				{

					//�e�̓����蔻��
					diff = VSub(shot->GetPos(), vsPlyer->GetPos(2));
					disPow = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
					if (disPow < shot->GetCollisionRadius() * shot->GetCollisionRadius() + Player::DAMAGE_RADIUS * Player::DAMAGE_RADIUS)//�U�����a�~�U�����a
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

		VECTOR diff = VSub(AsoUtility::VECTOR_ZERO, plyer->GetTransform().pos);

		float disPow = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
		float distance = Player::DAMAGE_RADIUS * Player::DAMAGE_RADIUS + Stage::STAGE_RADIUS * Stage::STAGE_RADIUS;
		if (disPow > distance)//�_���[�W���a�~�U�����a
		{
			plyer->Damage(100);
			plyer->ChangeState(Player::STATE::DEAD);
		}
	}
	

}

//�f�o�b�O�`��
void GameScene::DrawDebug(void)
{
	//�O���b�h���̕`��
	stage_->DrawDebug();

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;


	int plyNum = 0;
	//�v���C���[�̍X�V
	for (auto& plyer : players_)
	{
		//HP�o�[�̕\��
		plyer->DrawPram(plyNum);
		plyNum += 1;
	}
	printf("Effect ID: %d, Position: (0, 0, 0), Scale: %f\n", effectHitPlayId_, BLAST_SCALE);

	DrawFormatString(0, 64, 0xff0000, "Effect ID: %d, Position: (0, 0, 0), Scale: %f\n", effectHitPlayId_, BLAST_SCALE);


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
