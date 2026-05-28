#include "pch.h"

#include <algorithm>
#include <format>

#include "Core/VectorMath.h"

namespace Core
{
    int FloatToInt(const float& value) { return static_cast<int>(std::round(value)); }

    Vector2Int Vector2Int::Clamp(const Vector2Int& vector, const int& min, const int& max)
    {
        return Vector2Int(std::clamp(vector.x, min, max), std::clamp(vector.y, min, max));
    }

    std::string Vector2Int::ToString() const { return std::format("({}, {})", x, y); }

    Vector2 Vector2::Normalize(const Vector2& v)
    {
        float mag = v.Magnitude();
        if (mag < K_EPSILON) return ZERO;
        return { v.x / mag, v.y / mag };
    }

    float Vector2::Distance(const Vector2& a, const Vector2& b)
    {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    Vector2 Vector2::Clamp(const Vector2& vector, const float& min, const float& max)
    { 
        return Vector2(std::clamp(vector.x, min, max), std::clamp(vector.y, min, max)); 
    }

    std::string Vector2::ToString() const { return std::format("({}, {})", x, y); }

    Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
    {
        return
        {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }

    Vector3 Vector3::Normalize(const Vector3& v)
    {
        float mag = v.Magnitude();
        if (mag < K_EPSILON) return ZERO;
        return { v.x / mag, v.y / mag, v.z / mag };
    }

    float Vector3::Distance(const Vector3& a, const Vector3& b)
    {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        float dz = b.z - a.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    Vector3 Vector3::Clamp(const Vector3& vector, const float& min, const float& max)
    {
        return Vector3(std::clamp(vector.x, min, max), std::clamp(vector.y, min, max), std::clamp(vector.z, min, max));
    }

    std::string Vector3::ToString() const { return std::format("({}, {}, {})", x, y, z); }

    Vector2 Vector2::Abs(const Vector2& vector) { return Vector2(std::abs(vector.x), std::abs(vector.y)); }

    bool Vector2::IsPointOnSegment(const Vector2& point, const Vector2& segA, const Vector2& segB)
    {
        Vector2 ab = segB - segA;
        Vector2 ap = point - segA;
        if (std::abs(Cross(ab, ap)) > K_EPSILON) return false;
        float dot = Dot(ab, ap);
        return dot >= 0.f && dot <= Dot(ab, ab);
    }

    bool Vector2::IsPointInsideSegment(const Vector2& point, const Vector2& segA, const Vector2& segB)
    {
        Vector2 ab = segB - segA;
        Vector2 ap = point - segA;
        if (std::abs(Cross(ab, ap)) > K_EPSILON) return false;
        float dot = Dot(ab, ap);
        float abLenSq = Dot(ab, ab);
        return dot > K_EPSILON && dot < abLenSq - K_EPSILON;
    }

    bool Vector2::DoSegmentsIntersect(const Vector2& p1, const Vector2& p2, const Vector2& q1, const Vector2& q2)
    {
        Vector2 pq = p2 - p1;
        Vector2 ab = q2 - q1;
        float d1 = Cross(ab, p1 - q1);
        float d2 = Cross(ab, p2 - q1);
        float d3 = Cross(pq, q1 - p1);
        float d4 = Cross(pq, q2 - p1);

        if (d1 * d2 < 0.f && d3 * d4 < 0.f) return true;

        if (std::abs(Cross(ab, p1 - q1)) > K_EPSILON) return false;
        if (std::abs(Cross(ab, p2 - q1)) > K_EPSILON) return false;

        float abLenSq = Dot(ab, ab);
        float tP1 = Dot(ab, p1 - q1);
        float tP2 = Dot(ab, p2 - q1);
        return std::max(tP1, tP2) > K_EPSILON && std::min(tP1, tP2) < abLenSq - K_EPSILON;
    }

    bool Vector3::IsPointOnSegment(const Vector3& point, const Vector3& segA, const Vector3& segB)
    {
        Vector3 ab = segB - segA;
        Vector3 ap = point - segA;
        if (Cross(ab, ap).Magnitude() > K_EPSILON) return false;
        float dot = Dot(ab, ap);
        return dot >= 0.f && dot <= Dot(ab, ab);
    }

    bool Vector3::IsPointInsideSegment(const Vector3& point, const Vector3& segA, const Vector3& segB)
    {
        Vector3 ab = segB - segA;
        Vector3 ap = point - segA;
        if (Cross(ab, ap).Magnitude() > K_EPSILON) return false;
        float dot = Dot(ab, ap);
        float abLenSq = Dot(ab, ab);
        return dot > K_EPSILON && dot < abLenSq - K_EPSILON;
    }

    bool Vector3::DoSegmentsIntersect(const Vector3& p1, const Vector3& p2, const Vector3& q1, const Vector3& q2)
    {
        Vector3 d1 = p2 - p1;
        Vector3 d2 = q2 - q1;
        Vector3 w = p1 - q1;

        float a = Dot(d1, d1);
        float b = Dot(d1, d2);
        float c = Dot(d1, w);
        float e = Dot(d2, d2);
        float f = Dot(d2, w);
        float denom = a * e - b * b;

        if (std::abs(denom) < K_EPSILON) return false;

        float s = (b * f - c * e) / denom;
        float t = (a * f - b * c) / denom;

        if (s < 0.f || s > 1.f || t < 0.f || t > 1.f) return false;

        return (p1 + d1 * s - (q1 + d2 * t)).Magnitude() < K_EPSILON;
    }
}
