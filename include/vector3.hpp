#pragma once

#include <iostream>
#include <glm/glm.hpp>

namespace util {
    struct vec3 {
        double x, y, z;
        vec3(double x, double y, double z) : x(x), y(y), z(z) {}

        vec3 operator* (const double scalar) const {
            return {x * scalar, y * scalar, z * scalar};
        }
        vec3& operator*= (const double scalar) {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        vec3 operator/ (const double scalar) const {
            return {x / scalar, y / scalar, z / scalar};
        }
        vec3& operator/= (const double scalar) {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        vec3 operator+ (const vec3& other) const {
            return {x + other.x, y + other.y, z + other.z};
        }
        vec3& operator+= (const vec3& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        vec3 operator- (const vec3& other) const {
            return {x - other.x, y - other.y, z - other.z};
        }
        vec3& operator-= (const vec3& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        explicit operator glm::vec3() const {
            return glm::vec3(x, y, z);
        }
    };

    inline vec3 operator* (const double scalar, const vec3& vec) {
        return vec * scalar;
    }
    inline vec3 operator/ (const double scalar, const vec3& vec) {
        return {scalar / vec.x, scalar / vec.y, scalar / vec.z};
    }


    inline double dot(const vec3& vec1, const vec3& vec2) {
        return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
    }

    inline vec3 cross(const vec3& vec1, const vec3& vec2) {
        return {
            vec1.y * vec2.z - vec1.z * vec2.y,
            vec1.z * vec2.x - vec1.x * vec2.z,
            vec1.x * vec2.y - vec1.y * vec2.x,
        };
    }

    inline double magnitude(const vec3& vec) {
        double mag = std::sqrt(dot(vec, vec));
        if (mag == 0.0)
            std::cerr << "magnitude is zero" << "\n";
        return mag;
    }
    inline double magnitudeSqr(const vec3& vec) {
        return dot(vec, vec);
    }

    inline vec3 normalize(const vec3& vec) {
        double mag = magnitude(vec);

        if (mag == 0.0) {
            std::cerr << "tried to normalize with mag := 0.0" << "\n";
            return vec;
        }
        if (mag < 0.00001)
            return vec;

        return {vec.x / mag, vec.y / mag, vec.z / mag};
    }
    inline vec3 normalize(const vec3& vec, const double& mag) {
        if (mag == 0.0) {
            std::cerr << "tried to normalize with mag := 0.0" << "\n";
            return vec;
        }
        if (mag < 0.00001)
            return vec;

        return {vec.x / mag, vec.y / mag, vec.z / mag};
    }

    inline std::ostream& operator<<(std::ostream& os, const vec3& vec) {
        os << "[" << vec.x << ", " << vec.y << ", " << vec.z << "]";
        return os;
    }
}