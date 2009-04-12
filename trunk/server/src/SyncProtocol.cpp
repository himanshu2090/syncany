#include "SyncAny.h"

extern ACE_Atomic_Op<ACE_Thread_Mutex,int> g_RemainLifeCount;
PmString g_strNewLine("\r\n");



	SyncProtocol::SyncProtocol(ACE_Reactor *r)
:ACE_Event_Handler (r),
	m_mask(0),
	m_input_buffer(NULL),
	m_output_buffer(NULL),
	m_bQuit(PM_FALSE),
	m_bAuth(PM_FALSE),
	m_uCmdID(0),
	m_ptimeout(NULL),
	m_pSocketIO(NULL)
{
}

SyncProtocol::~SyncProtocol()
{
}

//Get the I/O handle
ACE_HANDLE SyncProtocol::get_handle() const 
{
	return m_sock.get_handle();
};

// Get a reference to the contained <ACE_SOCK_Stream>
ACE_SOCK_Stream& SyncProtocol::peer() 
{ 
	return m_sock; 
}

void SyncProtocol::SetTime(time_t* pTime)
{
	m_ptimeout = pTime;
}

int SyncProtocol::open() 
{ 
	ACE_NEW_RETURN( m_input_buffer, ACE_Message_Block (2*1024, ACE_Message_Block::MB_HANGUP), -1 );
	ACE_NEW_RETURN( m_output_buffer, ACE_Message_Block (8*1024, ACE_Message_Block::MB_HANGUP), -1 );
	ACE_NEW_RETURN( m_pSocketIO, SocketIO(&m_sock,m_input_buffer,m_output_buffer), -1 );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//设置SOCKET特性
	int opt = 1;
	m_sock.set_option(SOL_SOCKET,SO_KEEPALIVE,(void*)(&opt),sizeof(int));//使用SOCKET保活
	int idle = 20;
	m_sock.set_option(SOL_TCP,TCP_KEEPIDLE,(void*)(&idle),sizeof(int));//TCP空闲idle时间才开始发送第一个保活包
	int interval = 10 * 60;
	m_sock.set_option(SOL_TCP,TCP_KEEPINTVL,(void*)(&interval),sizeof(int));//2次保活包之间的时间间隔
	int count = 3;
	m_sock.set_option(SOL_TCP,TCP_KEEPCNT,(void*)(&count),sizeof(int));//连续发送3个保活包且都未收到响应后才关闭socket

	struct linger Linger; 
	Linger.l_onoff = 1;
	Linger.l_linger = 30;
	m_sock.set_option(SOL_SOCKET,SO_LINGER,(void*)(&Linger),sizeof(Linger));//设置延时关闭时间为30秒
	m_sock.set_option(SOL_SOCKET,SO_REUSEADDR,(void*)(&opt),sizeof(int));//重用本地断口

	//注册socket的读事件
	ACE_SET_BITS(m_mask, ACE_Event_Handler::READ_MASK);
	errno = 0;
	int iret = reactor()->register_handler( this, m_mask );
	if (iret == -1)
	{
		pm_debug4("register_handler fail ret = -1,errno = <%d>,handle=<%d>,mask=<%d>,this=<%X>",errno,(int)get_handle(),m_mask,this);
		return iret;
	}

	pm_debug4("register_handler success ret = 0,errno = <%d>,handle=<%d>,mask=<%d>,this=<%X>",errno,(int)get_handle(),m_mask,this);
	return iret;
}


int SyncProtocol::handle_timeout()
{
	pm_debug("handle_timeout enter!!!");
	m_mask = 0;
	reactor()->remove_handler( this, ACE_Event_Handler::READ_MASK );
	reactor()->remove_handler( this, ACE_Event_Handler::WRITE_MASK );
	m_sock.close();//必须先做remove_handler然后才能关闭socket，否则register_handler可能会失败


	if (NULL != m_input_buffer)
	{
		delete m_input_buffer;
		m_input_buffer = NULL;
	}
	if (NULL != m_output_buffer)
	{
		delete m_output_buffer;
		m_output_buffer = NULL;
	}
	if (NULL != m_pSocketIO)
	{
		delete m_pSocketIO;
		m_pSocketIO = NULL;
	}

	*m_ptimeout = 1;//表示服务断开
	if (--g_RemainLifeCount == 0)
	{
		//本连接是本进程的最后一个连接
		pm_debug("超时断开最后一个连接");
		//reactor()->end_reactor_event_loop();
	}
	return 0;
}

//Called when a <handle_*()> method returns -1 or when the <remove_handler> method is called on an ACE_Reactor. 
//The <close_mask> indicates which event has triggered the <handle_close> method callback on a particular handle. 
//每次一个handle_*() 方法返回-1,或者反应器remove_handler被调用 则会调用此handle_close()hook 方法，并将正在被解除登记的事件的
//ACE_Reactor_Mask值传递给它，该hook方法清除mask_相应的位。
int SyncProtocol::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask) 
{
	return 0;
}


//当写事件被注册而且端口可写时，回调本函数
int SyncProtocol::handle_output(ACE_HANDLE fd) 
{
	int iret = m_pSocketIO->write_all_out();
	int nRemainLen = m_pSocketIO->length();

	if (nRemainLen > 0)
	{
		//数据还未写完，等待再次被回调
	}
	else
	{
		//数据已经写完
		reactor()->remove_handler( this, ACE_Event_Handler::WRITE_MASK );
		*m_ptimeout = time(0);//服务端处理完成，开始计算客户端的超时
	}
	return 0;
}

//有可读数据时，回调此函数
int SyncProtocol::handle_input(ACE_HANDLE fd)
{
	errno = 0;
	int iret = m_pSocketIO->recv();
	if (iret == 0)
	{   
		//客户端关闭socket连接
		m_mask = 0;
		reactor()->remove_handler( this, ACE_Event_Handler::READ_MASK );
		reactor()->remove_handler( this, ACE_Event_Handler::WRITE_MASK );
		m_sock.close();
		if (NULL != m_input_buffer)
		{   
			m_input_buffer->release();
			m_input_buffer = NULL;
		}   
		if (NULL != m_output_buffer)
		{   
			m_output_buffer->release();
			m_output_buffer = NULL;
		}   
		if (NULL != m_pSocketIO)
		{   
			delete m_pSocketIO;
			m_pSocketIO = NULL;
		}   

		*m_ptimeout = 1;
		if (--g_RemainLifeCount <= 0)
		{
			pm_debug("本进程已经服务完所有的连接，即将安全退出");
		}
	}
	else if(iret < 0)
	{
		//读取数据失败（可能是被外部信号打断）
		pm_error2("read from socket fail,errno=<%d>,m_pSocketIO<%X>",errno,&m_pSocketIO);
	}
	else
	{
		PM_BOOL bReg = PM_FALSE;
		PmString strCmd;

		while(m_pSocketIO->get_line(strCmd) > 0)
		{
			//读取到了一个完整的命令行，开始进行命令行处理
			//协议处理
			do_command_ex(strCmd);
		}

		//存在可写的数据，将写事件注册到reactor
		if (m_pSocketIO->length() > 0)
		{
			pm_info("有数据需要发送");
			reactor()->register_handler( this, ACE_Event_Handler::WRITE_MASK );
		}
	}
	return 0;
};


int SyncProtocol::do_ping(PmString& strCmdID,KEYVAL& KeyVal)
{
	PmString strOut = "state ";
	strOut += strCmdID;
	strOut += " 200 ";
	strOut += PmTime::getCurrentTime().getTimeStamp();
	strOut += g_strNewLine;
	m_pSocketIO->write_to_cache(strOut);
	return 0;
}


int SyncProtocol::do_hello(PmString& strCmdID,KEYVAL& KeyVal)
{
	PmString strOut = "state ";
	strOut += strCmdID;
	strOut += " 200 ";
	
	
	KEYVAL::iterator it;
	for (it = KeyVal.begin() ; it != KeyVal.end() ; it++)
	{
		PmString strKey = it->first;
		PmString strVal = it->second;
		pm_info2("'%s'='%s'",strKey.c_str(),strVal.c_str());
	}
	strOut += g_strNewLine;
	m_pSocketIO->write_to_cache(strOut);
	return 0;
}





int SyncProtocol::do_command_ex(PmString& strCmd)
{
	pm_info1("receive cmd(%s)",strCmd.c_str());
	PmString strCmdName;
	PmString strCmdID;
	KEYVAL KeyVal;

	PmString strOut;
	if (parse_command_ex(strCmd,strCmdName,strCmdID,KeyVal) < 2)
	{
		strOut = "illegial command";
	}
	else if (strCmdName == "ping")
	{
		return do_ping(strCmdID,KeyVal);
	}
	else if(strCmdName == "hello")
	{
		return do_hello(strCmdID,KeyVal);
	}
	else
	{
		strOut = "state " + strCmdID;
		strOut += " 401";
	}

	strOut += g_strNewLine;
	m_pSocketIO->write_to_cache(strOut);
	return 0;
}
		










int SyncProtocol::do_command(PmString& strCmd)
{
	PmString strOut;
	pm_info1("接收到数据:%s",strCmd.c_str());
	//strCmd.toUpper();//将接受到的命令转换为大写
	PmStringListPtr PtrResultList;
	unsigned int nCount = parse_command(strCmd,PtrResultList);

	if(nCount < 2)
	{
		pm_error1("接收到非法的命令:%s",strCmd.c_str());
		strOut.format("发送的命令(%s)非法",strCmd.c_str());
	}
	else
	{
		strOut = "state ";
		PmString str1(PtrResultList->getAt(0));
		PmString str2(PtrResultList->getAt(1));

		strOut += str2;
		if (0 == str1.compare("ping"))
		{
			if (nCount == 2)
			{
				strOut += " 200 ";
			}
			else
			{
				strOut += " 402 ";
				pm_error2("命令的参数个数不正确:(%s)(%d)",strCmd.c_str(),nCount);
			}
			strOut += PmTime::getCurrentTime().getTimeStamp();
		}
		else if (0 == str1.compare("hello"))
		{
			int flag = 0;
			if (nCount != 3)
			{
				flag = 1;
			}

			if (0 == flag)
			{
				PmString strRemain = PtrResultList->getAt(2);
				PmStringListPtr PtrHelloParams = strRemain.regMatch("^(\\S*)\\s+(\\d*.?\\d*)\\s*$");
				m_strDevicePlatform = PtrHelloParams->getAt(0);
				m_strDeviceVersion = PtrHelloParams->getAt(1);
				if (m_strDevicePlatform == "" || m_strDeviceVersion == "")
				{
					flag = 1;
					pm_error2("m_strDevicePlatform=(%s),m_strDeviceVersion=(%s)",m_strDevicePlatform.c_str(),m_strDeviceVersion.c_str());
				}
			}
			if (0 == flag)
			{
				strOut += " 200 ";
				if(!m_bAuth)
				{
					//用户还未曾鉴权
					strOut += g_strNewLine;
					++m_uCmdID;
					strOut.format("%swhoareyou %d",strOut.c_str(),m_uCmdID);
					UnFinishCmdPtr ptr = new UnFinishCmd("hello");
					m_UnFinishRecord[m_uCmdID] = ptr; 
					pm_info3("未完成队列中增加了一条记录,命令id(%d),命令生成时间(%s),未完成个数(%d)",ptr->nCmdID,ptr->CmdStartTime.getTimeStamp().c_str(),m_UnFinishRecord.size());
				}
			}
			else
			{
				strOut += " 402 ";
				pm_error1("命令的参数个数不正确:(%s)",strCmd.c_str());
			}
		}
		else if(0 == str1.compare("session"))
		{
			strOut += str2;
			strOut += " 200 ";
		}
		else if(0 == str1.compare("state"))
		{
			//收到了客户端的相应
			;
		}
		else
		{
			strOut += str2;
			strOut += " 401";
		}

	}
	strOut += g_strNewLine;
	m_pSocketIO->write_to_cache(strOut);
}

int SyncProtocol::parse_command(PmString& strCmd,PmStringListPtr& PtrResultList)
{
	PtrResultList = strCmd.regMatch("^(\\w+)\\s+(\\d+)\\s+(.*)\\s*$");
	if (PtrResultList->getAt(0) == "")
	{
		return 0;
	}
	if (PtrResultList->getAt(1) == "")
	{
		return 1;
	}
	if (PtrResultList->getAt(2) == "")
	{
		return 2;
	}
	return 3;
}



int SyncProtocol::parse_command_ex(PmString& strCmd,PmString& strCmdName,PmString& strCmdID,KEYVAL& KeyVal)
{

	PmStringListPtr PtrResultList  = strCmd.regMatch("^(\\w+)\\s+(\\d+)\\s+(.*)\\s*$");
	pm_info1("parse count(%d)",PtrResultList->count());
	if (PtrResultList->count() < 2)
	{
		return -1;
	}
	strCmdName = PtrResultList->getAt(0);
	strCmdID = PtrResultList->getAt(1);

	if (strCmdName == "" || strCmdID == "")
	{
        pm_info2("CmdName=%s  CmdID=%s",strCmdName.c_str(),strCmdID.c_str());
		return -1;
	}

	PmString strRemain = PtrResultList->getAt(2);
	if (strRemain == "")
	{
		return 2;
	}

	int nValidCount = 2;

	PmStringListPtr PtrSplitList = strRemain.split(" ");
	int nCount = PtrSplitList->count();
	int nPos = strRemain.find("\"");
	PmString strTemp;
	PmStringListPtr PtrSplitListTemp;
	if (nPos < 0)
	{
		//命令行内不包含"
		for (int i = 0 ; i < nCount ; ++i)
		{
			strTemp = PtrSplitList->getAt(i);
			PtrSplitListTemp = strTemp.split("=");
			if (2 != PtrSplitListTemp->count())
			{
				//没有等号或者存在多个等号
				pm_info1("count = %d",PtrSplitListTemp->count());
				return nValidCount;
			}
			KeyVal[PtrSplitListTemp->getAt(0)] = PtrSplitListTemp->getAt(1);
			++nValidCount;
		}
	}
	else
	{
		PmString strMerge;
		//命令行内包含"
		for (int i = 0 ; i < nCount ; ++i)
		{
			strTemp = PtrSplitList->getAt(i);
			if (strTemp.find("\"") < 0)
			{
				//本字符串内不存在"
				PtrSplitListTemp = strTemp.split("=");
				if (2 != PtrSplitListTemp->count())
				{
					//没有等号或者存在多个等号
					return nValidCount;
				}
				KeyVal[PtrSplitListTemp->getAt(0)] = PtrSplitListTemp->getAt(1);
				++nValidCount;
			}
			else
			{
				//本字符串内存在"
				if (strMerge != "")
				{
					strMerge += strTemp;
					int nLength = strMerge.length();
					int nQuotation = 0;
					for (int pos = 0 ; pos < nLength ; ++pos)
					{
						if ('"' == strMerge.getAt(pos))
						{
							++nQuotation;
						}
					}
					if (3 == nQuotation)
					{
						continue;
					}

					PtrSplitListTemp = strMerge.split("=");
					PmString strKey = PtrSplitListTemp->getAt(0);
					PmString strVal = PtrSplitListTemp->getAt(1);
					if (strKey.getAt(0) == '"')
					{
						strKey = strKey.substr(1,strKey.length()-2);
					}

					if (strKey.getAt(0) == '"')
					{
						strVal = strVal.substr(1,strVal.length()-2);
					}
					KeyVal[strKey] = strVal;
					++nValidCount;	
					strMerge = "";
					continue;
				}
				else
				{
					strMerge = strTemp;
				}
				
			}

		}
	}
	
	return nValidCount;
}



