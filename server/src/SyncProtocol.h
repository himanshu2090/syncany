#ifndef ___SYNC_PROTOCOL__
#define ___SYNC_PROTOCOL__



struct CMD_ID_MAPPING
{
	const char* pszCmd;
	unsigned short id;
};

static unsigned short GetCmdID(const char* pszCmd)
{
	static CMD_ID_MAPPING m[] =
	{
		{"whoareyou",1},
		{"TMNew",2},
		{"TMDel",3},
		{"TMMov",4},
		{"SyncAll",5},
		{"cd",6},
		{"ls",7},
		{"rm",8},
		{"mv",9},
		{"get",10},
		{"put",11},
	};

	int nCount = sizeof(m) / sizeof(CMD_ID_MAPPING);
	for(int i = 0; i < nCount; ++i)
	{
		if(0 == strcmp(pszCmd,m[i].pszCmd))
		{
			return m[i].id;
		}
	}
	return 0;
};


typedef map<PmString,PmString> KEYVAL;

class UnFinishCmd
{
public:
	UnFinishCmd(const char* pszCmd):
		nCmdID(GetCmdID(pszCmd)),
		CmdStartTime(PmTime::getCurrentTime()),
		m_lRef(0)
	{
	};
	virtual ~UnFinishCmd()
	{
		pm_info("UnFinishCmd destruction");
	};
	virtual int addRef()
	{
		return pmcom_inc( &m_lRef );
	}
	virtual int release()
	{
		int ret = pmcom_dec( &m_lRef );
		if( ret == 0 )
			delete this;
		return ret;
	}
public:
	unsigned short nCmdID;
	PmTime CmdStartTime;
private:
	PM_LONG m_lRef;
};

typedef PmSmartPtr<UnFinishCmd> UnFinishCmdPtr;
typedef map<unsigned int , UnFinishCmdPtr> UnFinishRecord;

class SyncProtocol : public ACE_Event_Handler 
{
private :
    //socket wrapper for read and write
    ACE_SOCK_Stream m_sock;

    //输入2k，输出缓冲区16k
    ACE_Message_Block *m_input_buffer;
    ACE_Message_Block *m_output_buffer;
    ACE_Reactor_Mask m_mask;
    SocketIO* m_pSocketIO;

	PM_BOOL m_bQuit;	
	time_t* m_ptimeout;
	
	PM_BOOL m_bAuth;
	unsigned int m_uCmdID;
	UnFinishRecord m_UnFinishRecord;//记录未完成的命令

	PmString m_strDevicePlatform;
	PmString m_strDeviceVersion;
public:
	 SyncProtocol(ACE_Reactor *r = ACE_Reactor::instance ());
	 
	 virtual ~SyncProtocol();
	 
	 //Get the I/O handle
     virtual ACE_HANDLE get_handle() const ;
     // Get a reference to the contained <ACE_SOCK_Stream>
     ACE_SOCK_Stream &peer();
	 void SetTime(time_t* pTime);
	 virtual int open();
	 virtual int handle_timeout();
	 virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);
	 virtual int handle_output( ACE_HANDLE fd = ACE_INVALID_HANDLE );
	 virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
	 virtual int do_command(PmString& strCmd);
	 int parse_command(PmString& strCmd,PmStringListPtr& PtrResultList);


	 int do_command_ex(PmString& strCmd);
	 int do_ping(PmString& strCmdID,KEYVAL& KeyVal);
	 int do_hello(PmString& strCmdID,KEYVAL& KeyVal);
	 int parse_command_ex(PmString& strCmd,PmString& strCmdName,PmString& strCmdID,KEYVAL& KeyVal);


#endif

