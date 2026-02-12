// CMMvcdDlg.cpp : implementation file
//
// 2003. 09. 29    by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvcdDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"
#include "..\wg_db\wg_db_DBlib.h"
#include "..\wg_dbLock\LockMgr.h"   // for CLockMgr

#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_base\TestEnvMgr.h"

#include "..\wg_main\wg_main.h"
#include "..\wg_db\wg_db_InitCtrl.h"//T_PREFERENCE

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvcdDlg dialog


CCMMvcdDlg::CCMMvcdDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvcdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvcdDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	MakeCodeData();
}

CCMMvcdDlg::~CCMMvcdDlg()
{
}

void CCMMvcdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvcdDlg)
	DDX_Control(pDX, IDC_CMD_LCODE_COMBO, m_cobxCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMMvcdDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvcdDlg)
	ON_BN_CLICKED(IDC_CMD_MVCT_OK, OnCmdMvctOk)
	ON_BN_CLICKED(IDC_CMD_MVCT_CANCEL, OnCmdMvctCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvcdDlg message handlers

// *^^* 여기추가
BOOL CCMMvcdDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();
	// TODO: Add extra initialization here
	InitCodCombo();

	m_cobxCode.SelectString(-1, GetInitCodeName());    

	//combo box init 프리퍼런스에 맞춰서.
	T_PREFERENCE rPref;
	rPref.Initialize();
	m_pDoc->m_pInitCtrl->GetPreference(rPref);
	int nIndex = m_cobxCode.FindString(-1,rPref.LoadCode.MLCode);
	m_cobxCode.SetCurSel(nIndex);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CCMMvcdDlg::GetInitCodeName()
{
	int nCodeType = 0;
	nCodeType = GetInitCodeType();
	
	return GetCodeName(nCodeType);
}

int CCMMvcdDlg::GetInitCodeType() 
{
	int nCodeType = 0;

	T_MVCD_D data;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(pDoc == NULL) return nCodeType;
	
	if (pDoc->m_pAttrCtrl->ExistMvcd())
		pDoc->m_pAttrCtrl->GetMvcd(data);
	else
	{
		data.Initialize();
		int nMovigType = CProduct::GetMovingType();
#if defined(_MGEN)
		// 2014-10-27 sjhuh
		//Moving Load 관련 MQC 4951-18 수정.
		data.nCodeType = D_MOVE_CODE_NONE;
//     if(nMovigType == D_PRODUCT_MOVING_ORG || nMovigType == D_PRODUCT_MOVING_ALL)   // 국내     
//       data.nCodeType = D_MOVE_CODE_KOREA;
//     else if (nMovigType == D_PRODUCT_MOVING_US || nMovigType == D_PRODUCT_MOVING_RUS)   
//     {
//       if (CProduct::IsIndia()) // 인도       
//         data.nCodeType = D_MOVE_CODE_INDIA;
//       else if (CProduct::IsTaiwan())    // 대만
//         data.nCodeType = D_MOVE_CODE_TAIWAN;
//       else     // 미국 
//         data.nCodeType = D_MOVE_CODE_AASHTO_STAN;            
//     }
//     else
//       data.nCodeType = D_MOVE_CODE_NONE;    
#elif defined(_CIVIL)  
		// 20120111 KIMJM
		// 전국가 default 값 None, 
		// 리본메뉴가 들어가면서 리본메뉴에 보여지는 콤보리스트는 이미 들어있는 code값을 보여주기 때문에. 
		data.nCodeType = D_MOVE_CODE_NONE;
//     if (nMovigType == D_PRODUCT_MOVING_ORG || nMovigType == D_PRODUCT_MOVING_ALL)   // 국내 
//       data.nCodeType = D_MOVE_CODE_KOREA;
//     else if (nMovigType == D_PRODUCT_MOVING_US)  
//     {
//       if (CProduct::IsIndia()) // 인도 
//         data.nCodeType = D_MOVE_CODE_INDIA;
//       else if (CProduct::IsTaiwan())    // 대만
//         data.nCodeType = D_MOVE_CODE_TAIWAN;
//       else     // 미국 
//         data.nCodeType = D_MOVE_CODE_AASHTO_STAN;      
//     }
//     else if(nMovigType == D_PRODUCT_MOVING_RUS)
//       data.nCodeType == D_MOVE_CODE_RUSSIA;
//     else if (nMovigType == D_PRODUCT_MOVING_CH)   // 중국 
//       data.nCodeType = D_MOVE_CODE_CHINA;
//     else if (nMovigType == D_PRODUCT_MOVING_JP)   // 일본  : 2003.12.17 고영현 차장님 요청으로 모든 국가 가능하도록 수정
//       data.nCodeType = D_MOVE_CODE_NONE;    
#endif
	}

	nCodeType = data.nCodeType;
	return nCodeType;
}

void CCMMvcdDlg::OnCmdMvctOk() 
{
	// TODO: Add your control notification handler code here

	CString csCodeName;
	int ix = m_cobxCode.GetCurSel();
	if (ix == CB_ERR) return;
	m_cobxCode.GetLBText(ix, csCodeName);

	if(!AddMvcd(csCodeName)) return;

	I_GENModelBase::SetSurfLaneDataSet(FALSE);
	I_GENModelBase::SetLineLaneDataSet(FALSE);
	I_GENModelBase::SetSurfLaneopDataSet(FALSE);
	I_GENModelBase::SetLineLaneopDataSet(FALSE);

#if defined (_CIVIL) || defined (_MGEN)
	AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_LOAD_MOVING_LOAD, 0), 0);
#endif

	CDialogMove::OnOK();
}

BOOL CCMMvcdDlg::AddMvcd(CString strCodeName)
{
	T_MVCD_D data;
	data.nCodeType = GetCodeData(strCodeName);
	CDBDoc* pDoc = CDBDoc::GetDocPoint(); ASSERT(pDoc);
	return pDoc->m_pDataCtrl->AddMvcd(data);
}

void CCMMvcdDlg::OnCmdMvctCancel() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

void CCMMvcdDlg::InitCodCombo()
{
	m_cobxCode.ResetContent();

	for (int i=0; i<m_aCodeName.GetSize(); i++)
		m_cobxCode.AddString(m_aCodeName[i]);
	m_cobxCode.SetCurSel(0);
}

// *^^* 여기추가
int  CCMMvcdDlg::GetCodeData(CString csCodeName)
{
/*
	int nNum = m_aCodeName.GetSize();
	for (int i=0; i<nNum; i++)
		if (m_aCodeName[i] == csCodeName) return m_aCodeData[i];
	return D_MOVE_CODE_NONE;
*/
	int nCodeID = D_MOVE_CODE_NONE;

	if     (csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_None))==0)             nCodeID = D_MOVE_CODE_NONE;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_Korea))==0)            nCodeID = D_MOVE_CODE_KOREA;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_AASHTO_Strd))==0)      nCodeID = D_MOVE_CODE_AASHTO_STAN;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_AASHTO_LRFD))==0)      nCodeID = D_MOVE_CODE_AASHTO_LRFD;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_PENDOT))==0)           nCodeID = D_MOVE_CODE_PENDOT;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_China))==0)            nCodeID = D_MOVE_CODE_CHINA;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_Japan))==0)            nCodeID = D_MOVE_CODE_JAPAN;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_JapanGrid))==0)        nCodeID = D_MOVE_CODE_JP_GRID;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_JapanRail))==0)        nCodeID = D_MOVE_CODE_JAPAN_RAIL;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_India))==0)            nCodeID = D_MOVE_CODE_INDIA;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_Taiwan))==0)           nCodeID = D_MOVE_CODE_TAIWAN;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_Canada))==0)           nCodeID = D_MOVE_CODE_CANADA;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_BS))==0)               nCodeID = D_MOVE_CODE_BS;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_EURO_BS))==0)          nCodeID = D_MOVE_CODE_EURO_BS;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_RUSSIA))==0)           nCodeID = D_MOVE_CODE_RUSSIA;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_KOREA_LRFD_2011))==0)  nCodeID = D_MOVE_CODE_KOREA_LRFD_2011;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_AUSTRALIA))==0)        nCodeID = D_MOVE_CODE_AUSTRALIA;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_POLAND))==0)           nCodeID = D_MOVE_CODE_POLAND;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_SOUTH_AFRICA))==0)     nCodeID = D_MOVE_CODE_SOUTH_AFRICA;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_France))==0)           nCodeID = D_MOVE_CODE_FRANCE;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_NEWZEALAND))==0)       nCodeID = D_MOVE_CODE_NEWZEALAND;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_TRANS))==0)            nCodeID = D_MOVE_CODE_TRANS;
	else if(csCodeName.CompareNoCase(_LS(IDS_CMD_MVCD_BRAZIL))==0)           nCodeID = D_MOVE_CODE_BRAZIL;
	else ASSERT(0);

	return nCodeID;
}

CString CCMMvcdDlg::GetCodeName(int nCode)
{
	int nNum = m_aCodeData.GetSize();
	for (int i=0; i<nNum; i++)
		if (m_aCodeData[i] == nCode) return m_aCodeName[i];
	return _LS(IDS_CMD_MVCD_None);
}

// *^^* 여기추가
// [주의] 코드 추가, 변경시 꼭 MGT/MCT에 반영해야 합니다!!
void CCMMvcdDlg::MakeCodeData()
{
	m_aCodeName.RemoveAll();
	m_aCodeData.RemoveAll();

	CArray<int, int> aCode;
	CDBLib::GetMLCodeSeqList(aCode);

#define M_NAME_ID_ADD(NAME, ID)  m_aCodeName.Add(NAME); m_aCodeData.Add(ID)    

	int nSize = aCode.GetSize();
	for(int i=0; i<nSize; i++)
	{
		int     nCodeID = aCode.GetAt(i);
		CString strMsg(_T(""));
		BOOL bInclude = CLoadDgnCodeChecker::CheckCodeAndErrorMsgST(CLoadDgnCodeChecker::eLoadMoving,
																																nCodeID, strMsg);
		if(bInclude)
		{
			M_NAME_ID_ADD(GetMoveCodeName(nCodeID), nCodeID);
		}
	}

#undef  M_NAME_ID_ADD
}

// *^^* 여기추가
CString CCMMvcdDlg::GetMoveCodeName(int nCodeID)
{
	CString strCode(_T(""));
	if     (nCodeID==D_MOVE_CODE_NONE)              strCode = _LS(IDS_CMD_MVCD_None);
	else if(nCodeID==D_MOVE_CODE_KOREA)             strCode = _LS(IDS_CMD_MVCD_Korea);
	else if(nCodeID==D_MOVE_CODE_AASHTO_STAN)       strCode = _LS(IDS_CMD_MVCD_AASHTO_Strd);
	else if(nCodeID==D_MOVE_CODE_AASHTO_LRFD)       strCode = _LS(IDS_CMD_MVCD_AASHTO_LRFD);
	else if(nCodeID==D_MOVE_CODE_PENDOT)            strCode = _LS(IDS_CMD_MVCD_PENDOT);
	else if(nCodeID==D_MOVE_CODE_CHINA)             strCode = _LS(IDS_CMD_MVCD_China);
	else if(nCodeID==D_MOVE_CODE_JAPAN)             strCode = _LS(IDS_CMD_MVCD_Japan);
	else if(nCodeID==D_MOVE_CODE_JP_GRID)           strCode = _LS(IDS_CMD_MVCD_JapanGrid);
	else if(nCodeID==D_MOVE_CODE_JAPAN_RAIL)        strCode = _LS(IDS_CMD_MVCD_JapanRail);
	else if(nCodeID==D_MOVE_CODE_INDIA)             strCode = _LS(IDS_CMD_MVCD_India);
	else if(nCodeID==D_MOVE_CODE_TAIWAN)            strCode = _LS(IDS_CMD_MVCD_Taiwan);
	else if(nCodeID==D_MOVE_CODE_CANADA)            strCode = _LS(IDS_CMD_MVCD_Canada);
	else if(nCodeID==D_MOVE_CODE_BS)                strCode = _LS(IDS_CMD_MVCD_BS);
	else if(nCodeID==D_MOVE_CODE_EURO_BS)           strCode = _LS(IDS_CMD_MVCD_EURO_BS);
	else if(nCodeID==D_MOVE_CODE_RUSSIA)            strCode = _LS(IDS_CMD_MVCD_RUSSIA);
	else if(nCodeID==D_MOVE_CODE_KOREA_LRFD_2011)   strCode = _LS(IDS_CMD_MVCD_KOREA_LRFD_2011);
	else if(nCodeID==D_MOVE_CODE_AUSTRALIA)         strCode = _LS(IDS_CMD_MVCD_AUSTRALIA);
	else if(nCodeID==D_MOVE_CODE_POLAND)            strCode = _LS(IDS_CMD_MVCD_POLAND);
	else if(nCodeID==D_MOVE_CODE_SOUTH_AFRICA)      strCode = _LS(IDS_CMD_MVCD_SOUTH_AFRICA);
	else if(nCodeID==D_MOVE_CODE_FRANCE)            strCode = _LS(IDS_CMD_MVCD_France);
	else if(nCodeID==D_MOVE_CODE_TRANS)             strCode = _LS(IDS_CMD_MVCD_TRANS);
	else if(nCodeID==D_MOVE_CODE_NEWZEALAND)        strCode = _LS(IDS_CMD_MVCD_NEWZEALAND);
	else if(nCodeID==D_MOVE_CODE_BRAZIL)            strCode = _LS(IDS_CMD_MVCD_BRAZIL);
	else ASSERT(0);
	
	ASSERT(!strCode.IsEmpty());

	return strCode;
}

void CCMMvcdDlg::GetCodeNameList(CStringArray &aCodeName)
{
	aCodeName.RemoveAll();
	aCodeName.Copy(m_aCodeName);
}

void CCMMvcdDlg::GetCodeDataList(CArray<int, int> &aCodeData)
{
	aCodeData.RemoveAll();
	aCodeData.Copy(m_aCodeData);
}
