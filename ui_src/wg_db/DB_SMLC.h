#ifndef __DB_SMLC_DB_H__
#define __DB_SMLC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SMPT;

class CDB_SMLC
{
public:
	CDB_SMLC();
	virtual ~CDB_SMLC();
	CDBDoc* m_pDoc;

public:
	T_SMLC_K m_nStartNum;
	T_SMLC_K m_nLastNum;

public:
	void Add(T_SMLC_K Key,T_SMLC_D& rData,CDB_SMPT* pSmpt);
	BOOL Del(T_SMLC_K Key,CDB_SMPT* pSmpt);

public:
	BOOL Get(T_SMLC_K Key,T_SMLC_D& rData);
		//{return m_smlc.Lookup(Key,rData);}
	int GetCount();
		//{return m_smlc.GetCount();}
	POSITION GetStart();
		//{return m_smlc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SMLC_K& rKey,T_SMLC_D& rData);
		//{m_smlc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SMLC_K,T_SMLC_K,T_SMLC_D,T_SMLC_D&>m_smlc;
};

#endif
