#ifndef __DB_SLANop_DB_H__
#define __DB_SLANop_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_SLANop
{
public:
	CDB_SLANop();
	virtual ~CDB_SLANop();
	CDBDoc* m_pDoc;

public:
	T_SLANop_K m_nStartNum;
	T_SLANop_K m_nLastNum;

public:
	void Add(T_SLANop_K Key,T_SLANop_D& rData,CDB_NODE* pNode);
	BOOL Del(T_SLANop_K Key,CDB_NODE* pNode);

public:
	BOOL Get(T_SLANop_K Key,T_SLANop_D& rData);
		//{return m_slanop.Lookup(Key,rData);}
	int GetCount();
		//{return m_slanop.GetCount();}
	POSITION GetStart();
		//{return m_slanop.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SLANop_K& rKey,T_SLANop_D& rData);
		//{m_slanop.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SLANop_K,T_SLANop_K,T_SLANop_D,T_SLANop_D&>m_slanop;
};

#endif

