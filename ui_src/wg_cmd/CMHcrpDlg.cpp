// CMHcrpDlg.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMHcrpDlg.h"
#include "CMTimeDepMatFuncDlg.h"
#include "MTdmfItemShowDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_dbLock\WebLockUtil.h"
#include "..\wg_dbLock\LockOption.h"

#include "..\wg_base\wg_base_StrParser.h"  

#include "..\wg_base\TestEnvMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMHcrpDlg dialog
//
CCMHcrpDlg::CCMHcrpDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMHcrpDlg::IDD, pParent)
{
	m_pJapanDlg = 0;
	m_pChJTG04Dlg = 0;
	m_pEuropeanDlg = 0;
	m_pIndiaDlg = 0;
	m_pIndia2011Dlg = 0;
	m_pIndia2011_2020Dlg = 0;
	m_pAASHTODlg = 0;
	m_pASDlg = 0;
	m_pRusDlg = 0;
	m_pChJTG16Dlg = 0;
	m_pNZDlg = 0;
	m_pKDS2016Dlg = 0;
	m_pJSCE07Dlg = 0;
	m_pJSCE12Dlg = 0;
	m_pABNT_NBR_6118Dlg = 0;

	m_bIsTimeDependentMatMode= FALSE;   // Time dependent material type �Է� ��� �ΰ�..
	m_nOptionForTimeDep      = 0;       // (0) Add (1) Modify 
	m_type = 2;
	//{{AFX_DATA_INIT(CCMHcrpDlg)
	m_strTDefMatName = _T("");
	m_curemethod = 0;
	m_cementType = 0;

	m_combLFac = 0;
	m_combvsratio = 0;
	m_combprog = 0;
	m_combsvsratio = 0;
	m_ACIorUser = 0;
	m_jpancemen = 0;
	m_jpanhumid = 1;
	m_chinaRelradio = 1;
	m_bSSTR = FALSE;		
	m_bPCAEffect = TRUE;
	//}}AFX_DATA_INIT

	// initialize CArray type member 
	m_acigroup       .RemoveAll();
	m_acishrinkgroup .RemoveAll();
	m_cebgroup       .RemoveAll();
	m_cebshrinkgroup .RemoveAll();
	m_memgroup       .RemoveAll();
	m_udefgroup      .RemoveAll();
	m_udefcrp        .RemoveAll();
	m_udefshr        .RemoveAll();
	m_combroup       .RemoveAll();
	m_pcagroup       .RemoveAll();
	m_aciacigroup    .RemoveAll();
	m_aciUsergroup   .RemoveAll();
	m_buttongroup    .RemoveAll();
	m_japangroup     .RemoveAll();
	m_chinagroup     .RemoveAll();
	m_jscegroup      .RemoveAll();
	m_ChJTG04Group   .RemoveAll();

	//-----------------------------------------------------------------------------
	// ACI Group
	//-----------------------------------------------------------------------------
	m_acigroup.Add(IDC_CMD_ACI_FRAME);    
	m_acigroup.Add(IDC_CMD_HCRP_STATIC1);         m_acigroup.Add(IDC_CMD_HCRP_STATIC2);
	m_acigroup.Add(IDC_CMD_HCRP_STATIC3);         m_acigroup.Add(IDC_CMD_HCRP_STATIC4);
	m_acigroup.Add(IDC_CMD_HCRP_STATIC5);         m_acigroup.Add(IDC_CMD_HCRP_STATIC6);
	m_acigroup.Add(IDC_CMD_HCRP_STATIC7);         m_acigroup.Add(IDC_CMD_HCRP_STATIC8);
	m_acigroup.Add(IDC_CMD_HCRP_STATIC9);         m_acigroup.Add(IDC_CMD_HCRP_ACI_COMP);       
	m_acigroup.Add(IDC_CMD_HCRP_ACI_COMP_UNIT);   m_acigroup.Add(IDC_CMD_HCRP_ACI_RELA);
	m_acigroup.Add(IDC_CMD_HCRP_ACI_RELA_UNIT);
	m_acigroup.Add(IDC_CMD_HCRP_ACI_MOIST_CHK);   m_acigroup.Add(IDC_CMD_HCRP_ACI_STEAM_CHK);
	m_acigroup.Add(IDC_CMD_HCRP_ACI_VOLU);        m_acigroup.Add(IDC_CMD_HCRP_ACI_VOLU_UNIT);
	m_acigroup.Add(IDC_CMD_HCRP_ACI_SLUM);        m_acigroup.Add(IDC_CMD_HCRP_ACI_SLUM_UNIT);
	m_acigroup.Add(IDC_CMD_HCRP_ACI_FINE);        
	m_acigroup.Add(IDC_CMD_HCRP_ACI_FINE_UNIT);   m_acigroup.Add(IDC_CMD_HCRP_ACI_AIR);
	m_acigroup.Add(IDC_CMD_HCRP_ACI_AIR_UNIT);
	m_acigroup.Add(IDC_CMD_HCRP_ACI_AGE);         
	m_acigroup.Add(IDC_CMD_HCRP_ACI_AGE_UNIT);    m_acigroup.Add(IDC_CMD_HCRP_ACI_CEME);
	m_acigroup.Add(IDC_CMD_HCRP_ACI_CEME_UNIT);   m_acigroup.Add(IDC_CMD_HCRP_STATIC101);
	m_acigroup.Add(IDC_CMD_ACI_CREEP_COEF);       m_acigroup.Add(IDC_CMD_HCRP_STATIC102);
	m_acigroup.Add(IDC_CMD_ACI_SRKG_STRN);        m_acigroup.Add(IDC_CMD_ACI_SRKG_STRN_UNIT);
	m_acigroup.Add(IDC_ACI_FRAME2);               m_acigroup.Add(IDC_CMD_ACI_ACI_RD);
	m_acigroup.Add(IDC_CMD_ACI_USER_RD);          m_acigroup.Add(IDC_ACI_FRAME_TYPE);
	m_acigroup.Add(IDC_CMD_HCRP_CONC_STATIC);          
	m_acigroup.Add(IDC_CMD_HCRP_CONC_A_STATIC);   m_acigroup.Add(IDC_CMD_HCRP_ACI_CONC_A);
	m_acigroup.Add(IDC_CMD_HCRP_CONC_A2_STATIC);  m_acigroup.Add(IDC_CMD_HCRP_CONC_B_STATIC);
	m_acigroup.Add(IDC_CMD_HCRP_ACI_CONC_B);      m_acigroup.Add(IDC_CMD_HCRP_CONC_B2_STATIC);

	m_acishrinkgroup.Add(IDC_CMD_HCRP_STATIC8);   m_acishrinkgroup.Add(IDC_CMD_HCRP_STATIC9);
	m_acishrinkgroup.Add(IDC_CMD_HCRP_ACI_AGE);   
	m_acishrinkgroup.Add(IDC_CMD_HCRP_ACI_CEME);  m_acishrinkgroup.Add(IDC_CMD_HCRP_ACI_AGE_UNIT);
	m_acishrinkgroup.Add(IDC_CMD_HCRP_ACI_CEME_UNIT);
	//-----------------------------------------------------------------------------
	// CEB - FIP(1990)
	//-----------------------------------------------------------------------------
	m_cebgroup.Add(IDC_CMD_CEB_FRAME);
	m_cebgroup.Add(IDC_CMD_HCRP_STATIC11);        m_cebgroup.Add(IDC_CMD_HCRP_STATIC12);
	m_cebgroup.Add(IDC_CMD_HCRP_STATIC13);        m_cebgroup.Add(IDC_CMD_HCRP_STATIC14);
	m_cebgroup.Add(IDC_CMD_HCRP_STATIC15);        m_cebgroup.Add(IDC_CMD_HCRP_STATIC16);
	m_cebgroup.Add(IDC_CMD_HCRP_CEB_COMP);        m_cebgroup.Add(IDC_CMD_HCRP_CEB_COMP_UNIT);
	m_cebgroup.Add(IDC_CMD_HCRP_CEB_RELA);        
	m_cebgroup.Add(IDC_CMD_HCRP_CEB_RELA_UNIT);   m_cebgroup.Add(IDC_CMD_HCRP_CEB_NOTA);
	m_cebgroup.Add(IDC_CMD_HCRP_CEB_NOTA_UNIT);   m_cebgroup.Add(IDC_CMD_HCRP_CEB_RSCHK);
	m_cebgroup.Add(IDC_CMD_HCRP_CEB_NRCHK);       m_cebgroup.Add(IDC_CMD_HCRP_CEB_SLCHK);
	m_cebgroup.Add(IDC_CMD_HCRP_CEB_AGE);         
	m_cebgroup.Add(IDC_CMD_HCRP_CEB_AGE_UNIT);    m_cebgroup.Add(IDC_CEB_FIP_CMB);
	m_cebgroup.Add(IDC_CMD_HCRP_CEB_RELA2);
	m_cebgroup.Add(IDC_CEB_FIP_AGGRE_CMB);
	m_cebgroup.Add(IDC_CMD_HCRP_STATIC17);

	m_cebshrinkgroup.Add(IDC_CMD_HCRP_STATIC16);  m_cebshrinkgroup.Add(IDC_CMD_HCRP_CEB_AGE);
	m_cebshrinkgroup.Add(IDC_CMD_HCRP_CEB_AGE_UNIT);

	//-----------------------------------------------------------------------------
	// Modify Modulus of Elasticity
	//-----------------------------------------------------------------------------
	m_memgroup.Add(IDC_CMD_MEM_FRAME);
	m_memgroup.Add(IDC_CMD_ELASTIC_STATIC1);      m_memgroup.Add(IDC_CMD_ELASTIC_STATIC2);
	m_memgroup.Add(IDC_CMD_ELASTIC_STATIC3);      m_memgroup.Add(IDC_CMD_ELASTIC_STATIC4);
	m_memgroup.Add(IDC_CMD_ELASTIC_STATIC5);      m_memgroup.Add(IDC_CMD_ELASTIC_STATIC6);
	m_memgroup.Add(IDC_CMD_ELASTIC_STATIC7);      m_memgroup.Add(IDC_CMD_MEM_N1);
	m_memgroup.Add(IDC_CMD_MEM_N1_UNIT);
	m_memgroup.Add(IDC_CMD_MEM_PHI1);             m_memgroup.Add(IDC_CMD_MEM_N2);
	m_memgroup.Add(IDC_CMD_MEM_N2_UNIT);
	m_memgroup.Add(IDC_CMD_MEM_PHI2);
	//-----------------------------------------------------------------------------
	// User Defined Group
	//-----------------------------------------------------------------------------
	m_udefgroup.Add(IDC_CMD_UDEF_FRAME        );
	m_udefgroup.Add(IDC_CMD_UDEF_CFUNC_STATIC );  m_udefgroup.Add(IDC_CMD_UDEF_STATIC001    );
	m_udefgroup.Add(IDC_CMD_UDEF_STATIC2      );  m_udefgroup.Add(IDC_CMD_UDEF_CFUNC_COMBO);
	m_udefgroup.Add(IDC_CMD_UDEF_CFUNC_BUTTON);   m_udefgroup.Add(IDC_CMD_UDEF_LAGE_EDIT);
	m_udefgroup.Add(IDC_CMD_UDEF_CFUNC_LIST);     m_udefgroup.Add(IDC_CMD_UDEF_ADD_CFUNC_BUTTON);
	m_udefgroup.Add(IDC_CMD_UDEF_DEL_CFUNC_BUTTON);m_udefgroup.Add(IDC_CMD_UDEF_MODIFY_CFUNC_BUTTON);
	m_udefgroup.Add(IDC_CMD_UDEF_SSTR_CHECK);     m_udefgroup.Add(IDC_CMD_UDEF_SSTR_COMBO);
	m_udefgroup.Add(IDC_CMD_UDEF_SSTR_BUTTON);    m_udefgroup.Add(IDC_CMD_DAY_STATIC);

	m_udefcrp.Add(IDC_CMD_UDEF_FRAME);            m_udefcrp.Add(IDC_CMD_UDEF_CFUNC_STATIC);
	m_udefcrp.Add(IDC_CMD_UDEF_STATIC001);        m_udefcrp.Add(IDC_CMD_UDEF_STATIC2);
	m_udefcrp.Add(IDC_CMD_UDEF_CFUNC_COMBO);      m_udefcrp.Add(IDC_CMD_UDEF_CFUNC_BUTTON); 
	m_udefcrp.Add(IDC_CMD_UDEF_LAGE_EDIT);        m_udefcrp.Add(IDC_CMD_UDEF_CFUNC_LIST);
	m_udefcrp.Add(IDC_CMD_UDEF_ADD_CFUNC_BUTTON); m_udefcrp.Add(IDC_CMD_UDEF_MODIFY_CFUNC_BUTTON);
	m_udefcrp.Add(IDC_CMD_UDEF_DEL_CFUNC_BUTTON); m_udefcrp.Add(IDC_CMD_DAY_STATIC);

	m_udefshr.Add(IDC_CMD_UDEF_FRAME);            m_udefshr.Add(IDC_CMD_UDEF_CFUNC_STATIC);
	m_udefshr.Add(IDC_CMD_UDEF_SSTR_CHECK);       m_udefshr.Add(IDC_CMD_UDEF_SSTR_COMBO);
	m_udefshr.Add(IDC_CMD_UDEF_SSTR_BUTTON);
	//-----------------------------------------------------------------------------
	// PCA Group
	//-----------------------------------------------------------------------------
	m_pcagroup.Add(IDC_CMD_PCA_FRAME);
	m_pcagroup.Add(IDC_CMD_PCA_STATIC1);          m_pcagroup.Add(IDC_CMD_PCA_CS28);
	m_pcagroup.Add(IDC_CMD_PCA_CS28_UNIT);        m_pcagroup.Add(IDC_CMD_PCA_STATIC2);
	m_pcagroup.Add(IDC_CMD_PCA_SRKG_STRN);        m_pcagroup.Add(IDC_CMD_PCA_SRKG_STRN_UNIT);
	m_pcagroup.Add(IDC_CMD_PCA_STATIC3);          m_pcagroup.Add(IDC_CMD_PCA_CREEP_STRN);
	m_pcagroup.Add(IDC_CMD_PCA_CREEP_STRN_UNIT);  m_pcagroup.Add(IDC_CMD_PCA_STATIC4);
	m_pcagroup.Add(IDC_CMD_PCA_RLTV_HUMID);       
	m_pcagroup.Add(IDC_CMD_PCA_RLTV_HUMID_UNIT);  m_pcagroup.Add(IDC_CMD_PCA_STATIC5);
	m_pcagroup.Add(IDC_CMD_PCA_VS_RATIO);         m_pcagroup.Add(IDC_CMD_PCA_VS_RATIO_UNIT);
	m_pcagroup.Add(IDC_CMD_PCA_STATIC6);          m_pcagroup.Add(IDC_CMD_PCA_RNFC_RATIO);
	m_pcagroup.Add(IDC_CMD_PCA_RNFC_RATIO_UNIT);
	m_pcagroup.Add(IDC_CMD_PCA_STATIC7);          m_pcagroup.Add(IDC_CMD_PCA_ELAST_MODUL);
	m_pcagroup.Add(IDC_CMD_PCA_ELAST_MODUL_UNIT); 
	
	//-----------------------------------------------------------------------------
	// Combined Group
	//-----------------------------------------------------------------------------
	m_combroup.Add(IDC_CMD_COMB_FRAME);
	m_combroup.Add(IDC_CMD_COMB_STATIC1);         m_combroup.Add(IDC_CMD_COMB_CS28);
	m_combroup.Add(IDC_CMD_COMB_CS28_UNIT);       m_combroup.Add(IDC_CMD_COMB_STATIC2);
	m_combroup.Add(IDC_CMD_COMB_RELT_HUMID);      
	m_combroup.Add(IDC_CMD_COMB_RELT_HUMID_UNIT); m_combroup.Add(IDC_CMD_COMB_STATIC3);
	m_combroup.Add(IDC_CMD_COMB_VOLM_SURF);       m_combroup.Add(IDC_CMD_COMB_VOLM_SURF_UNIT);
	m_combroup.Add(IDC_CMD_COMB_FRAME2);          m_combroup.Add(IDC_CMD_COMB_STATIC4);
	m_combroup.Add(IDC_CMD_COMB_CREEP_MATF);      m_combroup.Add(IDC_CMD_COMB_CREEP_MATF_UNIT);
	m_combroup.Add(IDC_CMD_COMB_CREEP_MATF_UNIT2);m_combroup.Add(IDC_CMD_COMB_STATIC5);
	m_combroup.Add(IDC_CMD_COMB_CREEP_VS_RD1);    m_combroup.Add(IDC_CMD_COMB_CREEP_VS_RD2);
	m_combroup.Add(IDC_CMD_COMB_STATIC6);         m_combroup.Add(IDC_CMD_COMB_CREEP_LDAF_RD1);
	m_combroup.Add(IDC_CMD_COMB_CREEP_LDAF_RD2);  m_combroup.Add(IDC_CMD_COMB_CREEP_LDAF_RD3);
	m_combroup.Add(IDC_CMD_COMB_STATIC7);         m_combroup.Add(IDC_CMD_COMB_FRAME3);
	m_combroup.Add(IDC_CMD_COMB_STATIC8);         m_combroup.Add(IDC_CMD_COMB_SRKG_MATF);
	m_combroup.Add(IDC_CMD_COMB_SRKG_MATF_UNIT);  m_combroup.Add(IDC_CMD_COMB_STATIC9);
	m_combroup.Add(IDC_CMD_COMB_SRKG_VS_RD1);     m_combroup.Add(IDC_CMD_COMB_SRKG_VS_RD2);
	m_combroup.Add(IDC_CMD_COMB_STATIC10);        m_combroup.Add(IDC_CMD_COMB_SRKG_PRGRS_RD1);
	m_combroup.Add(IDC_CMD_COMB_SRKG_PRGRS_RD3);  m_combroup.Add(IDC_RADIO9IDC_CMD_COMB_SRKG_PRGRS_RD2);
	m_combroup.Add(IDC_CMD_COMB_FRAME4);          m_combroup.Add(IDC_CMD_COMB_STATIC11);
	m_combroup.Add(IDC_CMD_COMB_EFFCT_RATIO);     
	m_combroup.Add(IDC_CMD_COMB_EFFCT_RATIO_UNIT);m_combroup.Add(IDC_CMD_COMB_STATIC12);
	m_combroup.Add(IDC_CMD_COMB_EFFCT_MODUL);     m_combroup.Add(IDC_CMD_COMB_EFFCT_MODUL_UNIT);
	m_combroup.Add(IDC_CMD_COMB_PCA_EFFECT_CHK);
	//-----------------------------------------------------------------------------
	// Japanese Group
	//-----------------------------------------------------------------------------
	m_japangroup.Add(IDC_CMD_JAPAN_FRAME);
	m_japangroup.Add(IDC_CMD_HCRP_JAPAN_STATIC_HEQUAL ); m_japangroup.Add(IDC_CMD_HCRP_JAPAN_STATIC_ENV );
	m_japangroup.Add(IDC_CMD_HCRP_JAPAN_STATIC_AC ); m_japangroup.Add(IDC_CMD_HCRP_JAPAN_STATIC_U );
	m_japangroup.Add(IDC_CMD_HCRP_JAPAN_STATIC1); m_japangroup.Add(IDC_CMD_HCRP_JAPAN_COMP);
	m_japangroup.Add(IDC_CMD_HCRP_JAPAN_COMP_UNIT);m_japangroup.Add(IDC_CMD_HCRP_JAPAN_STATIC2);
	m_japangroup.Add(IDC_CMD_HCRP_JAPAN_REL_RD1); m_japangroup.Add(IDC_CMD_HCRP_JAPAN_REL_RD2);
	m_japangroup.Add(IDC_CMD_HCRP_JAPAN_RELA);    
	m_japangroup.Add(IDC_CMD_HCRP_JAPAN_STATIC3); m_japangroup.Add(IDC_CMD_HCRP_JAPAN_RELA_UNIT);
	m_japangroup.Add(IDC_CMD_HCRP_JAPAN_NOTA);    m_japangroup.Add(IDC_CMD_HCRP_JAPAN_NOTA_UNIT);
	m_japangroup.Add(IDC_CMD_HCRP_JAPAN_STATIC4); m_japangroup.Add(IDC_CMD_HCRP_JAPAN_STATIC5);
	m_japangroup.Add(IDC_CMD_HCRP_JAPAN_RAPID);   m_japangroup.Add(IDC_CMD_HCRP_JAPAN_NORMAL);
	m_japangroup.Add(IDC_CMD_HCRP_JAPAN_STATIC6); m_japangroup.Add(IDC_CMD_HCRP_JAPAN_AGE);
	m_japangroup.Add(IDC_CMD_HCRP_JAPAN_AGE_UNIT);
	//-----------------------------------------------------------------------------
	// chinese Group - 2002. 7. 9
	//-----------------------------------------------------------------------------
	m_chinagroup.Add(IDC_CMD_CHINA_FRAME);
	m_chinagroup.Add(IDC_CMD_HCRP_CHINA_STATIC1); m_chinagroup.Add(IDC_CMD_HCRP_CHINA_COMP);
	m_chinagroup.Add(IDC_CMD_HCRP_CHINA_STATIC2); m_chinagroup.Add(IDC_CMD_HCRP_CHINA_COMP_UNIT);
	m_chinagroup.Add(IDC_CMD_HCRP_CHINA_REL_RD1); m_chinagroup.Add(IDC_CMD_HCRP_CHINA_REL_RD2);
	m_chinagroup.Add(IDC_CMD_HCRP_CHINA_RELA);    
	m_chinagroup.Add(IDC_CMD_HCRP_CHINA_STATIC3); m_chinagroup.Add(IDC_CMD_HCRP_CHINA_RELA_UNIT);
	m_chinagroup.Add(IDC_CMD_HCRP_CHINA_NOTA);    m_chinagroup.Add(IDC_CMD_HCRP_CHINA_NOTA_UNIT);
	m_chinagroup.Add(IDC_CMD_HCRP_CHINA_STATIC4); m_chinagroup.Add(IDC_CMD_HCRP_CHINA_STATIC5);
	m_chinagroup.Add(IDC_CMD_HCRP_CHINA_AGE);     
	m_chinagroup.Add(IDC_CMD_HCRP_CHINA_AGE_UNIT);
	m_chinagroup.Add(IDC_CMD_HCRP_CHINA_STATIC6);
	//-----------------------------------------------------------------------------
	// ETC
	//-----------------------------------------------------------------------------
	m_aciacigroup.Add(IDC_CMD_HCRP_STATIC5);      m_aciacigroup.Add(IDC_CMD_HCRP_ACI_SLUM);  
	m_aciacigroup.Add(IDC_CMD_HCRP_ACI_SLUM_UNIT);m_aciacigroup.Add(IDC_CMD_HCRP_STATIC6);  
	m_aciacigroup.Add(IDC_CMD_HCRP_ACI_FINE);     
	m_aciacigroup.Add(IDC_CMD_HCRP_ACI_FINE_UNIT);m_aciacigroup.Add(IDC_CMD_HCRP_STATIC7);  
	m_aciacigroup.Add(IDC_CMD_HCRP_ACI_AIR);      
	m_aciacigroup.Add(IDC_CMD_HCRP_ACI_AIR_UNIT); m_aciacigroup.Add(IDC_CMD_HCRP_STATIC9);  
	m_aciacigroup.Add(IDC_CMD_HCRP_ACI_CEME);     m_aciacigroup.Add(IDC_CMD_HCRP_ACI_CEME_UNIT);  
 
	m_aciUsergroup.Add(IDC_CMD_HCRP_STATIC101);   m_aciUsergroup.Add(IDC_CMD_ACI_CREEP_COEF);
	m_aciUsergroup.Add(IDC_CMD_HCRP_STATIC102);   m_aciUsergroup.Add(IDC_CMD_ACI_SRKG_STRN);
	m_aciUsergroup.Add(IDC_CMD_ACI_SRKG_STRN_UNIT);

	m_buttongroup.Add(IDC_CMD_HCRP_REMOVE_BTN);   m_buttongroup.Add(IDC_CMD_ETC_HCRP_SHOW);
	m_buttongroup.Add(IDOK);                      m_buttongroup.Add(IDCANCEL);
	m_buttongroup.Add(IDC_CMD_APPLY);
	//-----------------------------------------------------------------------------
	// JSCE // SKN
	//-----------------------------------------------------------------------------
	m_jscegroup.Add(IDC_CMD_JSCE_FRM);                 m_jscegroup.Add(IDC_CMD_JSCE_STATIC1);
	m_jscegroup.Add(IDC_CMD_JSCE_RH_EDT);              
	m_jscegroup.Add(IDC_CMD_JSCE_STATIC6);             m_jscegroup.Add(IDC_CMD_JSCE_STATIC2);
	m_jscegroup.Add(IDC_CMD_JSCE_VS_EDT);              m_jscegroup.Add(IDC_CMD_VS_UNIT);//m_jscegroup.Add(IDC_CMD_MM);
	m_jscegroup.Add(IDC_CMD_JSCE_STATIC3);             m_jscegroup.Add(IDC_CMD_JSCE_CC_EDT);
	m_jscegroup.Add(IDC_CMD_CC_UNIT);                  m_jscegroup.Add(IDC_CMD_JSCE_STATIC4);
	m_jscegroup.Add(IDC_CMD_JSCE_WC_EDT);              m_jscegroup.Add(IDC_CMD_WC_UNIT);
	m_jscegroup.Add(IDC_CMD_JSCE_STATIC5);             m_jscegroup.Add(IDC_CMD_JSCE_AC_EDT);
	m_jscegroup.Add(IDC_CMD_JSCE_STATIC_7);

	m_ChJTG04Group.Add(IDC_CMD_CHJTG04_FRM);
	//-----------------------------------------------------------------------------
	// INDIA(IRC:18-2000)
	//-----------------------------------------------------------------------------
}
/*
IDC_CMD_HCRP_JAPAN_STATIC1
IDC_CMD_HCRP_JAPAN_COMP
IDC_CMD_HCRP_JAPAN_STATIC2
IDC_CMD_HCRP_JAPAN_REL_RD1
IDC_CMD_HCRP_JAPAN_REL_RD2
IDC_CMD_HCRP_JAPAN_RELA
IDC_CMD_HCRP_JAPAN_RELA_SPIN
IDC_CMD_HCRP_JAPAN_RELA_UNIT
IDC_CMD_HCRP_JAPAN_STATIC3
IDC_CMD_HCRP_JAPAN_STATIC_HEQUAL
IDC_CMD_HCRP_JAPAN_STATIC_ENV
IDC_CMD_HCRP_JAPAN_STATIC4
IDC_CMD_HCRP_JAPAN_NOTA
IDC_CMD_HCRP_JAPAN_NOTA_UNIT
IDC_CMD_HCRP_JAPAN_STATIC_AC
IDC_CMD_HCRP_JAPAN_STATIC_U
IDC_CMD_HCRP_JAPAN_STATIC5
IDC_CMD_HCRP_JAPAN_RAPID
IDC_CMD_HCRP_JAPAN_NORMAL
IDC_CMD_HCRP_JAPAN_STATIC6
IDC_CMD_HCRP_JAPAN_AGE
IDC_CMD_HCRP_JAPAN_AGE_SPIN
IDC_CMD_HCRP_JAPAN_AGE_UNIT
*/
void CCMHcrpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMHcrpDlg)
	DDX_Control(pDX, IDC_CMD_JSCE_AC_EDT, m_jsceAc);
	DDX_Control(pDX, IDC_CMD_JSCE_WC_EDT, m_jsceWc);
	DDX_Control(pDX, IDC_CMD_JSCE_CC_EDT, m_jsceCc);
	DDX_Control(pDX, IDC_CMD_JSCE_VS_EDT, m_jsceVs);
	DDX_Control(pDX, IDC_CMD_JSCE_RH_EDT, m_jsceRh);  
	DDX_Control(pDX, IDC_CMD_CC_UNIT, m_jsceCcUnit);
	DDX_Control(pDX, IDC_CMD_WC_UNIT, m_jsceWcUnit);
	DDX_Control(pDX, IDC_CMD_VS_UNIT, m_jsceVsUnit);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_RELA_UNIT, m_chinaRelaunit);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_RELA, m_chinaRela);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_NOTA_UNIT, m_chinaNotaunit);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_NOTA, m_chinaNota);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_COMP_UNIT, m_chinaCompunit);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_COMP, m_chinaComp);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_AGE_UNIT, m_chinaAgeunit);
	DDX_Control(pDX, IDC_CMD_HCRP_CHINA_AGE, m_chinaAge);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_RELA, m_jpanrela);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_NOTA_UNIT, m_jpannotaunit);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_NOTA, m_jpannota);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_COMP_UNIT, m_jpancompunit);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_COMP, m_jpancomp);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_AGE_UNIT, m_jpanageunit);
	DDX_Control(pDX, IDC_CMD_HCRP_JAPAN_AGE, m_jpanage);
	DDX_Control(pDX, IDC_CMD_COMB_CREEP_MATF_UNIT, m_combmatFacUT);
	DDX_Control(pDX, IDC_CMD_COMB_VOLM_SURF_UNIT, m_combVSUT);
	DDX_Control(pDX, IDC_CMD_COMB_EFFCT_MODUL_UNIT, m_combmodulUT);
	DDX_Control(pDX, IDC_CMD_COMB_CS28_UNIT, m_combfc28UT);
	DDX_Control(pDX, IDC_CMD_PCA_VS_RATIO_UNIT, m_pcavsratioUT);
	DDX_Control(pDX, IDC_CMD_PCA_ELAST_MODUL_UNIT, m_pcamodulUT);
	DDX_Control(pDX, IDC_CMD_PCA_CS28_UNIT, m_pcafc28UT);
	DDX_Control(pDX, IDC_CMD_COMB_VOLM_SURF, m_combVS);
	DDX_Control(pDX, IDC_CMD_COMB_SRKG_MATF, m_combsmatFac);
	DDX_Control(pDX, IDC_CMD_PCA_SRKG_STRN, m_pcasrkgstrn);
	DDX_Control(pDX, IDC_CMD_COMB_RELT_HUMID, m_combhumid);
	DDX_Control(pDX, IDC_CMD_COMB_EFFCT_RATIO, m_combratio);
	DDX_Control(pDX, IDC_CMD_COMB_EFFCT_MODUL, m_combmodul);
	DDX_Control(pDX, IDC_CMD_COMB_CS28, m_combfc28);
	DDX_Control(pDX, IDC_CMD_COMB_CREEP_MATF, m_combmatFac);
	DDX_Control(pDX, IDC_CMD_PCA_VS_RATIO, m_pcavsratio);
	DDX_Control(pDX, IDC_CMD_PCA_RNFC_RATIO, m_pcareinratio);
	DDX_Control(pDX, IDC_CMD_PCA_RLTV_HUMID, m_pcahumid);
	DDX_Control(pDX, IDC_CMD_PCA_ELAST_MODUL, m_pcamodul);
	DDX_Control(pDX, IDC_CMD_PCA_CS28, m_pcafc28);
	DDX_Control(pDX, IDC_CMD_PCA_CREEP_STRN, m_pcacrpstrn);
	DDX_Control(pDX, IDC_CMD_ACI_SRKG_STRN, m_acisrkgstrn);
	DDX_Control(pDX, IDC_CMD_ACI_CREEP_COEF, m_acicrpcoef);
	DDX_Control(pDX, IDC_CMD_CEB_FRAME, m_frmCEB);
	DDX_Control(pDX, IDC_CMD_UDEF_LAGE_EDIT, m_wndLoadAge);
	DDX_Control(pDX, IDC_CMD_UDEF_CFUNC_COMBO, m_CobxCreep);
	DDX_Control(pDX, IDC_CMD_UDEF_SSTR_COMBO, m_CobxSstr);
	DDX_Control(pDX, IDC_CMD_UDEF_CFUNC_LIST, m_wndUdefFList);
	DDX_Control(pDX, IDC_CMD_MEM_PHI2, m_memPhi2);
	DDX_Control(pDX, IDC_CMD_MEM_PHI1, m_memPhi1);
	DDX_Control(pDX, IDC_CMD_HCRP_CEB_RELA, m_cebrela);
	DDX_Control(pDX, IDC_CMD_HCRP_CEB_NOTA_UNIT, m_notaunit);
	DDX_Control(pDX, IDC_CMD_HCRP_CEB_NOTA, m_nota);
	DDX_Control(pDX, IDC_CMD_HCRP_CEB_COMP_UNIT, m_cebcompunit);
	DDX_Control(pDX, IDC_CMD_HCRP_CEB_COMP, m_cebcomp);
	DDX_Control(pDX, IDC_CMD_HCRP_CEB_AGE_UNIT, m_cebageunit);
	DDX_Control(pDX, IDC_CMD_HCRP_ACI_VOLU_UNIT, m_voluunit);
	DDX_Control(pDX, IDC_CMD_HCRP_ACI_VOLU, m_volu);
	DDX_Control(pDX, IDC_CMD_HCRP_ACI_SLUM_UNIT, m_slumunit);
	DDX_Control(pDX, IDC_CMD_HCRP_ACI_SLUM, m_slum);
	DDX_Control(pDX, IDC_CMD_HCRP_ACI_RELA, m_acirela);
	DDX_Control(pDX, IDC_CMD_HCRP_ACI_COMP_UNIT, m_acicompunit);
	DDX_Control(pDX, IDC_CMD_HCRP_ACI_COMP, m_acicomp);
	DDX_Control(pDX, IDC_CMD_HCRP_ACI_AIR, m_air);
	DDX_Control(pDX, IDC_CMD_HCRP_ACI_FINE, m_fine);
	DDX_Control(pDX, IDC_CMD_HCRP_ACI_CEME_UNIT, m_cemeunit);
	DDX_Control(pDX, IDC_CMD_HCRP_ACI_CEME, m_ceme);
	DDX_Control(pDX, IDC_CMD_HCRP_ACI_AGE_UNIT, m_aciageunit);
	DDX_Control(pDX, IDC_HCRP_CODE_COMBO, m_CobxCode);
	DDX_Control(pDX, IDC_CMD_HCRP_CEB_RELA2, m_cebrela2);

	// MNET:2406-JHLEE-20070822
	DDX_Control(pDX, IDC_CEB_FIP_CMB, m_cmbCebRela);
	DDX_Control(pDX, IDC_CMD_HCRP_STATIC12 , m_stcRHEnv);
	DDX_Control(pDX, IDC_CEB_FIP_AGGRE_CMB , m_cmbAggre);
	////////////////////////////////////////////////////////////

	DDX_Control(pDX, IDC_CMD_HCRP_ACI_AGE, m_aciage);
	DDX_Radio(pDX, IDC_CMD_HCRP_ACI_MOIST_CHK, m_curemethod);
	DDX_Control(pDX, IDC_CMD_HCRP_CEB_AGE, m_cebage);
	DDX_Radio(pDX, IDC_CMD_HCRP_CEB_RSCHK, m_cementType);
	DDX_Control(pDX, IDC_CMD_MEM_N1, m_memN1);
	DDX_Control(pDX, IDC_CMD_MEM_N2, m_memN2);
	DDX_Check(pDX, IDC_CMD_UDEF_SSTR_CHECK, m_bSSTR);		
	DDX_Text(pDX, IDC_CMD_TDEPNAME_EDIT, m_strTDefMatName);
	DDX_Radio(pDX, IDC_CMD_COMB_CREEP_LDAF_RD1, m_combLFac);
	DDX_Radio(pDX, IDC_CMD_COMB_CREEP_VS_RD1, m_combvsratio);
	DDX_Radio(pDX, IDC_CMD_COMB_SRKG_PRGRS_RD1, m_combprog);
	DDX_Radio(pDX, IDC_CMD_COMB_SRKG_VS_RD1, m_combsvsratio);
	DDX_Radio(pDX, IDC_CMD_ACI_ACI_RD, m_ACIorUser);
	DDX_Check(pDX, IDC_CMD_COMB_PCA_EFFECT_CHK, m_bPCAEffect);
	DDX_Radio(pDX, IDC_CMD_HCRP_JAPAN_RAPID, m_jpancemen);
	DDX_Radio(pDX, IDC_CMD_HCRP_JAPAN_REL_RD1, m_jpanhumid);
	DDX_Radio(pDX, IDC_CMD_HCRP_CHINA_REL_RD1, m_chinaRelradio);
	DDX_Control(pDX, IDC_CMD_HCRP_ACI_CONC_A, m_concA);
	DDX_Control(pDX, IDC_CMD_HCRP_ACI_CONC_B, m_concB);

	//}}AFX_DATA_MAP
	if (m_pJapanDlg)    m_pJapanDlg->UpdateData(pDX->m_bSaveAndValidate);
	if (m_pChJTG04Dlg)  m_pChJTG04Dlg->UpdateData(pDX->m_bSaveAndValidate);
	if (m_pAASHTODlg)   m_pAASHTODlg->UpdateData(pDX->m_bSaveAndValidate);
	if (m_pEuropeanDlg) m_pEuropeanDlg->UpdateData(pDX->m_bSaveAndValidate);
	if (m_pIndiaDlg)    m_pIndiaDlg->UpdateData(pDX->m_bSaveAndValidate);
	if (m_pIndia2011Dlg)m_pIndiaDlg->UpdateData(pDX->m_bSaveAndValidate);
	if (m_pIndia2011_2020Dlg)m_pIndiaDlg->UpdateData(pDX->m_bSaveAndValidate);
	if (m_pASDlg)       m_pASDlg->UpdateData(pDX->m_bSaveAndValidate);
	if (m_pRusDlg)      m_pRusDlg->UpdateData(pDX->m_bSaveAndValidate);
	if (m_pChJTG16Dlg)  m_pChJTG16Dlg->UpdateData(pDX->m_bSaveAndValidate);
	if (m_pNZDlg)       m_pNZDlg->UpdateData(pDX->m_bSaveAndValidate);
	if (m_pKDS2016Dlg)  m_pKDS2016Dlg->UpdateData(pDX->m_bSaveAndValidate);
	if (m_pJSCE07Dlg)   m_pJSCE07Dlg->UpdateData(pDX->m_bSaveAndValidate);
	if (m_pJSCE12Dlg)   m_pJSCE12Dlg->UpdateData(pDX->m_bSaveAndValidate);
	if (m_pABNT_NBR_6118Dlg)   m_pABNT_NBR_6118Dlg->UpdateData(pDX->m_bSaveAndValidate);
	//DDX_Radio(pDX, IDC_HCRP_CREEP_CHK, m_type);
}


BEGIN_MESSAGE_MAP(CCMHcrpDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMHcrpDlg)
	ON_CBN_SELCHANGE(IDC_HCRP_CODE_COMBO, OnSelchangeHcrpCodeCombo)
	ON_BN_CLICKED(IDC_CMD_HCRP_REMOVE_BTN, OnCmdHcrpRemoveBtn)	
	ON_BN_CLICKED(IDC_CMD_UDEF_SSTR_CHECK, OnSSTR)		
	ON_BN_CLICKED(IDC_CMD_UDEF_CFUNC_BUTTON, OnCreepFunctionButton)		
	ON_BN_CLICKED(IDC_CMD_UDEF_SSTR_BUTTON, OnSSTRFuncButton)	
	ON_BN_CLICKED(IDC_CMD_UDEF_ADD_CFUNC_BUTTON, OnCmdUdefAddCfuncButton)
	ON_BN_CLICKED(IDC_CMD_UDEF_DEL_CFUNC_BUTTON, OnCmdUdefDelCfuncButton)
	ON_BN_CLICKED(IDC_CMD_UDEF_MODIFY_CFUNC_BUTTON, OnCmdUdefModifyCfuncButton)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_HCRP_ACI_RELA, OnDeltaposACI_RELA)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_HCRP_ACI_FINE, OnDeltaposACI_FINE)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_HCRP_ACI_AIR, OnDeltaposACI_AIR)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_HCRP_ACI_AGE, OnDeltaposACI_AGE)	
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_HCRP_CEB_AGE, OnDeltaposCEB_AGE)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_HCRP_CEB_RELA, OnDeltaposCEB_RELA)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_MEM_N1, OnDeltaposEMOD_N1)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_MEM_N2, OnDeltaposEMOD_N2)
	ON_BN_CLICKED(IDC_CMD_ETC_HCRP_SHOW, OnCmdEtcHcrpShow)
	ON_BN_CLICKED(IDC_CMD_ACI_ACI_RD, OnCmdAciAciRd)
	ON_BN_CLICKED(IDC_CMD_ACI_USER_RD, OnCmdAciUserRd)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_COMB_EFFCT_RATIO, OnDeltaposCmdCombEffctRatioSpin)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_COMB_RELT_HUMID, OnDeltaposCmdCombReltHumidSpin)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_PCA_RLTV_HUMID, OnDeltaposCmdPcaRltvHumidSpin)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_PCA_RNFC_RATIO, OnDeltaposCmdPcaRnfcRatioSpin)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_PCA_SRKG_STRN, OnDeltaposCmdPcaSrkgStrnSpin)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_PCA_CREEP_STRN, OnDeltaposCmdPcaCreepStrnSpin)
	ON_BN_CLICKED(IDC_CMD_COMB_PCA_EFFECT_CHK, OnCmdCombPcaEffectChk)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_HCRP_JAPAN_RELA, OnDeltaposCmdHcrpJapanRelaSpin)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_HCRP_JAPAN_AGE, OnDeltaposCmdHcrpJapanAgeSpin)
	ON_BN_CLICKED(IDC_CMD_HCRP_JAPAN_REL_RD1, OnCmdHcrpJapanRelRd1)
	ON_BN_CLICKED(IDC_CMD_HCRP_JAPAN_REL_RD2, OnCmdHcrpJapanRelRd2)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_HCRP_CHINA_RELA, OnDeltaposCmdHcrpChinaRelaSpin)
// 	ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_HCRP_CHINA_AGE, OnDeltaposCmdHcrpChinaAgeSpin)
	ON_BN_CLICKED(IDC_CMD_HCRP_CHINA_REL_RD1, OnCmdHcrpChinaRelRd)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_JSCE_RH_SPIN1, OnDeltaposCmdJsceRhSpin1)
	ON_BN_CLICKED(IDC_CMD_HCRP_CHINA_REL_RD2, OnCmdHcrpChinaRelRd)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_JSCE_AC_SPIN2, OnDeltaposCmdJsceAcSpin2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
	
/////////////////////////////////////////////////////////////////////////////
// CCMHcrpDlg message handlers
//

/////////////////////////////////////////////////////////////////////////////
//
// �ܺ� ���� �Լ�
//
void CCMHcrpDlg::SetTimeDepMatMode(BOOL bIsTimeDepMatMode, int nOPMode)
{
	m_bIsTimeDependentMatMode = bIsTimeDepMatMode;
	m_nOptionForTimeDep = nOPMode;
}

BOOL CCMHcrpDlg::SetCurTDMTData(T_TDMT_D * pTdmt)
{
	if(m_nOptionForTimeDep == 1) // Be Only Modify Mode 
	{
		 m_data = *pTdmt;
		 m_strOldMatName = pTdmt->strMatlName;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
// Initialize
//
void CCMHcrpDlg::ModeCtrlMan()
{
	m_CobxCode.ResetContent();

#if defined(_US)
	{
		CDlgUtil::CobxAddItem(m_CobxCode, _T("CEB-FIP(2010)"), 27);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("CEB-FIP(1990)"),  2);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("CEB-FIP(1978)"), 13);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("ACI"),            1);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("PCA"),            6);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Combined__ACI___PCA_), 7);
//    CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japanese_Standard), 8);
//    CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Korean_Standard), 5);
		#if defined(_CIVIL)
		CDlgUtil::CobxAddItem(m_CobxCode, _T("AASHTO"), 12);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japane_JSCE12)       , 34); 
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japane_JSCE07)       , 16); 
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japanese_Standard)   ,  8);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japane_JSCE)         , 10); // SKN
		#endif
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__INDIA_STANDARD)         , 14);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__EUROPEAN)               , 15);

		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_5100_2017_AMD), TDMT_CODE_AUSTRALIA_2017_AMD_2024);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2017), TDMT_CODE_AUSTRALIA_2017);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2016), TDMT_CODE_AUSTRALIA_2016);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2011), TDMT_CODE_AUSTRALIA_2011);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_3600_2018_AMD), TDMT_CODE_AUSTRALIA_2018_AMD_2021);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2009), TDMT_CODE_AUSTRALIA_2009);		
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__NEWZEALAND_2022), TDMT_CODE_NEWZEALAND_2022);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__NEWZEALAND),      TDMT_CODE_NEWZEALAND);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2020)"), 35);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2011)")                         , 25);       // wg_mkor�� ���ڿ� ������ ��!!
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__RUSSIAN), TDMT_CODE_RUSSIAN);
#if defined(_CIVIL)
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Chinese_Standard)    ,  9);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD_TDMT_CHJTG04)             , TDMT_CODE_CHINA_JTG);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD_TDMT_CHJTG3362_18)        , TDMT_CODE_CHINA_JTG_16);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD_TDMT_CHJTG_T_D65_2015)    , TDMT_CODE_CHINA_CHJTG_T_D65_2015);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("KDS-2016"), 33);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("KCI-USD12"), 21);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("KSCE 2010"), 20);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Korean_Standard), 5);
#else
		int nNationa = CDBLib::GetLockCode();
		if (IsOnBitCode(nNationa, AllowNationCode::_EUS_KR))
		{
			CDlgUtil::CobxAddItem(m_CobxCode, _T("KDS-2016"), 33);
			CDlgUtil::CobxAddItem(m_CobxCode, _T("KCI-USD12"), 21);
			CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Korean_Standard), 5);
		}
		if (IsOnBitCode(nNationa, AllowNationCode::_EUS_JP))
		{
			CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japane_JSCE12), 34);
			CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japane_JSCE07), 16);
			CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japanese_Standard), 8);
			CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japane_JSCE), 10); // SKN, Ȯ���� ����  // ���� ����..
		}
		if (IsOnBitCode(nNationa, AllowNationCode::_EUS_CH))
		{
			CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Chinese_Standard), 9);
			CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD_TDMT_CHJTG04)     , TDMT_CODE_CHINA_JTG);
			CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD_TDMT_CHJTG3362_18), TDMT_CODE_CHINA_JTG_16);
			CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD_TDMT_CHJTG_T_D65_2015), TDMT_CODE_CHINA_CHJTG_T_D65_2015);
		}
#endif
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__ABNTNBR61182023), TDMT_CODE_ABNT_NBR_6118_2023);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__User_Defined)                ,  4);

//  CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Modify_Elasticity_Modulus), 3);
	}
#elif defined(_RUS)
	{
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__RUSSIAN), TDMT_CODE_RUSSIAN);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("CEB-FIP(2010)"), 27);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("CEB-FIP(1990)"),  2);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("CEB-FIP(1978)"), 13);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("ACI"),            1);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("PCA"),            6);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Combined__ACI___PCA_), 7);
		//    CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japanese_Standard), 8);
		//    CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Korean_Standard), 5);
#if defined(_CIVIL)
		CDlgUtil::CobxAddItem(m_CobxCode, _T("AASHTO"), 12);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japane_JSCE12)       , 34); 
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japane_JSCE07)       , 16); 
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japanese_Standard)   ,  8);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japane_JSCE)         , 10); // SKN
#endif
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__INDIA_STANDARD)         , 14);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__EUROPEAN)               , 15);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_5100_2017_AMD), TDMT_CODE_AUSTRALIA_2017_AMD_2024);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2017), TDMT_CODE_AUSTRALIA_2017);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2016), TDMT_CODE_AUSTRALIA_2016);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2011), TDMT_CODE_AUSTRALIA_2011);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_3600_2018_AMD), TDMT_CODE_AUSTRALIA_2018_AMD_2021);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2009), TDMT_CODE_AUSTRALIA_2009);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__NEWZEALAND_2022), TDMT_CODE_NEWZEALAND_2022);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__NEWZEALAND),      TDMT_CODE_NEWZEALAND);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2020)"), 35);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2011)")                         , 25);       // wg_mkor�� ���ڿ� ������ ��!!
#if defined(_CIVIL)
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Chinese_Standard)    ,  9);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD_TDMT_CHJTG04)             , TDMT_CODE_CHINA_JTG);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD_TDMT_CHJTG3362_18)        , TDMT_CODE_CHINA_JTG_16);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD_TDMT_CHJTG_T_D65_2015)    , TDMT_CODE_CHINA_CHJTG_T_D65_2015);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("KDS-2016"), 33);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("KCI-USD12"), 21);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("KSCE 2010"), 20);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Korean_Standard), 5);
#endif
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__ABNTNBR61182023), TDMT_CODE_ABNT_NBR_6118_2023);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__User_Defined)                ,  4);
		
		//  CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Modify_Elasticity_Modulus), 3);
	}
#elif defined(_JP)
	{
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japane_JSCE12)       , 34); 
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japane_JSCE07)       , 16); 
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japanese_Standard)   ,  8);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japane_JSCE)         , 10); // SKN
		CDlgUtil::CobxAddItem(m_CobxCode, _T("CEB-FIP(2010)")                       , 27);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("CEB-FIP(1990)")                       ,  2);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("CEB-FIP(1978)")                       , 13);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("ACI")                                 ,  1);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("PCA")                                 ,  6);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Combined__ACI___PCA_),  7);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Korean_Standard)     ,  5);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__INDIA_STANDARD) , 14);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__EUROPEAN)       , 15);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_5100_2017_AMD), TDMT_CODE_AUSTRALIA_2017_AMD_2024);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2017), TDMT_CODE_AUSTRALIA_2017);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2016), TDMT_CODE_AUSTRALIA_2016);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2011), TDMT_CODE_AUSTRALIA_2011);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_3600_2018_AMD), TDMT_CODE_AUSTRALIA_2018_AMD_2021);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2009), TDMT_CODE_AUSTRALIA_2009);		
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__NEWZEALAND_2022), TDMT_CODE_NEWZEALAND_2022);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__NEWZEALAND),      TDMT_CODE_NEWZEALAND);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2020)"), 35);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2011)")                         , 25);       // wg_mkor�� ���ڿ� ������ ��!!
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__RUSSIAN), TDMT_CODE_RUSSIAN);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__ABNTNBR61182023), TDMT_CODE_ABNT_NBR_6118_2023);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__User_Defined)                ,  4);  
// CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Modify_Elasticity_Modulus), 3);    
		
		// �Ϻ� �������� �߰�
	}
#elif defined(_CH)
	{
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD_TDMT_CHJTG_T_D65_2015)    , TDMT_CODE_CHINA_CHJTG_T_D65_2015);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD_TDMT_CHJTG3362_18)        , TDMT_CODE_CHINA_JTG_16);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD_TDMT_CHJTG04)             , TDMT_CODE_CHINA_JTG);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Chinese_Standard)    ,  9);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("CEB-FIP(2010)")                       , 27);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("CEB-FIP(1990)")                       ,  2);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("CEB-FIP(1978)")                       , 13);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("ACI")                                 ,  1);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("PCA")                                 ,  6);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Combined__ACI___PCA_),  7);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japanese_Standard)   ,  8);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Korean_Standard)     ,  5);    
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__INDIA_STANDARD) , 14);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__EUROPEAN)       , 15);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_5100_2017_AMD), TDMT_CODE_AUSTRALIA_2017_AMD_2024);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2017), TDMT_CODE_AUSTRALIA_2017);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2016), TDMT_CODE_AUSTRALIA_2016);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2011), TDMT_CODE_AUSTRALIA_2011);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_3600_2018_AMD), TDMT_CODE_AUSTRALIA_2018_AMD_2021);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2009), TDMT_CODE_AUSTRALIA_2009);		
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__NEWZEALAND_2022), TDMT_CODE_NEWZEALAND_2022);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__NEWZEALAND),      TDMT_CODE_NEWZEALAND);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2020)"), 35);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2011)")                         , 25);       // wg_mkor�� ���ڿ� ������ ��!!
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__RUSSIAN), TDMT_CODE_RUSSIAN);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__ABNTNBR61182023), TDMT_CODE_ABNT_NBR_6118_2023);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__User_Defined)                ,  4);    
//  CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Modify_Elasticity_Modulus), 3);
	}
#else
	{
		CDlgUtil::CobxAddItem(m_CobxCode, _T("KDS-2016"), 33);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("KCI-USD12"), 21);
		#if defined(_CIVIL)
		CDlgUtil::CobxAddItem(m_CobxCode, _T("KSCE 2010"), 20);
		#endif
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Korean_Standard), 5);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("CEB-FIP(2010)"), 27);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("CEB-FIP(1990)"),  2);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("CEB-FIP(1978)"), 13);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("ACI"), 1);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("PCA"), 6);  
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Combined__ACI___PCA_), 7);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japane_JSCE12)    , 34); 
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japane_JSCE07)    , 16); 
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japanese_Standard), 8);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Japane_JSCE), 10); // SKN, Ȯ���� ����  // ���� ����..
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Chinese_Standard), 9);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD_TDMT_CHJTG04)     , TDMT_CODE_CHINA_JTG);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD_TDMT_CHJTG3362_18), TDMT_CODE_CHINA_JTG_16);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD_TDMT_CHJTG_T_D65_2015), TDMT_CODE_CHINA_CHJTG_T_D65_2015);
#if defined(_CIVIL)
		CDlgUtil::CobxAddItem(m_CobxCode, _T("AASHTO"), 12);
#endif
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__INDIA_STANDARD), 14);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__EUROPEAN), 15);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_5100_2017_AMD), TDMT_CODE_AUSTRALIA_2017_AMD_2024);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2017), TDMT_CODE_AUSTRALIA_2017);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2016), TDMT_CODE_AUSTRALIA_2016);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2011), TDMT_CODE_AUSTRALIA_2011);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_3600_2018_AMD), TDMT_CODE_AUSTRALIA_2018_AMD_2021);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__AUSTRALIA_STANDARD_2009), TDMT_CODE_AUSTRALIA_2009);		
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__NEWZEALAND_2022), TDMT_CODE_NEWZEALAND_2022);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__NEWZEALAND),      TDMT_CODE_NEWZEALAND);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2020)"), 35);
		CDlgUtil::CobxAddItem(m_CobxCode, _T("INDIA(IRC:112-2011)")                         , 25);       // wg_mkor�� ���ڿ� ������ ��!!
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__RUSSIAN), TDMT_CODE_RUSSIAN);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_WG_CMD__ADDD__ABNTNBR61182023), TDMT_CODE_ABNT_NBR_6118_2023);
		CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__User_Defined), 4);
//  CDlgUtil::CobxAddItem(m_CobxCode, _LS(IDS_CMD0417__Modify_Elasticity_Modulus), 3);
	}
#endif

	SetComboWidth(m_CobxCode);
}

// �޺��ڽ� ���� ���� ���̰� ��.
void CCMHcrpDlg::SetComboWidth(CComboBox& ComboBox)
{
	CString str;
	CSize   sz;
	int     dx = 0;
	CDC* pDC = ComboBox.GetDC();
	for (int i = 0; i < ComboBox.GetCount(); i++)
	{
		ComboBox.GetLBText(i, str);
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx)
			dx = sz.cx;
	}
// 	dx += globalUtils.ScaleByDPI(6); // left/right offset
// 	dx += globalUtils.GetSystemMetricsForDPI(this, SM_CXVSCROLL);
	ComboBox.SetDroppedWidth(dx);
	ComboBox.ReleaseDC(pDC);
}

void CCMHcrpDlg::AlignControls()
{
	int nDistY;
	CRect rRef, rToMove;

	// 1st point 
	GetDlgItem(IDC_CMD_ACI_FRAME)->GetWindowRect(rRef);

	// CEB1990, KS, CEB1978
	GetDlgItem(IDC_CMD_CEB_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_cebgroup, nDistY);
	// CEB1978 combo
	GetDlgItem(IDC_CMD_HCRP_CEB_RELA)->GetWindowRect(rRef);
	GetDlgItem(IDC_CEB_FIP_CMB)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CArray<UINT, UINT> aCtrlID; aCtrlID.Add(IDC_CEB_FIP_CMB);
	CDlgUtil::CtrlMoveDistY(this, aCtrlID, nDistY);
	GetDlgItem(IDC_CMD_HCRP_CEB_RELA2)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CArray<UINT, UINT> aCtrlID2; aCtrlID2.Add(IDC_CMD_HCRP_CEB_RELA2);
	CDlgUtil::CtrlMoveDistY(this, aCtrlID2, nDistY);
	GetDlgItem(IDC_CMD_ACI_FRAME)->GetWindowRect(rRef);

	GetDlgItem(IDC_CMD_MEM_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_memgroup, nDistY);

	GetDlgItem(IDC_CMD_UDEF_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_udefgroup, nDistY);

	GetDlgItem(IDC_CMD_PCA_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_pcagroup, nDistY);
		
	GetDlgItem(IDC_CMD_COMB_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_combroup, nDistY);

	GetDlgItem(IDC_CMD_JAPAN_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_japangroup, nDistY);
	GetDlgItem(IDC_CMD_JAPAN_FRAME)->GetWindowRect(rToMove);
	ScreenToClient(rToMove);
	m_pJapanDlg->MoveWindow(rToMove, FALSE);

	GetDlgItem(IDC_CMD_CHINA_FRAME)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_chinagroup, nDistY);

	GetDlgItem(IDC_CMD_JSCE_FRM)->GetWindowRect(rToMove); // SKN
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_jscegroup, nDistY);
	
	GetDlgItem(IDC_CMD_CHJTG04_FRM)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_ChJTG04Group, nDistY);
	GetDlgItem(IDC_CMD_CHJTG04_FRM)->GetWindowRect(rToMove);
	ScreenToClient(rToMove);
	m_pChJTG04Dlg->MoveWindow(rToMove, FALSE);

	m_pEuropeanDlg->GetWindowRect(rToMove);
	int dWidth = rToMove.Width();
	int dHeight = rToMove.Height();
	rToMove.left = rRef.left;
	rToMove.top = rRef.top;
	rToMove.right = rToMove.left + dWidth;
	rToMove.bottom = rToMove.top + dHeight;
	ScreenToClient(rToMove);
	m_pEuropeanDlg->MoveWindow(rToMove, FALSE);

	m_pIndiaDlg->GetWindowRect(rToMove);
	dWidth = rToMove.Width();
	dHeight = rToMove.Height();
	rToMove.left = rRef.left;
	rToMove.top = rRef.top;
	rToMove.right = rToMove.left + dWidth;
	rToMove.bottom = rToMove.top + dHeight;
	ScreenToClient(rToMove);
	m_pIndiaDlg->MoveWindow(rToMove, FALSE);

	m_pAASHTODlg->GetWindowRect(rToMove);
	dWidth = rToMove.Width();
	dHeight = rToMove.Height();
	rToMove.left = rRef.left;
	rToMove.top = rRef.top;
	rToMove.right = rToMove.left + dWidth;
	rToMove.bottom = rToMove.top + dHeight;
	ScreenToClient(rToMove);
	m_pAASHTODlg->MoveWindow(rToMove, FALSE);

	m_pASDlg->GetWindowRect(rToMove);
	dWidth = rToMove.Width();
	dHeight = rToMove.Height();
	rToMove.left = rRef.left;
	rToMove.top = rRef.top;
	rToMove.right = rToMove.left + dWidth;
	rToMove.bottom = rToMove.top + dHeight;
	ScreenToClient(rToMove);
	m_pASDlg->MoveWindow(rToMove, FALSE);

	m_pIndia2011Dlg->GetWindowRect(rToMove);
	dWidth = rToMove.Width();
	dHeight = rToMove.Height();
	rToMove.left = rRef.left;
	rToMove.top = rRef.top;
	rToMove.right = rToMove.left + dWidth;
	rToMove.bottom = rToMove.top + dHeight;
	ScreenToClient(rToMove);
	m_pIndia2011Dlg->MoveWindow(rToMove, FALSE);

    m_pIndia2011_2020Dlg->GetWindowRect(rToMove);
    dWidth = rToMove.Width();
    dHeight = rToMove.Height();
    rToMove.left = rRef.left;
    rToMove.top = rRef.top;
    rToMove.right = rToMove.left + dWidth;
    rToMove.bottom = rToMove.top + dHeight;
    ScreenToClient(rToMove);
    m_pIndia2011_2020Dlg->MoveWindow(rToMove, FALSE);

	m_pRusDlg->GetWindowRect(rToMove);
	dWidth = rToMove.Width();
	dHeight = rToMove.Height();
	rToMove.left = rRef.left;
	rToMove.top = rRef.top;
	rToMove.right = rToMove.left + dWidth;
	rToMove.bottom = rToMove.top + dHeight;
	ScreenToClient(rToMove);
	m_pRusDlg->MoveWindow(rToMove, FALSE);

	m_pChJTG16Dlg->GetWindowRect(rToMove);
	dWidth = rToMove.Width();
	dHeight = rToMove.Height();
	rToMove.left = rRef.left;
	rToMove.top = rRef.top;
	rToMove.right = rToMove.left + dWidth;
	rToMove.bottom = rToMove.top + dHeight;
	ScreenToClient(rToMove);
	m_pChJTG16Dlg->MoveWindow(rToMove, FALSE);

	m_pNZDlg->GetWindowRect(rToMove);
	dWidth = rToMove.Width();
	dHeight = rToMove.Height();
	rToMove.left = rRef.left;
	rToMove.top = rRef.top;
	rToMove.right = rToMove.left + dWidth;
	rToMove.bottom = rToMove.top + dHeight;
	ScreenToClient(rToMove);
	m_pNZDlg->MoveWindow(rToMove, FALSE);

	m_pKDS2016Dlg->GetWindowRect(rToMove);
	dWidth = rToMove.Width();
	dHeight = rToMove.Height();
	rToMove.left = rRef.left;
	rToMove.top = rRef.top;
	rToMove.right = rToMove.left + dWidth;
	rToMove.bottom = rToMove.top + dHeight;
	ScreenToClient(rToMove);
	m_pKDS2016Dlg->MoveWindow(rToMove, FALSE);
	
	m_pJSCE07Dlg->GetWindowRect(rToMove);
	dWidth = rToMove.Width();
	dHeight = rToMove.Height();
	rToMove.left = rRef.left;
	rToMove.top = rRef.top;
	rToMove.right = rToMove.left + dWidth;
	rToMove.bottom = rToMove.top + dHeight;
	ScreenToClient(rToMove);
	m_pJSCE07Dlg->MoveWindow(rToMove, FALSE);

	m_pJSCE12Dlg->GetWindowRect(rToMove);
	dWidth = rToMove.Width();
	dHeight = rToMove.Height();
	rToMove.left = rRef.left;
	rToMove.top = rRef.top;
	rToMove.right = rToMove.left + dWidth;
	rToMove.bottom = rToMove.top + dHeight;
	ScreenToClient(rToMove);
	m_pJSCE12Dlg->MoveWindow(rToMove, FALSE);

	m_pABNT_NBR_6118Dlg->GetWindowRect(rToMove);
	dWidth = rToMove.Width();
	dHeight = rToMove.Height();
	rToMove.left = rRef.left;
	rToMove.top = rRef.top;
	rToMove.right = rToMove.left + dWidth;
	rToMove.bottom = rToMove.top + dHeight;
	ScreenToClient(rToMove);
	m_pABNT_NBR_6118Dlg->MoveWindow(rToMove, FALSE);
	
	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+16;

	MoveWindow(r);
}

void CCMHcrpDlg::SetUnitType()
{
	//----------------------------------------------------------------
	// ACI
	//----------------------------------------------------------------
	m_acisrkgstrn.SetUnitType(0);
	m_acicrpcoef.SetUnitType(0);
	m_acicompunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_acicomp.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_aciageunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);
	m_concA.SetUnitType(0);
	m_concB.SetUnitType(0);

	//----------------------------------------------------------------
	// CEB & Korea (1990)
	//----------------------------------------------------------------
	m_cebrela2.SetUnitType(0);
	m_notaunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMemberSize);
	m_nota.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMemberSize);
	m_cebcompunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_cebcomp.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_cebageunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);
	m_cemeunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCementContent);
	m_ceme.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCementContent);

	//----------------------------------------------------------------
	// Modify Modulus of Elasticity
	//----------------------------------------------------------------
	m_memPhi1.SetUnitType(0);
	m_memPhi2.SetUnitType(0);
	m_slumunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dSlump);

	//----------------------------------------------------------------
	// User Defined
	//----------------------------------------------------------------
	m_slum.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dSlump);
	m_voluunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dVolumeSurfaceRatio);
	m_volu.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dVolumeSurfaceRatio);

	//----------------------------------------------------------------
	// Combined
	//----------------------------------------------------------------
	m_combVS.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dVolumeSurfaceRatio);
	m_combVSUT.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dVolumeSurfaceRatio);
	m_combmodul.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dModulusOfElasticity);
	m_combmodulUT.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dModulusOfElasticity);
	m_combfc28.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_combfc28UT.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_combmatFac.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCreep_CreepStrain);
	m_combmatFacUT.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCreep_CreepStrain);

	//----------------------------------------------------------------
	// PCA
	//----------------------------------------------------------------
	m_pcavsratio.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dVolumeSurfaceRatio);
	m_pcavsratioUT.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dVolumeSurfaceRatio);
	m_pcamodul.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dModulusOfElasticity);
	m_pcamodulUT.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dModulusOfElasticity);
	m_pcafc28.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_pcafc28UT.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);

	//----------------------------------------------------------------
	// Japanese Standard
	//----------------------------------------------------------------
	m_jpannotaunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMemberSize);
	m_jpannota.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMemberSize);
	m_jpancompunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_jpancomp.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_jpanageunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);

	//----------------------------------------------------------------
	// Chinese Standard
	//----------------------------------------------------------------
	m_chinaRelaunit.SetUnitType(0);
	m_chinaAgeunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);
	m_chinaNotaunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMemberSize);
	m_chinaNota.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dMemberSize);
	m_chinaCompunit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);
	m_chinaComp.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCompStrength);

	//----------------------------------------------------------------
	// JAPAN(JSCE) // SKN
	//----------------------------------------------------------------
//  m_jsceAc.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dConcreteAge);
	m_jsceWcUnit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dWaterContent);
	m_jsceWc.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dWaterContent);

	m_jsceCcUnit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCementContent);
	m_jsceCc.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dCementContent);

	m_jsceVs.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dVolumeSurfaceRatio);
	m_jsceVsUnit.SetUnitType(CUnitCtrl::m_TDMT_UNIT.dVolumeSurfaceRatio);  
}
 
void CCMHcrpDlg::SetData2Dlg()
{
	m_strTDefMatName = m_data.strMatlName;
	CDlgUtil::CobxSetCurSelItemData(m_CobxCode, m_data.COMMON.nCode);

	//----------------------------------------------------------------
	// ACI
	//----------------------------------------------------------------
	m_acirela.SetValue(m_data.COMMON.dRelativeHumidity);
	m_acicomp.SetEditUnit(m_data.COMMON.dCompStrength);
	m_aciage.SetValue(m_data.COMMON.dConcreteAge);
	m_volu.SetEditUnit(m_data.ACI.dVolumeSurfaceRatio);
	m_curemethod = m_data.ACI.nCuringMethod - 1;
	m_ACIorUser = m_data.ACI.nType;
	// m_data.ACI.nType == 0 �϶� 
	m_slum.SetEditUnit(m_data.ACI.dSlump);
	m_air.SetValue(m_data.ACI.dAirContent);
	m_fine.SetValue(m_data.ACI.dFineAggPercent);
	m_ceme.SetEditUnit(m_data.ACI.dCementContent);
	// m_data.ACI.nType != 0 �϶� 
	m_acicrpcoef.SetEditUnit(m_data.ACI.dCreepCoeff);
	m_acisrkgstrn.SetEditUnit(m_data.ACI.dShrinkStrain);
	m_concA.SetEditUnit(m_data.ACI.dConcFactorA);
	m_concB.SetEditUnit(m_data.ACI.dConcFactorB);

	//----------------------------------------------------------------
	// CEB & Korea (1990) or CEB-FIP(2010)
	//----------------------------------------------------------------
	if (m_data.COMMON.dRelativeHumidity == 100.0)
	{
		if (m_data.COMMON.nCode != 13) m_data.COMMON.dRelativeHumidity = 99.0;
	}

	m_cebrela.SetValue(m_data.COMMON.dRelativeHumidity);
	m_cebcomp.SetEditUnit(m_data.COMMON.dCompStrength);
	m_cebage.SetValue(m_data.COMMON.dConcreteAge);
	m_cementType = m_data.CEB.nCementType - 1;
	m_cmbAggre.SetCurSel(m_data.CEB.nTypeofAffr);
	m_nota.SetEditUnit(m_data.CEB.dMemberSize);

	//----------------------------------------------------------------
	// CEB & Korea (1978)  // MNET:2406-JHLEE-20070822
	//----------------------------------------------------------------

	if (m_data.COMMON.dRelativeHumidity == 30.0) m_cmbCebRela.SetCurSel(0);
	else if (m_data.COMMON.dRelativeHumidity == 70.0) m_cmbCebRela.SetCurSel(1);
	else if (m_data.COMMON.dRelativeHumidity == 90.0) m_cmbCebRela.SetCurSel(2);
	else if (m_data.COMMON.dRelativeHumidity == 100.0) m_cmbCebRela.SetCurSel(3);

	m_cebrela2.SetEditUnit(m_data.COMMON.dRelativeHumidity);

	//----------------------------------------------------------------
	// Modify Modulus of Elasticity
	//----------------------------------------------------------------
	m_memN1.SetValue(m_data.ELAST.dN1);
	m_memN2.SetValue(m_data.ELAST.dN2);
	m_memPhi1.SetEditUnit(m_data.ELAST.dPhi1);
	m_memPhi2.SetEditUnit(m_data.ELAST.dPhi2);

	//----------------------------------------------------------------
	// User Defined 
	//----------------------------------------------------------------
	CStringArray Contents;
	int nUCreep = m_data.aCreepFunc.GetSize();

	CString TStr;
	T_TDMF_D TdmfD;
	T_TDMT_BASE TdmtBase;

	for (int i = 0; i < nUCreep; i++)
	{
		Contents.RemoveAll();
		TdmtBase = m_data.aCreepFunc[i];

		if (m_pDoc->m_pAttrCtrl->GetTdmf(TdmtBase.CreepFuncKey, TdmfD))
		{
			TStr.Format(_T("%.15g"), TdmtBase.dLoadingAge);
			Contents.Add(TdmfD.MatlFuncName);
			Contents.Add(TStr);
			CDlgUtil::SetListItem(&m_wndUdefFList, i, Contents, TdmtBase.CreepFuncKey);
		}
	}
	if (0 != m_data.USER.ShrinkageStrainFuncKey) m_bSSTR = TRUE;
	else                                        m_bSSTR = FALSE;

	m_CobxSstr.ChangeSelect(m_data.USER.ShrinkageStrainFuncKey);

	//----------------------------------------------------------------
	// PCA
	//----------------------------------------------------------------
	m_pcahumid.SetValue(m_data.COMMON.dRelativeHumidity);
	m_pcafc28.SetEditUnit(m_data.COMMON.dCompStrength);
	m_pcasrkgstrn.SetValue(m_data.PCA.dUltimateShrinkageStrain);
	m_pcacrpstrn.SetValue(m_data.PCA.dUltimateCreepStrain);
	m_pcavsratio.SetEditUnit(m_data.PCA.dVolumeSurfaceRatio);
	m_pcareinratio.SetValue(m_data.PCA.dReinforcementRatio);
	m_pcamodul.SetEditUnit(m_data.PCA.dModulusOfElasticity);

	//----------------------------------------------------------------
	// Combined
	//----------------------------------------------------------------
	m_combfc28.SetEditUnit(m_data.COMMON.dCompStrength);
	m_combhumid.SetValue(m_data.COMMON.dRelativeHumidity);
	m_combVS.SetEditUnit(m_data.COMBINED.dVolumeSurfaceRatio);
	m_combmatFac.SetEditUnit(m_data.COMBINED.dCreep_CreepStrain);
	m_combvsratio = m_data.COMBINED.nCreep_VolumeSurfaceRatio;
	m_combLFac = m_data.COMBINED.nCreep_LoadingAgedFactor;
	m_combsmatFac.SetEditUnit(m_data.COMBINED.dShrink_Shrinkage);
	m_combsvsratio = m_data.COMBINED.nShrink_VolumeSurfaceRatio;
	m_combprog = m_data.COMBINED.nShrink_ProgressOfShrinkage;
	m_bPCAEffect = m_data.COMBINED.bReinforcedConcreteEffect;
	if (m_data.COMBINED.bReinforcedConcreteEffect)
	{
		m_combratio.SetValue(m_data.COMBINED.dReinforcementRatio);
		m_combmodul.SetEditUnit(m_data.COMBINED.dModulusOfElasticity);
	}

	//----------------------------------------------------------------
	// JAPAN
	//----------------------------------------------------------------
	/*
	m_jpancomp.SetEditUnit(m_data.COMMON.dCompStrength);
	m_jpanrela.SetEditUnit(m_data.COMMON.dRelativeHumidity);
	m_jpanage.SetEditUnit(m_data.COMMON.dConcreteAge);
	m_jpannota.SetEditUnit(m_data.JAPAN.dMemberSize);
	m_jpancemen = m_data.JAPAN.nCementType - 1;        // (JAPAN) 1:Rapid Hardening cement, 2:Normal cement
	m_jpanhumid = m_data.JAPAN.nRelativeHumidity - 1;  // (JAPAN) 1:Curing Underwater, 2:Relative Humidity
	EnableJapanRelaCtrl(m_jpanhumid);
	*/
	if (m_pJapanDlg) m_pJapanDlg->SetData2Dlg();

	if (m_pChJTG04Dlg) m_pChJTG04Dlg->SetData2Dlg();

	if (m_pAASHTODlg) m_pAASHTODlg->SetData2Dlg();

	if (m_pEuropeanDlg) m_pEuropeanDlg->SetData2Dlg();

	if (m_pIndiaDlg) m_pIndiaDlg->SetData2Dlg();

	if (m_pASDlg) m_pASDlg->SetData2Dlg();

	if (m_pIndia2011Dlg) m_pIndia2011Dlg->SetData2Dlg();

	if (m_pIndia2011_2020Dlg) m_pIndia2011_2020Dlg->SetData2Dlg();

	if (m_pRusDlg) m_pRusDlg->SetData2Dlg();

	if (m_pChJTG16Dlg) m_pChJTG16Dlg->SetData2Dlg();

	if (m_pNZDlg) m_pNZDlg->SetData2Dlg();

	if (m_pKDS2016Dlg) m_pKDS2016Dlg->SetData2Dlg();

	if (m_pJSCE07Dlg) m_pJSCE07Dlg->SetData2Dlg();

	if (m_pJSCE12Dlg) m_pJSCE12Dlg->SetData2Dlg();

  if (m_pABNT_NBR_6118Dlg) m_pABNT_NBR_6118Dlg->SetData2Dlg();
	//----------------------------------------------------------------
	// CHINA
	//----------------------------------------------------------------
	m_chinaRela.SetValue(m_data.COMMON.dRelativeHumidity);
	m_chinaComp.SetEditUnit(m_data.COMMON.dCompStrength);
	m_chinaAge.SetValue(m_data.COMMON.dConcreteAge);
	m_chinaNota.SetEditUnit(m_data.CHINA.dMemberSize);
	m_chinaRelradio = m_data.CHINA.nRelativeHumidity - 1;
	EnableChinaRelaCtrl(m_chinaRelradio);

	//----------------------------------------------------------------
	// JAPAN(JSCE) // skn
	//----------------------------------------------------------------
	m_jsceAc.SetValue(m_data.JPJSCE.dConcreteAge);
	m_jsceWc.SetEditUnit(m_data.JPJSCE.dWaterContent);
	m_jsceCc.SetEditUnit(m_data.JPJSCE.dCementContent);
	m_jsceVs.SetEditUnit(m_data.JPJSCE.dVolumeSurfaceRatio);
	m_jsceRh.SetValue(m_data.JPJSCE.dRelativeHumidity);

	UpdateData(FALSE);

	OnCmdCombPcaEffectChk();
}

BOOL CCMHcrpDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	// Japanese Standard���� ���� Child Dialog �����.
	m_pJapanDlg = new CCMTdmtJapan;
	m_pJapanDlg->m_pData = &m_data;
	m_pJapanDlg->Create(m_pJapanDlg->IDD, this);
	m_pJapanDlg->UpdateData(FALSE);
	m_pJapanDlg->ShowWindow(SW_HIDE); // �ϴ��� �����.

	m_pChJTG04Dlg = new CCMTdmtChJTG04;
	m_pChJTG04Dlg->m_pData = &m_data;
	m_pChJTG04Dlg->Create(m_pChJTG04Dlg->IDD, this);
	m_pChJTG04Dlg->UpdateData(FALSE);
	m_pChJTG04Dlg->ShowWindow(SW_HIDE); // �ϴ��� �����.

	m_pAASHTODlg = new CCMTdmtAASHTO;
	m_pAASHTODlg->m_pData = &m_data;
	m_pAASHTODlg->Create(m_pAASHTODlg->IDD, this);
	m_pAASHTODlg->UpdateData(FALSE);
	m_pAASHTODlg->ShowWindow(SW_HIDE);  // �ϴ��� �����.

	m_pEuropeanDlg = new CCMTdmtEuropean;
	m_pEuropeanDlg->m_pData = &m_data;
	m_pEuropeanDlg->Create(m_pEuropeanDlg->IDD, this);
	m_pEuropeanDlg->UpdateData(FALSE);
	m_pEuropeanDlg->ShowWindow(SW_HIDE);  // �ϴ��� �����.

	m_pIndiaDlg = new CCMTdmtIndia;
	m_pIndiaDlg->m_pData = &m_data;
	m_pIndiaDlg->Create(m_pIndiaDlg->IDD, this);
	m_pIndiaDlg->UpdateData(FALSE);
	m_pIndiaDlg->ShowWindow(SW_HIDE);  // �ϴ��� �����.

	m_pASDlg = new CCMTdmtAUSTRALIA;
	m_pASDlg->m_pData = &m_data;
	m_pASDlg->Create(m_pASDlg->IDD, this);
	m_pASDlg->UpdateData(FALSE);
	m_pASDlg->ShowWindow(SW_HIDE);  // �ϴ��� �����.
		
	m_pIndia2011Dlg = new CCMTdmtIndia2011;
	m_pIndia2011Dlg->m_pData = &m_data;
	m_pIndia2011Dlg->Create(m_pIndia2011Dlg->IDD, this);
	m_pIndia2011Dlg->UpdateData(FALSE);
	m_pIndia2011Dlg->ShowWindow(SW_HIDE);  // �ϴ��� �����.

    m_pIndia2011_2020Dlg = new CCMTdmtIndia2011_2020;
    m_pIndia2011_2020Dlg->m_pData = &m_data;
    m_pIndia2011_2020Dlg->Create(m_pIndia2011_2020Dlg->IDD, this);
    m_pIndia2011_2020Dlg->UpdateData(FALSE);
    m_pIndia2011_2020Dlg->ShowWindow(SW_HIDE);  // �ϴ��� �����.

	m_pRusDlg = new CCMTdmtRUS;
	m_pRusDlg->m_pData = &m_data;
	m_pRusDlg->Create(m_pRusDlg->IDD, this);
	m_pRusDlg->UpdateData(FALSE);
	m_pRusDlg->ShowWindow(SW_HIDE);  // �ϴ��� �����.

	m_pChJTG16Dlg = new CCMTdmtChJTG16;
	m_pChJTG16Dlg->m_pData = &m_data;
	m_pChJTG16Dlg->Create(m_pChJTG16Dlg->IDD, this);
	m_pChJTG16Dlg->UpdateData(FALSE);
	m_pChJTG16Dlg->ShowWindow(SW_HIDE);  // �ϴ��� �����.

	m_pNZDlg = new CCMTdmtNEWZEALAND;
	m_pNZDlg->m_pData = &m_data;
	m_pNZDlg->m_nModify = m_nOptionForTimeDep;
	m_pNZDlg->Create(m_pNZDlg->IDD, this);
	m_pNZDlg->UpdateData(FALSE);
	m_pNZDlg->ShowWindow(SW_HIDE);  // �ϴ��� �����.  

	m_pKDS2016Dlg = new CCMTdmtKDS2016;
	m_pKDS2016Dlg->m_pData = &m_data;
	m_pKDS2016Dlg->m_nModify = m_nOptionForTimeDep;
	m_pKDS2016Dlg->Create(m_pKDS2016Dlg->IDD, this);
	m_pKDS2016Dlg->UpdateData(FALSE);
	m_pKDS2016Dlg->ShowWindow(SW_HIDE);  // �ϴ��� �����.  

	m_pJSCE07Dlg = new CCMTdmtJSCE07;
	m_pJSCE07Dlg->m_pData = &m_data;
	m_pJSCE07Dlg->Create(m_pJSCE07Dlg->IDD, this);
	m_pJSCE07Dlg->UpdateData(FALSE);
	m_pJSCE07Dlg->ShowWindow(SW_HIDE);  // �ϴ��� �����.  

	m_pJSCE12Dlg = new CCMTdmtJSCE12;
	m_pJSCE12Dlg->m_pData = &m_data;
	m_pJSCE12Dlg->Create(m_pJSCE12Dlg->IDD, this);
	m_pJSCE12Dlg->UpdateData(FALSE);
	m_pJSCE12Dlg->ShowWindow(SW_HIDE);  // �ϴ��� �����.  

	m_pABNT_NBR_6118Dlg = new CCMTdmt_ABNT_NBR_6118;
	m_pABNT_NBR_6118Dlg->m_pData = &m_data;
	m_pABNT_NBR_6118Dlg->Create(m_pABNT_NBR_6118Dlg->IDD, this);
	m_pABNT_NBR_6118Dlg->UpdateData(FALSE);
	m_pABNT_NBR_6118Dlg->ShowWindow(SW_HIDE);  // �ϴ��� �����.

	m_CobxCreep.ChangeType(D_COBXTDMF_CREEP);
	m_CobxSstr .ChangeType(D_COBXTDMF_SHRINKAGE);

	m_pDoc = CDBDoc::GetDocPoint();

	// �� Code ���� Setting
	ModeCtrlMan(); 

	// Code�� Controls Layout
	AlignControls();
	CWnd *pWnd = GetDlgItem(IDC_CMD_HCRP_REMOVE_BTN);
	pWnd->ShowWindow(SW_HIDE);

	// Set Unit
	SetUnitType();

	// Data Initialize
	if(m_nOptionForTimeDep == 0) 
	{
		m_data.Initialize();
		m_data.RUSSIA.dMaxSize = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitLengthOut(0.02);	// Default 20mm

		double dLength = CDBDoc::GetDocPoint()->m_pUnitCtrl->ConvertUnitLengthOut(1);	// Default 1m
		m_data.RUSSIA.dWater = 180.0 / pow(dLength,3);
		m_data.RUSSIA.dAir = 30.0 / pow(dLength,3);
	}

	// Spin �ʱⰪ �� ���� ����
	m_acirela.SetRange(40, 99);
	m_acirela.SetValue(70);

	m_aciage.SetRange(0, 10000);
	m_aciage.SetValue(3);

	m_fine.SetRange(0, 100);
	m_fine.SetValue(0);

	m_air.SetRange(0, 100);
	m_air.SetValue(0);

	m_cebrela.SetRange(40, 99);
	m_cebrela.SetValue(70);

	m_cebage.SetRange(0, 100);
	m_cebage.SetValue(3);

	m_memN1.SetRange(0, 10000);
	m_memN1.SetValue(0);

	m_memN2.SetRange(0, 10000);
	m_memN2.SetValue(0);

	m_jpanrela.SetRange(40, 90);
	m_jpanrela.SetValue(70);

	m_jpanage.SetRange(0, 100);
	m_jpanage.SetValue(3);

	m_chinaRela.SetRange(40, 90);
	m_chinaRela.SetValue(70);

	m_chinaAge.SetRange(0, 100);
	m_chinaAge.SetValue(3);

	m_pcasrkgstrn.SetRange(500, 800);
	m_pcasrkgstrn.SetValue(780);

	m_pcacrpstrn.SetRange(3, 5);
	m_pcacrpstrn.SetValue(4);

	m_pcahumid.SetRange(40, 99);
	m_pcahumid.SetValue(70);

	m_pcareinratio.SetRange(0, 100);
	m_pcareinratio.SetValue(0);

	m_combhumid.SetRange(40, 99);
	m_combhumid.SetValue(70);

	m_combratio.SetRange(0, 100);
	m_combratio.SetValue(0);

	m_jsceRh.SetRange(45, 80);
	m_jsceRh.SetValue(70);

	m_jsceAc.SetRange(1, 10000);
	m_jsceAc.SetValue(3);
	//

	// Data �ʱ�ȭ ����   
	CStringArray HTitles;
	CArray<float,float> HRatio;
	CString strTemp;
	strTemp = _LS(IDS_CMD0417__Name);
	HTitles.Add(strTemp);
	strTemp = _LS(IDS_CMD0417__Loading_Age);
	HTitles.Add(strTemp);
	HRatio.Add(0.45f);
	HRatio.Add(0.55f);

	// MNET:2406-JHLEE-20070822
	m_cmbCebRela.AddString(_T("30"));
	m_cmbCebRela.AddString(_T("70"));
	m_cmbCebRela.AddString(_T("90"));
	m_cmbCebRela.AddString(_T("100(Water)"));
	m_stcRHEnv.SetWindowText(_T("Relative Humidity of ambient environment (30 - 99) "));

	if (m_data.COMMON.dRelativeHumidity == 30.0 ) m_cmbCebRela.SetCurSel(0);
	else if (m_data.COMMON.dRelativeHumidity == 70.0 ) m_cmbCebRela.SetCurSel(1);
	else if (m_data.COMMON.dRelativeHumidity == 90.0 ) m_cmbCebRela.SetCurSel(2);
	else if (m_data.COMMON.dRelativeHumidity == 100.0 ) m_cmbCebRela.SetCurSel(3);
	else
		m_cmbCebRela.SetCurSel(1);
		
	m_cmbAggre.AddString(_LS(IDS_CMD_ETC_HCRP_CEB_FIP_2010_BASALT));
	m_cmbAggre.AddString(_LS(IDS_CMD_ETC_HCRP_CEB_FIP_2010_QUARTZITE));
	m_cmbAggre.AddString(_LS(IDS_CMD_ETC_HCRP_CEB_FIP_2010_LIMESTONE));
	m_cmbAggre.AddString(_LS(IDS_CMD_ETC_HCRP_CEB_FIP_2010_SANDSTONE));

	CDlgUtil::_SetListCtrlHeader(&m_wndUdefFList,HTitles,&HRatio,NULL);

	SetData2Dlg();
	if(m_nOptionForTimeDep==0) m_CobxCode.SetCurSel(0); // Add �� ��
	
#if defined(_RUS)
	if(m_nOptionForTimeDep==0) CDlgUtil::CobxSetCurSelItemData(m_CobxCode, TDMT_CODE_RUSSIAN);
#endif

#if defined(_CIVIL_JP)
	if (!m_pDoc->AllowCommand(D_OPTN_ID_CVLNX_JP_STAGE)
		&& !m_pDoc->AllowCommand(D_OPTN_ID_CVLNX_JP_THERMAL_STRESS))
	{
		GetDlgItem(IDC_CMD_UDEF_CFUNC_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_UDEF_SSTR_BUTTON)->EnableWindow(FALSE);
	}
#endif

	OnSelchangeHcrpCodeCombo();
	OnHcrpSelChangeType();
	
	this->m_wndLoadAge.SetWindowText(_T("0"));
 
	ManUDefCtrls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/////////////////////////////////////////////////////////////////////////////
//
// OnOK
//
void CCMHcrpDlg::SetDlg2Data()
{
	UpdateData(TRUE);
	m_data.Initialize();

	DWORD nCode;
	CDlgUtil::CobxGetItemDataByCurSel(m_CobxCode, nCode);
	m_data.COMMON.nCode = nCode;
	m_data.strMatlName  = m_strTDefMatName;
 
	if (nCode == 1)
	{
	  m_data.COMMON.dRelativeHumidity = m_acirela.GetEditValue();
		m_data.COMMON.dCompStrength     = m_acicomp.GetEditValue();
		m_data.COMMON.dConcreteAge      = m_aciage.GetEditValue();
		m_data.ACI.dVolumeSurfaceRatio  = m_volu.GetEditValue();
		m_data.ACI.nCuringMethod       = m_curemethod + 1;
		m_data.ACI.nType = m_ACIorUser;
		m_data.ACI.dConcFactorA        = m_concA.GetEditValue();
		m_data.ACI.dConcFactorB        = m_concB.GetEditValue();
		if(m_ACIorUser == 0)      // ACI Code
		{
		  m_data.ACI.dSlump              = m_slum.GetEditValue();
		  m_data.ACI.dAirContent         = m_air.GetEditValue() ;
		  m_data.ACI.dFineAggPercent     = m_fine.GetEditValue();
		  m_data.ACI.dCementContent      = m_ceme.GetEditValue();
		}
		else                      // User
		{
			m_data.ACI.dCreepCoeff         = m_acicrpcoef.GetEditValue();
			m_data.ACI.dShrinkStrain       = m_acisrkgstrn.GetEditValue();
		}
	}
	else if (nCode == 2 || nCode == 5 || nCode == 20 || nCode == 21 || nCode == 27)
	{
		m_data.COMMON.dRelativeHumidity = m_cebrela.GetEditValue();
		m_data.COMMON.dCompStrength     = m_cebcomp.GetEditValue();
		m_data.COMMON.dConcreteAge      = m_cebage.GetEditValue();
	  m_data.CEB.nCementType          = m_cementType + 1;
		m_data.CEB.nTypeofAffr          = m_cmbAggre.GetCurSel();
		m_data.CEB.dMemberSize          = m_nota.GetEditValue();
	}
	else if (nCode == 3)
	{
	  m_data.ELAST.dN1 = m_memN1.GetEditValue();
		m_data.ELAST.dN2 = m_memN2.GetEditValue();
		m_data.ELAST.dPhi1 = m_memPhi1.GetEditValue();
		m_data.ELAST.dPhi2 = m_memPhi2.GetEditValue();
	}
	else if (nCode == 4)
	{    
		m_data.aCreepFunc;
		m_data.aCreepFunc.RemoveAll();
		T_TDMF_K TempK;
		T_TDMT_BASE TdmtBase;
		DWORD ItemData;

		CStringArray Contents;      
		int nCFunc = m_wndUdefFList.GetItemCount();
		for(int I = 0 ; I < nCFunc ; I++)
		{
			CDlgUtil::GetListItem(&m_wndUdefFList,I,Contents,ItemData);
			TempK = (T_TDMF_K) m_wndUdefFList.GetItemData(I);
			CString tempStr = Contents.GetAt(1);
			CStrParser::GetFloatNumber(tempStr,TdmtBase.dLoadingAge);
			TdmtBase.CreepFuncKey = (T_TDMF_K)ItemData;
			m_data.aCreepFunc.Add(TdmtBase);
		}
		
		if(m_bSSTR) m_CobxSstr.GetSelectedTdmf(m_data.USER.ShrinkageStrainFuncKey);
	}
	else if(nCode == 6)   //PCA
	{
		m_data.COMMON.dRelativeHumidity     = m_pcahumid.GetEditValue();
		m_data.COMMON.dCompStrength         = m_pcafc28.GetEditValue();
		m_data.PCA.dUltimateShrinkageStrain = m_pcasrkgstrn.GetEditValue();
		m_data.PCA.dUltimateCreepStrain     = m_pcacrpstrn.GetEditValue();
		m_data.PCA.dVolumeSurfaceRatio      = m_pcavsratio.GetEditValue();
		m_data.PCA.dReinforcementRatio      = m_pcareinratio.GetEditValue();
		m_data.PCA.dModulusOfElasticity     = m_pcamodul.GetEditValue();
	}
	else if(nCode == 7)   // Combined
	{
		m_data.COMMON.dCompStrength               = m_combfc28.GetEditValue();
		m_data.COMMON.dRelativeHumidity           = m_combhumid.GetEditValue();
		m_data.COMBINED.dVolumeSurfaceRatio       = m_combVS.GetEditValue();
		m_data.COMBINED.dCreep_CreepStrain        = m_combmatFac.GetEditValue();
		m_data.COMBINED.nCreep_VolumeSurfaceRatio = m_combvsratio; 
		m_data.COMBINED.nCreep_LoadingAgedFactor  = m_combLFac; 
		m_data.COMBINED.dShrink_Shrinkage         = m_combsmatFac.GetEditValue();
		m_data.COMBINED.nShrink_VolumeSurfaceRatio= m_combsvsratio; 
		m_data.COMBINED.nShrink_ProgressOfShrinkage = m_combprog; 
		m_data.COMBINED.bReinforcedConcreteEffect = m_bPCAEffect;
		if(m_bPCAEffect)
		{
			m_data.COMBINED.dReinforcementRatio       = m_combratio.GetEditValue();
			m_data.COMBINED.dModulusOfElasticity      = m_combmodul.GetEditValue();
		}
	}
	else if(nCode == 8)
	{
		/*
		m_data.COMMON.dCompStrength     = m_jpancomp.GetEditValue();
		m_data.COMMON.dRelativeHumidity = m_jpanrela.GetEditValue();
		m_data.COMMON.dConcreteAge      = m_jpanage.GetEditValue();
		m_data.JAPAN.dMemberSize        = m_jpannota.GetEditValue();
		m_data.JAPAN.nCementType        = m_jpancemen + 1;    // (JAPAN) 1:Rapid Hardening cement, 2:Normal cement
		m_data.JAPAN.nRelativeHumidity  = m_jpanhumid + 1;    // (JAPAN) 1:Curing Underwater, 2:Relative Humidity
		*/
		if (m_pJapanDlg) m_pJapanDlg->SetDlg2Data();
	}
	else if(nCode == 9)   // Chinese Standard
	{
		m_data.COMMON.dCompStrength     = m_chinaComp.GetEditValue();
		m_data.COMMON.dRelativeHumidity = m_chinaRela.GetEditValue();
		m_data.COMMON.dConcreteAge      = m_chinaAge.GetEditValue();
		m_data.CHINA.dMemberSize        = m_chinaNota.GetEditValue();
		m_data.CHINA.nRelativeHumidity  = m_chinaRelradio + 1; 
	}
	else if(nCode == 10)
	{
		
		m_data.JPJSCE.dCementContent = m_jsceCc.GetEditValue();
		m_data.JPJSCE.dConcreteAge = m_jsceAc.GetEditValue();
		
		m_data.JPJSCE.dRelativeHumidity = m_jsceRh.GetEditValue();
		m_data.JPJSCE.dVolumeSurfaceRatio =  m_jsceVs.GetEditValue();
		m_data.JPJSCE.dWaterContent = m_jsceWc.GetEditValue();
	}
	else if(nCode == TDMT_CODE_CHINA_JTG)  // China JTG D62-2004
	{
		if(m_pChJTG04Dlg) m_pChJTG04Dlg->SetDlg2Data();
	}
	else if (nCode == 12) // AASHTO
	{
		if (m_pAASHTODlg) m_pAASHTODlg->SetDlg2Data();
	}
	// MNET:2406-JHLEE-20070822
	else if (nCode == 13)
	{
		m_data.COMMON.dCompStrength     = m_cebcomp.GetEditValue();
		m_data.COMMON.dConcreteAge      = m_cebage.GetEditValue();
	  m_data.CEB.nCementType          = m_cementType + 1;
		m_data.CEB.dMemberSize          = m_nota.GetEditValue();

		switch(m_cmbCebRela.GetCurSel())
		{
		case 0: m_data.COMMON.dRelativeHumidity = 30.0; break;
		case 1: m_data.COMMON.dRelativeHumidity = 70.0; break;
		case 2: m_data.COMMON.dRelativeHumidity = 90.0; break;
		case 3: m_data.COMMON.dRelativeHumidity = 100.0; break;
		default : break;
		}

		m_data.COMMON.dRelativeHumidity = m_cebrela2.GetEditValue();
	}
	else if (nCode == 14)
	{
		if(m_pIndiaDlg) m_pIndiaDlg->SetDlg2Data();
	}
	else if (nCode == 15)
	{
		if (m_pEuropeanDlg) m_pEuropeanDlg->SetDlg2Data();
	}
	else if(nCode == 16)  // TDMT_CODE_JPJSCE07
	{
		if(m_pJSCE07Dlg) m_pJSCE07Dlg->SetDlg2Data();
	}  
	else if (nCode == TDMT_CODE_AUSTRALIA_2009 || nCode == TDMT_CODE_AUSTRALIA_2011 || 
		     nCode == TDMT_CODE_AUSTRALIA_2016 || nCode == TDMT_CODE_AUSTRALIA_2017 ||  // AUSTRALIA 2009, 2011, 2016, 2017
			 nCode == TDMT_CODE_AUSTRALIA_2018_AMD_2021 || nCode == TDMT_CODE_AUSTRALIA_2017_AMD_2024)
	{
		if (m_pASDlg) m_pASDlg->SetDlg2Data();
	}
	else if(nCode == 25)
	{
		if(m_pIndia2011Dlg) m_pIndia2011Dlg->SetDlg2Data();
	}
	else if(nCode == TDMT_CODE_RUSSIAN)
	{
		if(m_pRusDlg) m_pRusDlg->SetDlg2Data();
	}
	else if(nCode == TDMT_CODE_CHINA_JTG_16 || nCode == TDMT_CODE_CHINA_CHJTG_T_D65_2015)  // China JTG3362-2018
	{
		if(m_pChJTG16Dlg) m_pChJTG16Dlg->SetDlg2Data();
	}
	else if(nCode == TDMT_CODE_NEWZEALAND || nCode == TDMT_CODE_NEWZEALAND_2022)  // NEWZEALAND
	{
		if(m_pNZDlg) m_pNZDlg->SetDlg2Data();
	}  
	else if(nCode == 33)  // KDS-2016
	{
		if(m_pKDS2016Dlg) m_pKDS2016Dlg->SetDlg2Data();
	}  
	else if(nCode == 34)  // TDMT_CODE_JPJSCE12
	{
		if(m_pJSCE12Dlg) m_pJSCE12Dlg->SetDlg2Data();
	}  
	else if (nCode == 35)
	{
		if (m_pIndia2011_2020Dlg) m_pIndia2011_2020Dlg->SetDlg2Data();
	}
	else if (nCode == TDMT_CODE_ABNT_NBR_6118_2023)
	{
		if (m_pABNT_NBR_6118Dlg) m_pABNT_NBR_6118Dlg->SetDlg2Data();
	}
}

void CCMHcrpDlg::OnOK() 
{
	SetDlg2Data();
	
	BOOL bDBSuccess;
	if(m_nOptionForTimeDep == 0)  bDBSuccess = m_pDoc->m_pDataCtrl->AddTdmt(m_data);
	else
	{
		T_TDMT_D OldTdmt;
		m_pDoc->m_pAttrCtrl->GetTdmt(m_strOldMatName, OldTdmt);
		m_data.nSeq = OldTdmt.nSeq;
		bDBSuccess = m_pDoc->m_pDataCtrl->ModifyTdmt(m_strOldMatName,m_data);
	}

	if(bDBSuccess) CDialogMove::OnOK();
}

void CCMHcrpDlg::OnSelchangeHcrpCodeCombo()
{
	UpdateData(TRUE);
	DWORD nCode;
	CDlgUtil::CobxGetItemDataByCurSel(m_CobxCode, nCode);

	CRect rRef, rToMove;
	int   nDistY;

	CDlgUtil::CtrlShowHide(this, m_acigroup, FALSE);
	CDlgUtil::CtrlShowHide(this, m_cebgroup, FALSE);	
	CDlgUtil::CtrlShowHide(this, m_memgroup, FALSE);
	CDlgUtil::CtrlShowHide(this, m_udefgroup,FALSE);
	CDlgUtil::CtrlShowHide(this, m_pcagroup, FALSE);
	CDlgUtil::CtrlShowHide(this, m_combroup, FALSE);
	CDlgUtil::CtrlShowHide(this, m_japangroup, FALSE);
	CDlgUtil::CtrlShowHide(this, m_chinagroup, FALSE);
	CDlgUtil::CtrlShowHide(this, m_jscegroup, FALSE);  //skn
	CDlgUtil::CtrlShowHide(this, m_ChJTG04Group, FALSE);  //skn
	m_pJapanDlg->ShowWindow(SW_HIDE);
	m_pChJTG04Dlg->ShowWindow(SW_HIDE);
	m_pAASHTODlg->ShowWindow(SW_HIDE);
	m_pEuropeanDlg->ShowWindow(SW_HIDE);
	m_pIndiaDlg->ShowWindow(SW_HIDE);
	m_pASDlg->ShowWindow(SW_HIDE);
	m_pIndia2011Dlg->ShowWindow(SW_HIDE);
	m_pIndia2011_2020Dlg->ShowWindow(SW_HIDE);
	m_pRusDlg->ShowWindow(SW_HIDE);
	m_pChJTG16Dlg->ShowWindow(SW_HIDE);
	m_pNZDlg->ShowWindow(SW_HIDE);
	m_pKDS2016Dlg->ShowWindow(SW_HIDE);
 	m_pJSCE07Dlg->ShowWindow(SW_HIDE);
	m_pJSCE12Dlg->ShowWindow(SW_HIDE);
	m_pABNT_NBR_6118Dlg->ShowWindow(SW_HIDE);

	if (nCode == 1)                       // ACI
	{
		CDlgUtil::CtrlShowHide(this, m_acigroup, TRUE);

		if(m_data.ACI.nType == 0) OnCmdAciAciRd();
		else                      OnCmdAciUserRd();

		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_ACI_FRAME)->GetWindowRect(rRef);
	}
	else if (nCode == 2 || nCode == 5 || nCode == 20 || nCode == 21 || nCode == 27)    // CEB-FIP, Korean Standard, KSCE 2010, KCI-USD12
	{
		CDlgUtil::CtrlShowHide(this, m_cebgroup, TRUE);

		if (m_cebrela.GetEditValue() == 100.0)
			m_cebrela.SetValue(99.0);
		
		if     (nCode == 2)	m_frmCEB.SetWindowText(_LS(IDS_CMD_CEB_FIP_1990));
		else if(nCode == 5)	m_frmCEB.SetWindowText(_T("KS"));
		else if(nCode == 20)	m_frmCEB.SetWindowText(_T("KSCE 2010"));
		else if(nCode == 21)	m_frmCEB.SetWindowText(_T("KCI-USD12"));
		else if(nCode == 27)	m_frmCEB.SetWindowText(_LS(IDS_CMD_CEB_FIP_2010));

		m_stcRHEnv.SetWindowText(_LS(IDS_CMD_RELA_HUMIDITY_ENV_SPN));
		GetDlgItem(IDC_CMD_HCRP_CEB_RSCHK)->SetWindowText(_LS(IDS_CMD_RAPID_HIGH_SPN));
		GetDlgItem(IDC_CMD_HCRP_CEB_NRCHK)->SetWindowText(_LS(IDS_CMD_RAPID_CEMENT_SPN));
		GetDlgItem(IDC_CMD_HCRP_CEB_SLCHK)->SetWindowText(_LS(IDS_CMD_SLW_CEMENT_SPN));

		GetDlgItem(IDC_CEB_FIP_CMB)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_HCRP_CEB_RELA2)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_HCRP_CEB_RELA)->ShowWindow(TRUE); 

		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_CEB_FRAME)->GetWindowRect(rRef);

		if(nCode == 27)
		{
			GetDlgItem(IDC_CMD_HCRP_CEB_RSCHK)->SetWindowText(_T("42.5 R, 52.5 N, 52.5 R"));
			GetDlgItem(IDC_CMD_HCRP_CEB_NRCHK)->SetWindowText(_T("32.5 R, 42.5 N"));
			GetDlgItem(IDC_CMD_HCRP_CEB_SLCHK)->SetWindowText(_T("32.5 N"));
		}
	}
	else if (nCode == 3)                  // Modify Elasticity Modulus
	{
		CDlgUtil::CtrlShowHide(this, m_memgroup, TRUE);

		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_MEM_FRAME)->GetWindowRect(rRef);
	}
	else if (nCode == 4)                  // User Defined
	{
		CDlgUtil::CtrlShowHide(this, m_udefgroup,TRUE );

		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_UDEF_FRAME)->GetWindowRect(rRef);
	}
	else if(nCode == 6)                   // PCA
	{
		CDlgUtil::CtrlShowHide(this, m_pcagroup, TRUE);

		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_PCA_FRAME)->GetWindowRect(rRef);
	}
	else if(nCode == 7)                   // Combined
	{
		CDlgUtil::CtrlShowHide(this, m_combroup, TRUE);

		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_COMB_FRAME)->GetWindowRect(rRef);
	}
	else if(nCode == 8)                   // Japanese Standard
	{
		/*
		CDlgUtil::CtrlShowHide(this, m_japangroup, TRUE);
		*/
		m_pJapanDlg->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_JAPAN_FRAME)->GetWindowRect(rRef);
	}
	else if(nCode == 9)                   // Chinese Standard
	{
		CDlgUtil::CtrlShowHide(this, m_chinagroup, TRUE);

		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_CHINA_FRAME)->GetWindowRect(rRef);
	}
	else if(nCode == 10) // skn
	{
		CDlgUtil::CtrlShowHide(this, m_jscegroup, TRUE);

		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_JSCE_FRM)->GetWindowRect(rRef);
	}
	else if(nCode == TDMT_CODE_CHINA_JTG)                   // China JTG D62-2004
	{
		m_pChJTG04Dlg->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_CHJTG04_FRM)->GetWindowRect(rRef);   
	}
	else if (nCode == 12)   // AASHTO
	{
		m_pAASHTODlg->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		//GetDlgItem(IDC_CMD_ACI_FRAME)->GetWindowRect(rRef);   
		m_pAASHTODlg->GetWindowRect(rRef);
	}
	// MNET:2406-JHLEE-20070822
	else if (nCode == 13)    // CEB-FIP(1978), Korean Standard
	{
		CDlgUtil::CtrlShowHide(this, m_cebgroup, TRUE);
		
		m_frmCEB.SetWindowText(_LS(IDS_CMD_CEB_FIP_1978));
		//m_stcRHEnv.SetWindowText(_LS(IDS_CMD_RELA_HUMIDITY_ENV_CMB));
		m_stcRHEnv.SetWindowText(_LS(IDS_CMD_RELA_HUMIDITY_ENV_CMB2));
		GetDlgItem(IDC_CMD_HCRP_CEB_RSCHK)->SetWindowText(_LS(IDS_CMD_NOR_SLW_CEMENT_CMB));
		GetDlgItem(IDC_CMD_HCRP_CEB_NRCHK)->SetWindowText(_LS(IDS_CMD_RAPID_CEMENT_CMB));
		GetDlgItem(IDC_CMD_HCRP_CEB_SLCHK)->SetWindowText(_LS(IDS_CMD_RAPID_HIGH_CEMENT_CMB));

		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);

		GetDlgItem(IDC_CMD_HCRP_CEB_RELA)->ShowWindow(FALSE);
		GetDlgItem(IDC_CEB_FIP_CMB)->ShowWindow(FALSE);
		GetDlgItem(IDC_CMD_HCRP_CEB_RELA2)->ShowWindow(TRUE);
		
		GetDlgItem(IDC_CMD_CEB_FRAME)->GetWindowRect(rRef);
	}
//   else if (nCode == 27)    // CEB-FIP(2010), Korean Standard
//   {
//     CDlgUtil::CtrlShowHide(this, m_cebgroup, TRUE);
// 
//     m_frmCEB.SetWindowText(_LS(IDS_CMD_CEB_FIP_1978));
//     //m_stcRHEnv.SetWindowText(_LS(IDS_CMD_RELA_HUMIDITY_ENV_CMB));
//     m_stcRHEnv.SetWindowText(_LS(IDS_CMD_RELA_HUMIDITY_ENV_CMB2));
//     GetDlgItem(IDC_CMD_HCRP_CEB_RSCHK)->SetWindowText(_LS(IDS_CMD_NOR_SLW_CEMENT_CMB));
//     GetDlgItem(IDC_CMD_HCRP_CEB_NRCHK)->SetWindowText(_LS(IDS_CMD_RAPID_CEMENT_CMB));
//     GetDlgItem(IDC_CMD_HCRP_CEB_SLCHK)->SetWindowText(_LS(IDS_CMD_RAPID_HIGH_CEMENT_CMB));
// 
//     GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
// 
//     GetDlgItem(IDC_CMD_HCRP_CEB_RELA)->ShowWindow(FALSE);
//     GetDlgItem(IDC_CMD_HCRP_CEB_RELA_SPIN)->ShowWindow(FALSE);
//     GetDlgItem(IDC_CEB_FIP_CMB)->ShowWindow(FALSE);
//     GetDlgItem(IDC_CMD_HCRP_CEB_RELA2)->ShowWindow(TRUE);
// 
//     GetDlgItem(IDC_CMD_CEB_FRAME)->GetWindowRect(rRef);
// 
//   }
	else if (nCode == 14 )    // INDIA(IRC:18-2000)
	{
		m_pIndiaDlg->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		m_pIndiaDlg->GetWindowRect(rRef);
	}
	else if (nCode == 15)   // European
	{
		m_pEuropeanDlg->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		m_pEuropeanDlg->GetWindowRect(rRef);
	}
	else if (nCode == 16)    // JSCE07
	{
		m_pJSCE07Dlg->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		m_pJSCE07Dlg->GetWindowRect(rRef);
	} 
	else if (nCode == TDMT_CODE_AUSTRALIA_2009 || nCode == TDMT_CODE_AUSTRALIA_2011 ||
    		 nCode == TDMT_CODE_AUSTRALIA_2016 || nCode == TDMT_CODE_AUSTRALIA_2017 ||  // AUSTRALIA 2009, 2011, 2016, 2017
			 nCode == TDMT_CODE_AUSTRALIA_2018_AMD_2021 || nCode == TDMT_CODE_AUSTRALIA_2017_AMD_2024)
	{
		m_pASDlg->SetData2DlgByCodeChange(nCode);
		m_pASDlg->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		m_pASDlg->GetWindowRect(rRef);
	}
	else if (nCode == 25 )    // INDIA(IRC:112-2011)
	{
		m_pIndia2011Dlg->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		m_pIndia2011Dlg->GetWindowRect(rRef);
	}
	else if (nCode == TDMT_CODE_RUSSIAN)    // Russia
	{
		m_pRusDlg->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		m_pRusDlg->GetWindowRect(rRef);
	}
    else if (nCode == 35)    // INDIA(IRC:112-2020)
    {
       m_pIndia2011_2020Dlg->ShowWindow(SW_SHOW);
       GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
       m_pIndia2011_2020Dlg->GetWindowRect(rRef);
    }
	else if (nCode == TDMT_CODE_CHINA_JTG_16 || nCode == TDMT_CODE_CHINA_CHJTG_T_D65_2015)    // China JTG3362-2018
	{
		m_pChJTG16Dlg->SetData2DlgByCodeChange(nCode);
		m_pChJTG16Dlg->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		m_pChJTG16Dlg->GetWindowRect(rRef);
	}
	else if (nCode == TDMT_CODE_NEWZEALAND || nCode == TDMT_CODE_NEWZEALAND_2022)    // NEWZEALAND
	{
		m_pNZDlg->SetData2DlgByCodeChange(nCode);
		m_pNZDlg->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		m_pNZDlg->GetWindowRect(rRef);
	}  
	else if (nCode == 33)    // KDS-2016
	{
		m_pKDS2016Dlg->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		m_pKDS2016Dlg->GetWindowRect(rRef);
	}  
	else if (nCode == 34)    // JSCE12
	{
		m_pJSCE12Dlg->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		m_pJSCE12Dlg->GetWindowRect(rRef);
	}  
  else if (nCode == TDMT_CODE_ABNT_NBR_6118_2023)    // Brazil ABNT NBR 6118:2023
	{
		m_pABNT_NBR_6118Dlg->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CMD_ETC_HCRP_SHOW)->EnableWindow(TRUE);
		m_pABNT_NBR_6118Dlg->GetWindowRect(rRef);
	}

	GetDlgItem(IDC_CEB_FIP_AGGRE_CMB)->ShowWindow(nCode == TDMT_CODE_CEB_2010); 
	GetDlgItem(IDC_CMD_HCRP_STATIC17)->ShowWindow(nCode == TDMT_CODE_CEB_2010); 
		
	GetDlgItem(IDCANCEL)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(14);
	CDlgUtil::CtrlMoveDistY(this, m_buttongroup, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(10);

	MoveWindow(r);

	UpdateData(FALSE);
}

void CCMHcrpDlg::OnHcrpSelChangeType()
{
	UpdateData(TRUE);
	DWORD nCode;
	CDlgUtil::CobxGetItemDataByCurSel(m_CobxCode, nCode);

	// Modify Elasticity Modulus /////////////////////////////////
	//////////////////////////////////////////////////////////////
	if(m_type == 0)
	{		
		if (nCode == 1)
			CDlgUtil::CtrlEnableDisable(this, m_acishrinkgroup, FALSE);
		else if (nCode == 4)
		{
			CDlgUtil::CtrlEnableDisable(this,m_udefgroup,FALSE);
			CDlgUtil::CtrlEnableDisable(this,m_udefcrp,TRUE);
			ManUDefCtrls();
		}
		else
		{
			CDlgUtil::CtrlEnableDisable(this, m_cebshrinkgroup, FALSE);
			CDlgUtil::CtrlEnableDisable(this, m_aCebshrinkgroup, FALSE);
		}
	}
	else if(m_type == 1)
	{
		if (nCode == 1)
			CDlgUtil::CtrlEnableDisable(this, m_acishrinkgroup, TRUE);
		else if (nCode == 4)
		{
			CDlgUtil::CtrlEnableDisable(this,m_udefgroup,FALSE);
			CDlgUtil::CtrlEnableDisable(this,m_udefshr,TRUE); 
			ManUDefCtrls();
		}
		else
		{
			CDlgUtil::CtrlEnableDisable(this, m_cebshrinkgroup, TRUE);
			CDlgUtil::CtrlEnableDisable(this, m_aCebshrinkgroup, TRUE);
		}
	}
	else if(m_type == 2)
	{		
		if (nCode == 1)
			CDlgUtil::CtrlEnableDisable(this, m_acishrinkgroup, TRUE);
		else if (nCode == 4)
		{
			CDlgUtil::CtrlEnableDisable(this,m_udefgroup,TRUE);  
			ManUDefCtrls();
		}
		else
		{
			CDlgUtil::CtrlEnableDisable(this, m_cebshrinkgroup, TRUE);
			CDlgUtil::CtrlEnableDisable(this, m_aCebshrinkgroup, TRUE);
		}
	}
}

class TempClass : public CObject
{

};

void CCMHcrpDlg::OnCmdUdefAddCfuncButton() 
{
	CMap<int, int, double, double> AgeMap;
	CStringArray Contents;
	double dAge = 0.0;
	DWORD ItemData;

	int nCFunc = m_wndUdefFList.GetItemCount();

	int nIndex = 0;
	for(nIndex = 0 ; nIndex < nCFunc ; nIndex++)
	{
		CDlgUtil::GetListItem(&m_wndUdefFList,nIndex,Contents,ItemData);
		
		CStrParser::GetFloatNumber(Contents[1],dAge);
		AgeMap.SetAt(nIndex,dAge);
	}

	T_TDMF_K rKey;
	T_TDMF_D rData;
	TempClass Temp;
	TempClass* pTemp;
	pTemp = &Temp;
	m_CobxCreep.GetSelectedTdmf(rKey);

	if(m_pDoc->m_pAttrCtrl->GetTdmf(rKey,rData))
	{
		CString strTemp, strAge;
		double dAgeCur = 0.0;
		double dAgeOld = 0.0;
		BOOL bNewData = TRUE;
		CStringArray Contents;
		strTemp = rData.MatlFuncName;
		Contents.Add(strTemp);
		this->m_wndLoadAge.GetWindowText(strAge);
		if(CStrParser::GetFloatNumber(strAge,dAgeCur))
		{
			Contents.Add(strAge);

			for(int i=0; i<nIndex; i++)
			{
				AgeMap.Lookup(i,dAgeOld);

				if(dAgeCur == dAgeOld)
				{
					bNewData = FALSE;
					break;
				}
			}

			if(bNewData) CDlgUtil::SetListItem(&m_wndUdefFList,m_wndUdefFList.GetItemCount(),Contents,(DWORD)rKey);
		}
	}


//   CMap<int, int, int ,int> AgeMap;
//   CStringArray Contents;      
//   int nAge;
//   DWORD ItemData;
//   
//   int nCFunc = m_wndUdefFList.GetItemCount();
// 
//   for(int i = 0 ; i < nCFunc ; i++)
//   {
//     CDlgUtil::GetListItem(&m_wndUdefFList,i,Contents,ItemData);
//     
//     CStrParser::GetINumber(Contents[1],nAge);
//     AgeMap.SetAt(nAge,nAge);
//   }
// 
//   T_TDMF_K rKey  ;
//   T_TDMF_D rData ;
//   TempClass Temp;
//   TempClass* pTemp;
//   pTemp = &Temp;
// 	 m_CobxCreep.GetSelectedTdmf(rKey);
// 
//   if(m_pDoc->m_pAttrCtrl->GetTdmf(rKey,rData))
//   {
//     CString      strTemp, strAge;
//     int nAge,ITemp;
//     CStringArray Contents;
//     strTemp = rData.MatlFuncName;
//     Contents.Add(strTemp);
//     this->m_wndLoadAge.GetWindowText(strAge);
//     if(CStrParser::GetINumber(strAge,nAge))
//     {
//       Contents.Add(strAge);
// 
//       if(!AgeMap.Lookup(nAge,ITemp))
//         CDlgUtil::SetListItem(&m_wndUdefFList,m_wndUdefFList.GetItemCount(),Contents,(DWORD)rKey);
//     }
//   }
}


class TempTdmtO : public CObject
{
public:
	T_TDMT_BASE m_TdmtBase;
};

void CCMHcrpDlg::OnCmdUdefDelCfuncButton() 
{
	CMapStringToOb StrMap;
	//CArray<T_TDMT_BASE,T_TDMT_BASE&> arTdmtBase;
	CStringArray ListString;
	
	T_TDMF_D rData ;
	T_TDMT_BASE TdmtBase;
	DWORD ItemData;

	CString      _strTemp;
	TCHAR TBuffer[1000];
	
	TempTdmtO TTdmtO;
	TempTdmtO * pTTdmtO;
	pTTdmtO = &TTdmtO;

	CStringArray Contents;      
	int nCFunc = m_wndUdefFList.GetItemCount();

	for(int I = 0 ; I < nCFunc ; I++)
	{
		CDlgUtil::GetListItem(&m_wndUdefFList,I,Contents,ItemData);
		CString TempStr = Contents.GetAt(1);
		CStrParser::GetFloatNumber(TempStr,TdmtBase.dLoadingAge);
		TdmtBase.CreepFuncKey = (T_TDMF_K)ItemData;
 
		Contents[0].TrimLeft(_T("\t "));
		Contents[0].TrimRight(_T("\t "));
		Contents[1].TrimLeft(_T("\t "));
		Contents[1].TrimRight(_T("\t "));
		_strTemp = Contents[0] + Contents[1];
		memcpy(TBuffer,_strTemp.GetBuffer(_strTemp.GetLength()+1),_strTemp.GetLength()+1);

		pTTdmtO = new TempTdmtO;
		pTTdmtO->m_TdmtBase.CreepFuncKey = TdmtBase.CreepFuncKey;
		pTTdmtO->m_TdmtBase.dLoadingAge = TdmtBase.dLoadingAge;

		StrMap.SetAt(TBuffer,(CObject*&)pTTdmtO);

		ListString.Add(_strTemp);
	}
	
	CArray<int, int> arIRet;
	CDlgUtil::GetListItemByMask(&m_wndUdefFList,arIRet,LVIS_SELECTED);
	nCFunc = arIRet.GetSize();
	
	for(int I = 0 ; I < nCFunc ; I++)
	{
		CDlgUtil::GetListItem(&m_wndUdefFList,arIRet[I],Contents,ItemData);
		Contents[0].TrimLeft(_T("\t "));
		Contents[0].TrimRight(_T("\t "));
		Contents[1].TrimLeft(_T("\t "));
		Contents[1].TrimRight(_T("\t "));
		_strTemp = Contents[0] + Contents[1];
		memcpy(TBuffer,_strTemp.GetBuffer(_strTemp.GetLength()+1),_strTemp.GetLength()+1);
		

		if(StrMap.Lookup(TBuffer,(CObject *&)pTTdmtO))
		{
			StrMap.RemoveKey(TBuffer);
			delete pTTdmtO;
		}
	}

	m_wndUdefFList.DeleteAllItems();
	int nListItem = ListString.GetSize();
	for( int i = 0 ; i < nListItem ;i++)
	{
		if(StrMap.Lookup(ListString[i],(CObject*&)pTTdmtO))
		{
			Contents.RemoveAll();
			if(m_pDoc->m_pAttrCtrl->GetTdmf(pTTdmtO->m_TdmtBase.CreepFuncKey,rData))
			{
				Contents.Add(rData.MatlFuncName);
				_strTemp.Format(_T("%g"),pTTdmtO->m_TdmtBase.dLoadingAge);
				Contents.Add(_strTemp);
			}
			
			CDlgUtil::SetListItem(&m_wndUdefFList,m_wndUdefFList.GetItemCount(),Contents,
														(DWORD)pTTdmtO->m_TdmtBase.CreepFuncKey);
			delete pTTdmtO;
		}
	}
}


void CCMHcrpDlg::OnCmdUdefModifyCfuncButton() 
{
	CMap<int, int, double, double> AgeMap;
	CStringArray Contents;
	double dAge = 0.0;
	DWORD ItemData;

	int nCFunc = m_wndUdefFList.GetItemCount();

	int nIndex = 0;
	for(nIndex = 0 ; nIndex < nCFunc ; nIndex++)
	{
		CDlgUtil::GetListItem(&m_wndUdefFList,nIndex,Contents,ItemData);
		
		CStrParser::GetFloatNumber(Contents[1],dAge);
		AgeMap.SetAt(nIndex,dAge);
	}

	CArray<int, int> arIRet;
	CDlgUtil::GetListItemByMask(&m_wndUdefFList,arIRet,LVIS_SELECTED);
	nCFunc = arIRet.GetSize();

	if(nCFunc > 1)
	{
		MessageBox(_LS(IDS_CMD0417__Multi_selecting_is_not_allowed__),_LS(IDS_CMD0417__Warning));
		return;
	}

	if(nCFunc <= 0)
	{
		MessageBox(_LS(IDS_CMD0417__Must_be_exist_selected_item__),_LS(IDS_CMD0417__Warning));
		return;
	}

	double dSelectedAge = 0.0;
	double dSelectedAgeTmp = 0.0;
	CDlgUtil::GetListItem(&m_wndUdefFList,arIRet[0],Contents,ItemData);
	CStrParser::GetFloatNumber(Contents[1],dSelectedAge);

	for(int j=0; j<nIndex; j++)
	{
		AgeMap.Lookup(j,dSelectedAgeTmp);

		if(dSelectedAge == dSelectedAgeTmp)
		{
			AgeMap.RemoveKey(dSelectedAge);   
			break;
		}
	}
	
	T_TDMF_K rKey   ;
	T_TDMF_D rData  ;
	TempClass Temp  ;
	TempClass* pTemp;
	pTemp = &Temp;
	m_CobxCreep.GetSelectedTdmf(rKey);

	if(m_pDoc->m_pAttrCtrl->GetTdmf(rKey,rData))
	{
		CString strTemp, strAge;
		double dAgeCur = 0.0;
		double dAgeOld = 0.0;
		BOOL bModifyData = TRUE;
		CStringArray Contents;
		strTemp = rData.MatlFuncName;
		Contents.Add(strTemp);
		this->m_wndLoadAge.GetWindowText(strAge);
		if(CStrParser::GetFloatNumber(strAge,dAgeCur))
		{
			Contents.Add(strAge);

			for(int i=0; i<nIndex; i++)
			{
				AgeMap.Lookup(i,dAgeOld);

				if(dAgeCur == dAgeOld)
				{
					bModifyData = FALSE;
					break;
				}
			}

			if(bModifyData) CDlgUtil::SetListItem(&m_wndUdefFList,arIRet[0],Contents,(DWORD)rKey);
		}
	}


//   CMap<int, int, int ,int> AgeMap;
//   CStringArray Contents;      
//   int nAge;
//   DWORD ItemData;
//   int nCFunc = m_wndUdefFList.GetItemCount();
// 
//   for(int i = 0 ; i < nCFunc ; i++)
//   {
//     CDlgUtil::GetListItem(&m_wndUdefFList,i,Contents,ItemData);
//     
//     CStrParser::GetINumber(Contents[1],nAge);
//     AgeMap.SetAt(nAge,nAge);
//   }
// 
//   CArray<int, int> arIRet;
//   CDlgUtil::GetListItemByMask(&m_wndUdefFList,arIRet,LVIS_SELECTED);
//   nCFunc = arIRet.GetSize();
// 
//   if(nCFunc > 1)
//   {
//     MessageBox(_LS(IDS_CMD0417__Multi_selecting_is_not_allowed__)),_LS(IDS_CMD0417__Warning);
//     return;
//   }
// 
//   if(nCFunc <= 0)
//   {
//     MessageBox(_LS(IDS_CMD0417__Must_be_exist_selected_item__)),_LS(IDS_CMD0417__Warning);
//     return;
//   }
// 
//   int SelectedAge;
//   CDlgUtil::GetListItem(&m_wndUdefFList,arIRet[0],Contents,ItemData);
//   CStrParser::GetINumber(Contents[1],SelectedAge);
// 
//   AgeMap.RemoveKey(SelectedAge);
// 
//   
//   T_TDMF_K rKey   ;
//   T_TDMF_D rData  ;
//   TempClass Temp  ;
//   TempClass* pTemp;
//   pTemp = &Temp;
// 	m_CobxCreep.GetSelectedTdmf(rKey);
// 
//   if(m_pDoc->m_pAttrCtrl->GetTdmf(rKey,rData))
//   {
//     CString      strTemp, strAge;
//     int nAge,ITemp;
//     CStringArray Contents;
//     strTemp = rData.MatlFuncName;
//     Contents.Add(strTemp);
//     this->m_wndLoadAge.GetWindowText(strAge);
//     if(CStrParser::GetINumber(strAge,nAge))
//     {
//       Contents.Add(strAge);
// 
//       //int nContents = m_wndUdefFList.GetItemCount();
//       if(!AgeMap.Lookup(nAge,ITemp))
//       {
//         //m_wndUdefFList.DeleteItem(arIRet[0]);
//         CDlgUtil::SetListItem(&m_wndUdefFList,arIRet[0],Contents,(DWORD)rKey);
//       }
//     }
//   }
}

void CCMHcrpDlg::OnCmdHcrpRemoveBtn() 
{
	// TODO: Add your control notification handler code here
	if(m_pDoc->m_pDataCtrl->DelHcrp())
	{
		CDialogMove::OnOK();
	}
}

void CCMHcrpDlg::OnSSTR() 
{
	UpdateData(TRUE);
	CArray<UINT, UINT> ar;
	ar.Add(IDC_CMD_UDEF_SSTR_COMBO);
	ar.Add(IDC_CMD_UDEF_SSTR_BUTTON);
	CDlgUtil::CtrlEnableDisable(this,ar,m_bSSTR);
}

void CCMHcrpDlg::ManUDefCtrls()
{

	if(m_type == 0) // Creep
	{    
		m_bSSTR = FALSE;        
	}
	UpdateData(FALSE);

	CArray<UINT, UINT> ar;

	ar.RemoveAll();
	ar.Add(IDC_CMD_UDEF_SSTR_COMBO);
	ar.Add(IDC_CMD_UDEF_SSTR_BUTTON);
	CDlgUtil::CtrlEnableDisable(this,ar,m_bSSTR);
}

void CCMHcrpDlg::OnCreepFunctionButton() 
{
	CCMTimeDepMatFuncDlg Dlg;
	Dlg.SetCurPage(0);
	Dlg.DoModal();
}

void CCMHcrpDlg::OnSSTRFuncButton() 
{
	CCMTimeDepMatFuncDlg Dlg;
	Dlg.SetCurPage(1);
	Dlg.DoModal();
	
}

void CCMHcrpDlg::OnDeltaposACI_RELA(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	//IDC_CMD_HCRP_ACI_RELA [40 , 99]
 
	CDlgUtil::SetSpinValue(this,IDC_CMD_HCRP_ACI_RELA,40,99,pNMHDR,pResult);
	
	*pResult = 0;
}

void CCMHcrpDlg::OnDeltaposACI_FINE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	// IDC_CMD_HCRP_ACI_FINE[0 , 100]
	CDlgUtil::SetSpinValue(this,IDC_CMD_HCRP_ACI_FINE,0,100,pNMHDR,pResult);
	
	*pResult = 0;
}

void CCMHcrpDlg::OnDeltaposACI_AIR(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	// IDC_CMD_HCRP_ACI_AIR [0 , 100]
	CDlgUtil::SetSpinValue(this,IDC_CMD_HCRP_ACI_AIR,0,100,pNMHDR,pResult);
	
	*pResult = 0;
}

void CCMHcrpDlg::OnDeltaposACI_AGE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	// IDC_CMD_HCRP_ACI_AGE [0 , 10000]
	CDlgUtil::SetSpinValue(this,IDC_CMD_HCRP_ACI_AGE,0,10000,pNMHDR,pResult);
	*pResult = 0;
}

void CCMHcrpDlg::OnDeltaposCEB_AGE(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	// IDC_CMD_HCRP_CEB_AGE [0 , 100]
	CDlgUtil::SetSpinValue(this,IDC_CMD_HCRP_CEB_AGE,0,100,pNMHDR,pResult);
	
	*pResult = 0;
}

void CCMHcrpDlg::OnDeltaposCEB_RELA(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	// IDC_CMD_HCRP_CEB_RELA [30 , 99]
	CDlgUtil::SetSpinValue(this,IDC_CMD_HCRP_CEB_RELA,30,99,pNMHDR,pResult);
	
	*pResult = 0;
}

void CCMHcrpDlg::OnDeltaposEMOD_N1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	// IDC_CMD_MEM_N1[ 0 , 10000]
	CDlgUtil::SetSpinValue(this,IDC_CMD_MEM_N1,0,10000,pNMHDR,pResult);
	
	*pResult = 0;
}

void CCMHcrpDlg::OnDeltaposEMOD_N2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	// IDC_CMD_MEM_N2 [ 0 , 10000]
	CDlgUtil::SetSpinValue(this,IDC_CMD_MEM_N2,0,10000,pNMHDR,pResult);
	
	*pResult = 0;
}

void CCMHcrpDlg::OnDeltaposCmdCombEffctRatioSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_CMD_COMB_EFFCT_RATIO,0,100,pNMHDR,pResult);
	
	*pResult = 0;
}

void CCMHcrpDlg::OnDeltaposCmdCombReltHumidSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_CMD_COMB_RELT_HUMID,40,99,pNMHDR,pResult);
	
	*pResult = 0;
}

void CCMHcrpDlg::OnDeltaposCmdPcaRltvHumidSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_CMD_PCA_RLTV_HUMID,40,99,pNMHDR,pResult);
	
	*pResult = 0;
}

void CCMHcrpDlg::OnDeltaposCmdPcaRnfcRatioSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_CMD_PCA_RNFC_RATIO,0,100,pNMHDR,pResult);
	
	*pResult = 0;
}

void CCMHcrpDlg::OnDeltaposCmdPcaSrkgStrnSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_CMD_PCA_SRKG_STRN,500,800,pNMHDR,pResult);

	*pResult = 0;
}

void CCMHcrpDlg::OnDeltaposCmdPcaCreepStrnSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_CMD_PCA_CREEP_STRN,3,5,pNMHDR,pResult);

	*pResult = 0;
}

void CCMHcrpDlg::OnDeltaposCmdHcrpJapanRelaSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_CMD_HCRP_JAPAN_RELA,40,90,pNMHDR,pResult);
	
	*pResult = 0;
}

void CCMHcrpDlg::OnDeltaposCmdHcrpJapanAgeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this,IDC_CMD_HCRP_JAPAN_AGE,0,100,pNMHDR,pResult);
	
	*pResult = 0;
}

// China Spin
void CCMHcrpDlg::OnDeltaposCmdHcrpChinaRelaSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this, IDC_CMD_HCRP_CHINA_RELA,40,90, pNMHDR, pResult);	
	*pResult = 0;
}

void CCMHcrpDlg::OnDeltaposCmdHcrpChinaAgeSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this, IDC_CMD_HCRP_CHINA_AGE,0,100, pNMHDR, pResult);	
	
	*pResult = 0;
}

// JSCE spin skn
void CCMHcrpDlg::OnDeltaposCmdJsceRhSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this, IDC_CMD_JSCE_RH_EDT,45,80, pNMHDR, pResult);

	*pResult = 0;
}

void CCMHcrpDlg::OnDeltaposCmdJsceAcSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this, IDC_CMD_JSCE_AC_EDT,1,100000, pNMHDR, pResult);
	
	*pResult = 0;
}

void CCMHcrpDlg::OnCmdEtcHcrpShow() 
{
	SetDlg2Data();

	T_TDMT_D tdmt_d = m_data;

	if(m_pDoc->m_pEditData->CheckTdmtData(tdmt_d,FALSE,TRUE))
	{
		if (tdmt_d.COMMON.nCode == TDMT_CODE_CHINA_CHJTG_T_D65_2015)
		{
			if (!tdmt_d.CHJTG04.bEqv) return;
		}

  	    CMTdmfItemShowDlg dlg;
		dlg.SetTdmt(tdmt_d);
		dlg.DoModal();
	}
}

void CCMHcrpDlg::OnCmdAciAciRd() 
{
	CDlgUtil::CtrlShowHide(this, m_aciacigroup, TRUE);
	CDlgUtil::CtrlShowHide(this, m_aciUsergroup,FALSE);
}

void CCMHcrpDlg::OnCmdAciUserRd() 
{
	CDlgUtil::CtrlShowHide(this, m_aciacigroup, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aciUsergroup,TRUE);

	CRect rRef;
	CRect rToMove;
	int nDistY;

	GetDlgItem(IDC_CMD_HCRP_STATIC5)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_HCRP_STATIC101)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aciUsergroup, nDistY);
}


void CCMHcrpDlg::OnCmdCombPcaEffectChk() 
{
	UpdateData(TRUE);

	CWnd* pWnd[3];
	pWnd[0] = GetDlgItem(IDC_CMD_COMB_EFFCT_RATIO);
	pWnd[1] = GetDlgItem(IDC_CMD_COMB_EFFCT_RATIO);
	pWnd[2] = GetDlgItem(IDC_CMD_COMB_EFFCT_MODUL);
	ASSERT(pWnd[0] && pWnd[1] && pWnd[2]);

	for(int i=0; i<3; i++) pWnd[i]->EnableWindow(m_bPCAEffect);
}

void CCMHcrpDlg::EnableJapanRelaCtrl(BOOL bEnable)
{
	CWnd* pWnd[1];
	pWnd[0] = GetDlgItem(IDC_CMD_HCRP_JAPAN_RELA);      ASSERT(pWnd[0]);

	for(int i=0; i<1; i++) pWnd[i]->EnableWindow(bEnable);
}

void CCMHcrpDlg::OnCmdHcrpJapanRelRd1() 
{
	EnableJapanRelaCtrl(FALSE);	
}

void CCMHcrpDlg::OnCmdHcrpJapanRelRd2() 
{
	EnableJapanRelaCtrl(TRUE);		
}

void CCMHcrpDlg::EnableChinaRelaCtrl(BOOL bEnable)
{
	CWnd* pWnd[1];
	pWnd[0] = GetDlgItem(IDC_CMD_HCRP_CHINA_RELA); ASSERT(pWnd[0]);
	

	for(int i=0; i<1; i++) pWnd[i]->EnableWindow(bEnable);
}

void CCMHcrpDlg::OnCmdHcrpChinaRelRd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	EnableChinaRelaCtrl(m_chinaRelradio);
}

void CCMHcrpDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	SetDlg2Data();
	
	BOOL bDBSuccess;
	if(m_nOptionForTimeDep == 0)  bDBSuccess = m_pDoc->m_pDataCtrl->AddTdmt(m_data);
	else
	{
		T_TDMT_D OldTdmt;
		m_pDoc->m_pAttrCtrl->GetTdmt(m_strOldMatName, OldTdmt);
		m_data.nSeq = OldTdmt.nSeq;
		bDBSuccess = m_pDoc->m_pDataCtrl->ModifyTdmt(m_strOldMatName,m_data);
	}

	if(bDBSuccess) GetDlgItem(IDC_CMD_TDEPNAME_EDIT)->SetWindowText(_T(""));		
}


void CCMHcrpDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pJapanDlg) 
	{
		m_pJapanDlg->DestroyWindow();
		delete m_pJapanDlg;
		m_pJapanDlg = 0;
	}

	if (m_pChJTG04Dlg) 
	{
		m_pChJTG04Dlg->DestroyWindow();
		delete m_pChJTG04Dlg;
		m_pChJTG04Dlg = 0;
	}

	if (m_pAASHTODlg)
	{
		m_pAASHTODlg->DestroyWindow();
		delete m_pAASHTODlg;
		m_pAASHTODlg = 0;
	}

	if (m_pEuropeanDlg)
	{
		m_pEuropeanDlg->DestroyWindow();
		delete m_pEuropeanDlg;
		m_pEuropeanDlg = 0;
	}

	if (m_pIndiaDlg)
	{
		m_pIndiaDlg->DestroyWindow();
		delete m_pIndiaDlg;
		m_pIndiaDlg = 0;
	}

	if (m_pASDlg)
	{
		m_pASDlg->DestroyWindow();
		delete m_pASDlg;
		m_pASDlg = 0;
	}

	if (m_pIndia2011Dlg)
	{
		m_pIndia2011Dlg->DestroyWindow();
		delete m_pIndia2011Dlg;
		m_pIndia2011Dlg = 0;
	}

	if (m_pRusDlg)
	{
		m_pRusDlg->DestroyWindow();
		delete m_pRusDlg;
		m_pRusDlg = 0;
	}

	if (m_pChJTG16Dlg)
	{
		m_pChJTG16Dlg->DestroyWindow();
		delete m_pChJTG16Dlg;
		m_pChJTG16Dlg = 0;
	}

	if (m_pNZDlg)
	{
		m_pNZDlg->DestroyWindow();
		delete m_pNZDlg;
		m_pNZDlg = 0;
	}

	if (m_pKDS2016Dlg)
	{
		m_pKDS2016Dlg->DestroyWindow();
		delete m_pKDS2016Dlg;
		m_pKDS2016Dlg = 0;
	}

	if (m_pJSCE07Dlg)
	{
		m_pJSCE07Dlg->DestroyWindow();
		delete m_pJSCE07Dlg;
		m_pJSCE07Dlg = 0;
	}

	if (m_pJSCE12Dlg)
	{
		m_pJSCE12Dlg->DestroyWindow();
		delete m_pJSCE12Dlg;
		m_pJSCE12Dlg = 0;
	}

	if (m_pIndia2011_2020Dlg)
	{
		m_pIndia2011_2020Dlg->DestroyWindow();
		delete m_pIndia2011_2020Dlg;
		m_pIndia2011_2020Dlg = 0;
	}

	if (m_pABNT_NBR_6118Dlg)
	{
		m_pABNT_NBR_6118Dlg->DestroyWindow();
		delete m_pABNT_NBR_6118Dlg;
		m_pABNT_NBR_6118Dlg = 0;
	}

	CDialogMove::PostNcDestroy();
}
