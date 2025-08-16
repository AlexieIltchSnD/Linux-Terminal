/**
 * Code provided by the course lecturer.
 * **/

#ifndef FIRSTPROJECT_RCOBJECT_H
#define FIRSTPROJECT_RCOBJECT_H

class RCObject {
protected:
    RCObject() : refCount(0), shareable(true) { }
    RCObject(const RCObject&) : refCount(0), shareable(true) { }
    RCObject& operator=(const RCObject&) { return *this; }

    virtual ~RCObject(){};

public:
    void addReference() { ++refCount; }
    void removeReference() { if (--refCount == 0) delete this; }
    int  getRefCount() const{ return refCount; }
    void markUnshareable() { shareable = false; }
    bool isShareable() const { return shareable; }
    bool isShared() const { return refCount > 1; }

private:
    int refCount;
    bool shareable;
};
#endif //FIRSTPROJECT_RCOBJECT_H