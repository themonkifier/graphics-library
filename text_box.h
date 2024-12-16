#ifndef TEXT_BOX_HH
#define TEXT_BOX_HH

#include <string>
#include <GLFW/glfw3.h>

class TextBox {
public:
    bool selected;
    void* enterCallbackData;
    void (*enterCallback)(TextBox*, void*); //function to call when enter is pressed

    TextBox(void (*enterCallback)(TextBox*, void*), void* enterCallbackData);

    /* glfw callbacks */
    void keyCallback(int key, int action);
    void characterCallback(unsigned int codepoint);

private:
    std::size_t cursor_pos;
    std::string contained_text;
};

#endif
