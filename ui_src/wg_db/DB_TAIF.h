#ifndef __DB_TAIF_DB_H__
#define __DB_TAIF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_TAIF
{
public:
	CDB_TAIF();
	virtual ~CDB_TAIF();
	CDBDoc* m_pDoc;

public:
	void Add(T_TAIF_D& rData);
	BOOL Del();

public:
	BOOL Get(T_TAIF_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition, T_TAIF_K& rKey, T_TAIF_D& rData);
protected:
	CMap<T_TAIF_K, T_TAIF_K, T_TAIF_D, T_TAIF_D&> m_taif;
};

#endif
