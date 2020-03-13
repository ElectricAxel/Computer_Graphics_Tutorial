//Node.cpp

class Node {
  public:
    Node() : vis(true) , pos(glm::vec3(0.0f, 0.0f, 0.0f) , sca(glm::vec3(1.0f, 1.0f, 1.0f), ang(glm::vec3(0.0f, 0.0f, 0.0f) {

    }

    void Render(glm::mat4 m_currMat) {

    /* translate => rotate => children => 
       translate (center based on size) => 
       scale => draw */
      
      if(pos.x != 0.0f ||
         pos.y != 0.0f ||
         pos.z != 0.0f) {
        Translate(m_currMat);
      }

      if(ang.z != 0.0f) {
        RotateZ(m_currMat);
      }

      if(ang.y != 0.0f) {
        RotateY(m_currMat);
      }

      if(ang.x != 0.0f) {
        RotateX(m_currMat);
      }

      for(size_t i = 0; i < children.size(); ++i) {
        children.Render(m_currMat);
      }

      if(vis) {
        TranslateAndScale(m_currMat);
        glUniformMatrix4fv(modelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(m_currMat));
			  glDrawElements(GL_TRIANGLES, ARRAY_COUNT(indexData), GL_UNSIGNED_SHORT, 0);
      }

    }

    void AddChild(Node child) {
      children.push_back(child);
    }

  bool vis; //visible yes or no, this is used for organization to build mid points to translate / rotate
  glm::vec3 pos;
  glm::vec3 sca;
  glm::vec3 ang;

  private:

    void Translate(glm::mat4 &m_currMat) {
      glm::mat4 translateMat(1.0f);
      translateMat[3] = glm::vec4(pos, 1.0f);

      m_currMat = m_currMat * translateMat;
    }

    void Scale(glm::mat4 &m_currMat) {
      glm::mat4 scaleMat(1.0f);
      scaleMat[0].x = sca.x;
      scaleMat[1].y = sca.y;
      scaleMat[2].z = sca.z;

      m_currMat = m_currMat * scaleMat;
    }

    void TranslateAndScale(glm::mat4 &m_currMat) {
      glm::mat4 translateMat(1.0f);
      translateMat[0].x = sca.x / 2;
      translateMat[1].y = sca.y / 2;
      translateMat[2].z = sca.z / 2;

      m_currMat = m_currMat * translateMat;
      glm::mat4 scaleMat(1.0f);
      scaleMat[0].x = sca.x;
      scaleMat[1].y = sca.y;
      scaleMat[2].z = sca.z;

      m_currMat = m_currMat * scaleMat;
    }
  
    void RotateX(glm::mat4 &m_currMat) {
      m_currMat = m_currMat * glm::mat4(::RotateX(ang.x));
    }

    void RotateY(glm::mat4 &m_currMat) {
      m_currMat = m_currMat * glm::mat4(::RotateY(ang.y));
    }

    void RotateZ(glm::mat4 &m_currMat) {
      m_currMat = m_currMat * glm::mat4(::RotateZ(ang.z));
    }
  
  std::vector<Node> children;
};