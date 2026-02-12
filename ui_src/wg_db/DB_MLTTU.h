#ifndef __DB_MLTTU_DB_H__ 
#define __DB_MLTTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MLTTU
{
public:
	CDB_MLTTU()
	{
		m_mltt.InitHashTable(HASHSIZEMLTT);
	}
	virtual ~CDB_MLTTU(){};

public:
	void Add(T_UDRD_KEY Key,T_MLTT_UDRD_D& rData)
		{m_mltt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mltt.RemoveKey(Key);}
	void DelAll()
		{m_mltt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MLTT_UDRD_D& rData)
		{return m_mltt.Lookup(Key,rData);}
	int GetCount()
		{return m_mltt.GetCount();}
	POSITION GetStart()
		{return m_mltt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MLTT_UDRD_D& rData)
		{m_mltt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MLTT_UDRD_D,T_MLTT_UDRD_D&>m_mltt;
};

#endif