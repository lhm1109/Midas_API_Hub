// DgnEngineProgressDlg.cpp: implementation of the CDgnEngineProgressDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnEngineProgressDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnEngineProgressDlg dialog


CDgnEngineProgressDlg::CDgnEngineProgressDlg(CWnd* pParent /*=NULL*/)
	: CDgnProgressDlg(pParent)
{
	m_strTitle_Pre   = _T("");
	m_strMessage_Pre = _T("");
	m_nPercent_Pre = 0;
}


BEGIN_MESSAGE_MAP(CDgnEngineProgressDlg, CDgnProgressDlg)
	//{{AFX_MSG_MAP(CDgnEngineProgressDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnEngineProgressDlg message handlers

BOOL CDgnEngineProgressDlg::OnInitDialog() 
{
	CDgnProgressDlg::OnInitDialog();

 	// TODO: Add extra initialization here
	m_ProgressInte.Initialize();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

D_XL_PROGRESS_INTERFACE* CDgnEngineProgressDlg::Get_InterfacePoint()
{	
	if(m_pDlg == NULL) return NULL;
	m_ProgressInte.pDlgWnd = (CWnd*)m_pDlg;
	return &m_ProgressInte;
}


void CDgnEngineProgressDlg::OnPaint() 
{
	if(m_pDlg == NULL) return;

	CString strTitle = _T("");
	CString strMessage = _T("");	
	int iPercent = -1;

	if(m_ProgressInte.bUpdateTitle)   strTitle   = m_ProgressInte.strTitle;
	if(m_ProgressInte.bUpdateMessage) strMessage = m_ProgressInte.strMessage;
	if(m_ProgressInte.bUpdatePercent) iPercent   = m_ProgressInte.iPercent;                           

	bool bRefresh = [&] ()
	{
		if ( m_strTitle_Pre != strTitle ) return true;
		if ( m_strMessage_Pre != strMessage ) return true;
		if ( m_nPercent_Pre != iPercent ) return true;
		return false;
	}();		

	if ( bRefresh )
	{
		m_strTitle_Pre   = strTitle;
		m_strMessage_Pre = strMessage;
		m_nPercent_Pre   = iPercent;
		Set_DlgData(strTitle, strMessage, iPercent);
	}

	// Stop여부를 넘겨줌
	m_ProgressInte.bStopExecute = m_bStopExecute;

	CDgnProgressDlg::OnPaint();
}
