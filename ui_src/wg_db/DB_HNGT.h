#ifndef __DB_HNGT_DB_H__
#define __DB_HNGT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_HNGT
{
public:
	CDB_HNGT();
	virtual ~CDB_HNGT();
	CDBDoc* m_pDoc;

public:
	T_HNGT_K m_nStartNum;
	T_HNGT_K m_nLastNum;

public:
	void Add(T_HNGT_K Key,T_HNGT_D& rData);
	BOOL Del(T_HNGT_K Key);

public:
	BOOL Get(T_HNGT_K Key,T_HNGT_D& rData);
		//{return m_hngt.Lookup(Key,rData);}
	int GetCount();
		//{return m_hngt.GetCount();}
	POSITION GetStart();
		//{return m_hngt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_HNGT_K& rKey,T_HNGT_D& rData);
		//{m_hngt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_HNGT_K,T_HNGT_K,T_HNGT_D,T_HNGT_D&>m_hngt;
};

#endif
