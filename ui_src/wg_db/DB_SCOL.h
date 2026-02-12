#ifndef __DB_SCOL_DB_H__
#define __DB_SCOL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDBDoc;
class CDB_ELEM;
class CDB_SCOL
{
public:
	CDB_SCOL();
	virtual ~CDB_SCOL();
	CDBDoc* m_pDoc;

public:
	void Add(T_SCOL_K Key,T_SCOL_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SCOL_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SCOL_K Key,T_SCOL_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_SCOL_K& rKey,T_SCOL_D& rData);

protected:
	CMap<T_SCOL_K,T_SCOL_K,T_SCOL_D,T_SCOL_D&>m_scol;
};

#endif