#ifndef __DB_STBK_DB_H__
#define __DB_STBK_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_LDGR;

class CDB_STBK
{
public:
	CDB_STBK();
	virtual ~CDB_STBK();
	CDBDoc* m_pDoc;

public:
	T_STBK_K m_nLastNum;
	T_STBK_K m_nStartNum;

public:
	void Add(T_STBK_K Key, T_STBK_D& rData, CDB_NODE* pNode, CDB_LDGR* pLdgr);
	BOOL Del(T_STBK_K Key, CDB_NODE* pNode, CDB_LDGR* pLdgr);

public:
	BOOL Get(T_STBK_K Key, T_STBK_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_STBK_K& rKey, T_STBK_D& rData);

protected:
	CMap<T_STBK_K,T_STBK_K,T_STBK_D,T_STBK_D&>m_stbk;
};

#endif