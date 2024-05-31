#include "Sprite.h"

#include "ShaderProgram.h"
#include "Texture2D.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Renderer
{
    Sprite::Sprite(std::shared_ptr<Texture2D> pTexture, std::string initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram) : m_pTexture(std::move(pTexture)), m_pShaderProgram(std::move(pShaderProgram))
    {
        const GLfloat vertexCoords[] = {
            // 1 - 2
            // | / |
            // 0 - 3

            // X Y
            0.f, 0.f,
            0.f, 1.f,
            1.f, 1.f,
            1.f, 0.f
        };

        auto subTexture = m_pTexture->getSubTexture(std::move(initialSubTexture));

        const GLfloat textureCoords[] = {
            // U V
            subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
            subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
            subTexture.rightTopUV.x, subTexture.rightTopUV.y,
            subTexture.rightTopUV.x, subTexture.leftBottomUV.y
        };

        const GLuint indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        m_vertexCoordBuffer.init(vertexCoords, 2 * 4 * sizeof(GLfloat));
        VertexBufferLayout vertexCoordLayout;
        vertexCoordLayout.addElementLayoutFloat(2, false);
        m_vertexArray.addBuffer(m_vertexCoordBuffer, vertexCoordLayout);

        m_textureCoordBuffer.init(textureCoords, 2 * 4 * sizeof(GLfloat));
        VertexBufferLayout textureCoordLayout;
        textureCoordLayout.addElementLayoutFloat(2, false);
        m_vertexArray.addBuffer(m_textureCoordBuffer, textureCoordLayout);

        m_IndexBuffer.init(indices, 6 * sizeof(GLfloat));

        m_vertexArray.unbind();
        m_IndexBuffer.unbind();
    }
    
    Sprite::~Sprite()
    {
    }

    void Sprite::render(const glm::vec2& position, const glm::vec2& size, const float rotation) const
    {
        m_pShaderProgram->use();

        glm::mat4 model(1.f);

        model = glm::translate(model, glm::vec3(position, 0.f));
        model = glm::translate(model, glm::vec3(0.5 * size.x, 0.5 * size.y, 0.f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
        model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5 * size.y, 0.f));
        model = glm::scale(model, glm::vec3(size, 1.f));

        m_vertexArray.bind();
        m_pShaderProgram->setMatrix4("modelMat", model);

        glActiveTexture(GL_TEXTURE0);
        m_pTexture->bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        m_vertexArray.unbind();
    }
}