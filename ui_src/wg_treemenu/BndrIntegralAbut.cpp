// BndrIntegralAbut.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrIntegralAbut.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_SpecialChar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrIntegralAbut dialog


CBndrIntegralAbut::CBndrIntegralAbut(SpringAbutment* pData,CWnd* pParent /*=NULL*/)
	: CDlgChild(CBndrIntegralAbut::IDD, pParent)
{
	m_pData=pData;
	//{{AFX_DATA_INIT(CBndrIntegralAbut)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBndrIntegralAbut::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrIntegralAbut)
	DDX_Control(pDX, IDC_TM_SPDIR_EDT, m_edtDirection);
	DDX_Control(pDX, IDC_TM_SPDIR_STC, m_stcDirection);
	DDX_Control(pDX, IDC_TM_ELIST_STC, m_stcEList);
	DDX_Control(pDX, IDC_TM_AWIDTH_UNT, m_untWidth);
	DDX_Control(pDX, IDC_TM_AHEIGHT_UNT, m_untHeight); 
	//DDX_Control(pDX, IDC_TM_GLEVEL_UNT, m_untGround);
	DDX_Control(pDX, IDC_TM_DLENGTH_UNT, m_untDeck);
	DDX_Control(pDX, IDC_TM_TEMP_UNT, m_untTemp);
	DDX_Control(pDX, IDC_TM_MATERIAL_UNT, m_untThermal); 
	DDX_Control(pDX, IDC_TM_FWIDTH_UNT, m_untFWidth);
	DDX_Control(pDX, IDC_TM_FPRESS_UNT, m_untFPress);
	//DDX_Control(pDX, IDC_TM_GLEVEL_EDT, m_edtGround);
	DDX_Control(pDX, IDC_TM_AHEIGHT_EDT, m_edtHeight); 
	DDX_Control(pDX, IDC_TM_AWIDTH_EDT, m_edtWidth);
	DDX_Control(pDX, IDC_TM_DLENGTH_EDT, m_edtDeck);
	DDX_Control(pDX, IDC_TM_VOID_EDT, m_edtVoid);
	DDX_Control(pDX, IDC_TM_GS_EDT, m_edtGS); 
	DDX_Control(pDX, IDC_TM_CFACTOR_EDT, m_edtCycle); 
	//DDX_Control(pDX, IDC_TM_EARTH_EDT, m_edtEarth); 
	DDX_Control(pDX, IDC_TM_TEMP_EDT, m_edtTemp);
	DDX_Control(pDX, IDC_TM_MATERIAL_EDT, m_edtThermal); 
	DDX_Control(pDX, IDC_TM_FWIDTH_EDT, m_edtFWidth);
	DDX_Control(pDX, IDC_TM_FPRESS_EDT, m_edtFPress);
	DDX_Control(pDX, IDC_TM_SPDIR_CMB, m_cmbDirection);
	DDX_Control(pDX, IDC_TM_ETYPE_CMB, m_cmbElemType);
	DDX_Control(pDX, IDC_TM_FACESEL_CMB, m_cmbFace);
	DDX_Control(pDX, IDC_TM_MATERIAL_CMB, m_cmbMatl);
	DDX_Control(pDX, IDC_TM_ROTATION_CMB, m_cmbRotation);
	DDX_Control(pDX, IDC_TM_ELIST_EDT, m_edtEList); 
	DDX_Control(pDX, IDC_TM_NLIST_EDT, m_edtNList); 
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBndrIntegralAbut, CDlgChild)
	//{{AFX_MSG_MAP(CBndrIntegralAbut)
	ON_CBN_SELCHANGE(IDC_TM_ETYPE_CMB, OnSelchangeTmElemTypeCombo)
	ON_CBN_SELCHANGE(IDC_TM_FACESEL_CMB, OnSelchangeTmBndrSospTypeSolidCbo)
	ON_CBN_SELCHANGE(IDC_TM_MATERIAL_CMB, OnSelchangeTmBndrSospMaterialCbo)
	ON_EN_SETFOCUS(IDC_TM_NLIST_EDT, OnSetfocusNList)
	ON_EN_SETFOCUS(IDC_TM_ELIST_EDT, OnSetfocusEList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrIntegralAbut message handlers

BOOL CBndrIntegralAbut::OnInitDialog() 
{
	CDlgChild::OnInitDialog();
	m_cmbElemType.SetCurSel(1);
	CheckMode(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrIntegralAbut::InitSubCtrl()
{
	// special character
	CString strAlpha = D_CH_ALPHA;	
	GetDlgItem(IDC_TM_MATERIAL_STC)->SetWindowText(strAlpha);

	m_edtEList.Connect(SC_ID_ELEM, &m_edtEList);
	//m_edtNList.Connect(SC_ID_NODE, &m_edtNList); // start disconnected

	m_untWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untHeight.SetUnitType(D_UNITSYS_BASE_LENGTH);
	//m_untGround.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDeck.SetUnitType(D_UNITSYS_BASE_LENGTH); 
	m_untTemp.SetUnitType(D_UNITSYS_BASE_TEMPER); 
	m_untThermal.SetUnitType(D_UNITSYS_BASE_THERMAL); 
	m_untFWidth.SetUnitType(D_UNITSYS_BASE_LENGTH); 
	m_untFPress.SetUnitType(D_UNITSYS_BASE_PRESSURE); 
	
	m_edtGS.SetUnitType(0);
	m_edtGS.SetEditUnit(2.65);
	m_edtVoid.SetUnitType(0);
	m_edtCycle.SetUnitType(0);
	m_edtCycle.SetEditUnit(2);
	//m_edtEarth.SetUnitType(0);
	//m_edtEarth.SetEditUnit(0.4);
	m_edtTemp.SetUnitType(D_UNITSYS_BASE_TEMPER);
	m_edtThermal.SetUnitType(D_UNITSYS_BASE_THERMAL); 
	m_edtFWidth.SetUnitType(D_UNITSYS_BASE_LENGTH); 
	m_edtFPress.SetUnitType(D_UNITSYS_BASE_PRESSURE);

	//m_edtGround.SetAttWcsPos();
	//m_edtGround.SetModeToUse(MOUSEEDIT_USE_POS_Z_ONLY);  // Default : Global (-Z)
	//m_edtGround.SetWindowText("0");

	m_edtDeck.SetAttUcsDistance();
	m_edtDeck.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_edtDeck.SetDistValueMode(TRUE);
	m_edtDeck.SetNextLink(NULL);
	m_edtDeck.SetWindowText(_T("0"));

	m_edtHeight.SetModeToUse(MOUSEEDIT_USE_GET_SWDISTANCE_DATA);
	m_edtHeight.SetAttWcsDistance();
	m_edtHeight.SetSWDistZMode();
	m_edtHeight.SetWindowText(_T("0"));
	
	m_edtWidth.SetAttUcsDistance();
	m_edtWidth.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_edtWidth.SetDistValueMode(TRUE);
	m_edtWidth.SetNextLink(NULL);
	m_edtWidth.SetWindowText(_T("0"));

	m_edtDirection.SetAttUcsDistance();
	m_edtDirection.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_edtDirection.ShowWindow(SW_HIDE); //start hidden

	m_cmbElemType.ResetContent();
	m_cmbElemType.AddString(_LS(IDS_TM_SSPR_Frame)     );    m_cmbElemType.SetItemData(0,0);
	m_cmbElemType.AddString(_LS(IDS_TM_SSPR_Planar)    );    m_cmbElemType.SetItemData(1,1);
	m_cmbElemType.AddString(_LS(IDS_TM_SSPR_Solid_Face_));    m_cmbElemType.SetItemData(2,2);
	m_cmbElemType.AddString(_LS(IDS_TM_SSPR_Solid_Node_));    m_cmbElemType.SetItemData(3,3);
	m_cmbElemType.SetCurSel(1);

	m_cmbRotation.ResetContent();
	m_cmbRotation.AddString(_LS(IDS_TM_ABUT_RX));  m_cmbRotation.SetItemData(0,0);
	m_cmbRotation.AddString(_LS(IDS_TM_ABUT_RY));  m_cmbRotation.SetItemData(1,1);
	m_cmbRotation.SetCurSel(1);

	// Element Type의 Face ComboBox 초기화.
	CString strElemType[] = {_LS(IDS_WG_TREEMENU_Face__1),_LS(IDS_WG_TREEMENU_Face__2),_LS(IDS_WG_TREEMENU_Face__3),_LS(IDS_WG_TREEMENU_Face__4),_LS(IDS_WG_TREEMENU_Face__5),_LS(IDS_WG_TREEMENU_Face__6)};
	for(int i=0; i<6; i++)
		m_cmbFace.AddString(strElemType[i]);
	m_cmbFace.SetCurSel(0);

	// Direction ComboBox 초기화.
	CString strDirection[] = {_LS(IDS_WG_TREEMENU_Normal___),_LS(IDS_WG_TREEMENU_Normal____MSG1) };
	for(int i = 0; i< 2; i++)
		m_cmbDirection.AddString(strDirection[i]);
	m_cmbDirection.SetCurSel(0);

	MakeMatlCombo();
	m_edtThermal.SetEditUnit(0);
	
}
BOOL CBndrIntegralAbut::GetParameter()
{
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  

	m_pData->nElType = m_cmbElemType.GetCurSel();
	m_pData->nFace = m_cmbFace.GetCurSel()+1;
	m_pData->nDirection = m_cmbDirection.GetCurSel();
	
	// get vector input
	CString strVector;
	double ux, uy, uz;
	if(m_pData->nElType == 0)
	{
		m_edtDirection.GetWindowText(strVector);
		if (!GetPosition(strVector, ux, uy, uz))
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_vector_));
			return FALSE;
		}
		pIGM->GetWcsVector(ux, uy, uz, m_pData->aVector[0], m_pData->aVector[1], m_pData->aVector[2]);
	} 

	m_edtEList.GetWindowText(m_pData->strEList); // Element/Nodes for Abutment
	m_edtNList.GetWindowText(m_pData->strNList); // Nodes for Footing
	
	/*
	if (!CFormulaEdit::GetEditValue(&m_edtGround, m_pData->dGround))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___Invalid_reference_level_));
		return FALSE;
	}*/
	CFormulaEdit::GetEditValue(&m_edtHeight, m_pData->dHeight);
	CFormulaEdit::GetEditValue(&m_edtWidth, m_pData->dWidth);
	if(!CFormulaEdit::GetEditValue(&m_edtDeck, m_pData->dDeck)) return FALSE;

	CFormulaEdit::GetEditValue(&m_edtVoid, m_pData->dVoid);
	CFormulaEdit::GetEditValue(&m_edtGS, m_pData->dGS);
	CFormulaEdit::GetEditValue(&m_edtCycle, m_pData->dCycle);
	//CFormulaEdit::GetEditValue(&m_edtEarth, m_pData->dEarth);
	
	CFormulaEdit::GetEditValue(&m_edtTemp, m_pData->dTemp);
	if(!CFormulaEdit::GetEditValue(&m_edtThermal, m_pData->dThermal)) return FALSE;
	
	m_pData->MatlK = m_cmbMatl.GetItemData(m_cmbMatl.GetCurSel()); 

	CFormulaEdit::GetEditValue(&m_edtFWidth, m_pData->dFWidth);
	CFormulaEdit::GetEditValue(&m_edtFPress, m_pData->dFPress);
	m_pData->nRotation = m_cmbRotation.GetCurSel();

	return TRUE;
}

BOOL CBndrIntegralAbut::GetDirectionVector(CString &strVector, double& Ux, double& Uy, double& Uz)
{
	return GetEQDistance(strVector,Ux,Uy,Uz);
}

BOOL CBndrIntegralAbut::GetEQDistance(CString& EQStr, double& Dx, double& Dy, double& Dz)
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

void CBndrIntegralAbut::CheckMode(BOOL bCheck)
{
	CDBDoc* pDoc;
	if(bCheck)
	{
		UpdateData(TRUE);
		pDoc = CDBDoc::GetDocPoint();

		int nSel = m_cmbElemType.GetCurSel();
		
		m_cmbFace.EnableWindow(nSel == 2);
		if(pDoc)
		{
			if(nSel >= 2) // Solid
			{
				int PresInputMode;
				if(nSel == 2 && m_edtEList.m_bConnected) // Solid(Face) 이면서 Element Select
				{
					PresInputMode = 8; 
				}
				else // Solid(Node) 일 경우,  Solid(face) 이면서 Node Select
				{
					PresInputMode = 10; 
				}

				 pDoc->m_pViewCtrl->SetPressureInputMode(TRUE);
			   pDoc->m_pViewCtrl->SetPressureInputParam(PresInputMode, m_cmbFace.GetCurSel());	
			}
			else
			{
				pDoc->m_pViewCtrl->SetPressureInputMode(FALSE);
				pDoc->m_pViewCtrl->SetPressureInputParam(-1,-1);	
			}
		}
	}
	else // bCheck == FALSE
	{
		pDoc = CDBDoc::GetDocPoint();
		if(pDoc)
		{
		  pDoc->m_pViewCtrl->SetPressureInputMode(FALSE);
			pDoc->m_pViewCtrl->SetPressureInputParam(-1,-1);	
		}
	}
}

void CBndrIntegralAbut::OnSelchangeTmElemTypeCombo() 
{
	SwitchOnSolidNode();
	UseVectorOnFrame();
	ChangeDirCombo();

	CheckMode(TRUE);
}

void CBndrIntegralAbut::SwitchOnSolidNode() 
{
	CString strBlank = _T("");
	
	int nSel = m_cmbElemType.GetCurSel();
	m_edtNList.Disconnect();
	m_edtEList.Disconnect();
	
	if ( nSel == 3 ) // solid(node)
	{
		m_edtEList.Connect(SC_ID_NODE, &m_edtEList);
		m_stcEList.SetWindowText(_LS(IDS_TM_ABUT_NLIST));
	}
	else
	{
		m_edtEList.Connect(SC_ID_ELEM, &m_edtEList);
		m_stcEList.SetWindowText(_LS(IDS_TM_ABUT_ELIST));
	}
}

void CBndrIntegralAbut::UseVectorOnFrame() 
{
	int nSel = m_cmbElemType.GetCurSel();
	if (nSel == 0) // frame
	{
		m_stcDirection.SetWindowText(_LS(IDS_TM_0524Vector));
		m_edtDirection.ShowWindow(SW_SHOW);
		m_cmbDirection.ShowWindow(SW_HIDE);
	}
	else
	{
		m_stcDirection.SetWindowText(_LS(IDS_TM_DIRECTION));
		m_edtDirection.ShowWindow(SW_HIDE);
		m_cmbDirection.ShowWindow(SW_SHOW);
	}
}

// Solid 선택시 Direction ComboBox의 선택메뉴를 변경
void CBndrIntegralAbut::ChangeDirCombo() 
{
	int nSel = m_cmbElemType.GetCurSel();
	if (nSel >= 2) // solid
	{
		m_cmbDirection.ResetContent();
		m_cmbDirection.AddString(_LS(IDS_TM_0524Normal));
		m_cmbDirection.SetCurSel(0);
	}
	else
	{
		m_cmbDirection.ResetContent();
		CString strDirection[] = {_LS(IDS_WG_TREEMENU_Normal___),_LS(IDS_WG_TREEMENU_Normal____MSG1) };
		for(int i = 0; i< 2; i++)
			m_cmbDirection.AddString(strDirection[i]);
		m_cmbDirection.SetCurSel(0);
	}
}

void CBndrIntegralAbut::OnSelchangeTmBndrSospTypeSolidCbo() 
{
	CheckMode(TRUE);	
}

void CBndrIntegralAbut::OnSelchangeTmBndrSospMaterialCbo() 
{
	T_MATL_K MatlK;
	T_MATL_D MatlData;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	MatlK = (UINT)m_cmbMatl.GetItemData(m_cmbMatl.GetCurSel());
	if(MatlK==0)
	{
		m_edtThermal.SetReadOnly(FALSE);
	}
	else
	{
		m_edtThermal.SetReadOnly(TRUE);
		if(!pDoc->m_pAttrCtrl->GetMatl(MatlK,MatlData)) ASSERT(0);
		double a = (MatlData.Type==_T("SRC")) ? MatlData.Data2.Analysis.Thermal : MatlData.Data1.Analysis.Thermal;
		m_edtThermal.SetEditUnit(a);
	}
}

void CBndrIntegralAbut::PostNcDestroy() 
{
	CheckMode(FALSE);
	CDlgChild::PostNcDestroy();
}

void CBndrIntegralAbut::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	POSITION pos = pViewBuff->GetStartBuffer();
	T_UDRD_BUFFER buffer_ur;
	BOOL bMATL = FALSE;
	while(pos)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case UR_MATL_ADD:
		case UR_MATL_DEL:
		case UR_MATL_MFD:
		case UR_MATL_MFS:
			bMATL = TRUE;
			goto Exit;
			break;
		default:
			break;
		}
	}
Exit:
	if(bMATL)
		MakeMatlCombo();
}

void CBndrIntegralAbut::MakeMatlCombo()
{
	CArray<T_MATL_K,T_MATL_K> arMatl;
	T_MATL_D MatlData;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetMatlKeyList(arMatl);
	m_cmbMatl.ResetContent();
	m_cmbMatl.AddString(_LS(IDS_TMWK_NONE)); m_cmbMatl.SetItemData(0,0);
	for(int i=0;i<arMatl.GetSize();i++)
	{
		if(!pDoc->m_pAttrCtrl->GetMatl(arMatl[i],MatlData)) ASSERT(0);
		m_cmbMatl.AddString(MatlData.Name); m_cmbMatl.SetItemData(i+1,arMatl[i]);
	}
	m_cmbMatl.SetCurSel(0);
}

//Abutmemt 요소와 Footing 요소 선택간의 전환
void CBndrIntegralAbut::OnSetfocusNList() 
{
	m_edtEList.Disconnect();
	
	// text preserve
	CString strPreviousText = _T("");
	m_edtNList.GetWindowText(strPreviousText);
	m_edtNList.Connect(SC_ID_NODE, &m_edtNList);
	m_edtNList.SetWindowText(strPreviousText);
	
	// model select
	CString strBlank = _T("");
	m_edtNList.SelectByStr(strBlank);
	m_edtNList.SelectByStr(strPreviousText);
	CheckMode(TRUE);
}

void CBndrIntegralAbut::OnSetfocusEList() 
{
	m_edtNList.Disconnect();
	
	// text preserve
	CString strPreviousText = _T("");
	m_edtEList.GetWindowText(strPreviousText);

	int nSel = m_cmbElemType.GetCurSel();
	if (nSel == 3) //Solid(node)
		m_edtEList.Connect(SC_ID_NODE, &m_edtEList);
	else
		m_edtEList.Connect(SC_ID_ELEM, &m_edtEList);
	m_edtEList.SetWindowText(strPreviousText);
	
	// model select
	CString strBlank = _T("");
	m_edtEList.SelectByStr(strBlank);
	m_edtEList.SelectByStr(strPreviousText);
	CheckMode(TRUE);
}

BOOL CBndrIntegralAbut::GetPosition(CString& strPos, double& Px, double& Py, double& Pz)
{
	MyParser Parser;
	if(Parser.ParsingPositionArg(strPos))
	{
		Parser.GetPositionData(Px,Py,Pz);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

