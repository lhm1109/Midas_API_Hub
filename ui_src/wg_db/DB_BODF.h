#ifndef __DB_BODF_DB_H__
#define __DB_BODF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_LDGR;

class CDB_BODF
{
public:
	CDB_BODF();
	virtual ~CDB_BODF();
	CDBDoc* m_pDoc;

public:
	T_BODF_K m_nLastNum;
	T_BODF_K m_nStartNum;

public:
	void Add(T_BODF_K Key,T_BODF_D& rData,CDB_STLD* pStld,CDB_LDGR* pLdgr);
	BOOL Del(T_BODF_K Key,CDB_STLD* pStld,CDB_LDGR* pLdgr);

public:
	BOOL Get(T_BODF_K Key,T_BODF_D& rData);
		//{return m_bodf.Lookup(Key,rData);}
	int GetCount();
		//{return m_bodf.GetCount();}
	POSITION GetStart();
		//{return m_bodf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_BODF_K& rKey,T_BODF_D& rData);
		//{m_bodf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_BODF_K,T_BODF_K,T_BODF_D,T_BODF_D&>m_bodf;
};

#endif