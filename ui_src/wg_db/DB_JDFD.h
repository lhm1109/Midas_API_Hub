#ifndef __DB_JDFD_DB_H__
#define __DB_JDFD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDBDoc;
class CDB_STLD;

class CDB_JDFD
{
public:
	CDB_JDFD();
	virtual ~CDB_JDFD();
	CDBDoc* m_pDoc;

public:
	void Add(T_JDFD_K Key,T_JDFD_D& rData,CDB_STLD* pStld);
	BOOL Del(T_JDFD_K Key,CDB_STLD* pStld);

public:
	BOOL Get(T_JDFD_K Key,T_JDFD_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_JDFD_K& rKey,T_JDFD_D& rData);
	
protected:
	CMap<T_JDFD_KEY,T_JDFD_KEY,T_JDFD_D,T_JDFD_D&>m_jdfd;
};

#endif
