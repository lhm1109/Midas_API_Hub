#ifndef __DB_ULEC_DB_H__
#define __DB_ULEC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_ULEC
{
public:
	CDB_ULEC();
	virtual ~CDB_ULEC();
	CDBDoc* m_pDoc;

public:
	void Add(T_ULEC_K Key,T_ULEC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_ULEC_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_ULEC_K Key,T_ULEC_D& rData);
		//{return m_ulec.Lookup(Key,rData);}
	int GetCount();
		//{return m_ulec.GetCount();}
	POSITION GetStart();
		//{return m_ulec.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_ULEC_K& rKey,T_ULEC_D& rData);
		//{m_ulec.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_ULEC_K,T_ULEC_K,T_ULEC_D,T_ULEC_D&>m_ulec;
};

#endif

