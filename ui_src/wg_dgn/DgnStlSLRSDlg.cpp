// DgnStlSLRSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlSLRSDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\DBCodeDef.h"

#include "..\wg_main\wg_mainres2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlSLRSDlg dialog


CDgnStlSLRSDlg::CDgnStlSLRSDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlSLRSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlSLRSDlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnStlSLRSDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlSLRSDlg)
	DDX_Control(pDX, IDC_DGN_SLRS_FRAME_CMB, m_cmbStructureType);
	DDX_Control(pDX, IDC_DGN_SLRS_OPTION_CHK, m_chkOption);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlSLRSDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlSLRSDlg)
	ON_CBN_SELCHANGE(IDC_DGN_SLRS_FRAME_CMB, OnSelchangeDgnStructureType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlSLRSDlg message handlers

void CDgnStlSLRSDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnStlSLRSDlg::OnDgnExecute() 
{
	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	if(aSelKey.GetSize() > 0)
	{
		T_SLRS_D rData;
		rData.Initialize();

		T_DSTL_D DstlD;
		m_pDoc->m_pAttrCtrl->GetDstl(DstlD);

		CArray<UINT,UINT> aRdoCtrls;
		aRdoCtrls.Add(IDC_DGN_AR_RDO);
		aRdoCtrls.Add(IDC_DGN_DEL_RDO);
		int nOption = 0;
		CDlgUtil::CtrlRadioGetCheck(this, aRdoCtrls, nOption);
			
		if(nOption==0)	// add/replace
		{
			int ix = m_cmbStructureType.GetCurSel();
			if (ix == CB_ERR) rData.nFrameType = EN_STL_AISC_SEISTYPE_SCBF;
			else              rData.nFrameType = m_cmbStructureType.GetItemData(ix);
			if (DstlD.DesignCode != STLCODE_IS800_2007)
			{
				if (rData.nFrameType == EN_STL_AISC_SEISTYPE_SCBF ||
					rData.nFrameType == EN_STL_AISC_SEISTYPE_OCBF ||
					rData.nFrameType == EN_STL_AISC_SEISTYPE_EBF   )     rData.bCheck = m_chkOption.GetCheck();
				else    rData.bCheck = FALSE;
			}
			else
			{
				if (rData.nFrameType == EN_STL_AISC_SEISTYPE_OMF ||
					rData.nFrameType == EN_STL_AISC_SEISTYPE_SMF ||
					rData.nFrameType == EN_STL_AISC_SEISTYPE_SCBF )     rData.bCheck = m_chkOption.GetCheck();
				else    rData.bCheck = FALSE;
			}

			for(int i=0; i<aSelKey.GetSize(); i++)
			{
				T_ELEM_K key = aSelKey.GetAt(i);
				T_ELEM_D eData;
				eData.Initialize();
				m_pDoc->m_pAttrCtrl->GetElem(key,eData);
				// Beam, Truss.
				if(m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp))	  rSelKey.Add(key);
			}

			if(rSelKey.GetSize() > 0)
			{
		  	if(m_pDoc->m_pDataCtrl->AddSlrs(rSelKey,rData))	Initial_SelectItem();
			}
			else	
			{
				AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE),MB_OK);
			}
		}
		else	// Delete
		{
			if(m_pDoc->m_pDataCtrl->DelSlrs(aSelKey))	Initial_SelectItem();
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
}

void CDgnStlSLRSDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
}

void CDgnStlSLRSDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
	case D_UPDATE_BUFFER_AFTER:
		Update_InitDataByCode();
		break;
	default:
			break;
	}
}

void CDgnStlSLRSDlg::Update_InitDataByCode()
{
	CString strTitle=_T("");
	BOOL bShow = FALSE;
	CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_SLRS, strTitle, bShow);

	GetDlgItem(IDC_DGN_AR_RDO)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_DEL_RDO)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_SLRS_FRAME_CMB)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_SLRS_OPTION_CHK)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_SLRS_OPTION_DESC_STC)->EnableWindow(bShow);
}

BOOL CDgnStlSLRSDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	if(m_pDoc==NULL)	return TRUE;

	CArray<UINT,UINT> aRdoCtrls;
	aRdoCtrls.Add(IDC_DGN_AR_RDO);
	aRdoCtrls.Add(IDC_DGN_DEL_RDO);
	CDlgUtil::CtrlRadioSetCheck(this, aRdoCtrls, 0);

	int nIndex = 0;
	//T_DSTL_D DstlD;
	T_DSTL_D DstlD;
	CDBDoc* m_pDoc;
	m_pDoc = CDBDoc::GetDocPoint();
	m_pDoc->m_pAttrCtrl->GetDstl(DstlD);


	if (DstlD.DesignCode == STLCODE_IS800_2007) {
		m_cmbStructureType.ResetContent();
		nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_ORDINARY_MOMENT_FRAMES));
		m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_OMF);
		nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_SPECIAL_MOMENT_FRAMES_IS));
		m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_SMF);
		nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_SPECIAL_CONCENTRICALLY_BRACED_FRAMES));
		m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_SCBF);
		//nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_ECCENTRICALLY_BRACED_FRAMES));
		//m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_EBF);
	}
	else{
		m_cmbStructureType.ResetContent();
		nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_SPECIAL_CONCENTRICALLY_BRACED_FRAMES));
		m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_SCBF);
		nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_ORDINARY_CONCENTRICALLY_BRACED_FRAMES));
		m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_OCBF);
		nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_ECCENTRICALLY_BRACED_FRAMES));
		m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_EBF);
		nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_BUCKLING_RESTRAINED_BRACED_FRAMES));
		m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_BRBF);
		nIndex = m_cmbStructureType.AddString(_LS(IDS_DGN_SPECIAL_PLATE_SHEAR_WALLS));
		m_cmbStructureType.SetItemData(nIndex, EN_STL_AISC_SEISTYPE_SPSW);
	}
	
	
	if(!m_pDoc->m_pAttrCtrl->GetDstl(DstlD))      DstlD.Initialize();
	if(DstlD.nFrameType == EN_STL_AISC_SEISTYPE_SMF ||
		 DstlD.nFrameType == EN_STL_AISC_SEISTYPE_IMF ||
		 DstlD.nFrameType == EN_STL_AISC_SEISTYPE_OMF  )  m_cmbStructureType.SetCurSel(0);
	else  m_cmbStructureType.SetCurSel(DstlD.nFrameType-3);

	OnSelchangeDgnStructureType();

	m_chkOption.SetCheck(TRUE);
	
	CDlgUtil::CobxAdjustListBoxWidth(m_cmbStructureType);

	Update_InitDataByCode();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlSLRSDlg::OnSelchangeDgnStructureType()
{
	int ix = m_cmbStructureType.GetCurSel();
	int nFrameType = m_cmbStructureType.GetItemData(ix);
	T_DSTL_D DstlD;
	CDBDoc* m_pDoc;
	m_pDoc = CDBDoc::GetDocPoint();
	m_pDoc->m_pAttrCtrl->GetDstl(DstlD);
	
	if((nFrameType == EN_STL_AISC_SEISTYPE_SCBF && DstlD.DesignCode != STLCODE_IS800_2007) ||
		 nFrameType == EN_STL_AISC_SEISTYPE_OCBF )
	{
		CString strStlCode = CDBLib::GetStlCodeName();
		CString strDesc = strStlCode==_T("TWN-ASD96") || strStlCode==_T("TWN-LSD96") ? _LS(IDS_DGN_BRACE_SLENERNESS_RATIO_DESC_TWN96) : _LS(IDS_DGN_BRACE_SLENERNESS_RATIO_DESC);

		GetDlgItem(IDC_DGN_SLRS_OPTION_CHK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SLRS_OPTION_DESC_STC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SLRS_OPTION_CHK)->SetWindowText(_LS(IDS_DGN_CHECK_FOR_BRACE_SLENDERNESS_RATIO));
		GetDlgItem(IDC_DGN_SLRS_OPTION_DESC_STC)->SetWindowText(strDesc);
	}
	else if(nFrameType == EN_STL_AISC_SEISTYPE_EBF)
	{
		GetDlgItem(IDC_DGN_SLRS_OPTION_CHK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_SLRS_OPTION_DESC_STC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SLRS_OPTION_CHK)->SetWindowText(_LS(IDS_DGN_CHECK_FOR_LINKS));
	}
	else if(nFrameType == EN_STL_AISC_SEISTYPE_BRBF ||
					nFrameType == EN_STL_AISC_SEISTYPE_SPSW)
	{
		GetDlgItem(IDC_DGN_SLRS_OPTION_CHK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_SLRS_OPTION_DESC_STC)->ShowWindow(SW_HIDE);
	}
	else if (DstlD.DesignCode == STLCODE_IS800_2007) 
	{
		if (nFrameType == EN_STL_AISC_SEISTYPE_SCBF)
		{
			GetDlgItem(IDC_DGN_SLRS_OPTION_CHK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DGN_SLRS_OPTION_DESC_STC)->ShowWindow(SW_HIDE);
		}
		else if (nFrameType == EN_STL_AISC_SEISTYPE_SMF)
		{
			GetDlgItem(IDC_DGN_SLRS_OPTION_CHK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DGN_SLRS_OPTION_DESC_STC)->ShowWindow(SW_HIDE);
		}
		else if (nFrameType == EN_STL_AISC_SEISTYPE_OMF)
		{
			GetDlgItem(IDC_DGN_SLRS_OPTION_CHK)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DGN_SLRS_OPTION_DESC_STC)->ShowWindow(SW_HIDE);
		}
	}
	else      {  ASSERT(0);		return ;	}


}