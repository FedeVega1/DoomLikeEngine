#pragma once

static float kEpsilon = .00001f;

struct Vector2Int
{
	int x, y;

	Vector2Int(int x = 0, int y = 0)
	{
		this->x = x;
		this->y = y;
	}

	Vector2Int operator+(Vector2Int other) const { return Plus(other); }
	void operator+=(Vector2Int other)
	{
		Vector2Int res = Plus(other);
		x = res.x;
		y = res.y;
	}

	Vector2Int operator-(Vector2Int other) const { return Minus(other); }
	void operator-=(Vector2Int other)
	{
		Vector2Int res = Minus(other);
		x = res.x;
		y = res.y;
	}

	Vector2Int operator-() const { return Vector2Int(-x, -y); }
	Vector2Int operator*(int s) const { return ScalarMult(s); }
	Vector2Int operator/(int s) const { return ScalarDiv(s); }
	bool operator==(Vector2Int other) const { return Equals(other); }

	static Vector2Int Clamp(Vector2Int vector, int min, int max) { return Vector2Int(std::clamp(vector.x, min, max), std::clamp(vector.y, min, max)); }

	inline std::string ToString() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ")"; }

private:
	Vector2Int Plus(Vector2Int other) const { return Vector2Int(x + other.x, y + other.y); }
	Vector2Int Minus(Vector2Int other) const { return Vector2Int(x - other.x, y - other.y); }

	Vector2Int ScalarMult(int scalar) const { return Vector2Int(x * scalar, y * scalar); }
	Vector2Int ScalarDiv(int scalar) const { return Vector2Int(x / scalar, y / scalar); }

	bool Equals(Vector2Int other) const { return x == other.x && y == other.y; }
};

struct Vector2
{
	float x, y;

	Vector2(float x = 0, float y = 0)
	{
		this->x = x;
		this->y = y;
	}

	Vector2(Vector2Int other)
	{
		x = static_cast<float>(other.x);
		y = static_cast<float>(other.y);
	}

	Vector2 operator+(Vector2 other) const { return Plus(other); }
	void operator+=(Vector2 other)
	{
		Vector2 res = Plus(other);
		x = res.x;
		y = res.y;
	}

	Vector2 operator-(Vector2 other) const { return Minus(other); }
	void operator-=(Vector2 other)
	{
		Vector2 res = Minus(other);
		x = res.x;
		y = res.y;
	}

	Vector2 operator-() const { return Vector2(-x, -y); }
	Vector2 operator*(float s) const { return ScalarMult(s); }
	Vector2 operator/(float s) const { return ScalarDiv(s); }
	bool operator==(Vector2 other) const { return Equals(other); }

	static float Distance(Vector2 a, Vector2 b)
	{
		Vector2 diff = Vector2(a.x - b.x, a.y - b.y);
		return std::sqrtf(diff.x * diff.x + diff.y * diff.y);
	}

	static Vector2 Clamp(Vector2 vector, float min, float max) { return Vector2(std::clamp(vector.x, min, max), std::clamp(vector.y, min, max)); }

	inline std::string ToString() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ")"; }

private:
	Vector2 Plus(Vector2 other) const { return Vector2(x + other.x, y + other.y); }
	Vector2 Minus(Vector2 other) const { return Vector2(x - other.x, y - other.y); }

	Vector2 ScalarMult(float scalar) const { return Vector2(x * scalar, y * scalar); }
	Vector2 ScalarDiv(float scalar) const { return Vector2(x / scalar, y / scalar); }

	bool Equals(Vector2 other, float e = .00001f) const { return (std::abs(x - other.x) <= e) && (std::abs(y - other.y) <= e); }
};

struct Vector3
{
	float x, y, z;

	Vector3(float x = 0, float y = 0, float z = 0)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3(Vector2 other)
	{
		x = other.x;
		y = other.y;
		z = 0;
	}

	Vector3(Vector2 other, float z)
	{
		x = other.x;
		y = other.y;
		this->z = z;
	}

	Vector3 operator+(Vector3 other) const { return Plus(other); }
	void operator+=(Vector3 other)
	{ 
		Vector3 res = Plus(other);
		x = res.x;
		y = res.y;
		z = res.z;
	}

	Vector3 operator-(Vector3 other) const { return Minus(other); }
	void operator-=(Vector3 other)
	{
		Vector3 res = Minus(other);
		x = res.x;
		y = res.y;
		z = res.z;
	}

	Vector3 operator-() const { return Vector3(-x, -y, -z); }
	Vector3 operator*(float s) const { return ScalarMult(s); }
	Vector3 operator/(float s) const { return ScalarDiv(s); }
	bool operator==(Vector3 other) const { return Equals(other); }

	static Vector3 Cross(Vector3 a, Vector3 b)
	{
		return Vector3(
			(a.x * b.z) - (a.z * b.y),
			(a.z * b.x) - (a.x * b.z),
			(a.x * b.y) - (a.y * b.x)
		);
	}

	static Vector3 Normalize(Vector3 vector)
	{
		float magnitude = vector.Magnitude();
		if (magnitude > kEpsilon) return vector / magnitude;
		return Vector3();
	}

	static float Dot(Vector3 a, Vector3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

	static float Distance(Vector3 a, Vector3 b)
	{
		Vector3 diff = Vector3(a.x - b.x, a.y - b.y , a.z - b.z);
		return std::sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
	}

	static Vector3 Clamp(Vector3 vector, float min, float max) { return Vector3(std::clamp(vector.x, min, max), std::clamp(vector.y, min, max), std::clamp(vector.z, min, max)); }

	float Magnitude() const { return std::sqrtf(x * x + y * y + z * z); }

	inline std::string ToString() const { return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")"; }

	Vector2 XY() { return Vector2(x, y); }
	Vector2 XZ() { return Vector2(x, z); }
	Vector2 YZ() { return Vector2(y, z); }

	void SetXY(float x, float y) { this->x = x; this->y = y; }
	void SetXY(Vector2 xy) { x = xy.x; y = xy.y; }

	void AddXY(float x, float y) { this->x += x; this->y += y; }
	void AddXY(Vector2 xy) { x += xy.x; y += xy.y; }

	void SetXZ(float x, float z) { this->x = x; this->z = z; }
	void SetXZ(Vector2 xz) { x = xz.x; z = xz.y; }

	void AddXZ(float x, float z) { this->x += x; this->z += z; }
	void AddXZ(Vector2 xz) { x += xz.x; z += xz.y; }

	void SetYZ(float y, float z) { this->y = y; this->z = z; }
	void SetYZ(Vector2 yz) { y = yz.x; z = yz.y; }

	void AddYZ(float y, float z) { this->y += y; this->z += z; }
	void AddYZ(Vector2 yz) { y += yz.x; z += yz.y; }

private:
	Vector3 Plus(Vector3 other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
	Vector3 Minus(Vector3 other) const { return Vector3(x - other.x, y - other.y, z - other.z); }

	Vector3 ScalarMult(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
	Vector3 ScalarDiv(float scalar) const { return Vector3(x / scalar, y / scalar, z / scalar); }

	bool Equals(Vector3 other, float e = .00001f) const { return (std::abs(x - other.x) <= e) && (std::abs(y - other.y) <= e) && (std::abs(z - other.z) <= e); }
};

static const Vector2Int V2INT_ZERO = Vector2Int(0, 0);
static const Vector2Int V2INT_ONE = Vector2Int(1, 1);
static const Vector2Int V2INT_LEFT = Vector2Int(-1, 0);
static const Vector2Int V2INT_RIGHT = Vector2Int(1, 0);
static const Vector2Int V2INT_DOWN = Vector2Int(0, -1);
static const Vector2Int V2INT_UP = Vector2Int(0, 1);

static const Vector2 V2_ZERO = Vector2(0.0f, 0.0f);
static const Vector2 V2_ONE = Vector2(1.0f, 1.0f);
static const Vector2 V2_LEFT = Vector2(-1.0f, 0.0f);
static const Vector2 V2_RIGHT = Vector2(1.0f, 0.0f);
static const Vector2 V2_DOWN = Vector2(0.0f, -1.0f);
static const Vector2 V2_UP = Vector2(0.0f, 1.0f);

static const Vector3 V3_ZERO = Vector3(0.0f, 0.0f, 0.0f);
static const Vector3 V3_ONE = Vector3(1.0f, 1.0f, 1.0f);
static const Vector3 V3_LEFT = Vector3(-1.0f, 0.0f, 0.0f);
static const Vector3 V3_RIGHT = Vector3(1.0f, 0.0f, 0.0f);
static const Vector3 V3_BACK = Vector3(0.0f, -1.0f, 0.0f);
static const Vector3 V3_FORWARD = Vector3(0.0f, 1.0f, 0.0f);
static const Vector3 V3_DOWN = Vector3(0.0f, 0.0f, -1.0f);
static const Vector3 V3_UP = Vector3(0.0f, 0.0f, 1.0f);
