// CMSplcSubDlg.cpp : implementation file
//
// 2003. 08. 28   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSplcSubDlg.h"

#include "CMSpecGrid.h"
#include "CMSplcSubCalcEccnDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include "stdio.h"
//#include "time.h"

/////////////////////////////////////////////////////////////////////////////
// CCMSplcSubDlg dialog


CCMSplcSubDlg::CCMSplcSubDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSplcSubDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSplcSubDlg)
	m_nMothod = 0;
	m_nAuto = 0;
	//}}AFX_DATA_INIT

	m_Data.Initialize();

	m_pDoc = NULL;
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);

	m_wndGrid = NULL;
	m_wndGrid = new CCMSpecGrid();
}

CCMSplcSubDlg::~CCMSplcSubDlg()
{
	if (m_wndGrid)
	{
		delete m_wndGrid;
		m_wndGrid = NULL;
	}
}

void CCMSplcSubDlg::SetEccenData(T_SPLC_D* pData)
{
	ASSERT(pData);
	m_Data = *pData;
}

BOOL CCMSplcSubDlg::GetEccenData(T_SPLC_D& rData)
{
	rData.nEccenMethod = m_Data.nEccenMethod;
	rData.nAutomatic = m_Data.nAutomatic;
	rData.dEccenPercent = m_Data.dEccenPercent;
	rData.aSplcEccen.Copy(m_Data.aSplcEccen);
	rData.aSplcEccen_AL.Copy(m_Data.aSplcEccen_AL);
	rData.bUseGL = m_Data.bUseGL;
	rData.bConsEccMnt = m_Data.bConsEccMnt;
	return TRUE;
}

void CCMSplcSubDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSplcSubDlg)
	DDX_Control(pDX, IDC_CMD_SPLC_DATA_EDIT,  m_editRate);
	DDX_Control(pDX, IDC_CMD_SPLC_USE_GL_CHK, m_chkUseGL);
	DDX_Control(pDX, IDC_CMD_SPLC_CND_MNT_CHK, m_chkCnsMnt);
	DDX_Radio(pDX, IDC_CMD_SPLC_METHOD_RD1, m_nMothod);
	DDX_Radio(pDX, IDC_CMD_SPLC_DATA_RD1, m_nAuto);
	DDX_Control(pDX, IDC_CMD_SPLC_GRID, *m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSplcSubDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSplcSubDlg)
	ON_BN_CLICKED(IDC_CMD_SPLC_DATA_RD1, OnCmdSplcDataRd)
	ON_BN_CLICKED(IDC_CMD_SPLC_DATA_RD2, OnCmdSplcDataRd)
	ON_BN_CLICKED(IDC_CMD_SPLC_USE_GL_CHK, OnCmdSplcDataRd)
	ON_BN_CLICKED(IDC_CMD_SPLC_ECC_CALC_BTN, OnCmdSplcCalcEccentricity)
	ON_EN_CHANGE(IDC_CMD_SPLC_DATA_EDIT, OnChangeCmdSplcDataEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSplcSubDlg message handlers

BOOL CCMSplcSubDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndGrid->Initialize();

#if defined(_MGEN_JP) || defined(_IGEN)
	// 2014-06-18. by Hsshim
	// Story Shear 계산시 절점 정보로 계산하면 830 부터 해석 결과가 달라지므로, 솔버에서 Level별로 계산하도록 수정
	// iGen의 경우 층 다이어프램이 해제된 경우에도 우발편심 모멘트가 나오는 요구(중국,영어권)사항을 반영하지 않기로 협의함. (이학현,이해주,홍성일,김종민,심후성)
	m_chkCnsMnt.EnableWindow(FALSE);
#endif

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSplcSubDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;

	CDialogMove::OnOK();
}

void CCMSplcSubDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CCMSplcSubDlg::OnCmdSplcDataRd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CheckEccnCalcBtnEnable();

	m_Data.nAutomatic = m_nAuto;
	BOOL bEnable = (m_nAuto == 0) ? FALSE : TRUE;
	m_editRate.EnableWindow(!bEnable);
	if(m_chkUseGL.GetCheck()) m_Data.bUseGL = TRUE;
	else m_Data.bUseGL = FALSE;
	if(m_chkCnsMnt.GetCheck()) m_Data.bConsEccMnt = TRUE;
	else m_Data.bConsEccMnt = FALSE;

	if (m_Data.bAutoSearchAngle) // Auto-Search-Angle 인 경우
	{
		if(m_pDoc->m_pPostCtrl->IsPostEnable())
		{
			if(m_Data.aSplcEccen.GetSize() > 0) m_wndGrid->Data2Grid(m_Data.aSplcEccen, m_Data.aSplcEccen_AL);
			else m_pDoc->m_pAttrCtrl->GetSplcEccenData(m_Data);
		}
		else m_pDoc->m_pAttrCtrl->GetSplcEccenData(m_Data); // 해석 전

		m_wndGrid->EnableGrid(FALSE, m_Data.bUseGL);
	}
	else
	{
		m_pDoc->m_pAttrCtrl->GetSplcEccenData(m_Data);
		m_wndGrid->Data2Grid(m_Data.aSplcEccen, m_Data.aSplcEccen_AL);
		m_wndGrid->EnableGrid(bEnable, m_Data.bUseGL); 
	}
}

void CCMSplcSubDlg::OnCmdSplcCalcEccentricity()
{
	UpdateData(TRUE);

	CheckEccnCalcBtnEnable();

	BOOL bEnable = (m_nAuto == 0) ? FALSE : TRUE;
	m_editRate.EnableWindow(!bEnable);
	if (m_chkUseGL.GetCheck()) m_Data.bUseGL = TRUE;
	else m_Data.bUseGL = FALSE;
	if (m_chkCnsMnt.GetCheck()) m_Data.bConsEccMnt = TRUE;
	else m_Data.bConsEccMnt = FALSE;

	CCMSplcSubCalcEccnDlg dlg;
	dlg.SetDlgType(EN_RS);
	if (dlg.DoModal() == IDOK)
	{
		double dPercent4PlanDimEccn = 0.0;
		double dMultiple4TopStoryEccn = 0.0;
		bool bConsiderEccnbyAnal = false;
		double dRatio4AnalEccn = 0.0;

		dlg.GetInputData(dPercent4PlanDimEccn, dMultiple4TopStoryEccn, bConsiderEccnbyAnal, dRatio4AnalEccn);

		if (m_Data.bAutoSearchAngle)
		{
			if (m_pDoc->m_pPostCtrl->IsPostEnable())
			{
				if (m_Data.aSplcEccen.GetSize() > 0) m_wndGrid->Data2Grid(m_Data.aSplcEccen, m_Data.aSplcEccen_AL);
				else m_pDoc->m_pAttrCtrl->CalcSplcEccn4UserMethod(m_Data,
					dPercent4PlanDimEccn, dMultiple4TopStoryEccn, bConsiderEccnbyAnal, dRatio4AnalEccn);
			}
			else m_pDoc->m_pAttrCtrl->CalcSplcEccn4UserMethod(m_Data,
				dPercent4PlanDimEccn, dMultiple4TopStoryEccn, bConsiderEccnbyAnal, dRatio4AnalEccn);
		}
		else
		{
			m_pDoc->m_pAttrCtrl->CalcSplcEccn4UserMethod(m_Data,
				dPercent4PlanDimEccn, dMultiple4TopStoryEccn, bConsiderEccnbyAnal, dRatio4AnalEccn);
			m_wndGrid->Data2Grid(m_Data.aSplcEccen, m_Data.aSplcEccen_AL);
			m_wndGrid->EnableGrid(bEnable, m_Data.bUseGL);
		}
	}

	return;
}

void CCMSplcSubDlg::OnChangeCmdSplcDataEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogMove::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString csRate;
	m_editRate.GetWindowText(csRate);
	m_Data.dEccenPercent = _tstof(csRate);

	if (m_Data.bAutoSearchAngle) // Auto-Search-Angle 인 경우
	{
		if(m_pDoc->m_pPostCtrl->IsPostEnable())
		{
			if(m_Data.aSplcEccen.GetSize() > 0) m_wndGrid->Data2Grid(m_Data.aSplcEccen, m_Data.aSplcEccen_AL);
			else m_pDoc->m_pAttrCtrl->GetSplcEccenData(m_Data);
		}
		else m_pDoc->m_pAttrCtrl->GetSplcEccenData(m_Data); // 해석 전

		m_wndGrid->EnableGrid(FALSE, m_Data.bUseGL);
	}
	else
	{
		m_pDoc->m_pAttrCtrl->GetSplcEccenData(m_Data);
		m_wndGrid->Data2Grid(m_Data.aSplcEccen, m_Data.aSplcEccen_AL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMSplcSubDlg::Data2Dlg()
{  
	if (m_Data.bAutoSearchAngle)
	{
		CString csRate;
		csRate.Format(_T("%g"), m_Data.dEccenPercent);
		m_editRate.SetWindowText(csRate);
		m_nMothod = m_Data.nEccenMethod;
		m_Data.nAutomatic = 0; // 무조건 Auto
		m_nAuto = m_Data.nAutomatic;
		if(m_Data.bUseGL) m_chkUseGL.SetCheck(BST_CHECKED);
		else m_chkUseGL.SetCheck(BST_UNCHECKED);
		GetDlgItem(IDC_CMD_SPLC_DATA_RD2)->EnableWindow(FALSE);
		m_editRate.EnableWindow(TRUE);

		if(m_Data.bConsEccMnt) m_chkCnsMnt.SetCheck(BST_CHECKED);
		else m_chkCnsMnt.SetCheck(BST_UNCHECKED);

		if(m_pDoc->m_pPostCtrl->IsPostEnable())
		{
			if(m_Data.aSplcEccen.GetSize() > 0) m_wndGrid->Data2Grid(m_Data.aSplcEccen, m_Data.aSplcEccen_AL);
			else m_pDoc->m_pAttrCtrl->GetSplcEccenData(m_Data);
		}
		else m_pDoc->m_pAttrCtrl->GetSplcEccenData(m_Data); // 해석 전

		m_wndGrid->EnableGrid(FALSE, m_Data.bUseGL);
	}
	else // 기존
	{
		CString csRate;
		csRate.Format(_T("%g"), m_Data.dEccenPercent);
		m_editRate.SetWindowText(csRate);
		m_nMothod = m_Data.nEccenMethod;
		m_nAuto = m_Data.nAutomatic;
		if(m_Data.bUseGL) m_chkUseGL.SetCheck(BST_CHECKED);
		else m_chkUseGL.SetCheck(BST_UNCHECKED);
		m_editRate.EnableWindow(m_Data.nAutomatic == 0);
		if(m_Data.bConsEccMnt) m_chkCnsMnt.SetCheck(BST_CHECKED);
		else m_chkCnsMnt.SetCheck(BST_UNCHECKED);

		/*
		clock_t timeStart = clock();
		for (int j=0 ; j<10 ; ++j)
		{
			for (int i=0 ; i<180 ; i=i+10)
			{
				m_Data.dblAngle = i;
				*/
				m_pDoc->m_pAttrCtrl->GetSplcEccenData(m_Data);
				/*
			}
		}
		int miliSec = (double)(clock() - timeStart) / CLOCKS_PER_SEC * 10;
		CString csTmp;
		csTmp.Format(_T("%d"), miliSec);
		AfxMessageBox(csTmp);
		*/

		m_wndGrid->Data2Grid(m_Data.aSplcEccen, m_Data.aSplcEccen_AL);
		m_wndGrid->EnableGrid(m_Data.nAutomatic != 0, m_Data.bUseGL);
	}

	CheckEccnCalcBtnEnable();

	UpdateData(FALSE);
}


BOOL CCMSplcSubDlg::Dlg2Data()
{
	UpdateData(TRUE);

	CString csRate;
	m_editRate.GetWindowText(csRate);
	m_Data.dEccenPercent = _tstof(csRate);
	m_Data.nEccenMethod = m_nMothod;
	m_Data.nAutomatic = m_nAuto;  
	m_Data.bUseGL = m_chkUseGL.GetCheck();
	if (m_Data.nAutomatic == 1)
	{
		if (!m_wndGrid->Grid2Data(m_Data.aSplcEccen, m_Data.aSplcEccen_AL))
			return FALSE;
	}
	m_Data.bConsEccMnt = m_chkCnsMnt.GetCheck();
	return TRUE;
}

void CCMSplcSubDlg::CheckEccnCalcBtnEnable()
{
	if (m_nAuto == 0)
	{
		GetDlgItem(IDC_CMD_SPLC_ECC_CALC_BTN)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CMD_SPLC_ECC_CALC_BTN)->EnableWindow(TRUE);
	}
}
