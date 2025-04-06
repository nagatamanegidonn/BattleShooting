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
	//ロードが完了したか判断
	//if (GetASyncLoadNum() == 0) { return; }

	SetFontSize(28);//文字のサイズを設定
	// TCHAR 型を使う場合、std::basic_string<TCHAR> を使う
	std::basic_string<TCHAR> loadStr = _T("now loading");  // TCHAR型の文字列リテラル
	std::basic_string<TCHAR> dotStr = _T(".");  // ドットもTCHAR型で

	const int count = SceneManager::GetInstance().LoadCunt() % 4;

	for (int i = 0; i < count; i++) {
		loadStr += dotStr;  // ドットを追加
	}

	// DrawStringToHandle に渡す際も、TCHAR型の文字列を使用
	//DrawStringToHandle(950, 750, loadStr.c_str(), 0xffffff, 0);
	DrawFormatString(500, 300, 0xffffff, loadStr.c_str());
	SetFontSize(16);
}

void SceneBase::Release(void)
{
}

bool SceneBase::IsLoad()
{
	//ロードが完了したか判断
	if ((GetASyncLoadNum() == 0) && (SceneManager::GetInstance().LoadCunt() > 3))
	{
		return true;
	}

	return false;
}
