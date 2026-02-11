#ifndef __DB_SERV_DB_H__
#define __DB_SERV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SERV
{
public:
	CDB_SERV();
	virtual ~CDB_SERV();
	CDBDoc* m_pDoc;

public:
	void Add(T_SERV_K Key,T_SERV_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SERV_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SERV_K Key,T_SERV_D& rData);
		//{return m_serv.Lookup(Key,rData);}
	int GetCount();
		//{return m_serv.GetCount();}
	POSITION GetStart();
		//{return m_serv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SERV_K& rKey,T_SERV_D& rData);
		//{m_serv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SERV_K,T_SERV_K,T_SERV_D,T_SERV_D&>m_serv;
};

#endif

