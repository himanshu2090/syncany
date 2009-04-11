
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


//�������ļ��ж�ȡ�ַ�������pszFileNameΪ�յ�ʱ��Ҫ��Ŀ������ٴ��ڻ�������"SYNC_HOME"�����Ҹ�Ŀ¼�´���һ��confĿ¼����Ŀ¼�´���sync.conf�����ļ���
PmString SyncConfGetString(const char*pszSection,const char* pszKey,const char* pszDefault,const char* pszFileName = NULL);
int SyncConfGetInt(const char*pszSection,const char* pszKey,int nDefault,const char* pszFileName = NULL);
char SyncConfGetChar(const char*pszSection,const char* pszKey,char chDefault,const char* pszFileName = NULL);


