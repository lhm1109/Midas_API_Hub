#ifndef __DB_BLRG_DB_H__
#define __DB_BLRG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_GRUP;

class CDB_BLRG
{
public:
	CDB_BLRG();
	virtual ~CDB_BLRG();
	CDBDoc* m_pDoc;

public:
	void Add(T_BLRG_K Key,T_BLRG_D& rData,CDB_GRUP* pGrup);
	BOOL Del(T_BLRG_K Key,CDB_GRUP* pGrup);

public:
	BOOL Get(T_BLRG_K Key,T_BLRG_D& rData);
		//{return m_blrg.Lookup(Key,rData);}
	int GetCount();
		//{return m_blrg.GetCount();}
	POSITION GetStart();
		//{return m_blrg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_BLRG_K& rKey,T_BLRG_D& rData);
		//{m_blrg.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_BLRG_K,T_BLRG_K,T_BLRG_D,T_BLRG_D&>m_blrg;
};

#endif

