#include <Novice.h>
#include <iostream>
#include "Struct.h"
#include "Const.h"
#include "./Func/Vector/Vector.h"
#include "./Func/Matrix/Matrix.h"
#include "./Func/Draw/Draw.h"
#include "./Func/Hits/Hits.h"
#include "imgui.h"

const char kWindowTitle[] = "LE2A_11_フクダソウワ_MT3";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };


	/*---------------
		変数を作る
	---------------*/

	// カメラの移動・回転
	Vector3 cameraTranslate = { 0.0f , 1.9f , -6.49f };
	Vector3 cameraRotate = { 0.26f , 0.0f , 0.0f };

	// カーソルの位置を取得する変数
	int getCursorPointX = 0;
	int getCursorPointY = 0;

	// カーソルの始点
	Vector2 cursorOrigin = { 0.0f , 0.0f };

	// カーソルの終点
	Vector2 cursorDiff = { 0.0f , 0.0f };

	// カーソルによる回転量
	Vector2 cursorRotate = { 0.0f , 0.0f };

	// カーソルによる総回転量
	Vector2 cursorRotateSum = { 0.0f , 0.0f };


	// 振り子
	Pendulum pendulum =
	{
		.anchor = {0.0f , 1.0f , 0.0f},
		.length = 0.8f,
		.angle = 0.7f,
		.anglerVelocity = 0.0f,
		.anglerAcceleration = 0.0f,
		.point = {0.0f , 0.0f , 0.0f}
	};

	// 振り子が揺れているかどうか
	uint32_t isPendulumShake = false;

	// デルタタイム
	float deltaTime = 1.0f / 60.0f;



	/*   振り子の初期位置を求める   */

	pendulum.anglerAcceleration =
		-(9.8f / pendulum.length) * std::sin(pendulum.angle);
	pendulum.anglerVelocity += pendulum.anglerAcceleration * deltaTime;
	pendulum.angle += pendulum.anglerVelocity * deltaTime;

	pendulum.point.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
	pendulum.point.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
	pendulum.point.z = pendulum.anchor.z;



	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {

		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		ImGui::Begin("Window1");
		ImGui::DragFloat3("cameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("cameraRotate", &cameraRotate.x, 0.01f);

		if (isPendulumShake == false)
		{
			if (ImGui::Button("start"))
			{
				isPendulumShake = true;
			}
		} else
		{
			if (ImGui::Button("stop"))
			{
				isPendulumShake = false;
			}
		}

		ImGui::End();



		/*----------------
			カメラ操作
		----------------*/

		// Shiftキーを押しながら、マウスの左を押し続けて、スクロールする
		if (keys[DIK_RSHIFT] || keys[DIK_LSHIFT])
		{
			if (Novice::IsPressMouse(0))
			{
				// 押した瞬間のカーソルの始点を取得する
				if (Novice::IsTriggerMouse(0))
				{
					Novice::GetMousePosition(&getCursorPointX, &getCursorPointY);
					cursorOrigin = { static_cast<float>(getCursorPointX) , static_cast<float>(getCursorPointY) };

					// カメラの回転量を記録する
					cursorRotateSum = { cameraRotate.x , cameraRotate.y };
				}

				// カーソルの終点を取得する
				Novice::GetMousePosition(&getCursorPointX, &getCursorPointY);
				cursorDiff = { static_cast<float>(getCursorPointX) - cursorOrigin.x , static_cast<float>(getCursorPointY) - cursorOrigin.y };

				// 今のカーソルでの回転量を求める
				cursorRotate.y = cursorDiff.x / 1000.0f;
				cursorRotate.x = cursorDiff.y / 1000.0f;

				// 記録した回転量と、今の回転量を加算する
				cameraRotate.x = cursorRotate.x + cursorRotateSum.x;
				cameraRotate.y = cursorRotate.y + cursorRotateSum.y;
			}
		}



		/*----------------
			振り子の動き
		----------------*/

		if (isPendulumShake)
		{
			pendulum.anglerAcceleration =
				-(9.8f / pendulum.length) * std::sin(pendulum.angle);
			pendulum.anglerVelocity += pendulum.anglerAcceleration * deltaTime;
			pendulum.angle += pendulum.anglerVelocity * deltaTime;

			pendulum.point.x = pendulum.anchor.x + std::sin(pendulum.angle) * pendulum.length;
			pendulum.point.y = pendulum.anchor.y - std::cos(pendulum.angle) * pendulum.length;
			pendulum.point.z = pendulum.anchor.z;
		}

		// 描画する線
		Segment segment;
		segment.origin = pendulum.anchor;
		segment.diff = { std::sin(pendulum.angle) * pendulum.length ,-std::cos(pendulum.angle) * pendulum.length , 0.0f };

		// 描画する球
		Sphere sphere;
		sphere.center = pendulum.point;
		sphere.radius = 0.05f;



		/*-------------------
			座標変換の行列
		-------------------*/

		// ビュー行列
		Matrix4x4 viewMatrix = Inverse(MakeAffineMatrix({ 1.0f , 1.0f , 1.0f }, cameraRotate, cameraTranslate));

		// 透視投影行列
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight), 0.1f, 100.0f);

		// ビューポート行列
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, static_cast<float>(kWindowWidth), static_cast<float>(kWindowHeight), 0.0f, 1.0f);


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッド
		DrawGrid(Multiply(viewMatrix, projectionMatrix), viewportMatrix);

		// 線分
		DrawSegment(segment, Multiply(viewMatrix, projectionMatrix), viewportMatrix, 0xFFFFFFFF);
		DrawSphere(sphere, Multiply(viewMatrix, projectionMatrix), viewportMatrix, 0xFFFFFFFF);


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
