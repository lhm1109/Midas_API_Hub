#ifndef __DB_FBLA_DB_H__
#define __DB_FBLA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_FBLD;
class CDB_NODE;
class CDB_LDGR;

class CDB_FBLA
{
public:
	CDB_FBLA();
	virtual ~CDB_FBLA();
	CDBDoc* m_pDoc;

public:
	T_MATL_K m_nLastNum;
	T_MATL_K m_nStartNum;

public:
	void Add(T_FBLA_K Key,T_FBLA_D& rData,CDB_FBLD* pFbld,CDB_NODE* pNode,CDB_LDGR* pLdgr);
	BOOL Del(T_FBLA_K Key,CDB_FBLD* pFbld,CDB_NODE* pNode,CDB_LDGR* pLdgr);

public:
	BOOL Get(T_FBLA_K Key,T_FBLA_D& rData);
		//{return m_fbla.Lookup(Key,rData);}
	int GetCount();
		//{return m_fbla.GetCount();}
	POSITION GetStart();
		//{return m_fbla.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_FBLA_K& rKey,T_FBLA_D& rData);
		//{m_fbla.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_FBLA_K,T_FBLA_K,T_FBLA_D,T_FBLA_D&>m_fbla;
};

#endif