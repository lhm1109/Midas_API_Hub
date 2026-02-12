#ifndef __DB_IEHG_DB_H__
#define __DB_IEHG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_IEHG
{
public:
	CDB_IEHG();
	virtual ~CDB_IEHG();
	CDBDoc* m_pDoc;

public:
	void Add(T_IEHG_K Key,T_IEHG_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_IEHG_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_IEHG_K Key,T_IEHG_D& rData);
		//{return m_iehg.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_iehg.GetCount();}
	POSITION GetStart();
		//{return m_iehg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_IEHG_K& rKey,T_IEHG_D& rData);
		//{m_iehg.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_IEHG_KEY, T_IEHG_KEY,T_IEHG_D,T_IEHG_D&>m_iehg;
};

#endif