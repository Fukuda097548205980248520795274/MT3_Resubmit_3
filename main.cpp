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


	// 円運動
	CircularMotion circularMotion;
	circularMotion.center = { 0.0f , 0.0f , 0.0f };
	circularMotion.angle = 0.0f;
	circularMotion.anglerVelocity = float(M_PI);
	circularMotion.orbitRadius = 0.8f;
	circularMotion.point = { 0.0f , 0.0f , 0.0f };
	circularMotion.pointRadius = 0.03f;

	// 点の初期位置
	circularMotion.point =
	{
		circularMotion.center.x + std::cos(circularMotion.angle) * circularMotion.orbitRadius,
		circularMotion.center.y + std::sin(circularMotion.angle) * circularMotion.orbitRadius,
		circularMotion.center.z
	};


	// 回転しているかどうか（回転フラグ）
	int isRotation = false;

	// デルタタイム
	float deltaTime = 1.0f / 60.0f;



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

		// 回転していない（回転フラグがfalse）であるときに押すと、回転する（回転フラグがtrueになる）
		if (isRotation == false)
		{
			if (ImGui::Button("start"))
			{
				isRotation = true;
			}
		} else
		{
			// 回転している（回転フラグがtrue）であるときに押すと、停止する（回転フラグがfalseになる）
			if (ImGui::Button("stop"))
			{
				isRotation = false;
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


		/*-----------
			円運動
		-----------*/

		// 回転する（回転フラグがtrueのとき）
		if (isRotation)
		{
			// 角度を動かす
			circularMotion.angle += circularMotion.anglerVelocity * deltaTime;

			// 点の位置
			circularMotion.point.x = circularMotion.center.x + std::cos(circularMotion.angle) * circularMotion.orbitRadius;
			circularMotion.point.y = circularMotion.center.y + std::sin(circularMotion.angle) * circularMotion.orbitRadius;
			circularMotion.point.z = circularMotion.center.z;
		}


		// 球の描画
		Sphere sphere;
		sphere.center = circularMotion.point;
		sphere.radius = circularMotion.pointRadius;




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

		// 球
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
