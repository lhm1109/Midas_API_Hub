#ifndef __DB_NMAS_DB_H__
#define __DB_NMAS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_NMAS
{
public:
	CDB_NMAS();
	virtual ~CDB_NMAS();
	CDBDoc* m_pDoc;

public:
	void Add(T_NMAS_K Key,T_NMAS_D& rData,CDB_NODE* pNode);
	BOOL Del(T_NMAS_K Key,CDB_NODE* pNode);

public:
	BOOL Get(T_NMAS_K Key,T_NMAS_D& rData);
		//{return m_nmas.Lookup(Key,rData);}
	int GetCount();
		//{return m_nmas.GetCount();}
	POSITION GetStart();
		//{return m_nmas.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_NMAS_K& rKey,T_NMAS_D& rData);
		//{m_nmas.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_NMAS_K,T_NMAS_K,T_NMAS_D,T_NMAS_D&>m_nmas;
};

#endif