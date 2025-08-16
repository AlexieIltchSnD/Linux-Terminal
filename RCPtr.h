/**
 * Code provided by the course lecturer.
 * **/
#ifndef FIRSTPROJECT_RCPTR_H
#define FIRSTPROJECT_RCPTR_H
#include <iostream>

template<class T>
class RCPtr
{
	T* pointee;
	void init();
public:
	RCPtr(T* realPtr=nullptr): pointee(realPtr) { init();}
	RCPtr(const RCPtr& rhs): pointee(rhs.pointee) 
	{ init();}
	~RCPtr(){ if (pointee) pointee->removeReference();}
	RCPtr& operator=(const RCPtr& rhs);
	T* operator->() const{return pointee;}
	T& operator*() const{return *pointee;}
};

template<class T>
void RCPtr<T>::init()
{
  if(pointee== nullptr) return;

  if(pointee->isShareable() == false) {
    pointee= new T(*pointee); // requires a copy c'tor for T
  }
  pointee->addReference();
}

template<class T>
RCPtr<T>& RCPtr<T>::operator=(const RCPtr<T>& rhs)
{
  if (pointee != rhs.pointee)	{
    T* oldPointee = pointee; 
    pointee = rhs.pointee;
    init();
    if(oldPointee)
      oldPointee->removeReference();
  }
  return *this;
}
#endif //FIRSTPROJECT_RCPTR_H