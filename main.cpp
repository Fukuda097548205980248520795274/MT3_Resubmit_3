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


	/// [0] 肩   [1] 肘   [2] 手

	// 移動
	Vector3 translate[3] =
	{
		{0.2f , 1.0f , 0.0f},
		{0.4f , 0.0f , 0.0f},
		{0.3f , 0.0f , 0.0f}
	};

	// 回転
	Vector3 rotate[3] =
	{
		{0.0f , 0.0f , -6.8f},
		{0.0f , 0.0f , -1.4f},
		{0.0f , 0.0f , 0.0f}
	};

	// 拡縮
	Vector3 scale[3] =
	{
		{1.0f , 1.0f , 1.0f},
		{1.0f , 1.0f , 1.0f},
		{1.0f , 1.0f , 1.0f}
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
		ImGui::DragFloat3("translate[0]", &translate[0].x, 0.01f);
		ImGui::DragFloat3("rotate[0]", &rotate[0].x, 0.01f);
		ImGui::DragFloat3("scale[0]", &scale[0].x, 0.01f);
		ImGui::DragFloat3("translate[1]", &translate[1].x, 0.01f);
		ImGui::DragFloat3("rotate[1]", &rotate[1].x, 0.01f);
		ImGui::DragFloat3("scale[1]", &scale[1].x, 0.01f);
		ImGui::DragFloat3("translate[2]", &translate[2].x, 0.01f);
		ImGui::DragFloat3("rotate[2]", &rotate[2].x, 0.01f);
		ImGui::DragFloat3("scale[2]", &scale[2].x, 0.01f);
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
			階層構造を作る
		-------------------*/

		Matrix4x4 worldMatrix[3];
		worldMatrix[0] = MakeAffineMatrix(scale[0], rotate[0], translate[0]);
		worldMatrix[1] = Multiply(MakeAffineMatrix(scale[1], rotate[1], translate[1]), worldMatrix[0]);
		worldMatrix[2] = Multiply(MakeAffineMatrix(scale[2], rotate[2], translate[2]), worldMatrix[1]);

		// 球
		Sphere sphere[3];

		for (uint32_t i = 0; i < 3; i++)
		{
			sphere[i].center = { worldMatrix[i].m[3][0] , worldMatrix[i].m[3][1] , worldMatrix[i].m[3][2] };
			sphere[i].radius = 0.05f;
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


		// 点のスクリーン座標
		Vector3 screenPoint[3];
		for (uint32_t i = 0; i < 3; i++)
		{
			screenPoint[i] = Transform(Transform(sphere[i].center, Multiply(viewMatrix, projectionMatrix)), viewportMatrix);
		}


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// グリッド
		DrawGrid(Multiply(viewMatrix, projectionMatrix), viewportMatrix);


		// 部位をつなぐ線
		Novice::DrawLine
		(
			static_cast<int>(screenPoint[0].x), static_cast<int>(screenPoint[0].y),
			static_cast<int>(screenPoint[1].x), static_cast<int>(screenPoint[1].y),
			0xFFFFFFFF
		);

		Novice::DrawLine
		(
			static_cast<int>(screenPoint[1].x), static_cast<int>(screenPoint[1].y),
			static_cast<int>(screenPoint[2].x), static_cast<int>(screenPoint[2].y),
			0xFFFFFFFF
		);

		// 部位の球
		DrawSphere(sphere[0], Multiply(viewMatrix, projectionMatrix), viewportMatrix, 0xFF0000FF);
		DrawSphere(sphere[1], Multiply(viewMatrix, projectionMatrix), viewportMatrix, 0x00FF00FF);
		DrawSphere(sphere[2], Multiply(viewMatrix, projectionMatrix), viewportMatrix, 0x0000FFFF);


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
