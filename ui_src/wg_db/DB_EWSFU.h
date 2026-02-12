#ifndef __DB_EWSFU_DB_H__
#define __DB_EWSFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EWSFU
{
public:
	CDB_EWSFU()
	{
		m_ewsf.InitHashTable(HASHSIZEEWSF);
		m_ewsfDesign.InitHashTable(HASHSIZEEWSF);
	}
	virtual ~CDB_EWSFU(){};

public:
	///// ewsf
	void Add(T_UDRD_KEY Key,T_EWSF_UDRD_D& rData)
		{m_ewsf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ewsf.RemoveKey(Key);}
	void DelAll()
		{m_ewsf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_EWSF_UDRD_D& rData)
		{return m_ewsf.Lookup(Key,rData);}
	int GetCount()
		{return m_ewsf.GetCount();}
	POSITION GetStart()
		{return m_ewsf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_EWSF_UDRD_D& rData)
		{m_ewsf.GetNextAssoc(rNextPosition,rKey,rData);}

	///// ewsd
	void AddDesign(T_UDRD_KEY Key,T_EWSF_UDRD_D& rData)
		{m_ewsfDesign.SetAt(Key,rData);}
	BOOL DelDesign(T_UDRD_KEY Key)
		{return m_ewsfDesign.RemoveKey(Key);}
	void DelAllDesign()
		{m_ewsfDesign.RemoveAll();}
	BOOL GetDesign(T_UDRD_KEY Key,T_EWSF_UDRD_D& rData)
		{return m_ewsfDesign.Lookup(Key,rData);}
	int GetCountDesign()
		{return m_ewsfDesign.GetCount();}
	POSITION GetStartDesign()
		{return m_ewsfDesign.GetStartPosition();}
	void GetNextDesign(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_EWSF_UDRD_D& rData)
		{m_ewsfDesign.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_EWSF_UDRD_D,T_EWSF_UDRD_D&>m_ewsf;
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_EWSF_UDRD_D,T_EWSF_UDRD_D&>m_ewsfDesign;
};

#endif