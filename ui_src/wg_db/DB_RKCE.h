#ifndef __DB_RKCE_DB_H__
#define __DB_RKCE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RKCE
{
public:
	CDB_RKCE();
	virtual ~CDB_RKCE();
	CDBDoc* m_pDoc;

public:
	void Add(T_RKCE_K Key,T_RKCE_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RKCE_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RKCE_K Key,T_RKCE_D& rData);
		//{return m_rkce.Lookup(Key,rData);}
	int GetCount();
		//{return m_rkce.GetCount();}
	POSITION GetStart();
		//{return m_rkce.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RKCE_K& rKey,T_RKCE_D& rData);
		//{m_rkce.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RKCE_K,T_RKCE_K,T_RKCE_D,T_RKCE_D&>m_rkce;
};

#endif

