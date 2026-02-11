#ifndef __DB_SDDP_DB_H__
#define __DB_SDDP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SDDP
{
public:
	CDB_SDDP();
	virtual ~CDB_SDDP();
	CDBDoc* m_pDoc;

public:
	void Add(T_SDDP_K Key,T_SDDP_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SDDP_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SDDP_K Key,T_SDDP_D& rData);
		//{return m_sddp.Lookup(Key,rData);}
	int GetCount();
		//{return m_sddp.GetCount();}
	POSITION GetStart();
		//{return m_sddp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SDDP_K& rKey,T_SDDP_D& rData);
		//{m_sddp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SDDP_K,T_SDDP_K,T_SDDP_D,T_SDDP_D&>m_sddp;
};

#endif

