#ifndef __PM_PUBLIC_H__
#define __PM_PUBLIC_H__

PmPropertiesPtr PmLoadFileConfig( const char* pszFileName );
PM_LPCSTR PmGetProperty( PmPropertiesPtr ptrProperties, const char* pszName, const char* pszDefault );
PmException PmGetEmptyException();
PmStringListPtr PmCreateStringList();
PmMapStringToStringPtr PmCreateMapStringToString();
void pm_removeq( char* psz );

PmStringListPtr PmGetFolderFiles( const char* pszDirName );
PmPropertiesPtr PmLoadConfig( const char* pszConfigContent );
PmPropertiesPtr PmLoadConfigEx( const char* pszConfigContent, const char* pszSplit = "&|" );
PM_BOOL PmErrorExist(PmException& ex, PM_DWORD errNo);

#ifdef PM_PERFORMANCE_ANA

class PmPermSector
{
public:
	PmPermSector( const char* psz, const char* pszFile, int nLine )
	{
		pm_perm_sector_begin( psz, pszFile, nLine );
	}
	~PmPermSector()
	{
		pm_perm_sector_end();
	}	
};
class PmPermRecord
{
public:
	PmPermRecord( int index )
	{
		m_index = index;
		pm_perm_item_begin( m_index );
	}
	~PmPermRecord()
	{
		pm_perm_item_end( m_index );
	}
private:
	int m_index;
};
	#define PM_PERM_SECTOR( s ) PmPermSector pm_perm_sector_##s( #s, __FILE__, __LINE__ )
	#define PM_PERM_SECTOR_BEGIN( s ) { PmPermSector pm_perm_sector_##s( #s, __FILE__, __LINE__ )
	#define PM_PERM_SECTOR_END }

	#define PM_PERM( key ) static int s_##key##_index = 0;\
			if( s_##key##_index == 0 )\
				s_##key##_index = pm_perm_item_register( #key, __FILE__,__LINE__);\
			PmPermRecord pm_perm_record_##key(s_##key##_index);

	#define PM_PERM_BEGIN( key ) { PM_PERM(key)
	#define PM_PERM_NEXT( key ) } { PM_PERM(key)
	#define PM_PERM_END	}
#else
	#define PM_PERM_SECTOR( s )
	#define PM_PERM_SECTOR_BEGIN( s )
	#define PM_PERM_SECTOR_END
	#define PM_PERM_BEGIN( key )
	#define PM_PERM_NEXT( key )
	#define PM_PERM_END
	#define PM_PERM( key )
#endif

//���ܷ�������Ҫ�ڴ����м������
//�ȴ򿪿���PM_PERFORMANCE_ANA, ������makefile�м���-D PM_PERFORMANCE_ANA��
//  �����ڽ������ܷ�����ģ��Դ��ͷ����#defien PM_PERFORMANCE_ANA
//�����ڳ�����ڳ�ʼ��һ�£�ʹ��PM_PERM_INIT;
//�˳���ʱ���ӡ���ܷ�������, ʹ��PM_PERM_REPORT;(Ҳ�����ڳ�������Ҫ����ĵط����,����ʹ��pm_perm_get_report(ByteStr*)��ñ���
//������Ҫͳ�ƵĴ���λ���ϣ�����sector, ����Ҫ��λ�ĵط�����ɼ��㣬��������

//#define PM_PERFORMANCE_ANA
//#include ...
//
//int main()
//{
//	PM_PERM_INIT;
//  myfunc();
//  PM_PERM_REPORT;
//}
//void test1(){}
//void test2(){}
//void test3()
//{
//   ...
//   PM_PERM( test1_2_in_test3 )//�ɼ���test1_2_in_test3, �ں�����Ž���
//   test1();
//   test2();
//}
//void myfunc()
//{
//   PM_PERM_SECTOR( myfunc1 );//����sector myfunc1, �ں�����Ž���
//   PM_PERM_BEGIN( test1 );//�ɼ���test1
//   test1();
//   PM_PERM_NEXT( test2 );//�ɼ���test2
//   test2();
//   PM_PERM_NEXT( test3 );//�ɼ���test3
//   test3();
//   PM_PERM_END;
//}
#endif

