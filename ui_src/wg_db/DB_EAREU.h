#ifndef __DB_EAREU_DB_H__
#define __DB_EAREU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EAREU
{
public:
	CDB_EAREU()
	{
		m_eare.InitHashTable(HASHSIZEPRES);
	}
	virtual ~CDB_EAREU(){};

public:
	void Add(T_UDRD_KEY Key,T_EARE_UDRD_D& rData)
		{m_eare.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_eare.RemoveKey(Key);}
	void DelAll()
		{m_eare.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_EARE_UDRD_D& rData)
		{return m_eare.Lookup(Key,rData);}
	int GetCount()
		{return m_eare.GetCount();}
	POSITION GetStart()
		{return m_eare.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_EARE_UDRD_D& rData)
		{m_eare.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_EARE_UDRD_D,T_EARE_UDRD_D&>m_eare;
};

#endif

