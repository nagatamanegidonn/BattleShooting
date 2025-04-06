#include <DxLib.h>
#include <string>
#include "../Manager/SceneManager.h"
#include "SceneBase.h"

SceneBase::SceneBase(void)
{
}

SceneBase::~SceneBase(void)
{
}

void SceneBase::AsyncPreLoad(void)
{
}

void SceneBase::Init(void)
{
}

void SceneBase::Update(void)
{
}

void SceneBase::Draw(void)
{
}
void SceneBase::LoadingDraw(void)
{
	//���[�h���������������f
	//if (GetASyncLoadNum() == 0) { return; }

	SetFontSize(28);//�����̃T�C�Y��ݒ�
	// TCHAR �^���g���ꍇ�Astd::basic_string<TCHAR> ���g��
	std::basic_string<TCHAR> loadStr = _T("now loading");  // TCHAR�^�̕����񃊃e����
	std::basic_string<TCHAR> dotStr = _T(".");  // �h�b�g��TCHAR�^��

	const int count = SceneManager::GetInstance().LoadCunt() % 4;

	for (int i = 0; i < count; i++) {
		loadStr += dotStr;  // �h�b�g��ǉ�
	}

	// DrawStringToHandle �ɓn���ۂ��ATCHAR�^�̕�������g�p
	//DrawStringToHandle(950, 750, loadStr.c_str(), 0xffffff, 0);
	DrawFormatString(500, 300, 0xffffff, loadStr.c_str());
	SetFontSize(16);
}

void SceneBase::Release(void)
{
}

bool SceneBase::IsLoad()
{
	//���[�h���������������f
	if ((GetASyncLoadNum() == 0) && (SceneManager::GetInstance().LoadCunt() > 3))
	{
		return true;
	}

	return false;
}
