#ifndef __DB_RPRC_DB_H__
#define __DB_RPRC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RPRC
{
public:
	CDB_RPRC();
	virtual ~CDB_RPRC();
	CDBDoc* m_pDoc;

public:
	void Add(T_RPRC_K Key,T_RPRC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_RPRC_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_RPRC_K Key,T_RPRC_D& rData);
	//{return m_rkpo.Lookup(Key,rData);}
	int GetCount();
	//{return m_rkpo.GetCount();}
	POSITION GetStart();
	//{return m_rkpo.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RPRC_K& rKey,T_RPRC_D& rData);
	//{m_rkpo.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RPRC_K,T_RPRC_K,T_RPRC_D,T_RPRC_D&>m_rprc;
};

#endif

