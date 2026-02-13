// DgnSeisMPhiExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisMPhiExportDlg.h"

#include "DgnMPhiCurveConditionDlg.h"
#include "DgnSeisMPhiAutoAxialForceDlg.h"
#include "DgnSeisMPhiCurveDlg.h"
#include "DgnSeisMPhiIdealGuideDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

//#include "..\dgndb\src\GSD_DataBase\GSD_DB_ST_PROP.h"
//#include "../wg_main/wg_main.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiExportDlg dialog

CDgnSeisMPhiExportDlg::CDgnSeisMPhiExportDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisMPhiExportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisMPhiExportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bPhgt   = FALSE;
	m_bIehp = FALSE;
	m_bMyPhgt = FALSE;
	m_bMzPhgt = FALSE;
	m_bMyIehp = FALSE;
	m_bMzIehp = FALSE;
	m_Data.Initialize();

	m_pDoc = CDBDoc::GetDocPoint();

	m_aCtrlMyIehp.RemoveAll();
	m_aCtrlMyIehp.Add(IDC_DGN_SEIS_MPHI_EXPORT_IEHP_HLENG_MY_EDT);
	m_aCtrlMyIehp.Add(IDC_DGN_SEIS_MPHI_EXPORT_IEHP_HLENG_MY_UNT);

	m_aCtrlMzIehp.RemoveAll();
	m_aCtrlMzIehp.Add(IDC_DGN_SEIS_MPHI_EXPORT_IEHP_HLENG_MZ_EDT);
	m_aCtrlMzIehp.Add(IDC_DGN_SEIS_MPHI_EXPORT_IEHP_HLENG_MZ_UNT);

	m_aCtrlMyPhgt.RemoveAll();
	m_aCtrlMyPhgt.Add(IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLOC_MY_CMB);
	m_aCtrlMyPhgt.Add(IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLENG_I_MY_EDT);
	m_aCtrlMyPhgt.Add(IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLENG_J_MY_EDT);

	m_aCtrlMzPhgt.RemoveAll();
	m_aCtrlMzPhgt.Add(IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLOC_MZ_CMB);
	m_aCtrlMzPhgt.Add(IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLENG_I_MZ_EDT);
	m_aCtrlMzPhgt.Add(IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLENG_J_MZ_EDT);

}


void CDgnSeisMPhiExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisMPhiExportDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_EXPORT_NAME_EDT, m_edtName);
	DDX_Check(pDX, IDC_DGN_SEIS_MPHI_EXPORT_IEHP_CHK, m_bIehp);
	DDX_Check(pDX, IDC_DGN_SEIS_MPHI_EXPORT_PHGT_CHK, m_bPhgt);

	DDX_Check(pDX, IDC_DGN_SEIS_MPHI_EXPORT_IEHP_COMPO_MY_CHK,   m_bMyIehp);
	DDX_Check(pDX, IDC_DGN_SEIS_MPHI_EXPORT_IEHP_COMPO_MZ_CHK,   m_bMzIehp);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_EXPORT_IEHP_HLENG_MY_EDT, m_edtMyIehp);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_EXPORT_IEHP_HLENG_MY_UNT, m_untMyIehp);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_EXPORT_IEHP_HLENG_MZ_EDT, m_edtMzIehp);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_EXPORT_IEHP_HLENG_MZ_UNT, m_untMzIehp);

	DDX_Check(pDX, IDC_DGN_SEIS_MPHI_EXPORT_PHGT_COMPO_MY_CHK, m_bMyPhgt);
	DDX_Check(pDX, IDC_DGN_SEIS_MPHI_EXPORT_PHGT_COMPO_MZ_CHK, m_bMzPhgt);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLOC_MY_CMB, m_CmbMyPhgtLoc);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLOC_MZ_CMB, m_CmbMzPhgtLoc);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLENG_I_MY_EDT, m_edtPhgtMy_I);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLENG_I_MZ_EDT, m_edtPhgtMz_I);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLENG_J_MY_EDT, m_edtPhgtMy_J);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLENG_J_MZ_EDT, m_edtPhgtMz_J);

	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_EXPORT_REAF_EDT, m_edtMMy);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_EXPORT_FCAF_EDT, m_edtDDy);


	//}}AFX_DATA_MAP
}

void CDgnSeisMPhiExportDlg::InitUnit()
{
	m_edtMyIehp.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMyIehp.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtMzIehp.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMzIehp.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtPhgtMy_I.SetUnitType(D_UNITSYS_NONE);
	m_edtPhgtMz_I.SetUnitType(D_UNITSYS_NONE);
	m_edtPhgtMy_J.SetUnitType(D_UNITSYS_NONE);
	m_edtPhgtMz_J.SetUnitType(D_UNITSYS_NONE);
	m_edtMMy.SetUnitType(D_UNITSYS_NONE);
	m_edtDDy.SetUnitType(D_UNITSYS_NONE);

}

void CDgnSeisMPhiExportDlg::InitCombo()
{
	m_CmbMyPhgtLoc.ResetContent();
	CDlgUtil::CobxAddItem(m_CmbMyPhgtLoc, _LS(IDS_DGN_SEIS_EXPORT_LOC_I_END), 0);
	CDlgUtil::CobxAddItem(m_CmbMyPhgtLoc, _LS(IDS_DGN_SEIS_EXPORT_LOC_J_END), 1);
	CDlgUtil::CobxAddItem(m_CmbMyPhgtLoc, _LS(IDS_DGN_SEIS_EXPORT_LOC_IJ_END), 2);
	CDlgUtil::CobxAdjustListBoxWidth(m_CmbMyPhgtLoc);
	m_CmbMyPhgtLoc.SetCurSel(0);

	m_CmbMzPhgtLoc.ResetContent();
	CDlgUtil::CobxAddItem(m_CmbMzPhgtLoc, _LS(IDS_DGN_SEIS_EXPORT_LOC_I_END), 0);
	CDlgUtil::CobxAddItem(m_CmbMzPhgtLoc, _LS(IDS_DGN_SEIS_EXPORT_LOC_J_END), 1);
	CDlgUtil::CobxAddItem(m_CmbMzPhgtLoc, _LS(IDS_DGN_SEIS_EXPORT_LOC_IJ_END), 2);
	CDlgUtil::CobxAdjustListBoxWidth(m_CmbMzPhgtLoc);
	m_CmbMzPhgtLoc.SetCurSel(0);

}


void CDgnSeisMPhiExportDlg::Data2Dlg()
{
	m_edtName.SetWindowText(m_Data.strName);
	m_bIehp   = m_Data.bIehp;
	m_bPhgt   = m_Data.bPhgt;
	m_bMyIehp = m_Data.bMyIehp;
	m_bMzIehp = m_Data.bMzIehp;
	m_bMyPhgt = m_Data.bMyPhgt;
	m_bMzPhgt = m_Data.bMzPhgt;

	CDlgUtil::CobxSetCurSelItemData(m_CmbMyPhgtLoc, m_Data.nMyLocPhgt);
	CDlgUtil::CobxSetCurSelItemData(m_CmbMzPhgtLoc, m_Data.nMzLocPhgt);

	double d1M = M_InitValueCurUnit(1.0, N, M, D_UNITSYS_BASE_LENGTH);
	m_edtMyIehp.SetEditUnit(d1M/*m_Data.dMyIehpLp*/);
	m_edtMzIehp.SetEditUnit(d1M/*m_Data.dMzIehpLp*/);
	m_edtPhgtMy_I.SetEditUnit(m_Data.dMyPhgt_I);
	m_edtPhgtMy_J.SetEditUnit(m_Data.dMyPhgt_J);
	m_edtPhgtMz_I.SetEditUnit(m_Data.dMzPhgt_I);
	m_edtPhgtMz_J.SetEditUnit(m_Data.dMzPhgt_J);

	m_edtMMy.SetEditUnit(m_Data.dMMy);
	m_edtDDy.SetEditUnit(m_Data.dDDy);

	UpdateData(FALSE);
}

BOOL CDgnSeisMPhiExportDlg::Dlg2Data()
{
	UpdateData(TRUE);

	if (!CheckData()) return FALSE;

	m_edtName.GetWindowText(m_Data.strName);
	m_Data.bIehp   = m_bIehp;
	m_Data.bPhgt   = m_bPhgt;
	m_Data.bMyIehp = m_bMyIehp;
	m_Data.bMzIehp = m_bMzIehp;
	m_Data.bMyPhgt = m_bMyPhgt;
	m_Data.bMzPhgt = m_bMzPhgt;

	m_Data.nMyLocPhgt = m_CmbMyPhgtLoc.GetItemData(m_CmbMyPhgtLoc.GetCurSel());
	m_Data.nMzLocPhgt = m_CmbMzPhgtLoc.GetItemData(m_CmbMzPhgtLoc.GetCurSel());

	m_Data.dMyIehpLp = m_edtMyIehp.GetEditValue();
	m_Data.dMzIehpLp = m_edtMzIehp.GetEditValue();
	m_Data.dMyPhgt_I = m_edtPhgtMy_I.GetEditValue();
	m_Data.dMyPhgt_J = m_edtPhgtMy_J.GetEditValue();
	m_Data.dMzPhgt_I = m_edtPhgtMz_I.GetEditValue();
	m_Data.dMzPhgt_J = m_edtPhgtMz_J.GetEditValue();
	m_Data.dMMy = m_edtMMy.GetEditValue();
	m_Data.dDDy = m_edtDDy.GetEditValue();


	T_PHGT_D PhgtD_Org;
	MakePhgtData(PhgtD_Org);

	if (m_bPhgt)
	{
		T_PHGT_D PhgtD;
		PhgtD = PhgtD_Org;

		if (!m_Data.bMyPhgt)  PhgtD.bDOF[4] = FALSE;
		if (!m_Data.bMzPhgt)  PhgtD.bDOF[5] = FALSE;

		if (!m_pDoc->m_pDataCtrl->AddPhgt(PhgtD)) { ASSERT(0); return FALSE; }
	}

	T_IEHP_D IehpD;
	if (m_bPhgt)
	{
		IehpD.Initialize();

		T_PHGT_D PhgtD;
		PhgtD = PhgtD_Org;

		// 이름
		if (!m_pDoc->m_pAttrCtrl->ExistIehp((CString)PhgtD.strName))
		{
			IehpD.strName = PhgtD.strName;
		}
		else
		{
			CString strNameTemp;
			int nCopyNameCount = 1;
			while (TRUE)
			{
				strNameTemp.Format(_T("%s-%d"), PhgtD.strName, nCopyNameCount++);
				if (!m_pDoc->m_pAttrCtrl->ExistIehp(strNameTemp))
					break;
			}
			IehpD.strName = strNameTemp;
		}

		//if (!m_pDoc->m_pAttrCtrl->ConvertPhgt2Iehp(PhgtD, IehpD)) { ASSERT(0); return FALSE; }

		for(int nDof=4; nDof<=5; nDof++)
		{
			double dIehpLp = 0.0;
			if(nDof == 4) 
			{
				if (!m_Data.bMyIehp) continue;
				nDof = 4;
				dIehpLp = m_Data.dMyIehpLp;
			}
			if (nDof == 5)
			{
				if (!m_Data.bMzIehp)  continue;;
				nDof = 5;
				dIehpLp = m_Data.dMzIehpLp;
			}

			PhgtD.Skcv[nDof].FEMA.dRotation[0] *= dIehpLp;
			PhgtD.Skcv[nDof].FEMA.dRotation[1] *= dIehpLp;
			PhgtD.Skcv[nDof].FEMA.aFEMASub[0].dRotationId[0] *= dIehpLp;
			PhgtD.Skcv[nDof].FEMA.aFEMASub[0].dRotationId[1] *= dIehpLp;

			IehpD.cDir[nDof] = '1';
			IehpD.nSectionNum[nDof] = 3;
			IehpD.nHingeLocation[nDof] = 2;
			IehpD.nHysModel[nDof] = D_IEHP_FBIL;
			
			T_IEHP_D IehpDTemp;
			if (!m_pDoc->m_pAttrCtrl->Convert_PhgtToIehp_FEMA(nDof, PhgtD, IehpDTemp)) { ASSERT(0); return FALSE; }
			IehpD.AllProp.PROP[nDof].FEMA = IehpDTemp.AllProp.PROP[nDof].FEMA;
			IehpD.AllSubProp.PROP[nDof].FEMA = IehpDTemp.AllSubProp.PROP[nDof].FEMA;
		}

		if (!m_pDoc->m_pDataCtrl->AddIehp(IehpD)) { ASSERT(0); return FALSE; }
	}

	return TRUE;
}

BOOL CDgnSeisMPhiExportDlg::CheckData()
{
	//  GSD의 CCmdExportMPhiDlg::CheckErrorPre 참고
	CString strError;

	CString strName;
	m_edtName.GetWindowText(strName);

	if (strName.GetLength() == 0) { strError.Format(_T("[Error] : There is no Name. ")); AfxMessageBox(strError); return FALSE; }

	// IEHP
	if(m_bIehp)
	{
		if (!m_Data.bMyIehp && !m_Data.bMzIehp) { strError.Format(_T("[Error] : There is no data. ")); AfxMessageBox(strError); return FALSE; }

		if (m_Data.bMyIehp)
		{
			if (m_Data.dMyIehpLp <= 0 ) { strError.Format(_T("[Error] :  Lp <= 0 ")); AfxMessageBox(strError); return FALSE; }
		}
		if (m_Data.bMzIehp)
		{
			if (m_Data.dMzIehpLp <= 0) { strError.Format(_T("[Error] :  Lp <= 0 ")); AfxMessageBox(strError); return FALSE; }
		}
	}

	// PHGT
	if (m_bPhgt)
	{
		if (!m_Data.bMyPhgt && !m_Data.bMzPhgt) { strError.Format(_T("[Error] : There is no data. ")); AfxMessageBox(strError); return FALSE; }

		if (m_Data.bMyPhgt)
		{
			switch (m_Data.nMyLocPhgt)
			{
			case (MPHI_I_end):
				if (0 >= m_Data.dMyPhgt_I || 0.5 <= m_Data.dMyPhgt_I) { strError.Format(_T("[Error] :  Lp/L <= 0, Lp/L >= 0.5 ")); AfxMessageBox(strError); return FALSE; }
				break;
			case MPHI_J_end:
				if (0 >= m_Data.dMyPhgt_J || 0.5 <= m_Data.dMyPhgt_J) { strError.Format(_T("[Error] :  Lp/L <= 0, Lp/L >= 0.5 ")); AfxMessageBox(strError); return FALSE; }
				break;
			case MPHI_IJ_end:
				if (0 >= m_Data.dMyPhgt_I + m_Data.dMyPhgt_J || 1.0 <= (m_Data.dMyPhgt_I + m_Data.dMyPhgt_J))
				{
					strError.Format(_T("[Error] :  Lp/L <= 0, Lp/L >= 1.0 ")); AfxMessageBox(strError); return FALSE;
				}
				break;
			default: ASSERT(0);
				break;
			}
		}
		if (m_Data.bMzPhgt)
		{
			switch (m_Data.nMzLocPhgt)
			{
			case (MPHI_I_end):
				if (0 >= m_Data.dMzPhgt_I || 0.5 <= m_Data.dMzPhgt_I) { strError.Format(_T("[Error] :  Lp/L <= 0, Lp/L >= 0.5 ")); AfxMessageBox(strError); return FALSE; }
				break;
			case MPHI_J_end:
				if (0 >= m_Data.dMzPhgt_J || 0.5 <= m_Data.dMzPhgt_J) { strError.Format(_T("[Error] :  Lp/L <= 0, Lp/L >= 0.5 ")); AfxMessageBox(strError); return FALSE; }
				break;
			case MPHI_IJ_end:
				if (0 >= m_Data.dMzPhgt_I + m_Data.dMzPhgt_J || 1.0 <= (m_Data.dMzPhgt_I + m_Data.dMzPhgt_J))
				{
					strError.Format(_T("[Error] :  Lp/L <= 0, Lp/L >= 1.0 ")); AfxMessageBox(strError); return FALSE;
				}
				break;
			default: ASSERT(0);
				break;
			}
		}
	}


	if (m_Data.dMMy < 0) { strError.Format(_T("[Error] :  M/My < 0")); AfxMessageBox(strError); return FALSE; }
	if (m_Data.dMMy > 1) { strError.Format(_T("[Error] :  M/My > 1")); AfxMessageBox(strError); return FALSE; }
	if (m_Data.dDDy <= 1) { strError.Format(_T("[Error] :  D/Dy <= 1")); AfxMessageBox(strError); return FALSE; }

	return TRUE;
}

void CDgnSeisMPhiExportDlg::MakePhgtData(T_PHGT_D& PhgtD)
{
	PhgtD.Initialize();

	DGN_SEiS_MPHI_FEMA FEMA;
	FEMA.Initialize();

	FEMA.dAxialForce = m_MpccData.dAxialForce;
	// 0:Crack, 1:Yield(init.), 2:Yield, 3:Ultimate, 4:Yield(ideal), 5:Ultimate(ideal)

	// FEMA : D/DY
	FEMA.dDDy[0][0] = -(m_MphiResD2[0].StateD.dCurvature[5] / m_MphiResD2[0].StateD.dCurvature[4]) * 1.50; // -E  // -m_Data.dDDy    
	FEMA.dDDy[0][1] = -(m_MphiResD2[0].StateD.dCurvature[5] / m_MphiResD2[0].StateD.dCurvature[4]) * 1.05; // -D
	FEMA.dDDy[0][2] = -(m_MphiResD2[0].StateD.dCurvature[5] / m_MphiResD2[0].StateD.dCurvature[4]);        // -C
	FEMA.dDDy[0][3] = -1;       // -B
	FEMA.dDDy[0][4] = 0;        //  A
	FEMA.dDDy[0][5] = 1;        //  B
	FEMA.dDDy[0][6] = m_MphiResD1[0].StateD.dCurvature[5] / m_MphiResD1[0].StateD.dCurvature[4];         //  C
	FEMA.dDDy[0][7] = m_MphiResD1[0].StateD.dCurvature[5] / m_MphiResD1[0].StateD.dCurvature[4] * 1.05;  //  D
	FEMA.dDDy[0][8] = m_MphiResD1[0].StateD.dCurvature[5] / m_MphiResD1[0].StateD.dCurvature[4] * 1.50;  //  E  // m_Data.dDDy

	FEMA.dDDy[1][0] = -(m_MphiResD2[1].StateD.dCurvature[5] / m_MphiResD2[1].StateD.dCurvature[4]) * 1.50; // -E  // -m_Data.dDDy 
	FEMA.dDDy[1][1] = -(m_MphiResD2[1].StateD.dCurvature[5] / m_MphiResD2[1].StateD.dCurvature[4]) * 1.05; // -D
	FEMA.dDDy[1][2] = -(m_MphiResD2[1].StateD.dCurvature[5] / m_MphiResD2[1].StateD.dCurvature[4]);        // -C
	FEMA.dDDy[1][3] = -1;       // -B
	FEMA.dDDy[1][4] = 0;        //  A
	FEMA.dDDy[1][5] = 1;        //  B
	FEMA.dDDy[1][6] = m_MphiResD1[1].StateD.dCurvature[5] / m_MphiResD1[1].StateD.dCurvature[4];          //  C
	FEMA.dDDy[1][7] = m_MphiResD1[1].StateD.dCurvature[5] / m_MphiResD1[1].StateD.dCurvature[4] * 1.05;   //  D
	FEMA.dDDy[1][8] = m_MphiResD1[1].StateD.dCurvature[5] / m_MphiResD1[1].StateD.dCurvature[4] * 1.50;   //  E  // m_Data.dDDy   

   //////////////////////////////////////////////////////////////////////////
	// D/Dy 변경
	double dDDymax = -1.0e8;
	double dDDymin = 1.0e8;
		if (dDDymin > FEMA.dDDy[0][0]) dDDymin = FEMA.dDDy[0][0];  // -E 
		if (dDDymax < FEMA.dDDy[0][8]) dDDymax = FEMA.dDDy[0][8];  //  E 
		if (dDDymin > FEMA.dDDy[1][0]) dDDymin = FEMA.dDDy[1][0];  // -E 
		if (dDDymax < FEMA.dDDy[1][8]) dDDymax = FEMA.dDDy[1][8];  //  E 

	BOOL bUseDDy = FALSE;
	double dChangeDDy = max(fabs(dDDymin), dDDymax);

	if (dChangeDDy < m_Data.dDDy) bUseDDy = TRUE;

	if (!bUseDDy)
	{
		AfxMessageBox(_T("D/Dy has been changed to %g."), dChangeDDy);
	}
	FEMA.dDDy[0][0] = (bUseDDy) ? -m_Data.dDDy : -dChangeDDy;
	FEMA.dDDy[0][8] = (bUseDDy) ?  m_Data.dDDy :  dChangeDDy;
	FEMA.dDDy[1][0] = (bUseDDy) ? -m_Data.dDDy : -dChangeDDy;
	FEMA.dDDy[1][8] = (bUseDDy) ?  m_Data.dDDy :  dChangeDDy;
	//////////////////////////////////////////////////////////////////////////

	// FEMA : M/MY
	for (int i = 0; i < 2; i++)
	{
		FEMA.dMMy[i][0] = -m_Data.dMMy; // -E
		FEMA.dMMy[i][1] = -m_Data.dMMy; // -D
		FEMA.dMMy[i][2] = -(m_MphiResD2[i].StateD.dMoment[5] / m_MphiResD2[i].StateD.dMoment[4]); // -C
		FEMA.dMMy[i][3] = -1; // -B
		FEMA.dMMy[i][4] = 0; //  A
		FEMA.dMMy[i][5] = 1; //  B
		FEMA.dMMy[i][6] = m_MphiResD1[i].StateD.dMoment[5] / m_MphiResD1[i].StateD.dMoment[4]; ; //  C
		FEMA.dMMy[i][7] = m_Data.dMMy; //  D
		FEMA.dMMy[i][8] = m_Data.dMMy; //  E
	}

	for (int i = 0; i < 2; i++)
	{
		// dDDy should be incremental.
		for (int j = 0; j < MPHI_POINT_NUM - 1; j++)
		{
			if (FEMA.dDDy[i][j] > FEMA.dDDy[i][j + 1]) { /*GMsgOutFormatNF(_LS(IDS_CMD_MPHI_DDY_INCREMENTAL));*/ return; }
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// CurvePointDYAxis1 : Code Unit
	// GSD에서 계산된 aFEMA[i].dMY, aFEMA[i].dDY 는 각 축력에 대한 항복모멘트, 항복곡률을 의미하지만
	// Civil에서는 P=0일때의 항복모멘트(MY0)와 항복곡률(DY0)로 사용된다. 
	// 따라서 아래의 값들을 항복모멘트(MY0)와 항복곡률(DY0)로 적용한다.
	// 추후 다시 개선할때 변수 추가해서 정리하자.
	// dMY0[4] 변수와 매칭되도록 하면 좋을듯..
	// Civil 에서 CGSDDataCtrl::ImportMPhiFromGSDAndMakeHinge 함께 확인
	// => 축력 하나일땐 그 축력에 대한 결과임.
	FEMA.dMY[0][0] = m_MphiResD1[0].StateD.dMoment[4];   //   0' : Y-Axis(+)
	FEMA.dDY[0][0] = m_MphiResD1[0].StateD.dCurvature[4];
	FEMA.dMY[1][0] = m_MphiResD1[1].StateD.dMoment[4];   //  90' : Z-Axis(+)
	FEMA.dDY[1][0] = m_MphiResD1[1].StateD.dCurvature[4];
	FEMA.dMY[0][1] = m_MphiResD2[0].StateD.dMoment[4];    // 180' : Y-Axis(-)
	FEMA.dDY[0][1] = m_MphiResD2[0].StateD.dCurvature[4];
	FEMA.dMY[1][1] = m_MphiResD2[1].StateD.dMoment[4];    // 270' : Y-Axis(-)
	FEMA.dDY[1][1] = m_MphiResD2[1].StateD.dCurvature[4];

	// Current Unit -> N/m 
// 	double dLen = M_InitValueCurUnit(1.0, N, M, D_UNITSYS_BASE_LENGTH);
// 
// 	FEMA.dAxialForce *= m_FactorForce;
// 	for (int i = 0; i < 2; i++)
// 		for (int j = 0; j < 2; j++)
// 		{
// 			FEMA.dMY[i][j] *= m_FactorMoment;
// 			FEMA.dDY[i][j] /= dLen;
// 		}
// 
// 	return TRUE;
	//////////////////////////////////////////////////////////////////////////

	PhgtD.strName = m_Data.strName;
	PhgtD.nElemType = 0;
	PhgtD.nMaterial = 0;
	PhgtD.nDefinition = 1;
	PhgtD.nInteractionType = 0; // 0:None, 1:P-M, 2:P-M-M
	PhgtD.bConsiderHingeLength = TRUE;

	const int MY = 4;
	const int MZ = 5;
	//if (m_Data.bMyPhgt)
	{
		PhgtD.bDOF[MY] = TRUE;
		PhgtD.nLoc[MY] = m_Data.nMyLocPhgt + 1;
		PhgtD.nSkcvType[MY] = D_SKCV_FEMA;
		PhgtD.dLengthRatio_I[MY] = m_Data.dMyPhgt_I;
		PhgtD.dLengthRatio_J[MY] = m_Data.dMyPhgt_J;
		PhgtD.Skcv[MY].nCalcMethod = D_PHGT_CALC_USER;
		PhgtD.Skcv[MY].bSymmetricIJ = TRUE;
		PhgtD.Skcv[MY].FEMA.bSymmetric = FALSE;

		// GSD에서 계산된 aFEMA[i].dMY, aFEMA[i].dDY 는 각 축력에 대한 항복모멘트, 항복곡률을 의미하지만
		// Civil에서는 P=0일때의 항복모멘트(MY0)와 항복곡률(DY0)로 사용된다. 
		// 따라서 아래의 값들을 항복모멘트(MY0)와 항복곡률(DY0)로 적용한다.
		// 추후 다시 개선할때 변수 추가해서 정리하자
		// dMY0[4] 변수와 매칭되도록 하면 좋을듯..
		// GSD 에서 CCmdExportMPhiDlg::MakeFEMACurveByAxialForce 함께 확인
		 
		PhgtD.Skcv[MY].FEMA.bUserDefineYieldDeform = TRUE;
		PhgtD.Skcv[MY].FEMA.dYieldMoment[0] = FEMA.dMY[0][0];
		PhgtD.Skcv[MY].FEMA.dYieldMoment[1] = FEMA.dMY[0][1];
		PhgtD.Skcv[MY].FEMA.dRotation[0]    = FEMA.dDY[0][0];
		PhgtD.Skcv[MY].FEMA.dRotation[1]    = FEMA.dDY[0][1];

		for (int i = 0; i < MPHI_POINT_NUM; i++)
		{
			PhgtD.Skcv[MY].FEMA.dForceMomentRatio[0][i] = FEMA.dDDy[0][i];
			PhgtD.Skcv[MY].FEMA.dForceMomentRatio[1][i] = FEMA.dMMy[0][i];
		}

		int nSize = PhgtD.Skcv[MY].FEMA.nUseSubCount = 1; // MPhiExpData.aFEMA.GetSize();
		T_PHGT_FEMA_SUB sub;
		for (int i = 1; i < nSize; i++) PhgtD.Skcv[MY].FEMA.aFEMASub.Add(sub);

		for (int i = 0; i < nSize; i++)
		{
			//
			PhgtD.Skcv[MY].FEMA.aFEMASub[i].dYieldMomentId[0] = FEMA.dMY[0][0];
			PhgtD.Skcv[MY].FEMA.aFEMASub[i].dYieldMomentId[1] = FEMA.dMY[0][1];
			PhgtD.Skcv[MY].FEMA.aFEMASub[i].dRotationId[0]    = FEMA.dDY[0][0];
			PhgtD.Skcv[MY].FEMA.aFEMASub[i].dRotationId[1]    = FEMA.dDY[0][1];
			//
			PhgtD.Skcv[MY].FEMA.aFEMASub[i].dAxialForce = FEMA.dAxialForce;
			for (int j = 0; j < MPHI_POINT_NUM; j++)
			{
				PhgtD.Skcv[MY].FEMA.aFEMASub[i].dForceMomentRatio[0][j] = FEMA.dDDy[0][j];
				PhgtD.Skcv[MY].FEMA.aFEMASub[i].dForceMomentRatio[1][j] = FEMA.dMMy[0][j];
			}
		}
		PhgtD.Skcv[MY].FEMA.bMultiCurve = PhgtD.Skcv[MY].FEMA.nUseSubCount > 1 ? TRUE : FALSE;  // MultiCurve 개수가 1이면 FALSE
	}

	//if (m_Data.bMzPhgt)
	{
		PhgtD.bDOF[MZ] = TRUE;
		PhgtD.nLoc[MZ] = m_Data.nMzLocPhgt + 1;
		PhgtD.nSkcvType[MZ] = D_SKCV_FEMA;
		PhgtD.dLengthRatio_I[MZ] = m_Data.dMzPhgt_I;
		PhgtD.dLengthRatio_J[MZ] = m_Data.dMzPhgt_J;
		PhgtD.Skcv[MZ].nCalcMethod = D_PHGT_CALC_USER;
		PhgtD.Skcv[MZ].bSymmetricIJ = TRUE;
		PhgtD.Skcv[MZ].FEMA.bSymmetric = FALSE;

		PhgtD.Skcv[MZ].FEMA.bUserDefineYieldDeform = TRUE;
		PhgtD.Skcv[MZ].FEMA.dYieldMoment[0] = FEMA.dMY[1][0];
		PhgtD.Skcv[MZ].FEMA.dYieldMoment[1] = FEMA.dMY[1][1];
		PhgtD.Skcv[MZ].FEMA.dRotation[0]    = FEMA.dDY[1][0];
		PhgtD.Skcv[MZ].FEMA.dRotation[1]    = FEMA.dDY[1][1];

		for (int i = 0; i < MPHI_POINT_NUM; i++)
		{
			PhgtD.Skcv[MZ].FEMA.dForceMomentRatio[0][i] = FEMA.dDDy[1][i];
			PhgtD.Skcv[MZ].FEMA.dForceMomentRatio[1][i] = FEMA.dMMy[1][i];
		}

		int nSize = PhgtD.Skcv[MZ].FEMA.nUseSubCount = 1; // MPhiExpData.aFEMA.GetSize();
		T_PHGT_FEMA_SUB sub;
		for (int i = 1; i < nSize; i++) PhgtD.Skcv[MZ].FEMA.aFEMASub.Add(sub);

		for (int i = 0; i < nSize; i++)
		{
			//
			PhgtD.Skcv[MZ].FEMA.aFEMASub[i].dYieldMomentId[0] = FEMA.dMY[1][0];
			PhgtD.Skcv[MZ].FEMA.aFEMASub[i].dYieldMomentId[1] = FEMA.dMY[1][1];
			PhgtD.Skcv[MZ].FEMA.aFEMASub[i].dRotationId[0]    = FEMA.dDY[1][0];
			PhgtD.Skcv[MZ].FEMA.aFEMASub[i].dRotationId[1]    = FEMA.dDY[1][1];
			//
			PhgtD.Skcv[MZ].FEMA.aFEMASub[i].dAxialForce = FEMA.dAxialForce;
			for (int j = 0; j < MPHI_POINT_NUM; j++)
			{
				PhgtD.Skcv[MZ].FEMA.aFEMASub[i].dForceMomentRatio[0][j] = FEMA.dDDy[1][j];
				PhgtD.Skcv[MZ].FEMA.aFEMASub[i].dForceMomentRatio[1][j] = FEMA.dMMy[1][j];
			}
		}
		PhgtD.Skcv[MZ].FEMA.bMultiCurve = PhgtD.Skcv[MZ].FEMA.nUseSubCount > 1 ? TRUE : FALSE;
	}
}


void CDgnSeisMPhiExportDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_SEIS_MPHI_EXPORT_IEHP_GRP, m_bIehp, TRUE);
	if(m_bIehp)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlMyIehp, m_bMyIehp);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlMzIehp, m_bMzIehp);
	}
	else
	{
 		m_bMyIehp = FALSE;
 		m_bMzIehp = FALSE;
	}

	CDlgUtil::CtrlEnableDisableByRect(this, IDC_DGN_SEIS_MPHI_EXPORT_PHGT_GRP, m_bPhgt, TRUE);
	if(m_bPhgt)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlMyPhgt, m_bMyPhgt);
		if (m_bMyPhgt)
		{
			int nMyLocPhgt = m_CmbMyPhgtLoc.GetItemData(m_CmbMyPhgtLoc.GetCurSel());
			GetDlgItem(IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLENG_I_MY_EDT)->EnableWindow(nMyLocPhgt != MPHI_J_end);
			GetDlgItem(IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLENG_J_MY_EDT)->EnableWindow(nMyLocPhgt != MPHI_I_end);
		}

		CDlgUtil::CtrlEnableDisable(this, m_aCtrlMzPhgt, m_bMzPhgt);
		if (m_bMzPhgt)
		{
			int nMzLocPhgt = m_CmbMzPhgtLoc.GetItemData(m_CmbMzPhgtLoc.GetCurSel());
			GetDlgItem(IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLENG_I_MZ_EDT)->EnableWindow(nMzLocPhgt != MPHI_J_end);
			GetDlgItem(IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLENG_J_MZ_EDT)->EnableWindow(nMzLocPhgt != MPHI_I_end);
		}
	}
	else
	{
 		m_bMyPhgt = FALSE;
 		m_bMzPhgt = FALSE;
	}

	UpdateData(FALSE);  // 변경된 m_bMyIehp 등을 저장 
}


void CDgnSeisMPhiExportDlg::SetParamData(T_MPCC_D &MpccData, T_BRDGEVAL_MPHI_RES MphiResD1[2], T_BRDGEVAL_MPHI_RES MphiResD2[2])
{
	m_MpccData = MpccData; 
	m_MphiResD1[0] = MphiResD1[0];
	m_MphiResD1[1] = MphiResD1[1];
	m_MphiResD2[0] = MphiResD2[0];
	m_MphiResD2[1] = MphiResD2[1];
}


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiExportDlg implementation functions
BEGIN_MESSAGE_MAP(CDgnSeisMPhiExportDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisMPhiExportDlg)
	ON_BN_CLICKED(IDOK, OnOKBtn)
	ON_BN_CLICKED(IDCANCEL, OnCancelBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_EXPORT_APPLY_BTN,         OnApplyBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_EXPORT_IEHP_CHK,          OnIehpChkBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_EXPORT_PHGT_CHK,          OnPhgtChkBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_EXPORT_IEHP_COMPO_MY_CHK, OnMyIehpChkBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_EXPORT_IEHP_COMPO_MZ_CHK, OnMzIehpChkBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_EXPORT_PHGT_COMPO_MY_CHK, OnMyPhgtChkBtn)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_EXPORT_PHGT_COMPO_MZ_CHK, OnMzPhgtChkBtn)

	ON_CBN_SELCHANGE(IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLOC_MY_CMB, OnMyPhgtLocCmb)
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_MPHI_EXPORT_PHGT_HLOC_MZ_CMB, OnMzPhgtLocCmb)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiExportDlg message handlers

BOOL CDgnSeisMPhiExportDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
		
	InitUnit();
	InitCombo();
	Data2Dlg();
	
	EnableDisableControls();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisMPhiExportDlg::OnOKBtn()
{
	if(!Dlg2Data()) return;

// 	if(m_bModify)
// 	{
// 		if(!m_pDoc->m_pAttrCtrl2->ExistMpcc(m_Key))  // ... 버튼으로 연 대화상자에서의 연관관계 처리로 인해 자기자신이 지워지면 Add 해 주어야 함
// 		{
// 			if(!m_pDoc->m_pDataCtrl->AddMpcc(m_Key, m_Data)) return;
// 		}
// 		else
// 		{
// 			if(!m_pDoc->m_pDataCtrl->ModifyMpcc(m_Key, m_Key, m_Data)) return;
// 		}
// 	}
// 	else
// 	{
// 		m_Key = m_pDoc->m_pAttrCtrl2->GetStartNumMpcc();
// 		if(!m_pDoc->m_pDataCtrl->AddMpcc(m_Key, m_Data)) return;
// 	}

	CDialogMove::OnOK();
}

void CDgnSeisMPhiExportDlg::OnCancelBtn()
{
	CDialogMove::OnCancel();
}

void CDgnSeisMPhiExportDlg::OnApplyBtn()
{
	if(!Dlg2Data()) return;

// 	if(m_bModify)
// 	{
// 		if(!m_pDoc->m_pAttrCtrl2->ExistMpcc(m_Key))
// 		{
// 			if(!m_pDoc->m_pDataCtrl->AddMpcc(m_Key, m_Data)) return;
// 		}
// 		else
// 		{
// 			if(!m_pDoc->m_pDataCtrl->ModifyMpcc(m_Key, m_Key, m_Data)) return;
// 		}
// 	}
// 	else
// 	{
// 		m_Key = m_pDoc->m_pAttrCtrl2->GetStartNumMpcc();
// 		if(!m_pDoc->m_pDataCtrl->AddMpcc(m_Key, m_Data)) return;
// 	}
}

void CDgnSeisMPhiExportDlg::OnIehpChkBtn()
{
	EnableDisableControls();
}
void CDgnSeisMPhiExportDlg::OnPhgtChkBtn()
{
	EnableDisableControls();
}
void CDgnSeisMPhiExportDlg::OnMyIehpChkBtn()
{
	EnableDisableControls();
}
void CDgnSeisMPhiExportDlg::OnMzIehpChkBtn()
{
	EnableDisableControls();
}
void CDgnSeisMPhiExportDlg::OnMyPhgtChkBtn()
{
	EnableDisableControls();
}
void CDgnSeisMPhiExportDlg::OnMzPhgtChkBtn()
{
	EnableDisableControls();
}
void CDgnSeisMPhiExportDlg::OnMyPhgtLocCmb()
{
	EnableDisableControls();
}
void CDgnSeisMPhiExportDlg::OnMzPhgtLocCmb()
{
	EnableDisableControls();
}
