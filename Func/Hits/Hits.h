#pragma once
#include <algorithm>
#include "Struct.h"
#include "./Func/Vector/Vector.h"
#include "./Func/Draw/Draw.h"

/// <summary>
/// 当たり判定
/// </summary>
/// <param name="s1">球1</param>
/// <param name="s2">球2</param>
/// <returns>true or false</returns>
bool IsCollision(const Sphere& s1, const Sphere& s2);

/// <summary>
/// 当たり判定
/// </summary>
/// <param name="sphere">球</param>
/// <param name="plane">平面</param>
/// <returns>true or false</returns>
bool IsCollision(const Sphere& sphere, const Plane& plane);

/// <summary>
/// 当たり判定
/// </summary>
/// <param name="segment">線分</param>
/// <param name="plane">平面</param>
/// <returns>true or false</returns>
bool IsCollision(const Segment& segment, const Plane& plane);

/// <summary>
/// 当たり判定
/// </summary>
/// <param name="line">直線</param>
/// <param name="plane">平面</param>
/// <returns>true or false</returns>
bool IsCollision(const Line& line, const Plane& plane);

/// <summary>
/// 当たり判定
/// </summary>
/// <param name="ray">半直線</param>
/// <param name="plane">平面</param>
/// <returns>ture or false</returns>
bool IsCollision(const Ray& ray, const Plane& plane);

/// <summary>
/// 当たり判定
/// </summary>
/// <param name="triangle">三角形</param>
/// <param name="segment">線分</param>
/// <returns>true or false</returns>
bool IsCollision(const Triangle& triangle, const Segment& segment);

/// <summary>
/// 当たり判定
/// </summary>
/// <param name="triangle">三角形</param>
/// <param name="line">直線</param>
/// <returns>true or false</returns>
bool IsCollision(const Triangle& triangle, const Line& line);

/// <summary>
/// 当たり判定
/// </summary>
/// <param name="triangle">三角形</param>
/// <param name="ray">半直線</param>
/// <returns>true or false</returns>
bool IsCollision(const Triangle& triangle, const Ray& ray);

/// <summary>
/// 当たり判定
/// </summary>
/// <param name="aabb1">AABB1</param>
/// <param name="aabb2">AABB2</param>
/// <returns>true or false</returns>
bool IsCollision(const AABB& aabb1, const AABB aabb2);

/// <summary>
/// 当たり判定
/// </summary>
/// <param name="aabb">AABB</param>
/// <param name="sphere">球</param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Sphere& sphere);

/// <summary>
/// 当たり判定
/// </summary>
/// <param name="aabb">AABB</param>
/// <param name="segment">線分</param>
/// <returns>true or false</returns>
bool IsCollision(const AABB& aabb, const Segment& segment);

/// <summary>
/// 当たり判定
/// </summary>
/// <param name="aabb">AABB</param>
/// <param name="line">直線</param>
/// <returns>true or false</returns>
bool IsCollision(const AABB& aabb, const Line& line);

/// <summary>
/// 当たり判定
/// </summary>
/// <param name="aabb">AABB</param>
/// <param name="ray">半直線</param>
/// <returns>true or false</returns>
bool IsCollision(const AABB& aabb, const Ray& ray);