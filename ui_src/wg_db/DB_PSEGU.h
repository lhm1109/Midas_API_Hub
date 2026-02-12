#ifndef __DB_PSEGU_DB_H__
#define __DB_PSEGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PSEGU
{
public:
	CDB_PSEGU()
	{
		m_pseg.InitHashTable(HASHSIZEPSEG);
	}
	virtual ~CDB_PSEGU(){};

public:
	void Add(T_UDRD_KEY Key,T_PSEG_UDRD_D& rData)
		{m_pseg.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pseg.RemoveKey(Key);}
	void DelAll()
		{m_pseg.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PSEG_UDRD_D& rData)
		{return m_pseg.Lookup(Key,rData);}
	int GetCount()
		{return m_pseg.GetCount();}
	POSITION GetStart()
		{return m_pseg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PSEG_UDRD_D& rData)
		{m_pseg.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PSEG_UDRD_D,T_PSEG_UDRD_D&> m_pseg;
};

#endif
