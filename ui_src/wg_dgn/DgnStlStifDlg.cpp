// DgnStlStifDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlStifDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_PostCtrl.h"

#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_STL_STFN

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlStifDlg dialog


CDgnStlStifDlg::CDgnStlStifDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlStifDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlStifDlg)
	m_nOption = -1;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnStlStifDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlStifDlg)
	DDX_Control(pDX, IDC_DGN_STIFFBOX_WSIDE, m_WSpin);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_FSIDE, m_FSpin);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_WIDTH, m_Width);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_THICK, m_Thick);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_SPACE, m_Space);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_B, m_B);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_CW, m_Cw);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_H, m_H);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_TF2, m_Tf2);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_TF, m_Tf);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_TW, m_Tw);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_TWUNIT, m_TwUnit);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_TFUNIT2, m_Tf2Unit);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_TFUNIT, m_TfUnit);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_HUNIT, m_HUnit);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_CWUNIT, m_CwUnit);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_BUNIT, m_BUnit);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_WUNIT, m_WUnit);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_TUNIT, m_TUnit);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_SPACEUNIT, m_SpaceUnit);
	DDX_Control(pDX, IDC_DGN_STIFFBOX_PROPNO, m_PropNo);
	DDX_Radio(pDX, IDC_DGN_STIFFBOX_AR, m_nOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlStifDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlStifDlg)
	ON_BN_CLICKED(IDC_DGN_STIFFBOX_SECT, OnDgnStiffboxSect)
	ON_BN_CLICKED(IDC_DGN_STIFFBOX_AR, OnDgnStiffboxAr)
	ON_BN_CLICKED(IDC_DGN_STIFFBOX_DEL, OnDgnStiffboxDel)
	ON_CBN_SELCHANGE(IDC_DGN_STIFFBOX_PROPNO, OnSelchangeDgnStiffboxPropno)
	ON_NOTIFY(UDN_DELTAPOS, IDC_DGN_STIFFBOX_WSPIN, OnDeltaposDgnStiffboxWspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_DGN_STIFFBOX_FSPIN, OnDeltaposDgnStiffboxFspin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlStifDlg message handlers
void CDgnStlStifDlg::Execute() 
{
	int key,Index;
	CString strKey=_T("");
	Index = m_PropNo.GetCurSel();
	if(Index != -1) 
	{
		m_PropNo.GetLBText(Index,strKey);
		key = _ttoi(strKey);

	  //	Option의 _T("add/replace")와 _T("Delete")의 선택여부를 Check합니다.
		if(m_nOption == 0)	//	add/replace
	  {
			UpdateData(TRUE);
			//if(ErrorCheck())
			{
				T_STFN_D rData;
				rData.Initialize();

				rData.Astf = m_Space.GetEditValue();
				rData.Bstf = m_Width.GetEditValue();
				rData.Tstf = m_Thick.GetEditValue();
				rData.Noy  = m_FSpin.GetEditValueInt();
				rData.Noz  = m_WSpin.GetEditValueInt();

				if(m_pDoc->m_pDataCtrl->AddStfn(key,rData))
				{
					// Initialize selected Element.
					Initial_SelectItem();
				}
			}
		}
		else	// Delete
		{
			// Stiffener Data를 DB에서 지워버리자 !!!
			if(m_pDoc->m_pDataCtrl->DelStfn(key))
			{
				// Initialize selected Element.
				Initial_SelectItem();
			}
		}
	}
}

BOOL CDgnStlStifDlg::ErrorCheck()
{
	double H   = m_H.GetEditValue();
	double B   = m_B.GetEditValue();
	double Tw  = m_Tw.GetEditValue();
	double Tf1 = m_Tf.GetEditValue();
	double Tf2 = m_Tf2.GetEditValue();

	double Bd = B-2.*Tw;
	double Hd = H-Tf1-Tf2;
	double minLen = min(Bd,Hd);

	BOOL bCheck=TRUE;
	double Width = m_Width.GetEditValue();
	if(Width <= 0.)
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_LONG_STIFF_WIDTH1),MB_OK);
		return bCheck;
	}
	if(Width > minLen/2.)
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_LONG_STIFF_WIDTH2),MB_OK);
		return bCheck;
	}

	if(m_Space.GetEditValue() <= 0.)
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_TRAN_STIFF_SPACE),MB_OK);
		return bCheck;
	}
	int FSideNo = m_FSpin.GetEditValueInt();
	if(FSideNo < 2)
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_FLANGE_SIDE_PANEL),MB_OK);
		return bCheck;
	}
	int WSideNo = m_WSpin.GetEditValueInt();
	if(WSideNo < 2)
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_WEB_SIDE_PANEL),MB_OK);
		return bCheck;
	}

	int nNoy,nNoz;
	nNoy = FSideNo-1;
	nNoz = WSideNo-1;
	double Thick = m_Thick.GetEditValue();
	if(Thick <= 0.)
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_LONG_STIF_THIK1),MB_OK);
		return bCheck;
	}
	if(nNoy*Thick > Bd)
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_LONG_STIF_THIK2),MB_OK);
		return bCheck;
	}
	if(nNoz*Thick > Hd)
	{
		bCheck=FALSE;
		AfxMessageBox(_LS(IDS_DGN_CHK_LONG_STIF_THIK3),MB_OK);
		return bCheck;
	}
	return bCheck;
}

void CDgnStlStifDlg::Initial_SelectItem()
{
	/////////////////////////////////////////////////////////////////////////////////////
	//  ComboBox의 Property No를 초기화하자 !!!
	m_PropNo.SetCurSel(-1);
	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount())  m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	//  Data를 초기화시킵니다.
	Initial_Data();
	Initial_WithoutData();
}

BOOL CDgnStlStifDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return TRUE;
//**/  if(m_pDoc->m_pViewCtrl->SelectedElemCount())  m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	// ComboBox Data를 초기화합니다.
	Initial_PropCombo();
	//  Unit를 초기화합니다.
	Initial_Unit();
	// 일반 Data를 초기화합니다.
	Initial_Data();

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;
}

void CDgnStlStifDlg::Initial_PropCombo()
{
	if(m_PropNo.GetCount() != 0)    m_PropNo.ResetContent();
	if(m_PropData.GetCount() != 0)  m_PropData.RemoveAll();

	//  Element의 갯수를 받아오자.
	CArray<T_ELEM_K, T_ELEM_K> keyList;
	m_pDoc->m_pAttrCtrl->GetElemKeyList(keyList);
	int iCount = keyList.GetSize();
	for(int i=0; i<iCount; i++)
	{
		int key = keyList.GetAt(i);

		T_ELEM_D eData;
		eData.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetElem(key,eData))	ASSERT(0);

		T_SECT_D sData;
		sData.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetSectDesign(eData.elpro,sData))
		{
			if(sData.SectBefore.Shape == _T("B"))
			{
				m_PropData.SetAt(eData.elpro,eData.elpro);
			}
		}
	}

	int Index;
	POSITION pos;
	for(pos=m_PropData.GetStartPosition(); pos != NULL;)
	{
		int Data;
		m_PropData.GetNextAssoc(pos,Index,Data);

		CString strPropNo=_T("");
		strPropNo.Format(_T("%-6d"),Data);
		m_PropNo.AddString(strPropNo);
	}
}

void CDgnStlStifDlg::Initial_Unit()
{
	m_Width.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Thick.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Space.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_B.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Cw.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_H.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Tf2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Tf.SetUnitType(D_UNITSYS_BASE_LENGTH);	
	m_Tw.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_TwUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Tf2Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_TfUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_HUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_CwUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_BUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_WUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_TUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_SpaceUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CDgnStlStifDlg::Initial_Data()
{
	m_nOption = 0;    //  add/replace
	m_bUpdate = FALSE;

	m_B.SetEditUnit(0.);
	m_Cw.SetEditUnit(0.);
	m_H.SetEditUnit(0.);
	m_Tf2.SetEditUnit(0.);
	m_Tf.SetEditUnit(0.);
	m_Tw.SetEditUnit(0.);

	m_Width.SetEditUnit(0.);
	m_Thick.SetEditUnit(0.);
	m_Space.SetEditUnit(0.);

	m_B.EnableWindow(FALSE);
	m_Cw.EnableWindow(FALSE);
	m_H.EnableWindow(FALSE);
	m_Tf2.EnableWindow(FALSE);
	m_Tf.EnableWindow(FALSE);
	m_Tw.EnableWindow(FALSE);

	m_FSpin.SetValue(2);
	m_FSpin.SetRange(1,100);
	m_FSpin.SetInteger(TRUE);
	m_WSpin.SetValue(2);
	m_WSpin.SetRange(1,100);
	m_WSpin.SetInteger(TRUE);

	m_nFSpinNo = 2;
	m_nWSpinNo = 2;
	// Add by ZINU.('01.1.3).
	Update_InitDataByCode();

	UpdateData(FALSE);
}

void CDgnStlStifDlg::OnDgnStiffboxSect() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_STRUCT_PROP_SECT4DGN,0));
}

void CDgnStlStifDlg::OnDgnStiffboxAr() 
{
	m_nOption  = 0;			//	add/replace
	UpdateData(FALSE);
}

void CDgnStlStifDlg::OnDgnStiffboxDel() 
{
	m_nOption  = 1;			//	Delete
	UpdateData(FALSE);
}

void CDgnStlStifDlg::Change_Section4Property()
{
	int Index = m_PropNo.GetCurSel();
	if(Index != -1)
	{
		CString strPropNo=_T("");
		m_PropNo.GetLBText(Index,strPropNo);
		int key = _ttoi(strPropNo);
		m_ProNo = key;

		// 뷰에 선택을 하자 !!!
		if(!m_bUpdate)  Selected_Element(key);

		T_SECT_D sData;
		sData.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetSectDesign(key,sData))
		{
			if(sData.SectBefore.Shape==_T("B"))
			{
				m_H.SetEditUnit(sData.SectBefore.SectI.Size[0]);
				m_B.SetEditUnit(sData.SectBefore.SectI.Size[1]);
				m_Tw.SetEditUnit(sData.SectBefore.SectI.Size[2]);
				m_Tf.SetEditUnit(sData.SectBefore.SectI.Size[3]);
				m_Cw.SetEditUnit(sData.SectBefore.SectI.Size[4]);
				m_Tf2.SetEditUnit(sData.SectBefore.SectI.Size[5]);
			}
		}
		T_STFN_D rData;
		rData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStfn(key,rData);
		if(bCheck)  Initial_ExistData(rData);
		else        Initial_WithoutData();
	}
}

void CDgnStlStifDlg::Initial_ExistData(T_STFN_D Data)
{
	//  Transverse Stiffener...
	m_Space.SetEditUnit(Data.Astf);
	//  Logitudinal Stiffener...
	m_Width.SetEditUnit(Data.Bstf);
	m_Thick.SetEditUnit(Data.Tstf);
	//  Flange Side...
	m_FSpin.SetValue(Data.Noy);
	//  Web Side...
	m_WSpin.SetValue(Data.Noz);

	m_nFSpinNo = Data.Noy;
	m_nWSpinNo = Data.Noz;

	UpdateData(FALSE);
}

void CDgnStlStifDlg::Initial_WithoutData()
{
	m_Width.SetEditUnit(0.);
	m_Thick.SetEditUnit(0.);
	m_Space.SetEditUnit(0.);

	m_FSpin.SetValue(2);
	m_WSpin.SetValue(2);

	m_nFSpinNo = 2;
	m_nWSpinNo = 2;

	UpdateData(FALSE);
}

void CDgnStlStifDlg::Selected_Element(int PropNo)
{
	//  선택된 것들을 지우자 !!!
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);

	CArray<T_ELEM_K,T_ELEM_K> ElemKey;
	POSITION pos;
	T_ELEM_K key;
	T_ELEM_D rData;

	for(pos=m_pDoc->m_pAttrCtrl->GetStartElem(); pos!=NULL;)
	{
	  rData.Initialize();
		m_pDoc->m_pAttrCtrl->GetNextElem(pos,key,rData);
		if(rData.elpro == PropNo) 
		{
			if(rData.eltyp == TRUSS_EL || rData.eltyp == BEAM_EL) ElemKey.Add(key);
		}
	}
	// 그려라 !!! View에...
	m_pDoc->m_pViewCtrl->SelectElem(NULL,ElemKey,FALSE);
}

void CDgnStlStifDlg::OnSelchangeDgnStiffboxPropno() 
{
	Change_Section4Property();
}

void CDgnStlStifDlg::OnDeltaposDgnStiffboxWspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str=_T("");
	GetDlgItem(IDC_DGN_STIFFBOX_WSIDE)->GetWindowText(str);

	int Factor = (int)(pNMUpDown->iDelta);
	m_nWSpinNo = _ttoi(str)+1*Factor;

	if(m_nWSpinNo < 2)
	{
		if(Factor == 1) m_nWSpinNo = 2;
		else            m_nWSpinNo = 100;
	}
	else if(m_nWSpinNo > 100)  m_nWSpinNo = 2;

	str.Format(_T("%d"),m_nWSpinNo);
	GetDlgItem(IDC_DGN_STIFFBOX_WSIDE)->SetWindowText(str);

	*pResult = 0;
}

void CDgnStlStifDlg::OnDeltaposDgnStiffboxFspin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	CString str=_T("");
	GetDlgItem(IDC_DGN_STIFFBOX_FSIDE)->GetWindowText(str);

	int Factor = (int)(pNMUpDown->iDelta);
	m_nFSpinNo = _ttoi(str)+1*Factor;

	if(m_nFSpinNo < 2)
	{
		if(Factor == 1) m_nFSpinNo = 2;
		else            m_nFSpinNo = 100;
	}
	else if(m_nFSpinNo > 100)  m_nFSpinNo = 2;

	str.Format(_T("%d"),m_nFSpinNo);
	GetDlgItem(IDC_DGN_STIFFBOX_FSIDE)->SetWindowText(str);

	*pResult = 0;
}

void CDgnStlStifDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
		    UpdateBuffer();
				// Add by ZINU.('01.1.3).
				Update_InitDataByCode();
	      break;
			}
		default:
			//ASSERT(FALSE);
			break;
	  }
}

void CDgnStlStifDlg::UpdateBuffer()
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
			case(UR_SECD_ADD):
			case(UR_SECD_DEL):
			case(UR_SECD_MFS):
			case(UR_SECD_MFD):	{bMFD=TRUE;		break;}
			default:						{bOther=TRUE;	break;}
		}
	}

	if(bMFD && !bOther) 
	{
		m_bUpdate = TRUE;
		Initial_PropCombo();
		Change_Section4Property();
		m_bUpdate = FALSE;
	}
//**/	else	Define_PropertyCombo();
}

void CDgnStlStifDlg::Define_PropertyCombo()
{
	//  이미 Property ComboBox에 존재하는 놈의 Property NO를 저장합니다.
	CString strPropNo=_T("");
	int Index = m_PropNo.GetCurSel();
	if(Index != -1)  m_PropNo.GetLBText(Index,strPropNo);

	int key = _ttoi(strPropNo);
	//  Property ComboBox를 초기화합니다.
	if(m_PropNo.GetCount() != 0)  m_PropNo.ResetContent();
	Initial_PropCombo();

	Index = m_PropNo.FindStringExact(-1,strPropNo);
	if(Index != CB_ERR)
	{
		m_bUpdate = TRUE;
		m_PropNo.SetCurSel(Index);
		Change_Section4Property();
		m_bUpdate = FALSE;
	}
	else
	{
		m_nOption = 0;    //  add/replace
		//  대화상자가 바뀌면 변수를 초기화합니다.
	  m_B.SetEditUnit(0.);
	  m_Cw.SetEditUnit(0.);
	  m_H.SetEditUnit(0.);
	  m_Tf2.SetEditUnit(0.);
	  m_Tf.SetEditUnit(0.);
	  m_Tw.SetEditUnit(0.);

		T_STFN_D rData;
		rData.Initialize();
		BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStfn(key,rData);
		if(bCheck)  Initial_ExistData(rData);
		else        Initial_WithoutData();
	}
}

// Add by ZINU.('01.1.3).
void CDgnStlStifDlg::Update_InitDataByCode()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleStl(ID_DGN_STL_STFN, strTitle, bShow);

	GetDlgItem(IDC_DGN_STIFFBOX_AR)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STIFFBOX_DEL)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STIFFBOX_PROPNO)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STIFFBOX_SECT)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STIFFBOX_H)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STIFFBOX_B)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STIFFBOX_TW)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STIFFBOX_TF)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STIFFBOX_CW)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STIFFBOX_TF2)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STIFFBOX_WIDTH)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STIFFBOX_THICK)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STIFFBOX_SPACE)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STIFFBOX_FSIDE)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STIFFBOX_WSIDE)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STIFFBOX_FSPIN)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STIFFBOX_WSPIN)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(bShow);
}