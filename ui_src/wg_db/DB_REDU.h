#ifndef __DB_REDU_DB_H__
#define __DB_REDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_REDU
{
public:
	CDB_REDU();
	virtual ~CDB_REDU();
	CDBDoc* m_pDoc;

public:
	void Add(T_REDU_K Key,T_REDU_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_REDU_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_REDU_K Key,T_REDU_D& rData);
		//{return m_redu.Lookup(Key,rData);}
	int GetCount();
		//{return m_redu.GetCount();}
	POSITION GetStart();
		//{return m_redu.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_REDU_K& rKey,T_REDU_D& rData);
		//{m_redu.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_REDU_K,T_REDU_K,T_REDU_D,T_REDU_D&>m_redu;
};

#endif

