#pragma once
#include <Novice.h>
#include <math.h>
#include "Struct.h"
#include "Const.h"

/// <summary>
/// 加算する
/// </summary>
/// <param name="v1">ベクトル1</param>
/// <param name="v2">ベクトル2</param>
/// <returns>加算したベクトル</returns>
Vector3 Add(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 減算する
/// </summary>
/// <param name="v1">ベクトル1</param>
/// <param name="v2">ベクトル2</param>
/// <returns>減産したベクトル</returns>
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

/// <summary>
/// スカラー倍する
/// </summary>
/// <param name="scalar">スカラー</param>
/// <param name="v">ベクトル</param>
/// <returns>スカラー倍したベクトル</returns>
Vector3 Multiply(float scalar, const Vector3& v);

/// <summary>
/// 内積を求める
/// </summary>
/// <param name="v1">ベクトル1</param>
/// <param name="v2">ベクトル2</param>
/// <returns>内積</returns>
float Dot(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 長さ（ノルム）を求める
/// </summary>
/// <param name="v">ベクトル</param>
/// <returns>長さ（ノルム）</returns>
float Length(const Vector3& v);

/// <summary>
/// 正規化する
/// </summary>
/// <param name="v">ベクトル</param>
/// <returns>正規化したベクトル</returns>
Vector3 Normalize(const Vector3& v);

/// <summary>
/// ベクトルを出力する
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="vector">ベクトル</param>
/// <param name="label">文字</param>
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

/// <summary>
/// クロス積を求める
/// </summary>
/// <param name="v1">ベクトル1</param>
/// <param name="v2">ベクトル2</param>
/// <returns>クロス積</returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 正射影ベクトルを求める
/// </summary>
/// <param name="v1">ベクトル1</param>
/// <param name="v2">ベクトル2</param>
/// <returns>正射影ベクトル</returns>
Vector3 Project(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 最近接点を求める
/// </summary>
/// <param name="point">点</param>
/// <param name="segment">線分</param>
/// <returns>最近接点</returns>
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

/// <summary>
/// 垂直ベクトルを求める
/// </summary>
/// <param name="vector">ベクトル</param>
/// <returns>垂直ベクトル</returns>
Vector3 Perpendiculer(const Vector3& vector);

/// <summary>
/// 線形補間を行う
/// </summary>
/// <param name="start">始点</param>
/// <param name="end">終点</param>
/// <param name="t">媒介変数</param>
/// <returns></returns>
Vector3 Leap(const Vector3& start, const Vector3& end, float t);

/// <summary>
/// ベジェ曲線の点を媒介変数を使って求める
/// </summary>
/// <param name="controlPoint0">制御点0</param>
/// <param name="controlPoint1">制御点1</param>
/// <param name="controlPoint2">制御点2</param>
/// <param name="t">媒介変数</param>
/// <returns></returns>
Vector3 Bezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, float t);

/// <summary>
/// 反射ベクトルを求める
/// </summary>
/// <param name="input">入力</param>
/// <param name="normal">法線</param>
/// <returns></returns>
Vector3 Reflect(const Vector3& input, const Vector3& normal);

namespace
{
	// 二項演算子
	Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Add(v1, v2); }
	Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Subtract(v1, v2); }
	Vector3 operator*(float s, const Vector3& v) { return Multiply(s, v); }
	Vector3 operator*(const Vector3& v, float s) { return s * v; }
	Vector3 operator/(const Vector3& v, float s) { return Multiply(1.0f / s, v); }

	// 単項演算子
	Vector3 operator-(const Vector3& v) { return { -v.x , -v.y , -v.z }; }
	Vector3 operator+(const Vector3& v) { return v; }
}