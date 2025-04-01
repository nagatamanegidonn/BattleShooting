#pragma once
#include <map>
#include <vector>

class Controller
{

public:

	// 操作の種類
	enum class MODE
	{
		FORWARD,//前進
		BACK,	//後退
		LEFT,	//左切り替え
		RIGHT,	//右切り替え
		ATTACK,	//攻撃
		XBUTTUN,	//リロード
		JATTACK,
		MAX
	};

	Controller();  // コンストラクタ
	~Controller(); // デストラクタ

	void Init(int i);  // 初期化
	void Update();  // 更新
	void Draw();  // 描画

	void Reset();//入力をリセット

	void SetisControl(MODE mode, bool isBool);
	bool GetisControl(MODE mode);

private:

	//コントロール関係
	//std::map<MODE, std::vector<bool>> isControl_;
	bool isControl_[static_cast<int>(MODE::MAX)];

	int modeId_;
	void IsCommnd(void);
};

