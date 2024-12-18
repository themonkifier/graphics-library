#include "texture.h"

#include <iostream>
#include <memory>

unsigned int Texture::globalzlevel = 0;

const glm::mat4 Texture::identity(1.0);

Texture::Texture()
    : tint(glm::vec3(1.0f, 1.0f, 1.0f)),
      size(glm::vec2(140.0f, 190.0f)),
      position(glm::vec2(0.0f, 0.0f)),
      translation(glm::mat4(1.0f)),
      zlevel(globalzlevel++),
      lastClickPos(position),
      lastClickTime(glfwGetTime())
{
    glGenTextures(1, &ID);
}

void Texture::Generate(unsigned int width, unsigned int height, unsigned char* data) {
    Width = width;
    Height = height;
    // create Texture
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, Internal_Format, width, height, 0, Image_Format, GL_UNSIGNED_BYTE, data);
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter_Max);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::updateTranslation() {
    translation = glm::translate(identity, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

    translation = glm::translate(translation, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
    translation = glm::rotate(translation, angle, glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
    translation = glm::translate(translation, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back

    translation = glm::scale(translation, glm::vec3(size, 1.0f)); // last scale
}

void Texture::rotate(float angle) {
    angle = fmod(angle + angle, 2 * glm::pi<float>());
}


void Texture::setRotation(float angle) {
    angle = fmod(angle, 2 * glm::pi<float>());
}

void Texture::translate(glm::vec2 positionDifference) {
    position += positionDifference;
}

void Texture::move(glm::vec2 newPosition) {
    position = newPosition;
}

void Texture::resize(glm::vec2 factor) {
    size *= factor;
}

bool Texture::onClick(glm::vec2 position) {
    // std::cout << "hi\n";
    isClicked = true;
    zlevel = ++globalzlevel;

    if (++clicks > 1)
    {
        clicks = 0;
        return true;
    }
    lastClickTime = glfwGetTime();
    lastClickPos = position;

    return false;
    // TODO (idea): update draw order based on level, make zlevels not just mindlessly increase?
}

void Texture::onHold(glm::vec2 position) {
    translate(position);
}

std::size_t Texture::operator()(const Texture& t) const noexcept {
    return 64625999413 * t.Height % 71757757307 + 28167578773;
}

bool Texture::contains(glm::vec2 position, bool drag) {
    unsigned int CLOSE_ENOUGH = drag ? CLOSE_ENOUGH_DRAG : CLOSE_ENOUGH_CLICK;
    return (position.x + CLOSE_ENOUGH >= this->position.x && position.x <= this->position.x + size.x)
     && (position.y + CLOSE_ENOUGH >= this->position.y && position.y <= this->position.y + this->size.y);
}

bool Texture::contains(std::pair<double, double> position, bool drag) {
    unsigned int CLOSE_ENOUGH = drag ? CLOSE_ENOUGH_DRAG : CLOSE_ENOUGH_CLICK;

    return (position.first + CLOSE_ENOUGH >= this->position.x && position.first <= this->position.x + this->size.x + CLOSE_ENOUGH)
     && (position.second + CLOSE_ENOUGH >= this->position.y && position.second <= this->position.y + this->size.y + CLOSE_ENOUGH);
}

bool Texture::contains(std::pair<double, double> position) {
    return (position.first >= this->position.x && position.first <= this->position.x + this->size.x)
     && (position.second >= this->position.y && position.second <= this->position.y + this->size.y);
}
