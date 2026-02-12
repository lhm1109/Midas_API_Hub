#ifndef __DB_IEPI_DB_H__
#define __DB_IEPI_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_IEPI
{
public:
	CDB_IEPI();
	virtual ~CDB_IEPI();
	CDBDoc* m_pDoc;

public:
	void Add(T_IEPI_K Key,T_IEPI_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_IEPI_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_IEPI_K Key,T_IEPI_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_IEPI_K& rKey,T_IEPI_D& rData);

protected:
	CMap<T_IEPI_K,T_IEPI_K,T_IEPI_D,T_IEPI_D&>m_iepi;
};

#endif
