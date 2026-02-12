#ifndef __DB_GRDP_DB_H__
#define __DB_GRDP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GRUP;
class CDB_BNGR;

class CDBDoc;

class CDB_GRDP
{
public:
	CDB_GRDP();
	virtual ~CDB_GRDP();
	CDBDoc* m_pDoc;

public:
	void Add(T_GRDP_D& rData, CDB_GRUP* pGrup, CDB_BNGR* pBngr);
	BOOL Del(CDB_GRUP* pGrup, CDB_BNGR* pBngr);

public:
	BOOL Get(T_GRDP_D& rData);
		//{return m_grdp.Lookup(1,rData);}
	int GetCount();
		//{return m_grdp.GetCount();}

protected:
	CMap<T_GRDP_K,T_GRDP_K,T_GRDP_D,T_GRDP_D&> m_grdp;
};

#endif