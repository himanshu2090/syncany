
#ifndef __SYNC_FRAME_H__
#define __SYNC_FRAME_H__


//服务框架
class SyncFrame
{
public:

	SyncFrame();
	~SyncFrame();
	
	//超时监控线程
	int DispatchProcess(char* pszCmd,//命令行
						unsigned short nPort,//监听的端口号
						unsigned int nProcessCount,//启动的子进程个数
						unsigned int nThreadCount,//每个进程启动的线程个数
						unsigned int nConnectionCount,//每个进程接收的连接个数
						unsigned int nTimeout,//超时的时长
						unsigned int nBacklog,//backlong队列大小
						const char* pszSemaphoreName,//信号量的名称
						unsigned int nSemaphoreCount//信号量的初始值
						);
		
	

	/**
	子进程主线程。在主线程中启动工作子线程（使用TP Reactor）
	在主线程中接收客户端连接请求，客户端请求加入到消息循环中。
	主线程接收到规定的客户端连接请求后，不再接收客户端请求，由其他工作子进程接收处理。
         在此处使用进程间锁来控制工作子进程顺序接收客户端连接请求，该进程间锁在主控进程中创建。
         */
	int WorkProcess(char* pszSocket,
					char* pszThreadCount,
					char* pszConnectionCount,
					char* pszTimeout,
					char* pszSemaphoreName,
					char* pszSemaphoreCount
					);


};


#endif


