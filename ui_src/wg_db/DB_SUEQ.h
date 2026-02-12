#ifndef __DB_SUEQ_DB_H__
#define __DB_SUEQ_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SUEQ
{
public:
	CDB_SUEQ();
	virtual ~CDB_SUEQ();
	CDBDoc* m_pDoc;

public:
	void Add(T_SUEQ_K Key,T_SUEQ_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SUEQ_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SUEQ_K Key,T_SUEQ_D& rData);
		//{return m_sueq.Lookup(Key,rData);}
	int GetCount();
		//{return m_sueq.GetCount();}
	POSITION GetStart();
		//{return m_sueq.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SUEQ_K& rKey,T_SUEQ_D& rData);
		//{m_sueq.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SUEQ_K,T_SUEQ_K,T_SUEQ_D,T_SUEQ_D&>m_sueq;
};

#endif

