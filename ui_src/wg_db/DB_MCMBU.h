#ifndef __DB_MCMBU_DB_H__
#define __DB_MCMBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MCMBU
{
public:
	CDB_MCMBU()
	{
		m_mcmb.InitHashTable(HASHSIZEMCMB);
	}
	virtual ~CDB_MCMBU(){};

public:
	void Add(T_UDRD_KEY Key,T_MCMB_UDRD_D& rData)
		{m_mcmb.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mcmb.RemoveKey(Key);}
	void DelAll()
		{m_mcmb.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MCMB_UDRD_D& rData)
		{return m_mcmb.Lookup(Key,rData);}
	int GetCount()
		{return m_mcmb.GetCount();}
	POSITION GetStart()
		{return m_mcmb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MCMB_UDRD_D& rData)
		{m_mcmb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MCMB_UDRD_D,T_MCMB_UDRD_D&>m_mcmb;
};

#endif