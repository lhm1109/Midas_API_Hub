#ifndef __DB_SEEW_DB_H__
#define __DB_SEEW_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDBDoc;
class CDB_STOR;
class CDB_SEEW
{
public:
	CDB_SEEW();
	virtual ~CDB_SEEW();
	CDBDoc* m_pDoc;

public:
	void Add(T_SEEW_K Key,T_SEEW_D& rData,CDB_STOR* pStor);
	BOOL Del(T_SEEW_K Key,CDB_STOR* pStor);

public:
	BOOL Get(T_SEEW_K Key,T_SEEW_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_SEEW_K& rKey,T_SEEW_D& rData);

protected:
	CMap<T_SEEW_K,T_SEEW_K,T_SEEW_D,T_SEEW_D&>m_SEEW;
};

#endif