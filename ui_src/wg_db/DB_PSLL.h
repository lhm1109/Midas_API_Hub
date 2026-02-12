#ifndef __DB_PSLL_DB_H__
#define __DB_PSLL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;

class CDB_PSLL
{
public:
	CDB_PSLL();
	virtual ~CDB_PSLL();
	CDBDoc* m_pDoc;

public:
	void Add(T_PSLL_K Key,T_PSLL_D& rData,CDB_STLD* pStld);
	BOOL Del(T_PSLL_K Key,CDB_STLD* pStld);

public:
	BOOL Get(T_PSLL_K Key,T_PSLL_D& rData);
		//{return m_psll.Lookup(Key,rData);}
	int GetCount();
		//{return m_psll.GetCount();}
	POSITION GetStart();
		//{return m_psll.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PSLL_K& rKey,T_PSLL_D& rData);
		//{m_psll.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PSLL_K,T_PSLL_K,T_PSLL_D,T_PSLL_D&>m_psll;
};

#endif