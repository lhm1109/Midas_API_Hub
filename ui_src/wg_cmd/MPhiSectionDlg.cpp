// MPhiSectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "MPhiSectionDlg.h"
#include "MPhiParameterDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMPhiSectionDlg dialog

CMPhiSectionDlg::CMPhiSectionDlg(MPhiDataAll* pMPhiDataAll, CWnd* pParent /*=NULL*/)
	: CDlgChild(CMPhiSectionDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pParent = (CMPhiParameterDlg *) pParent;
	m_pMPhiDataAll = pMPhiDataAll;  
	//{{AFX_DATA_INIT(CMPhiSectionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// set flag childHasBtn Flag....
	SetChildHasBtnFlag(TRUE);
	m_pGrid = NULL;

	for (int i = 0; i < 6; i++)
	{
		m_bCodeExist[i] = FALSE;	
	}
}

CMPhiSectionDlg::~CMPhiSectionDlg()
{
	if(m_pGrid)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}
}

void CMPhiSectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMPhiSectionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_CMD_SECTION_GRID, *m_pGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMPhiSectionDlg, CDlgChild)
	//{{AFX_MSG_MAP(CMPhiSectionDlg)
	ON_BN_CLICKED(IDC_BEAM_BTN, OnBeamBtn)
	ON_BN_CLICKED(IDC_COLUMN_BTN, OnColumnBtn)
	ON_BN_CLICKED(IDC_PSC_BTN, OnPscBtn)
	//ON_BN_CLICKED(IDC_CMD_APPLY_BTN, OnCmdApplyBtn)
	ON_BN_CLICKED(IDC_RESET_BTN, OnResetBtn)
	
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPhiSectionDlg message handlers
BOOL CMPhiSectionDlg::OnInitDialog() 
{
	if (m_pGrid)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}

	m_pGrid = new CMPhiSectGrid();

	CDlgChild::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	InitData();
	InitGrid();

	// Beam, column section data for checking dialog is Modaless....
	m_aReinforceCtrl.Add(IDC_COLUMN_STATIC);
	m_aReinforceCtrl.Add(IDC_BEAM_STATIC);
	m_aReinforceCtrl.Add(IDC_BEAM_BTN);
	m_aReinforceCtrl.Add(IDC_COLUMN_BTN);

	//CDlgUtil::CtrlShowHide(this, m_aReinforceCtrl, FALSE);
	//CDlgUtil::CtrlEnableDisable(this,IDC_BEAM_BTN,FALSE);
	//CDlgUtil::CtrlEnableDisable(this,IDC_COLUMN_BTN,FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMPhiSectionDlg::InitData()
{	
	ASSERT(m_pMPhiDataAll->aMpstD.GetSize() == m_pMPhiDataAll->aMpstK.GetSize());
	m_aData.RemoveAll();

	CArray<T_SECT_K, T_SECT_K> aSectK;	aSectK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetSectKeyList(aSectK);
	for(int i=0; i < aSectK.GetSize() ; i++)
	{
		int nFindIdx = -1;
		for (int j = 0; j < m_pMPhiDataAll->aMpstK.GetSize(); j++)
		{
			if(aSectK[i] == m_pMPhiDataAll->aMpstK[j])
			{
				nFindIdx = j;
				break;
			}
		}   

		T_SECT_DATA tempD;
		GetDefaultSectData(aSectK[i], tempD);
		if(nFindIdx == -1)
		{        
			if(tempD.bCandidate)	m_aData.Add(tempD);
		}
		else
		{
			tempD.MpstD = m_pMPhiDataAll->aMpstD[nFindIdx]; // User가 입력했던 Data를 세팅해줌..(tab을 딴곳에 갔다가 온경우에 해당..)
			if(tempD.bCandidate)
			{
				m_aData.Add(tempD);
			}
			else
			{
				ASSERT(0); // 이상황은 안생기겠지.. 그래도 혹시 모르니.. 처리함
			}        
		}      
	}
	
	for (int i = 0; i < 6; i++)
	{
		m_bCodeExist[i] = FALSE;	
	}
	int nSize;
	nSize = m_pMPhiDataAll->aMphgD.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		if(m_pMPhiDataAll->aMphgD[i].nCode < 0 || m_pMPhiDataAll->aMphgD[i].nCode >= 6)
		{
			ASSERT(0);
			continue;
		}

		m_bCodeExist[m_pMPhiDataAll->aMphgD[i].nCode] = TRUE;
	}

	return TRUE;
}

void CMPhiSectionDlg::GetDefaultSectData(T_SECT_K SectK, T_SECT_DATA &tempD)
{
	T_SECT_D rSectD;	rSectD.Initialize();
	T_MPST_D rMpstD;	rMpstD.Initialize();
	m_pDoc->m_pAttrCtrl->GetSect(SectK, rSectD);
	tempD.bCandidate = m_pDoc->m_pEditData->GetMpstDefaultData(SectK, m_bCodeExist, rMpstD, tempD.bRebar, tempD.bRC, tempD.bStl, 
		tempD.bSRC, tempD.bUseMiddle, tempD.dRFY, tempD.dRFZ, tempD.dRRY, tempD.dRRZ, tempD.bUse, tempD.bSatisfy);
	tempD.SectK = SectK;
	tempD.strName = rSectD.SName;
	tempD.strShape = rSectD.SectBefore.Shape;		
	tempD.MpstD = rMpstD;
	tempD.nStype = rSectD.nStype;

	BOOL bCol, bBeam, bBrace;
	m_pDoc->m_pEditData->GetMemberType4Mpst(SectK, bCol, bBeam, bBrace);
	if(bCol)
	{
		tempD.nMembType = 1;
	}
	else if(bBeam)
	{
		tempD.nMembType = 2;
	}
	else if(bBrace)
	{
		tempD.nMembType = 3;
	}
	else
	{
		tempD.nMembType = 0;
	}
}

BOOL CMPhiSectionDlg::InitGrid()
{		
	m_pGrid->Initialize(m_bCodeExist);	
	m_pGrid->SetData(m_aData);  

	return TRUE;
}

void CMPhiSectionDlg::OnBeamBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DGN_CON_REBB, 0));		
}

void CMPhiSectionDlg::OnColumnBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DGN_CON_REBC, 0));			
}

void CMPhiSectionDlg::OnPscBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_PROP_SECTION_RPSC, 0));			
}

void CMPhiSectionDlg::OnResetBtn() 
{
	m_aData.RemoveAll();		
	CArray<T_SECT_K, T_SECT_K> aSectK;	aSectK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetSectKeyList(aSectK);
	for(int i=0; i < aSectK.GetSize() ; i++)
	{
		T_SECT_DATA tempD;
		GetDefaultSectData(aSectK[i], tempD);
		if(tempD.bCandidate)	m_aData.Add(tempD);
	}
	m_pGrid->SetData(m_aData);
}

void CMPhiSectionDlg::OnOK()
{
	return;
}

void CMPhiSectionDlg::OnCancel() 
{
	return;
}

void CMPhiSectionDlg::SetDlgDataToParent() 
{
	// 결국 MPhiParameterDlg::m_pMPhiDataAll 의 Data가 변경됨
	m_pMPhiDataAll->aMpstK.RemoveAll();
	m_pMPhiDataAll->aMpstD.RemoveAll();

	for(int i=0; i<m_aData.GetSize(); i++)
	{	
		m_pMPhiDataAll->aMpstK.Add(m_aData[i].SectK);
		m_pMPhiDataAll->aMpstD.Add(m_aData[i].MpstD);
	}
}

void CMPhiSectionDlg::Dlg2Data() 
{
	m_pGrid->GetData(m_aData);

	SetDlgDataToParent();
}


void CMPhiSectionDlg::MyUpdate(LPARAM lHint, CObject* pHint)
{
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START:
		//DestroyWindow(); return;
	case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}


void CMPhiSectionDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SECT_K Key;
	T_SECT_D Data, DataBak;
	T_RPSC_D tempD;
	T_RCHK_D rchkD;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_RPSC_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.				
				pViewBuff->GetRpsc(nKey, Key, tempD);
				m_pDoc->m_pAttrCtrl->GetSect(Key, Data);
				ModifyItem(Key, Data);
				break;
		case(UR_RPSC_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetRpsc(nKey, Key, tempD);
				m_pDoc->m_pAttrCtrl->GetSect(Key, Data);
				ModifyItem(Key, Data);
				break;

		case(UR_RCHK_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.				
				pViewBuff->GetRchk(nKey, Key, rchkD);
				m_pDoc->m_pAttrCtrl->GetSect(Key, Data);
				ModifyItem(Key, Data);
				break;
		case(UR_RCHK_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetRchk(nKey, Key, rchkD);
				m_pDoc->m_pAttrCtrl->GetSect(Key, Data);
				ModifyItem(Key, Data);
				break;
		default:
			break;
		}
	} // end of while
}

BOOL CMPhiSectionDlg::ModifyItem(T_SECT_K Key, T_SECT_D &mData)
{	
	T_SECT_DATA tempD;
	GetDefaultSectData(Key, tempD);
 
	BOOL bChange = FALSE;
	m_pGrid->GetData(m_aData);
	for (int i = 0; i < m_aData.GetSize(); i++)
	{
		if(Key == m_aData[i].SectK)
		{
			bChange = TRUE;
			if(tempD.bCandidate)
			{
				m_aData[i] = tempD;
			}
			else
			{
				m_aData.RemoveAt(i);
			}      
			break;
		}
	}
	if(bChange == FALSE && tempD.bCandidate)
	{
		m_aData.Add(tempD);
	}

	if(bChange)
	{    
	  m_pGrid->SetData(m_aData);
	}
	
	return TRUE;
}

void CMPhiSectionDlg::OnDestroy() 
{
	Dlg2Data();

	if (m_pGrid)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}

	CDlgChild::OnDestroy();
}
