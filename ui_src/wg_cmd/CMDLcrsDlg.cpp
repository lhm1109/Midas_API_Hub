// CMDLcrsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMDLcrsDlg.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_EditData.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMDLcrsDlg dialog


CCMDLcrsDlg::CCMDLcrsDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMDLcrsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMDLcrsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CCMDLcrsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMDLcrsDlg)
	DDX_Control(pDX, IDC_LCRS_CHKLIST, m_cklistLCRS);
	DDX_Control(pDX, IDC_REMOVE, m_btnRemove);
	DDX_Control(pDX, IDOK, m_btnIdOk);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMDLcrsDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMDLcrsDlg)
		// NOTE: the ClassWizard will add message map macros here
ON_BN_CLICKED(IDOK, OnOk)
ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMDLcrsDlg message handlers
BOOL CCMDLcrsDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	Data2Dlg();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMDLcrsDlg::GetTotalLoadCase(T_SELECTEDLOAD_LIST &aLoadList)
{
	CSelectLC::GetSelectedLoadName(D_SELECTLOAD_STATIC |
																 D_SELECTLOAD_MOVING | 
																 D_SELECTLOAD_GRIDMODEL,aLoadList);

	T_SGLD_K sgldKey;
	T_SGLD_D sgldData;
	sgldKey = m_pDoc->m_pAttrCtrl->GetSgldKey(CString(_LS(IDS_DB_SGLD_DL)));
	m_pDoc->m_pAttrCtrl->GetSgld(sgldKey, sgldData);

	if(!m_pDoc->m_pAttrCtrl->ExistSgld(sgldKey))
		return;
	
	aLoadList.m_aLoadName.Add(_T("CS:") + sgldData.LoadCaseName);

	T_SELECTEDLOAD load;
	load.nLoadKey = sgldKey;
	load.nLoadType = D_LOADCASE_STAGE;
	aLoadList.m_aLoad.Add(load);
}

void CCMDLcrsDlg::MakeLoadList()
{
	GetTotalLoadCase(m_aLoadList);
	
	m_cklistLCRS.ResetContent();
	m_arLType.RemoveAll();
	

	int nIdx = 0; int ni = 0;
	for(int i=0; i<m_aLoadList.m_aLoad.GetSize(); i++)
	{ 
		ni = m_cklistLCRS.AddString(m_aLoadList.m_aLoadName[i]);
		m_cklistLCRS.SetItemData(nIdx,m_aLoadList.m_aLoad[i].nLoadKey);
		

		if( m_pDoc->m_pAttrCtrl->ExistLcrs() )
		{
			for(int j =0; j<m_LoadAttr.aCase.GetSize(); j++)
			{

				int nAnal = 0;
				CDBLib::GetLoadCombTypeByLoadCaseType(m_aLoadList.m_aLoad[i].nLoadType, nAnal);

				if(  m_LoadAttr.aCase.GetAt(j).LoadCaseKey == m_aLoadList.m_aLoad[i].nLoadKey 
						&&  m_LoadAttr.aCase.GetAt(j).AnalType == nAnal )
				{
					m_cklistLCRS.SetCheck(i,1);//m_aLoadList.m_aLoad[i].nLoadKey);
				}
			}
		}
		m_arLType.Add(m_aLoadList.m_aLoad[i].nLoadType);
		nIdx ++ ;
	}
}

void CCMDLcrsDlg::OnOk() 
{
	if( m_pDoc->m_pAttrCtrl->ExistLcrs() ) return;
 
	int i = 0;
	for( i=0; i< m_aLoadList.m_aLoad.GetSize() ; i++ )
	{
		if( m_cklistLCRS.GetCheck(i) == TRUE ) break;
	}
	if( i == m_aLoadList.m_aLoad.GetSize() ) return;

	if(!Dlg2Data()) return;
	
	CDialogMove::OnOK();
}

void CCMDLcrsDlg::OnCancel() 
{	
	CDialogMove::OnCancel();
}

void CCMDLcrsDlg::OnRemove()  
{
	if( ! m_pDoc->m_pDataCtrl->DelLcrs() )
		return;
	m_btnRemove.EnableWindow(FALSE);
	CDialogMove::OnOK();
}

BOOL CCMDLcrsDlg::Dlg2Data()
{
	UpdateData(TRUE);

	T_LCRS_BASE tempData;
	tempData.Initialize();

	for(int i=0; i<m_aLoadList.m_aLoad.GetSize(); i++)
	{
		if( m_cklistLCRS.GetCheck(i) == 1 ) 
		{
			tempData.LoadCaseKey = m_aLoadList.m_aLoad[i].nLoadKey;

			int nAnal = 0;
			CDBLib::GetLoadCombTypeByLoadCaseType(m_aLoadList.m_aLoad[i].nLoadType, nAnal);
			tempData.AnalType = nAnal;

			m_LoadAttr.aCase.Add(tempData);
		}
	}

	/*
	if ( m_pDoc->m_pAttrCtrl->ExistLcrs() ) return FALSE;
	else if ( ! m_pDoc->m_pDataCtrl->AddLcrs(m_LoadAttr) ) return FALSE;
	*/
	if (!m_pDoc->m_pDataCtrl->AddLcrs(m_LoadAttr)) return FALSE;

	return TRUE;
}
void CCMDLcrsDlg::Data2Dlg()
{
	m_LoadAttr.Initialize();
	m_pDoc->m_pAttrCtrl->GetLcrs(m_LoadAttr);
	
	BOOL bExist = m_pDoc->m_pAttrCtrl->ExistLcrs();
	m_btnRemove.EnableWindow( bExist );
	//m_btnIdOk.EnableWindow( !bExist );

	MakeLoadList();

	UpdateData(FALSE);
}
