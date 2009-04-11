
typedef map<PmString, PmString> CONFCONTENT;
typedef pair<PmString,PmString>CONFCONTENTPR;

class Config
{
	private:
		static pthread_mutex_t conf_mutex; 
		static Config * instance;
	public:
		Config();
		virtual ~Config();

		static int init();
		static int readConf(PmString fileName,PmString secName, CONFCONTENT& outConfContent);
		static int writeConf(PmString fileName,PmString secName,PmString key,PmString value);
		static int removeConf(PmString fileName,PmString secName,PmString key);	
		static Config * Instance();
};


//从配置文件中读取字符串，当pszFileName为空的时候（要求目标机器少存在环境变量"SYNC_HOME"，而且该目录下存在一个conf目录，该目录下存在sync.conf配置文件）
PmString SyncConfGetString(const char*pszSection,const char* pszKey,const char* pszDefault,const char* pszFileName = NULL);
int SyncConfGetInt(const char*pszSection,const char* pszKey,int nDefault,const char* pszFileName = NULL);
char SyncConfGetChar(const char*pszSection,const char* pszKey,char chDefault,const char* pszFileName = NULL);


