#ifndef __CEF_PM_COM_SMART_PTR_H__
#define __CEF_PM_COM_SMART_PTR_H__

/**	
 *@file PmComSmartPtr.h
 *@brief COM智能指针文件，
*/

//#define PM_SMARTPTR_DISPLAY_DETAIL

#ifdef PM_SMARTPTR_DISPLAY_DETAIL
/**	\brief	The PmUnknownPtr class

*/
#define PM_SMARTPTR_DEBUG0( pszDetail ) pm_debug( "PmComSmartPtr[%08x][%08x]:" pszDetail,getIID(), ptr )
/**	\brief	The PmUnknownPtr class

*/
#define PM_SMARTPTR_DEBUG1( pszDetail,pszDetaiPara1 ) pm_debug( "PmComSmartPtr[%08x][%08x]:" pszDetail ,getIID(), ptr, pszDetaiPara1 )
/**	\brief	The PmUnknownPtr class

*/
#define PM_SMARTPTR_DEBUG2( pszDetail,pszDetaiPara1,pszDetaiPara2 ) pm_debug( "PmComSmartPtr[%08x][%08x]:" pszDetail ,getIID(), ptr, pszDetaiPara1,pszDetaiPara2 )
#else
/**	\brief	The PmUnknownPtr class

*/
#define PM_SMARTPTR_DEBUG0( pszDetail )
/**	\brief	The PmUnknownPtr class

*/
#define PM_SMARTPTR_DEBUG1( pszDetail,pszDetaiPara1 ) 
/**	\brief	The PmUnknownPtr class

*/
#define PM_SMARTPTR_DEBUG2( pszDetail,pszDetaiPara1,pszDetaiPara2 ) 
#endif
/**	\brief	The PmUnknownPtr class

*/

class PmUnknownPtr
{
private:
	typedef IPmUnknown T;
	T* ptr;
	typedef PmUnknownPtr ThisSmartPtr;
	ThisSmartPtr& attachRef( T* ptr_new ){
		if( ptr_new == PM_NULL ) 
		{
			safeRelease();
			return *this;
		}
		IPmUnknown* pUnknown = PM_NULL;
		if( !ptr_new->queryInterface( IID_IPmUnknown, (void**)&pUnknown) || pUnknown == PM_NULL ) 
			PmThrowException( PM_HERE, PM_ERROR_SYSTEM, "ptr_new's unknown ineterface is NULL");
		safeRelease();
		ptr = pUnknown;
		return *this;
	}
	const char* pszCreateFileName;
	int nCreateFileLine;

public:
	const char* getCreateFileName() const{
		return pszCreateFileName;
	}
	const int getCreateFileLine() const{
		return nCreateFileLine;
	}

	void safeRelease(){
		if( ptr != PM_NULL ) 
		{
			long retref = 0;
			retref = ptr->release();
			PM_SMARTPTR_DEBUG1( "release %d", retref );
			ptr = PM_NULL;
		}
	}
	PM_BOOL isValid()const{
		return ptr != PM_NULL;
	}

    const PM_IID getIID(void) const  {
        return IID_IPmUnknown;
    }
	PmUnknownPtr( const char* pszFileName, int nFileLine ):ptr(PM_NULL),
		pszCreateFileName( pszFileName ),
		nCreateFileLine( nFileLine )
	{
		PM_SMARTPTR_DEBUG0( "normal constructor1 to NULL" );
	}
	PmUnknownPtr():ptr(PM_NULL),
		pszCreateFileName( "unknown" ),
		nCreateFileLine( 0 )
	{
		PM_SMARTPTR_DEBUG0( "normal constructor to NULL" );
	}
	PmUnknownPtr( const ThisSmartPtr& obj )
	{
		ptr = obj.ptr;
		pszCreateFileName = obj.pszCreateFileName;
		nCreateFileLine = obj.nCreateFileLine;
		if( ptr != PM_NULL )
			ptr->addRef();
	}
	~PmUnknownPtr(){//不要派生此类
		safeRelease();
	}
	ThisSmartPtr& operator=(const ThisSmartPtr& obj ) {
		return attachRef( obj.ptr );
	}
	IPmUnknown** getPtr(){
		return &ptr;
	}
	operator T*(){
		return ptr;
	}
	T* operator->() {
		if( ptr == PM_NULL )
			PmThrowExceptionFmt( PM_HERE, PM_ERROR_SYSTEM, "unknown pointer is NULL,create on %s(%d)",pszCreateFileName, nCreateFileLine);//@@@
		return ptr;
	}
	T* operator->() const {
		if( ptr == PM_NULL )
			PmThrowExceptionFmt( PM_HERE, PM_ERROR_SYSTEM, "unknown pointer is NULL,create on %s(%d)",pszCreateFileName, nCreateFileLine);//@@@
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
	int compare( const ThisSmartPtr& p ) const {
		if( p.ptr == ptr ) return 0;
		if( p.ptr > ptr ) return 1;
		return -1;
	}
	int compare( const long n ) const {
		if( n == (const long)ptr ) return 0;
		if( n > (const long)ptr ) return 1;
		return -1;
	}
	int compare( const T* p ) const {
		if( p == ptr ) return 0;
		if( p > ptr ) return 1;
		return -1;
	}
};

/**	\brief	The pmcom_null_ptr_real function

	\param	pszFileName	a parameter of type const char*
	\param	nFileLine	a parameter of type int

	\return	PmUnknownPtr

	
*/
PmUnknownPtr pmcom_null_ptr_real( const char* pszFileName, int nFileLine );
/**	\brief	The PmComSmartPtr class

*/
#define pmcom_null_ptr() pmcom_null_ptr_real( __FILE__, __LINE__ )

template<class InterfaceType,PM_IID IID_PmInterface>
/**	\brief	The PmComSmartPtr class

*/
class PmComSmartPtr
{
private:
	const char* pszCreateFileName;
	int nCreateFileLine;
	typedef InterfaceType T;
	T* ptr;
	typedef PmComSmartPtr<T,IID_PmInterface> ThisSmartPtr;

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
			PM_SMARTPTR_DEBUG1( "release %d", retref );
			ptr = PM_NULL;
		}
	}
	PM_BOOL isValid()const{
		return ptr != PM_NULL;
	}
    const PM_IID getIID() const  {
        return IID_PmInterface;
    }
	PmComSmartPtr():ptr(PM_NULL),
		pszCreateFileName("unknown"),
		nCreateFileLine(0)
	{
		PM_SMARTPTR_DEBUG0( "normal constructor to NULL" );
	}
	PmComSmartPtr( const char* pszFileName, int nFileLine ):ptr(PM_NULL),
		pszCreateFileName(pszFileName),
		nCreateFileLine(nFileLine)
	{
		PM_SMARTPTR_DEBUG0( "normal constructor1 to NULL" );
	}

	PmComSmartPtr( const ThisSmartPtr& obj ) : 
		pszCreateFileName(obj.pszCreateFileName),
		nCreateFileLine(obj.nCreateFileLine)
	{
		long retref = 0;
		ptr = obj.ptr;
		if( ptr != PM_NULL )
			retref = ptr->addRef();
		PM_SMARTPTR_DEBUG2( "constructor this& to %08x,ref %d", obj.ptr, retref );
	}
    PmComSmartPtr( const PmUnknownPtr& ptrUnknown) : ptr(PM_NULL),
		pszCreateFileName( ptrUnknown.getCreateFileName() ),
		nCreateFileLine( ptrUnknown.getCreateFileLine() )
	{
		if( !ptrUnknown.isValid() ) return;
		if( !ptrUnknown->queryInterface(getIID(), (void**)&ptr)) 
			PmThrowExceptionFmt( PM_HERE, PM_ERROR_SYSTEM, "can not get interface[0x%x],create on %s(%d)",getIID(),pszCreateFileName,nCreateFileLine );//@@@
 	}

	~PmComSmartPtr(){//不要派生此类
		safeRelease();
	}
	T& operator *()	{
		PM_SMARTPTR_DEBUG0( "operator *" );
		return *ptr;
	}
	const T& operator *() const{
		PM_SMARTPTR_DEBUG0( "operator * const" );
		return *ptr;
	}
	ThisSmartPtr& operator=(const ThisSmartPtr& obj ) {
		pszCreateFileName = obj.pszCreateFileName;
		nCreateFileLine = obj.nCreateFileLine;
		PM_SMARTPTR_DEBUG1( "operator =(cosnt this&[%08x])", obj.ptr );
		return attachRef( obj.ptr );
	}
	ThisSmartPtr& operator=(const PmUnknownPtr& obj ) {
		pszCreateFileName = obj.getCreateFileName();
		nCreateFileLine = obj.getCreateFileLine();
		safeRelease();
		if( !obj.isValid() ) return *this;
		if( !obj->queryInterface(getIID(), (void**)&ptr)) 
			PmThrowExceptionFmt( PM_HERE, PM_ERROR_SYSTEM, "can not get interface[0x%x] create on %s(%d)",getIID(), pszCreateFileName, nCreateFileLine);//@@@
		return *this;
	}	
	void** getPtr(){
		PM_SMARTPTR_DEBUG0( "getPtr" );
		return (void**)&ptr;
	}
	T* getTypePtr(){
		return ptr;
	}
	T* getTypePtr() const {
		return ptr;
	}

	T* operator->() {
		PM_SMARTPTR_DEBUG0( "operator->" );
		if( ptr == PM_NULL )
			PmThrowExceptionFmt( PM_HERE, PM_ERROR_SYSTEM, "[%08x]pointer is NULL,create on %s(%d)",getIID(), pszCreateFileName, nCreateFileLine);//@@@
		return ptr;
	}
	T* operator->() const {
		PM_SMARTPTR_DEBUG0( "operator-> const" );
		if( ptr == PM_NULL )
			PmThrowExceptionFmt( PM_HERE, PM_ERROR_SYSTEM, "[%08x]pointer is NULL,create on %s(%d)",getIID(), pszCreateFileName, nCreateFileLine);//@@@
		return ptr;
	}
	bool operator !() const {
		PM_SMARTPTR_DEBUG0( "operator !" );
		return (ptr==PM_NULL);
	}
	operator bool() const {
		PM_SMARTPTR_DEBUG0( "operator bool()" );
		return (ptr!=PM_NULL);
	}
	operator bool(){
		PM_SMARTPTR_DEBUG0( "operator bool()" );
		return (ptr!=PM_NULL);
	}
	int compare( const ThisSmartPtr & p ) const {
		PM_SMARTPTR_DEBUG0( "compare( const this& )" );
		if( p.ptr == ptr ) return 0;
		if( p.ptr > ptr ) return 1;
		return -1;
	}
	int compare( const PmUnknownPtr& p ) const {
		PM_SMARTPTR_DEBUG0( "compare( const PmUnknownPtr& )" );
		return p.compare( ((ThisSmartPtr*)this)->operator PmUnknownPtr() );
	}

	int compare( const long n ) const {
		PM_SMARTPTR_DEBUG0( "compare( const long )" );
		if( n == (const long)ptr ) return 0;
		if( n > (const long)ptr ) return 1;
		return -1;
	}
	int compare( const T* p ) const {
		PM_SMARTPTR_DEBUG0( "compare( const T* )" );
		if( p == ptr ) return 0;
		if( p > ptr ) return 1;
		return -1;
	}
	operator PmUnknownPtr(){
		PM_SMARTPTR_DEBUG0( "operator PmUnknownPtr" );
		if( ptr == PM_NULL ) return pmcom_null_ptr_real( pszCreateFileName, nCreateFileLine);
		PmUnknownPtr ptrUnknown( pszCreateFileName, nCreateFileLine );
		if( !ptr->queryInterface(IID_IPmUnknown, (void**)&ptrUnknown)) 
			PmThrowExceptionFmt( PM_HERE, PM_ERROR_SYSTEM, "can not get interface[0x%x]",IID_IPmUnknown);
		return ptrUnknown;
	}

};
/**	\brief	Redefinition of PmUnknownPtr as PmVariant
*/
typedef PmUnknownPtr PmVariant;

/*******************************************************************************************/
/*
	模版操作符函数的实现
	肖保锋 2006-10-12
*/

template<class T,PM_IID IID_PmInterface>
/**	\brief	operator==

*/
bool operator==(const PmComSmartPtr<T,IID_PmInterface>& left,const PmComSmartPtr<T,IID_PmInterface>& right)
{
	return left.compare(right) == 0;
}

template<class T,PM_IID IID_PmInterface>
/**	\brief	operator==

*/
bool operator==(const PmComSmartPtr<T,IID_PmInterface>& left,const long right)
{
	return left.compare(right) == 0;
}

template<class T,PM_IID IID_PmInterface>
/**	\brief	operator==

*/
bool operator==(const PmComSmartPtr<T,IID_PmInterface>& left,const T* right)
{
	return left.compare(right) == 0;
}

template<class T,PM_IID IID_PmInterface>
/**	\brief	operator==

*/
bool operator==(const PmComSmartPtr<T,IID_PmInterface>& left,const PmUnknownPtr& right)
{
	return left.compare(right) == 0;
}

template<class T,PM_IID IID_PmInterface>
/**	\brief	operator!=

*/
bool operator!=(const PmComSmartPtr<T,IID_PmInterface>& left,const PmComSmartPtr<T,IID_PmInterface>& right)
{
	return left.compare(right) != 0;
}

template<class T,PM_IID IID_PmInterface>
/**	\brief	operator!=

*/
bool operator!=(const PmComSmartPtr<T,IID_PmInterface>& left,const long right)
{
	return left.compare(right) != 0;
}

template<class T,PM_IID IID_PmInterface>
/**	\brief	operator!=

*/
bool operator!=(const PmComSmartPtr<T,IID_PmInterface>& left,const T* right)
{
	return left.compare(right) != 0;
}

template<class T,PM_IID IID_PmInterface>
/**	\brief	operator!=

*/
bool operator!=(const PmComSmartPtr<T,IID_PmInterface>& left,const PmUnknownPtr& right)
{
	return left.compare(right) != 0;
}


template<class T,PM_IID IID_PmInterface>
/**	\brief	operator==

*/
bool operator==(const long left,const PmComSmartPtr<T,IID_PmInterface>& right)
{
	return right.compare(left) == 0;
}

template<class T,PM_IID IID_PmInterface>
/**	\brief	operator==

*/
bool operator==(const T* left,const PmComSmartPtr<T,IID_PmInterface>& right)
{
	return right.compare(left) == 0;
}

template<class T,PM_IID IID_PmInterface>
/**	\brief	operator==

*/
bool operator==(const PmUnknownPtr& left,const PmComSmartPtr<T,IID_PmInterface>& right)
{
	return right.compare(left) == 0;
}

template<class T,PM_IID IID_PmInterface>
/**	\brief	operator!=

*/
bool operator!=(const long left,const PmComSmartPtr<T,IID_PmInterface>& right)
{
	return right.compare(left) != 0;
}

template<class T,PM_IID IID_PmInterface>
/**	\brief	operator!=

*/
bool operator!=(const T* left,const PmComSmartPtr<T,IID_PmInterface>& right)
{
	return right.compare(left) != 0;
}

template<class T,PM_IID IID_PmInterface>
/**	\brief	operator!=

*/
bool operator!=(const PmUnknownPtr& left,const PmComSmartPtr<T,IID_PmInterface>& right)
{
	return right.compare(left) != 0;
}

/*******************************************************************************************/

/**	\brief	PMCOM_SMARTPTR_TYPE

*/
#define PMCOM_SMARTPTR_TYPE PmComSmartPtr<T,IID_PmInterface>

/**	\brief	PMCOM_DECLARE_OPERATOR_COMPARE_LEFT

*/
#define PMCOM_DECLARE_OPERATOR_COMPARE_LEFT( type1, type2 ) \
	template<class T,PM_IID IID_PmInterface> bool operator==( type1 left, type2 right ){\
		return left.compare(right) == 0;\
	}\
	template<class T,PM_IID IID_PmInterface> bool operator!=( type1 left, type2 right ){\
		return left.compare( right ) != 0;\
	}

/**	\brief	PMCOM_DECLARE_OPERATOR_COMPARE_RIGHT
	
*/
#define PMCOM_DECLARE_OPERATOR_COMPARE_RIGHT( type1, type2 ) \
	template<class T,PM_IID IID_PmInterface> bool operator==( type1 left, type2 right ){\
		return right.compare(left) == 0;\
	}\
	template<class T,PM_IID IID_PmInterface> bool operator!=( type1 left, type2 right ){\
		return right.compare( left ) != 0;\
	}

/**	\brief	PMCOM_DECLARE_OPERATOR_COMPARE_LEFT_ALL
	
*/
#define PMCOM_DECLARE_OPERATOR_COMPARE_LEFT_ALL( type1, type2 ) \
	PMCOM_DECLARE_OPERATOR_COMPARE_LEFT( const type1, const type2 )\
	PMCOM_DECLARE_OPERATOR_COMPARE_LEFT( const type1, type2 )\
	PMCOM_DECLARE_OPERATOR_COMPARE_LEFT( type1, const type2 )\
	PMCOM_DECLARE_OPERATOR_COMPARE_LEFT( type1, type2 )

/**	\brief	The PMCOM_DECLARE_OPERATOR_COMPARE_RIGHT function

*/
#define PMCOM_DECLARE_OPERATOR_COMPARE_RIGHT_ALL( type1, type2 ) \
	PMCOM_DECLARE_OPERATOR_COMPARE_RIGHT( const type1, const type2 )\
	PMCOM_DECLARE_OPERATOR_COMPARE_RIGHT( const type1, type2 )\
	PMCOM_DECLARE_OPERATOR_COMPARE_RIGHT( type1, const type2 )\
	PMCOM_DECLARE_OPERATOR_COMPARE_RIGHT( type1, type2 )



/**	\brief	PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_LEFT

*/
#define PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_LEFT( type1, type2 ) \
	inline bool operator==( type1 left, type2 right ){\
		return left.compare(right) == 0;\
	}\
	inline bool operator!=( type1 left, type2 right ){\
		return left.compare( right ) != 0;\
	}
/**	\brief	PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_RIGHT

*/
#define PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_RIGHT( type1, type2 ) \
	inline bool operator==( type1 left, type2 right ){\
		return right.compare(left) == 0;\
	}\
	inline bool operator!=( type1 left, type2 right ){\
		return right.compare( left ) != 0;\
	}

/**	\brief	PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_LEFT_ALL

*/
#define PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_LEFT_ALL( type1, type2 ) \
	PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_LEFT( const type1, const type2 )\
	PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_LEFT( const type1, type2 )\
	PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_LEFT( type1, const type2 )\
	PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_LEFT( type1, type2 )

/**	\brief	PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_RIGHT_ALL
	
*/
#define PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_RIGHT_ALL( type1, type2 ) \
	PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_RIGHT( const type1, const type2 )\
	PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_RIGHT( const type1, type2 )\
	PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_RIGHT( type1, const type2 )\
	PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_RIGHT( type1, type2 )

/*
 *******************************************************************************************
 这部分包括嵌套宏和模板的代码在GCC的环境下不能正常展开，已经被修改为模板public操作符函数
 和friend操作符函数的形式。
 肖保锋	2006-10-12

PMCOM_DECLARE_OPERATOR_COMPARE_LEFT_ALL( PMCOM_SMARTPTR_TYPE&, PMCOM_SMARTPTR_TYPE& )

PMCOM_DECLARE_OPERATOR_COMPARE_LEFT( PMCOM_SMARTPTR_TYPE&, const long )
PMCOM_DECLARE_OPERATOR_COMPARE_LEFT( const PMCOM_SMARTPTR_TYPE&, const long )
PMCOM_DECLARE_OPERATOR_COMPARE_RIGHT( const long , PMCOM_SMARTPTR_TYPE& )
PMCOM_DECLARE_OPERATOR_COMPARE_RIGHT( const long , const PMCOM_SMARTPTR_TYPE& )

PMCOM_DECLARE_OPERATOR_COMPARE_LEFT_ALL( PMCOM_SMARTPTR_TYPE&, T* )
PMCOM_DECLARE_OPERATOR_COMPARE_RIGHT_ALL( T* , PMCOM_SMARTPTR_TYPE& )

PMCOM_DECLARE_OPERATOR_COMPARE_LEFT_ALL( PMCOM_SMARTPTR_TYPE&, PmUnknownPtr& )
PMCOM_DECLARE_OPERATOR_COMPARE_RIGHT_ALL( PmUnknownPtr&, PMCOM_SMARTPTR_TYPE& )
*/

PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_RIGHT_ALL( PmUnknownPtr&, PmUnknownPtr& )

PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_LEFT( const PmUnknownPtr&, const long )
PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_LEFT( PmUnknownPtr&, const long )
PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_RIGHT( const long, const PmUnknownPtr& )
PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_RIGHT( const long, PmUnknownPtr& )

PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_LEFT_ALL( PmUnknownPtr&, IPmUnknown* )
PMCOM_DECLARE_OPERATOR_COMPARE_UNKNOWN_RIGHT_ALL( IPmUnknown*, PmUnknownPtr& )

/**	\brief	The _pm_smart_ptr_real function

	\param	pszFileName	a parameter of type const char*
	\param	nFileLine	a parameter of typelong 
	\param	pInterface	a parameter of type InterfaceType*

	\return	template< class InterfaceType > PmUnknownPtr

	
*/
template< class InterfaceType > PmUnknownPtr _pm_smart_ptr_real( const char* pszFileName, int nFileLine, InterfaceType* pInterface )
{
	if( pInterface == PM_NULL ) 
	{
		pm_throw("_pm_smart_ptr input para is NULL");
		return pmcom_null_ptr_real( pszFileName, nFileLine );
	}
	PmUnknownPtr ptrUnknown( pszFileName, nFileLine );
	if( !pInterface->queryInterface(IID_IPmUnknown, (void**)&ptrUnknown)) 
		PmThrowExceptionFmt( PM_HERE, PM_ERROR_SYSTEM, "can not get interface[0x%x]",IID_IPmUnknown);
	return ptrUnknown;
}
/**	\brief	_pm_smart_ptr
	
*/
#define _pm_smart_ptr( pInterFace ) _pm_smart_ptr_real( __FILE__, __LINE__, pInterFace )


#endif


