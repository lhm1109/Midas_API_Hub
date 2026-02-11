#ifndef __DB_FPSW_DB_H__
#define __DB_FPSW_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_FPSW
{
public:
	CDB_FPSW();
	virtual ~CDB_FPSW();
	CDBDoc* m_pDoc;

public:
	void Add(T_FPSW_K Key,T_FPSW_D& rData);
	BOOL Del(T_FPSW_K Key);

public:
	BOOL Get(T_FPSW_K Key,T_FPSW_D& rData);
		//{return m_fpsw.Lookup(Key,rData);}
	int GetCount();
		//{return m_fpsw.GetCount();}
	POSITION GetStart();
		//{return m_fpsw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_FPSW_K& rKey,T_FPSW_D& rData);
	//{m_fpsw.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	CMap<T_FPSW_K,T_FPSW_K,T_FPSW_D,T_FPSW_D&>m_fpsw;
};

#endif