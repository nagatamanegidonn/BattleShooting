#pragma once
#include <chrono>
class SceneBase;
class Fader;
class Camera;

class SceneManager
{

public:

	// �V�[���Ǘ��p
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		SELECT,
		GAME,
		RESULT
	};

	enum class WINNER
	{
		NONE,
		PLAYER_ONE,
		PLAYER_TWO,
	};
	
	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);

	// ���\�[�X�̔j��
	void Destroy(void);

	// ��ԑJ��
	void ChangeScene(SCENE_ID nextId);

	//�Q�[���̃��Z�b�g
	void ResetGame(void);
	WINNER GetWinner(void) const;//���҂̎擾
	void SetWinner(WINNER win);//���҂̐ݒ�

	SCENE_ID GetSceneID(void);// �V�[��ID�̎擾

	// �f���^�^�C���̎擾
	float GetDeltaTime(void) const;

	// �J�����̎擾
	Camera* GetCamera(void) const;


	// ���[�h�������ׂ�
	bool IsLoading(void) const;

private:

	// �ÓI�C���X�^���X
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// �t�F�[�h
	Fader* fader_;

	// �e��V�[��
	SceneBase* scene_;

	// �J����
	Camera* camera_;

	// �V�[���J�ڒ�����
	bool isSceneChanging_;

	// �f���^�^�C��
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	

	//�Q�[���Ɏg���ϐ�
	WINNER isWinner_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	SceneManager(void);
	// �R�s�[�R���X�g���N�^�����l
	SceneManager(const SceneManager&);
	// �f�X�g���N�^�����l
	~SceneManager(void) = default;

	// �f���^�^�C�������Z�b�g����
	void ResetDeltaTime(void);

	// �V�[���J��
	void DoChangeScene(SCENE_ID sceneId);

	// �t�F�[�h
	void Fade(void);

};