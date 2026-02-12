// CMSpecModeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSpecModeDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSpecModeDlg dialog

CCMSpecModeDlg::CCMSpecModeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSpecModeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSpecModeDlg)
	m_nSignType = 0;	
	//}}AFX_DATA_INIT
	m_aCtrlRadio.Add(IDC_CMD_SPEC_TYPE_SRSS);
	m_aCtrlRadio.Add(IDC_CMD_SPEC_TYPE_CQC);
	m_aCtrlRadio.Add(IDC_CMD_SPEC_TYPE_ABS);
	m_aCtrlRadio.Add(IDC_CMD_SPEC_TYPE_LINEAR);

	m_arCtrlSignType.Add(IDC_CMD_SPEC_RESULT_DIRECTION);
	m_arCtrlSignType.Add(IDC_CMD_SPEC_RESULT_ABSOLUTE);  
	m_arCtrlHide.Add(IDC_CMD_SPEC_MODE_GRUP);
	m_arCtrlHide.Add(IDC_CMD_SPEC_MODE_GRID);
	m_arCtrlHide.Add(IDC_CMD_SPEC_ALL_BTN);
	m_arCtrlHide.Add(IDC_CMD_SPEC_NONE_BTN);
	m_arCtrlMove.Add(IDOK);
	m_arCtrlMove.Add(IDCANCEL);
	m_bShifted=FALSE;

}


void CCMSpecModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSpecModeDlg)	
	DDX_Control(pDX, IDC_CMD_SPEC_TYPE_RESULTSIGN, m_chkResultSign);
	DDX_Control(pDX, IDC_CMD_SPEC_SELECT_MODE_CHK, m_chkSelectUse);
	DDX_Radio(pDX, IDC_CMD_SPEC_RESULT_DIRECTION, m_nSignType);	
	DDX_Control(pDX, IDC_CMD_SPEC_MODE_GRID, m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSpecModeDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSpecModeDlg)
	ON_BN_CLICKED(IDC_CMD_SPEC_TYPE_SRSS, OnClickRadio)
	ON_BN_CLICKED(IDC_CMD_SPEC_TYPE_RESULTSIGN, OnCmdSpecTypeResultsign)
	ON_BN_CLICKED(IDC_CMD_SPEC_SELECT_MODE_CHK, OnCmdSpecSelectModeChk)
	ON_BN_CLICKED(IDC_CMD_SPEC_ALL_BTN, OnCmdSpecAllBtn)
	ON_BN_CLICKED(IDC_CMD_SPEC_TYPE_CQC, OnClickRadio)
	ON_BN_CLICKED(IDC_CMD_SPEC_TYPE_ABS, OnClickRadio)	
	ON_BN_CLICKED(IDC_CMD_SPEC_NONE_BTN, OnCmdSpecNoneBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSpecModeDlg message handlers

BOOL CCMSpecModeDlg::OnInitDialog() 
{
 	CDialogMove::OnInitDialog();
	InitControls();

	
	// TODO: Add extra initialization here

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRadio, m_pData->nSpecCombType);
 	m_chkResultSign.SetCheck(m_pData->bResultSign);
	if (m_pData->nAngleType == 1)
		m_chkResultSign.EnableWindow(FALSE);

	m_chkSelectUse.SetCheck(m_pData->bSelectUse);
	CDlgUtil::CtrlEnableDisable(this, m_arCtrlSignType, m_pData->bResultSign == TRUE);
	m_nSignType = m_pData->nSignType;
	OnCmdSpecSelectModeChk();

	IsExistEigvComplex();
	
	CString csVal;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSpecModeDlg::OnClickRadio() 
{
	// TODO: Add your control notification handler code here
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadio, nCheck);
	
}

void CCMSpecModeDlg::OnOK() 
{
	UpdateData(TRUE);

	int nCheck;
	int nCheckedCount=0;
	for(int i=0;i<m_pData->aUseMode.GetSize();i++)
	{
		if(m_pData->aUseMode[i])
			nCheckedCount++;
	}
	if(nCheckedCount==0&&m_pData->bSelectUse)
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_Mode));
		return;
	}
	
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRadio, nCheck);
	
	m_pData->nSpecCombType = nCheck;

	m_pData->bResultSign = m_chkResultSign.GetCheck();
	m_pData->bSelectUse = m_chkSelectUse.GetCheck();
	m_pData->nSignType = m_nSignType;
	
	CDialogMove::OnOK();
}

void CCMSpecModeDlg::OnCmdSpecTypeResultsign() 
{
	CDlgUtil::CtrlEnableDisable(this, m_arCtrlSignType, m_chkResultSign.GetCheck() == TRUE);
}
void CCMSpecModeDlg::SetDataSource(T_SPLC_D* pData)
{
	m_pData=pData;

}
void CCMSpecModeDlg::InitControls()
{
	m_wndGrid.InitGrid();
	m_wndGrid.SetDataSource(m_pData);

}



void CCMSpecModeDlg::OnCmdSpecSelectModeChk() 
{
	BOOL bShowHide;
	m_pData->bSelectUse = m_chkSelectUse.GetCheck();
	bShowHide=m_pData->bSelectUse;
	CDlgUtil:: CtrlShowHide (this, m_arCtrlHide, bShowHide);

	CRect rRef, rMove;
	GetDlgItem(IDC_CMD_SPEC_MODE_GRUP)->GetWindowRect(rRef);
	CArray<UINT,UINT> arCtrlXMove;
	arCtrlXMove.Add(IDC_CMD_SPEC_SELECT_MODE_CHK);
	double dDistY=(rRef.bottom-rRef.top)*0.9;
	double dDistX=(rRef.right-rRef.left)/22.0;
	if(bShowHide)
	{
		if(m_bShifted)
		{
			CDlgUtil::CtrlMoveDistY(this, m_arCtrlMove, dDistY);
			//CDlgUtil::CtrlMoveDistX(this, arCtrlXMove, dDistX); 체크박스 Off 일때 왼쪽으로 이동한 결과 체크박스가 짤려보녀 주석처리
			m_bShifted=FALSE;
		}
	}
	else
	{
		CDlgUtil::CtrlMoveDistY(this, m_arCtrlMove, -dDistY);
		//CDlgUtil::CtrlMoveDistX(this, arCtrlXMove, -dDistX);
		m_bShifted=TRUE;
		for(int i=0;i<m_pData->aUseMode.GetSize();i++)
		{
			m_pData->aUseMode[i]=TRUE;
			m_pData->aMSFactor[i]=1.0;
		}
	}

	GetDlgItem(IDOK)->GetWindowRect(rRef);
	GetWindowRect(rMove);
	rMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(8);  
	MoveWindow(rMove);
	m_wndGrid.ShowData();
	
}

void CCMSpecModeDlg::OnCmdSpecAllBtn() 
{
	for(int i=0;i<m_pData->aUseMode.GetSize();i++)
	{
		m_pData->aUseMode[i]=TRUE;
	}
	m_wndGrid.ShowData();
}

void CCMSpecModeDlg::OnCmdSpecNoneBtn() 
{
	for(int i=0;i<m_pData->aUseMode.GetSize();i++)
	{
		m_pData->aUseMode[i]=FALSE;
	}
	m_wndGrid.ShowData();
	
}

void CCMSpecModeDlg::IsExistEigvComplex()
{
	T_EIGV_D EigvD;
	EigvD.Initialize();
	BOOL bExistEigv;
	if (CDBDoc::GetDocPoint()->m_pAttrCtrl->GetEigv(EigvD) && EigvD.nType > 2)
		bExistEigv = TRUE;
	else
		bExistEigv = FALSE;

	CArray<UINT, UINT> arCtrlHide;
	arCtrlHide.Add(IDC_CMD_SPEC_TYPE_ABS);
	arCtrlHide.Add(IDC_CMD_SPEC_TYPE_LINEAR);
	CDlgUtil::CtrlShowHide(this, arCtrlHide, !bExistEigv);

	if (bExistEigv)
	{
		GetDlgItem(IDC_CMD_SPEC_TYPE_SRSS)->SetWindowText(_T("CSRSS"));
		GetDlgItem(IDC_CMD_SPEC_TYPE_CQC)->SetWindowText(_T("CCQC"));
	}
}