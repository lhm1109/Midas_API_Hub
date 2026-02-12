#ifndef __DB_HNGE_DB_H__
#define __DB_HNGE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_HNGE
{
public:
	CDB_HNGE();
	virtual ~CDB_HNGE();
	CDBDoc* m_pDoc;

public:
	void Add(T_HNGE_K Key,T_HNGE_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_HNGE_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_HNGE_K Key,T_HNGE_D& rData);
		//{return m_hnge.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_hnge.GetCount();}
	POSITION GetStart();
		//{return m_hnge.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_HNGE_K& rKey,T_HNGE_D& rData);
		//{m_hnge.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_HNGE_KEY,T_HNGE_KEY,T_HNGE_D,T_HNGE_D&>m_hnge;
};

#endif