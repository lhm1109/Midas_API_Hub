#ifndef __DB_OCDFU_DB_H__
#define __DB_OCDFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_OCDFU
{
public:
	CDB_OCDFU()
	{
		m_ocdf.InitHashTable(HASHSIZEOCDF);
	}
	virtual ~CDB_OCDFU(){};

public:
	void Add(T_UDRD_KEY Key,T_OCDF_UDRD_D& rData)
		{m_ocdf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ocdf.RemoveKey(Key);}
	void DelAll()
		{m_ocdf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_OCDF_UDRD_D& rData)
		{return m_ocdf.Lookup(Key,rData);}
	int GetCount()
		{return m_ocdf.GetCount();}
	POSITION GetStart()
		{return m_ocdf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_OCDF_UDRD_D& rData)
		{m_ocdf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_OCDF_UDRD_D,T_OCDF_UDRD_D&>m_ocdf;
};

#endif