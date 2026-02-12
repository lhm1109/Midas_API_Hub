#ifndef __DB_SSEI_DB_H__
#define __DB_SSEI_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;

class CDB_SSEI
{
public:
	CDB_SSEI();
	virtual ~CDB_SSEI();
	CDBDoc* m_pDoc;

public:
	void Add(T_SSEI_K Key,T_SSEI_D& rData,CDB_STLD* pStld);
	BOOL Del(T_SSEI_K Key,CDB_STLD* pStld);

public:
	BOOL Get(T_SSEI_K Key,T_SSEI_D& rData);
		//{return m_ssei.Lookup(Key,rData);}
	int GetCount();
		//{return m_ssei.GetCount();}
	POSITION GetStart();
		//{return m_ssei.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SSEI_K& rKey,T_SSEI_D& rData);
		//{m_ssei.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SSEI_K,T_SSEI_K,T_SSEI_D,T_SSEI_D&>m_ssei;
};

#endif