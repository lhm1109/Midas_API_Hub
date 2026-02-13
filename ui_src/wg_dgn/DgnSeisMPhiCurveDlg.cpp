// DgnSeisMPhiCurveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisMPhiCurveDlg.h"

#include "..\wg_base\MSOffice.h"

#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_MathFunc.h"

#include "BrdgSeisEvalKALIS2023.h"
#include "BrdgSeisEvalKISTEC2019.h"
#include "BrdgSeisEvalKISTEC2015.h"
#include "BrdgSeisEvalKEC2012.h"
#include "DgnSeisMPhiExportDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiCurveDlg dialog

CDgnSeisMPhiCurveDlg::CDgnSeisMPhiCurveDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisMPhiCurveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisMPhiCurveDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nAngleDir = 0;

	m_pDoc = CDBDoc::GetDocPoint();

	m_pGrid = NULL;
	m_pGraph = NULL;

	m_Data.Initialize();
	m_MphiResD[0].initialize();
	m_MphiResD[1].initialize();
	m_CurveOpt.Initialize();	
	
	m_bSetBmp = FALSE;
	m_strBmpPath = _T("");
}

CDgnSeisMPhiCurveDlg::~CDgnSeisMPhiCurveDlg()
{
	if(m_pSeisEval)
	{
		delete m_pSeisEval;
		m_pSeisEval = nullptr;
	}

	if(m_pGraph)
	{
		if(m_pGraph->GetSafeHwnd())
			m_pGraph->DestroyWindow();
		delete m_pGraph;
		m_pGraph = NULL;
	}
}

void CDgnSeisMPhiCurveDlg::MakeBmpAndClose(CString strPath)
{
	m_bSetBmp = TRUE;
	m_strBmpPath = strPath;
}

void CDgnSeisMPhiCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisMPhiCurveDlg)
	DDX_Radio  (pDX, IDC_DGN_SEIS_MPHI_CURVE_LONGI_RDO,     m_nAngleDir);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_CURVE_TYPE_CMB,      m_cmbCurve);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_CURVE_I_LONGI_EDT,   m_edtIyL);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_CURVE_I_TRANS_EDT,   m_edtIyT);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_CURVE_FILE_NAME_EDT, m_edtFileName);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_CURVE_GRID, *m_pGrid);
	//}}AFX_DATA_MAP
}

void CDgnSeisMPhiCurveDlg::InitUnit()
{
	m_edtIyL.SetEditUnit(D_UNITSYS_BASE_STIF);
	m_edtIyT.SetEditUnit(D_UNITSYS_BASE_STIF);
}

void CDgnSeisMPhiCurveDlg::InitCombo()
{
	m_cmbCurve.ResetContent();
	m_cmbCurve.AddString(_LS(IDS_DGN_SEIS_MPCC_CURVE_M_C));
	m_cmbCurve.AddString(_LS(IDS_DGN_SEIS_MPCC_CURVE_NA_C));
	m_cmbCurve.AddString(_LS(IDS_DGN_SEIS_MPCC_CURVE_CS_C));
	m_cmbCurve.AddString(_LS(IDS_DGN_SEIS_MPCC_CURVE_SS_C));
	m_cmbCurve.AddString(_LS(IDS_DGN_SEIS_MPCC_CURVE_CC_C));
	m_cmbCurve.AddString(_LS(IDS_DGN_SEIS_MPCC_CURVE_SC_C));
	m_cmbCurve.AddString(_LS(IDS_DGN_SEIS_MPCC_CURVE_ST_C));
	m_cmbCurve.SetCurSel(0);
}

void CDgnSeisMPhiCurveDlg::InitGrid()
{
	m_pGrid->Initialize(this);
}

void CDgnSeisMPhiCurveDlg::InitView()
{
	CRect FrameRect;
	GetDlgItem(IDC_DGN_SEIS_MPHI_CURVE_VIEW)->GetWindowRect(FrameRect);
	ScreenToClient(FrameRect);

	if(m_pGraph)
	{
		ASSERT(0);
		if(m_pGraph->GetSafeHwnd())
			m_pGraph->DestroyWindow();
		delete m_pGraph;
		m_pGraph = NULL;
	}

	FrameRect.InflateRect(-3, -3);
	m_pGraph = new CDgnSeisMPhiCurveGraph;
	m_pGraph->Create(NULL, NULL, WS_VISIBLE|WS_CHILD, FrameRect, this, 101);

	m_pGraph->SetData(m_MphiResD);
	m_pGraph->SetCurveOpt(&m_CurveOpt);
	m_pGraph->OnInitialUpdate();
}

void CDgnSeisMPhiCurveDlg::InitControl()
{
	TCHAR *lpBuffer;
	lpBuffer = new TCHAR[255];

	CString strPath;
	strPath = CDBDoc::GetDocPoint()->GetPathName();
	int idx = strPath.ReverseFind('\\');
	if(idx!=-1) strPath = strPath.Left(idx);

	if(strPath == _T(""))
	{
		GetCurrentDirectory(255, lpBuffer);
		strPath = lpBuffer;
	}

    bool bRTF = CDBLib::IsWordRTF();
    CString strDotExt = _T(".") + CMSOffice::GetExtensionName(bRTF ? EXT_WORDRTF : EXT_WORD);
	CString strFileName;
    if(m_Data.strName.IsEmpty())
	    strFileName = strPath + _T("\\") + _T("m_phi_Result")+strDotExt;
    else
        strFileName = strPath + _T("\\") + m_Data.strName+strDotExt;

	m_edtFileName.SetWindowText(strFileName);
	m_edtFileName.EnableFileBrowseButton(NULL, _LS(IDS_DGN_SEIS_MPCC_ALL_FILE), NULL, OFN_HIDEREADONLY, FALSE, FALSE);
}

void CDgnSeisMPhiCurveDlg::EnableDisableControls()
{
	GetDlgItem(IDC_DGN_SEIS_MPHI_CURVE_EFF_STIFF_GRP)->EnableWindow(m_Data.EvgpK != 0);
	GetDlgItem(IDC_DGN_SEIS_MPHI_CURVE_I_LONGI_STC  )->EnableWindow(m_Data.EvgpK != 0);
	GetDlgItem(IDC_DGN_SEIS_MPHI_CURVE_I_LONGI_EDT  )->EnableWindow(m_Data.EvgpK != 0);
	GetDlgItem(IDC_DGN_SEIS_MPHI_CURVE_IYY_STC      )->EnableWindow(m_Data.EvgpK != 0);
	GetDlgItem(IDC_DGN_SEIS_MPHI_CURVE_I_TRANS_STC  )->EnableWindow(m_Data.EvgpK != 0);
	GetDlgItem(IDC_DGN_SEIS_MPHI_CURVE_I_TRANS_EDT  )->EnableWindow(m_Data.EvgpK != 0);
	GetDlgItem(IDC_DGN_SEIS_MPHI_CURVE_IZZ_STC      )->EnableWindow(m_Data.EvgpK != 0);
	GetDlgItem(IDC_DGN_SEIS_MPHI_CURVE_SECT_EFF_STIFF_BTN)->EnableWindow(m_Data.EvgpK != 0);

	GetDlgItem(IDC_DGN_SEIS_MPHI_CURVE_EXPORT_BTN)->EnableWindow(m_Data.bIdealize);
}

//void CDgnSeisMPhiCurveDlg::SetAngleDirChange(int nAngleDir)
//{
//	m_CurveOpt.nAngleDir = nAngleDir;
//}

BOOL CDgnSeisMPhiCurveDlg::Get_MPhiResult(const T_MPCC_D& MpccD, T_BRDGEVAL_MPHI_RES mphiResD[2], BOOL bRecal, double dAddAngle/* = 0.0*/)
{
	int nError = m_pSeisEval->Get_MPhiResult(MpccD, mphiResD, bRecal, dAddAngle);

	if      (nError == 1) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_1));   return FALSE; }
	else if (nError == 2) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_2));   return FALSE; }
	else if (nError == 3) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_3));   return FALSE; }
	else if (nError == 4) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_4));   return FALSE; }
	else if (nError == 5) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_5));   return FALSE; }
	else if (nError == 6) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_6));   return FALSE; }
	else if (nError == 7) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_7));   return FALSE; }
	else if (nError == 8) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_8));   return FALSE; }
	else if (nError == 9) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_9));   return FALSE; }
	else if (nError == 10) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_10));  return FALSE; }
	else if (nError == 11) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_11));  return FALSE; }
	else if (nError == 12) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_12));  return FALSE; }
	else if (nError == 13) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_13));  return FALSE; }
	else if (nError == 31) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_31));  return FALSE; }
	else if (nError == 32) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_32));  return FALSE; }
	else if (nError == 33) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_33));  return FALSE; }
	else if (nError == 100) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_100)); return FALSE; }
	else if (nError == 200) { AfxMessageBox(_LS(IDS_DGN_MPHI_CALC_MOMENT_CURVATURE_ERROR_200)); return FALSE; }
	else if (nError == 50)
	{
		CString stForceUnit;  m_pDoc->m_pUnitCtrl->GetUnitSystemForce(stForceUnit);
		CString strValue;
		if (mphiResD[0].StateD.nChk == 50)
			strValue.Format(_T("%d %s"), (int)mphiResD[0].StateD.dLimitAxialForce, stForceUnit);
		else
			strValue.Format(_T("%d %s"), (int)mphiResD[1].StateD.dLimitAxialForce, stForceUnit);
		CString strMsg;  strMsg.Format(_LS(IDS_DGN_MPHI_CURVE_OVER_AXIALFORCE), strValue);  AfxMessageBox(strMsg);  return FALSE;
	}

	return TRUE;
}


void CDgnSeisMPhiCurveDlg::Data2Dlg()
{
	double dIeffRat[2];
	dIeffRat[0] = 1.0;
	dIeffRat[1] = 1.0;

	if (!Get_MPhiResult(m_Data, m_MphiResD, !m_bSetBmp, 0.0)) return;

	if(m_Data.EvgpK != 0)
	{		
		T_BRDGEVAL_RESULT Result[2];
		m_pSeisEval->CalcIeff(m_Data, dIeffRat, Result, FALSE);
	}

	m_edtIyL.SetEditUnit(dIeffRat[0]);
	m_edtIyT.SetEditUnit(dIeffRat[1]);

	m_pGrid->SetData2Grid(m_CurveOpt.nAngleDir, m_MphiResD);

	if(m_bSetBmp || m_CurveOpt.bIdealize)
	{
		for(int i=0; i<2; ++i)
		{
			T_BRDGEVAL_MPHI_UNIT BeforeUnitD; BeforeUnitD.initialize();
			int nmpih = m_MphiResD[i].arMPhiCruve.GetSize() - 1;
			for(int j=nmpih; j>=0; --j)
			{
				if(m_MphiResD[i].arMPhiCruve[j].dCurvature > m_MphiResD[i].StateD.dCurvature[5]) 
				{
					BeforeUnitD = m_MphiResD[i].arMPhiCruve[j];
					m_MphiResD[i].arMPhiCruve.RemoveAt(j);
				}
				else
				{
					T_BRDGEVAL_MPHI_UNIT EndUnit; EndUnit.initialize();

					double dRat = 0.0;
					if(fabs(m_MphiResD[i].arMPhiCruve[j].dCurvature)< 1.0E-12)
						dRat = (m_MphiResD[i].StateD.dCurvature[5]) / (BeforeUnitD.dCurvature);
					else
						dRat = (m_MphiResD[i].StateD.dCurvature[5] - m_MphiResD[i].arMPhiCruve[j].dCurvature) / (BeforeUnitD.dCurvature - m_MphiResD[i].arMPhiCruve[j].dCurvature);

					EndUnit.dMoment     = CMathFunc::mathInterpolate(m_MphiResD[i].arMPhiCruve[j].dMoment    , BeforeUnitD.dMoment    , dRat);
					EndUnit.dCurvature  = CMathFunc::mathInterpolate(m_MphiResD[i].arMPhiCruve[j].dCurvature , BeforeUnitD.dCurvature , dRat);
					EndUnit.dXn         = CMathFunc::mathInterpolate(m_MphiResD[i].arMPhiCruve[j].dXn        , BeforeUnitD.dXn        , dRat);
					EndUnit.dConcStrain = CMathFunc::mathInterpolate(m_MphiResD[i].arMPhiCruve[j].dConcStrain, BeforeUnitD.dConcStrain, dRat);
					EndUnit.dStlStrain  = CMathFunc::mathInterpolate(m_MphiResD[i].arMPhiCruve[j].dStlStrain , BeforeUnitD.dStlStrain , dRat);
					EndUnit.dConcComp   = CMathFunc::mathInterpolate(m_MphiResD[i].arMPhiCruve[j].dConcComp  , BeforeUnitD.dConcComp  , dRat);
					EndUnit.dStlComp    = CMathFunc::mathInterpolate(m_MphiResD[i].arMPhiCruve[j].dStlComp   , BeforeUnitD.dStlComp   , dRat);
					EndUnit.dStlTens    = CMathFunc::mathInterpolate(m_MphiResD[i].arMPhiCruve[j].dStlTens   , BeforeUnitD.dStlTens   , dRat);
					m_MphiResD[i].arMPhiCruve.Add(EndUnit);
					break;
				}
			}
		}
	}

	m_pGraph->SetData(m_MphiResD);
	m_pGraph->SetCurveOpt(&m_CurveOpt);

	m_pGraph->DrawGraph();
	m_pGraph->Invalidate();
}

void CDgnSeisMPhiCurveDlg::GetCurveOpt()
{
	UpdateData(TRUE);

	m_CurveOpt.dAxialForce = m_Data.dAxialForce;
	m_CurveOpt.bIdealize = m_Data.bIdealize;
	m_CurveOpt.bCurvature = m_Data.bCurvature;
	m_CurveOpt.dCurvature = m_Data.dCurvature;
	m_CurveOpt.nAngleDir = m_nAngleDir;
	m_CurveOpt.nCurveType = m_cmbCurve.GetCurSel();
}

void CDgnSeisMPhiCurveDlg::GetTempData()
{
	m_MphiResD[0].initialize();
	m_MphiResD[1].initialize();

	m_MphiResD[0].arMPhiCruve.SetSize(50);
	m_MphiResD[0].arMPhiCruve[ 0].dMoment = 66.072;
	m_MphiResD[0].arMPhiCruve[ 1].dMoment = 257.657;
	m_MphiResD[0].arMPhiCruve[ 2].dMoment = 433.787;
	m_MphiResD[0].arMPhiCruve[ 3].dMoment = 593.502;
	m_MphiResD[0].arMPhiCruve[ 4].dMoment = 768.917;
	m_MphiResD[0].arMPhiCruve[ 5].dMoment = 971.597;
	m_MphiResD[0].arMPhiCruve[ 6].dMoment = 1205.375;
	m_MphiResD[0].arMPhiCruve[ 7].dMoment = 1472.051;
	m_MphiResD[0].arMPhiCruve[ 8].dMoment = 1772.322;
	m_MphiResD[0].arMPhiCruve[ 9].dMoment = 2104.566;
	m_MphiResD[0].arMPhiCruve[10].dMoment = 2468.201;
	m_MphiResD[0].arMPhiCruve[11].dMoment = 2863.853;
	m_MphiResD[0].arMPhiCruve[12].dMoment = 3292.535;
	m_MphiResD[0].arMPhiCruve[13].dMoment = 3713.232;
	m_MphiResD[0].arMPhiCruve[14].dMoment = 3980.318;
	m_MphiResD[0].arMPhiCruve[15].dMoment = 4174.963;
	m_MphiResD[0].arMPhiCruve[16].dMoment = 4321.816;
	m_MphiResD[0].arMPhiCruve[17].dMoment = 4449.864;
	m_MphiResD[0].arMPhiCruve[18].dMoment = 4539.916;
	m_MphiResD[0].arMPhiCruve[19].dMoment = 4624.633;
	m_MphiResD[0].arMPhiCruve[20].dMoment = 4696.152;
	m_MphiResD[0].arMPhiCruve[21].dMoment = 4763.465;
	m_MphiResD[0].arMPhiCruve[22].dMoment = 4810.886;
	m_MphiResD[0].arMPhiCruve[23].dMoment = 4870.914;
	m_MphiResD[0].arMPhiCruve[24].dMoment = 4910.161;
	m_MphiResD[0].arMPhiCruve[25].dMoment = 4943.831;
	m_MphiResD[0].arMPhiCruve[26].dMoment = 4976.033;
	m_MphiResD[0].arMPhiCruve[27].dMoment = 5012.335;
	m_MphiResD[0].arMPhiCruve[28].dMoment = 5035.432;
	m_MphiResD[0].arMPhiCruve[29].dMoment = 5055.603;
	m_MphiResD[0].arMPhiCruve[30].dMoment = 5078.214;
	m_MphiResD[0].arMPhiCruve[31].dMoment = 5099.622;
	m_MphiResD[0].arMPhiCruve[32].dMoment = 5120.829;
	m_MphiResD[0].arMPhiCruve[33].dMoment = 5131.8;
	m_MphiResD[0].arMPhiCruve[34].dMoment = 5144.691;
	m_MphiResD[0].arMPhiCruve[35].dMoment = 5173.361;
	m_MphiResD[0].arMPhiCruve[36].dMoment = 5210.589;
	m_MphiResD[0].arMPhiCruve[37].dMoment = 5244.997;
	m_MphiResD[0].arMPhiCruve[38].dMoment = 5289.871;
	m_MphiResD[0].arMPhiCruve[39].dMoment = 5337.585;
	m_MphiResD[0].arMPhiCruve[40].dMoment = 5382.318;
	m_MphiResD[0].arMPhiCruve[41].dMoment = 5432.947;
	m_MphiResD[0].arMPhiCruve[42].dMoment = 5476.15;
	m_MphiResD[0].arMPhiCruve[43].dMoment = 5524.827;
	m_MphiResD[0].arMPhiCruve[44].dMoment = 5574.453;
	m_MphiResD[0].arMPhiCruve[45].dMoment = 5619.064;
	m_MphiResD[0].arMPhiCruve[46].dMoment = 5665.353;
	m_MphiResD[0].arMPhiCruve[47].dMoment = 5707.394;
	m_MphiResD[0].arMPhiCruve[48].dMoment = 5756.838;
	m_MphiResD[0].arMPhiCruve[49].dMoment = 5786.223;

	m_MphiResD[0].arMPhiCruve[ 0].dCurvature = 0.008574;
	m_MphiResD[0].arMPhiCruve[ 1].dCurvature = 0.034295;
	m_MphiResD[0].arMPhiCruve[ 2].dCurvature = 0.077164;
	m_MphiResD[0].arMPhiCruve[ 3].dCurvature = 0.13718;
	m_MphiResD[0].arMPhiCruve[ 4].dCurvature = 0.214344;
	m_MphiResD[0].arMPhiCruve[ 5].dCurvature = 0.308656;
	m_MphiResD[0].arMPhiCruve[ 6].dCurvature = 0.420115;
	m_MphiResD[0].arMPhiCruve[ 7].dCurvature = 0.548721;
	m_MphiResD[0].arMPhiCruve[ 8].dCurvature = 0.694475;
	m_MphiResD[0].arMPhiCruve[ 9].dCurvature = 0.857377;
	m_MphiResD[0].arMPhiCruve[10].dCurvature = 1.037426;
	m_MphiResD[0].arMPhiCruve[11].dCurvature = 1.234623;
	m_MphiResD[0].arMPhiCruve[12].dCurvature = 1.448967;
	m_MphiResD[0].arMPhiCruve[13].dCurvature = 1.680459;
	m_MphiResD[0].arMPhiCruve[14].dCurvature = 1.929098;
	m_MphiResD[0].arMPhiCruve[15].dCurvature = 2.194885;
	m_MphiResD[0].arMPhiCruve[16].dCurvature = 2.477819;
	m_MphiResD[0].arMPhiCruve[17].dCurvature = 2.777901;
	m_MphiResD[0].arMPhiCruve[18].dCurvature = 3.09513;
	m_MphiResD[0].arMPhiCruve[19].dCurvature = 3.429507;
	m_MphiResD[0].arMPhiCruve[20].dCurvature = 3.781032;
	m_MphiResD[0].arMPhiCruve[21].dCurvature = 4.149704;
	m_MphiResD[0].arMPhiCruve[22].dCurvature = 4.535523;
	m_MphiResD[0].arMPhiCruve[23].dCurvature = 4.93849;
	m_MphiResD[0].arMPhiCruve[24].dCurvature = 5.358605;
	m_MphiResD[0].arMPhiCruve[25].dCurvature = 5.795867;
	m_MphiResD[0].arMPhiCruve[26].dCurvature = 6.250277;
	m_MphiResD[0].arMPhiCruve[27].dCurvature = 6.721834;
	m_MphiResD[0].arMPhiCruve[28].dCurvature = 7.210539;
	m_MphiResD[0].arMPhiCruve[29].dCurvature = 7.716391;
	m_MphiResD[0].arMPhiCruve[30].dCurvature = 8.239391;
	m_MphiResD[0].arMPhiCruve[31].dCurvature = 8.779538;
	m_MphiResD[0].arMPhiCruve[32].dCurvature = 9.336833;
	m_MphiResD[0].arMPhiCruve[33].dCurvature = 9.911276;
	m_MphiResD[0].arMPhiCruve[34].dCurvature = 10.503;
	m_MphiResD[0].arMPhiCruve[35].dCurvature = 11.112;
	m_MphiResD[0].arMPhiCruve[36].dCurvature = 11.737;
	m_MphiResD[0].arMPhiCruve[37].dCurvature = 12.381;
	m_MphiResD[0].arMPhiCruve[38].dCurvature = 13.041;
	m_MphiResD[0].arMPhiCruve[39].dCurvature = 13.718;
	m_MphiResD[0].arMPhiCruve[40].dCurvature = 14.413;
	m_MphiResD[0].arMPhiCruve[41].dCurvature = 15.124;
	m_MphiResD[0].arMPhiCruve[42].dCurvature = 15.853;
	m_MphiResD[0].arMPhiCruve[43].dCurvature = 16.599;
	m_MphiResD[0].arMPhiCruve[44].dCurvature = 17.362;
	m_MphiResD[0].arMPhiCruve[45].dCurvature = 18.142;
	m_MphiResD[0].arMPhiCruve[46].dCurvature = 18.939;
	m_MphiResD[0].arMPhiCruve[47].dCurvature = 19.754;
	m_MphiResD[0].arMPhiCruve[48].dCurvature = 20.586;
	m_MphiResD[0].arMPhiCruve[49].dCurvature = 21.434;

	m_MphiResD[0].StateD.dCurvature[0] = 0.03414;
	m_MphiResD[0].StateD.dMoment[0] = 256.502;

	m_MphiResD[0].StateD.dCurvature[1] = 1.593753;
	m_MphiResD[0].StateD.dMoment[1] = 3555.659;

	m_MphiResD[0].StateD.dCurvature[2] = 12.626;
	m_MphiResD[0].StateD.dMoment[2] = 5261.715;

	m_MphiResD[0].StateD.dCurvature[3] = 20.255;
	m_MphiResD[0].StateD.dMoment[3] = 5737.197;

	m_MphiResD[0].StateD.dCurvature[4] = 2.339646;
	m_MphiResD[0].StateD.dMoment[4] = 5219.745;

	m_MphiResD[0].StateD.dCurvature[5] = 2.339646;
	m_MphiResD[0].StateD.dMoment[5] = 5219.745;
	
	m_CurveOpt.Initialize();
	m_CurveOpt.dAxialForce = 1000.0;
	m_CurveOpt.bIdealize = TRUE;
	m_CurveOpt.bCurvature = FALSE;
	m_CurveOpt.dCurvature = 4.0;

	m_CurveOpt.nAngleDir = m_nAngleDir;
	m_CurveOpt.nCurveType = m_cmbCurve.GetCurSel();
}


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiCurveDlg implementation functions
BEGIN_MESSAGE_MAP(CDgnSeisMPhiCurveDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisMPhiCurveDlg)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_CURVE_LONGI_RDO,           OnDgnSeisMPhiAngleDirRdo)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_CURVE_TRANS_RDO,           OnDgnSeisMPhiAngleDirRdo)
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_MPHI_CURVE_TYPE_CMB,         OnDgnSeisMPhiCurveCmb)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_CURVE_SECT_EFF_STIFF_BTN,  OnDgnSeisMPhiSectEffStiffBtn)
	//ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_CURVE_BROWSER_BTN,         OnDgnSeisMPhiBrowserBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_CURVE_OUTPUT_BTN,          OnDgnSeisMPhiOutputBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_CURVE_CLOSE_BTN,           OnDgnSeisMPhiCloseBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_CURVE_EXPORT_BTN,          OnDgnSeisMPhiExportBtn)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiCurveDlg message handlers

BOOL CDgnSeisMPhiCurveDlg::OnInitDialog() 
{
	if (m_pGrid == nullptr)
		m_pGrid = new CDgnSeisMPhiCurveGrid();

	CDialogMove::OnInitDialog();

	T_BSED_D BsedD; BsedD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD);

	if     (BsedD.nDesignCode == KISTEC_2015) m_pSeisEval = new CBrdgSeisEvalKISTEC2015;
    else if(BsedD.nDesignCode == KISTEC_2019) m_pSeisEval = new CBrdgSeisEvalKISTEC2019;
	else if(BsedD.nDesignCode == KALIS_2023)  m_pSeisEval = new CBrdgSeisEvalKALIS2023;
	else if(BsedD.nDesignCode == KEC_2012)    m_pSeisEval = new CBrdgSeisEvalKEC2012;
	else                                      m_pSeisEval = new CBrdgSeisEvalKISTEC2015;
	
	InitUnit();
	InitCombo();
	InitGrid();
	InitView();
	InitControl();  

	EnableDisableControls();

	GetCurveOpt();
	
	Data2Dlg();

	if(m_bSetBmp)
	{    
		m_pGraph->MakeBmp(m_strBmpPath);
		PostMessage(WM_CLOSE, 0, 0);

		if(m_pGraph)
		{
			delete m_pGraph;
			m_pGraph = nullptr;
		}
		::Sleep(1000);
	}  

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisMPhiCurveDlg::OnDgnSeisMPhiAngleDirRdo()
{
	GetCurveOpt();

	m_pGrid->SetData2Grid(m_CurveOpt.nAngleDir, m_MphiResD);

	m_pGraph->SetCurveOpt(&m_CurveOpt);

	m_pGraph->DrawGraph();
	m_pGraph->Invalidate();
}

void CDgnSeisMPhiCurveDlg::OnDgnSeisMPhiCurveCmb()
{
	GetCurveOpt();
	m_pGraph->SetCurveOpt(&m_CurveOpt);

	m_pGraph->DrawGraph();
	m_pGraph->Invalidate();
}

void CDgnSeisMPhiCurveDlg::OnDgnSeisMPhiSectEffStiffBtn()
{
	T_SECF_D SecfD; SecfD.Initialize();
	SecfD.dScaleFactor[4] = m_edtIyL.GetEditValue();
	SecfD.dScaleFactor[5] = m_edtIyT.GetEditValue();

	if(!m_pDoc->m_pDataCtrl->AddSecf(m_Data.SectK, SecfD)) { ASSERT(0); return; }
}

void CDgnSeisMPhiCurveDlg::OnDgnSeisMPhiExportBtn()
{
	T_BRDGEVAL_MPHI_RES MphiResD2[2];
	if (!Get_MPhiResult(m_Data, MphiResD2, !m_bSetBmp, 180.0)) return;

	CDgnSeisMPhiExportDlg dlg;
	dlg.SetParamData(m_Data, m_MphiResD, MphiResD2);
	if (dlg.DoModal() == IDOK)
	{

	}
}

void CDgnSeisMPhiCurveDlg::OnDgnSeisMPhiBrowserBtn()
{
    CString strPath;
    m_edtFileName.GetWindowText(strPath);
	CFileDialog dlg(FALSE, NULL, strPath, OFN_HIDEREADONLY, _LS(IDS_DGN_SEIS_MPCC_ALL_FILE));
	if(dlg.DoModal()==IDOK) 
	{
		CString strPath;
		strPath = dlg.GetPathName();
		m_edtFileName.SetWindowText(strPath);

		UpdateData(FALSE); 
	}
}

void CDgnSeisMPhiCurveDlg::OnDgnSeisMPhiOutputBtn()
{ 
	// Set MPCC Data
	CArray<T_MPCC_D, T_MPCC_D&> arMpccD;
	arMpccD.RemoveAll();
	arMpccD.Add(m_Data);
	if(m_Data.bEvgp) arMpccD[0].dAxisAngle = 0.0;
	arMpccD[0].MatlConcK1Trans = 0;
	arMpccD[0].MatlConcK2Trans = 0;

	arMpccD.Add(m_Data);
	if(m_Data.bEvgp) arMpccD[1].dAxisAngle  = 90.0;
	else             arMpccD[1].dAxisAngle += 90.0;
	arMpccD[1].MatlConcK1  = arMpccD[1].MatlConcK1Trans;
	arMpccD[1].MatlConcK2  = arMpccD[1].MatlConcK2Trans;

	CString strPath;
	m_edtFileName.GetWindowText(strPath);

	if(!m_pSeisEval->Print_MPhiResult(strPath, arMpccD))
		ASSERT(0);
}

void CDgnSeisMPhiCurveDlg::OnDgnSeisMPhiCloseBtn()
{
	if(m_pGrid) { delete m_pGrid; m_pGrid = NULL; }
	CDialogMove::OnCancel();
}