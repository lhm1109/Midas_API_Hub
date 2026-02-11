#include "stdafx.h"
#include "DB_GRUP.h"

#include "DB_CRGR.h"
#include "DBDoc.h"
#include "StagCtrl.h"

CDB_CRGR::CDB_CRGR()
{
	m_crgr.InitHashTable(HASHSIZECRGR);
	m_pDoc = CDBDoc::GetDocPoint();
}

CDB_CRGR::~CDB_CRGR()
{
}

void CDB_CRGR::Add(T_CRGR_D& rData,CDB_GRUP* pGrup)
{
	T_CRGR_D Data;	
	BOOL bExist=m_crgr.Lookup(1, Data);

	CMap<T_GRUP_K, T_GRUP_K, T_GRUP_K, T_GRUP_K>mapGrupNew;
	CMap<T_GRUP_K, T_GRUP_K, T_GRUP_K, T_GRUP_K>mapGrupOld;

	T_GRUP_K KeyGrupTemp;

	mapGrupNew.InitHashTable(HASHSIZEGRUP);

	for(int i = 0; i<rData.aSelectedGroup.GetSize(); i++)
		mapGrupNew.SetAt(rData.aSelectedGroup[i], rData.aSelectedGroup[i]);

	if(bExist)
	{
		mapGrupOld.InitHashTable(HASHSIZEGRUP);

		for(int i = 0; i<Data.aSelectedGroup.GetSize(); i++)
			mapGrupOld.SetAt(Data.aSelectedGroup[i], Data.aSelectedGroup[i]);

		POSITION pos;
		T_GRUP_K GrupKey;
		pos = mapGrupOld.GetStartPosition();
		while(pos)
		{
			mapGrupOld.GetNextAssoc(pos, GrupKey, GrupKey);
			if(!mapGrupNew.Lookup(GrupKey, KeyGrupTemp))
				VERIFY(pGrup->DelListItem(GrupKey, LT_CRGR_CMD, 1));
		}
	}

	m_crgr.SetAt(1, rData);

	POSITION pos;
	T_GRUP_K GrupKey;
	pos = mapGrupNew.GetStartPosition();
	while (pos)
	{
		mapGrupNew.GetNextAssoc(pos, GrupKey, GrupKey);
		if(!mapGrupOld.Lookup(GrupKey, KeyGrupTemp))		// old list에 없었던 것만 Add한다...
			pGrup->AddListItem(GrupKey, LT_CRGR_CMD, 1);
	}
}

BOOL CDB_CRGR::Del(CDB_GRUP* pGrup)
{
	T_CRGR_D Data;
	BOOL bExist=m_crgr.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_crgr.RemoveKey(1);
		ASSERT(ret);
		if(ret)
		{
			CMap<T_GRUP_K, T_GRUP_K, T_GRUP_K, T_GRUP_K>mapGrupOld;
			for(int i=0; i<Data.aSelectedGroup.GetSize(); i++)
				mapGrupOld.SetAt(Data.aSelectedGroup[i], Data.aSelectedGroup[i]);

			POSITION pos;
			T_GRUP_K GrupKey;
			pos = mapGrupOld.GetStartPosition();
			while(pos)
			{
				mapGrupOld.GetNextAssoc(pos, GrupKey, GrupKey);
				VERIFY(pGrup->DelListItem(GrupKey, LT_CRGR_CMD, 1));
			}
		}
		return ret;
	}
	return bExist;
}

//-------------------------------------------------------------------------
BOOL CDB_CRGR::Get(T_CRGR_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_crgr.Lookup(1,rData);
}

int CDB_CRGR::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_crgr.GetCount();
}
