#ifndef __CEF_PM_SIMPLE_SMART_PTR_H__
#define __CEF_PM_SIMPLE_SMART_PTR_H__

/**	\brief pm 简单智能指针

*/
template<class T>struct PmSimpleSmartPtrTraits {
	static void Release(T* ptr) 	{
		delete ptr;
	}
};

/**	\brief	The PmSimpleSmartPtr class

*/
template<class T,class Tr = PmSimpleSmartPtrTraits<T> >
class PmSimpleSmartPtr
{
private:
	typedef PmSimpleSmartPtr<T,Tr> ThisSmartPtr;
	PmSimpleSmartPtr( const ThisSmartPtr& obj );//don't use copy construct
	ThisSmartPtr& operator=(const ThisSmartPtr& obj );//don't use this function

public:
	PmSimpleSmartPtr():ptr(PM_NULL){}	
	PmSimpleSmartPtr(T* p):ptr(p){}

	void release(){
		if( ptr != PM_NULL ) Tr::Release( ptr );
		ptr = PM_NULL;
	}
	ThisSmartPtr& operator=(T* p) {
		if( ptr == p ) return *this;//防止自己赋值自己
		release();
		ptr = p;
		return *this;
	}
	T** operator &() {
		return &ptr;
	}
	const T** operator &() const {
		return &ptr;
	}
	T& operator *()	{
		return *ptr;
	}
	const T& operator *() const {
		return *ptr;
	}
	T* operator->() {
		return ptr;
	}
	const T* operator->() const {
		return ptr;
	}
	operator T*() {
		return ptr;
	}
	operator const T*() const{
		return ptr;
	}
	T& operator []( long i )	{
		return ptr[ i ];
	}
	const T& operator []( long i ) const {
		return ptr[ i ];
	}
	bool operator !() const {
		return (ptr==PM_NULL);
	}
	operator bool() const {
		return (ptr!=PM_NULL);
	}
	long compare( const long n ) const {
		if( n == (const long)ptr ) return 0;
		if( n > (const long)ptr ) return 1;
		return -1;
	}
	long compare( const T* p ) const {
		if( p == (const T*)ptr ) return 0;
		if( p > (const T*)ptr ) return 1;
		return -1;
	}

	~PmSimpleSmartPtr()//不要派生此类
	{
		release();
	}
	void attach( T* ptr_new )
	{
		if( ptr == ptr_new ) return;//防止自己赋值自己
		release();
		ptr = ptr_new;
	}
	T* detach()
	{
		T* ptrtmp = ptr;
		ptr = PM_NULL;
		return ptrtmp;
	}
private:
	T* ptr;
};

template<class T,class Tr> bool operator==( const PmSimpleSmartPtr<T,Tr>& left, const long right ){
	return left.compare(right) == 0;
}
template<class T,class Tr> bool operator!=( const PmSimpleSmartPtr<T,Tr>& left, const long right ){
	return left.compare(right) != 0;
}
template<class T,class Tr> bool operator==( const long left, const PmSimpleSmartPtr<T,Tr>& right ){
	return right.compare(left) == 0;
}
template<class T,class Tr> bool operator!=( const long left, const PmSimpleSmartPtr<T,Tr>& right ){
	return right.compare(left) != 0;
}
template<class T,class Tr> bool operator==( const PmSimpleSmartPtr<T,Tr>& left, const T* right ){
	return left.compare(right) == 0;
}
template<class T,class Tr> bool operator!=( const PmSimpleSmartPtr<T,Tr>& left, const T* right ){
	return left.compare(right) != 0;
}
template<class T,class Tr> bool operator==( const T* left, const PmSimpleSmartPtr<T,Tr>& right ){
	return right.compare(left) == 0;
}
template<class T,class Tr> bool operator!=( const T* left, const PmSimpleSmartPtr<T,Tr>& right ){
	return right.compare(left) != 0;
}

#endif
