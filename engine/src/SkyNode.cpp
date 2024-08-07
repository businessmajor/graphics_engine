#include "SkyNode.hpp"

SkyNode::SkyNode(std::shared_ptr<Skybox> ob)
    : SceneNode(ob, "./shaders/cube_vert.glsl", "./shaders/cube_frag.glsl") {}

void SkyNode::Update(glm::mat4 projectionMatrix, Camera* camera) {
  if (!m_object) {
    std::cout << "Damn the SkyNode Update didn't work..." << std::endl;
  } else {
    m_worldTransform = this->GetLocalTransform();

    // Now apply our shader
    m_shader->Bind();
    // Set the uniforms in our current shader
    // For our object, we apply the texture in the following way
    // Note that we set the value to 0, because we have bound
    // our texture to slot 0.
    m_shader->SetUniform1i("u_skybox", 0);

    glm::mat4 view = glm::mat4(glm::mat3(camera->GetWorldToViewmatrix()));
    m_shader->SetUniformMatrix4fv("projection", &projectionMatrix[0][0]);
    m_shader->SetUniformMatrix4fv("view", &view[0][0]);

    for (int i = 0; i < m_children.size(); ++i) {
      m_children[i]->Update(projectionMatrix, camera);
    }

    // Create a 'light'
    // m_shader->SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
    // m_shader->SetUniform3f(
    //     "lightPos", camera->GetEyeXPosition() + camera->GetViewXDirection(),
    //     camera->GetEyeYPosition() + camera->GetViewYDirection(),
    //     camera->GetEyeZPosition() + camera->GetViewZDirection());
    // m_shader->SetUniform1f("ambientIntensity", 0.5f);
  }
}
