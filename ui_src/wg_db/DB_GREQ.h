#ifndef __DB_GREQ_DB_H__
#define __DB_GREQ_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_GREQ
{
public:
	CDB_GREQ();
	virtual ~CDB_GREQ();
	CDBDoc* m_pDoc;

public:
	void Add(T_GREQ_K Key,T_GREQ_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_GREQ_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_GREQ_K Key,T_GREQ_D& rData);
		//{return m_greq.Lookup(Key,rData);}
	int GetCount();
		//{return m_greq.GetCount();}
	POSITION GetStart();
		//{return m_greq.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_GREQ_K& rKey,T_GREQ_D& rData);
		//{m_greq.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_GREQ_K,T_GREQ_K,T_GREQ_D,T_GREQ_D&>m_greq;
};

#endif

