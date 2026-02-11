//spcs
#ifndef __DB_SPCS_DB_H__
#define __DB_SPCS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SPCS
{
public:
	CDB_SPCS();
	virtual ~CDB_SPCS();
	CDBDoc* m_pDoc;

public:
	void Add(T_SPCS_K Key, T_SPCS_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_SPCS_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_SPCS_K Key, T_SPCS_D& rData);
	//{return m_Spcs.Lookup(Key,rData);}
	int GetCount();
	//{return m_Spcs.GetCount();}
	POSITION GetStart();
	//{return m_Spcs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_SPCS_K& rKey, T_SPCS_D& rData);
	//{m_Spcs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SPCS_K, T_SPCS_K, T_SPCS_D, T_SPCS_D&> m_Spcs;
};

#endif