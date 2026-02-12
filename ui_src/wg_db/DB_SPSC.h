#ifndef __DB_SPSC_DB_H__
#define __DB_SPSC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SPSC
{
public:
	CDB_SPSC();
	virtual ~CDB_SPSC();
	CDBDoc* m_pDoc;

public:
	void Add(T_SPSC_K Key, T_SPSC_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_SPSC_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_SPSC_K Key, T_SPSC_D& rData);
	//{return m_Spsc.Lookup(Key,rData);}
	int GetCount();
	//{return m_Spsc.GetCount();}
	POSITION GetStart();
	//{return m_Spsc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_SPSC_K& rKey, T_SPSC_D& rData);
	//{m_Spsc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SPSC_K, T_SPSC_K, T_SPSC_D, T_SPSC_D&> m_Spsc;
};

#endif
