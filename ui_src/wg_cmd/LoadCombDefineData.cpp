#include "stdafx.h"
#include "LoadCombDefineData.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\wg_base_CompFunc.h"

void LoadFactorTable::initial()
{
	m_aFactor.RemoveAll();
	m_aType.RemoveAll();
}

CLoadCombDefineData::CLoadCombDefineData()
{
	initial();
}

void CLoadCombDefineData::initial()
{
	m_bImperfection  = FALSE;
	m_bWindForward = FALSE;
	m_strCode		 = _T("");
	m_nStage		 = 0;
	m_bBiEarthquake  = FALSE;
	m_mapBiEarthquake.clear();
	m_dCombFactor[0] = 0.7;
	m_dCombFactor[1] = 0.6;
	m_dCombFactor[2] = 0.6;
	m_mapCombFactor.clear();
	m_dLLAdjustCombFactor = 1.0;
	m_mapSpecialFactor.clear();
	m_item.initial();
	m_dLLGravityFactor = 0.5;
	m_mapGravityFactor.clear();
	m_nSpecitlArrange = 0;
	m_aLLUnfavorable.RemoveAll();
	m_mapExclusion.clear();
	m_mapBC2C.clear();
}

ILoadCombDefineMgr *ILoadCombDefineMgr::Instance()
{
	static ILoadCombDefineMgr g_ILoadCombDefineMgr;
	return &g_ILoadCombDefineMgr;
}

ILoadCombDefineMgr::ILoadCombDefineMgr()
{
	m_pData = new CLoadCombDefineData();
}

ILoadCombDefineMgr::~ILoadCombDefineMgr()
{
	if (m_pData != NULL)
		delete m_pData;
	
	m_pData = NULL;
}

CLoadCombDefineData* ILoadCombDefineMgr::Para()
{
	//m_pData->initial();
	return m_pData;
}

void ILoadCombDefineMgr::getCodeData(int nIndex, LoadFactorTable &TableItem, int &nRow, int &nCol)
{
	TableItem.initial();
	TableItem.m_aType.Add(_T("D")  );
	TableItem.m_aType.Add(_T("L")  );
	TableItem.m_aType.Add(_T("LR")  );
	TableItem.m_aType.Add(_T("W")  );
	TableItem.m_aType.Add(_T("E")  );
	TableItem.m_aType.Add(_T("EVT")) ;
	TableItem.m_aType.Add(_T("S")  );
	TableItem.m_aType.Add(_T("T")  );
	if (nIndex==0 || nIndex==3 || nIndex==6 || nIndex==8 || nIndex == 9)	//GB 50017-17 || GB/T50010-10 || GB50018-02 || GB50429-07 || GB55006-2021
	{
		TableItem.m_aType.Add(_T("PS"));
		nCol = 9;
		nRow = 12;
		for (int k=0; k<12; k++)
		{
			CArrayEx<double, double> arFactor;
			arFactor.SetSize(9);
			for (int i=0; i<9; i++)
			{
				arFactor[i] = CoefGB17_17[k][i];
			}
			TableItem.m_aFactor.Add(arFactor);
		}
	
	}
	else if (nIndex==1 || nIndex==4)	//JGJ99-2015 || JGJ3-2010
	{
		TableItem.m_aType.Add(_T("PS"));
		nCol = 9;
		nRow = 20;
		for (int k=0; k<20; k++)
		{
			CArrayEx<double, double> arFactor;
			arFactor.SetSize(9);
			for (int i=0; i<9; i++)
			{
				arFactor[i] = CoefJGJ[k][i];
			}
			TableItem.m_aFactor.Add(arFactor);
		}
	}
	else if (nIndex==2 || nIndex==5)	// GB50017-03 || GB50010-02
	{
		nCol = 8;
		nRow = 13;
		for (int k=0; k<13; k++)
		{
			CArrayEx<double, double> arFactor;
			arFactor.SetSize(8);
			for (int i=0; i<8; i++)
			{
				arFactor[i] = CoefGB17_03[k][i];
			}
			TableItem.m_aFactor.Add(arFactor);
		}
	}
	else if (nIndex==7)	//GB50069-02
	{
		TableItem.m_aType.Add(_T("EH")  );
		TableItem.m_aType.Add(_T("EV")  );
		TableItem.m_aType.Add(_T("WP")  );
		TableItem.m_aType.Add(_T("FP")  );
		TableItem.m_aType.Add(_T("SF")  );
		TableItem.m_aType.Add(_T("B")  );
		TableItem.m_aType.Add(_T("RS"));

		nCol = 15;
		nRow = 4;	
		for (int k=0; k<4; k++)
		{
			CArrayEx<double, double> arFactor;
			arFactor.SetSize(15);
			for (int i=0; i<15; i++)
			{
				arFactor[i] = CoefGB69_02[k][i];
			}
			TableItem.m_aFactor.Add(arFactor);
		}		
	}
	TableItem.m_aType.Add(_T("RS"));
	TableItem.m_aType.Add(_T("COMB"));
}

BOOL ILoadCombDefineMgr::GetUsedKeyList(BOOL bSeismicEvaluation, CArray<T_STLD_K, T_STLD_K> &arKeyList)
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	POSITION pos;
	T_STLD_K StldK;

	CMap<T_STLD_K, T_STLD_K, BOOL, BOOL> mStldK;
	// Self Weight
	pos = pDoc->m_pAttrCtrl->GetStartBodf();
	while (pos)
	{
		T_BODF_D BodfD;
		pDoc->m_pAttrCtrl->GetNextBodf(pos, StldK, BodfD);
		if (pDoc->m_pAttrCtrl->ExistStld(BodfD.LoadCaseKey)) mStldK.SetAt(BodfD.LoadCaseKey, TRUE);
	}
	// Nodal Load
	pos = pDoc->m_pAttrCtrl->GetStartCnld();
	while (pos)
	{
		T_CNLD_K CnldK; T_CNLD_D CnldD;
		pDoc->m_pAttrCtrl->GetNextCnld(pos, CnldK, CnldD);
		if (pDoc->m_pAttrCtrl->ExistStld(CnldD.LoadCaseKey)) mStldK.SetAt(CnldD.LoadCaseKey, TRUE);
	}
	// Add by GAY.('06.04.11). Nodal Body Force
	pos = pDoc->m_pAttrCtrl->GetStartNbof();
	while (pos)
	{
		T_NBOF_K NbofK; T_NBOF_D NbofD;
		pDoc->m_pAttrCtrl->GetNextNbof(pos, NbofK, NbofD);
		if (pDoc->m_pAttrCtrl->ExistStld(NbofD.LoadCaseKey)) mStldK.SetAt(NbofD.LoadCaseKey, TRUE);
	}
	// Specified Displacement
	pos = pDoc->m_pAttrCtrl->GetStartSdsp();
	while (pos)
	{
		T_SDSP_K SdspK; T_SDSP_D SdspD;
		pDoc->m_pAttrCtrl->GetNextSdsp(pos, SdspK, SdspD);
		if (pDoc->m_pAttrCtrl->ExistStld(SdspD.LoadCaseKey)) mStldK.SetAt(SdspD.LoadCaseKey, TRUE);
	}
	// Beam Load
	pos = pDoc->m_pAttrCtrl->GetStartBmld();
	while (pos)
	{
		T_BMLD_K BmldK; T_BMLD_D BmldD;
		pDoc->m_pAttrCtrl->GetNextBmld(pos, BmldK, BmldD);
		if (pDoc->m_pAttrCtrl->ExistStld(BmldD.LoadCaseKey)) mStldK.SetAt(BmldD.LoadCaseKey, TRUE);
	}
	// Typical Beam Load 
	// Floor Load
	pos = pDoc->m_pAttrCtrl->GetStartFbla();
	while (pos)
	{
		T_FBLA_K TblaK; T_FBLA_D FblaD; T_FBLD_D FbldD;
		pDoc->m_pAttrCtrl->GetNextFbla(pos, TblaK, FblaD);
		pDoc->m_pAttrCtrl->GetFbld(FblaD.LoadTypeKey, FbldD);
		if (pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[0])) mStldK.SetAt(FbldD.LoadCaseKey[0], TRUE);
		if (pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[1])) mStldK.SetAt(FbldD.LoadCaseKey[1], TRUE);
		if (pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[2])) mStldK.SetAt(FbldD.LoadCaseKey[2], TRUE);
		if (pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[3])) mStldK.SetAt(FbldD.LoadCaseKey[3], TRUE);
		if (pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[4])) mStldK.SetAt(FbldD.LoadCaseKey[4], TRUE);
		if (pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[5])) mStldK.SetAt(FbldD.LoadCaseKey[5], TRUE);
		if (pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[6])) mStldK.SetAt(FbldD.LoadCaseKey[6], TRUE);
		if (pDoc->m_pAttrCtrl->ExistStld(FbldD.LoadCaseKey[7])) mStldK.SetAt(FbldD.LoadCaseKey[7], TRUE);
	}
	// Plane Load
	pos = pDoc->m_pAttrCtrl->GetStartPnla();
	while (pos)
	{
		T_PNLA_K PlnaK; T_PNLA_D PnlaD;
		pDoc->m_pAttrCtrl->GetNextPnla(pos, PlnaK, PnlaD);
		if (pDoc->m_pAttrCtrl->ExistStld(PnlaD.LoadCaseKey)) mStldK.SetAt(PnlaD.LoadCaseKey, TRUE);
	}
	// Finishing Material Load
	pos = pDoc->m_pAttrCtrl->GetStartFmld();
	while (pos)
	{
		T_FMLD_K FmldK; T_FMLD_D FmldD;
		pDoc->m_pAttrCtrl->GetNextFmld(pos, FmldK, FmldD);
		if (pDoc->m_pAttrCtrl->ExistStld(FmldD.LoadCaseKey)) mStldK.SetAt(FmldD.LoadCaseKey, TRUE);
	}
	// Prestress Beam Load
	pos = pDoc->m_pAttrCtrl->GetStartPrst();
	while (pos)
	{
		T_PRST_K PrstK; T_PRST_D PrstD;
		pDoc->m_pAttrCtrl->GetNextPrst(pos, PrstK, PrstD);
		if (pDoc->m_pAttrCtrl->ExistStld(PrstD.LoadCaseKey)) mStldK.SetAt(PrstD.LoadCaseKey, TRUE);
	}
	// Pretension Load
	pos = pDoc->m_pAttrCtrl->GetStartPtns();
	while (pos)
	{
		T_PTNS_K PtnsK; T_PTNS_D PtnsD;
		pDoc->m_pAttrCtrl->GetNextPtns(pos, PtnsK, PtnsD);
		if (pDoc->m_pAttrCtrl->ExistStld(PtnsD.LoadCaseKey)) mStldK.SetAt(PtnsD.LoadCaseKey, TRUE);
	}
	// Tendon Prestress Load
	pos = pDoc->m_pAttrCtrl->GetStartTdpl();
	while (pos)
	{
		T_TDPL_K TdplK; T_TDPL_D TdplD;
		pDoc->m_pAttrCtrl->GetNextTdpl(pos, TdplK, TdplD);
		if (pDoc->m_pAttrCtrl->ExistStld(TdplD.LoadCaseKey)) mStldK.SetAt(TdplD.LoadCaseKey, TRUE);
	}
	// Pressure Load
	pos = pDoc->m_pAttrCtrl->GetStartPres();
	while (pos)
	{
		T_PRES_K PresK; T_PRES_D PresD;
		pDoc->m_pAttrCtrl->GetNextPres(pos, PresK, PresD);
		if (pDoc->m_pAttrCtrl->ExistStld(PresD.LoadCaseKey)) mStldK.SetAt(PresD.LoadCaseKey, TRUE);
	}
	// Hydrostatic Pressure Load
	// Initial Force
	if (pDoc->m_pAttrCtrl->ExistIfct())
	{
		T_IFCT_D IfctD;
		pDoc->m_pAttrCtrl->GetIfct(IfctD);
		if (pDoc->m_pAttrCtrl->ExistStld(IfctD.LoadCaseKey)) mStldK.SetAt(IfctD.LoadCaseKey, TRUE);
	}
	// System Temperature
	T_STMP_K StmpK; T_STMP_D StmpD;
	pos = pDoc->m_pAttrCtrl->GetStartStmp();
	while (pos)
	{
		pDoc->m_pAttrCtrl->GetNextStmp(pos, StmpK, StmpD);
		if (pDoc->m_pAttrCtrl->ExistStld(StmpD.LoadCaseKey)) mStldK.SetAt(StmpD.LoadCaseKey, TRUE);
	}
	// Nodal Temperature
	pos = pDoc->m_pAttrCtrl->GetStartNtmp();
	while (pos)
	{
		T_NTMP_K NtmpK; T_NTMP_D NtmpD;
		pDoc->m_pAttrCtrl->GetNextNtmp(pos, NtmpK, NtmpD);
		if (pDoc->m_pAttrCtrl->ExistStld(NtmpD.LoadCaseKey)) mStldK.SetAt(NtmpD.LoadCaseKey, TRUE);
	}
	// Element Temperature
	pos = pDoc->m_pAttrCtrl->GetStartEtmp();
	while (pos)
	{
		T_ETMP_K EtmpK; T_ETMP_D EtmpD;
		pDoc->m_pAttrCtrl->GetNextEtmp(pos, EtmpK, EtmpD);
		if (pDoc->m_pAttrCtrl->ExistStld(EtmpD.LoadCaseKey)) mStldK.SetAt(EtmpD.LoadCaseKey, TRUE);
	}
	// Temperature Gradient
	T_GTMP_K GtmpK; T_GTMP_D GtmpD;
	pos = pDoc->m_pAttrCtrl->GetStartGtmp();
	while (pos)
	{
		pDoc->m_pAttrCtrl->GetNextGtmp(pos, GtmpK, GtmpD);
		if (pDoc->m_pAttrCtrl->ExistStld(GtmpD.LoadCaseKey)) mStldK.SetAt(GtmpD.LoadCaseKey, TRUE);
	}
	// Beam Section Temperature
	pos = pDoc->m_pAttrCtrl->GetStartBtmp();
	while (pos)
	{
		T_BTMP_K BtmpK; T_BTMP_D BtmpD;
		pDoc->m_pAttrCtrl->GetNextBtmp(pos, BtmpK, BtmpD);
		if (pDoc->m_pAttrCtrl->ExistStld(BtmpD.LoadCaseKey)) mStldK.SetAt(BtmpD.LoadCaseKey, TRUE);
	}
	// Wind Load
	pos = pDoc->m_pAttrCtrl->GetStartWind();
	while (pos)
	{
		T_WIND_K WindK; T_WIND_D WindD;
		pDoc->m_pAttrCtrl->GetNextWind(pos, WindK, WindD);
		if (pDoc->m_pAttrCtrl->ExistStld((T_STLD_K)WindK)) mStldK.SetAt((T_STLD_K)WindK, TRUE);
	}
	// Static Seismic Load
	pos = pDoc->m_pAttrCtrl->GetStartSeis();
	while (pos)
	{
		T_SEIS_K SeisK; T_SEIS_D SeisD;
		pDoc->m_pAttrCtrl->GetNextSeis(pos, SeisK, SeisD);
		if (pDoc->m_pAttrCtrl->ExistStld((T_STLD_K)SeisK)) mStldK.SetAt((T_STLD_K)SeisK, TRUE);
	}
	// Superstructure Seismic Load
	pos = pDoc->m_pAttrCtrl->GetStartSsei();
	while (pos)
	{
		T_SSEI_K SseiK; T_SSEI_D SseiD;
		pDoc->m_pAttrCtrl->GetNextSsei(pos, SseiK, SseiD);
		//if(pDoc->m_pAttrCtrl->ExistStld(SseiD.StldKey)) mStldK.SetAt(SseiD.StldKey, TRUE);
		if (pDoc->m_pAttrCtrl->ExistStld((T_STLD_K)SseiK)) mStldK.SetAt((T_STLD_K)SseiK, TRUE);
	}

	BOOL tmp;
	pos = mStldK.GetStartPosition();
	while (pos)
	{
		mStldK.GetNextAssoc(pos, StldK, tmp);
		arKeyList.Add(StldK);
	}
	arKeyList.FreeExtra();
	qsort(arKeyList.GetData(), arKeyList.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);

	return TRUE;
}

void ILoadCombDefineMgr::GetLCaseCoef(double& dW, double& dL, double& dGL)
{
	dW = m_dW;
	dL = m_dL;
	dGL = m_dGL;
}

void ILoadCombDefineMgr::SetLCaseCoef(double dW, double dL, double dGL)
{
	m_dW = dW;
	m_dL = dL;
	m_dGL = dGL;
}

void ILoadCombDefineMgr::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	switch (lHint)
	{
	case D_UPDATE_DEFAULT:
		m_pData->initial();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}
