#ifndef __DB_SCDR_DB_H__
#define __DB_SCDR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CDBDoc;
class CDB_STOR;
class CDB_SCDR
{
public:
	CDB_SCDR();
	virtual ~CDB_SCDR();
	CDBDoc* m_pDoc;

public:
	void Add(T_SCDR_K Key,T_SCDR_D& rData,CDB_STOR* pStor);
	BOOL Del(T_SCDR_K Key,CDB_STOR* pStor);

public:
	T_SCDR_K m_nStartNum;
	T_SCDR_K m_nLastNum;

public:
	BOOL Get(T_SCDR_K Key,T_SCDR_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_SCDR_K& rKey,T_SCDR_D& rData);

protected:
	CMap<T_SCDR_K,T_SCDR_K,T_SCDR_D,T_SCDR_D&>m_SCDR;
};

#endif