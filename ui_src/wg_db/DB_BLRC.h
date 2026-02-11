#ifndef __DB_BLRC_DB_H__
#define __DB_BLRC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_BLRC
{
public:
	CDB_BLRC();
	virtual ~CDB_BLRC();
	CDBDoc* m_pDoc;

public:
	void Add(T_BLRC_K Key,T_BLRC_D& rData);
	BOOL Del(T_BLRC_K Key);

public:
	BOOL Get(T_BLRC_K Key,T_BLRC_D& rData);
		//{return m_blrc.Lookup(Key,rData);}
	int GetCount();
		//{return m_blrc.GetCount();}
	POSITION GetStart();
		//{return m_blrc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_BLRC_K& rKey,T_BLRC_D& rData);
		//{m_blrc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_BLRC_K,T_BLRC_K,T_BLRC_D,T_BLRC_D&>m_blrc;
};

#endif
