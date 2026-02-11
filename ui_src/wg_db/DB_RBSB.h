#ifndef __DB_RBSB_DB_H__
#define __DB_RBSB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SECT;

class CDB_RBSB
{
public:
	CDB_RBSB();
	virtual ~CDB_RBSB();
	CDBDoc* m_pDoc;

public:
	void Add(T_RBSB_K Key,T_RBSB_D& rData,CDB_SECT* pSect);
	BOOL Del(T_RBSB_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_RBSB_K Key,T_RBSB_D& rData);
		//{return m_rbsb.Lookup(Key,rData);}
	int GetCount();
		//{return m_rbsb.GetCount();}
	POSITION GetStart();
		//{return m_rbsb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RBSB_K& rKey,T_RBSB_D& rData);
		//{m_rbsb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RBSB_K,T_RBSB_K,T_RBSB_D,T_RBSB_D&>m_rbsb;
};

#endif

