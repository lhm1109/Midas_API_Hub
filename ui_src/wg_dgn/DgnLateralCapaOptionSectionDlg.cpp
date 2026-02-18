// DgnLateralCapaOptionSectionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnLateralCapaOptionSectionDlg.h"
#include "DgnLateralCapaOptionDlg.h"

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

#include "oncSectionRebarViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnLateralCapaOptionSectionDlg dialog

CDgnLateralCapaOptionSectionDlg::CDgnLateralCapaOptionSectionDlg(LateralCapaDataAll* pDataAll, CWnd* pParent /*=NULL*/)
	: CDlgChild(CDgnLateralCapaOptionSectionDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pParent = (CDgnLateralCapaOptionDlg *) pParent;
	m_pDataAll = pDataAll;
	//{{AFX_DATA_INIT(CDgnLateralCapaOptionSectionDlg)
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

CDgnLateralCapaOptionSectionDlg::~CDgnLateralCapaOptionSectionDlg()
{
	if(m_pGrid)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}
}

void CDgnLateralCapaOptionSectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnLateralCapaOptionSectionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//DDX_Control(pDX, IDC_DGN_SECTION_GRID, *m_pGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnLateralCapaOptionSectionDlg, CDlgChild)
	//{{AFX_MSG_MAP(CDgnLateralCapaOptionSectionDlg)
	ON_BN_CLICKED(IDC_COLUMN_BTN, OnColumnBtn)
	ON_BN_CLICKED(IDC_COLUMN_GEN_BTN, OnColumnGenBtn)	
	ON_BN_CLICKED(IDC_RESET_BTN, OnResetBtn)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnLateralCapaOptionSectionDlg message handlers
BOOL CDgnLateralCapaOptionSectionDlg::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_pGrid)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}
	m_pGrid = new CDgnLateralCapaOptionSectGrid();
	InitData();
	InitGrid();

	// Beam, column section data for checking dialog is Modaless....
	m_aReinforceCtrl.Add(IDC_COLUMN_STATIC);
	m_aReinforceCtrl.Add(IDC_COLUMN_BTN);

	//CDlgUtil::CtrlShowHide(this, m_aReinforceCtrl, FALSE);
	//CDlgUtil::CtrlEnableDisable(this,IDC_BEAM_BTN,FALSE);
	//CDlgUtil::CtrlEnableDisable(this,IDC_COLUMN_BTN,FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDgnLateralCapaOptionSectionDlg::InitData()
{
	ASSERT(m_pDataAll->aMpstD.GetSize() == m_pDataAll->aMpstK.GetSize());
	m_aData.RemoveAll();

	CArray<T_SECT_K, T_SECT_K> aSectK;	aSectK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetSectKeyList(aSectK);
	for(int i=0; i < aSectK.GetSize() ; i++)
	{
		int nFindIdx = -1;
		for (int j = 0; j < m_pDataAll->aMpstK.GetSize(); j++)
		{
			if(aSectK[i] == m_pDataAll->aMpstK[j])
			{
				nFindIdx = j;
				break;
			}
		}   

		T_DGN_SECT_DATA tempD;
		GetDefaultSectData(aSectK[i], tempD);
		if(nFindIdx == -1)
		{        
			if(tempD.bCandidate)	m_aData.Add(tempD);
		}
		else
		{
			tempD.MpstD = m_pDataAll->aMpstD[nFindIdx]; // User가 입력했던 Data를 세팅해줌..(tab을 딴곳에 갔다가 온경우에 해당..)
			if(tempD.bCandidate)
			{
				m_aData.Add(tempD);
			}
			else
			{
				// 보유수평내력쪽에서는 쓰는단면이 아닐 경우엔 tempD.bCandidate을 false로 처리하는 코드가 있어서 여기로 들어올수도 있음.. 이 경우는 Data를 무시함..
			}        
		}      
	}

	for (int i = 0; i < 6; i++)
	{
		m_bCodeExist[i] = FALSE;	
	}
	int nSize;
	nSize = m_pDataAll->aMphgD.GetSize();
	for (int i = 0; i < nSize ; i++)
	{
		if(m_pDataAll->aMphgD[i].nCode < 0 || m_pDataAll->aMphgD[i].nCode >= 6)
		{
			ASSERT(0);
			continue;
		}

		m_bCodeExist[m_pDataAll->aMphgD[i].nCode] = TRUE;
	}

	return TRUE;
}

void CDgnLateralCapaOptionSectionDlg::GetDefaultSectData(T_SECT_K SectK, T_DGN_SECT_DATA &tempD)
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

	// SectK를 할당한 Elem이 어떤 Type인지를 확인
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

	// SectK가 M-phi계산 중 어느것을 지원하는지 확인
	BOOL bRCAv, bStlAv, bSrcAv;
	m_pDoc->m_pEditData->IsValidSectType4MpstEachMatl(rSectD.SectBefore.Shape, rSectD.nStype, TRUE, TRUE, bRCAv, bStlAv, bSrcAv);// M-Phi와 컨셉을 맞추기 위해 기둥이든 보든 상관없이 일단 행은 생기게 함.. 그래서 col과 beam을 TRUE로 넘겨줌
	if(!bRCAv) // 보유수평내력은 RC만..
	{
		tempD.bCandidate = FALSE;
		return;
	}
}

BOOL CDgnLateralCapaOptionSectionDlg::InitGrid()
{		
	m_pGrid->SubclassDlgItem(IDC_DGN_SECTION_GRID, this);
	m_pGrid->Initialize(m_pDataAll->MpgbD.nCode);	
	m_pGrid->SetData(m_aData);  

	return TRUE;
}

void CDgnLateralCapaOptionSectionDlg::OnColumnBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DGN_CON_REBC, 0));			
}

void CDgnLateralCapaOptionSectionDlg::OnColumnGenBtn() 
{
	//RC 임의단면 기둥 설계기능
	ConcSectionRebarViewDlg dlg;
	dlg.SetSectionInputType(TRUE, TRUE, TRUE);	
	dlg.DoModal();
}

void CDgnLateralCapaOptionSectionDlg::OnResetBtn() 
{
	m_aData.RemoveAll();		
	CArray<T_SECT_K, T_SECT_K> aSectK;	aSectK.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetSectKeyList(aSectK);
	for(int i=0; i < aSectK.GetSize() ; i++)
	{
		T_DGN_SECT_DATA tempD;
		GetDefaultSectData(aSectK[i], tempD);
		if(tempD.bCandidate)	m_aData.Add(tempD);
	}
	m_pGrid->SetData(m_aData);
}

void CDgnLateralCapaOptionSectionDlg::OnOK()
{
	return;
}

void CDgnLateralCapaOptionSectionDlg::OnCancel() 
{
	return;
}

void CDgnLateralCapaOptionSectionDlg::SetDlgDataToParent() 
{
	// 결국 Parent의 m_pDataAll 의 Data가 변경됨
	m_pDataAll->aMpstK.RemoveAll();
	m_pDataAll->aMpstD.RemoveAll();

	for(int i=0; i<m_aData.GetSize(); i++)
	{	
		m_pDataAll->aMpstK.Add(m_aData[i].SectK);
		m_pDataAll->aMpstD.Add(m_aData[i].MpstD);
	}
}

void CDgnLateralCapaOptionSectionDlg::Dlg2Data() 
{
	m_pGrid->GetData(m_aData);

	SetDlgDataToParent();
}

void CDgnLateralCapaOptionSectionDlg::MyUpdate(LPARAM lHint, CObject* pHint)
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


void CDgnLateralCapaOptionSectionDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SECT_K Key;
	T_SECT_D Data;
	T_RPSC_D tempD;
	T_RCHK_D rchkD;
	T_REBT_D rebtD;

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

		
		case(UR_REBT_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.				
				pViewBuff->GetRebt(nKey, Key, rebtD);
				m_pDoc->m_pAttrCtrl->GetSect(Key, Data);
				ModifyItem(Key, Data);
				break;
		case(UR_REBT_DEL):
			// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetRebt(nKey, Key, rebtD);
				m_pDoc->m_pAttrCtrl->GetSect(Key, Data);
				ModifyItem(Key, Data);
				break;
		default:
			break;
		}
	} // end of while
}

BOOL CDgnLateralCapaOptionSectionDlg::ModifyItem(T_SECT_K Key, T_SECT_D &mData)
{
	T_DGN_SECT_DATA tempD;
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

void CDgnLateralCapaOptionSectionDlg::OnDestroy() 
{
	Dlg2Data();

	if (m_pGrid)
	{
		delete m_pGrid;
		m_pGrid = NULL;
	}

	CDlgChild::OnDestroy();
}
