#ifndef __DB_STUS_DB_H__
#define __DB_STUS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_STUS
{
public:
	CDB_STUS();
	virtual ~CDB_STUS();
	CDBDoc* m_pDoc;

public:
	void Add(T_STUS_K Key,T_STUS_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_STUS_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_STUS_K Key,T_STUS_D& rData);
		//{return m_stus.Lookup(Key,rData);}
	int GetCount();
		//{return m_stus.GetCount();}
	POSITION GetStart();
		//{return m_stus.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_STUS_K& rKey,T_STUS_D& rData);
		//{m_stus.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_STUS_K,T_STUS_K,T_STUS_D,T_STUS_D&>m_stus;
};

#endif

