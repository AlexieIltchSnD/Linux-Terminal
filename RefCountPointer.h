/**
 * Code provided by the course lecturer.
 * **/
#ifndef FIRSTPROJECT_REFCOUNTPOINTER_H
#define FIRSTPROJECT_REFCOUNTPOINTER_H
template <class T>
class RefCountPointer {
  T* p;
  unsigned* count;
  void checkIfScavengable() {
    if (*count == 0) {
        delete count;
        delete p;
      }
  }
public:
  RefCountPointer (T* s) : p(s), count(new unsigned) {*count = 1;}
  RefCountPointer (const RefCountPointer& rcp) : p(rcp.p), count(rcp.count)
    { ++(*count); }
  ~RefCountPointer() { --(*count); checkIfScavengable(); } 
  T& operator*() const {return *p;}
  T* operator->() const {return p;}
  RefCountPointer& operator= (const RefCountPointer& rcp) {
    ++(*(rcp.count));
    --(*count);
    checkIfScavengable();
    p = rcp.p;
    count = rcp.count;
    return *this;
  }

  bool operator== (const RefCountPointer<T>& ptr) const
    {return ptr.p == p;}

  bool operator!= (const RefCountPointer<T>& ptr) const
    {return ptr.p != p;}

};

#endif //FIRSTPROJECT_REFCOUNTPOINTER_H
/* Remaining TODOs:
	nullptr pointers
	const pointers
*/