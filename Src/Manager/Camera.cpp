#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"

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
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	}

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

}

#pragma region mode_による描画方法

void Camera::SetBeforeDrawFixedPoint(void)
{
	// 何もしない
}
void Camera::SetBeforeDrawFree(void)
{
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

VECTOR Camera::GetPos(void) const
{
	return pos_;
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
