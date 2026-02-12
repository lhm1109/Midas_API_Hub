#ifndef __DB_STFN_DB_H__
#define __DB_STFN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_STFN
{
public:
	CDB_STFN();
	virtual ~CDB_STFN();
	CDBDoc* m_pDoc;

public:
	void Add(T_STFN_K Key,T_STFN_D& rData,CDB_SECT* pSect);
	BOOL Del(T_STFN_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_STFN_K Key,T_STFN_D& rData);
		//{return m_stfn.Lookup(Key,rData);}
	int GetCount();
		//{return m_stfn.GetCount();}
	POSITION GetStart();
		//{return m_stfn.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_STFN_K& rKey,T_STFN_D& rData);
		//{m_stfn.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_STFN_K,T_STFN_K,T_STFN_D,T_STFN_D&>m_stfn;
};

#endif

