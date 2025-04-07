#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		TITLE_LOGO,
		START_LOGO,
		PLAY_LOGO,
		EXIT_LOGO,

		CURSOR,
		FRAME,

		ALERT_MSG,
		LIGHT,
		SKYDOME,
		DUNGEON,
		SHOT_EXPLOSION,
		ALPHABET,
		SPEECH_BALLOON,

		P1_IMAGE, // P1キノコ画像
		P2_IMAGE, // P2キノコ画像
		P3_IMAGE, // P3画像
		P4_IMAGE, // P4画像

		SHOT_MODEL,
		P1_SHOT_MODEL, // P1キノコ弾モデル
		P2_SHOT_MODEL, // P2キノコ弾モデル

		ROCK01,
		ROCK02,
		BACKGROUNDCITY,
		BACKGROUNDFOREST,

		JET,
		BOOST,
		PLAYER_SHIP_EXPLOSION,
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	Resource Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource*> loadedMap_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);

	// デストラクタも同様
	~ResourceManager(void) = default;

	// 内部ロード
	Resource* _Load(SRC src);

};

