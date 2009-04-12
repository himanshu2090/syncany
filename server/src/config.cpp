#include "SyncAny.h"

pthread_mutex_t  Config::conf_mutex = PTHREAD_MUTEX_INITIALIZER;
Config * Config::instance = NULL;

Config::Config()
{
}

Config::~Config()
{
}

Config * Config::Instance()
{
	if (instance == NULL)
	{
		pthread_mutex_lock(&conf_mutex);
		if (instance == NULL)
		{
			instance = new Config();
		}
		pthread_mutex_unlock(&conf_mutex);			
	}
	return instance;
}

int Config::init()
{
	Instance();
	return 0;
}

int Config::readConf(PmString fileName,PmString secName,CONFCONTENT& outConfContent)
{
	ACE_Configuration_Heap Conf;
	ACE_Registry_ImpExp  * Conf_impExp;
	ACE_Configuration::VALUETYPE type;
	int index = 0;
	ACE_TString key; 
	ACE_TString value;

	Conf.open();
	Conf_impExp = new ACE_Registry_ImpExp(Conf);
	if(Conf_impExp->import_config(fileName.c_str()) !=0 )
	{
		pm_error1("open %s error! ",fileName.c_str());
		delete Conf_impExp;
		return -1;
	}

	ACE_Configuration_Section_Key  confSec ;

	if (Conf.open_section(Conf.root_section(), secName.c_str(), 0, confSec))
	{
		pm_error2("open section:%s in %s", secName.c_str(),fileName.c_str());
		delete Conf_impExp;
		return -1;
	}

	while(Conf.enumerate_values(confSec , index++ , key , type) == 0 )
	{
		if(Conf.get_string_value(confSec , key.c_str() , value ) ==0)
		{
			outConfContent.insert(CONFCONTENTPR(key.c_str() ,value.c_str()));
		}
	}

	delete Conf_impExp;
	return 0;
}

int Config::writeConf(PmString fileName,PmString secName,PmString strKey,PmString strValue)
{

	ACE_Configuration_Heap Conf;
	ACE_Registry_ImpExp  * Conf_impExp;
	ACE_Configuration::VALUETYPE type;	
	ACE_Configuration_Section_Key  confSec ;

	Conf.open();
	Conf_impExp = new ACE_Registry_ImpExp(Conf);
	
	if(Conf_impExp->import_config(fileName.c_str()) !=0 )
	{
		pm_info1("file %s not exist ! ",fileName.c_str());
	}

	if (-1 == Conf.open_section(Conf.root_section(), ACE_TEXT(secName.c_str()) , true , confSec))
	{
		pm_debug1("debug: writeConf() error, ace errorcode:[%d]",ACE_OS::last_error());
		delete Conf_impExp;
		return -1;
	}

	Conf.set_string_value(confSec, (char*)strKey.c_str(), ACE_TString(strValue.c_str()));

	pthread_mutex_lock(&conf_mutex);
	Conf.export_config(fileName.c_str());
	pthread_mutex_unlock(&conf_mutex);	

	delete Conf_impExp;
	return 0;
}

int Config::removeConf(PmString fileName,PmString secName,PmString strKey)
{
	ACE_Configuration_Heap Conf;
	ACE_Registry_ImpExp  * Conf_impExp;
	ACE_Configuration::VALUETYPE type;	
	ACE_Configuration_Section_Key  confSec ;	
	
	Conf.open();
	Conf_impExp = new ACE_Registry_ImpExp(Conf);

	if(Conf_impExp->import_config(fileName.c_str()) !=0 )
	{
		pm_error1("open %s error! ",fileName.c_str());
		delete Conf_impExp;
		return -1;
	}
	
	if (-1 == Conf.open_section(Conf.root_section(), ACE_TEXT(secName.c_str()) , true , confSec))
	{
		pm_debug1("debug: removeConf() error, ace errorcode:[%d]",ACE_OS::last_error());
		delete Conf_impExp;
		return -1;
	}
	
	Conf.remove_value(confSec, (char*)strKey.c_str() );

	pthread_mutex_lock(&conf_mutex);
	Conf.export_config(fileName.c_str());
	pthread_mutex_unlock(&conf_mutex);			

	delete Conf_impExp;
	return 0;
}


//从配置文件中读取字符串，当pszFileName为空的时候（要求目标机器少存在环境变量"SYNC_HOME"，而且该目录下存在一个conf目录，该目录下存在sync.conf配置文件）
PmString SyncConfGetString(const char*pszSection,const char* pszKey,const char* pszDefault,const char* pszFileName )
{
	CONFCONTENT c;
	PmString strFileName;
	PmPath path;
	if( pszFileName == PM_NULL )
	{
		path = pm_getenv("SYNC_HOME" );
		path += "conf";
		path += "sync.conf";
	}
	else
	{
		path = pszFileName;
	}
	
	strFileName = path.toString();
	int ret = Config::readConf( strFileName, pszSection, c );
	if( ret == -1 )
		return pszDefault;
	if( c.find( pszKey ) == c.end ())
		return pszDefault;
	return c[pszKey];
}

int SyncConfGetInt(const char*pszSection,const char* pszKey,int nDefault,const char* pszFileName)
{
	char buf[32];
	sprintf( buf, "%d", nDefault );
	return pm_atoi( SyncConfGetString( pszSection, pszKey, buf, pszFileName ) ) ;
}

char SyncConfGetChar(const char*pszSection,const char* pszKey,char chDefault,const char* pszFileName)
{
	char buf[2] = { chDefault, '\0' };
	return SyncConfGetString( pszSection, pszKey, buf, pszFileName ).c_str()[0];
}


