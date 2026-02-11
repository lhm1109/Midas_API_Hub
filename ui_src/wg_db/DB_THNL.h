#ifndef __DB_THNL_DB_H__
#define __DB_THNL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_THIS;
class CDB_NODE;

class CDB_THNL
{
public:
	CDB_THNL();
	virtual ~CDB_THNL();
	CDBDoc* m_pDoc;

public:
	void Add(T_THNL_K Key,T_THNL_D& rData,CDB_THIS* pThis,CDB_NODE* pNode);
	BOOL Del(T_THNL_K Key,CDB_THIS* pThis,CDB_NODE* pNode);

public:
	BOOL Get(T_THNL_K Key,T_THNL_D& rData);
		//{return m_thnl.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_thnl.GetCount();}
	POSITION GetStart();
		//{return m_thnl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_THNL_K& rKey,T_THNL_D& rData);
		//{m_thnl.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_THNL_KEY,T_THNL_KEY,T_THNL_D,T_THNL_D&>m_thnl;
};

#endif