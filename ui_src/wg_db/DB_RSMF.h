#ifndef __DB_RSMF_DB_H__
#define __DB_RSMF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SPLC;

class CDB_RSMF
{
public:
	CDB_RSMF();
	virtual ~CDB_RSMF();
	CDBDoc* m_pDoc;

public:
	void Add(T_RSMF_K Key,T_RSMF_D& rData);
	BOOL Del(T_RSMF_K Key);

public:
	BOOL Get(T_RSMF_K Key,T_RSMF_D& rData);
		//{return m_rsmf.Lookup(Key,rData);}
	int GetCount();
		//{return m_rsmf.GetCount();}
	POSITION GetStart();
		//{return m_rsmf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RSMF_K& rKey,T_RSMF_D& rData);
		//{m_rsmf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RSMF_K,T_RSMF_K,T_RSMF_D,T_RSMF_D&>m_rsmf;
};

#endif