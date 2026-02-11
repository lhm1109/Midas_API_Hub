#ifndef __DB_FPSWU_DB_H__
#define __DB_FPSWU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_FPSWU
{
public:
	CDB_FPSWU()
	{
		m_fpsw.InitHashTable(HASHSIZEFPSW);
	}
	virtual ~CDB_FPSWU(){};

public:
	void Add(T_UDRD_KEY Key,T_FPSW_UDRD_D& rData)
		{m_fpsw.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_fpsw.RemoveKey(Key);}
	void DelAll()
		{m_fpsw.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_FPSW_UDRD_D& rData)
		{return m_fpsw.Lookup(Key,rData);}
	int GetCount()
		{return m_fpsw.GetCount();}
	POSITION GetStart()
		{return m_fpsw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_FPSW_UDRD_D& rData)
		{m_fpsw.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_FPSW_UDRD_D,T_FPSW_UDRD_D&>m_fpsw;
};

#endif