#include "stdafx.h"
#include "DB_GRDP.h"
#include "DB_GRUP.h"
#include "DB_BNGR.h"

#include "DBDoc.h"
#include "StagCtrl.h"

CDB_GRDP::CDB_GRDP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_grdp.InitHashTable(HASHSIZEGRDP);
}

CDB_GRDP::~CDB_GRDP()
{

}
/*
void CDB_GRDP::Add(T_GRDP_D& rData)
{
	m_grdp.SetAt(1,rData);
}

BOOL CDB_GRDP::Del()
{
	T_GRDP_D Data;
	BOOL bExist=m_grdp.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_grdp.RemoveKey(1);
		ASSERT(ret);
		return ret;
	}
	return bExist;
}
*/
void CDB_GRDP::Add(T_GRDP_D& rData, CDB_GRUP* pGrup, CDB_BNGR* pBngr)
{
	T_GRDP_D Data;
	BOOL bExist=m_grdp.Lookup(1, Data);
	CMap<T_GRUP_K, T_GRUP_K, T_GRUP_K, T_GRUP_K>mapGrupOld;
	CMap<T_GRUP_K, T_GRUP_K, T_GRUP_K, T_GRUP_K>mapGrupNew;
	CMap<T_BNGR_K, T_BNGR_K, T_BNGR_K, T_BNGR_K>mapBngrOld;
	CMap<T_BNGR_K, T_BNGR_K, T_BNGR_K, T_BNGR_K>mapBngrNew;
	T_GRUP_K KeyGrupTemp;
	T_BNGR_K KeyBngrTemp;
	
	mapGrupNew.InitHashTable(HASHSIZEGRUP);
	mapBngrNew.InitHashTable(HASHSIZEBNGR);
	
	for(int i = 0; i<rData.aGrupDamping.GetSize(); i++)
	{
		if(rData.aGrupDamping[i].nGrupType == 0)
			mapGrupNew.SetAt(rData.aGrupDamping[i].GrupBngrKey, rData.aGrupDamping[i].GrupBngrKey);
	}
	for(int i = 0; i<rData.aGrupDamping.GetSize(); i++)
	{
		if(rData.aGrupDamping[i].nGrupType == 1)
			mapBngrNew.SetAt(rData.aGrupDamping[i].GrupBngrKey, rData.aGrupDamping[i].GrupBngrKey);
	}  
	
	if(bExist)
	{
		mapGrupOld.InitHashTable(HASHSIZEGRUP);
		mapBngrOld.InitHashTable(HASHSIZEBNGR);
		for(int i = 0; i<Data.aGrupDamping.GetSize(); i++)
		{
			if(Data.aGrupDamping[i].nGrupType == 0)
				mapGrupOld.SetAt(Data.aGrupDamping[i].GrupBngrKey, Data.aGrupDamping[i].GrupBngrKey);
		}
		for(int i = 0; i<Data.aGrupDamping.GetSize(); i++)
		{
			if(Data.aGrupDamping[i].nGrupType == 1)
				mapBngrOld.SetAt(Data.aGrupDamping[i].GrupBngrKey, Data.aGrupDamping[i].GrupBngrKey);
		}
		
		POSITION pos;
		T_GRUP_K GrupKey;
		pos = mapGrupOld.GetStartPosition();
		while(pos)
		{   
			mapGrupOld.GetNextAssoc(pos, GrupKey, GrupKey);
			if(!mapGrupNew.Lookup(GrupKey, KeyGrupTemp))
				VERIFY(pGrup->DelListItem(GrupKey, LT_GRDP_CMD, 1));      
		}
		T_BNGR_K BngrKey;
		pos = mapBngrOld.GetStartPosition();
		while(pos)
		{
			mapBngrOld.GetNextAssoc(pos, BngrKey, BngrKey);
			if(!mapBngrNew.Lookup(BngrKey, KeyBngrTemp))
				VERIFY(pBngr->DelListItem(BngrKey, LT_GRDP_CMD, 1));
		}
	}
	
	m_grdp.SetAt(1, rData);
	
	POSITION pos;
	T_GRUP_K GrupKey;
	pos = mapGrupNew.GetStartPosition();
	while (pos)
	{
		mapGrupNew.GetNextAssoc(pos, GrupKey, GrupKey);
		if (bExist)
		{
			if (!mapGrupOld.Lookup(GrupKey, KeyGrupTemp))
				pGrup->AddListItem(GrupKey, LT_GRDP_CMD, 1);
		}
		else
			pGrup->AddListItem(GrupKey, LT_GRDP_CMD, 1);
	}
	T_BNGR_K BngrKey;
	pos = mapBngrNew.GetStartPosition();
	while (pos)
	{
		mapBngrNew.GetNextAssoc(pos, BngrKey, BngrKey);
		if (bExist)
		{
			if (!mapBngrOld.Lookup(BngrKey, KeyBngrTemp))
				pBngr->AddListItem(BngrKey, LT_GRDP_CMD, 1);
		}
		else
			pBngr->AddListItem(BngrKey, LT_GRDP_CMD, 1);
	}
}

BOOL CDB_GRDP::Del(CDB_GRUP* pGrup, CDB_BNGR* pBngr)
{
	T_GRDP_D Data;
	BOOL bExist=m_grdp.Lookup(1, Data);
	if(bExist)
	{
		BOOL ret=m_grdp.RemoveKey(1);
		ASSERT(ret);
		if(ret)
		{
			CMap<T_GRUP_K, T_GRUP_K, T_GRUP_K, T_GRUP_K>mapGrupOld;
			CMap<T_BNGR_K, T_BNGR_K, T_BNGR_K, T_BNGR_K>mapBngrOld;      
			for(int i = 0; i<Data.aGrupDamping.GetSize(); i++) 
			{
				if(Data.aGrupDamping[i].nGrupType == 0)
					mapGrupOld.SetAt(Data.aGrupDamping[i].GrupBngrKey, Data.aGrupDamping[i].GrupBngrKey);      
			}
			for(int i = 0; i<Data.aGrupDamping.GetSize(); i++) 
			{
				if(Data.aGrupDamping[i].nGrupType == 1)
				mapBngrOld.SetAt(Data.aGrupDamping[i].GrupBngrKey, Data.aGrupDamping[i].GrupBngrKey);
			}
			
			POSITION pos;
			T_GRUP_K GrupKey;
			pos = mapGrupOld.GetStartPosition();
			while(pos)
			{
				mapGrupOld.GetNextAssoc(pos, GrupKey, GrupKey);
				VERIFY(pGrup->DelListItem(GrupKey, LT_GRDP_CMD, 1));
			}
			T_BNGR_K BngrKey;
			pos = mapBngrOld.GetStartPosition();
			while(pos)
			{
				mapBngrOld.GetNextAssoc(pos, BngrKey, BngrKey);
				VERIFY(pBngr->DelListItem(BngrKey, LT_GRDP_CMD, 1));
			}      
		}
		return ret;
	}
	return bExist;
}
//-------------------------------------------------------------------------
BOOL CDB_GRDP::Get(T_GRDP_D& rData)
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return FALSE;
	return m_grdp.Lookup(1,rData);
}

int CDB_GRDP::GetCount()
{
	if (m_pDoc && m_pDoc->m_pStagCtrl->GetCurStag() != 0 && !m_pDoc->m_pStagCtrl->IsFinalStag())
		return 0;
	return m_grdp.GetCount();
}
