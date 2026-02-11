#ifndef __DB_DFBA_DB_H__
#define __DB_DFBA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_DFBA
{
public:
	CDB_DFBA();
	virtual ~CDB_DFBA();
	CDBDoc* m_pDoc;

public:
	void Add(T_DFBA_K Key,T_DFBA_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_DFBA_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_DFBA_K Key,T_DFBA_D& rData);
		//{return m_dfba.Lookup(Key,rData);}
	int GetCount();
		//{return m_dfba.GetCount();}
	POSITION GetStart();
		//{return m_dfba.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DFBA_K& rKey,T_DFBA_D& rData);
		//{m_dfba.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DFBA_K,T_DFBA_K,T_DFBA_D,T_DFBA_D&>m_dfba;
};

#endif

