#ifndef __DB_EARE_DB_H__
#define __DB_EARE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_NODE;

class CDB_EARE
{
public:
	CDB_EARE();
	virtual ~CDB_EARE();
	CDBDoc* m_pDoc;

public:
	T_EARE_K m_nStartNum;
	T_EARE_K m_nLastNum;

public:
	void Add(T_EARE_K Key,T_EARE_D& rData,CDB_ELEM* pElem, CDB_NODE* pNode);
	BOOL Del(T_EARE_K Key,CDB_ELEM* pElem,CDB_NODE* pNode);

public:
	BOOL Get(T_EARE_K Key,T_EARE_D& rData);
		//{return m_ssps.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_ssps.GetCount();}
	POSITION GetStart();
		//{return m_ssps.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_EARE_K& rKey,T_EARE_D& rData);
		//{m_ssps.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_EARE_K,T_EARE_K,T_EARE_D,T_EARE_D&>m_eare;
};

#endif
