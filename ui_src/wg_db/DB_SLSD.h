#ifndef __DB_SLSD_DB_H__
#define __DB_SLSD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
//class CDB_ELEM;

class CDB_SLSD
{
public:
	CDB_SLSD();
	virtual ~CDB_SLSD();
	CDBDoc* m_pDoc;

public:
	void Add(T_SLSD_K Key,T_SLSD_D& rData,CDB_SECT* pSect);
	BOOL Del(T_SLSD_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_SLSD_K Key,T_SLSD_D& rData);
		//{return m_kfac.Lookup(Key,rData);}
	int GetCount();
		//{return m_kfac.GetCount();}
	POSITION GetStart();
		//{return m_kfac.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SLSD_K& rKey,T_SLSD_D& rData);
		//{m_kfac.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SLSD_K,T_SLSD_K,T_SLSD_D,T_SLSD_D&>m_slsd;
};

#endif

