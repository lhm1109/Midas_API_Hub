#ifndef __DB_CBEM_DB_H__
#define __DB_CBEM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_CBEM
{
public:
	CDB_CBEM();
	virtual ~CDB_CBEM();
	CDBDoc* m_pDoc;

public:
	T_CBEM_K m_nStartNum;
	T_CBEM_K m_nLastNum;

public:
	void Add(T_CBEM_K Key,T_CBEM_D& rData,CDB_NODE* pNode);
	BOOL Del(T_CBEM_K Key,CDB_NODE* pNode);

public:
	BOOL Get(T_CBEM_K Key,T_CBEM_D& rData);
		//{return m_cbem.Lookup(Key,rData);}
	int GetCount();
		//{return m_cbem.GetCount();}
	POSITION GetStart();
		//{return m_cbem.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CBEM_K& rKey,T_CBEM_D& rData);
		//{m_cbem.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CBEM_K,T_CBEM_K,T_CBEM_D,T_CBEM_D&>m_cbem;
};

#endif

