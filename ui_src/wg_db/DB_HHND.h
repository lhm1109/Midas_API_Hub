#ifndef __DB_HHND_DB_H__
#define __DB_HHND_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_ELEM;

class CDB_HHND
{
public:
	CDB_HHND();
	virtual ~CDB_HHND();
	CDBDoc* m_pDoc;

public:
	T_HHND_K m_nStartNum;
	T_HHND_K m_nLastNum;

public:
	void Add(T_HHND_K Key,T_HHND_D& rData, CDB_NODE* pNode, CDB_ELEM* pElem);
	BOOL Del(T_HHND_K Key, CDB_NODE* pNode, CDB_ELEM* pElem);

public:
	BOOL Get(T_HHND_K Key,T_HHND_D& rData)
		{return m_hhnd.Lookup(Key,rData);}
	int GetCount()
		{return m_hhnd.GetCount();}
	POSITION GetStart()
		{return m_hhnd.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_HHND_K& rKey,T_HHND_D& rData)
		{m_hhnd.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_HHND_K,T_HHND_K,T_HHND_D,T_HHND_D&>m_hhnd;
};

#endif