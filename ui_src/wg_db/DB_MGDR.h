#ifndef __DB_MGDR_DB_H__
#define __DB_MGDR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_MGDR
{
public:
	CDB_MGDR();
	virtual ~CDB_MGDR();
	CDBDoc* m_pDoc;

public:
	T_MGDR_K m_nStartNum;
	T_MGDR_K m_nLastNum;

public:
	void Add(T_MGDR_K Key,T_MGDR_D& rData,CDB_NODE* pNode);
	BOOL Del(T_MGDR_K Key,CDB_NODE* pNode);

public:
	BOOL Get(T_MGDR_K Key,T_MGDR_D& rData);
		//{return m_mgdr.Lookup(Key,rData);}
	int GetCount();
		//{return m_mgdr.GetCount();}
	POSITION GetStart();
		//{return m_mgdr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MGDR_K& rKey,T_MGDR_D& rData);
		//{m_mgdr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MGDR_K,T_MGDR_K,T_MGDR_D,T_MGDR_D&>m_mgdr;
};

#endif

