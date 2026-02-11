// BndrSkewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrSkewDlg.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_cmd\wg_cmdAll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrSkewDlg dialog


CBndrSkewDlg::CBndrSkewDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrSkewDlg::IDD, pParent)
{

	//{{AFX_DATA_INIT(CBndrSkewDlg)
	//}}AFX_DATA_INIT
	m_aCtrlOption.Add(IDC_TM_OPT_ADD);
	m_aCtrlOption.Add(IDC_TM_OPT_DELETE);
		
	m_aCtrlDisable.Add(IDC_TM_INPUT_METHOD_CMB);
	m_aCtrlDisable.Add(IDC_TM_TITLE_DIRECTION_TITLE);
	m_aCtrlDisable.Add(IDC_TM_DIRECTION_CMB);
	m_aCtrlDisable.Add(IDC_TM_REF_PT_RDO);
	m_aCtrlDisable.Add(IDC_TM_GLOBAL_DIR_RDO);
	m_aCtrlDisable.Add(IDC_TM_LINEVECTOR_P0_TITLE);
	m_aCtrlDisable.Add(IDC_TM_LINEVECTOR_P1_TITLE);
	m_aCtrlDisable.Add(IDC_TM_LINE_VECTOR_P0_EDT);
	m_aCtrlDisable.Add(IDC_TM_LINE_VECTOR_P1_EDT);
	m_aCtrlDisable.Add(IDC_TM_LINE_VECTOR_P0_UNIT);
	m_aCtrlDisable.Add(IDC_TM_LINE_VECTOR_P1_UNIT);
	m_aCtrlDisable.Add(IDC_TM_REF_GLOBAL_P1_CMB);
	m_aCtrlDisable.Add(IDC_GROUP_LINEVECTER_REF);

	m_aCtrlDisable.Add(IDC_TM_ANGLE_X_TITLE);
	m_aCtrlDisable.Add(IDC_TM_ANGLE_X_COBX);
	m_aCtrlDisable.Add(IDC_TM_ANGLE_X_UNIT);
	m_aCtrlDisable.Add(IDC_TM_ANGLE_Y_TITLE);
	m_aCtrlDisable.Add(IDC_TM_ANGLE_Y_COBX);
	m_aCtrlDisable.Add(IDC_TM_ANGLE_Y_UNIT);
	m_aCtrlDisable.Add(IDC_TM_ANGLE_Z_TITLE);
	m_aCtrlDisable.Add(IDC_TM_ANGLE_Z_COBX);
	m_aCtrlDisable.Add(IDC_TM_ANGLE_Z_UNIT);
	m_aCtrlDisable.Add(IDC_TM_3PT_P0_TITLE);
	m_aCtrlDisable.Add(IDC_TM_3PT_P0_EDIT);
	m_aCtrlDisable.Add(IDC_TM_3PT_P0_UNIT);
	m_aCtrlDisable.Add(IDC_TM_3PT_P1_TITLE);
	m_aCtrlDisable.Add(IDC_TM_3PT_P1_EDIT);
	m_aCtrlDisable.Add(IDC_TM_3PT_P1_UNIT);
	m_aCtrlDisable.Add(IDC_TM_3PT_P2_TITLE);
	m_aCtrlDisable.Add(IDC_TM_3PT_P2_EDIT);
	m_aCtrlDisable.Add(IDC_TM_3PT_P2_UNIT);
	m_aCtrlDisable.Add(IDC_TM_VECTOR_V1_TITLE);
	m_aCtrlDisable.Add(IDC_TM_VECTOR_V1_EDIT);
	m_aCtrlDisable.Add(IDC_TM_VECTOR_V2_TITLE);
	m_aCtrlDisable.Add(IDC_TM_VECTOR_V2_EDIT);
	
	m_aCtrlAngle.Add(IDC_TM_ANGLE_X_TITLE);
	m_aCtrlAngle.Add(IDC_TM_ANGLE_X_COBX);
	m_aCtrlAngle.Add(IDC_TM_ANGLE_X_UNIT);
	m_aCtrlAngle.Add(IDC_TM_ANGLE_Y_TITLE);
	m_aCtrlAngle.Add(IDC_TM_ANGLE_Y_COBX);
	m_aCtrlAngle.Add(IDC_TM_ANGLE_Y_UNIT);
	m_aCtrlAngle.Add(IDC_TM_ANGLE_Z_TITLE);
	m_aCtrlAngle.Add(IDC_TM_ANGLE_Z_COBX);
	m_aCtrlAngle.Add(IDC_TM_ANGLE_Z_UNIT);

	m_aCtrl3Points.Add(IDC_TM_3PT_P0_TITLE);
	m_aCtrl3Points.Add(IDC_TM_3PT_P0_EDIT);
	m_aCtrl3Points.Add(IDC_TM_3PT_P0_UNIT);
	m_aCtrl3Points.Add(IDC_TM_3PT_P1_TITLE);
	m_aCtrl3Points.Add(IDC_TM_3PT_P1_EDIT);
	m_aCtrl3Points.Add(IDC_TM_3PT_P1_UNIT);
	m_aCtrl3Points.Add(IDC_TM_3PT_P2_TITLE);
	m_aCtrl3Points.Add(IDC_TM_3PT_P2_EDIT);
	m_aCtrl3Points.Add(IDC_TM_3PT_P2_UNIT);

	m_aCtrlVector.Add(IDC_TM_VECTOR_V1_TITLE);
	m_aCtrlVector.Add(IDC_TM_VECTOR_V1_EDIT);
	m_aCtrlVector.Add(IDC_TM_VECTOR_V2_TITLE);
	m_aCtrlVector.Add(IDC_TM_VECTOR_V2_EDIT);

	m_aCtrlLineVector.Add(IDC_TM_TITLE_DIRECTION_TITLE);
	m_aCtrlLineVector.Add(IDC_TM_DIRECTION_CMB);
	m_aCtrlLineVector.Add(IDC_TM_REF_PT_RDO);
	m_aCtrlLineVector.Add(IDC_TM_GLOBAL_DIR_RDO);
	m_aCtrlLineVector.Add(IDC_TM_LINEVECTOR_P0_TITLE);
	m_aCtrlLineVector.Add(IDC_TM_LINE_VECTOR_P0_EDT);
	m_aCtrlLineVector.Add(IDC_TM_LINE_VECTOR_P0_UNIT);
	m_aCtrlLineVector.Add(IDC_TM_LINEVECTOR_P1_TITLE);		
	m_aCtrlLineVector.Add(IDC_TM_LINE_VECTOR_P1_EDT);	
	m_aCtrlLineVector.Add(IDC_TM_LINE_VECTOR_P1_UNIT);
	m_aCtrlLineVector.Add(IDC_TM_REF_GLOBAL_P1_CMB);
	m_aCtrlLineVector.Add(IDC_GROUP_LINEVECTER_REF);	

	m_aCtrlLineVectorDirection.Add(IDC_TM_DIRECTION_CMB);
	m_aCtrlLineVectorDirection.Add(IDC_TM_TITLE_DIRECTION_TITLE);

	m_aCtrlRefLineRdo.Add(IDC_TM_REF_PT_RDO);
	m_aCtrlRefLineRdo.Add(IDC_TM_GLOBAL_DIR_RDO);	

	m_aGlobalDirectionGroup.Add(IDC_TM_REF_GLOBAL_P1_CMB); 

	m_aRefPointGroup.Add(IDC_TM_LINE_VECTOR_P1_EDT); 
	m_aRefPointGroup.Add(IDC_TM_LINE_VECTOR_P1_UNIT);
}

CBndrSkewDlg::~CBndrSkewDlg()
{
	
}

void CBndrSkewDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);

	UINT aAngleID[] = {IDC_TM_ANGLE_X_COBX, IDC_TM_ANGLE_Y_COBX, IDC_TM_ANGLE_Z_COBX};
	UINT a3DPoint[][3] = {{IDC_TM_3PT_P0_EDIT, IDC_TM_3PT_P1_EDIT, IDC_TM_3PT_P2_EDIT},{IDC_TM_3PT_P0_UNIT, IDC_TM_3PT_P1_UNIT, IDC_TM_3PT_P2_UNIT}};  	
	for (int i = 0; i < 3; i++)
	{
		DDX_Control(pDX, aAngleID[i], AngleCobx[i]);
		DDX_Control(pDX, a3DPoint[0][i], Pt3Edit[i]);
		DDX_Control(pDX, a3DPoint[1][i], Pt3Unit[i]);
	}

	UINT aVector[] = {IDC_TM_VECTOR_V1_EDIT, IDC_TM_VECTOR_V2_EDIT};
	for(int i=0; i < 2; i++)
		DDX_Control(pDX, aVector[i], VtEdit[i]);

	UINT aLineVector[] = {IDC_TM_LINE_VECTOR_P0_EDT, IDC_TM_LINE_VECTOR_P1_EDT};
	UINT aLineVectorUnit[] = {IDC_TM_LINE_VECTOR_P0_UNIT, IDC_TM_LINE_VECTOR_P1_UNIT};
	for(int i = 0; i < 2; i++)
	{
		DDX_Control(pDX, aLineVector[i], m_LineVectorEdt[i]);
		DDX_Control(pDX, aLineVectorUnit[i], m_LineVectorUnit[i]);		
	}

	DDX_Control(pDX, IDC_TM_INPUT_METHOD_CMB	, m_CmbInputMethod);
	DDX_Control(pDX, IDC_TM_DIRECTION_CMB			, m_CmbLineVectorDirection);
	DDX_Control(pDX, IDC_TM_REF_GLOBAL_P1_CMB	, m_CmbRefLineP1);	
	DDX_Control(pDX, IDC_TM_PICTURE, m_wndPicture);
}

void CBndrSkewDlg::AlignControl(int nMethod)
{	
	CRect rRef;
	CRect rMove;
	int nDistY;

	GetDlgItem(m_aCtrlAngle[0])->GetWindowRect(rRef);
	if(nMethod == 1)
	{
		// 3points를 angle과 동일 위치
		GetDlgItem(m_aCtrl3Points[0])->GetWindowRect(rMove);
		nDistY = rRef.top - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrl3Points, nDistY);
	}
	else if(nMethod == 2)
	{
	// vector를 angle과 동일 위치
	GetDlgItem(m_aCtrlVector[0])->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlVector, nDistY);
	}
	else if(nMethod == 3)
	{
		// LineVector를 angle과 동일 위치
		GetDlgItem(m_aCtrlLineVectorDirection[0])->GetWindowRect(rMove);
		nDistY = rRef.top - rMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrlLineVectorDirection, nDistY);
	}
	
	GetDlgItem(IDC_TM_LINE_VECTOR_P1_EDT)->GetWindowRect(rRef);	
	GetDlgItem(IDC_TM_REF_GLOBAL_P1_CMB)->GetWindowRect(rMove);
	nDistY = rRef.top - rMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aGlobalDirectionGroup, nDistY);	
}

void CBndrSkewDlg::ChangeBitmap()
{
	//Need to UpdateData()??
	CString aBitmapPath[] = {
	_T("SVG\\Illustration\\Dialog\\tm_skew1.svg"),
	_T("SVG\\Illustration\\Dialog\\tm_skew2.svg"),
	_T("SVG\\Illustration\\Dialog\\tm_skew3.svg"),
	_T("SVG\\Illustration\\Dialog\\tm_skew4.svg")
	};
	
	int nMethod = m_CmbInputMethod.GetCurSel();  
	
	m_wndPicture.SetImage(aBitmapPath[nMethod]);
}

BEGIN_MESSAGE_MAP(CBndrSkewDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CBndrSkewDlg)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)

	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnChangeOption)  
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)

	ON_BN_CLICKED(IDC_TM_REF_PT_RDO, OnChangeRefOption)
	ON_BN_CLICKED(IDC_TM_GLOBAL_DIR_RDO, OnChangeRefOption)

	ON_CBN_SELCHANGE(IDC_TM_INPUT_METHOD_CMB, OnChangeMethod)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrSkewDlg message handlers

void CBndrSkewDlg::InitGlobalDirectionCmb()
{
	m_CmbRefLineP1.ResetContent();
	m_CmbRefLineP1.AddString(_LS(IDS_TM_GLOBAL_X));
	m_CmbRefLineP1.AddString(_LS(IDS_TM_GLOBAL_Y));
	m_CmbRefLineP1.AddString(_LS(IDS_TM_GLOBAL_Z));
	m_CmbRefLineP1.SetCurSel(0);
}

void CBndrSkewDlg::InitDirectionCmb()
{
	m_CmbLineVectorDirection.ResetContent();
	m_CmbLineVectorDirection.AddString(_LS(IDS_TM_LOCAL_X));
	m_CmbLineVectorDirection.AddString(_LS(IDS_TM_LOCAL_Y));
	m_CmbLineVectorDirection.AddString(_LS(IDS_TM_LOCAL_Z));
	m_CmbLineVectorDirection.SetCurSel(0);
}

void CBndrSkewDlg::InitInputMethodCmb()
{
	m_CmbInputMethod.ResetContent();
	int nIdx = m_CmbInputMethod.AddString(_LS(IDS_TM_LOCAL_ANGLE));
	nIdx = m_CmbInputMethod.AddString(_LS(IDS_TM_LOCAL_3PT));
	nIdx = m_CmbInputMethod.AddString(_LS(IDS_TM_LOCAL_VECTOR));
	nIdx = m_CmbInputMethod.AddString(_LS(IDS_TM_LOCAL_LINE_VECTOR));
	m_CmbInputMethod.SetCurSel(0);
}

BOOL CBndrSkewDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();
	
	InitInputMethodCmb();	InitGlobalDirectionCmb();	InitDirectionCmb();	
	int nMethod = m_CmbInputMethod.GetCurSel();	  

	AlignControl(nMethod);
	int nOpt = 0, nRefLineDefaultValue = 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption,  nOpt);	
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRefLineRdo, nRefLineDefaultValue);

	
	CDlgUtil::CtrlShowHide(this, m_aCtrlAngle, nMethod == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrl3Points, nMethod == 1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlVector, nMethod == 2);
	CDlgUtil::CtrlShowHide(this, m_aCtrlLineVector, nMethod == 3);

	CDlgUtil::CtrlShowHide(this, m_aRefPointGroup, nRefLineDefaultValue == 0 && nMethod == 3);
	CDlgUtil::CtrlShowHide(this, m_aGlobalDirectionGroup, nRefLineDefaultValue == 1 && nMethod == 3);

	ChangeBitmap();

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDisable, nOpt==0);
	InitDirectionCmb();

	for (int i = 0; i < 3; i++)
	{
		AngleCobx[i].ResetContent();
		AngleCobx[i].AddString(_T("0"));
		AngleCobx[i].AddString(_LS(IDS_WG_TREEMENU_45));
		AngleCobx[i].AddString(_LS(IDS_WG_TREEMENU_90));
		AngleCobx[i].AddString(_LS(IDS_WG_TREEMENU_180));
		AngleCobx[i].SetCurSel(0);

		Pt3Edit[i].SetAttUcsPos();
		Pt3Edit[i].SetModeToUse(MOUSEEDIT_USE_SET_POS);
		Pt3Unit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}
	Pt3Edit[0].SetNextLink(&Pt3Edit[1]);
	Pt3Edit[1].SetNextLink(&Pt3Edit[2]);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrSkewDlg::Execute() 
{
	// TODO: Add your control notification handler code here
	CArray<T_NODE_K, T_NODE_K> aSelKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedNodeKeyList(aSelKey);
	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
		return;
	}

	T_SKEW_D data;
	data.Initialize();
	int nOption;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOption);
	if (nOption == 0)
	{    
		int nMethod = m_CmbInputMethod.GetCurSel();      
		data.nType = nMethod+1;
		if (nMethod == 0)
		{
			for (int i = 0; i < 3; i++)
				if (!CFormulaEdit::GetEditValue(&AngleCobx[i], data.dAngle[i]))
				{
					GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_angle_value_));
					return;
				}
		}
		else if (nMethod == 1)
		{
			double ux, uy, uz;
			CString csPos;
			for (int i = 0; i < 3; i++)
			{
				Pt3Edit[i].GetWindowText(csPos);
				if (!GetPosition(csPos, ux, uy, uz))
				{
					GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_point_coordinate_));
					return;
				}
				pIGM->GetWcsPos(ux, uy, uz, data.dPoints[i][0], data.dPoints[i][1], data.dPoints[i][2]);
			}
		}
		else if(nMethod == 2)
		{
			CString csVector;
			for (int i = 0; i < 2; i++)
			{
				double ux, uy, uz;
				VtEdit[i].GetWindowText(csVector);
				if (!GetPosition(csVector, ux, uy, uz))
				{
					GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_vector_));
					return;
				}
				pIGM->GetWcsVector(ux, uy, uz, data.dVector[i][0], data.dVector[i][1], data.dVector[i][2]);
			}
		}
		else if(nMethod == 3)
		{
			int nRefOpt;
			CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRefLineRdo, nRefOpt);			
			if(nRefOpt == 0)
			{
				for(int i=0; i<2; i++)
				{
					double ux, uy, uz;
					CString csPos;
					m_LineVectorEdt[i].GetWindowText(csPos);
					if (!GetPosition(csPos, ux, uy, uz))
					{
						GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_point_coordinate_));
						return;
					}
					pIGM->GetWcsPos(ux, uy, uz, data.dPoints[i][0], data.dPoints[i][1], data.dPoints[i][2]);				
				}
			}												
			else
			{				
				double ux, uy, uz;
				CString csPos;
				m_LineVectorEdt[0].GetWindowText(csPos);
				if (!GetPosition(csPos, ux, uy, uz))
				{
					GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_point_coordinate_));
					return;
				}
				pIGM->GetWcsPos(ux, uy, uz, data.dPoints[0][0], data.dPoints[0][1], data.dPoints[0][2]);				

				int nSel = m_CmbRefLineP1.GetCurSel();
				if(nSel < 0 || nSel > 2) {ASSERT(0); return;}								

				data.dPoints[1][0] = data.dPoints[0][0];
				data.dPoints[1][1] = data.dPoints[0][1];
				data.dPoints[1][2] = data.dPoints[0][2];
				data.dPoints[1][nSel] = data.dPoints[0][nSel] + 1.0;				
			}
			
			data.nRefType = nRefOpt+1;
			data.nGlobalDirection =  m_CmbRefLineP1.GetCurSel();
			data.nLocalDirection  =  m_CmbLineVectorDirection.GetCurSel();
			//PO , P1 과 Local Direction, GlobalDirection만 입력 받고, 계산은 CheckSKew에서 처리한다.
		}
		else 
		{
			ASSERT(0);
			return;
		}
	}

	BOOL bSuccess=FALSE;
	if (nOption == 0) // Add or
		bSuccess = m_pDoc->m_pDataCtrl->AddSkew(aSelKey, data);
	else if (nOption == 1)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelSkew(aSelKey);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CBndrSkewDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CBndrSkewDlg::OnChangeRefOption()
{	
	int nRefLineDefaultValue;
	int nMethod = m_CmbInputMethod.GetCurSel();	  	
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRefLineRdo, nRefLineDefaultValue);
	CDlgUtil::CtrlShowHide(this, m_aRefPointGroup, nRefLineDefaultValue == 0 && nMethod == 3);
	CDlgUtil::CtrlShowHide(this, m_aGlobalDirectionGroup, nRefLineDefaultValue == 1 && nMethod == 3);	

	for (int i = 0; i < 2; i++)
	{
		VtEdit[i].SetAttUcsDistance();
		VtEdit[i].SetModeToUse(MOUSEEDIT_USE_SET_DIST);		

		m_LineVectorEdt[i].SetAttUcsPos();
		m_LineVectorEdt[i].SetModeToUse(MOUSEEDIT_USE_SET_POS);
		m_LineVectorUnit[i].SetUnitType(D_UNITSYS_BASE_LENGTH);
	}

	if(nRefLineDefaultValue == 0)	m_LineVectorEdt[0].SetNextLink(&m_LineVectorEdt[1]);		
}

void CBndrSkewDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	int nOpt;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlOption, nOpt);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDisable, nOpt==0);
}

void CBndrSkewDlg::OnChangeMethod() 
{	
	int nMethod = m_CmbInputMethod.GetCurSel();    
	ChangeMethod(nMethod);
}

void CBndrSkewDlg::ChangeMethod(int nMethod)
{
	AlignControl(nMethod);
	CDlgUtil::CtrlShowHide(this, m_aCtrlAngle, nMethod == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrl3Points, nMethod == 1);
	CDlgUtil::CtrlShowHide(this, m_aCtrlVector, nMethod == 2);
	CDlgUtil::CtrlShowHide(this, m_aCtrlLineVector, nMethod == 3);
	OnChangeRefOption();

	ChangeBitmap();

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->SetModelingModeST(IG_STATE_INITALL);
}

BOOL CBndrSkewDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_SKEW_D data;
	m_pDoc->m_pAttrCtrl->GetSkew(key, data);
	int nMethod = data.nType-1;
	
	m_CmbInputMethod.SetCurSel(nMethod);
	ChangeMethod(nMethod);

	//////////////////////////////////////////////////////////
	CString str;
	if (nMethod == 0)
	{
		for (int i=0; i < 3; i++)
		{
			str.Format(_LS(IDS_WG_TREEMENU__g), data.dAngle[i]);
			AngleCobx[i].SetWindowText(str);
		}
	}
	else if (nMethod == 1)
	{
		CString csPos;
		for (int i=0; i < 3; i++)
		{
			csPos.Format(_LS(IDS_WG_TREEMENU___2lf____2lf____2lf), data.dPoints[i][0], data.dPoints[i][1], data.dPoints[i][2]);
			Pt3Edit[i].SetWindowText(csPos);
		}
	}
	else
	{
		CString csVector;
		for (int i=0; i < 2; i++)
		{
			csVector.Format(_LS(IDS_WG_TREEMENU___2lf____2lf____2lf), data.dVector[i][0], data.dVector[i][1], data.dVector[i][2]);
			VtEdit[i].SetWindowText(csVector);
		}
	}
	//////////////////////////////////////////////////////////

	int nOpt = 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlOption, nOpt);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlDisable, nOpt==0);
	UpdateData(FALSE);
	return TRUE;
}
