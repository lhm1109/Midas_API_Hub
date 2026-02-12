#ifndef __DB_RMFR_DB_H__
#define __DB_RMFR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RMFR
{
public:
	CDB_RMFR();
	virtual ~CDB_RMFR();
	CDBDoc* m_pDoc;

public:
	void Add(T_RMFR_K Key,T_RMFR_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RMFR_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RMFR_K Key,T_RMFR_D& rData);
		//{return m_rmfr.Lookup(Key,rData);}
	int GetCount();
		//{return m_rmfr.GetCount();}
	POSITION GetStart();
		//{return m_rmfr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RMFR_K& rKey,T_RMFR_D& rData);
		//{m_rmfr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RMFR_K,T_RMFR_K,T_RMFR_D,T_RMFR_D&>m_rmfr;
};

#endif

