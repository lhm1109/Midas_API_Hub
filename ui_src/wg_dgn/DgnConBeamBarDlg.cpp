// DgnConBeamBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBeamBarDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamBarDlg dialog


CDgnConBeamBarDlg::CDgnConBeamBarDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConBeamBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBeamBarDlg)
	m_BOneArray = -1;
	m_TOneArray = -1;	
	//}}AFX_DATA_INIT
	m_iMode = 0;
	m_bInit = false;
}


void CDgnConBeamBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConBeamBarDlg)
	DDX_Control(pDX, IDC_DGN_RC_STIRRUPUNIT, m_StirrupUnit);
	DDX_Control(pDX, IDC_DGN_RC_BSTIRRUP, m_StirrupCtrl);
	DDX_Control(pDX, IDC_DGN_RC_BSTIRRUP_NUM, m_StirrupNumCtrl);
	DDX_Control(pDX, IDC_DGN_RC_NTSIZE, m_NtSize);
	DDX_Control(pDX, IDC_DGN_RC_NBCOMBO, m_NbSize);
	DDX_Radio(pDX, IDC_DGN_BEAMB_ONEARRAY, m_BOneArray);
	DDX_Radio(pDX, IDC_DGN_BEAMT_ONEARRAY, m_TOneArray);
	DDX_Control(pDX, IDC_DGN_RC_NT, m_edtNt);
	DDX_Control(pDX, IDC_DGN_RC_NB, m_edtNb);
	DDX_Control(pDX, IDC_DGN_RC_NB2, m_edtNb2);
	DDX_Control(pDX, IDC_DGN_RC_NT2, m_edtNt2);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnConBeamBarDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnConBeamBarDlg)
	ON_BN_CLICKED(IDC_DGN_BEAMT_ONEARRAY, OnDgnBeamtOnearray)
	ON_BN_CLICKED(IDC_DGN_BEAMT_TWOARRAY, OnDgnBeamtTwoarray)
	ON_BN_CLICKED(IDC_DGN_BEAMB_ONEARRAY, OnDgnBeambOnearray)
	ON_BN_CLICKED(IDC_DGN_BEAMB_TWOARRAY, OnDgnBeambTwoarray)	
	ON_EN_CHANGE(IDC_DGN_RC_NB, OnChangeDgnRcNb)
	ON_EN_CHANGE(IDC_DGN_RC_NB2, OnChangeDgnRcNb2)
	ON_EN_CHANGE(IDC_DGN_RC_NT, OnChangeDgnRcNt)
	ON_EN_CHANGE(IDC_DGN_RC_NT2, OnChangeDgnRcNt2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBeamBarDlg message handlers

void CDgnConBeamBarDlg::OnDgnBeamtOnearray() 
{
	UpdateData(TRUE);

	m_TOneArray = 0;
	// 이중갯수를 입력받기 위한 Edit&Spin을 Hide시키자.
	GetDlgItem(IDC_DGN_RC_NT2)->ShowWindow(SW_HIDE);
	Draw_Beam();

	UpdateData(FALSE);
}

void CDgnConBeamBarDlg::OnDgnBeamtTwoarray() 
{
	UpdateData(TRUE);

	m_TOneArray = 1;
	// 이중갯수를 입력받기 위한 Edit&Spin을 Show시키자.
	GetDlgItem(IDC_DGN_RC_NT2)->ShowWindow(SW_SHOW);
	Draw_Beam();

	UpdateData(FALSE);
}

void CDgnConBeamBarDlg::OnDgnBeambOnearray() 
{
	UpdateData(TRUE);

	m_BOneArray = 0;
	// 이중갯수를 입력받기 위한 Edit&Spin을 Hide시키자.
	GetDlgItem(IDC_DGN_RC_NB2)->ShowWindow(SW_HIDE);
	Draw_Beam();

	UpdateData(FALSE);
}

void CDgnConBeamBarDlg::OnDgnBeambTwoarray() 
{
	UpdateData(TRUE);

	m_BOneArray = 1;
	// 이중갯수를 입력받기 위한 Edit&Spin을 Hide시키자.
	GetDlgItem(IDC_DGN_RC_NB2)->ShowWindow(SW_SHOW);
	Draw_Beam();

	UpdateData(FALSE);
}

BOOL CDgnConBeamBarDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	if(m_NtSize.GetCount() > 0)	m_NtSize.ResetContent();
	if(m_NbSize.GetCount() > 0)	m_NbSize.ResetContent();
	if(m_iMode == 0) //RC-Beam
	{
		m_SectWnd.m_SectionType = eDGN_BEAM;
		CDBLib::GetRebarNameAtComboBox(&m_NtSize);
		CDBLib::GetRebarNameAtComboBox(&m_NbSize);
	}
	else if(m_iMode == 1) //SRC-Beam
	{
		m_SectWnd.m_SectionType = eSRC_BEAM;
		CDBLib::GetRebarNameAtComboBox(&m_NtSize, FALSE);
		CDBLib::GetRebarNameAtComboBox(&m_NbSize, FALSE);
	}
	else ASSERT(0);

	if(m_StirrupNumCtrl.GetCount() > 0)	m_StirrupNumCtrl.ResetContent();
	CStringArray arStirrupNum;
	CDBLib::GetSubRebarNumList(arStirrupNum, TRUE);
	for(int i=0; i<arStirrupNum.GetSize(); i++)	m_StirrupNumCtrl.AddString(arStirrupNum[i]);

	// Initialize Window.
	CWnd* PlaceHolder0 = GetDlgItem(IDC_STATIC_VIEW);
	ASSERT(PlaceHolder0);
	m_SectWnd.Initial(PlaceHolder0);

	// Initialize Units.
	Initial_UnitData();
	// Initialize Section Data.
	Initial_SectionData();
	// Initialize General Data.
	Initial_Data();
	
	UpdateData(FALSE);

	m_bInit = true;

	Draw_Beam();

	return TRUE;
}

void CDgnConBeamBarDlg::Initial_SectionData()
{
	T_SECT_D rData;
	rData.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetSectDesign(m_RchkKey,rData))	return;

	const T_SECT_SECTBASE_D& SectBaseI = rData.SectBefore.SectI;
	if(m_iMode == 0) //RC-Beam
	{
		if(m_BeamShape==_T("SB"))
		{
			m_Hc = SectBaseI.Size[0];
			m_Bc = SectBaseI.Size[1];
		}
		else if(m_BeamShape==_T("T"))
		{
			m_Hc = SectBaseI.Size[0];
			m_Bf = SectBaseI.Size[1];
			m_Bc = SectBaseI.Size[2];
			m_Hf = SectBaseI.Size[3];
		}
	}
	else if(m_iMode == 1) //SRC-Beam
	{
		if(m_BeamShape==_T("RHB"))
		{
			m_H = SectBaseI.Size[0];
			m_B1 = SectBaseI.Size[1];
			m_Tw = SectBaseI.Size[2];
			m_Tf = SectBaseI.Size[3];
			m_B2 = SectBaseI.Size[4];
			if(m_B2==0.0) m_B2 = m_B1;

			if(SectBaseI.Size[5]==0.)	m_Tf2 = SectBaseI.Size[3];
			else						m_Tf2 = SectBaseI.Size[5];

			m_Hc = rData.SectBefore.SectJ.Size[0];
			m_Bc = rData.SectBefore.SectJ.Size[1];
		}
		else ASSERT(0);
	}
	else ASSERT(0);

	if(m_DbNo==0)	m_DbNo = Get_SectionDimension();
	if(m_DtNo==0)	m_DtNo = Get_SectionDimension();
}

double CDgnConBeamBarDlg::Get_SectionDimension()
{
	T_SECT_D rData;
	rData.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetSectDesign(m_RchkKey,rData))	return 1.0;

	double H = 0.0;
	if(m_iMode == 0) //RC-Beam
		H = rData.SectBefore.SectI.Size[0];
	else if(m_iMode == 1)
		H = rData.SectBefore.SectJ.Size[0];
	else ASSERT(0);

	double maxN = H/10.;

	double Factor=Get_ChangeUnitFactor();
	double ComN=6.35*Factor;

	maxN = max(maxN,ComN);
	double Max = 7.62*Factor;
	if(maxN >= Max) maxN = Max;

	return maxN;
}

double CDgnConBeamBarDlg::Get_ChangeUnitFactor()
{
	//  기준 = tonf/cm^2
	T_UNIT_INDEX CurrentUnitIndex;
	//	우선 Current Unit Index를 얻어오자 !!!
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrentUnitIndex);

	double LenFactor=0.0;
	if(CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM)		LenFactor = 10;
	else if(CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM)	LenFactor = 1.;
	else if(CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)	LenFactor = 0.01;
	else if(CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN)	LenFactor = 1./2.54;
	else if(CurrentUnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)	LenFactor = 1./30.48;

	return LenFactor;
}

void CDgnConBeamBarDlg::Initial_UnitData()
{
	//  Spin Button을 초기화합니다.
	m_edtNt.SetInteger(TRUE);
	m_edtNt.SetRange(1, 100);
	m_edtNb.SetInteger(TRUE);
	m_edtNb.SetRange(1, 100);
	m_edtNt2.SetInteger(TRUE);
	m_edtNt2.SetRange(1, 100);
	m_edtNb2.SetInteger(TRUE);
	m_edtNb2.SetRange(1, 100);
	
	m_StirrupUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_StirrupCtrl.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CDgnConBeamBarDlg::Initial_Data()
{
	CString strNtSize=_T(""),strNbSize=_T("");
	/////////////////////////////////////////////////////////////
	if(m_sNt != _T(""))		Get_RebarSizeNumByString(m_sNt,m_Nt,strNtSize);
	else
	{
		m_Nt = 2;		
		if(m_iMode == 0)	CDBLib::GetDefaultMainRebarSize(1, strNtSize);
		else if(m_iMode==1) CDBLib::GetDefaultSrcRebarSize(strNtSize);
		else ASSERT(0);
	}
	int Index = m_NtSize.FindStringExact(-1, strNtSize);
	m_NtSize.SetCurSel(Index);

	if(m_sNt2 != _T(""))  Get_RebarSizeNumByString(m_sNt2,m_Nt2,strNtSize);
	else
	{
		m_Nt2 = 2;		
		if(m_iMode == 0)	CDBLib::GetDefaultMainRebarSize(1, strNtSize);
		else if(m_iMode==1) CDBLib::GetDefaultSrcRebarSize(strNtSize);
		else ASSERT(0);
	}

	if(m_sNt != _T("") && m_sNt2 != _T(""))
	{
		m_TOneArray = 1;
		//  이중갯수를 입력받기 위한 Edit&Spin을 Hide시키자.
		GetDlgItem(IDC_DGN_RC_NT2)->ShowWindow(SW_SHOW);
	}
	else
	{
		m_TOneArray = 0;
		//  이중갯수를 입력받기 위한 Edit&Spin을 Hide시키자.
		GetDlgItem(IDC_DGN_RC_NT2)->ShowWindow(SW_HIDE);
	}
	
	/////////////////////////////////////////////////////////////
	if(m_sNb != _T(""))  Get_RebarSizeNumByString(m_sNb,m_Nb,strNbSize);
	else
	{
		m_Nb = 2;		
		if(m_iMode == 0)	CDBLib::GetDefaultMainRebarSize(1, strNbSize);
		else if(m_iMode==1) CDBLib::GetDefaultSrcRebarSize(strNbSize);
		else ASSERT(0);
	}
	Index = m_NbSize.FindStringExact(-1,strNbSize);
	m_NbSize.SetCurSel(Index);

	if(m_sNb2 != _T(""))  Get_RebarSizeNumByString(m_sNb2,m_Nb2,strNbSize);
	else
	{
		m_Nb2 = 2;		
		if(m_iMode == 0)	CDBLib::GetDefaultMainRebarSize(1, strNbSize);
		else if(m_iMode==1) CDBLib::GetDefaultSrcRebarSize(strNbSize);
		else ASSERT(0);
	}
	if(m_sNb != _T("") && m_sNb2 != _T(""))  
	{
		m_BOneArray = 1;
		//  이중갯수를 입력받기 위한 Edit&Spin을 Hide시키자.
		GetDlgItem(IDC_DGN_RC_NB2)->ShowWindow(SW_SHOW);
	}
	else
	{
		m_BOneArray = 0;
		//  이중갯수를 입력받기 위한 Edit&Spin을 Hide시키자.
		GetDlgItem(IDC_DGN_RC_NB2)->ShowWindow(SW_HIDE);		
	}

	m_edtNt.SetValue(m_Nt);
	m_edtNt2.SetValue(m_Nt2);
	m_edtNb.SetValue(m_Nb);
	m_edtNb2.SetValue(m_Nb2);

	m_StirrupCtrl.SetEditUnit(m_Space);
	CString strStirrupNum=_T("");
	strStirrupNum.Format(_T("%d"), m_iStirrupNum);
	int iIndex = m_StirrupNumCtrl.FindStringExact(-1, strStirrupNum);
	m_StirrupNumCtrl.SetCurSel(iIndex<0 ? 0 : iIndex);
	/////////////////////////////////////////////////////////////
	if(m_iMode == 0) //RC-Beam
	{
		if(m_BeamShape==_T("SB"))
		{
			m_SectWnd.Beam_DrawData(eDGN_RECT,m_Hc,m_Bc,m_Hf,m_Bf,m_TOneArray,m_BOneArray,
															m_Nb,m_Nb2,m_Nt,m_Nt2,m_DtNo,m_DbNo,0);
		}
		else if(m_BeamShape==_T("T"))
		{
			m_SectWnd.Beam_DrawData(eDGN_TEE,m_Hc,m_Bc,m_Hf,m_Bf,m_TOneArray,m_BOneArray,
															m_Nb,m_Nb2,m_Nt,m_Nt2,m_DtNo,m_DbNo,0);
		}
	}
	else if(m_iMode == 1) //SRC-Beam
	{
		if(m_BeamShape==_T("RHB"))
		{
			m_SectWnd.SRCBeam_DrawData(eSRC_RHB,m_H,m_B1,m_B2,m_Tw,m_Tf,m_Tf2,m_Hc,m_Bc,m_TOneArray,m_BOneArray,
															m_Nb,m_Nb2,m_Nt,m_Nt2,m_DtNo,m_DbNo);
		}
	}
	else ASSERT(0);
	m_SectWnd.Invalidate(TRUE);
}

void CDgnConBeamBarDlg::OnOK() 
{
	UpdateData(TRUE);

	BOOL bCheck=TRUE;
	CString strNtSize=_T(""), strNbSize=_T("");
	int Index = m_NtSize.GetCurSel();
	if(Index != -1) m_NtSize.GetLBText(Index,strNtSize);
	else            bCheck = FALSE;

	Index = m_NbSize.GetCurSel();
	if(Index != -1) m_NbSize.GetLBText(Index,strNbSize);
	else            bCheck = FALSE;

	if(dgn::LE0(m_DtNo))  bCheck = FALSE;
	if(dgn::LE0(m_DbNo))  bCheck = FALSE;
	m_Space = m_StirrupCtrl.GetEditValue();
	if(dgn::LE0(m_Space))  bCheck = FALSE;

	CString strStirrupNum=_T("");
	Index = m_StirrupNumCtrl.GetCurSel();
	if(Index != -1) m_StirrupNumCtrl.GetLBText(Index, strStirrupNum);
	else            bCheck = FALSE;
	m_iStirrupNum = _ttoi(strStirrupNum);
	if(m_iStirrupNum < 2)	bCheck = FALSE;

	m_Nt = m_edtNt.GetEditValueInt();
	m_Nb = m_edtNb.GetEditValueInt();
	m_Nt2 = m_edtNt2.GetEditValueInt();
	m_Nb2 = m_edtNb2.GetEditValueInt();

	if(bCheck)
	{
		if(m_TOneArray == 0)  //  Top One Array
		{
			m_sNt.Format(_T("%d-%s"),m_Nt,strNtSize);
			m_sNt2 = _T("");
		}
		else                  //  Top Two Array
		{
			m_sNt.Format(_T("%d-%s"),m_Nt,strNtSize);
			m_sNt2.Format(_T("%d-%s"),m_Nt2,strNtSize);
		}

		if(m_BOneArray == 0)  //  Bottom One Array
		{
			m_sNb.Format(_T("%d-%s"),m_Nb,strNbSize);
			m_sNb2 = _T("");
		}
		else                  //  Bottom Two Array
		{
			m_sNb.Format(_T("%d-%s"),m_Nb,strNbSize);
			m_sNb2.Format(_T("%d-%s"),m_Nb2,strNbSize);
		}
	  CDialogMove::OnOK();
	}
	else  AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_SIZE_SPACE),MB_OK);
}

void CDgnConBeamBarDlg::OnChangeDgnRcNt()
{
	Draw_Beam();
}

void CDgnConBeamBarDlg::OnChangeDgnRcNt2()
{
	Draw_Beam();
}

void CDgnConBeamBarDlg::OnChangeDgnRcNb()
{
	Draw_Beam();
}

void CDgnConBeamBarDlg::OnChangeDgnRcNb2()
{
	Draw_Beam();
}

void CDgnConBeamBarDlg::Get_RebarSizeNumByString(CString strTemp, int& iNum, CString& RebarNa)
{
	int iCutNo=0;
	int iCount = strTemp.GetLength();
	if(iCount > 0)
	{
		for(int i=0; i<iCount; i++)
		{
			TCHAR ch = strTemp.GetAt(i);
			if(ch=='-')	iCutNo = i;
		}
		iNum		= _ttoi(strTemp.Left(iCutNo));
		RebarNa	= strTemp.Mid(iCutNo+1);
	}
	else
	{
		iNum		= 0;
		RebarNa	= _T("");
	}
}

void CDgnConBeamBarDlg::Draw_Beam()
{
	if (!m_bInit) return;

	UpdateData(TRUE);
	BOOL bCheck = TRUE;

	m_Nt   = m_edtNt.GetEditValue();
	m_Nt2  = m_edtNt2.GetEditValue(); 

	m_Nb  = m_edtNb.GetEditValue();
	m_Nb2 = m_edtNb2.GetEditValue();

	if(m_Nt <= 0) 
	{
		m_Nt = 2;		
		m_edtNt.SetValue(m_Nt);		
		bCheck = FALSE;
	}
	if(m_Nb <= 0)
	{
		m_Nb = 2;
		m_edtNb.SetValue(m_Nb);
		bCheck = FALSE;
	}

	if(!bCheck) AfxMessageBox(_LS(IDS_DGN_CHK_REBAR_NUM),MB_OK);

	if(m_iMode == 0) // RC-Beam
	{
		if(m_BeamShape ==_T("SB"))
		{
			m_SectWnd.Beam_DrawData(eDGN_RECT,m_Hc,m_Bc,m_Hf,m_Bf,m_TOneArray,m_BOneArray,
															m_Nb,m_Nb2,m_Nt,m_Nt2,m_DtNo,m_DbNo,0);
		}
		else if(m_BeamShape ==_T("T"))
		{
			m_SectWnd.Beam_DrawData(eDGN_TEE,m_Hc,m_Bc,m_Hf,m_Bf,m_TOneArray,m_BOneArray,
															m_Nb,m_Nb2,m_Nt,m_Nt2,m_DtNo,m_DbNo,0);
		}
	}
	else if(m_iMode == 1) // SRC-Beam
	{
		if(m_BeamShape ==_T("RHB"))
		{
			m_SectWnd.SRCBeam_DrawData(eSRC_RHB,m_H,m_B1,m_B2,m_Tw,m_Tf,m_Tf2,m_Hc,m_Bc,m_TOneArray,m_BOneArray,
																m_Nb,m_Nb2,m_Nt,m_Nt2,m_DtNo,m_DbNo);
		}
		else ASSERT(0);
	}
	else ASSERT(0);

	m_SectWnd.Invalidate(TRUE);
}
