#ifndef __DB_CRGR_DB_H__
#define __DB_CRGR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_CRGR
{
public:
	CDB_CRGR();
	virtual ~CDB_CRGR();
	CDBDoc* m_pDoc;

public:
	void Add(T_CRGR_D& rData,CDB_GRUP* pGrup);
	BOOL Del(CDB_GRUP* pGrup);

public:
	BOOL Get(T_CRGR_D& rData);
	int GetCount();

protected:
	CMap<T_CRGR_K,T_CRGR_K,T_CRGR_D,T_CRGR_D&> m_crgr;
};

#endif