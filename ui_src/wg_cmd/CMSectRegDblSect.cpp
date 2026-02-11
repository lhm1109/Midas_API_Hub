// CMSectRegDblSect.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectRegDblSect.h"
//#include "CMSecPageRegPage.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SectDB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CDialogMove

/////////////////////////////////////////////////////////////////////////////
// CCMSectRegDblSect dialog


CCMSectRegDblSect::CCMSectRegDblSect(CString csShape, int x, int y, CWnd* pParent /*=NULL*/)
	: CDialog(CCMSectRegDblSect::IDD, pParent)
{
	ASSERT(csShape == D_SECT_SHAPE_REG_2L || csShape == D_SECT_SHAPE_REG_2C);
	ASSERT(pParent);
	if (csShape == D_SECT_SHAPE_REG_2L) m_csShape = D_SECT_SHAPE_REG_L;
	else m_csShape = D_SECT_SHAPE_REG_C;

	m_x = x; m_y = y;
	m_nCurDB = -1;
	m_pParent = pParent;

	//{{AFX_DATA_INIT(CCMSectRegDblSect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMSectRegDblSect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectRegDblSect)
	DDX_Control(pDX, IDC_CMD_SP_SECT_NAME, m_cboName);
	DDX_Control(pDX, IDC_CMD_SP_DB_NAME, m_cboDB);
	//}}AFX_DATA_MAP
}

void CCMSectRegDblSect::SetSectNameList()
{
	m_cboName.ResetContent();

	CArray<CString, CString&> SectNameList;
	CString strDBName;
	m_cboDB.GetWindowText(strDBName);
	if (strDBName.IsEmpty()) return;

	// get name list
	m_pDoc->m_pSectDB->GetSectNameList(strDBName, m_csShape, SectNameList);

	int nSize = SectNameList.GetSize();
	for (int i = 0; i < nSize; i++)
		m_cboName.AddString(SectNameList.GetAt(i));
	OnChangeCurSect();  // Trigger name change!
}

void CCMSectRegDblSect::MoveDialog()
{
	// move position
	CRect rect;
	GetWindowRect(rect);
	
	MoveWindow(m_x, m_y-rect.Height(), rect.Width(), rect.Height());
}

BEGIN_MESSAGE_MAP(CCMSectRegDblSect, CDialog)
	//{{AFX_MSG_MAP(CCMSectRegDblSect)
	ON_CBN_SELCHANGE(IDC_CMD_SP_DB_NAME, OnChangeCurDB)
	ON_CBN_SELCHANGE(IDC_CMD_SP_SECT_NAME, OnChangeCurSect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectRegDblSect message handlers

BOOL CCMSectRegDblSect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pDoc = CDBDoc::GetDocPoint();
	// TODO: Add extra initialization here
	// set DB name list
	CArray<CString, CString&> DBNameList;
	m_pDoc->m_pSectDB->GetDBNameList(DBNameList);
	for (int i = 0; i < DBNameList.GetSize(); i++)
		m_cboDB.AddString(DBNameList.GetAt(i));
	m_cboDB.SetCurSel(0);

	SetSectNameList();

	MoveDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectRegDblSect::OnChangeCurDB() 
{
	// TODO: Add your control notification handler code here
	if (m_nCurDB == m_cboDB.GetCurSel()) return;
	m_nCurDB = m_cboDB.GetCurSel();
	SetSectNameList();
}

void CCMSectRegDblSect::OnChangeCurSect() 
{
	// TODO: Add your control notification handler code here
	CString csDB, csName;
	m_cboDB.GetWindowText(csDB);
	m_cboName.GetWindowText(csName);
	if (csDB.IsEmpty() || csName.IsEmpty()) return;
	
	// read data
	T_SECT_SECTBASE_D SectData;
	if (!m_pDoc->m_pSectDB->GetSectData(csDB, csName, SectData)) return;

	//CCMSecPageRegPage* pParent;
	//pParent = (CCMSecPageRegPage*) m_pParent;
	//if (pParent->GetSafeHwnd() == 0) return;
	//pParent->UpdateDimFromChildDlg(SectData);
}
