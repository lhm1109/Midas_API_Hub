#include "stdafx.h"
#include "DB_GRUP.h"
#include "DB_BNGR.h"
#include "DB_LDGR.h"
#include "DB_HSTG.h"

CDB_HSTG::CDB_HSTG()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_nLastStag = 0;
	m_hstg.InitHashTable(HASHSIZEHSTG);
}

CDB_HSTG::~CDB_HSTG()
{
}

void CDB_HSTG::Add(T_HSTG_K Key,T_HSTG_D& rData,CDB_GRUP* pGrup,CDB_BNGR* pBngr,CDB_LDGR* pLdgr)
{
	T_HSTG_D Data;
	BOOL bExist=m_hstg.Lookup(Key, Data);
	CMap<T_GRUP_K, T_GRUP_K, T_GRUP_K, T_GRUP_K>mapGrupOld;
	CMap<T_GRUP_K, T_GRUP_K, T_GRUP_K, T_GRUP_K>mapGrupNew;
	CMap<T_BNGR_K, T_BNGR_K, T_BNGR_K, T_BNGR_K>mapBngrOld;
	CMap<T_BNGR_K, T_BNGR_K, T_BNGR_K, T_BNGR_K>mapBngrNew;
	CMap<T_LDGR_K, T_LDGR_K, T_LDGR_K, T_LDGR_K>mapLdgrOld;
	CMap<T_LDGR_K, T_LDGR_K, T_LDGR_K, T_LDGR_K>mapLdgrNew;
	T_GRUP_K KeyGrupTemp;
	T_BNGR_K KeyBngrTemp;
	T_LDGR_K KeyLdgrTemp;

	mapGrupNew.InitHashTable(HASHSIZEGRUP);
	mapBngrNew.InitHashTable(HASHSIZEBNGR);
	mapLdgrNew.InitHashTable(HASHSIZELDGR);
	for(int i = 0; i<rData.aActiveElem.GetSize(); i++)mapGrupNew.SetAt(rData.aActiveElem[i], rData.aActiveElem[i]);
	for(int i = 0; i<rData.aActiveBndr.GetSize(); i++)mapBngrNew.SetAt(rData.aActiveBndr[i], rData.aActiveBndr[i]);
	for(int i = 0; i<rData.aDeactiveBndr.GetSize(); i++)mapBngrNew.SetAt(rData.aDeactiveBndr[i], rData.aDeactiveBndr[i]);
	for(int i = 0; i<rData.aActiveLoad.GetSize(); i++)mapLdgrNew.SetAt(rData.aActiveLoad[i].LdgrKey, rData.aActiveLoad[i].LdgrKey);
	for(int i = 0; i<rData.aDeactiveLoad.GetSize(); i++)mapLdgrNew.SetAt(rData.aDeactiveLoad[i].LdgrKey, rData.aDeactiveLoad[i].LdgrKey);

	if(bExist)
	{
		mapGrupOld.InitHashTable(HASHSIZEGRUP);
		mapBngrOld.InitHashTable(HASHSIZEBNGR);
		mapLdgrOld.InitHashTable(HASHSIZELDGR);
		for(int i = 0; i<Data.aActiveElem.GetSize(); i++)mapGrupOld.SetAt(Data.aActiveElem[i], Data.aActiveElem[i]);
		for(int i = 0; i<Data.aActiveBndr.GetSize(); i++)mapBngrOld.SetAt(Data.aActiveBndr[i], Data.aActiveBndr[i]);
		for(int i = 0; i<Data.aDeactiveBndr.GetSize(); i++)mapBngrOld.SetAt(Data.aDeactiveBndr[i], Data.aDeactiveBndr[i]);
		for(int i = 0; i<Data.aActiveLoad.GetSize(); i++)mapLdgrOld.SetAt(Data.aActiveLoad[i].LdgrKey, Data.aActiveLoad[i].LdgrKey);
		for(int i = 0; i<Data.aDeactiveLoad.GetSize(); i++)mapLdgrOld.SetAt(Data.aDeactiveLoad[i].LdgrKey, Data.aDeactiveLoad[i].LdgrKey);

		POSITION pos;
		T_GRUP_K GrupKey;
		pos = mapGrupOld.GetStartPosition();
		while(pos)
		{
			mapGrupOld.GetNextAssoc(pos, GrupKey, GrupKey);
			if(!mapGrupNew.Lookup(GrupKey, KeyGrupTemp))
				VERIFY(pGrup->DelListItem(GrupKey, LT_HSTG_CMD, Key));
		}
		T_BNGR_K BngrKey;
		pos = mapBngrOld.GetStartPosition();
		while(pos)
		{
			mapBngrOld.GetNextAssoc(pos, BngrKey, BngrKey);
			if(!mapBngrNew.Lookup(BngrKey, KeyBngrTemp))
				VERIFY(pBngr->DelListItem(BngrKey, LT_HSTG_CMD, Key));
		}
		T_LDGR_K LdgrKey;
		pos = mapLdgrOld.GetStartPosition();
		while(pos)
		{
			mapLdgrOld.GetNextAssoc(pos, LdgrKey, LdgrKey);
			if(!mapLdgrNew.Lookup(LdgrKey, KeyLdgrTemp))
				VERIFY(pLdgr->DelListItem(LdgrKey, LT_HSTG_CMD, Key));
		}
	}
	else
	{
		T_HSTG_K key;
		T_HSTG_D data;
		POSITION pos = GetStart();
		while (pos)
		{
			GetNext(pos, key, data);
			if (data.nStageId >= rData.nStageId) 
			{
				data.nStageId++;
				m_hstg.SetAt(key, data);
			}
		}
	}
	m_hstg.SetAt(Key,rData);

	if(Key == m_nStartNum)
	{
		T_HSTG_D data;
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;

	POSITION pos;
	T_GRUP_K GrupKey;
	pos = mapGrupNew.GetStartPosition();
	while (pos)
	{
		mapGrupNew.GetNextAssoc(pos, GrupKey, GrupKey);
		if (bExist)
		{
			if (!mapGrupOld.Lookup(GrupKey, KeyGrupTemp))
				pGrup->AddListItem(GrupKey, LT_HSTG_CMD, Key);
		}
		else
			pGrup->AddListItem(GrupKey, LT_HSTG_CMD, Key);
	}
	T_BNGR_K BngrKey;
	pos = mapBngrNew.GetStartPosition();
	while (pos)
	{
		mapBngrNew.GetNextAssoc(pos, BngrKey, BngrKey);
		if (bExist)
		{
			if (!mapBngrOld.Lookup(BngrKey, KeyBngrTemp))
				pBngr->AddListItem(BngrKey, LT_HSTG_CMD, Key);
		}
		else
			pBngr->AddListItem(BngrKey, LT_HSTG_CMD, Key);
	}
	T_LDGR_K LdgrKey;
	pos = mapLdgrNew.GetStartPosition();
	while (pos)
	{
		mapLdgrNew.GetNextAssoc(pos, LdgrKey, LdgrKey);
		if (bExist)
		{
			if (!mapLdgrOld.Lookup(LdgrKey, KeyLdgrTemp))
				pLdgr->AddListItem(LdgrKey, LT_HSTG_CMD, Key);
		}
		else
			pLdgr->AddListItem(LdgrKey, LT_HSTG_CMD, Key);
	}

	// Last Stage 유지
	if (rData.nStageId == m_hstg.GetCount()) m_nLastStag = Key;
}

BOOL CDB_HSTG::Del(T_HSTG_K Key,CDB_GRUP* pGrup,CDB_BNGR* pBngr,CDB_LDGR* pLdgr)
{
	T_HSTG_D Data;
	BOOL bExist=m_hstg.Lookup(Key, Data);
	if(bExist)
	{
		BOOL ret=m_hstg.RemoveKey(Key);
		ASSERT(ret);
		if(ret)
		{
			CMap<T_GRUP_K, T_GRUP_K, T_GRUP_K, T_GRUP_K>mapGrupOld;
			CMap<T_BNGR_K, T_BNGR_K, T_BNGR_K, T_BNGR_K>mapBngrOld;
			CMap<T_LDGR_K, T_LDGR_K, T_LDGR_K, T_LDGR_K>mapLdgrOld;
			for(int i = 0; i<Data.aActiveElem.GetSize(); i++)mapGrupOld.SetAt(Data.aActiveElem[i], Data.aActiveElem[i]);
			for(int i = 0; i<Data.aActiveBndr.GetSize(); i++)mapBngrOld.SetAt(Data.aActiveBndr[i], Data.aActiveBndr[i]);
			for(int i = 0; i<Data.aDeactiveBndr.GetSize(); i++)mapBngrOld.SetAt(Data.aDeactiveBndr[i], Data.aDeactiveBndr[i]);
			for(int i = 0; i<Data.aActiveLoad.GetSize(); i++)mapLdgrOld.SetAt(Data.aActiveLoad[i].LdgrKey, Data.aActiveLoad[i].LdgrKey);
			for(int i = 0; i<Data.aDeactiveLoad.GetSize(); i++)mapLdgrOld.SetAt(Data.aDeactiveLoad[i].LdgrKey, Data.aDeactiveLoad[i].LdgrKey);

			POSITION pos;
			T_GRUP_K GrupKey;
			pos = mapGrupOld.GetStartPosition();
			while(pos)
			{
				mapGrupOld.GetNextAssoc(pos, GrupKey, GrupKey);
				VERIFY(pGrup->DelListItem(GrupKey, LT_HSTG_CMD, Key));
			}
			T_BNGR_K BngrKey;
			pos = mapBngrOld.GetStartPosition();
			while(pos)
			{
				mapBngrOld.GetNextAssoc(pos, BngrKey, BngrKey);
				VERIFY(pBngr->DelListItem(BngrKey, LT_HSTG_CMD, Key));
			}
			T_LDGR_K LdgrKey;
			pos = mapLdgrOld.GetStartPosition();
			while(pos)
			{
				mapLdgrOld.GetNextAssoc(pos, LdgrKey, LdgrKey);
				VERIFY(pLdgr->DelListItem(LdgrKey, LT_HSTG_CMD, Key));
			}

			if(Key < m_nStartNum)m_nStartNum=Key;
			if(Key==m_nLastNum)
			{
				T_HSTG_K key;
				T_HSTG_D data;
				if(Get(m_nLastNum-1,data))
				{
					m_nLastNum--;
				}
				else
				{
					m_nLastNum=0;
					POSITION pos=GetStart();
					while(pos != NULL)
					{
						GetNext(pos,key,data);
						if(key > m_nLastNum)m_nLastNum=key;
					}
				}
			}

			// Last Stage 유지(1)
			BOOL bLastStagDeleted = (Key == m_nLastStag);
			int nStagNum = m_hstg.GetCount();
			T_HSTG_K LastStag;

			// Hstg ID 유지
			T_HSTG_K key;
			T_HSTG_D data;
			pos = GetStart();
			while (pos)
			{
				GetNext(pos, key, data);
				if (data.nStageId > Data.nStageId)
				{
					data.nStageId--;
					m_hstg.SetAt(key, data);
				}
				// Last Stage 유지(2)
				if (bLastStagDeleted && data.nStageId == nStagNum) LastStag = key;
			}
			// Last Stage 유지(3)
			if (bLastStagDeleted)
			{
				if (m_hstg.GetCount() == 0) m_nLastStag = 0;
				else m_nLastStag = LastStag;
			}
		}
		return ret;
	}
	return bExist;
}

