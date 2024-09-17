#pragma once

static float kEpsilon = .00001f;

typedef struct _vector2
{
	float x, y;

	_vector2(float x = 0, float y = 0)
	{
		this->x = x;
		this->y = y;
	}

	_vector2 operator+(_vector2 other) const { return Plus(other); }
	_vector2 operator-(_vector2 other) const { return Minus(other); }

	_vector2 operator*(float s) const { return ScalarMult(s); }
	_vector2 operator/(float s) const { return ScalarDiv(s); }
	bool operator==(_vector2 other) const { return Equals(other); }

private:
	_vector2 Plus(_vector2 other) const { return _vector2(x + other.x, y + other.y); }
	_vector2 Minus(_vector2 other) const { return _vector2(x - other.x, y - other.y); }

	_vector2 ScalarMult(float scalar) const { return _vector2(x * scalar, y * scalar); }
	_vector2 ScalarDiv(float scalar) const { return _vector2(x / scalar, y / scalar); }

	bool Equals(_vector2 other, float e = .00001f) const { return (std::abs(x - other.x) <= e) && (std::abs(y - other.y) <= e); }

} Vector2;

typedef struct _vector3
{
	float x, y, z;

	_vector3(float x = 0, float y = 0, float z = 0)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	_vector3(Vector2 other)
	{
		x = other.x;
		y = other.y;
		z = 0;
	}

	_vector3 operator+(_vector3 other) const { return Plus(other); }
	_vector3 operator-(_vector3 other) const { return Minus(other); }

	_vector3 operator*(float s) const { return ScalarMult(s); }
	_vector3 operator/(float s) const { return ScalarDiv(s); }
	bool operator==(_vector3 other) const { return Equals(other); }

	static _vector3 Cross(_vector3 a, _vector3 b)
	{
		return _vector3(
			(a.x * b.z) - (a.z * b.y),
			(a.z * b.x) - (a.x * b.z),
			(a.x * b.y) - (a.y * b.x)
		);
	}

	static _vector3 Normalize(_vector3 vector)
	{
		float magnitude = vector.Magnitude();
		if (magnitude > kEpsilon) return vector / magnitude;
		return _vector3();
	}

	static float Dot(_vector3 a, _vector3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

	float Magnitude() const { return std::sqrtf(x * x + y * y + z * z); }

private:
	_vector3 Plus(_vector3 other) const { return _vector3(x + other.x, y + other.y, z + other.z); }
	_vector3 Minus(_vector3 other) const { return _vector3(x - other.x, y - other.y, z - other.z); }

	_vector3 ScalarMult(float scalar) const { return _vector3(x * scalar, y * scalar, z * scalar); }
	_vector3 ScalarDiv(float scalar) const { return _vector3(x / scalar, y / scalar, z / scalar); }

	bool Equals(_vector3 other, float e = .00001f) const { return (std::abs(x - other.x) <= e) && (std::abs(y - other.y) <= e) && (std::abs(z - other.z) <= e); }
} Vector3;
