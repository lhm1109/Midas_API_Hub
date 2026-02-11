#ifndef __DB_THNLU_DB_H__
#define __DB_THNLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THNLU
{
public:
	CDB_THNLU()
	{
		m_thnl.InitHashTable(HASHSIZETHNL);
	}
	virtual ~CDB_THNLU(){};

public:
	void Add(T_UDRD_KEY Key,T_THNL_UDRD_D& rData)
		{m_thnl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_thnl.RemoveKey(Key);}
	void DelAll()
		{m_thnl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_THNL_UDRD_D& rData)
		{return m_thnl.Lookup(Key,rData);}
	int GetCount()
		{return m_thnl.GetCount();}
	POSITION GetStart()
		{return m_thnl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_THNL_UDRD_D& rData)
		{m_thnl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_THNL_UDRD_D,T_THNL_UDRD_D&>m_thnl;
};

#endif