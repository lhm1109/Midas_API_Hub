// Coded by Seungjun MNet:No.2393 ('20060829)
// DgnConServiceLimtDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConReinfoShearDlg.h"

#include "DgnStruct.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_dbLock\LockOption.h"
#include "..\wg_db\DBCodeDef.h"

#include "..\wg_main\wg_mainres2.h"	


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConReinfoShearDlg dialog


CDgnConReinfoShearDlg::CDgnConReinfoShearDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConReinfoShearDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConReinfoShearDlg)
	m_aDataCtrl.Add(IDC_STATIC_FRAME_TYPE1);
	m_aDataCtrl.Add(IDC_STATIC_FRAME_TYPE2);
	m_aDataCtrl.Add(IDC_DGN_REIN_LONG_CHK);
	m_aDataCtrl.Add(IDC_DGN_REIN_LONG_EDT);
	m_aDataCtrl.Add(IDC_DGN_REIN_LONG_UNT);
	m_aDataCtrl.Add(IDC_DGN_REIN_SHOR_CHK);
	m_aDataCtrl.Add(IDC_DGN_REIN_SHOR_EDT);
	m_aDataCtrl.Add(IDC_DGN_REIN_SHOR_UNT);
	m_aDataCtrl.Add(IDC_DGN_REIN_RATIO_STC);
	m_aDataCtrl.Add(IDC_DGN_REIN_RATIO_EDT);
	m_aDataCtrl.Add(IDC_DGN_REIN_RATIO_UNT);
	m_aDataCtrl.Add(IDC_DGN_ALPHA_LONG_CHK);
	m_aDataCtrl.Add(IDC_DGN_ALPHA_SHOR_CHK);
	m_aDataCtrl.Add(IDC_DGN_REIN_SHOR_UNT);
	m_aDataCtrl.Add(IDC_DGN_REIN_SHOR_UNT);
	m_aDataCtrl.Add(IDC_DGN_REIN_SHOR_UNT);
	m_aDataCtrl.Add(IDC_DGN_REIN_SHOR_UNT);
	m_aDataCtrl.Add(IDC_STATIC_FRAME_TYPE3);
	m_aDataCtrl.Add(IDC_STATIC_FRAME_TYPE4);
	m_aDataCtrl.Add(IDC_DGN_ALPHA_LONG_AUTO_RDO);
	m_aDataCtrl.Add(IDC_DGN_ALPHA_SHOR_AUTO_RDO);
	m_aDataCtrl.Add(IDC_DGN_ALPHA_LONG_USER_RDO);
	m_aDataCtrl.Add(IDC_DGN_ALPHA_SHOR_USER_RDO);
	m_aDataCtrl.Add(IDC_DGN_ALPHA_LONG_EDT);
	m_aDataCtrl.Add(IDC_DGN_ALPHA_SHOR_EDT);
	m_aDataCtrl.Add(IDC_STATIC_FRAME_TYPE5);
	m_aDataCtrl.Add(IDC_DGN_SHORT_REDUCT_YES_RDO);
	m_aDataCtrl.Add(IDC_DGN_SHORT_REDUCT_NO_RDO);

	m_aLongAlpha.Add(IDC_DGN_ALPHA_LONG_AUTO_RDO);
	m_aLongAlpha.Add(IDC_DGN_ALPHA_LONG_USER_RDO);
	m_aLongAlpha.Add(IDC_DGN_ALPHA_LONG_EDT);
	m_aShorAlpha.Add(IDC_DGN_ALPHA_SHOR_AUTO_RDO);
	m_aShorAlpha.Add(IDC_DGN_ALPHA_SHOR_USER_RDO);
	m_aShorAlpha.Add(IDC_DGN_ALPHA_SHOR_EDT);

	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnConReinfoShearDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConReinfoShearDlg)
	DDX_Control(pDX, IDC_DGN_REIN_LONG_EDT, m_edtLongRein);
	DDX_Control(pDX, IDC_DGN_REIN_SHOR_EDT, m_edtShorRein);
	DDX_Control(pDX, IDC_DGN_REIN_LONG_UNT, m_untLongRein);
	DDX_Control(pDX, IDC_DGN_REIN_SHOR_UNT, m_untShorRein);

	DDX_Control(pDX, IDC_DGN_REIN_RATIO_EDT, m_edtShearRatio);

	DDX_Control(pDX, IDC_DGN_ALPHA_LONG_EDT, m_edtLongAlpha);
	DDX_Control(pDX, IDC_DGN_ALPHA_SHOR_EDT, m_edtShorAlpha);

	DDX_Check(pDX, IDC_DGN_REIN_LONG_CHK, m_bLongRein);
	DDX_Check(pDX, IDC_DGN_REIN_SHOR_CHK, m_bShorRein);

	DDX_Check(pDX, IDC_DGN_ALPHA_LONG_CHK, m_bLongAlpha);
	DDX_Check(pDX, IDC_DGN_ALPHA_SHOR_CHK, m_bShorAlpha);

	DDX_Radio(pDX, IDC_DGN_CON_SERV_AR ,   m_nOption );
	DDX_Radio(pDX, IDC_DGN_CONC_SELECT_ALL, m_iSeletType);

	DDX_Radio(pDX, IDC_DGN_ALPHA_LONG_AUTO_RDO, m_nLongAlpha);
	DDX_Radio(pDX, IDC_DGN_ALPHA_SHOR_AUTO_RDO, m_nShorAlpha);

	DDX_Radio(pDX, IDC_DGN_SHORT_REDUCT_YES_RDO, m_iShortReduction);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConReinfoShearDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConReinfoShearDlg)
	ON_BN_CLICKED(IDC_DGN_CLOSE,          OnDgnClose           )
	ON_BN_CLICKED(IDC_DGN_EXECUTE,        OnDgnExecute         )
	ON_BN_CLICKED(IDC_DGN_CON_SERV_AR, OnDgnConOption)
	ON_BN_CLICKED(IDC_DGN_CON_SERV_DEL, OnDgnConOption)

	ON_BN_CLICKED(IDC_DGN_REIN_LONG_CHK,  OnDgnReinLongChk     )
	ON_BN_CLICKED(IDC_DGN_REIN_SHOR_CHK,  OnDgnReinShorChk	   )

	ON_BN_CLICKED(IDC_DGN_ALPHA_LONG_CHK, OnDgnAlphaLongChk)
	ON_BN_CLICKED(IDC_DGN_ALPHA_SHOR_CHK, OnDgnAlphaShorChk)

	ON_BN_CLICKED(IDC_DGN_ALPHA_LONG_AUTO_RDO, OnDgnAlphaLongAutoRdo)
	ON_BN_CLICKED(IDC_DGN_ALPHA_LONG_USER_RDO, OnDgnAlphaLongAutoRdo)
	ON_BN_CLICKED(IDC_DGN_ALPHA_SHOR_AUTO_RDO, OnDgnAlphaShorAutoRdo)
	ON_BN_CLICKED(IDC_DGN_ALPHA_SHOR_USER_RDO, OnDgnAlphaShorAutoRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConReinfoShearDlg message handlers

BOOL CDgnConReinfoShearDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;
	
	Initial_Unit();

	Initial_Data();

	Initial_Control();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Set current Unit.
void CDgnConReinfoShearDlg::Initial_Unit()
{
	m_edtLongRein.SetUnitType(CUnitCtrl::m_RESS_UNIT.dReinforce);
	m_edtShorRein.SetUnitType(CUnitCtrl::m_RESS_UNIT.dReinforce);
	m_untLongRein.SetUnitType(CUnitCtrl::m_RESS_UNIT.dReinforce);
	m_untShorRein.SetUnitType(CUnitCtrl::m_RESS_UNIT.dReinforce);

	m_edtShearRatio.SetUnitType(D_UNITSYS_NONE);
	m_edtLongAlpha.SetUnitType(D_UNITSYS_NONE);
	m_edtShorAlpha.SetUnitType(D_UNITSYS_NONE);
}



void CDgnConReinfoShearDlg::Initial_Control()
{
	OnDgnReinLongChk();
	OnDgnReinShorChk();
	OnDgnAlphaLongChk();
	OnDgnAlphaShorChk();
}

void CDgnConReinfoShearDlg::Initial_Data()
{
	m_nOption = 0;
	m_iSeletType = 0;

	T_RESS_D SsedD; SsedD.Initialize();
	m_bLongRein = SsedD.bLongRein;
	m_bShorRein = SsedD.bShorRein;
	m_bLongAlpha = SsedD.bLongAlpha;
	m_bShorAlpha = SsedD.bShorAlpha;
	m_nLongAlpha = SsedD.nLongAlpha;
	m_nShorAlpha = SsedD.nShorAlpha;
	m_iShortReduction = SsedD.nShortReduction;

	UpdateData(FALSE);

	double dLong = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, CUnitCtrl::m_RESS_UNIT.dReinforce, SsedD.dLongRein);
	double dShor = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, CUnitCtrl::m_RESS_UNIT.dReinforce, SsedD.dShorRein);
	m_edtLongRein.SetEditUnit(dLong);
	m_edtShorRein.SetEditUnit(dShor);
	m_edtShearRatio.SetEditUnit(SsedD.dReinRatio);
	m_edtLongAlpha.SetEditUnit(SsedD.dLongAlpha);
	m_edtShorAlpha.SetEditUnit(SsedD.dShorAlpha);
}

void CDgnConReinfoShearDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CDgnConReinfoShearDlg::UpdateBuffer()
{
// 	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;		
// 	T_UDRD_BUFFER buffer_ur;
// 	POSITION pos = pViewBuff->GetStartBuffer();
// 	while(pos)
// 	{
// 		buffer_ur = pViewBuff->GetNextBuffer(pos);
// 		int nCmd = buffer_ur.nCmd;
// 		int nKey = buffer_ur.nKey;
// 		
// 		switch(nCmd)
// 		{
// 			case(UR_DCON_ADD):  // Design Code 변경시.
// 			case(UR_DCON_DEL):
// 				{      
// 				if(m_bIsCsgDesign)
// 				{
// 					if(!GetDesignCsgCodeNo()) ASSERT(0);
// 				}
// 				else
// 				{
// 					m_iCode    = CDgnCodeCtrl::GetConCodeID(CDBLib::GetConCodeName());  // Set Design Code to Enum.
// 					m_iSubCode = CDBLib::GetConSubCode();       // Set Design Sub Code 
// 				}
// 				ChangeDlgItemInfo();                        // Change the Form of the dialog bar.
// 				Initial_Data();                             // Initialize the data.
// 				OnDgnCrackWidthChk();
// 				ResizeDlg();
// 				break;      
// 			}
// 			default:	break;
// 		}
// 	}
}

// If check the 'Add/Replace' radio button.
void CDgnConReinfoShearDlg::OnDgnConOption()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_aDataCtrl, m_nOption==0);
	if (m_nOption == 0)
	{
		Initial_Control();
	}
}

// If press the 'Close' button.
void CDgnConReinfoShearDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
}

// If press the 'Apply' button.
void CDgnConReinfoShearDlg::OnDgnExecute() 
{
	// TODO: Add your control notification handler code here
	if (!CDBLib::IsCodeForMeshedDesignAIJ_JP()) return;
	
	UpdateData(TRUE);

	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;  aSelKey.RemoveAll();
	if(m_iSeletType)
	{
	  I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	  pIGM->GetSelectedElemKeyListForDgn(aSelKey);
	}
	else
	{
		m_pDoc->m_pAttrCtrl->GetElemKeyList(aSelKey);  
	}

	CArray<T_ELEM_K, T_ELEM_K> rSelKey;
	for (int i = 0; i < aSelKey.GetSize(); i++)
	{
		T_ELEM_K key = aSelKey.GetAt(i);
		// Check Material.
		T_ELEM_D ElemD; ElemD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetElem(key, ElemD))       continue;

		if (m_pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp))         rSelKey.Add(key);  
		else continue;
	}

	if (rSelKey.GetSize() < 1)
	{
		AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_Plate_Con_only), MB_OK);
		return;
	}

	if (m_nOption == 0)
	{
		T_RESS_D SsedD;
		SsedD.bLongRein = m_bLongRein;
		SsedD.bShorRein = m_bShorRein;
		SsedD.bLongAlpha = m_bLongAlpha;
		SsedD.bShorAlpha = m_bShorAlpha;
		SsedD.nLongAlpha = m_nLongAlpha;
		SsedD.nShorAlpha = m_nShorAlpha;
		SsedD.nShortReduction = m_iShortReduction;

		SsedD.dLongRein=m_edtLongRein.GetEditValue();
		SsedD.dShorRein=m_edtShorRein.GetEditValue();
		SsedD.dReinRatio = m_edtShearRatio.GetEditValue();
		SsedD.dLongAlpha= m_edtLongAlpha.GetEditValue();
		SsedD.dShorAlpha= m_edtShorAlpha.GetEditValue();

		if (m_pDoc->m_pDataCtrl->AddRess(rSelKey, SsedD))	Initial_SelectItem();
	}
	else
	{
		if (m_pDoc->m_pDataCtrl->DelRess(rSelKey))	Initial_SelectItem();
	}
	
	//Initial_SelectItem();	
}
void CDgnConReinfoShearDlg::Initial_SelectItem()
{
	// Unselected All.
	if (m_pDoc == NULL)	return;
	if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
}

void CDgnConReinfoShearDlg::OnDgnReinLongChk()
{
	UpdateData(TRUE);
	m_edtLongRein.EnableWindow(m_bLongRein);
}
void CDgnConReinfoShearDlg::OnDgnReinShorChk()
{
	UpdateData(TRUE);
	m_edtShorRein.EnableWindow(m_bShorRein);
}
void CDgnConReinfoShearDlg::OnDgnAlphaLongChk()
{
	UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_aLongAlpha, m_bLongAlpha);
	if (m_bLongAlpha) OnDgnAlphaLongAutoRdo();
}
void CDgnConReinfoShearDlg::OnDgnAlphaShorChk()
{
	UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_aShorAlpha, m_bShorAlpha);
	if (m_bShorAlpha) OnDgnAlphaShorAutoRdo();
}

void CDgnConReinfoShearDlg::OnDgnAlphaLongAutoRdo()
{
	UpdateData(TRUE);
	m_edtLongAlpha.EnableWindow(m_nLongAlpha);
}
void CDgnConReinfoShearDlg::OnDgnAlphaShorAutoRdo()
{
	UpdateData(TRUE);
	m_edtShorAlpha.EnableWindow(m_nShorAlpha);
}