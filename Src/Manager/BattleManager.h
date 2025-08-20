#pragma once
#include "SceneManager.h"

class BattleManager
{

public:

	static constexpr int PLAYER_SIZE = 2;

	static constexpr int COLOR_SIZE = 2;
	static constexpr int CHARACTER_MAX = 4;//キャラの種類（色含む）

	enum class RESULT
	{
		NONE,
		PLAYER_ONE,
		PLAYER_TWO,
		DRAW,
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static BattleManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	//ゲームのリセット
	void ResetGame(void);
	//プレイキャラを扱う変数
	const int GetCharId(int i) const { return charcterId_[i]; }
	void SetCharcterId(int i, int setNum);
	
	const int GetColorId(int i) const { return charColorId_[i]; }
	void SetColorId(int i, int setNum);



	//勝者の変数を扱う関数
	const RESULT GetResult(void) const { return isWinner_; }
	void SetResult(RESULT win);//勝者の設定


private:

	// 静的インスタンス
	static BattleManager* instance_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	BattleManager(void);

	// デストラクタも同様
	~BattleManager(void) = default;


	//ゲームに使う変数
	int charcterId_[PLAYER_SIZE];
	int charColorId_[PLAYER_SIZE];

	RESULT isWinner_;

};

