// CMSectItemPSCValueDBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCValueDBDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\CMSectImportSPC.h"
#include "..\wg_db\IUsageCounter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCValueDBDlg dialog


CCMSectItemPSCValueDBDlg::CCMSectItemPSCValueDBDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectItemPSCValueDBDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSCValueDBDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_SectD.Initialize();
	m_aImportID.RemoveAll();
	m_aImportSect.RemoveAll();

	m_nCodeForLog = 0; // MNET:XXXX 20070613 mylee - 로그 분석 위해

}


void CCMSectItemPSCValueDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSCValueDBDlg)
	DDX_Control(pDX, IDC_CMD_SECT_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_TYPE_CMB, m_cmbType);
	DDX_Control(pDX, IDC_CMD_CODE_CMB, m_cmbCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSectItemPSCValueDBDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectItemPSCValueDBDlg)
	ON_CBN_SELCHANGE(IDC_CMD_CODE_CMB, OnSelchangeCmdCodeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_TYPE_CMB, OnSelchangeCmdTypeCmb)
	ON_LBN_SELCHANGE(IDC_CMD_SECT_LIST, OnSelchangeCmdSectList)
	ON_BN_CLICKED(IDOK, OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////
// user customizing
void CCMSectItemPSCValueDBDlg::SetInitCode()
{
	m_cmbCode.ResetContent();
	m_cmbCode.AddString(_LS(IDS_CMD_PSC_VALUE_DB_CODE_UK));
	m_cmbCode.AddString(_LS(IDS_CMD_PSC_VALUE_DB_CODE_ITALY));
	m_cmbCode.AddString(_LS(IDS_CMD_PSC_VALUE_DB_CODE_USA));
	m_cmbCode.AddString(_LS(IDS_CMD_PSC_VALUE_DB_CODE_CANADA));
	m_cmbCode.AddString(_LS(IDS_CMD_PSC_VALUE_DB_CODE_AS));
	m_cmbCode.AddString(_LS(IDS_CMD_PSC_VALUE_DB_CODE_NZ));
	m_cmbCode.AddString(_LS(IDS_CMD_PSC_VALUE_DB_CODE_RUSSIA));
	m_cmbCode.SetCurSel(0);
}

int CCMSectItemPSCValueDBDlg::GetTypeListByCode(const CString strCode, CStringArray &aList)
{
	aList.RemoveAll();
	if(strCode==_LS(IDS_CMD_PSC_VALUE_DB_CODE_UK))
	{
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_UK_M));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_UK_UMB));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_UK_MY));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_UK_MYE));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_UK_Solid_Box));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_UK_SY));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_UK_SYE));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_UK_T));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_UK_TY_Rebate));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_UK_TYE_Rebate));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_UK_TY));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_UK_TYE));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_UK_U_SU));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_UK_W));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_UK_Y));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_UK_YE));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_UK_UM));
	}
	else if(strCode==_LS(IDS_CMD_PSC_VALUE_DB_CODE_ITALY))
	{
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_Italy_UH));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_Italy_VH));
	}
	else if(strCode==_LS(IDS_CMD_PSC_VALUE_DB_CODE_USA))
	{		
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_FDOT));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_IADOT));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_ILDOT));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_massDOT));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_MoDOT));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_LADOT));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_ODOT));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_RIDOT));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_TXDOT));	
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_WIDOT));
	}
	else if (strCode == _LS(IDS_CMD_PSC_VALUE_DB_CODE_CANADA))
	{
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_ONTARIO));		
	}	
	else if(strCode==_LS(IDS_CMD_PSC_VALUE_DB_CODE_AS))
	{
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_AS_Super_T_RMS_2019));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_AS_Super_T_RMS_Pre2019));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_AS_I_Girder));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_AS_Plank_Girder));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_AS_PSC_Deck_Unit_TMR_QLD));
	}
	else if(strCode==_LS(IDS_CMD_PSC_VALUE_DB_CODE_NZ))
	{
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_NZ_Super_T));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_NZ_I_Girder));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_NZ_Hollow_Core));
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_NZ_SuperHI));
	}
	else if(strCode==_LS(IDS_CMD_PSC_VALUE_DB_CODE_RUSSIA))
	{
		aList.Add(_LS(IDS_CMD_PSC_VALUE_DB_CODE_TYPE_Soyuzdorproekt));
	}
	else
	{
		ASSERT(false);
	}
	
	aList.FreeExtra();
	return aList.GetSize();
}
///////////////////////////////////////////////////////////////////////


void CCMSectItemPSCValueDBDlg::SetDataSource(T_SECT_D *pSectD)
{  
	ASSERT(pSectD);
	m_wndSecView.SetDataSource(pSectD, FALSE);
	m_wndSecView.SetShearCheckFlag(FALSE);
	m_wndSecView.SetCentroidFlag(FALSE);
	m_wndSecView.Invalidate();
}

void CCMSectItemPSCValueDBDlg::SetSectDBList(const CString strType)
{
	m_aImportID.RemoveAll(), m_aImportSect.RemoveAll();
	
	CString strFileName = _T("");
	strFileName = GetSectDBFileName(strType);
	if(strFileName.IsEmpty())
	{
		AfxMessageBox(_LS(IDS_CMD_ERROR__NOT_EXIST_PSC_SECTDB));
		return;
	}
	CCMSectImportSPC Importer;
	if(!Importer.Import(strFileName))
	{
		AfxMessageBox(_LS(IDS_CMD_ERROR__NOT_EXIST_PSC_SECTDB));
		return;
	}
	Importer.GetList(m_aImportID, m_aImportSect);

	m_List.ResetContent();

	int nCount = m_aImportID.GetSize();
	if(nCount < 1)
	{
		AfxMessageBox(_LS(IDS_CMD_ERROR__NOT_EXIST_PSC_SECTDB));
		return;
	}

	CString strList = _T("");
	for(int i=0 ; i < nCount; i++)
	{
		strList.Format(_T("%d:%s"), m_aImportID[i], m_aImportSect[i].SName);
		m_List.SetItemData(m_List.AddString(strList), m_aImportID[i]);
	}
	m_List.SetCurSel(0);
}

CString CCMSectItemPSCValueDBDlg::GetSectDBFileName(const CString strType)
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CString strFileName = pDoc->GetProgramPath()+pDoc->GetDBDirectory()+strType+_T(".sec");
	return strFileName;
}

BOOL CCMSectItemPSCValueDBDlg::GetCurSectData(const int nSel)
{
	if(nSel<0 || nSel>=m_aImportID.GetSize())
	{
		AfxMessageBox(_LS(IDS_CMD_ERROR__NOT_EXIST_SELECTED_SECTDB));
		return FALSE;
	}
	m_SectD = m_aImportSect[nSel];
	m_SectD.SectBefore.Shape = D_SECT_SHAPE_PSC_VALUE;

	CString strCode = _T("");
	m_cmbCode.GetLBText(m_cmbCode.GetCurSel(), strCode);
	if(strCode==_LS(IDS_CMD_PSC_VALUE_DB_CODE_AS) || strCode==_LS(IDS_CMD_PSC_VALUE_DB_CODE_NZ))
	{
		m_SectD.SectBefore.SectI.SName = m_SectD.SName;
	}

	SetDataSource(&m_SectD);

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCValueDBDlg message handlers

BOOL CCMSectItemPSCValueDBDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	CWnd* pWnd = GetDlgItem(IDC_CMD_SECT_VIEW);
	ASSERT(pWnd);
	m_wndSecView.Init(pWnd);

	SetInitCode();

	OnSelchangeCmdCodeCmb();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectItemPSCValueDBDlg::OnSelchangeCmdCodeCmb() 
{
	CString strCode = _T("");
	m_cmbCode.GetLBText(m_cmbCode.GetCurSel(), strCode);

	m_nCodeForLog = m_cmbCode.GetCurSel(); // MNET:XXXX 20070613 mylee - 로그 분석 위해

	m_cmbType.ResetContent();
	CStringArray aTypeList;
	int nSize = GetTypeListByCode(strCode, aTypeList);
	for(int i=0; i<nSize; ++i)
	{
		m_cmbType.AddString(aTypeList[i]);
	}
	m_cmbType.SetCurSel(0);
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbType);
	OnSelchangeCmdTypeCmb();
}

void CCMSectItemPSCValueDBDlg::OnSelchangeCmdTypeCmb() 
{
	CString strType = _T("");
	m_cmbType.GetLBText(m_cmbType.GetCurSel(), strType);

	SetSectDBList(strType);
	OnSelchangeCmdSectList();
}

void CCMSectItemPSCValueDBDlg::OnSelchangeCmdSectList() 
{
	int nSel = m_List.GetCurSel();
	GetCurSectData(nSel);
}

void CCMSectItemPSCValueDBDlg::OnOK()
{
	CString strCode = _T("");
	m_cmbCode.GetLBText(m_cmbCode.GetCurSel(), strCode);

	// 기능 로그 반영
	if(strCode == _LS(IDS_CMD_PSC_VALUE_DB_CODE_AS)) IUsageCounter::Use(_T("SPAS"));
	else if(strCode == _LS(IDS_CMD_PSC_VALUE_DB_CODE_NZ)) IUsageCounter::Use(_T("SPNZ"));

	CDialogMove::OnOK();
}
