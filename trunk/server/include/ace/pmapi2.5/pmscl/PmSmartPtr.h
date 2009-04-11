#ifndef __CEF_PM_SMART_PTR_H__
#define __CEF_PM_SMART_PTR_H__

/**	
 *@file PmSmartPtr.h 
 *@brief 智能指针
*/

/**	\brief	The PmSmartPtr

*/

template<class InterfaceType>
class PmSmartPtr//simple smart pointer
{
private:
	const char* pszCreateFileName;
	int nCreateFileLine;
	typedef InterfaceType T;
	T* ptr;
	typedef PmSmartPtr< T > ThisSmartPtr;

	ThisSmartPtr& attachRef( T* ptr_new ){
		if( ptr == ptr_new ) return *this;//防止自己赋值自己
		safeRelease();
		ptr = ptr_new;
		if( ptr != PM_NULL ) ptr->addRef();
		return *this;
	}
public:
	void safeRelease(){
		if( ptr != PM_NULL )
		{
			long retref = ptr->release();
			ptr = PM_NULL;
		}
	}
	PM_BOOL isValid()const{
		return ptr != PM_NULL;
	}
	PmSmartPtr():ptr(PM_NULL),
		pszCreateFileName("unknown"),
		nCreateFileLine(0)
	{
	}
	PmSmartPtr( const char* pszFileName, int nFileLine ):ptr(PM_NULL),
		pszCreateFileName(pszFileName),
		nCreateFileLine(nFileLine)
	{
	}
	PmSmartPtr( const ThisSmartPtr& obj ) : 
		pszCreateFileName(obj.pszCreateFileName),
		nCreateFileLine(obj.nCreateFileLine)
	{
		long retref = 0;
		ptr = obj.ptr;
		if( ptr != PM_NULL )
			retref = ptr->addRef();
	}
	PmSmartPtr( const T* pobj ) : 
		pszCreateFileName(__FILE__),
		nCreateFileLine(__LINE__)
	{
		long retref = 0;
		ptr = (T*)pobj;
		if( ptr != PM_NULL )
			retref = ptr->addRef();
	}

	~PmSmartPtr(){//不要派生此类
		safeRelease();
	}
	T& operator *()	{
		return *ptr;
	}
	const T& operator *() const{
		return *ptr;
	}
	ThisSmartPtr& operator=(const ThisSmartPtr& obj ) {
		pszCreateFileName = obj.pszCreateFileName;
		nCreateFileLine = obj.nCreateFileLine;
		return attachRef( obj.ptr );
	}
	void** getPtr(){
		return (void**)&ptr;
	}
	T* getTypePtr(){
		return ptr;
	}
	T* operator->() {
		if( ptr == PM_NULL )
			PmThrowExceptionFmt( PM_HERE, PM_ERROR_SYSTEM, "pointer is NULL,create on %s(%d)",pszCreateFileName, nCreateFileLine);//@@@
		return ptr;
	}
	T* operator->() const {
		if( ptr == PM_NULL )
			PmThrowExceptionFmt( PM_HERE, PM_ERROR_SYSTEM, "pointer is NULL,create on %s(%d)",pszCreateFileName, nCreateFileLine);//@@@
		return ptr;
	}
	bool operator !() const {
		return (ptr==PM_NULL);
	}
	operator bool() const {
		return (ptr!=PM_NULL);
	}
	operator bool(){
		return (ptr!=PM_NULL);
	}

	ThisSmartPtr& operator=(T*p){
		attachRef( p );
		return *this;
	}
//	T** operator &() const	{
//		return &ptr;
//	}
//	T** operator &()	{
//		return &ptr;
//	}
	operator T*()	{
		return ptr;
	}
	operator T*() const	{
		return ptr;
	}
	T& operator []( int i )	{
		return ptr[ i ];
	}
	T& operator []( int i )	const {
		return ptr[ i ];
	}
	void attach( T* ptr_new )
	{
		safeRelease();
		ptr = ptr_new;
	}
	T* detach()
	{
		T* ptmp = ptr;
		ptr = PM_NULL;
		return ptmp;
	}
	int compare( const ThisSmartPtr & p ) const {
		if( p.ptr == ptr ) return 0;
		if( p.ptr > ptr ) return 1;
		return -1;
	}
	int compare( const T* p ) const {
		if( p == ptr ) return 0;
		if( p > ptr ) return 1;
		return -1;
	}
};

/**	\brief	PM_DECLARE_OPERATOR_COMPARE_LEFT

*/
#define PM_DECLARE_OPERATOR_COMPARE_LEFT( type1, type2 ) \
	template<class T> bool operator==( type1 left, type2 right ){\
		return left.compare(right) == 0;\
	}\
	template<class T> bool operator!=( type1 left, type2 right ){\
		return left.compare( right ) != 0;\
	}
/**	\brief	PM_DECLARE_OPERATOR_COMPARE_RIGHT

*/
#define PM_DECLARE_OPERATOR_COMPARE_RIGHT( type1, type2 ) \
	template<class T> bool operator==( type1 left, type2 right ){\
		return right.compare(left) == 0;\
	}\
	template<class T> bool operator!=( type1 left, type2 right ){\
		return right.compare( left ) != 0;\
	}
/**	\brief	PM_DECLARE_OPERATOR_COMPARE_LEFT_ALL

*/
#define PM_DECLARE_OPERATOR_COMPARE_LEFT_ALL( type1, type2 ) \
	PM_DECLARE_OPERATOR_COMPARE_LEFT( const type1, const type2 )\
	PM_DECLARE_OPERATOR_COMPARE_LEFT( const type1, type2 )\
	PM_DECLARE_OPERATOR_COMPARE_LEFT( type1, const type2 )\
	PM_DECLARE_OPERATOR_COMPARE_LEFT( type1, type2 )
/**	\brief	PM_DECLARE_OPERATOR_COMPARE_RIGHT_ALL

*/
#define PM_DECLARE_OPERATOR_COMPARE_RIGHT_ALL( type1, type2 ) \
	PM_DECLARE_OPERATOR_COMPARE_RIGHT( const type1, const type2 )\
	PM_DECLARE_OPERATOR_COMPARE_RIGHT( const type1, type2 )\
	PM_DECLARE_OPERATOR_COMPARE_RIGHT( type1, const type2 )\
	PM_DECLARE_OPERATOR_COMPARE_RIGHT( type1, type2 )

PMCOM_DECLARE_OPERATOR_COMPARE_LEFT_ALL( PmSmartPtr< T > &, PmSmartPtr< T > & )

PMCOM_DECLARE_OPERATOR_COMPARE_LEFT( PmSmartPtr< T >& , const long )
PMCOM_DECLARE_OPERATOR_COMPARE_RIGHT( const long& , PmSmartPtr< T >& )
PMCOM_DECLARE_OPERATOR_COMPARE_LEFT( const PmSmartPtr< T >& , const long )
PMCOM_DECLARE_OPERATOR_COMPARE_RIGHT( const long& , const PmSmartPtr< T >& )

PMCOM_DECLARE_OPERATOR_COMPARE_LEFT_ALL( PmSmartPtr< T > &, T* )
PMCOM_DECLARE_OPERATOR_COMPARE_RIGHT_ALL( T*, PmSmartPtr< T > & )


#endif
