// DgnStlServiceLimitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlServiceLimitDlg.h"

#include "DgnStruct.h"
#include "DgnDataCtrl.h"
#include "DgnCodeCtrl.h"

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
// CDgnStlServiceLimitDlg dialog


CDgnStlServiceLimitDlg::CDgnStlServiceLimitDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnStlServiceLimitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnStlServiceLimitDlg)
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
	m_arIDBeam.Add(IDC_DGN_STL_BDEFLECT17);

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

	m_strDeflectFrm1Init = _T("");
	m_strDeflectFrm2Init = _T("");
}


void CDgnStlServiceLimitDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlServiceLimitDlg)
	DDX_Radio(pDX, IDC_DGN_STL_BDEFLECT31, m_iVertical);
	DDX_Text (pDX, IDC_DGN_STL_BDEFLECT35, m_dVerticalUser);
	DDX_Text (pDX, IDC_DGN_STL_DAF,        m_dDAF);
	DDX_Radio(pDX, IDC_DGN_STL_SERV_AR,    m_nOption);
	DDX_Radio(pDX, IDC_DGN_STL_SELECT_ALL, m_nSeletType);
	DDX_Radio(pDX, IDC_DGN_STL_BDEFLECT11, m_iHorizontal);
	DDX_Text(pDX, IDC_DGN_STL_BDEFLECT15, m_dHorizontalUser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlServiceLimitDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnStlServiceLimitDlg)
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
// CDgnStlServiceLimitDlg message handlers

BOOL CDgnStlServiceLimitDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;

	// 초기 텍스트를 멤버 변수에 저장
	GetDlgItemText(IDC_DGN_STL_DeflectFrm1, m_strDeflectFrm1Init);
	GetDlgItemText(IDC_DGN_STL_DeflectFrm2, m_strDeflectFrm2Init);

	Initial_Data();
	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);

	 if (m_iCode == IRC24_2010) { // Added by Apoorva for IRC 24: 2010
		CWnd* pGroupBox = GetDlgItem(IDC_DGN_STL_DeflectFrm3);
		if (pGroupBox)
	    {
		   CRect rect;
		   pGroupBox->GetWindowRect(&rect);
		   ScreenToClient(&rect);   // Convert to client coordinates
		   rect.top -= 140;       // move up
		   rect.bottom -= 140;       // maintain height
		   pGroupBox->MoveWindow(&rect);
	     }
	    CWnd* pEditText = GetDlgItem(IDC_DGN_STL_DAF);
	    if (pEditText)
	    {
		   CRect rect;
		   pEditText->GetWindowRect(&rect);
		   ScreenToClient(&rect);
		   rect.top -= 140;
		   rect.bottom -= 140;
		   pEditText->MoveWindow(&rect);
	    }
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlServiceLimitDlg::Initial_Data()
{
	m_iCode = CDgnCodeCtrl::GetStlCodeID(CDBLib::GetStlCodeName());

	m_nOption	= 0;      // add/replace
	m_nSeletType = 1;

	// PMS:4130-Seungjun-20100205 Taiwan Steel Deflection checking (TWN-ASD96, LSD96).
    switch ( m_iCode )
    {
    case TWN_LSD96:
    case TWN_ASD96:
    case AISC_LRFD05:
    case AISC_ASD05:
    case AISC_LRFD10:
    case AISC_ASD10:
    case AISC_LRFD16:
    case AISC_ASD16:
	case AISC_LRFD22:
	case AISC_ASD22:
	case NSCP_2015_S_LRFD:
    case NSCP_2015_S_ASD:
        {
            m_iHorizontal = 0;
            m_dHorizontalUser = 360.0;

            m_iVertical = 0;
            m_dVerticalUser = 300.0;
        }
        break;
    case KDS_41_30_10_2022:
    case KDS_41_31_2019:
    case KSSC_LSD16:
    case KSSC_LSD09:
	case KR_BRG_LSD15:
        {
            m_iHorizontal = 0;
            m_dHorizontalUser = 300.0;

            m_iVertical = 0;
            m_dVerticalUser = 500.0;
        }
        break;
    case Eurocode3_05:
        {
            const int nSubCode = CDBLib::GetStlSubCode();
            switch ( nSubCode )
            {
            case dgn::def::enNationalAnnex::Sweden:
            case dgn::def::enNationalAnnex::Sweden19:
                {
                    m_iHorizontal = 3;
                    m_iVertical   = 3;
                    m_dHorizontalUser = 200.0;
                    m_dVerticalUser   = 200.0;
                }
                break;
            case dgn::def::enNationalAnnex::Singapore:
                {
                    m_iHorizontal = 0;
                    m_iVertical   = 0;
                    m_dHorizontalUser = 200.0; // NA.2.23. Vertical deflections for beams (horizontal member)
                    m_dVerticalUser   = 300.0; // NA.2.24. Horizontal deflections for columns (vertical member)
                }
                break;                
			default: // recommended
                {
                    m_iHorizontal = 1;
                    m_iVertical   = 1;
                    m_dHorizontalUser = 250.0;
                    m_dVerticalUser   = 250.0;
                }
                break;
                break;
            }            
        }
		break;
	case GBJ17_88:
	case GB50017_03:
	case GB50017_17:
	case GB51249_2017:
	case JGJ209_2010:
		{
			m_iHorizontal = 1;
			m_iVertical   = 1;
			m_dHorizontalUser = 250.0;
			m_dVerticalUser   = 250.0;
		}
        break;
	case AIJ_ASD02:
		{
			if ( m_pDoc->EnableJUD() )
			{
				m_iHorizontal = 3;
				m_dHorizontalUser = 250.0;

				m_iVertical = 3;
				m_dVerticalUser = 250.0;
			}
			else
			{
				ASSERT(0);
			}			
		}
		break;
    case IS800_1984:
    case IRS_SBC:
        m_iHorizontal = 3;
        m_iVertical = 3;
        m_dHorizontalUser = 200.0;
        m_dVerticalUser = 200.0;
        break;
    case SP_16_13330_2017:
		m_iHorizontal = 1;
		m_iVertical = 1;
		m_dHorizontalUser = 250.0;
		m_dVerticalUser = 250.0;
		break;
	case IS800_2007:
		m_iHorizontal = 0;
		m_dHorizontalUser = 360.0;
		m_iVertical = 0;
		m_dVerticalUser = 500.0;
		break;
	case IRC24_2010:
		m_iHorizontal = 0;
		m_dHorizontalUser = 0;
		m_iVertical = 0;
		m_dVerticalUser = 0;
		break;
    default:
        ASSERT(0);
        break;
    }
	
	m_dDAF = 1.0;

	Update_InitDataByCode();

	UpdateData(FALSE);

	OnHorizontalUser();
	OnVerticalUser();
}

void CDgnStlServiceLimitDlg::Execute() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//	Select된 Element를 얻어옵니다.
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
		BOOL bExistColdFormedSect = FALSE;
		for(int i=0; i<aSelKey.GetSize(); i++)
		{
			T_ELEM_K key = aSelKey.GetAt(i);

			// Check Material.
			T_ELEM_D ElemD; ElemD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetElem(key, ElemD))       continue;
			T_MATD_D MatlD; MatlD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatlD)) continue;
			if(MatlD.Type != _T("S")) continue; 

#ifdef _MGEN
			if(m_pDoc->m_pAttrCtrl->IsColdFormedSect(ElemD.elpro))
			{
				bExistColdFormedSect = TRUE;
				continue;
			}
#endif // _MGEN       

			//(20121109) : 해외사업팀 요청.   brace 경우 테이블에서 error 표시되는 것 => 테이블에서 제외. 
			int nMbType = m_pDoc->m_pAttrCtrl->GetMemberType(key);    
			if(nMbType == D_MBTP_BEAM )       rBeamSelKey.Add(key);
			else if(nMbType==D_MBTP_COLUMN)   rColmSelKey.Add(key);      
			else if(nMbType==D_MBTP_BRACE)  
			{
				int nKey = (int)key;
				//         m_pDoc->m_pPostCtrl->DisplayErrorMessage(_LS(IDS_DB_SERVLIMIT_ERR_BRACE), nKey);
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
					double dLimit = pDataCtrl->GetDeflectLimByCode(D_MBTP_BEAM,iEnv);

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
					double dLimit = pDataCtrl->GetDeflectLimByCode(D_MBTP_COLUMN,iEnv);

					sData.Initialize();
					sData.iDeflecEnv  = m_iVertical;
					sData.dDefLim     = m_iVertical == iEnv ? dLimit : m_dVerticalUser;
					sData.dDAF        = m_dDAF;
					// Initialize selected Element.
					if(m_pDoc->m_pDataCtrl->AddServ(rColmSelKey,sData))	bAssign = TRUE;
				}
				if(bAssign) Initial_SelectItem();
			}
			else
			{
				if(bExistColdFormedSect==TRUE) AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_BEAM_Stl),MB_OK);
			}

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

void CDgnStlServiceLimitDlg::OnHorizontalUser() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	BOOL bEnable=FALSE;
	if (m_iHorizontal == 4)  bEnable = TRUE;
	else                  bEnable=FALSE;
	GetDlgItem(IDC_DGN_STL_BDEFLECT15)->EnableWindow(bEnable);

	UpdateData(FALSE);	
}

void CDgnStlServiceLimitDlg::OnVerticalUser() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	BOOL bEnable=FALSE;
	if(m_iVertical==3)  bEnable=TRUE;
	else                bEnable=FALSE;
	GetDlgItem(IDC_DGN_STL_BDEFLECT35)->EnableWindow(bEnable);

	UpdateData(FALSE);
}

void CDgnStlServiceLimitDlg::UpdateBuffer()
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
		case(UR_DSTL_ADD):
		case(UR_DSTL_DEL):
			{
				Initial_Data();
				break;      
			}
		default:	break;
		}
	}
}

void CDgnStlServiceLimitDlg::EnableItems(BOOL bEnable)
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
	if ( m_iCode == IS800_2007 )
	{
		GetDlgItem( IDC_DGN_STL_DeflectFrm3 )->EnableWindow( FALSE );
		GetDlgItem( IDC_DGN_STL_DAF )->EnableWindow( FALSE );
	}
	BOOL bShowOne = IsShowOneDeflLimit(m_iCode);

	// PMS:4130-Seungjun-20100205 Taiwan Steel Deflection checking (TWN-ASD96, LSD96).
	if (bShowOne)
	{
		GetDlgItem(IDC_DGN_STL_BDEFLECT12)->ShowWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_BDEFLECT13)->ShowWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_BDEFLECT32)->ShowWindow(FALSE);
		GetDlgItem(IDC_DGN_STL_BDEFLECT33)->ShowWindow(FALSE);
	}
	else
	{
		int nNationalAnnex = CDBLib::GetStlSubCode();

		GetDlgItem(IDC_DGN_STL_BDEFLECT12)->ShowWindow(TRUE);
		GetDlgItem(IDC_DGN_STL_BDEFLECT13)->ShowWindow(TRUE);

		if (nNationalAnnex==dgn::def::enNationalAnnex::Singapore)
		{			
			GetDlgItem(IDC_DGN_STL_BDEFLECT32)->ShowWindow(FALSE);
			GetDlgItem(IDC_DGN_STL_BDEFLECT33)->ShowWindow(FALSE);
		}
		else if (m_iCode == IRC24_2010) {
			GetDlgItem(IDC_DGN_STL_BDEFLECT32)->ShowWindow(FALSE);
			GetDlgItem(IDC_DGN_STL_BDEFLECT33)->ShowWindow(FALSE);
	
		}
		else
		{			
			GetDlgItem(IDC_DGN_STL_BDEFLECT32)->ShowWindow(TRUE);
			GetDlgItem(IDC_DGN_STL_BDEFLECT33)->ShowWindow(TRUE);
		}
	}
	/*


	GetDlgItem(IDC_DGN_STL_DeflectFrm1)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_STL_BDEFLECT11)->EnableWindow(bEnable);
	// PMS:4130-Seungjun-20100205 Taiwan Steel Deflection checking (TWN-ASD96, LSD96).
	if(m_iCode==TWN_ASD96 || m_iCode==TWN_LSD96)
	{
	GetDlgItem(IDC_DGN_STL_BDEFLECT12)->ShowWindow(FALSE);
	GetDlgItem(IDC_DGN_STL_BDEFLECT13)->ShowWindow(FALSE);
	}
	else
	{
	GetDlgItem(IDC_DGN_STL_BDEFLECT12)->ShowWindow(TRUE);
	GetDlgItem(IDC_DGN_STL_BDEFLECT13)->ShowWindow(TRUE);
	GetDlgItem(IDC_DGN_STL_BDEFLECT12)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_STL_BDEFLECT13)->EnableWindow(bEnable);
	}

	GetDlgItem(IDC_DGN_STL_BDEFLECT14)->EnableWindow(bEnable);
	if(m_iHorizontal==3 && bEnable)
	GetDlgItem(IDC_DGN_STL_BDEFLECT15)->EnableWindow(TRUE);
	else
	GetDlgItem(IDC_DGN_STL_BDEFLECT15)->EnableWindow(FALSE);

	GetDlgItem(IDC_DGN_STL_DeflectFrm2)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_STL_BDEFLECT31)->EnableWindow(bEnable);
	// PMS:4130-Seungjun-20100205 Taiwan Steel Deflection checking (TWN-ASD96, LSD96).
	if(m_iCode==TWN_ASD96 || m_iCode==TWN_LSD96)
	{
	GetDlgItem(IDC_DGN_STL_BDEFLECT32)->ShowWindow(FALSE);
	GetDlgItem(IDC_DGN_STL_BDEFLECT33)->ShowWindow(FALSE);
	}
	else
	{
	GetDlgItem(IDC_DGN_STL_BDEFLECT32)->ShowWindow(TRUE);
	GetDlgItem(IDC_DGN_STL_BDEFLECT33)->ShowWindow(TRUE);
	GetDlgItem(IDC_DGN_STL_BDEFLECT32)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_STL_BDEFLECT33)->EnableWindow(bEnable);
	}
	GetDlgItem(IDC_DGN_STL_BDEFLECT34)->EnableWindow(bEnable);
	if(m_iVertical==3 && bEnable)
	GetDlgItem(IDC_DGN_STL_BDEFLECT35)->EnableWindow(TRUE);
	else
	GetDlgItem(IDC_DGN_STL_BDEFLECT35)->EnableWindow(FALSE);

	GetDlgItem(IDC_DGN_STL_DeflectFrm3)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_STL_DAF)->EnableWindow(bEnable);
	*/
	MoveCtrl();

	//GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(bEnable);
}

void CDgnStlServiceLimitDlg::MoveCtrl()
{
	CRect RectCng1, RectCng2;
	double dSpace1=0., dSpace2=0., dSpace3=0.;
	GetDlgItem(IDC_DGN_STL_BDEFLECT11)->GetWindowRect(RectCng1);
	GetDlgItem(IDC_DGN_STL_BDEFLECT12)->GetWindowRect(RectCng2);
	dSpace1 = RectCng2.top - RectCng1.bottom; 

	BOOL bShowOne = IsShowOneDeflLimit(m_iCode);
	if (bShowOne)
	{    
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

		GetDlgItem(IDC_DGN_STL_BDEFLECT31)->GetWindowRect(RectCng1);
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
		RectCng2.bottom -= dSpace2;
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
	else
	{
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

		int nNationalAnnex = CDBLib::GetStlSubCode();
        if ( nNationalAnnex==dgn::def::enNationalAnnex::Singapore )
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

		//GetDlgItem(IDC_DGN_EXECUTE)->GetWindowRect(RectCng2);
		//RectCng2.top -= dSpace3;
		//ScreenToClient(RectCng2);
		//GetDlgItem(IDC_DGN_EXECUTE)->SetWindowPos(NULL, RectCng2.left, RectCng2.top, RectCng2.Width(), RectCng2.Height(), SWP_NOSIZE | SWP_NOZORDER);
		//
		//GetDlgItem(IDC_DGN_CLOSE)->GetWindowRect(RectCng2);
		//RectCng2.top -= dSpace3;
		//ScreenToClient(RectCng2);
		//GetDlgItem(IDC_DGN_CLOSE)->SetWindowPos(NULL, RectCng2.left, RectCng2.top, RectCng2.Width(), RectCng2.Height(), SWP_NOSIZE | SWP_NOZORDER);

	}
}


BOOL CDgnStlServiceLimitDlg::IsShowOneDeflLimit(int nCode)
{
	BOOL bShowOne = FALSE;
	switch (m_iCode)
	{
	case TWN_ASD96:
	case TWN_LSD96:
	case AISC_LRFD05:
	case AISC_ASD05:
	case AISC_LRFD10:
	case AISC_ASD10:
	case AISC_LRFD16:
	case AISC_ASD16:
	case AISC_LRFD22:
	case AISC_ASD22:
	case KDS_41_31_2019:
	case KSSC_LSD16:
	case KSSC_LSD09:
	case IS800_2007:
		bShowOne = TRUE;
		break;
	default:
		break;
	}
	return bShowOne;
}

// 아이템들을 enable / disable 시킨다.
void CDgnStlServiceLimitDlg::Update_InitDataByCode()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	m_pDoc->GetDesignTitleStl(ID_DGN_STL_SERV, strTitle, bShow);



	switch (m_iCode)
	{
	case Eurocode3:
	case Eurocode3_05:
	case Eurocode3_2_05:
		{
			int nNationalAnnex = CDBLib::GetStlSubCode();
            if ( nNationalAnnex==dgn::def::enNationalAnnex::Singapore )
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
		break;
	case TWN_ASD96: // PMS:4130-Seungjun-20100205 Taiwan Steel Deflection checking (TWN-ASD96, LSD96).
	case TWN_LSD96:
	case AISC_LRFD05:
	case AISC_ASD05:
	case AISC_LRFD10:
	case AISC_ASD10:
	case AISC_LRFD16:
	case AISC_ASD16:
	case AISC_LRFD22:
	case AISC_ASD22:
		{
			GetDlgItem(IDC_DGN_STL_BDEFLECT11)->SetWindowText(_T("L / 360"));
			GetDlgItem(IDC_DGN_STL_BDEFLECT31)->SetWindowText(_T("h / 300"));
		}
		break;
	case KDS_41_31_2019:
	case KSSC_LSD16:
	case KSSC_LSD09:
		{
			GetDlgItem(IDC_DGN_STL_BDEFLECT11)->SetWindowText(_T("L / 300"));
			GetDlgItem(IDC_DGN_STL_BDEFLECT31)->SetWindowText(_T("h / 500"));
		}
		break;
	case IS800_2007:
	{
		GetDlgItem( IDC_DGN_STL_BDEFLECT11 )->SetWindowText( _T( "L / 360" ) );
		GetDlgItem( IDC_DGN_STL_BDEFLECT31 )->SetWindowText( _T( "h / 500" ) );
	}
	break;
	case IRC24_2010:
	{
		GetDlgItem(IDC_DGN_STL_BDEFLECT11)->SetWindowTextW(_T("L / 600"));
		GetDlgItem(IDC_DGN_STL_BDEFLECT12)->SetWindowTextW(_T("L / 800"));
		GetDlgItem(IDC_DGN_STL_BDEFLECT13)->SetWindowTextW(_T("L / 300"));
		GetDlgItem(IDC_DGN_STL_BDEFLECT31)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STL_BDEFLECT32)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STL_BDEFLECT33)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STL_BDEFLECT34)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STL_BDEFLECT35)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_STL_DeflectFrm2)->ShowWindow(SW_HIDE);
		
		
		GetDlgItem(IDC_DGN_STL_BDEFLECT17)->ShowWindow(SW_SHOW);
	}
	break;
	default:
		{
			GetDlgItem(IDC_DGN_STL_BDEFLECT11)->SetWindowText(_T("L / 400"));
			GetDlgItem(IDC_DGN_STL_BDEFLECT31)->SetWindowText(_T("h / 500"));
		}
		break;
	}

	if ( m_iCode == SP_16_13330_2017 )
	{
		GetDlgItem(IDC_DGN_STL_DeflectFrm1)->SetWindowText(_LS(IDS_DGN_STL_SERVICE_PARAM_ULTIMATE_DEFLECTION_BEAM));
		GetDlgItem(IDC_DGN_STL_DeflectFrm2)->SetWindowText(_LS(IDS_DGN_STL_SERVICE_PARAM_ULTIMATE_DEFLECTION_COLUMN));
	}
	else
	{
		GetDlgItem(IDC_DGN_STL_DeflectFrm1)->SetWindowText(m_strDeflectFrm1Init);
		GetDlgItem(IDC_DGN_STL_DeflectFrm2)->SetWindowText(m_strDeflectFrm2Init);
	}

	EnableItems(bShow);

	GetDlgItem(IDC_STATIC_SELECT_TYPE)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STL_SELECT_ALL)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STL_SELECT_SEL)->EnableWindow(bShow);

	GetDlgItem(IDC_STATIC_OPTION)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_STL_SERV_AR)->EnableWindow(bShow);    
	GetDlgItem(IDC_DGN_STL_SERV_DEL)->EnableWindow(bShow);    

	//GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(bShow);
}

void CDgnStlServiceLimitDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
void CDgnStlServiceLimitDlg::OnDgnConServAr() 
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
void CDgnStlServiceLimitDlg::OnDgnConServDel() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nOption  = 1;			//	Delete.
	EnableItems(FALSE);
	//GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(TRUE);
	UpdateData(FALSE);
}

// 선택된 element를 모두 unselect시킨다.
void CDgnStlServiceLimitDlg::Initial_SelectItem() 
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