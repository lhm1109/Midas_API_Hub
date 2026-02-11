#ifndef __DB_LESF_DB_H__
#define __DB_LESF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;

class CDB_LESF
{
public:
	CDB_LESF();
	virtual ~CDB_LESF();
	CDBDoc* m_pDoc;

public:
	void Add(T_LESF_K Key,T_LESF_D& rData,CDB_STLD* pStld);
	BOOL Del(T_LESF_K Key,CDB_STLD* pStld);

public:
	BOOL Get(T_LESF_K Key,T_LESF_D& rData);
		//{return m_lesf.Lookup(Key,rData);}
	int GetCount();
		//{return m_lesf.GetCount();}
	POSITION GetStart();
		//{return m_lesf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LESF_K& rKey,T_LESF_D& rData);
		//{m_lesf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_LESF_K,T_LESF_K,T_LESF_D,T_LESF_D&>m_lesf;
};

#endif