#ifndef ELEMENT_H
#define ELEMENT_H

class Element 
{
    friend class ElementXMLFactory;
public:
    enum Type {
        NONE            = 0x0,
        COMPUTER        = 0x1,
        STORE           = 0x2,
        SWITCH          = 0x4,
        LINK            = 0x8,
        COMPUTATIONAL   = COMPUTER | STORE,
        NETWORK         = SWITCH | LINK,
        NODE            = COMPUTATIONAL | SWITCH
    };
protected:    
    Element()
    :
        type(NONE)
    {}

    virtual ~Element() {}
public:
    virtual void setAvailable(bool available = true){
        this->available = available; 
    } 
    
    virtual bool canHostAssignment(const Element * other ) {
        if ( !isAvailable() ) return false;
        if ( isVirtual() ) return false;
        if ( !other->isVirtual() ) return false;
        if ( type != other->type ) return false;
        return true;
    }

    virtual bool assign(const Element * other) = 0;
public:
    inline bool isComputer() const { return type & COMPUTER; }
    inline bool isStore() const { return type & STORE;}
    inline bool isSwitch() const { return type & SWITCH; }
    inline bool isLink() const { return type & LINK; }
    inline bool isComputational() const { return type & COMPUTATIONAL; }
    inline bool isNetworking() const { return type & NETWORKING; }
    inline bool isNode() const { return type & NODE; }
    inline bool isEdge() const { return isLink(); }
    inline bool isAvailable() const { return available; }
    inline bool isPhysical() const { return physical; }
    inline bool isVirtual() const { return !physical; }
protected:
    Type type;
    bool physical;
    bool available;
};

#endif // ELEMENT_H
