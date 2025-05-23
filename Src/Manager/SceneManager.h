#pragma once
#include <chrono>
class SceneBase;
class Fader;
class Camera;

class SceneManager
{

public:

	static constexpr int PLAYER_SIZE = 2;

	// シーン管理用
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
		DRAW,
	};
	
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);

	// リソースの破棄
	void Destroy(void);

	// 状態遷移
	void ChangeScene(SCENE_ID nextId);

	//ゲームのリセット
	void ResetGame(void);
//プレイキャラを扱う変数
	int GetPlayerId(int i) const;
	void SetPlayerId(int i,int setNum);
//勝者の変数を扱う関数
	WINNER GetWinner(void) const;
	void SetWinner(WINNER win);//勝者の設定

	SCENE_ID GetSceneID(void);// シーンIDの取得

	// デルタタイムの取得
	float GetDeltaTime(void) const;
	// カメラの取得
	Camera* GetCamera(void) const;


	// ロード中か調べる
	bool IsLoading(void) const;
	int LoadCunt(void) const;

private:

	// 静的インスタンス
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// フェード
	Fader* fader_;

	// 各種シーン
	SceneBase* scene_;

	// カメラ
	Camera* camera_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	

	//ゲームに使う変数
	int playerId_[PLAYER_SIZE];

	WINNER isWinner_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);
	// コピーコンストラクタも同様
	SceneManager(const SceneManager&);
	// デストラクタも同様
	~SceneManager(void) = default;

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	// フェード
	void Fade(void);

};