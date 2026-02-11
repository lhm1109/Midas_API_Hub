#ifndef __DB_RDGN_DB_H__
#define __DB_RDGN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SECT;

class CDB_RDGN
{
public:
	CDB_RDGN();
	virtual ~CDB_RDGN();
	CDBDoc* m_pDoc;

public:
	void Add(T_RDGN_K Key,T_RDGN_D& rData,CDB_SECT* pSect);
	BOOL Del(T_RDGN_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_RDGN_K Key,T_RDGN_D& rData);
		//{return m_rdgn.Lookup(Key,rData);}
	int GetCount();
		//{return m_rdgn.GetCount();}
	POSITION GetStart();
		//{return m_rdgn.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RDGN_K& rKey,T_RDGN_D& rData);
		//{m_rdgn.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RDGN_K,T_RDGN_K,T_RDGN_D,T_RDGN_D&>m_rdgn;
};

#endif

