#ifndef __DB_ESQW_DB_H__
#define __DB_ESQW_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_STOR;
class CDB_MATL;

class CDB_ESQW
{
public:
	CDB_ESQW();
	virtual ~CDB_ESQW();
	CDBDoc* m_pDoc;

public:
	void Add(T_ESQW_D& rData,CDB_STLD* pStld);
	BOOL Del(CDB_STLD* pStld);

public:
	BOOL GetFromBaseDB(T_ESQW_D& rData)
		{return m_esqw.Lookup(1,rData);}
	BOOL Get(T_ESQW_D& rData);
		//{return m_esqw.Lookup(1,rData);}
	int GetCount();
		//{return m_esqw.GetCount();}

protected:
	CMap<T_ESQW_K,T_ESQW_K,T_ESQW_D,T_ESQW_D&>m_esqw;
};

#endif