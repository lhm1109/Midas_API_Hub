#ifndef __DB_DMAS_DB_H__
#define __DB_DMAS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STOR;

class CDB_DMAS
{
public:
	CDB_DMAS();
	virtual ~CDB_DMAS();
	CDBDoc* m_pDoc;

public:
	void Add(T_DMAS_K Key,T_DMAS_D& rData,CDB_STOR* pStor);
	BOOL Del(T_DMAS_K Key,CDB_STOR* pStor);

public:
	BOOL Get(T_DMAS_K Key,T_DMAS_D& rData);
		//{return m_dmas.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_dmas.G;etCount();}
	POSITION GetStart();
		//{return m_dmas.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DMAS_K& rKey,T_DMAS_D& rData);
		//{m_dmas.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_DMAS_KEY,T_DMAS_KEY,T_DMAS_D,T_DMAS_D&>m_dmas;
};

#endif
