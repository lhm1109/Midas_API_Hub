#ifndef __DB_SLANch_DB_H__
#define __DB_SLANch_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_SLANch
{
public:
	CDB_SLANch();
	virtual ~CDB_SLANch();
	CDBDoc* m_pDoc;

public:
	T_SLANch_K m_nStartNum;
	T_SLANch_K m_nLastNum;

public:
	void Add(T_SLANch_K Key,T_SLANch_D& rData,CDB_NODE* pNode);
	BOOL Del(T_SLANch_K Key,CDB_NODE* pNode);

public:
	BOOL Get(T_SLANch_K Key,T_SLANch_D& rData);
		//{return m_slanch.Lookup(Key,rData);}
	int GetCount();
		//{return m_slanch.GetCount();}
	POSITION GetStart();
		//{return m_slanch.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SLANch_K& rKey,T_SLANch_D& rData);
		//{m_slanch.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SLANch_K,T_SLANch_K,T_SLANch_D,T_SLANch_D&>m_slanch;
};

#endif

