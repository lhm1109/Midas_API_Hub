// SeisIS02PeriodCalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisIS1893_25PeriodCalcDlg.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_SpecialChar.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CSeisIS1893_25PeriodCalcDlg::CSeisIS1893_25PeriodCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSeisIS1893_25PeriodCalcDlg::IDD, pParent)
{
	m_nXMethod = 0;
	m_nYMethod = 0;
	m_dXAfpedit = 0.0;
	m_dYAfpedit = 0.0;
	m_dXAcoedit = 0.0;
	m_dYAcoedit = 0.0;
	m_dXAwaedit = 0.0;
	m_dYAwaedit = 0.0;
	m_dXAmwedit = 0.0;
	m_dYAmwedit = 0.0;
	m_dXDedit = 0.0;
	m_dYDedit = 0.0;
	m_dXHedit = 0.0;
	m_dYHedit = 0.0;
	m_bNTCFlag = FALSE;
	m_bP100Flag = FALSE;
	m_nBaseStoryK = 0;

	m_Data.Initialize();
	//m_bPrint = false;
}


void CSeisIS1893_25PeriodCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ETC_SEIS_YH, m_wndYHedit);
	DDX_Control(pDX, IDC_ETC_SEIS_XH, m_wndXHedit);
	DDX_Control(pDX, IDC_ETC_SEIS_YD, m_wndYDedit);
	DDX_Control(pDX, IDC_ETC_SEIS_XD, m_wndXDedit);
	DDX_Control(pDX, IDC_CMD_SEIS_YAW, m_wndYAwaedit);
	DDX_Control(pDX, IDC_CMD_SEIS_XAW, m_wndXAwaedit);
	DDX_Control(pDX, IDC_CMD_SEIS_YACO, m_wndYAcoedit);
	DDX_Control(pDX, IDC_CMD_SEIS_XACO, m_wndXAcoedit);
	DDX_Control(pDX, IDC_CMD_SEIS_YAFP, m_wndYAfpedit);
	DDX_Control(pDX, IDC_CMD_SEIS_XAFP, m_wndXAfpedit);
	DDX_Control(pDX, IDC_CMD_SEIS_YAMW, m_wndYAmwedit);
	DDX_Control(pDX, IDC_CMD_SEIS_XAMW, m_wndXAmwedit);
	DDX_Radio(pDX, IDC_RDO_XMETHOD1, m_nXMethod);
	DDX_Radio(pDX, IDC_RDO_YMETHOD1, m_nYMethod);
	DDX_Text(pDX, IDC_CMD_SEIS_XAW, m_dXAwaedit);
	DDX_Text(pDX, IDC_CMD_SEIS_YAW, m_dYAwaedit);
	DDX_Text(pDX, IDC_CMD_SEIS_XACO, m_dXAcoedit);
	DDX_Text(pDX, IDC_CMD_SEIS_YACO, m_dYAcoedit);
	DDX_Text(pDX, IDC_CMD_SEIS_XAFP, m_dXAfpedit);
	DDX_Text(pDX, IDC_CMD_SEIS_YAFP, m_dYAfpedit);
	DDX_Text(pDX, IDC_CMD_SEIS_XAMW, m_dXAmwedit);
	DDX_Text(pDX, IDC_CMD_SEIS_YAMW, m_dYAmwedit);
	DDX_Text(pDX, IDC_ETC_SEIS_XD, m_dXDedit);
	DDX_Text(pDX, IDC_ETC_SEIS_YD, m_dYDedit);
	DDX_Text(pDX, IDC_ETC_SEIS_XH, m_dXHedit);
	DDX_Text(pDX, IDC_ETC_SEIS_YH, m_dYHedit);
	DDX_Control(pDX, IDC_IS_CALC_PERIOD_REPORT_BTN, m_btnReport);
	DDX_Control(pDX, IDC_CMB_BASE_LEVEL_STORY, m_cmbBaseStory);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisIS1893_25PeriodCalcDlg, CDialogMove)
	//{{AFX_MSG_MAP(CSeisIS1893_25PeriodCalcDlg)
	ON_BN_CLICKED(IDC_RDO_XMETHOD1, OnRdoXmethod)
	ON_BN_CLICKED(IDC_RDO_YMETHOD1, OnRdoYmethod)
	ON_BN_CLICKED(IDC_RDO_XMETHOD2, OnRdoXmethod)
	ON_BN_CLICKED(IDC_RDO_YMETHOD2, OnRdoYmethod)
	ON_BN_CLICKED(IDC_RDO_XMETHOD3, OnRdoXmethod)
	ON_BN_CLICKED(IDC_RDO_YMETHOD3, OnRdoYmethod)
	ON_BN_CLICKED(IDC_RDO_XMETHOD4, OnRdoXmethod)
	ON_BN_CLICKED(IDC_RDO_YMETHOD4, OnRdoYmethod)
	ON_BN_CLICKED(IDC_RDO_XMETHOD5, OnRdoXmethod)
	ON_BN_CLICKED(IDC_RDO_YMETHOD5, OnRdoYmethod)
	ON_BN_CLICKED(IDC_RDO_XMETHOD6, OnRdoXmethod)
	ON_BN_CLICKED(IDC_RDO_YMETHOD6, OnRdoYmethod)
	ON_BN_CLICKED(IDC_RDO_XMETHOD7, OnRdoXmethod)
	ON_BN_CLICKED(IDC_RDO_YMETHOD7, OnRdoYmethod)
	ON_BN_CLICKED(IDC_IS_CALC_PERIOD_REPORT_BTN, OnButtonReport)
	ON_CBN_SELCHANGE(IDC_CMB_BASE_LEVEL_STORY, OnBaseStoryChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisIS1893_25PeriodCalcDlg message handlers
BOOL CSeisIS1893_25PeriodCalcDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	EnableDisableWindow();

	T_UNIT_INDEX OrgUnitIndex = SetCodeUnits();

	Dlg2Data();
	InitBaseStoryCmb();
	if(dgn::EQ0(m_Data.DirX.dH) || dgn::EQ0(m_Data.DirY.dH))
	{
		CalculateHeight();
	}
	if(dgn::EQ0(m_Data.DirX.dD) || dgn::EQ0(m_Data.DirY.dD))
	{
		SetStoryWidthD();
	}
	CSeisIS1893_25PeriodCalcHelper PeriodHelper(m_Data.nBaseStoryK);
	bool bCalcX = dgn::EQ0(m_Data.DirX.dAfp);
	bool bCalcY = dgn::EQ0(m_Data.DirY.dAfp);
	PeriodHelper.CalcTimePeriod(m_Data.DirX, EN_IS1893_PC_DIR_X, NULL, false, bCalcX);
	PeriodHelper.CalcTimePeriod(m_Data.DirY, EN_IS1893_PC_DIR_Y, NULL, false, bCalcY);

	Data2Dlg();

	CString strUnit = _T("");
	strUnit.Format(_T("(m%s)"), D_CH_2SQUARE);
	GetDlgItem(IDC_CMD_ACX_UNIT)->SetWindowText(strUnit);
	GetDlgItem(IDC_CMD_ACX_UNIT2)->SetWindowText(strUnit);

	RestoreOriginalUnits(OrgUnitIndex);

	UpdateData(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

T_UNIT_INDEX CSeisIS1893_25PeriodCalcDlg::SetCodeUnits()
{
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;

	T_UNIT_INDEX CurIndex, CodeIndex;
	pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CodeIndex = CurIndex;
	CodeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M; // M ±âÁØ
	pUnitCtrl->SetUnitIndexCurrent(CodeIndex);
	return CurIndex;
}

void CSeisIS1893_25PeriodCalcDlg::RestoreOriginalUnits(T_UNIT_INDEX& rCurIndex)
{
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;
	pUnitCtrl->SetUnitIndexCurrent(rCurIndex);
}

void CSeisIS1893_25PeriodCalcDlg::EnableDisableWindow()
{
	m_wndXHedit.EnableWindow(m_nXMethod == 0 || m_nXMethod == 1 || m_nXMethod == 2 || m_nXMethod == 3 || m_nXMethod == 5 || m_nXMethod == 6);
	m_wndYHedit.EnableWindow(m_nYMethod == 0 || m_nYMethod == 1 || m_nYMethod == 2 || m_nYMethod == 3 || m_nXMethod == 5 || m_nXMethod == 6);
	m_wndXAcoedit.EnableWindow(m_nXMethod == 3);
	m_wndYAcoedit.EnableWindow(m_nXMethod == 3);
	m_wndXAwaedit.EnableWindow(FALSE);
	m_wndYAwaedit.EnableWindow(FALSE);
	m_wndXAfpedit.EnableWindow(m_nXMethod == 3);
	m_wndYAfpedit.EnableWindow(m_nYMethod == 3);
	m_wndXAmwedit.EnableWindow(m_nXMethod == 3);
	m_wndYAmwedit.EnableWindow(m_nYMethod == 3);
	m_wndXDedit.EnableWindow(m_nXMethod == 4);
	m_wndYDedit.EnableWindow(m_nYMethod == 4);
	m_btnReport.EnableWindow(m_nXMethod == 3 || m_nYMethod == 3);
}

void CSeisIS1893_25PeriodCalcDlg::InitBaseStoryCmb()
{
	if(m_cmbBaseStory.GetCount() != 0) m_cmbBaseStory.ResetContent();

	CArray<CString, CString&> strStoryList;
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetStorList(strStoryList);
	int nCount = strStoryList.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		CString tempStoryList = _T("");
		tempStoryList = strStoryList.GetAt(i);
		m_cmbBaseStory.AddString(tempStoryList);
	}
	T_STOR_D StorD; StorD.Initialize();
	pDoc->m_pAttrCtrl->GetStor(m_nBaseStoryK, StorD);
	m_cmbBaseStory.SelectString(0, StorD.StoryName);
}

void CSeisIS1893_25PeriodCalcDlg::CalculateHeight()
{
	T_UNIT_INDEX OrgUnitIndex = SetCodeUnits();

	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	T_STOR_D storD; storD.Initialize();
	pDoc->m_pAttrCtrl->GetStor(m_nBaseStoryK, storD);
	double dCurHeight = storD.dStoryLevel;
	CArray<T_STOR_D, T_STOR_D&> arStorD; arStorD.RemoveAll();
	pDoc->m_pAttrCtrl->GetStorData(arStorD);
	int nCount = arStorD.GetSize();
	double dMaxHeight = -1.0 * DBL_MAX;
	for(int i = 0; i < nCount; i++)
	{
		if(arStorD.GetAt(i).dStoryLevel > dMaxHeight)
		{
			dMaxHeight = arStorD.GetAt(i).dStoryLevel;
		}
	}
	double dHeight = dMaxHeight - dCurHeight;
	m_Data.DirX.dH = dHeight;
	m_Data.DirY.dH = dHeight;

	RestoreOriginalUnits(OrgUnitIndex);

}

void CSeisIS1893_25PeriodCalcDlg::SetStoryWidthD()
{
	T_UNIT_INDEX OrgUnitIndex = SetCodeUnits();

	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	T_STOR_D StorD; StorD.Initialize();
	pDoc->m_pAttrCtrl->GetStor(m_nBaseStoryK, StorD);
	m_Data.DirX.dD = StorD.WindWidthX;
	m_Data.DirY.dD = StorD.WindWidthY;

	RestoreOriginalUnits(OrgUnitIndex);
}

void CSeisIS1893_25PeriodCalcDlg::OnRdoXmethod()
{
	UpdateData(TRUE);
	m_wndXHedit.EnableWindow(m_nXMethod == 0 || m_nXMethod == 1 || m_nXMethod == 2 || m_nXMethod == 3 || m_nXMethod == 4 || m_nXMethod == 5 || m_nXMethod == 6);
	m_wndXAcoedit.EnableWindow(m_nXMethod == 3);
	m_wndXAwaedit.EnableWindow(FALSE);
	m_wndXAfpedit.EnableWindow(m_nXMethod == 3);
	m_wndXAmwedit.EnableWindow(m_nXMethod == 3);
	m_wndXDedit.EnableWindow(m_nXMethod == 4);
	m_btnReport.EnableWindow(m_nXMethod == 3 || m_nYMethod == 3);
}

void CSeisIS1893_25PeriodCalcDlg::OnRdoYmethod()
{
	UpdateData(TRUE);
	m_wndYHedit.EnableWindow(m_nYMethod == 0 || m_nYMethod == 1 || m_nYMethod == 2 || m_nYMethod == 3 || m_nYMethod == 4 || m_nYMethod == 5 || m_nYMethod == 6);
	m_wndYAcoedit.EnableWindow(m_nYMethod == 3);
	m_wndYAwaedit.EnableWindow(FALSE);
	m_wndYAfpedit.EnableWindow(m_nYMethod == 3);
	m_wndYAmwedit.EnableWindow(m_nYMethod == 3);
	m_wndYDedit.EnableWindow(m_nYMethod == 4);
	m_btnReport.EnableWindow(m_nXMethod == 3 || m_nYMethod == 3);
}

void CSeisIS1893_25PeriodCalcDlg::OnBaseStoryChange()
{
	T_UNIT_INDEX OrgUnitIndex = SetCodeUnits();
	int Index = m_cmbBaseStory.GetCurSel();
	if(Index == -1)   return;

	CString strStory = _T("");
	m_cmbBaseStory.GetLBText(Index, strStory);

	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	T_STOR_D StorD; StorD.Initialize();
	m_nBaseStoryK = pDoc->m_pAttrCtrl->GetStorKey(strStory);

	Dlg2Data();
	CalculateHeight();
	SetStoryWidthD();
	CSeisIS1893_25PeriodCalcHelper PeriodHelper(m_Data.nBaseStoryK);
	PeriodHelper.CalcTimePeriod(m_Data.DirX, EN_IS1893_PC_DIR_X, NULL, false, true);
	PeriodHelper.CalcTimePeriod(m_Data.DirY, EN_IS1893_PC_DIR_Y, NULL, false, true);

	Data2Dlg();
	RestoreOriginalUnits(OrgUnitIndex);
}
BOOL CSeisIS1893_25PeriodCalcDlg::CalcPeriod(int nDir, double& dPeriod, BOOL bPrint)
{
	T_NPC_IS1893_2025_DIR_D& rDir = ( nDir == EN_IS1893_PC_DIR_X ) ? m_Data.DirX : m_Data.DirY;
	double dPeriod1 = 0.0;
	double dPeriod2 = 0.0;
	switch(rDir.nMethod)
	{
	case 0: dPeriod = 0.075 * pow(rDir.dH, ( 3.0 / 4.0 ));	break;
	case 1:	dPeriod = 0.080 * pow(rDir.dH, ( 3.0 / 4.0 ));	break;
	case 2:	dPeriod = 0.085 * pow(rDir.dH, ( 3.0 / 4.0 ));	break;
	case 3:
	{
		dPeriod1 = 0.09 * rDir.dH / sqrt(rDir.dD);
		T_NPC_IS1893_2025_DIR_D rData = nDir == EN_IS1893_PC_DIR_X ? m_Data.DirX : m_Data.DirY;
		CSeisIS1893_25PeriodCalcHelper PeriodHelper(m_nBaseStoryK);
		dPeriod2 = PeriodHelper.CalcTimePeriod(rData, nDir, m_Fout, bPrint, false);
		dPeriod = dPeriod2 >= dPeriod1 ? dPeriod2 : dPeriod1;
		break;
	}
	case 4:
	{
		dPeriod = 0.09 * rDir.dH / sqrt(rDir.dD);
		break;
	}
	case 5: dPeriod = 0.0644 * pow(rDir.dH, 0.9);	break;
	case 6: dPeriod = 0.0672 * pow(rDir.dH, ( 3.0 / 4.0 ));	break;
	}
	return TRUE;
}

void CSeisIS1893_25PeriodCalcDlg::OnOK()
{
	T_UNIT_INDEX OrgUnitIndex = SetCodeUnits();

	UpdateData(TRUE);
	Dlg2Data();
	if(!CheckData()) { return; }
	CSeisIS1893_25PeriodCalcHelper PeriodHelper(m_nBaseStoryK);
	CalcPeriod(EN_IS1893_PC_DIR_X, m_dPeriod_X, TRUE);
	CalcPeriod(EN_IS1893_PC_DIR_Y, m_dPeriod_Y, TRUE);

	Data2Dlg();
	RestoreOriginalUnits(OrgUnitIndex);
	CDialogMove::OnOK();
}

bool CSeisIS1893_25PeriodCalcDlg::CheckData()
{
	if(dgn::LE0(m_Data.DirX.dH) || dgn::LE0(m_Data.DirY.dH))
	{
		AfxMessageBox(_LS(IDS_CMD0417__Warning__Invalid_value_of_Building_Hei));
		return false;
	}
	if(!CheckDataDir(m_Data.DirX)) { return false; }
	if(!CheckDataDir(m_Data.DirX)) { return false; }
	return true;
}

bool CSeisIS1893_25PeriodCalcDlg::CheckDataDir(T_NPC_IS1893_2025_DIR_D& rData)
{
	switch(rData.nMethod)
	{
	case 3:
	{
		if(rData.dAw < 0.0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__the_value_Aw_is_invalid_));
			return false;
		}
		if(rData.dAc < 0.0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__the_value_Ac_is_invalid_));
			return false;
		}
		if(rData.dAfp <= 0.0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__the_value_Afp_is_invalid_));
			return false;
		}
		if(rData.dAmw < 0.0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__the_value_Amw_is_invalid_));
			return false;
		}
	}
	break;
	case 4:
		if(rData.dD <= 0.0)
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__the_value_d_is_invalid_));
			return false;
		}
	default:
		break;
	}
	return true;
}

void CSeisIS1893_25PeriodCalcDlg::OnButtonReport()
{
	T_UNIT_INDEX OrgUnitIndex = SetCodeUnits();
	UpdateData(TRUE);

	Dlg2Data();
	if(!CheckData()) { return; }

	GetFilePath();
	if(!SetFile(m_strFile)) return;

	CalcPeriod(EN_IS1893_PC_DIR_X, m_dPeriod_X, TRUE);
	CalcPeriod(EN_IS1893_PC_DIR_Y, m_dPeriod_Y, TRUE);

	if(m_Fout)
	{
		fclose(m_Fout);
		m_Fout = nullptr;
	}

	RestoreOriginalUnits(OrgUnitIndex);

	CDBLib::RunTextEditor(
		m_strFile,
		_T("IS 1893:2025 Period Calculation")
	);
}

BOOL CSeisIS1893_25PeriodCalcDlg::SetFile(const CString& strFile)
{
	CFileStatus FStatus;
	if(strFile == _T("")) return FALSE;

	if(!CFile::GetStatus(strFile, FStatus)) // file doesn't exist
	{
		if(( m_Fout = _tfopen(strFile, _T("w")) ) == NULL) return FALSE;
	}
	else                                     // file exist
	{
		if(AfxMessageBox(_LS(IDS_DB_TOOL_File_Already_Exist), MB_YESNO) == IDYES)  // _T("The file already exists, overwrite?\nSelect 'No' to add")
		{
			if(( m_Fout = _tfopen(strFile, _T("w")) ) == NULL) return FALSE;
		}
		else
			if(( m_Fout = _tfopen(strFile, _T("a")) ) == NULL) return FALSE;
	}

#ifdef _CH
	_wsetlocale(0, _T("chs"));
#endif

	m_strFile = strFile;

	return TRUE;
}

void CSeisIS1893_25PeriodCalcDlg::GetFilePath()
{
	// FileName;
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	m_strFile = pDoc->GetPathName();
	if(m_strFile.IsEmpty())
	{
		TCHAR* lpBuffer = new TCHAR[255];
		GetCurrentDirectory(255, lpBuffer);
		m_strFile = lpBuffer;
		m_strFile += _T("\\Untitled.npc");
		delete[] lpBuffer;
	}
	else
	{
		TCHAR path_buffer[_MAX_PATH];
		TCHAR drive[_MAX_DRIVE];
		TCHAR dir[_MAX_DIR];
		TCHAR fname[_MAX_FNAME];
		TCHAR ext[_MAX_EXT];

		m_strFile.GetBuffer(255);
		_tcscpy(path_buffer, m_strFile);
		m_strFile.ReleaseBuffer();

		_tsplitpath(path_buffer, drive, dir, fname, ext);
		_tmakepath(path_buffer, drive, dir, fname, _T("npc"));
		m_strFile = path_buffer;
	}
}


bool CSeisIS1893_25PeriodCalcDlg::Data2Dlg()
{
	m_nBaseStoryK = m_Data.nBaseStoryK;

	m_nXMethod = m_Data.DirX.nMethod;
	m_nYMethod = m_Data.DirY.nMethod;
	m_wndXAwaedit.SetEditUnit(m_Data.DirX.dAw,3);
	m_wndYAwaedit.SetEditUnit(m_Data.DirY.dAw,3);
	m_wndXAcoedit.SetEditUnit(m_Data.DirX.dAc,3);
	m_wndYAcoedit.SetEditUnit(m_Data.DirY.dAc,3);
	m_wndXAfpedit.SetEditUnit(m_Data.DirX.dAfp,3);
	m_wndYAfpedit.SetEditUnit(m_Data.DirY.dAfp,3);
	m_wndXAmwedit.SetEditUnit(m_Data.DirX.dAmw,3);
	m_wndYAmwedit.SetEditUnit(m_Data.DirY.dAmw,3);
	m_wndXDedit.SetEditUnit(m_Data.DirX.dD,3);
	m_wndYDedit.SetEditUnit(m_Data.DirY.dD,3);
	m_wndXHedit.SetEditUnit(m_Data.DirX.dH,3);
	m_wndYHedit.SetEditUnit(m_Data.DirY.dH,3);

	m_dXAwaedit = m_Data.DirX.dAw;
	m_dYAwaedit = m_Data.DirY.dAw;
	m_dXAcoedit = m_Data.DirX.dAc;
	m_dYAcoedit = m_Data.DirY.dAc;
	m_dXAfpedit = m_Data.DirX.dAfp;
	m_dYAfpedit = m_Data.DirY.dAfp;
	m_dXAmwedit = m_Data.DirX.dAmw;
	m_dYAmwedit = m_Data.DirY.dAmw;
	m_dXDedit = m_Data.DirX.dD;
	m_dYDedit = m_Data.DirY.dD;
	m_dXHedit = m_Data.DirX.dH;
	m_dYHedit = m_Data.DirY.dH;
	return true;
}

bool CSeisIS1893_25PeriodCalcDlg::Dlg2Data()
{
	m_Data.nBaseStoryK = m_nBaseStoryK;
	m_Data.DirX.nMethod = m_nXMethod;
	m_Data.DirY.nMethod = m_nYMethod;
	m_Data.DirX.dAw = m_dXAwaedit;
	m_Data.DirY.dAw = m_dYAwaedit;
	m_Data.DirX.dAc = m_dXAcoedit;
	m_Data.DirY.dAc = m_dYAcoedit;
	m_Data.DirX.dAfp = m_dXAfpedit;
	m_Data.DirY.dAfp = m_dYAfpedit;
	m_Data.DirX.dAmw = m_dXAmwedit;
	m_Data.DirY.dAmw = m_dYAmwedit;
	m_Data.DirX.dD = m_dXDedit;
	m_Data.DirY.dD = m_dYDedit;
	m_Data.DirX.dH = m_dXHedit;
	m_Data.DirY.dH = m_dYHedit;
	return true;
}