/*
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mathutils.h"

#include <string.h>  // Needed for strtok_r and strstr
#include <time.h>
#include <unistd.h>

#include <array>
#include <cmath>
#include <random>
#include <sstream>
#include <string>

#include <GLES2/gl2.h>


/**
 * Calculates vector norm
 *
 * @param vec Vector
 * @return Norm value
 */
float VectorNorm(const std::array<float, 4>& vec) {
  return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

/**
 * Calculates dot product of two vectors
 *
 * @param vec1 First vector
 * @param vec2 Second vector
 * @return Dot product value.
 */
float VectorDotProduct(const std::array<float, 4>& vec1,
                       const std::array<float, 4>& vec2) {
  float product = 0;
  for (int i = 0; i < 3; i++) {
    product += vec1[i] * vec2[i];
  }
  return product;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& right) {
  Matrix4x4 result;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result.m[i][j] = 0.0f;
      for (int k = 0; k < 4; ++k) {
        result.m[i][j] += this->m[k][j] * right.m[i][k];
      }
    }
  }
  return result;
}

std::array<float, 4> Matrix4x4::operator*(const std::array<float, 4>& vec) {
  std::array<float, 4> result;
  for (int i = 0; i < 4; ++i) {
    result[i] = 0;
    for (int k = 0; k < 4; ++k) {
      result[i] += this->m[k][i] * vec[k];
    }
  }
  return result;
}

std::array<float, 16> Matrix4x4::ToGlArray() {
  std::array<float, 16> result;
  memcpy(&result[0], m, 16 * sizeof(float));
  return result;
}

Matrix4x4 Quatf::ToMatrix() {
  // Based on ion::math::RotationMatrix3x3
  const float xx = 2 * x * x;
  const float yy = 2 * y * y;
  const float zz = 2 * z * z;

  const float xy = 2 * x * y;
  const float xz = 2 * x * z;
  const float yz = 2 * y * z;

  const float xw = 2 * x * w;
  const float yw = 2 * y * w;
  const float zw = 2 * z * w;

  Matrix4x4 m;
  m.m[0][0] = 1 - yy - zz;
  m.m[0][1] = xy + zw;
  m.m[0][2] = xz - yw;
  m.m[0][3] = 0;
  m.m[1][0] = xy - zw;
  m.m[1][1] = 1 - xx - zz;
  m.m[1][2] = yz + xw;
  m.m[1][3] = 0;
  m.m[2][0] = xz + yw;
  m.m[2][1] = yz - xw;
  m.m[2][2] = 1 - xx - yy;
  m.m[2][3] = 0;
  m.m[3][0] = 0;
  m.m[3][1] = 0;
  m.m[3][2] = 0;
  m.m[3][3] = 1;
  /*
  m.m[0][0] = 1 - yy - zz;
  m.m[0][1] = xy + zw;
  m.m[0][2] = -(xz - yw);
  m.m[0][3] = 0;
  m.m[1][0] = xy - zw;
  m.m[1][1] = 1 - xx - zz;
  m.m[1][2] = -(yz + xw);
  m.m[1][3] = 0;
  m.m[2][0] = xz + yw;
  m.m[2][1] = yz - xw;
  m.m[2][2] = -(1 - xx - yy);
  m.m[2][3] = 0;
  m.m[3][0] = 0;
  m.m[3][1] = 0;
  m.m[3][2] = 0;
  m.m[3][3] = 1;
  */
  return m;
}

Matrix4x4 GetMatrixFromGlArray(float* vec) {
  Matrix4x4 result;
  memcpy(result.m, vec, 16 * sizeof(float));
  return result;
}

Matrix4x4 GetTranslationMatrix(const std::array<float, 3>& translation) {
  return {{
          {1.0f, 0.0f, 0.0f, 0.0f},
          {0.0f, 1.0f, 0.0f, 0.0f},
          {0.0f, 0.0f, 1.0f, 0.0f},
          {translation.at(0), translation.at(1), translation.at(2), 1.0f}
      }};
}

Matrix4x4 GetPerspectiveProjectionMatrix(float aspect, float fovy, float near, float far) {
    /*
    float yScale = 1.0 / tan(fovy * 0.5);
    float xScale = yScale / aspect;
    float zRange = far - near;
    float zScale = -(far + near) / zRange;
    float wzScale = -far * near / zRange;

    std::array<float, 4> P = { xScale, 0, 0, 0 };
    std::array<float, 4> Q = { 0, yScale, 0, 0 };
    std::array<float, 4> R = { 0, 0, zScale, -1 };
    std::array<float, 4> S = { 0, 0, wzScale, 0 };
    */
    const float height = 1.0 / tan(fovy);
    const float width = height / aspect;

    const float xLeft   = -(width/2.0) * near;
    const float xRight  = (width/2.0) * near;
    const float yBottom = -(height/2.0) * near;
    const float yTop    = (height/2.0) * near;

    const float X = (2.0 * near) / (xRight - xLeft);
    const float Y = (2.0 * near) / (yTop - yBottom);
    const float A = (xRight + xLeft) / (xRight - xLeft);
    const float B = (yTop + yBottom) / (yTop - yBottom);
    const float C = (near + far) / (near - far);
    const float D = (2.0 * near * far) / (near - far);

    return {{
            { X, 0, 0, 0 },
            { 0, Y, 0, 0 },
            { A, B, C, -1 },
            { 0, 0, D, 0 }
        }};
}

Matrix4x4 GetInverseMatrix(Matrix4x4& in) {
    std::array<float, 16> m = in.ToGlArray();
    std::array<float, 16> inv;

    inv[0] = m[5]  * m[10] * m[15] -
             m[5]  * m[11] * m[14] -
             m[9]  * m[6]  * m[15] +
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] -
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] +
              m[4]  * m[11] * m[14] +
              m[8]  * m[6]  * m[15] -
              m[8]  * m[7]  * m[14] -
              m[12] * m[6]  * m[11] +
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] -
             m[4]  * m[11] * m[13] -
             m[8]  * m[5] * m[15] +
             m[8]  * m[7] * m[13] +
             m[12] * m[5] * m[11] -
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] +
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] -
               m[8]  * m[6] * m[13] -
               m[12] * m[5] * m[10] +
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] +
              m[1]  * m[11] * m[14] +
              m[9]  * m[2] * m[15] -
              m[9]  * m[3] * m[14] -
              m[13] * m[2] * m[11] +
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] -
             m[0]  * m[11] * m[14] -
             m[8]  * m[2] * m[15] +
             m[8]  * m[3] * m[14] +
             m[12] * m[2] * m[11] -
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] +
              m[0]  * m[11] * m[13] +
              m[8]  * m[1] * m[15] -
              m[8]  * m[3] * m[13] -
              m[12] * m[1] * m[11] +
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] -
              m[0]  * m[10] * m[13] -
              m[8]  * m[1] * m[14] +
              m[8]  * m[2] * m[13] +
              m[12] * m[1] * m[10] -
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] -
             m[1]  * m[7] * m[14] -
             m[5]  * m[2] * m[15] +
             m[5]  * m[3] * m[14] +
             m[13] * m[2] * m[7] -
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] +
              m[0]  * m[7] * m[14] +
              m[4]  * m[2] * m[15] -
              m[4]  * m[3] * m[14] -
              m[12] * m[2] * m[7] +
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] -
              m[0]  * m[7] * m[13] -
              m[4]  * m[1] * m[15] +
              m[4]  * m[3] * m[13] +
              m[12] * m[1] * m[7] -
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] +
               m[0]  * m[6] * m[13] +
               m[4]  * m[1] * m[14] -
               m[4]  * m[2] * m[13] -
               m[12] * m[1] * m[6] +
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] +
              m[1] * m[7] * m[10] +
              m[5] * m[2] * m[11] -
              m[5] * m[3] * m[10] -
              m[9] * m[2] * m[7] +
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] -
             m[0] * m[7] * m[10] -
             m[4] * m[2] * m[11] +
             m[4] * m[3] * m[10] +
             m[8] * m[2] * m[7] -
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] +
               m[0] * m[7] * m[9] +
               m[4] * m[1] * m[11] -
               m[4] * m[3] * m[9] -
               m[8] * m[1] * m[7] +
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] -
              m[0] * m[6] * m[9] -
              m[4] * m[1] * m[10] +
              m[4] * m[2] * m[9] +
              m[8] * m[1] * m[6] -
              m[8] * m[2] * m[5];

    float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return {{
                { 1, 0, 0, 0 },
                { 0, 1, 0, 0 },
                { 0, 0, 1, 0 },
                { 0, 0, 0, 1}
            }};

    det = 1.0 / det;

    for (int i = 0; i < 16; i++)
        inv[i] *= det;

    return GetMatrixFromGlArray(inv.data());
}
float AngleBetweenVectors(const std::array<float, 4>& vec1,
                          const std::array<float, 4>& vec2) {
  return std::acos(
      std::max(-1.f, std::min(1.f, VectorDotProduct(vec1, vec2) /
                                       (VectorNorm(vec1) * VectorNorm(vec2)))));
}

static constexpr uint64_t kNanosInSeconds = 1000000000;

long GetMonotonicTimeNano() {
  struct timespec res;
  clock_gettime(CLOCK_MONOTONIC, &res);
  return (res.tv_sec * kNanosInSeconds) + res.tv_nsec;
}

float RandomUniformFloat(float min, float max) {
  static std::random_device random_device;
  static std::mt19937 random_generator(random_device());
  static std::uniform_real_distribution<float> random_distribution(0, 1);
  return random_distribution(random_generator) * (max - min) + min;
}

int RandomUniformInt(int max_val) {
  static std::random_device random_device;
  static std::mt19937 random_generator(random_device());
  std::uniform_int_distribution<int> random_distribution(0, max_val - 1);
  return random_distribution(random_generator);
}
