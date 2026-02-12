#ifndef __DB_CGTE_DB_H__
#define __DB_CGTE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_CGTE
{
public:
	CDB_CGTE();
	virtual ~CDB_CGTE();
	CDBDoc* m_pDoc;

public:
	void Add(T_CGTE_K Key,T_CGTE_D& rData,CDB_NODE* pNode);
	BOOL Del(T_CGTE_K Key,CDB_NODE* pNode);

public:
	BOOL Get(T_CGTE_K Key,T_CGTE_D& rData);
		//{return m_cgte.Lookup(Key,rData);}
	int GetCount();
		//{return m_cgte.GetCount();}
	POSITION GetStart();
		//{return m_cgte.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CGTE_K& rKey,T_CGTE_D& rData);
		//{m_cgte.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CGTE_K,T_CGTE_K,T_CGTE_D,T_CGTE_D&>m_cgte;
};

#endif

