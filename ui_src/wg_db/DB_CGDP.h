#ifndef __DB_CGDP_DB_H__
#define __DB_CGDP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_CGDP
{
public:
	CDB_CGDP();
	virtual ~CDB_CGDP();
	CDBDoc* m_pDoc;

public:
	void Add(T_CGDP_K Key,T_CGDP_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_CGDP_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_CGDP_K Key,T_CGDP_D& rData);
		//{return m_cgdp.Lookup(Key,rData);}
	int GetCount();
		//{return m_cgdp.GetCount();}
	POSITION GetStart();
		//{return m_cgdp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CGDP_K& rKey,T_CGDP_D& rData);
		//{m_cgdp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CGDP_K,T_CGDP_K,T_CGDP_D,T_CGDP_D&>m_cgdp;
};

#endif

