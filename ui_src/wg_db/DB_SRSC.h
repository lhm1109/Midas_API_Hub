#ifndef __DB_SRSC_DB_H__
#define __DB_SRSC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SRSC
{
public:
	CDB_SRSC();
	virtual ~CDB_SRSC();
	CDBDoc* m_pDoc;

public:
	void Add(T_SRSC_K Key, T_SRSC_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_SRSC_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_SRSC_K Key, T_SRSC_D& rData);
	//{return m_srsc.Lookup(Key,rData);}
	int GetCount();
	//{return m_srsc.GetCount();}
	POSITION GetStart();
	//{return m_srsc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_SRSC_K& rKey, T_SRSC_D& rData);
	//{m_srsc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SRSC_K, T_SRSC_K, T_SRSC_D, T_SRSC_D&>m_srsc;
};

#endif

