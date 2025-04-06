/* -------------------------
�@�t���[�����[�g����n�f�[�^
 --------------------------- */

class FrameRate
{
public:

	


	/* --- <<< �֐��v���g�^�C�v�錾 >>> --- */

	FrameRate(void); // �f�t�H���g�R���X�g���N�^

	void Update(void); // �X�V����

	void Draw(void); // �`�揈��

	void SetFrameRate(void); // �t���[�����[�g ���蓖��

	// �t���[�����[�g���� ����
	bool GetLimitFrameRate(void);


private:

	/* --- <<< �萔��` >>> --- */

	// ���σt���[�����[�g
	static constexpr int FRAME_RATE_AVE = 60;

	// �ݒ�t���[�����[�g(�~���b�P��)
	static constexpr float FRAME_RATE = (1000 / FRAME_RATE_AVE);

	// �e�L�X�g���ʒu
	static constexpr int TEXT_POS_X = 65;

	// �e�L�X�g�c�ʒu
	static constexpr int TEXT_POS_Y = 15;


	/* --- <<< �ϐ���` >>> --- */

	int curTime_;  // ���݂̎���
	int lateTime_; // �O�t���[�����s����

	int counter_;	 // �t���[���J�E���^
	int updateTime_; // �t���[���X�V����

	float viewFramelate_; // �t���[�����[�g(�`��p)
};