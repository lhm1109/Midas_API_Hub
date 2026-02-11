#ifndef __DB_ULFD_DB_H__
#define __DB_ULFD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_LCOM;

class CDB_ULFD
{
public:
	CDB_ULFD();
	virtual ~CDB_ULFD();
	CDBDoc* m_pDoc;

public:
	T_ULFD_K m_nStartNum;
	T_ULFD_K m_nLastNum;

public:
	void Add(T_ULFD_K Key,T_ULFD_D& rData);
	BOOL Del(T_ULFD_K Key);

public:
	BOOL Get(T_ULFD_K Key,T_ULFD_D& rData);
		//{return m_ulfd.Lookup(Key,rData);}
	int GetCount();
		//{return m_ulfd.GetCount();}
	POSITION GetStart();
		//{return m_ulfd.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_ULFD_K& rKey,T_ULFD_D& rData);
		//{m_ulfd.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_ULFD_K,T_ULFD_K,T_ULFD_D,T_ULFD_D&>m_ulfd;
};

#endif