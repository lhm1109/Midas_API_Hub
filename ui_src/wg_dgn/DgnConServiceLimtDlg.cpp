// Coded by Seungjun MNet:No.2393 ('20060829)
// DgnConServiceLimtDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConServiceLimtDlg.h"
#include "DgnDataCtrl.h"
#include "DgnCodeCtrl.h"
#include "DgnConDlg.h"
#include "DgnCpgDlg.h"
#include "DgnStruct.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_dbLock\LockOption.h"
#include "..\wg_db\DBCodeDef.h"

#include "..\wg_main\wg_mainres2.h"	

#include "..\dgnengine\idesign\DGN_lib\NationalAnnexInterface.h"
#include "..\dgnengine\idesign\DGN_def\DgnTypeEnumDef.h"
#include "..\dgnengine\idesign\DGN_def\DgnEUNationalAnnexDef.h"
#include "DgnTypeConverter.h"

using namespace dgn::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConServiceLimtDlg dialog


CDgnConServiceLimtDlg::CDgnConServiceLimtDlg(BOOL bSlabServ/* = FALSE*/, CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CDgnConServiceLimtDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConServiceLimtDlg)
	m_nOption = 0;
	m_iCrackEnv = 0;
	m_iDeflectEnv = 0;
	m_dUserDeflect = 0.0;
	m_iDeflectEnv_Long = 0;
	m_dUserDeflect_Long = 0.0;
	m_dk1 = 0.6;
	m_dk2 = 0.45;
	m_dk3 = 0.8;
	m_dk4 = 1.0;
	m_dRH = 0.0;
	m_dDAF = 0.0;
	m_iSeletType = 1;
	m_dCharacLim = 250.0;
	m_bCharCrack = FALSE;
	m_bFreqCrack = FALSE;
	m_bQuasCrack = FALSE;

	m_iServLife = 1;
	m_bCorrosionHigh = FALSE;
	m_bCrackW = FALSE;
	m_bIsCsgDesign = FALSE;
	m_bSlabService = bSlabServ;
	//}}AFX_DATA_INIT
	//m_iCurType = 0;
	m_iCode = 0;
	m_iSubCode = 0;
	m_dStrainLimit		= 0.0;
	m_dStrainGrowthRate	= 0.0;
	m_dK		= 0.0;

	m_arExpMoveCtrl.RemoveAll();
	m_arStressMoveCtrl.RemoveAll();
	m_arCrack1MoveCtrl.RemoveAll();
	m_arCrack2MoveCtrl.RemoveAll();
	m_arCrack2UserMoveCtrl.RemoveAll();
	m_arDeflectMoveCtrl.RemoveAll();
	m_arDeflectUserMoveCtrl.RemoveAll();
	m_arBtnMoveCtrl.RemoveAll();
	m_arSwedenMoveCtrl.RemoveAll();
	m_arCrackWidth.RemoveAll();
	m_arServiceLifeCtrl.RemoveAll();
	m_arCorrosionCtrl.RemoveAll();

	//Add Member types
	m_arExpMoveCtrl.Add(IDC_DGN_CONC_ExpClassFrm);
	m_arExpMoveCtrl.Add(IDC_DGN_CONC_EXP1);
	m_arExpMoveCtrl.Add(IDC_DGN_CONC_EXP2);

	m_arStressMoveCtrl.Add(IDC_DGN_CONC_StressFrm);
	m_arStressMoveCtrl.Add(IDC_DGN_CONC_STRESS1);
	m_arStressMoveCtrl.Add(IDC_DGN_CONC_STRESS2);
	m_arStressMoveCtrl.Add(IDC_DGN_CONC_STRESS3);
	m_arStressMoveCtrl.Add(IDC_DGN_CONC_STRESS4);
	m_arStressMoveCtrl.Add(IDC_DGN_CONC_STRESS5);
	m_arStressMoveCtrl.Add(IDC_DGN_CONC_STRESS6);
	m_arStressMoveCtrl.Add(IDC_DGN_CONC_STRESS7);
	m_arStressMoveCtrl.Add(IDC_DGN_CONC_STRESS8);

	m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_CrackFrm1); // for Eurocode2. Relative humidity
	//m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_CRACK1);
	//m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_CRACK2);
	//m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_CRACK3);
	m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_CrackFrm11); // for Eurocode2. Characteristic
	m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_CharCrack);
	m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_CRACK4);
	m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_CRACK5);
	m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_CRACK6);
	m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_CrackFrm12); // for Eurocode2.  Frequent
	m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_FreqCrack);
	m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_CRACK7);
	m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_CRACK8);
	m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_CRACK9);
	m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_CrackFrm13); // for Eurocode2.  Quasi-permanent
	m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_QuasCrack);
	m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_CRACK10);
	m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_CRACK11);
	m_arCrack1MoveCtrl.Add(IDC_DGN_CONC_CRACK12);  

	m_arCrack2MoveCtrl.Add(IDC_DGN_CONC_CrackFrm2); // for General.
	m_arCrack2MoveCtrl.Add(IDC_DGN_CONC_BCRACK1);
	m_arCrack2MoveCtrl.Add(IDC_DGN_CONC_BCRACK2);
	m_arCrack2MoveCtrl.Add(IDC_DGN_CONC_BCRACK3);
	m_arCrack2MoveCtrl.Add(IDC_DGN_CONC_BCRACK4);
	
	m_arCrack2UserMoveCtrl.Add(IDC_DGN_CONC_BCRACK5);
	m_arCrack2UserMoveCtrl.Add(IDC_DGN_CONC_BCRACK6);
	m_arCrack2UserMoveCtrl.Add(IDC_DGN_CONC_BCRACK7);

	m_arDeflectMoveCtrl.Add(IDC_DGN_CONC_DeflectFrm);
	m_arDeflectMoveCtrl.Add(IDC_DGN_CONC_BDEFLECT1);
	m_arDeflectMoveCtrl.Add(IDC_DGN_CONC_BDEFLECT2);
	m_arDeflectMoveCtrl.Add(IDC_DGN_CONC_BDEFLECT3);
	m_arDeflectMoveCtrl.Add(IDC_DGN_CONC_BDEFLECT4);

	m_arDeflectUserMoveCtrl.Add(IDC_DGN_CONC_BDEFLECT5);
	m_arDeflectUserMoveCtrl.Add(IDC_DGN_CONC_BDEFLECT6);
	m_arDeflectUserMoveCtrl.Add(IDC_DGN_CONC_DeflectFrm3);
	m_arDeflectUserMoveCtrl.Add(IDC_DGN_CONC_DEFLECT1);
	m_arDeflectUserMoveCtrl.Add(IDC_DGN_CONC_DEFLECT2);
	m_arDeflectUserMoveCtrl.Add(IDC_DGN_CONC_DeflectFrm2);
	m_arDeflectUserMoveCtrl.Add(IDC_DGN_CONC_BDEFLECT7);

	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_CrackFrm1);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_CrackFrm11);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_CrackFrm12);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_CrackFrm13);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_CRACK4);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_CRACK5);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_CRACK6);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_CRACK7);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_CRACK8);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_CRACK9);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_CRACK10);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_CRACK11);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_CRACK12);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_CharCrack);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_FreqCrack);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_QuasCrack);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_DeflectFrm);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_BDEFLECT1);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_BDEFLECT2);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_BDEFLECT3);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_BDEFLECT4);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_BDEFLECT5);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_BDEFLECT6);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_DeflectFrm3);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_DEFLECT1);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_DEFLECT2);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_DeflectFrm2);
	m_arSwedenMoveCtrl.Add(IDC_DGN_CONC_BDEFLECT7);	

	m_arCrackWidth.Add(IDC_DGN_CONC_CRACK_WIDTH_GRP);
	m_arCrackWidth.Add(IDC_DGN_CONC_CRACK_WIDTH_CHK);
	m_arCrackWidth.Add(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_STC);
	m_arCrackWidth.Add(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_EDT);
	m_arCrackWidth.Add(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_UNT);

	m_arLongDeflect.Add(IDC_DGN_CONC_DeflectFrm5);
	m_arLongDeflect.Add(IDC_DGN_CONC_LIVE_LOAD_REDUCTION_EDT);
	//m_arLongDeflect.Add(IDC_DGN_CONC_LIVE_LOAD_REDUCTION_UNT);
	m_arLongDeflect.Add(IDC_DGN_CONC_DeflectFrm6);
	m_arLongDeflect.Add(IDC_DGN_CONC_TIME_DEP_XI_STC);
	m_arLongDeflect.Add(IDC_DGN_CONC_TIME_DEP_EDT);
	m_arLongDeflect.Add(IDC_DGN_CONC_TIME_DEP_CMB);
	m_arLongDeflect.Add(IDC_DGN_CONC_TIME_DEP_XI2_STC);
	m_arLongDeflect.Add(IDC_DGN_CONC_DeflectFrm4);
	m_arLongDeflect.Add(IDC_DGN_CONC_BDEFLECT8);
	m_arLongDeflect.Add(IDC_DGN_CONC_BDEFLECT9);
	m_arLongDeflect.Add(IDC_DGN_CONC_BDEFLECT10);
	m_arLongDeflect.Add(IDC_DGN_CONC_BDEFLECT11);
	m_arLongDeflect.Add(IDC_DGN_CONC_BDEFLECT12);
	m_arLongDeflect.Add(IDC_DGN_CONC_BDEFLECT13);

	m_arAIJ_WSD99_MoveCtrl.Add(IDC_DGN_CONC_DEFLECTION_REVIEW_GRUP);
	m_arAIJ_WSD99_MoveCtrl.Add(IDC_DGN_CONC_STRAIN_LIMIT_STC);
	m_arAIJ_WSD99_MoveCtrl.Add(IDC_DGN_CONC_STRAIN_LIMIT_EDT);
	m_arAIJ_WSD99_MoveCtrl.Add(IDC_DGN_CONC_STRAIN_GROWTH_RATE_STC);
	m_arAIJ_WSD99_MoveCtrl.Add(IDC_DGN_CONC_STRAIN_GROWTH_RATE_EDT);
	m_arAIJ_WSD99_MoveCtrl.Add(IDC_DGN_CONC_CRACK_REVIEW_GRUP);
	m_arAIJ_WSD99_MoveCtrl.Add(IDC_DGN_CONC_CRACK_REVIEW_STC);
	m_arAIJ_WSD99_MoveCtrl.Add(IDC_DGN_CONC_CRACK_REVIEW_STC2);
	m_arAIJ_WSD99_MoveCtrl.Add(IDC_DGN_CONC_CRACK_REVIEW_EDT);

	m_arServiceLifeCtrl.Add(IDC_DGN_CONC_ServiceLifeFrm);
	m_arServiceLifeCtrl.Add(IDC_DGN_CONC_SERVLIFE1_RDO);
	m_arServiceLifeCtrl.Add(IDC_DGN_CONC_SERVLIFE2_RDO);
	m_arServiceLifeCtrl.Add(IDC_DGN_CONC_SERVLIFE3_RDO);

	m_arCorrosionCtrl.Add(IDC_DGN_CONC_CrackDetFrm);
	m_arCorrosionCtrl.Add(IDC_DGN_CONC_Crack_Corrosion_Low_RDO);
	m_arCorrosionCtrl.Add(IDC_DGN_CONC_Crack_Corrosion_High_RDO);
/*
	m_arCharDeflectUserMoveCtrl.Add(IDC_DGN_CONC_DeflectFrm3);
	m_arCharDeflectUserMoveCtrl.Add(IDC_DGN_CONC_DEFLECT1);
	m_arCharDeflectUserMoveCtrl.Add(IDC_DGN_CONC_DEFLECT2);
	m_arCharDeflectUserMoveCtrl.Add(IDC_DGN_CONC_DeflectFrm2);
	m_arCharDeflectUserMoveCtrl.Add(IDC_DGN_CONC_BDEFLECT7);
*/	

	if(m_arBeamData.GetCount()>0) m_arBeamData.RemoveAll();

	m_pDoc = CDBDoc::GetDocPoint();
}


void CDgnConServiceLimtDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConServiceLimtDlg)
	DDX_Control(pDX, IDC_DGN_CONC_EXP2,      m_ctrEC2ExpComb  );
	DDX_Radio  (pDX, IDC_DGN_CON_SERV_AR,    m_nOption        );
	DDX_Radio  (pDX, IDC_DGN_CONC_BCRACK1,   m_iCrackEnv      );
	DDX_Radio  (pDX, IDC_DGN_CONC_BDEFLECT1, m_iDeflectEnv    );
	DDX_Radio  (pDX, IDC_DGN_CONC_BDEFLECT8, m_iDeflectEnv_Long);
	DDX_Control(pDX, IDC_DGN_CONC_BCRACK6,   m_dUserCrack     );
	DDX_Control(pDX, IDC_DGN_CONC_CRACK5,    m_dCharCrack     );
	DDX_Control(pDX, IDC_DGN_CONC_CRACK8,    m_dFreqCrack     );
	DDX_Control(pDX, IDC_DGN_CONC_CRACK11,   m_dQuasCrack     );
	DDX_Text   (pDX, IDC_DGN_CONC_BDEFLECT6, m_dUserDeflect   );
	DDX_Text   (pDX, IDC_DGN_CONC_BDEFLECT13, m_dUserDeflect_Long);
	DDX_Text   (pDX, IDC_DGN_CONC_LIVE_LOAD_REDUCTION_EDT, m_dReductFactor);
	DDX_Control(pDX, IDC_DGN_CONC_BCRACK7,   m_dCrackCurUnit1 );
	DDX_Control(pDX, IDC_DGN_CONC_CRACK6,    m_dCrackCurUnit2 );
	DDX_Control(pDX, IDC_DGN_CONC_CRACK9,    m_dCrackCurUnit3 );
	DDX_Control(pDX, IDC_DGN_CONC_CRACK12,   m_dCrackCurUnit4 );
	DDX_Text   (pDX, IDC_DGN_CONC_STRESS2,   m_dk1            );
	DDX_Text   (pDX, IDC_DGN_CONC_STRESS4,   m_dk2            );
	DDX_Text   (pDX, IDC_DGN_CONC_STRESS6,   m_dk3            );
	DDX_Text   (pDX, IDC_DGN_CONC_STRESS8,   m_dk4            );
	DDX_Text   (pDX, IDC_DGN_CONC_CRACK2,    m_dRH            );
	DDX_Text   (pDX, IDC_DGN_CONC_BDEFLECT7, m_dDAF           );
	DDX_Radio  (pDX, IDC_DGN_CONC_SELECT_ALL,m_iSeletType     );
	DDX_Text   (pDX, IDC_DGN_CONC_DEFLECT2,  m_dCharacLim     );
	DDX_Check  (pDX, IDC_DGN_CONC_CharCrack, m_bCharCrack     );
	DDX_Check  (pDX, IDC_DGN_CONC_FreqCrack, m_bFreqCrack     );
	DDX_Check  (pDX, IDC_DGN_CONC_QuasCrack, m_bQuasCrack     );

	DDX_Control(pDX, IDC_DGN_CONC_SERVLIFE1_RDO, m_RdoServLife100);
	DDX_Control(pDX, IDC_DGN_CONC_SERVLIFE2_RDO, m_RdoServLife50);
	DDX_Control(pDX, IDC_DGN_CONC_SERVLIFE3_RDO, m_RdoServLife20);

	DDX_Control(pDX, IDC_DGN_CONC_Crack_Corrosion_High_RDO, m_RdoCorrosionHigh);
	DDX_Control(pDX, IDC_DGN_CONC_Crack_Corrosion_Low_RDO,  m_RdoCorrosionLow);
	DDX_Check  (pDX, IDC_DGN_CONC_CRACK_WIDTH_CHK,       m_bCrackW);
	DDX_Control(pDX, IDC_DGN_CONC_CRACK_WIDTH_LIMIT_EDT, m_edtCrackWLimit);
	DDX_Control(pDX, IDC_DGN_CONC_CRACK_WIDTH_LIMIT_UNT, m_untCrackWLimit);
	DDX_Control(pDX, IDC_DGN_CONC_LIVE_LOAD_REDUCTION_EDT, m_edtReductFactor);
	//DDX_Control(pDX, IDC_DGN_CONC_LIVE_LOAD_REDUCTION_UNT, m_untReductFactor);
	DDX_Control(pDX, IDC_DGN_CONC_TIME_DEP_CMB, m_cmbTimeDep);
	DDX_Control(pDX, IDC_DGN_CONC_TIME_DEP_EDT, m_edtTimeDep);
	DDX_Text(pDX, IDC_DGN_CONC_STRAIN_LIMIT_EDT, m_dStrainLimit);
	DDX_Text(pDX, IDC_DGN_CONC_STRAIN_GROWTH_RATE_EDT, m_dStrainGrowthRate);
	DDX_Text(pDX, IDC_DGN_CONC_CRACK_REVIEW_EDT, m_dK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConServiceLimtDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CDgnConServiceLimtDlg)
	ON_BN_CLICKED(IDC_DGN_CON_SERV_AR,    OnDgnConServAr       )
	ON_BN_CLICKED(IDC_DGN_CON_SERV_DEL,   OnDgnConServDel      )
	ON_BN_CLICKED(IDC_DGN_CONC_BCRACK1,   OnDgnConCrackUser    )
	ON_BN_CLICKED(IDC_DGN_CONC_BDEFLECT1, OnDgnConDeflectUser  )
	ON_BN_CLICKED(IDC_DGN_CONC_BCRACK2,   OnDgnConCrackUser    )
	ON_BN_CLICKED(IDC_DGN_CONC_BCRACK3,   OnDgnConCrackUser    )
	ON_BN_CLICKED(IDC_DGN_CONC_BCRACK4,   OnDgnConCrackUser    )
	ON_BN_CLICKED(IDC_DGN_CONC_BCRACK5,   OnDgnConCrackUser    )
	ON_BN_CLICKED(IDC_DGN_CONC_BDEFLECT2, OnDgnConDeflectUser  )
	ON_BN_CLICKED(IDC_DGN_CONC_BDEFLECT3, OnDgnConDeflectUser  )
	ON_BN_CLICKED(IDC_DGN_CONC_BDEFLECT4, OnDgnConDeflectUser  )
	ON_BN_CLICKED(IDC_DGN_CONC_BDEFLECT5, OnDgnConDeflectUser  )
	ON_BN_CLICKED(IDC_DGN_CONC_CharCrack, OnDgnConCrackcCheck  )
	ON_BN_CLICKED(IDC_DGN_CONC_FreqCrack, OnDgnConCrackcCheck  )
	ON_BN_CLICKED(IDC_DGN_CONC_QuasCrack, OnDgnConCrackcCheck  )
	ON_BN_CLICKED(IDC_DGN_CONC_BDEFLECT8, OnDgnConDeflectUserLong)
	ON_BN_CLICKED(IDC_DGN_CONC_BDEFLECT9, OnDgnConDeflectUserLong)
	ON_BN_CLICKED(IDC_DGN_CONC_BDEFLECT10, OnDgnConDeflectUserLong)
	ON_BN_CLICKED(IDC_DGN_CONC_BDEFLECT11, OnDgnConDeflectUserLong)
	ON_BN_CLICKED(IDC_DGN_CONC_BDEFLECT12, OnDgnConDeflectUserLong)
	ON_CBN_SELCHANGE(IDC_DGN_CONC_EXP2,   OnSelchangeDgnConcExp)
	ON_CBN_SELCHANGE(IDC_DGN_CONC_TIME_DEP_CMB, OnSelChangeTimeCmb)
	ON_BN_CLICKED(IDC_DGN_CONC_SERVLIFE1_RDO,              OnDgnConServLife)
	ON_BN_CLICKED(IDC_DGN_CONC_SERVLIFE2_RDO,              OnDgnConServLife)
	ON_BN_CLICKED(IDC_DGN_CONC_SERVLIFE3_RDO,              OnDgnConServLife)
	ON_BN_CLICKED(IDC_DGN_CONC_Crack_Corrosion_High_RDO,   OnDgnConCorrosion)
	ON_BN_CLICKED(IDC_DGN_CONC_Crack_Corrosion_Low_RDO,    OnDgnConCorrosion)
	ON_BN_CLICKED(IDC_DGN_CONC_CRACK_WIDTH_CHK,            OnDgnCrackWidthChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConServiceLimtDlg message handlers

BOOL CDgnConServiceLimtDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	if(m_pDoc==NULL)	return TRUE;
	
    const CString& strCodeName = CDBLib::GetConCodeName();

    if ( strCodeName == CONCODE_IRC112_2011 || strCodeName == CONCODE_IRC112_2020)
	{
		CString asrtIRCExpList[4] = {_T("Moderate"),_T("Severe"),_T("Very Severe"),_T("Extreme")};
        if ( m_ctrEC2ExpComb.GetCount() > 0 )
        {
            m_ctrEC2ExpComb.ResetContent();
        }
        for ( int i=0; i<4; i++ )
        {
            m_ctrEC2ExpComb.AddString(asrtIRCExpList[i]);
        }
	}
    else if ( strCodeName == CONCODE_IRS )
	{
		CString asrtIRCExpList[4] = {_T("Moderate"),_T("Severe"),_T("Very Severe"),_T("Extreme")};
        if ( m_ctrEC2ExpComb.GetCount() > 0 )
        {
            m_ctrEC2ExpComb.ResetContent();
        }
        for ( int i=0; i<4; i++ )
        {
            m_ctrEC2ExpComb.AddString(asrtIRCExpList[i]);
        }
	}
	else if (strCodeName == CONCODE_AREMA_2023)
	{
		CString asrtIRCExpList[2] = { _T("Moderate"),_T("Severe")};
		if (m_ctrEC2ExpComb.GetCount() > 0)
		{
			m_ctrEC2ExpComb.ResetContent();
		}
		for (int i = 0; i < 2; i++)
		{
			m_ctrEC2ExpComb.AddString(asrtIRCExpList[i]);
		}
	}
	else
	{
		CString asrtEC2ExpList[X_NUM] = { _T("X0"),_T("XC1"),_T("XC2"),_T("XC3"),_T("XC4"),_T("XD1"),_T("XD2"),_T("XD3"),_T("XS1"),_T("XS2"),_T("XS3"),
																_T("XF1"),_T("XF2"),_T("XF3"),_T("XF4"),_T("XA1"),_T("XA2"),_T("XA3") };

		if (m_ctrEC2ExpComb.GetCount() > 0)
		{
			m_ctrEC2ExpComb.ResetContent();
		}
		for (int i = 0; i < X_NUM; i++)
		{
			m_ctrEC2ExpComb.AddString(asrtEC2ExpList[i]);
		}
	}

	GetDlgItem(IDC_DGN_CONC_CRACK1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CONC_CRACK2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CONC_CRACK3)->ShowWindow(SW_HIDE);

	m_cmbTimeDep.ResetContent();
	m_cmbTimeDep.AddString(_LS(IDS_GPS_SLAB_SERV_CHK_TIME_5Y ));
	m_cmbTimeDep.AddString(_LS(IDS_GPS_SLAB_SERV_CHK_TIME_12M));
	m_cmbTimeDep.AddString(_LS(IDS_GPS_SLAB_SERV_CHK_TIME_6M ));
	m_cmbTimeDep.AddString(_LS(IDS_GPS_SLAB_SERV_CHK_TIME_3M ));
	m_cmbTimeDep.AddString(_LS(IDS_GPS_SLAB_SERV_CHK_USER_INPUT)); // TIME_CMB_IDX_USER
	m_cmbTimeDep.SetCurSel(0);

	OnSelChangeTimeCmb();

	// Set Current Design Code.
	if(m_bIsCsgDesign)
	{
		if(!GetDesignCsgCodeNo()) ASSERT(0);
	}
	else
	{
		m_iCode    = CDgnCodeCtrl::GetConCodeID(CDBLib::GetConCodeName());
		m_iSubCode = CDBLib::GetConSubCode();
	}

	ChangeDlgItemInfo();

	if (m_iCode == BS5400_4_90)
	{
		m_iCrackEnv = 1; /// BS 5400인 경우, severe를 default로 설정.
	}

	if (m_iCode == AREMA23)
	{
		m_iCrackEnv = 1; ///Rahul
	}

	if (m_iCode == TMH07_89)
	{
		m_iCrackEnv = 1; /// BS 5400인 경우, severe를 default로 설정.
	}
	Initial_Unit();
	Initial_Data();	
	OnDgnCrackWidthChk();

	ResizeDlg();

	// Design Crack Width 막는 것으로 한다. 20131021
	GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_CHK)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConServiceLimtDlg::Initial_StressFactor_EN04(int iIdx)
{
	if(m_iCode==Eurocode2 || m_iCode==Eurocode2_04)
	{
		// EN 1992-1-1:2004 7.2 (2) k1 for stress limitation
		// X0 XC1 XC2 XC3 XC4 XD1 XD2 XD3 XS1 XS2 XS3 XF1 XF2 XF3 XF4 XA1 XA2 XA3
		// 0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17
		switch (m_iSubCode)
		{
			case dgn::def::enNationalAnnex::Belgium:
			{
				if(5 <= iIdx && iIdx <= 14) m_dk1 = 0.5;
				else                        m_dk1 = 0.6;
			}
			break;
			case dgn::def::enNationalAnnex::Poland:	m_dk1 = 1.0; break;
			default:								m_dk1 = 0.6; break; // recommended
		}
			
		// EN 1992-1-1:2004 7.2 (3) k2 for stress limitation
		m_dk2 = 0.45; 
			
		// EN 1992-1-1:2004 7.2 (5) k3, k4 for stress limitation
		switch (m_iSubCode)
		{
		case dgn::def::enNationalAnnex::Netherlands:
		{
			m_dk3 = 0.0;
			m_dk4 = 0.0;
		}
		break;
		case dgn::def::enNationalAnnex::Finland:
		{
			m_dk3 = 0.6;
			m_dk4 = 0.8;
		}
		break;
		case dgn::def::enNationalAnnex::France:
		case dgn::def::enNationalAnnex::Germany:
		{
			m_dk3 = 0.8;
			m_dk4 = 1.0;
		}
		break;
		case dgn::def::enNationalAnnex::Italy:
		{
			m_dk3 = 0.8;
			m_dk4 = 0.9;
		}
		break;
		case dgn::def::enNationalAnnex::Sweden:
		case dgn::def::enNationalAnnex::Sweden19:
		{
			m_dk3 = 1.0;
			m_dk4 = 1.0;
		}
		break;
		default:
		{
			m_dk3 = 0.8;
			m_dk4 = 1.0;
		}
		break;
		}
	}
}

void CDgnConServiceLimtDlg::Initial_Data()
{
	m_nOption = 0;      // add/replace
	UpdateData(FALSE);

	//////////////// For EuroCode2 ////////////////
		// Exposure Class.
	int Index = 5;
	m_ctrEC2ExpComb.SetCurSel(Index);
	if (m_iCode == IRC_112_11 || m_iCode == IRS_RC || m_iCode == IRC_112_20)
	{
		int Index = 0;
		m_ctrEC2ExpComb.SetCurSel(Index);
	}
	// Crack Width Option.
	m_bCharCrack = FALSE;
	m_bFreqCrack = FALSE;
	m_bQuasCrack = TRUE;
	m_bCrackW = FALSE;
	if (m_iCode == AREMA23){
		//m_bQuasCrack = FALSE;
		int Index = 0;
		m_ctrEC2ExpComb.SetCurSel(Index);
    }

	if(m_iCode==Eurocode2 || m_iCode==Eurocode2_04)
	{
        OnSelchangeDgnConcExp();
        m_iDeflectEnv = 1;
        m_dUserDeflect = 250;

		Initial_StressFactor_EN04(Index);
	}
	else if(m_iCode==Eurocode2_2_05)
	{
        OnSelchangeDgnConcExp();
        m_iDeflectEnv = 1;
        m_dUserDeflect = 250;

		auto* pNA = CNationalAnnexInterface::Instance();
		const auto Code = pNA->GetBridgeCode(dgn::def::enDgn::RCS, m_iSubCode);
		if ( pNA == NULL ) return;
		pNA->SetCode(Code);

		m_dk1 = 0.6;
		m_dk2 = 0.45;

		m_dk3 = pNA->Getk3StressLimitation();
		m_dk4 = pNA->Getk4StressLimitation();
	}
    else if ( m_iCode == ACI318_05 || m_iCode==ACI318_08 || m_iCode==ACI318_11 || 
        m_iCode==ACI318_14 || m_iCode==ACI318M_14 || 
        m_iCode == ACI318_19 || m_iCode == ACI318M_19 ||
		m_iCode == ACI318_25 || m_iCode == ACI318M_25 ||
        m_iCode==NSR_10 || m_iCode == NSCP_2015_C || m_iCode == TWN_USD112)
	{
		//Default 변경시 DgnDataCtrl::Get_DgnConServ()에도 같이 변경해야함.
		m_iDeflectEnv = 1;
		m_dUserDeflect = 500.0;
	}
	else if (m_iCode == KCI_USD12 || m_iCode == KDS_41_30_2018 || m_iCode == KDS_41_20_2022 )
	{
		//Default 변경시 DgnDataCtrl::Get_DgnConServ()에도 같이 변경해야함.
		m_iDeflectEnv = 1;
		m_dUserDeflect = 500.0;
	}
    else if ( m_iCode==SNiP_20503_84RC || m_iCode==SP_35_13330_11RC || m_iCode==SNiP_20503_84RC_MKS || m_iCode==SP_35_13330_11RC_MKS  ||
        m_iCode==SNiP_20503_84_CSG || m_iCode==SP_35_13330_11_CSG )
    {
// 				sData.dCrackLim[0]  = m_edtCrackWLimit.GetEditValue();
//         sData.bCrackFlag[0] = m_bCrackW;		
	}
///////////////////////////////////////////////
	else if (m_iCode == IS456_2000)
	{
		//Default 변경시 DgnDataCtrl::Get_DgnConServ()에도 같이 변경해야함.
		m_iDeflectEnv = 1;
		m_dUserDeflect = 250;
		GetCrackLimByIS456();
		//Only Characteristic Crack width check
		m_bCharCrack = TRUE;
		m_bFreqCrack = FALSE;
		m_bQuasCrack = FALSE;
		GetDlgItem(IDC_DGN_CONC_CharCrack)->SetWindowTextW(_LSX(Crack width));
	}
	else if(m_iCode==IRC_112_11 || m_iCode == IRC_112_20)
	{
		OnSelchangeDgnConcExp();
		m_iDeflectEnv = 1;
		m_dUserDeflect = 250; 
		// Enable
		GetCrackLimByIRC112();
		m_dk1 = 0.6;
		//Disable. 단, DataChecking을 위해서 Data를 채워둠!!
		m_dk2 = 0.45; 
        m_dk3 = 0.8;  
        m_dk4 = 1.0;
	}
	else if(m_iCode==IRS_RC)
	{
		OnSelchangeDgnConcExp();
		m_iDeflectEnv = 1;
		m_dUserDeflect = 250; 
		// Enable
		GetCrackLimByIRC112();
		m_dk1 = 0.6;
		//Disable. 단, DataChecking을 위해서 Data를 채워둠!!
		m_dk2 = 0.45; 
        m_dk3 = 0.8;
        m_dk4 = 1.0;
	}
	else if(m_iCode == SP_63_1330_2018)
	{
		m_bCharCrack = TRUE;
		m_bFreqCrack = TRUE;
		m_iDeflectEnv = 1;
		m_dUserDeflect = 250;
	}
	else
	{
		m_iDeflectEnv = 0;
		m_dUserDeflect = 500;
	}

	m_iDeflectEnv_Long = 0;
	m_dUserDeflect_Long = 500;
	m_dReductFactor = 0.5;
	m_dUserCrack.SetEditUnit(0.0);

	m_RdoCorrosionLow.SetCheck(1);
	m_RdoServLife20  .SetCheck(1);

	m_dDAF = 1.0;
	m_dCharacLim = 250.0;
	m_iSeletType = 1;
	m_dStrainLimit = 250;
	m_dStrainGrowthRate = 16;
	m_dK = 0.56;
	BOOL bEnable=FALSE;
	if(m_iCrackEnv==4)    bEnable=TRUE;
	else                  bEnable=FALSE;
	GetDlgItem(IDC_DGN_CONC_BCRACK6)->EnableWindow(bEnable);
	if(m_iDeflectEnv==4)  bEnable=TRUE;
	else                  bEnable=FALSE;
	GetDlgItem(IDC_DGN_CONC_BDEFLECT6)->EnableWindow(bEnable);
	if(m_iDeflectEnv_Long==4)  bEnable=TRUE;
	else                       bEnable=FALSE;
	GetDlgItem(IDC_DGN_CONC_BDEFLECT13)->EnableWindow(bEnable);

	Update_InitDlgItems();

	UpdateData(FALSE);
}

// Set current Unit.
void CDgnConServiceLimtDlg::Initial_Unit()
{
	m_dUserCrack.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dCharCrack.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dFreqCrack.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dQuasCrack.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_dCrackCurUnit1.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dCrackCurUnit2.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dCrackCurUnit3.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dCrackCurUnit4.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtCrackWLimit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untCrackWLimit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_untReductFactor.SetUnitType(D_UNITSYS_BASE_PERCENT);
}

void CDgnConServiceLimtDlg::ChangeDlgItemInfo()
{
	// Exposure classes For Eurocede2.
	if(m_iCode == Eurocode2 || m_iCode==Eurocode2_04 ||m_iCode==Eurocode2_2_05 || m_iCode==IRC_112_11 || m_iCode==IS456_2000 || 
       m_iCode == IRC_112_20)
	{
		GetDlgItem(IDC_DGN_CONC_ExpClassFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_EXP1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_EXP2)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_CONC_DeflectFrm3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_DEFLECT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_DEFLECT2)->ShowWindow(SW_SHOW);
	}
	else if (m_iCode == IRS_RC)
	{

		GetDlgItem(IDC_DGN_CONC_ExpClassFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_EXP1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_EXP2)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_CONC_DeflectFrm3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_DEFLECT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_DEFLECT2)->ShowWindow(SW_HIDE);
	}
	else if (m_iCode == AREMA23)
	{

		GetDlgItem(IDC_DGN_CONC_ExpClassFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_EXP1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_EXP2)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_CONC_DeflectFrm3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_DEFLECT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_DEFLECT2)->ShowWindow(SW_HIDE);
	}
//   else if(m_iCode==Eurocode2_2_05)
//   {
//     GetDlgItem(IDC_DGN_CONC_ExpClassFrm)->ShowWindow(SW_SHOW);
//     GetDlgItem(IDC_DGN_CONC_EXP1)->ShowWindow(SW_SHOW);
//     GetDlgItem(IDC_DGN_CONC_EXP2)->ShowWindow(SW_SHOW);
// 
//     GetDlgItem(IDC_DGN_CONC_DeflectFrm3)->ShowWindow(SW_HIDE);
//     GetDlgItem(IDC_DGN_CONC_DEFLECT1)->ShowWindow(SW_HIDE);
//     GetDlgItem(IDC_DGN_CONC_DEFLECT2)->ShowWindow(SW_HIDE);
//   }
	else
	{
		GetDlgItem(IDC_DGN_CONC_ExpClassFrm)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_EXP1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_EXP2)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_CONC_DeflectFrm3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_DEFLECT1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_DEFLECT2)->ShowWindow(SW_HIDE);
	}

	// Stress
	if(m_iCode == Eurocode2 || m_iCode==Eurocode2_04 || m_iCode==Eurocode2_2_05 || m_iCode==IRC_112_11 || m_iCode == IS456_2000 || m_iCode == IRS_RC || 
       m_iCode == IRC_112_20) // Eurocode2-2:05에서는 Stress항목은 Hide함.
	{
		GetDlgItem(IDC_DGN_CONC_StressFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_STRESS1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_STRESS2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_STRESS3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_STRESS4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_STRESS5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_STRESS6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_STRESS7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_STRESS8)->ShowWindow(SW_SHOW);
	}
//   else if(m_iCode==Eurocode2_2_05)
//   {
//     GetDlgItem(IDC_DGN_CONC_StressFrm)->ShowWindow(SW_SHOW);
//     GetDlgItem(IDC_DGN_CONC_STRESS1)->ShowWindow(SW_SHOW);
//     GetDlgItem(IDC_DGN_CONC_STRESS2)->ShowWindow(SW_SHOW);
//     GetDlgItem(IDC_DGN_CONC_STRESS3)->ShowWindow(SW_SHOW);
//     GetDlgItem(IDC_DGN_CONC_STRESS4)->ShowWindow(SW_SHOW);
//     GetDlgItem(IDC_DGN_CONC_STRESS5)->ShowWindow(SW_SHOW);
//     GetDlgItem(IDC_DGN_CONC_STRESS6)->ShowWindow(SW_SHOW);
//     GetDlgItem(IDC_DGN_CONC_STRESS7)->ShowWindow(SW_SHOW);
//     GetDlgItem(IDC_DGN_CONC_STRESS8)->ShowWindow(SW_SHOW);
//   }
	else if(m_iCode == SP_63_1330_2018)
	{
		int nDistY = 0;

		CRect rRef, rToMove;
		GetDlgItem(IDC_DGN_CONC_ExpClassFrm)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CONC_CrackFrm1)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_arSwedenMoveCtrl, nDistY);

		CDlgUtil::CtrlShowHide(this, m_arCrack2MoveCtrl, FALSE);
		CDlgUtil::CtrlShowHide(this, m_arCrack2UserMoveCtrl, FALSE);
		CDlgUtil::CtrlShowHide(this, m_arCrack1MoveCtrl, TRUE);

		GetDlgItem(IDC_DGN_CONC_CrackFrm13)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_QuasCrack)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK10)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK11)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK12)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_CONC_ServiceLifeFrm)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_SERVLIFE1_RDO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_SERVLIFE2_RDO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_SERVLIFE3_RDO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CrackDetFrm)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_Crack_Corrosion_High_RDO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_Crack_Corrosion_Low_RDO)->ShowWindow(SW_HIDE);
	}
	else 
	{
		GetDlgItem(IDC_DGN_CONC_StressFrm)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_STRESS1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_STRESS2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_STRESS3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_STRESS4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_STRESS5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_STRESS6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_STRESS7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_STRESS8)->ShowWindow(SW_HIDE);
	}

	// Crack
	if(m_iCode == Eurocode2 || m_iCode==Eurocode2_04 || m_iCode==Eurocode2_2_05 || m_iCode==IRC_112_11 || m_iCode == IS456_2000 ||
	   m_iCode == IRS_RC    || m_iCode == IRC_112_20)
	{
		int nDistY = 0;
		GetDlgItem(IDC_DGN_CONC_CrackFrm1)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_DGN_CONC_CRACK1)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_DGN_CONC_CRACK2)->ShowWindow(SW_SHOW);
		//GetDlgItem(IDC_DGN_CONC_CRACK3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CrackFrm11)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CRACK4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CRACK5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CRACK6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CrackFrm12)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CRACK7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CRACK8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CRACK9)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CrackFrm13)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CRACK10)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CRACK11)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CRACK12)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_CONC_CharCrack)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_FreqCrack)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_QuasCrack)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_DGN_CONC_CrackFrm2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK7)->ShowWindow(SW_HIDE);

        BOOL bShow = (m_iCode == Eurocode2_04 && 
			(m_iSubCode == dgn::def::enNationalAnnex::Sweden || m_iSubCode==dgn::def::enNationalAnnex::Sweden19 ||
				m_iSubCode == dgn::def::enNationalAnnex::Norway)) ? TRUE : FALSE;// National Annex SWeden PMS4588:DNKIM-20130306
		CDlgUtil::CtrlShowHide(this, m_arServiceLifeCtrl, bShow);
		
		// Corrosion Sensitivity: Hide for Norway, Show for Sweden
		BOOL bShowCorrosion = bShow && m_iSubCode != dgn::def::enNationalAnnex::Norway;
		CDlgUtil::CtrlShowHide(this, m_arCorrosionCtrl, bShowCorrosion);


		// For Norway: Hide L20, set L50 as default and move L50 to center
		BOOL bIsNorway = (bShow && m_iSubCode == dgn::def::enNationalAnnex::Norway);
		GetDlgItem(IDC_DGN_CONC_SERVLIFE3_RDO)->ShowWindow(bIsNorway ? SW_HIDE : bShow);
		
		// Get L100, L50, L20
		CRect rL100, rL50, rL20;
		GetDlgItem(IDC_DGN_CONC_SERVLIFE1_RDO)->GetWindowRect(rL100);
		GetDlgItem(IDC_DGN_CONC_SERVLIFE2_RDO)->GetWindowRect(rL50);
		GetDlgItem(IDC_DGN_CONC_SERVLIFE3_RDO)->GetWindowRect(rL20);

		ScreenToClient(rL100);
		ScreenToClient(rL50);
		ScreenToClient(rL20);

		if (bIsNorway)
		{
			m_RdoServLife20.SetCheck(0);
			m_RdoServLife50.SetCheck(1);
			m_iServLife = 1;

			int nNewX = rL100.left + (rL100.left + rL20.right) / 2;
			GetDlgItem(IDC_DGN_CONC_SERVLIFE2_RDO)->MoveWindow(nNewX, rL50.top, rL50.Width(), rL50.Height());
		}
		else if (bShow)
		{
			// For Sweden/Sweden19: set L20 as default and restore L50 position
			m_RdoServLife20.SetCheck(1);
			m_RdoServLife50.SetCheck(0);
			m_iServLife = 2;

			int nNewX = (rL20.left + rL100.left) / 2;
			GetDlgItem(IDC_DGN_CONC_SERVLIFE2_RDO)->MoveWindow(nNewX, rL50.top, rL50.Width(), rL50.Height());
		}

		CRect rRef, rRef2, rToMove, rTemp;
		GetDlgItem(IDC_DGN_CONC_ServiceLifeFrm)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CONC_CrackDetFrm)->GetWindowRect(rRef2);
		GetDlgItem(IDC_DGN_CONC_CrackFrm1)->GetWindowRect(rToMove);
		GetDlgItem(IDC_DGN_CONC_CrackDetFrm)->GetWindowRect(rTemp);
		
		nDistY = (bShow) ? rRef2.bottom - rToMove.top + globalUtils.ScaleByDPI(4) : rRef.top - rToMove.top;
		
		// For Norway: Hide Corrosion Sensitivity, so subtract its height
		if (m_iSubCode == dgn::def::enNationalAnnex::Norway && bShow)
		{
			CRect rCorrosion;
			GetDlgItem(IDC_DGN_CONC_CrackDetFrm)->GetWindowRect(rCorrosion);
			nDistY -= rCorrosion.Height() + globalUtils.ScaleByDPI(4);
		}
		
		CDlgUtil::CtrlMoveDistY(this, m_arSwedenMoveCtrl, nDistY);

	}
	else if (m_iCode == SP_63_1330_2018)
	{
		int nDistY = 0;

		CRect rRef, rToMove;
		GetDlgItem(IDC_DGN_CONC_ExpClassFrm)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CONC_CrackFrm1)->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_arSwedenMoveCtrl, nDistY);

		CDlgUtil::CtrlShowHide(this, m_arCrack2MoveCtrl, FALSE);
		CDlgUtil::CtrlShowHide(this, m_arCrack2UserMoveCtrl, FALSE);
		CDlgUtil::CtrlShowHide(this, m_arCrack1MoveCtrl, TRUE);

		GetDlgItem(IDC_DGN_CONC_CrackFrm13)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_QuasCrack)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK10)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK11)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK12)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_CONC_ServiceLifeFrm)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_SERVLIFE1_RDO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_SERVLIFE2_RDO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_SERVLIFE3_RDO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CrackDetFrm)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_Crack_Corrosion_High_RDO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_Crack_Corrosion_Low_RDO)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CONC_CrackFrm1)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_DGN_CONC_CRACK1)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_DGN_CONC_CRACK2)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_DGN_CONC_CRACK3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CrackFrm11)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CrackFrm12)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CrackFrm13)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK10)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK11)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK12)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_CONC_CharCrack)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_FreqCrack)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_QuasCrack)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_DGN_CONC_CrackFrm2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK7)->ShowWindow(SW_HIDE);

		CStringArray aCrackEnv;
		aCrackEnv.RemoveAll();

		int iCrackEnvNum = CDBLib::GetCrackEnvList(aCrackEnv);

		// Add by Unsang :: DBLib에서 처리하도록 변경할 것.
		// if(m_iCode==Eurocode2_2_05) iCrackEnvNum = 0;  
		AlignControls(2,iCrackEnvNum);  //(KyungSik) Eurocode2_04와 같도록함.

		if(iCrackEnvNum ==1)
		{
			GetDlgItem(IDC_DGN_CONC_CrackFrm2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK1)->SetWindowText(aCrackEnv[0]);
			GetDlgItem(IDC_DGN_CONC_BCRACK5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK6)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK7)->ShowWindow(SW_SHOW);
		}
		else if(iCrackEnvNum ==2)
		{
			GetDlgItem(IDC_DGN_CONC_CrackFrm2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK1)->SetWindowText(aCrackEnv[0]);
			GetDlgItem(IDC_DGN_CONC_BCRACK2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK2)->SetWindowText(aCrackEnv[1]);
			GetDlgItem(IDC_DGN_CONC_BCRACK5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK6)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK7)->ShowWindow(SW_SHOW);
		}
		else if(iCrackEnvNum == 3)
		{
			GetDlgItem(IDC_DGN_CONC_CrackFrm2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK1)->SetWindowText(aCrackEnv[0]);
			GetDlgItem(IDC_DGN_CONC_BCRACK2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK2)->SetWindowText(aCrackEnv[1]);
			GetDlgItem(IDC_DGN_CONC_BCRACK3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK3)->SetWindowText(aCrackEnv[2]);
			GetDlgItem(IDC_DGN_CONC_BCRACK5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK6)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK7)->ShowWindow(SW_SHOW);
		}
		else if(iCrackEnvNum == 4)
		{
			GetDlgItem(IDC_DGN_CONC_CrackFrm2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK1)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK1)->SetWindowText(aCrackEnv[0]);
			GetDlgItem(IDC_DGN_CONC_BCRACK2)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK2)->SetWindowText(aCrackEnv[1]);
			GetDlgItem(IDC_DGN_CONC_BCRACK3)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK3)->SetWindowText(aCrackEnv[2]);
			GetDlgItem(IDC_DGN_CONC_BCRACK4)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK4)->SetWindowText(aCrackEnv[3]);
			GetDlgItem(IDC_DGN_CONC_BCRACK5)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK6)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_DGN_CONC_BCRACK7)->ShowWindow(SW_SHOW);
		}

		BOOL bShow = FALSE;// PMS4588:DNKIM-20130306
		GetDlgItem(IDC_DGN_CONC_ServiceLifeFrm)->ShowWindow(bShow);
		GetDlgItem(IDC_DGN_CONC_SERVLIFE1_RDO)->ShowWindow(bShow);
		GetDlgItem(IDC_DGN_CONC_SERVLIFE2_RDO)->ShowWindow(bShow);
		GetDlgItem(IDC_DGN_CONC_SERVLIFE3_RDO)->ShowWindow(bShow);
		GetDlgItem(IDC_DGN_CONC_CrackDetFrm)->ShowWindow(bShow);
		GetDlgItem(IDC_DGN_CONC_Crack_Corrosion_High_RDO)->ShowWindow(bShow);
		GetDlgItem(IDC_DGN_CONC_Crack_Corrosion_Low_RDO)->ShowWindow(bShow);
	}

	// Crack Rename
	if(m_iCode == SP_63_1330_2018)
	{
		GetDlgItem(IDC_DGN_CONC_CharCrack)->SetWindowText(_T("Short-term"));
		GetDlgItem(IDC_DGN_CONC_FreqCrack)->SetWindowText(_T("Long-term"));

		CRect rRef, rRect1;
		GetDlgItem(IDC_DGN_CONC_CrackFrm1)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CONC_CrackFrm12)->GetWindowRect(rRect1);

		rRef.bottom = rRect1.bottom + 6;
		ScreenToClient(rRef);
		GetDlgItem(IDC_DGN_CONC_CrackFrm1)->SetWindowPos(NULL, rRef.left, rRef.top, rRef.Width(), rRef.Height(), SWP_DRAWFRAME);
	}
	else
	{
		GetDlgItem(IDC_DGN_CONC_CharCrack)->SetWindowText(_LS(IDS_DGN_CONC_CharCrack));
		GetDlgItem(IDC_DGN_CONC_FreqCrack)->SetWindowText(_LS(IDS_DGN_CONC_FreqCrack));

		CRect rRef, rRect1;
		GetDlgItem(IDC_DGN_CONC_CrackFrm1)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CONC_CrackFrm13)->GetWindowRect(rRect1);

		rRef.bottom = rRect1.bottom + 6;
		ScreenToClient(rRef);
		GetDlgItem(IDC_DGN_CONC_CrackFrm1)->SetWindowPos(NULL, rRef.left, rRef.top, rRef.Width(), rRef.Height(), SWP_DRAWFRAME);
	}

	// Deflection
	GetDlgItem(IDC_DGN_CONC_DeflectFrm)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CONC_BDEFLECT1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CONC_BDEFLECT2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CONC_BDEFLECT3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CONC_BDEFLECT4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CONC_BDEFLECT5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CONC_BDEFLECT6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CONC_DeflectFrm2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CONC_BDEFLECT7)->ShowWindow(SW_HIDE);

	//AIJ-WSD99 기준일때만 Show(meshed slab 인 경우만)
	BOOL bAIJ_WSD99 = IsCodeForMeshedDesignAIJ_JP();
	CDlgUtil::CtrlShowHide(this, m_arAIJ_WSD99_MoveCtrl, bAIJ_WSD99);

	BOOL bACI_New = !m_bSlabService && (m_iCode==ACI318_14 || m_iCode==ACI318M_14 ||
        m_iCode == ACI318_19 || m_iCode == ACI318M_19 ||
		m_iCode == ACI318_25 || m_iCode == ACI318M_25 ||
        m_iCode==NSR_10 || m_iCode==NSCP_2015_C || m_iCode==TWN_USD112) ? TRUE : FALSE;
	CDlgUtil::CtrlShowHide(this, m_arLongDeflect, bACI_New);

	CStringArray aDeflectEnv;
	aDeflectEnv.RemoveAll();

	int iDeflectEnvNum = 0;
	if(!m_bIsCsgDesign)   iDeflectEnvNum = CDBLib::GetConDeflectionEnvList(aDeflectEnv);

	// Add by Unsang.. DBLib에서변경하도록 해야 함.
	//  if(m_iCode==Eurocode2_2_05) iDeflectEnvNum = 0;

	AlignControls(1,iDeflectEnvNum); //(KyungSik) Eurocode2_04와 같도록함.

	if(m_iCode == Eurocode2 || m_iCode==Eurocode2_04 || m_iCode==Eurocode2_2_05 || m_iCode==IRC_112_11 || m_iCode == IS456_2000 || 
       m_iCode == IRC_112_20 || m_iCode == SP_63_1330_2018)
	{
		GetDlgItem(IDC_DGN_CONC_DeflectFrm)->SetWindowText(_LS(IDS_DGN_TEXT_QuasiPermanentDefletLim));
		GetDlgItem(IDC_DGN_CONC_DeflectFrm2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT7)->ShowWindow(SW_SHOW);
	}
	else if(m_iCode==IRS_RC )
	{
		GetDlgItem(IDC_DGN_CONC_DeflectFrm)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_DeflectFrm2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT7)->ShowWindow(SW_HIDE);
	}
	else if ( m_iCode == BS5400_4_90 )
	{
		GetDlgItem(IDC_DGN_CONC_CrackFrm2)->SetWindowText(_LS(IDS_DGN_EXPOSURE_ENVIROMENT));
        GetDlgItem(IDC_DGN_CONC_DeflectFrm)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_DGN_CONC_DeflectFrm2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_DGN_CONC_BDEFLECT7)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_DGN_CONC_BCRACK5)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_DGN_CONC_BCRACK6)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_DGN_CONC_BCRACK7)->ShowWindow(SW_HIDE);
	}
	else if (m_iCode == AREMA23)
	{
		GetDlgItem(IDC_DGN_CONC_CrackFrm2)->SetWindowText(_LS(IDS_DGN_EXPOSURE_ENVIROMENT));
		GetDlgItem(IDC_DGN_CONC_DeflectFrm)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_DeflectFrm2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK7)->ShowWindow(SW_HIDE);
	}
	else if (m_iCode == TMH07_89)
	{
		GetDlgItem(IDC_DGN_CONC_CrackFrm2)->SetWindowText(_LS(IDS_DGN_EXPOSURE_ENVIROMENT));
		GetDlgItem(IDC_DGN_CONC_DeflectFrm)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_DeflectFrm2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK7)->ShowWindow(SW_HIDE);
	}
	else if(m_iCode==SNiP_20503_84RC || m_iCode==SP_35_13330_11RC || m_iCode==SNiP_20503_84RC_MKS || m_iCode==SP_35_13330_11RC_MKS || 
								m_iCode==SNiP_20503_84_CSG || m_iCode==SP_35_13330_11_CSG)
	{
		// Nothing
	}
	else if(!IsCodeForMeshedDesignAIJ_JP())
	{
		GetDlgItem(IDC_DGN_CONC_DeflectFrm )->SetWindowText(_LS(IDS_DGN_TEXT_NORMAL_DefletLim));
		GetDlgItem(IDC_DGN_CONC_DeflectFrm2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT7  )->ShowWindow(SW_SHOW);
	}

	if(iDeflectEnvNum ==1)
	{
		GetDlgItem(IDC_DGN_CONC_DeflectFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT1)->SetWindowText(aDeflectEnv[0]);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT6)->ShowWindow(SW_SHOW);    
	}
	else if(iDeflectEnvNum ==2)
	{
		GetDlgItem(IDC_DGN_CONC_DeflectFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT1)->SetWindowText(aDeflectEnv[0]);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT2)->SetWindowText(aDeflectEnv[1]);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT6)->ShowWindow(SW_SHOW);
	}
	else if(iDeflectEnvNum == 3)
	{
		GetDlgItem(IDC_DGN_CONC_DeflectFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT1)->SetWindowText(aDeflectEnv[0]);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT2)->SetWindowText(aDeflectEnv[1]);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT3)->SetWindowText(aDeflectEnv[2]);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT6)->ShowWindow(SW_SHOW);
	}
	else if(iDeflectEnvNum == 4)
	{
		GetDlgItem(IDC_DGN_CONC_DeflectFrm)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT1)->SetWindowText(aDeflectEnv[0]);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT2)->SetWindowText(aDeflectEnv[1]);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT3)->SetWindowText(aDeflectEnv[2]);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT4)->SetWindowText(aDeflectEnv[3]);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT6)->ShowWindow(SW_SHOW);
	}

	if(m_iCode==SNiP_20503_84RC || m_iCode==SP_35_13330_11RC || m_iCode==SNiP_20503_84RC_MKS || m_iCode==SP_35_13330_11RC_MKS )
	{
		GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_CHK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_GRP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_STC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_EDT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_UNT)->ShowWindow(SW_SHOW);
	}
	else if(m_iCode==SNiP_20503_84_CSG || m_iCode==SP_35_13330_11_CSG)
	{
		GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_CHK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_GRP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_STC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_EDT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_UNT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_CONC_CrackFrm2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_BCRACK7)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_CHK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_GRP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_STC)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_EDT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_UNT)->ShowWindow(SW_HIDE);
	}

	UpdateData(FALSE);
	Invalidate();
}

// Allign the Items of the dialog by Design Code.
void CDgnConServiceLimtDlg::AlignControls(int iWhich, int iEnvNum/*=0*/)
{
	CRect RectDlg, rRef, rToMove;
	CRect rRect1, rRect2, rRectUser;
	int nDistY = 0;

	if(iWhich==1)
	{   
		// Deflection.
		if(iEnvNum == 0) iEnvNum = -1;
		GetWindowRect(&RectDlg);
		if(m_iCode == Eurocode2 || m_iCode==Eurocode2_04 || m_iCode==Eurocode2_2_05 || m_iCode==IRC_112_11 || m_iCode == IS456_2000 ||
            m_iCode == IRS_RC || m_iCode == IRC_112_20 || m_iCode == SP_63_1330_2018)
			GetDlgItem(IDC_DGN_CONC_CrackFrm1)->GetWindowRect(rToMove);
//     else if(m_iCode==Eurocode2_2_05)
//       GetDlgItem(IDC_DGN_CONC_StressFrm)->GetWindowRect(rToMove);
		else
			GetDlgItem(IDC_DGN_CONC_CrackFrm2)->GetWindowRect(rToMove);

		GetDlgItem(IDC_DGN_CONC_DeflectFrm)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT1)->GetWindowRect(rRect1);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT2)->GetWindowRect(rRect2);
		GetDlgItem(IDC_DGN_CONC_BDEFLECT6)->GetWindowRect(rRectUser);

		nDistY = -rRef.bottom;
		
		CRect rDef;
		rDef.left = rRef.left;
		rDef.top = rRef.top;
		rDef.right = rRef.right;
		rDef.bottom = rRef.top + (iEnvNum+1)*(rRect2.top - rRect1.top) + (rRect1.top - rRef.top) + globalUtils.ScaleByDPI(8);// + 52;
		
		nDistY += rDef.bottom;

		ScreenToClient(rDef);
		GetDlgItem(IDC_DGN_CONC_DeflectFrm)->SetWindowPos(NULL, rDef.left, rDef.top, rDef.Width(), rDef.Height(), SWP_DRAWFRAME);
		CDlgUtil::CtrlMoveDistY(this, m_arDeflectUserMoveCtrl, nDistY);
		//CDlgUtil::CtrlMoveDistY(this, m_arCharDeflectUserMoveCtrl, nDistY+4);

		int nAddMoveY = 0;
		BOOL bACI = (m_iCode == ACI318_05 || m_iCode==ACI318_08 || m_iCode==ACI318_11 ||
            m_iCode==ACI318_14 || m_iCode==ACI318M_14 ||
            m_iCode==ACI318_19 || m_iCode==ACI318M_19 || 
			m_iCode==ACI318_25 || m_iCode==ACI318M_25 || 
            m_iCode==NSR_10 || m_iCode==NSCP_2015_C || m_iCode==TWN_USD112) ? TRUE : FALSE;
		BOOL bKCIUSD12 = (m_iCode == KCI_USD12 || m_iCode == KDS_41_30_2018 || m_iCode == KDS_41_20_2022) ? TRUE : FALSE;
		if(bACI || bKCIUSD12)// PMS4603:DNKIM-20130130
		{
			CRect rRefB;
			GetDlgItem(IDC_DGN_CONC_ExpClassFrm)->GetWindowRect(rRefB);
			nAddMoveY = rRefB.Height(); 
		}
		nDistY = -rRef.top + rToMove.bottom + globalUtils.ScaleByDPI(8) - nAddMoveY;
		CDlgUtil::CtrlMoveDistY(this, m_arDeflectMoveCtrl, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_arDeflectUserMoveCtrl, nDistY);
		//CDlgUtil::CtrlMoveDistY(this, m_arCharDeflectUserMoveCtrl, nDistY);

		BOOL bACI_New = !m_bSlabService && (m_iCode==ACI318_14 || m_iCode==ACI318M_14 || 
            m_iCode==ACI318_19 || m_iCode==ACI318M_19 ||
			m_iCode==ACI318_25 || m_iCode==ACI318M_25 || 
            m_iCode==NSR_10 || m_iCode==NSCP_2015_C || m_iCode==TWN_USD112) ? TRUE : FALSE;
		nDistY = 0;
		CRect rRefDef_short,rMovDef_long;
		GetDlgItem(IDC_DGN_CONC_DeflectFrm)->GetWindowRect(rRefDef_short);
		GetDlgItem(IDC_DGN_CONC_DeflectFrm4)->GetWindowRect(rMovDef_long);

		nDistY = rRefDef_short.bottom - rMovDef_long.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_arLongDeflect, nDistY);

		CRect rRefDAF, rToMove;
		if(bACI_New) GetDlgItem(IDC_DGN_CONC_DeflectFrm4)->GetWindowRect(rRefDAF);
		else GetDlgItem(IDC_DGN_CONC_DeflectFrm3)->GetWindowRect(rRefDAF);
		GetDlgItem(IDC_DGN_CONC_DeflectFrm2)->GetWindowRect(rToMove);
		int nDistDAFY = (bACI || bKCIUSD12 || m_iCode == SP_63_1330_2018) ? rRefDAF.top - rToMove.top : rRefDAF.bottom - rToMove.top + globalUtils.ScaleByDPI(4);
		if(bACI_New) nDistDAFY = rRefDAF.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CArray<UINT, UINT> arDAFMoveCtrls; arDAFMoveCtrls.RemoveAll();
		arDAFMoveCtrls.Add(IDC_DGN_CONC_DeflectFrm2);
		arDAFMoveCtrls.Add(IDC_DGN_CONC_BDEFLECT7);
		CDlgUtil::CtrlMoveDistY(this, arDAFMoveCtrls, nDistDAFY);
				

		
		nDistY = 0;
		GetDlgItem(IDC_DGN_CONC_CRACK1)->GetWindowRect(rRef);
		if(iEnvNum!=-1)
		{
			if(bACI_New) GetDlgItem(IDC_DGN_CONC_DeflectFrm4)->GetWindowRect(rToMove);
			else GetDlgItem(IDC_DGN_CONC_DeflectFrm)->GetWindowRect(rToMove);
		}
		if(m_iCode == Eurocode2 || m_iCode==Eurocode2_04 || m_iCode==Eurocode2_2_05 || m_iCode==IRC_112_11 || m_iCode==IS456_2000 ||
            bACI== TRUE || bKCIUSD12== TRUE || m_iCode == IRC_112_20 || m_iCode == SP_63_1330_2018)
			GetDlgItem(IDC_DGN_CONC_DeflectFrm2)->GetWindowRect(rToMove);

		nDistY = -rRef.top + rToMove.bottom + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_arBtnMoveCtrl, nDistY);
	}
	else if(iWhich==2)
	{
		// Crack.
		GetWindowRect(&RectDlg);
		GetDlgItem(IDC_STATIC_SELECT_TYPE)->GetWindowRect(rToMove);
		GetDlgItem(IDC_DGN_CONC_CrackFrm2)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CONC_BCRACK1)->GetWindowRect(rRect1);
		GetDlgItem(IDC_DGN_CONC_BCRACK2)->GetWindowRect(rRect2);
		
		nDistY = -rRef.bottom;

		CRect rCrack;
		rCrack.left = rRef.left;
		rCrack.top = rRef.top;
		rCrack.right = rRef.right;
		rCrack.bottom = rRef.top + (iEnvNum+1)*(rRect2.top - rRect1.top) + (rRect1.top - rRef.top) + globalUtils.ScaleByDPI(4);

		nDistY += rCrack.bottom;

		ScreenToClient(rCrack);
		GetDlgItem(IDC_DGN_CONC_CrackFrm2)->SetWindowPos(NULL, rCrack.left, rCrack.top, rCrack.Width(), rCrack.Height(), SWP_DRAWFRAME);
		CDlgUtil::CtrlMoveDistY(this, m_arCrack2UserMoveCtrl, nDistY);
		
		nDistY = -rRef.top + rToMove.bottom + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_arCrack2MoveCtrl, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_arCrack2UserMoveCtrl, nDistY);
	}

	CArray<UINT, UINT> aControls; aControls.RemoveAll();
	aControls.Add(IDC_DGN_CONC_CRACK_WIDTH_CHK);
	aControls.Add(IDC_DGN_CONC_CRACK_WIDTH_GRP);
	aControls.Add(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_STC);
	aControls.Add(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_EDT);
	aControls.Add(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_UNT);
	
	GetDlgItem(IDC_DGN_CONC_ExpClassFrm)->GetWindowRect(rRef);
	GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_CHK)->GetWindowRect(rToMove);
	
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	
	if(m_iCode==SNiP_20503_84RC || m_iCode==SP_35_13330_11RC || m_iCode==SNiP_20503_84RC_MKS || m_iCode==SP_35_13330_11RC_MKS  || 
			 m_iCode==SNiP_20503_84_CSG || m_iCode==SP_35_13330_11_CSG)
	{
		GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CONC_DeflectFrm2)->GetWindowRect(rToMove);

		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_arBtnMoveCtrl, nDistY);
	}
	else if (m_iCode==IRS_RC)
	{
		GetDlgItem(IDC_DGN_CONC_CrackFrm1)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CONC_DeflectFrm2)->GetWindowRect(rToMove);

		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_arBtnMoveCtrl, nDistY);
	}
	else if(IsCodeForMeshedDesignAIJ_JP())
	{
		GetDlgItem(IDC_STATIC_SELECT_TYPE)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CONC_DEFLECTION_REVIEW_GRUP)->GetWindowRect(rToMove);

		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_arAIJ_WSD99_MoveCtrl, nDistY);

		GetDlgItem(IDC_DGN_CONC_CRACK_REVIEW_GRUP)->GetWindowRect(rRef);
		GetDlgItem(IDC_DGN_CONC_DeflectFrm2)->GetWindowRect(rToMove);

		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlMoveDistY(this, m_arBtnMoveCtrl, nDistY);
	}
}

void CDgnConServiceLimtDlg::ResizeDlg()
{
	CRect rectLast;
	GetDlgItem(IDC_DGN_CONC_DeflectFrm2)->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(16);
// 	BOOL bRet = this->SetWindowPos(NULL, r.left, r.top, r.Width(), r.Height(), NULL);

	CWnd* pParent = GetParent();
	pParent = pParent->GetParent();
	pParent->ScreenToClient(r);
	

	if(m_bIsCsgDesign) {/*do nothing*/;}
	else ((CDgnConDlg*)pParent)->LayoutChildDialog();

	RedrawWindow();
}

void CDgnConServiceLimtDlg::SetIsCpgDesign(BOOL bIsCpgDesign)
{
	m_bIsCsgDesign = bIsCpgDesign;
}

void CDgnConServiceLimtDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	double dFlen = 0.0;
	
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

void CDgnConServiceLimtDlg::UpdateBuffer()
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
			case(UR_DCON_ADD):  // Design Code 변경시.
			case(UR_DCON_DEL):
				{      
				if(m_bIsCsgDesign)
				{
					if(!GetDesignCsgCodeNo()) ASSERT(0);
				}
				else
				{
					m_iCode    = CDgnCodeCtrl::GetConCodeID(CDBLib::GetConCodeName());  // Set Design Code to Enum.
					m_iSubCode = CDBLib::GetConSubCode();       // Set Design Sub Code 
				}
				ChangeDlgItemInfo();                        // Change the Form of the dialog bar.
				Initial_Data();                             // Initialize the data.
				OnDgnCrackWidthChk();
				ResizeDlg();
				break;      
			}
			default:	break;
		}
	}
}

// If press the 'Close' button.
void CDgnConServiceLimtDlg::OnDgnClose() 
{
	// TODO: Add your control notification handler code here
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);	
}

// If press the 'Apply' button.
void CDgnConServiceLimtDlg::OnDgnExecute() 
{
#if defined(_MGEN_JP)
	if(m_bSlabService && !m_pDoc->AllowCommand(D_OPTN_ID_SLAB_STRIP))
	{
		m_pDoc->DisplayHistoryMessage(_LS(IDS_MAIN_NOT_LICENCED_MESHED_SLAB_STRIP));
		return;
	}
#endif

	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	//	Select된 Element를 얻어옵니다.
	CArray<T_ELEM_K,T_ELEM_K> aSelKey;  aSelKey.RemoveAll();
	CArray<T_ELEM_K,T_ELEM_K> rSelKey;
	if(m_iSeletType)
	{
	  I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	  pIGM->GetSelectedElemKeyListForDgn(aSelKey);
	}
	else
	{
		m_pDoc->m_pAttrCtrl->GetElemKeyList(aSelKey);  
	}

	// if Selected element is existed.
	if(aSelKey.GetSize() > 0)
	{
		for(int i=0; i<aSelKey.GetSize(); i++)
		{      
			T_ELEM_K key = aSelKey.GetAt(i);
			// Check Material.
			T_ELEM_D ElemD; ElemD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetElem(key, ElemD))       continue;
			T_MATD_D MatlD; MatlD.Initialize();
			if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatlD)) continue;
			
			if(!m_bIsCsgDesign){if(MatlD.Type != _T("C")) continue;      }
			BOOL bColumnServiceOk = (m_iCode == Eurocode2_2_05 || m_iCode == Eurocode2_04 || m_iCode == Eurocode2 ||
				m_iCode == IRC_112_11 || m_iCode == IS456_2000 || m_iCode == IRC_112_20 || m_iCode == SP_63_1330_2018) ? TRUE : FALSE;

			// Beam      
#ifdef _MGEN
			if(m_bSlabService)
			{
				if(m_pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp))         rSelKey.Add(key);  // PMS:4020-Seungjun-20090818 EC2:04 Slab/Wall Design.
			}
			else
			{
				if(m_pDoc->m_pAttrCtrl->GetMemberType(key) == D_MBTP_BEAM) rSelKey.Add(key);
				else if(m_pDoc->m_pAttrCtrl->GetMemberType(key) == D_MBTP_COLUMN && bColumnServiceOk == TRUE) rSelKey.Add(key);
			}
#else
			if(m_pDoc->m_pAttrCtrl->GetMemberType(key) == D_MBTP_BEAM) rSelKey.Add(key);
			else if(m_pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp))         rSelKey.Add(key);  // PMS:4020-Seungjun-20090818 EC2:04 Slab/Wall Design.
			
			// if Eurocode2-2:05, Add Column.
			if(m_pDoc->m_pAttrCtrl->GetMemberType(key) == D_MBTP_COLUMN && bColumnServiceOk == TRUE) rSelKey.Add(key);

			// RUS Column
			if(m_iCode==SNiP_20503_84RC || m_iCode==SP_35_13330_11RC || m_iCode==SNiP_20503_84RC_MKS || m_iCode==SP_35_13330_11RC_MKS)
			{
				if(m_pDoc->m_pAttrCtrl->GetMemberType(key) == D_MBTP_COLUMN) rSelKey.Add(key);
			}
#endif     
		}

		T_SERV_D sData;
		sData.Initialize();
		if(m_nOption==0)	// add/replace
		{
			/////// use only Eurocode2 /////////////////////////////////////////////
			if( (m_dk1<=0||m_dk1>1) || (m_dk2<=0||m_dk2>1) || (m_dk3<=0||m_dk3>1) || (m_dk4<=0||m_dk4>1) )
			{
				AfxMessageBox(_LS(IDS_DGN_SERV_STRESS_PARAMETER_ERROR));
				return;
			}
			sData.iStressEnv  = m_ctrEC2ExpComb.GetCurSel();
			sData.dStressk[0] = m_dk1;
			sData.dStressk[1] = m_dk2;
			sData.dStressk[2] = m_dk3;
			sData.dStressk[3] = m_dk4;
			sData.iCrackEnv   = m_iCrackEnv;
			if(m_iCode == Eurocode2 || m_iCode==Eurocode2_04 || m_iCode==Eurocode2_2_05 || m_iCode==IRC_112_11 || m_iCode == IRC_112_20)
			{
				// TCHARactristic
				sData.dCrackLim[0]  = m_dCharCrack.GetEditValue();
				sData.bCrackFlag[0] = m_bCharCrack;

				// frequency
				sData.dCrackLim[1]  = m_dFreqCrack.GetEditValue();
				sData.bCrackFlag[1] = m_bFreqCrack;

				// quasi-permanent
				sData.dCrackLim[2]  = m_dQuasCrack.GetEditValue();
				sData.bCrackFlag[2] = m_bQuasCrack;
			}
			else if(m_iCode==SNiP_20503_84RC || m_iCode==SP_35_13330_11RC || m_iCode==SNiP_20503_84RC_MKS || m_iCode==SP_35_13330_11RC_MKS  || 
										m_iCode==SNiP_20503_84_CSG || m_iCode==SP_35_13330_11_CSG)
			{
				sData.dCrackLim[0]  = m_edtCrackWLimit.GetEditValue();
				sData.bCrackFlag[0] = m_bCrackW;		
			}
//       else if(m_iCode==Eurocode2_2_05)
//       {
//         m_bCharCrack=FALSE;
//         m_bFreqCrack=FALSE;
//         m_bQuasCrack=FALSE;
//       }
			///////////////////////////////////////////////////////////////////////
			else if (m_iCode==IS456_2000 || m_iCode==IRS_RC )
			{
				// TCHARactristic
				sData.dCrackLim[0]  = m_dCharCrack.GetEditValue();
				sData.bCrackFlag[0] = m_bCharCrack;

				// frequency
				sData.dCrackLim[1]  = m_dCharCrack.GetEditValue();
				sData.bCrackFlag[1] = m_bFreqCrack;

				// quasi-permanent
				sData.dCrackLim[2]  = m_dCharCrack.GetEditValue();
				sData.bCrackFlag[2] = m_bQuasCrack;
			}
			else if(m_iCode == SP_63_1330_2018)
			{
				//short
				sData.dCrackLim[0] = m_dCharCrack.GetEditValue();
				sData.bCrackFlag[0] = m_bCharCrack;
				//long
				sData.dCrackLim[1] = m_dFreqCrack.GetEditValue();
				sData.bCrackFlag[1] = m_bFreqCrack;
			}
			else
			{
				sData.dCrackLim[0]= m_dUserCrack.GetEditValue();
				sData.bCrackFlag[0] = TRUE;
			}
			
			sData.dRH         = m_dRH;
			sData.iDeflecEnv  = m_iDeflectEnv;
			sData.iDeflecEnv_long  = m_iDeflectEnv_Long;
			CArray<double, double> aDefLims; aDefLims.RemoveAll();
			CArray<double, double> aDefLims_Long; aDefLims_Long.RemoveAll();
			switch (m_iCode)
			{
			case Eurocode2:
			case Eurocode2_04:
			case Eurocode2_2_05:
			case IRC_112_11:
			case IRC_112_20:
				aDefLims.Add(500.0);
				aDefLims.Add(250.0);
				break;
			case IS456_2000:
				aDefLims.Add(250.0);
				break;
			case KCI_USD03:
			case KCI_USD07:
			case KCI_USD12:
            case KDS_41_30_2018:
            case KDS_41_20_2022:
			case ACI318_05:
			case ACI318_08:
			case ACI318_11:
			case ACI318_14:
			case ACI318M_14:
            case ACI318_19:
            case ACI318M_19:
			case ACI318_25:
			case ACI318M_25:
			case NSR_10:
            case NSCP_2015_C:
			case TWN_USD112:
			case NTC_DCEC_2017:
			case NTC_DCEC_2023:
			case SP_63_1330_2018:
				aDefLims.Add(480.0); 
				aDefLims.Add(360.0);
				aDefLims.Add(240.0); 
				aDefLims.Add(180.0); 
				break;
			default:
				break;
			}

			aDefLims_Long.Add(480.0); 
			aDefLims_Long.Add(360.0);
			aDefLims_Long.Add(240.0); 
			aDefLims_Long.Add(180.0); 

			if(aDefLims.GetSize() > m_iDeflectEnv)
			{
				m_dUserDeflect = aDefLims.GetAt(m_iDeflectEnv);
			}
			if (m_iCode == IS456_2000)
			{
				m_dUserDeflect = m_dCharacLim;
			}
			if(aDefLims_Long.GetSize() > m_iDeflectEnv_Long)
			{
				m_dUserDeflect_Long = aDefLims_Long.GetAt(m_iDeflectEnv_Long);
			}
			sData.dDefLim_long= m_dUserDeflect_Long;
			sData.dReductFactor=m_dReductFactor;
			sData.dRareDefl   = m_dCharacLim;

			if(IsCodeForMeshedDesignAIJ_JP())
			{
				sData.dDAF		= m_dStrainGrowthRate;
				sData.dDefLim	= m_dStrainLimit;
				sData.dK		= m_dK;
			}
			else
			{
				sData.dDAF      = m_dDAF; // Deflectioin Amplification Factor.
				sData.dDefLim	= m_dUserDeflect;
			}

            if ( m_iCode == Eurocode2_04 && 
				(m_iSubCode == dgn::def::enNationalAnnex::Sweden || m_iSubCode == dgn::def::enNationalAnnex::Sweden19 ||
					m_iSubCode == dgn::def::enNationalAnnex::Norway) ) //National Annex : Sweden
			{
				sData.iServLife      = m_iServLife;
				sData.bHighCorrosion = m_bCorrosionHigh;
			}
			
			sData.iTimeFactor = m_cmbTimeDep.GetCurSel();
			sData.dXi = m_edtTimeDep.GetEditValue();
//       sData.dRareDefl = m_edtTimeDep.GetEditValue();

			if(rSelKey.GetSize() > 0)
			{
	      // Initialize selected Element.
		  	if(m_pDoc->m_pDataCtrl->AddServ(rSelKey,sData))	Initial_SelectItem();
			}
			else
			{
			#ifdef _MGEN
				if(m_bSlabService)  AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_Plate_Con_only),MB_OK);
				else AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_BEAM_Con_only),MB_OK);
			#else
				AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE_BEAM_Con),MB_OK);
			#endif

			}
		}
		else	// Delete
		{
			// Initialize selected Element.
			if(m_pDoc->m_pDataCtrl->DelServ(rSelKey))	Initial_SelectItem();
		}
		
	}
	else	AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM),MB_OK);
	
	//Initial_SelectItem();	
}

// If check the 'Add/Replace' radio button.
void CDgnConServiceLimtDlg::OnDgnConServAr() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);  
	m_nOption  = 0;			//	Add/Replace.
	EnableItems(TRUE);
	SetCrackcItem();
	UpdateData(FALSE);
}

// If check the 'Delete' radio button.
void CDgnConServiceLimtDlg::OnDgnConServDel() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_nOption  = 1;			//	Delete.
	EnableItems(FALSE);
	UpdateData(FALSE);
}

void CDgnConServiceLimtDlg::EnableItems(BOOL bEnable)
{
	GetDlgItem(IDC_DGN_CONC_ExpClassFrm)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_EXP1)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_EXP2)->EnableWindow(bEnable);
	if (m_iCode == AREMA23) {
		bEnable = FALSE;
	}

	if(m_iCode==IS456_2000)
	{
		GetDlgItem(IDC_DGN_CONC_ExpClassFrm)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_EXP1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_EXP2)->EnableWindow(FALSE);
	}

	

		GetDlgItem(IDC_DGN_CONC_StressFrm)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_STRESS1)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_STRESS2)->EnableWindow(bEnable);

		//(2009.06.14) Add by Unsang :: EC2-2:05에서는 아래 입력Tool을 비활성화함.
		// if(m_iCode==Eurocode2_2_05) bEnable = FALSE; (2012.08.10) EC2-04와 같도록 함.

		GetDlgItem(IDC_DGN_CONC_STRESS3)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_STRESS4)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_STRESS5)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_STRESS6)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_STRESS7)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_STRESS8)->EnableWindow(bEnable);
	

	if(m_iCode==IS456_2000)
	{
		GetDlgItem(IDC_DGN_CONC_StressFrm)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_STRESS1)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_STRESS2)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_STRESS3)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_STRESS4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_STRESS5)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_STRESS6)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_STRESS7)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_STRESS8)->EnableWindow(FALSE);

	}

	
		GetDlgItem(IDC_DGN_CONC_CrackFrm1)->EnableWindow(bEnable);
		//GetDlgItem(IDC_DGN_CONC_CRACK1)->EnableWindow(bEnable);
		//GetDlgItem(IDC_DGN_CONC_CRACK2)->EnableWindow(bEnable);
		//GetDlgItem(IDC_DGN_CONC_CRACK3)->EnableWindow(bEnable);

		GetDlgItem(IDC_DGN_CONC_CRACK4)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_CRACK5)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_CRACK6)->EnableWindow(bEnable);

		GetDlgItem(IDC_DGN_CONC_CRACK7)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_CRACK8)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_CRACK9)->EnableWindow(bEnable);

		GetDlgItem(IDC_DGN_CONC_CRACK10)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_CRACK11)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_CRACK12)->EnableWindow(bEnable);

	

	
	if(m_iCode==IS456_2000)
	{
		GetDlgItem(IDC_DGN_CONC_CRACK7)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_CRACK8)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_CRACK9)->EnableWindow(FALSE);

		GetDlgItem(IDC_DGN_CONC_CRACK10)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_CRACK11)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_CRACK12)->EnableWindow(FALSE);
		m_bShowQuas=FALSE;
		m_bShowChar=TRUE;
	}


	if(m_bShowQuas)
		GetDlgItem(IDC_DGN_CONC_QuasCrack)->EnableWindow(bEnable);
	else
		GetDlgItem(IDC_DGN_CONC_QuasCrack)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_DGN_CONC_FreqCrack)->EnableWindow(bEnable);
	if(m_iCode == IS456_2000) GetDlgItem(IDC_DGN_CONC_FreqCrack)->EnableWindow(FALSE);
	else if(m_iCode == SP_63_1330_2018)
	{
		m_bShowChar = TRUE;
	}

	if(m_bShowChar)
		GetDlgItem(IDC_DGN_CONC_CharCrack)->EnableWindow(bEnable);
	else
		GetDlgItem(IDC_DGN_CONC_CharCrack)->EnableWindow(FALSE);

	
		GetDlgItem(IDC_DGN_CONC_CrackFrm2)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_BCRACK1)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_BCRACK2)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_BCRACK3)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_BCRACK4)->EnableWindow(bEnable);
		GetDlgItem(IDC_DGN_CONC_BCRACK5)->EnableWindow(bEnable);
	
	if(m_iCrackEnv==4 && bEnable)
		GetDlgItem(IDC_DGN_CONC_BCRACK6)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_DGN_CONC_BCRACK6)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_CONC_BCRACK7)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_CONC_DeflectFrm)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_BDEFLECT1)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_BDEFLECT2)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_BDEFLECT3)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_BDEFLECT4)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_BDEFLECT5)->EnableWindow(bEnable);
	if(m_iDeflectEnv==4 && bEnable)
		GetDlgItem(IDC_DGN_CONC_BDEFLECT6)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_DGN_CONC_BDEFLECT6)->EnableWindow(FALSE);
	GetDlgItem(IDC_DGN_CONC_DeflectFrm2)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_BDEFLECT7)->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_CONC_DeflectFrm3)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_DEFLECT1)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CONC_DEFLECT2)->EnableWindow(bEnable);

	if(m_iCode == IS456_2000)
	{
		GetDlgItem(IDC_DGN_CONC_BDEFLECT7)->EnableWindow(FALSE);
	}


	//GetDlgItem(IDC_STATIC_SELECT_TYPE)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_CONC_SELECT_ALL)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_CONC_SELECT_SEL)->EnableWindow(bEnable);

	//GetDlgItem(IDC_DGN_EXECUTE)->EnableWindow(bEnable);
}

// 아이템들을 enable / disable 시킨다.
void CDgnConServiceLimtDlg::Update_InitDlgItems()
{
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	if(m_bIsCsgDesign) {bShow = TRUE;}
	else if(IsCodeForMeshedDesignAIJ_JP()) {bShow = TRUE;}
	else if(m_bSlabService) m_pDoc->GetDesignTitleCon(ID_DGN_CON_SERV_MESH, strTitle, bShow, m_pDoc->IsPostMode());
	else {m_pDoc->GetDesignTitleCon(ID_DGN_CON_SERV, strTitle, bShow, m_pDoc->IsPostMode());}

	EnableItems(bShow);    
	SetCrackcItem();
	GetDlgItem(IDC_DGN_CON_SERV_AR)->EnableWindow(bShow);    
	GetDlgItem(IDC_DGN_CON_SERV_DEL)->EnableWindow(bShow);    
	
	GetDlgItem(IDC_STATIC_SELECT_TYPE)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_CONC_SELECT_ALL)->EnableWindow(bShow);
	GetDlgItem(IDC_DGN_CONC_SELECT_SEL)->EnableWindow(bShow);	
}

// 선택된 element를 모두 unselect시킨다.
void CDgnConServiceLimtDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	Initial_Data();
	OnDgnCrackWidthChk();
}

// crack의 radio button을 클릭할 경우 호출.
void CDgnConServiceLimtDlg::OnDgnConCrackUser() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	BOOL bEnable=FALSE;
	if(m_iCrackEnv==4)  bEnable=TRUE;
	else                bEnable=FALSE;
	GetDlgItem(IDC_DGN_CONC_BCRACK6)->EnableWindow(bEnable);

	UpdateData(FALSE);
}

// deflectioin의 radio button을 클릭할 경우 호출.
void CDgnConServiceLimtDlg::OnDgnConDeflectUser() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	BOOL bEnable=FALSE;
	if(m_iDeflectEnv==4)  bEnable=TRUE;
	else                  bEnable=FALSE;
	GetDlgItem(IDC_DGN_CONC_BDEFLECT6)->EnableWindow(bEnable);	

	UpdateData(FALSE);
}

void CDgnConServiceLimtDlg::OnDgnConDeflectUserLong() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	BOOL bEnable=FALSE;
	if(m_iDeflectEnv_Long==4)  bEnable=TRUE;
	else                       bEnable=FALSE;
	GetDlgItem(IDC_DGN_CONC_BDEFLECT13)->EnableWindow(bEnable);	

	UpdateData(FALSE);
}
// Service Life Class의 radio button을 클릭할 경우 호출.
void CDgnConServiceLimtDlg::OnDgnConServLife()
{
	UpdateData(TRUE);
	
	if      (m_RdoServLife100.GetCheck() == BST_CHECKED) {m_iServLife = 0; }
	else if (m_RdoServLife50 .GetCheck() == BST_CHECKED) {m_iServLife = 1;}
	else if (m_RdoServLife20 .GetCheck() == BST_CHECKED) {m_iServLife = 2;}
	else ASSERT(0);

	int iIndex = m_ctrEC2ExpComb.GetCurSel();
	
	GetCrackLimByEC2(iIndex);
	
	UpdateData(FALSE);
}

void CDgnConServiceLimtDlg::OnDgnConCorrosion()
{
	UpdateData(TRUE);
	
	if      (m_RdoCorrosionHigh.GetCheck() == BST_CHECKED) {m_bCorrosionHigh = TRUE;}
	else if (m_RdoCorrosionLow .GetCheck() == BST_CHECKED) {m_bCorrosionHigh = FALSE;}
	else ASSERT(0);

	int iIndex = m_ctrEC2ExpComb.GetCurSel();
	
	GetCrackLimByEC2(iIndex);
	
	UpdateData(FALSE);
}


BOOL CDgnConServiceLimtDlg::GetDesignCsgCodeNo()
{
	T_CPGD_D CpgdD;
	if(!m_pDoc->m_pAttrCtrl->GetCpgd(CpgdD)) return FALSE;

	m_iCode = CpgdD.iDgnCode;

	return TRUE;
}


// Coded by Seungjun MNet:No.2393 ('20060918)
void CDgnConServiceLimtDlg::OnSelchangeDgnConcExp() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int iIndex = m_ctrEC2ExpComb.GetCurSel();

	if (m_iCode != AREMA23) {
		GetCrackLimByEC2(iIndex);

		// for stress for Eurocode.
		switch (iIndex)
		{
		case(X0):
		case(XC1):
		case(XC2):
		case(XC3):
		case(XC4):
		{
			EnableItems(TRUE);
			//GetDlgItem(IDC_DGN_CONC_STRESS2)->EnableWindow(FALSE);        
		}
		break;
		case(XD1):
		case(XD2):
		case(XS1):
		case(XS2):
		case(XS3):
		{
			EnableItems(TRUE);
		}
		break;
		case(XD3):
		case(XF1):
		case(XF2):
		case(XF3):
		case(XF4):
		{
			EnableItems(TRUE);
			GetDlgItem(IDC_DGN_CONC_CRACK2)->EnableWindow(FALSE);
		}
		break;
		case(XA1):
		case(XA2):
		case(XA3):
		{
			EnableItems(TRUE);
			GetDlgItem(IDC_DGN_CONC_CRACK2)->EnableWindow(FALSE);
		}
		break;
		default: ASSERT(0);
		}
	}

	// stress factor 

	// Crack width for Eurocode
	if (m_iCode == Eurocode2_04)
	{
		CDgnDataCtrl DataCtrl;
		EN_EC2_EXPOSURE_CLASS enExClass  = static_cast<EN_EC2_EXPOSURE_CLASS>(iIndex);
		EN_EC2_SWE_SERV_LIFE  enServLife = static_cast<EN_EC2_SWE_SERV_LIFE>(m_iServLife);
		CDgnDataCtrl DgnDataCtrl;
		T_CRACK_EC2 CrackD;
		DgnDataCtrl.Get_CrackData_EN04(m_iSubCode, enExClass, enServLife, m_bCorrosionHigh, CrackD);

		m_bShowChar = CrackD.bActivate[EN_LCOM_CHAR];
		m_bShowQuas = CrackD.bActivate[EN_LCOM_QUAS];

		GetDlgItem(IDC_DGN_CONC_CharCrack)->EnableWindow(CrackD.bActivate[EN_LCOM_CHAR]);
		GetDlgItem(IDC_DGN_CONC_FreqCrack)->EnableWindow(CrackD.bActivate[EN_LCOM_FREQ]);
		GetDlgItem(IDC_DGN_CONC_QuasCrack)->EnableWindow(CrackD.bActivate[EN_LCOM_QUAS]);
		
		GetDlgItem(IDC_DGN_CONC_CRACK4)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_CRACK5)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_CRACK6)->EnableWindow(FALSE);

		m_bCharCrack = CrackD.bChkFlag[EN_LCOM_CHAR];
		m_bFreqCrack = CrackD.bChkFlag[EN_LCOM_FREQ];
		m_bQuasCrack = CrackD.bChkFlag[EN_LCOM_QUAS];

		GetDlgItem(IDC_DGN_CONC_CRACK4)->EnableWindow(m_bShowChar);
		GetDlgItem(IDC_DGN_CONC_CRACK5)->EnableWindow(m_bShowChar);
		GetDlgItem(IDC_DGN_CONC_CRACK6)->EnableWindow(m_bShowChar); 

		GetDlgItem(IDC_DGN_CONC_CRACK7)->EnableWindow(CrackD.bActivate[EN_LCOM_FREQ]);
		GetDlgItem(IDC_DGN_CONC_CRACK8)->EnableWindow(CrackD.bActivate[EN_LCOM_FREQ]);
		GetDlgItem(IDC_DGN_CONC_CRACK9)->EnableWindow(CrackD.bActivate[EN_LCOM_FREQ]);

		GetDlgItem(IDC_DGN_CONC_CRACK10)->EnableWindow(m_bShowQuas);
		GetDlgItem(IDC_DGN_CONC_CRACK11)->EnableWindow(m_bShowQuas);
		GetDlgItem(IDC_DGN_CONC_CRACK12)->EnableWindow(m_bShowQuas);

		Initial_StressFactor_EN04(iIndex);
	}
	else if (m_iCode == Eurocode2_2_05)
	{
		const BOOL bEnableQuas = dgn::EQ0(m_dQuasCrack.GetEditValue()) ? FALSE : TRUE;
		const BOOL bEnableFreq = dgn::EQ0(m_dFreqCrack.GetEditValue()) ? FALSE : TRUE;
		const BOOL bEnableChar = FALSE;

		GetDlgItem(IDC_DGN_CONC_QuasCrack)->EnableWindow(bEnableQuas);
		GetDlgItem(IDC_DGN_CONC_FreqCrack)->EnableWindow(bEnableFreq);
		GetDlgItem(IDC_DGN_CONC_CharCrack)->EnableWindow(bEnableChar);

		GetDlgItem(IDC_DGN_CONC_CRACK4)->EnableWindow(bEnableQuas);
		GetDlgItem(IDC_DGN_CONC_CRACK5)->EnableWindow(bEnableQuas);
		GetDlgItem(IDC_DGN_CONC_CRACK6)->EnableWindow(bEnableQuas);

		GetDlgItem(IDC_DGN_CONC_CRACK7)->EnableWindow(bEnableFreq);
		GetDlgItem(IDC_DGN_CONC_CRACK8)->EnableWindow(bEnableFreq);
		GetDlgItem(IDC_DGN_CONC_CRACK9)->EnableWindow(bEnableFreq);

		GetDlgItem(IDC_DGN_CONC_CRACK10)->EnableWindow(bEnableChar);
		GetDlgItem(IDC_DGN_CONC_CRACK11)->EnableWindow(bEnableChar);
		GetDlgItem(IDC_DGN_CONC_CRACK12)->EnableWindow(bEnableChar);

		m_bShowChar = TRUE;
		m_bShowQuas = TRUE;

		m_bCharCrack = bEnableChar;
		m_bFreqCrack = bEnableFreq;
		m_bQuasCrack = bEnableQuas;

		EnableItems(TRUE);
		SetCrackcItem();
	}
	else if (m_iCode == AREMA23)
	{
		//const BOOL bEnableQuas = dgn::EQ0(m_dQuasCrack.GetEditValue()) ? FALSE : TRUE;
		//const BOOL bEnableFreq = dgn::EQ0(m_dFreqCrack.GetEditValue()) ? FALSE : TRUE;
		//const BOOL bEnableChar = FALSE;

		//GetDlgItem(IDC_DGN_CONC_QuasCrack)->EnableWindow(bEnableQuas);
		//GetDlgItem(IDC_DGN_CONC_FreqCrack)->EnableWindow(bEnableFreq);
		//GetDlgItem(IDC_DGN_CONC_CharCrack)->EnableWindow(bEnableChar);

		//GetDlgItem(IDC_DGN_CONC_CRACK4)->EnableWindow(bEnableQuas);
		//GetDlgItem(IDC_DGN_CONC_CRACK5)->EnableWindow(bEnableQuas);
		//GetDlgItem(IDC_DGN_CONC_CRACK6)->EnableWindow(bEnableQuas);

		//GetDlgItem(IDC_DGN_CONC_CRACK7)->EnableWindow(bEnableFreq);
		//GetDlgItem(IDC_DGN_CONC_CRACK8)->EnableWindow(bEnableFreq);
		//GetDlgItem(IDC_DGN_CONC_CRACK9)->EnableWindow(bEnableFreq);

		//GetDlgItem(IDC_DGN_CONC_CRACK10)->EnableWindow(bEnableChar);
		//GetDlgItem(IDC_DGN_CONC_CRACK11)->EnableWindow(bEnableChar);
		//GetDlgItem(IDC_DGN_CONC_CRACK12)->EnableWindow(bEnableChar);

		//m_bShowChar = TRUE;
		//m_bShowQuas = TRUE;

		//m_bCharCrack = bEnableChar;
		//m_bFreqCrack = bEnableFreq;
		//m_bQuasCrack = bEnableQuas;

		//EnableItems(TRUE);
		//SetCrackcItem();
	}
	else
	{
		ASSERT(0);
	}	

	if ( m_iCode == Eurocode2_2_05 )
    {
        auto enDgnCode = dgn::converter::CDgnTypeConverter::GetDgnCode(m_iCode, m_iSubCode);
        dgn::lib::CNationalAnnexInterface::Instance()->SetCode(enDgnCode);

        def::enk1Stress Type;
        dgn::lib::CNationalAnnexInterface::Instance()->GetParamType(Type);

        dgn::def::TNAParam Param;
        switch ( Type )
        {
        case dgn::def::enk1Stress::None:
            break;
        case dgn::def::enk1Stress::ExposureClass:
            {
				Param.ExposureClass = [iIndex] ()
				{
					switch ( iIndex )
					{
					case X0:
                        return dgn::def::enExposureClassEU::X0;
					case XC1:
						return dgn::def::enExposureClassEU::XC1;
					case XC2:
						return dgn::def::enExposureClassEU::XC2;
					case XC3:
						return dgn::def::enExposureClassEU::XC3;
					case XC4:
						return dgn::def::enExposureClassEU::XC4;
					case XD1:
						return dgn::def::enExposureClassEU::XD1;
					case XD2:
						return dgn::def::enExposureClassEU::XD2;
					case XD3:
						return dgn::def::enExposureClassEU::XD3;
					case XS1:
						return dgn::def::enExposureClassEU::XS1;
					case XS2:
						return dgn::def::enExposureClassEU::XS2;
					case XS3:
						return dgn::def::enExposureClassEU::XS3;
					case XF1:
						return dgn::def::enExposureClassEU::XF1;
					case XF2:
						return dgn::def::enExposureClassEU::XF2;
					case XF3:
						return dgn::def::enExposureClassEU::XF3;
					case XF4:
						return dgn::def::enExposureClassEU::XF4;
					case XA1:
						return dgn::def::enExposureClassEU::XA1;
					case XA2:
						return dgn::def::enExposureClassEU::XA2;
					case XA3:
						return dgn::def::enExposureClassEU::XA3;
						break;
					default:
						ASSERT(0);
						break;
					}
					return dgn::def::enExposureClassEU::X0;
				}();
            }
            break;
        default:
            break;
        }

        m_dk1 = dgn::lib::CNationalAnnexInterface::Instance()->Getk1StressLimitation(Type, Param);
        m_dk2 = dgn::lib::CNationalAnnexInterface::Instance()->Getk2StressLimitation();
        m_dk3 = dgn::lib::CNationalAnnexInterface::Instance()->Getk3StressLimitation();
        m_dk4 = dgn::lib::CNationalAnnexInterface::Instance()->Getk4StressLimitation();
    }

	UpdateData(FALSE);
}

// Coded by Seungjun MNet:No.2393 ('20061025) For EC2
void CDgnConServiceLimtDlg::SetCrackcItem() 
{
	if(m_iCode == SP_63_1330_2018)
	{
		GetDlgItem(IDC_DGN_CONC_CRACK4)->EnableWindow(m_bCharCrack);
		GetDlgItem(IDC_DGN_CONC_CRACK5)->EnableWindow(m_bCharCrack);
		GetDlgItem(IDC_DGN_CONC_CRACK6)->EnableWindow(m_bCharCrack);
		GetDlgItem(IDC_DGN_CONC_CRACK7)->EnableWindow(m_bFreqCrack);
		GetDlgItem(IDC_DGN_CONC_CRACK8)->EnableWindow(m_bFreqCrack);
		GetDlgItem(IDC_DGN_CONC_CRACK9)->EnableWindow(m_bFreqCrack);
		return;
	}
	int iIndex = m_ctrEC2ExpComb.GetCurSel();

	if (m_iCode == Eurocode2_2_05)
	{
		GetDlgItem(IDC_DGN_CONC_CRACK4)->EnableWindow(m_bCharCrack);
		GetDlgItem(IDC_DGN_CONC_CRACK5)->EnableWindow(m_bCharCrack);
		GetDlgItem(IDC_DGN_CONC_CRACK6)->EnableWindow(m_bCharCrack);

		GetDlgItem(IDC_DGN_CONC_CRACK7)->EnableWindow(m_bFreqCrack);
		GetDlgItem(IDC_DGN_CONC_CRACK8)->EnableWindow(m_bFreqCrack);
		GetDlgItem(IDC_DGN_CONC_CRACK9)->EnableWindow(m_bFreqCrack);

		GetDlgItem(IDC_DGN_CONC_CRACK10)->EnableWindow(m_bQuasCrack);
		GetDlgItem(IDC_DGN_CONC_CRACK11)->EnableWindow(m_bQuasCrack);
		GetDlgItem(IDC_DGN_CONC_CRACK12)->EnableWindow(m_bQuasCrack);
	}
	else
	{
		switch (iIndex)
		{
		case(X0):
		case(XC1):
		case(XC2):
		case(XC3):
		case(XC4):
		case(XD1):
		case(XD2):
		case(XD3):
		case(XS1):
		case(XS2):
		case(XS3):
		case(IRC_Extreme):
		case(IRC_Moderate):
		case(IRC_Severe):
		case(IRC_Very_Severe):
			{
				GetDlgItem(IDC_DGN_CONC_CRACK4)->EnableWindow(FALSE);
				GetDlgItem(IDC_DGN_CONC_CRACK5)->EnableWindow(FALSE);
				GetDlgItem(IDC_DGN_CONC_CRACK6)->EnableWindow(FALSE);

				if (m_bFreqCrack)
				{
					GetDlgItem(IDC_DGN_CONC_CRACK7)->EnableWindow(TRUE);
					GetDlgItem(IDC_DGN_CONC_CRACK8)->EnableWindow(TRUE);
					GetDlgItem(IDC_DGN_CONC_CRACK9)->EnableWindow(TRUE);
				}
				else
				{
					GetDlgItem(IDC_DGN_CONC_CRACK7)->EnableWindow(FALSE);
					GetDlgItem(IDC_DGN_CONC_CRACK8)->EnableWindow(FALSE);
					GetDlgItem(IDC_DGN_CONC_CRACK9)->EnableWindow(FALSE);
				}
				if (m_bQuasCrack)
				{
					GetDlgItem(IDC_DGN_CONC_CRACK10)->EnableWindow(TRUE);
					GetDlgItem(IDC_DGN_CONC_CRACK11)->EnableWindow(TRUE);
					GetDlgItem(IDC_DGN_CONC_CRACK12)->EnableWindow(TRUE);
				}
				else
				{
					GetDlgItem(IDC_DGN_CONC_CRACK10)->EnableWindow(FALSE);
					GetDlgItem(IDC_DGN_CONC_CRACK11)->EnableWindow(FALSE);
					GetDlgItem(IDC_DGN_CONC_CRACK12)->EnableWindow(FALSE);
				}
			}
			break;
		case(XF1):
		case(XF2):
		case(XF3):
		case(XF4):
		case(XA1):
		case(XA2):
		case(XA3):
			{
				GetDlgItem(IDC_DGN_CONC_CRACK10)->EnableWindow(FALSE);
				GetDlgItem(IDC_DGN_CONC_CRACK11)->EnableWindow(FALSE);
				GetDlgItem(IDC_DGN_CONC_CRACK12)->EnableWindow(FALSE);

				if (m_bCharCrack)
				{
					GetDlgItem(IDC_DGN_CONC_CRACK4)->EnableWindow(TRUE);
					GetDlgItem(IDC_DGN_CONC_CRACK5)->EnableWindow(TRUE);
					GetDlgItem(IDC_DGN_CONC_CRACK6)->EnableWindow(TRUE);
				}
				else
				{
					GetDlgItem(IDC_DGN_CONC_CRACK4)->EnableWindow(FALSE);
					GetDlgItem(IDC_DGN_CONC_CRACK5)->EnableWindow(FALSE);
					GetDlgItem(IDC_DGN_CONC_CRACK6)->EnableWindow(FALSE);
				}
				if (m_bFreqCrack)
				{
					GetDlgItem(IDC_DGN_CONC_CRACK7)->EnableWindow(TRUE);
					GetDlgItem(IDC_DGN_CONC_CRACK8)->EnableWindow(TRUE);
					GetDlgItem(IDC_DGN_CONC_CRACK9)->EnableWindow(TRUE);
				}
				else
				{
					GetDlgItem(IDC_DGN_CONC_CRACK7)->EnableWindow(FALSE);
					GetDlgItem(IDC_DGN_CONC_CRACK8)->EnableWindow(FALSE);
					GetDlgItem(IDC_DGN_CONC_CRACK9)->EnableWindow(FALSE);
				}
			}
			break;
		default:
			ASSERT(0);
			break;
		}
	}
		
	if (m_iCode == IS456_2000)
	{
		//GetDlgItem(IDC_DGN_CONC_CharCrack)->EnableWindow(TRUE);
		//GetDlgItem(IDC_DGN_CONC_CRACK5)->EnableWindow(TRUE);
		//GetDlgItem(IDC_DGN_CONC_CRACK11)->EnableWindow(TRUE);
		if (m_bCharCrack)
		{
			GetDlgItem(IDC_DGN_CONC_CRACK4)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_CONC_CRACK5)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_CONC_CRACK6)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_DGN_CONC_CRACK4)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_CONC_CRACK5)->EnableWindow(FALSE);
			GetDlgItem(IDC_DGN_CONC_CRACK6)->EnableWindow(FALSE);
		}


		//QP Disable
		GetDlgItem(IDC_DGN_CONC_QuasCrack)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_CRACK10)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_CRACK11)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CONC_CRACK12)->EnableWindow(FALSE);
	}
	UpdateData(FALSE);
}
// Coded by Seungjun MNet:No.2393 ('20061025)
void CDgnConServiceLimtDlg::OnDgnConCrackcCheck() 
{
	UpdateData(TRUE);

	SetCrackcItem();
}

// Coded by Seungjun MNet:No.2393 ('20061025)
void CDgnConServiceLimtDlg::GetCrackLimByEC2(int iExClass)
{
	// Exposure Class
	// X0 XC1 XC2 XC3 XC4 XD1 XD2 XD3 XS1 XS2 XS3 XF1 XF2 XF3 XF4 XA1 XA2 XA3
	// 0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17
	//m_nBefore_Length = D_UNITSYS_LENGTH_INDEX_MM;

	if (m_iCode == Eurocode2_04)
	{
		EN_EC2_EXPOSURE_CLASS enExClass  = static_cast<EN_EC2_EXPOSURE_CLASS>(iExClass);
		EN_EC2_SWE_SERV_LIFE  enServLife = static_cast<EN_EC2_SWE_SERV_LIFE>(m_iServLife);
		CDgnDataCtrl DgnDataCtrl;
		T_CRACK_EC2 CrackD;
		DgnDataCtrl.Get_CrackData_EN04(m_iSubCode, enExClass, enServLife, m_bCorrosionHigh, CrackD);

		m_dCharCrack.SetEditUnit(CrackD.dCrkLim[EN_LCOM_CHAR]);
		m_dFreqCrack.SetEditUnit(CrackD.dCrkLim[EN_LCOM_FREQ]);
		m_dQuasCrack.SetEditUnit(CrackD.dCrkLim[EN_LCOM_QUAS]);
	}
	else if (m_iCode == Eurocode2_2_05)
	{
		CDgnDataCtrl DgnDataCtrl;
		T_DCON_D DconD;
		DgnDataCtrl.Get_DgnConDcon(DconD);
		const int nLife = static_cast<int>(DconD.enSL); /// only SWE available.

		double dWmaxQ = 0.0;
		double dWmaxF = 0.0;
		DgnDataCtrl.Get_DgnConCrackDef_EC2_05(m_iSubCode, iExClass, nLife, dWmaxQ, dWmaxF);

		m_dQuasCrack.SetEditUnit(dWmaxQ);
		m_dFreqCrack.SetEditUnit(dWmaxF);
		m_dCharCrack.SetEditUnit(0.0);
	}
	else
	{
		ASSERT(0);
	}
}

void CDgnConServiceLimtDlg::GetCrackLimByIS456()
{
	// Exposure Class
	// X0 XC1 XC2 XC3 XC4 XD1 XD2 XD3 XS1 XS2 XS3 XF1 XF2 XF3 XF4 XA1 XA2 XA3
	// 0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17
	//m_nBefore_Length = D_UNITSYS_LENGTH_INDEX_MM;

	CDgnDataCtrl*	pDgnDataCtrl = new CDgnDataCtrl;
	double dCrackLim[4] = {0.0};

	pDgnDataCtrl->Get_DgnConCrackDef(CDBLib::GetConCodeName(), m_iSubCode, dCrackLim);
    	
	m_dCharCrack.SetEditUnit(dCrackLim[1]);
	delete pDgnDataCtrl;
	pDgnDataCtrl = NULL;  
}

void CDgnConServiceLimtDlg::GetCrackLimByIRC112()
{
	// Exposure Class
	// X0 XC1 XC2 XC3 XC4 XD1 XD2 XD3 XS1 XS2 XS3 XF1 XF2 XF3 XF4 XA1 XA2 XA3
	// 0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17
    //m_nBefore_Length = D_UNITSYS_LENGTH_INDEX_MM;

    CDgnDataCtrl*	pDgnDataCtrl = new CDgnDataCtrl;
    double dCrackLim[4] = { 0.0 };

    pDgnDataCtrl->Get_DgnConCrackDef(CDBLib::GetConCodeName(), m_iSubCode, dCrackLim);

    m_dQuasCrack.SetEditUnit(dCrackLim[2]);

    //// Frequent.
    m_dFreqCrack.SetEditUnit(dCrackLim[3]);

    //// Characteristic
    m_dCharCrack.SetEditUnit(dCrackLim[1]);  // Rare.
	
	delete pDgnDataCtrl;
	pDgnDataCtrl = NULL;  
}

double CDgnConServiceLimtDlg::SetCrackWidth_CorrosionHigh(int ServLife, int iExClass)
{
	// Exposure Class
	// X0 XC1 XC2 XC3 XC4 XD1 XD2 XD3 XS1 XS2 XS3 XF1 XF2 XF3 XF4 XA1 XA2 XA3
	// 0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17

	int iCodeLenUnit = D_UNITSYS_LENGTH_INDEX_MM;
	CDgnDataCtrl*	pDgnDataCtrl = new CDgnDataCtrl;
	double dFlen = pDgnDataCtrl->Change_Flen(iCodeLenUnit);

	double dCrackWidth = 0.0;
	switch(ServLife)
	{
		case 0:
		{
			if      (iExClass == 1                  )                                  dCrackWidth = 0.40;
			else if (iExClass == 2                  )                                  dCrackWidth = 0.30;
			else if (iExClass == 3 || iExClass == 4 )                                  dCrackWidth = 0.20;
			else if (iExClass == 5 || iExClass == 6 || iExClass == 8 || iExClass == 9) dCrackWidth = 0.15;
			else if (iExClass == 7 || iExClass == 10)                                  dCrackWidth = 0.10;
			else {dCrackWidth = 0.0;} //Limit Check Skip.
			break;
		}
		case 1:
		{
			if      (iExClass == 1                  )                                  dCrackWidth = 0.45;
			else if (iExClass == 2                  )                                  dCrackWidth = 0.40;
			else if (iExClass == 3 || iExClass == 4 )                                  dCrackWidth = 0.30;
			else if (iExClass == 5 || iExClass == 6 || iExClass == 8 || iExClass == 9) dCrackWidth = 0.20;
			else if (iExClass == 7 || iExClass == 10)                                  dCrackWidth = 0.15;
			else {dCrackWidth = 0.0;} //Limit Check Skip.
			break;
		}
		case 2:
		{
			if      (iExClass == 2                  )                                  dCrackWidth = 0.45;
			else if (iExClass == 3 || iExClass == 4 )                                  dCrackWidth = 0.40;
			else if (iExClass == 5 || iExClass == 6 || iExClass == 8 || iExClass == 9) dCrackWidth = 0.30;
			else if (iExClass == 7 || iExClass == 10)                                  dCrackWidth = 0.20;
			else {dCrackWidth = 0.0;} //Limit Check Skip.
			break;
		}
	}

	delete pDgnDataCtrl;
	pDgnDataCtrl = NULL;  

	return dCrackWidth/=dFlen;
}

double CDgnConServiceLimtDlg::SetCrackWidth_CorrosionLow(int ServLife, int iExClass)
{
	int iCodeLenUnit = D_UNITSYS_LENGTH_INDEX_MM;
	CDgnDataCtrl*	pDgnDataCtrl = new CDgnDataCtrl;
	double dFlen = pDgnDataCtrl->Change_Flen(iCodeLenUnit);

	// Exposure Class
	// X0 XC1 XC2 XC3 XC4 XD1 XD2 XD3 XS1 XS2 XS3 XF1 XF2 XF3 XF4 XA1 XA2 XA3
	// 0   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17
	double dCrackWidth = 0.0;
	switch(ServLife)
	{
		case 0:
		{
			if      (iExClass == 1                  )                                  dCrackWidth = 0.45;
			else if (iExClass == 2                  )                                  dCrackWidth = 0.40;
			else if (iExClass == 3 || iExClass == 4 )                                  dCrackWidth = 0.30;
			else if (iExClass == 5 || iExClass == 6 || iExClass == 8 || iExClass == 9) dCrackWidth = 0.20;
			else if (iExClass == 7 || iExClass == 10)                                  dCrackWidth = 0.15;
			else {dCrackWidth = 0.0;} //Limit Check Skip.
			break;
		}
		case 1:
		{
			if      (iExClass == 2                  )                                  dCrackWidth = 0.45;
			else if (iExClass == 3 || iExClass == 4 )                                  dCrackWidth = 0.40;
			else if (iExClass == 5 || iExClass == 6 || iExClass == 8 || iExClass == 9) dCrackWidth = 0.30;
			else if (iExClass == 7 || iExClass == 10)                                  dCrackWidth = 0.20;
			else {dCrackWidth = 0.0;} //Limit Check Skip.
			break;
		}
		case 2:
		{
			if      (iExClass == 5 || iExClass == 6 || iExClass == 8 || iExClass == 9) dCrackWidth = 0.40;
			else if (iExClass == 7 || iExClass == 10)                                  dCrackWidth = 0.30;
			else {dCrackWidth = 0.0;} //Limit Check Skip.
			break;
		}
	}
	delete pDgnDataCtrl;
	pDgnDataCtrl = NULL;  
	
	return dCrackWidth/=dFlen;
}

void CDgnConServiceLimtDlg::OnDgnCrackWidthChk()
{
	UpdateData(TRUE);

// 	GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_STC)->EnableWindow(m_bCrackW);
// 	GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_EDT)->EnableWindow(m_bCrackW);
// 	GetDlgItem(IDC_DGN_CONC_CRACK_WIDTH_LIMIT_UNT)->EnableWindow(m_bCrackW);
}	

const int TIME_CMB_IDX_USER=4;
void CDgnConServiceLimtDlg::OnSelChangeTimeCmb()
{
	int nIndex = m_cmbTimeDep.GetCurSel();
	if(nIndex==-1) return;

	double dXi;
	switch(nIndex)
	{
	case 0: dXi = 2.0; break;
	case 1: dXi = 1.4; break;
	case 2: dXi = 1.2; break;
	case 3: dXi = 1.0; break;
	}

	BOOL bUserInput = nIndex==TIME_CMB_IDX_USER ? TRUE:FALSE;
	m_edtTimeDep.SetReadOnly(!bUserInput);
	if(!bUserInput) m_edtTimeDep.SetEditUnit(dXi);
}

BOOL CDgnConServiceLimtDlg::IsCodeForMeshedDesignAIJ_JP()
{
	return (m_bSlabService && CDBLib::IsCodeForMeshedDesignAIJ_JP());
}