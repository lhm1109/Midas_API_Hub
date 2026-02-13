// DgnConWallDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConWallDataDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\wg_base_LocaleLib.h"

#include "DgnDataCtrl.h"
#include "DgnConBarSpaceDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConWallDataDlg dialog


CDgnConWallDataDlg::CDgnConWallDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConWallDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConWallDataDlg)
	m_nMethod = -1;
	m_bOutOfPlane = FALSE;
	m_Dist1 = 0.0;
	m_Dist2 = 0.0;
	m_Dist3 = 0.0;
	m_strWVSpace = _T("");
	m_WHSpace = 0.0;
	m_bSpaceLimit450 = FALSE;
	//}}AFX_DATA_INIT

	m_Dcrb.Initialize();

}


void CDgnConWallDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConWallDataDlg)
	DDX_Control(pDX, IDC_STATIC_WALLDIST3, m_Dist3Unit);
	DDX_Control(pDX, IDC_STATIC_WALLDIST2, m_Dist2Unit);
	DDX_Control(pDX, IDC_STATIC_WALLDIST1, m_Dist1Unit);
	DDX_Control(pDX, IDC_DGN_STATIC_HSPACE, m_HSpaceUnit);
	DDX_Radio(pDX, IDC_DGN_RC_WMETHOD1, m_nMethod);
	DDX_Check(pDX, IDC_DGN_RC_WOUTPLANE, m_bOutOfPlane);
	DDX_Text(pDX, IDC_DGN_RC_WENDSPACE1, m_Dist1);
	DDX_Text(pDX, IDC_DGN_RC_WENDSPACE2, m_Dist2);
	DDX_Text(pDX, IDC_DGN_RC_WENDSPACE3, m_Dist3);
	DDX_Text(pDX, IDC_DGN_RC_WVSPACE, m_strWVSpace);
	DDX_Text(pDX, IDC_DGN_RC_WHSPACE, m_WHSpace);
	DDX_Check(pDX, IDC_DGN_RC_450SPACELIMIT, m_bSpaceLimit450);
	DDX_Control(pDX, IDC_ENDTYPE1_BITMAP, m_wndWallEnd1);
	DDX_Control(pDX, IDC_ENDTYPE2_BITMAP, m_wndWallEnd2);
	DDX_Control(pDX, IDC_ENDTYPE3_BITMAP, m_wndWallEnd3);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnConWallDataDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnConWallDataDlg)
	ON_BN_CLICKED(IDC_DGN_RC_WMETHOD1, OnDgnRcWmethod1)
	ON_BN_CLICKED(IDC_DGN_RC_WMETHOD2, OnDgnRcWmethod2)
	ON_BN_CLICKED(IDC_DGN_RC_WMETHOD3, OnDgnRcWmethod3)
	ON_BN_CLICKED(IDC_DGN_RC_WSPACESEL, OnDgnRcWspacesel)
	ON_BN_CLICKED(IDC_DGN_RC_WMETHOD4, OnDgnRcWmethod4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConWallDataDlg message handlers


BOOL CDgnConWallDataDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	CString strDgnCode = CDBLib::GetConCodeName();
	if(strDgnCode==_T("KCI-USD99"))	GetDlgItem(IDC_DGN_RC_450SPACELIMIT)->ShowWindow(SW_SHOW);
	else												GetDlgItem(IDC_DGN_RC_450SPACELIMIT)->ShowWindow(SW_HIDE);

	m_wndWallEnd1.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_wall_end1.svg"));
	m_wndWallEnd2.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_wall_end2.svg"));
	m_wndWallEnd3.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_wall_end3.svg"));

	Initial_Units();
	/*
	if(m_pDoc->m_pAttrCtrl->ExistDcrb())	Initial_ExistData();
	else																	Initial_Data();
	*/
	Initial_ExistData();
	EnableCtrls();

	UpdateData(FALSE);
	// Add by ZINU.('01.7.7). Not used End Rebar if WSD.
	Enable_EndBarEdit();
	
	return TRUE;
}

void CDgnConWallDataDlg::SetDcrb(T_DCRB_D& rDcrb)
{
	m_Dcrb = rDcrb;
}

T_DCRB_D CDgnConWallDataDlg::GetDcrb()
{
	return m_Dcrb;
}

void CDgnConWallDataDlg::Initial_ExistData()
{
	double dEndDefaultSpace[3] = {0.0};
	CDBLib::GetDefaultEndRebarSpace(dEndDefaultSpace);
	if(m_Dcrb.dEndRebarSpace[0]==0.)	m_Dist1 = dEndDefaultSpace[0];
	else														  m_Dist1 = m_Dcrb.dEndRebarSpace[0];
	if(m_Dcrb.dEndRebarSpace[1]==0.)	m_Dist2 = dEndDefaultSpace[1];
	else														  m_Dist2 = m_Dcrb.dEndRebarSpace[1];
	if(m_Dcrb.dEndRebarSpace[2]==0.)	m_Dist3 = dEndDefaultSpace[2];
	else														  m_Dist3 = m_Dcrb.dEndRebarSpace[2];

	if(m_Dcrb.nEndRebarDesignMethod==0)	m_nMethod = 0;
	else																m_nMethod = m_Dcrb.nEndRebarDesignMethod-1;
	m_bOutOfPlane = m_Dcrb.bOutofPlaneBending;

	if(m_Dcrb.dSpacingHorizontalRebar==0.) m_WHSpace = dEndDefaultSpace[2]/2.;
	else																	  m_WHSpace = m_Dcrb.dSpacingHorizontalRebar;

	if(m_Dcrb.SpacingVerticalRebar[0]==_T(""))	CDBLib::GetDefaultWallRebarSpace(m_strWVSpace);
	else																	  m_strWVSpace = Adding_ReBarSpace(m_Dcrb.SpacingVerticalRebar);

	m_bSpaceLimit450 = m_Dcrb.bSpaceLimit450;
}

void CDgnConWallDataDlg::Initial_Units()
{
	m_Dist3Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Dist2Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Dist1Unit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_HSpaceUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	// Change by ZINU.('03.12.20). Always SHOW, Agree with Ko CJ.
/*
	#if defined(_US) || defined(_JP)
		GetDlgItem(IDC_DGN_RC_WOUTPLANE)->ShowWindow(SW_HIDE);
	#else
		GetDlgItem(IDC_DGN_RC_WOUTPLANE)->ShowWindow(SW_SHOW);
	#endif
*/
}

void CDgnConWallDataDlg::Initial_Data()
{
	// Method Type을 Method-1으로 초기화합니다.
	m_nMethod	= 0;          //  Method Type(Method-1=0, Method-2=1, Method-3=2, Method-4=3)
	m_bOutOfPlane = FALSE;  //  약축에 대한 강도검증을 수행하지 않는 것이 기본입니다.

	double dEndDefaultSpace[3] = {0.0};
	CDBLib::GetDefaultEndRebarSpace(dEndDefaultSpace);
	m_Dist1 = dEndDefaultSpace[0];
	m_Dist2 = dEndDefaultSpace[1];
	m_Dist3 = dEndDefaultSpace[2];

	CDBLib::GetDefaultWallRebarSpace(m_strWVSpace);
	m_WHSpace = dEndDefaultSpace[2]/2.;
	m_bSpaceLimit450 = FALSE;
}

void CDgnConWallDataDlg::OnOK() 
{
	UpdateData(TRUE);
	//  Error Check 후에 DB에 저장합니다.
	if(ErrorCheck())
	{
		m_Dcrb.dEndRebarSpace[0] = m_Dist1;
		m_Dcrb.dEndRebarSpace[1] = m_Dist2;
		m_Dcrb.dEndRebarSpace[2] = m_Dist3;
	  m_Dcrb.nEndRebarDesignMethod   = m_nMethod+1;
	  m_Dcrb.bOutofPlaneBending      = m_bOutOfPlane;
		// Wall의 Horizontal Rebar Spacing Data를 저장합니다.
		m_Dcrb.dSpacingHorizontalRebar = m_WHSpace;
		m_Dcrb.bSpaceLimit450 = m_bSpaceLimit450;

		// 읽어들인 Vertical Rebar Spacing Data를 잘게 쪼게 저장합니다.
		CArray<CString,CString> arStrSpace;
		Cutting_ReBarSize(m_strWVSpace, arStrSpace);
		int iCount = arStrSpace.GetSize();
		for(int i=0; i<iCount; i++)							m_Dcrb.SpacingVerticalRebar[i] = arStrSpace.GetAt(i);
		for(int i=iCount; i<CONST_DGN_iWALL_VSPACE; i++)	m_Dcrb.SpacingVerticalRebar[i] = _T("");
		// DB에 저장합니다.
	  CDialogMove::OnOK();
	}
}

BOOL CDgnConWallDataDlg::ErrorCheck()
{
	BOOL bCheck=TRUE;
	BOOL bZero=FALSE;

	if(m_Dist1 <= 0.)	bCheck = FALSE;
	if(m_Dist2 <= 0.)	bCheck = FALSE;
	if(m_Dist3 <= 0.)	bCheck = FALSE;

	if(m_nMethod < 0) bCheck = FALSE;

	if(m_WHSpace < 0.0) bCheck = FALSE;

	CArray<CString,CString> arStrSpace;
	Cutting_ReBarSize(m_strWVSpace, arStrSpace);
	for(int i=0; i<arStrSpace.GetSize(); i++)										
	{
		CString SpacingVerticalRebar = arStrSpace.GetAt(i);
		if(SpacingVerticalRebar==_T("")) {bZero=TRUE; continue;}
		else
		{
			int iTextCount=0;
			if(SpacingVerticalRebar.Right(1)==_T("\\"))  iTextCount = 2; // in.
			else                                     iTextCount = 1; // mm.
			int iCount = SpacingVerticalRebar.GetLength() - iTextCount;
			CString strSpace = SpacingVerticalRebar.Mid(1,iCount); // 0 is @.
			if(_ttoi(strSpace) <= 0) 
			{
				bCheck=FALSE;
				break;
			}
		}
		// Check if exists Zero between values.
		if(bZero) 
		{
			bCheck=FALSE;
			break;
		}
	}

	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_WALL_DATA),MB_OK);

	return bCheck;
}

void CDgnConWallDataDlg::OnDgnRcWmethod1() 
{
	UpdateData(TRUE);
	m_nMethod	= 0;      //  Method-1
	EnableCtrls();
	UpdateData(FALSE);
}

void CDgnConWallDataDlg::OnDgnRcWmethod2() 
{
	UpdateData(TRUE);
	m_nMethod	= 1;      //  Method-2
	EnableCtrls();
	UpdateData(FALSE);
}

void CDgnConWallDataDlg::OnDgnRcWmethod3() 
{
	UpdateData(TRUE);
	m_nMethod	= 2;      //  Method-3
	EnableCtrls();
	UpdateData(FALSE);
}

void CDgnConWallDataDlg::OnDgnRcWmethod4() 
{
	UpdateData(TRUE);
	m_nMethod	= 3;      //  Method-4
	EnableCtrls();
	UpdateData(FALSE);
}

void CDgnConWallDataDlg::OnDgnRcWspacesel() 
{
	UpdateData(TRUE);
	CDgnConBarSpaceDlg dlg;
	dlg.m_TotalVSpace = m_strWVSpace;
	if(dlg.DoModal() == IDOK)
	{
		m_strWVSpace = dlg.m_TotalVSpace;
		UpdateData(FALSE);
	}
}

void CDgnConWallDataDlg::Cutting_ReBarSize(CString strSize, CArray<CString,CString>& VRebarSpace)
{
	if(VRebarSpace.GetSize() > 0)	VRebarSpace.RemoveAll();

	CString temp=_T("");
	if(strSize != _T(""))
	{
		int nLen = strSize.GetLength();
		for(int i=0; i<nLen; i++)
		{
		  TCHAR ch = strSize.GetAt(i);
		  if(ch==',' || i+1 == nLen)
		  {
			  if(ch==',') VRebarSpace.Add(temp);
			  else			  
				{
					temp = temp+ch;
					VRebarSpace.Add(temp);
				}
			  temp=_T("");
		  }
		  else	temp = temp+ch;
		}
	}
}

CString CDgnConWallDataDlg::Adding_ReBarSpace(CString* strSpace)
{
	CString strTotal=_T(""), strData=_T("");
	for(int i=0; i<CONST_DGN_iWALL_VSPACE; i++)
	{
		CString strTemp = strSpace[i];
		if(strTemp!=_T(""))	strData = strData + strTemp + _T(",");
	}
	int iLen = strData.GetLength();
	if(iLen > 0)	strTotal = strData.Left(iLen-1);

	return strTotal;
}

// Add by ZINU.('01.7.7). Not used End Rebar if WSD.
void CDgnConWallDataDlg::Enable_EndBarEdit()
{
	// Change by ZINU.('01.9.14). Although WSD, Let's usd All Methods.
/*/
	CDgnDataCtrl DataCtrl;
	T_DCON_D rData;
	rData.Initialize();
	DataCtrl.Get_DgnConDcon(rData);
	CString strCode = rData.DesignCode;
	if(strCode==")AIK-WSD2K_T(" || strCode==")AIJ-WSD99")	// WSD.
	{
		GetDlgItem(IDC_DGN_RC_WENDSPACE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RC_WENDSPACE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RC_WENDSPACE3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RC_WMETHOD2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RC_WMETHOD3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RC_WMETHOD4)->EnableWindow(FALSE);
	}
	else	// USD, LSD.
	{
		GetDlgItem(IDC_DGN_RC_WENDSPACE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_RC_WENDSPACE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_RC_WENDSPACE3)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_RC_WMETHOD2)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_RC_WMETHOD3)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_RC_WMETHOD4)->EnableWindow(TRUE);
	}
*/
}

void CDgnConWallDataDlg::EnableCtrls()
{
	const BOOL bEnableEndSpace = [](int nMethod)
	{
		CDgnDataCtrl DataCtrl;
		T_DCON_D DconD;
		DataCtrl.Get_DgnConDcon(DconD);

		if (DconD.DesignCode == CONCODE_IS456_2000) { return TRUE; }
		return (nMethod == 0) ? FALSE : TRUE;
	}(m_nMethod);
		
	GetDlgItem(IDC_DGN_RC_WENDSPACE1)->EnableWindow(bEnableEndSpace);
	GetDlgItem(IDC_DGN_RC_WENDSPACE2)->EnableWindow(bEnableEndSpace);
	GetDlgItem(IDC_DGN_RC_WENDSPACE3)->EnableWindow(bEnableEndSpace);	
}