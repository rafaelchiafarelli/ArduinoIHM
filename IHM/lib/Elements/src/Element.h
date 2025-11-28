#ifndef _ELEMENT_H_
#define _ELEMENT_H_
#include <standardDefinitions.h>
//all elements are draw from this class, like buttons, textfields, labels, etc
//this is a template class to allow different elements types to have the same interfaces with the outside world
/**
 * @template T The type of the element (e.g., Button, Label, TextField)
 * @class Element
 * @brief A template class representing a generic GUI element.
 * This class serves as a base for various GUI components, providing a common interface.
 * It can be extended to include specific properties and methods for different element types.
 * the standart methods are:
 * setLabel
 * setPosition
 * setSize
 * setState
 * update
 * 
 */

 typedef enum{
    LABEL,
    INDICATOR,
    GAUGE,
    BAR_GRAPH,
    TAB_SELECTOR,
    STATUS_BAR,
    ICON
 }ElementType;

typedef enum {
    top,
    bottom,
    left,
    right
}LocationType;

#define LABEL_STRING_SIZE 15

typedef enum{
    VISIBLE,
    GONE,
    IDLE
}VisibilityControl;

class Element {
protected:
    ElementType elementType; // Type of the element (e.g., Button, Label, TextField)
    char label[LABEL_STRING_SIZE]; // Label of the element
    uint16_t x, y; // Position of the element
    uint16_t width, height; // Size of the element
    uint16_t state; // State of the element (implementation dependent)
    uint8_t location; // position of the label around the object of the element (implementation dependent)
    Display* tft; // Pointer to the display object

public:
    Element(ElementType t, Display* display):elementType(t){
        elementType = t;
        tft = display;
    }
    ~Element(){}
    int getX(){ return x; }
    int getY(){ return y; }
    int getWidth(){ return width; }
    int getHeight(){ return height; }
    int getState(){ return state; }
    char* getLabel(){ return label; }
    int getLocation(){ return location; }

    virtual Element setLabel(char* label){ return *this; }
    virtual Element setLocation(int location){ return *this; }
    virtual Element setPosition(int x, int y){ return *this; }
    virtual Element setSize(int width, int height){ return *this; }    
    virtual Element setState(int state){ return *this; }
    virtual Element update(){ return *this; }   
};


#endif // _ELEMENT_H_
