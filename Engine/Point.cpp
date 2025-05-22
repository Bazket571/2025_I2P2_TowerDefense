#include "Point.hpp"
#include <cmath>

namespace Engine {
    Point::Point(float x, float y, float z) : x(x), y(y), z(z) {}
    bool Point::operator==(const Point &rhs) const {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }
    bool Point::operator!=(const Point &rhs) const {
        return !operator==(rhs);
    }
    Point Point::operator+(const Point &rhs) const {
        return Point(x + rhs.x, y + rhs.y, z + rhs.z);
    }
    Point Point::operator-(const Point &rhs) const {
        return Point(x - rhs.x, y - rhs.y, z - rhs.z);
    }
    Point Point::operator*(const float &rhs) const {
        return Point(x * rhs, y * rhs, z * rhs);
    }
    Point Point::operator/(const float &rhs) const {
        return Point(x / rhs, y / rhs, z / rhs);
    }
    Point Point::Normalize() const {
        if (Magnitude() == 0)
            return Point();
        return Point(x, y, z) / Magnitude();
    }
    float Point::Dot(const Point &rhs) const {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }
    Point Point::Cross(const Point& rhs) const{
        return Point(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
    }
    float Point::MagnitudeSquared() const {
        return x * x + y * y + z * z;
    }
    float Point::Magnitude() const {
        return sqrt(MagnitudeSquared());
    };
    Point operator*(const float &lhs, const Point &rhs) {
        return rhs * lhs;
    }
}
