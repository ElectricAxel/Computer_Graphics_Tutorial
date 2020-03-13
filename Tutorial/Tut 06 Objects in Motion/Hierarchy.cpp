// Copyright (C) 2010-2012 by Jason L. McKesson
// This file is licensed under the MIT License.

#include <string>
#include <vector>
#include <stack>
#include <math.h>
#include <stdio.h>
#include <glload/gl_3_3.h>
#include <GL/freeglut.h>
#include "../framework/framework.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint theProgram;
GLuint positionAttrib;
GLuint colorAttrib;

GLuint modelToCameraMatrixUnif;
GLuint cameraToClipMatrixUnif;

glm::mat4 cameraToClipMatrix(0.0f);

float CalcFrustumScale(float fFovDeg) {
  const float degToRad = 3.14159f * 2.0f / 360.0f;
  float fFovRad = fFovDeg * degToRad;
  return 1.0f / tan(fFovRad / 2.0f);
}

const float fFrustumScale = CalcFrustumScale(45.0f);

void InitializeProgram() {
  std::vector<GLuint> shaderList;

  shaderList.push_back(
      Framework::LoadShader(GL_VERTEX_SHADER, "PosColorLocalTransform.vert"));
  shaderList.push_back(
      Framework::LoadShader(GL_FRAGMENT_SHADER, "ColorPassthrough.frag"));

  theProgram = Framework::CreateProgram(shaderList);

  positionAttrib = glGetAttribLocation(theProgram, "position");
  colorAttrib = glGetAttribLocation(theProgram, "color");

  modelToCameraMatrixUnif =
      glGetUniformLocation(theProgram, "modelToCameraMatrix");
  cameraToClipMatrixUnif =
      glGetUniformLocation(theProgram, "cameraToClipMatrix");

  float fzNear = 1.0f;
  float fzFar = 100.0f;

  cameraToClipMatrix[0].x = fFrustumScale;
  cameraToClipMatrix[1].y = fFrustumScale;
  cameraToClipMatrix[2].z = (fzFar + fzNear) / (fzNear - fzFar);
  cameraToClipMatrix[2].w = -1.0f;
  cameraToClipMatrix[3].z = (2 * fzFar * fzNear) / (fzNear - fzFar);

  glUseProgram(theProgram);
  glUniformMatrix4fv(cameraToClipMatrixUnif, 1, GL_FALSE,
                     glm::value_ptr(cameraToClipMatrix));
  glUseProgram(0);
}

const int numberOfVertices = 24;

#define RED_COLOR 1.0f, 0.0f, 0.0f, 1.0f
#define GREEN_COLOR 0.0f, 1.0f, 0.0f, 1.0f
#define BLUE_COLOR 0.0f, 0.0f, 1.0f, 1.0f

#define YELLOW_COLOR 1.0f, 1.0f, 0.0f, 1.0f
#define CYAN_COLOR 0.0f, 1.0f, 1.0f, 1.0f
#define MAGENTA_COLOR 1.0f, 0.0f, 1.0f, 1.0f

const float vertexData[] = {
    // Front
    +1.0f,
    +1.0f,
    +1.0f,
    +1.0f,
    -1.0f,
    +1.0f,
    -1.0f,
    -1.0f,
    +1.0f,
    -1.0f,
    +1.0f,
    +1.0f,

    // Top
    +1.0f,
    +1.0f,
    +1.0f,
    -1.0f,
    +1.0f,
    +1.0f,
    -1.0f,
    +1.0f,
    -1.0f,
    +1.0f,
    +1.0f,
    -1.0f,

    // Left
    +1.0f,
    +1.0f,
    +1.0f,
    +1.0f,
    +1.0f,
    -1.0f,
    +1.0f,
    -1.0f,
    -1.0f,
    +1.0f,
    -1.0f,
    +1.0f,

    // Back
    +1.0f,
    +1.0f,
    -1.0f,
    -1.0f,
    +1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    +1.0f,
    -1.0f,
    -1.0f,

    // Bottom
    +1.0f,
    -1.0f,
    +1.0f,
    +1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    +1.0f,

    // Right
    -1.0f,
    +1.0f,
    +1.0f,
    -1.0f,
    -1.0f,
    +1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    -1.0f,
    +1.0f,
    -1.0f,

    GREEN_COLOR,
    GREEN_COLOR,
    GREEN_COLOR,
    GREEN_COLOR,

    BLUE_COLOR,
    BLUE_COLOR,
    BLUE_COLOR,
    BLUE_COLOR,

    RED_COLOR,
    RED_COLOR,
    RED_COLOR,
    RED_COLOR,

    YELLOW_COLOR,
    YELLOW_COLOR,
    YELLOW_COLOR,
    YELLOW_COLOR,

    CYAN_COLOR,
    CYAN_COLOR,
    CYAN_COLOR,
    CYAN_COLOR,

    MAGENTA_COLOR,
    MAGENTA_COLOR,
    MAGENTA_COLOR,
    MAGENTA_COLOR,
};

const GLshort indexData[] = {
    0,  1,  2,  2,  3,  0,

    4,  5,  6,  6,  7,  4,

    8,  9,  10, 10, 11, 8,

    12, 13, 14, 14, 15, 12,

    16, 17, 18, 18, 19, 16,

    20, 21, 22, 22, 23, 20,
};

GLuint vertexBufferObject;
GLuint indexBufferObject;
GLuint vao;

void InitializeVAO() {
  glGenBuffers(1, &vertexBufferObject);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &indexBufferObject);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  size_t colorDataOffset = sizeof(float) * 3 * numberOfVertices;
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glEnableVertexAttribArray(positionAttrib);
  glEnableVertexAttribArray(colorAttrib);
  glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, 0,
                        (void *)colorDataOffset);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);

  glBindVertexArray(0);
}

inline float DegToRad(float fAngDeg) {
  const float fDegToRad = 3.14159f * 2.0f / 360.0f;
  return fAngDeg * fDegToRad;
}

inline float Clamp(float fValue, float fMinValue, float fMaxValue) {
  if (fValue < fMinValue)
    return fMinValue;

  if (fValue > fMaxValue)
    return fMaxValue;

  return fValue;
}

glm::mat3 RotateX(float fAngDeg) {
  float fAngRad = DegToRad(fAngDeg);
  float fCos = cosf(fAngRad);
  float fSin = sinf(fAngRad);

  glm::mat3 theMat(1.0f);
  theMat[1].y = fCos;
  theMat[2].y = -fSin;
  theMat[1].z = fSin;
  theMat[2].z = fCos;
  return theMat;
}

glm::mat3 RotateY(float fAngDeg) {
  float fAngRad = DegToRad(fAngDeg);
  float fCos = cosf(fAngRad);
  float fSin = sinf(fAngRad);

  glm::mat3 theMat(1.0f);
  theMat[0].x = fCos;
  theMat[2].x = fSin;
  theMat[0].z = -fSin;
  theMat[2].z = fCos;
  return theMat;
}

glm::mat3 RotateZ(float fAngDeg) {
  float fAngRad = DegToRad(fAngDeg);
  float fCos = cosf(fAngRad);
  float fSin = sinf(fAngRad);

  glm::mat3 theMat(1.0f);
  theMat[0].x = fCos;
  theMat[1].x = -fSin;
  theMat[0].y = fSin;
  theMat[1].y = fCos;
  return theMat;
}

// Node.cpp

class Node {
public:
  enum TRANSFORMATION_TYPE { TRANSLATION, ROTATION, SCALE };

  struct Transformation {
    float x;
    float y;
    float z;
    TRANSFORMATION_TYPE type;
  };

  Node() : vis(true) {}

  void Render(glm::mat4 m_currMat) {

    /* translate => rotate => children =>
       translate (center based on size) =>
       scale => draw */

    for (Transformation t : preRenderTransformations) {
      switch (t.type) {
      case TRANSFORMATION_TYPE::TRANSLATION:
        Translate(m_currMat, t);
        break;
      case TRANSFORMATION_TYPE::ROTATION:
        Rotate(m_currMat, t);
        break;
      case TRANSFORMATION_TYPE::SCALE:
        Scale(m_currMat, t);
        break;
      }
    }

    for (size_t i = 0; i < children.size(); ++i) {
      children[i]->Render(m_currMat);
    }

    if (vis) {
      for (Transformation t : postRenderTransformations) {
        switch (t.type) {
        case TRANSFORMATION_TYPE::TRANSLATION:
          Translate(m_currMat, t);
          break;
        case TRANSFORMATION_TYPE::ROTATION:
          Rotate(m_currMat, t);
          break;
        case TRANSFORMATION_TYPE::SCALE:
          Scale(m_currMat, t);
          break;
        }
      }

      glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE,
                         glm::value_ptr(m_currMat));
      glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT,
                     0);
    }
  }

  void AddChild(Node &child) { children.push_back(&child); }

  void AddPreRenderTransformation(Transformation transformation) {
    preRenderTransformations.push_back(transformation);
  }

  void AddPreRenderTranslation(glm::vec3 translation) {
    AddPreRenderTransformation(
        Transformation{translation.x, translation.y, translation.z, TRANSFORMATION_TYPE::TRANSLATION});
  }

  void AddPreRenderTranslation(float x, float y, float z) {
    AddPreRenderTransformation(
        Transformation{x, y, z, TRANSFORMATION_TYPE::TRANSLATION});
  }

  void AddPreRenderRotation(float x, float y, float z) {
    AddPreRenderTransformation(
        Transformation{x, y, z, TRANSFORMATION_TYPE::ROTATION});
  }

  void AddPreRenderScale(float x, float y, float z) {
    AddPreRenderTransformation(
        Transformation{x, y, z, TRANSFORMATION_TYPE::SCALE});
  }

  void AddPostRenderTransformation(Transformation transformation) {
    postRenderTransformations.push_back(transformation);
  }

  void AddPostRenderTranslation(float x, float y, float z) {
    AddPostRenderTransformation(
        Transformation{x, y, z, TRANSFORMATION_TYPE::TRANSLATION});
  }

  void AddPostRenderRotation(float x, float y, float z) {
    AddPostRenderTransformation(
        Transformation{x, y, z, TRANSFORMATION_TYPE::ROTATION});
  }

  void AddPostRenderScale(float x, float y, float z) {
    AddPostRenderTransformation(
        Transformation{x, y, z, TRANSFORMATION_TYPE::SCALE});
  }

  bool vis; // visible yes or no, this is used for organization to build mid
            // points to translate / rotate

private:
  void Translate(glm::mat4 &m_currMat, Transformation transformation) {
    glm::mat4 translateMat(1.0f);
    translateMat[3] =
        glm::vec4(transformation.x, transformation.y, transformation.z, 1.0f);

    m_currMat = m_currMat * translateMat;
  }

  void Scale(glm::mat4 &m_currMat, Transformation transformation) {
    glm::mat4 scaleMat(1.0f);
    scaleMat[0].x = transformation.x;
    scaleMat[1].y = transformation.y;
    scaleMat[2].z = transformation.z;

    m_currMat = m_currMat * scaleMat;
  }

  void Rotate(glm::mat4 &m_currMat, Transformation transformation) {
    // Order is important.
    if (transformation.z) {
      RotateZ(m_currMat, transformation.z);
    }
    if (transformation.y) {
      RotateY(m_currMat, transformation.y);
    }
    if (transformation.x) {
      RotateX(m_currMat, transformation.x);
    }
  }

  void RotateX(glm::mat4 &m_currMat, float radians) {
    m_currMat = m_currMat * glm::mat4(::RotateX(radians));
  }

  void RotateY(glm::mat4 &m_currMat, float radians) {
    m_currMat = m_currMat * glm::mat4(::RotateY(radians));
  }

  void RotateZ(glm::mat4 &m_currMat, float radians) {
    m_currMat = m_currMat * glm::mat4(::RotateZ(radians));
  }

  std::vector<Node *> children;
  std::vector<Transformation> preRenderTransformations;
  std::vector<Transformation> postRenderTransformations;
};

class Crane {
public:
  Crane() : baseTranslation(glm::vec3(4.0f, -5.0f, -40.0f)), 
        baseRotationY(45.0f),
        baseLeftTranslation(glm::vec3(2.0f, 0.0f, 0.0f)),
        baseRightTranslation(glm::vec3(-2.0f, 0.0f, 0.0f)),
        baseLength(3.0f),
        upperArmRotationX(-33.75f),
        upperArmLength(9.0f),
        lowerArmTranslation(glm::vec3(0.0f, 0.0f, 8.0f)),
        lowerArmRotationX(146.25f), lowerArmLength(5.0f),
        lowerArmWidth(1.5f),
        wristTranslation(glm::vec3(0.0f, 0.0f, 5.0f)),
        wristRotationZ(0.0f),
        wristRotationX(67.5f),
        wristLength(2.0f),
        wristWidth(2.0f),
        leftFingerTranslation(glm::vec3(1.0f, 0.0f, 1.0f)),
        rightFingerTranslation(glm::vec3(-1.0f, 0.0f, 1.0f)),
        fingerOpenRotationY(180.0f),
        fingerLength(2.0f),
        fingerWidth(0.5f),
        lowerFingerRotationY(45.0f) {}

  void Render() {
    // Initialize all nodes
    Node base, baseLeft, baseRight, upperArm, lowerArm, wrist, leftFinger,
        leftLowerFinger, rightFinger, rightLowerFinger;

    // Set up the hierarchy
    base.AddChild(baseLeft);
    base.AddChild(baseRight);
    base.AddChild(upperArm);
    upperArm.AddChild(lowerArm);
    lowerArm.AddChild(wrist);
    wrist.AddChild(leftFinger);
    leftFinger.AddChild(leftLowerFinger);
    wrist.AddChild(rightFinger);
    rightFinger.AddChild(rightLowerFinger);

    // TODO: turn all angles into radians
    base.vis = false;
    base.AddPreRenderTranslation(baseTranslation);
    base.AddPreRenderRotation(0.0f, baseRotationY, 0.0f);

    baseLeft.AddPreRenderTranslation(baseLeftTranslation);
    baseLeft.AddPreRenderScale(1.0f, 1.0f, baseLength);

    baseRight.AddPreRenderTranslation(baseRightTranslation);
    baseRight.AddPreRenderScale(1.0f, 1.0f, baseLength);

    upperArm.AddPreRenderRotation(upperArmRotationX, 0.0f, 0.0f);

    upperArm.AddPostRenderTranslation(0.0f, 0.0f, upperArmLength / 2.0f - 1.0f);
    upperArm.AddPostRenderScale(1.0f, 1.0f, upperArmLength / 2.0f);

    lowerArm.AddPreRenderTranslation(lowerArmTranslation);
    lowerArm.AddPreRenderRotation(lowerArmRotationX, 0.0f, 0.0f);

    lowerArm.AddPostRenderTranslation(0.0f, 0.0f, lowerArmLength / 2.0f);
    lowerArm.AddPostRenderScale(lowerArmWidth / 2.0f, lowerArmWidth / 2.0f, lowerArmLength / 2.0f);

    wrist.AddPreRenderTranslation(wristTranslation);
    wrist.AddPreRenderRotation(wristRotationX, 0.0f, wristRotationZ);

    wrist.AddPostRenderScale(wristWidth / 2.0f, wristWidth / 2.0f, wristLength / 2.0f);

    leftFinger.AddPreRenderTranslation(leftFingerTranslation);
    leftFinger.AddPreRenderRotation(0.0f, fingerOpenRotationY, 0.0f);

    leftFinger.AddPostRenderTranslation(0.0f, 0.0f, fingerLength / 2.0f);
    leftFinger.AddPostRenderScale(fingerWidth / 2.0f, fingerWidth / 2.0f, fingerLength / 2.0f);

    leftLowerFinger.AddPreRenderTranslation(0.0f, 0.0f, fingerLength);
    leftLowerFinger.AddPreRenderRotation(0.0f, -lowerFingerRotationY, 0.0f);

    leftLowerFinger.AddPostRenderTranslation(0.0f, 0.0f, fingerLength / 2.0f);
    leftLowerFinger.AddPostRenderScale(fingerWidth / 2.0f, fingerWidth / 2.0f, fingerLength / 2.0f);

    rightFinger.AddPreRenderTranslation(rightFingerTranslation);
    rightFinger.AddPreRenderRotation(0.0f, -fingerOpenRotationY, 0.0f);

    rightFinger.AddPostRenderTranslation(0.0f, 0.0f, fingerLength / 2.0f);
    rightFinger.AddPostRenderScale(fingerWidth / 2.0f, fingerWidth / 2.0f, fingerLength / 2.0f);

    rightLowerFinger.AddPreRenderTranslation(0.0f, 0.0f, fingerLength);
    rightLowerFinger.AddPreRenderRotation(0.0f, lowerFingerRotationY, 0.0f);

    rightLowerFinger.AddPostRenderTranslation(0.0f, 0.0f, fingerLength / 2.0f);
    rightLowerFinger.AddPostRenderScale(fingerWidth / 2.0f, fingerWidth / 2.0f, fingerLength / 2.0f);

    glm::mat4 node_mat(1.0f);

    glUseProgram(theProgram);
    glBindVertexArray(vao);

    base.Render(node_mat);

    glBindVertexArray(0);
    glUseProgram(0);
  }


void addToBaseRotationY(float val) {
  baseRotationY += val;
  baseRotationY = fmodf(baseRotationY, 360.0f);
}

// void addToBaseLength(float val) {
//   baseLength += val;
// }

void addToUpperArmRotationX(float val) {
  upperArmRotationX += val;
  upperArmRotationX = Clamp(upperArmRotationX, -90.0f, 0.0f);
}

// void addToUpperArmLength(float val) {
//   upperArmLength += val;
// }

void addToLowerArmRotationX(float val) {
  lowerArmRotationX += val;
  lowerArmRotationX = Clamp(lowerArmRotationX, 0.0f, 146.25f);
}

// void addToLowerArmLength(float val) {
//   lowerArmLength += val;
// }

// void addToLowerArmWidth(float val) {
//   lowerArmWidth += val;
// }

void addToWristRotationZ(float val) {
  wristRotationZ += val;
  wristRotationZ = fmodf(wristRotationZ, 360.0f);
}

void addToWristRotationX(float val) {
  wristRotationX += val;
  wristRotationX = Clamp(wristRotationX, 0.0f, 90.0f);
}

// void addToWristLength(float val) {
//   wristLength += val;
// }

// void addToWristWidth(float val) {
//   wristWidth += val;
// }

void addToFingerOpenRotationY(float val) {
  fingerOpenRotationY += val;
  fingerOpenRotationY = Clamp(fingerOpenRotationY, 9.0f, 180.0f);
}

// void addToFingerLength(float val) {
//   fingerLength += val;
// }

// void addToFingerWidth(float val) {
//   fingerWidth += val;
// }

// void addToLowerFingerRotationY(float val) {
//   lowerFingerRotationY += val;
// }

private:
  glm::vec3 baseTranslation;
  float baseRotationY;

  glm::vec3 baseLeftTranslation;
  glm::vec3 baseRightTranslation;
  float baseLength;

  float upperArmRotationX;
  float upperArmLength;

  glm::vec3 lowerArmTranslation;
  float lowerArmRotationX;
  float lowerArmLength;
  float lowerArmWidth;

  glm::vec3 wristTranslation;
  float wristRotationZ; // ROLL
  float wristRotationX; // PITCH
  float wristLength;
  float wristWidth;

  glm::vec3 leftFingerTranslation;
  glm::vec3 rightFingerTranslation;
  float fingerOpenRotationY;
  float fingerLength;
  float fingerWidth;
  float lowerFingerRotationY;
};

class MatrixStack {
public:
  MatrixStack() : m_currMat(1.0f) {}

  const glm::mat4 &Top() { return m_currMat; }

  void RotateX(float fAngDeg) {
    m_currMat = m_currMat * glm::mat4(::RotateX(fAngDeg));
  }

  void RotateY(float fAngDeg) {
    m_currMat = m_currMat * glm::mat4(::RotateY(fAngDeg));
  }

  void RotateZ(float fAngDeg) {
    m_currMat = m_currMat * glm::mat4(::RotateZ(fAngDeg));
  }

  void Scale(const glm::vec3 &scaleVec) {
    glm::mat4 scaleMat(1.0f);
    scaleMat[0].x = scaleVec.x;
    scaleMat[1].y = scaleVec.y;
    scaleMat[2].z = scaleVec.z;

    m_currMat = m_currMat * scaleMat;
  }

  void Translate(const glm::vec3 &offsetVec) {
    glm::mat4 translateMat(1.0f);
    translateMat[3] = glm::vec4(offsetVec, 1.0f);

    m_currMat = m_currMat * translateMat;
  }

  void Push() { m_matrices.push(m_currMat); }

  void Pop() {
    m_currMat = m_matrices.top();
    m_matrices.pop();
  }

private:
  glm::mat4 m_currMat;
  std::stack<glm::mat4> m_matrices;
};

class Hierarchy {
public:
  Hierarchy()
      : posBase(glm::vec3(-6.0f, -5.0f, -40.0f)), angBase(45.0f),
        posBaseLeft(glm::vec3(2.0f, 0.0f, 0.0f)),
        posBaseRight(glm::vec3(-2.0f, 0.0f, 0.0f)), scaleBaseZ(3.0f),
        angUpperArm(-33.75f), sizeUpperArm(9.0f),
        posLowerArm(glm::vec3(0.0f, 0.0f, 8.0f)), angLowerArm(146.25f),
        lenLowerArm(5.0f), widthLowerArm(1.5f),
        posWrist(glm::vec3(0.0f, 0.0f, 5.0f)), angWristRoll(0.0f),
        angWristPitch(67.5f), lenWrist(2.0f), widthWrist(2.0f),
        posLeftFinger(glm::vec3(1.0f, 0.0f, 1.0f)),
        posRightFinger(glm::vec3(-1.0f, 0.0f, 1.0f)), angFingerOpen(180.0f),
        lenFinger(2.0f), widthFinger(0.5f), angLowerFinger(45.0f) {}

  void Draw() {
    MatrixStack modelToCameraStack;

    glUseProgram(theProgram);
    glBindVertexArray(vao);

    modelToCameraStack.Translate(posBase);
    modelToCameraStack.RotateY(angBase);

    // Draw left base.
    {
      modelToCameraStack.Push();
      modelToCameraStack.Translate(posBaseLeft);
      modelToCameraStack.Scale(glm::vec3(1.0f, 1.0f, scaleBaseZ));
      glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE,
                         glm::value_ptr(modelToCameraStack.Top()));
      glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT,
                     0);
      modelToCameraStack.Pop();
    }

    // Draw right base.
    {
      modelToCameraStack.Push();
      modelToCameraStack.Translate(posBaseRight);
      modelToCameraStack.Scale(glm::vec3(1.0f, 1.0f, scaleBaseZ));
      glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE,
                         glm::value_ptr(modelToCameraStack.Top()));
      glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT,
                     0);
      modelToCameraStack.Pop();
    }

    // Draw main arm.
    DrawUpperArm(modelToCameraStack);

    glBindVertexArray(0);
    glUseProgram(0);
  }

#define STANDARD_ANGLE_INCREMENT 11.25f
#define SMALL_ANGLE_INCREMENT 9.0f

  void AdjBase(bool bIncrement) {
    angBase +=
        bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
    angBase = fmodf(angBase, 360.0f);
  }

  void AdjUpperArm(bool bIncrement) {
    angUpperArm +=
        bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
    angUpperArm = Clamp(angUpperArm, -90.0f, 0.0f);
  }

  void AdjLowerArm(bool bIncrement) {
    angLowerArm +=
        bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
    angLowerArm = Clamp(angLowerArm, 0.0f, 146.25f);
  }

  void AdjWristPitch(bool bIncrement) {
    angWristPitch +=
        bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
    angWristPitch = Clamp(angWristPitch, 0.0f, 90.0f);
  }

  void AdjWristRoll(bool bIncrement) {
    angWristRoll +=
        bIncrement ? STANDARD_ANGLE_INCREMENT : -STANDARD_ANGLE_INCREMENT;
    angWristRoll = fmodf(angWristRoll, 360.0f);
  }

  void AdjFingerOpen(bool bIncrement) {
    angFingerOpen +=
        bIncrement ? SMALL_ANGLE_INCREMENT : -SMALL_ANGLE_INCREMENT;
    angFingerOpen = Clamp(angFingerOpen, 9.0f, 180.0f);
  }

  void WritePose() {
    printf("angBase:\t%f\n", angBase);
    printf("angUpperArm:\t%f\n", angUpperArm);
    printf("angLowerArm:\t%f\n", angLowerArm);
    printf("angWristPitch:\t%f\n", angWristPitch);
    printf("angWristRoll:\t%f\n", angWristRoll);
    printf("angFingerOpen:\t%f\n", angFingerOpen);
    printf("\n");
  }

private:
  void DrawFingers(MatrixStack &modelToCameraStack) {
    // Draw left finger
    modelToCameraStack.Push();
    modelToCameraStack.Translate(posLeftFinger);
    modelToCameraStack.RotateY(angFingerOpen);

    modelToCameraStack.Push();
    modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
    modelToCameraStack.Scale(
        glm::vec3(widthFinger / 2.0f, widthFinger / 2.0f, lenFinger / 2.0f));
    glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE,
                       glm::value_ptr(modelToCameraStack.Top()));
    glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
    modelToCameraStack.Pop();

    {
      // Draw left lower finger
      modelToCameraStack.Push();
      modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger));
      modelToCameraStack.RotateY(-angLowerFinger);

      modelToCameraStack.Push();
      modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
      modelToCameraStack.Scale(
          glm::vec3(widthFinger / 2.0f, widthFinger / 2.0f, lenFinger / 2.0f));
      glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE,
                         glm::value_ptr(modelToCameraStack.Top()));
      glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT,
                     0);
      modelToCameraStack.Pop();

      modelToCameraStack.Pop();
    }

    modelToCameraStack.Pop();

    // Draw right finger
    modelToCameraStack.Push();
    modelToCameraStack.Translate(posRightFinger);
    modelToCameraStack.RotateY(-angFingerOpen);

    modelToCameraStack.Push();
    modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
    modelToCameraStack.Scale(
        glm::vec3(widthFinger / 2.0f, widthFinger / 2.0f, lenFinger / 2.0f));
    glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE,
                       glm::value_ptr(modelToCameraStack.Top()));
    glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
    modelToCameraStack.Pop();

    {
      // Draw right lower finger
      modelToCameraStack.Push();
      modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger));
      modelToCameraStack.RotateY(angLowerFinger);

      modelToCameraStack.Push();
      modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenFinger / 2.0f));
      modelToCameraStack.Scale(
          glm::vec3(widthFinger / 2.0f, widthFinger / 2.0f, lenFinger / 2.0f));
      glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE,
                         glm::value_ptr(modelToCameraStack.Top()));
      glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT,
                     0);
      modelToCameraStack.Pop();

      modelToCameraStack.Pop();
    }

    modelToCameraStack.Pop();
  }

  void DrawWrist(MatrixStack &modelToCameraStack) {
    modelToCameraStack.Push();
    modelToCameraStack.Translate(posWrist);
    modelToCameraStack.RotateZ(
        angWristRoll); // Inversing this is pretty interesting
    modelToCameraStack.RotateX(angWristPitch);

    modelToCameraStack.Push();
    modelToCameraStack.Scale(
        glm::vec3(widthWrist / 2.0f, widthWrist / 2.0f, lenWrist / 2.0f));
    glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE,
                       glm::value_ptr(modelToCameraStack.Top()));
    glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
    modelToCameraStack.Pop();

    DrawFingers(modelToCameraStack);

    modelToCameraStack.Pop();
  }

  void DrawLowerArm(MatrixStack &modelToCameraStack) {
    modelToCameraStack.Push();
    modelToCameraStack.Translate(posLowerArm);
    modelToCameraStack.RotateX(angLowerArm);

    modelToCameraStack.Push();
    modelToCameraStack.Translate(glm::vec3(0.0f, 0.0f, lenLowerArm / 2.0f));
    modelToCameraStack.Scale(glm::vec3(
        widthLowerArm / 2.0f, widthLowerArm / 2.0f, lenLowerArm / 2.0f));
    glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE,
                       glm::value_ptr(modelToCameraStack.Top()));
    glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
    modelToCameraStack.Pop();

    DrawWrist(modelToCameraStack);

    modelToCameraStack.Pop();
  }

  void DrawUpperArm(MatrixStack &modelToCameraStack) {
    modelToCameraStack.Push();
    modelToCameraStack.RotateX(angUpperArm);

    {
      modelToCameraStack.Push();
      modelToCameraStack.Translate(
          glm::vec3(0.0f, 0.0f, (sizeUpperArm / 2.0f) - 1.0f));
      modelToCameraStack.Scale(glm::vec3(1.0f, 1.0f, sizeUpperArm / 2.0f));
      glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE,
                         glm::value_ptr(modelToCameraStack.Top()));
      glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT,
                     0);
      modelToCameraStack.Pop();
    }

    DrawLowerArm(modelToCameraStack);

    modelToCameraStack.Pop();
  }

  glm::vec3 posBase;
  float angBase;

  glm::vec3 posBaseLeft, posBaseRight;
  float scaleBaseZ;

  float angUpperArm;
  float sizeUpperArm;

  glm::vec3 posLowerArm;
  float angLowerArm;
  float lenLowerArm;
  float widthLowerArm;

  glm::vec3 posWrist;
  float angWristRoll;
  float angWristPitch;
  float lenWrist;
  float widthWrist;

  glm::vec3 posLeftFinger, posRightFinger;
  float angFingerOpen;
  float lenFinger;
  float widthFinger;
  float angLowerFinger;
};

Crane g_crane;
Hierarchy g_armature;

// Called after the window and OpenGL are initialized. Called exactly once,
// before the main loop.
void init() {
  InitializeProgram();
  InitializeVAO();

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);
}

// Called to update the display.
// You should call glutSwapBuffers after all of your rendering to display what
// you rendered. If you need continuous updates of the screen, call
// glutPostRedisplay() at the end of the function.
void display() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  g_crane.Render();

  g_armature.Draw();

  glutSwapBuffers();
  glutPostRedisplay();
}

// Called whenever the window is resized. The new window size is given, in
// pixels. This is an opportunity to call glViewport or glScissor to keep up
// with the change in size.
void reshape(int w, int h) {
  cameraToClipMatrix[0].x = fFrustumScale * (h / (float)w);
  cameraToClipMatrix[1].y = fFrustumScale;

  glUseProgram(theProgram);
  glUniformMatrix4fv(cameraToClipMatrixUnif, 1, GL_FALSE,
                     glm::value_ptr(cameraToClipMatrix));
  glUseProgram(0);

  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

// Called whenever a key on the keyboard was pressed.
// The key is given by the ''key'' parameter, which is in ASCII.
// It's often a good idea to have the escape key (ASCII value 27) call
// glutLeaveMainLoop() to exit the program.
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27:
    glutLeaveMainLoop();
    return;
  case 'a':
    g_armature.AdjBase(true);
    g_crane.addToBaseRotationY(STANDARD_ANGLE_INCREMENT);
    break;
  case 'd':
    g_armature.AdjBase(false);
    g_crane.addToBaseRotationY(-STANDARD_ANGLE_INCREMENT);
    break;
  case 'w':
    g_armature.AdjUpperArm(false);
    g_crane.addToUpperArmRotationX(-STANDARD_ANGLE_INCREMENT);
    break;
  case 's':
    g_armature.AdjUpperArm(true);
    g_crane.addToUpperArmRotationX(STANDARD_ANGLE_INCREMENT);
    break;
  case 'r':
    g_armature.AdjLowerArm(false);
    g_crane.addToLowerArmRotationX(-STANDARD_ANGLE_INCREMENT);
    break;
  case 'f':
    g_armature.AdjLowerArm(true);
    g_crane.addToLowerArmRotationX(STANDARD_ANGLE_INCREMENT);
    break;
  case 't':
    g_armature.AdjWristPitch(false);
    g_crane.addToWristRotationX(-STANDARD_ANGLE_INCREMENT);
    break;
  case 'g':
    g_armature.AdjWristPitch(true);
    g_crane.addToWristRotationX(STANDARD_ANGLE_INCREMENT);
    break;
  case 'z':
    g_armature.AdjWristRoll(true);
    g_crane.addToWristRotationZ(-STANDARD_ANGLE_INCREMENT);
    break;
  case 'c':
    g_armature.AdjWristRoll(false);
    g_crane.addToWristRotationZ(STANDARD_ANGLE_INCREMENT);
    break;
  case 'q':
    g_armature.AdjFingerOpen(true);
    g_crane.addToFingerOpenRotationY(SMALL_ANGLE_INCREMENT);
    break;
  case 'e':
    g_armature.AdjFingerOpen(false);
    g_crane.addToFingerOpenRotationY(-SMALL_ANGLE_INCREMENT);
    break;
  case 32:
    g_armature.WritePose();
    break;
  }
}

unsigned int defaults(unsigned int displayMode, int &width, int &height) {
  return displayMode;
}