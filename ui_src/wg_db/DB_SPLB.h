//splb
#ifndef __DB_SPLB_DB_H__
#define __DB_SPLB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SPLB
{
public:
	CDB_SPLB();
	virtual ~CDB_SPLB();
	CDBDoc* m_pDoc;

public:
	void Add(T_SPLB_K Key, T_SPLB_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_SPLB_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_SPLB_K Key, T_SPLB_D& rData);
	//{return m_Splb.Lookup(Key,rData);}
	int GetCount();
	//{return m_Splb.GetCount();}
	POSITION GetStart();
	//{return m_Splb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_SPLB_K& rKey, T_SPLB_D& rData);
	//{m_Splb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SPLB_K, T_SPLB_K, T_SPLB_D, T_SPLB_D&> m_Splb;
};

#endif