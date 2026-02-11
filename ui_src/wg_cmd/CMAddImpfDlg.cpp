// CMMlsrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMAddImpfDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  Truss 1
#define  Beam  2
#define  Plate 3
/////////////////////////////////////////////////////////////////////////////
// CMAddImpfDlg dialog

//IMPLEMENT_DYNCREATE(CMAddImpfDlg, CCMChildBarBase)

CMAddImpfDlg::CMAddImpfDlg()
	: CCMChildBarBase(CMAddImpfDlg::IDD)
{
	//{{AFX_DATA_INIT(CMAddImpfDlg)
	m_pDoc = NULL;
	//}}AFX_DATA_INIT

	m_aCtrlOption.RemoveAll();
	m_aCtrlLaneCheck.RemoveAll();
	m_aCtrlEslImpf.RemoveAll();
	m_aCtrlElemBeamPart.RemoveAll();
	m_aCtrlElemPlatePart.RemoveAll();
	m_aCtrlElemTrussPart.RemoveAll();
	m_aCtrlUserInput.RemoveAll();
	m_aCtrlBeamID.RemoveAll();
	m_aCtrlPlateID.RemoveAll();
	m_aCtrlTrussID.RemoveAll();

	//Add or Del
	m_aCtrlOption.Add(IDC_CMD_OPTION_ADD_RDO);
	m_aCtrlOption.Add(IDC_CMD_OPTION_DELETE_RDO);

	//Line Lane or Surface Lane
	m_aCtrlLaneCheck.Add(IDC_CMD_OPTION_LINE_RDO);
	m_aCtrlLaneCheck.Add(IDC_CMD_OPTION_SURFACE_RDO);
	
	//Effective Span Length or Impact Factor
	m_aCtrlEslImpf.Add(IDC_CMD_IMPACT_FACTOR_RDO);
	m_aCtrlEslImpf.Add(IDC_CMD_EFSL_RDO);

	//User Input or Auto Calculation
	m_aCtrlUserInput.Add(IDC_CMD_USER_INPUT_RDO);
	m_aCtrlUserInput.Add(IDC_CMD_AUTO_CALC_RDO);

	//Beam Type
	m_aCtrlElemBeamPart.Add(IDC_CMD_ELEM_PART_FRM);
	m_aCtrlElemBeamPart.Add(IDC_CMD_PART_CHK1);
	m_aCtrlElemBeamPart.Add(IDC_CMD_PART_CHK2);
	m_aCtrlElemBeamPart.Add(IDC_CMD_PART_CHK3);
	m_aCtrlElemBeamPart.Add(IDC_CMD_PART_CHK4);
	m_aCtrlElemBeamPart.Add(IDC_CMD_PART_CHK5);
	m_aCtrlElemBeamPart.Add(IDC_CMD_ELEM_COMP_FRM);
	m_aCtrlElemBeamPart.Add(IDC_CMD_IMPF_MYMAX_CHK);
	m_aCtrlElemBeamPart.Add(IDC_CMD_IMPF_MYMIN_CHK);
	m_aCtrlElemBeamPart.Add(IDC_CMD_IMPF_MZMAX_CHK);
	m_aCtrlElemBeamPart.Add(IDC_CMD_IMPF_MZMIN_CHK);
	m_aCtrlElemBeamPart.Add(IDC_CMD_IMPF_FXMAX_CHK);
	m_aCtrlElemBeamPart.Add(IDC_CMD_IMPF_FXMIN_CHK);

	//Plate Type
	m_aCtrlElemPlatePart.Add(IDC_CMD_ELEM_PART_FRM2);
	m_aCtrlElemPlatePart.Add(IDC_CMD_PART_CHK6);
	m_aCtrlElemPlatePart.Add(IDC_CMD_PART_CHK7);
	m_aCtrlElemPlatePart.Add(IDC_CMD_PART_CHK8);
	m_aCtrlElemPlatePart.Add(IDC_CMD_PART_CHK9);
	m_aCtrlElemPlatePart.Add(IDC_CMD_PART_CHK10);
	m_aCtrlElemPlatePart.Add(IDC_CMD_ELEM_COMP_FRM2);
	m_aCtrlElemPlatePart.Add(IDC_CMD_IMPF_MXXMAX_CHK);
	m_aCtrlElemPlatePart.Add(IDC_CMD_IMPF_MXXMIN_CHK);
	m_aCtrlElemPlatePart.Add(IDC_CMD_IMPF_MYYMAX_CHK);
	m_aCtrlElemPlatePart.Add(IDC_CMD_IMPF_MYYMIN_CHK);
	m_aCtrlElemPlatePart.Add(IDC_CMD_IMPF_FXXMAX_CHK);
	m_aCtrlElemPlatePart.Add(IDC_CMD_IMPF_FXXMIN_CHK);
	m_aCtrlElemPlatePart.Add(IDC_CMD_IMPF_FYYMAX_CHK);
	m_aCtrlElemPlatePart.Add(IDC_CMD_IMPF_FYYMIN_CHK);

	//Truss Type
	m_aCtrlElemTrussPart.Add(IDC_CMD_ELEM_TRUSS_COMP_FRM);
	m_aCtrlElemTrussPart.Add(IDC_CMD_IMPF_MAX_CHK);
	m_aCtrlElemTrussPart.Add(IDC_CMD_IMPF_MIN_CHK);

	//Beam CheckBox ID
	m_aCtrlBeamID.Add(IDC_CMD_PART_CHK1);
	m_aCtrlBeamID.Add(IDC_CMD_PART_CHK2);
	m_aCtrlBeamID.Add(IDC_CMD_PART_CHK3);
	m_aCtrlBeamID.Add(IDC_CMD_PART_CHK4);
	m_aCtrlBeamID.Add(IDC_CMD_PART_CHK5);
	m_aCtrlBeamID.Add(IDC_CMD_IMPF_MYMAX_CHK);
	m_aCtrlBeamID.Add(IDC_CMD_IMPF_MYMIN_CHK);
	m_aCtrlBeamID.Add(IDC_CMD_IMPF_MZMAX_CHK);
	m_aCtrlBeamID.Add(IDC_CMD_IMPF_MZMIN_CHK);
	m_aCtrlBeamID.Add(IDC_CMD_IMPF_FXMAX_CHK);
	m_aCtrlBeamID.Add(IDC_CMD_IMPF_FXMIN_CHK);

	//Plate CheckBox ID
	m_aCtrlPlateID.Add(IDC_CMD_PART_CHK6);
	m_aCtrlPlateID.Add(IDC_CMD_PART_CHK7);
	m_aCtrlPlateID.Add(IDC_CMD_PART_CHK8);
	m_aCtrlPlateID.Add(IDC_CMD_PART_CHK9);
	m_aCtrlPlateID.Add(IDC_CMD_PART_CHK10);
	m_aCtrlPlateID.Add(IDC_CMD_IMPF_MXXMAX_CHK);
	m_aCtrlPlateID.Add(IDC_CMD_IMPF_MXXMIN_CHK);
	m_aCtrlPlateID.Add(IDC_CMD_IMPF_MYYMAX_CHK);
	m_aCtrlPlateID.Add(IDC_CMD_IMPF_MYYMIN_CHK);
	m_aCtrlPlateID.Add(IDC_CMD_IMPF_FXXMAX_CHK);
	m_aCtrlPlateID.Add(IDC_CMD_IMPF_FXXMIN_CHK);
	m_aCtrlPlateID.Add(IDC_CMD_IMPF_FYYMAX_CHK);
	m_aCtrlPlateID.Add(IDC_CMD_IMPF_FYYMIN_CHK);

	//Truss CheckBox ID
	m_aCtrlTrussID.Add(IDC_CMD_IMPF_MAX_CHK);
	m_aCtrlTrussID.Add(IDC_CMD_IMPF_MIN_CHK);

	m_Data.Initialize();
}

CMAddImpfDlg::~CMAddImpfDlg()
{
}

void CMAddImpfDlg::DoDataExchange(CDataExchange* pDX)
{                        
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMAddImpfDlg)
	DDX_Control(pDX, IDC_CMD_IMPF_SPNLENGTH_EDT,  m_edtEsl);
	DDX_Control(pDX, IDC_CMD_IMPF_SPNLENGTH_UNT,  m_untEsl);
	DDX_Control(pDX, IDC_CMD_IMPF_EDT, m_edtImpf);
	DDX_Control(pDX, IDC_CMD_LANE_LST, m_List);
	DDX_Control(pDX, IDC_CMD_ELEM_TYPE_CMB, m_cmbElemType);
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CMAddImpfDlg, CCMChildBarBase)
//{{AFX_MSG_MAP(CMAddImpfDlg)

ON_BN_CLICKED(IDC_CMD_APPLY, OnApply)
ON_BN_CLICKED(IDC_CMD_CLOSE, OnClose)
ON_BN_CLICKED(IDC_CMD_OPTION_LINE_RDO   ,    OnLineSurfaceRdo)
ON_BN_CLICKED(IDC_CMD_OPTION_SURFACE_RDO,    OnLineSurfaceRdo)
ON_BN_CLICKED(IDC_CMD_OPTION_ADD_RDO    ,    OnAddReplaceRdo )
ON_BN_CLICKED(IDC_CMD_OPTION_DELETE_RDO ,    OnAddReplaceRdo )
ON_BN_CLICKED(IDC_CMD_EFSL_RDO          ,    OnEslImpfRdo )
ON_BN_CLICKED(IDC_CMD_IMPACT_FACTOR_RDO ,    OnEslImpfRdo )
ON_BN_CLICKED(IDC_CMD_USER_INPUT_RDO    ,    OnUserInputRdo)
ON_BN_CLICKED(IDC_CMD_AUTO_CALC_RDO     ,    OnUserInputRdo)
ON_CBN_SELCHANGE(IDC_CMD_ELEM_TYPE_CMB  ,    OnSelchangeElemTypeCmb)

//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMAddImpfDlg::SetListItem(const int nType)
{
	m_List.ResetContent();
	if(nType<0 || nType>2) { ASSERT(0); return; }
	
	CArray<UINT, UINT> aKey;
	CArray<CString, CString&> aName;
	switch(nType)
	{
	case 0: // LLAN
		m_pDoc->m_pAttrCtrl->GetLlanKeyList(aKey);
		m_pDoc->m_pAttrCtrl->GetLlanList(aName);
		break;
	case 1: // SLAN
		m_pDoc->m_pAttrCtrl->GetSlanKeyList(aKey);
		m_pDoc->m_pAttrCtrl->GetSlanList(aName);
		break;
	default: ASSERT(0); break;
	}
	ASSERT(aKey.GetSize()==aName.GetSize());
	int nSize = aKey.GetSize(), nIndex = 0;
	for(int i=0; i<nSize; ++i)
	{
		nIndex = m_List.AddString(aName[i]);
		m_List.SetItemData(nIndex, aKey[i]);
		m_List.SetCheck(nIndex, 1);
	}
}

 void CMAddImpfDlg::Data2Dlg()
 { 
	// Dlg2Data(TRUE);

	// m_edtEsl = m_Data.dEffectiveSpanLength.();
	// m_edtImpf.SetEditUnit(m_Data.dImpactFactor);
// 
//   //CAttrCtrl *pAttrCtrl = CDBDoc::GetDocPoint()->m_pAttrCtrl;
//   //T_IMPF_D ImpfD;
 }

BOOL CMAddImpfDlg::Dlg2Data()
{
	m_Data.Initialize();
	int i=0, nCheck = 0, nCursor = 0;
	int nFactorType = 0;

	CDlgUtil::CtrlRadioGetCheck(this,m_aCtrlLaneCheck,  m_Data.nLaneType);
	CDlgUtil::CtrlRadioGetCheck(this,m_aCtrlEslImpf  ,  nFactorType);
	CDlgUtil::CtrlRadioGetCheck(this,m_aCtrlUserInput,  nCheck);
	nCursor = m_cmbElemType.GetCurSel();
	
	if(nFactorType==1)  //  0: Impact Factor , 1: Effective Span Length
	{
		if (nCheck == 0)      // 0: User Input
		{
			m_Data.dEffectiveSpanLength = m_edtEsl.GetEditValue();
			m_Data.nFactorType = 0;
		}
		else                  // 1: Auto Calculation
		{
			 m_Data.nFactorType = 2; 
				
			 nCursor++;
			 CButton* pButton = NULL;
			 switch(nCursor)
			 {
			 case Truss:  m_Data.nElemType = 0;       // Truss 일 때
				 for (i=0; i<2; ++i)
				 {
					 pButton = (CButton*)GetDlgItem(m_aCtrlTrussID[i]);
					 m_Data.bComponents[i] = pButton->GetCheck();
				 }
				 break;
			 case Beam:   m_Data.nElemType = 1;       // Beam 일 때 
				 for (i=0; i<5; ++i)                    // m_aCtrlBeamID Array에 0~4번까지는 Part 
				 { 
					 pButton = (CButton*)GetDlgItem(m_aCtrlBeamID[i]);
					 m_Data.bParts[i] = pButton->GetCheck(); 
				 }
				 for (i=0; i<6; ++i)                    // m_aCtrlBeamID Array에 5~10번까지는 Component 
				 { 
					 pButton = (CButton*)GetDlgItem(m_aCtrlBeamID[i+5]);
					 m_Data.bComponents[i] = pButton->GetCheck(); 
				 }
				 break;     
			 case Plate:  m_Data.nElemType = 2;       // Plate 일 때 
				 for (i=0; i<5; ++i)                    // m_aCtrlPlateID Array에 0~4번까지는 Part 
				 { 
					 pButton = (CButton*)GetDlgItem(m_aCtrlPlateID[i]);
					 m_Data.bParts[i] = pButton->GetCheck(); 
				 }
				 for (i=0; i<8; ++i)                    // m_aCtrlPlateID Array에 5~12번까지는 Component 
				 { 
					 pButton = (CButton*)GetDlgItem(m_aCtrlPlateID[i+5]);
					 m_Data.bComponents[i] = pButton->GetCheck(); 
				 }
				 break;
			 default:
				 break;
			 }
		}
	}
	else
	{
		m_Data.dImpactFactor = m_edtImpf.GetEditValue();
		m_Data.nFactorType = 1;
	}
	return TRUE;
}

void CMAddImpfDlg::AlignControls()
{
	int nDist;
	CRect rRef, rToMove;
	
	GetDlgItem(IDC_CMD_ELEM_PART_FRM)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_ELEM_PART_FRM2)->GetWindowRect(rToMove);
	nDist = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlElemPlatePart, nDist);

	GetDlgItem(IDC_CMD_ELEM_TRUSS_COMP_FRM)->GetWindowRect(rToMove);
	nDist = rRef.left - rToMove.left;
	CDlgUtil::CtrlMoveDistX(this, m_aCtrlElemTrussPart, nDist);
 
}

void CMAddImpfDlg::MakeItemEx()
{
	int nType=0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLaneCheck, nType);
	SetListItem(nType);
}

//////////////////////////////////////////////////////////////////////////
// CMAddImpfDlg message handler

BOOL CMAddImpfDlg::OnInitDialog()
{               
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	
	CCMChildBarBase::OnInitDialog();

	AlignControls();
	
	int nSelect=0;
	CDlgUtil::CtrlRadioSetCheck(this,m_aCtrlOption,    nSelect);
	CDlgUtil::CtrlRadioSetCheck(this,m_aCtrlLaneCheck, nSelect);
	CDlgUtil::CtrlRadioSetCheck(this,m_aCtrlEslImpf,   nSelect);
	CDlgUtil::CtrlRadioSetCheck(this,m_aCtrlUserInput, nSelect);
	m_edtEsl.SetUnitType(CUnitCtrl::m_IMPF_UNIT.dEffectiveSpanLength);
	m_untEsl.SetUnitType(CUnitCtrl::m_IMPF_UNIT.dEffectiveSpanLength);
	m_edtImpf.SetUnitType(CUnitCtrl::m_IMPF_UNIT.dImpactFactor);
	m_edtImpf.SetEditUnit(0.3);
	
	CDlgUtil::CtrlShowHide(this, m_aCtrlElemBeamPart, TRUE);

	CDlgUtil::CobxAddItem(m_cmbElemType, _LS(IDS_CMD_IMPF_Truss), 1);
	CDlgUtil::CobxAddItem(m_cmbElemType, _LS(IDS_CMD_IMPF_Beam) , 2);
	CDlgUtil::CobxAddItem(m_cmbElemType, _LS(IDS_CMD_IMPF_Plate), 3);

	CDlgUtil::CobxSetCurSelItemData(m_cmbElemType, Beam);
	
	//체크박스에 초기값을 설정한다.
	CButton* pButton = NULL;
	pButton = (CButton*)GetDlgItem(IDC_CMD_PART_CHK1);  
	pButton->SetCheck(1);
	pButton = (CButton*)GetDlgItem(IDC_CMD_IMPF_MYMAX_CHK);
	pButton->SetCheck(1);
	pButton = (CButton*)GetDlgItem(IDC_CMD_PART_CHK6);
	pButton->SetCheck(1);
	pButton = (CButton*)GetDlgItem(IDC_CMD_IMPF_MXXMAX_CHK);
	pButton->SetCheck(1);
	pButton = (CButton*)GetDlgItem(IDC_CMD_IMPF_MAX_CHK);
	pButton->SetCheck(1);

	CDlgUtil::CtrlShowHide(this, m_aCtrlElemPlatePart, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aCtrlElemTrussPart, FALSE);
	OnAddReplaceRdo();
	OnLineSurfaceRdo();
	
	return TRUE;
}

void CMAddImpfDlg::OnLineSurfaceRdo()
{ 
	int nType=0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlLaneCheck, nType);
	SetListItem(nType);

	// MNET:XXXX 20080117 mylee - 740 Surface 일때 AUTO 임시로 막음
	int nUserAuto=0, nFactorLength=0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlEslImpf, nFactorLength);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlUserInput, nUserAuto);
	if(nType==1 && nFactorLength==1) // Surface
	{
		CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlUserInput, 0);
		OnUserInputRdo();
		GetDlgItem(IDC_CMD_AUTO_CALC_RDO)->EnableWindow(FALSE);
	}
	if(nType==0 && nFactorLength==1) // Line
	{
		OnUserInputRdo();
		GetDlgItem(IDC_CMD_AUTO_CALC_RDO)->EnableWindow(TRUE);
	}
}

void CMAddImpfDlg::OnAddReplaceRdo()
{
	int nOption=0;
	CDlgUtil::CtrlRadioGetCheck(this,m_aCtrlOption, nOption);
	
	GetDlgItem(IDC_CMD_SELECT_LANE_FRM)   ->EnableWindow(nOption==0);
	GetDlgItem(IDC_CMD_EFSL_RDO)          ->EnableWindow(nOption==0);
	GetDlgItem(IDC_CMD_IMPACT_FACTOR_RDO) ->EnableWindow(nOption==0);
	GetDlgItem(IDC_CMD_OPTION_LINE_RDO)   ->EnableWindow(nOption==0);
	GetDlgItem(IDC_CMD_OPTION_SURFACE_RDO)->EnableWindow(nOption==0);
	GetDlgItem(IDC_CMD_IMPF_FRM)          ->EnableWindow(nOption==0);
	GetDlgItem(IDC_CMD_LANE_LST)          ->EnableWindow(nOption==0);
	
	OnEslImpfRdo();
}
void CMAddImpfDlg::OnEslImpfRdo()
{
	int nOption=0, nSelect=0, nLineSurface=0;
	CDlgUtil::CtrlRadioGetCheck(this,  m_aCtrlOption,  nOption);
	CDlgUtil::CtrlRadioGetCheck(this,  m_aCtrlEslImpf, nSelect);
	CDlgUtil::CtrlRadioGetCheck(this,  m_aCtrlLaneCheck, nLineSurface);
	
	//Impact Factor 
	GetDlgItem(IDC_CMD_IMPF_EDT)->EnableWindow(nOption==0 && nSelect==0);

	//Effective Span Length
	GetDlgItem(IDC_CMD_USER_INPUT_RDO)->EnableWindow(nOption==0 && nSelect==1);
	GetDlgItem(IDC_CMD_AUTO_CALC_RDO)->EnableWindow(nOption==0 && nSelect==1);

	// MNET:XXXX 20080117 mylee - 740 Surface 일때 AUTO 임시로 막음
	if(nLineSurface==1)
		GetDlgItem(IDC_CMD_AUTO_CALC_RDO)->EnableWindow(FALSE);

	OnUserInputRdo();
}

void CMAddImpfDlg::OnUserInputRdo()
{
	int nOption=0, nSelect=0, nUserSelect =0;
	CDlgUtil::CtrlRadioGetCheck(this,  m_aCtrlOption,  nOption);
	CDlgUtil::CtrlRadioGetCheck(this,  m_aCtrlEslImpf, nSelect);
	CDlgUtil::CtrlRadioGetCheck(this,  m_aCtrlUserInput, nUserSelect);

	//Effective Span Length && User Input
	BOOL bEnable = (nOption==0 && nSelect==1 && nUserSelect==0);
	GetDlgItem(IDC_CMD_IMPF_SPNLENGTH_UNT) ->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_IMPF_SPNLENGTH_EDT) ->EnableWindow(bEnable);

	//Effective Span Length && Auto Calculation
	bEnable = (nOption==0 && nSelect==1 && nUserSelect==1);
	GetDlgItem(IDC_CMD_ELEM_TYPE_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_ELEM_TYPE_CMB)->EnableWindow(bEnable);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlElemBeamPart, bEnable);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlElemPlatePart, bEnable);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlElemTrussPart, bEnable);
}

void CMAddImpfDlg::OnApply()
{
	if(!Dlg2Data()) return;
	CArray<UINT, UINT> aElemK;                 // List에서 선택된 aElemK를 받아오는 과정  
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aElemK);
	if(aElemK.GetSize()<1)
	{
		AfxMessageBox(_LS(IDS_CMD_IMPF_ELEM_NOT_EXSIT));   // 오류 메시지
		return ;
	}
	CArray<UINT, UINT> aLaneK;
	int nCount = m_List.GetCount();
	if(nCount<1)
	{
		AfxMessageBox(_LS(IDS_CMD_IMPF_NO_DEFINE));      // 오류 메시지
		return ;
	}
	UINT LaneK = 0;
	for(int i=0; i<nCount; ++i)
	{
		if(m_List.GetCheck(i)==0) continue;
		LaneK = m_List.GetItemData(i);
		aLaneK.Add(LaneK);                                            // 선택된 Lane Key를 가져온다.
	}
	if(aLaneK.GetSize()<1)
	{
		AfxMessageBox(_LS(IDS_CMD_IMPF_LANE_NOT_EXSIT));     //선택된 Lane 오류메시지
		return ;
	}
	
	int bSuccess=0;
	int nAddDel=0;
	CDlgUtil::CtrlRadioGetCheck(this,m_aCtrlOption, nAddDel);
	
	if (nAddDel == 0)
	{
		BOOL bSuccess = m_pDoc->m_pDataCtrl->AddImpf(aElemK, aLaneK, m_Data); 
		if (bSuccess)
		{
			m_pDoc->m_pViewCtrl->UnselectAll(NULL);

			T_IMPF_D ImpfDCopy = m_Data;
			m_pDoc->m_pUnitCtrl->ConvertUnitImpfIn(ImpfDCopy);

		 if(0. < ImpfDCopy.dEffectiveSpanLength && ImpfDCopy.dEffectiveSpanLength < 15./0.3-40.)
		 {
			GSaveHistoryFormatNF(_LS(IDS_CMD_IMPF_EXCEED_VALUE));
		 }
		 GSaveHistoryFormatNF(_LS(IDS_CMD_ADD_SUCCESS), aElemK.GetSize());
		}
	}
	else
	{
		bSuccess = m_pDoc->m_pDataCtrl->DelImpf(aElemK); 
		if (bSuccess) 
		{
			m_pDoc->m_pViewCtrl->UnselectAll(NULL);
			GSaveHistoryFormatNF(_LS(IDS_CMD_DEL_SUCCESS), aElemK.GetSize());
		}
	}
}

void CMAddImpfDlg::OnClose() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CMAddImpfDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
	case D_UPDATE_STAGE_CHANGED:
	case D_UPDATE_STAGE_DBALL:
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
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CMAddImpfDlg::UpdateBuffer()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	
	CViewBuff* pViewBuff = pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);
	
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	
	BOOL bChanged = FALSE;
	BOOL bMvcdChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
			case(UR_LLAN_ADD):
			case(UR_LLAN_DEL):
			case(UR_LLAN_MFS):
			case(UR_LLAN_MFD):

			case(UR_SLAN_ADD):
			case(UR_SLAN_DEL):
			case(UR_SLAN_MFS):
			case(UR_SLAN_MFD):
			
				 bChanged = TRUE;
			break;
			
			case(UR_MVCD_ADD):
			case(UR_MVCD_DEL):
				bMvcdChanged = TRUE;
			break;
	 
			default:
			break;
		}

		if (bChanged)
			break;
	} // end of while
	
	if (bChanged) MakeItemEx();
	if (bMvcdChanged) DestroyWindow();
}

BOOL CMAddImpfDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_DELETE && GetFocus() == &m_List)
		{
			return TRUE;
		}
	}
	return CCMChildBarBase::PreTranslateMessage(pMsg);
}

void CMAddImpfDlg::OnSelchangeElemTypeCmb()
{
	DWORD nCode;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbElemType, nCode);

	CDlgUtil::CtrlShowHide(this, m_aCtrlElemBeamPart , FALSE);
	CDlgUtil::CtrlShowHide(this, m_aCtrlElemPlatePart, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aCtrlElemTrussPart, FALSE);

	if (nCode == Truss)
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlElemTrussPart, TRUE);
	}
	else if (nCode == Beam)
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlElemBeamPart, TRUE);
	}
	else if (nCode == Plate)
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrlElemPlatePart, TRUE);
	}
}