#ifndef __DB_EWSCU_DB_H__
#define __DB_EWSCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EWSCU
{
public:
	CDB_EWSCU()
	{
		m_ewsc.InitHashTable(HASHSIZEEWSC);
		m_ewscDesign.InitHashTable(HASHSIZEEWSC);
	}
	virtual ~CDB_EWSCU(){};

public:
	///// ewsc
	void Add(T_UDRD_KEY Key,T_EWSC_UDRD_D& rData)
	{m_ewsc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_ewsc.RemoveKey(Key);}
	void DelAll()
	{m_ewsc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_EWSC_UDRD_D& rData)
	{return m_ewsc.Lookup(Key,rData);}
	int GetCount()
	{return m_ewsc.GetCount();}
	POSITION GetStart()
	{return m_ewsc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_EWSC_UDRD_D& rData)
	{m_ewsc.GetNextAssoc(rNextPosition,rKey,rData);}

	///// ewcd
	void AddDesign(T_UDRD_KEY Key,T_EWSC_UDRD_D& rData)
	{m_ewscDesign.SetAt(Key,rData);}
	BOOL DelDesign(T_UDRD_KEY Key)
	{return m_ewscDesign.RemoveKey(Key);}
	void DelAllDesign()
	{m_ewscDesign.RemoveAll();}
	BOOL GetDesign(T_UDRD_KEY Key,T_EWSC_UDRD_D& rData)
	{return m_ewscDesign.Lookup(Key,rData);}
	int GetCountDesign()
	{return m_ewscDesign.GetCount();}
	POSITION GetStartDesign()
	{return m_ewscDesign.GetStartPosition();}
	void GetNextDesign(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_EWSC_UDRD_D& rData)
	{m_ewscDesign.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_EWSC_UDRD_D,T_EWSC_UDRD_D&>m_ewsc;
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_EWSC_UDRD_D,T_EWSC_UDRD_D&>m_ewscDesign;
};

#endif