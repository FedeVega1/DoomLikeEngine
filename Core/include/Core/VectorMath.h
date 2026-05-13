#pragma once

#include <cmath>
#include <numbers>

namespace Core
{
    inline constexpr float K_EPSILON = 0.00001f;

    struct Vector2;
    struct Vector3;

    int FloatToInt(const float& value);

    struct Vector2Int
    {
        int x, y;

        Vector2Int() : x(0), y(0) {}
        Vector2Int(int x, int y) : x(x), y(y) {}

        Vector2Int operator+(const Vector2Int& other) const { return Vector2Int(x + other.x, y + other.y); }
        Vector2Int operator-(const Vector2Int& other) const { return Vector2Int(x - other.x, y - other.y); }
        Vector2Int operator-() const { return Vector2Int(-x, -y); }
        Vector2Int operator*(int scalar) const { return Vector2Int(x * scalar, y * scalar); }
        Vector2Int operator/(int scalar) const { return Vector2Int(x / scalar, y / scalar); }
        Vector2Int& operator+=(const Vector2Int& other) { x += other.x; y += other.y; return *this; }
        Vector2Int& operator-=(const Vector2Int& other) { x -= other.x; y -= other.y; return *this; }
        bool operator==(const Vector2Int& other) const { return x == other.x && y == other.y; }

        static Vector2Int Clamp(const Vector2Int& value, const Vector2Int& min, const Vector2Int& max);
        static Vector2Int Clamp(const Vector2Int& vector, const int& min, const int& max);
        std::string ToString() const;

        static const Vector2Int ZERO;
        static const Vector2Int ONE;
        static const Vector2Int LEFT;
        static const Vector2Int RIGHT;
        static const Vector2Int DOWN;
        static const Vector2Int UP;
    };

    struct Vector2
    {
        float x, y;

        Vector2() : x(0), y(0) {}
        Vector2(float x, float y) : x(x), y(y) {}
        explicit Vector2(const Vector2Int& v) : x(static_cast<float>(v.x)), y(static_cast<float>(v.y)) {}

        Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
        Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
        Vector2 operator-(const Vector2Int& other) const { return Vector2(x - other.x, y - other.y); }
        Vector2 operator-() const { return Vector2(-x, -y); }
        Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
        Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }
        Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; return *this; }
        Vector2& operator-=(const Vector2& other) { x -= other.x; y -= other.y; return *this; }
        Vector2& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
        Vector2& operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }
        bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }

        float Magnitude() const { return std::sqrtf(x * x + y * y); }
        std::string ToString() const;

        static Vector2 Normalize(const Vector2& v);
        static float Distance(const Vector2& a, const Vector2& b);
        static float Cross(const Vector2& a, const Vector2& b) { return a.x * b.y - a.y * b.x; }
        static Vector2 Clamp(const Vector2& vector, const float& min, const float& max);

        static const Vector2 ZERO;
        static const Vector2 ONE;
        static const Vector2 LEFT;
        static const Vector2 RIGHT;
        static const Vector2 DOWN;
        static const Vector2 UP;
    };

    struct Vector3
    {
        float x, y, z;

        Vector3() : x(0), y(0), z(0) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
        explicit Vector3(const Vector2& v, float z = 0) : x(v.x), y(v.y), z(z) {}

        Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
        Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
        Vector3 operator-() const { return Vector3(-x, -y, -z); }
        Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
        Vector3 operator/(float scalar) const { return Vector3(x / scalar, y / scalar, z / scalar); }
        Vector3& operator+=(const Vector3& other) { x += other.x; y += other.y; z += other.z; return *this; }
        Vector3& operator-=(const Vector3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
        Vector3& operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
        Vector3& operator/=(float scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }
        bool operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z; }

        float Magnitude() const { return std::sqrtf(x * x + y * y + z * z); }
        Vector2 XY() const { return Vector2(x, y); }
        Vector2 XZ() const { return Vector2(x, z); }
        Vector2 YZ() const { return Vector2(y, z); }
        void SetXY(float nx, float ny) { x = nx; y = ny; }
        void SetXZ(float nx, float nz) { x = nx; z = nz; }
        void AddXY(float dx, float dy) { x += dx; y += dy; }
        std::string ToString() const;

        static Vector3 Cross(const Vector3& a, const Vector3& b);
        static float Dot(const Vector3& a, const Vector3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
        static Vector3 Normalize(const Vector3& v);
        static float Distance(const Vector3& a, const Vector3& b);
        static Vector3 Clamp(const Vector3& value, const Vector3& min, const Vector3& max);
        static Vector3 Clamp(const Vector3& vector, const float& min, const float& max);

        static const Vector3 ZERO;
        static const Vector3 ONE;
        static const Vector3 LEFT;
        static const Vector3 RIGHT;
        static const Vector3 BACK;
        static const Vector3 FORWARD;
        static const Vector3 DOWN;
        static const Vector3 UP;
    };

    // Precomputed sin/cos table for integer degrees 0-359
    struct SinCosTable
    {
        double sinValues[360];
        double cosValues[360];

        SinCosTable()
        {
            for (int i = 0; i < 360; i++)
            {
                double radians = i * std::numbers::pi / 180.0;
                sinValues[i] = std::sin(radians);
                cosValues[i] = std::cos(radians);
            }
        }
    };

    inline const SinCosTable SCTABLE;

    inline const Vector2Int Vector2Int::ZERO = Vector2Int(0, 0);
    inline const Vector2Int Vector2Int::ONE = Vector2Int(1, 1);
    inline const Vector2Int Vector2Int::LEFT = Vector2Int(-1, 0);
    inline const Vector2Int Vector2Int::RIGHT = Vector2Int(1, 0);
    inline const Vector2Int Vector2Int::DOWN = Vector2Int(0, -1);
    inline const Vector2Int Vector2Int::UP = Vector2Int(0, 1);

    inline const Vector2 Vector2::ZERO = Vector2(0.0f, 0.0f);
    inline const Vector2 Vector2::ONE = Vector2(1.0f, 1.0f);
    inline const Vector2 Vector2::LEFT = Vector2(-1.0f, 0.0f);
    inline const Vector2 Vector2::RIGHT = Vector2(1.0f, 0.0f);
    inline const Vector2 Vector2::DOWN = Vector2(0.0f, -1.0f);
    inline const Vector2 Vector2::UP = Vector2(0.0f, 1.0f);

    inline const Vector3 Vector3::ZERO = Vector3(0.0f, 0.0f, 0.0f);
    inline const Vector3 Vector3::ONE = Vector3(1.0f, 1.0f, 1.0f);
    inline const Vector3 Vector3::LEFT = Vector3(-1.0f, 0.0f, 0.0f);
    inline const Vector3 Vector3::RIGHT = Vector3(1.0f, 0.0f, 0.0f);
    inline const Vector3 Vector3::BACK = Vector3(0.0f, -1.0f, 0.0f);
    inline const Vector3 Vector3::FORWARD = Vector3(0.0f, 1.0f, 0.0f);
    inline const Vector3 Vector3::DOWN = Vector3(0.0f, 0.0f, -1.0f);
    inline const Vector3 Vector3::UP = Vector3(0.0f, 0.0f, 1.0f);
}
