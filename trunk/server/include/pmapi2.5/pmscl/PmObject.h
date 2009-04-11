#if !defined(__PmObject__h__200696)
#define __PmObject__h__200696

/**	
 *@file PmObject.h 
 *@brief 
*/


/* define if the compiler supports Run-Time Type Identification */
#ifndef PMUNIT_HAVE_RTTI
# ifdef _CPPRTTI // Defined by the compiler option /GR
#  define PMUNIT_HAVE_RTTI 1
# else
//#  define PMUNIT_HAVE_RTTI 0
# endif
#endif

#if PMUNIT_HAVE_RTTI
#  include <typeinfo>
#endif

struct PM_OBJECT_TYPE;
/**
 *  Define the basic elements of a object
 */
class PmObject
{
private:
	PM_OBJECT_TYPE *m_pType;

protected:
	PM_VOID setClassType(PM_LPCSTR type);

public:
	PmObject();
	virtual ~PmObject();
	
	PM_BOOL isTypeOf(PM_LPCSTR type);
};


#endif
