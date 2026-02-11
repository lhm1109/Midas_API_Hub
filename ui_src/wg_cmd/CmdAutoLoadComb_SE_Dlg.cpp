// CmdAutoLoadComb_SE_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadComb_SE_Dlg.h"
#include "LoadCombCtrl.h"
#include "LoadCaseWindDirectDlg.h"
#include "CmdLcomGravityDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_main\wg_mainres2.h"	

#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"
#include "..\wg_dbLock\LockMgr.h"   // for CLockMgr
#include "..\wg_dbLock\LockOption.h"
#include "..\wg_dbLock\LockUtil.h"

#include "CmdSetOrthoLCDlg.h"

#include "..\wg_dgn\Dgn_ServiceDef.h"
#include "..\wg_base\ServiceProvider.h"
//#include "..\wg_dgn\DgnGenSeisFactor.h"
//#include "..\wg_dgn\wg_dgnAll.h"

#include "CMDLoadCalcAlphaH.h"
#include "CmdLcomPsiFactorDlg.h"
#include "CmdAutoLoadCombImperfectionLoadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadComb_SE_Dlg dialog

enum { COLINDEX_NAME=0, COLINDEX_J, COLINDEX_TYPE };

CCmdAutoLoadComb_SE_Dlg::CCmdAutoLoadComb_SE_Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdAutoLoadComb_SE_Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdAutoLoadComb_SE_Dlg)
	m_nOption = -1;
	m_nDesignType = EN_SEIS_EVL;
	m_Factor = 0.0;
	//}}AFX_DATA_INIT
}


void CCmdAutoLoadComb_SE_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdAutoLoadComb_SE_Dlg)
	DDX_Radio(pDX, IDC_CMD_LOADCOMB_ADD, m_nOption);
	DDX_Control(pDX, IDC_CMD_DESIGN_OTHO_CHK, m_wndOthoChk);
	DDX_Control(pDX, IDC_CMD_LOADCOMB_CODE, m_Code);
	DDX_Radio(pDX, IDC_CMD_DESIGN_OTHO_LCB_10030_RDO, m_nOthoType);
	DDX_Control(pDX, IDC_CMD_DESIGN_VERT_SEIS_CHK, m_wndVertForceChk);
//   DDX_Control(pDX, IDC_CMD_J_LCASE, m_SpecLcase);
//   DDX_Control(pDX, IDC_CMD_J_LIST, m_CtrlList);
//   DDX_Text(pDX, IDC_CMD_J_FACTOR, m_Factor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadComb_SE_Dlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdAutoLoadComb_SE_Dlg)
	ON_BN_CLICKED(IDC_CMD_LOADCOMB_ADD, OnCmdLoadcombAdd)
	ON_CBN_SELCHANGE(IDC_CMD_LOADCOMB_CODE, OnCodeSelchange)
	ON_BN_CLICKED(IDC_CMD_DESIGN_OTHO_CHK, OnCmdOthoChk)
	ON_BN_CLICKED(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO, OnCmdOthoType)
	ON_BN_CLICKED(IDC_CMD_DESIGN_VERT_SEIS_CHK, OnCmdSpecialVertChk)
	ON_BN_CLICKED(IDC_CMD_DESIGN_SEIS_FACTOR_BTN, OnCmdSeisFactorBtn)
	ON_BN_CLICKED(IDC_CMD_GRAVITY_BTN, OnCmdLFforGravityBtn)
	ON_BN_CLICKED(IDC_CMD_DESIGN_OTHOLC_SET_BTN, OnCmdSetOtholcBtn)
	ON_BN_CLICKED(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO, OnCmdOthoType)

//   ON_BN_CLICKED(IDC_CMD_J_ADD, OnCmd_J_Add)
//   ON_BN_CLICKED(IDC_CMD_J_DEL, OnCmd_J_Del)
//   ON_BN_CLICKED(IDC_CMD_J_MODIFY, OnCmd_J_Modify)
//   ON_NOTIFY(NM_CLICK, IDC_CMD_J_LIST, OnClickCmd_J_List)
//   ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_J_LIST, OnItemchangedCmd_J_List)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadComb_SE_Dlg message handlers

BOOL CCmdAutoLoadComb_SE_Dlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	// 변수들을 초기화합니다.
	m_nOthoType = 0;
	m_strCodeName = _T("");
	m_strOldCodeName = _T("KISTEC2013");//

	Initial_Data();  
	CString strCodeName=_T("");
	int Index = m_Code.GetCurSel();
	m_Code.GetLBText(Index,strCodeName);

	int nSeisOrthoType = GetSeisOrthoType(strCodeName);
	switch (nSeisOrthoType)
	{
	case EN_SEIS_ORTHO_BOTH:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(TRUE);
		}
		break;
	case EN_SEIS_ORTHO_SRSS:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(TRUE);
			m_wndOthoChk.SetCheck(0);
			m_nOthoType = 1;
		}
		break;
	case EN_SEIS_ORTHO_100_30:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(TRUE);
			m_wndOthoChk.SetCheck(0);
			m_nOthoType = 0;
		}
		break;
	default:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(FALSE);
			m_wndOthoChk.SetCheck(0);
		}
		break;
	}

	GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
	GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(m_wndOthoChk.GetCheck()?TRUE:FALSE);
	GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);

//   Initial_LCaseComboBox();  
	Initial_Data();  
//   Initial_ListCtrlBox();
	AlignControl(strCodeName);
	CtrlManager();

	return TRUE;
}

BOOL CCmdAutoLoadComb_SE_Dlg::GetLoadCombTypeByLoadCaseType(int nLoadCaseType, int& nAnalType)
{
	switch(nLoadCaseType)
	{
	case D_LOADCASE_STATIC			: nAnalType = D_LCOM_STATIC;		break;
	case D_LOADCASE_SPECTRUM		: nAnalType = D_LCOM_SPECTRUM;		break;
	case D_LOADCASE_SPECTRUM_ND		: nAnalType = D_LCOM_SPECTRUM_ND;	break;
	case D_LOADCASE_HISTORY			: nAnalType = D_LCOM_HISTORY;		break;
	case D_LOADCASE_MOVING			: nAnalType = D_LCOM_MOVING;		break;
	case D_LOADCASE_SETTLE			: nAnalType = D_LCOM_SETTLEMENT;	break;
	case D_LOADCASE_ENVELOPE		: nAnalType = D_LCOM_ENVELOPE;		break;
	case D_LOADCASE_COMB_GENERAL	: nAnalType = D_LCOM_CB_GENERAL;	break;
	case D_LOADCASE_COMB_STEEL		: nAnalType = D_LCOM_CB_STEEL;		break;
	case D_LOADCASE_COMB_CONCRETE	: nAnalType = D_LCOM_CB_CONCRETE;	break;
	case D_LOADCASE_COMB_FDN		: nAnalType = D_LCOM_CB_FOOTING;	break;
	case D_LOADCASE_COMB_SRC		: nAnalType = D_LCOM_CB_SRC;		break;
	case D_LOADCASE_COMB_ALUMINUM	: nAnalType = D_LCOM_CB_ALUMINUM;	break;
	case D_LOADCASE_COMB_STLCOMP	: nAnalType = D_LCOM_CB_STLCOMP;	break;
	case D_LOADCASE_PUSHOVER		: nAnalType = D_LCOM_PUSHOVER;		break;
	case D_LOADCASE_STAGE			: nAnalType = D_LCOM_STAGE;			break;
	case D_LOADCASE_ECCENSPEC		: nAnalType = D_LCOM_ECCENSPEC;		break;
	case D_LOADCASE_ECCENSPEC_ND	: nAnalType = D_LCOM_ECCENSPEC_ND;	break;
	case D_LOADCASE_GRIDMODEL		: nAnalType = D_LCOM_GRIDMODEL;		break;
	case D_LOADCASE_CRANE			: nAnalType = D_LCOM_CRANE;			break;      
	default:  ASSERT(0);    return FALSE;
	}
	return TRUE;
}

void CCmdAutoLoadComb_SE_Dlg::SetDefaultGravData()
{
	m_pDoc->m_pAttrCtrl->GetStldKeyListVariableParam(m_aSelGravLoad, 4, _T("D"), _T("L"), _T("LR"), _T("S"));
}

void CCmdAutoLoadComb_SE_Dlg::Initial_Data()
{
	if(m_nOption!=0 && m_nOption!=1) 
		m_nOption = 0;  //  0 : Add,   1 : Replace
	m_Code.ResetContent();

	Set_InitCode();
	m_Factor = 1.;
	SetDefaultGravData();

	UpdateData(FALSE);
}

// Coded by Seungjun ('20070719) MNet:No.2915.
void CCmdAutoLoadComb_SE_Dlg::Set_InitCode()
{
	int nNationalAnnex = -1;
	CString strCurrentCode=_T("");
	CStringArray GenCodeName;	  

	m_aCodeEnable.RemoveAll();
	if(m_nDesignType == EN_SEIS_EVL) //
	{
		CDBLib::GetSeisEvalCodeNameList(GenCodeName);
		for (int i=0; i<GenCodeName.GetSize(); i++)	m_Code.AddString(GenCodeName.GetAt(i));  

		if(m_pDoc->m_pAttrCtrl2->ExistSeme())
		{
			T_SEME_D SemeD; SemeD.Initialize();
			m_pDoc->m_pAttrCtrl2->GetSeme(SemeD);
			strCurrentCode = CDBLib::GetSeisEvalCodeNameByMethod(SemeD.nMethod);
		}
		else
		{
			if (GenCodeName.GetSize() > 0) strCurrentCode = GenCodeName[0];
		}
	}

	int Index = m_Code.FindString(-1, strCurrentCode);
	if(Index==-1)	Index = 0;
	m_Code.SetCurSel(Index);

	m_nOldCodeIndex = Index;
}

void CCmdAutoLoadComb_SE_Dlg::OnCmdDesignTypeRdo()
{
	UpdateData(TRUE);
	Initial_Data();

	CString strCodeName=_T("");
	int Index = m_Code.GetCurSel();
	m_Code.GetLBText(Index,strCodeName);

	AlignControl(strCodeName);

	UpdateData(FALSE);
}

void CCmdAutoLoadComb_SE_Dlg::OnCmdLoadcombAdd() 
{
	m_nOption = 0;
}

void CCmdAutoLoadComb_SE_Dlg::OnCmdLoadcombReplace() 
{
	m_nOption = 1;
}

void CCmdAutoLoadComb_SE_Dlg::OnOK() 
{
	UpdateData(TRUE);

	CString strCodeName=_T("");
	int Index = m_Code.GetCurSel();
	m_Code.GetLBText(Index,strCodeName);

	CString csErrMsg;
	int nType;
	if (m_nDesignType == EN_SEIS_EVL ) nType = CLoadDgnCodeChecker::eLCBSeisEvaluation;
	else { ASSERT(0); return; }
	BOOL bAllow = CLoadDgnCodeChecker::CheckCodeAndErrorMsgST(nType, strCodeName, csErrMsg);
	if (!bAllow)
	{
		AfxMessageBox(csErrMsg);
		return;
	}

//   SaveLfcaListData();

	if(CDBLib::IsAutoLoadCombCodeName(strCodeName))
	{
		CLoadCombCtrl LoadCombCtrl;
		LoadCombCtrl.m_nDesignType = m_nDesignType;
		LoadCombCtrl.Set_LcomType(m_nLcomType);

		LoadCombCtrl.m_nOrthoType	 = (m_wndOthoChk.GetCheck() ? m_nOthoType	: -1);
		LoadCombCtrl.SetSlcmVertFlag(m_wndVertForceChk.GetCheck() ? TRUE : FALSE);

		/// set parameter for seismic evaluation.
		LoadCombCtrl.m_aSelGravLoad.RemoveAll();
		for (int i=0; i<m_aSelGravLoad.GetSize(); ++i)
		{
			LoadCombCtrl.m_aSelGravLoad.Add(m_aSelGravLoad[i]);
		}

		LoadCombCtrl.m_aEffectiveLoad.RemoveAll();
		for (int i=0; i<m_aEffectiveLoad.GetSize(); ++i)
		{
			LoadCombCtrl.m_aEffectiveLoad.Add(m_aEffectiveLoad[i]);
		}

		BOOL bOK = LoadCombCtrl.Set_DefaultLoadComb(m_nOption, strCodeName);

		if(bOK)	CDialogMove::OnOK();
	}
	else	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Generation_code_unavailable_),MB_OK);
}

void CCmdAutoLoadComb_SE_Dlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CCmdAutoLoadComb_SE_Dlg::AlignControl(CString strCurCode)
{

}

void CCmdAutoLoadComb_SE_Dlg::CtrlManager()
{
	UpdateData(FALSE);
}

//add by cylee 05.04.22
void CCmdAutoLoadComb_SE_Dlg::OnCmdSetOtholcBtn()
{
	CCmdSetOrthoLCDlg dlg;
	dlg.SetSeismicEvaluation(TRUE);
	dlg.DoModal();
}

void CCmdAutoLoadComb_SE_Dlg::OnCmdOthoChk()
{
	CString strCodeName=_T("");
	int Index = m_Code.GetCurSel();
	m_Code.GetLBText(Index,strCodeName);

	BOOL bCheck = m_wndOthoChk.GetCheck();
	GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(bCheck);
	GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(bCheck);
}

void CCmdAutoLoadComb_SE_Dlg::OnCmdOthoType()
{
	// if(m_nOthoType==0) m_=2;
}

//add by cylee 05.05.21
void CCmdAutoLoadComb_SE_Dlg::OnCodeSelchange()
{
	CString strCodeName=_T("");
	int Index = m_Code.GetCurSel();
	m_Code.GetLBText(Index,strCodeName);
	m_strCodeName = strCodeName;

	if(!m_aCodeEnable.IsEmpty() && m_aCodeEnable[Index]==FALSE)
	{
		CString strCodeName = _T("");
		m_Code.GetLBText(Index, strCodeName);
		CString strErrMsg = _T("");
		strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), strCodeName);
		GSaveHistoryFormatNF(strErrMsg);
		m_Code.SetCurSel(m_nOldCodeIndex);

		return;
	}

	m_nOldCodeIndex = Index;

	int nSeisOrthoType = GetSeisOrthoType(strCodeName);
	switch (nSeisOrthoType)
	{
	case EN_SEIS_ORTHO_BOTH:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(TRUE);
			m_wndOthoChk.SetCheck(0);
			GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
		}
		break;
	case EN_SEIS_ORTHO_SRSS:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(TRUE);
			m_wndOthoChk.SetCheck(0);
			GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(FALSE);
			m_nOthoType = 1;
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
		}
		break;
	case EN_SEIS_ORTHO_100_30:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(TRUE);
			m_wndOthoChk.SetCheck(0);
			GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(FALSE);
			m_nOthoType = 0;
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
		}
		break;
	default:
		{
			GetDlgItem(IDC_CMD_DESIGN_OTHO_CHK)->EnableWindow(FALSE);
			m_wndOthoChk.SetCheck(0);
			GetDlgItem(IDC_CMD_DESIGN_OTHOLC_SET_BTN)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_10030_RDO)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_DESIGN_OTHO_LCB_SRSS_RDO)->EnableWindow(FALSE);
		}
		break;
	}

	UpdateData(FALSE);
	AlignControl(strCodeName);
	CtrlManager();
}

//add by cylee 05.04.22
void CCmdAutoLoadComb_SE_Dlg::OnCmdSpecialVertChk()
{
	//	m_wndSpecialChk = 0 or 1
	//  m_wndVertForceChk = 0 or 1
}

void CCmdAutoLoadComb_SE_Dlg::OnCmdSeisFactorBtn()
{
	const BOOL bHideSpecialGroup = TRUE;
	
	_DGN_GEN_SEIS_FACTOR_DATA SeisFactor;
	SeisFactor.bHideSpecialGroup = bHideSpecialGroup;
	SeisFactor.strConCode = m_strCodeName;

	CServiceHeadOffice* pHeadOffice = CServiceHeadOffice::Instance();
	pHeadOffice->ReqService(_ULS(dgn), EN_DGNSA_GEN_SEIS_FACTOR, (void*)&SeisFactor);
}

int CCmdAutoLoadComb_SE_Dlg::GetSeisOrthoType(const CString& strCodeName)
{
	if (strCodeName == _T("KISTEC2013")) return EN_SEIS_ORTHO_BOTH;
    if (strCodeName == _T("KISTEC2019")) return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("MOE2018"))     return EN_SEIS_ORTHO_BOTH;
	if (strCodeName == _T("MOE2019"))     return EN_SEIS_ORTHO_BOTH;

	ASSERT(0);
	return EN_SEIS_ORTHO_NONE;
}

int CCmdAutoLoadComb_SE_Dlg::GetSeisAddLcomType(const CString& strCodeName)
{
	if (strCodeName == _T("KISTEC2013")) return EN_SEIS_LCOMADD_BOTH;
    if (strCodeName == _T("KISTEC2019")) return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("MOE2018"))     return EN_SEIS_LCOMADD_BOTH;
	if (strCodeName == _T("MOE2019"))     return EN_SEIS_LCOMADD_BOTH;

	ASSERT(0);
	return EN_SEIS_LCOMADD_NONE;
}

void CCmdAutoLoadComb_SE_Dlg::OnCmdLFforGravityBtn()
{
	CCmdLcomGravityDlg dlg;
	dlg.SetData(m_aSelGravLoad, m_aUnselGravLoad, m_aEffectiveLoad);
	if(dlg.DoModal()==IDOK)
	{
		dlg.GetData(m_aSelGravLoad, m_aUnselGravLoad, m_aEffectiveLoad);
	}
}

// void CCmdAutoLoadComb_SE_Dlg::OnCmd_J_Add() 
// {
//   UpdateData(TRUE);
//   if(ErrorCheck())
//   {
//     //  LFCA의 순번을 얻어옵니다.
//     int ListNo;
//     int Index = m_SpecLcase.GetCurSel();
//     int nType=CDlgUtil::CobxGetCurSelItemData(m_SpecLcase, Index);
//     if(Index != -1)
//     {
//       CString strName=_T("");
//       m_SpecLcase.GetLBText(Index,strName);
//       if(CheckLCName(strName,ListNo))  //  TRUE(같은 이름이 존재하지 않을 때)=Add
//       {
//         //  List Control Box에 작성합니다.
//         int No = m_CtrlList.GetItemCount();
//         Write_ListCtrlBox(No,m_Factor,strName,nType);
//       }
//       else                        //  FALSE(같은 이름이 존재할 때)=Modify or Cancel
//       {
//         if(AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Existed_Load_Case_____nM),MB_YESNO)==IDYES)
//         {
//           CString strName=_T("");
//           m_SpecLcase.GetLBText(Index,strName);
//           //  List Control Box에 재작성합니다.
//           OverWrite_ListCtrlBox(ListNo,m_Factor,strName,nType);
// 
//           m_CtrlList.SetItemState(ListNo,LVIS_FOCUSED | LVIS_SELECTED,LVIS_SELECTED | LVIS_FOCUSED);
//         }
//       }
//     }
//     else
//       AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Loa),MB_OK);
//   }
// }

// BOOL CCmdAutoLoadComb_SE_Dlg::CheckLCName(CString str,int& Index)
// {
//   BOOL bCheck=TRUE;
//   int Count = m_CtrlList.GetItemCount();
//   for(int i=0; i<Count; i++)
//   {
//     CString strLcName = m_CtrlList.GetItemText(i,COLINDEX_NAME);
//     if(str == strLcName)  
//     {
//       Index = i;
//       bCheck = FALSE;
//     }
//   }
//   return bCheck;
// }

// void CCmdAutoLoadComb_SE_Dlg::OnCmd_J_Del() 
// {
//   int nItem = m_CtrlList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
//   if(nItem == -1)
//   {
//     AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Data____));
//     return;
//   }
// 
//   m_CtrlList.DeleteItem(nItem);
// 
//   int nCount = m_CtrlList.GetItemCount();
//   if (nCount <= nItem) nItem -= 1;
//   if (nItem >= 0) 
//     m_CtrlList.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
// }

// void CCmdAutoLoadComb_SE_Dlg::OnCmd_J_Modify() 
// {
//   int nItem = m_CtrlList.GetNextItem(-1,LVNI_ALL | LVNI_SELECTED);
//   if(nItem == -1)
//   {
//     AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Data____));
//     return;
//   }
// 
//   UpdateData(TRUE);
//   if(ErrorCheck())
//   {
//     //  LFCA의 순번을 얻어옵니다.
//     int Index = m_SpecLcase.GetCurSel();
//     int nType = CDlgUtil::CobxGetCurSelItemData(m_SpecLcase, nItem);
//     if(Index != -1)
//     {
//       CString strName=_T("");
//       m_SpecLcase.GetLBText(Index,strName);
// 
//       //  List Control Box에 재작성합니다.
//       OverWrite_ListCtrlBox(nItem,m_Factor,strName,nType);
// 
//       m_CtrlList.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
//     }
//     else
//       AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Check_Load_Case____),MB_OK);
//   }
// }

BOOL CCmdAutoLoadComb_SE_Dlg::ErrorCheck()
{
	BOOL bCheck=TRUE;

	if(m_Factor < 0. || m_Factor > 10.)  bCheck = FALSE;
	if(!bCheck)	
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Check_J_Factor____),MB_OK); return bCheck;
	}

	if(m_Factor < 1. && m_Factor > 0.)  bCheck = FALSE;
	if(!bCheck)	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Warning___Check_J_Factor__),MB_OK);

	return TRUE;
}

// void CCmdAutoLoadComb_SE_Dlg::Write_ListCtrlBox(int Index,double Factor,CString strLoadCase,int nType)
// {
//   LV_ITEM lvitem;
// 
//   //  ListCtrl Box에 값을 입력합니다.
//   lvitem.mask		  = LVIF_TEXT;
//   lvitem.iItem  	= Index;
//   lvitem.iSubItem = COLINDEX_NAME;
//   lvitem.pszText	= (LPTSTR)(LPCTSTR)strLoadCase;
//   m_CtrlList.InsertItem(&lvitem);
// 
//   CString strFactor=_T("");
//   strFactor.Format(_T("%10.3f"),Factor);
//   lvitem.mask 		= LVIF_TEXT;
//   lvitem.iItem  	= Index;
//   lvitem.iSubItem = COLINDEX_J;
//   lvitem.pszText	= (LPTSTR)(LPCTSTR)strFactor;
//   m_CtrlList.SetItem(&lvitem);
// 
//   CString strType=_T("");
//   strType.Format(_T("%d"),nType);
//   lvitem.mask 		= LVIF_TEXT;
//   lvitem.iItem  	= Index;
//   lvitem.iSubItem = COLINDEX_TYPE;
//   lvitem.pszText	= (LPTSTR)(LPCTSTR)strType;
//   m_CtrlList.SetItem(&lvitem);
// }

// void CCmdAutoLoadComb_SE_Dlg::OverWrite_ListCtrlBox(int Index,double Factor,CString strLoadCase,int nType)
// {
//   //  ListCtrl Box에 값을 입력합니다.
//   m_CtrlList.SetItemText(Index,COLINDEX_NAME,strLoadCase);
// 
//   CString strFactor=_T("");
//   strFactor.Format(_T("%10.3f"),Factor);
//   m_CtrlList.SetItemText(Index,COLINDEX_J,strFactor);
// 
//   CString strType;
//   strType.Format(_T("%d"), nType);
//   m_CtrlList.SetItemText(Index,COLINDEX_TYPE,strType);
// }

// void CCmdAutoLoadComb_SE_Dlg::OnClickCmd_J_List(NMHDR* pNMHDR, LRESULT* pResult) 
// {
//   *pResult = 0;
// }
// 
// void CCmdAutoLoadComb_SE_Dlg::OnItemchangedCmd_J_List(NMHDR* pNMHDR, LRESULT* pResult) 
// {
//   NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//   int nItem = pNMListView->iItem;
//   CString strLcName = m_CtrlList.GetItemText(nItem,COLINDEX_NAME);
//   CString strFactor = m_CtrlList.GetItemText(nItem,COLINDEX_J);
//   if(strFactor != _T("") && strLcName != _T(""))
//   {
//     m_Factor = _tstof(strFactor);
//     int Index = m_SpecLcase.FindString(-1,strLcName);
//     if(Index != -1) m_SpecLcase.SetCurSel(Index);
// 
//     UpdateData(FALSE);
//   }
//   *pResult = 0;
// }

// void CCmdAutoLoadComb_SE_Dlg::Initial_LCaseComboBox()
// {
//   m_SpecLcase.ResetContent();
// 
//   CDBDoc* pDoc = CDBDoc::GetDocPoint();
//   CArray<T_LFCA_K,T_LFCA_K> aLfcaK;
//   CArray<T_LFCA_D,T_LFCA_D&> aLfcaD;
//   CArray<CString,CString&> aName;
// 
//   // Seismic Load
//   CArray<T_PSLL_K,T_PSLL_K> aPsllK;
//   pDoc->m_pAttrCtrl->GetPsllKeyList(aPsllK);
//   for(int i=0; i<aPsllK.GetSize(); i++)
//   {
//     T_LFCA_K LfcaK = {0};
//     LfcaK.key.type = D_LFCA_TYPE_PSLL;
//     LfcaK.key.objK = aPsllK[i];
// 
//     T_LFCA_D LfcaD;
//     pDoc->m_pAttrCtrl->GetLfcaDefault(LfcaK, LfcaD);
// 
//     T_STLD_D StldD;
//     pDoc->m_pAttrCtrl->GetStld(LfcaK.key.objK, StldD);
// 
//     aLfcaK.Add(LfcaK);
//     aLfcaD.Add(LfcaD);
//     aName.Add(StldD.LoadCaseName);
//   }
// 
//   // Spectrum Load
//   CArray<T_SPLC_K,T_SPLC_K> aSplcK;
//   pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcK);
//   for(int i=0; i<aSplcK.GetSize(); i++)
//   {
//     T_LFCA_K LfcaK = {0};
//     LfcaK.key.type = D_LFCA_TYPE_SPLC;
//     LfcaK.key.objK = aSplcK[i];
// 
//     T_LFCA_D LfcaD;
//     pDoc->m_pAttrCtrl->GetLfcaDefault(LfcaK, LfcaD);
// 
//     T_SPLC_D SplcD;
//     pDoc->m_pAttrCtrl->GetSplc(LfcaK.key.objK, SplcD);
// 
//     aLfcaK.Add(LfcaK);
//     aLfcaD.Add(LfcaD);
//     aName.Add(SplcD.LoadCaseName);
//   }
// 
//   for(int i=0; i<aName.GetSize(); i++)
//     CDlgUtil::CobxAddItem(m_SpecLcase, aName[i], aLfcaK[i].key.type);
//   m_SpecLcase.SetCurSel(0);
// }

// void CCmdAutoLoadComb_SE_Dlg::Initial_ListCtrlBox()
// {
//   DWORD dwStyle;
//   dwStyle = ListView_GetExtendedListViewStyle(m_CtrlList.GetSafeHwnd());
//   dwStyle |= LVS_EX_FULLROWSELECT; 
//   ListView_SetExtendedListViewStyle(m_CtrlList.GetSafeHwnd(),dwStyle);
// 
//   LV_COLUMN lvcolumn;
//   CString aTitle[] = { _LS(IDS_WG_CMD__ADDD__Load_Case), _T("J"), _T("Type") };
// 
//   CString title;
//   int width[] = {80, 60, 0};
//   for(int i = 0; i < sizeof(aTitle) / sizeof(CString); i++)
//   {
//     title = aTitle[i];
// 
//     lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
//     if(i==0) lvcolumn.fmt      = LVCFMT_LEFT;
//     else     lvcolumn.fmt      = LVCFMT_CENTER;
//     lvcolumn.pszText  = title.GetBuffer(0);
//     lvcolumn.iSubItem = i;
//     lvcolumn.cx       = width[i];
//     m_CtrlList.InsertColumn(i,&lvcolumn);
//     title.ReleaseBuffer();
//   }
// 
//   CArray<T_LFCA_K,T_LFCA_K> aLfcaK;
//   CDBDoc* pDoc = CDBDoc::GetDocPoint();
//   pDoc->m_pAttrCtrl->GetLfcaKeyList(aLfcaK);
//   for(int i=0; i<aLfcaK.GetSize(); i++)
//   {
//     T_LFCA_D LfcaD;
//     if(!pDoc->m_pAttrCtrl->GetLfca(aLfcaK[i], LfcaD)) { ASSERT(0); continue; }
// 
//     CString strName;
//     if(aLfcaK[i].key.type == D_LFCA_TYPE_SPLC)
//     {
//       T_SPLC_D SplcD;
//       if(!pDoc->m_pAttrCtrl->GetSplc(aLfcaK[i].key.objK, SplcD)) { ASSERT(0); continue; }
//       strName = SplcD.LoadCaseName;
//     }
//     else
//     {
//       T_STLD_D StldD;
//       if(!pDoc->m_pAttrCtrl->GetStld(aLfcaK[i].key.objK, StldD)) { ASSERT(0); continue; }
//       strName = StldD.LoadCaseName;
//     }
// 
//     Write_ListCtrlBox(i, LfcaD.dJ, strName, aLfcaK[i].key.type);
//   }
// }

// void CCmdAutoLoadComb_SE_Dlg::SaveLfcaListData()
// {
//   // 다 지우고 새로 할당. 데이터 갯수 많지 않을 것임
//   CDBDoc* pDoc = CDBDoc::GetDocPoint();
//   if(!pDoc->m_pUndoCtrl->StartEditDB(_LS(IDS_DB_SEIS_EVAL_J_UPDATE), CMDTYPE_FREE)) return;
//     
//   pDoc->m_pEditData->DelAllLfca();
// 
//   for(int i=0; i<m_CtrlList.GetItemCount(); i++)
//   {
//     CString strLcName = m_CtrlList.GetItemText(i,COLINDEX_NAME);
//     CString strJValue = m_CtrlList.GetItemText(i,COLINDEX_J);
//     CString strType   = m_CtrlList.GetItemText(i,COLINDEX_TYPE);
// 
//     T_LFCA_K LfcaK;
//     LfcaK.key.type = _ttoi(strType);
//     T_LFCA_D LfcaD;
//     LfcaD.dJ = _ttof(strJValue);
// 
//     if(LfcaK.key.type==D_LFCA_TYPE_SPLC)
//       LfcaK.key.objK = pDoc->m_pAttrCtrl->GetSplcKey(strLcName);
//     else
//       LfcaK.key.objK = pDoc->m_pAttrCtrl->GetStldKey(strLcName);
// 
//     if(!pDoc->m_pEditData->AddLfca(LfcaK,LfcaD)) { ASSERT(0); continue; }
//   }
//   pDoc->m_pUndoCtrl->CloseEditDB(TRUE);
// }