#ifndef __DB_IFGS_DB_H__
#define __DB_IFGS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_IFGS
{
public:
	CDB_IFGS();
	virtual ~CDB_IFGS();
	CDBDoc* m_pDoc;

public:
	void Add(T_IFGS_K Key,T_IFGS_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_IFGS_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_IFGS_K Key,T_IFGS_D& rData);
		//{return m_ifgs.Lookup(Key,rData);}
	int GetCount();
		//{return m_ifgs.GetCount();}
	POSITION GetStart();
		//{return m_ifgs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_IFGS_K& rKey,T_IFGS_D& rData);
		//{m_ifgs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_IFGS_K,T_IFGS_K,T_IFGS_D,T_IFGS_D&>m_ifgs;
};

#endif