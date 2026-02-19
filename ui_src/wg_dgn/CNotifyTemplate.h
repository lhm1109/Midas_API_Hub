// (060123) sshan Create
#ifndef __PROXY_NOTIFYRECEIVER__H__
#define __PROXY_NOTIFYRECEIVER__H__

//-------------------------------------------------------------------------------
// 다중 상속을 받기 힘든 상황일때 사용한다. 
// class T에 void OnNotifyCommand(I_NotifyCommand* pNotifyCmd) member Function이 
// 구현되어 있어야 한다. 
//

#include "CNotifyManager.h"
template<class T> 
class Proxy_NotifyReceiver : public I_NotifyReceiver
{
protected:
	T* m_pReceiver;
public:
	
	Proxy_NotifyReceiver()
	{
		m_pReceiver = 0;    
	}

	void SetReceiver(T* pReceiver)
	{
		m_pReceiver = pReceiver;     
	}

	~Proxy_NotifyReceiver()
	{
		
	}

	virtual void OnNotifyCommand(I_NotifyCommand* pNotifyCmd)
	{
		 m_pReceiver->OnNotifyCommand(pNotifyCmd);
	}
};

#endif

