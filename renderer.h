#ifndef RENDERER_HH
#define RENDERER_HH

#include <glad/glad.h>

#include "texture.h"
#include "shader.h"
#include "card.h"
#include "pile.h"

class Renderer
{
public:
    // Constructor (inits shaders/shapes)
    Renderer(Shader shader);
    // Destructor
    ~Renderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(Texture& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
    void DrawTexture(Texture texture);
    void DrawCard(Card c);

    void DrawPile(Pile& pile)
    {
        for (Card& card : pile)
        {
            DrawTexture(card.texture);
        }
    }

protected:
    // Render state
    Shader shader;
    unsigned int VAO, VBO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();
};

#endif
