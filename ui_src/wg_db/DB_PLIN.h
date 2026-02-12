#ifndef __DB_PLIN_DB_H__
#define __DB_PLIN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_PLIN
{
public:
	CDB_PLIN();
	virtual ~CDB_PLIN();
	CDBDoc* m_pDoc;

public:
	void Add(T_PLIN_K Key,T_PLIN_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_PLIN_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_PLIN_K Key,T_PLIN_D& rData);
		//{return m_leng.Lookup(Key,rData);}
	int GetCount();
		//{return m_leng.GetCount();}
	POSITION GetStart();
		//{return m_leng.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PLIN_K& rKey,T_PLIN_D& rData);
		//{m_leng.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PLIN_K,T_PLIN_K,T_PLIN_D,T_PLIN_D&>m_plin;
};

#endif

