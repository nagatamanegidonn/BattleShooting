/* -------------------------
　フレームレート制御系データ
 --------------------------- */

class FrameRate
{
public:

	


	/* --- <<< 関数プロトタイプ宣言 >>> --- */

	FrameRate(void); // デフォルトコンストラクタ

	void Update(void); // 更新処理

	void Draw(void); // 描画処理

	void SetFrameRate(void); // フレームレート 割り当て

	// フレームレート制限 判定
	bool GetLimitFrameRate(void);


private:

	/* --- <<< 定数定義 >>> --- */

	// 平均フレームレート
	static constexpr int FRAME_RATE_AVE = 60;

	// 設定フレームレート(ミリ秒単位)
	static constexpr float FRAME_RATE = (1000 / FRAME_RATE_AVE);

	// テキスト横位置
	static constexpr int TEXT_POS_X = 65;

	// テキスト縦位置
	static constexpr int TEXT_POS_Y = 15;


	/* --- <<< 変数定義 >>> --- */

	int curTime_;  // 現在の時間
	int lateTime_; // 前フレーム実行時間

	int counter_;	 // フレームカウンタ
	int updateTime_; // フレーム更新時間

	float viewFramelate_; // フレームレート(描画用)
};