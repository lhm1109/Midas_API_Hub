#ifndef __DB_LLCD_DB_H__
#define __DB_LLCD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDBDoc;
class CDB_STLD;
class CDB_LLCD
{
public:
	CDB_LLCD();
	virtual ~CDB_LLCD();
	CDBDoc* m_pDoc;
	
public:
	void Add(T_LLCD_K Key,T_LLCD_D& rData,CDB_STLD* pStld);
	BOOL Del(T_LLCD_K Key,CDB_STLD* pStld);
	
public:
	BOOL Get(T_LLCD_K Key,T_LLCD_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_LLCD_K& rKey,T_LLCD_D& rData);
	
protected:
	CMap<T_LLCD_K,T_LLCD_K,T_LLCD_D,T_LLCD_D&>m_LLCD;
};

#endif