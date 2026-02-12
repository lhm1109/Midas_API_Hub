// ServiceAgencyCmd.cpp: implementation of the CServiceAgencyTreemenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ServiceAgencyTreemenu.h"
#include "ServiceDefTreemenu.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\CMWindPressureProfileDlg.h"
#include "..\wg_cmd\CMWindPressureProfileKBC2016Dlg.h"
#include "..\wg_cmd\CMStaticEarthPressureProfileDlg.h"
#include "..\wg_cmd\CMSeismicEarthPressureProfileDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\DB_ST_DT_WDPR.h"

#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"

//#include "..\wg_base\wg_base_MsgDll.h"

#include "StldWindPressureAreaDlg_CH2019_CrossTorsion.h"
#include "StldWindPressureAreaDlg_CH2012_CrossTorsion.h"
#include "StldWindPressureAreaDlg_KBC2009_FrcCoefAuto.h"
#include "StldWindPressureAreaDlg_KBC2016_FrcCoefAuto.h"
#include "StldWindPressureAreaDlg_CH2001_PulsMag.h"
#include "StldWindPressureNodalDlg_StructureType.h"
#include "StldWindPressureVibrationDlg.h"
#include "StldWindPressureStructDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServiceAgencyTreemenu::CServiceAgencyTreemenu()
{

}

CServiceAgencyTreemenu::~CServiceAgencyTreemenu()
{

}


int CServiceAgencyTreemenu::DoService(int nServiceNo, void* pData)
{
	switch(nServiceNo)
	{
	case D_TREEMENU_DO_AFX_TESTMSG: 
		return afxTestMsg();
	case D_TREEMENU_DO_WINDP_CH2019_CONSIDER_CRSTOR:
		return WindPCH2019_ConsiderCrsTor(pData);
	case D_TREEMENU_DO_WINDP_CH2012_CONSIDER_CRSTOR:
		return WindPCH2012_ConsiderCrsTor(pData);
	case D_TREEMENU_DO_WINDP_KBC2009_AUTOFORCECOEF:
		return WindPKBC2009_AutoCalcCoeff(pData);
	case D_TREEMENU_DO_WINDP_KBC2016_STRUCT:
		return WindPKBC2016_SelectStructType(pData);
	case D_TREEMENU_DO_WINDP_WINDPROFILE:
		return WindPWindProfile(pData, FALSE);
	case D_TREEMENU_DO_WINDP_WINDPROFILEKBC2016:
		return WindPWindProfile(pData, TRUE);
	case D_TREEMENU_DO_WINDP_CH2001_MAGFACTOR:
		return WindPCH2001_CalcMagFactor(pData);
	case D_TREEMENU_DO_WINDP_NODAL_STRUCTURE:
		return WindPNordalStructure(pData);
	case D_TREEMENU_DO_WINDP_KBC2016_WINDDIR:
		return WindPKBC2016_WindDir(pData);
	case D_TREEMENU_DO_EARTHP_STATIC_PROFILE:
		return EarthPStaticProfile(pData);
	case D_TREEMENU_DO_EARTHP_SEISMIC_PROFILE:
		return EarthPSeismicProfile(pData);
	case D_TREEMENU_DO_WINDP_ASCE7_16_STRUCT:
		return WindPASCE7_16_SelectStructType(pData);
	default:
		ASSERT(0);
		break;
	}
	return D_SP_ERR_IRG_SERVICE_NO;
}

int CServiceAgencyTreemenu::afxTestMsg()
{
	AfxMessageBox(_T("TEST"));
	return 0;
}

int CServiceAgencyTreemenu::WindPCH2019_ConsiderCrsTor(void* pData)
{
	T_WDPR_D* pWdprD = (T_WDPR_D*)pData;

	CStldWindPressureAreaDlg_CH2019_CrossTorsion dlg;
	dlg.SetData(pWdprD->CodeParam.CH2019);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetData(pWdprD->CodeParam.CH2019);
	}

	return 0;
}

int CServiceAgencyTreemenu::WindPCH2012_ConsiderCrsTor(void* pData)
{
	T_WDPR_D* pWdprD = (T_WDPR_D*)pData;

	CStldWindPressureAreaDlg_CH2012_CrossTorsion dlg;
	dlg.SetData(pWdprD->CodeParam.CH2012);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetData(pWdprD->CodeParam.CH2012);
	}

	return 0;
}

int CServiceAgencyTreemenu::WindPKBC2016_WindDir( void* pData )
{
	T_WDPR_D* pWdprD = (T_WDPR_D*)pData;

	CStldWindPressureVibrationDlg dlg;
	dlg.m_nBuildingType = pWdprD->CodeParam.KBC2016.nBuildingType;
	dlg.m_nAcrossCheck = pWdprD->CodeParam.KBC2016.bAcrossWind;
	dlg.m_nTorsionalCheck = pWdprD->CodeParam.KBC2016.bTorsionalWind;
	dlg.SetWindData(&pWdprD->CodeParam.KBC2016, pWdprD->nDirection, pWdprD->dAngle);
	if(dlg.DoModal() == IDOK)
	{
		pWdprD->CodeParam.KBC2016.dBLy = dlg.m_dBreadthX;
		pWdprD->CodeParam.KBC2016.dBLx = dlg.m_dBreadthY;
		pWdprD->CodeParam.KBC2016.dNoa = dlg.m_dFreqA;
		pWdprD->CodeParam.KBC2016.dNot = dlg.m_dFreqT;
		pWdprD->CodeParam.KBC2016.dZf = dlg.m_dZf;
	}

	return 0;  
}

int CServiceAgencyTreemenu::EarthPStaticProfile(void* pData)
{
	T_EPST_D* pEpstD = (T_EPST_D*)pData;
	T_EPST_D EpstD = *pEpstD;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == nullptr) return D_SP_ERR_IRG_SERVICE_NO;

	double dHeight = 0.0;
	BOOL bRetVal = CWindLoadGeneratorCtrl::GetModelHeight4EarthPressure(EpstD.KeyPosp, dHeight);
	if (dHeight <= 1E-5 || bRetVal == FALSE)
	{
		AfxMessageBox(_LS(IDS_TM_EARTHP_NO_NODEINFO));
		return D_SP_ERR_IRG_SERVICE_NO;
	}

	BOOL bXYDir = (EpstD.nDirection == 1);

	double dVecTemp[3] = { 0.0, };
	if (bXYDir == FALSE)
	{
		if (!pDoc->m_pAttrCtrl2->GetEpstAverageVec(EpstD, dVecTemp, TRUE)) return D_SP_ERR_IRG_SERVICE_NO;
	}

	CCMStaticEarthPressureProfileDlg dlg;
	dlg.InitProfile(EpstD, FALSE, bXYDir ? NULL : dVecTemp);
	dlg.DoModal();

	BOOL bDiff = FALSE;
	if (EpstD.arPresProfile.GetSize() != pEpstD->arPresProfile.GetSize()) bDiff = TRUE;
	if (bDiff == FALSE)
	{
		int nSize = EpstD.arPresProfile.GetSize();
		for(int i=0; i < nSize; i++)
		{
			if (pEpstD->arPresProfile[i] == EpstD.arPresProfile[i]) continue;
			bDiff = TRUE;
			break;
		}
	}

	if (bDiff == FALSE) return 1;

	*pEpstD = EpstD;
	return 0;
}

int CServiceAgencyTreemenu::EarthPSeismicProfile(void* pData)
{
	T_EPSE_D* pEpseD = (T_EPSE_D*)pData;
	T_EPSE_D EpseD = *pEpseD;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == nullptr) return D_SP_ERR_IRG_SERVICE_NO;

	double dHeight = 0.0;
	BOOL bRetVal = CWindLoadGeneratorCtrl::GetModelHeight4EarthPressure(EpseD.KeyPosp, dHeight);
	if (dHeight <= 1E-5 || bRetVal == FALSE)
	{
		AfxMessageBox(_LS(IDS_TM_EARTHP_NO_NODEINFO));
		return D_SP_ERR_IRG_SERVICE_NO;
	}

	BOOL bXYDir = (EpseD.nDirection == 1);

	double dVecTemp[3] = { 0.0, };
	if (bXYDir == FALSE)
	{
		if (!pDoc->m_pAttrCtrl2->GetEpseAverageVec(EpseD, dVecTemp, TRUE)) return D_SP_ERR_IRG_SERVICE_NO;
	}
	pDoc->m_pEditData->CheckEpseEquivalent(EpseD);

	CCMSeismicEarthPressureProfileDlg dlg;
	dlg.InitProfile(EpseD, FALSE);
	dlg.DoModal();

	BOOL bDiff = FALSE;
	if (EpseD.arPresProfile.GetSize() != pEpseD->arPresProfile.GetSize()) bDiff = TRUE;
	if (bDiff == FALSE)
	{
		int nSize = EpseD.arPresProfile.GetSize();
		for(int i=0; i < nSize; i++)
		{
			if (pEpseD->arPresProfile[i] == EpseD.arPresProfile[i]) continue;
			bDiff = TRUE;
			break;
		}
	}

	if (bDiff == FALSE) return 1;

	*pEpseD = EpseD;
	return 0;
}
int CServiceAgencyTreemenu::WindPKBC2009_AutoCalcCoeff( void* pData )
{
	T_WDPR_D* pWdprD = (T_WDPR_D*)pData;
	
	CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto dlg;
	dlg.SetData(pWdprD);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetData(pWdprD);
	}

	return 0;  
}

int CServiceAgencyTreemenu::WindPKBC2016_SelectStructType( void* pData )
{
	T_WDPR_D* pWdprD = (T_WDPR_D*)pData;

	int* pCoefStructType = NULL;
	if (pWdprD->nCodeType == KDS_W_2022) pCoefStructType = &pWdprD->CodeParam.KDS2021.nAutoForceCoefStructType;
	else if(pWdprD->nCodeType == KDS_W_2019 || pWdprD->nCodeType == KBC_W_2016)
		pCoefStructType = &pWdprD->GetKBC2016()->nAutoForceCoefStructType;
	else { ASSERT(0); return 0; }

	CStldWindPressureStructDlg StructDlg(NULL, *pCoefStructType);
	if(StructDlg.DoModal() == IDOK)
	{
		int nStructureType = StructDlg.GetStructureType();
		CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto dlg(NULL,nStructureType,pWdprD->KeyWvep);
		dlg.SetData(pWdprD);
		if(dlg.DoModal() == IDOK)
		{
			dlg.GetData(pWdprD);
			*pCoefStructType = nStructureType;
		}
	}

	return 0;  
}

int CServiceAgencyTreemenu::WindPASCE7_16_SelectStructType(void* pData)
{
	T_WDPR_D* pWdprD = (T_WDPR_D*)pData;
	T_WDPR_ASCE7_16* pWdprCode = pWdprD->GetASCE7Series();
	CStldWindPressureStructDlg StructDlg(NULL, pWdprCode->nAutoForceCoefStructType, pWdprD->nCodeType);
	if (StructDlg.DoModal() == IDOK)
	{
		int nStructureType = StructDlg.GetStructureType();
		CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto dlg(NULL, nStructureType, pWdprD->KeyWvep);
		dlg.SetData(pWdprD);
		if (dlg.DoModal() == IDOK)
		{
			dlg.GetData(pWdprD);
			pWdprCode->nAutoForceCoefStructType = nStructureType;
		}
	}

	return 0;
}
int CServiceAgencyTreemenu::WindPWindProfile( void* pData, BOOL bKBC2016 )
{
	T_WDPR_D* pWdprD = (T_WDPR_D*)pData;
	T_WDPR_D WdprD = *pWdprD;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(pDoc==nullptr) return D_SP_ERR_IRG_SERVICE_NO;

	if(bKBC2016)
	{
		if(!pDoc->m_pEditData->CheckWdprKBC2016VibrValue(WdprD)) return D_SP_ERR_IRG_SERVICE_NO;
	}

	int nStore = 0;
	double dHeight = 0.0;
	double dWidth = 0.0;
	BOOL bRetVal = CWindLoadGeneratorCtrl::GetModelHeightWidthTotal(NULL, dHeight, nStore, dWidth);
	if(dHeight <= 1E-5 || bRetVal == FALSE)
	{
		AfxMessageBox(_LS(IDS_TM_WINDP_NO_NODEINFO));
		return D_SP_ERR_IRG_SERVICE_NO;
	}

	BOOL bXYDir = (WdprD.nDirection == 1);

	double dWindVecTemp[3] = {0.0, };
	if(bXYDir == FALSE)
	{
		if(!pDoc->m_pAttrCtrl2->GetWdprAverageWindVec(WdprD, dWindVecTemp, TRUE)) return D_SP_ERR_IRG_SERVICE_NO;
	}

	if(bKBC2016)
	{
		CCMWindPressureProfileKBC2016Dlg dlg;
		dlg.InitWindProfile(WdprD, bXYDir ? NULL : dWindVecTemp);
		dlg.DoModal();
	}
	else
	{
		CCMWindPressureProfileDlg dlg;
		dlg.InitWindProfile(WdprD, bXYDir ? NULL : dWindVecTemp);
		dlg.DoModal();
	}

	BOOL bDiff = FALSE;
	if(WdprD.arPresProfile.GetSize() != pWdprD->arPresProfile.GetSize()) bDiff = TRUE;
	if(bDiff == FALSE)
	{
		int i;
		int nSize = WdprD.arPresProfile.GetSize();
		for(i = 0 ; i < nSize; i++)
		{
			if(pWdprD->arPresProfile[i] == WdprD.arPresProfile[i]) continue;
			bDiff = TRUE;
			break;
		}
	}

	if(bDiff == FALSE) return 1;

	*pWdprD = WdprD;
	return 0;
}

int CServiceAgencyTreemenu::WindPCH2001_CalcMagFactor( void* pData )
{
	T_WDPR_D* pWdprD = (T_WDPR_D*)pData;

	T_WVEP_K WvepK = pWdprD->KeyWvep;

	CStldWindPressureAreaDlg_CH2001_PulsMag dlg;
	dlg.SetWdpr(pWdprD);
	if(dlg.DoModal() == IDOK)
	{
		pWdprD->CodeParam.CH2001.dDampingRatio = dlg.GetDampingRatio();
		pWdprD->CodeParam.CH2001.dPeriod = dlg.GetPeriod();
		pWdprD->CodeParam.CH2001.dPulseMagnifyFactor = dlg.GetPulsMagnifyFactor();
		return 0;
	}
	return 1;
}

int CServiceAgencyTreemenu::WindPNordalStructure( void* pData )
{
	T_WDPR_D* pWdprD = (T_WDPR_D*)pData;

	T_WVEP_K WvepK = pWdprD->KeyWvep;

	CStldWindPressureNodalDlg_StructureType dlg;
	dlg.SetWdpr(pWdprD);
	if(dlg.DoModal() == IDOK)
	{
		dlg.GetNodalStructure(pWdprD);
		double dArea;
		double dConcenPt[3];
		if(CWindLoadGeneratorCtrl::GetNodal_WindAreaConcenPt(*pWdprD, dArea, dConcenPt))
		{
			pWdprD->dWindArea = dArea;
			pWdprD->dConcenPt[0] = dConcenPt[0];
			pWdprD->dConcenPt[1] = dConcenPt[1];
			pWdprD->dConcenPt[2] = dConcenPt[2];
			pWdprD->arPresProfile.RemoveAll();
		}
		return 0;
	}
	return 1;  
}

class CServiceAgencyTreemenuInit
{
public:
	CServiceAgencyTreemenuInit() 
	{ 
		CServiceHeadOffice* pHeadOffice = CServiceHeadOffice::Instance();
		if (pHeadOffice)
		{
			pHeadOffice->RegAgency(_ULS(treemenu), new CServiceAgencyTreemenu);
		}
	}
};

CServiceAgencyTreemenuInit init;
