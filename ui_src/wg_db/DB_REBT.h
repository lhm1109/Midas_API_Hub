#ifndef __DB_REBT_DB_H__
#define __DB_REBT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SECT;

class CDB_REBT
{
public:
	CDB_REBT();
	virtual ~CDB_REBT();
	CDBDoc* m_pDoc;

public:
	void Add(T_REBT_K Key,T_REBT_D& rData,CDB_SECT* pSect);
	BOOL Del(T_REBT_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_REBT_K Key,T_REBT_D& rData);
		//{return m_rebt.Lookup(Key,rData);}
	int GetCount();
		//{return m_rebt.GetCount();}
	POSITION GetStart();
		//{return m_rebt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_REBT_K& rKey,T_REBT_D& rData);
		//{m_rebt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_REBT_K,T_REBT_K,T_REBT_D,T_REBT_D&>m_rebt;
};

#endif

