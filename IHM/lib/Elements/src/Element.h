#ifndef _ELEMENT_H_
#define _ELEMENT_H_
#include <standardDefinitions.h>
// Base for every on-screen widget (Label, LED, Icon, TabSelector, ...):
// shared position/size/state/label storage plus the Display* every widget
// draws through. Not polymorphic -- nothing in this codebase ever holds an
// Element* or Element& and calls through it, every call site uses its
// widget's own concrete type directly, so these are plain (non-virtual)
// methods, not an interface to override. Subclasses that need a setter with
// real behavior (e.g. LED::setLocation) just declare their own; a subclass
// that has nothing to do for a given setter simply doesn't declare it,
// rather than providing a no-op override that looks like real API surface.

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
    char label[LABEL_STRING_SIZE]; // Label of the element
    uint16_t x, y; // Position of the element
    uint16_t width, height; // Size of the element
    uint16_t state; // State of the element (implementation dependent)
    uint8_t location; // position of the label around the object of the element (implementation dependent)
    Display* tft; // Pointer to the display object

public:
    Element(Display* display){
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
};


#endif // _ELEMENT_H_
