#ifndef __DB_UIMGU_DB_H__
#define __DB_UIMGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_UIMGU
{
public:
	CDB_UIMGU()
	{
		m_uimg.InitHashTable(HASHSIZEUIMG);
	}
	virtual ~CDB_UIMGU(){};

public:
	void Add(T_UDRD_KEY Key,T_UIMG_UDRD_D& rData)
		{m_uimg.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_uimg.RemoveKey(Key);}
	void DelAll()
		{m_uimg.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_UIMG_UDRD_D& rData)
		{return m_uimg.Lookup(Key,rData);}
	int GetCount()
		{return m_uimg.GetCount();}
	POSITION GetStart()
		{return m_uimg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_UIMG_UDRD_D& rData)
		{m_uimg.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_UIMG_UDRD_D,T_UIMG_UDRD_D&>m_uimg;
};

#endif