#ifndef __DB_TDATU_DB_H__
#define __DB_TDATU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDATU
{
public:
	CDB_TDATU()
	{
		m_tdat.InitHashTable(HASHSIZETDAT);
	}
	virtual ~CDB_TDATU(){};

public:
	void Add(T_UDRD_KEY Key,T_TDAT_UDRD_D& rData)
		{m_tdat.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_tdat.RemoveKey(Key);}
	void DelAll()
		{m_tdat.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_TDAT_UDRD_D& rData)
		{return m_tdat.Lookup(Key,rData);}
	int GetCount()
		{return m_tdat.GetCount();}
	POSITION GetStart()
		{return m_tdat.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_TDAT_UDRD_D& rData)
		{m_tdat.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_TDAT_UDRD_D,T_TDAT_UDRD_D&>m_tdat;
};

#endif