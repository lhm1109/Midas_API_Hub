// CRCDgnOption.cpp: implementation of the CCRCDgnOption class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CRCDgnOption.h"
#include "CRCDesignOptTabIDlg.h"
#include "CRCDesignOptTabJDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_base\TestEnvMgr.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "CRCDataCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

constexpr int ELEM_TYPE_ELEMENT = 0;
constexpr int ELEM_TYPE_VBEAM = 1;

CCRCDgnOption::CCRCDgnOption(CWnd* pParent /*=NULL*/)
    : CMenuBarChildDlg(CCRCDgnOption::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCRCDgnOption)
    m_nOption = -1;
    m_nMOption = -1;
    m_nSOption = -1;
    m_nTOption = -1;
    m_nFOption = -1;
    m_nCOption = -1;
	m_nClass   = -1;
	m_bTypeC   = FALSE;
    //
    m_iDgnCode = -1;
    m_nElemType = ELEM_TYPE_ELEMENT;
    //}}AFX_DATA_INIT
    m_pDoc = CDBDoc::GetDocPoint();

    //{{AFX_DATA_INIT(CCPGTranStifDlg)
    m_nOption = 0;
    m_bBothIJ = TRUE;
    //}}AFX_DATA_INIT
    m_pDoc = CDBDoc::GetDocPoint();

    m_I_Data.Initialize();
    m_J_Data.Initialize();

    m_bJTabEnable = FALSE;

    m_Data.Initialize();

    m_aElemTypeCtrl.RemoveAll();
    m_aElemTypeCtrl.Add(IDC_DGN_PSC_ELEM_TYPE_GRP);
    m_aElemTypeCtrl.Add(IDC_DGN_PSC_ELEM_RDO);
    m_aElemTypeCtrl.Add(IDC_DGN_PSC_VELEM_RDO);

    m_aMoveCtrl.RemoveAll();
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_MOMENT_STC);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_M1);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_M2);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_M3);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_M4);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_SHEAR_STC);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_S1);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_S2);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_S3);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_S4);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_TORSION_STC);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_T1);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_T2);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_T3);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_T4);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_FATIGUE_STC);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_F1);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_F2);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_F3);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_F4);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_CRACK_STC);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_C1);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_C2);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_C3);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_C4);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_CLASS_GRP);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_CLASS1_RDO);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_CLASS2_RDO);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_CLASS3_RDO);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_CLASS3_TYPE_C_GRP);
    m_aMoveCtrl.Add(IDC_DGN_CRC_POSC_CLASS3_TYPE_C_CHK);
    m_aMoveCtrl.Add(IDC_DGN_CPG_CPUL_IJBOTH_CHK);
    m_aMoveCtrl.Add(IDC_DGN_EXECUTE);
    m_aMoveCtrl.Add(IDC_DGN_CLOSE);
    m_aMoveCtrl.Add(IDC_DGN_CPG_CPUL_TAB);
}


void CCRCDgnOption::DoDataExchange(CDataExchange* pDX)
{
    CMenuBarChildDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDgnGenKFacDlg)
    DDX_Radio(pDX, IDC_DGN_CRC_POSC_AR,                m_nOption);
    DDX_Radio(pDX, IDC_DGN_CRC_POSC_M1,                m_nMOption);
    DDX_Radio(pDX, IDC_DGN_CRC_POSC_S1,                m_nSOption);
    DDX_Radio(pDX, IDC_DGN_CRC_POSC_T1,				   m_nTOption);
    DDX_Radio(pDX, IDC_DGN_CRC_POSC_F1,				   m_nFOption);
    DDX_Radio(pDX, IDC_DGN_CRC_POSC_C1,                m_nCOption);
    DDX_Radio(pDX, IDC_DGN_PSC_ELEM_RDO, m_nElemType);
	DDX_Radio(pDX, IDC_DGN_CRC_POSC_CLASS1_RDO,        m_nClass);
	DDX_Check(pDX, IDC_DGN_CRC_POSC_CLASS3_TYPE_C_CHK, m_bTypeC);

    //DDX_Radio(pDX, IDC_DGN_CPG_OPT_ADD_RDO, m_nOption);
    DDX_Check(pDX, IDC_DGN_CPG_CPUL_IJBOTH_CHK, m_bBothIJ);
    DDX_Control(pDX, IDC_DGN_CPG_CPUL_TAB, m_Tab);
    DDX_Control(pDX, IDC_DGN_CPG_CPUL_IJBOTH_CHK, m_Ctrl_BothIJ);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCRCDgnOption, CMenuBarChildDlg)
    //{{AFX_MSG_MAP(CCRCDgnOption)
    ON_BN_CLICKED(IDC_DGN_CRC_POSC_AR,         OnDgnCRCPoscAr)
    ON_BN_CLICKED(IDC_DGN_CRC_POSC_DEL,        OnDgnCRCPoscDel)
	ON_BN_CLICKED(IDC_DGN_CRC_POSC_CLASS1_RDO, OnDgnClassRdo)
	ON_BN_CLICKED(IDC_DGN_CRC_POSC_CLASS2_RDO, OnDgnClassRdo)
	ON_BN_CLICKED(IDC_DGN_CRC_POSC_CLASS3_RDO, OnDgnClassRdo)

    //ON_BN_CLICKED(IDC_DGN_CPG_OPT_ADD_RDO, OnDgnCPGLoadAppAdd)
   // ON_BN_CLICKED(IDC_DGN_CPG_OPT_DEL_RDO, OnDgnCPGLoadAppDel)
    ON_BN_CLICKED(IDC_DGN_CPG_CPUL_IJBOTH_CHK, OnBothIJChk)
    ON_BN_CLICKED(IDC_DGN_PSC_ELEM_RDO, OnDgnPSCElemTypeRdo)
    ON_BN_CLICKED(IDC_DGN_PSC_VELEM_RDO, OnDgnPSCElemTypeRdo)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CCRCDgnOption::OnInitDialog() 
{
    CMenuBarChildDlg::OnInitDialog();

    if(m_pDoc==NULL)	return TRUE;

	InitArray();
    Initial_Data();
	AlignControls();
	EnableDisableControls();
    ShowHideControls();

    ResizeDialog();


    m_Tab.DeleteAllItems();

    // I
    m_pSubDlg_I = new CCRCDesignOptTabIDlg(this);
    m_Tab.AddTab(m_pSubDlg_I, _LS(IDS_DGN_CPG_I), CCRCDesignOptTabIDlg::IDD, TRUE);

    // J
    m_pSubDlg_J = new CCRCDesignOptTabJDlg(this);
    m_Tab.AddTab(m_pSubDlg_J, _LS(IDS_DGN_CPG_J), CCRCDesignOptTabJDlg::IDD, TRUE);

    m_Tab.ShowTab(0);

	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCRCDgnOption::Dlg2Data()
{
    m_Data.bBothIJ = m_Ctrl_BothIJ.GetCheck();

    if (m_Tab.GetCurSel() == 0)
    {
        if (m_Data.bBothIJ)
        {
            //I, J 가 같을 경우
            m_pSubDlg_I->GetCpulData();
            m_Data.TfType[0] = m_I_Data;
            m_Data.TfType[1] = m_Data.TfType[0];
        }
        else
        {
            m_pSubDlg_I->GetCpulData();
            m_Data.TfType[0] = m_I_Data;
            m_Data.TfType[1] = m_J_Data;
        }
    }
    else
    {
        if (m_Data.bBothIJ)
        {
            m_Data.TfType[0] = m_I_Data;
            m_Data.TfType[1] = m_Data.TfType[0];
        }
        else
        {
            m_Data.TfType[0] = m_I_Data;
            m_pSubDlg_J->GetCpulData();
            m_Data.TfType[1] = m_J_Data;
        }
    }

    return TRUE;
}


void CCRCDgnOption::OnDgnPSCElemTypeRdo()
{
  UpdateData(TRUE);

  I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
  pIGM->ChangeSelectionFilterCombo(m_nElemType == 0 ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);
}

void CCRCDgnOption::Execute()
{
    // Select된 Element를 얻어옵니다.
    CArray<T_ELEM_K,T_ELEM_K> aSelKey;
    CArray<T_ELEM_K,T_ELEM_K> rSelKey;
    I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
    if (m_nElemType == ELEM_TYPE_ELEMENT)
    {
      pIGM->GetSelectedElemKeyListForDgn(aSelKey);

      if(aSelKey.GetSize() > 0)
      {
          for(int i=0; i<aSelKey.GetSize(); i++)
          {
              T_ELEM_K key = aSelKey.GetAt(i);
              T_ELEM_D eData;
              eData.Initialize();
              if(!m_pDoc->m_pAttrCtrl->GetElem(key,eData)) continue;
              ElemPairK EPairK(key, EN_EL_BEAM);
              if(CDBLib::IsPscDgnFor1st(EPairK,TRUE))	rSelKey.Add(key);
          }
          T_POSC_D rData;
          rData.Initialize();
          if(m_nOption==0)	// add/replace
          {
              UpdateData(TRUE);
              Dlg2Data();
              rData.iMomChk   = m_nMOption;
              rData.iShrChk   = m_nSOption;
              rData.nTorChk   = m_nTOption;
              rData.nFatiChk  = m_nFOption;
              rData.nCrackChk = m_nCOption;
			  rData.nClass    = m_nClass;
			  rData.bTypeC    = m_bTypeC;
              rData.dIThetaC = m_I_Data.dLengthTop;
              rData.dIThetaV = m_I_Data.dLengthBot;
              if (m_Data.bBothIJ)
              {
                  rData.dJThetaC =m_I_Data.dLengthTop;
                  rData.dJThetaV =m_I_Data.dLengthBot;
              }
              else
              {
                  rData.dJThetaC = m_J_Data.dLengthTop;
                  rData.dJThetaV = m_J_Data.dLengthBot;
              }
            

              if(rSelKey.GetSize() > 0)
              {
                  // Initialize selected Element.
                  if(m_pDoc->m_pDataCtrl->AddPosc(rSelKey,rData))	Initial_SelectItem();
              }
              else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_PSC),MB_OK);
          }
          else	// Delete
          {
              // Initialize selected Element.
              if(m_pDoc->m_pDataCtrl->DelPosc(rSelKey))	Initial_SelectItem();
          }
      }
      else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
    }
    else
    {
      pIGM->GetSelectedVbemKeyListForDgn(aSelKey);

      if(aSelKey.GetSize() > 0)
      {
          //ASSERT(0);
          // The function below must be applied according to the virtual beam.
          // Refer to the ELEM_TYPE_ELEMENT case as a guideline.
          for(int i=0; i<aSelKey.GetSize(); i++)
          {
              T_ELEM_K key = aSelKey.GetAt(i);
              T_ELEM_D eData;
              eData.Initialize();
              if(!m_pDoc->m_pAttrCtrl->GetElem(key,eData)) continue;
              if(CDBLib::IsPscDgnForVbeam(key,TRUE))	rSelKey.Add(key);
          }
          T_POVC_D rData;
          rData.Initialize();
          if(m_nOption==0)	// add/replace
          {
              UpdateData(TRUE);
              Dlg2Data();
              rData.iMomChk   = m_nMOption;
              rData.iShrChk   = m_nSOption;
              rData.nTorChk   = m_nTOption;
              rData.nFatiChk  = m_nFOption;
              rData.nCrackChk = m_nCOption;
			  rData.nClass    = m_nClass;
			  rData.bTypeC    = m_bTypeC;
              rData.dIThetaC = m_I_Data.dLengthTop;
              rData.dIThetaV = m_I_Data.dLengthBot;
              if (m_Data.bBothIJ)
              {
                  rData.dJThetaC =m_I_Data.dLengthTop;
                  rData.dJThetaV =m_I_Data.dLengthBot;
              }
              else
              {
                  rData.dJThetaC = m_J_Data.dLengthTop;
                  rData.dJThetaV = m_J_Data.dLengthBot;
              }
            

              if(rSelKey.GetSize() > 0)
              {
                  // Initialize selected Element.
                  if(m_pDoc->m_pDataCtrl->AddPovc(rSelKey,rData))	Initial_SelectItem();
              }
              else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_PSC),MB_OK);
          }
          else	// Delete
          {
              // Initialize selected Element.
              if(m_pDoc->m_pDataCtrl->DelPovc(rSelKey))	Initial_SelectItem();
          }
      }
      else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
    }
}

void CCRCDgnOption::Initial_SelectItem() 
{
    // Unselected All.
    if(m_pDoc==NULL)	return;
    if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
    // Initialize Data.
    Initial_Data();
}

void CCRCDgnOption::OnBothIJChk()
{
    m_bJTabEnable = (m_Ctrl_BothIJ.GetCheck() == 0) ? TRUE : FALSE;

    if (m_Tab.GetCurSel() == 1)
        m_pSubDlg_J->SetAllControlEnable(m_bJTabEnable);
}

void CCRCDgnOption::InitArray()
{
	m_aRUS.RemoveAll();
	m_aRUS.Add(IDC_DGN_CRC_POSC_TORSION_STC);
	m_aRUS.Add(IDC_DGN_CRC_POSC_T1);
	m_aRUS.Add(IDC_DGN_CRC_POSC_T2);
	m_aRUS.Add(IDC_DGN_CRC_POSC_T3);
	m_aRUS.Add(IDC_DGN_CRC_POSC_T4);
	m_aRUS.Add(IDC_DGN_CRC_POSC_FATIGUE_STC);
	m_aRUS.Add(IDC_DGN_CRC_POSC_F1);
	m_aRUS.Add(IDC_DGN_CRC_POSC_F2);
	m_aRUS.Add(IDC_DGN_CRC_POSC_F3);
	m_aRUS.Add(IDC_DGN_CRC_POSC_F4);
	m_aRUS.Add(IDC_DGN_CRC_POSC_CRACK_STC);
	m_aRUS.Add(IDC_DGN_CRC_POSC_C1);
	m_aRUS.Add(IDC_DGN_CRC_POSC_C2);
	m_aRUS.Add(IDC_DGN_CRC_POSC_C3);
	m_aRUS.Add(IDC_DGN_CRC_POSC_C4);

	m_aBS.RemoveAll();
	m_aBS.Add(IDC_DGN_CRC_POSC_CLASS_GRP);
	m_aBS.Add(IDC_DGN_CRC_POSC_CLASS1_RDO);
	m_aBS.Add(IDC_DGN_CRC_POSC_CLASS2_RDO);
	m_aBS.Add(IDC_DGN_CRC_POSC_CLASS3_RDO);
	m_aBS.Add(IDC_DGN_CRC_POSC_CLASS3_TYPE_C_GRP);
	m_aBS.Add(IDC_DGN_CRC_POSC_CLASS3_TYPE_C_CHK);	

    m_aTMH.RemoveAll();
    m_aTMH.Add(IDC_DGN_CPG_CPUL_IJBOTH_CHK);
    m_aTMH.Add(IDC_DGN_CPG_CPUL_TAB);
}

void CCRCDgnOption::Initial_Data()
{
    m_nOption  = 0;	// add/replace

    m_nMOption  = 3;
    m_nSOption  = 3;
    m_nTOption  = 3;
    m_nFOption  = 3;
    m_nCOption  = 3;
	m_nClass    = 0;
	m_bTypeC    = FALSE;

    // in case Eurocode..  Convert Group Box Item-Name.
    T_PSCD_D PscdD;
    if(!m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD))
    {
        T_PREFERENCE rPref;
        rPref.Initialize();
        m_pDoc->m_pInitCtrl->GetPreference(rPref);	
        m_iDgnCode = CDBLib::GetPscCodeNo(rPref.DgnCode.PSCCode);
    }
    else
    {
        m_iDgnCode = PscdD.iDgnCode;
    }

    if( CDBLib::IsPscCodeLSD(m_iDgnCode) || m_iDgnCode==IRC_112_2011_PSC ||
        m_iDgnCode==AS_5100_5_17_PSC  || m_iDgnCode==BS5400_90_PSC ||m_iDgnCode==TMH07_3_1989 || m_iDgnCode==BD_44_15_PSC || 
        m_iDgnCode==CS_455_PSC || m_iDgnCode == IRC_112_2020_PSC)
    {
        // Moment
        CString strGrpName = _T("");
        strGrpName = _LS(IDS_DGN_TEXT_MOMENT_RESISTANCE);
        GetDlgItem(IDC_DGN_CRC_POSC_MOMENT_STC)->SetWindowText(strGrpName);

        // Shear 
        strGrpName = _T("");
        strGrpName = _LS(IDS_DGN_TEXT_SHEAR_RESISTANCE);
        GetDlgItem(IDC_DGN_CRC_POSC_SHEAR_STC)->SetWindowText(strGrpName);
    }

    UpdateData(FALSE);
}

void CCRCDgnOption::AlignControls()
{
  HideAndMoveControlVBemByDB();

	int nDistX = 0.;
	int nDistY = 0.;
	CRect rRef, rToMove;

	GetDlgItem(IDC_DGN_CRC_POSC_TORSION_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_CRC_POSC_CLASS_GRP)->GetWindowRect(rToMove);

	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, m_aBS, nDistX, nDistY);
	CDlgUtil::CtrlMoveDistXY(this, m_aTMH, nDistX, nDistY);
}

void CCRCDgnOption::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_CRC_POSC_CLASS3_TYPE_C_CHK)->EnableWindow(m_nClass == 2);
	//if (m_iDgnCode == TMH07_3_1989) {
	//	GetDlgItem(IDC_DGN_CRC_POSC_CLASS3_TYPE_C_GRP)->IsWindowVisible()==False;
	//	GetDlgItem(IDC_DGN_CRC_POSC_CLASS_GRP)->IsWindowVisible() == True;

	//	GetDlgItem(IDC_DGN_CRC_POSC_CLASS3_TYPE_C_GRP)->ShowWindow(False);
	//	GetDlgItem(IDC_DGN_CRC_POSC_CLASS3_TYPE_C_CHK)->ShowWindow(False);
	//	GetDlgItem(IDC_DGN_CRC_POSC_CLASS_GRP)->GetWindowRect(rRef);
	//}
}

void CCRCDgnOption::HideAndMoveControlVBemByDB()
{
  CRect rRef;
  CRect rToMove;
  GetDlgItem(IDC_DGN_PSC_ELEM_TYPE_GRP)->GetWindowRect(rRef);
  GetDlgItem(IDC_DGN_CRC_POSC_MOMENT_STC)->GetWindowRect(rToMove);

  BOOL bPSCDgnVbem = CDBLib::ActivatePscVBEM();
  int nDistY;

  if (m_iDgnCode == EUROCODE2_2_05_PSC)
  {
    if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !bPSCDgnVbem)
    {
      nDistY = rRef.top - rToMove.top;
      CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, TRUE);
      m_nElemType = ELEM_TYPE_ELEMENT;
    }
    else
    {
      nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(4);
      CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, TRUE);
    }
  }
  else
  {
    nDistY = rRef.top - rToMove.top;
    m_nElemType = ELEM_TYPE_ELEMENT;
    CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, FALSE);
  }

  CDlgUtil::CtrlMoveDistY(this, m_aMoveCtrl, nDistY);
}

void CCRCDgnOption::ShowHideControls()
{
    HideAndMoveControlVBemByDB();

    if(m_iDgnCode==SNiP_20503_84_PSC || m_iDgnCode==SP_35_13330_11_PSC || m_iDgnCode==SNiP_20503_84_PSC_MKS || m_iDgnCode==SP_35_13330_11_PSC_MKS)
    {
		CDlgUtil::CtrlShowHide(this, m_aRUS, SW_SHOW);
		CDlgUtil::CtrlShowHide(this, m_aBS, SW_HIDE);
		CDlgUtil::CtrlShowHide(this, m_aTMH, SW_HIDE);
    }
	else if (m_iDgnCode == BS5400_90_PSC)
	{
		CDlgUtil::CtrlShowHide(this, m_aRUS, SW_HIDE);
		CDlgUtil::CtrlShowHide(this, m_aBS, SW_SHOW);
		CDlgUtil::CtrlShowHide(this, m_aTMH, SW_HIDE);
	}
    else if (m_iDgnCode == TMH07_3_1989)
    {
        CDlgUtil::CtrlShowHide(this, m_aRUS, SW_HIDE);
        CDlgUtil::CtrlShowHide(this, m_aBS, SW_SHOW);
        CDlgUtil::CtrlShowHide(this, m_aTMH, SW_SHOW);
    }
    else
    {
		CDlgUtil::CtrlShowHide(this, m_aRUS, SW_HIDE);
		CDlgUtil::CtrlShowHide(this, m_aBS, SW_HIDE);
		CDlgUtil::CtrlShowHide(this, m_aTMH, SW_HIDE);
    }
}

void CCRCDgnOption::ResizeDialog()
{
    CRect rRef;
    CRect rToMove;
    int nDistY = 0;
    CArray<UINT, UINT> aControls;
    CArray<UINT, UINT> aControls1;

	aControls.RemoveAll();
	aControls.Add(IDC_DGN_EXECUTE);
	aControls.Add(IDC_DGN_CLOSE);

    aControls1.RemoveAll();
    aControls1.Add(IDC_DGN_CPG_CPUL_IJBOTH_CHK);
    aControls1.Add(IDC_DGN_CPG_CPUL_TAB);

    if(m_iDgnCode==SNiP_20503_84_PSC || m_iDgnCode==SP_35_13330_11_PSC || m_iDgnCode==SNiP_20503_84_PSC_MKS || m_iDgnCode==SP_35_13330_11_PSC_MKS)
    {
        GetDlgItem(IDC_DGN_CRC_POSC_CRACK_STC)->GetWindowRect(rRef);
    }
	else if (m_iDgnCode == BS5400_90_PSC)
	{
		GetDlgItem(IDC_DGN_CRC_POSC_CLASS3_TYPE_C_GRP)->GetWindowRect(rRef);		
	}
	else if (m_iDgnCode == TMH07_3_1989)
	{
		//GetDlgItem(IDC_DGN_CRC_POSC_CLASS3_TYPE_C_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CRC_POSC_SHEAR_STC)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CRC_POSC_CLASS3_TYPE_C_GRP)->ShowWindow(False);
		GetDlgItem(IDC_DGN_CRC_POSC_CLASS3_TYPE_C_CHK)->ShowWindow(False);
		GetDlgItem(IDC_DGN_CRC_POSC_CLASS_GRP)->GetWindowRect(rRef);
        //GetDlgItem(IDC_DGN_CPG_CPUL_IJBOTH_CHK)->GetWindowRect(rRef);
       

        GetDlgItem(IDC_DGN_CPG_CPUL_IJBOTH_CHK)->GetWindowRect(rToMove);
        nDistY = rRef.bottom - rToMove.top + 8;
        CDlgUtil::CtrlMoveDistY(this, aControls1, nDistY);
        GetDlgItem(IDC_DGN_CPG_CPUL_TAB)->GetWindowRect(rRef);
	}
    else
    {
        GetDlgItem(IDC_DGN_CRC_POSC_SHEAR_STC)->GetWindowRect(rRef);
    }

    GetDlgItem(IDC_DGN_EXECUTE)->GetWindowRect(rToMove);
    nDistY = rRef.bottom - rToMove.top + 8;
    CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

    // resize self size
    CWnd* pWndLast = GetDlgItem(IDC_DGN_EXECUTE);
    CRect rectLast;
    pWndLast->GetWindowRect(rectLast);
    CRect r;
    GetWindowRect(r);
    r.bottom = rectLast.bottom+16;
    MoveWindow(r);
}

void CCRCDgnOption::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
    HWND hWnd = GetSafeHwnd();
    if (!hWnd || !IsWindow(hWnd)) return;
    double dFlen = 0.0;

    switch(lHint)
    {
    case D_UPDATE_DEFAULT:
        // do something...
        break;
    case D_UPDATE_BUFFER_BEFORE:
        // do something...
        break;
    case D_UPDATE_UNIT:
        // do something...
        break;
    case D_UPDATE_SEL_ADD:
        // do something...
        break;
    case D_UPDATE_SEL_DEL:
        // do something...
        break;
    case D_UPDATE_BUFFER_AFTER:
        // do something...
        UpdateBuffer();
        break;
    case D_UPDATE_PREF_CHANGED:
        break;
    default:
        //ASSERT(FALSE);
        break;
    }
}

void CCRCDgnOption::UpdateBuffer()
{
    CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;		
    T_UDRD_BUFFER buffer_ur;
    POSITION pos = pViewBuff->GetStartBuffer();
    while(pos)
    {
        buffer_ur = pViewBuff->GetNextBuffer(pos);
        int nCmd = buffer_ur.nCmd;
        int nKey = buffer_ur.nKey;

        switch(nCmd)
        {
        case(UR_PSCD_ADD):
        case(UR_PSCD_DEL):
            {
                T_PSCD_D PscdD;
                if(!m_pDoc->m_pAttrCtrl->GetDgnPscd(PscdD))
                {
                    T_PREFERENCE rPref;
                    rPref.Initialize();
                    m_pDoc->m_pInitCtrl->GetPreference(rPref);	
                    m_iDgnCode = CDBLib::GetPscCodeNo(rPref.DgnCode.PSCCode);
                }
                else
                {
                    m_iDgnCode = PscdD.iDgnCode;
                }

                Initial_Data();
				EnableDisableControls();
                ShowHideControls();
                ResizeDialog();
                break;      
            }
        default:	break;
        }
    }
}

void CCRCDgnOption::OnDgnClassRdo()
{
	EnableDisableControls();
}

void CCRCDgnOption::OnDgnCRCPoscAr() 
{
    m_nOption  = 0;			//	add/replace
    UpdateData(FALSE);
}

void CCRCDgnOption::OnDgnCRCPoscDel() 
{
    m_nOption  = 1;			//	Delete
    UpdateData(FALSE);
}

