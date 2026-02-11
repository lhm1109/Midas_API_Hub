// CMAnalysisOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "math.h"
#include "CMAnalysisOptionDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_dbLock\LockOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMAnalysisOptionDlg dialog

CCMAnalysisOptionDlg::CCMAnalysisOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMAnalysisOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMAnalysisOptionDlg)
	m_wndMemoryCbo = 0;
	m_wndMethodCbo = 0;
	//}}AFX_DATA_INIT
	m_arRadio.Add(IDC_CMD_ANALYOPT_SPARSE_CBO);
	m_arRadio.Add(IDC_CMD_ANALYOPT_SKYLINE_CBO);
	m_arRadio.Add(IDC_CMD_ANALYOPT_BAND_CBO);
	m_arRadio.Add(IDC_CMD_ANALYOPT_FRONTAL_CBO);
	//m_bUpdateTable = FALSE;
}


void CCMAnalysisOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMAnalysisOptionDlg)
	DDX_Control(pDX, IDC_CMD_PROCESSOR_NUM_EDT, m_wndProcessEditSpin);
	DDX_Control(pDX, IDC_CMD_MULTI_PROCESSOR_CHK, m_MultiProcessorChk);
	DDX_Control(pDX, IDC_CMD_GPU_ACCEL_CHK, m_GPUAccelChk);
	DDX_Radio(pDX, IDC_CMD_ANALYOPT_AUTO_CBO, m_wndMemoryCbo);
	DDX_Radio(pDX, IDC_CMD_ANALYOPT_SPARSE_CBO, m_wndMethodCbo);
	DDX_Control(pDX, IDC_CMD_ANALYOPT_USER_EDIT, m_wndMemoryEditSpin);
	DDX_Control(pDX, IDC_CMD_ANALYOPT_BAND_CBO , m_ctrlAnaltOpt);
	//DDX_Check(pDX, IDC_CMD_ANALYOPT_UPDATE_TABLE_CHK, m_bUpdateTable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMAnalysisOptionDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMAnalysisOptionDlg)
	ON_BN_CLICKED(IDC_CMD_ANALYOPT_AUTO_CBO, OnCmdAnalyoptMemoryMode)
	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_ANALYOPT_USER_EDIT, OnDeltaposCmdAnalyoptUserSpin)
	ON_BN_CLICKED(IDC_CMD_ANALYOPT_OK, OnCmdAnalyoptOk)
	ON_BN_CLICKED(IDC_CMD_ANALYOPT_CANCEL, OnCmdAnalyoptCancel)
	ON_EN_UPDATE(IDC_CMD_ANALYOPT_USER_EDIT, OnUpdateCmdAnalyoptUserEdit)
	ON_BN_CLICKED(IDC_CMD_MULTI_PROCESSOR_CHK, OnCmdMultiProcessorChk)
	ON_BN_CLICKED(IDC_CMD_ANALYOPT_USER_CBO, OnCmdAnalyoptMemoryMode)
	ON_BN_CLICKED(IDC_CMD_ANALYOPT_BAND_CBO, OnCmdAnalysisMethodRdo)
	ON_BN_CLICKED(IDC_CMD_ANALYOPT_FRONTAL_CBO, OnCmdAnalysisMethodRdo)
	ON_BN_CLICKED(IDC_CMD_ANALYOPT_SKYLINE_CBO, OnCmdAnalysisMethodRdo)
	ON_BN_CLICKED(IDC_CMD_ANALYOPT_SPARSE_CBO, OnCmdAnalysisMethodRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMAnalysisOptionDlg message handlers

void CCMAnalysisOptionDlg::OnCmdAnalyoptMemoryMode() 
{
	UpdateData(TRUE);

	m_wndMemoryEditSpin.EnableWindow(m_wndMemoryCbo == 1);
}

BOOL CCMAnalysisOptionDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	UINT uIntMethod, uIntMemory, uIntQuantity, uUseMulti, uNumMulti, uUseGPUAccel;
	double dbTempMemSize;
	MEMORYSTATUS MemoryStatus;
	CString strQuantity;

	GlobalMemoryStatus(&MemoryStatus);
	dbTempMemSize = MemoryStatus.dwTotalPhys / (1024*1024*1024);  //GB
	m_nMemorySize = (int)ceil(dbTempMemSize);

	/**
	uIntMethod = AfxGetApp()->GetProfileInt(_ULS(AnalysisOption),_ULS(Method), 3);
	uIntMemory = AfxGetApp()->GetProfileInt(_ULS(AnalysisOption),_ULS(Memory), 0);
	uIntQuantity = AfxGetApp()->GetProfileInt(_ULS(AnalysisOption),_ULS(Quantity),64);
	**/
	T_ANALOPTION AnalData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pInitCtrl->GetAnalysisOption(AnalData);
	uIntMethod = AnalData.nAnalMethod;
	uIntMemory = AnalData.nMemoryType;
	uIntQuantity = AnalData.nMemoryQuantityGB;
	uUseMulti = AnalData.nUseMultiProcessor;
	uNumMulti = AnalData.nNumMultiProcessor;
	uUseGPUAccel = AnalData.nUseGPUAccel;

	m_wndMethodCbo = (uIntMethod > 3 || uIntMethod < 0) ? 3 : uIntMethod;
	m_wndMemoryCbo = (uIntMemory > 1 || uIntMemory < 0) ? 0 : uIntMemory;

 	strQuantity.Format(_T("%d"), uIntQuantity);
	m_wndMemoryEditSpin.SetWindowText(strQuantity);
	m_wndMemoryEditSpin.EnableWindow(m_wndMemoryCbo == 1);
	if (!pDoc->AllowCommand(D_OPTN_ID_GPU))  m_GPUAccelChk.EnableWindow(FALSE);

	m_ctrlAnaltOpt.ShowWindow(SW_HIDE);

	UpdateData(FALSE);

	m_wndProcessEditSpin.SetRange(1, 100);

	if (m_wndMethodCbo == 3)
	{
		m_MultiProcessorChk.SetCheck(uUseMulti);
		m_wndProcessEditSpin.SetValue((int)uNumMulti);
		m_GPUAccelChk.SetCheck(uUseGPUAccel);
	}
	else
		m_wndProcessEditSpin.SetValue(1);

	//SWD-2243
	//m_bUpdateTable = pDoc->GetAutoExportUTBL();
	//GetDlgItem(IDC_CMD_ANALYOPT_UPDATE_TABLE_CHK)->SetWindowTextW(_LS(IDS_CMD_ANALYOPT_SAVE_USER_DEFINED_DATA));

	OnCmdAnalysisMethodRdo();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMAnalysisOptionDlg::OnDeltaposCmdAnalyoptUserSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// 동작 확인 부탁드립니다. 230406 syc1214 
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString strMemory;
	int nMemory=0;

	strMemory.Format(_T("%d"), m_nMemorySize);
	m_wndMemoryEditSpin.GetWindowText(strMemory);
	nMemory = _ttoi(strMemory);

	if(pNMUpDown->iDelta < 0)
	{
		nMemory = nMemory + 1; // (nMemory / 8 + 1) * 8;

		if(nMemory > m_nMemorySize )
			nMemory = m_nMemorySize;
	} 	
	else
	{
		nMemory = nMemory - 1; // (ceil(nMemory / 8. - 1)) * 8;

		if(nMemory < 1 )
			nMemory = 1;
	} 

	strMemory.Format(_T("%d"),nMemory);
	m_wndMemoryEditSpin.SetWindowText(strMemory);
	
	*pResult = 0;
}

void CCMAnalysisOptionDlg::OnCmdAnalyoptOk() 
{
	/**
	BOOL bCheckMethod, bCheckMemory, bCheckMemoryAmount;
	UINT uIntMethod, uIntMemory;
	CString strQuantity;

	UpdateData(TRUE);
	uIntMethod = m_wndMethodCbo;
	uIntMemory = m_wndMemoryCbo;

	bCheckMethod = AfxGetApp()->WriteProfileInt(_ULS(AnalysisOption), _ULS(Method), uIntMethod);
	bCheckMemory = AfxGetApp()->WriteProfileInt(_ULS(AnalysisOption), _ULS(Memory), uIntMemory);

	if(m_wndMemoryCbo == 1)
	{
		m_wndMemoryEdit.GetWindowText(strQuantity);
		if(_ttoi(strQuantity) < 4)
			strQuantity = _T("4");
		bCheckMemoryAmount = AfxGetApp()->WriteProfileInt(_ULS(AnalysisOption),_ULS(Quantity),_ttoi(strQuantity));
	} 
	**/

	UpdateData(TRUE);
	CString strQuantity;
	T_ANALOPTION data;
	data.Initialize();

	data.nAnalMethod = m_wndMethodCbo;
	data.nMemoryType = m_wndMemoryCbo;
	if (data.nMemoryType == 1)
	{
		m_wndMemoryEditSpin.GetWindowText(strQuantity);
		if(_ttoi(strQuantity) < 1)
			strQuantity = _T("1");
		data.nMemoryQuantityGB = _ttoi(strQuantity);
	}

	if (data.nAnalMethod == 3)
	{
		data.nUseMultiProcessor = m_MultiProcessorChk.GetCheck();
		if (data.nUseMultiProcessor == 1)
			data.nNumMultiProcessor = m_wndProcessEditSpin.GetEditValueInt();

		data.nUseGPUAccel = m_GPUAccelChk.GetCheck();
	}
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pInitCtrl->SetAnalysisOption(data);
	//pDoc->SetAutoExportUTBL(m_bUpdateTable);
	CDialogMove::OnOK();
}

void CCMAnalysisOptionDlg::OnCmdAnalyoptCancel() 
{
	CDialogMove::OnCancel();
}

void CCMAnalysisOptionDlg::OnUpdateCmdAnalyoptUserEdit() 
{
	CString strMemSize;
	
	m_wndMemoryEditSpin.GetWindowText(strMemSize);
	if(_ttoi(strMemSize) > m_nMemorySize)
	{
		strMemSize.Format(_T("%d"), m_nMemorySize);
		m_wndMemoryEditSpin.SetWindowText(strMemSize);
	}
}

void CCMAnalysisOptionDlg::OnCmdMultiProcessorChk() 
{
	BOOL bChk = m_MultiProcessorChk.GetCheck();

	m_wndProcessEditSpin.EnableWindow(bChk);
	//GetDlgItem(IDC_CMD_PROCESSOR_NUM_SPIN)->EnableWindow(bChk);
	CWnd* pWndTxt = GetDlgItem(IDC_CMD_STATIC);
	if (pWndTxt) pWndTxt->EnableWindow(bChk);
}

void CCMAnalysisOptionDlg::OnCmdAnalysisMethodRdo() 
{
	int nSel;
	CDlgUtil::CtrlRadioGetCheck(this, m_arRadio, nSel);

	m_MultiProcessorChk.EnableWindow(nSel == 3);
	if (nSel != 3)  m_MultiProcessorChk.SetCheck(FALSE);
	
	if (m_MultiProcessorChk.IsWindowEnabled())
		OnCmdMultiProcessorChk();
	else
	{
		m_wndProcessEditSpin.EnableWindow(FALSE);
		//GetDlgItem(IDC_CMD_PROCESSOR_NUM_SPIN)->EnableWindow(FALSE);
	}

	m_GPUAccelChk.EnableWindow(nSel == 3);
	if (nSel != 3)  m_GPUAccelChk.SetCheck(FALSE);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc->AllowCommand(D_OPTN_ID_GPU))  m_GPUAccelChk.EnableWindow(FALSE);
}
