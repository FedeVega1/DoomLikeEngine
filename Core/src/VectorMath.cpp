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
}
