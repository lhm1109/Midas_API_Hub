#ifndef __DB_ALCS_DB_H__
#define __DB_ALCS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_ALCS
{
public:
	CDB_ALCS();
	virtual ~CDB_ALCS();
	CDBDoc* m_pDoc;

public:
	void Add(T_ALCS_K Key,T_ALCS_D& rData);
	BOOL Del(T_ALCS_K Key);

public:
	BOOL Get(T_ALCS_K Key,T_ALCS_D& rData);
		//{return m_alcs.Lookup(Key,rData);}
	int GetCount();
		//{return m_alcs.GetCount();}
	POSITION GetStart();
		//{return m_alcs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_ALCS_K& rKey,T_ALCS_D& rData);
		//{m_alcs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_ALCS_K,T_ALCS_K,T_ALCS_D,T_ALCS_D&>m_alcs;
};

#endif
