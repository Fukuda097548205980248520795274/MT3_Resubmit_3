#pragma once
#include <Novice.h>
#include <stdint.h>
#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>
#include "Struct.h"

/// <summary>
/// 行列の加法
/// </summary>
/// <param name="m1">行列1</param>
/// <param name="m2">行列2</param>
/// <returns>行列を加算した値</returns>
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>
/// 行列の減法
/// </summary>
/// <param name="m1">行列1</param>
/// <param name="m2">行列2</param>
/// <returns>行列を減産した値</returns>
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>
/// 行列の積
/// </summary>
/// <param name="m1">行列1</param>
/// <param name="m2">行列2</param>
/// <returns>行列の積</returns>
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>
/// 逆行列
/// </summary>
/// <param name="m">行列</param>
/// <returns>逆行列</returns>
Matrix4x4 Inverse(const Matrix4x4& m);

/// <summary>
/// 転置行列
/// </summary>
/// <param name="m">行列</param>
/// <returns>転置行列</returns>
Matrix4x4 Transpose(const Matrix4x4& m);

/// <summary>
/// 単位行列の作成
/// </summary>
/// <returns>単位行列</returns>
Matrix4x4 MakeIdenity4x4();

/// <summary>
/// 行列を出力する
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="matrix">行列</param>
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label);

/// <summary>
/// 平行移動行列を作る
/// </summary>
/// <param name="translate">移動</param>
/// <returns>平行移動行列</returns>
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

/// <summary>
/// 拡大縮小行列を作る
/// </summary>
/// <param name="scale">拡縮</param>
/// <returns>拡大縮小行列</returns>
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

/// <summary>
/// 座標変換を行う
/// </summary>
/// <param name="vector">ベクトル</param>
/// <param name="matrix">行列</param>
/// <returns>変換した座標</returns>
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

/// <summary>
/// X軸の回転行列を作る
/// </summary>
/// <param name="radian">ラジアン</param>
/// <returns>X軸の回転行列</returns>
Matrix4x4 MakeRotateXMatrix(float radian);

/// <summary>
/// Y軸の回転行列を作る
/// </summary>
/// <param name="radian">ラジアン</param>
/// <returns>Y軸の回転行列</returns>
Matrix4x4 MakeRotateYMatrix(float radian);

/// <summary>
/// Z軸の回転行列を作る
/// </summary>
/// <param name="radian">ラジアン</param>
/// <returns>Z軸の回転行列</returns>
Matrix4x4 MakeRotateZMatrix(float radian);

/// <summary>
/// 3次元アフィン変換行列を作る
/// </summary>
/// <param name="scale">拡縮</param>
/// <param name="rotate">回転</param>
/// <param name="translate">移動</param>
/// <returns>3次元アフィン変換行列</returns>
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

/// <summary>
/// 透視投影行列を作る
/// </summary>
/// <param name="fovY">画角</param>
/// <param name="aspectRatio">アスペクト比</param>
/// <param name="nearClip">近平面との距離</param>
/// <param name="farClip">遠平面との距離</param>
/// <returns>透視投影行列</returns>
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

/// <summary>
/// 正射影行列を作る
/// </summary>
/// <param name="left">左</param>
/// <param name="top">上</param>
/// <param name="right">右</param>
/// <param name="bottom">下</param>
/// <param name="nearClip">近平面との距離</param>
/// <param name="farClip">遠平面との距離</param>
/// <returns>正射影行列</returns>
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

/// <summary>
/// ビューポート変換行列を作る
/// </summary>
/// <param name="left">左</param>
/// <param name="top">上</param>
/// <param name="width">幅</param>
/// <param name="height">高さ</param>
/// <param name="minDepth">最小深度値</param>
/// <param name="maxDepth">最大深度値</param>
/// <returns>ビューポート変換行列</returns>
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

namespace
{
	// 二項演算子
	Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4 m2) { return Add(m1, m2); }
	Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4 m2) { return Subtract(m1, m2); }
	Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4 m2) { return Multiply(m1, m2); }
}