// DgnGenMemberTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConSPVAREMADlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConSPVAREMADlg dialog


CDgnConSPVAREMADlg::CDgnConSPVAREMADlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConSPVAREMADlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConSPVAREMADlg)
	m_nOption = 0;
	m_nElemType = 2;
	//}}AFX_DATA_INIT

	//Add Member types by Hong,js 03/01/02 for Chinese Code
	//m_arMCtrl_MType.Add(IDC_DGN_ETYPE_STA);
	//m_arMCtrl_MType.Add(IDC_DGN_ETYPE_BEAM);
	//m_arMCtrl_MType.Add(IDC_DGN_ETYPE_COLUMN);
	//m_arMCtrl_MType.Add(IDC_DGN_ETYPE_BRACE);
	////
	//m_arMCtrl_MType_Ch.Add(IDC_DGN_ETYPE_STA_CH);
	////m_arMCtrl_MType_Ch.Add(IDC_DGN_ETYPE_BEAM_CH);
	//m_arMCtrl_MType_Ch.Add(IDC_DGN_ETYPE_COLUMN_CH);
	//m_arMCtrl_MType_Ch.Add(IDC_DGN_ETYPE_BRACE_CH);
	//m_arMCtrl_MType_Ch.Add(IDC_DGN_ETYPE_WALL_CH);
	//m_arMCtrl_MType_Ch.Add(IDC_DGN_ETYPE_TRUSS_CH);
	////
	//m_arMCtrl_SubType[0].Add(IDC_DGN_ETYPE_BEAM_STA);
	//m_arMCtrl_SubType[0].Add(IDC_DGN_ETYPE_BEAM_FRM);
	//m_arMCtrl_SubType[0].Add(IDC_DGN_ETYPE_BEAM_LNTL);
	//m_arMCtrl_SubType[0].Add(IDC_DGN_ETYPE_BEAM_SUPFRM);
	//m_arMCtrl_SubType[0].Add(IDC_DGN_ETYPE_BEAM_DISENERGY);
	////
	//m_arMCtrl_SubType[1].Add(IDC_DGN_ETYPE_COL_STA);
	//m_arMCtrl_SubType[1].Add(IDC_DGN_ETYPE_COL_FRM);
	//m_arMCtrl_SubType[1].Add(IDC_DGN_ETYPE_COL_LOWFRM);
	//m_arMCtrl_SubType[1].Add(IDC_DGN_ETYPE_COL_CORN);
	//m_arMCtrl_SubType[1].Add(IDC_DGN_ETYPE_COL_LOWCORN);
	//m_arMCtrl_SubType[1].Add(IDC_DGN_ETYPE_COL_SUPFRM);
	////
	//m_arMCtrl_SubType[2].Add(IDC_DGN_ETYPE_WALL_STA);
	//m_arMCtrl_SubType[2].Add(IDC_DGN_ETYPE_WALL_GEN_STA);
	//m_arMCtrl_SubType[2].Add(IDC_DGN_ETYPE_WALL_GENSTR);
	//m_arMCtrl_SubType[2].Add(IDC_DGN_ETYPE_WALL_GENORD);
	//m_arMCtrl_SubType[2].Add(IDC_DGN_ETYPE_WALL_COM_STA);
	//m_arMCtrl_SubType[2].Add(IDC_DGN_ETYPE_WALL_COMSTR);
	//m_arMCtrl_SubType[2].Add(IDC_DGN_ETYPE_WALL_COMORD);
	//m_arMCtrl_SubType[2].Add(IDC_DGN_ETYPE_WALL_SHT_STA);
	//m_arMCtrl_SubType[2].Add(IDC_DGN_ETYPE_WALL_SHTSTR);
	//m_arMCtrl_SubType[2].Add(IDC_DGN_ETYPE_WALL_SHTORD);
	////
	//m_arMCtrl_SubType[3].Add(IDC_DGN_ETYPE_BRACE_STA);
	//m_arMCtrl_SubType[3].Add(IDC_DGN_ETYPE_BRACE_CENTER);
	//m_arMCtrl_SubType[3].Add(IDC_DGN_ETYPE_BRACE_ECCE);
	////
	//m_arBtn.Add(IDC_DGN_EXECUTE);
	//m_arBtn.Add(IDC_DGN_CLOSE);
	//
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnConSPVAREMADlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConSPVAREMADlg)
	DDX_Radio(pDX, IDC_DGN_CON_SPLV_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_CON_MONO, m_nElemType);
	//DDX_Radio(pDX, IDC_DGN_ETYPE_BEAM_CH, m_nMType_Ch_Rad);
	//DDX_Radio(pDX, IDC_DGN_ETYPE_BEAM_FRM, m_nBeam_Rad);
	//DDX_Radio(pDX, IDC_DGN_ETYPE_COL_FRM, m_nCol_Rad);
	//DDX_Radio(pDX, IDC_DGN_ETYPE_WALL_GENSTR, m_nWall_Rad);
	//DDX_Radio(pDX, IDC_DGN_ETYPE_BRACE_CENTER, m_nBrace_Rad);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConSPVAREMADlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConSPVAREMADlg)
	//ON_BN_CLICKED(IDC_DGN_CLOSE, OnDgnClose)
	//ON_BN_CLICKED(IDC_DGN_EXECUTE, OnDgnExecute)
	ON_BN_CLICKED(IDC_DGN_CON_SPLV_AR, OnDgnSpvaAr)
	ON_BN_CLICKED(IDC_DGN_CON_SPLV_DEL, OnDgnSpvaDel)
	ON_BN_CLICKED(IDC_DGN_CON_MONO, OnDgnSpvaMono)
	ON_BN_CLICKED(IDC_DGN_CON_SS, OnDgnSpvaSS)
	ON_BN_CLICKED(IDC_DGN_CON_NONE, OnDgnSpvaNone)
	//ON_BN_CLICKED(IDC_DGN_ETYPE_BEAM_CH, OnDgnEtypeBeamCh)
	/*ON_BN_CLICKED(IDC_DGN_ETYPE_BRACE_CH, OnDgnEtypeBraceCh)
	ON_BN_CLICKED(IDC_DGN_ETYPE_COLUMN_CH, OnDgnEtypeColumnCh)
	ON_BN_CLICKED(IDC_DGN_ETYPE_WALL_CH, OnDgnEtypeWallCh)
	ON_BN_CLICKED(IDC_DGN_ETYPE_TRUSS_CH, OnDgnEtypeTrussCh)*/
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConSPVAREMADlg message handlers

void CDgnConSPVAREMADlg::OnDgnClose()
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnConSPVAREMADlg::OnDgnExecute()
{
	//if (m_bUseSubType)
		UpdateData_SubType();
	//else
		//UpdateData_NonSubType();
}

void CDgnConSPVAREMADlg::UpdateData_SubType()
{//This is needed for US once we create the new Data Base for Slva
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	CArray<T_ELEM_K, T_ELEM_K> rSelElemKey, rSelWallKey;
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->GetSelectedElemKeyListForDgn(aSelKey);


	T_ELEM_D elem_data;
	elem_data.Initialize();
	if (aSelKey.GetSize() > 0)
	{
		bool bcheck = True;
		bool istheirbcheck = False;
	
		for (int i = 0; i < aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);
			m_pDoc->m_pAttrCtrl->GetElem(key, elem_data);
			if (m_pDoc->m_pAttrCtrl->GetMemberType(key) == D_MBTP_BEAM)
			{
			//	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_BEAM_Con_only), MB_OK);
				bcheck = True;

			}
			else if ((m_pDoc->m_pAttrCtrl->GetMemberType(key) == D_MBTP_PLATEBEAM) ||(elem_data.eltyp==4))
			{
				//	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_BEAM_Con_only), MB_OK);
				bcheck = True;

			}
			else if (m_pDoc->m_pAttrCtrl->GetMemberType(key) == D_MBTP_COLUMN)
			{
				//AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_BEAM_Con_only), MB_OK)

				bcheck = False;
				istheirbcheck = True;
			
			}
			else if (m_pDoc->m_pAttrCtrl->GetMemberType(key) == D_MBTP_PLATECOLM)
			{
				//AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_BEAM_Con_only), MB_OK)

				bcheck = False;
				istheirbcheck = True;

			}

		
			
			
			// Beam, Truss.
			if (m_pDoc->m_pAttrCtrl->IsFrameType(elem_data.eltyp)&&bcheck==True)
			{
							rSelElemKey.Add(key);
			}
			//Wall
			else if (m_pDoc->m_pAttrCtrl->IsPlaneType(elem_data.eltyp) && bcheck == True)	rSelWallKey.Add(key);
		}
		T_SPVM_D rData;
		rData.Initialize();
		if (m_nOption == 0)	// add/replace
		{
			UpdateData(TRUE);
			if (m_nElemType == 0)
			{
				rData.nMbType = D_SPVT_MONO;
				
			}
			else if (m_nElemType == 1)
			{
				rData.nMbType = D_SPVT_SS;
			
			}
			else 
			{
				rData.nMbType = D_SPVT_NONE;
			
			}


			if (rSelElemKey.GetSize() > 0) {
				// Initialize selected Element. (FRAME)
				if (m_pDoc->m_pDataCtrl->AddSpvm(rSelElemKey, rData))	Initial_SelectItem();
			}

			if (rSelWallKey.GetSize() > 0) {
				// Initialize selected Element. (FRAME)
				if (m_pDoc->m_pDataCtrl->AddSpvm(rSelWallKey, rData))	Initial_SelectItem();
			}

			if (istheirbcheck) {
				AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_BEAM_Con_only), MB_OK);
			}
			//if (rSelElemKey.GetSize() > 0 && (m_nMType_Ch_Rad < 3 || m_nMType_Ch_Rad == 4))//add by maxiao 2015-10-14
			//{
			//	// Initialize selected Element. (FRAME)
			//	if (m_pDoc->m_pDataCtrl->AddMbtp(rSelElemKey, rData))	Initial_SelectItem();
			//}
			//else if (rSelWallKey.GetSize() > 0 && m_nMType_Ch_Rad == 3)
			//{
			//	// Initialize selected Element. (WALL)
			//	if (m_pDoc->m_pDataCtrl->AddMbtp(rSelWallKey, rData))	Initial_SelectItem();
			//}
			//else
			//{
			//	if (m_nMType_Ch_Rad < 3) AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE), MB_OK);
			//	else           AfxMessageBox(_LS(IDS_DGN_CHK_WALL_TYPE), MB_OK);
			//}
			
		}
		else	// DELETE
		{
			// Initialize selected Element.
			if (m_pDoc->m_pDataCtrl->DelSpvm(rSelElemKey))	Initial_SelectItem();
			if (m_pDoc->m_pDataCtrl->DelMbtp(rSelWallKey))	Initial_SelectItem();
			// Initialize Data.
			Initial_Data(m_bUseSubType);
		}
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);

}

void CDgnConSPVAREMADlg::UpdateData_NonSubType()
{
	//
	//	Select된 Element를 얻어옵니다.
	//CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	//CArray<T_ELEM_K, T_ELEM_K> rSelKey;
	//I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	//pIGM->GetSelectedElemKeyListForDgn(aSelKey);

	//if (aSelKey.GetSize() > 0)
	//{
	//	for (int i = 0; i < aSelKey.GetSize(); i++)
	//	{
	//		T_ELEM_K key = aSelKey.GetAt(i);
	//		T_ELEM_D eData;
	//		eData.Initialize();
	//		m_pDoc->m_pAttrCtrl->GetElem(key, eData);
	//		// Beam, Truss.
	//		if (m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp))	rSelKey.Add(key);
	//	}
	//	T_MBTP_D rData;
	//	rData.Initialize();
	//	if (m_nOption == 0)	// add/replace
	//	{
	//		UpdateData(TRUE);
	//		if (m_nElemType == 0)       rData.nMbType = D_MBTP_BEAM;
	//		else if (m_nElemType == 1)  rData.nMbType = D_MBTP_COLUMN;
	//		else if (m_nElemType == 2)  rData.nMbType = D_MBTP_BRACE;
	//		else if (m_nElemType == 4)  rData.nMbType = D_MBTP_TRUSS;//add by maxiao 2015-10-14
	//		rData.nSubType = 0; //add Hong,js 03/01/04

	//		if (rSelKey.GetSize() > 0)
	//		{
	//			// Initialize selected Element.
	//			if (m_pDoc->m_pDataCtrl->AddMbtp(rSelKey, rData))	Initial_SelectItem();
	//		}
	//		else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE), MB_OK);
	//	}
	//	else	// Delete
	//	{
	//		// Initialize selected Element.
	//		if (m_pDoc->m_pDataCtrl->DelMbtp(rSelKey))	Initial_SelectItem();
	//		// Initialize Data.
	//		Initial_Data(m_bUseSubType);
	//	}
	//}
	//else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
}

void CDgnConSPVAREMADlg::Initial_SelectItem()
{
	// Unselected All.
	if (m_pDoc == NULL)	return;
	if (m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
}

void CDgnConSPVAREMADlg::OnDgnSpvaAr()
{
	// TODO: Add your control notification handler code here
	m_nOption = 0;			//	Add/Replace
	EnableItems(TRUE);
	UpdateData(FALSE);
}

void CDgnConSPVAREMADlg::OnDgnSpvaDel()
{
	// TODO: Add your control notification handler code here
	m_nOption = 1;			//	Delete
	EnableItems(FALSE);
	UpdateData(FALSE);
}

void CDgnConSPVAREMADlg::OnDgnSpvaMono()
{
	// TODO: Add your control notification handler code here
	m_nElemType = 0;    //  Beam
	UpdateData(FALSE);
}

void CDgnConSPVAREMADlg::OnDgnSpvaSS()
{
	// TODO: Add your control notification handler code here
	m_nElemType = 1;    //  Column
	UpdateData(FALSE);
}

void CDgnConSPVAREMADlg::OnDgnSpvaNone()
{
	// TODO: Add your control notification handler code here
	m_nElemType = 2;    //  Brace
	UpdateData(FALSE);
}

BOOL CDgnConSPVAREMADlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	if (m_nElemType < 0)	bCheck = FALSE;
	if (!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE), MB_OK);

	return bCheck;
}

BOOL CDgnConSPVAREMADlg::OnInitDialog()
{
	CMenuBarChildDlg::OnInitDialog();

	//bFirstDlg = TRUE;

	//if (!CDBLib::Is_UseMbtpSubType())	Initial_Data(FALSE);
	//else															Initial_Data(TRUE);

	return TRUE;
}

void CDgnConSPVAREMADlg::Initial_Data(BOOL bUseSubType)
{
	EnableItems(TRUE);
	m_nOption = 0;			//	Add/Replace
	m_nElemType = 2;  //None
	//if (!bUseSubType)
	//{
	//	Show_MainItem(FALSE);
	//	m_nElemType = 1;      //  Column으로 초기화
	//	//m_bUseSubType = FALSE;

	//}
	//else
	//{
	//	Show_MainItem(TRUE);
	//	//m_bUseSubType = TRUE;
	//	//m_nMType_Ch_Rad = 1;     //Main Type :Column
	//	//m_nCol_Rad = 0; //Sub Type  :Frame Column
	//}

	UpdateData(FALSE);

}

void CDgnConSPVAREMADlg::Show_MainItem(BOOL bUseSubType)
{
	//if (!bUseSubType)
	//{
	//	for (int i = 0; i < m_arMCtrl_MType.GetSize(); i++)
	//	{
	//		UINT Item = m_arMCtrl_MType.GetAt(i);
	//		GetDlgItem(Item)->ShowWindow(SW_SHOW);
	//	}
	//	for (int i = 0; i < m_arMCtrl_MType_Ch.GetSize(); i++)
	//	{
	//		UINT Item = m_arMCtrl_MType_Ch.GetAt(i);
	//		GetDlgItem(Item)->ShowWindow(SW_HIDE);
	//	}
	//	Show_SubItem(FALSE, FALSE, FALSE); //Hide All
	//	AlignItems(0);
	//}
	//else
	//{
	//	for (int i = 0; i < m_arMCtrl_MType.GetSize(); i++)
	//	{
	//		UINT Item = m_arMCtrl_MType.GetAt(i);
	//		GetDlgItem(Item)->ShowWindow(SW_HIDE);
	//	}
	//	for (int i = 0; i < m_arMCtrl_MType_Ch.GetSize(); i++)
	//	{
	//		UINT Item = m_arMCtrl_MType_Ch.GetAt(i);
	//		GetDlgItem(Item)->ShowWindow(SW_SHOW);
	//	}
	//	Show_SubItem(FALSE, TRUE, FALSE); //Hide All
	//	AlignItems(2);
	//}
}


//void CDgnConSPVAREMADlg::Show_SubItem(BOOL bBeam, BOOL bCol, BOOL bWall, BOOL bBrace)
//{
//
//	int iShow[4];
//	iShow[0] = (bBeam ? SW_SHOW : SW_HIDE);
//	iShow[1] = (bCol ? SW_SHOW : SW_HIDE);
//	iShow[2] = (bWall ? SW_SHOW : SW_HIDE);
//	iShow[3] = (bBrace ? SW_SHOW : SW_HIDE);
//
//	for (int k = 0; k < 4; k++)
//	{
//		for (int i = 0; i < m_arMCtrl_SubType[k].GetSize(); i++)
//		{
//			UINT Item = m_arMCtrl_SubType[k].GetAt(i);
//			GetDlgItem(Item)->ShowWindow(iShow[k]);
//		}
//	}//END - for(k)
//}

//void CDgnConSPVAREMADlg::AlignItems(int iOption)
//{
//	CRect rect_MType, rect_MType_Ch, rect_SubType[4], rect;
//
//	// CRect RectDlg;
////  GetWindowRect(&m_RectDlg);
//	GetDlgItem(IDC_DGN_ETYPE_STA)->GetWindowRect(rect_MType);
//	GetDlgItem(IDC_DGN_ETYPE_STA_CH)->GetWindowRect(rect_MType_Ch);
//	GetDlgItem(IDC_DGN_ETYPE_BEAM_STA)->GetWindowRect(rect_SubType[0]);
//	GetDlgItem(IDC_DGN_ETYPE_COL_STA)->GetWindowRect(rect_SubType[1]);
//	GetDlgItem(IDC_DGN_ETYPE_WALL_STA)->GetWindowRect(rect_SubType[2]);
//	GetDlgItem(IDC_DGN_ETYPE_BRACE_STA)->GetWindowRect(rect_SubType[3]);//add by maxiao 2015-10-14
//	GetDlgItem(IDC_DGN_EXECUTE)->GetWindowRect(rect);
//
//	int nDistY = 0;
//	switch (iOption)
//	{
//	case(0): //bUseSubType:FALSE
//		if (bFirstDlg)
//		{
//			nDistY = rect_MType.bottom - rect_SubType[3].bottom;
//			bFirstDlg = FALSE;
//		}
//		else
//			nDistY = rect_MType.bottom - m_CurSubType_bottom;
//		CDlgUtil::CtrlMoveDistY(this, m_arBtn, nDistY);
//		break;
//	case(1): //bUseSubType:TRUE, SubBeam Show
//	case(2)://bUseSubType:TRUE, SubColumn Show
//	case(3)://bUseSubType:TRUE, SubWall Show
//	case(4): //bUseSubType:TRUE, SubBrace show
//	case(5): //bUseSubType:TRUE, SubTruss
//		nDistY = rect_MType.top - rect_MType_Ch.top;
//		CDlgUtil::CtrlMoveDistY(this, m_arMCtrl_MType_Ch, nDistY);
//		CDlgUtil::CtrlMoveDistY(this, m_arMCtrl_SubType[0], nDistY);
//
//		nDistY += rect_SubType[0].top - rect_SubType[1].top;
//		CDlgUtil::CtrlMoveDistY(this, m_arMCtrl_SubType[1], nDistY);			//
//
//		nDistY += rect_SubType[1].top - rect_SubType[2].top;
//		CDlgUtil::CtrlMoveDistY(this, m_arMCtrl_SubType[2], nDistY);
//
//		nDistY += rect_SubType[2].top - rect_SubType[3].top;
//		CDlgUtil::CtrlMoveDistY(this, m_arMCtrl_SubType[3], nDistY);
//
//		GetDlgItem(IDC_DGN_ETYPE_STA_CH)->GetWindowRect(rect_MType_Ch);
//		GetDlgItem(IDC_DGN_ETYPE_BEAM_STA)->GetWindowRect(rect_SubType[0]);
//		GetDlgItem(IDC_DGN_ETYPE_COL_STA)->GetWindowRect(rect_SubType[1]);
//		GetDlgItem(IDC_DGN_ETYPE_WALL_STA)->GetWindowRect(rect_SubType[2]);
//		GetDlgItem(IDC_DGN_ETYPE_BRACE_STA)->GetWindowRect(rect_SubType[3]);
//		if (bFirstDlg)
//		{
//			nDistY += (rect_SubType[1].bottom - rect_SubType[1].top) - (rect_SubType[3].bottom - rect_SubType[3].top);
//			bFirstDlg = FALSE;
//		}
//		else if (iOption < 5)
//			nDistY = rect_SubType[iOption - 1].bottom - m_CurSubType_bottom;
//		else
//			nDistY = rect_MType_Ch.bottom - m_CurSubType_bottom;
//		bFirstDlg = FALSE;
//		CDlgUtil::CtrlMoveDistY(this, m_arBtn, nDistY);
//		break;
//	}
//
//	if (iOption == 3) // bUseSubType:TRUE, SubWall Show.
//	{
//		T_DCON_D DconD;
//		if (!m_pDoc->m_pAttrCtrl->GetDcon(DconD)) DconD.Initialize();
//		BOOL bCodeGB10 = DconD.DesignCode == _T("GB/T50010-10") ? TRUE : FALSE;
//
//		int nCmdShow = bCodeGB10 ? SW_SHOW : SW_HIDE;
//		GetDlgItem(IDC_DGN_ETYPE_WALL_SHT_STA)->ShowWindow(nCmdShow);
//		GetDlgItem(IDC_DGN_ETYPE_WALL_SHTORD)->ShowWindow(nCmdShow);
//		GetDlgItem(IDC_DGN_ETYPE_WALL_SHTSTR)->ShowWindow(nCmdShow);
//
//		if (!bCodeGB10)
//		{
//			CRect rectShortStatic;
//			GetDlgItem(IDC_DGN_ETYPE_WALL_SHT_STA)->GetWindowRect(&rectShortStatic);
//
//			int nHeight = rectShortStatic.top - rect_SubType[2].top;
//			GetDlgItem(IDC_DGN_ETYPE_WALL_STA)->SetWindowPos(NULL, rect_SubType[2].left, rect_SubType[2].top, rect_SubType[2].Width(), nHeight, SWP_NOMOVE);
//		}
//		else
//		{
//			CRect rectShortRadio;
//			GetDlgItem(IDC_DGN_ETYPE_WALL_SHTORD)->GetWindowRect(&rectShortRadio);
//
//			int nHeight = rectShortRadio.bottom - rect_SubType[2].top + rectShortRadio.Height()/*=Margin*/;
//			GetDlgItem(IDC_DGN_ETYPE_WALL_STA)->SetWindowPos(NULL, rect_SubType[2].left, rect_SubType[2].top, rect_SubType[2].Width(), nHeight, SWP_NOMOVE);
//		}
//	}
//
//	switch (iOption)
//	{
//	case(0):
//		GetDlgItem(IDC_DGN_ETYPE_STA)->GetWindowRect(rect);
//		m_CurSubType_bottom = rect.bottom;
//		break;
//	case(1):
//		GetDlgItem(IDC_DGN_ETYPE_BEAM_STA)->GetWindowRect(rect);
//		m_CurSubType_bottom = rect.bottom;
//		break;
//	case(2):
//		GetDlgItem(IDC_DGN_ETYPE_COL_STA)->GetWindowRect(rect);
//		m_CurSubType_bottom = rect.bottom;
//		break;
//	case(3):
//		GetDlgItem(IDC_DGN_ETYPE_WALL_STA)->GetWindowRect(rect);
//		m_CurSubType_bottom = rect.bottom;
//		break;
//	case(4):
//		GetDlgItem(IDC_DGN_ETYPE_BRACE_STA)->GetWindowRect(rect);
//		m_CurSubType_bottom = rect.bottom;
//		break;
//	case(5):
//		GetDlgItem(IDC_DGN_ETYPE_STA_CH)->GetWindowRect(rect);
//		m_CurSubType_bottom = rect.bottom;
//		break;
//	}
//}

void CDgnConSPVAREMADlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	//HWND hWnd = GetSafeHwnd();
	//if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	//switch (lHint)
	//{
	//case D_UPDATE_DEFAULT:
	//	// do something...
	//	break;
	//case D_UPDATE_BUFFER_BEFORE:
	//	// do something...
	//	break;
	//case D_UPDATE_UNIT:
	//	// do something...
	//	break;
	//case D_UPDATE_SEL_ADD:
	//	// do something...
	//	break;
	//case D_UPDATE_SEL_DEL:
	//	// do something...
	//	break;
	//case D_UPDATE_BUFFER_AFTER:
	//	// do something...
	//	UpdateBuffer();
	//	break;
	//case D_UPDATE_PREF_CHANGED:
	//	break;
	//default:
	//	//ASSERT(FALSE);
	//	break;
	//}
}

void CDgnConSPVAREMADlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	while (pos)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_DCFS_ADD):
		case(UR_DSTL_ADD):
		case(UR_DCON_ADD):
		case(UR_DSRC_ADD):
		case(UR_DCFS_DEL):
		case(UR_DSTL_DEL):
		case(UR_DCON_DEL):
		case(UR_DSRC_DEL):
		{
			if (!CDBLib::Is_UseMbtpSubType())
				Initial_Data(FALSE);
			else
				Initial_Data(TRUE);
			break;
		}
		default:	break;
		}
	}
}

void CDgnConSPVAREMADlg::EnableItems(BOOL bEnable)
{
	//GetDlgItem(IDC_DGN_ETYPE_BEAM)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_COLUMN)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_BRACE)->EnableWindow(bEnable);

	////GetDlgItem(IDC_DGN_ETYPE_BEAM_CH)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_COLUMN_CH)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_BRACE_CH)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_WALL_CH)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_TRUSS_CH)->EnableWindow(bEnable);//add by maxiao 2015-10-14

	//GetDlgItem(IDC_DGN_ETYPE_BEAM_FRM)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_BEAM_LNTL)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_BEAM_SUPFRM)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_BEAM_DISENERGY)->EnableWindow(bEnable);

	//GetDlgItem(IDC_DGN_ETYPE_COL_FRM)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_COL_LOWFRM)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_COL_CORN)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_COL_LOWCORN)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_COL_SUPFRM)->EnableWindow(bEnable);

	//GetDlgItem(IDC_DGN_ETYPE_WALL_GEN_STA)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_WALL_GENORD)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_WALL_GENSTR)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_WALL_COM_STA)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_WALL_COMORD)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_WALL_COMSTR)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_WALL_SHT_STA)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_WALL_SHTORD)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_WALL_SHTSTR)->EnableWindow(bEnable);

	//GetDlgItem(IDC_DGN_ETYPE_BRACE_STA)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_BRACE_CENTER)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_ETYPE_BRACE_ECCE)->EnableWindow(bEnable);


	//CString strStlCodeName = CDBLib::GetStlCodeName();
	//if ((strStlCodeName != _T("GB50017-15") && strStlCodeName != _T("GB50017-03") && strStlCodeName != _T("GB50017-17")) && bEnable)
	//{
	//	GetDlgItem(IDC_DGN_ETYPE_BRACE_CENTER)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_DGN_ETYPE_BRACE_ECCE)->EnableWindow(FALSE);
	//}
	//if (strStlCodeName != _T("GB50017-15") && bEnable)
	//{
	//	GetDlgItem(IDC_DGN_ETYPE_TRUSS_CH)->EnableWindow(FALSE);//add by maxiao 2015-10-14
	//	//GetDlgItem(IDC_DGN_ETYPE_BEAM_DISENERGY)->EnableWindow(FALSE);
	//}
}