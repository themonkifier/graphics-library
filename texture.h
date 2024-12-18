#ifndef TEXTURE_HH
#define TEXTURE_HH

// For some reason this file won't compile without...
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Texture
{
public:
    unsigned int ID;
    unsigned int Width = 0;
    unsigned int Height = 0;
    unsigned int Internal_Format = GL_RGB;
    unsigned int Image_Format = GL_RGB;
    unsigned int Wrap_S = GL_REPEAT;
    unsigned int Wrap_T = GL_REPEAT;
    unsigned int Filter_Min = GL_LINEAR;
    unsigned int Filter_Max = GL_LINEAR;

    unsigned int zlevel;
    static unsigned int globalzlevel;
    
    unsigned int clicks = 0;

    bool isClicked = false;
    bool isDraggable = true;

    glm::vec2 size; // texture size
    glm::vec2 position; // position in space
    glm::vec2 lastClickPos;
    double lastClickTime;
    glm::vec3 tint; // display tint
    float angle = 0.0f; // rotation angle
    glm::mat4 translation; // overall translation

    Texture();
    std::size_t operator()(const Texture& t) const noexcept;

    void Generate(unsigned int width, unsigned int height, unsigned char* data);
    void Bind() const;
    /**
      * @brief update the translation for display on the screen
      */
    void updateTranslation();

    /**
      * @brief rotate by the given angle (in radians)
      * @note 0.05f - 0.1f seems to be a pretty good range of values
      * @param angle angle to rotate by (in radians)
      */
    void rotate(float angle);

    /**
      * @brief set the current rotation to the given angle (in radians)
      * @param angle angle to rotate to (in radians)
      */
    void setRotation(float angle);

    /**
      * @brief translate relative to the current position
      */
    void translate(glm::vec2 positionDifference);

    /**
      * @brief move to an absolute location
      */
    void move(glm::vec2 newPosition);

    /**
      * @brief resizes by a given factor
      */
    void resize(glm::vec2 factor);

    /**
      * @brief action to perform when clicked on
      * @return true when the click was a double click, false otherwise
      */
    bool onClick(glm::vec2 position);

    /**
      * @brief action to preform when clicked and held on
      */
    void onHold(glm::vec2 position);

    bool contains(glm::vec2 position, bool drag);
    bool contains(std::pair<double, double> position, bool drag);
    bool contains(std::pair<double, double> position);
private:
    static const glm::mat4 identity; // identity matrix - does this stop additional copies from being made?
    static constexpr unsigned int CLOSE_ENOUGH_CLICK = 5; // define how many pixels is "close enough" to the card - without this, clicking and dragging won't feel as smooth since you'll sometimes "drop" the card
    static constexpr unsigned int CLOSE_ENOUGH_DRAG = 100; // define how many pixels is "close enough" to the card - without this, clicking and dragging won't feel as smooth since you'll sometimes "drop" the card
};

template<> struct std::hash<Texture>
{
    std::size_t operator()(const Texture& t) const noexcept
    {
        return 64625999413 * t.Height % 71757757307 + 28167578773;
    }
};

#endif
