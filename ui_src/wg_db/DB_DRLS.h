#ifndef __DB_DRLS_DB_H__
#define __DB_DRLS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_DRLS
{
public:
	CDB_DRLS();
	virtual ~CDB_DRLS();
	CDBDoc* m_pDoc;

public:
	void Add(T_DRLS_K Key,T_DRLS_D& rData,CDB_NODE* pNode);
	BOOL Del(T_DRLS_K Key,CDB_NODE* pNode);

public:
	BOOL Get(T_DRLS_K Key,T_DRLS_D& rData);
		//{return m_drls.Lookup(Key,rData);}
	int GetCount();
		//{return m_drls.GetCount();}
	POSITION GetStart();
		//{return m_drls.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DRLS_K& rKey,T_DRLS_D& rData);
		//{m_drls.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DRLS_K,T_DRLS_K,T_DRLS_D,T_DRLS_D&>m_drls;
};

#endif
