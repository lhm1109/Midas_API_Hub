#ifndef __DB_EVGP_DB_H__
#define __DB_EVGP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_NODE;

class CDB_EVGP
{
public:
	CDB_EVGP();
	virtual ~CDB_EVGP();
	CDBDoc* m_pDoc;

public:
	T_EVGP_K m_nStartNum;
	T_EVGP_K m_nLastNum;

public:
	void Add(T_EVGP_K Key,T_EVGP_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem);
	BOOL Del(T_EVGP_K Key,CDB_NODE* pNode,CDB_ELEM* pElem);

public:
	BOOL Get(T_EVGP_K Key,T_EVGP_D& rData);
	//{return m_ssps.Lookup(Key.keymap,rData);}
	int GetCount();
	//{return m_ssps.GetCount();}
	POSITION GetStart();
	//{return m_ssps.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_EVGP_K& rKey,T_EVGP_D& rData);
	//{m_ssps.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_EVGP_K,T_EVGP_K,T_EVGP_D,T_EVGP_D&>m_evgp;
};

#endif
