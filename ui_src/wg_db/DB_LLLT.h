#ifndef __DB_LLLT_DB_H__
#define __DB_LLLT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;

class CDB_LLLT
{
public:
	CDB_LLLT();
	virtual ~CDB_LLLT();
	CDBDoc* m_pDoc;

public:
	void Add(T_LLLT_K Key,T_LLLT_D& rData,CDB_STLD* pStld);
	BOOL Del(T_LLLT_K Key,CDB_STLD* pStld);

public:
	BOOL Get(T_LLLT_K Key,T_LLLT_D& rData);
		//{return m_lllt.Lookup(Key,rData);}
	int GetCount();
		//{return m_lllt.GetCount();}
	POSITION GetStart();
		//{return m_lllt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LLLT_K& rKey,T_LLLT_D& rData);
		//{m_lllt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_LLLT_K,T_LLLT_K,T_LLLT_D,T_LLLT_D&>m_lllt;
};

#endif