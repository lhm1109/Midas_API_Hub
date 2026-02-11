#ifndef __DB_REBS_DB_H__
#define __DB_REBS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SECT;

class CDB_REBS
{
public:
	CDB_REBS();
	virtual ~CDB_REBS();
	CDBDoc* m_pDoc;

public:
	void Add(T_REBS_K Key,T_REBS_D& rData,CDB_SECT* pSect);
	BOOL Del(T_REBS_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_REBS_K Key,T_REBS_D& rData);
		//{return m_rebs.Lookup(Key,rData);}
	int GetCount();
		//{return m_rebs.GetCount();}
	POSITION GetStart();
		//{return m_rebs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_REBS_K& rKey,T_REBS_D& rData);
		//{m_rebs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_REBS_K,T_REBS_K,T_REBS_D,T_REBS_D&>m_rebs;
};

#endif

