#ifndef __DB_ULFC_DB_H__
#define __DB_ULFC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_ELEM;

class CDB_ULFC
{
public:
	CDB_ULFC();
	virtual ~CDB_ULFC();
	CDBDoc* m_pDoc;

public:
	T_ULFC_K m_nStartNum;
	T_ULFC_K m_nLastNum;

public:
	void Add(T_ULFC_K Key,T_ULFC_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem);
	BOOL Del(T_ULFC_K Key,CDB_NODE* pNode,CDB_ELEM* pElem);

public:
	BOOL Get(T_ULFC_K Key,T_ULFC_D& rData);
		//{return m_ulfc.Lookup(Key,rData);}
	int GetCount();
		//{return m_ulfc.GetCount();}
	POSITION GetStart();
		//{return m_ulfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_ULFC_K& rKey,T_ULFC_D& rData);
		//{m_ulfc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_ULFC_K,T_ULFC_K,T_ULFC_D,T_ULFC_D&>m_ulfc;
};

#endif