#include "matrixs.hpp"
#include <stdexcept>

namespace gll {
    Mat3 Mat3::inverse() const {
        Gfloat det = determinant();
        if (det == 0.0f) {
            throw std::runtime_error("Singular matrix");
        }

        Gfloat invDet = 1.0f / det;

        Gfloat m00 = data_[0][0], m01 = data_[1][0], m02 = data_[2][0];
        Gfloat m10 = data_[0][1], m11 = data_[1][1], m12 = data_[2][1];
        Gfloat m20 = data_[0][2], m21 = data_[1][2], m22 = data_[2][2];

        Gfloat c00 =  (m11 * m22 - m12 * m21);
        Gfloat c01 = -(m01 * m22 - m02 * m21);
        Gfloat c02 =  (m01 * m12 - m02 * m11);
        Gfloat c10 = -(m10 * m22 - m12 * m20);
        Gfloat c11 =  (m00 * m22 - m02 * m20);
        Gfloat c12 = -(m00 * m12 - m02 * m10);
        Gfloat c20 =  (m10 * m21 - m11 * m20);
        Gfloat c21 = -(m00 * m21 - m01 * m20);
        Gfloat c22 =  (m00 * m11 - m01 * m10);

        Mat3 res{};
        res[0][0] = c00 * invDet;
        res[1][0] = c10 * invDet;
        res[2][0] = c20 * invDet;
        res[0][1] = c01 * invDet;
        res[1][1] = c11 * invDet;
        res[2][1] = c21 * invDet; 
        res[0][2] = c02 * invDet;
        res[1][2] = c12 * invDet;
        res[2][2] = c22 * invDet;
        return res;
    }

    Mat4 Mat4::inverse() const {
        Gfloat det = determinant();
        if (det == 0.0f) {
            throw std::runtime_error("Singular matrix");
        }

        Gfloat invDet = 1.0f / det;
        Mat4 res;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                Gfloat cf = ((i + j) % 2 == 0 ? 1.0f : -1.0f) * minorDet3(i, j);
                res[i][j] = cf * invDet;
            }
        }
        return res;
    }
}
