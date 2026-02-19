// DgnRCDesignParamParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRCDesignParamParamDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\DlgUtil.h"

#include "DgnRCDesignParamBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DgnRCDesignParamParamDlg dialog
#define IDRDOPT(n) m_aRoad_OptID.Add(IDC_RC_DESIGN_PARAMETER##n)
#define IDRDOUT(n) m_aRoad_OutID.Add(IDC_RC_PRINT_PARAMETER##n)
#define IDRLOPT(n) m_aRail_OptID.Add(IDC_RAIL_DESIGN_PARAMETER##n)
#define IDRLOUT(n) m_aRail_OutID.Add(IDC_RAIL_PRINT_PARAMETER##n)
#define IDSEOPT(n) m_aSeis_OptID.Add(IDC_SEIS_DESIGN_PARAMETER##n)
#define IDSEOUT(n) m_aSeis_OutID.Add(IDC_SEIS_PRINT_PARAMETER##n)
#define IDCJJSEOPT(n) m_aCjjSeis_OptID.Add(IDC_CJJ166_SEIS_DESIGN_PARAMETER##n)
#define IDCJJSEOUT(n) m_aCjjSeis_OutID.Add(IDC_CJJ166_SEIS_PRINT_PARAMETER##n)


DgnRCDesignParamParamDlg::DgnRCDesignParamParamDlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(DgnRCDesignParamParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(DgnRCDesignParamParamDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iDimType = -1;
	m_iSafeType = -1;
	m_iCrackCode = -1;
	m_iCrackType = -1;
	for (int i=0; i<iIDPRINT; i++) {
		m_bDesignPrint[i] = FALSE;
	}

	m_iRailCrackCode = -1;
	for (int i=0; i<iRAILIDPRINT; i++) {
		m_bRailDesignPrint[i] = FALSE;
	}
	m_bSeismic = FALSE;
	
	ASSERT(pParent);
	m_pParent = (DgnRCDesignParamBaseDlg*)pParent;  
}


void DgnRCDesignParamParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DgnRCDesignParamParamDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_RC_DESIGN_CODE, m_DesignCode);
	DDX_Radio(pDX, IDC_RC_DESIGN_PARAMETER1, m_iDimType);
	DDX_Radio(pDX, IDC_RC_DESIGN_PARAMETER4, m_iSafeType);
	DDX_Radio(pDX, IDC_RC_DESIGN_PARAMETER7, m_iCrackCode);
	DDX_Radio(pDX, IDC_RC_DESIGN_PARAMETER9, m_iCrackType);
	DDX_Check(pDX, IDC_RC_PRINT_PARAMETER1, m_bDesignPrint[0]);
	DDX_Check(pDX, IDC_RC_PRINT_PARAMETER2, m_bDesignPrint[1]);
	DDX_Check(pDX, IDC_RC_PRINT_PARAMETER3, m_bDesignPrint[2]);
	DDX_Check(pDX, IDC_RC_PRINT_PARAMETER4, m_bDesignPrint[3]);
	DDX_Check(pDX, IDC_RC_PRINT_PARAMETER5, m_bDesignPrint[4]);
	DDX_Check(pDX, IDC_RC_PRINT_PARAMETER6, m_bDesignPrint[5]);
	DDX_Check(pDX, IDC_RC_PRINT_PARAMETER7, m_bDesignPrint[6]);
	DDX_Check(pDX, IDC_RC_PRINT_PARAMETER8, m_bDesignPrint[7]);
	DDX_Check(pDX, IDC_RC_PRINT_PARAMETER9, m_bDesignPrint[8]);
	DDX_Check(pDX, IDC_RC_PRINT_PARAMETER10, m_bDesignPrint[9]);
	DDX_Check(pDX, IDC_RC_PRINT_PARAMETER11, m_bDesignPrint[10]);
	DDX_Check(pDX, IDC_RC_PRINT_PARAMETER12, m_bDesignPrint[11]);


	DDX_Radio(pDX, IDC_RAIL_DESIGN_PARAMETER2, m_iRailCrackCode);
	DDX_Check(pDX, IDC_RAIL_PRINT_PARAMETER3, m_bRailDesignPrint[0]);
	DDX_Check(pDX, IDC_RAIL_PRINT_PARAMETER4, m_bRailDesignPrint[1]);
	DDX_Check(pDX, IDC_RAIL_PRINT_PARAMETER5, m_bRailDesignPrint[2]);
	DDX_Check(pDX, IDC_RAIL_PRINT_PARAMETER6, m_bRailDesignPrint[3]);
	DDX_Check(pDX, IDC_RAIL_PRINT_PARAMETER7, m_bRailDesignPrint[4]);
	DDX_Check(pDX, IDC_RAIL_PRINT_PARAMETER8, m_bRailDesignPrint[5]);
	DDX_Check(pDX, IDC_RAIL_PRINT_PARAMETER9, m_bRailDesignPrint[6]);
	DDX_Check(pDX, IDC_RAIL_PRINT_PARAMETER10, m_bRailDesignPrint[7]);
	DDX_Check(pDX, IDC_RAIL_PRINT_PARAMETER11, m_bRailDesignPrint[8]);
	DDX_Check(pDX, IDC_RAIL_PRINT_PARAMETER12, m_bRailDesignPrint[9]);
	DDX_Check(pDX, IDC_RAIL_PRINT_PARAMETER13, m_bRailDesignPrint[10]);
	DDX_Check(pDX, IDC_RAIL_PRINT_PARAMETER14, m_bRailDesignPrint[11]);
	DDX_Check(pDX, IDC_RAIL_PRINT_PARAMETER17, m_bSeismic);

	
	DDX_Radio(pDX, IDC_SEIS_DESIGN_PARAMETER1, m_iEarthquakeType);
	DDX_Radio(pDX, IDC_SEIS_DESIGN_PARAMETER5, m_iBridgeType);
	DDX_Control(pDX, IDC_SEIS_DESIGN_PARAMETER1, m_RadioEarthE1);
	DDX_Control(pDX, IDC_SEIS_DESIGN_PARAMETER2, m_RadioEarthE2_E);
	DDX_Control(pDX, IDC_SEIS_DESIGN_PARAMETER3, m_RadioEarthE2_EP);
	DDX_Control(pDX, IDC_SEIS_DESIGN_PARAMETER5, m_RadioBridgeType_BC);
	DDX_Control(pDX, IDC_SEIS_DESIGN_PARAMETER6, m_RadioBridgeType_D);	
	DDX_Control(pDX, IDC_SEIS_PRINT_PARAMETER11, m_ChkCopingDis);
	DDX_Control(pDX, IDC_SEIS_PRINT_PARAMETER12, m_ChkPlasticTorsion);
	DDX_Check(pDX, IDC_SEIS_PRINT_PARAMETER1,  m_bSeisDesignPrint[0]);
	DDX_Check(pDX, IDC_SEIS_PRINT_PARAMETER2,  m_bSeisDesignPrint[1]);
	DDX_Check(pDX, IDC_SEIS_PRINT_PARAMETER3,  m_bSeisDesignPrint[2]);
	DDX_Check(pDX, IDC_SEIS_PRINT_PARAMETER4,  m_bSeisDesignPrint[3]);
	DDX_Check(pDX, IDC_SEIS_PRINT_PARAMETER5,  m_bSeisDesignPrint[4]);
	DDX_Check(pDX, IDC_SEIS_PRINT_PARAMETER6,  m_bSeisDesignPrint[5]);
	DDX_Check(pDX, IDC_SEIS_PRINT_PARAMETER7,  m_bSeisDesignPrint[6]);
	DDX_Check(pDX, IDC_SEIS_PRINT_PARAMETER8,  m_bSeisDesignPrint[7]);
	DDX_Check(pDX, IDC_SEIS_PRINT_PARAMETER9,  m_bSeisDesignPrint[8]);
	DDX_Check(pDX, IDC_SEIS_PRINT_PARAMETER10, m_bSeisDesignPrint[9]);
	DDX_Check(pDX, IDC_SEIS_PRINT_PARAMETER11, m_bSeisDesignPrint[10]);
	DDX_Check(pDX, IDC_SEIS_PRINT_PARAMETER12, m_bSeisDesignPrint[11]);

	DDX_Radio(pDX, IDC_CJJ166_SEIS_DESIGN_PARAMETER1, m_iCjjEarthquakeType);
	DDX_Radio(pDX, IDC_CJJ166_SEIS_DESIGN_PARAMETER5, m_iCjjBridgeType);
	DDX_Control(pDX, IDC_CJJ166_SEIS_DESIGN_PARAMETER1, m_RadioCjjEarthE1);
	DDX_Control(pDX, IDC_CJJ166_SEIS_DESIGN_PARAMETER2, m_RadioCjjEarthE2_E);
	DDX_Control(pDX, IDC_CJJ166_SEIS_DESIGN_PARAMETER3, m_RadioCjjEarthE2_EP);
	DDX_Control(pDX, IDC_CJJ166_SEIS_DESIGN_PARAMETER5, m_RadioCjjBridgeType_A);
	DDX_Control(pDX, IDC_CJJ166_SEIS_DESIGN_PARAMETER6, m_RadioCjjBridgeType_B);	
	DDX_Control(pDX, IDC_CJJ166_SEIS_PRINT_PARAMETER11, m_ChkCjjCopingDis);
	DDX_Control(pDX, IDC_CJJ166_SEIS_PRINT_PARAMETER12, m_ChkCjjPlasticTorsion);
	DDX_Check(pDX, IDC_CJJ166_SEIS_PRINT_PARAMETER1,  m_bCjjSeisDesignPrint[0]);
	DDX_Check(pDX, IDC_CJJ166_SEIS_PRINT_PARAMETER2,  m_bCjjSeisDesignPrint[1]);
	DDX_Check(pDX, IDC_CJJ166_SEIS_PRINT_PARAMETER3,  m_bCjjSeisDesignPrint[2]);
	DDX_Check(pDX, IDC_CJJ166_SEIS_PRINT_PARAMETER4,  m_bCjjSeisDesignPrint[3]);
	DDX_Check(pDX, IDC_CJJ166_SEIS_PRINT_PARAMETER5,  m_bCjjSeisDesignPrint[4]);
	DDX_Check(pDX, IDC_CJJ166_SEIS_PRINT_PARAMETER6,  m_bCjjSeisDesignPrint[5]);
	DDX_Check(pDX, IDC_CJJ166_SEIS_PRINT_PARAMETER7,  m_bCjjSeisDesignPrint[6]);
	DDX_Check(pDX, IDC_CJJ166_SEIS_PRINT_PARAMETER8,  m_bCjjSeisDesignPrint[7]);
	DDX_Check(pDX, IDC_CJJ166_SEIS_PRINT_PARAMETER9,  m_bCjjSeisDesignPrint[8]);
	DDX_Check(pDX, IDC_CJJ166_SEIS_PRINT_PARAMETER10, m_bCjjSeisDesignPrint[9]);
	DDX_Check(pDX, IDC_CJJ166_SEIS_PRINT_PARAMETER11, m_bCjjSeisDesignPrint[10]);
	DDX_Check(pDX, IDC_CJJ166_SEIS_PRINT_PARAMETER12, m_bCjjSeisDesignPrint[11]);
}


BEGIN_MESSAGE_MAP(DgnRCDesignParamParamDlg, CInternationalDlg)
	//{{AFX_MSG_MAP(DgnRCDesignParamParamDlg)
		// NOTE: the ClassWizard will add message map macros here
	ON_CBN_SELCHANGE(IDC_RC_DESIGN_CODE, OnSelchangeDgnCtrldataDesigncode)
	ON_BN_CLICKED(IDC_RC_PRINT_PARAMETER13,   OnClickSelectAll)
	ON_BN_CLICKED(IDC_RC_PRINT_PARAMETER14,   OnClickUnSelectAll)
	ON_BN_CLICKED(IDC_RAIL_PRINT_PARAMETER15, OnClickRailSelectAll)
	ON_BN_CLICKED(IDC_RAIL_PRINT_PARAMETER16, OnClickRailUnSelectAll)
	ON_BN_CLICKED(IDC_SEIS_DESIGN_PARAMETER1, OnChangeEarthquakeType)
	ON_BN_CLICKED(IDC_SEIS_DESIGN_PARAMETER2, OnChangeEarthquakeType)
	ON_BN_CLICKED(IDC_SEIS_DESIGN_PARAMETER3, OnChangeEarthquakeType)
	ON_BN_CLICKED(IDC_SEIS_DESIGN_PARAMETER5, OnChangeBridgeType)
	ON_BN_CLICKED(IDC_SEIS_DESIGN_PARAMETER6, OnChangeBridgeType)
	ON_BN_CLICKED(IDC_SEIS_PRINT_PARAMETER11, OnCheck_E2BC_Ctrl)
	ON_BN_CLICKED(IDC_SEIS_PRINT_PARAMETER12, OnCheck_E2BC_Ctrl)
	ON_BN_CLICKED(IDC_SEIS_PRINT_PARAMETER13, OnClickSeisSelectAll)
	ON_BN_CLICKED(IDC_SEIS_PRINT_PARAMETER14, OnClickSeisUnSelectAll)
	ON_BN_CLICKED(IDC_DGN_RC_DESIGN_CHK_SPECIALLOAD, OnClickSpecialLoad)
	ON_BN_CLICKED(IDC_CJJ166_SEIS_PRINT_PARAMETER13, OnClickCjjSeisSelectAll)
	ON_BN_CLICKED(IDC_CJJ166_SEIS_PRINT_PARAMETER14, OnClickCjjSeisUnSelectAll)
	ON_BN_CLICKED(IDC_CJJ166_SEIS_DESIGN_PARAMETER5, OnClickCjjBridgeType)
	ON_BN_CLICKED(IDC_CJJ166_SEIS_DESIGN_PARAMETER6, OnClickCjjBridgeType)

	ON_BN_CLICKED(IDC_CJJ166_SEIS_DESIGN_PARAMETER1, OnClickCjjEarthquakeType)
	ON_BN_CLICKED(IDC_CJJ166_SEIS_DESIGN_PARAMETER2, OnClickCjjEarthquakeType)
	ON_BN_CLICKED(IDC_CJJ166_SEIS_DESIGN_PARAMETER3, OnClickCjjEarthquakeType)
	ON_BN_CLICKED(IDC_CJJ166_SEIS_PRINT_PARAMETER12, OnCheck_CjjE2BC_Ctrl)
	ON_BN_CLICKED(IDC_CJJ166_SEIS_PRINT_PARAMETER11, OnCheck_CjjE2BC_Ctrl)

	//ON_BN_CLICKED(IDC_DGN_CONCMAT_MODIFY, SaveOrUpdate)
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DgnRCDesignParamParamDlg message handlers

BOOL DgnRCDesignParamParamDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	GetDlgItem(IDC_DGN_RC_DESIGN_CHK_SPECIALLOAD)->ShowWindow(SW_HIDE);

	m_DesignCode.ResetContent();
	CStringArray designCode; designCode.RemoveAll();
	//the Design-CODE of PSC and RC are same, so use PSC's getCodeNameList method at here.
	CDBLib::GetCHRcCodeNameList(designCode);
	for(int i=0; i<designCode.GetSize(); i++)	  m_DesignCode.AddString(designCode[i]);

	m_iDimType = 0;
	m_iSafeType = 0;
	m_iCrackCode = 0;
	m_iCrackType = 0;
	m_iRailCrackCode = 0;
	m_iEarthquakeType = 0;
	m_iBridgeType     = 0;
	for (int j=0; j<iIDPRINT; j++) {
		m_bDesignPrint[j] = TRUE;
	}
	for (int j=0; j<iRAILIDPRINT; j++) {
		m_bRailDesignPrint[j] = TRUE;
	}
	m_bSeismic = TRUE;
	for (int j=0; j<iSEISIDPRINT; j++) {		
		if(j==10 || j==11) m_bSeisDesignPrint[j] = FALSE;
		else               m_bSeisDesignPrint[j] = TRUE;
	}

	for (int j=0; j<iCJJ166SEISIDPRINT; j++) {		
		if(j==10 || j==11) m_bCjjSeisDesignPrint[j] = FALSE;
		else               m_IDCjjSeisDesignPrint[j] = TRUE;
	}
	
	InitCtrl();
	if(pDoc->m_pAttrCtrl->ExistChrp())
		Initial_ExistData();
	else
		Initial_Data();

	UpdateData(FALSE);

	return TRUE;
}

void DgnRCDesignParamParamDlg::OnClickSelectAll(){
	UpdateData(TRUE);
	for (int i=0; i<iIDPRINT; i++) {
		m_bDesignPrint[i] = TRUE;
	}	
	UpdateData(FALSE);
}
void DgnRCDesignParamParamDlg::OnClickUnSelectAll(){
	UpdateData(TRUE);
	for (int i=0; i<iIDPRINT; i++) {
		m_bDesignPrint[i] = FALSE;
	}
	UpdateData(FALSE);
}

void DgnRCDesignParamParamDlg::OnClickRailSelectAll(){
	UpdateData(TRUE);
	for (int i=0; i<iRAILIDPRINT; i++) {
		m_bRailDesignPrint[i] = TRUE;
	}	
	m_bSeismic = TRUE;
	UpdateData(FALSE);
}
void DgnRCDesignParamParamDlg::OnClickRailUnSelectAll(){
	UpdateData(TRUE);
	for (int i=0; i<iRAILIDPRINT; i++) {
		m_bRailDesignPrint[i] = FALSE;
	}
	m_bSeismic = FALSE;
	UpdateData(FALSE);
}

void DgnRCDesignParamParamDlg::OnChangeEarthquakeType()
{	
	Init_BridgeCtrl();
	Init_CheckCtrl();
}
void DgnRCDesignParamParamDlg::OnChangeBridgeType()
{
	Init_CheckCtrl();
}
void DgnRCDesignParamParamDlg::OnCheck_E2BC_Ctrl()
{
	Init_CheckCtrl_E2BC();
}
void DgnRCDesignParamParamDlg::OnClickSeisSelectAll()
{
	UpdateData(TRUE);
	for (int i=0; i<iSEISIDPRINT; i++) {	
		if(GetDlgItem(m_IDSeisDesignPrint[i])->IsWindowEnabled())
		{		
			if(i==11) m_bSeisDesignPrint[i] = FALSE;
			else      m_bSeisDesignPrint[i] = TRUE;
		}
	}
	UpdateData(FALSE);
	Init_BridgeCtrl();
}
void DgnRCDesignParamParamDlg::OnClickSeisUnSelectAll()
{
	UpdateData(TRUE);
	for (int i=0; i<iSEISIDPRINT; i++) {	
		if(GetDlgItem(m_IDSeisDesignPrint[i])->IsWindowEnabled())
		{ m_bSeisDesignPrint[i] = FALSE; }
	}
	UpdateData(FALSE);
	Init_BridgeCtrl();
}
void DgnRCDesignParamParamDlg::OnClickCjjSeisSelectAll()
{
	UpdateData(TRUE);
	for (int i=0; i<iCJJ166SEISIDPRINT; i++) {	
		if(GetDlgItem(m_IDCjjSeisDesignPrint[i])->IsWindowEnabled())
		{		
			if(i==11) m_bCjjSeisDesignPrint[i] = FALSE;
			else      m_bCjjSeisDesignPrint[i] = TRUE;
		}
	}
	UpdateData(FALSE);
	Init_CJJCheckCtrl();
}
void DgnRCDesignParamParamDlg::OnClickCjjSeisUnSelectAll()
{
	UpdateData(TRUE);
	for (int i=0; i<iCJJ166SEISIDPRINT; i++) {	
		if(GetDlgItem(m_IDCjjSeisDesignPrint[i])->IsWindowEnabled())
		{ m_bCjjSeisDesignPrint[i] = FALSE; }
	}
	UpdateData(FALSE);
	Init_CJJCheckCtrl();
}

void DgnRCDesignParamParamDlg::InitCtrl()
{	
	m_aRoad_OptID.RemoveAll();  m_aRoad_OutID.RemoveAll();
	m_aRail_OptID.RemoveAll();  m_aRail_OutID.RemoveAll();
	m_aSeis_OptID.RemoveAll();  m_aSeis_OutID.RemoveAll();
	m_aCjjSeis_OptID.RemoveAll();  m_aCjjSeis_OutID.RemoveAll();

	IDRDOPT(0);  IDRDOPT(1);  IDRDOPT(2);  IDRDOPT(3);  IDRDOPT(4);  IDRDOPT(5);  IDRDOPT(6);  IDRDOPT(7);  IDRDOPT(8);  IDRDOPT(9);
	IDRDOPT(10); IDRDOPT(11); IDRDOPT(12); IDRDOPT(13); IDRDOPT(14); IDRDOPT(15); IDRDOPT(16);
	
	IDRDOUT(0);  IDRDOUT(1);  IDRDOUT(2);  IDRDOUT(3);  IDRDOUT(4);  IDRDOUT(5);  IDRDOUT(6);  IDRDOUT(7);  IDRDOUT(8);  IDRDOUT(9);
	IDRDOUT(10); IDRDOUT(11); IDRDOUT(12); IDRDOUT(13); IDRDOUT(14); IDRDOUT(15); IDRDOUT(16);
	
	IDRLOPT(0);  IDRLOPT(1);  IDRLOPT(2);  IDRLOPT(3);  IDRLOPT(4);  IDRLOPT(5);  IDRLOPT(6);  IDRLOPT(7);  IDRLOPT(8); 
	
	IDRLOUT(0);  IDRLOUT(1);  IDRLOUT(2);  IDRLOUT(3);  IDRLOUT(4);  IDRLOUT(5);  IDRLOUT(6);  IDRLOUT(7);  IDRLOUT(8);  IDRLOUT(9);
	IDRLOUT(10); IDRLOUT(11); IDRLOUT(12); IDRLOUT(13); IDRLOUT(14); IDRLOUT(15); IDRLOUT(16); IDRLOUT(17);
	
	IDSEOPT(0);  IDSEOPT(1);  IDSEOPT(2);  IDSEOPT(3);  IDSEOPT(4);  IDSEOPT(5);  IDSEOPT(6);
	
	IDSEOUT(0);  IDSEOUT(1);  IDSEOUT(2);  IDSEOUT(3);  IDSEOUT(4);  IDSEOUT(5);  IDSEOUT(6);  IDSEOUT(7);  IDSEOUT(8);  IDSEOUT(9);
	IDSEOUT(10); IDSEOUT(11); IDSEOUT(12); IDSEOUT(13); IDSEOUT(14);

	IDCJJSEOPT(4);  IDCJJSEOPT(5);  IDCJJSEOPT(6);IDCJJSEOPT(0);  IDCJJSEOPT(1);  IDCJJSEOPT(2);  IDCJJSEOPT(3);  
	
	IDCJJSEOUT(0);  IDCJJSEOUT(1);  IDCJJSEOUT(2);  IDCJJSEOUT(3);  IDCJJSEOUT(4);  IDCJJSEOUT(5);  IDCJJSEOUT(6);  IDCJJSEOUT(7);  IDCJJSEOUT(8);  IDCJJSEOUT(9);
	IDCJJSEOUT(10); IDCJJSEOUT(11); IDCJJSEOUT(12); IDCJJSEOUT(13); IDCJJSEOUT(14);

	
	m_IDSeisDesignPrint[0]  = IDC_SEIS_PRINT_PARAMETER1;
	m_IDSeisDesignPrint[1]  = IDC_SEIS_PRINT_PARAMETER2;
	m_IDSeisDesignPrint[2]  = IDC_SEIS_PRINT_PARAMETER3;
	m_IDSeisDesignPrint[3]  = IDC_SEIS_PRINT_PARAMETER4;
	m_IDSeisDesignPrint[4]  = IDC_SEIS_PRINT_PARAMETER5;
	m_IDSeisDesignPrint[5]  = IDC_SEIS_PRINT_PARAMETER6;
	m_IDSeisDesignPrint[6]  = IDC_SEIS_PRINT_PARAMETER7;
	m_IDSeisDesignPrint[7]  = IDC_SEIS_PRINT_PARAMETER8;
	m_IDSeisDesignPrint[8]  = IDC_SEIS_PRINT_PARAMETER9;
	m_IDSeisDesignPrint[9]  = IDC_SEIS_PRINT_PARAMETER10;
	m_IDSeisDesignPrint[10] = IDC_SEIS_PRINT_PARAMETER11;
	m_IDSeisDesignPrint[11] = IDC_SEIS_PRINT_PARAMETER12;

	m_IDCjjSeisDesignPrint[0]  = IDC_CJJ166_SEIS_PRINT_PARAMETER1;
	m_IDCjjSeisDesignPrint[1]  = IDC_CJJ166_SEIS_PRINT_PARAMETER2;
	m_IDCjjSeisDesignPrint[2]  = IDC_CJJ166_SEIS_PRINT_PARAMETER3;
	m_IDCjjSeisDesignPrint[3]  = IDC_CJJ166_SEIS_PRINT_PARAMETER4;
	m_IDCjjSeisDesignPrint[4]  = IDC_CJJ166_SEIS_PRINT_PARAMETER5;
	m_IDCjjSeisDesignPrint[5]  = IDC_CJJ166_SEIS_PRINT_PARAMETER6;
	m_IDCjjSeisDesignPrint[6]  = IDC_CJJ166_SEIS_PRINT_PARAMETER7;
	m_IDCjjSeisDesignPrint[7]  = IDC_CJJ166_SEIS_PRINT_PARAMETER8;
	m_IDCjjSeisDesignPrint[8]  = IDC_CJJ166_SEIS_PRINT_PARAMETER9;
	m_IDCjjSeisDesignPrint[9]  = IDC_CJJ166_SEIS_PRINT_PARAMETER10;
	m_IDCjjSeisDesignPrint[10] = IDC_CJJ166_SEIS_PRINT_PARAMETER11;
	m_IDCjjSeisDesignPrint[11] = IDC_CJJ166_SEIS_PRINT_PARAMETER12;


	CRect RectOptOrg, RectOptCrr, RectOutOrg, RectOutCrr;
	GetDlgItem(IDC_RC_DESIGN_PARAMETER0)->GetWindowRect(&RectOptOrg);
	GetDlgItem(IDC_RC_PRINT_PARAMETER0)->GetWindowRect(&RectOutOrg);

	GetDlgItem(IDC_RAIL_DESIGN_PARAMETER0)->GetWindowRect(&RectOptCrr);
	GetDlgItem(IDC_RAIL_PRINT_PARAMETER0)->GetWindowRect(&RectOutCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aRail_OptID, (RectOptOrg.top - RectOptCrr.top), TRUE);
	CDlgUtil::CtrlMoveDistY(this, m_aRail_OutID, (RectOutOrg.top - RectOutCrr.top), TRUE);

	GetDlgItem(IDC_SEIS_DESIGN_PARAMETER0)->GetWindowRect(&RectOptCrr);
	GetDlgItem(IDC_SEIS_PRINT_PARAMETER0)->GetWindowRect(&RectOutCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aSeis_OptID, (RectOptOrg.top - RectOptCrr.top), TRUE);
	CDlgUtil::CtrlMoveDistY(this, m_aSeis_OutID, (RectOutOrg.top - RectOutCrr.top), TRUE);

	GetDlgItem(IDC_CJJ166_SEIS_DESIGN_PARAMETER4)->GetWindowRect(&RectOptCrr);
	GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER0)->GetWindowRect(&RectOutCrr);
	CDlgUtil::CtrlMoveDistY(this, m_aCjjSeis_OptID, (RectOptOrg.top - RectOptCrr.top), TRUE);
	CDlgUtil::CtrlMoveDistY(this, m_aCjjSeis_OutID, (RectOutOrg.top - RectOutCrr.top), TRUE);

	
	CRect RectDlg;
	GetWindowRect(&RectDlg);	
	SetWindowPos(NULL, 0, 0, RectDlg.Width(), (RectOutOrg.top+RectOutOrg.Height()+5), SWP_NOMOVE | SWP_NOZORDER);	
}

void DgnRCDesignParamParamDlg::Initial_ExistData()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);  
	T_CHRP_D RcD; RcD.Initialize();
	 pDoc->m_pAttrCtrl->GetChrp(RcD);
	m_iDgnCode   = RcD.iDgnCode;  // 1:JSCE02, 2:KSCE-USD03, 3:JTG D62-04
	CString strCodeName=_T("");
	//the Design-CodeName of PSC and RC are same, so use PSC's GetPscCodeName method at here.
	strCodeName = CDBLib::GetCHRcCodeName(m_iDgnCode);
	int Index=0;
	if(strCodeName!=_T(""))	Index = m_DesignCode.FindStringExact(-1,strCodeName);
	m_DesignCode.SetCurSel(Index);
	
	if(m_iDgnCode==RC_JTG_D62_04|| m_iDgnCode==RC_CJJ_11_2011) {
		m_iDimType = RcD.iDimType;
		m_iSafeType = RcD.iSafeType;
		m_iCrackCode = RcD.iCrackCode;
		m_iCrackType = RcD.iCrackType;
		for (int i=0; i<iIDPRINT; i++) {
			m_bDesignPrint[i] = RcD.bDesignPrint[i];
		}
	}else if (m_iDgnCode==RC_TB_10002_3_05) {
		m_iRailCrackCode = RcD.iCrackCode;
		for (int i=0; i<iRAILIDPRINT; i++) {
			m_bRailDesignPrint[i] = RcD.bDesignPrint[i];
		}
		if (RcD.iCrackType == 0) m_bSeismic = FALSE;//seismic =1; unseismic =0;
		else if (RcD.iCrackType == 1)  m_bSeismic = TRUE;  
	}else if (m_iDgnCode==RC_JTG_B02_01_2008) {
		m_iEarthquakeType = RcD.iEarthquakeType;
		m_iBridgeType     = RcD.iBridgeType;
		for (int i=0; i<iSEISIDPRINT; i++) {
			m_bSeisDesignPrint[i] = RcD.bDesignPrint[i];
		}
	}
	else if (m_iDgnCode==RC_CJJ_166_2011) {
		m_iCjjEarthquakeType = RcD.iEarthquakeType;
		m_iCjjBridgeType     = RcD.iBridgeType;
		for (int i=0; i<iCJJ166SEISIDPRINT; i++) {
			m_bCjjSeisDesignPrint[i] = RcD.bDesignPrint[i];
		}
	}
	else  ASSERT(0);
	if(m_iDgnCode==RC_CJJ_11_2011)
		CheckDlgButton(IDC_DGN_RC_DESIGN_CHK_SPECIALLOAD,RcD.bExistSpecialLoad);

	UpdateData(FALSE);
	OnSelchangeDgnCtrldataDesigncode();
}

void DgnRCDesignParamParamDlg::Initial_Data()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);  
	T_CHRP_D RcD; RcD.Initialize();
	pDoc->m_pAttrCtrl->GetChrp(RcD);
	m_iDgnCode   = RcD.iDgnCode;  
	m_DesignCode.SetCurSel(0);
	m_iDimType = RcD.iDimType;
	m_iSafeType = RcD.iSafeType;
	m_iCrackCode = RcD.iCrackCode;
	m_iCrackType = RcD.iCrackType;
	for (int j=0; j<iIDPRINT; j++) {
		m_bDesignPrint[j] = TRUE;
	}

	m_iRailCrackCode = 0;
	for (int j=0; j<iRAILIDPRINT; j++) {
		m_bRailDesignPrint[j] = TRUE;
	}
	m_bSeismic = TRUE;

	m_iEarthquakeType = 0;
	m_iBridgeType     = 0;
	for (int j=0; j<iSEISIDPRINT; j++) {		
		if(j==10 || j==11) m_bSeisDesignPrint[j] = FALSE;
		else               m_bSeisDesignPrint[j] = TRUE;
	}
	m_iCjjEarthquakeType = 0;
	m_iCjjBridgeType     = 0;
	for (int j=0; j<iCJJ166SEISIDPRINT; j++) {		
		if(j==10 || j==11) m_bCjjSeisDesignPrint[j] = FALSE;
		else               m_bCjjSeisDesignPrint[j] = TRUE;
	}
	if(m_iDgnCode==RC_CJJ_11_2011)
		CheckDlgButton(IDC_DGN_RC_DESIGN_CHK_SPECIALLOAD,RcD.bExistSpecialLoad);

	UpdateData(FALSE);
	OnSelchangeDgnCtrldataDesigncode();
}

void DgnRCDesignParamParamDlg::SaveOrUpdate() 
{
	UpdateData(TRUE);
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	T_CHRP_D RcdD; RcdD.Initialize();
	pDoc->m_pAttrCtrl->GetChrp(RcdD);
	
	CString strCodeNa=_T("");
	int iIndex = m_DesignCode.GetCurSel();
	m_DesignCode.GetLBText(iIndex, strCodeNa);
	//the Design-CodeNo of PSC and RC are same, so use PSC's GetPscCodeNo method at here.
	RcdD.iDgnCode = CDBLib::GetCHRcCodeNo(strCodeNa);
	if(RcdD.iDgnCode==RC_JTG_D62_04|| m_iDgnCode==RC_CJJ_11_2011){
		RcdD.iDimType= m_iDimType;
		RcdD.iSafeType = m_iSafeType;
		RcdD.iCrackCode = m_iCrackCode;
		RcdD.iCrackType = m_iCrackType;
		for (int i=0; i<iIDPRINT; i++) {
			RcdD.bDesignPrint[i] = m_bDesignPrint[i];
		} 
	}else if (RcdD.iDgnCode == RC_TB_10002_3_05) {
		RcdD.iCrackCode = m_iRailCrackCode;
		for (int i=0; i<iRAILIDPRINT;i++) {
			RcdD.bDesignPrint[i] = m_bRailDesignPrint[i];
		}
		if (m_bSeismic) RcdD.iCrackType = 1;
		else RcdD.iCrackType = 0;
	}else if (RcdD.iDgnCode == RC_JTG_B02_01_2008) {
		RcdD.iEarthquakeType = m_iEarthquakeType;
		RcdD.iBridgeType = m_iBridgeType;
		for (int i=0; i<iSEISIDPRINT;i++) {
			RcdD.bDesignPrint[i] = m_bSeisDesignPrint[i];
		}
	}
	else if (RcdD.iDgnCode == RC_CJJ_166_2011) {
		RcdD.iEarthquakeType = m_iCjjEarthquakeType;
		RcdD.iBridgeType = m_iCjjBridgeType;
		for (int i=0; i<iCJJ166SEISIDPRINT;i++) {
			RcdD.bDesignPrint[i] = m_bCjjSeisDesignPrint[i];
		}
	}
	else  ASSERT(0);

	
	if(m_iDgnCode==RC_CJJ_11_2011)
	{
		RcdD.bExistSpecialLoad = IsDlgButtonChecked(IDC_DGN_RC_DESIGN_CHK_SPECIALLOAD);
	}
	BOOL bCheck = pDoc->m_pDataCtrl->AddChrp(RcdD);
	if(!bCheck) 
		AfxMessageBox(_LS(IDS_DGN_CHK_TITLE_ERROR), MB_OK); 
//	  CChildDialog::OnOK();
	if (bCheck) m_pParent->SaveOrUpdate();
}

void DgnRCDesignParamParamDlg::ChangeDesignCode()
{
	UpdateData(FALSE);
	OnSelchangeDgnCtrldataDesigncode();
}


void DgnRCDesignParamParamDlg::OnSelchangeDgnCtrldataDesigncode() 
{
	UpdateData(TRUE);
	
	CString strCode=_T("");
	m_DesignCode.GetWindowText(strCode);
	int iStatus = CDBLib::GetCHRcCodeNo(strCode);
	m_iDgnCode = iStatus;

	GetDlgItem(IDC_DGN_REFPOS)->ShowWindow(SW_HIDE);

	CDlgUtil::CtrlShowHide(this, m_aRoad_OptID, (m_iDgnCode==RC_JTG_D62_04));
	CDlgUtil::CtrlShowHide(this, m_aRoad_OutID, (m_iDgnCode==RC_JTG_D62_04));
	
	if(m_iDgnCode==RC_CJJ_11_2011)
	{
		CDlgUtil::CtrlShowHide(this, m_aRoad_OptID, (m_iDgnCode==RC_CJJ_11_2011));
		CDlgUtil::CtrlShowHide(this, m_aRoad_OutID, (m_iDgnCode==RC_CJJ_11_2011));

		GetDlgItem(IDC_DGN_RC_DESIGN_CHK_SPECIALLOAD)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_DGN_RC_DESIGN_CHK_SPECIALLOAD)->ShowWindow(SW_HIDE);
	}
	CDlgUtil::CtrlShowHide(this, m_aRail_OptID, (m_iDgnCode==RC_TB_10002_3_05));
	CDlgUtil::CtrlShowHide(this, m_aRail_OutID, (m_iDgnCode==RC_TB_10002_3_05));
	CDlgUtil::CtrlShowHide(this, m_aSeis_OptID, (m_iDgnCode==RC_JTG_B02_01_2008));
	CDlgUtil::CtrlShowHide(this, m_aSeis_OutID, (m_iDgnCode==RC_JTG_B02_01_2008));
	CDlgUtil::CtrlShowHide(this, m_aCjjSeis_OptID, (m_iDgnCode==RC_CJJ_166_2011));
	CDlgUtil::CtrlShowHide(this, m_aCjjSeis_OutID, (m_iDgnCode==RC_CJJ_166_2011));

	EnableCtrls();
	OnClickSpecialLoad();

	UpdateData(FALSE);  

	if(m_iDgnCode==RC_JTG_B02_01_2008) Init_BridgeCtrl();
	if(m_iDgnCode==RC_CJJ_166_2011) Init_CJJBridgeCtrl();

 
	

}

void DgnRCDesignParamParamDlg::EnableCtrls()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
}

void DgnRCDesignParamParamDlg::Init_BridgeCtrl()
{			
	if(m_RadioEarthE1.GetCheck())
	{
		m_RadioBridgeType_D.EnableWindow(TRUE);
	}
	else 
	{
		m_RadioBridgeType_D.EnableWindow(FALSE);
		m_RadioBridgeType_BC.SetCheck(TRUE);
		m_RadioBridgeType_D.SetCheck(FALSE);
	}
	Init_CheckCtrl();
}
void DgnRCDesignParamParamDlg::Init_CheckCtrl()
{
	if(m_RadioEarthE1.GetCheck())
	{
		if(m_RadioBridgeType_BC.GetCheck())
		{
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER1 )->EnableWindow(TRUE);  //교각강도           
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER2 )->EnableWindow(TRUE);  //교대강도     
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER3 )->EnableWindow(TRUE);  //기초강도   
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER4 )->EnableWindow(FALSE); //Capping beam강도   
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER5 )->EnableWindow(TRUE);  //Main arch강도     
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER6 )->EnableWindow(FALSE); //교각소성힌지전단강도
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER7 )->EnableWindow(TRUE);  //지지점두께판식고무지지점    
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER8 )->EnableWindow(TRUE);  //지지점미끄럼온성성(판식고무지지점)    
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER9 )->EnableWindow(TRUE);  //지지점수평변위(활동쟁반형지지점)
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER10)->EnableWindow(TRUE);  //지지점수평재하력(고정쟁반형지지점)
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER11)->EnableWindow(FALSE); //교각상부변위(정형교량)        
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER12)->EnableWindow(FALSE); //소성비틀림각변위(비정형교량)  
		}
		else 
		{
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER1 )->EnableWindow(TRUE);  //교각강도           
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER2 )->EnableWindow(FALSE); //교대강도     
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER3 )->EnableWindow(TRUE);  //기초강도   
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER4 )->EnableWindow(TRUE);  //Capping beam강도   
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER5 )->EnableWindow(FALSE); //Main arch강도     
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER6 )->EnableWindow(FALSE); //교각소성힌지전단강도
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER7 )->EnableWindow(TRUE);  //지지점두께판식고무지지점    
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER8 )->EnableWindow(TRUE);  //지지점미끄럼온성성(판식고무지지점)    
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER9 )->EnableWindow(TRUE);  //지지점수평변위(활동쟁반형지지점)
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER10)->EnableWindow(TRUE);  //지지점수평재하력(고정쟁반형지지점)
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER11)->EnableWindow(FALSE); //교각상부변위(정형교량)        
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER12)->EnableWindow(FALSE); //소성비틀림각변위(비정형교량)  
		}
	}
	else if(m_RadioEarthE2_E.GetCheck())
	{
		if(m_RadioBridgeType_BC.GetCheck())
		{
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER1 )->EnableWindow(TRUE);  //교각강도           
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER2 )->EnableWindow(TRUE);  //교대강도     
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER3 )->EnableWindow(TRUE);  //기초강도   
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER4 )->EnableWindow(TRUE);  //Capping beam강도   
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER5 )->EnableWindow(TRUE);  //Main arch강도     
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER6 )->EnableWindow(FALSE); //교각소성힌지전단강도
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER7 )->EnableWindow(TRUE); //지지점두께판식고무지지점    
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER8 )->EnableWindow(TRUE); //지지점미끄럼온성성(판식고무지지점)    
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER9 )->EnableWindow(TRUE); //지지점수평변위(활동쟁반형지지점)
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER10)->EnableWindow(TRUE); //지지점수평재하력(고정쟁반형지지점)
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER11)->EnableWindow(FALSE); //교각상부변위(정형교량)        
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER12)->EnableWindow(FALSE); //소성비틀림각변위(비정형교량)  
		}
		else 
		{
			ASSERT(0);
		}
	}
	else if(m_RadioEarthE2_EP.GetCheck())
	{
		if(m_RadioBridgeType_BC.GetCheck())
		{
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER1 )->EnableWindow(TRUE);  //교각강도           
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER2 )->EnableWindow(TRUE);  //교대강도     
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER3 )->EnableWindow(FALSE); //기초강도   
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER4 )->EnableWindow(TRUE);  //Capping beam강도   
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER5 )->EnableWindow(TRUE);  //Main arch강도     
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER6 )->EnableWindow(TRUE);  //교각소성힌지전단강도
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER7 )->EnableWindow(TRUE); //지지점두께판식고무지지점    
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER8 )->EnableWindow(TRUE); //지지점미끄럼온성성(판식고무지지점)    
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER9 )->EnableWindow(TRUE); //지지점수평변위(활동쟁반형지지점)
			GetDlgItem(IDC_SEIS_PRINT_PARAMETER10)->EnableWindow(TRUE); //지지점수평재하력(고정쟁반형지지점)
			Init_CheckCtrl_E2BC();
		}
		else 
		{
			ASSERT(0);
		}
	}
}	
void DgnRCDesignParamParamDlg::Init_CheckCtrl_E2BC()
{
	if(m_RadioEarthE2_EP.GetCheck())
	{
		if(m_RadioBridgeType_BC.GetCheck())
		{
			if(m_ChkCopingDis.GetCheck() && m_ChkPlasticTorsion.GetCheck())
			{
				ASSERT(0); // 2개 모두 선택상태는 존재할수 없음
				m_ChkPlasticTorsion.SetCheck(FALSE);
			}
			BOOL bChk = (m_ChkCopingDis.GetCheck() || (!m_ChkCopingDis.GetCheck() && !m_ChkPlasticTorsion.GetCheck()));
			m_ChkCopingDis.EnableWindow(bChk);         
			bChk = (m_ChkPlasticTorsion.GetCheck() || (!m_ChkCopingDis.GetCheck() && !m_ChkPlasticTorsion.GetCheck()));
			m_ChkPlasticTorsion.EnableWindow(bChk);   
		}
	}
}
void DgnRCDesignParamParamDlg::Init_CJJBridgeCtrl()
{			
	if(m_RadioCjjBridgeType_A.GetCheck())
	{

		m_RadioCjjEarthE1.EnableWindow(TRUE);
		m_RadioCjjEarthE2_E.EnableWindow(TRUE);
		m_RadioCjjEarthE2_EP.EnableWindow(TRUE);
	}
	else 
	{
		m_RadioCjjEarthE1.EnableWindow(TRUE);
		m_RadioCjjEarthE1.SetCheck(TRUE);
		m_RadioCjjEarthE2_E.EnableWindow(FALSE);
		m_RadioCjjEarthE2_E.SetCheck(FALSE);
		m_RadioCjjEarthE2_EP.EnableWindow(FALSE);
		m_RadioCjjEarthE2_EP.SetCheck(FALSE);
	}
	Init_CJJCheckCtrl();
}
void DgnRCDesignParamParamDlg::Init_CJJCheckCtrl()
{
	if(m_iDgnCode==RC_CJJ_166_2011)
	{
		GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER3 )->ShowWindow(SW_HIDE);
		CheckDlgButton(IDC_CJJ166_SEIS_PRINT_PARAMETER3,FALSE);
	}
	if(m_RadioCjjEarthE1.GetCheck())
	{
		
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER1 )->EnableWindow(TRUE);  //교각강도           
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER2 )->EnableWindow(TRUE);  //교대강도     
			//GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER3 )->EnableWindow(TRUE);  //기초강도   
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER4 )->EnableWindow(TRUE); //Capping beam강도   
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER5 )->EnableWindow(TRUE);  //Main arch강도     
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER6 )->EnableWindow(FALSE); //교각소성힌지전단강도
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER7 )->EnableWindow(TRUE);  //지지점두께판식고무지지점    
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER8 )->EnableWindow(TRUE);  //지지점미끄럼온성성(판식고무지지점)    
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER9 )->EnableWindow(TRUE);  //지지점수평변위(활동쟁반형지지점)
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER10)->EnableWindow(TRUE);  //지지점수평재하력(고정쟁반형지지점)
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER11)->EnableWindow(FALSE); //교각상부변위(정형교량)        
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER12)->EnableWindow(FALSE); //소성비틀림각변위(비정형교량)  
		
	}
	else if(m_RadioCjjEarthE2_E.GetCheck())
	{

			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER1 )->EnableWindow(TRUE);  //교각강도           
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER2 )->EnableWindow(TRUE);  //교대강도     
			//GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER3 )->EnableWindow(TRUE);  //기초강도   
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER4 )->EnableWindow(TRUE);  //Capping beam강도   
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER5 )->EnableWindow(TRUE);  //Main arch강도     
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER6 )->EnableWindow(FALSE); //교각소성힌지전단강도
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER7 )->EnableWindow(TRUE); //지지점두께판식고무지지점    
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER8 )->EnableWindow(TRUE); //지지점미끄럼온성성(판식고무지지점)    
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER9 )->EnableWindow(TRUE); //지지점수평변위(활동쟁반형지지점)
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER10)->EnableWindow(TRUE); //지지점수평재하력(고정쟁반형지지점)
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER11)->EnableWindow(FALSE); //교각상부변위(정형교량)        
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER12)->EnableWindow(FALSE); //소성비틀림각변위(비정형교량)  
		
	}
	else if(m_RadioCjjEarthE2_EP.GetCheck())
	{

			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER1 )->EnableWindow(TRUE);  //교각강도           
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER2 )->EnableWindow(TRUE);  //교대강도     
			//GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER3 )->EnableWindow(TRUE); //기초강도   
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER4 )->EnableWindow(TRUE);  //Capping beam강도   
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER5 )->EnableWindow(TRUE);  //Main arch강도     
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER6 )->EnableWindow(TRUE);  //교각소성힌지전단강도
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER7 )->EnableWindow(TRUE); //지지점두께판식고무지지점    
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER8 )->EnableWindow(TRUE); //지지점미끄럼온성성(판식고무지지점)    
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER9 )->EnableWindow(TRUE); //지지점수평변위(활동쟁반형지지점)
			GetDlgItem(IDC_CJJ166_SEIS_PRINT_PARAMETER10)->EnableWindow(TRUE); //지지점수평재하력(고정쟁반형지지점)
			Init_CJJCheckCtrl_E2BC();
		
	}
}	
void DgnRCDesignParamParamDlg::Init_CJJCheckCtrl_E2BC()
{
	if(m_RadioCjjEarthE2_EP.GetCheck())
	{

			if(m_ChkCjjCopingDis.GetCheck() && m_ChkCjjPlasticTorsion.GetCheck())
			{
				ASSERT(0); // 2개 모두 선택상태는 존재할수 없음
				m_ChkCjjPlasticTorsion.SetCheck(FALSE);
			}
			BOOL bChk = (m_ChkCjjCopingDis.GetCheck() || (!m_ChkCjjCopingDis.GetCheck() && !m_ChkCjjPlasticTorsion.GetCheck()));
			m_ChkCjjCopingDis.EnableWindow(bChk);         
			bChk = (m_ChkCjjPlasticTorsion.GetCheck() || (!m_ChkCjjCopingDis.GetCheck() && !m_ChkCjjPlasticTorsion.GetCheck()));
			m_ChkCjjPlasticTorsion.EnableWindow(bChk);   
		
	}
}
void DgnRCDesignParamParamDlg::OnClickSpecialLoad()
{
	if(m_iDgnCode==RC_CJJ_11_2011 || m_iDgnCode==RC_JTG_D62_04)
	{
		
	}
	else
		return;
	
	BOOL bChecked = IsDlgButtonChecked(IDC_DGN_RC_DESIGN_CHK_SPECIALLOAD);
	if(m_iDgnCode==RC_CJJ_11_2011 && bChecked)
	{
		GetDlgItem(IDC_RC_DESIGN_PARAMETER14)->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_SPECIALLOAD ));
		GetDlgItem(IDC_RC_DESIGN_PARAMETER4 )->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_SPECIALLOAD1));
		GetDlgItem(IDC_RC_DESIGN_PARAMETER5 )->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_SPECIALLOAD2));
		GetDlgItem(IDC_RC_DESIGN_PARAMETER6 )->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_SPECIALLOAD3));
	}
	else if(m_iDgnCode==RC_JTG_D62_04 || (m_iDgnCode==RC_CJJ_11_2011 && !bChecked))
	{
		GetDlgItem(IDC_RC_DESIGN_PARAMETER14)->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_ROADGRADE ));
		GetDlgItem(IDC_RC_DESIGN_PARAMETER4 )->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_ROADGRADE1));
		GetDlgItem(IDC_RC_DESIGN_PARAMETER5 )->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_ROADGRADE2));
		GetDlgItem(IDC_RC_DESIGN_PARAMETER6 )->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_ROADGRADE3));
		
	}
	else
	{
		
	 }
}

void DgnRCDesignParamParamDlg::OnClickCjjBridgeType()
{
	Init_CJJBridgeCtrl();
}

void DgnRCDesignParamParamDlg::OnClickCjjEarthquakeType()
{
	Init_CJJBridgeCtrl();
}

void DgnRCDesignParamParamDlg::OnCheck_CjjE2BC_Ctrl()
{
	Init_CJJCheckCtrl_E2BC();
}

#undef IDRDOPT
#undef IDRDOUT
#undef IDRLOPT
#undef IDRLOUT
#undef IDSEOPT
#undef IDSEOUT
#undef IDCJJSEOPT
#undef IDCJJSEOUT