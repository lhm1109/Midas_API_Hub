#ifndef __DB_SKEW_DB_H__
#define __DB_SKEW_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_SKEW
{
public:
	CDB_SKEW();
	virtual ~CDB_SKEW();
	CDBDoc* m_pDoc;

public:
	void Add(T_SKEW_K Key,T_SKEW_D& rData,CDB_NODE* pNode);
	BOOL Del(T_SKEW_K Key,CDB_NODE* pNode);

public:
	BOOL Get(T_SKEW_K Key,T_SKEW_D& rData);
		//{return m_skew.Lookup(Key,rData);}
	int GetCount();
		//{return m_skew.GetCount();}
	POSITION GetStart();
		//{return m_skew.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SKEW_K& rKey,T_SKEW_D& rData);
		//{m_skew.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SKEW_K,T_SKEW_K,T_SKEW_D,T_SKEW_D&>m_skew;
};

#endif