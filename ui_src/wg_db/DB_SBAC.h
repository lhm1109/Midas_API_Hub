#ifndef __DB_SBAC_DB_H__
#define __DB_SBAC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_ELEM;

class CDB_SBAC
{
public:
	CDB_SBAC();
	virtual ~CDB_SBAC();
	CDBDoc* m_pDoc;

public:
	T_SBAC_K m_nStartNum;
	T_SBAC_K m_nLastNum;

public:
	void Add(T_SBAC_K Key,T_SBAC_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem);
	BOOL Del(T_SBAC_K Key,CDB_NODE* pNode,CDB_ELEM* pElem);

public:
	BOOL Get(T_SBAC_K Key,T_SBAC_D& rData);
		//{return m_sbac.Lookup(Key,rData);}
	int GetCount();
		//{return m_sbac.GetCount();}
	POSITION GetStart();
		//{return m_sbac.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SBAC_K& rKey,T_SBAC_D& rData);
		//{m_sbac.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SBAC_K,T_SBAC_K,T_SBAC_D,T_SBAC_D&>m_sbac;
};

#endif