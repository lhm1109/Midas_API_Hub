#ifndef __DB_RPSCU_DB_H__
#define __DB_RPSCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RPSCU
{
public:
	CDB_RPSCU()
	{
		m_rpsc.InitHashTable(HASHSIZERPSC);
		m_rpscDesign.InitHashTable(HASHSIZERPSC);
	}
	virtual ~CDB_RPSCU(){};

public:
	// rpsc
	void Add(T_UDRD_KEY Key,T_RPSC_UDRD_D& rData)
		{m_rpsc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rpsc.RemoveKey(Key);}
	void DelAll()
		{m_rpsc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RPSC_UDRD_D& rData)
		{return m_rpsc.Lookup(Key,rData);}
	int GetCount()
		{return m_rpsc.GetCount();}
	POSITION GetStart()
		{return m_rpsc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RPSC_UDRD_D& rData)
		{m_rpsc.GetNextAssoc(rNextPosition,rKey,rData);}

	// rpsc design
	void AddDesign(T_UDRD_KEY Key,T_RPSC_UDRD_D& rData)
		{m_rpscDesign.SetAt(Key,rData);}
	BOOL DelDesign(T_UDRD_KEY Key)
		{return m_rpscDesign.RemoveKey(Key);}
	void DelAllDesign()
		{m_rpscDesign.RemoveAll();}
	BOOL GetDesign(T_UDRD_KEY Key,T_RPSC_UDRD_D& rData)
		{return m_rpscDesign.Lookup(Key,rData);}
	int GetCountDesign()
		{return m_rpscDesign.GetCount();}
	POSITION GetStartDesign()
		{return m_rpscDesign.GetStartPosition();}
	void GetNextDesign(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RPSC_UDRD_D& rData)
		{m_rpscDesign.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RPSC_UDRD_D,T_RPSC_UDRD_D&>m_rpsc;
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RPSC_UDRD_D,T_RPSC_UDRD_D&>m_rpscDesign;
};

#endif
