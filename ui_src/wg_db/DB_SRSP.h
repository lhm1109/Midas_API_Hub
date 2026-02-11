#ifndef __DB_SRSP_DB_H__
#define __DB_SRSP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SRSP
{
public:
	CDB_SRSP();
	virtual ~CDB_SRSP();
	CDBDoc* m_pDoc;

public:
	void Add(T_SRSP_K Key, T_SRSP_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_SRSP_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_SRSP_K Key, T_SRSP_D& rData);
	//{return m_srsp.Lookup(Key,rData);}
	int GetCount();
	//{return m_srsp.GetCount();}
	POSITION GetStart();
	//{return m_srsp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_SRSP_K& rKey, T_SRSP_D& rData);
	//{m_srsp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SRSP_K, T_SRSP_K, T_SRSP_D, T_SRSP_D&>m_srsp;
};

#endif

