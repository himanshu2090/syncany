#include "SyncAny.h"

ACE_Atomic_Op<ACE_Thread_Mutex,int> g_RemainLifeCount;

typedef struct tagTIMEOUT_MONITOR_PARAM
{
	int nConnectCount; 
	int nTimeOutValue;
	time_t* CmdTimeQueue;
	void**  handlers;
}TIMEOUT_MONITOR_PARAM;



//work process route
ACE_THR_FUNC_RETURN ThreadFunction(void *arg) 
{
    ACE_Reactor *reactor = (ACE_Reactor *) arg;
	//pm_debug2("进程<%d> 线程<%d>",ACE_OS::getpid(),ACE_OS::thr_self());
    reactor->run_reactor_event_loop();
    return 0;
}

//超时监控线程
static ACE_THR_FUNC_RETURN TimeOutMonitor(void *arg) 
	{
		TIMEOUT_MONITOR_PARAM* ptm = (TIMEOUT_MONITOR_PARAM*)arg;
		int nConnectCount = ptm->nConnectCount;
		int nTimeOutValue = ptm->nTimeOutValue;//超时时间间隔量
		int nDeadConnectCount;
		time_t tnow;
		time_t cmdtime;

		while(1)
		{
			nDeadConnectCount = 0;
			int nHadUsedConnectCount = 0;//已经建立过的连接数
			tnow = time(0);
			for (int i = 0 ; i < nConnectCount ; ++i)
			{
				cmdtime = ptm->CmdTimeQueue[i];
				if (cmdtime == 0)
				{
					//状态说明CmdTimeQueue[i] = 0表示未接受连接，或者服务端尚在处理中，总之不需要考虑客户端的超时
					continue;
				}
				else if ( cmdtime == 2 ) 
				{
					++nHadUsedConnectCount;//已经建立过的连接数
				}
				else if (cmdtime == 1 )
				{
					//状态说明CmdTimeQueue[i] = 1表示连接已经断开，不需要再考虑超时了,
					++nDeadConnectCount;//断开计数加1
					++nHadUsedConnectCount;//已经建立过的连接数
					continue;
				}
				else if (cmdtime + nTimeOutValue < tnow)
				{
					//已经超时
					SyncProtocol* handler = (SyncProtocol*)(ptm->handlers[i]);
					pm_debug("连接超时");
					handler->handle_timeout();//关闭连接
					++nDeadConnectCount;
					++nHadUsedConnectCount;//已经建立过的连接数
				}
				else 
				{
					//未超时
					++nHadUsedConnectCount;
				}
			}


			if (nDeadConnectCount == nConnectCount)
			{
				//所有的连接都已经关闭
				pm_debug("所有的连接都已经关闭");
				break;
			};
			pm_sleep(5000);//休息5秒
		}
		return 0;
	}

	
SyncFrame::SyncFrame()
{
}
SyncFrame::~SyncFrame()
{
}
	



	
	int SyncFrame::DispatchProcess(char* pszCmd,//命令行
						unsigned short nPort,//监听的端口号
						unsigned int nProcessCount,//启动的子进程个数
						unsigned int nThreadCount,//每个进程启动的线程个数
						unsigned int nConnectionCount,//每个进程接收的连接个数
						unsigned int nTimeout,//超时的时长
						unsigned int nBacklog,//backlong队列大小
						const char* pszSemaphoreName,//信号量的名称
						unsigned int nSemaphoreCount//信号量的初始值
						)
	{
		ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("(%P|%t) SyncFrame::DispatchProcess enter!!!\n")));  
        ACE_INET_Addr inet_addr(nPort);
		ACE_SOCK_Acceptor acceptor;
		//创建共享socket
		if (acceptor.open(inet_addr,1,PF_UNSPEC,nBacklog) == -1 ) 
		{
			//创建监听端口失败，直接退出返回，需要重新定义返回值，异常退出返回值。
			pm_debug("创建监听端口失败");
			return 1;
		}

		//生成命令行
		ACE_Process_Options options;
		options.command_line (ACE_TEXT ("%s %d %d %d %d %s %d"), 
							  pszCmd,
							  acceptor.get_handle(),
							  nThreadCount,
							  nConnectionCount,
							  nTimeout,
							  pszSemaphoreName,
							  nSemaphoreCount);
							  
		pid_t * pids = new pid_t[nProcessCount];
		if ( ACE_Process_Manager::instance()->spawn_n(nProcessCount, options, pids) == -1 ) 
		{
			//创建工作子进程失败，直接返回。
			pm_info("父进程创建子进程失败...");
			return 2;
		}
		
		        //等待工作子进程安全退出。
		pm_info("父进程进入等待子进程退出状态...");
		ACE_Time_Value timeout(10);
		while (1) 
		{
			pm_sleep(120000);
			//pid_t pid = ACE_Process_Manager::instance()->wait(0,ACE_Time_Value::max_time);
			pid_t pid = ACE_Process_Manager::instance()->wait(0,timeout);
			if ( pid == ACE_INVALID_PID ) 
			{
				pm_debug("检测是否有子进程退出时发生错误...");
			}
			else if ( pid == 0 ) 
			{
				pm_debug("检测是否子进程退出时发生超时...");
			}
			else 
			{
				pm_debug("检测到有子进程退出，创建新的子进程...");
				while ( (pid = ACE_Process_Manager::instance()->spawn(options)) == ACE_INVALID_PID ) 
				{
					pm_sleep(1);
				}
			}
		}
		pm_info("父进程退出");
		return 0;
	};
	
	
	/**
	子进程主线程。在主线程中启动工作子线程（使用TP Reactor）
	在主线程中接收客户端连接请求，客户端请求加入到消息循环中。
	主线程接收到规定的客户端连接请求后，不再接收客户端请求，由其他工作子进程接收处理。
         在此处使用进程间锁来控制工作子进程顺序接收客户端连接请求，该进程间锁在主控进程中创建。
         */
	int SyncFrame::WorkProcess(char* pszSocket,
					char* pszThreadCount,
					char* pszConnectionCount,
					char* pszTimeout,
					char* pszSemaphoreName,
					char* pszSemaphoreCount
					)
	{
		//解析应该启动的工作进程数，端口号，地址，socket描述字
		ACE_HANDLE fd = ACE_INVALID_HANDLE;
		fd = (ACE_HANDLE)ACE_OS::atoi(pszSocket);
		int nThreadCount = ACE_OS::atoi(pszThreadCount);//线程数
		int nConnectionCount = ACE_OS::atoi(pszConnectionCount);//每进程可以处理的连接数
		int nTimeout = ACE_OS::atoi(pszTimeout);
		int nSemaphoreCount = ACE_OS::atoi(pszSemaphoreCount);
		g_RemainLifeCount = 0;
		

		TIMEOUT_MONITOR_PARAM tm;
		tm.nConnectCount = nConnectionCount;
		tm.nTimeOutValue = nTimeout;
		tm.CmdTimeQueue = new time_t[nConnectionCount];
		memset(tm.CmdTimeQueue,0,nConnectionCount * sizeof(time_t));
		tm.handlers = new void*[nConnectionCount];
		memset(tm.handlers,0,nConnectionCount * sizeof(void*));
		
		
		//创建TP_Reactor
		ACE_TP_Reactor tpReactor;
		ACE_Reactor reactor(&tpReactor);
	
		//启动工作线程。
		errno = 0;
		long flags=THR_NEW_LWP|THR_JOINABLE|THR_INHERIT_SCHED;
		int grp_id = ACE_Thread_Manager::instance()->spawn_n(nThreadCount, ThreadFunction, &reactor,flags);
		if ( grp_id == -1 ) 
		{
			pm_info3("工作进程创建工作线程失败 errno<%d> info<%s> 已经创建的线程数<%d>",errno,ACE_OS::strerror(errno),ACE_Thread_Manager::instance()->count_threads());
		}
		
		
		pm_info1("已经创建的线程数<%d>",ACE_Thread_Manager::instance()->count_threads());
        //应用Cmpop进程间锁，用于控制工作子进程顺序接收客户端连接。
        ACE_SOCK_Acceptor acceptor;
        acceptor.set_handle(fd);
		
		//执行accept操作
		ACE_Reactor_Mask mask = ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK;
        ACE_Time_Value timeout(5,0);
		int iAcceptCount = 0;
		
		pm_info1("子进程主线程准备初始化进程锁 <%s> ......",pszSemaphoreName);
		ACE_SV_Semaphore_Complex  ProcessSemaphore(ACE_TEXT_ALWAYS_CHAR (pszSemaphoreName),ACE_SV_Semaphore_Complex::ACE_CREATE,nSemaphoreCount);
		pm_info1("子进程主线程准备获取进程锁<%s> ......",pszSemaphoreName);
		int iRet = 0;
		int myerrno = 0;
		
		do
		{
			errno = 0;
			iRet = ProcessSemaphore.acquire(0,SEM_UNDO);
			myerrno = errno;
			pm_info2("errno = %d ,iRet = %d",myerrno,iRet);
			if (iRet != 0 && myerrno == EINTR)
			{
				pm_debug("信号量获取被中断");
				return 0;
			}
		}while(iRet != 0);
		
		pm_info1("子进程主线程获取进程锁<%s>",pszSemaphoreName);
		//启动超时扫描线程
		int timeout_monitor_thread = ACE_Thread_Manager::instance()->spawn(TimeOutMonitor,(void*)(&tm));
		
		
		//接受socket连接
		while (iAcceptCount < nConnectionCount) 
        {
    		SyncProtocol *peer_handler = 0;
    		ACE_NEW_NORETURN ( peer_handler,SyncProtocol(&reactor ));
            if ( peer_handler == 0 ) 
            {
                break;
            }
						
            ACE_INET_Addr remote_addr;
            //在ACE_Sock_Accept带超时的实现中，内部是使用poll来实现的，所以外不用先select然后再调用accept

			errno = 0;
			//pm_debug("等待接收客户端连接请求......");
			int iret = acceptor.accept(peer_handler->peer(),&remote_addr,&timeout);
			if ( iret < 0 ) //有错误发生。包括被外部信号打断
			{
				//errno可能是：超时errno = ETIMEDOUT
				//释放资源
				delete peer_handler;
				continue;//未能接受到新的连接不改变计数
			}

			tm.handlers[iAcceptCount] = (void*)peer_handler;
			tm.CmdTimeQueue[iAcceptCount] = time(0);
			peer_handler->SetTime(&(tm.CmdTimeQueue[iAcceptCount]));//确保对时间地址的赋值在该地址被使用前（handle_output中使用了该地址对时间赋值);

			if ( peer_handler->open() == -1 ) 
			{
				pm_debug1("登记第<%d>个连接的读写事件失败",iAcceptCount);
				tm.CmdTimeQueue[iAcceptCount] = 0;//计时恢复
				tm.handlers[iAcceptCount] = (void*)(0);
				peer_handler->handle_close ( ACE_INVALID_HANDLE,mask);
				delete peer_handler;
				continue;
			}

	        ++iAcceptCount;//连接计数递增
			++g_RemainLifeCount;
			pm_debug3("接受了<%d>个连接请求,还可以接受<%d>个连接请求,port<%d>",iAcceptCount,nConnectionCount - iAcceptCount,remote_addr.get_port_number());
        }
		
		ProcessSemaphore.release(0,SEM_UNDO);
        //释放锁资源
		pm_info("本进程将不再接收新的连接,释放进程锁!!!");

		//等待所有子线程安全退出。Block
		while (1) 
		{
			if (g_RemainLifeCount <= 0) 
			{
				pm_debug("所有连接均处理完成，主线程退出检测等待");
				reactor.end_reactor_event_loop();
				break;
			}
			sleep(1);
		}
		ACE_Thread_Manager::instance()->wait();
		pm_debug("所有的子线程都已经安全的退出，本进程退出");
		//delete []tm.CmdTimeQueue;
		//delete []tm.handlers;
		return 0;
	}


