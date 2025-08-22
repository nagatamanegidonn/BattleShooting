#include <DxLib.h>
#include <string>

#include "../Application.h"

#include "../Manager/SceneManager.h"
#include "../Manager/FadeManager.h"
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
	DrawStringToHandle(Application::SCREEN_SIZE_X - 200, Application::SCREEN_SIZE_Y - 50, loadStr.c_str(), 0xffffff
		, SceneManager::GetInstance().GetFont());

	SetFontSize(16);
}

void SceneBase::Release(void)
{
}

bool SceneBase::IsLoad()
{
	//ロードが完了したか判断
	if ((GetASyncLoadNum() == 0) && (SceneManager::GetInstance().LoadCunt() > FadeManager::GetInstance().GetLoadCnt()))
	{
		return true;
	}

	return false;
}
