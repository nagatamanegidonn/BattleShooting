#pragma once
#include<map>
#include"Sound.h"

class SoundManager
{
public:
	//リソース名
	enum class SRC
	{
		TITLE_BGM,		//タイトルシーンのBGM
		RULE_BGM,		//ルールシーンのBGM
		GAME_BGM,		//ゲームシーンのBGM
		BTTLE_BGM,		//ゲームシーンのBGM

		VICTORY_BGM,

		ATTRCK,
		DAMAGE,

		KEEN,//金属音

		WIN_BGM,		//勝った時のBGM
		DRAW_BGM,		//引き分けの時のBGM
		LOSE_BGM,		//負けた時のBGM

		ENTER_SOUND,	//決定音
		SELECT_SOUND,	//選択しているときの効果音
		DAMAGE_SOUND,	//ダメージを受けた時の効果音
		HEAL_SOUND,		//回復した時の効果音
		POISON_SOUND,	//毒になった時の効果音

	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static SoundManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	bool Play(SRC src, Sound::TIMES times);	//二次元音源用
	bool Play(SRC src, Sound::TIMES times, VECTOR pos, float radius);	//三次元音源用

	void Stop(SRC src);	//音源を停止する
	void AllStop(void);	//音源を停止する

	bool CheckMove(SRC src);

	void ChengeVolume(SRC src, float per);	//per 0.0～1.0でパーセントを設定する
private:
	// 静的インスタンス
	static SoundManager* instance_;

	std::map<SRC, Sound> soundMap_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SoundManager(void) = default;

	// デストラクタも同様
	~SoundManager(void) = default;
};
