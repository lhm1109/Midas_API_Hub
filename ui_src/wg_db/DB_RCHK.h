#ifndef __DB_RCHK_DB_H__
#define __DB_RCHK_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SECT;

class CDB_RCHK
{
public:
	CDB_RCHK();
	virtual ~CDB_RCHK();
	CDBDoc* m_pDoc;

public:
	void Add(T_RCHK_K Key,T_RCHK_D& rData,CDB_SECT* pSect);
	BOOL Del(T_RCHK_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_RCHK_K Key,T_RCHK_D& rData);
		//{return m_rchk.Lookup(Key,rData);}
	int GetCount();
		//{return m_rchk.GetCount();}
	POSITION GetStart();
		//{return m_rchk.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RCHK_K& rKey,T_RCHK_D& rData);
		//{m_rchk.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RCHK_K,T_RCHK_K,T_RCHK_D,T_RCHK_D&>m_rchk;
};

#endif

