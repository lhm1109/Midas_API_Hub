#ifndef __DB_SRSV_DB_H__
#define __DB_SRSV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SRSV
{
public:
	CDB_SRSV();
	virtual ~CDB_SRSV();
	CDBDoc* m_pDoc;

public:
	void Add(T_SRSV_K Key, T_SRSV_D& rData/*, CDB_ELEM* pElem*/);
	BOOL Del(T_SRSV_K Key/*, CDB_ELEM* pElem*/);

public:
	BOOL Get(T_SRSV_K Key, T_SRSV_D& rData);
	//{return m_srsv.Lookup(Key,rData);}
	int GetCount();
	//{return m_srsv.GetCount();}
	POSITION GetStart();
	//{return m_srsv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_SRSV_K& rKey, T_SRSV_D& rData);
	//{m_srsv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SRSV_K, T_SRSV_K, T_SRSV_D, T_SRSV_D&>m_srsv;
};

#endif

