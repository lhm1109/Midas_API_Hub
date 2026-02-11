#ifndef __DB_PHGE_DB_H__
#define __DB_PHGE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_PHGE
{
public:
	CDB_PHGE();
	virtual ~CDB_PHGE();
	CDBDoc* m_pDoc;

public:
	void Add(T_PHGE_K Key,T_PHGE_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_PHGE_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_PHGE_K Key,T_PHGE_D& rData);
		//{return m_phge.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_phge.GetCount();}
	POSITION GetStart();
		//{return m_phge.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PHGE_K& rKey,T_PHGE_D& rData);
		//{m_phge.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_PHGE_KEY,T_PHGE_KEY,T_PHGE_D,T_PHGE_D&>m_phge;
};

#endif