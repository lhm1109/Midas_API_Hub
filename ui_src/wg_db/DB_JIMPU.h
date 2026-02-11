#ifndef __DB_JIMPU_DB_H__ 
#define __DB_JIMPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_JIMPU
{
public:
	CDB_JIMPU()
	{
		m_jimp.InitHashTable(HASHSIZEJIMP);
	}
	virtual ~CDB_JIMPU(){};

public:
	void Add(T_UDRD_KEY Key,T_JIMP_UDRD_D& rData)
		{m_jimp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_jimp.RemoveKey(Key);}
	void DelAll()
		{m_jimp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JIMP_UDRD_D& rData)
		{return m_jimp.Lookup(Key,rData);}
	int GetCount()
		{return m_jimp.GetCount();}
	POSITION GetStart()
		{return m_jimp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_JIMP_UDRD_D& rData)
		{m_jimp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JIMP_UDRD_D,T_JIMP_UDRD_D&>m_jimp;
};

#endif