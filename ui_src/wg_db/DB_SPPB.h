//sppb
#ifndef __DB_SPPB_DB_H__
#define __DB_SPPB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SPPB
{
public:
	CDB_SPPB();
	virtual ~CDB_SPPB();
	CDBDoc* m_pDoc;

public:
	void Add(T_SPPB_K Key, T_SPPB_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_SPPB_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_SPPB_K Key, T_SPPB_D& rData);
	//{return m_Sppb.Lookup(Key,rData);}
	int GetCount();
	//{return m_Sppb.GetCount();}
	POSITION GetStart();
	//{return m_Sppb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_SPPB_K& rKey, T_SPPB_D& rData);
	//{m_Sppb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SPPB_K, T_SPPB_K, T_SPPB_D, T_SPPB_D&> m_Sppb;
};

#endif