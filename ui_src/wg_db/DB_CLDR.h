#ifndef __DB_CLDR_DB_H__
#define __DB_CLDR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_CLDR
{
public:
	CDB_CLDR();
	virtual ~CDB_CLDR();
	CDBDoc* m_pDoc;

public:
	void Add(T_CLDR_K Key,T_CLDR_D& rData,CDB_NODE* pNode);
	BOOL Del(T_CLDR_K Key,CDB_NODE* pNode);

public:
	BOOL Get(T_CLDR_K Key,T_CLDR_D& rData);
		//{return m_cldr.Lookup(Key,rData);}
	int GetCount();
		//{return m_cldr.GetCount();}
	POSITION GetStart();
		//{return m_cldr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CLDR_K& rKey,T_CLDR_D& rData);
		//{m_cldr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CLDR_K,T_CLDR_K,T_CLDR_D,T_CLDR_D&>m_cldr;
};

#endif