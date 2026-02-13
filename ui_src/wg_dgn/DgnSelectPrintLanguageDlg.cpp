// DgnSelectPrintLanguageDlg.cpp: implementation of the CDgnSelectPrintLanguageDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSelectPrintLanguageDlg.h"

#include "../wg_db/wg_db_DBLib.h"

// CDgnSelectPrintLanguageDlg 대화 상자입니다.
CDgnSelectPrintLanguageDlg::CDgnSelectPrintLanguageDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSelectPrintLanguageDlg::IDD, pParent)
{

}

CDgnSelectPrintLanguageDlg::~CDgnSelectPrintLanguageDlg()
{
}

void CDgnSelectPrintLanguageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDgnPSCSelectPrintUnitDlg)
    DDX_Control(pDX, IDC_SELECT_LANG, m_CmbSelLang);
    //}}AFX_DATA_MAP
}


#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnSelectPrintLanguageDlg, CInternationalDlg)

#undef CDialog
    //{{AFX_MSG_MAP(CDgnPSCSelectPrintUnitDlg)
    //}}AFX_MSG_MAP
    
END_MESSAGE_MAP()


// CDgnSelectPrintLanguageDlg 메시지 처리기입니다.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CDgnSelectPrintLanguageDlg::OnInitDialog() 
{
    CDialogMove::OnInitDialog();

    m_CmbSelLang.AddString(_T("English")); // 0
    if(CDBLib::AllowCzech())
    {
        m_CmbSelLang.AddString(_T("Czech"));
    }
    if (CDBLib::AllowPolish())
    {
        m_CmbSelLang.AddString(_T("Polish"));
    }
    m_CmbSelLang.SetCurSel(0);

    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSelectPrintLanguageDlg::OnCancel() 
{
    // TODO: Add extra cleanup here
    CDialogMove::OnCancel();
}

void CDgnSelectPrintLanguageDlg::OnOK() 
{
    UpdateData(TRUE);
    
    m_iSelLang = 0;

    CString strLang;
    int iIndex = m_CmbSelLang.GetCurSel();
    if (iIndex!=-1)
    {
        m_CmbSelLang.GetLBText(iIndex, strLang);
        if (strLang == _T("English"))     m_iSelLang = 0;
        else if (strLang == _T("Czech"))  m_iSelLang = 1;
        else if (strLang == _T("Polish")) m_iSelLang = 2;
        else ASSERT(0);
    }
    else ASSERT(0);

    CDialogMove::OnOK();  
}
