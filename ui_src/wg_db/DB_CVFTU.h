#ifndef __DB_CVFTU_DB_H__
#define __DB_CVFTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CVFTU
{
public:
	CDB_CVFTU()
	{
		m_cvft.InitHashTable(HASHSIZECVFT);
	}
	virtual ~CDB_CVFTU(){};

public:
	void Add(T_UDRD_KEY Key,T_CVFT_UDRD_D& rData)
		{m_cvft.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cvft.RemoveKey(Key);}
	void DelAll()
		{m_cvft.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CVFT_UDRD_D& rData)
		{return m_cvft.Lookup(Key,rData);}
	int GetCount()
		{return m_cvft.GetCount();}
	POSITION GetStart()
		{return m_cvft.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CVFT_UDRD_D& rData)
		{m_cvft.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CVFT_UDRD_D,T_CVFT_UDRD_D&>m_cvft;
};

#endif