#ifndef __DB_DALW_DB_H__
#define __DB_DALW_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_DALW
{
public:
	CDB_DALW();
	virtual ~CDB_DALW();
	CDBDoc* m_pDoc;

public:
	void Add(T_DALW_K Key,T_DALW_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_DALW_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_DALW_K Key,T_DALW_D& rData);
		//{return m_dalw.Lookup(Key,rData);}
	int GetCount();
		//{return m_dalw.GetCount();}
	POSITION GetStart();
		//{return m_dalw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DALW_K& rKey,T_DALW_D& rData);
		//{m_dalw.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DALW_K,T_DALW_K,T_DALW_D,T_DALW_D&>m_dalw;
};

#endif

