//spls
#ifndef __DB_SPLS_DB_H__
#define __DB_SPLS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SPLS
{
public:
	CDB_SPLS();
	virtual ~CDB_SPLS();
	CDBDoc* m_pDoc;

public:
	void Add(T_SPLS_K Key, T_SPLS_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_SPLS_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_SPLS_K Key, T_SPLS_D& rData);
	//{return m_Spls.Lookup(Key,rData);}
	int GetCount();
	//{return m_Spls.GetCount();}
	POSITION GetStart();
	//{return m_Spls.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_SPLS_K& rKey, T_SPLS_D& rData);
	//{m_Spls.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SPLS_K, T_SPLS_K, T_SPLS_D, T_SPLS_D&> m_Spls;
};

#endif
