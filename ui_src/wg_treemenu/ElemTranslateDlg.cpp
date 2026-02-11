// ElemTranslateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemTranslateDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_dbRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Element에 대한 Define이 없어서 아래처럼 Define 사용
// 나중에 ELEM이 정의되면 이 define만 수정하면 된다.
#define MOUSEEDIT_USE_WHAT_SET_DIST MOUSEEDIT_USE_NODE_SET_DIST

/////////////////////////////////////////////////////////////////////////////
// CElemTranslateDlg dialog


CElemTranslateDlg::CElemTranslateDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemTranslateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemTranslateDlg)
	m_nCopyOrMove = 0;
	m_nOption = 1;
	m_nChooseAxis = 0;
	m_bAttrNodeCopy = FALSE;
	m_bAttrElemCopy = FALSE;
	m_bIntstNode = FALSE;
	m_bIntstElem = FALSE;
	m_bDelFreeNode = FALSE;
	//}}AFX_DATA_INIT
	m_DisableOnMove.Add(IDC_TM_COPY_NODEATTR_CHECK );
	m_DisableOnMove.Add(IDC_TM_COPY_ELEMATTR_CHECK );
	m_DisableOnMove.Add(IDC_TM_COPYNODEDATA);
#if !defined(_TOWER)
	m_DisableOnMove.Add(IDC_TM_COPYELEMDATA);
#endif
	m_DisableOnMove.Add(IDC_TM_MATL_INC_TITLE);
	m_DisableOnMove.Add(IDC_TM_MATL_INC_EDIT);
	//m_DisableOnMove.Add(IDC_TM_MATL_INC_SPIN);
	m_DisableOnMove.Add(IDC_TM_SECT_INC_TITLE);
	m_DisableOnMove.Add(IDC_TM_SECT_INC_EDIT);
	//m_DisableOnMove.Add(IDC_TM_SECT_INC_SPIN);
	m_DisableOnMove.Add(IDC_TM_THIK_INC_TITLE);
	m_DisableOnMove.Add(IDC_TM_THIK_INC_EDIT);
	//m_DisableOnMove.Add(IDC_TM_THIK_INC_SPIN);
	m_DisableOnMove.Add(IDC_TM_INTST_TITLE);
	m_DisableOnMove.Add(IDC_TM_INTST_NODE);
	m_DisableOnMove.Add(IDC_TM_INTST_ELEM);
	m_DisableOnMove.Add(IDC_TM_MATL_INC_REP);
	m_DisableOnMove.Add(IDC_TM_SECT_INC_REP);
	m_DisableOnMove.Add(IDC_TM_THIK_INC_REP);

	m_EnableOnMove.Add(IDC_TM_DEL_FREENODE);

	m_aThik.Add(IDC_TM_THIK_INC_TITLE);
	m_aThik.Add(IDC_TM_THIK_INC_EDIT );
	//m_aThik.Add(IDC_TM_THIK_INC_SPIN );

	m_aIterNum.Add(IDC_TM_NTIMES_STATIC         );
	m_aIterNum.Add(IDC_TM_ITER_NUM              );
	//m_aIterNum.Add(IDC_TM_ITER_NUM_SPIN         );

	m_aIterNum2.Add(IDC_TM_NTIMES_STATIC2       );
	m_aIterNum2.Add(IDC_TM_ITER_NUM2            );
	//m_aIterNum2.Add(IDC_TM_ITER_NUM_SPIN2       );

	m_NodeInc.Add(IDC_TM_NODE_INC_EDIT);
	//m_NodeInc.Add(IDC_TM_NODE_INC_SPIN);

	m_EqualDistance.Add(IDC_TM_EQU_XYZ_STATIC   );
	m_EqualDistance.Add(IDC_TM_XYZ_EQUDIST_EDIT );
	m_EqualDistance.Add(IDC_TM_DIST_UNIT1       );
	
	m_UEqualDistance.Add(IDC_TM_STATIC4         );  
	m_UEqualDistance.Add(IDC_TM_XAXIS_RADIO     );
	m_UEqualDistance.Add(IDC_TM_YAXIS_RADIO     );
	m_UEqualDistance.Add(IDC_TM_ZAXIS_RADIO     );
	m_UEqualDistance.Add(IDC_TM_ARBITRARY_RADIO );
	m_UEqualDistance.Add(IDC_TM_STATIC5         );
	m_UEqualDistance.Add(IDC_TM_UNEQ_DIST_EDIT  );
	m_UEqualDistance.Add(IDC_TM_DIST_UNIT2      );
	m_UEqualDistance.Add(IDC_TM_STATIC3         );
	m_UEqualDistance.Add(IDC_TM_DIR_STATIC      );
	m_UEqualDistance.Add(IDC_TM_DIR_VECTOR_EDIT );
	m_UEqualDistance.Add(IDC_TM_DIST_UNIT3      );
}


void CElemTranslateDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemTranslateDlg)
	DDX_Control(pDX, IDC_TM_THIK_INC_REP, m_wndThikIncRep);
	DDX_Control(pDX, IDC_TM_SECT_INC_REP, m_wndSectIncRep);
	DDX_Control(pDX, IDC_TM_MATL_INC_REP, m_wndMatlIncRep);
	DDX_Control(pDX, IDC_TM_THIK_INC_EDIT, m_wndThikInc);
	DDX_Control(pDX, IDC_TM_SECT_INC_EDIT, m_wndSectInc);
	DDX_Control(pDX, IDC_TM_MATL_INC_EDIT, m_wndMatlInc);
	DDX_Control(pDX, IDC_TM_ITER_NUM2, m_wndIterNum2);
	DDX_Control(pDX, IDC_TM_DIR_VECTOR_EDIT,  m_wndDirVector);
	DDX_Control(pDX, IDC_TM_XYZ_EQUDIST_EDIT, m_wndEQDist);
	DDX_Control(pDX, IDC_TM_UNEQ_DIST_EDIT,   m_wndUEQDist);
	DDX_Control(pDX, IDC_TM_ITER_NUM, m_wndIterNum);
	DDX_Control(pDX, IDC_TM_NODE_INC_EDIT, m_wndNodeInc);
	DDX_Control(pDX, IDC_TM_DIST_UNIT3, m_DistUnit3);
	DDX_Control(pDX, IDC_TM_DIST_UNIT2, m_DistUnit2);
	DDX_Control(pDX, IDC_TM_DIST_UNIT1, m_DistUnit1);
	DDX_Radio(pDX, IDC_TM_COPY_RADIO, m_nCopyOrMove);
	DDX_Radio(pDX, IDC_TM_NODE_INC_RADIO, m_nOption);
	DDX_Radio(pDX, IDC_TM_XAXIS_RADIO, m_nChooseAxis);
	DDX_Check(pDX, IDC_TM_COPY_NODEATTR_CHECK,  m_bAttrNodeCopy);
	DDX_Check(pDX, IDC_TM_COPY_ELEMATTR_CHECK,  m_bAttrElemCopy);
	DDX_Check(pDX, IDC_TM_INTST_NODE, m_bIntstNode);
	DDX_Check(pDX, IDC_TM_INTST_ELEM, m_bIntstElem);
	DDX_Check(pDX, IDC_TM_DEL_FREENODE, m_bDelFreeNode);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CElemTranslateDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CElemTranslateDlg)
	ON_BN_CLICKED(IDC_TM_COPY_RADIO, OnTmCopyRadio)
	ON_BN_CLICKED(IDC_TM_MOVE_RADIO, OnTmMoveRadio)
	ON_BN_CLICKED(IDC_TM_ARBITRARY_RADIO, OnTmAxisRadios)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_ITER_NUM_SPIN, OnDeltaposTmIterNumSpin)
	ON_BN_CLICKED(IDC_TM_NODE_INC_RADIO, OnTmNodeIncRadio)
	ON_BN_CLICKED(IDC_TM_EQUAL_DIST_RADIO, OnTmEqualDistRadio)
	ON_BN_CLICKED(IDC_TM_UNEQUAL_DIST_RADIO, OnTmUnequalDistRadio)
	ON_BN_CLICKED(IDC_TM_COPYNODEDATA, OnTmCopynodedata)
	ON_BN_CLICKED(IDC_TM_COPYELEMDATA, OnTmCopyelemdata)
	ON_BN_CLICKED(IDC_TM_TOLERANCE, OnTmTolerance)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_NODE_INC_SPIN, OnDeltaposTmIncSpin)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_ITER_NUM_SPIN2, OnDeltaposTmIterNumSpin)
	ON_BN_CLICKED(IDC_TM_XAXIS_RADIO, OnTmAxisRadios)
	ON_BN_CLICKED(IDC_TM_YAXIS_RADIO, OnTmAxisRadios)
	ON_BN_CLICKED(IDC_TM_ZAXIS_RADIO, OnTmAxisRadios)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_MATL_INC_SPIN, OnDeltaposTmIncSpin)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_SECT_INC_SPIN, OnDeltaposTmIncSpin)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_THIK_INC_SPIN, OnDeltaposTmIncSpin)
	ON_BN_CLICKED(IDC_TM_INTST_TOL, OnTmIntstTol)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemTranslateDlg message handlers
BOOL CElemTranslateDlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndEQDist.SetAttUcsDistance();
	m_wndEQDist.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_wndDirVector.SetAttUcsDistance();
	m_wndDirVector.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	//m_wndDirVector.SetAttU
		
	m_DistUnit1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_DistUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_DistUnit3.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_wndIterNum.SetRange(0, SHRT_MAX);
	m_wndIterNum.SetWindowText(_T("1"));
	m_wndIterNum.SetInteger(TRUE);
	m_wndIterNum2.SetRange(0, SHRT_MAX);
	m_wndIterNum2.SetWindowText(_T("1"));
	m_wndIterNum2.SetInteger(TRUE);
	m_wndDirVector.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndEQDist.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndNodeInc.SetRange(SHRT_MIN, SHRT_MAX);
	m_wndNodeInc.SetWindowText(_T("0"));
	m_wndMatlInc.SetRange(SHRT_MIN, SHRT_MAX);
	m_wndMatlInc.SetWindowText(_T("0"));
	m_wndSectInc.SetRange(SHRT_MIN, SHRT_MAX);
	m_wndSectInc.SetWindowText(_T("0"));
	m_wndThikInc.SetRange(SHRT_MIN, SHRT_MAX);
	m_wndThikInc.SetWindowText(_T("0"));

	if(m_nCopyOrMove == 0)
	{
		CtrlEnableDisable(m_DisableOnMove,TRUE);
		CtrlEnableDisable(m_EnableOnMove,FALSE);
	}
	else
	{
		CtrlEnableDisable(m_DisableOnMove,FALSE);
		CtrlEnableDisable(m_EnableOnMove,TRUE);
	}

	BOOL bEnable;
	if (m_nOption == 2 && m_nChooseAxis == 3) // when unequal and arbitray axis
		bEnable = TRUE;
	else bEnable = FALSE;
	CWnd* pWnd = GetDlgItem(IDC_TM_DIR_VECTOR_EDIT);
	if(pWnd) pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_TM_DIR_STATIC);
	if(pWnd) pWnd->EnableWindow(bEnable);

	if( m_nOption == 0)
	{
		CtrlEnableDisable(m_EqualDistance,FALSE);
		CtrlEnableDisable(m_UEqualDistance,FALSE);
		if (m_nCopyOrMove == 0) CtrlEnableDisable(m_aIterNum, TRUE);
		else CtrlEnableDisable(m_aIterNum, FALSE);
		CtrlEnableDisable(m_aIterNum2, FALSE);
	}
	else if (m_nOption == 1)
	{
		CtrlEnableDisable(m_NodeInc, FALSE);
		CtrlEnableDisable(m_UEqualDistance,FALSE);
		if (m_nCopyOrMove == 0) CtrlEnableDisable(m_aIterNum2, TRUE);
		else CtrlEnableDisable(m_aIterNum2, FALSE);
		CtrlEnableDisable(m_aIterNum, FALSE);
	}
	else
	{
		CtrlEnableDisable(m_NodeInc, FALSE);
		CtrlEnableDisable(m_EqualDistance,FALSE);
		CtrlEnableDisable(m_aIterNum, FALSE);
		CtrlEnableDisable(m_aIterNum2, FALSE);
	}

#ifdef _MVIL
	CtrlEnableDisable(m_aThik, FALSE);
#endif

	GotoDlgCtrl(&m_wndNodeInc);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/*
void CElemTranslateDlg::CtrlEnableDisable(CArray<UINT,UINT>&CtrlArray,BOOL bIsEnable)
{
	CWnd * pWnd;
	int nSize = CtrlArray.GetSize();
	for(int i = 0 ; i < nSize; i++)
	{
		pWnd = GetDlgItem(CtrlArray[i]);
		if(pWnd)
			pWnd->EnableWindow(bIsEnable);
	}
}
*/
void CElemTranslateDlg::OnTmCopyRadio() 
{
	if(1 == m_nCopyOrMove)
	{
		m_nCopyOrMove = 0;
		CtrlEnableDisable(m_DisableOnMove,TRUE);
		CtrlEnableDisable(m_EnableOnMove,FALSE);

		BOOL bEnable, bEnable2;
		if (m_nOption == 0) { bEnable = TRUE; bEnable2 = FALSE; }
		else if (m_nOption == 1) { bEnable = FALSE; bEnable2 = TRUE; }
		else { bEnable = FALSE; bEnable2 = FALSE; }
		CtrlEnableDisable(m_aIterNum, bEnable);
		CtrlEnableDisable(m_aIterNum2, bEnable2);
#ifdef _MVIL
		CtrlEnableDisable(m_aThik, FALSE);
#endif
	}
}

void CElemTranslateDlg::OnTmMoveRadio() 
{
	if(0 == m_nCopyOrMove)
	{
		m_nCopyOrMove = 1;
		CtrlEnableDisable(m_DisableOnMove,FALSE);
		CtrlEnableDisable(m_EnableOnMove,TRUE);
		CtrlEnableDisable(m_aIterNum, FALSE);
		CtrlEnableDisable(m_aIterNum2, FALSE);
	}
}


void CElemTranslateDlg::OnTmAxisRadios() 
{
	UpdateData(TRUE);
	if(m_nChooseAxis == 3)
	{
		CWnd* pWnd = GetDlgItem(IDC_TM_DIR_VECTOR_EDIT);
		if(pWnd) pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_TM_DIR_STATIC);
		if(pWnd) pWnd->EnableWindow(TRUE);
	}
	else
	{
		CWnd* pWnd = GetDlgItem(IDC_TM_DIR_VECTOR_EDIT);
		if(pWnd) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_TM_DIR_STATIC);
		if(pWnd) pWnd->EnableWindow(FALSE);
	}
	GotoDlgCtrl(&m_wndUEQDist);
}

void CElemTranslateDlg::OnDeltaposTmIterNumSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const MSG* pMsg = GetCurrentMessage();
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	int NIter;
	
	CString strIter;

	CFormulaEditSpin* pIterNum;
	UINT nID = LOWORD(pMsg->wParam);
	switch(nID)
	{
	case IDC_TM_ITER_NUM_SPIN: pIterNum = &m_wndIterNum; break;
	case IDC_TM_ITER_NUM_SPIN2: pIterNum = &m_wndIterNum2; break;
	default: ASSERT(0);
	}

	pIterNum->GetEditValue(NIter);
	NIter -= pNMUpDown->iDelta;
	if(NIter < 1) NIter = 1;
	
	strIter.Format(_LS(IDS_WG_TREEMENU__d),NIter);
	
	pIterNum->SetWindowText(strIter);
	*pResult = 0;
}

void CElemTranslateDlg::OnDeltaposTmIncSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const MSG* pMsg = GetCurrentMessage();
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	int NIter;
	
	CString strIter;

	CFormulaEditSpin* pIterNum;
	UINT nID = LOWORD(pMsg->wParam);
	switch(nID)
	{
	case IDC_TM_NODE_INC_SPIN: pIterNum = &m_wndNodeInc; break;
	case IDC_TM_MATL_INC_SPIN: pIterNum = &m_wndMatlInc; break;
	case IDC_TM_SECT_INC_SPIN: pIterNum = &m_wndSectInc; break;
	case IDC_TM_THIK_INC_SPIN: pIterNum = &m_wndThikInc; break;
	default: ASSERT(0);
	}

	pIterNum->GetEditValue(NIter);
	NIter -= pNMUpDown->iDelta;
	
	strIter.Format(_LS(IDS_WG_TREEMENU__d),NIter);
	
	pIterNum->SetWindowText(strIter);
	*pResult = 0;
}

void CElemTranslateDlg::OnTmNodeIncRadio()
{
	if(m_nOption != 0)
	{
		m_nOption = 0;
		CtrlEnableDisable(m_NodeInc,TRUE);
		CtrlEnableDisable(m_EqualDistance,FALSE);
		CtrlEnableDisable(m_UEqualDistance,FALSE);

		if (m_nCopyOrMove == 0) CtrlEnableDisable(m_aIterNum, TRUE);
		else CtrlEnableDisable(m_aIterNum, FALSE);
		CtrlEnableDisable(m_aIterNum2, FALSE);
	}
	GotoDlgCtrl(&m_wndNodeInc);
}

void CElemTranslateDlg::OnTmEqualDistRadio() 
{
	if(m_nOption != 1)
	{
		m_nOption = 1;
		CtrlEnableDisable(m_EqualDistance,TRUE);
		CtrlEnableDisable(m_NodeInc,FALSE);
		CtrlEnableDisable(m_UEqualDistance,FALSE);

		CtrlEnableDisable(m_aIterNum, FALSE);
		if (m_nCopyOrMove == 0) CtrlEnableDisable(m_aIterNum2, TRUE);
		else CtrlEnableDisable(m_aIterNum2, FALSE);
	}
	GotoDlgCtrl(&m_wndEQDist);
}

void CElemTranslateDlg::OnTmUnequalDistRadio() 
{
	if(m_nOption != 2)
	{
		m_nOption = 2;
		CtrlEnableDisable(m_UEqualDistance,TRUE);
		CtrlEnableDisable(m_NodeInc,FALSE);
		CtrlEnableDisable(m_EqualDistance,FALSE);
		OnTmAxisRadios();

		CtrlEnableDisable(m_aIterNum, FALSE);
		CtrlEnableDisable(m_aIterNum2, FALSE);
	}
}

// 설정된 Option 

BOOL CElemTranslateDlg::GetParameter(_ElemTranslate& ET)
{
	UpdateData(TRUE);
	
	CString strDirVector;
	CString strEQDist;
	CString strUEQDist;
	CString strIterNum;
	//double dwx,dwy,dwz;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	if(m_nCopyOrMove == 0)  ET.m_bIsCopy = TRUE;
	else                    ET.m_bIsCopy = FALSE;

	ET.m_nOption = m_nOption;

	if (m_nOption == 0) // node increment
	{
		if (!m_wndNodeInc.GetEditValue(ET.m_nNodeInc))
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_node_increment_valu));
			return FALSE;
		}
	}
	else if (m_nOption == 1)
	{
		m_wndEQDist.GetWindowText(strEQDist);
		if(TRUE == m_wndEQDist.IsKeyPressed())
		{
			if(GetEQDistance(strEQDist,ET.m_EQ_UX, ET.m_EQ_UY, ET.m_EQ_UZ)) 
			{
				pIGM->GetWcsVector(ET.m_EQ_UX, ET.m_EQ_UY, ET.m_EQ_UZ,ET.m_EQ_WX, ET.m_EQ_WY, ET.m_EQ_WZ);
			}
			else return FALSE;
		}
		else
		{
			//m_wndEQDist.GetCurMousePosDist(dwx,dwy,dwz,ET.m_EQ_DX,ET.m_EQ_DY,ET.m_EQ_DZ);
			m_wndEQDist.GetCurMousePosDist(ET.m_EQ_WX, ET.m_EQ_WY, ET.m_EQ_WZ,
																		 ET.m_EQ_UX, ET.m_EQ_UY, ET.m_EQ_UZ);
		}
	}
	else
	{
		ET.m_nUEQ_Axis = m_nChooseAxis;
		
		m_wndUEQDist.GetWindowText(strUEQDist);
		if(GetUEDistance(strUEQDist,ET.m_UEQ_DistList)) ;
		else return FALSE;

		if(!ET.m_UEQ_DistList.GetSize()) return FALSE;

		ET.m_UEQ_UX  = 0.;
		ET.m_UEQ_UY  = 0.;
		ET.m_UEQ_UZ  = 0.;

		switch(m_nChooseAxis)
		{
		case 0: // X Axis...
			ET.m_UEQ_UX = 1.0;
			pIGM->GetWcsVector(ET.m_UEQ_UX,ET.m_UEQ_UY,ET.m_UEQ_UZ,
												 ET.m_UEQ_WX,ET.m_UEQ_WY,ET.m_UEQ_WZ);
			break;
		case 1: // Y Axis...
			ET.m_UEQ_UY = 1.0;
			pIGM->GetWcsVector(ET.m_UEQ_UX,ET.m_UEQ_UY,ET.m_UEQ_UZ,
												 ET.m_UEQ_WX,ET.m_UEQ_WY,ET.m_UEQ_WZ);
			break;
		case 2: // Z Axis...
			ET.m_UEQ_UZ = 1.0;
			pIGM->GetWcsVector(ET.m_UEQ_UX,ET.m_UEQ_UY,ET.m_UEQ_UZ,
												 ET.m_UEQ_WX,ET.m_UEQ_WY,ET.m_UEQ_WZ);
			break;
		case 3: // Arbitrary...
			{
				m_wndDirVector.GetWindowText(strDirVector);
				if(TRUE == m_wndDirVector.IsKeyPressed())
				{
					if(GetDirectionVector(strDirVector,ET.m_UEQ_UX,ET.m_UEQ_UY,ET.m_UEQ_UZ))
					{
						 pIGM->GetWcsVector(ET.m_UEQ_UX,ET.m_UEQ_UY,ET.m_UEQ_UZ,
																ET.m_UEQ_WX,ET.m_UEQ_WY,ET.m_UEQ_WZ);
					}
					else return FALSE;
				}
				else
				{
					//double dwx,dwy,dwz;
					m_wndDirVector.GetCurMousePosDist(ET.m_UEQ_WX,ET.m_UEQ_WY,ET.m_UEQ_WZ,
																						ET.m_UEQ_UX,ET.m_UEQ_UY,ET.m_UEQ_UZ);
				}
				
				double VL = ET.m_UEQ_WX * ET.m_UEQ_WX + ET.m_UEQ_WY * ET.m_UEQ_WY + 
										ET.m_UEQ_WZ * ET.m_UEQ_WZ ; 
				
				
				
				if(pIGM->IsZero(VL))
				{
					// Error Message 표시할것 ..
					return FALSE;
				}
				
				VL = sqrt(VL);

				ET.m_UEQ_WX = ET.m_UEQ_WX / VL;
				ET.m_UEQ_WY = ET.m_UEQ_WY / VL;
				ET.m_UEQ_WZ = ET.m_UEQ_WZ / VL;
				//double Dist = ET.m_UEQ_DistList[0];

				//ET.m_UEQ_UX = ET.m_UEQ_UX * Dist;
				//ET.m_UEQ_UY = ET.m_UEQ_UY * Dist;
				//ET.m_UEQ_UZ = ET.m_UEQ_UZ * Dist;
			}
			break;
		default:
			return FALSE;
		}
	}

	if (m_nOption == 0 || m_nOption == 1) // when node increment or equal distance
	{
		if(m_nCopyOrMove == 0)
		{  
			if (m_nOption == 0) m_wndIterNum.GetWindowText(strIterNum);
			else m_wndIterNum2.GetWindowText(strIterNum);

			if(GetNumberOfTimes(strIterNum,ET.m_NTimes));
			else return FALSE;
		}
	}
	else // when unequal distance
	{
		if (m_nCopyOrMove == 0) // when copy
			ET.m_NTimes = ET.m_UEQ_DistList.GetSize();
	}
	
	ET.m_bAttrNodeCopy = m_bAttrNodeCopy;
	ET.m_bAttrElemCopy = m_bAttrElemCopy;

	if (!m_wndMatlInc.GetEditValue(ET.m_nMatlInc))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_material_increment_));
		return FALSE;
	}
	if (!m_wndSectInc.GetEditValue(ET.m_nSectInc))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_section_increment_v));
		return FALSE;
	}
	if (!m_wndThikInc.GetEditValue(ET.m_nThikInc))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_thickness_increment));
		return FALSE;
	}

	ET.m_bMatlIncRep = (m_wndMatlIncRep.GetCheck() == 1) ? TRUE : FALSE;
	ET.m_bSectIncRep = (m_wndSectIncRep.GetCheck() == 1) ? TRUE : FALSE;
	ET.m_bThikIncRep = (m_wndThikIncRep.GetCheck() == 1) ? TRUE : FALSE;

	ET.m_bIntstNode = m_bIntstNode;
	ET.m_bIntstElem = m_bIntstElem;
	ET.m_bDelFreeNode = m_bDelFreeNode;
	
	return TRUE;
}

void CElemTranslateDlg::MoveElem(_ElemTranslate& ET)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<unsigned int, unsigned int> NKeyArray;
	
	if(pIGM->GetSelectedElemKeyList(NKeyArray))
	{
		if (ET.m_nOption == 0)  // Node Increment
		{
			if (pDoc->m_pDataCtrl->TranslateElem_Move_Node(NKeyArray, ET.m_nNodeInc, ET.m_bDelFreeNode))
			{
				pDoc->m_pViewCtrl->UnselectAll(NULL);
			}
		}
		else if (ET.m_nOption == 1) // Equal Distance 
		{
			if((pDoc->m_pDataCtrl)->TranslateElem_Move_Equal(NKeyArray, ET.m_EQ_WX, 
																							 ET.m_EQ_WY, ET.m_EQ_WZ, ET.m_bDelFreeNode))
			{
				//Move Operation시 발생하는 Delete , Add Operation으로 인해 Local Select가 해제 된다. 
				//CDBDoc에서는 Select가 해제 되지 않으므로 Local select에 Move대상을 반영할것. 
				pDoc->m_pViewCtrl->UnselectAll(NULL);
			}
		}
		else
		{// Unequal Distance
			double Dist = ET.m_UEQ_DistList[0];
			if((pDoc->m_pDataCtrl)->TranslateElem_Move_Equal(NKeyArray, ET.m_UEQ_WX*Dist,
																							ET.m_UEQ_WY*Dist, ET.m_UEQ_WZ*Dist, ET.m_bDelFreeNode))
			{
				pDoc->m_pViewCtrl->UnselectAll(NULL);
			}
		}
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
	}
}

void CElemTranslateDlg::CopyElem(_ElemTranslate& ET)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<unsigned int, unsigned int> NKeyArray;
	
	if(pIGM->GetSelectedElemKeyList(NKeyArray))
	{
		if (ET.m_nOption == 0)  // Node Increment
		{
			if (pDoc->m_pDataCtrl->TranslateElem_Copy_Node(NKeyArray, 
								ET.m_NTimes, ET.m_nNodeInc, ET.m_bAttrNodeCopy, ET.m_bAttrElemCopy,
								ET.m_nMatlInc, ET.m_bMatlIncRep,
								ET.m_nSectInc, ET.m_bSectIncRep,
								ET.m_nThikInc, ET.m_bThikIncRep,
								ET.m_bIntstNode, ET.m_bIntstElem))
			{
				pDoc->m_pViewCtrl->UnselectAll(NULL);
			}
		}
		else if (ET.m_nOption == 1) // Equal Distance 
		{
//      BOOL TranslateElem_Copy_Unequal(CArray<T_ELEM_K, T_ELEM_K>& rKey, int nCopy, double ux, double uy, double uz,
//		  CArray<double, double>& arDistance, BOOL bAllowDuplicate, BOOL bCopyData);
			if((pDoc->m_pDataCtrl)->TranslateElem_Copy_Equal(NKeyArray,ET.m_NTimes,ET.m_EQ_WX, 
															ET.m_EQ_WY, ET.m_EQ_WZ, ET.m_bAttrNodeCopy, ET.m_bAttrElemCopy,
															ET.m_nMatlInc, ET.m_bMatlIncRep, 
															ET.m_nSectInc, ET.m_bSectIncRep, 
															ET.m_nThikInc, ET.m_bThikIncRep,
															ET.m_bIntstNode, ET.m_bIntstElem))
			{
				pDoc->m_pViewCtrl->UnselectAll(NULL);
			}
		}
		else
		{// Unequal Distance
			if((pDoc->m_pDataCtrl)->TranslateElem_Copy_Unequal(NKeyArray,ET.m_NTimes,ET.m_UEQ_WX,
															ET.m_UEQ_WY, ET.m_UEQ_WZ,ET.m_UEQ_DistList,
															ET.m_bAttrNodeCopy, ET.m_bAttrElemCopy,
															ET.m_nMatlInc, ET.m_bMatlIncRep, 
															ET.m_nSectInc, ET.m_bSectIncRep, 
															ET.m_nThikInc, ET.m_bThikIncRep,
															ET.m_bIntstNode, ET.m_bIntstElem))
			{
				pDoc->m_pViewCtrl->UnselectAll(NULL);
			}
		}
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
	}
}

void CElemTranslateDlg::Execute()
{
	_ElemTranslate ET;

	if(GetParameter(ET))
	{
		//CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
		//I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
		if(ET.m_bIsCopy == FALSE) // Move
		{
			MoveElem(ET);
		}
		else // Copy
		{
			CopyElem(ET); 
		}
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Parameter_Error____));
	}
}

void CElemTranslateDlg::OnTmCopynodedata() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_COPYATTR_NODE,0));
}

void CElemTranslateDlg::OnTmCopyelemdata() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_COPYATTR_ELEM,0));
}

void CElemTranslateDlg::OnTmTolerance() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_MERGETOL,0));
}


void CElemTranslateDlg::OnTmIntstTol() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_INTERSECTTOL,0));	
}
