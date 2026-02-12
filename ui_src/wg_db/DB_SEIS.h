#ifndef __DB_SEIS_DB_H__
#define __DB_SEIS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;

class CDB_SEIS
{
public:
	CDB_SEIS();
	virtual ~CDB_SEIS();
	CDBDoc* m_pDoc;

public:
	void Add(T_SEIS_K Key,T_SEIS_D& rData,CDB_STLD* pStld);
	BOOL Del(T_SEIS_K Key,CDB_STLD* pStld);

public:
	BOOL Get(T_SEIS_K Key,T_SEIS_D& rData);
		//{return m_seis.Lookup(Key,rData);}
	int GetCount();
		//{return m_seis.GetCount();}
	POSITION GetStart();
		//{return m_seis.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SEIS_K& rKey,T_SEIS_D& rData);
		//{m_seis.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SEIS_K,T_SEIS_K,T_SEIS_D,T_SEIS_D&>m_seis;
};

#endif