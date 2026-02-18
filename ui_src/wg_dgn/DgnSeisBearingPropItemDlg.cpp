// DgnSeisBearingPropItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisBearingPropItemDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisBearingPropItemDlg dialog

CDgnSeisBearingPropItemDlg::CDgnSeisBearingPropItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisBearingPropItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisBearingPropItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_nViewType = 1;
	m_nBearing = 0;
	m_pDoc = CDBDoc::GetDocPoint();

	m_Key = 0;
	m_Data.Initialize();
	m_bModify = FALSE;
	m_bSetEmf = FALSE;
}

void CDgnSeisBearingPropItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisBearingPropItemDlg)
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_NAME_EDT,      m_edtName);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_INSTALL_CMB,   m_cmbInstall);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_ANCHOR_CMB,    m_cmbAnchor);

	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_HEF_EDT,       m_edthef);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_HEF_UNT,       m_unthef);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_FY_EDT,        m_edtfy);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_FY_UNT,        m_untfy);	
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_FUTA_EDT,      m_edtfuta);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_FUTA_UNT,      m_untfuta);	
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_DH_EDT,        m_edtdh);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_DH_UNT,        m_untdh);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_EH_EDT,        m_edteh);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_EH_UNT,        m_unteh);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_NP_EDT,        m_edtNp);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_NP_UNT,        m_untNp);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_BA_EDT,        m_edtBa);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_BA_UNT,        m_untBa);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_HA_EDT,        m_edtHa);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_HA_UNT,        m_untHa);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_SX_EDT,        m_edtSx);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_SX_UNT,        m_untSx);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_N1_EDT,        m_edtN1);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_S1_EDT,        m_edtS1);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_S1_UNT,        m_untS1);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_SY_EDT,        m_edtSy);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_SY_UNT,        m_untSy);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_N2_EDT,        m_edtN2);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_S2_EDT,        m_edtS2);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_S2_UNT,        m_untS2);
	DDX_Check  (pDX, IDC_DGN_SEIS_BEARING_PROP_CONTI_WELD_CHK, m_bContiWeld);	
	DDX_Radio  (pDX, IDC_DGN_SEIS_BEARING_PROP_VIEW_DRAW_RDO, m_nViewType);

	//
	DDX_Radio(pDX, IDC_DGN_SEIS_BEARING_EVAL_BEARING_RDO1, m_nBearing);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_STL_BEARING_CMB, m_cmbStlBearing);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_DA_EDT,     m_edtda);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_DA_UNT,     m_untda);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_DS_EDT,     m_edtds);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_DS_UNT,     m_untds);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_CAPA_L_EDT2, m_edtCapaL);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_CAPA_L_UNT, m_untCapaL);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_CAPA_T_EDT2, m_edtCapaT);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_CAPA_T_UNT, m_untCapaT);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_DISP_L_EDT2, m_edtDispL);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_DISP_L_UNT, m_untDispL);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_DISP_T_EDT2, m_edtDispT);
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_EVAL_DISP_T_UNT, m_untDispT);
	//
	DDX_Control(pDX, IDC_DGN_SEIS_BEARING_PROP_VIEW, m_View);
	DDX_Control(pDX, IDC_DGN_PICTURE, m_wndPicture);

	//}}AFX_DATA_MAP
}

void CDgnSeisBearingPropItemDlg::MakeEmfAndClose(CString strPath)
{
	m_bSetEmf = TRUE;
	m_strPath = strPath;
}

void CDgnSeisBearingPropItemDlg::InitUnit()
{
	//
	m_edtda.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dda);
	m_untda.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dda);
	m_edtds.SetUnitType(CUnitCtrl::m_BREV_UNIT.dds);
	m_untds.SetUnitType(CUnitCtrl::m_BREV_UNIT.dds);
	m_edtCapaL.SetUnitType(CUnitCtrl::m_BREV_UNIT.dCapaL);
	m_untCapaL.SetUnitType(CUnitCtrl::m_BREV_UNIT.dCapaL);
	m_edtCapaT.SetUnitType(CUnitCtrl::m_BREV_UNIT.dCapaT);
	m_untCapaT.SetUnitType(CUnitCtrl::m_BREV_UNIT.dCapaT);
	m_edtDispL.SetUnitType(CUnitCtrl::m_BREV_UNIT.dDispL);
	m_untDispL.SetUnitType(CUnitCtrl::m_BREV_UNIT.dDispL);
	m_edtDispT.SetUnitType(CUnitCtrl::m_BREV_UNIT.dDispT);
	m_untDispT.SetUnitType(CUnitCtrl::m_BREV_UNIT.dDispT);
	//

	m_edthef.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dhef);
	m_unthef.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dhef);
	m_edtfy.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dfy);
	m_untfy.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dfy);
	m_edtfuta.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dfuta);
	m_untfuta.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dfuta);
	m_edtdh.SetUnitType(CUnitCtrl::m_BRPR_UNIT.ddh);
	m_untdh.SetUnitType(CUnitCtrl::m_BRPR_UNIT.ddh);
	m_edteh.SetUnitType(CUnitCtrl::m_BRPR_UNIT.deh);
	m_unteh.SetUnitType(CUnitCtrl::m_BRPR_UNIT.deh);
	m_edtNp.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dNp);
	m_untNp.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dNp);
	m_edtBa.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untBa.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtHa.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untHa.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtSx.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dSx);
	m_untSx.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dSx);
	m_edtN1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtS1.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dS1);
	m_untS1.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dS1);
	m_edtSy.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dSy);
	m_untSy.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dSy);
	m_edtN2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtS2.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dS2);
	m_untS2.SetUnitType(CUnitCtrl::m_BRPR_UNIT.dS2);
}

void CDgnSeisBearingPropItemDlg::InitCombo(int nInstall, int nAnchor)
{
	// 설치종류
	m_cmbInstall.ResetContent();
	m_cmbInstall.SetItemData(m_cmbInstall.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_CAST)), 0);
	m_cmbInstall.SetItemData(m_cmbInstall.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_POST)), 1);
	m_cmbInstall.SetCurSel(nInstall);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbInstall);
		
	// 앵커종류
	m_cmbAnchor.ResetContent();
	if(nInstall == 0)
	{		
		m_cmbAnchor.SetItemData(m_cmbAnchor.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_CAST_1)), 0);
		m_cmbAnchor.SetItemData(m_cmbAnchor.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_CAST_2)), 1);
		m_cmbAnchor.SetItemData(m_cmbAnchor.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_CAST_3)), 2);
		m_cmbAnchor.SetItemData(m_cmbAnchor.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_CAST_4)), 3);
	}
	else
	{
		m_cmbAnchor.SetItemData(m_cmbAnchor.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_POST_1)), 0);
		m_cmbAnchor.SetItemData(m_cmbAnchor.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_POST_2)), 1);
		m_cmbAnchor.SetItemData(m_cmbAnchor.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_POST_3)), 2);
		m_cmbAnchor.SetItemData(m_cmbAnchor.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_POST_4)), 3);
	}
	m_cmbAnchor.SetCurSel(nAnchor);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbAnchor);

	// 강재 받침 
	m_cmbStlBearing.ResetContent();
	m_cmbStlBearing.SetItemData(m_cmbStlBearing.AddString(_LS(IDS_DGN_SEIS_BEARING_EVAL_STL_BEARING_TYPE1)), 0);  // _T("고력황동받침")
	m_cmbStlBearing.SetItemData(m_cmbStlBearing.AddString(_LS(IDS_DGN_SEIS_BEARING_EVAL_STL_BEARING_TYPE2)), 1);  // _T("포트받침")
	m_cmbStlBearing.SetItemData(m_cmbStlBearing.AddString(_LS(IDS_DGN_SEIS_BEARING_EVAL_STL_BEARING_TYPE3)), 2);  // _T("포트받침(소켓매몰)")
	m_cmbStlBearing.SetCurSel(0);

}

void CDgnSeisBearingPropItemDlg::InitView()
{
	m_View.SetParent(this);
	m_View.Init();
	//m_View.SetAxisMode(EN_DRAW_AXISMODE_PANTALINE_YZ, 40, 15);
	m_View.SetMode(EN_DRAW_WINMODE_SELECT);
	//m_View.SetContext(TRUE, TRUE);// 이거 주석 풀면 컨텍스트 메뉴 열림
	//m_View.SetDrawType(DRAW_EDIT_BTN_TYPE_XY);
	m_View.MakeDrawUnit();

	m_wndPicture.SetIgnoreLargeView(TRUE);
}

void CDgnSeisBearingPropItemDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistX = 0;
	int nDistY = 0;	
	CArray<UINT, UINT> aControls;

	// Bmp
	aControls.RemoveAll();
	aControls.Add(IDC_DGN_PICTURE);
	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_VIEW)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_PICTURE)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	nDistX = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);

	// 소켓지름.
	aControls.RemoveAll();
	aControls.Add(IDC_DGN_SEIS_BEARING_PROP_DS_STC);
	aControls.Add(IDC_DGN_SEIS_BEARING_PROP_DS_EDT);
	aControls.Add(IDC_DGN_SEIS_BEARING_PROP_DS_UNT);

	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_DA_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_DS_STC)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	nDistX = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);

}

void CDgnSeisBearingPropItemDlg::ControlsShowHide()
{
	UpdateData(TRUE);

 	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_VIEW)->ShowWindow(m_nViewType == 0);
 	GetDlgItem(IDC_DGN_PICTURE)->ShowWindow(m_nViewType == 1);
	if(m_nViewType == 1)
	{
		CString aElemTypePicture[] = {
			_T("SVG\\illustration\\Dialog\\Dgn_Seis_Bearing_Prop.svg")
		};
		m_wndPicture.SetImage(aElemTypePicture[0]);
	}
}

void CDgnSeisBearingPropItemDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	int nInstall = m_cmbInstall.GetItemData(m_cmbInstall.GetCurSel());
	int nAnchor = m_cmbAnchor.GetItemData(m_cmbAnchor.GetCurSel());

	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_DH_STC)->EnableWindow(nInstall == 0 && (nAnchor == 0 || nAnchor == 1));
	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_DH_EDT)->EnableWindow(nInstall == 0 && (nAnchor == 0 || nAnchor == 1));
	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_DH_UNT)->EnableWindow(nInstall == 0 && (nAnchor == 0 || nAnchor == 1));

	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_EH_STC)->EnableWindow(nInstall == 0 && (nAnchor == 2 || nAnchor == 3));
	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_EH_EDT)->EnableWindow(nInstall == 0 && (nAnchor == 2 || nAnchor == 3));
	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_EH_UNT)->EnableWindow(nInstall == 0 && (nAnchor == 2 || nAnchor == 3));

	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_NP_STC)->EnableWindow(nInstall == 1);
	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_NP_EDT)->EnableWindow(nInstall == 1);
	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_NP_UNT)->EnableWindow(nInstall == 1);

	int nN1 = m_edtN1.GetEditValue();
	
	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_S1_STC)->EnableWindow(nN1 > 1);
	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_S1_EDT)->EnableWindow(nN1 > 1);
	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_S1_UNT)->EnableWindow(nN1 > 1);

	int nN2 = m_edtN2.GetEditValue();

	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_S2_STC)->EnableWindow(nN2 > 1);
	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_S2_EDT)->EnableWindow(nN2 > 1);
	GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_S2_UNT)->EnableWindow(nN2 > 1);

	if(m_bModify) GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_APPLY_BTN)->EnableWindow(FALSE);

	//
	int nStlBearing = m_cmbStlBearing.GetItemData(m_cmbStlBearing.GetCurSel());
	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_STL_BEARING_CMB)->EnableWindow(m_nBearing == 0);

	if (m_nBearing == 0) // 강재 받침
	{
		GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_DS_STC)->ShowWindow(nStlBearing == 2);
		GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_DS_EDT)->ShowWindow(nStlBearing == 2);
		GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_DS_UNT)->ShowWindow(nStlBearing == 2);

		GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_DA_STC)->ShowWindow(nStlBearing != 2);
		GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_DA_EDT)->ShowWindow(nStlBearing != 2);
		GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_DA_UNT)->ShowWindow(nStlBearing != 2);
	}
	else // 탄성 받침
	{
		GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_DS_STC)->ShowWindow(FALSE);
		GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_DS_EDT)->ShowWindow(FALSE);
		GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_DS_UNT)->ShowWindow(FALSE);

		GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_DA_STC)->ShowWindow(TRUE);
		GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_DA_EDT)->ShowWindow(TRUE);
		GetDlgItem(IDC_DGN_SEIS_BEARING_PROP_DA_UNT)->ShowWindow(TRUE);
	}

	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DISP_GRP)->EnableWindow(m_nBearing == 1);
	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DISP_L_STC)->EnableWindow(m_nBearing == 1);
	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DISP_L_EDT2)->EnableWindow(m_nBearing == 1);
	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DISP_L_UNT)->EnableWindow(m_nBearing == 1);
	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DISP_T_STC)->EnableWindow(m_nBearing == 1);
	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DISP_T_EDT2)->EnableWindow(m_nBearing == 1);
	GetDlgItem(IDC_DGN_SEIS_BEARING_EVAL_DISP_T_UNT)->EnableWindow(m_nBearing == 1);
	//
}

void CDgnSeisBearingPropItemDlg::Data2Dlg()
{
	m_edtName.SetWindowText(m_Data.strName);

	m_cmbInstall.SetCurSel(m_Data.nInstall);
	m_cmbAnchor.SetCurSel(m_Data.nAnchor);

	m_edthef.SetEditUnit(m_Data.dhef);
	m_edtfy.SetEditUnit(m_Data.dfy);
	m_edtfuta.SetEditUnit(m_Data.dfuta);
	m_edtdh.SetEditUnit(m_Data.ddh);
	m_edteh.SetEditUnit(m_Data.deh);
	m_edtNp.SetEditUnit(m_Data.dNp);

	m_edtSx.SetEditUnit(m_Data.dSx);
	m_edtN1.SetEditUnit(m_Data.nN1);
	m_edtS1.SetEditUnit(m_Data.dS1);
	m_edtSy.SetEditUnit(m_Data.dSy);
	m_edtN2.SetEditUnit(m_Data.nN2);
	m_edtS2.SetEditUnit(m_Data.dS2);

	m_bContiWeld = m_Data.bContiWeld;

	//
	m_nBearing = m_Data.nBearing;
	m_cmbStlBearing.SetCurSel(m_Data.nStlBearing);
	m_edtda.SetEditUnit(m_Data.dda);
	m_edtds.SetEditUnit(m_Data.dds);
	m_edtCapaL.SetEditUnit(m_Data.dCapaL);
	m_edtCapaT.SetEditUnit(m_Data.dCapaT);
	m_edtDispL.SetEditUnit(m_Data.dDispL);
	m_edtDispT.SetEditUnit(m_Data.dDispT);
	//

	UpdateData(FALSE);

	SetViewData();
}

BOOL CDgnSeisBearingPropItemDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.Initialize();

	m_edtName.GetWindowText(m_Data.strName);

	m_Data.nInstall = m_cmbInstall.GetCurSel();
	m_Data.nAnchor = m_cmbAnchor.GetCurSel();

	m_Data.dhef = m_edthef.GetEditValue();
	m_Data.dfy = m_edtfy.GetEditValue();
	m_Data.dfuta = m_edtfuta.GetEditValue();
	m_Data.ddh = m_edtdh.GetEditValue();
	m_Data.deh = m_edteh.GetEditValue();
	m_Data.dNp = m_edtNp.GetEditValue();

	m_Data.dSx = m_edtSx.GetEditValue();
	m_Data.nN1 = m_edtN1.GetEditValue();
	m_Data.dS1 = m_edtS1.GetEditValue();
	m_Data.dSy = m_edtSy.GetEditValue();
	m_Data.nN2 = m_edtN2.GetEditValue();
	m_Data.dS2 = m_edtS2.GetEditValue();
	
	m_Data.bContiWeld = m_bContiWeld;

	//
	m_Data.nBearing = m_nBearing;
	m_Data.nStlBearing = m_cmbStlBearing.GetItemData(m_cmbStlBearing.GetCurSel());
	m_Data.dda = m_edtda.GetEditValue();
	m_Data.dds = m_edtds.GetEditValue();
	if(m_Data.nBearing==0 && m_Data.nStlBearing==2)
	{
		m_Data.dda = m_Data.dds;
	}
	m_Data.dCapaL = m_edtCapaL.GetEditValue();
	m_Data.dCapaT = m_edtCapaT.GetEditValue();
	m_Data.dDispL = m_edtDispL.GetEditValue();
	m_Data.dDispT = m_edtDispT.GetEditValue();
	//
	return TRUE;
}

void CDgnSeisBearingPropItemDlg::SetViewData() 
{
	m_View.SetData(m_Data);
}

void CDgnSeisBearingPropItemDlg::CalcBa()
{
	double dSx = m_edtSx.GetEditValue();
	int    nN1 = m_edtN1.GetEditValue();
	double dS1 = m_edtS1.GetEditValue();

	double dBa = (dSx * 2) + ((nN1-1) * dS1);

	m_edtBa.SetEditUnit(dBa);

	UpdateData(FALSE);
}

void CDgnSeisBearingPropItemDlg::CalcHa()
{
	double dSy = m_edtSy.GetEditValue();
	int    nN2 = m_edtN2.GetEditValue();
	double dS2 = m_edtS2.GetEditValue();

	double dHa = (dSy * 2) + ((nN2-1) * dS2);

	m_edtHa.SetEditUnit(dHa);

	UpdateData(FALSE);
}

BOOL CDgnSeisBearingPropItemDlg::CheckBrpr()
{
	CArray<T_ANEV_K, T_ANEV_K> rKeyList;
	m_pDoc->m_pAttrCtrl2->GetAnevKeyList(rKeyList);

	T_ANEV_D rData;
	CArray<T_ELEM_K, T_ELEM_K> aBrprK;
	for (int m = 0; m < rKeyList.GetSize(); m++)
	{
		m_pDoc->m_pAttrCtrl2->GetAnev(rKeyList[m], rData);

		BOOL bUsed = FALSE;
		for (int i = 0; i < rData.BearingProp.aBrprKs.GetSize(); i++)
		{
			for (int j = 0; j < rData.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++)
			{
				if (m_Key == rData.BearingProp.aBrprKs[i].aBrprK[j])
				{
					bUsed = TRUE;
				}
			}
		}

		if (bUsed)
		{
			if (!CheckBrpr4Anev(rData)) return FALSE;
		}
	}

	return TRUE;
}

BOOL CDgnSeisBearingPropItemDlg::CheckBrpr4Anev(T_ANEV_D& AnevD)
{
	// Check 2
	T_BRPR_D BrprD;
	int nElastic = 0;
	int nSteel = 0;
	for (int i = 0; i < AnevD.BearingProp.aBrprKs.GetSize(); i++)
	{
		for (int j = 0; j < AnevD.BearingProp.aBrprKs[i].aBrprK.GetSize(); j++)
		{
			// 0 : 비대칭 고려
			if (AnevD.BearingProp.aBrprKs[i].aBrprK[j] < 0)
			{
				m_pDoc->DisplayHistoryMessage(_LS(IDS_DB_DT_ANEV_ERROR_DATA_CHECK));

				return FALSE;
			}

			BrprD.Initialize();
			if (m_Key == AnevD.BearingProp.aBrprKs[i].aBrprK[j])
			{
				BrprD = m_Data;
			}
			else
			{
				if (!m_pDoc->m_pAttrCtrl2->GetBrpr(AnevD.BearingProp.aBrprKs[i].aBrprK[j], BrprD)) { continue; }
			}

			if      (BrprD.nBearing == 0) nElastic += 1;
			else if (BrprD.nBearing == 1) nSteel += 1;

		}
	}
	if (nElastic > 0 && nSteel > 0)
	{
		CString msg;
		msg.Format(_LS(IDS_DGN_SEIS_BEARING_PROP_USED), m_Data.strName);  //_T("Error : Bearing Property(Name %s) is Used in Anchor Evaluation.")
		msg += _T("\n");
		msg += _T("Error : ") + _LS(IDS_DB_DT_BRPR_STEEL_ELASTIC);
		AfxMessageBox(msg);
		return FALSE;
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisBearingPropItemDlg implementation functions
BEGIN_MESSAGE_MAP(CDgnSeisBearingPropItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisBearingPropItemDlg)
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_BEARING_PROP_INSTALL_CMB,      OnSeisBearingPropInstallCmb)
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_BEARING_PROP_ANCHOR_CMB,       OnSeisBearingPropAnchorCmb)
	ON_CBN_SELCHANGE(IDC_DGN_SEIS_BEARING_EVAL_STL_BEARING_CMB, OnSeisBearingEvalStlBearingCmb)
 	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_EVAL_BEARING_RDO1,          OnSeisBearingEvalBearingRdo)	
 	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_EVAL_BEARING_RDO2,          OnSeisBearingEvalBearingRdo)	

	ON_EN_CHANGE(IDC_DGN_SEIS_BEARING_PROP_DA_EDT,				 OnSeisBearingPropDaEdt)
	ON_EN_CHANGE(IDC_DGN_SEIS_BEARING_PROP_DS_EDT,				 OnSeisBearingPropDaEdt)
	ON_EN_CHANGE(IDC_DGN_SEIS_BEARING_PROP_SX_EDT,               OnSeisBearingPropSxEdt)
	ON_EN_CHANGE(IDC_DGN_SEIS_BEARING_PROP_N1_EDT,               OnSeisBearingPropN1Edt)
	ON_EN_CHANGE(IDC_DGN_SEIS_BEARING_PROP_S1_EDT,               OnSeisBearingPropS1Edt)
	ON_EN_CHANGE(IDC_DGN_SEIS_BEARING_PROP_SY_EDT,               OnSeisBearingPropSyEdt)
	ON_EN_CHANGE(IDC_DGN_SEIS_BEARING_PROP_N2_EDT,               OnSeisBearingPropN2Edt)
	ON_EN_CHANGE(IDC_DGN_SEIS_BEARING_PROP_S2_EDT,               OnSeisBearingPropS2Edt)	
	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_PROP_VIEW_DRAW_RDO,       OnSeisBearingPropDrawRdo)	
	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_PROP_VIEW_GUIDE_RDO,      OnSeisBearingPropGuideRdo)	
// 	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_PROP_DA_RDO,				 OnSeisBearingPropDaRdo)
// 	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_PROP_DS_RDO,				 OnSeisBearingPropDaRdo)
	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_PROP_OK_BTN,              OnSeisBearingPropOKBtn)	
	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_PROP_CANCEL_BTN,          OnSeisBearingPropCancelBtn)	
	ON_BN_CLICKED(IDC_DGN_SEIS_BEARING_PROP_APPLY_BTN,           OnSeisBearingPropApplyBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisBearingPropItemDlg message handlers

BOOL CDgnSeisBearingPropItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitUnit();
	InitCombo(m_Data.nInstall, m_Data.nAnchor);
	InitView();

	Data2Dlg();

	if(m_bModify)
	{
		m_nViewType = 0;
		UpdateData(FALSE);
	}		
	
	AlignControl();
	ControlsShowHide();
	EnableDisableControls();

	CalcBa();
	CalcHa();
	
	if(m_bSetEmf)
	{
		CRect Rect;
		m_View.GetWindowRect(Rect);
		m_View.MakeEmfFile(m_strPath, Rect.Width() * 100, Rect.Height() * 100);
		PostMessage(WM_CLOSE, 0, 0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisBearingPropItemDlg::OnSeisBearingPropInstallCmb()
{
	UpdateData(TRUE);

	int nInstall = m_cmbInstall.GetItemData(m_cmbInstall.GetCurSel());

	// 앵커종류
	m_cmbAnchor.ResetContent();

	if(nInstall == 0)
	{		
		m_cmbAnchor.SetItemData(m_cmbAnchor.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_CAST_1)), 0);
		m_cmbAnchor.SetItemData(m_cmbAnchor.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_CAST_2)), 1);
		m_cmbAnchor.SetItemData(m_cmbAnchor.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_CAST_3)), 2);
		m_cmbAnchor.SetItemData(m_cmbAnchor.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_CAST_4)), 3);
	}
	else
	{
		m_cmbAnchor.SetItemData(m_cmbAnchor.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_POST_1)), 0);
		m_cmbAnchor.SetItemData(m_cmbAnchor.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_POST_2)), 1);
		m_cmbAnchor.SetItemData(m_cmbAnchor.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_POST_3)), 2);
		m_cmbAnchor.SetItemData(m_cmbAnchor.AddString(_LS(IDS_DGN_SEIS_BEARING_PROP_INSTALL_POST_4)), 3);
	}

	m_cmbAnchor.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbAnchor);

	EnableDisableControls();
}

void CDgnSeisBearingPropItemDlg::OnSeisBearingPropAnchorCmb()
{
	EnableDisableControls();
}

void CDgnSeisBearingPropItemDlg::OnSeisBearingEvalBearingRdo()
{
	EnableDisableControls();
}

void CDgnSeisBearingPropItemDlg::OnSeisBearingEvalStlBearingCmb()
{
	EnableDisableControls();
}


void CDgnSeisBearingPropItemDlg::OnSeisBearingPropDaEdt()
{
	Dlg2Data();
	SetViewData();
}

void CDgnSeisBearingPropItemDlg::OnSeisBearingPropSxEdt()
{
	CalcBa();
	Dlg2Data();
	SetViewData();
}

void CDgnSeisBearingPropItemDlg::OnSeisBearingPropN1Edt()
{
	// 1이하 일때 0.0 으로 변경하는걸 요청 했음
	int nN1 = m_edtN1.GetEditValue();
	if(nN1 <= 1) m_edtS1.SetWindowText(_T("0"));

	EnableDisableControls();

	CalcBa();
	Dlg2Data();
	SetViewData();
}

void CDgnSeisBearingPropItemDlg::OnSeisBearingPropS1Edt()
{
	CalcBa();
	Dlg2Data();
	SetViewData();
}

void CDgnSeisBearingPropItemDlg::OnSeisBearingPropSyEdt()
{
	CalcHa();
	Dlg2Data();
	SetViewData();
}

void CDgnSeisBearingPropItemDlg::OnSeisBearingPropN2Edt()
{
	// 1이하 일때 0.0 으로 변경하는걸 요청 했음
	int nN2 = m_edtN2.GetEditValue();
	if(nN2 <= 1) m_edtS2.SetWindowText(_T("0"));

	EnableDisableControls();

	CalcHa();
	Dlg2Data();
	SetViewData();
}

void CDgnSeisBearingPropItemDlg::OnSeisBearingPropS2Edt()
{
	CalcHa();
	Dlg2Data();
	SetViewData();
}

void CDgnSeisBearingPropItemDlg::OnSeisBearingPropDrawRdo()
{
	ControlsShowHide();
}

void CDgnSeisBearingPropItemDlg::OnSeisBearingPropGuideRdo()
{
	ControlsShowHide();
}

// void CDgnSeisBearingPropItemDlg::OnSeisBearingPropDaRdo()
// {
// 	EnableDisableControls();
// }

void CDgnSeisBearingPropItemDlg::OnSeisBearingPropOKBtn()
{
	if(!Dlg2Data()) return;

	if(m_bModify)
	{
		if (!CheckBrpr()) return;

		if(!m_pDoc->m_pDataCtrl->ModifyBrpr(m_Key, m_Data)) return;
	}
	else
	{
		if(!m_pDoc->m_pDataCtrl->AddBrpr(m_Data)) return;
	}

	CDialogMove::OnOK();	
}

void CDgnSeisBearingPropItemDlg::OnSeisBearingPropCancelBtn()
{
	CDialogMove::OnCancel();
}

void CDgnSeisBearingPropItemDlg::OnSeisBearingPropApplyBtn()
{
	if(!Dlg2Data()) return;

	if(!m_pDoc->m_pDataCtrl->AddBrpr(m_Data)) return;
}