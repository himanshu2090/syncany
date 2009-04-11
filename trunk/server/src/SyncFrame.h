
#ifndef __SYNC_FRAME_H__
#define __SYNC_FRAME_H__


//������
class SyncFrame
{
public:

	SyncFrame();
	~SyncFrame();
	
	//��ʱ����߳�
	int DispatchProcess(char* pszCmd,//������
						unsigned short nPort,//�����Ķ˿ں�
						unsigned int nProcessCount,//�������ӽ��̸���
						unsigned int nThreadCount,//ÿ�������������̸߳���
						unsigned int nConnectionCount,//ÿ�����̽��յ����Ӹ���
						unsigned int nTimeout,//��ʱ��ʱ��
						unsigned int nBacklog,//backlong���д�С
						const char* pszSemaphoreName,//�ź���������
						unsigned int nSemaphoreCount//�ź����ĳ�ʼֵ
						);
		
	

	/**
	�ӽ������̡߳������߳��������������̣߳�ʹ��TP Reactor��
	�����߳��н��տͻ����������󣬿ͻ���������뵽��Ϣѭ���С�
	���߳̽��յ��涨�Ŀͻ�����������󣬲��ٽ��տͻ������������������ӽ��̽��մ���
         �ڴ˴�ʹ�ý��̼��������ƹ����ӽ���˳����տͻ����������󣬸ý��̼��������ؽ����д�����
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


