#pragma once
#include <stdint.h>

// 2次元ベクトル
struct Vector2
{
	float x;
	float y;
};

// 3次元ベクトル
struct Vector3
{
	float x;
	float y;
	float z;

	Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
	Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }
};

// 4x4行列
struct Matrix4x4
{
	float m[4][4];
};

// 球
struct Sphere
{
	// 中心点
	Vector3 center;

	// 半径
	float radius;
};

// 線分
struct Segment
{
	// 始点
	Vector3 origin;

	// 終点への差分ベクトル
	Vector3 diff;
};

// 直線
struct Line
{
	// 始点
	Vector3 origin;

	// 終点への差分ベクトル
	Vector3 diff;
};

// 半直線
struct Ray
{
	// 始点
	Vector3 origin;

	// 終点への差分ベクトル
	Vector3 diff;
};

// 平面
struct Plane
{
	// 法線
	Vector3 normal;

	// 距離
	float distance;
};

// 三角形
struct Triangle
{
	// 頂点
	Vector3 vertices[3];
};

// AABB
struct AABB
{
	// 最小点
	Vector3 min;

	// 最大点
	Vector3 max;
};

// ばね
struct Spring
{
	// 固定された位置
	Vector3 anchor;

	// 自然長
	float naturalLength;

	// 剛性
	float siffiness;

	// 減衰係数
	float dampingCoefficent;
};

// ボール
struct Ball
{
	// 位置
	Vector3 position;

	// 速度
	Vector3 velocity;

	// 加速度
	Vector3 acceleration;

	// 質量
	float mass;

	// 半径
	float radius;

	// 色
	uint32_t color;
};

// 円運動
struct CircularMotion
{
	// 中心点
	Vector3 center;

	// 角度
	float angle;

	// 角速度
	float anglerVelocity;

	// 軌道の半径
	float orbitRadius;


	// 点
	Vector3 point;

	// 点の半径
	float pointRadius;
};

// 振り子
struct Pendulum
{
	// アンカーポイント　固定された位置
	Vector3 anchor;

	// 紐の長さ
	float length;

	// 現在の角度
	float angle;

	// 角速度ω
	float anglerVelocity;

	// 角加速度
	float anglerAcceleration;

	// 点（先端）
	Vector3 point;

};

// 円錐振り子
struct ConicalPendulum
{
	// アンカーポイント　固定された位置
	Vector3 anchor;

	// 紐の長さ
	float length;

	// 円錐の頂角の半分
	float halfApexAngle;

	// 現在の角度
	float angle;

	// 角速度
	float anglerVelocity;

	// 点（先端）
	Vector3 point;
};