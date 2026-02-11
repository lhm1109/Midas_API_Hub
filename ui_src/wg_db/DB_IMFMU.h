#ifndef __DB_IMFMU_DB_H__
#define __DB_IMFMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_IMFMU
{
public:
	CDB_IMFMU()
	{
		m_imfm.InitHashTable(HASHSIZEIMFM);
	}
	virtual ~CDB_IMFMU(){};

public:
	void Add(T_UDRD_KEY Key,T_IMFM_UDRD_D& rData)
		{m_imfm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_imfm.RemoveKey(Key);}
	void DelAll()
		{m_imfm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_IMFM_UDRD_D& rData)
		{return m_imfm.Lookup(Key,rData);}
	int GetCount()
		{return m_imfm.GetCount();}
	POSITION GetStart()
		{return m_imfm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_IMFM_UDRD_D& rData)
		{m_imfm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_IMFM_UDRD_D,T_IMFM_UDRD_D&>m_imfm;
};

#endif
