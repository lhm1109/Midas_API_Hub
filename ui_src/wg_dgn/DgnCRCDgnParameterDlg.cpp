// DgnCRCDgnParameterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "DgnCRCDgnParameterDlg.h"

//#include "DgnPscCommon.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"

#include "..\wg_main\wg_main.h"
#include "..\wg_base\TestEnvMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DgnCRCDgnParameter_EC2PSC.h"
#include "DgnCRCDgnParameter_BS.h"
#include "DgnCRCDgnParameter_CSA.h"
#include "DgnCRCDgnParameter_CSA_19.h"
#include "DgnCRCDgnParameter_AASHTO12.h"
#include "DgnCRCDgnParameter_LSDPSC.h"
#include "DgnCRCDgnParameter_RUS.h"
#include "DgnCRCDgnParameter_AS5100.h"
#include "DgnCRCDgnParameter_IRS.h"
#include "DgnCRCDgnParameter_TMH7.h"
#include "DgnCRCDgnParameter_ABNT.h"
#include "NationalAnnexTool.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg dialog

#define IDPSC0(n)\
	IDPSC0.Add(IDC_CRC_DESIGN_PARAMETER##n);
#define IDPSC1(n)\
	IDPSC1.Add(IDC_CRC_PRINT_PARAMETER##n);
#define IDPSC2(n)\
	IDPSC2.Add(IDC_KSCE_DESIGN_PARAMETER##n);
#define IDPSC3(n)\
	IDPSC3.Add(IDC_KSCE_PRINT_PARAMETER##n);
#define IDPSC4(n)\
	IDPSC4.Add(IDC_JTG_DESIGN_PARAMETER##n);
#define IDPSC5(n)\
	IDPSC5.Add(IDC_JTG_PRINT_PARAMETER##n);
// Add by sshan. MNET:2728. TB 10002.3-2005 ('20070411)
#define IDPSC6(n)\
	IDPSC6.Add(IDC_TB_DESIGN_PARAMETER##n);
#define IDPSC7(n)\
	IDPSC7.Add(IDC_TB_PRINT_PARAMETER##n);
#define IDPSC8(n)\
IDPSC8.Add(IDC_AASHTO_DESIGN_PARAMETER##n);
#define IDPSC9(n)\
IDPSC9.Add(IDC_AASHTO_PRINT_PARAMETER##n);
//(2009.11.10) Add by Unsang:: Resource 개수제한때문에 Eurocode2-2:05,PSC부터 Place-Holder를 이용해서 처리함.
#define IDPSC10(n)\
IDPSC10.Add(IDC_EC2PSC_PLACEHOLDER##n); 
#define IDPSC11(n)\
IDPSC11.Add(IDC_BS_PLACEHOLDER##n);
#define IDPSC12(n)\
IDPSC12.Add(IDC_CSA_PLACEHOLDER##n);
#define IDPSC13(n)\
IDPSC13.Add(IDC_AASHTO12_PLACEHOLDER##n);
#define IDPSC14(n)\
IDPSC14.Add(IDC_KSCELSD12_PLACEHOLDER##n); 
#define IDPSC15(n)\
IDPSC15.Add(IDC_RUS_PLACEHOLDER##n); 
#define IDPSC16(n)\
IDPSC16.Add(IDC_RUS_PLACEHOLDER##n); 
#define IDPSC17(n)\
IDPSC17.Add(IDC_AS5100_PLACEHOLDER##n); 
#define IDPSC18(n)\
IDPSC18.Add(IDC_IRS_PLACEHOLDER##n);
#define IDPSC19(n)\
IDPSC19.Add(IDC_CSA19_PLACEHOLDER##n);
#define IDPSC20(n)\
IDPSC20.Add(IDC_TMH_PLACEHOLDER##n);

enum CHILDDLGKEY
{
	EC = 0,
	BS,
	CSA,
	CSA19,
	AASHTO,
	KSCE,
	SNIP,
	SP,
	AS,
	IRS,
	TMH7,
	ABNT
};

using namespace dgn::def;

CDgnCRCDgnParameterDlg::CDgnCRCDgnParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnCRCDgnParameterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConMatDlg)
	m_iDimType = -1;
	m_iBrgType = -1;
	m_bCmpWeb = FALSE;
	m_iBrgDir = -1;
	m_iCrackCode = -1;
	m_iTCrackWidth = -1;
	m_iBCrackWidth = -1;
	m_iPsReduFactor = -1;
	m_iDimTypeKsce = -1;
	m_iBrgTypeKsce = -1;
	m_iTCrackWidthKsce = -1;
	m_iBCrackWidthKsce = -1;

	int m_ifpsMethod = -1;  
	int m_iTndnKind  = -1;
	m_bRefRebar = TRUE;

	m_iDimTypeJTG = -1;
	m_iBrgTypeJTG = -1;
	m_iBrgGradeJTG = -1;
	m_iCastTypeJTG = -1; 
	m_iSBarCrackWidthJTG = -1;
	m_iWSCrackWidthJTG = -1;

	m_iTndnTypeAASHTO = -1;
	m_iBrgTypeAASHTO = -1;
	m_iConstructionTypeAASHTO = -1;
	m_iCorrosiveTypeAASHTO = -1;
	m_iExposureTypeAASHTO = -1;
	m_iFlexureCalcTypeAASHTO = -1;

	int i=0;
	for(i=0; i<iIDPRINT0; i++)  m_bDesignPrint[i]=FALSE;
	for(i=0; i<iIDPRINT1; i++)      m_bStructPrint[i]=FALSE;
	for(i=0; i<iIDPRINT2; i++)      m_bMomentPrint[i]=FALSE;
	for(i=0; i<iIDPRINT3; i++)      m_bShearPrint[i]=FALSE;
	for(i=0; i<iIDPRINT4; i++)      m_bTorsionPrint[i]=FALSE;
	for(i=0; i<iIDPRINT5; i++)      m_bEtcPrint[i]=FALSE;
	for(i=0; i<iIDPRINT6; i++)      m_bBckPrint[i]=FALSE;
	
	for(i=0; i<iIDPRINTSERV; i++)  m_bServPrint[i]=FALSE; 
	for(i=0; i<iIDPRINTULTI; i++)  m_bUltiPrint[i]=FALSE;   

	for(i=0; i<iIDPRINTSERV_JTG; i++)  m_bServPrintJTG[i]=FALSE; 
	for(i=0; i<iIDPRINTULTI_JTG; i++)  m_bUltiPrintJTG[i]=FALSE;   

	// Add by sshan. MNET:2728. TB 10002.3-2005 ('20070411)
	for(i=0; i<iIDPRINTBEAM_TB; i++)  m_bBeamPrintTB[i]=FALSE; 
	for(i=0; i<iIDPRINTCOLU_TB; i++)  m_bColuPrintTB[i]=FALSE;
	
	
	for(i=0; i<iOUTPUTPARAM_SERVICE_AASHTO_COUNT; i++)	m_bPrintParam_Service_AASHTO[i] = FALSE;
	for(i=0; i<iOUTPUTPARAM_FACTORED_AASHTO_COUNT; i++)	m_bPrintParam_Factored_AASHTO[i] = FALSE;  
	//}}AFX_DATA_INIT

	m_nAnnex  = 0;
	m_nCurrentDlg = 0;
	m_aDgnCode.RemoveAll();
	m_mapChildDlg.RemoveAll();
	m_mapChildDlg.InitHashTable(6);

//*^^* 여기추가 : PSC Design Parameters // 참고. wg_cmd \ ETC_DESIGN_SPECT.cpp
//#define M_DEFINE_PSC_DGNPARAMETERS(KEY, CODE, DLG) m_aKey.Add(KEY); m_aDgnCode.Add(CODE); m_mapChildDlg.SetAt(KEY, DLG)
	//M_DEFINE_PSC_DGNPARAMETERS(0, EUROCODE2_2_05_PSC, new CDgnCRCDgnParameter_EC2PSC(pParamData));
//#undef M_DEFINE_PSC_DGNPARAMETERS
}

CDgnCRCDgnParameterDlg::~CDgnCRCDgnParameterDlg()
{
 	CMyChildDialog* dlg;
	int key;

	if(m_mapChildDlg.Lookup(m_nCurrentDlg, dlg))
		if(dlg->IsCreated())   dlg->End();

	POSITION pos = m_mapChildDlg.GetStartPosition();
	while(pos!=NULL)
	{
		m_mapChildDlg.GetNextAssoc(pos,key,dlg);
		delete dlg;
	}
	m_mapChildDlg.RemoveAll();
}

void CDgnCRCDgnParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConMatDlg)
	DDX_Control(pDX, IDC_CRC_DGN_CODE,  m_DesignCode);
	DDX_Control(pDX, IDC_CRC_DGN_ANNEX, m_DesignAnnex);
	DDX_Control(pDX, IDC_CRC_DGN_ANNEX_AASHTO_CMB, m_DesignAmendment);
	DDX_Radio(pDX, IDC_CRC_DESIGN_PARAMETER2, m_iDimType);
	DDX_Radio(pDX, IDC_CRC_DESIGN_PARAMETER6, m_iBrgType);
	DDX_Check(pDX, IDC_CRC_DESIGN_PARAMETER8, m_bCmpWeb);
	DDX_Radio(pDX, IDC_CRC_DESIGN_PARAMETER25, m_iBrgDir);
	DDX_Radio(pDX, IDC_CRC_DESIGN_PARAMETER10, m_iCrackCode);
	DDX_Radio(pDX, IDC_CRC_DESIGN_PARAMETER14, m_iTCrackWidth);
	DDX_Radio(pDX, IDC_CRC_DESIGN_PARAMETER18, m_iBCrackWidth);
	DDX_Radio(pDX, IDC_CRC_DESIGN_PARAMETER22, m_iPsReduFactor);
	
	DDX_Radio(pDX, IDC_KSCE_DESIGN_PARAMETER2, m_iDimTypeKsce);
	DDX_Radio(pDX, IDC_KSCE_DESIGN_PARAMETER6, m_iBrgTypeKsce);
	DDX_Radio(pDX, IDC_KSCE_DESIGN_PARAMETER10, m_iTCrackWidthKsce);
	DDX_Radio(pDX, IDC_KSCE_DESIGN_PARAMETER15, m_iBCrackWidthKsce);
	DDX_Radio(pDX, IDC_KSCE_DESIGN_PARAMETER21, m_ifpsMethod);
	DDX_Radio(pDX, IDC_KSCE_DESIGN_PARAMETER25, m_iTndnKind);
	DDX_Check(pDX, IDC_KSCE_DESIGN_PARAMETER23, m_bRefRebar);
		
	DDX_Radio(pDX, IDC_JTG_DESIGN_PARAMETER2, m_iDimTypeJTG);
	DDX_Radio(pDX, IDC_JTG_DESIGN_PARAMETER6, m_iBrgTypeJTG);
	DDX_Radio(pDX, IDC_JTG_DESIGN_PARAMETER10, m_iBrgGradeJTG);
	DDX_Radio(pDX, IDC_JTG_DESIGN_PARAMETER14, m_iCastTypeJTG);
	DDX_Radio(pDX, IDC_JTG_DESIGN_PARAMETER18, m_iSBarCrackWidthJTG);
	DDX_Radio(pDX, IDC_JTG_DESIGN_PARAMETER21, m_iWSCrackWidthJTG);

	DDX_Radio(pDX, IDC_TB_DESIGN_PARAMETER2, m_iDimTypeTB);
	DDX_Radio(pDX, IDC_TB_DESIGN_PARAMETER6, m_iMemTypeTB);
	DDX_Radio(pDX, IDC_TB_DESIGN_PARAMETER9, m_iCastTypeTB);

	
	DDX_Radio(pDX, IDC_AASHTO_DESIGN_PARAMETER2, m_iTndnTypeAASHTO);
	DDX_Radio(pDX, IDC_AASHTO_DESIGN_PARAMETER6, m_iBrgTypeAASHTO);
	DDX_Radio(pDX, IDC_AASHTO_DESIGN_PARAMETER9, m_iConstructionTypeAASHTO);
	DDX_Radio(pDX, IDC_AASHTO_DESIGN_PARAMETER12, m_iCorrosiveTypeAASHTO);
	DDX_Radio(pDX, IDC_AASHTO_DESIGN_PARAMETER15, m_iFlexureCalcTypeAASHTO);
	DDX_Radio(pDX, IDC_AASHTO_DESIGN_PARAMETER18, m_iExposureTypeAASHTO);	
	DDX_Control(pDX, IDC_AASHTO_DESIGN_PARAMETER21, m_dExposureFactor);

	// Place-Holder
	DDX_Control(pDX, IDC_EC2PSC_PLACEHOLDER0, m_wndPlaceHolder);


	//}}AFX_DATA_MAP

	CArray<UINT,UINT> IDPSC0; IDPSC0(0)IDPSC0(1)IDPSC0(2)IDPSC0(3)IDPSC0(4)IDPSC0(5)IDPSC0(6)IDPSC0(7)IDPSC0(8)IDPSC0(9)IDPSC0(10)\
														IDPSC0(11)IDPSC0(12)IDPSC0(13)IDPSC0(14)IDPSC0(15)IDPSC0(16)IDPSC0(17)IDPSC0(18)IDPSC0(19)IDPSC0(20)\
														IDPSC0(21)IDPSC0(22)IDPSC0(23)IDPSC0(24)IDPSC0(25)IDPSC0(26);
	CArray<UINT,UINT> IDPSC1; IDPSC1(0)IDPSC1(1)IDPSC1(2)IDPSC1(3)IDPSC1(4)IDPSC1(5)IDPSC1(6)IDPSC1(7)IDPSC1(8)IDPSC1(9)IDPSC1(10)\
														IDPSC1(11)IDPSC1(12)IDPSC1(13)IDPSC1(14)IDPSC1(15)IDPSC1(16)IDPSC1(17)IDPSC1(18)IDPSC1(19)IDPSC1(20)\
														IDPSC1(21)IDPSC1(22)IDPSC1(23)IDPSC1(24)IDPSC1(25)IDPSC1(26)IDPSC1(27)IDPSC1(28)IDPSC1(29)IDPSC1(30)\
														IDPSC1(31)IDPSC1(32)IDPSC1(33)IDPSC1(34)IDPSC1(35)IDPSC1(36)IDPSC1(37)IDPSC1(38)IDPSC1(39)IDPSC1(40)\
														IDPSC1(41)IDPSC1(42);
	CArray<UINT,UINT> IDPSC2; IDPSC2(0)IDPSC2(1)IDPSC2(2)IDPSC2(3)IDPSC2(4)IDPSC2(5)IDPSC2(6)IDPSC2(7)IDPSC2(8)IDPSC2(9)IDPSC2(10)\
														IDPSC2(11)IDPSC2(12)IDPSC2(13)IDPSC2(14)IDPSC2(15)IDPSC2(16)IDPSC2(17)IDPSC2(18)IDPSC2(19)IDPSC2(20)\
														IDPSC2(21)IDPSC2(22)IDPSC2(23)IDPSC2(24)IDPSC2(25)IDPSC2(26)IDPSC2(27);
	CArray<UINT,UINT> IDPSC3; IDPSC3(0)IDPSC3(1)IDPSC3(2)IDPSC3(3)IDPSC3(4)IDPSC3(5)IDPSC3(6)IDPSC3(7)IDPSC3(8)IDPSC3(9)IDPSC3(10)\
														IDPSC3(11)IDPSC3(12)IDPSC3(13)IDPSC3(14);
	CArray<UINT,UINT> IDPSC4; IDPSC4(0)IDPSC4(1)IDPSC4(2)IDPSC4(3)IDPSC4(4)IDPSC4(5)IDPSC4(6)IDPSC4(7)IDPSC4(8)IDPSC4(9)IDPSC4(10)\
														IDPSC4(11)IDPSC4(12)IDPSC4(13)IDPSC4(14)IDPSC4(15)IDPSC4(16)IDPSC4(17)IDPSC4(18)IDPSC4(19)IDPSC4(20)\
														IDPSC4(21)IDPSC4(22);
	CArray<UINT,UINT> IDPSC5; IDPSC5(0)IDPSC5(1)IDPSC5(2)IDPSC5(3)IDPSC5(4)IDPSC5(5)IDPSC5(6)IDPSC5(7)IDPSC5(8)IDPSC5(9)IDPSC5(10)\
														IDPSC5(11)IDPSC5(12);//IDPSC5(13)IDPSC5(14);
	// Add by sshan. MNET:2728. TB 10002.3-2005 ('20070411)
	CArray<UINT,UINT> IDPSC6; IDPSC6(0)IDPSC6(1)IDPSC6(2)IDPSC6(3)IDPSC6(4)IDPSC6(5)IDPSC6(6)IDPSC6(7)IDPSC6(8)IDPSC6(9)IDPSC6(10)\
														IDPSC6(11);
	CArray<UINT,UINT> IDPSC7; IDPSC7(0)IDPSC7(1)IDPSC7(2)IDPSC7(3)IDPSC7(4)IDPSC7(5)IDPSC7(6)IDPSC7(7)IDPSC7(8)IDPSC7(9)IDPSC7(10)\
														IDPSC7(11)IDPSC7(12)IDPSC7(13)IDPSC7(14)IDPSC7(15)IDPSC7(16)IDPSC7(17)IDPSC7(18)IDPSC7(19)IDPSC7(20)\
														IDPSC7(21)IDPSC7(22)IDPSC7(23)IDPSC7(24)IDPSC7(25)IDPSC7(26)IDPSC7(27)IDPSC7(28);

 
	CArray<UINT,UINT> IDPSC8;	IDPSC8(0)IDPSC8(1)IDPSC8(2)IDPSC8(3)IDPSC8(4)IDPSC8(5)IDPSC8(6)IDPSC8(7)IDPSC8(8)IDPSC8(9)IDPSC8(10)\
								IDPSC8(11)IDPSC8(12)IDPSC8(13)IDPSC8(14)IDPSC8(15)IDPSC8(16)IDPSC8(17)IDPSC8(18)IDPSC8(19)IDPSC8(20)\
								IDPSC8(21);
	CArray<UINT,UINT> IDPSC9;	IDPSC9(0)IDPSC9(1)IDPSC9(2)IDPSC9(3)IDPSC9(4)IDPSC9(5)IDPSC9(6)IDPSC9(7)IDPSC9(8)IDPSC9(9)IDPSC9(10)\
								IDPSC9(11)IDPSC9(12)IDPSC9(13)IDPSC9(14);
	CArray<UINT,UINT> IDPSC10;  IDPSC10(0);
	CArray<UINT,UINT> IDPSC11;  IDPSC11(0);
	CArray<UINT,UINT> IDPSC12;  IDPSC12(0);
	CArray<UINT,UINT> IDPSC13;  IDPSC13(0);
	CArray<UINT,UINT> IDPSC14;  IDPSC14(0);
	CArray<UINT,UINT> IDPSC15;  IDPSC15(0);
	CArray<UINT,UINT> IDPSC16;  IDPSC16(0);
	CArray<UINT,UINT> IDPSC17;  IDPSC17(0);
	CArray<UINT,UINT> IDPSC18;  IDPSC18(0);
	CArray<UINT,UINT> IDPSC19;  IDPSC19(0);
	CArray<UINT, UINT> IDPSC20; IDPSC20(0);

	
	int i=0;
	for(i=0; i<iDgn_PscID0; i++)	m_iID0[i] = IDPSC0[i];
	for(i=0; i<iDgn_PscID1; i++)	m_iID1[i] = IDPSC1[i];
	for(i=0; i<iDgn_PscID2; i++)	m_iID2[i] = IDPSC2[i];
	for(i=0; i<iDgn_PscID3; i++)	m_iID3[i] = IDPSC3[i];
	for(i=0; i<iDgn_PscID4; i++)	m_iID4[i] = IDPSC4[i];
	for(i=0; i<iDgn_PscID5; i++)	m_iID5[i] = IDPSC5[i];
	for(i=0; i<iDgn_PscID6; i++)	m_iID6[i] = IDPSC6[i];
	for(i=0; i<iDgn_PscID7; i++)	m_iID7[i] = IDPSC7[i];
	for(i=0; i<iDgn_PscID8; i++)	m_iID8[i] = IDPSC8[i];
	for(i=0; i<iDgn_PscID9; i++)	m_iID9[i] = IDPSC9[i];
	for(i=0; i<iDgn_PscID10;i++)  m_iID10[i]= IDPSC10[i];
	for(i=0; i<iDgn_PscID11;i++)  m_iID11[i]= IDPSC11[i];
	for(i=0; i<iDgn_PscID12;i++)  m_iID12[i]= IDPSC12[i];
	for(i=0; i<iDgn_PscID13;i++)  m_iID13[i]= IDPSC13[i];
	for(i=0; i<iDgn_PscID14;i++)  m_iID14[i]= IDPSC14[i];
	for(i=0; i<iDgn_PscID15;i++)  m_iID15[i]= IDPSC15[i];
	for(i=0; i<iDgn_PscID16;i++)  m_iID16[i]= IDPSC16[i];
	for(i=0; i<iDgn_PscID17;i++)  m_iID17[i]= IDPSC17[i];
	for(i=0; i<iDgn_PscID18;i++)  m_iID18[i]= IDPSC18[i];
	for(i=0; i<iDgn_PscID19;i++)  m_iID19[i]= IDPSC19[i];
	for(i=0; i<iDgn_PscID20;i++)  m_iID20[i]= IDPSC20[i];


	for(i=0; i<iIDPRINT0; i++) DDX_Check(pDX, IDPSC1[i+iPOS0], m_bDesignPrint[i]);
	for(i=0; i<iIDPRINT1; i++) DDX_Check(pDX, IDPSC1[i+iPOS1], m_bStructPrint[i]);
	for(i=0; i<iIDPRINT2; i++) DDX_Check(pDX, IDPSC1[i+iPOS2], m_bMomentPrint[i]);
	for(i=0; i<iIDPRINT3; i++) DDX_Check(pDX, IDPSC1[i+iPOS3], m_bShearPrint[i]);
	for(i=0; i<iIDPRINT4; i++) DDX_Check(pDX, IDPSC1[i+iPOS4], m_bTorsionPrint[i]);
	for(i=0; i<iIDPRINT5; i++) DDX_Check(pDX, IDPSC1[i+iPOS5], m_bEtcPrint[i]); 
	for(i=0; i<iIDPRINT6; i++) DDX_Check(pDX, IDPSC1[i+iPOS8], m_bBckPrint[i]); 
	
	for(i=0; i<iIDPRINTSERV; i++) DDX_Check(pDX, IDPSC3[i+iPOS6], m_bServPrint[i]); 
	for(i=0; i<iIDPRINTULTI; i++) DDX_Check(pDX, IDPSC3[i+iPOS7], m_bUltiPrint[i]);   

	for(i=0; i<iIDPRINTSERV_JTG; i++) DDX_Check(pDX, IDPSC5[i+iPOS9], m_bServPrintJTG[i]); 
	for(i=0; i<iIDPRINTULTI_JTG; i++) DDX_Check(pDX, IDPSC5[i+iPOS10], m_bUltiPrintJTG[i]);   

	for(i=0; i<iIDPRINTBEAM_TB; i++) DDX_Check(pDX, IDPSC7[i+iPOS11], m_bBeamPrintTB[i]); 
	for(i=0; i<iIDPRINTCOLU_TB; i++) DDX_Check(pDX, IDPSC7[i+iPOS12], m_bColuPrintTB[i]);   

	
	for(i=0; i<iOUTPUTPARAM_SERVICE_AASHTO_COUNT; i++) DDX_Check(pDX, IDPSC9[i+iOUTPUTPARAM_SERVICE_AASHTO_POS], m_bPrintParam_Service_AASHTO[i]); 
	for(i=0; i<iOUTPUTPARAM_FACTORED_AASHTO_COUNT; i++) DDX_Check(pDX, IDPSC9[i+iOUTPUTPARAM_FACTORED_AASHTO_POS], m_bPrintParam_Factored_AASHTO[i]);
	// Eurocode2-2:05, PSC의 경우 해당 동작 필요없음.
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnCRCDgnParameterDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnConMatDlg)
	ON_CBN_SELCHANGE(IDC_CRC_DGN_CODE,  OnSelchangeDgnCtrldataDesigncode)
	ON_CBN_SELCHANGE(IDC_CRC_DGN_ANNEX, OnSelchangeDgnCtrldataDesignAnnex)
	ON_CBN_SELCHANGE(IDC_CRC_DGN_ANNEX_AASHTO_CMB, OnSelchangeDgnCtrldataDesignAmendment)

	ON_BN_CLICKED(IDC_CRC_PRINT_PARAMETER41, OnClickSelectAll)
	ON_BN_CLICKED(IDC_CRC_PRINT_PARAMETER42, OnClickUnSelectAll)
	ON_BN_CLICKED(IDC_CRC_DESIGN_PARAMETER2, OnClickOption)
	ON_BN_CLICKED(IDC_CRC_DESIGN_PARAMETER3, OnClickOption)
	ON_BN_CLICKED(IDC_CRC_DESIGN_PARAMETER4, OnClickOption)
	ON_BN_CLICKED(IDC_CRC_DESIGN_PARAMETER6, OnClickOption)
	ON_BN_CLICKED(IDC_CRC_DESIGN_PARAMETER7, OnClickOption)
	ON_BN_CLICKED(IDC_CRC_DESIGN_PARAMETER8, OnClickOption)
	ON_BN_CLICKED(IDC_CRC_DESIGN_PARAMETER25, OnClickOption)
	ON_BN_CLICKED(IDC_CRC_DESIGN_PARAMETER26, OnClickOption)
	
	ON_BN_CLICKED(IDC_KSCE_DESIGN_PARAMETER2, OnClickOption)
	ON_BN_CLICKED(IDC_KSCE_DESIGN_PARAMETER6, OnClickOption)
	ON_BN_CLICKED(IDC_KSCE_DESIGN_PARAMETER7, OnClickOption)
	ON_BN_CLICKED(IDC_KSCE_DESIGN_PARAMETER21, OnClickOption)  
	ON_BN_CLICKED(IDC_KSCE_DESIGN_PARAMETER22, OnClickOption)  
	ON_BN_CLICKED(IDC_KSCE_PRINT_PARAMETER13, OnClickSelectAllKsce)
	ON_BN_CLICKED(IDC_KSCE_PRINT_PARAMETER14, OnClickUnSelectAllKsce)

	ON_BN_CLICKED(IDC_JTG_DESIGN_PARAMETER2, OnClickOption)
	ON_BN_CLICKED(IDC_JTG_DESIGN_PARAMETER3, OnClickOption)
	ON_BN_CLICKED(IDC_JTG_DESIGN_PARAMETER4, OnClickOption)
	ON_BN_CLICKED(IDC_JTG_DESIGN_PARAMETER6, OnClickOption)
	ON_BN_CLICKED(IDC_JTG_DESIGN_PARAMETER7, OnClickOption)
	ON_BN_CLICKED(IDC_JTG_DESIGN_PARAMETER8, OnClickOption)
	ON_BN_CLICKED(IDC_JTG_PRINT_PARAMETER11, OnClickSelectAllJtg)
	ON_BN_CLICKED(IDC_JTG_PRINT_PARAMETER12, OnClickUnSelectAllJtg)

	ON_BN_CLICKED(IDC_TB_DESIGN_PARAMETER6, OnClickOption)
	ON_BN_CLICKED(IDC_TB_DESIGN_PARAMETER7, OnClickOption)
	ON_BN_CLICKED(IDC_TB_DESIGN_PARAMETER8, OnClickOption)
	ON_BN_CLICKED(IDC_TB_PRINT_PARAMETER27, OnClickSelectAllTB)
	ON_BN_CLICKED(IDC_TB_PRINT_PARAMETER28, OnClickUnSelectAllTB)

	ON_BN_CLICKED(IDC_AASHTO_DESIGN_PARAMETER6, OnClickOption)
	ON_BN_CLICKED(IDC_AASHTO_DESIGN_PARAMETER7, OnClickOption)
	ON_BN_CLICKED(IDC_AASHTO_DESIGN_PARAMETER18, OnClickOption)
	ON_BN_CLICKED(IDC_AASHTO_DESIGN_PARAMETER19, OnClickOption)
	ON_BN_CLICKED(IDC_AASHTO_DESIGN_PARAMETER20, OnClickOption)
	ON_BN_CLICKED(IDC_AASHTO_PRINT_PARAMETER13, OnClickSelectAllAASHTO)
	ON_BN_CLICKED(IDC_AASHTO_PRINT_PARAMETER14, OnClickUnSelectAllAASHTO)

	ON_BN_CLICKED(IDC_CRC_DGN_CHK_SPECIALLOAD, OnClickSpecialLoad)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConMatDlg message handlers

BOOL CDgnCRCDgnParameterDlg::OnInitDialog() 
{
	MInitCombo initCombo;

	CDialogMove::OnInitDialog();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	GetDlgItem(IDC_CRC_DGN_CHK_SPECIALLOAD)->ShowWindow(SW_HIDE);

	Initial_AnnexAASHTO_Ctrl();

	m_DesignCode.ResetContent();
	CStringArray arPscCodeNa;
	CDBLib::GetPscCodeNameList(arPscCodeNa);
	int i=0;
	for(i=0; i<arPscCodeNa.GetSize(); i++)	m_DesignCode.AddString(arPscCodeNa[i]);
	//
	m_DesignAnnex.ResetContent();
    auto vString = CNationalAnnexTool::GetRcNationalAnnex();
    for ( int i = 0; i<vString.size(); ++i )
    {
        CDlgUtil::CobxAddItem(m_DesignAnnex, vString[i], CNationalAnnexTool::ConvertNationalAnnex(vString[i]));
    }
	//

	T_PSCD_D PscdD; PscdD.Initialize();

	if (pDoc->m_pAttrCtrl->GetPscd(PscdD))
	{
		Inittal_Amendment(PscdD.iDgnCode, PscdD.nNationalAnnex);
	}
	else
	{
		pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
		Inittal_Amendment(PscdD.iDgnCode, Amend_Recommended);
	}	

	m_iDimType = 0;
	m_iBrgType = 0;
	m_bCmpWeb = FALSE;
	m_iBrgDir = 0;
	m_iCrackCode = 0;
	m_iTCrackWidth = 0;
	m_iBCrackWidth = 0;
	m_iPsReduFactor = 0;
	m_iDimTypeKsce = 0;
	m_iBrgTypeKsce = 0;
	m_iTCrackWidthKsce = 0;
	m_iBCrackWidthKsce = 0;
	m_ifpsMethod = 0;
	m_iTndnKind = 0;
	m_bRefRebar = TRUE;

	m_iDimTypeJTG = 0;
	m_iBrgTypeJTG = 0;
	m_iBrgGradeJTG = 0;
	m_iCastTypeJTG = 0;
	m_iSBarCrackWidthJTG = 0;
	m_iWSCrackWidthJTG = 0;

	m_iTndnTypeAASHTO = 0;
	m_iBrgTypeAASHTO = 0;
	m_iConstructionTypeAASHTO = 0;
	m_iCorrosiveTypeAASHTO = 0;
	m_iExposureTypeAASHTO = 0;
	m_iFlexureCalcTypeAASHTO = 0;

	m_iDimTypeTB = 0;
	m_iMemTypeTB = 0;
	m_iCastTypeTB = 0;

	m_Data.Initialize();
	//m_Data.Initialize();

	//BOOL bExist = FALSE;
	//bExist = pDoc->m_pAttrCtrl->ExistPscd()? TRUE : FALSE ;

	if(!pDoc->m_pAttrCtrl->GetDgnPscd(m_Data)) 
		m_Data.Initialize();
	pParamData = &m_Data;

#define M_DEFINE_PSC_DGNPARAMETERS(KEY, CODE, DLG) m_aKey.Add(KEY); m_aDgnCode.Add(CODE); m_mapChildDlg.SetAt(KEY, DLG)
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::EC,     EUROCODE2_2_05_PSC,     new CDgnCRCDgnParameter_EC2PSC(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::EC,     IRC_112_2011_PSC,       new CDgnCRCDgnParameter_EC2PSC(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::EC,     IRC_112_2020_PSC,       new CDgnCRCDgnParameter_EC2PSC(pParamData));
	//M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::BS,		TMH07_3_1989,			new CDgnCRCDgnParameter_BS(pParamData));//Added by Rahul 
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::TMH7,		TMH07_3_1989,		new CDgnCRCDgnParameter_TMH7(pParamData));//Added by Rahul 
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::BS,     BS5400_90_PSC,          new CDgnCRCDgnParameter_BS(pParamData));
    M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::BS,     BD_44_15_PSC,           new CDgnCRCDgnParameter_BS(pParamData));
    M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::BS,     CS_455_PSC,             new CDgnCRCDgnParameter_BS(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::CSA,    CSA_S6_10,              new CDgnCRCDgnParameter_CSA(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::CSA,    CSA_S6_14,              new CDgnCRCDgnParameter_CSA(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::CSA19,  CSA_S6_19,              new CDgnCRCDgnParameter_CSA_19(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::AASHTO, AASHTO_LRFD12,          new CDgnCRCDgnParameter_AASHTO12(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::AASHTO, AASHTO_LRFD14,          new CDgnCRCDgnParameter_AASHTO12(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::AASHTO, AASHTO_LRFD16,          new CDgnCRCDgnParameter_AASHTO12(pParamData));
    M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::AASHTO, AASHTO_LRFD17,          new CDgnCRCDgnParameter_AASHTO12(pParamData));
    M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::AASHTO, AASHTO_LRFD20,          new CDgnCRCDgnParameter_AASHTO12(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::AASHTO, AASHTO_LRFD24,			new CDgnCRCDgnParameter_AASHTO12(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::KSCE,   KDS_24_14_21_2022_PSC,  new CDgnCRCDgnParameter_LSDPSC(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::KSCE,   KSCE_LSD15_PSC,         new CDgnCRCDgnParameter_LSDPSC(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::SNIP,   SNiP_20503_84_PSC,      new CDgnCRCDgnParameter_RUS(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::SNIP,   SNiP_20503_84_PSC_MKS,  new CDgnCRCDgnParameter_RUS(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::SP,     SP_35_13330_11_PSC,     new CDgnCRCDgnParameter_RUS(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::SP,     SP_35_13330_11_PSC_MKS, new CDgnCRCDgnParameter_RUS(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::AS,     AS_5100_5_17_PSC,       new CDgnCRCDgnParameter_AS5100(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::IRS,    IRS_PSC,		        new CDgnCRCDgnParameter_IRS(pParamData));
	M_DEFINE_PSC_DGNPARAMETERS(CHILDDLGKEY::ABNT,     ABNT_NBR_6118_2023_PSC, new CDgnCRCDgnParameter_ABNT(pParamData));
#undef M_DEFINE_PSC_DGNPARAMETERS
	
	//
	POSITION pos = m_mapChildDlg.GetStartPosition();
	CMyChildDialog* dlg;
	int key;

	while(pos!=NULL)
	{
		m_mapChildDlg.GetNextAssoc(pos, key, dlg);
	}

	//UpdateCurDlg(m_Data.iDgnCode);
	UpDateCurDlg_EC2(pParamData->iDgnCode);
	UpDateCurDlg_BS();
	UpDateCurDlg_CSA();
	UpDateCurDlg_CSA19();
	UpDateCurDlg_AASHTO12();
	UpDateCurDlg_LSD15();
	UpdateCurDlg_SNiP();
	UpdateCurDlg_SP();
	UpDateCurDlg_AS5100();
	UpDateCurDlg_IRS();
	UpDateCurDlg_TMH7();
	UpDateCurDlg_ABNT();

	if(pDoc->m_pAttrCtrl->ExistPscd())	Initial_ExistData();
	else								Initial_Data();

	// Initialize Grid, Unit, Matl Code.
	//EnableCtrls();
	UpdateData(FALSE);

	//++++++++++++++++++++++++++
	// Text File for Testing.
	CDgnPscCommon PscComm;
	PscComm.PrtTestForSect();
	//++++++++++++++++++++++++++

	return TRUE;
}

void CDgnCRCDgnParameterDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CDgnCRCDgnParameterDlg::OnOK() 
{
	UpdateData(TRUE);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	T_PSCD_D PscdD; PscdD.Initialize();
	pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	
	CString strCodeNa=_T("");
	int iIndex = m_DesignCode.GetCurSel();
	m_DesignCode.GetLBText(iIndex, strCodeNa);
	PscdD.iDgnCode = CDBLib::GetPscCodeNo(strCodeNa);

	if(PscdD.iDgnCode==JSCE02)
	{
		PscdD.iDimType       = m_iDimType;
		PscdD.iBrgType       = m_iBrgType;
		PscdD.bCmpWeb				 = m_bCmpWeb;
		PscdD.iBrgDir				 = m_iBrgDir;
		PscdD.iCrackCode     = m_iCrackCode;
		PscdD.iCrackWidth[0] = m_iTCrackWidth;
		PscdD.iCrackWidth[1] = m_iBCrackWidth;
		PscdD.iPsReduFactor  = m_iPsReduFactor;
		int i=0;
		for(i=0; i<iIDPRINT0; i++) PscdD.bDgnPrint[i]     = m_bDesignPrint[i];
		for(i=0; i<iIDPRINT1; i++) PscdD.bStructPrint[i]  = m_bStructPrint[i];
		for(i=0; i<iIDPRINT2; i++) PscdD.bMomPrint[i]     = m_bMomentPrint[i];
		for(i=0; i<iIDPRINT3; i++) PscdD.bShrPrint[i]     = m_bShearPrint[i];
		for(i=0; i<iIDPRINT4; i++) PscdD.bTorPrint[i]     = m_bTorsionPrint[i];
		for(i=0; i<iIDPRINT5; i++) PscdD.bEtcPrint[i]     = m_bEtcPrint[i];
		for(i=0; i<iIDPRINT6; i++) PscdD.bBckPrint[i]     = m_bBckPrint[i];
	}
	else if(PscdD.iDgnCode==KSCE_USD03 || PscdD.iDgnCode==KSCE_USD05 || PscdD.iDgnCode==KSCE_USD10 || 
					PscdD.iDgnCode==KSCE_RAIL_USD04 || PscdD.iDgnCode==KSCE_RAIL_USD11)
	{
		PscdD.iDimType       = m_iDimTypeKsce;
		PscdD.iBrgType       = 1; //m_iBrgTypeKsce;
		PscdD.iCrackWidth[0] = m_iTCrackWidthKsce;
		PscdD.iCrackWidth[1] = m_iBCrackWidthKsce;
		PscdD.ifpsMethod     = m_ifpsMethod;
		PscdD.iTndnKind      = m_iTndnKind;
		PscdD.bRefRebar      = m_bRefRebar;
		int i=0;
		
		for(i=0; i<iIDPRINTSERV; i++) PscdD.bDgnPrint[i]    = m_bServPrint[i];   //bDgnPrint[5]-> 0:응력해석결과, 1:시공단계별 응력도, 2:사용하중시 하중조합별 응력도, 3:응력검토 집계 결과 4:인장철근량계산.
		for(i=0; i<iIDPRINTULTI; i++) PscdD.bStructPrint[i] = m_bUltiPrint[i];   //bStructPrint[5]-> 0:하중조합별 부재력도, 1:하중조합별 부재력 집계, 2:휨강도 검토, 3:전단강도 검토, 4:비틀림강도 검토
	}
	else if(PscdD.iDgnCode==JTG_D62_04 || m_iDgnCode==CJJ_11_2011)
	{
		PscdD.iDimType       = m_iDimTypeJTG;
		PscdD.iBrgType       = m_iBrgTypeJTG;
		PscdD.iSafeType      = m_iBrgGradeJTG; //도로교함안전등급 (0=1급,1=2급,2=3급)
		PscdD.iCastType      = m_iCastTypeJTG; //타설타입(0=Precast, 1=Cast-in-place) 
		PscdD.iCrackWidth[0] = m_iSBarCrackWidthJTG;
		PscdD.iCrackWidth[1] = m_iWSCrackWidthJTG;
		int i=0;
		// Modify by GAY. MNET:2444. ('06.09.08). Save Data to T_PSCD_D>bMomPrint and T_PSCD_D>bShrPrint.
		/*
		for(i=0; i<iIDPRINTSERV_JTG; i++) PscdD.bDgnPrint[i]    = m_bServPrintJTG[i];   //bDgnPrint[5]-> 0:응력해석결과, 1:시공단계별 응력도, 2:사용하중시 하중조합별 응력도, 3:응력검토 집계 결과 4:인장철근량계산.
		for(i=0; i<iIDPRINTULTI_JTG; i++) PscdD.bStructPrint[i] = m_bUltiPrintJTG[i];   //bStructPrint[5]-> 0:하중조합별 부재력도, 1:하중조합별 부재력 집계, 2:휨강도 검토, 3:전단강도 검토, 4:비틀림강도 검토
		*/
		for(i=0; i<iIDPRINTSERV_JTG; i++) PscdD.bMomPrint[i] = m_bServPrintJTG[i];   //Save at bMomPrint.
		for(i=0; i<iIDPRINTULTI_JTG; i++) PscdD.bShrPrint[i] = m_bUltiPrintJTG[i];   //Save at bShrPrint.
	}
	else if(PscdD.iDgnCode==TB_10002_3_05)// Add by sshan. MNET:2728. TB 10002.3-2005 ('20070411)
	{
		PscdD.iDimType       = m_iDimTypeTB;
		PscdD.iBrgType       = m_iMemTypeTB;
		PscdD.iCastType      = m_iCastTypeTB;
		
		int i=0;    
		for(i=0; i<iIDPRINTBEAM_TB; i++) PscdD.bBeamPrintCH[i] = m_bBeamPrintTB[i];   //Save at bBeamPrintCH.
		for(i=0; i<iIDPRINTCOLU_TB; i++) PscdD.bColumnPrintCH[i] = m_bColuPrintTB[i];   //Save at bColumnPrintCH.
	}
	else if(PscdD.iDgnCode==AASHTO_LRFD08)
	{	  
		// Code. Nishant.
		Dlg2Data();
		PscdD.iTndnKind           = m_iTndnTypeAASHTO;
		PscdD.iBrgType            = m_iBrgTypeAASHTO;
		PscdD.iConstructionType   = m_iConstructionTypeAASHTO;
		PscdD.iCorrosiveCondition = m_iCorrosiveTypeAASHTO;
		PscdD.iExposureType       = m_iExposureTypeAASHTO;
		PscdD.dExposureFactor     = m_Data.dExposureFactor;    
		PscdD.ifpsMethod          = m_iFlexureCalcTypeAASHTO;

		int i=0;
		for(i=0; i<iOUTPUTPARAM_SERVICE_AASHTO_COUNT; i++) PscdD.bServPrint4AASHTO[i]  = m_bPrintParam_Service_AASHTO[i];
		for(i=0; i<iOUTPUTPARAM_FACTORED_AASHTO_COUNT; i++) PscdD.bStrePrint4AASHTO[i] = m_bPrintParam_Factored_AASHTO[i];
	}
	else if(PscdD.iDgnCode==EUROCODE2_2_05_PSC)
	{
		// SET CurrentDlg Index
	  m_nCurrentDlg = CHILDDLGKEY::EC;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_EC2PSC *pUser = (CDgnCRCDgnParameter_EC2PSC*) dlg;
		PscdD = *pUser->m_pParamData;

		// 해당 입력창에 대한 Data를 저장함. Only Eurocode2-2:05, PSC
		// Place-Holder로 Data를 전달할 당시의 Data가 save가 되기 때문에..
		iIndex = 0; iIndex = m_DesignCode.GetCurSel();
		m_DesignCode.GetLBText(iIndex, strCodeNa);
		PscdD.iDgnCode        = CDBLib::GetPscCodeNo(strCodeNa);
		PscdD.nNationalAnnex  = CDlgUtil::CobxGetCurSelItemData(m_DesignAnnex, m_DesignAnnex.GetCurSel());
	}
	else if (PscdD.iDgnCode == ABNT_NBR_6118_2023_PSC)
	{
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::ABNT;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_ABNT* pUser = (CDgnCRCDgnParameter_ABNT*)dlg;
		PscdD = *pUser->m_pParamData;

		// 해당 입력창에 대한 Data를 저장함. Only Eurocode2-2:05, PSC
		// Place-Holder로 Data를 전달할 당시의 Data가 save가 되기 때문에..
		iIndex = 0; iIndex = m_DesignCode.GetCurSel();
		m_DesignCode.GetLBText(iIndex, strCodeNa);
		PscdD.iDgnCode = CDBLib::GetPscCodeNo(strCodeNa);
	}
	else if(PscdD.iDgnCode==IRC_112_2011_PSC || PscdD.iDgnCode == IRC_112_2020_PSC)
	{
	  // SET CurrentDlg Index
	  m_nCurrentDlg = CHILDDLGKEY::EC;
	  CMyChildDialog* dlg;
	  m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
	  dlg->Dlg2Data();

	  CDgnCRCDgnParameter_EC2PSC *pUser = (CDgnCRCDgnParameter_EC2PSC*) dlg;
	  PscdD = *pUser->m_pParamData;

	  // 해당 입력창에 대한 Data를 저장함. Only Eurocode2-2:05, PSC
	  // Place-Holder로 Data를 전달할 당시의 Data가 save가 되기 때문에..
	  iIndex = 0; iIndex = m_DesignCode.GetCurSel();
	  m_DesignCode.GetLBText(iIndex, strCodeNa);
	  PscdD.iDgnCode        = CDBLib::GetPscCodeNo(strCodeNa);
	  //PscdD.nNationalAnnex  = CDlgUtil::CobxGetCurSelItemData(m_DesignAnnex, m_DesignAnnex.GetCurSel());
	}
	else if(PscdD.iDgnCode==AS_5100_5_17_PSC)
	{
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::AS;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_AS5100 *pUser = (CDgnCRCDgnParameter_AS5100*) dlg;
		PscdD = *pUser->m_pParamData;

		// 해당 입력창에 대한 Data를 저장함.
		// Place-Holder로 Data를 전달할 당시의 Data가 save가 되기 때문에..
		iIndex = 0; iIndex = m_DesignCode.GetCurSel();
		m_DesignCode.GetLBText(iIndex, strCodeNa);
		PscdD.iDgnCode        = CDBLib::GetPscCodeNo(strCodeNa);
		PscdD.nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_DesignAmendment, m_DesignAmendment.GetCurSel());
	}
	else if(PscdD.iDgnCode==IRS_PSC)
	{
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::IRS;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_IRS *pUser = (CDgnCRCDgnParameter_IRS*) dlg;
		PscdD = *pUser->m_pParamData;

		// 해당 입력창에 대한 Data를 저장함.
		// Place-Holder로 Data를 전달할 당시의 Data가 save가 되기 때문에..
		iIndex = 0; iIndex = m_DesignCode.GetCurSel();
		m_DesignCode.GetLBText(iIndex, strCodeNa);
		PscdD.iDgnCode        = CDBLib::GetPscCodeNo(strCodeNa);
	}
	else if(PscdD.iDgnCode==BS5400_90_PSC||  PscdD.iDgnCode==BD_44_15_PSC || PscdD.iDgnCode==CS_455_PSC )
	{
		// SET CurrentDlg Index
	  m_nCurrentDlg = CHILDDLGKEY::BS;
	  CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_BS *pUser = (CDgnCRCDgnParameter_BS*) dlg;
		PscdD = *pUser->m_pParamData;

		// 해당 입력창에 대한 Data를 저장함. Only Eurocode2-2:05, PSC
		// Place-Holder로 Data를 전달할 당시의 Data가 save가 되기 때문에..
		iIndex = 0; 
		iIndex = m_DesignCode.GetCurSel();
		m_DesignCode.GetLBText(iIndex, strCodeNa);
		PscdD.iDgnCode  = CDBLib::GetPscCodeNo(strCodeNa);
		
	}
	else if ( PscdD.iDgnCode == TMH07_3_1989 )
	{
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::TMH7;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_TMH7* pUser = (CDgnCRCDgnParameter_TMH7*)dlg;
		PscdD = *pUser->m_pParamData;

		if (PscdD.bUserInputData == false)
		{
			PscdD.dmb9 = 0.7;
			PscdD.bIncrevtu = FALSE;
		}

		// 해당 입력창에 대한 Data를 저장함. Only Eurocode2-2:05, PSC
		// Place-Holder로 Data를 전달할 당시의 Data가 save가 되기 때문에..
		iIndex = 0;
		iIndex = m_DesignCode.GetCurSel();
		m_DesignCode.GetLBText(iIndex, strCodeNa);
		PscdD.iDgnCode = CDBLib::GetPscCodeNo(strCodeNa);

		}
	else if(PscdD.iDgnCode == CSA_S6_19)
	{
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::CSA19;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();
		
		CDgnCRCDgnParameter_CSA_19 *pUser = (CDgnCRCDgnParameter_CSA_19*) dlg;
		PscdD = *pUser->m_pParamData;
		
		// 해당 입력창에 대한 Data를 저장함.
		// Place-Holder로 Data를 전달할 당시의 Data가 save가 되기 때문에..
		iIndex = 0; iIndex = m_DesignCode.GetCurSel();
		m_DesignCode.GetLBText(iIndex, strCodeNa);
		PscdD.iDgnCode = CDBLib::GetPscCodeNo(strCodeNa);
	}
	else if(PscdD.iDgnCode == CSA_S6_14 || PscdD.iDgnCode == CSA_S6_10)
	{
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::CSA;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_CSA *pUser = (CDgnCRCDgnParameter_CSA*) dlg;
		PscdD = *pUser->m_pParamData;

		// 해당 입력창에 대한 Data를 저장함.
		// Place-Holder로 Data를 전달할 당시의 Data가 save가 되기 때문에..
		iIndex = 0; iIndex = m_DesignCode.GetCurSel();
		m_DesignCode.GetLBText(iIndex, strCodeNa);
		PscdD.iDgnCode        = CDBLib::GetPscCodeNo(strCodeNa);
		}
	else if(CDBLib::IsPscCodeAASHTOSince12(PscdD.iDgnCode))
	{
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::AASHTO;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();
		
		BOOL bInit = FALSE;
		if (!pDoc->m_pAttrCtrl->GetPscd(PscdD)) bInit = TRUE;

		CDgnCRCDgnParameter_AASHTO12 *pUser = (CDgnCRCDgnParameter_AASHTO12*) dlg;
		PscdD = *pUser->m_pParamData;
		pUser->m_bInitData = bInit;
		
		// 해당 입력창에 대한 Data를 저장함.
		// Place-Holder로 Data를 전달할 당시의 Data가 save가 되기 때문에..
		iIndex = 0; iIndex = m_DesignCode.GetCurSel();
		m_DesignCode.GetLBText(iIndex, strCodeNa);
		PscdD.iDgnCode        = CDBLib::GetPscCodeNo(strCodeNa);

		if (PscdD.iDgnCode == AASHTO_LRFD17)
			PscdD.nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_DesignAmendment, m_DesignAmendment.GetCurSel());
		else
			PscdD.nNationalAnnex = Amend_Recommended; 
	}
    else if ( CDBLib::IsPscCodeLSD_KR(PscdD.iDgnCode) )
	{
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::KSCE;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();
		
		CDgnCRCDgnParameter_LSDPSC *pUser = (CDgnCRCDgnParameter_LSDPSC*) dlg;
		PscdD = *pUser->m_pParamData;
		
		// 해당 입력창에 대한 Data를 저장함.
		// Place-Holder로 Data를 전달할 당시의 Data가 save가 되기 때문에..
		iIndex = 0; iIndex = m_DesignCode.GetCurSel();
		m_DesignCode.GetLBText(iIndex, strCodeNa);
		PscdD.iDgnCode        = CDBLib::GetPscCodeNo(strCodeNa);    
	}
	else if(PscdD.iDgnCode==SNiP_20503_84_PSC || PscdD.iDgnCode==SNiP_20503_84_PSC_MKS)
	{
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::SNIP;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();
		
		CDgnCRCDgnParameter_RUS *pUser = (CDgnCRCDgnParameter_RUS*) dlg;
		PscdD = *pUser->m_pData;
		
		// 해당 입력창에 대한 Data를 저장함.
		// Place-Holder로 Data를 전달할 당시의 Data가 save가 되기 때문에..
		iIndex = m_DesignCode.GetCurSel();
		m_DesignCode.GetLBText(iIndex, strCodeNa);
		PscdD.iDgnCode        = CDBLib::GetPscCodeNo(strCodeNa);    
	}
	else if(PscdD.iDgnCode==SP_35_13330_11_PSC || PscdD.iDgnCode==SP_35_13330_11_PSC_MKS)
	{
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::SP;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();
		
		CDgnCRCDgnParameter_RUS *pUser = (CDgnCRCDgnParameter_RUS*) dlg;
		PscdD = *pUser->m_pData;
		
		// 해당 입력창에 대한 Data를 저장함.
		// Place-Holder로 Data를 전달할 당시의 Data가 save가 되기 때문에..
		iIndex = m_DesignCode.GetCurSel();
		m_DesignCode.GetLBText(iIndex, strCodeNa);
		PscdD.iDgnCode        = CDBLib::GetPscCodeNo(strCodeNa);    
	}
	else  ASSERT(0);

	if(m_iDgnCode==CJJ_11_2011)
	{
		PscdD.bExistSpecialLoad = IsDlgButtonChecked(IDC_CRC_DGN_CHK_SPECIALLOAD);
	}

	BOOL bCheck = pDoc->m_pDataCtrl->AddPscd(PscdD);

	if(bCheck) 
	{
#if defined (_CIVIL)
		AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_PSC_DESIGN_CODE, 0), 0);
#endif
		CDialogMove::OnOK();
	}
}

void CDgnCRCDgnParameterDlg::OnClickSelectAll()
{
	UpdateData(TRUE);

	int i=0;
	for(i=0; i<iIDPRINT0; i++) m_bDesignPrint[i] = 1;
	for(i=0; i<iIDPRINT1; i++) m_bStructPrint[i] = 1;
	for(i=0; i<iIDPRINT2; i++) m_bMomentPrint[i] = 1;
	for(i=0; i<iIDPRINT3; i++) m_bShearPrint[i] = 1;
	for(i=0; i<iIDPRINT4; i++) m_bTorsionPrint[i] = 1;
	for(i=0; i<iIDPRINT5; i++) m_bEtcPrint[i] = 1;
	
	if(m_bCmpWeb)
		for(i=0; i<iIDPRINT6; i++) m_bBckPrint[i] = 1;

	EnableCtrls();

	UpdateData(FALSE);
}

void CDgnCRCDgnParameterDlg::OnClickUnSelectAll()
{
	UpdateData(TRUE);

	int i=0;
	for(i=0; i<iIDPRINT0; i++) m_bDesignPrint[i] = 0;
	for(i=0; i<iIDPRINT1; i++) m_bStructPrint[i] = 0;
	for(i=0; i<iIDPRINT2; i++) m_bMomentPrint[i] = 0;
	for(i=0; i<iIDPRINT3; i++) m_bShearPrint[i] = 0;
	for(i=0; i<iIDPRINT4; i++) m_bTorsionPrint[i] = 0;
	for(i=0; i<iIDPRINT5; i++) m_bEtcPrint[i] = 0;
	for(i=0; i<iIDPRINT6; i++) m_bBckPrint[i] = 0;

	EnableCtrls();

	UpdateData(FALSE);
}

void CDgnCRCDgnParameterDlg::OnClickSelectAllKsce()
{
	UpdateData(TRUE);

	int i=0; 
	// Change by Jaeoh. ('2005.10.12) i=0 -> i=1 임시로.
	for(i=1; i<iIDPRINTSERV; i++)  m_bServPrint[i] = 1; 
	for(i=0; i<iIDPRINTULTI; i++)  m_bUltiPrint[i] = 1; 

	//EnableCtrls();
	if(m_iDgnCode==KSCE_USD10 || m_iDgnCode==KSCE_RAIL_USD11)
		m_bUltiPrint[4] = 0; 

	UpdateData(FALSE);
}

void CDgnCRCDgnParameterDlg::OnClickUnSelectAllKsce()
{
	UpdateData(TRUE);
	int i=0;
	// Change by Jaeoh. ('2005.10.12) i=0 -> i=1 임시로.
	for(i=1; i<iIDPRINTSERV; i++)  m_bServPrint[i] = 0; 
	for(i=0; i<iIDPRINTULTI; i++)  m_bUltiPrint[i] = 0; 
	//EnableCtrls();

	UpdateData(FALSE);
}

void CDgnCRCDgnParameterDlg::OnClickSelectAllJtg()
{
	UpdateData(TRUE);

	int i=0; 
	for(i=0; i<iIDPRINTSERV_JTG; i++)  m_bServPrintJTG[i] = 1; 
	for(i=0; i<iIDPRINTULTI_JTG; i++)  m_bUltiPrintJTG[i] = 1; 

	EnableCtrls();

	UpdateData(FALSE);
}

void CDgnCRCDgnParameterDlg::OnClickUnSelectAllJtg()
{
	UpdateData(TRUE);
	int i=0;
	for(i=0; i<iIDPRINTSERV_JTG; i++)  m_bServPrintJTG[i] = 0; 
	for(i=0; i<iIDPRINTULTI_JTG; i++)  m_bUltiPrintJTG[i] = 0; 

	EnableCtrls();

	UpdateData(FALSE);
}

void CDgnCRCDgnParameterDlg::OnClickSelectAllTB()
{
	UpdateData(TRUE);

	int i=0; 
	for(i=0; i<iIDPRINTBEAM_TB; i++)  m_bBeamPrintTB[i] = 1; 
	for(i=0; i<iIDPRINTCOLU_TB; i++)  m_bColuPrintTB[i] = 1; 

	EnableCtrls();

	UpdateData(FALSE);
}

void CDgnCRCDgnParameterDlg::OnClickUnSelectAllTB()
{
	UpdateData(TRUE);
	int i=0;
	for(i=0; i<iIDPRINTBEAM_TB; i++)  m_bBeamPrintTB[i] = 0; 
	for(i=0; i<iIDPRINTCOLU_TB; i++)  m_bColuPrintTB[i] = 0; 

	EnableCtrls();

	UpdateData(FALSE);
}
void CDgnCRCDgnParameterDlg::OnClickSelectAllAASHTO()
{
	UpdateData(TRUE);
	
	int i=0;
	for(i=0; i<iOUTPUTPARAM_SERVICE_AASHTO_COUNT; i++)  m_bPrintParam_Service_AASHTO[i] = TRUE; 
	for(i=0; i<iOUTPUTPARAM_FACTORED_AASHTO_COUNT; i++) m_bPrintParam_Factored_AASHTO[i] = TRUE; 
	
	EnableCtrls();
	
	UpdateData(FALSE);
}

void CDgnCRCDgnParameterDlg::OnClickUnSelectAllAASHTO()
{
	UpdateData(TRUE);
	
	int i=0;
	for(i=0; i<iOUTPUTPARAM_SERVICE_AASHTO_COUNT; i++)  m_bPrintParam_Service_AASHTO[i] = FALSE; 
	for(i=0; i<iOUTPUTPARAM_FACTORED_AASHTO_COUNT; i++) m_bPrintParam_Factored_AASHTO[i] = FALSE; 
	
	EnableCtrls();
	
	UpdateData(FALSE);
}

double CDgnCRCDgnParameterDlg::GetExposureFactorAASHTO()
{
	double dReturnExposureFactor=0.0; 
	
	if(m_iExposureTypeAASHTO == 0)
		dReturnExposureFactor = 1.0;
	else if(m_iExposureTypeAASHTO == 1)
		dReturnExposureFactor = 0.75;
	else if (m_iExposureTypeAASHTO == 2)
	{
		CString sExposureFactor;
		GetDlgItemText(m_iID8[21], sExposureFactor);
		dReturnExposureFactor = _ttof(sExposureFactor);
	}

	return dReturnExposureFactor;
}

void CDgnCRCDgnParameterDlg::SetExposureFactorAASHTO(int iExposureType)
{
	if(iExposureType == 0)
		m_iExposureTypeAASHTO = 0;
	else if(iExposureType == 0.75)
		m_iExposureTypeAASHTO = 1;
	else 
	{
		m_iExposureTypeAASHTO = 2;
		CString sExposureFactor;
		//sExposureFactor.Format(_T("%f"), dExposureFactor);
		SetDlgItemText(m_iID8[21], sExposureFactor);
	}
}

void CDgnCRCDgnParameterDlg::Initial_AnnexAASHTO_Ctrl()
{
	CArray<UINT, UINT> aCtrlData;
	aCtrlData.Add(IDC_CRC_DGN_ANNEX_AASHTO_STC);
	aCtrlData.Add(IDC_CRC_DGN_ANNEX_AASHTO_CMB);

	CRect rRef;
	CRect rToMove;
	int nDistX, nDistY;
	GetDlgItem(IDC_CRC_DGN_ANNEX_STC       )->GetWindowRect(rRef);
	GetDlgItem(IDC_CRC_DGN_ANNEX_AASHTO_STC)->GetWindowRect(rToMove);
	nDistX = rRef.left - rToMove.left;
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistXY(this, aCtrlData, nDistX, nDistY);
}

void CDgnCRCDgnParameterDlg::ShowHIdeAnnexCtrl()
{
	GetDlgItem(IDC_CRC_DGN_ANNEX_STC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CRC_DGN_ANNEX    )->ShowWindow(SW_HIDE);

	BOOL bShow = FALSE;
	bShow = m_iDgnCode == EUROCODE2_2_05_PSC ? TRUE : FALSE;
	if (bShow)
	{
		GetDlgItem(IDC_CRC_DGN_ANNEX_STC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CRC_DGN_ANNEX    )->ShowWindow(SW_SHOW);
	}

	GetDlgItem(IDC_CRC_DGN_ANNEX_AASHTO_STC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CRC_DGN_ANNEX_AASHTO_CMB)->ShowWindow(SW_HIDE);

	BOOL bShowAmendent = FALSE;

	if      (m_iDgnCode == AASHTO_LRFD17) bShowAmendent = TRUE;
	else if (m_iDgnCode == AS_5100_5_17_PSC) bShowAmendent = TRUE;

	if (bShowAmendent)
	{
		GetDlgItem(IDC_CRC_DGN_ANNEX_AASHTO_STC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CRC_DGN_ANNEX_AASHTO_CMB)->ShowWindow(SW_SHOW);
	}
}

void CDgnCRCDgnParameterDlg::Initial_Data()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	
	T_PSCD_D PscdD; PscdD.Initialize();

	T_PREFERENCE rPref;
	rPref.Initialize();
	pDoc->m_pInitCtrl->GetPreference(rPref);	
	m_iDgnCode = CDBLib::GetPscCodeNo(rPref.DgnCode.PSCCode);

	CString strCodeName = CDBLib::GetPscCodeName(m_iDgnCode);
	int Index=0;
	if(strCodeName!=_T(""))	Index = m_DesignCode.FindStringExact(-1,strCodeName);
	m_DesignCode.SetCurSel(Index);

	//
	Index = 0;  Index = rPref.DgnCode.nPscNationalAnnex;  // 0:Recommended 1:British 2:Italy
	CDlgUtil::CobxSetCurSelItemData(m_DesignAnnex, Index);
	m_nAnnex = Index;

	//
	CDlgUtil::CobxSetCurSelItemData(m_DesignAmendment, Amend_Recommended); 

	/*
	pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	m_iDgnCode   = PscdD.iDgnCode;  // 1:JSCE02, 2:KSCE-USD03 3:JTG D62-04
	m_DesignCode.SetCurSel(0); */

	//(2009.11.10) Add by Unsang :: Eurocode2-2:05,PSC가 아닌경우 National Annex 입력Tool은 모두 Hide함.
	// Default Hide!
	ShowHIdeAnnexCtrl();

	//
	if(m_iDgnCode==JSCE02)
	{
		m_iDimType   = PscdD.iDimType;
		m_iBrgType   = (PscdD.iBrgType<2 ? PscdD.iBrgType : 1);
		m_bCmpWeb		 = PscdD.bCmpWeb;
		m_iBrgDir		 = PscdD.iBrgDir;
		m_iCrackCode = PscdD.iCrackCode;
		m_iTCrackWidth = (PscdD.iCrackWidth[0]<3 ? PscdD.iCrackWidth[0] : 2);
		m_iBCrackWidth = (PscdD.iCrackWidth[1]<3 ? PscdD.iCrackWidth[1] : 2);
		m_iPsReduFactor = PscdD.iPsReduFactor;
		int i=0;
		for(i=0; i<iIDPRINT0; i++) m_bDesignPrint[i]  = PscdD.bDgnPrint[i];
		for(i=0; i<iIDPRINT1; i++) m_bStructPrint[i]  = PscdD.bStructPrint[i];
		for(i=0; i<iIDPRINT2; i++) m_bMomentPrint[i]  = PscdD.bMomPrint[i];
		for(i=0; i<iIDPRINT3; i++) m_bShearPrint[i]   = PscdD.bShrPrint[i];
		for(i=0; i<iIDPRINT4; i++) m_bTorsionPrint[i] = PscdD.bTorPrint[i];
		for(i=0; i<iIDPRINT5; i++) m_bEtcPrint[i]     = PscdD.bEtcPrint[i];
		for(i=0; i<iIDPRINT6; i++) m_bBckPrint[i]     = PscdD.bBckPrint[i];
	}
	else if(m_iDgnCode==KSCE_USD03 || m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_USD10 || 
					m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_RAIL_USD11)
	{
		m_iDimTypeKsce     = PscdD.iDimType;
		// Chang by Jaeoh. '2005.10.05. Fixed Partially  PSC
		m_iBrgTypeKsce     = 1; // (PscdD.iBrgType<2 ? PscdD.iBrgType : 1);
		m_iTCrackWidthKsce = PscdD.iCrackWidth[0];
		m_iBCrackWidthKsce = PscdD.iCrackWidth[1];
		m_ifpsMethod       = PscdD.ifpsMethod;
		m_iTndnKind        = PscdD.iTndnKind;
		m_bRefRebar        = TRUE;
		if(m_iDgnCode==KSCE_RAIL_USD04) m_bRefRebar        = FALSE;
		int i=0;
		for(i=0; i<iIDPRINTSERV; i++)  m_bServPrint[i] = 1; 
		for(i=0; i<iIDPRINTULTI; i++)  m_bUltiPrint[i] = 1; 
		// Add by Jaeoh. ('2005.10.13) 
		m_bServPrint[0] = 0;
	}
	else if(m_iDgnCode==JTG_D62_04 || m_iDgnCode==CJJ_11_2011)
	{
		m_iDimTypeJTG     = PscdD.iDimType;
		m_iBrgTypeJTG     = PscdD.iBrgType;
		m_iBrgGradeJTG    = PscdD.iSafeType;
		m_iCastTypeJTG    = PscdD.iCastType; 
		m_iSBarCrackWidthJTG = (PscdD.iCrackWidth[0]<2 ? PscdD.iCrackWidth[0] : 1);
		m_iWSCrackWidthJTG   = (PscdD.iCrackWidth[1]<2 ? PscdD.iCrackWidth[1] : 1);
		int i=0;
		for(i=0; i<iIDPRINTSERV_JTG; i++)  m_bServPrintJTG[i] = 1; 
		for(i=0; i<iIDPRINTULTI_JTG; i++)  m_bUltiPrintJTG[i] = 1; 
	}
	else if(m_iDgnCode==TB_10002_3_05)// Add by sshan. MNET:2728. TB 10002.3-2005 ('20070411)
	{
		m_iDimTypeTB     = PscdD.iDimType;
		m_iMemTypeTB     = PscdD.iBrgType;
		m_iCastTypeTB    = PscdD.iCastType;
		
		int i=0;
		for(i=0; i<iIDPRINTBEAM_TB; i++)  m_bBeamPrintTB[i] = 1; 
		for(i=0; i<iIDPRINTCOLU_TB; i++)  m_bColuPrintTB[i] = 1; 
	}
	else if(m_iDgnCode==AASHTO_LRFD08 )
	{
	  // Code.Nishant
		
		m_iTndnTypeAASHTO = PscdD.iTndnKind;
		m_iBrgTypeAASHTO = PscdD.iBrgType;
		m_iConstructionTypeAASHTO = PscdD.iConstructionType;
		
		// Eurocode2-2:05,PSC에서는 해당 변수가 0~2까지 SET!!
		// 따라서, 해당 변수가 2인 경우는 Default값인 0으로 SET함.
		if(PscdD.iCorrosiveCondition==2)  PscdD.iCorrosiveCondition = 0;  
		m_iCorrosiveTypeAASHTO = PscdD.iCorrosiveCondition;

		m_iExposureTypeAASHTO = PscdD.iExposureType;        
		m_iFlexureCalcTypeAASHTO = PscdD.ifpsMethod;

		int i=0;
		for(i=0; i<iOUTPUTPARAM_SERVICE_AASHTO_COUNT; i++) m_bPrintParam_Service_AASHTO[i] = 1;
		for(i=0; i<iOUTPUTPARAM_FACTORED_AASHTO_COUNT; i++) m_bPrintParam_Factored_AASHTO[i] = 1;
	}
	else if(m_iDgnCode==EUROCODE2_2_05_PSC)
	{
		PscdD.iDgnCode        = m_iDgnCode;
		PscdD.nNationalAnnex  = CDlgUtil::CobxGetCurSelItemData(m_DesignAnnex, m_DesignAnnex.GetCurSel());

		//PscdD Data를 National Annex에 맞도록 적합한 변수를 설정해야 함.
		PscdD.dtheta  = 45.0; // Default Cot_Theta = 1!!
        PscdD.dAggregateSize = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 16.0);

        pDoc->m_pDataCtrl->SetAnnexData(PscdD.nNationalAnnex, PscdD);
		//SetDefaultDatabyNationalAnnex(PscdD);

		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::EC;

	  CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_EC2PSC *pUser = (CDgnCRCDgnParameter_EC2PSC*) dlg;
		*pUser->m_pParamData  = PscdD;
		pUser->m_nAnnex      = m_nAnnex;
	}
	else if (m_iDgnCode == ABNT_NBR_6118_2023_PSC)
	{
		ASSERT(0);

		PscdD.iDgnCode = m_iDgnCode;
		PscdD.nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_DesignAnnex, m_DesignAnnex.GetCurSel());

		//PscdD Data를 National Annex에 맞도록 적합한 변수를 설정해야 함.
		PscdD.dtheta = 45.0; // Default Cot_Theta = 1!!
		PscdD.dAggregateSize = pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 16.0);

		pDoc->m_pDataCtrl->SetAnnexData(PscdD.nNationalAnnex, PscdD);
		//SetDefaultDatabyNationalAnnex(PscdD);

		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::EC;

		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_EC2PSC* pUser = (CDgnCRCDgnParameter_EC2PSC*)dlg;
		*pUser->m_pParamData = PscdD;
		pUser->m_nAnnex = m_nAnnex;
	}
	else if(m_iDgnCode==IRC_112_2011_PSC || m_iDgnCode == IRC_112_2020_PSC)
	{
	  PscdD.iDgnCode        = m_iDgnCode;
	  // PscdD.nNationalAnnex  = CDlgUtil::CobxGetCurSelItemData(m_DesignAnnex, m_DesignAnnex.GetCurSel());

	  //PscdD Data를 National Annex에 맞도록 적합한 변수를 설정해야 함.
	  PscdD.dtheta  = 45.0; // Default Cot_Theta = 1!!
	  PscdD.Set_IRC112Data();
	  //SetDefaultDatabyNationalAnnex(PscdD);

	  // SET CurrentDlg Index
	  m_nCurrentDlg = CHILDDLGKEY::EC;

	  CMyChildDialog* dlg;
	  m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
	  dlg->Dlg2Data();

	  CDgnCRCDgnParameter_EC2PSC *pUser = (CDgnCRCDgnParameter_EC2PSC*) dlg;
	  *pUser->m_pParamData  = PscdD;
	  //pUser->m_nAnnex      = m_nAnnex;
	}
	else if(m_iDgnCode==AS_5100_5_17_PSC)
	{
		PscdD.iDgnCode        = m_iDgnCode;
		PscdD.nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_DesignAmendment, m_DesignAmendment.GetCurSel());

		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::AS;

		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_AS5100 *pUser = (CDgnCRCDgnParameter_AS5100*) dlg;
		*pUser->m_pParamData  = PscdD;
	}
	else if(m_iDgnCode==IRS_PSC)
	{
		PscdD.iDgnCode        = m_iDgnCode;

		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::IRS;

		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_IRS *pUser = (CDgnCRCDgnParameter_IRS*) dlg;
		*pUser->m_pParamData  = PscdD;
	}
	else if(m_iDgnCode==BS5400_90_PSC||  m_iDgnCode==BD_44_15_PSC || m_iDgnCode==CS_455_PSC )
	{
		PscdD.iDgnCode        = m_iDgnCode;

		// SET CurrentDlg Index
	  m_nCurrentDlg = CHILDDLGKEY::BS;

	  CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_BS *pUser = (CDgnCRCDgnParameter_BS*) dlg;
		*pUser->m_pParamData  = PscdD;
	}
	else if (PscdD.iDgnCode == TMH07_3_1989 )
	{
		PscdD.iDgnCode = m_iDgnCode;
		PscdD.dmb9 = 0.7; // Tan Alpha f
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::TMH7;

		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_TMH7* pUser = (CDgnCRCDgnParameter_TMH7*)dlg;
		*pUser->m_pParamData = PscdD;
		}
	else if(m_iDgnCode == CSA_S6_19)
	{
		PscdD.iDgnCode        = m_iDgnCode;

		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::CSA19;

		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_CSA_19 *pUser = (CDgnCRCDgnParameter_CSA_19*) dlg;
		*pUser->m_pParamData  = PscdD;
	}
	else if(m_iDgnCode == CSA_S6_14 || m_iDgnCode == CSA_S6_10)
	{
		PscdD.iDgnCode        = m_iDgnCode;
		
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::CSA;
		
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();
		
		CDgnCRCDgnParameter_CSA *pUser = (CDgnCRCDgnParameter_CSA*) dlg;
		*pUser->m_pParamData  = PscdD;
	}
	else if(CDBLib::IsPscCodeAASHTOSince12(m_iDgnCode))
	{
		PscdD.iDgnCode        = m_iDgnCode;
		if (m_iDgnCode == AASHTO_LRFD17)
		{
			PscdD.nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_DesignAmendment, m_DesignAmendment.GetCurSel());
		}
		else
		{
			PscdD.nNationalAnnex = Amend_Recommended;
		}
		
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::AASHTO;
		
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();

		BOOL bInit = FALSE;
		if (!pDoc->m_pAttrCtrl->GetPscd(PscdD)) bInit = TRUE;

		CDgnCRCDgnParameter_AASHTO12 *pUser = (CDgnCRCDgnParameter_AASHTO12*) dlg;
		*pUser->m_pParamData  = PscdD;
		pUser->m_bInitData = bInit;

	}
    else if ( CDBLib::IsPscCodeLSD_KR(m_iDgnCode) )
	{
		PscdD.iDgnCode        = m_iDgnCode;
		
		PscdD.dtheta  = 45.0; // Default Cot_Theta = 1!!
		PscdD.Set_AnnexData_LSD();
		
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::KSCE;
		
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();
		
		CDgnCRCDgnParameter_LSDPSC *pUser = (CDgnCRCDgnParameter_LSDPSC*) dlg;
		*pUser->m_pParamData  = PscdD;
	}
	else if(m_iDgnCode==SNiP_20503_84_PSC || m_iDgnCode==SNiP_20503_84_PSC_MKS)
	{
		PscdD.iDgnCode        = m_iDgnCode;
	 
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::SNIP;
		
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		
		CDgnCRCDgnParameter_RUS *pUser = (CDgnCRCDgnParameter_RUS*) dlg;
		*pUser->m_pData  = PscdD;
	}
	else if(m_iDgnCode==SP_35_13330_11_PSC || m_iDgnCode==SP_35_13330_11_PSC_MKS)
	{
		PscdD.iDgnCode        = m_iDgnCode;
		
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::SP;
		
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		
		CDgnCRCDgnParameter_RUS *pUser = (CDgnCRCDgnParameter_RUS*) dlg;
		*pUser->m_pData  = PscdD;
	}
	else  ASSERT(0);

	if(m_iDgnCode==CJJ_11_2011)
		CheckDlgButton(IDC_CRC_DGN_CHK_SPECIALLOAD,PscdD.bExistSpecialLoad);

	UpdateData(FALSE);

	OnSelchangeDgnCtrldataDesigncode();
}

/*
void CDgnCRCDgnParameterDlg::SetDefaultDatabyNationalAnnex(T_PSCD_D& PscdD)
{
	int nAnnex = 0;
	nAnnex = PscdD.nNationalAnnex;

	// SET DEFAULT// (RECOMMENDED)
	//
	PscdD.dGamma_c_PT   = 1.50;
	PscdD.dGamma_s_PT   = 1.15;
	PscdD.dGamma_ps_PT  = 1.15;
	PscdD.dGamma_c_A    = 1.20;
	PscdD.dGamma_s_A    = 1.00;
	PscdD.dGamma_ps_A   = 1.00;
	PscdD.dGamma_c_serv = 1.00;
	PscdD.dGamma_s_serv = 1.00;
	//
	PscdD.dAlpha_cc     = 0.85;
	PscdD.dAlpha_ct     = 1.00;
	//
	PscdD.dk1_sl        = 0.60;
	PscdD.dk3_sl        = 0.80;
	PscdD.dk4_sl        = 1.00;
	PscdD.dk6_sl        = 0.70;
	//
	PscdD.dk1_ps        = 0.80;
	PscdD.dk2_ps        = 0.90;
	PscdD.dk5_ps        = 0.75;
	PscdD.dk7_ps        = 0.75;
	PscdD.dk8_ps        = 0.85;
	//
	PscdD.dk_c_ps_stage = 1.00;
	PscdD.dk_t_ps_stage = 1.00;
	PscdD.dk_c_ps       = 1.00;
	PscdD.dk_t_ps       = 1.00;
	//
	PscdD.dk3_cr        = 3.400;
	PscdD.dk4_cr        = 0.425;

	if(nAnnex==1) // British
	{
		// Annex별 다른 변수는 다시 SET함.
	}
	else if(nAnnex==2)  // Italy
	{
		// Annex별 다른 변수는 다시 SET함.
		PscdD.dAlpha_ct     = 0.85;
		PscdD.dk3_sl        = 0.70;
		PscdD.dk4_sl        = 0.90;
		PscdD.dk6_sl        = 0.65;
		PscdD.dk5_ps        = 0.60;
	}
}
*/

void CDgnCRCDgnParameterDlg::Initial_ExistData()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	
	T_PSCD_D PscdD; PscdD.Initialize();
	pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	m_iDgnCode   = PscdD.iDgnCode;  // 1:JSCE02, 2:KSCE-USD03, 3:JTG D62-04
	CString strCodeName=_T("");
	strCodeName = CDBLib::GetPscCodeName(m_iDgnCode);
	int Index=0;
	if(strCodeName!=_T(""))	Index = m_DesignCode.FindStringExact(-1,strCodeName);
	m_DesignCode.SetCurSel(Index);

	//
	Index = 0;  Index = PscdD.nNationalAnnex;
	if ( Index < Recommended || Index >= NA_End ) Index = Recommended;  // Default _T("RECOMMENDED")
	CDlgUtil::CobxSetCurSelItemData(m_DesignAnnex, Index);
	m_nAnnex = Index;

	//
	if (m_iDgnCode == AASHTO_LRFD17 || m_iDgnCode == AS_5100_5_17_PSC)
	{
		Index = PscdD.nNationalAnnex;
		CDlgUtil::CobxSetCurSelItemData(m_DesignAmendment, Index);
	}
	else
	{
		CDlgUtil::CobxSetCurSelItemData(m_DesignAmendment, Amend_Recommended);
	}

	//(2009.11.10) Add by Unsang :: Eurocode2-2:05,PSC가 아닌경우 National Annex 입력Tool은 모두 Hide함.
	// Default Hide!
	ShowHIdeAnnexCtrl();

	//
	if(m_iDgnCode==JSCE02)
	{
		m_iDimType   = PscdD.iDimType;
		m_iBrgType   = (PscdD.iBrgType<2 ? PscdD.iBrgType : 1);
		m_bCmpWeb		 = PscdD.bCmpWeb;
		m_iBrgDir		 = PscdD.iBrgDir;
		m_iCrackCode = PscdD.iCrackCode;
		m_iTCrackWidth = (PscdD.iCrackWidth[0]<3 ? PscdD.iCrackWidth[0] : 2);
		m_iBCrackWidth = (PscdD.iCrackWidth[1]<3 ? PscdD.iCrackWidth[1] : 2);
		m_iPsReduFactor = PscdD.iPsReduFactor;
		int i=0;
		for(i=0; i<iIDPRINT0; i++) m_bDesignPrint[i]  = PscdD.bDgnPrint[i];
		for(i=0; i<iIDPRINT1; i++) m_bStructPrint[i]  = PscdD.bStructPrint[i];
		for(i=0; i<iIDPRINT2; i++) m_bMomentPrint[i]  = PscdD.bMomPrint[i];
		for(i=0; i<iIDPRINT3; i++) m_bShearPrint[i]   = PscdD.bShrPrint[i];
		for(i=0; i<iIDPRINT4; i++) m_bTorsionPrint[i] = PscdD.bTorPrint[i];
		for(i=0; i<iIDPRINT5; i++) m_bEtcPrint[i]     = PscdD.bEtcPrint[i];
		for(i=0; i<iIDPRINT6; i++) m_bBckPrint[i]     = PscdD.bBckPrint[i];
	}
	else if(m_iDgnCode==KSCE_USD03 || m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_USD10 || 
					m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_RAIL_USD11)
	{
		m_iDimTypeKsce      = PscdD.iDimType;
		// Chang by Jaeoh. '2005.10.05. Fixed Partially  PSC
		m_iBrgTypeKsce     = 1; // (PscdD.iBrgType<2 ? PscdD.iBrgType : 1);
		m_iTCrackWidthKsce  = PscdD.iCrackWidth[0];
		m_iBCrackWidthKsce  = PscdD.iCrackWidth[1];
		m_ifpsMethod        = PscdD.ifpsMethod;
		m_iTndnKind         = PscdD.iTndnKind;
		m_bRefRebar         = PscdD.bRefRebar;
		int i=0;
		for(i=0; i<iIDPRINTSERV; i++) m_bServPrint[i] = PscdD.bDgnPrint[i];   //bDgnPrint[5]-> 0:응력해석결과, 1:시공단계별 응력도, 2:사용하중시 하중조합별 응력도, 3:응력검토 집계 결과 4:인장철근량계산.
		for(i=0; i<iIDPRINTULTI; i++) m_bUltiPrint[i] = PscdD.bStructPrint[i];   //bStructPrint[5]-> 0:하중조합별 부재력도, 1:하중조합별 부재력 집계, 2:휨강도 검토, 3:전단강도 검토, 4:비틀림강도 검토    
 
		if(m_iDgnCode==KSCE_USD10 || m_iDgnCode==KSCE_RAIL_USD11)
			m_bUltiPrint[4] = 0; 

		// Add by Jaeoh. ('2005.10.13)
		m_bServPrint[0] = 0;
	}
	else if(m_iDgnCode==JTG_D62_04 || m_iDgnCode==CJJ_11_2011)
	{
		m_iDimTypeJTG     = PscdD.iDimType;
		m_iBrgTypeJTG     = PscdD.iBrgType;
		m_iBrgGradeJTG    = PscdD.iSafeType;
		m_iCastTypeJTG    = PscdD.iCastType; 
		m_iSBarCrackWidthJTG = (PscdD.iCrackWidth[0]<2 ? PscdD.iCrackWidth[0] : 1);
		m_iWSCrackWidthJTG   = (PscdD.iCrackWidth[1]<2 ? PscdD.iCrackWidth[1] : 1);
		int i=0;
		// Modify by GAY. MNET:2444. ('06.09.08). Load Data from T_PSCD_D>bMomPrint and bShrPrint.
		/*
		for(i=0; i<iIDPRINTSERV_JTG; i++) m_bServPrintJTG[i] = PscdD.bDgnPrint[i];   //bDgnPrint[5]-> 0:응력해석결과, 1:시공단계별 응력도, 2:사용하중시 하중조합별 응력도, 3:응력검토 집계 결과 4:인장철근량계산.
		for(i=0; i<iIDPRINTULTI_JTG; i++) m_bUltiPrintJTG[i] = PscdD.bStructPrint[i];   //bStructPrint[5]-> 0:하중조합별 부재력도, 1:하중조합별 부재력 집계, 2:휨강도 검토, 3:전단강도 검토, 4:비틀림강도 검토        
		*/
		// bMomPrint[8]-> 0:시공단계 정단면 응력검토, 1:인장영역철근의 인장응력검토, 2:사용단계 정단면 균열검토.
		// 3:사용단계 경사단면 균열검토, 4:사용단계 정단면 압축응력 검토, 5:사용단계 경사단면 압축주응력검토.    
		for(i=0; i<iIDPRINTSERV_JTG; i++) m_bServPrintJTG[i] = PscdD.bMomPrint[i];   
		// bShrPrint[8]-> 0:사용단계 균열폭 검토, 1:사용단계 정단면휨검토, 2:사용단계 경사단면 전단검토, 3:사용단계 비틀림검토.
		for(i=0; i<iIDPRINTULTI_JTG; i++) m_bUltiPrintJTG[i] = PscdD.bShrPrint[i];
	}
	else if(m_iDgnCode==TB_10002_3_05)// Add by sshan. MNET:2728. TB 10002.3-2005 ('20070411)
	{
		m_iDimTypeTB     = PscdD.iDimType;
		m_iMemTypeTB     = PscdD.iBrgType;
		m_iCastTypeTB    = PscdD.iCastType;
		
		int i=0;
		
		for(i=0; i<iIDPRINTBEAM_TB; i++) m_bBeamPrintTB[i] = PscdD.bBeamPrintCH[i];       
		for(i=0; i<iIDPRINTCOLU_TB; i++) m_bColuPrintTB[i] = PscdD.bColumnPrintCH[i];
	}
	else if(m_iDgnCode==AASHTO_LRFD08)
	{
	  //Code. Nishant
		m_iTndnTypeAASHTO         = PscdD.iTndnKind;
		m_iBrgTypeAASHTO          = PscdD.iBrgType;
		m_iConstructionTypeAASHTO = PscdD.iConstructionType;

		// Eurocode2-2:05,PSC에서는 해당 변수가 0~2까지 SET!!
		// 따라서, 해당 변수가 2인 경우는 Default값인 0으로 SET함.
		if(PscdD.iCorrosiveCondition==2)  PscdD.iCorrosiveCondition = 0;  
		m_iCorrosiveTypeAASHTO = PscdD.iCorrosiveCondition;

		m_iFlexureCalcTypeAASHTO = PscdD.ifpsMethod;
		m_iExposureTypeAASHTO = PscdD.iExposureType;  
		//
		m_Data.dExposureFactor = PscdD.dExposureFactor;
		Data2Dlg();
			
		int i=0;
		for(i=0; i<iOUTPUTPARAM_SERVICE_AASHTO_COUNT; i++) m_bPrintParam_Service_AASHTO[i] = PscdD.bServPrint4AASHTO[i]; 
		for(i=0; i<iOUTPUTPARAM_FACTORED_AASHTO_COUNT; i++) m_bPrintParam_Factored_AASHTO[i] = PscdD.bStrePrint4AASHTO[i];
	}
	else if(m_iDgnCode==EUROCODE2_2_05_PSC)
	{
		PscdD.iDgnCode  = m_iDgnCode;
		PscdD.nNationalAnnex  = CDlgUtil::CobxGetCurSelItemData(m_DesignAnnex, m_DesignAnnex.GetCurSel());
 
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::EC;
	  CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_EC2PSC *pUser = (CDgnCRCDgnParameter_EC2PSC*) dlg;
		*pUser->m_pParamData  = PscdD;
		pUser->m_nAnnex      = m_nAnnex;
	}
	else if (m_iDgnCode == ABNT_NBR_6118_2023_PSC)
	{
		PscdD.iDgnCode = m_iDgnCode;

		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::ABNT;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_ABNT* pUser = (CDgnCRCDgnParameter_ABNT*)dlg;
		*pUser->m_pParamData = PscdD;
	}
	else if(m_iDgnCode==IRC_112_2011_PSC || m_iDgnCode == IRC_112_2020_PSC)
	{
	  PscdD.iDgnCode  = m_iDgnCode;
	  //PscdD.nNationalAnnex  = CDlgUtil::CobxGetCurSelItemData(m_DesignAnnex, m_DesignAnnex.GetCurSel());

	  // SET CurrentDlg Index
	  m_nCurrentDlg = CHILDDLGKEY::EC;
	  CMyChildDialog* dlg;
	  m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
	  dlg->Dlg2Data();

	  CDgnCRCDgnParameter_EC2PSC *pUser = (CDgnCRCDgnParameter_EC2PSC*) dlg;
	  *pUser->m_pParamData  = PscdD;
	  pUser->m_nAnnex      = m_nAnnex;
	}
	else if(m_iDgnCode==BS5400_90_PSC||  m_iDgnCode==BD_44_15_PSC || m_iDgnCode==CS_455_PSC )
	{
		PscdD.iDgnCode  = m_iDgnCode;

		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::BS;
	  CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_BS *pUser = (CDgnCRCDgnParameter_BS*) dlg;
		*pUser->m_pParamData  = PscdD;
	}
	else if (m_iDgnCode == TMH07_3_1989 )
	{
		PscdD.iDgnCode = m_iDgnCode;

		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::TMH7;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_TMH7* pUser = (CDgnCRCDgnParameter_TMH7*)dlg;
		*pUser->m_pParamData = PscdD;
		}
	else if(m_iDgnCode == CSA_S6_19)
	{
		PscdD.iDgnCode  = m_iDgnCode;

		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::CSA19;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_CSA_19 *pUser = (CDgnCRCDgnParameter_CSA_19*) dlg;
		*pUser->m_pParamData  = PscdD;
		}
	else if(m_iDgnCode == CSA_S6_14 || m_iDgnCode == CSA_S6_10)
	{
		PscdD.iDgnCode  = m_iDgnCode;
		
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::CSA;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();
		
		CDgnCRCDgnParameter_CSA *pUser = (CDgnCRCDgnParameter_CSA*) dlg;
		*pUser->m_pParamData  = PscdD;
	}
	else if(CDBLib::IsPscCodeAASHTOSince12(m_iDgnCode))
	{
		PscdD.iDgnCode  = m_iDgnCode;
		PscdD.nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_DesignAmendment, m_DesignAmendment.GetCurSel());

		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::AASHTO;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();

		BOOL bInit = FALSE;
		if (!pDoc->m_pAttrCtrl->GetPscd(PscdD)) bInit = TRUE;

		CDgnCRCDgnParameter_AASHTO12 *pUser = (CDgnCRCDgnParameter_AASHTO12*) dlg;
		*pUser->m_pParamData  = PscdD;
		pUser->m_bInitData = bInit;

	}
    else if ( CDBLib::IsPscCodeLSD_KR(m_iDgnCode) )
	{
		PscdD.iDgnCode  = m_iDgnCode;
		
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::KSCE;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();
		
		CDgnCRCDgnParameter_LSDPSC *pUser = (CDgnCRCDgnParameter_LSDPSC*) dlg;
		*pUser->m_pParamData  = PscdD;
	}
	else if(m_iDgnCode==SNiP_20503_84_PSC || m_iDgnCode==SNiP_20503_84_PSC_MKS)
	{
		PscdD.iDgnCode  = m_iDgnCode;
		
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::SNIP;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		
		CDgnCRCDgnParameter_RUS *pUser = (CDgnCRCDgnParameter_RUS*) dlg;
		*pUser->m_pData  = PscdD;
	}
	else if(m_iDgnCode==SP_35_13330_11_PSC || m_iDgnCode==SP_35_13330_11_PSC_MKS)
	{
		PscdD.iDgnCode  = m_iDgnCode;
		
		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::SP;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		
		CDgnCRCDgnParameter_RUS *pUser = (CDgnCRCDgnParameter_RUS*) dlg;
		*pUser->m_pData  = PscdD;
	}
	else if(m_iDgnCode==AS_5100_5_17_PSC)
	{
		PscdD.iDgnCode  = m_iDgnCode;

		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::AS;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_AS5100 *pUser = (CDgnCRCDgnParameter_AS5100*) dlg;
		*pUser->m_pParamData  = PscdD;
	}
	else if(m_iDgnCode==IRS_PSC)
	{
		PscdD.iDgnCode  = m_iDgnCode;

		// SET CurrentDlg Index
		m_nCurrentDlg = CHILDDLGKEY::IRS;
		CMyChildDialog* dlg;
		m_mapChildDlg.Lookup(m_nCurrentDlg,dlg);
		dlg->Dlg2Data();

		CDgnCRCDgnParameter_IRS *pUser = (CDgnCRCDgnParameter_IRS*) dlg;
		*pUser->m_pParamData  = PscdD;
	}
	else  ASSERT(0);

	if(m_iDgnCode==CJJ_11_2011)
		CheckDlgButton(IDC_CRC_DGN_CHK_SPECIALLOAD,PscdD.bExistSpecialLoad);


	UpdateData(FALSE);

	OnSelchangeDgnCtrldataDesigncode();
}

void CDgnCRCDgnParameterDlg::Inittal_Amendment(int iDgnCode, int nNationalAnnex)
{
	m_DesignAmendment.ResetContent();

	if (iDgnCode == AASHTO_LRFD17)
	{
		auto vString = CNationalAnnexTool::GetRcAASHTOAmendment();
		for (int i = 0; i < vString.size(); ++i)
		{
			CDlgUtil::CobxAddItem(m_DesignAmendment, vString[i], CNationalAnnexTool::ConvertAASHTOAmendment(vString[i]));
		}
	}
	else if (iDgnCode == AS_5100_5_17_PSC)
	{
		auto vString = CNationalAnnexTool::GetPscASAmendment();
		for (int i = 0; i < vString.size(); ++i)
		{
			CDlgUtil::CobxAddItem(m_DesignAmendment, vString[i], CNationalAnnexTool::ConvertPscASAmendment(vString[i]));
		}
	}

	CDlgUtil::CobxSetCurSelItemData(m_DesignAmendment, nNationalAnnex);
}

BOOL CDgnCRCDgnParameterDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.dExposureFactor = m_dExposureFactor.GetEditValue();

	return TRUE;
}

BOOL CDgnCRCDgnParameterDlg::Data2Dlg()
{
	m_dExposureFactor.SetUnitType(D_UNITSYS_NONE);
	m_dExposureFactor.SetEditUnit(m_Data.dExposureFactor);
	
	//m_Data.dExposureFactor = GetExposureFactorAASHTO();



	UpdateData(FALSE);
	return TRUE;
}

void CDgnCRCDgnParameterDlg::ChangeDesignCode()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	
	T_PSCD_D PscdD; PscdD.Initialize();
	//pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
	//m_iDgnCode   = PscdD.iDgnCode;  // 1:JSCE02, 2:KSCE-USD03, 3:JTG D62-04
	CString strCodeName=_T("");
	strCodeName = CDBLib::GetPscCodeName(m_iDgnCode);
	int Index=0;
	if(strCodeName!=_T(""))	Index = m_DesignCode.FindStringExact(-1,strCodeName);
	m_DesignCode.SetCurSel(Index);
	
	//(2009.11.10) Add by Unsang :: Eurocode2-2:05,PSC가 아닌경우 National Annex 입력Tool은 모두 Hide함.
	// Default Hide!
	ShowHIdeAnnexCtrl();

	//
	if(m_iDgnCode==JSCE02)
	{
		m_iDimType   = PscdD.iDimType;
		m_iBrgType   = PscdD.iBrgType;
		m_bCmpWeb		 = PscdD.bCmpWeb;
		m_iBrgDir		 = PscdD.iBrgDir;
		m_iCrackCode = PscdD.iCrackCode;
		m_iTCrackWidth = (PscdD.iCrackWidth[0]<3 ? PscdD.iCrackWidth[0] : 2);
		m_iBCrackWidth = (PscdD.iCrackWidth[1]<3 ? PscdD.iCrackWidth[1] : 2);
		m_iPsReduFactor = PscdD.iPsReduFactor;
		int i=0;
		for(i=0; i<iIDPRINT0; i++) m_bDesignPrint[i]  = PscdD.bDgnPrint[i];
		for(i=0; i<iIDPRINT1; i++) m_bStructPrint[i]  = PscdD.bStructPrint[i];
		for(i=0; i<iIDPRINT2; i++) m_bMomentPrint[i]  = PscdD.bMomPrint[i];
		for(i=0; i<iIDPRINT3; i++) m_bShearPrint[i]   = PscdD.bShrPrint[i];
		for(i=0; i<iIDPRINT4; i++) m_bTorsionPrint[i] = PscdD.bTorPrint[i];
		for(i=0; i<iIDPRINT5; i++) m_bEtcPrint[i]     = PscdD.bEtcPrint[i];
		for(i=0; i<iIDPRINT6; i++) m_bBckPrint[i]     = PscdD.bBckPrint[i];
	}
	else if(m_iDgnCode==KSCE_USD03 || m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_USD10 || 
					m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_RAIL_USD11)
	{
		m_iDimTypeKsce      = PscdD.iDimType;
		m_iBrgTypeKsce      = PscdD.iBrgType;
		m_iTCrackWidthKsce  = PscdD.iCrackWidth[0];
		m_iBCrackWidthKsce  = PscdD.iCrackWidth[1];
		m_ifpsMethod        = PscdD.ifpsMethod;
		m_iTndnKind         = PscdD.iTndnKind;
		m_bRefRebar         = PscdD.bRefRebar;
		int i=0;
		for(i=0; i<iIDPRINTSERV; i++) m_bServPrint[i] = PscdD.bDgnPrint[i];   //bDgnPrint[5]-> 0:응력해석결과, 1:시공단계별 응력도, 2:사용하중시 하중조합별 응력도, 3:응력검토 집계 결과 4:인장철근량계산.
		for(i=0; i<iIDPRINTULTI; i++) m_bUltiPrint[i] = PscdD.bStructPrint[i];   //bStructPrint[5]-> 0:하중조합별 부재력도, 1:하중조합별 부재력 집계, 2:휨강도 검토, 3:전단강도 검토, 4:비틀림강도 검토        
	}
	else if(m_iDgnCode==JTG_D62_04 || m_iDgnCode==CJJ_11_2011)
	{
		m_iDimTypeJTG     = PscdD.iDimType;
		m_iBrgTypeJTG     = PscdD.iBrgType;
		m_iBrgGradeJTG    = PscdD.iSafeType;
		m_iCastTypeJTG    = PscdD.iCastType; 
		m_iSBarCrackWidthJTG = (PscdD.iCrackWidth[0]<2 ? PscdD.iCrackWidth[0] : 1);
		m_iWSCrackWidthJTG   = (PscdD.iCrackWidth[1]<2 ? PscdD.iCrackWidth[1] : 1);
		int i=0;
		// Modify by GAY. MNET:2444. ('06.09.08). Load Data from T_PSCD_D>bMomPrint and T_PSCD_D>bShrPrint.
		/*
		for(i=0; i<iIDPRINTSERV_JTG; i++) m_bServPrintJTG[i] = PscdD.bDgnPrint[i];   //bDgnPrint[5]-> 0:응력해석결과, 1:시공단계별 응력도, 2:사용하중시 하중조합별 응력도, 3:응력검토 집계 결과 4:인장철근량계산.
		for(i=0; i<iIDPRINTULTI_JTG; i++) m_bUltiPrintJTG[i] = PscdD.bStructPrint[i];   //bStructPrint[5]-> 0:하중조합별 부재력도, 1:하중조합별 부재력 집계, 2:휨강도 검토, 3:전단강도 검토, 4:비틀림강도 검토    
		*/
		for(i=0; i<iIDPRINTSERV_JTG; i++) m_bServPrintJTG[i] = PscdD.bMomPrint[i];   
		for(i=0; i<iIDPRINTULTI_JTG; i++) m_bUltiPrintJTG[i] = PscdD.bShrPrint[i];

	}
	else if(m_iDgnCode==TB_10002_3_05)
	{
		m_iDimTypeTB     = PscdD.iDimType;
		m_iMemTypeTB     = PscdD.iBrgType;
		m_iCastTypeTB    = PscdD.iCastType;
		
		int i=0;    
		for(i=0; i<iIDPRINTBEAM_TB; i++) m_bBeamPrintTB[i] = PscdD.bBeamPrintCH[i];   
		for(i=0; i<iIDPRINTCOLU_TB; i++) m_bColuPrintTB[i] = PscdD.bColumnPrintCH[i];

	}
	else if(m_iDgnCode==AASHTO_LRFD08 )
	{
	  //Code. Nishant
		m_iTndnTypeAASHTO = PscdD.iTndnKind;
		m_iBrgTypeAASHTO = PscdD.iBrgType;
		m_iConstructionTypeAASHTO = PscdD.iConstructionType;

		// Eurocode2-2:05,PSC에서는 해당 변수가 0~2까지 SET!!
		// 따라서, 해당 변수가 2인 경우는 Default값인 0으로 SET함.
		if(PscdD.iCorrosiveCondition==2)  PscdD.iCorrosiveCondition = 0;  
		m_iCorrosiveTypeAASHTO = PscdD.iCorrosiveCondition;

		m_iExposureTypeAASHTO = PscdD.iExposureType;    
		m_iFlexureCalcTypeAASHTO = PscdD.ifpsMethod;
		//
		m_Data.dExposureFactor = PscdD.dExposureFactor;
		Data2Dlg();
		
		int i=0;
		for(i=0; i<iOUTPUTPARAM_SERVICE_AASHTO_COUNT; i++) m_bPrintParam_Service_AASHTO[i] = PscdD.bServPrint4AASHTO[i]; 
		for(i=0; i<iOUTPUTPARAM_FACTORED_AASHTO_COUNT; i++) m_bPrintParam_Factored_AASHTO[i] = PscdD.bStrePrint4AASHTO[i];
	}
	else if (m_iDgnCode==EUROCODE2_2_05_PSC || m_iDgnCode==ABNT_NBR_6118_2023_PSC)
	{
		//
	}
	else if (m_iDgnCode==IRC_112_2011_PSC || m_iDgnCode == IRC_112_2020_PSC)
	{
	  //
	}
	else if (m_iDgnCode==AS_5100_5_17_PSC)
	{
		//
	}
    else if ( CDBLib::IsPscCodeLSD_KR(m_iDgnCode) )
	{
		//
	}
	else if(m_iDgnCode==BS5400_90_PSC|| PscdD.iDgnCode == TMH07_3_1989 || m_iDgnCode==BD_44_15_PSC || m_iDgnCode==CS_455_PSC )
	{
		//
	}
	else if(m_iDgnCode == CSA_S6_19)
	{
		//
	}
	else if(m_iDgnCode == CSA_S6_14 || m_iDgnCode == CSA_S6_10)
	{
		//
	}
	else if(CDBLib::IsPscCodeAASHTOSince12(m_iDgnCode))
	{
		//
	}
	else if(m_iDgnCode==SNiP_20503_84_PSC || m_iDgnCode==SNiP_20503_84_PSC_MKS)
	{
		//
	}
	else if(m_iDgnCode==SP_35_13330_11_PSC || m_iDgnCode==SP_35_13330_11_PSC_MKS)
	{
		//
	}
	else if (m_iDgnCode==IRS_PSC)
	{
		//
	}
	else  ASSERT(0);

	if(m_iDgnCode==CJJ_11_2011)
		CheckDlgButton(IDC_CRC_DGN_CHK_SPECIALLOAD,PscdD.bExistSpecialLoad);


	UpdateData(FALSE);

	OnSelchangeDgnCtrldataDesigncode();
}

void CDgnCRCDgnParameterDlg::OnClickOption()
{
	UpdateData(TRUE);

	EnableCtrls();

	UpdateData(FALSE);
}

void CDgnCRCDgnParameterDlg::OnSelchangeDgnCtrldataDesigncode() 
{
	UpdateData(TRUE);
	
	CString strCode=_T("");
	m_DesignCode.GetWindowText(strCode);
	int iStatus = CDBLib::GetPscCodeNo(strCode);

	int nNationalAnnex = Amend_Recommended;
	if (m_iDgnCode == iStatus) nNationalAnnex = CDlgUtil::CobxGetCurSelItemData(m_DesignAmendment, m_DesignAmendment.GetCurSel());

	m_iDgnCode = iStatus;

	Inittal_Amendment(m_iDgnCode, nNationalAnnex);

	//(2009.11.10) Add by Unsang :: Eurocode2-2:05,PSC가 아닌경우 National Annex 입력Tool은 모두 Hide함.
	// Default Hide!
	ShowHIdeAnnexCtrl();

	if(m_iDgnCode==CJJ_11_2011)
	{
		GetDlgItem(IDC_CRC_DGN_CHK_SPECIALLOAD)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_CRC_DGN_CHK_SPECIALLOAD)->ShowWindow(SW_HIDE);
	}

	//
	int i=0;
	GetDlgItem(IDC_DGN_REFPOS)->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID0; i++)	GetDlgItem(m_iID0[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID1; i++)	GetDlgItem(m_iID1[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID2; i++)	GetDlgItem(m_iID2[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID3; i++)	GetDlgItem(m_iID3[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID4; i++)	GetDlgItem(m_iID4[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID5; i++)	GetDlgItem(m_iID5[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID6; i++)	GetDlgItem(m_iID6[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID7; i++)	GetDlgItem(m_iID7[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID8; i++)	GetDlgItem(m_iID8[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID9; i++)	GetDlgItem(m_iID9[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID10;i++)  GetDlgItem(m_iID10[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID11;i++)  GetDlgItem(m_iID11[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID12;i++)  GetDlgItem(m_iID12[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID13;i++)  GetDlgItem(m_iID13[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID14;i++)  GetDlgItem(m_iID14[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID15;i++)  GetDlgItem(m_iID15[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID16;i++)  GetDlgItem(m_iID16[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID17;i++)  GetDlgItem(m_iID17[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID18;i++)  GetDlgItem(m_iID18[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID19;i++)  GetDlgItem(m_iID19[i])->ShowWindow(SW_HIDE);
	for(i=0; i<iDgn_PscID20;i++)  GetDlgItem(m_iID20[i])->ShowWindow(SW_HIDE);	

	CRect RectDlg, RectOk, RectCn, RectOrg;
	GetWindowRect(&RectDlg);
	GetDlgItem(IDOK    )->GetWindowRect(&RectOk);
	GetDlgItem(IDCANCEL)->GetWindowRect(&RectCn);
	
	CRect RectRef;
	GetDlgItem(IDC_DGN_REFPOS)->GetWindowRect(&RectRef);
	// EQ
	if(m_iDgnCode==JSCE02)
	{
		RectOrg = MoveCtrl(0,m_iID0, RectRef);
		RectRef.top    = RectOrg.bottom + (RectOrg.left-RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom-RectOrg.top);
		RectRef.left   = RectOrg.left;
		RectRef.right  = RectOrg.right;
		RectOrg = MoveCtrl(1,m_iID1, RectRef);

	}
	else if(m_iDgnCode==KSCE_USD03 || m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_USD10 || 
					m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_RAIL_USD11)
	{
		RectOrg = MoveCtrl(2,m_iID2, RectRef);
		RectRef.top    = RectOrg.bottom + (RectOrg.left-RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom-RectOrg.top);
		RectRef.left   = RectOrg.left;
		RectRef.right  = RectOrg.right;
		RectOrg = MoveCtrl(3,m_iID3, RectRef);
	}
	else if(m_iDgnCode==JTG_D62_04 || m_iDgnCode==CJJ_11_2011)
	{
		RectOrg = MoveCtrl(4,m_iID4, RectRef);
		RectRef.top    = RectOrg.bottom + (RectOrg.left-RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom-RectOrg.top);
		RectRef.left   = RectOrg.left;
		RectRef.right  = RectOrg.right;
		RectOrg = MoveCtrl(5,m_iID5, RectRef);
	}
	else if(m_iDgnCode==TB_10002_3_05)
	{
		RectOrg = MoveCtrl(6,m_iID6, RectRef);
		RectRef.top    = RectOrg.bottom + (RectOrg.left-RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom-RectOrg.top);
		RectRef.left   = RectOrg.left;
		RectRef.right  = RectOrg.right;
		RectOrg = MoveCtrl(7,m_iID7, RectRef);
	}

	else if(m_iDgnCode==AASHTO_LRFD08)
	{
	   // Code.Nishant
		RectOrg = MoveCtrl(8,m_iID8, RectRef);
		RectRef.top    = RectOrg.bottom + (RectOrg.left-RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom-RectOrg.top);
		RectRef.left   = RectOrg.left;
		RectRef.right  = RectOrg.right;
		RectOrg = MoveCtrl(9,m_iID9, RectRef);
	}
	
	else if(m_iDgnCode==EUROCODE2_2_05_PSC)
	{
		RectOrg = MoveCtrl(10, m_iID10, RectRef);
		UpDateCurDlg_EC2(m_iDgnCode);
	}
	else if(m_iDgnCode==ABNT_NBR_6118_2023_PSC)
	{
		RectOrg = MoveCtrl(10, m_iID10, RectRef);
		UpDateCurDlg_ABNT();
	}
	else if(m_iDgnCode==IRC_112_2011_PSC || m_iDgnCode == IRC_112_2020_PSC)
	{
	  RectOrg = MoveCtrl(10, m_iID10, RectRef);
	  UpDateCurDlg_EC2(m_iDgnCode);
	}
	else if(m_iDgnCode==BS5400_90_PSC || m_iDgnCode==BD_44_15_PSC || m_iDgnCode==CS_455_PSC )
	{
		RectOrg = MoveCtrl(11, m_iID11, RectRef);
		UpDateCurDlg_BS();
	}
	else if ( m_iDgnCode == TMH07_3_1989 )
	{
		RectOrg = MoveCtrl(20, m_iID20, RectRef);
		//RectOrg = MoveCtrl(11, m_iID11, RectRef);
		UpDateCurDlg_TMH7();
	}
	else if(m_iDgnCode == CSA_S6_19)
	{
		RectOrg = MoveCtrl(19, m_iID19, RectRef);
		RectRef.top    = RectOrg.bottom + (RectOrg.left-RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom-RectOrg.top); //(rRef.bottom-rRef.top); 
		RectRef.left   = RectOrg.left;
		RectRef.right  = RectOrg.right;
		UpDateCurDlg_CSA19();

	}
	else if(m_iDgnCode == CSA_S6_14 || m_iDgnCode == CSA_S6_10)
	{
		RectOrg = MoveCtrl(12, m_iID12, RectRef);
		RectRef.top    = RectOrg.bottom + (RectOrg.left-RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom-RectOrg.top); //(rRef.bottom-rRef.top); 
		RectRef.left   = RectOrg.left;
		RectRef.right  = RectOrg.right;
		UpDateCurDlg_CSA();

	}
	else if(CDBLib::IsPscCodeAASHTOSince12(m_iDgnCode))
	{
		RectOrg = MoveCtrl(13, m_iID13, RectRef);
		RectRef.top    = RectOrg.bottom + (RectOrg.left-RectDlg.left);
		RectRef.bottom = RectRef.top + (RectOrg.bottom-RectOrg.top); //(rRef.bottom-rRef.top); 
		RectRef.left   = RectOrg.left;
		RectRef.right  = RectOrg.right;
		UpDateCurDlg_AASHTO12();
	}
	else if( CDBLib::IsPscCodeLSD_KR(m_iDgnCode) )
	{
		RectOrg = MoveCtrl(14, m_iID14, RectRef);
		UpDateCurDlg_LSD15();
	}
	else if(m_iDgnCode==SNiP_20503_84_PSC || m_iDgnCode==SNiP_20503_84_PSC_MKS)
	{
		RectOrg = MoveCtrl(15, m_iID15, RectRef);
		UpdateCurDlg_SNiP();
	}
	else if(m_iDgnCode==SP_35_13330_11_PSC || m_iDgnCode==SP_35_13330_11_PSC_MKS)
	{
		RectOrg = MoveCtrl(16, m_iID16, RectRef);
		UpdateCurDlg_SP();
	}
	else if(m_iDgnCode==AS_5100_5_17_PSC)
	{
		RectOrg = MoveCtrl(17, m_iID17, RectRef);
		UpDateCurDlg_AS5100();
	}
	else if(m_iDgnCode==IRS_PSC)
	{
		RectOrg = MoveCtrl(18, m_iID18, RectRef);
		UpDateCurDlg_IRS();
	}
	else ASSERT(0);
	
    MoveDlgOkCn(RectOrg);
	EnableCtrls();

	OnClickSpecialLoad();
	UpdateData(FALSE);  
}

void CDgnCRCDgnParameterDlg::OnSelchangeDgnCtrldataDesignAnnex()
{
	if ( m_iDgnCode!=EUROCODE2_2_05_PSC )
    {
        return;
    }

    CMyChildDialog* dlg;
    m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
    dlg->Dlg2Data();

    CDgnCRCDgnParameter_EC2PSC *pUser = (CDgnCRCDgnParameter_EC2PSC*)dlg;
    m_nAnnex = CDlgUtil::CobxGetCurSelItemData(m_DesignAnnex, m_DesignAnnex.GetCurSel());;
    pUser->m_nAnnex = m_nAnnex;
    pUser->SetAnnexCtrl();

    return;
}

void CDgnCRCDgnParameterDlg::OnSelchangeDgnCtrldataDesignAmendment()
{
	if (m_iDgnCode != AASHTO_LRFD17)
	{
		return;
	}

	CMyChildDialog* dlg;
	m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
	dlg->Dlg2Data();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	T_PSCD_D PscdD;
	BOOL bInit = FALSE;
	if (!pDoc->m_pAttrCtrl->GetPscd(PscdD)) bInit = TRUE;

	CDgnCRCDgnParameter_AASHTO12* pUser = (CDgnCRCDgnParameter_AASHTO12*)dlg;
	pUser->m_bInitData = bInit;

	m_nAnnex = CDlgUtil::CobxGetCurSelItemData(m_DesignAmendment, m_DesignAmendment.GetCurSel());;
	pUser->m_nAnnex = m_nAnnex;
	pUser->SetAnnexCtrl();
}

void CDgnCRCDgnParameterDlg::EnableCtrls()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

//   CWnd* pParent = GetParent();
//   if (pParent == NULL) 
//   {
//     ASSERT(FALSE);
//     return;
//   }
// 
//   CWnd* pItem = pParent->GetDlgItem(ID);
//   


	if(m_iDgnCode==JSCE02)
	{
		// Add by ZINU.('06.02.15). To Check Bridge Direction (0=Longitudinal, 1=Transverse).
		// 파형웹 Checkbox.
		GetDlgItem(IDC_CRC_DESIGN_PARAMETER8)->EnableWindow(TRUE);
		// 주형전단 검토.
		GetDlgItem(IDC_CRC_PRINT_PARAMETER23)->EnableWindow(TRUE);
		GetDlgItem(IDC_CRC_PRINT_PARAMETER24)->EnableWindow(TRUE);
		GetDlgItem(IDC_CRC_PRINT_PARAMETER25)->EnableWindow(TRUE);
		GetDlgItem(IDC_CRC_PRINT_PARAMETER26)->EnableWindow(TRUE);
		GetDlgItem(IDC_CRC_PRINT_PARAMETER27)->EnableWindow(TRUE);
		// 주형비틀림 검토.
		GetDlgItem(IDC_CRC_PRINT_PARAMETER29)->EnableWindow(TRUE);
		GetDlgItem(IDC_CRC_PRINT_PARAMETER30)->EnableWindow(TRUE);
		GetDlgItem(IDC_CRC_PRINT_PARAMETER31)->EnableWindow(TRUE);
		GetDlgItem(IDC_CRC_PRINT_PARAMETER32)->EnableWindow(TRUE);
		// 파형웹.
		GetDlgItem(IDC_CRC_PRINT_PARAMETER37)->EnableWindow(TRUE);
		GetDlgItem(IDC_CRC_PRINT_PARAMETER38)->EnableWindow(TRUE);
		GetDlgItem(IDC_CRC_PRINT_PARAMETER39)->EnableWindow(TRUE);
		GetDlgItem(IDC_CRC_PRINT_PARAMETER40)->EnableWindow(TRUE);

		if(m_iDimType==0)	// 2D
		{
			GetDlgItem(IDC_CRC_PRINT_PARAMETER29)->EnableWindow(FALSE);  m_bTorsionPrint[0] = FALSE;
			GetDlgItem(IDC_CRC_PRINT_PARAMETER30)->EnableWindow(FALSE);  m_bTorsionPrint[1] = FALSE;
			GetDlgItem(IDC_CRC_PRINT_PARAMETER31)->EnableWindow(FALSE);  m_bTorsionPrint[2] = FALSE;
			GetDlgItem(IDC_CRC_PRINT_PARAMETER32)->EnableWindow(FALSE);  m_bTorsionPrint[3] = FALSE;
		}
		else
		{
			GetDlgItem(IDC_CRC_PRINT_PARAMETER29)->EnableWindow(TRUE);
			GetDlgItem(IDC_CRC_PRINT_PARAMETER30)->EnableWindow(TRUE);
			GetDlgItem(IDC_CRC_PRINT_PARAMETER31)->EnableWindow(TRUE);
			GetDlgItem(IDC_CRC_PRINT_PARAMETER32)->EnableWindow(TRUE);
		}

		if(m_iBrgType==0) // PC
		{
			GetDlgItem(IDC_CRC_DESIGN_PARAMETER14)->EnableWindow(FALSE);
			GetDlgItem(IDC_CRC_DESIGN_PARAMETER15)->EnableWindow(FALSE);
			GetDlgItem(IDC_CRC_DESIGN_PARAMETER16)->EnableWindow(FALSE);
			GetDlgItem(IDC_CRC_DESIGN_PARAMETER18)->EnableWindow(FALSE);
			GetDlgItem(IDC_CRC_DESIGN_PARAMETER19)->EnableWindow(FALSE);
			GetDlgItem(IDC_CRC_DESIGN_PARAMETER20)->EnableWindow(FALSE);
			m_iTCrackWidth = 0;
			m_iBCrackWidth = 0;

			GetDlgItem(IDC_CRC_PRINT_PARAMETER19)->EnableWindow(FALSE);  m_bMomentPrint[5] = FALSE;
			GetDlgItem(IDC_CRC_PRINT_PARAMETER20)->EnableWindow(TRUE);

			GetDlgItem(IDC_CRC_PRINT_PARAMETER25)->EnableWindow(FALSE);  m_bShearPrint[2] = FALSE;
			GetDlgItem(IDC_CRC_PRINT_PARAMETER30)->EnableWindow(FALSE);  m_bTorsionPrint[1] = FALSE;

			if(m_bCmpWeb) // 파형웹
			{
				GetDlgItem(IDC_CRC_PRINT_PARAMETER37)->EnableWindow(TRUE);
				GetDlgItem(IDC_CRC_PRINT_PARAMETER38)->EnableWindow(TRUE);
				GetDlgItem(IDC_CRC_PRINT_PARAMETER39)->EnableWindow(TRUE);
				GetDlgItem(IDC_CRC_PRINT_PARAMETER40)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(IDC_CRC_PRINT_PARAMETER37)->EnableWindow(FALSE);  m_bBckPrint[0] = FALSE;
				GetDlgItem(IDC_CRC_PRINT_PARAMETER38)->EnableWindow(FALSE);  m_bBckPrint[1] = FALSE;
				GetDlgItem(IDC_CRC_PRINT_PARAMETER39)->EnableWindow(FALSE);  m_bBckPrint[2] = FALSE;
				GetDlgItem(IDC_CRC_PRINT_PARAMETER40)->EnableWindow(FALSE);  m_bBckPrint[3] = FALSE;
			}

			GetDlgItem(IDC_CRC_DESIGN_PARAMETER22)->EnableWindow(TRUE);
			GetDlgItem(IDC_CRC_DESIGN_PARAMETER23)->EnableWindow(TRUE);
		}
		else if(m_iBrgType==1) // PRC
		{
			GetDlgItem(IDC_CRC_DESIGN_PARAMETER14)->EnableWindow(TRUE);
			GetDlgItem(IDC_CRC_DESIGN_PARAMETER15)->EnableWindow(TRUE);
			GetDlgItem(IDC_CRC_DESIGN_PARAMETER16)->EnableWindow(TRUE);
			GetDlgItem(IDC_CRC_DESIGN_PARAMETER18)->EnableWindow(TRUE);
			GetDlgItem(IDC_CRC_DESIGN_PARAMETER19)->EnableWindow(TRUE);
			GetDlgItem(IDC_CRC_DESIGN_PARAMETER20)->EnableWindow(TRUE);

			GetDlgItem(IDC_CRC_PRINT_PARAMETER19)->EnableWindow(TRUE);
			GetDlgItem(IDC_CRC_PRINT_PARAMETER20)->EnableWindow(FALSE);  m_bMomentPrint[6] = FALSE;
			GetDlgItem(IDC_CRC_PRINT_PARAMETER25)->EnableWindow(TRUE);
			if(m_iDimType!=0) GetDlgItem(IDC_CRC_PRINT_PARAMETER30)->EnableWindow(TRUE);

			if(m_bCmpWeb) // 파형웹
			{
				GetDlgItem(IDC_CRC_PRINT_PARAMETER37)->EnableWindow(TRUE);
				GetDlgItem(IDC_CRC_PRINT_PARAMETER38)->EnableWindow(TRUE);
				GetDlgItem(IDC_CRC_PRINT_PARAMETER39)->EnableWindow(TRUE);
				GetDlgItem(IDC_CRC_PRINT_PARAMETER40)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(IDC_CRC_PRINT_PARAMETER37)->EnableWindow(FALSE);  m_bBckPrint[0] = FALSE;
				GetDlgItem(IDC_CRC_PRINT_PARAMETER38)->EnableWindow(FALSE);  m_bBckPrint[1] = FALSE;
				GetDlgItem(IDC_CRC_PRINT_PARAMETER39)->EnableWindow(FALSE);  m_bBckPrint[2] = FALSE;
				GetDlgItem(IDC_CRC_PRINT_PARAMETER40)->EnableWindow(FALSE);  m_bBckPrint[3] = FALSE;
			}

			GetDlgItem(IDC_CRC_DESIGN_PARAMETER22)->EnableWindow(FALSE);
			GetDlgItem(IDC_CRC_DESIGN_PARAMETER23)->EnableWindow(FALSE);
			m_iPsReduFactor = 0;
		}

		if(pDoc->IsPostMode())
		{
			T_POSTCTRL_DATACOUNT* pDataCount = pDoc->m_pPostCtrl->GetPointDataCount();
			BOOL bExistStage = (pDataCount->m_nNumStage > 0 ? TRUE : FALSE);

			if(bExistStage)
			{
				GetDlgItem(IDC_CRC_PRINT_PARAMETER9)->EnableWindow(TRUE);
				GetDlgItem(IDC_CRC_PRINT_PARAMETER15)->EnableWindow(TRUE);
				GetDlgItem(IDC_CRC_PRINT_PARAMETER23)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(IDC_CRC_PRINT_PARAMETER9)->EnableWindow(FALSE);   m_bStructPrint[1] = FALSE;
				GetDlgItem(IDC_CRC_PRINT_PARAMETER15)->EnableWindow(FALSE);  m_bMomentPrint[1] = FALSE;
				GetDlgItem(IDC_CRC_PRINT_PARAMETER23)->EnableWindow(FALSE);  m_bShearPrint[0] = FALSE;
			}
		}
		else
		{
			GetDlgItem(IDC_CRC_PRINT_PARAMETER9)->EnableWindow(TRUE);
			GetDlgItem(IDC_CRC_PRINT_PARAMETER15)->EnableWindow(TRUE);
			GetDlgItem(IDC_CRC_PRINT_PARAMETER23)->EnableWindow(TRUE);
		}
		// Add by ZINU.('04.09.13). Always Hide.
		//GetDlgItem(IDC_CRC_PRINT_PARAMETER35)->ShowWindow(SW_HIDE);

		// Add by ZINU.('06.02.15). To Check Bridge Direction (0=Longitudinal, 1=Transverse).
		if(m_iBrgDir==1)	// Transverse.
		{
			// 파형웹 Checkbox.
			GetDlgItem(IDC_CRC_DESIGN_PARAMETER8)->EnableWindow(FALSE);  m_bCmpWeb = FALSE;
			// 주형전단 검토.
			GetDlgItem(IDC_CRC_PRINT_PARAMETER23)->EnableWindow(FALSE);  m_bShearPrint[0] = FALSE;
			GetDlgItem(IDC_CRC_PRINT_PARAMETER24)->EnableWindow(FALSE);  m_bShearPrint[1] = FALSE;
			GetDlgItem(IDC_CRC_PRINT_PARAMETER25)->EnableWindow(FALSE);  m_bShearPrint[2] = FALSE;
			GetDlgItem(IDC_CRC_PRINT_PARAMETER26)->EnableWindow(FALSE);  m_bShearPrint[3] = FALSE;
			GetDlgItem(IDC_CRC_PRINT_PARAMETER27)->EnableWindow(FALSE);  m_bShearPrint[4] = FALSE;
			// 주형비틀림 검토.
			GetDlgItem(IDC_CRC_PRINT_PARAMETER29)->EnableWindow(FALSE);  m_bTorsionPrint[0] = FALSE;
			GetDlgItem(IDC_CRC_PRINT_PARAMETER30)->EnableWindow(FALSE);  m_bTorsionPrint[1] = FALSE;
			GetDlgItem(IDC_CRC_PRINT_PARAMETER31)->EnableWindow(FALSE);  m_bTorsionPrint[2] = FALSE;
			GetDlgItem(IDC_CRC_PRINT_PARAMETER32)->EnableWindow(FALSE);  m_bTorsionPrint[3] = FALSE;
			// 파형웹.
			GetDlgItem(IDC_CRC_PRINT_PARAMETER37)->EnableWindow(FALSE);  m_bBckPrint[0] = FALSE;
			GetDlgItem(IDC_CRC_PRINT_PARAMETER38)->EnableWindow(FALSE);  m_bBckPrint[1] = FALSE;
			GetDlgItem(IDC_CRC_PRINT_PARAMETER39)->EnableWindow(FALSE);  m_bBckPrint[2] = FALSE;
			GetDlgItem(IDC_CRC_PRINT_PARAMETER40)->EnableWindow(FALSE);  m_bBckPrint[3] = FALSE;
		}
	}

	else if(m_iDgnCode==KSCE_USD03)
	{
		// Change by Jaeoh. '2005.10.5
		int i=0;
		for(i=5; i<8; i++) GetDlgItem(m_iID2[i])->ShowWindow(SW_HIDE);
		for(i=8; i<19; i++)    GetDlgItem(m_iID2[i])->ShowWindow(SW_HIDE);
/*
		if(m_ifpsMethod == 0)
		{
			GetDlgItem(IDC_KSCE_DESIGN_PARAMETER25)->EnableWindow(TRUE);
			GetDlgItem(IDC_KSCE_DESIGN_PARAMETER26)->EnableWindow(TRUE);
			GetDlgItem(IDC_KSCE_DESIGN_PARAMETER27)->EnableWindow(TRUE);
		}
		else if(m_ifpsMethod == 1)
		{
			GetDlgItem(IDC_KSCE_DESIGN_PARAMETER25)->EnableWindow(FALSE);
			GetDlgItem(IDC_KSCE_DESIGN_PARAMETER26)->EnableWindow(FALSE);
			GetDlgItem(IDC_KSCE_DESIGN_PARAMETER27)->EnableWindow(FALSE);
			m_iTndnKind = 0;
		}
		*/

		GetDlgItem(IDC_KSCE_PRINT_PARAMETER2)->EnableWindow(FALSE);
	}

	else if(m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_USD10 || m_iDgnCode==KSCE_RAIL_USD04 || m_iDgnCode==KSCE_RAIL_USD11)
	{
		// Change by Jaeoh. '2005.10.5
		int i=0;
		for(i=5; i<8; i++) GetDlgItem(m_iID2[i])->ShowWindow(SW_HIDE);
		for(i=8; i<19; i++)    GetDlgItem(m_iID2[i])->ShowWindow(SW_HIDE);
/*
		if(m_ifpsMethod == 0)
		{
			GetDlgItem(IDC_KSCE_DESIGN_PARAMETER25)->EnableWindow(TRUE);
			GetDlgItem(IDC_KSCE_DESIGN_PARAMETER26)->EnableWindow(TRUE);
			GetDlgItem(IDC_KSCE_DESIGN_PARAMETER27)->EnableWindow(TRUE);      
		}
		else if(m_ifpsMethod == 1)
		{
			GetDlgItem(IDC_KSCE_DESIGN_PARAMETER25)->EnableWindow(FALSE);
			GetDlgItem(IDC_KSCE_DESIGN_PARAMETER26)->EnableWindow(FALSE);
			GetDlgItem(IDC_KSCE_DESIGN_PARAMETER27)->EnableWindow(FALSE);
			m_iTndnKind = 0;
		}
		*/
		GetDlgItem(IDC_KSCE_PRINT_PARAMETER2)->EnableWindow(FALSE);

		if(m_iDgnCode==KSCE_USD10 || m_iDgnCode==KSCE_RAIL_USD11)
			GetDlgItem(IDC_KSCE_PRINT_PARAMETER6)->ShowWindow(SW_HIDE);
		else
			GetDlgItem(IDC_KSCE_PRINT_PARAMETER6)->ShowWindow(SW_SHOW);

	}

	else if(m_iDgnCode==JTG_D62_04 || m_iDgnCode==CJJ_11_2011)
	{
		// According to Bridge Type.
		if(m_iBrgTypeJTG == 0 || m_iBrgTypeJTG == 1) //Fully PSC , Partially PSC(A)
		{
			GetDlgItem(IDC_JTG_DESIGN_PARAMETER18)->EnableWindow(FALSE);
			GetDlgItem(IDC_JTG_DESIGN_PARAMETER19)->EnableWindow(FALSE);
			GetDlgItem(IDC_JTG_DESIGN_PARAMETER21)->EnableWindow(FALSE);
			GetDlgItem(IDC_JTG_DESIGN_PARAMETER22)->EnableWindow(FALSE);
			m_iSBarCrackWidthJTG = 0;
			m_iWSCrackWidthJTG   = 0;

			// Add by GAY. MNET:2444. ('06.09.08).      
			GetDlgItem(IDC_JTG_PRINT_PARAMETER7)->EnableWindow(FALSE);     m_bUltiPrintJTG[0] = FALSE;
			GetDlgItem(IDC_JTG_PRINT_PARAMETER3)->EnableWindow(TRUE);
		}
		else if(m_iBrgTypeJTG == 2) //Partially PSC(B)
		{
			GetDlgItem(IDC_JTG_DESIGN_PARAMETER18)->EnableWindow(TRUE);
			GetDlgItem(IDC_JTG_DESIGN_PARAMETER19)->EnableWindow(TRUE);
			GetDlgItem(IDC_JTG_DESIGN_PARAMETER21)->EnableWindow(TRUE);
			GetDlgItem(IDC_JTG_DESIGN_PARAMETER22)->EnableWindow(TRUE);

			// Add by GAY. MNET:2444. ('06.09.08).
			GetDlgItem(IDC_JTG_PRINT_PARAMETER7)->EnableWindow(TRUE);
			GetDlgItem(IDC_JTG_PRINT_PARAMETER3)->EnableWindow(FALSE);     m_bServPrintJTG[2] = FALSE;
		}
		else ASSERT(0);

		// According to Design Dimension.
		if(m_iDimTypeJTG == 0)   // 2D.
		{
			GetDlgItem(IDC_JTG_PRINT_PARAMETER10)->EnableWindow(FALSE);    m_bUltiPrintJTG[3] = FALSE;
		}
		else
		{
			GetDlgItem(IDC_JTG_PRINT_PARAMETER10)->EnableWindow(TRUE);
		}
	}
	else if(m_iDgnCode==TB_10002_3_05)
	{
		//GetDlgItem(IDC_TB_PRINT_PARAMETER9)->EnableWindow(TRUE);
		GetDlgItem(IDC_TB_PRINT_PARAMETER13)->EnableWindow(TRUE);
		GetDlgItem(IDC_TB_PRINT_PARAMETER5)->EnableWindow(TRUE);
		GetDlgItem(IDC_TB_PRINT_PARAMETER6)->EnableWindow(TRUE);
		GetDlgItem(IDC_TB_PRINT_PARAMETER20)->EnableWindow(TRUE);		

		// According to Bridge Type.
		if(m_iMemTypeTB == 0) //Forbid Tension Stress
		{
			//GetDlgItem(IDC_TB_PRINT_PARAMETER9)->EnableWindow(FALSE);
			GetDlgItem(IDC_TB_PRINT_PARAMETER13)->EnableWindow(FALSE);
			m_bBeamPrintTB[7] = FALSE;
			m_bBeamPrintTB[11] = FALSE;
		}
		else if(m_iMemTypeTB == 1) //Forbid Crack
		{
//      GetDlgItem(IDC_TB_PRINT_PARAMETER5)->EnableWindow(FALSE);
//      GetDlgItem(IDC_TB_PRINT_PARAMETER6)->EnableWindow(FALSE);
			GetDlgItem(IDC_TB_PRINT_PARAMETER13)->EnableWindow(FALSE);
//      GetDlgItem(IDC_TB_PRINT_PARAMETER20)->EnableWindow(FALSE);
//			m_bBeamPrintTB[3] = FALSE;
//      m_bBeamPrintTB[4] = FALSE;
			m_bBeamPrintTB[11] = FALSE;
//      m_bColuPrintTB[3] = FALSE;
		}
		else if(m_iMemTypeTB == 2) //Allow Crack
		{
			GetDlgItem(IDC_TB_PRINT_PARAMETER5)->EnableWindow(FALSE);
//      GetDlgItem(IDC_TB_PRINT_PARAMETER6)->EnableWindow(FALSE);
			GetDlgItem(IDC_TB_PRINT_PARAMETER20)->EnableWindow(FALSE);      
			m_bBeamPrintTB[3] = FALSE;
//      m_bBeamPrintTB[4] = FALSE;
			m_bColuPrintTB[3] = FALSE;
		}
		else ASSERT(0);
		
		//사용하지 않는 컨트롤로 결정되어 마효의 요청으로 ... (070726 sshan)
		GetDlgItem(IDC_TB_PRINT_PARAMETER8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TB_PRINT_PARAMETER9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_TB_PRINT_PARAMETER22)->ShowWindow(SW_HIDE);
	}

	else if(m_iDgnCode==AASHTO_LRFD08)
	{
		GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER7)->ShowWindow(SW_SHOW);

		if(m_iBrgTypeAASHTO == 0)
	  {
		  GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER18)->EnableWindow(FALSE);
		  GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER19)->EnableWindow(FALSE);
		  GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER20)->EnableWindow(FALSE);
		  GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER21)->EnableWindow(FALSE);
	  }
	  else
	  {
		  GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER18)->EnableWindow(TRUE);
		  GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER19)->EnableWindow(TRUE);
		  GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER20)->EnableWindow(TRUE);
		  GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER21)->EnableWindow(TRUE);

		  if(m_iExposureTypeAASHTO == 2)
			  GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER21)->EnableWindow(TRUE);
		  else
			  GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER21)->EnableWindow(FALSE);	
	  }
	}
	else if(CDBLib::IsPscCodeAASHTOSince12(m_iDgnCode))
	{
//     GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER5)->ShowWindow(SW_HIDE);
//     GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER6)->ShowWindow(SW_HIDE);
//     GetDlgItem(IDC_AASHTO_DESIGN_PARAMETER7)->ShowWindow(SW_HIDE);
	}

	else if(m_iDgnCode==EUROCODE2_2_05_PSC || m_iDgnCode==ABNT_NBR_6118_2023_PSC)
	{
		// 
	}
	else if( CDBLib::IsPscCodeLSD_KR(m_iDgnCode) )
	{
		//
	}
	else if(m_iDgnCode==BS5400_90_PSC|| m_iDgnCode == TMH07_3_1989 || m_iDgnCode==BD_44_15_PSC || m_iDgnCode==CS_455_PSC )
	{
		//
	}
	else if(m_iDgnCode == CSA_S6_19)
	{
		//
	}
	else if(m_iDgnCode == CSA_S6_14 || m_iDgnCode == CSA_S6_10)
	{
		//
	}
	else if(m_iDgnCode==SNiP_20503_84_PSC || m_iDgnCode==SNiP_20503_84_PSC_MKS)
	{
		//
	}
	else if(m_iDgnCode==SP_35_13330_11_PSC || m_iDgnCode==SP_35_13330_11_PSC_MKS)
	{
		//
	}
	else if(m_iDgnCode==IRC_112_2011_PSC || m_iDgnCode == IRC_112_2020_PSC)
	{
		// 
	}
	else if(m_iDgnCode==AS_5100_5_17_PSC)
	{
		// 
	}
	else if(m_iDgnCode==IRS_PSC)
	{
		// 
	}
	else  ASSERT(0);
}

CRect CDgnCRCDgnParameterDlg::MoveCtrl(int iKind, int* pID, CRect Rect0)
{
	CRect RectOrg;
	int iArSize=0;
	if      (iKind== 0) iArSize = iDgn_PscID0;
	else if (iKind== 1)	iArSize = iDgn_PscID1;
	else if (iKind== 2)	iArSize = iDgn_PscID2;
	else if (iKind== 3) iArSize = iDgn_PscID3;
	else if (iKind== 4) iArSize = iDgn_PscID4;
	else if (iKind== 5) iArSize = iDgn_PscID5;
	else if (iKind== 6) iArSize = iDgn_PscID6;
	else if (iKind== 7) iArSize = iDgn_PscID7;
	else if (iKind== 8)	iArSize = iDgn_PscID8;
	else if (iKind== 9)	iArSize = iDgn_PscID9;
	else if (iKind==10) iArSize = iDgn_PscID10;
	else if (iKind==11) iArSize = iDgn_PscID11;
	else if (iKind==12) iArSize = iDgn_PscID12;
	else if (iKind==13) iArSize = iDgn_PscID13;
	else if (iKind==14) iArSize = iDgn_PscID14;
	else if (iKind==15) iArSize = iDgn_PscID15;
	else if (iKind==16) iArSize = iDgn_PscID16;
	else if (iKind==17) iArSize = iDgn_PscID17;
	else if (iKind==18) iArSize = iDgn_PscID18;
	else if (iKind==19) iArSize = iDgn_PscID19;
	else if (iKind==20) iArSize = iDgn_PscID20;

	else	ASSERT(0);

	CRect* pRectCng = new CRect[iArSize];

	CRect Rect1;
	int i=0;
	for(i=0; i<iArSize; i++)
	{
		GetDlgItem(pID[i])->ShowWindow(SW_SHOW);
		GetDlgItem(pID[i])->GetWindowRect(&(pRectCng[i]));
		if(i==0)	Rect1 = pRectCng[i];
		pRectCng[i].top		 -= Rect1.top - Rect0.top;
		pRectCng[i].bottom -= Rect1.top - Rect0.top;
		pRectCng[i].left   -= Rect1.left - Rect0.left;
		pRectCng[i].right  -= Rect1.left - Rect0.left;
	}
	RectOrg = pRectCng[0];
	for(i=0; i<iArSize; i++)
	{
		ScreenToClient(pRectCng[i]);
		GetDlgItem(pID[i])->SetWindowPos(NULL, pRectCng[i].left, pRectCng[i].top, pRectCng[i].Width(), pRectCng[i].Height(), SWP_NOSIZE | SWP_NOZORDER);
	}
	delete[] pRectCng;
	return RectOrg;
}

void CDgnCRCDgnParameterDlg::MoveDlgOkCn(CRect RectOrg)
{
    CRect RectDlg, RectOk, RectCn;
    GetWindowRect(&RectDlg);
    GetDlgItem(IDOK)->GetWindowRect(&RectOk);
    GetDlgItem(IDCANCEL)->GetWindowRect(&RectCn);

    CRect rDlg;
    rDlg.left	= RectDlg.left;
    rDlg.top	= RectDlg.top;
    rDlg.right	= RectOrg.right + (RectOrg.left-RectDlg.left);
    rDlg.bottom	= RectOrg.bottom + globalUtils.ScaleByDPI(16) + globalUtils.ScaleByDPI(8) + RectOk.Height(); //(int)(2.5*RectOk.Height());

    CRect rOk;
    rOk.left   = RectOk.left;
    rOk.right  = RectOk.right;
    rOk.top	   = rDlg.bottom - RectOk.Height() - globalUtils.ScaleByDPI(8); //(int)(1.75*RectOk.Height());
    rOk.bottom = rOk.top + RectOk.Height();

    CRect rCn;
    rCn.left   = RectCn.left;
    rCn.right  = RectCn.right;
    rCn.top	   = rOk.top;
    rCn.bottom = rOk.bottom;

    ScreenToClient(rDlg);
    ScreenToClient(rOk);
    ScreenToClient(rCn);
    SetWindowPos(NULL, rDlg.left, rDlg.top, rDlg.Width(), rDlg.Height(), SWP_NOMOVE | SWP_NOZORDER);
    GetDlgItem(IDOK)->SetWindowPos(NULL, rOk.left, rOk.top, rOk.Width(), rOk.Height(), SWP_NOSIZE);
    GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rCn.left, rCn.top, rCn.Width(), rCn.Height(), SWP_NOSIZE);

	Invalidate();
    return;
}

// void CDgnCRCDgnParameterDlg::UpdateCurDlg(int iDgnCode)
// {
//   if      (iDgnCode==EUROCODE2_2_05_PSC)  UpDateCurDlg_EC2();
//   else if (iDgnCode==BS5400_90_PSC)       UpDateCurDlg_BS();
// }

void CDgnCRCDgnParameterDlg::UpDateCurDlg_EC2(int m_iDgnCode)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CMyChildDialog* dlg;
	CWnd* pPlaceHolder = GetDlgItem(m_iID10[0]);
	// 
	m_nCurrentDlg = CHILDDLGKEY::EC;  // Eurocode2-2:05, PSC
	m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);  
	dlg->CreateInit(pPlaceHolder); 

	CDgnCRCDgnParameter_EC2PSC *pUser = (CDgnCRCDgnParameter_EC2PSC*) dlg;
	pUser->m_nAnnex = m_nAnnex;
	pUser->m_pParamData->iDgnCode=m_iDgnCode;
    if ( m_iDgnCode==EUROCODE2_2_05_PSC ) pUser->SetAnnexCtrl();

	if (pDoc->m_pAttrCtrl->ExistPscd()) return;

	// SET ANNEX-DATA
	T_PSCD_D PscdD; PscdD.Initialize();
	if (m_iDgnCode==IRC_112_2011_PSC || m_iDgnCode == IRC_112_2020_PSC)
	{
	  PscdD.Set_IRC112Data();
	  pUser->m_pParamData->Set_IRC112Data();
	}
	else if(m_iDgnCode==EUROCODE2_2_05_PSC)
	{
      pDoc->m_pDataCtrl->SetAnnexData(PscdD.nNationalAnnex, PscdD);
    }
	//
	if(pUser->m_pParamData->dGamma_c_PT  ==0) pUser->m_pParamData->dGamma_c_PT   = PscdD.dGamma_c_PT  ;
	if(pUser->m_pParamData->dGamma_s_PT  ==0) pUser->m_pParamData->dGamma_s_PT   = PscdD.dGamma_s_PT  ;
	if(pUser->m_pParamData->dGamma_ps_PT ==0) pUser->m_pParamData->dGamma_ps_PT  = PscdD.dGamma_ps_PT ;
	if(pUser->m_pParamData->dGamma_c_A   ==0) pUser->m_pParamData->dGamma_c_A    = PscdD.dGamma_c_A   ;
	if(pUser->m_pParamData->dGamma_s_A   ==0) pUser->m_pParamData->dGamma_s_A    = PscdD.dGamma_s_A   ;
	if(pUser->m_pParamData->dGamma_ps_A  ==0) pUser->m_pParamData->dGamma_ps_A   = PscdD.dGamma_ps_A  ;
	if(pUser->m_pParamData->dGamma_c_serv==0) pUser->m_pParamData->dGamma_c_serv = PscdD.dGamma_c_serv;
	if(pUser->m_pParamData->dGamma_s_serv==0) pUser->m_pParamData->dGamma_s_serv = PscdD.dGamma_s_serv;
	if(pUser->m_pParamData->dAlpha_cc    ==0) pUser->m_pParamData->dAlpha_cc     = PscdD.dAlpha_cc    ;
	if(pUser->m_pParamData->dAlphacc4Shear==0) pUser->m_pParamData->dAlphacc4Shear= PscdD.dAlphacc4Shear;
	if(pUser->m_pParamData->dAlpha_ct    ==0) pUser->m_pParamData->dAlpha_ct     = PscdD.dAlpha_ct    ;
	if(pUser->m_pParamData->dk1_sl       ==0) pUser->m_pParamData->dk1_sl        = PscdD.dk1_sl       ;
	if(pUser->m_pParamData->dk3_sl       ==0) pUser->m_pParamData->dk3_sl        = PscdD.dk3_sl       ;
	if(pUser->m_pParamData->dk4_sl       ==0) pUser->m_pParamData->dk4_sl        = PscdD.dk4_sl       ;
	if(pUser->m_pParamData->dk6_sl       ==0) pUser->m_pParamData->dk6_sl        = PscdD.dk6_sl       ;
	if(pUser->m_pParamData->dk1_ps       ==0) pUser->m_pParamData->dk1_ps        = PscdD.dk1_ps       ;
	if(pUser->m_pParamData->dk2_ps       ==0) pUser->m_pParamData->dk2_ps        = PscdD.dk2_ps       ;
	if(pUser->m_pParamData->dk5_ps       ==0) pUser->m_pParamData->dk5_ps        = PscdD.dk5_ps       ;
	if(pUser->m_pParamData->dk7_ps       ==0) pUser->m_pParamData->dk7_ps        = PscdD.dk7_ps       ;
	if(pUser->m_pParamData->dk8_ps       ==0) pUser->m_pParamData->dk8_ps        = PscdD.dk8_ps       ;
	if(pUser->m_pParamData->dk_c_ps_stage==0) pUser->m_pParamData->dk_c_ps_stage = PscdD.dk_c_ps_stage;
	if(pUser->m_pParamData->dk_t_ps_stage==0) pUser->m_pParamData->dk_t_ps_stage = PscdD.dk_t_ps_stage;
	if(pUser->m_pParamData->dk_c_ps      ==0) pUser->m_pParamData->dk_c_ps       = PscdD.dk_c_ps      ;
	if(pUser->m_pParamData->dk_t_ps      ==0) pUser->m_pParamData->dk_t_ps       = PscdD.dk_t_ps      ;
	if(pUser->m_pParamData->dk3_cr       ==0) pUser->m_pParamData->dk3_cr        = PscdD.dk3_cr       ;
	if(pUser->m_pParamData->dk4_cr       ==0) pUser->m_pParamData->dk4_cr        = PscdD.dk4_cr       ;
}

void CDgnCRCDgnParameterDlg::UpDateCurDlg_BS()
{
	CMyChildDialog* dlg;
	CWnd* pPlaceHolder = GetDlgItem(m_iID11[0]);
	// 
	m_nCurrentDlg = CHILDDLGKEY::BS;  // BS5400_90_PSC
	m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);  
	dlg->CreateInit(pPlaceHolder); 

	CDgnCRCDgnParameter_BS *pUser = (CDgnCRCDgnParameter_BS*) dlg;
	pUser->m_pParamData->iDgnCode=m_iDgnCode;
}

void CDgnCRCDgnParameterDlg::UpDateCurDlg_TMH7()
{
	CMyChildDialog* dlg;
	CWnd* pPlaceHolder = GetDlgItem(m_iID20[0]);
	// 
	m_nCurrentDlg = CHILDDLGKEY::TMH7;  // BS5400_90_PSC
	m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
	dlg->CreateInit(pPlaceHolder);

	CDgnCRCDgnParameter_TMH7* pUser = (CDgnCRCDgnParameter_TMH7*)dlg;
}

void CDgnCRCDgnParameterDlg::UpDateCurDlg_ABNT()
{
	CMyChildDialog* dlg;
	CWnd* pPlaceHolder = GetDlgItem(m_iID10[0]);
	// 
  m_nCurrentDlg = CHILDDLGKEY::ABNT;  // ABNT_NBR_6118_2023_PSC
	m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);
	dlg->CreateInit(pPlaceHolder);

	CDgnCRCDgnParameter_ABNT* pUser = (CDgnCRCDgnParameter_ABNT*)dlg;
}

void CDgnCRCDgnParameterDlg::UpDateCurDlg_CSA19()
{
	CMyChildDialog* dlg;
	CWnd* pPlaceHolder = GetDlgItem(m_iID19[0]);
	// 
	m_nCurrentDlg = CHILDDLGKEY::CSA19;  // CSA_S6_19
	m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);  
	dlg->CreateInit(pPlaceHolder); 

	CDgnCRCDgnParameter_CSA_19 *pUser = (CDgnCRCDgnParameter_CSA_19*) dlg;
	pUser->m_pParamData->iDgnCode=m_iDgnCode;
}

void CDgnCRCDgnParameterDlg::UpDateCurDlg_CSA()
{
	CMyChildDialog* dlg;
	CWnd* pPlaceHolder = GetDlgItem(m_iID12[0]);
	// 
	m_nCurrentDlg = CHILDDLGKEY::CSA;  // CSA_S6_10
	m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);  
	dlg->CreateInit(pPlaceHolder); 
	
	CDgnCRCDgnParameter_CSA *pUser = (CDgnCRCDgnParameter_CSA*) dlg;
}

void CDgnCRCDgnParameterDlg::UpDateCurDlg_AASHTO12()
{
	CMyChildDialog* dlg;
	CWnd* pPlaceHolder = GetDlgItem(m_iID13[0]);
	// 
	m_nCurrentDlg = CHILDDLGKEY::AASHTO;  // AASHTO12
	m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);  
	dlg->CreateInit(pPlaceHolder); 
	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	T_PSCD_D PscdD;
	BOOL bInit = FALSE;
	if (!pDoc->m_pAttrCtrl->GetPscd(PscdD)) bInit = TRUE;
	m_nAnnex = CDlgUtil::CobxGetCurSelItemData(m_DesignAmendment, m_DesignAmendment.GetCurSel());;

	CDgnCRCDgnParameter_AASHTO12 *pUser = (CDgnCRCDgnParameter_AASHTO12*) dlg;
	pUser->m_pParamData->iDgnCode=m_iDgnCode;
	pUser->m_bInitData = bInit;
	pUser->m_nAnnex = m_nAnnex;
	pUser->SetAnnexCtrl();
}

void CDgnCRCDgnParameterDlg::UpDateCurDlg_LSD15()
{
	CMyChildDialog* dlg;
	CWnd* pPlaceHolder = GetDlgItem(m_iID14[0]);
	// 
	m_nCurrentDlg = CHILDDLGKEY::KSCE;  // KSCE-LSD15, PSC
	m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);  
	dlg->CreateInit(pPlaceHolder); 
	
	CDgnCRCDgnParameter_LSDPSC *pUser = (CDgnCRCDgnParameter_LSDPSC*) dlg;
	pUser->m_pParamData->iDgnCode=m_iDgnCode;

	// SET ANNEX-DATA
	T_PSCD_D PscdD; PscdD.Initialize();
	PscdD.Set_AnnexData_LSD();
	//

	if(pUser->m_pParamData->dPhi_c[0] ==0) pUser->m_pParamData->dPhi_c[0]   = PscdD.dPhi_c[0]  ;
	if(pUser->m_pParamData->dPhi_s[0] ==0) pUser->m_pParamData->dPhi_s[0]   = PscdD.dPhi_s[0]  ;
	if(pUser->m_pParamData->dPhi_ps[0]==0) pUser->m_pParamData->dPhi_ps[0]  = PscdD.dPhi_ps[0] ;
	if(pUser->m_pParamData->dPhi_c[2] ==0) pUser->m_pParamData->dPhi_c[2]   = PscdD.dPhi_c[2]  ;
	if(pUser->m_pParamData->dPhi_s[2] ==0) pUser->m_pParamData->dPhi_s[2]   = PscdD.dPhi_s[2]  ;
	if(pUser->m_pParamData->dPhi_ps[2]==0) pUser->m_pParamData->dPhi_ps[2]  = PscdD.dPhi_ps[2] ;
	if(pUser->m_pParamData->dk1_sl       ==0) pUser->m_pParamData->dk1_sl        = PscdD.dk1_sl       ;
	if(pUser->m_pParamData->dk3_sl       ==0) pUser->m_pParamData->dk3_sl        = PscdD.dk3_sl       ;
	if(pUser->m_pParamData->dk4_sl       ==0) pUser->m_pParamData->dk4_sl        = PscdD.dk4_sl       ;
	if(pUser->m_pParamData->dk6_sl       ==0) pUser->m_pParamData->dk6_sl        = PscdD.dk6_sl       ;
	if(pUser->m_pParamData->dk1_ps       ==0) pUser->m_pParamData->dk1_ps        = PscdD.dk1_ps       ;
	if(pUser->m_pParamData->dk2_ps       ==0) pUser->m_pParamData->dk2_ps        = PscdD.dk2_ps       ;
	if(pUser->m_pParamData->dk5_ps       ==0) pUser->m_pParamData->dk5_ps        = PscdD.dk5_ps       ;
	if(pUser->m_pParamData->dk7_ps       ==0) pUser->m_pParamData->dk7_ps        = PscdD.dk7_ps       ;
	if(pUser->m_pParamData->dk8_ps       ==0) pUser->m_pParamData->dk8_ps        = PscdD.dk8_ps       ;
	if(pUser->m_pParamData->dk_c_ps_stage==0) pUser->m_pParamData->dk_c_ps_stage = PscdD.dk_c_ps_stage;
	if(pUser->m_pParamData->dk_t_ps_stage==0) pUser->m_pParamData->dk_t_ps_stage = PscdD.dk_t_ps_stage;
	if(pUser->m_pParamData->dk_c_ps      ==0) pUser->m_pParamData->dk_c_ps       = PscdD.dk_c_ps      ;
	if(pUser->m_pParamData->dk_t_ps      ==0) pUser->m_pParamData->dk_t_ps       = PscdD.dk_t_ps      ;
}

void CDgnCRCDgnParameterDlg::UpdateCurDlg_SNiP()
{
	CMyChildDialog* dlg;
	CWnd* pPlaceHolder = GetDlgItem(m_iID15[0]);
	// 
	m_nCurrentDlg = CHILDDLGKEY::SNIP;
	m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);  
	dlg->CreateInit(pPlaceHolder); 
	
	CDgnCRCDgnParameter_RUS *pUser = (CDgnCRCDgnParameter_RUS*) dlg;
	pUser->m_pData->iDgnCode=m_iDgnCode;
	pUser->Data2Dlg();
}

void CDgnCRCDgnParameterDlg::UpdateCurDlg_SP()
{
	CMyChildDialog* dlg;
	CWnd* pPlaceHolder = GetDlgItem(m_iID16[0]);
	// 
	m_nCurrentDlg = CHILDDLGKEY::SP;
	m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);  
	dlg->CreateInit(pPlaceHolder); 
	
	CDgnCRCDgnParameter_RUS *pUser = (CDgnCRCDgnParameter_RUS*) dlg;
	pUser->m_pData->iDgnCode=m_iDgnCode;
	pUser->Data2Dlg();
}

void CDgnCRCDgnParameterDlg::UpDateCurDlg_AS5100()
{
	CMyChildDialog* dlg;
	CWnd* pPlaceHolder = GetDlgItem(m_iID17[0]);
	// 
	m_nCurrentDlg = CHILDDLGKEY::AS;
	m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);  
	dlg->CreateInit(pPlaceHolder); 

	CDgnCRCDgnParameter_AS5100 *pUser = (CDgnCRCDgnParameter_AS5100*) dlg;
	pUser->m_pParamData->iDgnCode=m_iDgnCode;
}

void CDgnCRCDgnParameterDlg::UpDateCurDlg_IRS()
{
	CMyChildDialog* dlg;
	CWnd* pPlaceHolder = GetDlgItem(m_iID18[0]);
	// 
	m_nCurrentDlg = CHILDDLGKEY::IRS;
	m_mapChildDlg.Lookup(m_nCurrentDlg, dlg);  
	dlg->CreateInit(pPlaceHolder); 

	CDgnCRCDgnParameter_IRS *pUser = (CDgnCRCDgnParameter_IRS*) dlg;
	pUser->m_pParamData->iDgnCode=m_iDgnCode;

	T_PSCD_D PscdD; PscdD.Initialize();
	PscdD.Set_IRSData();
    
    pUser->m_pParamData->Set_IRSData();
	if(pUser->m_pParamData->dPhi_c[0] ==0) pUser->m_pParamData->dPhi_c[0]   = PscdD.dPhi_c[0]  ;
	if(pUser->m_pParamData->dPhi_s[0] ==0) pUser->m_pParamData->dPhi_s[0]   = PscdD.dPhi_s[0]  ;
	if(pUser->m_pParamData->dPhi_ps[0]==0) pUser->m_pParamData->dPhi_ps[0]  = PscdD.dPhi_ps[0] ;
	if(pUser->m_pParamData->dPhi_c[2] ==0) pUser->m_pParamData->dPhi_c[2]   = PscdD.dPhi_c[2]  ;
	if(pUser->m_pParamData->dPhi_s[2] ==0) pUser->m_pParamData->dPhi_s[2]   = PscdD.dPhi_s[2]  ;
	if(pUser->m_pParamData->dPhi_ps[2]==0) pUser->m_pParamData->dPhi_ps[2]  = PscdD.dPhi_ps[2] ;
	if(pUser->m_pParamData->dk1_sl       ==0) pUser->m_pParamData->dk1_sl        = PscdD.dk1_sl       ;
	if(pUser->m_pParamData->dk3_sl       ==0) pUser->m_pParamData->dk3_sl        = PscdD.dk3_sl       ;
	if(pUser->m_pParamData->dk4_sl       ==0) pUser->m_pParamData->dk4_sl        = PscdD.dk4_sl       ;
	if(pUser->m_pParamData->dk6_sl       ==0) pUser->m_pParamData->dk6_sl        = PscdD.dk6_sl       ;
	if(pUser->m_pParamData->dk1_ps       ==0) pUser->m_pParamData->dk1_ps        = PscdD.dk1_ps       ;
	if(pUser->m_pParamData->dk2_ps       ==0) pUser->m_pParamData->dk2_ps        = PscdD.dk2_ps       ;
	if(pUser->m_pParamData->dk5_ps       ==0) pUser->m_pParamData->dk5_ps        = PscdD.dk5_ps       ;
	if(pUser->m_pParamData->dk7_ps       ==0) pUser->m_pParamData->dk7_ps        = PscdD.dk7_ps       ;
	if(pUser->m_pParamData->dk8_ps       ==0) pUser->m_pParamData->dk8_ps        = PscdD.dk8_ps       ;
	if(pUser->m_pParamData->dk_c_ps_stage==0) pUser->m_pParamData->dk_c_ps_stage = PscdD.dk_c_ps_stage;
	if(pUser->m_pParamData->dk_t_ps_stage==0) pUser->m_pParamData->dk_t_ps_stage = PscdD.dk_t_ps_stage;
	if(pUser->m_pParamData->dk_c_ps      ==0) pUser->m_pParamData->dk_c_ps       = PscdD.dk_c_ps      ;
	if(pUser->m_pParamData->dk_t_ps      ==0) pUser->m_pParamData->dk_t_ps       = PscdD.dk_t_ps      ;
}

void CDgnCRCDgnParameterDlg::OnClickSpecialLoad()
{
	/*
IDC_JTG_DESIGN_PARAMETER9
IDC_JTG_DESIGN_PARAMETER10
IDC_JTG_DESIGN_PARAMETER11
IDC_JTG_DESIGN_PARAMETER12
IDC_JTG_DESIGN_PARAMETER18
IDC_JTG_DESIGN_PARAMETER19
IDC_JTG_DESIGN_PARAMETER21
IDC_JTG_DESIGN_PARAMETER22
		*/

	 if(m_iDgnCode==CJJ_11_2011 || m_iDgnCode==JTG_D62_04)
	 {

	 }
	 else
		 return;

	 BOOL bChecked = IsDlgButtonChecked(IDC_CRC_DGN_CHK_SPECIALLOAD);
	 if(m_iDgnCode==CJJ_11_2011 && bChecked)
	 {
		 GetDlgItem(IDC_JTG_DESIGN_PARAMETER9 )->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_SPECIALLOAD ));
		 GetDlgItem(IDC_JTG_DESIGN_PARAMETER10)->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_SPECIALLOAD1));
		 GetDlgItem(IDC_JTG_DESIGN_PARAMETER11)->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_SPECIALLOAD2));
		 GetDlgItem(IDC_JTG_DESIGN_PARAMETER12)->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_SPECIALLOAD3));
		 GetDlgItem(IDC_JTG_DESIGN_PARAMETER18)->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_CRACKWIDTH1));
		 GetDlgItem(IDC_JTG_DESIGN_PARAMETER19)->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_CRACKWIDTH2));
		 GetDlgItem(IDC_JTG_DESIGN_PARAMETER21)->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_CRACKWIDTH3));
		 GetDlgItem(IDC_JTG_DESIGN_PARAMETER22)->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_CRACKWIDTH4));
	 }
	 else if(m_iDgnCode==JTG_D62_04 || (m_iDgnCode==CJJ_11_2011 && !bChecked))
	 {
		 GetDlgItem(IDC_JTG_DESIGN_PARAMETER9 )->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_ROADGRADE ));
		 GetDlgItem(IDC_JTG_DESIGN_PARAMETER10)->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_ROADGRADE1));
		 GetDlgItem(IDC_JTG_DESIGN_PARAMETER11)->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_ROADGRADE2));
		 GetDlgItem(IDC_JTG_DESIGN_PARAMETER12)->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_ROADGRADE3));
		 GetDlgItem(IDC_JTG_DESIGN_PARAMETER18)->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_JTG_CRACKWIDTH1));
		 GetDlgItem(IDC_JTG_DESIGN_PARAMETER19)->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_JTG_CRACKWIDTH2));
		 GetDlgItem(IDC_JTG_DESIGN_PARAMETER21)->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_JTG_CRACKWIDTH3));
		 GetDlgItem(IDC_JTG_DESIGN_PARAMETER22)->SetWindowText(_LS(IDS_DGN_DESIGN_PARM_JTG_CRACKWIDTH4));

	 }
	 else
	 {

	 }

}