// DgnCfCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCfCodeDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DBCodeDef.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_main\wg_mainRes2.h"
#include "DgnDataCtrl.h"
#include "NationalAnnexTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCfCodeDlg dialog


CDgnCfCodeDlg::CDgnCfCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnCfCodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCfCodeDlg)
	//}}AFX_DATA_INIT
	m_aNationalAnnexCtrl.RemoveAll();
	m_aNationalAnnexCtrl.Add(IDC_DGN_NATIOANL_ANNEX_STC);	//国家标准
	m_aNationalAnnexCtrl.Add(IDC_DGN_CTRLDATA_SUBCODE_CMB);

	m_aEuroCodeCtrl.RemoveAll();
	m_aEuroCodeCtrl.Add(IDC_DGN_CFCTRL_SPECIAL);	//反复细化C-型截面屈曲折减系数. 5.5.3.2 (3)条

	m_aEtcCtrl.RemoveAll();
	m_aEtcCtrl.Add(IDC_DGN_CTRLDATA_GB);	//所有梁都不考虑横向屈曲

	m_aGBCodeCtrl.RemoveAll();
	m_aGBCodeCtrl.Add(IDC_DGN_STRUCT_IMPORT_COEF);
	m_aGBCodeCtrl.Add(IDC_DGN_STRUCT_IMPORT_COEF_EDT);
	m_aGBCodeCtrl.Add(IDC_DGN_CFCTRL_BEAMCHK_STATIC);
	m_aGBCodeCtrl.Add(IDC_DGN_CFCTRL_BEAMCHK_RADIO1);
	m_aGBCodeCtrl.Add(IDC_DGN_CFCTRL_BEAMCHK_RADIO2);
	m_aGBCodeCtrl.Add(IDC_DGN_STLCTRL_PURLIN_CHECK);
	m_aGBCodeCtrl.Add(IDC_DGN_CFCTRL_PURLIN_DGN_STATIC);
	m_aGBCodeCtrl.Add(IDC_DGN_STLCTRL_LOWEREDGE_CHECK);
	m_aGBCodeCtrl.Add(IDC_DGN_CFCTRL_PURLIN_DGN_CASE_STATIC);
	m_aGBCodeCtrl.Add(IDC_DGN_CFCTRL_PURLIN_DGN_CASE_COMBO);
	m_aGBCodeCtrl.Add(IDC_DGN_STLCTRL_UPEDGE_CHECK);
	m_aGBCodeCtrl.Add(IDC_DGN_STLCTRL_SLABCONS_CHECK);
	m_aGBCodeCtrl.Add(IDC_DGN_STLCTRL_BENDMODULUS_STATIC);
	m_aGBCodeCtrl.Add(IDC_DGN_STLCTRL_BENDMODULUS_Y_STATIC);
	m_aGBCodeCtrl.Add(IDC_DGN_STLCTRL_BENDMODULUS_Y_EDIT);
	m_aGBCodeCtrl.Add(IDC_DGN_STLCTRL_BENDMODULUS_Z_STATIC);
	m_aGBCodeCtrl.Add(IDC_DGN_STLCTRL_BENDMODULUS_Z_EDIT);
	m_aGBCodeCtrl.Add(IDC_DGN_STLCTRL_SPECIAL);
	m_aGBCodeCtrl.Add(IDC_DGN_SECT_NETGROSS_RATIO);
	m_aGBCodeCtrl.Add(IDC_DGN_SECT_NETGROSS_RATIO_EDT);

	m_aCodeEnable.RemoveAll();
	m_nOldCodeIndex = 0;
}

CDgnCfCodeDlg::~CDgnCfCodeDlg()
{

}

void CDgnCfCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCfCodeDlg)
	DDX_Control(pDX, IDC_DGN_CTRLDATA_DESIGNCODE, m_DesignCode);
	DDX_Control(pDX, IDC_DGN_CTRLDATA_SUBCODE_CMB, m_DesignSubCode);
	DDX_Check(pDX, IDC_DGN_CFCTRL_SPECIAL, m_bIteration);
	DDX_Check(pDX, IDC_DGN_CTRLDATA_GB, m_bNotUseBLu);
	DDX_Text(pDX, IDC_DGN_STRUCT_IMPORT_COEF_EDT, m_dStructImportCoef);
	DDX_Check(pDX, IDC_DGN_STLCTRL_SPECIAL, m_bSeismic);
	DDX_Text(pDX, IDC_DGN_SECT_NETGROSS_RATIO_EDT, m_dNetGrossAreaRatio);
	DDX_Radio(pDX, IDC_DGN_CFCTRL_BEAMCHK_RADIO1, m_nBeamChkProc);
	DDX_Check(pDX, IDC_DGN_STLCTRL_PURLIN_CHECK, m_bPurlinBend);
	DDX_Check(pDX, IDC_DGN_STLCTRL_SLABCONS_CHECK, m_bSlabCons);
	DDX_Text(pDX, IDC_DGN_STLCTRL_BENDMODULUS_Y_EDIT, m_dBendModuluY);
	DDX_Text(pDX, IDC_DGN_STLCTRL_BENDMODULUS_Z_EDIT, m_dBendModuluZ);
	DDX_Check(pDX, IDC_DGN_STLCTRL_LOWEREDGE_CHECK, m_bLowerEdge);
	DDX_Check(pDX, IDC_DGN_STLCTRL_UPEDGE_CHECK, m_bUpEdge);
	DDX_Control(pDX, IDC_DGN_CFCTRL_PURLIN_DGN_CASE_COMBO, m_WindCase);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnCfCodeDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnCfCodeDlg)
	ON_BN_CLICKED(IDC_DGN_CFCTRL_SPECIAL, OnDgnStlctrlSpecial)
	ON_CBN_SELCHANGE(IDC_DGN_CTRLDATA_DESIGNCODE, OnSelchangeDgnCtrldataDesigncode)
	ON_BN_CLICKED(IDC_DGN_STLCTRL_LOWEREDGE_CHECK, OnDgnStlChkLowerEdge)
	ON_CBN_SELCHANGE(IDC_DGN_CFCTRL_PURLIN_DGN_CASE_COMBO, OnSelchangeDgnWindCase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCfCodeDlg message handlers

BOOL CDgnCfCodeDlg::OnInitDialog() 
{
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	m_DesignCode.ResetContent();
	CStringArray CfsCodeName;
	CDBLib::GetCfsCodeNameList(CfsCodeName, &m_aCodeEnable);

	CString strCfsCodeName = _T("");
	for(int i=0; i<CfsCodeName.GetSize(); i++)
	{
		strCfsCodeName = CDBLib::ViewConvertCodeName(CfsCodeName[i]);
		if (strCfsCodeName == _T("GB50068-2018"))
			continue;
#if defined(_RUS)
		CInitCtrl::ConvertEngRusDesignCode(strCfsCodeName);
#endif
		m_DesignCode.AddString(strCfsCodeName);
	}

	const auto vString = CNationalAnnexTool::GetCfsNationalAnnex();
	const auto nNA = vString.size();
	for (int i = 0; i < nNA; ++i)
	{
		CDlgUtil::CobxAddItem(m_DesignSubCode, vString[i], CNationalAnnexTool::ConvertNationalAnnex(vString[i]));
	}

	if(m_pDoc->m_pAttrCtrl->ExistDcfs())	Initial_ExistData();
	else     							    Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnCfCodeDlg::Initial_Data()
{
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);
	rPref.DgnCode.CFSteelCode = CDBLib::ViewConvertCodeName(rPref.DgnCode.CFSteelCode);

	T_DCFS_D rData;
	rData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDcfs(rData);
	rData.DesignCode = CDBLib::ViewConvertCodeName(rData.DesignCode);
	CString CodeName;
	CodeName = rPref.DgnCode.CFSteelCode;
#if defined(_RUS)
	CInitCtrl::ConvertEngRusDesignCode(CodeName);
#endif
	int Index = m_DesignCode.FindStringExact(-1, CodeName);
#if defined(_CH) && defined(_MGEN)
	if(Index!=-1 && m_aCodeEnable[Index]==FALSE) Index = -1;
#endif
	if(Index!=-1)	m_DesignCode.SetCurSel(Index);
	m_nOldCodeIndex = Index;

	CDlgUtil::CobxSetCurSelItemData(m_DesignSubCode, rPref.DgnCode.nCfsNationalAnnex);
	rData.nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_DesignSubCode, m_DesignSubCode.GetCurSel());
	m_bIteration = TRUE;
	m_bNotUseBLu = FALSE;

	m_bSeismic = TRUE;
	m_dStructImportCoef = 1.0;
	m_dNetGrossAreaRatio = 0.85;
	m_nBeamChkProc = 0;
	m_bPurlinBend = TRUE;
	m_bSlabCons = TRUE;
	m_dBendModuluY = 1.0;
	m_dBendModuluZ = 1.0;
	m_bLowerEdge = TRUE;
	m_bUpEdge = TRUE;
	m_nWindCase = 0;
	CheckRadioButton(IDC_DGN_CFCTRL_BEAMCHK_RADIO1, IDC_DGN_CFCTRL_BEAMCHK_RADIO2, IDC_DGN_CFCTRL_BEAMCHK_RADIO1);
	InitWindCaseCombx();
	UpdateData(FALSE);
	SelchangeDgnCtrldataDesigncode(TRUE);
	if (rData.DesignCode == CFSCODE_GB50018_25)
	{
		OnDgnStlChkLowerEdge();
	}
}

void CDgnCfCodeDlg::Initial_ExistData()
{
	T_DCFS_D rData;
	rData.Initialize();
	m_pDoc->m_pAttrCtrl->GetDcfs(rData);
	rData.DesignCode = CDBLib::ViewConvertCodeName(rData.DesignCode);
	CString CodeName;
	CodeName = rData.DesignCode;
#if defined(_RUS)
	CInitCtrl::ConvertEngRusDesignCode(CodeName);
#endif

	int Index=0;
	if(rData.DesignCode!=_T(""))	Index = m_DesignCode.FindStringExact(-1, CodeName);
	else											Index = 0;
	m_DesignCode.SetCurSel(Index);
	m_nOldCodeIndex = Index;

	int IndexNA = (rData.nNationalAnnex < 0) ? 0 : rData.nNationalAnnex;
	CDlgUtil::CobxSetCurSelItemData(m_DesignSubCode, IndexNA);

	m_bIteration	= rData.bIteration;
	m_bNotUseBLu	= rData.bAllFrameLaterallyBraced;
	m_bSeismic		= rData.bSeismic;

	m_dStructImportCoef = rData.dStructImportCoef;
	m_dNetGrossAreaRatio = rData.dNetGrossAreaRatio;

	m_nBeamChkProc = rData.nBeamChkProc;
	m_bPurlinBend = rData.bPurlinBend;
	m_bSlabCons = rData.bSlabCons;
	m_dBendModuluY = rData.dBendModuluY;
	m_dBendModuluZ = rData.dBendModuluZ;
	m_bLowerEdge = rData.bLowerEdge;
	m_bUpEdge = rData.bUpEdge;
	m_nWindCase = rData.nWindCase;
	InitWindCaseCombx();

	if (m_nBeamChkProc == 0)
	{
		CheckRadioButton(IDC_DGN_CFCTRL_BEAMCHK_RADIO1, IDC_DGN_CFCTRL_BEAMCHK_RADIO2, IDC_DGN_CFCTRL_BEAMCHK_RADIO1);
	}
	else
	{
		CheckRadioButton(IDC_DGN_CFCTRL_BEAMCHK_RADIO1, IDC_DGN_CFCTRL_BEAMCHK_RADIO2, IDC_DGN_CFCTRL_BEAMCHK_RADIO2);
	}

	UpdateData(FALSE);
	SelchangeDgnCtrldataDesigncode(FALSE);
	if (rData.DesignCode == CFSCODE_GB50018_25)
	{
		if (m_bLowerEdge)
		{
			GetDlgItem(IDC_DGN_CFCTRL_PURLIN_DGN_CASE_COMBO)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_STLCTRL_UPEDGE_CHECK)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_DGN_CFCTRL_PURLIN_DGN_CASE_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_STLCTRL_UPEDGE_CHECK)->EnableWindow(FALSE);
		}
	}
}

void CDgnCfCodeDlg::OnOK() 
{
	UpdateData(TRUE);
	T_DCFS_D rData;
	rData.Initialize();
	if(m_pDoc->m_pAttrCtrl->ExistDcfs())	m_pDoc->m_pAttrCtrl->GetDcfs(rData);

	// Get Design Code.
	int Index = m_DesignCode.GetCurSel();
	m_DesignCode.GetLBText(Index,rData.DesignCode);

#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(rData.DesignCode);
#endif

	rData.DesignCode = CDBLib::GetConvertCodeName(rData.DesignCode);
	rData.nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_DesignSubCode, m_DesignSubCode.GetCurSel());
	rData.bIteration = m_bIteration;
	rData.bAllFrameLaterallyBraced = m_bNotUseBLu;
	rData.bSeismic = m_bSeismic;
	rData.dStructImportCoef = m_dStructImportCoef;
	rData.dNetGrossAreaRatio = m_dNetGrossAreaRatio;
	if (GetCheckedRadioButton(IDC_DGN_CFCTRL_BEAMCHK_RADIO1, IDC_DGN_CFCTRL_BEAMCHK_RADIO2) == IDC_DGN_CFCTRL_BEAMCHK_RADIO1)
	{
		rData.nBeamChkProc = 0;
	}
	else
	{
		rData.nBeamChkProc = 1;
	}
	rData.bPurlinBend = m_bPurlinBend;
	rData.bSlabCons = m_bSlabCons;
	rData.dBendModuluY = m_dBendModuluY;
	rData.dBendModuluZ = m_dBendModuluZ;
	rData.bLowerEdge = m_bLowerEdge;
	rData.bUpEdge = m_bUpEdge;
	CString strWcase = _T("");
	m_WindCase.GetWindowText(strWcase);
	m_nWindCase = m_pDoc->m_pAttrCtrl->GetStldKey(strWcase);
	rData.nWindCase = m_nWindCase;

	BOOL bCheck = m_pDoc->m_pDataCtrl->AddDcfs(rData);
	if(bCheck)	
	{
		CDialogMove::OnOK();
	}
}

void CDgnCfCodeDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnCfCodeDlg::OnDgnStlctrlSpecial() 
{
	// TODO: Add your control notification handler code here
	OnSelchangeDgnCtrldataDesigncode();
}

void CDgnCfCodeDlg::SelchangeDgnCtrldataDesigncode(BOOL bUpdateDef) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CString strDgnCode=_T("");
	m_DesignCode.GetWindowText(strDgnCode);

#if defined(_RUS)
	CInitCtrl::ConvertRusEngDesignCode(strDgnCode);
#endif

	CRect RectDlg, RectOk, RectCn, RectOrg;
	GetWindowRect(&RectDlg);
	GetDlgItem(IDOK    )->GetWindowRect(&RectOk);
	GetDlgItem(IDCANCEL)->GetWindowRect(&RectCn);

	BOOL bEuroCode = FALSE;
    if ( strDgnCode == CFSCODE_EC3_06 )
    {
        bEuroCode = TRUE;
    }
	
    BOOL bGBCode = FALSE;	
    if ( strDgnCode == CFSCODE_GB50018_02) 	//Add by tss(GB50018-02)
    {
        bGBCode = TRUE; 
    }

	BOOL bGBCode_25 = FALSE;
	if (strDgnCode == CFSCODE_GB50018_25) 
	{
		bGBCode_25 = TRUE;
	}

	CDlgUtil::CtrlShowHide(this, m_aNationalAnnexCtrl,  bEuroCode);
	CDlgUtil::CtrlShowHide(this, m_aEuroCodeCtrl,       bEuroCode);
	CDlgUtil::CtrlShowHide(this, m_aEtcCtrl,            bEuroCode==FALSE);
	CDlgUtil::CtrlShowHide(this, m_aGBCodeCtrl,			bGBCode || bGBCode_25);
	

	CRect refRect, moveRect;
	GetDlgItem(IDC_DGN_CTRLDATA_SUBCODE_CMB)->GetWindowRect(refRect);

	//Modify by tss(GB50018-02)
	if (bGBCode || bGBCode_25)
	{		
		GetDlgItem(IDC_DGN_CTRLDATA_DESIGNCODE)->GetWindowRect(refRect);
		CDlgUtil::CtrlShowHide(this, m_aEtcCtrl, FALSE);
		GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_STRUCT_IMPORT_COEF_EDT)->GetWindowRect(moveRect);
		int nDistY = refRect.bottom - moveRect.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aGBCodeCtrl, nDistY);
		GetDlgItem(IDC_DGN_STRUCT_IMPORT_COEF_EDT)->GetWindowRect(&RectOrg);
		GetDlgItem(IDC_DGN_STRUCT_IMPORT_COEF_EDT)->EnableWindow(TRUE);

		CRect moveRect1, moveRect2;
		GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->GetWindowRect(moveRect1);
		int nDistY1 = moveRect.bottom - moveRect1.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aGBCodeCtrl, nDistY1);
		GetDlgItem(IDC_DGN_STLCTRL_SPECIAL)->GetWindowRect(&RectOrg);

		GetDlgItem(IDC_DGN_STLCTRL_BENDMODULUS_Z_EDIT)->GetWindowRect(moveRect2);
		int nDistY2 = moveRect1.bottom - moveRect2.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aGBCodeCtrl, nDistY2);
		GetDlgItem(IDC_DGN_STLCTRL_BENDMODULUS_Z_EDIT)->GetWindowRect(&RectOrg);
		GetDlgItem(IDC_DGN_SECT_NETGROSS_RATIO_EDT)->EnableWindow(TRUE);

		CheckRadioButton(IDC_DGN_CFCTRL_BEAMCHK_RADIO1, IDC_DGN_CFCTRL_BEAMCHK_RADIO2, IDC_DGN_CFCTRL_BEAMCHK_RADIO1);
		if (bGBCode)
		{
			GetDlgItem(IDC_DGN_STLCTRL_PURLIN_CHECK)->SetWindowText(_LS(IDS_IDD_DGN_CF_CODE_DLG_IDC_DGN_STLCTRL_PURLIN_CHECK));
			GetDlgItem(IDC_DGN_STLCTRL_LOWEREDGE_CHECK)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_CFCTRL_PURLIN_DGN_CASE_STATIC)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_CFCTRL_PURLIN_DGN_CASE_COMBO)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_STLCTRL_UPEDGE_CHECK)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_DGN_STLCTRL_PURLIN_CHECK)->SetWindowText(_LS(IDS_IDD_DGN_CF_CODE_DLG_IDC_DGN_STLCTRL_PURLIN_CHECK_25));
			GetDlgItem(IDC_DGN_STLCTRL_LOWEREDGE_CHECK)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_CFCTRL_PURLIN_DGN_CASE_STATIC)->EnableWindow(TRUE);
			if (m_bLowerEdge)
			{
				CheckDlgButton(IDC_DGN_STLCTRL_LOWEREDGE_CHECK, m_bLowerEdge);
				GetDlgItem(IDC_DGN_CFCTRL_PURLIN_DGN_CASE_COMBO)->EnableWindow(TRUE);
				GetDlgItem(IDC_DGN_STLCTRL_UPEDGE_CHECK)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(IDC_DGN_CFCTRL_PURLIN_DGN_CASE_COMBO)->EnableWindow(FALSE);
				GetDlgItem(IDC_DGN_STLCTRL_UPEDGE_CHECK)->EnableWindow(FALSE);
			}
		}
	}
	else if(bEuroCode)
	{
		GetDlgItem(IDC_DGN_CFCTRL_SPECIAL )->GetWindowRect(moveRect);
		int nDistY = refRect.bottom - moveRect.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aEuroCodeCtrl, nDistY);

		GetDlgItem(IDC_DGN_CFCTRL_SPECIAL)->GetWindowRect(&RectOrg);
		GetDlgItem(IDC_DGN_CFCTRL_SPECIAL)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CTRLDATA_GB    )->GetWindowRect(moveRect);
		int nDistY = refRect.top - moveRect.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_aEtcCtrl, nDistY);

		GetDlgItem(IDC_DGN_CTRLDATA_GB)->GetWindowRect(&RectOrg);
		GetDlgItem(IDC_DGN_CTRLDATA_GB)->EnableWindow(TRUE);
	}

	CRect rDlg;
	rDlg.left = RectDlg.left;
	rDlg.top = RectDlg.top;
	rDlg.right = RectDlg.right;
	rDlg.bottom = RectOrg.bottom + RectOk.Height() + globalUtils.ScaleByDPI(24); //(int)(2.5*RectOk.Height());
	CRect rOk;
	rOk.left = RectOk.left;
	rOk.right = RectOk.right;
	rOk.top = rDlg.bottom - RectOk.Height() - globalUtils.ScaleByDPI(8); //(int)(1.75*RectOk.Height());
	rOk.bottom = rOk.top + RectOk.Height();
	CRect rCn;
	rCn.left = RectCn.left;
	rCn.right = RectCn.right;
	rCn.top = rOk.top;
	rCn.bottom = rOk.bottom;

	ScreenToClient(rDlg);
	ScreenToClient(rOk);
	ScreenToClient(rCn);
	SetWindowPos(NULL, rDlg.left, rDlg.top, rDlg.Width(), rDlg.Height(), SWP_NOMOVE | SWP_NOZORDER);
	GetDlgItem(IDOK    )->SetWindowPos(NULL, rOk.left, rOk.top, rOk.Width(), rOk.Height(), SWP_NOSIZE);
	GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rCn.left, rCn.top, rCn.Width(), rCn.Height(), SWP_NOSIZE);

	UpdateData(FALSE);

	RedrawWindow();
}

void CDgnCfCodeDlg::OnSelchangeDgnCtrldataDesigncode() 
{
	int Index = m_DesignCode.GetCurSel();
	if(m_aCodeEnable[Index]==FALSE)
	{
		CString strCodeName = _T("");
		m_DesignCode.GetLBText(Index, strCodeName);
		CString strErrMsg = _T("");
		strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), strCodeName);
		GSaveHistoryFormatNF(strErrMsg);
		m_DesignCode.SetCurSel(m_nOldCodeIndex);

		return;
	}

	m_nOldCodeIndex = Index;

	SelchangeDgnCtrldataDesigncode(TRUE);
}

void CDgnCfCodeDlg::OnDgnStlChkLowerEdge()
{
	UpdateData(TRUE);
	if (m_bLowerEdge)
	{
		GetDlgItem(IDC_DGN_CFCTRL_PURLIN_DGN_CASE_COMBO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_STLCTRL_UPEDGE_CHECK)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CFCTRL_PURLIN_DGN_CASE_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_STLCTRL_UPEDGE_CHECK)->EnableWindow(FALSE);
	}
	m_WindCase.SetCurSel(0);
	UpdateData(FALSE);
}

void CDgnCfCodeDlg::OnSelchangeDgnWindCase()
{

}

void CDgnCfCodeDlg::InitWindCaseCombx()
{
	m_WindCase.ResetContent();
	CArray<T_STLD_K, T_STLD_K> arStldKey;
	m_pDoc->m_pAttrCtrl2->GetUsedStldKeyList(arStldKey);
	T_STLD_D stldD, LcaseData;
	LcaseData.Initialize();

	int nIdx = 0, nCurSel = 0;;
	for (int i = 0; i < arStldKey.GetCount(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetStld(arStldKey[i], stldD);
		if (stldD.LoadCaseType == _T("W"))
		{
			m_WindCase.AddString(stldD.LoadCaseName);
			nIdx++;
		}
	}
	if (m_pDoc->m_pAttrCtrl->GetStld(m_nWindCase, LcaseData))
		m_WindCase.SetWindowText(LcaseData.LoadCaseName);
	else
		m_WindCase.SetCurSel(0);
}