// CRCPrtOption.cpp: implementation of the CCRCPrtOption class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CRCPrtOption.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"
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
#include "..\wg_main\wg_mainres2.h"	

#include "CRCDataCtrl.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

constexpr int ELEM_TYPE_ELEMENT = 0;
constexpr int ELEM_TYPE_VBEAM = 1;

CCRCPrtOption::CCRCPrtOption(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CCRCPrtOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCRCPrtOption)
	m_nOption   = -1;
	m_nMPOption = -1;
	m_nMNOption = -1;
	m_nSOption  = -1;  
	m_nTOption  = -1;
	m_nRPOption = -1;
	m_nRNOption = -1;
	m_nFOption  = -1;
	m_nCOption  = -1;

	//
	m_iDgnCode  = -1;
	
	m_nElemType = ELEM_TYPE_ELEMENT;

	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_PSC_VELEM_RDO);

	m_aMoveCtrl.RemoveAll();
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_MOMENT_STC);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_PMOMENT_STC);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_MP1);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_MP2);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_MP3);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_MP4);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_NMOMENT_STC);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_MN1);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_MN2);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_MN3);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_MN4);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_SHEAR_STC);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_V1);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_V2);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_V3);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_V4);
	m_aMoveCtrl.Add(IDC_STATIC_TORS);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_T1);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_T2);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_T3);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_T4);
	m_aMoveCtrl.Add(IDC_STATIC_TBCOLUMN);
	m_aMoveCtrl.Add(IDC_STATIC_TBTENDON);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_RP1);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_RP2);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_RP3);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_RP4);
	m_aMoveCtrl.Add(IDC_STATIC_TBCOMPRE);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_RN1);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_RN2);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_RN3);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_RN4);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_FATIGUE_STC);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_F1);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_F2);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_F3);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_F4);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_CRACK_STC);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_C1);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_C2);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_C3);
	m_aMoveCtrl.Add(IDC_DGN_CRC_POSX_C4);
}


void CCRCPrtOption::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenKFacDlg)
	DDX_Radio(pDX, IDC_DGN_CRC_POSX_AR,  m_nOption);
	DDX_Radio(pDX, IDC_DGN_CRC_POSX_MP1, m_nMPOption);
	DDX_Radio(pDX, IDC_DGN_CRC_POSX_MN1, m_nMNOption);
	DDX_Radio(pDX, IDC_DGN_CRC_POSX_V1,  m_nSOption);  
	DDX_Radio(pDX, IDC_DGN_CRC_POSX_T1,  m_nTOption);
	DDX_Radio(pDX, IDC_DGN_CRC_POSX_RP1, m_nRPOption);
	DDX_Radio(pDX, IDC_DGN_CRC_POSX_RN1, m_nRNOption);
	DDX_Radio(pDX, IDC_DGN_CRC_POSX_F1,  m_nFOption);
	DDX_Radio(pDX, IDC_DGN_CRC_POSX_C1,  m_nCOption);
	DDX_Radio(pDX, IDC_DGN_PSC_ELEM_RDO, m_nElemType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCRCPrtOption, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CCRCPrtOption)
	ON_BN_CLICKED(IDC_DGN_CRC_POSX_AR, OnDgnCRCPosxAr)
	ON_BN_CLICKED(IDC_DGN_CRC_POSX_DEL, OnDgnCRCPosxDel)
	ON_BN_CLICKED(IDC_DGN_PSC_ELEM_RDO, OnDgnPSCElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_PSC_VELEM_RDO, OnDgnPSCElemTypeRdo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CCRCPrtOption::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;

	Initial_Data();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCRCPrtOption::OnDgnPSCElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == 0 ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);
}

void CCRCPrtOption::Execute()
{
	// Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	if (m_nElemType == ELEM_TYPE_ELEMENT)
	{
		pIGM->GetSelectedElemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() > 0)
		{
			for (int i = 0; i < aSelKey.GetSize(); i++)
			{
				T_ELEM_K key = aSelKey.GetAt(i);
				T_ELEM_D eData;
				eData.Initialize();
				if (!m_pDoc->m_pAttrCtrl->GetElem(key, eData)) continue;
				ElemPairK EPairK(key, EN_EL_BEAM);
				if (CDBLib::IsPscDgnFor1st(EPairK, TRUE))	rSelKey.Add(key);
			}

			T_POSX_D rData;
			rData.Initialize();
			if (m_nOption == 0)	// add/replace
			{
				UpdateData(TRUE);
				rData.iMomChk[0] = m_nMPOption;   // 휨검토용 부재단(0=검토하지 않음, 1=I단, 2=J단, 3=양쪽모두)
				rData.iMomChk[1] = m_nMNOption;
				rData.iShrChk = m_nSOption;    // 전단검토용 부재단(0=검토하지 않음, 1=I단, 2=J단, 3=양쪽모두)  
				rData.iTorChk = m_nTOption;    // 비틀림검토용 부재단(0=검토하지 않음, 1=I단, 2=J단, 3=양쪽모두)
				rData.iBarChk[0] = m_nRPOption;   // 철근검토용 부재단(0=검토하지 않음, 1=I단, 2=J단, 3=양쪽모두)
				rData.iBarChk[1] = m_nRNOption;
				rData.nFatiChk = m_nFOption;
				rData.nCrack = m_nCOption;

				if (rSelKey.GetSize() > 0)
				{
					// Initialize selected Element.
					if (m_pDoc->m_pDataCtrl->AddPosx(rSelKey, rData))	Initial_SelectItem();
				}
				else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_PSC), MB_OK);
			}
			else	// Delete
			{
				// Initialize selected Element.
				if (m_pDoc->m_pDataCtrl->DelPosx(rSelKey))	Initial_SelectItem();
			}
		}
		else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
	}
	else
	{
		pIGM->GetSelectedVbemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() > 0)
		{
			//ASSERT(0);
			// The function below must be applied according to the virtual beam.
			// Refer to the ELEM_TYPE_ELEMENT case as a guideline.

			for (int i = 0; i < aSelKey.GetSize(); i++)
			{
				T_ELEM_K key = aSelKey.GetAt(i);
				T_ELEM_D eData;
				eData.Initialize();
				if (!m_pDoc->m_pAttrCtrl->GetElem(key, eData)) continue;
				if (CDBLib::IsPscDgnForVbeam(key, TRUE))	rSelKey.Add(key);
			}

			T_POSV_D rData;
			rData.Initialize();
			if (m_nOption == 0)	// add/replace
			{
				UpdateData(TRUE);
				rData.iMomChk[0] = m_nMPOption;   // 휨검토용 부재단(0=검토하지 않음, 1=I단, 2=J단, 3=양쪽모두)
				rData.iMomChk[1] = m_nMNOption;
				rData.iShrChk = m_nSOption;    // 전단검토용 부재단(0=검토하지 않음, 1=I단, 2=J단, 3=양쪽모두)  
				rData.iTorChk = m_nTOption;    // 비틀림검토용 부재단(0=검토하지 않음, 1=I단, 2=J단, 3=양쪽모두)
				rData.iBarChk[0] = m_nRPOption;   // 철근검토용 부재단(0=검토하지 않음, 1=I단, 2=J단, 3=양쪽모두)
				rData.iBarChk[1] = m_nRNOption;
				rData.nFatiChk = m_nFOption;
				rData.nCrack = m_nCOption;

				if (rSelKey.GetSize() > 0)
				{
					// Initialize selected Element.
					if (m_pDoc->m_pDataCtrl->AddPosv(rSelKey, rData))	Initial_SelectItem();
				}
				else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_PSC), MB_OK);
			}
			else	// Delete
			{
				// Initialize selected Element.
				if (m_pDoc->m_pDataCtrl->DelPosv(rSelKey))	Initial_SelectItem();
			}
		}
		else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
	}
}

void CCRCPrtOption::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

void CCRCPrtOption::Initial_Data()
{
	m_nOption  = 0;	// add/replace

	m_nMPOption = 3;
	m_nMNOption = 3;
	m_nSOption  = 3;  
	m_nTOption  = 3;
	m_nRPOption = 3;
	m_nRNOption = 3;
	m_nFOption  = 3;
	m_nCOption  = 3;

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

    if ( CDBLib::IsPscCodeLSD_KR(m_iDgnCode) || m_iDgnCode==BS5400_90_PSC||  PscdD.iDgnCode == TMH07_3_1989 || PscdD.iDgnCode==IRC_112_2011_PSC ||
		 m_iDgnCode==AS_5100_5_17_PSC || PscdD.iDgnCode == IRC_112_2020_PSC)
	{
		CString strGrpName = _T("");
		// Moment
		strGrpName = _LS(IDS_DGN_TEXT_MOMENT_RESISTANCE);
		GetDlgItem(IDC_DGN_CRC_POSX_MOMENT_STC)->SetWindowText(strGrpName);
		// Positive Moment  
		strGrpName = _T("");
		strGrpName = _LS(IDS_DGN_TEXT_POSITIVE_MOMENT);
		GetDlgItem(IDC_DGN_CRC_POSX_PMOMENT_STC)->SetWindowText(strGrpName);
		// Negative Moment
		strGrpName = _T("");
		strGrpName = _LS(IDS_DGN_TEXT_NEGATIVE_MOMENT);
		GetDlgItem(IDC_DGN_CRC_POSX_NMOMENT_STC)->SetWindowText(strGrpName);

		// Shear 
		strGrpName = _T("");
		strGrpName = _LS(IDS_DGN_TEXT_SHEAR_RESISTANCE);
		GetDlgItem(IDC_DGN_CRC_POSX_SHEAR_STC)->SetWindowText(strGrpName);

		// Torsion
		strGrpName = _T("");
		strGrpName = _LS(IDS_DGN_TEXT_TORSION_RESISTANCE);
		GetDlgItem(IDC_STATIC_TORS)->SetWindowText(strGrpName);
	}
	
	Update_InitDataByCode();

	UpdateData(FALSE);
}

void CCRCPrtOption::OnDgnCRCPosxAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CCRCPrtOption::OnDgnCRCPosxDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CCRCPrtOption::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

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
			{
//		    UpdateBuffer();
				// Add by ZINU.('01.1.3).
				Update_InitDataByCode();
	      break;
			}
		default:
			//ASSERT(FALSE);
			break;
	  }
}

void CCRCPrtOption::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	BOOL bOther=FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
			case(UR_PSCD_ADD):
			case(UR_PSCD_DEL):	{bMFD=TRUE;		break;}
			default:						{bOther=TRUE;	break;}
		}
	}

	if(bMFD && !bOther) Update_InitDataByCode();
}

void CCRCPrtOption::Update_InitDataByCode()
{
	T_PSCD_D PscdD; PscdD.Initialize(); 
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(pDoc)	pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

	CRect rRef;
	CRect rToMove;
	int nDistY = 0;
	CArray<UINT, UINT> aControls;

	GetDlgItem(IDC_DGN_PSC_ELEM_TYPE_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_CRC_POSX_MOMENT_STC)->GetWindowRect(rToMove);

	BOOL bPSCDgnVbem = CDBLib::ActivatePscVBEM();

	if (PscdD.iDgnCode == EUROCODE2_2_05_PSC)
	{
		if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !bPSCDgnVbem)
		{
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, FALSE);
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

	if (PscdD.iDgnCode==TB_10002_3_05) 
	{
		GetDlgItem(IDC_STATIC_TORS)->SetWindowText(_LS(IDS_DGN_DILG_TB_PRE_REINFORCE));
		GetDlgItem(IDC_STATIC_TBCOLUMN)->SetWindowText(_LS(IDS_DGN_DILG_TB_TEND_COMP));
		GetDlgItem(IDC_STATIC_TBTENDON)->SetWindowText(_LS(IDS_DGN_DILG_TB_TEND));
		GetDlgItem(IDC_STATIC_TBCOMPRE)->SetWindowText(_LS(IDS_DGN_DILG_TB_COMP));
	}

    if ( PscdD.iDgnCode==KSCE_USD03      || PscdD.iDgnCode==KSCE_USD05      || PscdD.iDgnCode==KSCE_USD10  || 
        CDBLib::IsPscCodeLSD(PscdD.iDgnCode)  ||
		PscdD.iDgnCode==KSCE_RAIL_USD04 || PscdD.iDgnCode==KSCE_RAIL_USD11 || 
		PscdD.iDgnCode==CJJ_11_2011     || PscdD.iDgnCode==TB_10002_3_05 || PscdD.iDgnCode==JTG_D62_04         ||
		CDBLib::IsPscCodeAASHTO(PscdD.iDgnCode) ||
		PscdD.iDgnCode==BS5400_90_PSC || PscdD.iDgnCode == TMH07_3_1989||
		CDBLib::IsPscCodeCSA(PscdD.iDgnCode) ||
		PscdD.iDgnCode==IRC_112_2011_PSC || PscdD.iDgnCode==AS_5100_5_17_PSC || 
		PscdD.iDgnCode==IRS_PSC || PscdD.iDgnCode == IRC_112_2020_PSC)
	{
		GetDlgItem(IDC_DGN_CRC_POSX_AR)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_MP1)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_MP2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_MP3)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_MP4)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_MN1)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_MN2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_MN3)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_MN4)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_V1)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_V2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_V3)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_V4)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_T1)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_T2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_T3)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_T4)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_RP1)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_RP2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_RP3)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_RP4)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_RN1)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_RN2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_RN3)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CRC_POSX_RN4)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
		
		if(CDBLib::IsPscCodeAASHTO(PscdD.iDgnCode) || PscdD.iDgnCode==BS5400_90_PSC|| PscdD.iDgnCode == TMH07_3_1989 ||
			 CDBLib::IsPscCodeCSA(PscdD.iDgnCode) ||
             CDBLib::IsPscCodeLSD(PscdD.iDgnCode) ||
			 PscdD.iDgnCode==KSCE_USD10    || PscdD.iDgnCode==KSCE_RAIL_USD11 || PscdD.iDgnCode==IRC_112_2011_PSC ||
			 PscdD.iDgnCode==AS_5100_5_17_PSC || PscdD.iDgnCode==IRS_PSC || PscdD.iDgnCode == IRC_112_2020_PSC)
		{
			GetDlgItem(IDC_STATIC_TBCOLUMN)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TBTENDON)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_TBCOMPRE)->ShowWindow(SW_HIDE);
		    GetDlgItem(IDC_DGN_CRC_POSX_RP1)->ShowWindow(SW_HIDE);
		    GetDlgItem(IDC_DGN_CRC_POSX_RP2)->ShowWindow(SW_HIDE);
		    GetDlgItem(IDC_DGN_CRC_POSX_RP3)->ShowWindow(SW_HIDE);
		    GetDlgItem(IDC_DGN_CRC_POSX_RP4)->ShowWindow(SW_HIDE);
		    GetDlgItem(IDC_DGN_CRC_POSX_RN1)->ShowWindow(SW_HIDE);
		    GetDlgItem(IDC_DGN_CRC_POSX_RN2)->ShowWindow(SW_HIDE);
		    GetDlgItem(IDC_DGN_CRC_POSX_RN3)->ShowWindow(SW_HIDE);
		    GetDlgItem(IDC_DGN_CRC_POSX_RN4)->ShowWindow(SW_HIDE);
		}

		GetDlgItem(IDC_DGN_CRC_POSX_FATIGUE_STC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_F1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_F2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_F3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_F4)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_CRC_POSX_CRACK_STC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_C1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_C2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_C3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_C4)->ShowWindow(SW_HIDE);
	}
	else if(PscdD.iDgnCode==JSCE02)
	{
		GetDlgItem(IDC_DGN_CRC_POSX_AR)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_MP1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_MP2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_MP3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_MP4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_MN1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_MN2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_MN3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_MN4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_V1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_V2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_V3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_V4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_T1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_T2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_T3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_T4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_RP1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_RP2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_RP3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_RP4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_RN1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_RN2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_RN3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CRC_POSX_RN4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(FALSE);

		GetDlgItem(IDC_DGN_CRC_POSX_FATIGUE_STC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_F1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_F2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_F3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_F4)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_DGN_CRC_POSX_CRACK_STC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_C1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_C2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_C3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_C4)->ShowWindow(SW_HIDE);
	}
	else if(PscdD.iDgnCode == SNiP_20503_84_PSC || PscdD.iDgnCode == SP_35_13330_11_PSC || PscdD.iDgnCode == SNiP_20503_84_PSC_MKS || PscdD.iDgnCode == SP_35_13330_11_PSC_MKS)
	{
		GetDlgItem(IDC_STATIC_TBCOLUMN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TBTENDON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_TBCOMPRE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_RP1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_RP2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_RP3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_RP4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_RN1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_RN2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_RN3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CRC_POSX_RN4)->ShowWindow(SW_HIDE);

		aControls.RemoveAll();
		aControls.Add(IDC_DGN_CRC_POSX_FATIGUE_STC);
		aControls.Add(IDC_DGN_CRC_POSX_F1);
		aControls.Add(IDC_DGN_CRC_POSX_F2);
		aControls.Add(IDC_DGN_CRC_POSX_F3);
		aControls.Add(IDC_DGN_CRC_POSX_F4);
		aControls.Add(IDC_DGN_CRC_POSX_CRACK_STC);
		aControls.Add(IDC_DGN_CRC_POSX_C1);
		aControls.Add(IDC_DGN_CRC_POSX_C2);
		aControls.Add(IDC_DGN_CRC_POSX_C3);
		aControls.Add(IDC_DGN_CRC_POSX_C4);

		GetDlgItem(IDC_STATIC_TBCOLUMN)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CRC_POSX_FATIGUE_STC)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	}
	else ASSERT(0);

	CRect rTors, rRein, rCrack, rApply;
	GetDlgItem(IDC_STATIC_TORS    )->GetWindowRect(&rTors);
	GetDlgItem(IDC_STATIC_TBCOLUMN)->GetWindowRect(&rRein);
	GetDlgItem(IDC_DGN_CRC_POSX_CRACK_STC)->GetWindowRect(&rCrack);
	GetDlgItem(IDC_DGN_EXECUTE    )->GetWindowRect(&rApply);
	
	CArray<UINT,UINT> aCtrl;
	aCtrl.Add(IDC_DGN_EXECUTE);
	aCtrl.Add(IDC_DGN_CLOSE);

	if(PscdD.iDgnCode==KSCE_USD03 || PscdD.iDgnCode==KSCE_USD05 || 
		 PscdD.iDgnCode==KSCE_RAIL_USD04 || 
		 PscdD.iDgnCode==TB_10002_3_05 || PscdD.iDgnCode==JTG_D62_04 || PscdD.iDgnCode==CJJ_11_2011 || PscdD.iDgnCode==JSCE02)
	{
		CDlgUtil::CtrlMoveDistY(this, aCtrl, rRein.bottom-rApply.top+globalUtils.ScaleByDPI(10));
	}
    else if ( CDBLib::IsPscCodeAASHTO(PscdD.iDgnCode) || 
              CDBLib::IsPscCodeLSD(PscdD.iDgnCode)|| PscdD.iDgnCode==BS5400_90_PSC || PscdD.iDgnCode == TMH07_3_1989 ||
			    CDBLib::IsPscCodeCSA(PscdD.iDgnCode) ||
			    PscdD.iDgnCode==KSCE_USD10 || PscdD.iDgnCode==KSCE_RAIL_USD11 || PscdD.iDgnCode==IRC_112_2011_PSC ||
			    PscdD.iDgnCode==AS_5100_5_17_PSC || PscdD.iDgnCode==IRS_PSC   || PscdD.iDgnCode == IRC_112_2020_PSC)
	{
		CDlgUtil::CtrlMoveDistY(this, aCtrl, rTors.bottom-rApply.top+globalUtils.ScaleByDPI(10));
	}
	else if(PscdD.iDgnCode == SNiP_20503_84_PSC || PscdD.iDgnCode == SP_35_13330_11_PSC || PscdD.iDgnCode == SNiP_20503_84_PSC_MKS || PscdD.iDgnCode == SP_35_13330_11_PSC_MKS)
	{
		CDlgUtil::CtrlMoveDistY(this, aCtrl, rCrack.bottom-rApply.top+globalUtils.ScaleByDPI(10));
	}
	else ASSERT(0);

	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);
}