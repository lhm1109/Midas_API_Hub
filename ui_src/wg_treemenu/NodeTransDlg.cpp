// NodeTransDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "NodeTransDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_dbRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNodeTransDlg dialog


CNodeTransDlg::CNodeTransDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CNodeTransDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNodeTransDlg)
	m_nCopyOrMove = 0;
	m_nEqualOrUnEqual = 0;
	m_nChooseAxis = 0;
	m_bDupNode = TRUE;
	m_bAttrCopy = FALSE;
	m_bIntersect = FALSE;
	//}}AFX_DATA_INIT
	m_DisableOnMove.Add(IDC_TM_MERGE_DUP_NODE_CHECK);
	m_DisableOnMove.Add(IDC_TM_COPY_NODEATTR_CHECK );
	m_DisableOnMove.Add(IDC_TM_TOLERANCE);
	m_DisableOnMove.Add(IDC_TM_COPYNODEDATA);

	m_aIterNum.Add(IDC_TM_NTIMES_STATIC    );
	m_aIterNum.Add(IDC_TM_ITER_NUM         );
	//m_aIterNum.Add(IDC_TM_ITER_NUM_SPIN    );

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

void CNodeTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNodeTransDlg)
	DDX_Control(pDX, IDC_TM_DIR_VECTOR_EDIT,  m_wndDirVector);
	DDX_Control(pDX, IDC_TM_XYZ_EQUDIST_EDIT, m_wndEQDist);
	DDX_Control(pDX, IDC_TM_UNEQ_DIST_EDIT,   m_wndUEQDist);
	DDX_Control(pDX, IDC_TM_ITER_NUM, m_wndIterNum);
	DDX_Control(pDX, IDC_TM_DIST_UNIT3, m_DistUnit3);
	DDX_Control(pDX, IDC_TM_DIST_UNIT2, m_DistUnit2);
	DDX_Control(pDX, IDC_TM_DIST_UNIT1, m_DistUnit1);
	DDX_Radio(pDX, IDC_TM_COPY_RADIO, m_nCopyOrMove);
	DDX_Radio(pDX, IDC_TM_EQUAL_DIST_RADIO, m_nEqualOrUnEqual);
	DDX_Radio(pDX, IDC_TM_XAXIS_RADIO, m_nChooseAxis);
	DDX_Check(pDX, IDC_TM_MERGE_DUP_NODE_CHECK, m_bDupNode);
	DDX_Check(pDX, IDC_TM_COPY_NODEATTR_CHECK,  m_bAttrCopy);
	DDX_Check(pDX, IDC_TM_ITST_CHECK, m_bIntersect);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNodeTransDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CNodeTransDlg)
	ON_BN_CLICKED(IDC_TM_COPY_RADIO, OnTmCopyRadio)
	ON_BN_CLICKED(IDC_TM_MOVE_RADIO, OnTmMoveRadio)
	ON_BN_CLICKED(IDC_TM_ARBITRARY_RADIO, OnTmAxisRadios)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_ITER_NUM_SPIN, OnDeltaposTmIterNumSpin)
	ON_BN_CLICKED(IDC_TM_EQUAL_DIST_RADIO, OnTmEqualDistRadio)
	ON_BN_CLICKED(IDC_TM_UNEQUAL_DIST_RADIO, OnTmUnequalDistRadio)
	ON_BN_CLICKED(IDC_TM_COPYNODEDATA, OnTmCopynodedata)
	ON_BN_CLICKED(IDC_TM_TOLERANCE, OnTmTolerance)
	ON_BN_CLICKED(IDC_TM_XAXIS_RADIO, OnTmAxisRadios)
	ON_BN_CLICKED(IDC_TM_YAXIS_RADIO, OnTmAxisRadios)
	ON_BN_CLICKED(IDC_TM_ZAXIS_RADIO, OnTmAxisRadios)
	ON_BN_CLICKED(IDC_TM_INTSECT_TOL, OnTmIntsectTol)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNodeTransDlg message handlers
BOOL CNodeTransDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndEQDist.SetAttUcsDistance();
	m_wndEQDist.SetModeToUse(MOUSEEDIT_USE_NODE_SET_DIST);
	m_wndDirVector.SetAttUcsDistance();
	m_wndDirVector.SetModeToUse(MOUSEEDIT_USE_NODE_SET_DIST);
	//m_wndDirVector.SetAttU
		
	m_DistUnit1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_DistUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_DistUnit3.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_wndIterNum.SetRange(0, SHRT_MAX);
	m_wndIterNum.SetValue(1);
	m_wndIterNum.SetInteger(TRUE);
	m_wndDirVector.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));
	m_wndEQDist.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));

	
	if(m_nCopyOrMove == 0)
		CtrlEnableDisable(m_DisableOnMove,TRUE);
	else
		CtrlEnableDisable(m_DisableOnMove,FALSE);

	BOOL bEnable;
	if (m_nEqualOrUnEqual == 1 && m_nChooseAxis == 3) // when unequal and arbitray axis
		bEnable = TRUE;
	else bEnable = FALSE;
	CWnd* pWnd = GetDlgItem(IDC_TM_DIR_VECTOR_EDIT);
	if(pWnd) pWnd->EnableWindow(bEnable);
	pWnd = GetDlgItem(IDC_TM_DIR_STATIC);
	if(pWnd) pWnd->EnableWindow(bEnable);

	if(m_nEqualOrUnEqual == 0)
	{
		CtrlEnableDisable(m_UEqualDistance,FALSE);
		if (m_nCopyOrMove == 0) // when copy and equal
			CtrlEnableDisable(m_aIterNum, TRUE);
		else CtrlEnableDisable(m_aIterNum, FALSE);
	}
	else
	{
		CtrlEnableDisable(m_EqualDistance,FALSE);
		CtrlEnableDisable(m_aIterNum, FALSE);
	}

	GotoDlgCtrl(&m_wndEQDist);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/*
void CNodeTransDlg::CtrlEnableDisable(CArray<UINT,UINT>&CtrlArray,BOOL bIsEnable)
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
void CNodeTransDlg::OnTmCopyRadio() 
{
	if(1 == m_nCopyOrMove)
	{
		m_nCopyOrMove = 0;
		CtrlEnableDisable(m_DisableOnMove,TRUE);
		if (m_nEqualOrUnEqual == 0)   // enable when equal
			CtrlEnableDisable(m_aIterNum, TRUE);
	}
}

void CNodeTransDlg::OnTmMoveRadio() 
{
	if(0 == m_nCopyOrMove)
	{
		m_nCopyOrMove = 1;
		CtrlEnableDisable(m_DisableOnMove,FALSE);
		if (m_nEqualOrUnEqual == 0) // disable when equal
			CtrlEnableDisable(m_aIterNum, FALSE);
	}
}


void CNodeTransDlg::OnTmAxisRadios() 
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

void CNodeTransDlg::OnDeltaposTmIterNumSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	int NIter;
	
	CString strIter;
	m_wndIterNum.GetWindowText(strIter);

	GetNumberOfTimes(strIter,NIter);
	if(pNMUpDown->iDelta < 0)
	{
			NIter += 1;
	}
	else
	{
		if(NIter > 0)
			NIter -= 1;
	}
	
	strIter.Format(_LS(IDS_WG_TREEMENU__d),NIter);
	
	m_wndIterNum.SetWindowText(strIter);
	*pResult = 0;
}


void CNodeTransDlg::OnTmEqualDistRadio() 
{
	if(m_nEqualOrUnEqual == 1)
	{
		m_nEqualOrUnEqual = 0;
		CtrlEnableDisable(m_EqualDistance,TRUE);
		CtrlEnableDisable(m_aIterNum, m_nCopyOrMove == 0);
		CtrlEnableDisable(m_UEqualDistance,FALSE);
	}
	GotoDlgCtrl(&m_wndEQDist);
}

void CNodeTransDlg::OnTmUnequalDistRadio() 
{
	if(m_nEqualOrUnEqual == 0)
	{
		m_nEqualOrUnEqual = 1;
		CtrlEnableDisable(m_EqualDistance,FALSE);
		CtrlEnableDisable(m_aIterNum, FALSE);
		CtrlEnableDisable(m_UEqualDistance,TRUE);
		OnTmAxisRadios();
	}
}

/*
BOOL TranslateNode_Move(CArray<T_NODE_K, T_NODE_K>& rKey, double dx, double dy, double dz);
BOOL TranslateNode_Copy_Equal(CArray<T_NODE_K, T_NODE_K>& rKey, int nCopy, 
		  double dx, double dy, double dz, BOOL bAllowDuplicate, BOOL bCopyData);
BOOL TranslateNode_Copy_Unequal(CArray<T_NODE_K, T_NODE_K>& rKey, int nCopy, double ux, double uy, double uz,
		  CArray<double, double>& arDistance, BOOL bAllowDuplicate, BOOL bCopyData);
*/

/*
BOOL CNodeTransDlg::GetNumberOfTimes(CString& NTStr,int & NTimes)
{
	MyParser Parser;
		
	if(Parser.ParsingINumberArg(NTStr))
	{
		Parser.GetINumberData(NTimes);
	}
	else
	{
		NTimes = 0;
		return FALSE;
	}
	return TRUE;
}

BOOL CNodeTransDlg::GetEQDistance(CString& EQStr, double& Dx, double& Dy, double& Dz)
{
	MyParser Parser;
	
	if(Parser.ParsingPositionArg(EQStr))
	{
		Parser.GetPositionData(Dx,Dy,Dz);
	}
	else
	{
		 Dx = 0. ; Dy = 0. ; Dz = 0.;  
		 return FALSE;
	}
	return TRUE;
}

BOOL CNodeTransDlg::GetDirectionVector(CString &strVector, double& Ux, double& Uy, double& Uz)
{
	return GetEQDistance(strVector,Ux,Uy,Uz);
}

BOOL CNodeTransDlg::GetUEDistance(CString& UEStr, CArray<double,double>&UEDistants)
{
	MyParser Parser;

	if(Parser.ParsingGridDefineRel(UEStr))
	{
		Parser.GetGridArgList(UEDistants);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}
*/
// 설정된 Option 

BOOL CNodeTransDlg::GetParameter(_NodeTranslate& NT)
{
	UpdateData(TRUE);
	
	CString strDirVector;
	CString strEQDist;
	CString strUEQDist;
	CString strIterNum;
	//double dwx,dwy,dwz;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	if(m_nCopyOrMove == 0)  NT.m_bIsCopy = TRUE;
	else                    NT.m_bIsCopy = FALSE;

	if(m_nEqualOrUnEqual == 0) NT.m_bIsEqual = TRUE;
	else                       NT.m_bIsEqual = FALSE;

	if(m_nEqualOrUnEqual == 0)
	{
		m_wndEQDist.GetWindowText(strEQDist);
		if(TRUE == m_wndEQDist.IsKeyPressed())
			if(GetEQDistance(strEQDist,NT.m_EQ_UX, NT.m_EQ_UY, NT.m_EQ_UZ)) 
			{
				pIGM->GetWcsVector(NT.m_EQ_UX, NT.m_EQ_UY, NT.m_EQ_UZ,NT.m_EQ_WX, NT.m_EQ_WY, NT.m_EQ_WZ);
			}
			else return FALSE;
		else
		{
			//m_wndEQDist.GetCurMousePosDist(dwx,dwy,dwz,NT.m_EQ_DX,NT.m_EQ_DY,NT.m_EQ_DZ);
			m_wndEQDist.GetCurMousePosDist(NT.m_EQ_WX, NT.m_EQ_WY, NT.m_EQ_WZ,
																		 NT.m_EQ_UX, NT.m_EQ_UY, NT.m_EQ_UZ);
		}
	}
	else
	{
		NT.m_nUEQ_Axis = m_nChooseAxis;
		
		m_wndUEQDist.GetWindowText(strUEQDist);
		if(GetUEDistance(strUEQDist,NT.m_UEQ_DistList)) ;
		else return FALSE;

		if(!NT.m_UEQ_DistList.GetSize()) return FALSE;

		NT.m_UEQ_UX  = 0.;
		NT.m_UEQ_UY  = 0.;
		NT.m_UEQ_UZ  = 0.;

		switch(m_nChooseAxis)
		{
		case 0: // X Axis...
			NT.m_UEQ_UX = 1.0;
			pIGM->GetWcsVector(NT.m_UEQ_UX,NT.m_UEQ_UY,NT.m_UEQ_UZ,
												 NT.m_UEQ_WX,NT.m_UEQ_WY,NT.m_UEQ_WZ);
			break;
		case 1: // Y Axis...
			NT.m_UEQ_UY = 1.0;
			pIGM->GetWcsVector(NT.m_UEQ_UX,NT.m_UEQ_UY,NT.m_UEQ_UZ,
												 NT.m_UEQ_WX,NT.m_UEQ_WY,NT.m_UEQ_WZ);
			break;
		case 2: // Z Axis...
			NT.m_UEQ_UZ = 1.0;
			pIGM->GetWcsVector(NT.m_UEQ_UX,NT.m_UEQ_UY,NT.m_UEQ_UZ,
												 NT.m_UEQ_WX,NT.m_UEQ_WY,NT.m_UEQ_WZ);
			break;
		case 3: // Arbitrary...
			{
				m_wndDirVector.GetWindowText(strDirVector);
				if(TRUE == m_wndDirVector.IsKeyPressed())
				{
					if(GetDirectionVector(strDirVector,NT.m_UEQ_UX,NT.m_UEQ_UY,NT.m_UEQ_UZ))
					{
						 pIGM->GetWcsVector(NT.m_UEQ_UX,NT.m_UEQ_UY,NT.m_UEQ_UZ,
																NT.m_UEQ_WX,NT.m_UEQ_WY,NT.m_UEQ_WZ);
																																
					}
					else return FALSE;
				}
				else
				{
					//double dwx,dwy,dwz;
					m_wndDirVector.GetCurMousePosDist(NT.m_UEQ_WX,NT.m_UEQ_WY,NT.m_UEQ_WZ,
																						NT.m_UEQ_UX,NT.m_UEQ_UY,NT.m_UEQ_UZ);
				}
								
				
				double VL = NT.m_UEQ_WX * NT.m_UEQ_WX + NT.m_UEQ_WY * NT.m_UEQ_WY + 
										NT.m_UEQ_WZ * NT.m_UEQ_WZ ; 
				
				
				
				if(pIGM->IsZero(VL))
				{
					// Error Message 표시할것 ..
					return FALSE;
				}
				
				VL = sqrt(VL);

				NT.m_UEQ_WX = NT.m_UEQ_WX / VL;
				NT.m_UEQ_WY = NT.m_UEQ_WY / VL;
				NT.m_UEQ_WZ = NT.m_UEQ_WZ / VL;
				//double Dist = NT.m_UEQ_DistList[0];

				//NT.m_UEQ_UX = NT.m_UEQ_UX * Dist;
				//NT.m_UEQ_UY = NT.m_UEQ_UY * Dist;
				//NT.m_UEQ_UZ = NT.m_UEQ_UZ * Dist;
			}
			break;
		default:
			return FALSE;
		}
	}

	// when copy, store number of iteration
	if(m_nCopyOrMove == 0)
	{  
		if (m_nEqualOrUnEqual == 0) // equal
		{
			if(m_wndIterNum.GetEditValue(NT.m_NTimes));
			else return FALSE;
		}
		else // unequal
		{
			NT.m_NTimes = NT.m_UEQ_DistList.GetSize();
		}
	}
	
	NT.m_bDupNode  = m_bDupNode;
	NT.m_bAttrCopy = m_bAttrCopy;
	NT.m_bIntersect = m_bIntersect;
	
	return TRUE;
}

void CNodeTransDlg::MoveNode(_NodeTranslate& NT)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<unsigned int, unsigned int> NKeyArray;
	
	if(pIGM->GetSelectedNodeKeyList(NKeyArray))
	{
		if(NT.m_bIsEqual) // Equal Distance 
		{
			if((pDoc->m_pDataCtrl)->TranslateNode_Move(NKeyArray, NT.m_EQ_WX, 
															NT.m_EQ_WY, NT.m_EQ_WZ, NT.m_bIntersect))
			{
				//Move Operation시 발생하는 Delete , Add Operation으로 인해 Local Select가 해제 된다. 
				//CDBDoc에서는 Select가 해제 되지 않으므로 Local select에 Move대상을 반영할것. 

				//pIGM->
				pDoc->m_pViewCtrl->UnselectAll(NULL);
			}
		}
		else
		{// Unequal Distance
			double Dist = NT.m_UEQ_DistList[0];
			if((pDoc->m_pDataCtrl)->TranslateNode_Move(NKeyArray, NT.m_UEQ_WX*Dist,
															NT.m_UEQ_WY*Dist, NT.m_UEQ_WZ*Dist, NT.m_bIntersect))
			{
				pDoc->m_pViewCtrl->UnselectAll(NULL);
			}
		}
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
	}
}

void CNodeTransDlg::CopyNode(_NodeTranslate& NT)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<unsigned int, unsigned int> NKeyArray;
	
	if(pIGM->GetSelectedNodeKeyList(NKeyArray))
	{
		if(NT.m_bIsEqual) // Equal Distance 
		{
//      BOOL TranslateNode_Copy_Unequal(CArray<T_NODE_K, T_NODE_K>& rKey, int nCopy, double ux, double uy, double uz,
//		  CArray<double, double>& arDistance, BOOL bAllowDuplicate, BOOL bCopyData);
			if((pDoc->m_pDataCtrl)->TranslateNode_Copy_Equal(NKeyArray,NT.m_NTimes,NT.m_EQ_WX, 
															NT.m_EQ_WY, NT.m_EQ_WZ,NT.m_bDupNode,NT.m_bAttrCopy,NT.m_bIntersect))
			{
				pDoc->m_pViewCtrl->UnselectAll(NULL);
			}
		}
		else
		{// Unequal Distance
			if((pDoc->m_pDataCtrl)->TranslateNode_Copy_Unequal(NKeyArray,NT.m_NTimes,NT.m_UEQ_WX,
																								 NT.m_UEQ_WY, NT.m_UEQ_WZ,NT.m_UEQ_DistList,
																								 NT.m_bDupNode,NT.m_bAttrCopy,NT.m_bIntersect))
			{
				pDoc->m_pViewCtrl->UnselectAll(NULL);
			}
		}
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
	}
}

void CNodeTransDlg::Execute()
{
	_NodeTranslate NT;

	if(GetParameter(NT))
	{
		//CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
		//I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
		if(NT.m_bIsCopy == FALSE) // Move
		{
			MoveNode(NT);
		}
		else // Copy
		{
			CopyNode(NT); 
		}
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Parameter_Error____));
	}
}

void CNodeTransDlg::OnTmCopynodedata() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_COPYATTR_NODE,0));
}

void CNodeTransDlg::OnTmTolerance() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_MERGETOL,0));
}

void CNodeTransDlg::OnTmIntsectTol() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_DB_INTERSECTTOL,0));
}
