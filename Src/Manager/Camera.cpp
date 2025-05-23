#include <EffekseerForDXLib.h>

#include "../Utility/AsoUtility.h"
#include "InputManager.h"
#include "SceneManager.h"

#include "../Object/Common/Transform.h"

#include "Camera.h"

Camera::Camera(void)
{
	mode_ = MODE::NONE;
	pos_ = { 0.0f, 0.0f, 0.0f };
	targetPos_ = { 0.0f, 0.0f, 0.0f };
	rot_ = Quaternion::Identity();
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	// カメラの初期設定
	SetDefault();
}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::NONE:
		SetBeforeDrawNone();
		break;
	case Camera::MODE::FIXED_POINT:

		//カメラの上方向更新
		cameraUp_ = rot_.GetForward();
		pos_ = { 0.0f, 1000.0f, 0.0f };
		targetPos_ = { 0.0f, -100.0f, 0.0f };

		/*rot_ = Quaternion::Identity();

		Quaternion rotPow = Quaternion();

		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(90.0f), AsoUtility::AXIS_X
			));
		
		 回転諒を加える(合成)
		rot_ = rot_.Mult(rotPow);
		SetBeforeDrawFixedPoint();
		pos_ = { 0.0f, 1000.0f , 0.0f};
		targetPos_ = { 0.0f, 0.0f, 0.0f };*/
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	case Camera::MODE::FOLLOW_POINT:
		SetBeforeDrawFollowPoint();
		break;
	case Camera::MODE::SHAKE:
		SetBeforeDrawShake();
		break;
	}

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
}

#pragma region mode_による描画方法

void Camera::SetBeforeDrawFixedPoint(void)
{
	// 何もしない
}
void Camera::SetBeforeDrawFree(void)
{
	auto& ins = InputManager::GetInstance();
	// WASDでカメラの位置を変える
	float movePow = 3.0f;

	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }

	//回転軸と量決め
	const float ROT_POW = 1.0f;
	VECTOR axisDeg = AsoUtility::VECTOR_ZERO;

	if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x = ROT_POW; }
	if (ins.IsNew(KEY_INPUT_UP)) { axisDeg.x = -ROT_POW; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y = ROT_POW; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { axisDeg.y = -ROT_POW; }

	if (!AsoUtility::EqualsVZero(axisDeg))
	{
		//今回回転させたい回転量をクォータニオンで作る
		Quaternion rotPow = Quaternion();

		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.z), AsoUtility::AXIS_Z
			));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.x), AsoUtility::AXIS_X
			));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.y), AsoUtility::AXIS_Y
			));

		// 回転諒を加える(合成)
		rot_ = rot_.Mult(rotPow);

		// 注視点の相対座標を回転させる
		VECTOR rotLocalPos = rot_.PosAxis(RELATIVE_C2T_POS);
		//相対座標をワールド座標にする（注視点の更新）
		targetPos_ = VAdd(pos_, rotLocalPos);

		//カメラの上方向更新
		cameraUp_ = rot_.GetUp();
	}

	if (!AsoUtility::EqualsVZero(moveDir))
	{
		//　移動＝座標＋移動量
		//移動量＝方向×スピード

		//入力された方向をかめらの回転情報を使って
		//カメラの進行方向に変換する
		VECTOR direction = rot_.PosAxis(moveDir);
		//移動量
		const float speed = SPEED / 2;
		VECTOR movePow = VScale(direction, speed);
		//
		pos_ = VAdd(pos_, movePow);
		targetPos_ = VAdd(targetPos_, movePow);

	}
}
void Camera::SetBeforeDrawFollow(void)
{
	// 追従対象の位置
	VECTOR followPos = followTransform_->pos;
	// 追従対象の向き
	Quaternion followRot = followTransform_->quaRot;
	// 追従対象からカメラまでの相対座標
	VECTOR relativeCPos = followRot.PosAxis(RELATIVE_F2C_POS_FOLLOW);
	// カメラ位置の更新
	pos_ = VAdd(followPos, relativeCPos);
	// カメラ位置から注視点までの相対座標
	VECTOR relativeTPos = followRot.PosAxis(RELATIVE_C2T_POS);
	// 注視点の更新
	targetPos_ = VAdd(pos_, relativeTPos);
	// カメラの上方向
	cameraUp_ = followRot.PosAxis(rot_.GetUp());
}
void Camera::SetBeforeDrawFollowPoint(void)
{

	//カメラの上方向更新
	cameraUp_ = rot_.GetForward();

	// 追従対象の位置
	VECTOR followPos = followTransform_->pos;
	VECTOR followSubPos = followSubTransform_->pos;

	VECTOR cPos = VScale(VAdd(followPos, followSubPos), 0.5f);
	cPos.y = pos_.y;

	pos_ = cPos;
	targetPos_ = pos_;
	targetPos_.y = -100.0f;
}
void Camera::SetBeforeDrawShake(void)
{
	// 一定時間カメラを揺らす
	stepShake_ -= SceneManager::GetInstance().GetDeltaTime();
	if (stepShake_ < 0.0f)
	{
		pos_ = defaultPos_;
		ChangeMode(MODE::NONE);
		return;
	}

	// -1.0f〜1.0f
	float f = sinf(stepShake_ * SPEED_SHAKE);
	// -1000.0f〜1000.0f
	f *= 1000.0f;
	// -1000 or 1000
	int d = static_cast<int>(f);
	// 0 or 1
	int shake = d % 2;
	// -1 or 1
	shake -= 1;

	// 追従対象の位置
	VECTOR followPos = followTransform_->pos;
	followPos.y = 50.0f;
	// 追従対象の向き
	Quaternion followRot = followTransform_->quaRot;
	// 追従対象からカメラまでの相対座標
	VECTOR relativeCPos = followRot.PosAxis(RELATIVE_F2C_POS_FOLLOW);
	// カメラ位置の更新
	defaultPos_ = VAdd(followPos, relativeCPos);

	// 移動量
	VECTOR velocity = VScale(shakeDir_, shake * WIDTH_SHAKE);
	// 移動先座標
	pos_ = VAdd(defaultPos_, velocity);
	//float pow = WIDTH_SHAKE * sinf(stepShake_ * SPEED_SHAKE);
	//VECTOR velocity = VScale(shakeDir_, pow);
	//VECTOR newPos = VAdd(defaultPos_, velocity);
	//pos_ = newPos;

	
	// カメラ位置から注視点までの相対座標
	VECTOR relativeTPos = followRot.PosAxis(RELATIVE_C2T_POS);
	// 注視点の更新
	targetPos_ = VAdd(pos_, relativeTPos);
	// カメラの上方向
	cameraUp_ = followRot.PosAxis(rot_.GetUp());
}
void Camera::SetBeforeDrawNone(void)
{
	// 追従対象の位置
	VECTOR followPos = followTransform_->pos;
	followPos.y = 50.0f;
	// 追従対象の向き
	Quaternion followRot = followTransform_->quaRot;
	// 追従対象からカメラまでの相対座標
	VECTOR relativeCPos = followRot.PosAxis(RELATIVE_F2C_POS_FOLLOW);
	// カメラ位置の更新
	pos_ = VAdd(followPos, relativeCPos);
	// カメラ位置から注視点までの相対座標
	VECTOR relativeTPos = followRot.PosAxis(RELATIVE_C2T_POS);
	// 注視点の更新
	targetPos_ = VAdd(pos_, relativeTPos);
	// カメラの上方向
	cameraUp_ = followRot.PosAxis(rot_.GetUp());
}

#pragma endregion



void Camera::Draw(void)
{
}
void Camera::Release(void)
{
}


void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}
void Camera::SetSubFollow(const Transform* follow)
{
	followSubTransform_ = follow;
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

//カメラのズーム
void Camera::FadeIn(void)
{	
	if (pos_.y < 300.0f)
	{
		return;
	}
	pos_.y--;
}
void Camera::FadeOut(void)
{
	pos_.y += 2.0f;
}

void Camera::ChangeMode(MODE mode)
{

	// カメラの初期設定
	SetDefault();

	// カメラモードの変更
	mode_ = mode;

	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FOLLOW:
		break;
	case Camera::MODE::FOLLOW_POINT:
		pos_ = { 0.0f, 500.0f, 0.0f };
		break;
	case Camera::MODE::SHAKE:
		stepShake_ = TIME_SHAKE;
		shakeDir_ = VNorm({ 0.7f, 0.7f ,0.0f });
		defaultPos_ = pos_;
		break;
	}

}

void Camera::SetDefault(void)
{

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = VAdd(pos_, RELATIVE_C2T_POS);

	// カメラの上方向
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	// カメラはX軸に傾いているが、
	// この傾いた状態を角度ゼロ、傾き無しとする
	rot_ = Quaternion::Identity();

}
