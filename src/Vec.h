#ifndef VEC_H
#define VEC_H
//Vec classes
//multiply by scalar is only operation possible on Vec
template <typename T>
class Vec2
{
public:
    Vec2() : x(T(0)), y(T(0)) {}
    Vec2(T xx, T yy) : x(xx), y(yy) {}
    Vec2 operator*(const T &r) const { return Vec2(x * r, y * r); }
    Vec2 &operator*=(const T &r)
    {
        x *= r, y *= r;
        return *this;
    }
    T length_squared() const {
        return x * x + y * y;
    }
    T length() const {
        return sqrt(length_squared());
    }
    Vec2<T> unit()
    {
        if (length() < 0.001f && length() > -0.001f)
            return Vec2<T>(0.0f, 0.0f);
        else
        {
            float invLength = T(1) / length();
            return Vec2<T>(x * invLength, y * invLength);
        }
    }
    T x, y;
};

typedef Vec2<float> Vec2f;

template <typename T>
T dot2(const Vec2<T> &v1, const Vec2<T> &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

template <typename T>
class Vec3
{
public:
    Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
    Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
    Vec3 operator*(const T &r) const { return Vec3(x * r, y * r, z * r); }
    Vec3 &operator*=(const T &r)
    {
        x *= r, y *= r, z *= r;
        return *this;
    }
    T length_squared()
    {
        return x * x + y * y + z * z;
    }
    T length()
    {
        return sqrt(length_squared());
    }
    Vec3<T> unit()
    {
        if (length() < 0.001f && length() > -0.001f)
            return Vec3<T>(0.0f, 0.0f, 0.0f);
        else
        {
            float invLength = T(1) / length();
            return Vec3<T>(x * invLength, y * invLength, z * invLength);
        }
    }
    T x, y, z;
};

template <typename T>
T dot3(const Vec3<T> &v1, const Vec3<T> &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename T>
Vec3<T> cross3(const Vec3<T>& v1, const Vec3<T>& v2) {
    T s1 = v1.y * v2.z - v1.z * v2.y;
    T s2 = v1.z * v2.x - v1.x * v2.z;
    T s3 = v1.x * v2.y - v1.y * v2.x;
    return Vec3<T>(s1, s2, s3);
}

typedef Vec3<float> Vec3f;

#endif //VEC_H