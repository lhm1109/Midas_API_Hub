// DgnConMatSLTEDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConMatSLTEDlg.h"
#include "DgnDataCtrl.h"
#include "DgnStruct.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\DBCodeDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConMatSLTEDlg dialog

CDgnConMatSLTEDlg::CDgnConMatSLTEDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConMatSLTEDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConMatSLTEDlg)
	m_dSTn = 0.0;
	m_dLTn = 0.0;
	//}}AFX_DATA_INIT
	m_dUnitElast = 0.0;
}


void CDgnConMatSLTEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConMatSLTEDlg)
	DDX_Text(pDX, IDC_DGN_CON_STEc, m_dSTn);
	DDX_Text(pDX, IDC_DGN_CON_LTEc, m_dLTn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnConMatSLTEDlg, CInternationalDlg)
	//{{AFX_MSG_MAP(CDgnConMatSLTEDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConMatSLTEDlg message handlers

BOOL CDgnConMatSLTEDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	Set_Unit();     // Code & Current Unit.
	Init_Unit();    // Set Unit String.
	Init_EcData();  // Set Ec both Short term and Long term.  
	
	UpdateData(FALSE);

	return TRUE;
}

// Set Code Unit.
void CDgnConMatSLTEDlg::Set_Unit()
{
	const CString& strCode = CDBLib::GetConCodeName();    
    int iCodeUnitL = 0;
    int iCodeUnitF = 0;    
	if(strCode==CONCODE_ACI318_89)			{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_IN;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_LBF;}
	else if(strCode==CONCODE_ACI318_95)		{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_IN;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_LBF;}
	else if(strCode==CONCODE_ACI318_99)		{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_IN;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_LBF;}
	else if(strCode==CONCODE_ACI318_02)		{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_IN;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_LBF;}
	else if(strCode==CONCODE_ACI318_05)		{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_IN;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_LBF;}
	else if(strCode==CONCODE_ACI318_08)     { iCodeUnitL = D_UNITSYS_LENGTH_INDEX_IN;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_LBF;} // Add by GAY. PMS:4450. ('12.03.12)
	else if(strCode==CONCODE_ACI318_11)     { iCodeUnitL = D_UNITSYS_LENGTH_INDEX_IN;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_LBF;} // Add by GAY. PMS:4450. ('12.03.12)
	else if(strCode==CONCODE_ACI318_14)     { iCodeUnitL = D_UNITSYS_LENGTH_INDEX_IN;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_LBF;} 
	else if(strCode==CONCODE_ACI318M_14)    { iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N  ;} 
    else if(strCode==CONCODE_ACI318_19)     { iCodeUnitL = D_UNITSYS_LENGTH_INDEX_IN;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_LBF; }
    else if(strCode==CONCODE_ACI318M_19)    { iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N; }
	else if(strCode==CONCODE_ACI318_25)     { iCodeUnitL = D_UNITSYS_LENGTH_INDEX_IN;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_LBF; }
    else if(strCode==CONCODE_ACI318M_25)    { iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N; }
	else if(strCode==CONCODE_NSR_10)        { iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N  ;} // by GAY. PMS:5009. ('15.04.03).
    else if(strCode==CONCODE_NSCP_2015 )    { iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N; }
	else if(strCode==CONCODE_NTC_DCEC_2017) { iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N;} 
	else if(strCode==CONCODE_NTC_DCEC_2023) { iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N; }
	else if(strCode==CONCODE_AIK_USD94)		{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_CM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_KG ;}
	else if(strCode==CONCODE_KSCE_USD96)	{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_CM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_KG ;}
	else if(strCode==CONCODE_KCI_USD99)		{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_CM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_KG ;}
	else if(strCode==CONCODE_KCI_USD03)		{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N  ;}
	else if(strCode==CONCODE_KCI_USD07)		{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N  ;}
	else if(strCode==CONCODE_KCI_USD12)		{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N  ;}
	else if(strCode==CONCODE_TWN_USD92)		{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_CM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_KG ;}
	else if(strCode==CONCODE_TWN_USD100)	{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_CM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_KG ;}
    else if(strCode==CONCODE_TWN_USD112)	{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_CM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_KG ;}
	else if(strCode==CONCODE_AIK_WSD2K)		{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_CM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_KG ;}
	else if(strCode==CONCODE_AIJ_WSD99)		{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N  ;}
	else if(strCode==CONCODE_BS8110_97)		{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N  ;}
	else if(strCode==CONCODE_EC2)			{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N  ;}
	else if(strCode==CONCODE_EC2_04)	    { iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N  ;} // Coded by Seungjun ('20070613) MNet:No.2313.
	else if(strCode==CONCODE_EC2_2_05)      { iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N  ;}
	else if(strCode==CONCODE_BS5400_90)     { iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N  ;}
	else if(strCode==CONCODE_TMH07_89)     { iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N  ;}
	else if(strCode==CONCODE_IS456_2000)	{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N  ;}
	else if(strCode==CONCODE_GB50010_02)	{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N  ;}
	else if(strCode==CONCODE_CSA_A23_3_94)	{ iCodeUnitL = D_UNITSYS_LENGTH_INDEX_MM;  iCodeUnitF = D_UNITSYS_FORCE_INDEX_N  ;}    
	else	ASSERT(0);

	// Code Unit.
	double dCodeUnitElast=Covert_Unit(iCodeUnitL, iCodeUnitF);
	
	// Current Unit.
	// Remember that you must change Units by Code for getting data.
	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	double dCurrUnitElast=Covert_Unit(CurIndex.nBase_Length, CurIndex.nBase_Force);

	// Unit Ratio.
	m_dUnitElast = dCodeUnitElast/dCurrUnitElast;
}

// Set Unit String.
void CDgnConMatSLTEDlg::Init_Unit()
{
	//m_STEcUnit.SetUnitType(D_UNITSYS_BASE_ELAST);
	//m_LTEcUnit.SetUnitType(D_UNITSYS_BASE_ELAST);
}

// Set Ec both Short term and Long term.
void CDgnConMatSLTEDlg::Init_EcData()
{
	T_MATD_D rData;
	rData.Initialize();
	  
	if(m_pDoc->m_pAttrCtrl->GetMatlDesign(m_iMatdKey, rData))
	{
		if(rData.bServCheck)
		{
			if(rData.dLongTerm > 0.0 || rData.dShortTerm > 0.0)
			{
				m_dSTn = rData.dShortTerm;
				m_dLTn = rData.dLongTerm;
			}
			else
				Set_InitByCode(rData);
		}
		else
				Set_InitByCode(rData);
	}	
	else
	{
		ASSERT(0);
	}
}

void CDgnConMatSLTEDlg::Set_InitByCode(T_MATD_D rData)
{
	double dfc = rData.Data1.Design.C_fc; 
	double dFy = rData.Data1.Design.S_Fy;
	dfc = Get_Fc(rData.Data1.CodeName, dfc) / m_dUnitElast; // Code Unit.
	double dEc = Get_Ec(dfc);
	double dEs = Get_Es(dFy);
	
	if(dEc == 0.0)  
		m_dSTn = 0.0;
	else
		m_dSTn = dEs/dEc;
	m_dLTn = m_dSTn > 0.0 ? m_dSTn * 2.0 : 0.0;          // Default : LTEc = STEc / 2.

	//m_dSTEc = Get_Ec(dfc) * m_dUnitElast;                   // Current Unit.
	//m_dLTEc = m_dSTEc > 0.0 ? m_dSTEc / 2.0 : 0.0;          // Default : LTEc = STEc / 2.
}
	
// Calculate Multiplier by unit.
double CDgnConMatSLTEDlg::Covert_Unit(int iLengthUnit, int iForceUnit)
{
	double dFlen=0.0;
	if(iLengthUnit==D_UNITSYS_LENGTH_INDEX_M)		dFlen = 1000.0;
	else if(iLengthUnit==D_UNITSYS_LENGTH_INDEX_CM)	dFlen = 10.0;
	else if(iLengthUnit==D_UNITSYS_LENGTH_INDEX_MM)	dFlen = 1.0;
	else if(iLengthUnit==D_UNITSYS_LENGTH_INDEX_IN)	dFlen = 25.4;
	else if(iLengthUnit==D_UNITSYS_LENGTH_INDEX_FT)	dFlen = 25.4*12.0;
	else	ASSERT(0);

	double dFfor=0.0;
	if(iForceUnit==D_UNITSYS_FORCE_INDEX_KG)		dFfor = 9.80665;
	else if(iForceUnit==D_UNITSYS_FORCE_INDEX_TON)	dFfor = 9806.65;
	else if(iForceUnit==D_UNITSYS_FORCE_INDEX_N)	dFfor = 1.0;
	else if(iForceUnit==D_UNITSYS_FORCE_INDEX_KN)	dFfor = 1000.0;
	else if(iForceUnit==D_UNITSYS_FORCE_INDEX_LBF)	dFfor = 4.448223;
	else if(iForceUnit==D_UNITSYS_FORCE_INDEX_KIP)	dFfor = 4448.223;
	else	ASSERT(0);

	// Get Stress Factor.
	double dStrUnit = dFfor / pow(dFlen,2);

	return dStrUnit;
}

void CDgnConMatSLTEDlg::OnOK() 
{
	// TODO: Add extra validation here
	// 여기에 T_SLEL_D Data Add시키는 코딩 추가할 것.
	UpdateData(TRUE);

	if(m_dLTn < 0.0 || m_dSTn < 0.0)
	{
		AfxMessageBox(_T("Ec must be larger than 0.0."), MB_OK);
		return;
	}

	T_MATD_D rData;
	rData.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(m_iMatdKey, rData)) ASSERT(0);

	rData.bServCheck = TRUE;
	rData.dShortTerm = m_dSTn;
	rData.dLongTerm  = m_dLTn;

	if(!m_pDoc->m_pDataCtrl->AddMatd(m_iMatdKey, rData)) ASSERT(0);  

	CDialog::OnOK();
}

// Coded by Seungjun MNet:No.2393 ('20061110)
// Use only Eurocode2 - calculation of modulus of elasticity. Unit : N, mm.
double CDgnConMatSLTEDlg::Get_Ec(double dfc)
{
	const CString& strCode = CDBLib::GetConCodeName();
	double dEc = 0.0;	
    if ( strCode== CONCODE_EC2 )
    {
        dEc = 9.5E+03*pow(dfc+8.0, 1./3.0);
    }
    else if ( strCode==CONCODE_EC2_04 || strCode == CONCODE_EC2_2_05 )
    {
        dEc = 22.E+03*pow((dfc+8.0)/10., 0.3);
    }
	else if ( strCode== CONCODE_BS5400_90 )
	{
		ASSERT(0);
	}
	else if (strCode == CONCODE_TMH07_89)
	{
		ASSERT(0);
	}
	return dEc;
}

double CDgnConMatSLTEDlg::Get_Es(double dFy)
{
	const CString& strCode = CDBLib::GetConCodeName();
	    
    if ( strCode==CONCODE_ACI318_89 )  return 2.90*(1.0E+07);	// lb/in2.	
    if ( strCode==CONCODE_ACI318_95 )  return 2.90*(1.0E+07);	// lb/in2.
    if ( strCode==CONCODE_ACI318_99 )  return 2.90*(1.0E+07);	// lb/in2.
    if ( strCode==CONCODE_ACI318_02 )  return 2.90*(1.0E+07);	// lb/in2.
    if ( strCode==CONCODE_ACI318_05 )  return 2.90*(1.0E+07);	// lb/in2.
    if ( strCode==CONCODE_ACI318_08 )  return 2.90*(1.0E+07);	// lb/in2.
    if ( strCode==CONCODE_ACI318_11 )  return 2.90*(1.0E+07);	// lb/in2. See ACI318-11, 8.5.2.
    if ( strCode==CONCODE_ACI318_14 )  return 2.90*(1.0E+07);   // lb/in2 = psi.
    if ( strCode==CONCODE_ACI318M_14)  return 2.00*(1.0E+05);   // N/mm2 = MPa.
    if ( strCode==CONCODE_ACI318_19 )  return 2.90*(1.0E+07);   // lb/in2 = psi.
    if ( strCode==CONCODE_ACI318M_19)  return 2.00*(1.0E+05);   // N/mm2 = MPa.
	if ( strCode==CONCODE_ACI318_25 )  return 2.90*(1.0E+07);   // lb/in2 = psi.
    if ( strCode==CONCODE_ACI318M_25)  return 2.00*(1.0E+05);   // N/mm2 = MPa.
    if ( strCode==CONCODE_NSR_10 )	   return 2.00*(1.0E+05);	// N/mm2, See NSR-10, C.8.5.2.
    if ( strCode==CONCODE_NSCP_2015 )  return 2.00*(1.0E+05);   // N/mm2 = MPa.
	if ( strCode==CONCODE_NTC_DCEC_2017 )  return 2.00*(1.0E+05);   // N/mm2 = MPa.
	if ( strCode==CONCODE_NTC_DCEC_2023 )  return 2.00*(1.0E+05);   // N/mm2 = MPa.
    if ( strCode==CONCODE_AIK_USD94 )  return 2.10*(1.0E+06);	// kgf/cm2.
    if ( strCode==CONCODE_KSCE_USD96 ) return 2.04*(1.0E+06);	// kgf/cm2.
    if ( strCode==CONCODE_KCI_USD99 )  return 2.00*(1.0E+06);	// kgf/cm2.
    if ( strCode==CONCODE_KCI_USD03 )  return 2.00*(1.0E+05);	// N/mm2, See KCI-USD03 3.4.3(2) p.67.
    if ( strCode==CONCODE_KCI_USD07 )  return 2.00*(1.0E+05);	// N/mm2, See KCI-USD03 3.4.3(2) p.67.
    if ( strCode==CONCODE_KCI_USD12 )  return 2.00*(1.0E+05);	// N/mm2, See KCI-USD03 3.4.3(2) p.67.
    if ( strCode==CONCODE_TWN_USD92 )  return 2.04*(1.0E+06);	// kgf/cm2, See TWN-USD92 1.9.2 p.1-3.
    if ( strCode==CONCODE_TWN_USD100 ) return 2.04*(1.0E+06);	// kgf/cm2, See TWN-USD92 1.9.2 p.1-3.
    if ( strCode==CONCODE_TWN_USD112 ) return 2.04*(1.0E+06);	// kgf/cm2, See TWN-USD92 1.9.2 p.1-3.
    if ( strCode==CONCODE_AIK_WSD2K )  return 2.10*(1.0E+06);	// kgf/cm2.
    if ( strCode==CONCODE_AIJ_WSD99 )  return 2.05*(1.0E+05);	// N/mm2.
    if ( strCode==CONCODE_BS8110_97 )  return 2.00*(1.0E+05);	// N/mm2.
    if ( strCode==CONCODE_EC2 )		   return 2.00*(1.0E+05);	// N/mm2.
    if ( strCode==CONCODE_EC2_04 )	   return 2.00*(1.0E+05);	// N/mm2. // Coded by Seungjun ('20070613) MNet:No.2313.
    if ( strCode==CONCODE_EC2_2_05 )   return 2.00*(1.0E+05);	// N/mm2.
    if ( strCode==CONCODE_BS5400_90 )  return 2.00*(1.0E+05);	// N/mm2.
    if ( strCode==CONCODE_TMH07_89 )  return 2.00*(1.0E+05);	// N/mm2.
    if ( strCode==CONCODE_IS456_2000 ) return 2.00*(1.0E+05);	// N/mm2, See IS456:2000 5.6.3 p.15.
    if ( strCode==CONCODE_GB50010_02 )	  // N/mm2, See GB50010-02 4.2.4 p.21.
    {
        if ( fabs(dFy-235.0) < cDgn_Zero )	return 2.1*(1.0E+05);	// HPB235.
		else								return 2.0*(1.0E+05);	// Etc.
	}
    if ( strCode==CONCODE_CSA_A23_3_94 ) return 2.0*(1.0E+05);	// N/mm2, See CSA-A23.3-94 8.5.4.1 p.19.
    
    ASSERT(0);
	return 0.0;
}

// Get strength of concrete. Unit : N, mm.
double CDgnConMatSLTEDlg::Get_Fc(CString strMatlDB, double dfc)
{
	CString strDgnCode = CDBLib::GetConCodeName();
	// fc = cube or cylinder strength by Matl Code.
	double dfcUse=0.0;
	CString strMatlType= _T("C");
	if(!m_pDoc->m_pMatlDB->GetMatlType(strMatlDB,strMatlType))	return dfc;

	// Check Matl Code.
	BOOL bUseCubeMatl = CDBLib::IsConCubeMatlCode(strMatlDB);
	
	// Check Design Code.
	BOOL bUseCubeCode = CDBLib::IsConCubeDesignCode(strDgnCode);
	
	// Get fck or fcu.
	if(bUseCubeMatl!=bUseCubeCode)
	{
		if(bUseCubeMatl)	dfcUse = 0.8*dfc;	// Cylinder.
		else				dfcUse = dfc/0.8;	// Cube.
	}
	else	dfcUse = dfc;

	return dfcUse;
}

