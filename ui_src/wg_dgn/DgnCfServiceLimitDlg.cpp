// DgnCfServiceLimitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnCfServiceLimitDlg.h"

#include "DgnStruct.h"
#include "DgnDataCtrl.h"

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
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_main\wg_mainres2.h"	

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnCfServiceLimitDlg dialog


CDgnCfServiceLimitDlg::CDgnCfServiceLimitDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnCfServiceLimitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnCfServiceLimitDlg)
	m_iVertical = -1;
	m_dVerticalUser = 0.0;
	m_dDAF = 0.0;
	m_nOption = -1;
	m_nSeletType = -1;
	m_iHorizontal = -1;
	m_dHorizontalUser = 0.0;
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	m_arIDBeam.RemoveAll();
	m_arIDColm.RemoveAll();
	m_arIDEtc.RemoveAll();

	m_arIDBeam.Add(IDC_DGN_STL_DeflectFrm1);
	m_arIDBeam.Add(IDC_DGN_STL_BDEFLECT11);
	m_arIDBeam.Add(IDC_DGN_STL_BDEFLECT12);
	m_arIDBeam.Add(IDC_DGN_STL_BDEFLECT13);
	m_arIDBeam.Add(IDC_DGN_STL_BDEFLECT14);
	m_arIDBeam.Add(IDC_DGN_STL_BDEFLECT15);
	
	m_arIDColm.Add(IDC_DGN_STL_DeflectFrm2);
	m_arIDColm.Add(IDC_DGN_STL_BDEFLECT31);
	m_arIDColm.Add(IDC_DGN_STL_BDEFLECT32);
	m_arIDColm.Add(IDC_DGN_STL_BDEFLECT33);
	m_arIDColm.Add(IDC_DGN_STL_BDEFLECT34);
	m_arIDColm.Add(IDC_DGN_STL_BDEFLECT35);

	m_arIDEtc.Add(IDC_DGN_STL_DeflectFrm3);
	m_arIDEtc.Add(IDC_DGN_STL_DAF);
	m_arIDEtc.Add(IDC_DGN_EXECUTE);
	m_arIDEtc.Add(IDC_DGN_CLOSE);
}


void CDgnCfServiceLimitDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnCfServiceLimitDlg)
	DDX_Radio(pDX, IDC_DGN_STL_BDEFLECT31, m_iVertical);
	DDX_Text (pDX, IDC_DGN_STL_BDEFLECT35, m_dVerticalUser);
	DDX_Text (pDX, IDC_DGN_STL_DAF,        m_dDAF);
	DDX_Radio(pDX, IDC_DGN_STL_SERV_AR,    m_nOption);
	DDX_Radio(pDX, IDC_DGN_STL_SELECT_ALL, m_nSeletType);
	DDX_Radio(pDX, IDC_DGN_STL_BDEFLECT11, m_iHorizontal);
	DDX_Text(pDX,  IDC_DGN_STL_BDEFLECT15, m_dHorizontalUser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnCfServiceLimitDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnCfServiceLimitDlg)
	//ON_BN_CLICKED(IDC_DGN_EXECUTE,        OnDgnExecute)
	//ON_BN_CLICKED(IDC_DGN_CLOSE,          OnDgnClose)
	ON_BN_CLICKED(IDC_DGN_STL_SERV_AR,    OnDgnConServAr  )
	ON_BN_CLICKED(IDC_DGN_STL_SERV_DEL,   OnDgnConServDel )
	ON_BN_CLICKED(IDC_DGN_STL_BDEFLECT11, OnHorizontalUser)
	ON_BN_CLICKED(IDC_DGN_STL_BDEFLECT12, OnHorizontalUser)
	ON_BN_CLICKED(IDC_DGN_STL_BDEFLECT13, OnHorizontalUser)
	ON_BN_CLICKED(IDC_DGN_STL_BDEFLECT14, OnHorizontalUser)
	ON_BN_CLICKED(IDC_DGN_STL_BDEFLECT31, OnVerticalUser)
	ON_BN_CLICKED(IDC_DGN_STL_BDEFLECT32, OnVerticalUser)
	ON_BN_CLICKED(IDC_DGN_STL_BDEFLECT33, OnVerticalUser)
	ON_BN_CLICKED(IDC_DGN_STL_BDEFLECT34, OnVerticalUser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnCfServiceLimitDlg message handlers

BOOL CDgnCfServiceLimitDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;

	Initial_Data();

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnCfServiceLimitDlg::Initial_Data()
{
	GetDesignCodeNo(CDBLib::GetCfsCodeName());

	m_nOption	= 0;      // add/replace
	m_nSeletType = 1;

	int nSubCode = CDBLib::GetCfsSubCode();// 0: Recommended, 1:Singapore. 2:Italy
	if (nSubCode == 0 || nSubCode == 2)
	{
		m_iHorizontal = 1;
		m_iVertical   = 1;
		m_dHorizontalUser = 250.0;
		m_dVerticalUser   = 250.0;
	}
	else if (nSubCode == 1)
	{
		m_iHorizontal = 0;
		m_iVertical   = 0;
		m_dHorizontalUser = 200.0; // NA.2.23. Vertical deflections for beams (horizontal member)
		m_dVerticalUser   = 300.0; // NA.2.24. Horizontal deflections for columns (vertical member)
	}
	else ASSERT(0);

	m_dDAF = 1.0;

	Update_InitDataByCode();

	UpdateData(FALSE);

	OnHorizontalUser();
	OnVerticalUser();
}

void CDgnCfServiceLimitDlg::OnDgnExecute() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;  aSelKey.RemoveAll();
	CArray<T_ELEM_K,T_ELEM_K> rBeamSelKey;
	CArray<T_ELEM_K,T_ELEM_K> rColmSelKey;
	if(m_nSeletType)
	{
		I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
		pIGM->GetSelectedElemKeyListForDgn(aSelKey);
	}
	else
	{
		m_pDoc->m_pAttrCtrl->GetElemKeyList(aSelKey);  
	}

	if(aSelKey.GetSize() > 0)
	{
		CString strCfsCodeName = CDBLib::GetCfsCodeName();

		T_MATD_D MatlD; MatlD.Initialize();
		T_SECT_D SectD; SectD.Initialize();
		for(int i=0; i<aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);

			// Check Material.
			T_ELEM_D ElemD; ElemD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetElem(key, ElemD))       continue;

			if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatlD)) continue;
			if(MatlD.Type != _T("S")) continue; 

			if(!m_pDoc->m_pAttrCtrl->GetSectDesign(ElemD.elpro, SectD)) continue;
			if(!m_pDoc->m_pAttrCtrl->IsColdFormedSect4Code(strCfsCodeName, SectD.SectBefore.Shape, TRUE)) continue;

			int nMbType = m_pDoc->m_pAttrCtrl->GetMemberType(key);    
			if(nMbType == D_MBTP_BEAM )       rBeamSelKey.Add(key);
			else if(nMbType==D_MBTP_COLUMN)   rColmSelKey.Add(key);      
			else if(nMbType==D_MBTP_BRACE)  
			{
				int nKey = (int)key;
				CString strMsg = _T("");
				strMsg.Format(_LS(IDS_DB_SERVLIMIT_ERR_BRACE),nKey);
				GSaveHistoryFormatNF(strMsg);

				continue;
			}
			else ASSERT(0); continue;
		}

		T_SERV_D sData;

		if(m_nOption==0)	// add/replace
		{
			CDgnDataCtrl* pDataCtrl = new CDgnDataCtrl;

			BOOL bAssign = FALSE;
			if(rBeamSelKey.GetSize() > 0 || rColmSelKey.GetSize() > 0)
			{
				if(rBeamSelKey.GetSize() > 0)
				{ 
					int    iEnv   = m_iHorizontal;
					double dLimit = pDataCtrl->GetDeflectLimByCode4ColdFormed(D_MBTP_BEAM,iEnv);

					sData.Initialize();
					sData.iDeflecEnv  = m_iHorizontal;
					sData.dDefLim     = m_iHorizontal == iEnv ? dLimit : m_dHorizontalUser;
					sData.dDAF        = m_dDAF;
					// Initialize selected Element.
					if(m_pDoc->m_pDataCtrl->AddServ(rBeamSelKey,sData))	bAssign = TRUE;
				}

				if(rColmSelKey.GetSize() > 0)      
				{
					int    iEnv   = m_iVertical;
					double dLimit = pDataCtrl->GetDeflectLimByCode4ColdFormed(D_MBTP_COLUMN,iEnv);

					sData.Initialize();
					sData.iDeflecEnv  = m_iVertical;
					sData.dDefLim     = m_iVertical == iEnv ? dLimit : m_dVerticalUser;
					sData.dDAF        = m_dDAF;
					// Initialize selected Element.
					if(m_pDoc->m_pDataCtrl->AddServ(rColmSelKey,sData))	bAssign = TRUE;
				}
				if(bAssign) Initial_SelectItem();
			}
			else	AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_BEAM_Cfs),MB_OK);

			delete pDataCtrl;
			pDataCtrl = NULL;    
		}
		else	// Delete
		{
			// Initialize selected Element.
			if(m_pDoc->m_pDataCtrl->DelServ(rBeamSelKey))	Initial_SelectItem();
			if(m_pDoc->m_pDataCtrl->DelServ(rColmSelKey))	Initial_SelectItem();
		}    
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);

	Initial_SelectItem();	
}

void CDgnCfServiceLimitDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}

void CDgnCfServiceLimitDlg::OnHorizontalUser() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	BOOL bEnable=FALSE;
	if(m_iHorizontal==3)  bEnable=TRUE;
	else                  bEnable=FALSE;
	GetDlgItem(IDC_DGN_STL_BDEFLECT15)->EnableWindow(bEnable);

	UpdateData(FALSE);	
}

void CDgnCfServiceLimitDlg::OnVerticalUser() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	BOOL bEnable=FALSE;
	if(m_iVertical==3)  bEnable=TRUE;
	else                bEnable=FALSE;
	GetDlgItem(IDC_DGN_STL_BDEFLECT35)->EnableWindow(bEnable);

	UpdateData(FALSE);
}

void CDgnCfServiceLimitDlg::UpdateBuffer()
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
		case(UR_DCFS_ADD):
		case(UR_DCFS_DEL):
			{
				Initial_Data();
				break;      
			}
		default:	break;
		}
	}
}

void CDgnCfServiceLimitDlg::EnableItems(BOOL bEnable)
{
	int nNum = m_arIDBeam.GetSize();
	for(int i=0; i<nNum; i++)
	{
		GetDlgItem(m_arIDBeam[i])->EnableWindow(bEnable);
	}
	nNum = m_arIDColm.GetSize();
	for(int i=0; i<nNum; i++)
	{
		GetDlgItem(m_arIDColm[i])->EnableWindow(bEnable);
	}
	nNum = m_arIDEtc.GetSize();
	for(int i=0; i<nNum-1; i++)
	{
		GetDlgItem(m_arIDEtc[i])->EnableWindow(bEnable);
	}

	int nNationalAnnex = CDBLib::GetCfsSubCode();

	GetDlgItem(IDC_DGN_STL_BDEFLECT12)->ShowWindow(TRUE);
	GetDlgItem(IDC_DGN_STL_BDEFLECT13)->ShowWindow(TRUE);

	if (nNationalAnnex==1)
	{			
		GetDlgItem(IDC_DGN_STL_BDEFLECT32)->ShowWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_BDEFLECT33)->ShowWindow(FALSE);
	}
	else
	{			
		GetDlgItem(IDC_DGN_STL_BDEFLECT32)->ShowWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_BDEFLECT33)->ShowWindow(TRUE);
	}

	MoveCtrl();
}

void CDgnCfServiceLimitDlg::MoveCtrl()
{
	CRect RectCng1, RectCng2;
	double dSpace1=0., dSpace2=0., dSpace3=0.;
	GetDlgItem(IDC_DGN_STL_BDEFLECT11)->GetWindowRect(RectCng1);
	GetDlgItem(IDC_DGN_STL_BDEFLECT12)->GetWindowRect(RectCng2);
	dSpace1 = RectCng2.top - RectCng1.bottom;

	GetDlgItem(IDC_DGN_STL_BDEFLECT13)->GetWindowRect(RectCng1);
	GetDlgItem(IDC_DGN_STL_BDEFLECT14)->GetWindowRect(RectCng2);
	dSpace2 = RectCng2.top - (RectCng1.bottom + dSpace1);
	RectCng2.top    = RectCng1.bottom + dSpace1;
	ScreenToClient(RectCng2);
	GetDlgItem(IDC_DGN_STL_BDEFLECT14)->SetWindowPos(NULL, RectCng2.left, RectCng2.top, RectCng2.Width(), RectCng2.Height(), SWP_NOSIZE | SWP_NOZORDER);

	GetDlgItem(IDC_DGN_STL_BDEFLECT15)->GetWindowRect(RectCng2);
	RectCng2.top    -= dSpace2;
	ScreenToClient(RectCng2);
	GetDlgItem(IDC_DGN_STL_BDEFLECT15)->SetWindowPos(NULL, RectCng2.left, RectCng2.top, RectCng2.Width(), RectCng2.Height(), SWP_NOSIZE | SWP_NOZORDER);

	GetDlgItem(IDC_DGN_STL_DeflectFrm1)->GetWindowRect(RectCng2);
	RectCng2.bottom -= dSpace2;
	ScreenToClient(RectCng2);
	GetDlgItem(IDC_DGN_STL_DeflectFrm1)->SetWindowPos(NULL, RectCng2.left, RectCng2.top, RectCng2.Width(), RectCng2.Height(), SWP_NOZORDER);

	GetDlgItem(IDC_DGN_STL_DeflectFrm2)->GetWindowRect(RectCng2);
	RectCng2.top -= dSpace2;
	ScreenToClient(RectCng2);
	GetDlgItem(IDC_DGN_STL_DeflectFrm2)->SetWindowPos(NULL, RectCng2.left, RectCng2.top, RectCng2.Width(), RectCng2.Height(), SWP_NOSIZE | SWP_NOZORDER);

	GetDlgItem(IDC_DGN_STL_BDEFLECT31)->GetWindowRect(RectCng2);
	RectCng2.top -= dSpace2;
	ScreenToClient(RectCng2);
	GetDlgItem(IDC_DGN_STL_BDEFLECT31)->SetWindowPos(NULL, RectCng2.left, RectCng2.top, RectCng2.Width(), RectCng2.Height(), SWP_NOSIZE | SWP_NOZORDER);

	int nNationalAnnex = CDBLib::GetCfsSubCode();
	if (nNationalAnnex==1)
	{
		GetDlgItem(IDC_DGN_STL_BDEFLECT31)->GetWindowRect(RectCng1);			
	}
	else
	{
		GetDlgItem(IDC_DGN_STL_BDEFLECT33)->GetWindowRect(RectCng1);			
	}

	GetDlgItem(IDC_DGN_STL_BDEFLECT34)->GetWindowRect(RectCng2);
	dSpace3 = RectCng2.top - (RectCng1.bottom + dSpace1);
	RectCng2.top = RectCng1.bottom + dSpace1;
	ScreenToClient(RectCng2);
	GetDlgItem(IDC_DGN_STL_BDEFLECT34)->SetWindowPos(NULL, RectCng2.left, RectCng2.top, RectCng2.Width(), RectCng2.Height(), SWP_NOSIZE | SWP_NOZORDER);

	GetDlgItem(IDC_DGN_STL_BDEFLECT34)->GetWindowRect(RectCng1);
	GetDlgItem(IDC_DGN_STL_BDEFLECT35)->GetWindowRect(RectCng2);
	RectCng2.top    -= dSpace3;
	ScreenToClient(RectCng2);
	GetDlgItem(IDC_DGN_STL_BDEFLECT35)->SetWindowPos(NULL, RectCng2.left, RectCng2.top, RectCng2.Width(), RectCng2.Height(), SWP_NOSIZE | SWP_NOZORDER);

	GetDlgItem(IDC_DGN_STL_DeflectFrm2)->GetWindowRect(RectCng2);
	RectCng2.bottom -= dSpace3;
	ScreenToClient(RectCng2);
	GetDlgItem(IDC_DGN_STL_DeflectFrm2)->SetWindowPos(NULL, RectCng2.left, RectCng2.top, RectCng2.Width(), RectCng2.Height(), SWP_NOZORDER);

	GetDlgItem(IDC_DGN_STL_DeflectFrm3)->GetWindowRect(RectCng2);
	RectCng2.top -= dSpace3;
	ScreenToClient(RectCng2);
	GetDlgItem(IDC_DGN_STL_DeflectFrm3)->SetWindowPos(NULL, RectCng2.left, RectCng2.top, RectCng2.Width(), RectCng2.Height(), SWP_NOSIZE | SWP_NOZORDER);

	GetDlgItem(IDC_DGN_STL_DAF)->GetWindowRect(RectCng2);
	RectCng2.top -= dSpace3;
	ScreenToClient(RectCng2);
	GetDlgItem(IDC_DGN_STL_DAF)->SetWindowPos(NULL, RectCng2.left, RectCng2.top, RectCng2.Width(), RectCng2.Height(), SWP_NOSIZE | SWP_NOZORDER);

	GetDlgItem(IDC_DGN_EXECUTE)->GetWindowRect(RectCng2);
	RectCng2.top -= dSpace3;
	ScreenToClient(RectCng2);
	GetDlgItem(IDC_DGN_EXECUTE)->SetWindowPos(NULL, RectCng2.left, RectCng2.top, RectCng2.Width(), RectCng2.Height(), SWP_NOSIZE | SWP_NOZORDER);

	GetDlgItem(IDC_DGN_CLOSE)->GetWindowRect(RectCng2);
	RectCng2.top -= dSpace3;
	ScreenToClient(RectCng2);
	GetDlgItem(IDC_DGN_CLOSE)->SetWindowPos(NULL, RectCng2.left, RectCng2.top, RectCng2.Width(), RectCng2.Height(), SWP_NOSIZE | SWP_NOZORDER);
}

// 아이템들을 enable / disable 시킨다.
void CDgnCfServiceLimitDlg::Update_InitDataByCode()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleCfs(ID_DGN_CF_SERV, strTitle, bShow);
	if(m_iCode == Eurocode3_1_3_06)
	{
		int nNationalAnnex = CDBLib::GetCfsSubCode();
		if (nNationalAnnex==1)
		{
			GetDlgItem(IDC_DGN_STL_BDEFLECT11)->SetWindowText(_T("L / 200"));
			GetDlgItem(IDC_DGN_STL_BDEFLECT12)->SetWindowText(_T("L / 360"));
			GetDlgItem(IDC_DGN_STL_BDEFLECT13)->SetWindowText(_T("L / 180"));

			GetDlgItem(IDC_DGN_STL_BDEFLECT31)->SetWindowText(_T("h / 300"));
		}
		else
		{
			GetDlgItem(IDC_DGN_STL_BDEFLECT11)->SetWindowText(_T("L / 400"));
			GetDlgItem(IDC_DGN_STL_BDEFLECT31)->SetWindowText(_T("h / 500"));
		}
	}
	if (m_iCode == IRC24_2010) {
		GetDlgItem(IDC_DGN_STL_BDEFLECT11)->SetWindowText(_T("L / 600"));
		GetDlgItem(IDC_DGN_STL_BDEFLECT12)->SetWindowText(_T("L / 800"));
		GetDlgItem(IDC_DGN_STL_BDEFLECT13)->SetWindowText(_T("L / 300"));
		GetDlgItem(IDC_DGN_STL_BDEFLECT32)->EnableWindow(bShow);
		GetDlgItem(IDC_DGN_STL_BDEFLECT33)->EnableWindow(bShow);
	}
	EnableItems(bShow);

	GetDlgItem(IDC_STATIC_SELECT_TYPE)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STL_SELECT_ALL)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STL_SELECT_SEL)->EnableWindow(bShow);

	GetDlgItem(IDC_STATIC_OPTION)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STL_SERV_AR)->EnableWindow(bShow);    
	GetDlgItem(IDC_DGN_STL_SERV_DEL)->EnableWindow(bShow);    

	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(bShow);
}

void CDgnCfServiceLimitDlg::GetDesignCodeNo(CString strCode) 
{
	if(strCode==_T("AIK-CFSD98"))	            m_iCode = AIK_CFSD98  ;
	else if(strCode==_T("AISI-CFSD86"))	        m_iCode = AISI_CFSD86 ;
	else if(strCode==_T("Eurocode3-1-3:06"))	m_iCode = Eurocode3_1_3_06 ;
	else if(strCode==_T("AISI-CFSD08"))	        m_iCode = AISI_CFSD08 ;
	else if (strCode==_T("GB50018-02"))	        m_iCode = GB50018_02;
	else if (strCode == _T("GB/T50018-25"))	    m_iCode = GB50018_25;
	else ASSERT(0);
}

void CDgnCfServiceLimitDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

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

// If check the 'Add/Replace' radio button.
void CDgnCfServiceLimitDlg::OnDgnConServAr() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);  
	m_nOption  = 0;			//	Add/Replace.
	EnableItems(TRUE);
	OnHorizontalUser();
	OnVerticalUser();
	UpdateData(FALSE);
}

// If check the 'Delete' radio button.
void CDgnCfServiceLimitDlg::OnDgnConServDel() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nOption  = 1;			//	Delete.
	EnableItems(FALSE);
	GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
	UpdateData(FALSE);
}

// 선택된 element를 모두 unselect시킨다.
void CDgnCfServiceLimitDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount())
	{
		m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);    
	}
	// Initialize Data.
	Initial_Data();
}