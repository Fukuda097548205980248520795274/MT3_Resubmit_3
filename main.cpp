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


	// 制御点
	Vector3 controlPoint[3] =
	{
		{-0.8f , 0.58f , 1.0f},
		{1.76f , 1.0f , -0.3f},
		{0.94f , -0.7f , 2.3f}
	};



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

		ImGui::Begin("Window");
		ImGui::DragFloat3("cameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("cameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("controlPoint[0]", &controlPoint[0].x, 0.01f);
		ImGui::DragFloat3("controlPoint[1]", &controlPoint[1].x, 0.01f);
		ImGui::DragFloat3("controlPoint[2]", &controlPoint[2].x, 0.01f);
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

		// ベジェ曲線
		DrawBezier(controlPoint[0], controlPoint[1], controlPoint[2], Multiply(viewMatrix, projectionMatrix), viewportMatrix, 0x0000FFFF);


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
