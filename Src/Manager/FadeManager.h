#pragma once

#include <functional>
#include <memory>  // shared_ptrを使うために必要(Controll)
#include <DxLib.h>
#include <map>
#include <vector>

//フェードのサポート名称違う
class FadeManager
{

public:

	enum class SCENE{
		NORMAL,
		SELECT,
		GAME,
	};

	struct ObectUI
	{
		int handleId = -1;
		VECTOR pos = {0.0f,0.0f,0.0f};
		int animIndex = 0;
		float speed = 0.0f;
		float totalTime = 0.0f;
		float step = 0.0f;
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static FadeManager& GetInstance(void);

	// 初期化
	void Init(void);
	void Draw(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);


	// 状態遷移
	void ChangeState(const SCENE scene);

	int GetLoadCnt(void)const { return loadMinCnt_; }

private:

	// 静的インスタンス
	static FadeManager* instance_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	FadeManager(void);

	// デストラクタも同様
	~FadeManager(void) = default;


	std::map<SCENE, std::vector<std::unique_ptr<ObectUI>>> objectUI_;


	// 状態管理(状態遷移時初期処理)
	std::map<SCENE, std::function<void(void)>> sceneChanges_;
	// 状態管理(ステップ)
	std::function<void(void)> sceneDraw_;

	//最低ロード時間
	int loadMinCnt_;

	int manualImg_;

	// 状態遷移
	void ChangeSceneNone(void);
	void ChangeSceneGame(void);


	//描画処理
	void FadeNormalDraw(void);
	void FadeGameDraw(void);

};

