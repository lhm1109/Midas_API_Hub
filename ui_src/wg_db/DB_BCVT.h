#ifndef __DB_BCVT_DB_H__
#define __DB_BCVT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_ELEM;

class CDB_BCVT
{
public:
	CDB_BCVT();
	virtual ~CDB_BCVT();
	CDBDoc* m_pDoc;

public:
	T_BCVT_K m_nStartNum;
	T_BCVT_K m_nLastNum;

public:
	void Add(T_BCVT_K Key,T_BCVT_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem);
	BOOL Del(T_BCVT_K Key,CDB_NODE* pNode,CDB_ELEM* pElem);

public:
	BOOL Get(T_BCVT_K Key,T_BCVT_D& rData);
		//{return m_bcvt.Lookup(Key,rData);}
	int GetCount();
		//{return m_bcvt.GetCount();}
	POSITION GetStart();
		//{return m_bcvt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_BCVT_K& rKey,T_BCVT_D& rData);
		//{m_bcvt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_BCVT_K,T_BCVT_K,T_BCVT_D,T_BCVT_D&>m_bcvt;
};

#endif