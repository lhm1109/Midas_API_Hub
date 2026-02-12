#ifndef __DB_PORD_DB_H__
#define __DB_PORD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_ELEM;

class CDB_PORD
{
public:
	CDB_PORD();
	virtual ~CDB_PORD();
	CDBDoc* m_pDoc;

public:
	T_PORD_K m_nStartNum;
	T_PORD_K m_nLastNum;

public:
	void Add(T_PORD_K Key,T_PORD_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem);
	BOOL Del(T_PORD_K Key,CDB_NODE* pNode,CDB_ELEM* pElem);

public:
	BOOL Get(T_PORD_K Key,T_PORD_D& rData);
		//{return m_pord.Lookup(Key,rData);}
	int GetCount();
		//{return m_pord.GetCount();}
	POSITION GetStart();
		//{return m_pord.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PORD_K& rKey,T_PORD_D& rData);
		//{m_pord.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PORD_K,T_PORD_K,T_PORD_D,T_PORD_D&>m_pord;
};

#endif