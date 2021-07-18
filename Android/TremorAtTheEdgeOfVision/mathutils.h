#ifndef MATHUTILS_H
#define MATHUTILS_H
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


#include <array>
#include <vector>

#include <GLES2/gl2.h>

class Matrix4x4 {
 public:
  float m[4][4];

  // Multiplies two matrices.
  Matrix4x4 operator*(const Matrix4x4& right);

  // Multiplies a matrix with a vector.
  std::array<float, 4> operator*(const std::array<float, 4>& vec);

  // Converts a matrix to an array of floats suitable for passing to OpenGL.
  std::array<float, 16> ToGlArray();
};

struct Quatf {
  float x;
  float y;
  float z;
  float w;

  Quatf(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}

  Quatf() : x(0), y(0), z(0), w(1) {}

  static Quatf FromXYZW(float q[4]) { return Quatf(q[0], q[1], q[2], q[3]); }

  Matrix4x4 ToMatrix();
};

/**
 * Converts an array of floats to a matrix.
 *
 * @param vec GL array
 * @return Obtained matrix
 */
Matrix4x4 GetMatrixFromGlArray(float* vec);

/**
 * Construct a translation matrix.
 *
 * @param translation Translation array
 * @return Obtained matrix
 */
Matrix4x4 GetTranslationMatrix(const std::array<float, 3>& translation);

Matrix4x4 GetPerspectiveProjectionMatrix(float aspect, float fovy, float near, float far);

Matrix4x4 GetInverseMatrix(Matrix4x4& in);

/**
 * Computes the angle between two vectors.
 *
 * @param vec1 First vector
 * @param vec2 Second vector
 * @return Angle between the vectors
 */
float AngleBetweenVectors(const std::array<float, 4>& vec1,
                          const std::array<float, 4>& vec2);

/**
 * Gets monotonic time in nanoseconds.
 *
 * @return Monotonic time in nanoseconds
 */
long GetMonotonicTimeNano();

/**
 * Generates a random floating point number between |min| and |max|.
 *
 * @param min Minimum range
 * @param max Maximum range
 * @return Random float number
 */
float RandomUniformFloat(float min, float max);

/**
 * Generates a random integer in the range [0, max_val).
 *
 * @param max_val Maximum range
 * @return Random int number
 */
int RandomUniformInt(int max_val);





#endif // MATHUTILS_H
