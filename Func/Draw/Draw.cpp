#include "Draw.h"

/// <summary>
/// 球を描画する
/// </summary>
/// <param name="sphere">球</param>
/// <param name="viewProjectionMatrix">ビュー * 透視投影　行列</param>
/// <param name="viewportMatrix">ビューポート行列</param>
/// <param name="color">色</param>
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	// 分割数
	const int kSubdivisions = 14;

	// 経度分割1つ分の角度
	const float kLonEvery = 2.0f * float(M_PI) / static_cast<float>(kSubdivisions);

	// 緯度分割1つ分の角度
	const float kLatEvery = float(M_PI) / static_cast<float>(kSubdivisions);


	// 緯度の方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivisions; ++latIndex)
	{
		// 現在の緯度
		float lat = -float(M_PI) / 2.0f + kLatEvery * latIndex;

		// 経度の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivisions; ++lonIndex)
		{
			// 現在の経度
			float lon = lonIndex * kLonEvery;


			// 3頂点のローカル座標
			Vector3 localVertices[3];

			localVertices[0] =
			{
				std::cos(lat) * std::cos(lon),
				std::sin(lat),
				std::cos(lat) * std::sin(lon)
			};

			localVertices[1] =
			{
				std::cos(lat + kLatEvery) * std::cos(lon),
				std::sin(lat + kLatEvery),
				std::cos(lat + kLatEvery) * std::sin(lon)
			};

			localVertices[2] =
			{
				std::cos(lat) * std::cos(lon + kLonEvery),
				std::sin(lat),
				std::cos(lat) * std::sin(lon + kLonEvery)
			};
			
			// ワールド行列
			Matrix4x4 worldMatrix = MakeAffineMatrix({ sphere.radius , sphere.radius,sphere.radius }, { 0.0f , 0.0f , 0.0f }, sphere.center);

			// ワールドビュープロジェクション行列
			Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);

			// 3頂点のスクリーン座標
			Vector3 screenVertices[3] = { 0.0f };

			for (uint32_t i = 0; i < 3; i++)
			{
				Vector3 ndcVertex = Transform(localVertices[i], worldViewProjectionMatrix);

				screenVertices[i] = Transform(ndcVertex, viewportMatrix);
			}

			Novice::DrawLine
			(
				static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
				static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y),
				color
			);

			Novice::DrawLine
			(
				static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
				static_cast<int>(screenVertices[2].x), static_cast<int>(screenVertices[2].y),
				color
			);
		}
	}
}

/// <summary>
/// グリッドを描画する
/// </summary>
/// <param name="viewProjectionMatrix">ビュー * 透視投影　行列</param>
/// <param name="viewportMatrix">ビューポート変換行列</param>
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	// グリッドの半分の幅
	const float kGridHalfWidth = 2.0f;

	// 分割数
	const int kSubdivisions = 16;

	// 長さ
	const float kGridEvery = (kGridHalfWidth * 2.0f) / static_cast<float>(kSubdivisions);


	// 奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivisions; ++xIndex)
	{
		// ワールド座標
		Vector3 worldVertices[2];

		worldVertices[0] =
		{
			-kGridHalfWidth + kGridEvery * static_cast<float>(xIndex),
			0.0f,
			-kGridHalfWidth,
		};

		worldVertices[1] =
		{
			-kGridHalfWidth + kGridEvery * static_cast<float>(xIndex),
			0.0f,
			kGridHalfWidth,
		};


		// スクリーン座標
		Vector3 screenVertices[2] = { 0.0f };

		for (uint32_t i = 0; i < 2; i++)
		{
			// ワールド行列
			Matrix4x4 worldMatrix = MakeAffineMatrix({ 0.0f , 0.0f ,0.0f }, { 0.0f , 0.0f , 0.0f }, worldVertices[i]);

			// ワールド * ビュープロジェクション　行列
			Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);

			Vector3 ndcVertex = Transform({ 1.0f , 1.0f , 1.0f }, Multiply(worldMatrix, viewProjectionMatrix));

			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}

		// 中心点は黒線
		if (xIndex == kSubdivisions / 2)
		{
			Novice::DrawLine
			(
				static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
				static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y),
				0x000000FF
			);
		}
		else
		{
			// その他はグレー

			Novice::DrawLine
			(
				static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
				static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y),
				0x999999FF
			);
		}
	}


	// 左から右への線を順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivisions; ++zIndex)
	{
		// ワールド座標
		Vector3 worldVertices[2];

		worldVertices[0] =
		{
			-kGridHalfWidth,
			0.0f,
			-kGridHalfWidth + kGridEvery * static_cast<float>(zIndex),
		};

		worldVertices[1] =
		{
			kGridHalfWidth,
			0.0f,
			-kGridHalfWidth + kGridEvery * static_cast<float>(zIndex),
		};


		// スクリーン座標
		Vector3 screenVertices[2] = { 0.0f };

		for (uint32_t i = 0; i < 2; i++)
		{
			// ワールド行列
			Matrix4x4 worldMatrix = MakeAffineMatrix({ 0.0f , 0.0f ,0.0f }, { 0.0f , 0.0f , 0.0f }, worldVertices[i]);

			// ワールド * ビュープロジェクション　行列
			Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);

			Vector3 ndcVertex = Transform({ 1.0f , 1.0f , 1.0f }, worldViewProjectionMatrix);

			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}

		// 中心は黒線
		if (zIndex == kSubdivisions / 2)
		{
			Novice::DrawLine
			(
				static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
				static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y),
				0x000000FF
			);
		} 
		else
		{
			// その他はグレー

			Novice::DrawLine
			(
				static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
				static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y),
				0x999999FF
			);
		}
	}
}

/// <summary>
/// 直線を描画する
/// </summary>
/// <param name="line">直線</param>
/// <param name="viewProjectionMatrix">ビュー * 透視投影　行列</param>
/// <param name="viewportMatrix">ビューポート変換行列</param>
/// <param name="color">色</param>
void DrawLine(const Line& line, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	// 始点
	Vector3 start = Transform(Transform(line.origin, viewProjectionMatrix), viewportMatrix);

	// 終点
	Vector3 end = Transform(Transform(Add(line.origin, line.diff), viewProjectionMatrix), viewportMatrix);

	Novice::DrawLine
	(
		static_cast<int>(start.x), static_cast<int>(start.y),
		static_cast<int>(end.x), static_cast<int>(end.y),
		color
	);
}

/// <summary>
/// 線分を描画する
/// </summary>
/// <param name="segment">線分</param>
/// <param name="viewProjectionMatrix">ビュー * 透視投影　行列</param>
/// <param name="viewportMatrix">ビューポート変換行列</param>
/// <param name="color">色</param>
void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	// 始点
	Vector3 start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
	Sphere startSphere = { segment.origin , 0.01f };

	// 終点
	Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), viewProjectionMatrix), viewportMatrix);
	Sphere endSphere = { Add(segment.origin , segment.diff) , 0.01f };

	Novice::DrawLine
	(
		static_cast<int>(start.x), static_cast<int>(start.y),
		static_cast<int>(end.x), static_cast<int>(end.y),
		color
	);

	DrawSphere(startSphere, viewProjectionMatrix, viewportMatrix, color);
	DrawSphere(endSphere, viewProjectionMatrix, viewportMatrix, color);
}


/// <summary>
/// 半直線を描画する
/// </summary>
/// <param name="ray">半直線</param>
/// <param name="viewProjectionMatrix">ビュー * 透視投影　行列</param>
/// <param name="viewportMatrix">ビューポート変換行列</param>
/// <param name="color">色</param>
void DrawRay(const Ray& ray, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	// 始点
	Vector3 start = Transform(Transform(ray.origin, viewProjectionMatrix), viewportMatrix);
	Sphere startSphere = { ray.origin , 0.01f };

	// 終点
	Vector3 end = Transform(Transform(Add(ray.origin, ray.diff), viewProjectionMatrix), viewportMatrix);

	Novice::DrawLine
	(
		static_cast<int>(start.x), static_cast<int>(start.y),
		static_cast<int>(end.x), static_cast<int>(end.y),
		color
	);

	DrawSphere(startSphere, viewProjectionMatrix, viewportMatrix, color);
}

/// <summary>
/// 平面を描画する
/// </summary>
/// <param name="plane">平面</param>
/// <param name="viewProjectionMatrix">ビュー * 透視投影　行列</param>
/// <param name="viewportMatrix">ビューポート変換行列</param>
/// <param name="color">色</param>
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	// 平面の中心点
	Vector3 center = Multiply(plane.distance, plane.normal);

	// 垂直ベクトル
	Vector3 perpendiculers[4];

	// 1. 法線と垂直なベクトル
	perpendiculers[0] = Normalize(Perpendiculer(plane.normal));

	// 2. 1の逆ベクトル
	perpendiculers[1] = { -perpendiculers[0].x ,-perpendiculers[0].y , -perpendiculers[0].z };

	// 3. 1と法線のクロス積
	perpendiculers[2] = Cross(plane.normal, perpendiculers[0]);

	// 4. 3の逆ベクトル
	perpendiculers[3] = { -perpendiculers[2].x , -perpendiculers[2].y , -perpendiculers[2].z };


	// 点
	Vector3 points[4];

	for (uint32_t index = 0; index < 4; index++)
	{
		Vector3 extend = Multiply(2.0f, perpendiculers[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawLine
	(
		static_cast<int>(points[0].x), static_cast<int>(points[0].y),
		static_cast<int>(points[3].x), static_cast<int>(points[3].y),
		color
	);

	Novice::DrawLine
	(
		static_cast<int>(points[3].x), static_cast<int>(points[3].y),
		static_cast<int>(points[1].x), static_cast<int>(points[1].y),
		color
	);

	Novice::DrawLine
	(
		static_cast<int>(points[1].x), static_cast<int>(points[1].y),
		static_cast<int>(points[2].x), static_cast<int>(points[2].y),
		color
	);

	Novice::DrawLine
	(
		static_cast<int>(points[2].x), static_cast<int>(points[2].y),
		static_cast<int>(points[0].x), static_cast<int>(points[0].y),
		color
	);
}

/// <summary>
/// 三角形を描画する
/// </summary>
/// <param name="triangle">三角形</param>
/// <param name="viewProjectionMatrix">ビュー * 透視投影　行列</param>
/// <param name="viewportMatrix">ビューポート変換行列</param>
/// <param name="color">色</param>
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	// スクリーン座標
	Vector3 screenVertices[3];

	for (uint32_t i = 0; i < 3; i++)
	{
		screenVertices[i] = Transform(Transform(triangle.vertices[i], viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawTriangle
	(
		static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
		static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y),
		static_cast<int>(screenVertices[2].x), static_cast<int>(screenVertices[2].y),
		color, kFillModeWireFrame
	);
}

/// <summary>
/// AABBを描画する
/// </summary>
/// <param name="aabb">AABB</param>
/// <param name="viewProjectionMatrix">ビュー * 透視投影　行列</param>
/// <param name="viewportMatrix">ビューポート変換行列</param>
/// <param name="color">色</param>
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	// スクリーン座標
	Vector3 screenVertices[8];

	screenVertices[0] = Transform(Transform(aabb.min, viewProjectionMatrix), viewportMatrix);
	screenVertices[1] = Transform(Transform({ aabb.max.x , aabb.min.y , aabb.min.z }, viewProjectionMatrix), viewportMatrix);
	screenVertices[2] = Transform(Transform({ aabb.min.x , aabb.min.y , aabb.max.z }, viewProjectionMatrix), viewportMatrix);
	screenVertices[3] = Transform(Transform({ aabb.max.x , aabb.min.y , aabb.max.z }, viewProjectionMatrix), viewportMatrix);
	screenVertices[4] = Transform(Transform({ aabb.min.x , aabb.max.y , aabb.min.z }, viewProjectionMatrix), viewportMatrix);
	screenVertices[5] = Transform(Transform({ aabb.max.x , aabb.max.y , aabb.min.z }, viewProjectionMatrix), viewportMatrix);
	screenVertices[6] = Transform(Transform({ aabb.min.x , aabb.max.y , aabb.max.z }, viewProjectionMatrix), viewportMatrix);
	screenVertices[7] = Transform(Transform(aabb.max, viewProjectionMatrix), viewportMatrix);


	Novice::DrawLine
	(
		static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
		static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y),
		color
	);

	Novice::DrawLine
	(
		static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
		static_cast<int>(screenVertices[2].x), static_cast<int>(screenVertices[2].y),
		color
	);

	Novice::DrawLine
	(
		static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y),
		static_cast<int>(screenVertices[3].x), static_cast<int>(screenVertices[3].y),
		color
	);

	Novice::DrawLine
	(
		static_cast<int>(screenVertices[2].x), static_cast<int>(screenVertices[2].y),
		static_cast<int>(screenVertices[3].x), static_cast<int>(screenVertices[3].y),
		color
	);


	Novice::DrawLine
	(
		static_cast<int>(screenVertices[4].x), static_cast<int>(screenVertices[4].y),
		static_cast<int>(screenVertices[5].x), static_cast<int>(screenVertices[5].y),
		color
	);

	Novice::DrawLine
	(
		static_cast<int>(screenVertices[4].x), static_cast<int>(screenVertices[4].y),
		static_cast<int>(screenVertices[6].x), static_cast<int>(screenVertices[6].y),
		color
	);

	Novice::DrawLine
	(
		static_cast<int>(screenVertices[5].x), static_cast<int>(screenVertices[5].y),
		static_cast<int>(screenVertices[7].x), static_cast<int>(screenVertices[7].y),
		color
	);

	Novice::DrawLine
	(
		static_cast<int>(screenVertices[6].x), static_cast<int>(screenVertices[6].y),
		static_cast<int>(screenVertices[7].x), static_cast<int>(screenVertices[7].y),
		color
	);


	Novice::DrawLine
	(
		static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
		static_cast<int>(screenVertices[4].x), static_cast<int>(screenVertices[4].y),
		color
	);

	Novice::DrawLine
	(
		static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y),
		static_cast<int>(screenVertices[5].x), static_cast<int>(screenVertices[5].y),
		color
	);

	Novice::DrawLine
	(
		static_cast<int>(screenVertices[2].x), static_cast<int>(screenVertices[2].y),
		static_cast<int>(screenVertices[6].x), static_cast<int>(screenVertices[6].y),
		color
	);

	Novice::DrawLine
	(
		static_cast<int>(screenVertices[3].x), static_cast<int>(screenVertices[3].y),
		static_cast<int>(screenVertices[7].x), static_cast<int>(screenVertices[7].y),
		color
	);
}

/// <summary>
/// ベジェ曲線を描画する
/// </summary>
/// <param name="controlPoint0">制御点0</param>
/// <param name="controlPoint1">制御点1</param>
/// <param name="controlPoint2">制御点2</param>
/// <param name="viewProjectionMatrix">ビュー * 透視投影　行列</param>
/// <param name="viewportMatrix">ビューポート変換行列</param>
/// <param name="color">色</param>
void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2,
	const Matrix4x4& viewProjectionMatrix, const Matrix4x4 viewportMatrix, uint32_t color)
{
	// 分割数
	const uint32_t kSubdivisions = 32;

	for (uint32_t index = 0; index < kSubdivisions; index++)
	{
		float t0 = static_cast<float>(index) / static_cast<float>(kSubdivisions);
		float t1 = static_cast<float>(index + 1) / static_cast<float>(kSubdivisions);

		Vector3 bezier0 = Bezier(controlPoint0, controlPoint1, controlPoint2, t0);
		Vector3 bezier1 = Bezier(controlPoint0, controlPoint1, controlPoint2, t1);

		Vector3 screenBezeir0 = Transform((Transform(bezier0, viewProjectionMatrix)), viewportMatrix);
		Vector3 screenBezeir1 = Transform((Transform(bezier1, viewProjectionMatrix)), viewportMatrix);

		Novice::DrawLine
		(
			static_cast<int>(screenBezeir0.x), static_cast<int>(screenBezeir0.y),
			static_cast<int>(screenBezeir1.x), static_cast<int>(screenBezeir1.y),
			color
		);
	}

	// 制御点の球
	Sphere sphere[3] =
	{
		{controlPoint0 , 0.01f},
		{controlPoint1 , 0.01f},
		{controlPoint2 , 0.01f}
	};

	// 球を描画する
	for (uint32_t i = 0; i < 3; i++)
	{
		DrawSphere(sphere[i], viewProjectionMatrix, viewportMatrix, 0x000000FF);
	}
}