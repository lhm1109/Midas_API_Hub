#ifndef __DB_RLCS_DB_H__
#define __DB_RLCS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RLCS
{
public:
	CDB_RLCS();
	virtual ~CDB_RLCS();
	CDBDoc* m_pDoc;

public:
	void Add(T_RLCS_K Key,T_RLCS_D& rData);
	BOOL Del(T_RLCS_K Key);

public:
	BOOL Get(T_RLCS_K Key,T_RLCS_D& rData);
		//{return m_rlcs.Lookup(Key,rData);}
	int GetCount();
		//{return m_rlcs.GetCount();}
	POSITION GetStart();
		//{return m_rlcs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RLCS_K& rKey,T_RLCS_D& rData);
		//{m_rlcs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RLCS_K,T_RLCS_K,T_RLCS_D,T_RLCS_D&>m_rlcs;
};

#endif
