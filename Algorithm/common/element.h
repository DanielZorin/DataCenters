#ifndef ELEMENT_H
#define ELEMENT_H

/// common datacenter and request underlying model

#include <string>

using std::string;

class Element {
private:
   Element();
protected:
   Element(string elementName = "unnamed", unsigned long elementCapacity = 0)
   :  name(elementName),
      capacity(elementCapacity)
   {
   
   }

   virtual ~Element() {}
public:
   virtual unsigned long getCapacity() { return capacity; }
   virtual string getName() { return name; }
   virtual bool isAssignmentPossible(Element const & other) { return capacity >= other.capacity; }
   virtual void Assign(Element const & other)
   { 
      if (isAssignmentPossible(other)) 
         capacity -= other.capacity;
   }
   
private:
   unsigned long capacity;
   string name; 
};

#endif // ELEMENT_H
