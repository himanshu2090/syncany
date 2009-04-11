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
	//pm_debug2("����<%d> �߳�<%d>",ACE_OS::getpid(),ACE_OS::thr_self());
    reactor->run_reactor_event_loop();
    return 0;
}

//��ʱ����߳�
static ACE_THR_FUNC_RETURN TimeOutMonitor(void *arg) 
	{
		TIMEOUT_MONITOR_PARAM* ptm = (TIMEOUT_MONITOR_PARAM*)arg;
		int nConnectCount = ptm->nConnectCount;
		int nTimeOutValue = ptm->nTimeOutValue;//��ʱʱ������
		int nDeadConnectCount;
		time_t tnow;
		time_t cmdtime;

		while(1)
		{
			nDeadConnectCount = 0;
			int nHadUsedConnectCount = 0;//�Ѿ���������������
			tnow = time(0);
			for (int i = 0 ; i < nConnectCount ; ++i)
			{
				cmdtime = ptm->CmdTimeQueue[i];
				if (cmdtime == 0)
				{
					//״̬˵��CmdTimeQueue[i] = 0��ʾδ�������ӣ����߷�������ڴ����У���֮����Ҫ���ǿͻ��˵ĳ�ʱ
					continue;
				}
				else if ( cmdtime == 2 ) 
				{
					++nHadUsedConnectCount;//�Ѿ���������������
				}
				else if (cmdtime == 1 )
				{
					//״̬˵��CmdTimeQueue[i] = 1��ʾ�����Ѿ��Ͽ�������Ҫ�ٿ��ǳ�ʱ��,
					++nDeadConnectCount;//�Ͽ�������1
					++nHadUsedConnectCount;//�Ѿ���������������
					continue;
				}
				else if (cmdtime + nTimeOutValue < tnow)
				{
					//�Ѿ���ʱ
					SyncProtocol* handler = (SyncProtocol*)(ptm->handlers[i]);
					pm_debug("���ӳ�ʱ");
					handler->handle_timeout();//�ر�����
					++nDeadConnectCount;
					++nHadUsedConnectCount;//�Ѿ���������������
				}
				else 
				{
					//δ��ʱ
					++nHadUsedConnectCount;
				}
			}


			if (nDeadConnectCount == nConnectCount)
			{
				//���е����Ӷ��Ѿ��ر�
				pm_debug("���е����Ӷ��Ѿ��ر�");
				break;
			};
			pm_sleep(5000);//��Ϣ5��
		}
		return 0;
	}

	
SyncFrame::SyncFrame()
{
}
SyncFrame::~SyncFrame()
{
}
	



	
	int SyncFrame::DispatchProcess(char* pszCmd,//������
						unsigned short nPort,//�����Ķ˿ں�
						unsigned int nProcessCount,//�������ӽ��̸���
						unsigned int nThreadCount,//ÿ�������������̸߳���
						unsigned int nConnectionCount,//ÿ�����̽��յ����Ӹ���
						unsigned int nTimeout,//��ʱ��ʱ��
						unsigned int nBacklog,//backlong���д�С
						const char* pszSemaphoreName,//�ź���������
						unsigned int nSemaphoreCount//�ź����ĳ�ʼֵ
						)
	{
		ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("(%P|%t) SyncFrame::DispatchProcess enter!!!\n")));  
        ACE_INET_Addr inet_addr(nPort);
		ACE_SOCK_Acceptor acceptor;
		//��������socket
		if (acceptor.open(inet_addr,1,PF_UNSPEC,nBacklog) == -1 ) 
		{
			//���������˿�ʧ�ܣ�ֱ���˳����أ���Ҫ���¶��巵��ֵ���쳣�˳�����ֵ��
			pm_debug("���������˿�ʧ��");
			return 1;
		}

		//����������
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
			//���������ӽ���ʧ�ܣ�ֱ�ӷ��ء�
			pm_info("�����̴����ӽ���ʧ��...");
			return 2;
		}
		
		        //�ȴ������ӽ��̰�ȫ�˳���
		pm_info("�����̽���ȴ��ӽ����˳�״̬...");
		ACE_Time_Value timeout(10);
		while (1) 
		{
			pm_sleep(120000);
			//pid_t pid = ACE_Process_Manager::instance()->wait(0,ACE_Time_Value::max_time);
			pid_t pid = ACE_Process_Manager::instance()->wait(0,timeout);
			if ( pid == ACE_INVALID_PID ) 
			{
				pm_debug("����Ƿ����ӽ����˳�ʱ��������...");
			}
			else if ( pid == 0 ) 
			{
				pm_debug("����Ƿ��ӽ����˳�ʱ������ʱ...");
			}
			else 
			{
				pm_debug("��⵽���ӽ����˳��������µ��ӽ���...");
				while ( (pid = ACE_Process_Manager::instance()->spawn(options)) == ACE_INVALID_PID ) 
				{
					pm_sleep(1);
				}
			}
		}
		pm_info("�������˳�");
		return 0;
	};
	
	
	/**
	�ӽ������̡߳������߳��������������̣߳�ʹ��TP Reactor��
	�����߳��н��տͻ����������󣬿ͻ���������뵽��Ϣѭ���С�
	���߳̽��յ��涨�Ŀͻ�����������󣬲��ٽ��տͻ������������������ӽ��̽��մ���
         �ڴ˴�ʹ�ý��̼��������ƹ����ӽ���˳����տͻ����������󣬸ý��̼��������ؽ����д�����
         */
	int SyncFrame::WorkProcess(char* pszSocket,
					char* pszThreadCount,
					char* pszConnectionCount,
					char* pszTimeout,
					char* pszSemaphoreName,
					char* pszSemaphoreCount
					)
	{
		//����Ӧ�������Ĺ������������˿ںţ���ַ��socket������
		ACE_HANDLE fd = ACE_INVALID_HANDLE;
		fd = (ACE_HANDLE)ACE_OS::atoi(pszSocket);
		int nThreadCount = ACE_OS::atoi(pszThreadCount);//�߳���
		int nConnectionCount = ACE_OS::atoi(pszConnectionCount);//ÿ���̿��Դ����������
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
		
		
		//����TP_Reactor
		ACE_TP_Reactor tpReactor;
		ACE_Reactor reactor(&tpReactor);
	
		//���������̡߳�
		errno = 0;
		long flags=THR_NEW_LWP|THR_JOINABLE|THR_INHERIT_SCHED;
		int grp_id = ACE_Thread_Manager::instance()->spawn_n(nThreadCount, ThreadFunction, &reactor,flags);
		if ( grp_id == -1 ) 
		{
			pm_info3("�������̴��������߳�ʧ�� errno<%d> info<%s> �Ѿ��������߳���<%d>",errno,ACE_OS::strerror(errno),ACE_Thread_Manager::instance()->count_threads());
		}
		
		
		pm_info1("�Ѿ��������߳���<%d>",ACE_Thread_Manager::instance()->count_threads());
        //Ӧ��Cmpop���̼��������ڿ��ƹ����ӽ���˳����տͻ������ӡ�
        ACE_SOCK_Acceptor acceptor;
        acceptor.set_handle(fd);
		
		//ִ��accept����
		ACE_Reactor_Mask mask = ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK;
        ACE_Time_Value timeout(5,0);
		int iAcceptCount = 0;
		
		pm_info1("�ӽ������߳�׼����ʼ�������� <%s> ......",pszSemaphoreName);
		ACE_SV_Semaphore_Complex  ProcessSemaphore(ACE_TEXT_ALWAYS_CHAR (pszSemaphoreName),ACE_SV_Semaphore_Complex::ACE_CREATE,nSemaphoreCount);
		pm_info1("�ӽ������߳�׼����ȡ������<%s> ......",pszSemaphoreName);
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
				pm_debug("�ź�����ȡ���ж�");
				return 0;
			}
		}while(iRet != 0);
		
		pm_info1("�ӽ������̻߳�ȡ������<%s>",pszSemaphoreName);
		//������ʱɨ���߳�
		int timeout_monitor_thread = ACE_Thread_Manager::instance()->spawn(TimeOutMonitor,(void*)(&tm));
		
		
		//����socket����
		while (iAcceptCount < nConnectionCount) 
        {
    		SyncProtocol *peer_handler = 0;
    		ACE_NEW_NORETURN ( peer_handler,SyncProtocol(&reactor ));
            if ( peer_handler == 0 ) 
            {
                break;
            }
						
            ACE_INET_Addr remote_addr;
            //��ACE_Sock_Accept����ʱ��ʵ���У��ڲ���ʹ��poll��ʵ�ֵģ������ⲻ����selectȻ���ٵ���accept

			errno = 0;
			//pm_debug("�ȴ����տͻ�����������......");
			int iret = acceptor.accept(peer_handler->peer(),&remote_addr,&timeout);
			if ( iret < 0 ) //�д��������������ⲿ�źŴ��
			{
				//errno�����ǣ���ʱerrno = ETIMEDOUT
				//�ͷ���Դ
				delete peer_handler;
				continue;//δ�ܽ��ܵ��µ����Ӳ��ı����
			}

			tm.handlers[iAcceptCount] = (void*)peer_handler;
			tm.CmdTimeQueue[iAcceptCount] = time(0);
			peer_handler->SetTime(&(tm.CmdTimeQueue[iAcceptCount]));//ȷ����ʱ���ַ�ĸ�ֵ�ڸõ�ַ��ʹ��ǰ��handle_output��ʹ���˸õ�ַ��ʱ�丳ֵ);

			if ( peer_handler->open() == -1 ) 
			{
				pm_debug1("�Ǽǵ�<%d>�����ӵĶ�д�¼�ʧ��",iAcceptCount);
				tm.CmdTimeQueue[iAcceptCount] = 0;//��ʱ�ָ�
				tm.handlers[iAcceptCount] = (void*)(0);
				peer_handler->handle_close ( ACE_INVALID_HANDLE,mask);
				delete peer_handler;
				continue;
			}

	        ++iAcceptCount;//���Ӽ�������
			++g_RemainLifeCount;
			pm_debug3("������<%d>����������,�����Խ���<%d>����������,port<%d>",iAcceptCount,nConnectionCount - iAcceptCount,remote_addr.get_port_number());
        }
		
		ProcessSemaphore.release(0,SEM_UNDO);
        //�ͷ�����Դ
		pm_info("�����̽����ٽ����µ�����,�ͷŽ�����!!!");

		//�ȴ��������̰߳�ȫ�˳���Block
		while (1) 
		{
			if (g_RemainLifeCount <= 0) 
			{
				pm_debug("�������Ӿ�������ɣ����߳��˳����ȴ�");
				reactor.end_reactor_event_loop();
				break;
			}
			sleep(1);
		}
		ACE_Thread_Manager::instance()->wait();
		pm_debug("���е����̶߳��Ѿ���ȫ���˳����������˳�");
		//delete []tm.CmdTimeQueue;
		//delete []tm.handlers;
		return 0;
	}


