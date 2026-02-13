// PrtKSCE_USD05.cpp: implementation of the CPrtKSCE_USD05 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "PrtKSCE_USD05.h"

#include "DgnKSCE_USD05.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_AnalysisResult.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrtKSCE_USD05::CPrtKSCE_USD05()
{
	m_pDoc=NULL;
	m_pDataCtrl=NULL;
	m_pForcCtrl=NULL;
	m_bPrintFlag=FALSE;

	m_arText.RemoveAll();
	m_StagTitleMap.RemoveAll();
	m_LcomTitleMap.RemoveAll();
	m_arElemK.RemoveAll();
	m_arStagK.RemoveAll();
	m_arLcomK.RemoveAll();

	m_amForcOrgLcom.RemoveAll();
	m_arElemMy[0].RemoveAll(); m_arElemMy[1].RemoveAll();
	m_arElemVz[0].RemoveAll(); m_arElemVz[1].RemoveAll();
	m_arElemMx[0].RemoveAll(); m_arElemMx[1].RemoveAll();

	m_TotalNum = 6;
	m_decimal = 2;

	m_strLength = _T("mm");
	m_strForce  = _T("kN");
	m_strArea   = _T("mm ~ ^2");  
	m_strAreaM  = _T("m^2");
	m_strStif   = _T("mm ~ ^4");
	m_strStifM  = _T("m^4");
	m_strMoment = _T("kN·m");
	m_strStress = _T("N/mm ~ ^2");
	m_strMpa    = _T("MPa");

	m_sTxArea   = _T("mm²");  
	m_sTxAreaM  = _T("m²");
	m_sTxSecM   = _T("mm³");
	m_sTxSecMM  = _T("m³");
	m_sTxStif   = _T("mm⁴");
	m_sTxStifM  = _T("m⁴");
	m_sTxStress = _T("N/mm²");
	
	m_strDegree = _T("˚");
	m_strRadian = _T("rad");

	m_iVciType = 2; // 1=자중고려함, 2=자중고려안함.
}

CPrtKSCE_USD05::~CPrtKSCE_USD05()
{
}

BOOL CPrtKSCE_USD05::Get_DetailTbarPos(ElemPairK ElemK)
{
	// Get Posx.
	T_POSX_D PosxD;
	PosxD.Initialize();
	if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD))	return FALSE;
	}
	else if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD))	return FALSE;
	}
	else ASSERT(0);

	BOOL bPrtI=FALSE, bPrtJ=FALSE;
	if(PosxD.iBarChk[0]==0)	return FALSE;											// NONE.
	else if(PosxD.iBarChk[0]==1)	{bPrtI=TRUE ; bPrtJ=FALSE;}	// I.
	else if(PosxD.iBarChk[0]==2)	{bPrtI=FALSE; bPrtJ=TRUE ;}	// J.
	else if(PosxD.iBarChk[0]==3)	{bPrtI=TRUE ; bPrtJ=TRUE ;}	// I&J.
	else	ASSERT(0);
	// Get Tbar.
	T_TBAR_D TbarD;
	TbarD.Initialize();
	if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetTbarData(ElemK,TbarD))	return FALSE;
	TbarD.TbarBase[0].bCHK = (TbarD.TbarBase[0].bCHK && bPrtI ? TRUE : FALSE);	// 0=I-TOP-MAX.
	TbarD.TbarBase[1].bCHK = FALSE;									                          	// 1=I-BOT-MAX.
	TbarD.TbarBase[2].bCHK = (TbarD.TbarBase[2].bCHK && bPrtJ ? TRUE : FALSE);	// 2=J-TOP-MAX.
	TbarD.TbarBase[3].bCHK = FALSE;									                         	  // 3=J-BOT-MAX.

	BOOL bExecutePrt = FALSE;
	for(int i=0; i<4; i++)
	{
		if(TbarD.TbarBase[i].bCHK) bExecutePrt = TRUE;
	}

	if(!bExecutePrt) return FALSE;

	// Print on Calculating.
	return m_pDataCtrl->Get_DgnKsce05Pointer()->Get_KsceTbar(ElemK,TbarD,bPrtI,bPrtJ,2);	// 2=Bot.
}

BOOL CPrtKSCE_USD05::Get_DetailTbarNeg(ElemPairK ElemK)
{
	// Get Posx.
	T_POSX_D PosxD;
	PosxD.Initialize();
	if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD))	return FALSE;
	}
	else if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD))	return FALSE;
	}
	else ASSERT(0);
	BOOL bPrtI=FALSE, bPrtJ=FALSE;
	if(PosxD.iBarChk[1]==0)	return FALSE;											// NONE.
	else if(PosxD.iBarChk[1]==1)	{bPrtI=TRUE ; bPrtJ=FALSE;}	// I.
	else if(PosxD.iBarChk[1]==2)	{bPrtI=FALSE; bPrtJ=TRUE ;}	// J.
	else if(PosxD.iBarChk[1]==3)	{bPrtI=TRUE ; bPrtJ=TRUE ;}	// I&J.
	else	ASSERT(0);
	// Get Tbar.
	T_TBAR_D TbarD;
	TbarD.Initialize();
	if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetTbarData(ElemK,TbarD))	return FALSE;
	TbarD.TbarBase[0].bCHK = FALSE;									          	                // 0=I-TOP-MAX.
	TbarD.TbarBase[1].bCHK = (TbarD.TbarBase[1].bCHK && bPrtI ? TRUE : FALSE);	// 1=I-BOT-MAX.
	TbarD.TbarBase[2].bCHK = FALSE;										                          // 2=J-TOP-MAX.
	TbarD.TbarBase[3].bCHK = (TbarD.TbarBase[3].bCHK && bPrtJ ? TRUE : FALSE);	// 3=J-BOT-MAX.

	BOOL bExecutePrt = FALSE;
	for(int i=0; i<4; i++)
	{
		if(TbarD.TbarBase[i].bCHK) bExecutePrt = TRUE;
	}

	if(!bExecutePrt) return FALSE;

	 // Print on Calculating.
	return m_pDataCtrl->Get_DgnKsce05Pointer()->Get_KsceTbar(ElemK,TbarD,bPrtI,bPrtJ,1);	// 1=Top.
}

BOOL CPrtKSCE_USD05::Get_DetailBdcyPos(ElemPairK ElemK, CMSExcel* pXL)
{
	// Get Posx.
	T_POSX_D PosxD;
	PosxD.Initialize();
	if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD))	return FALSE;
	}
	else if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD))	return FALSE;
	}
	else ASSERT(0);
	BOOL bPrtI=FALSE, bPrtJ=FALSE;
	if(PosxD.iMomChk[0]==0)	return FALSE;											// NONE.
	else if(PosxD.iMomChk[0]==1)	{bPrtI=TRUE ; bPrtJ=FALSE;}	// I.
	else if(PosxD.iMomChk[0]==2)	{bPrtI=FALSE; bPrtJ=TRUE ;}	// J.
	else if(PosxD.iMomChk[0]==3)	{bPrtI=TRUE ; bPrtJ=TRUE ;}	// I&J.
	else	ASSERT(0);
	// Get Bdcy.
	T_BDCY_KSCE BdcyKsceD;
	BdcyKsceD.Initialize();
	if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetBdcyKsceData(ElemK,BdcyKsceD))	return FALSE;
	BdcyKsceD.BdcyBase[0].bCHK = (BdcyKsceD.BdcyBase[0].bCHK && bPrtI ? TRUE : FALSE);	// 0=I-MAX.
	BdcyKsceD.BdcyBase[1].bCHK = FALSE;									  	                            // 1=I-MIN.
	BdcyKsceD.BdcyBase[2].bCHK = (BdcyKsceD.BdcyBase[2].bCHK && bPrtJ ? TRUE : FALSE);	// 2=J-MAX.
	BdcyKsceD.BdcyBase[3].bCHK = FALSE;										                              // 3=J-MIN.
	
	BOOL bExecutePrt = FALSE;
	for(int i=0; i<4; i++)
	{
		if(BdcyKsceD.BdcyBase[i].bCHK) bExecutePrt = TRUE;
	}

	if(!bExecutePrt) return FALSE;

	// Print on Calculating.

	return m_pDataCtrl->Get_DgnKsce05Pointer()->Get_KsceBdcy(ElemK,BdcyKsceD, pXL, bPrtI,bPrtJ);
}

BOOL CPrtKSCE_USD05::Get_DetailBdcyNeg(ElemPairK ElemK, CMSExcel* pXL)
{
	// Get Posx.
	T_POSX_D PosxD;
	PosxD.Initialize();
	if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD))	return FALSE;
	}
	else if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD))	return FALSE;
	}
	else ASSERT(0);
	BOOL bPrtI=FALSE, bPrtJ=FALSE;
	if(PosxD.iMomChk[1]==0)	return FALSE;											// NONE.
	else if(PosxD.iMomChk[1]==1)	{bPrtI=TRUE ; bPrtJ=FALSE;}	// I.
	else if(PosxD.iMomChk[1]==2)	{bPrtI=FALSE; bPrtJ=TRUE ;}	// J.
	else if(PosxD.iMomChk[1]==3)	{bPrtI=TRUE ; bPrtJ=TRUE ;}	// I&J.
	else	ASSERT(0);
	// Get Bdcy.
	T_BDCY_KSCE BdcyKsceD;
	BdcyKsceD.Initialize();
	if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetBdcyKsceData(ElemK,BdcyKsceD))	return FALSE;
	BdcyKsceD.BdcyBase[0].bCHK = FALSE;								  		                            // 0=I-MAX.
	BdcyKsceD.BdcyBase[1].bCHK = (BdcyKsceD.BdcyBase[1].bCHK && bPrtI ? TRUE : FALSE);	// 1=I-MIN.
	BdcyKsceD.BdcyBase[2].bCHK = FALSE;										                              // 2=J-MAX.
	BdcyKsceD.BdcyBase[3].bCHK = (BdcyKsceD.BdcyBase[3].bCHK && bPrtJ ? TRUE : FALSE);	// 3=J-MIN.
	
	BOOL bExecutePrt = FALSE;
	for(int i=0; i<4; i++)
	{
		if(BdcyKsceD.BdcyBase[i].bCHK) bExecutePrt = TRUE;
	}

	if(!bExecutePrt) return FALSE;

	 // Print on Calculating.
	
	return m_pDataCtrl->Get_DgnKsce05Pointer()->Get_KsceBdcy(ElemK,BdcyKsceD, pXL, bPrtI,bPrtJ);
}

BOOL CPrtKSCE_USD05::Get_DetailCums(ElemPairK ElemK)
{
	// Get Posx.
	T_POSX_D PosxD;
	PosxD.Initialize();
	if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD))	return FALSE;
	}
	else if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD))	return FALSE;
	}
	else ASSERT(0);
	BOOL bPrtI=FALSE, bPrtJ=FALSE;
	if(PosxD.iShrChk==0)	return FALSE;										// NONE.
	else if(PosxD.iShrChk==1)	{bPrtI=TRUE ; bPrtJ=FALSE;}	// I.
	else if(PosxD.iShrChk==2)	{bPrtI=FALSE; bPrtJ=TRUE ;}	// J.
	else if(PosxD.iShrChk==3)	{bPrtI=TRUE ; bPrtJ=TRUE ;}	// I&J.
	else	ASSERT(0);
	// Get Cums.
	T_CUMS_KSCE CumsKsceD;
	CumsKsceD.Initialize();
	if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCumsKsceData(ElemK,CumsKsceD))	return FALSE;
 
	double dRatMaxI = (fabs(CumsKsceD.CumsBase[0].dpVn) < cDgn_Zero ? 0.0 : fabs(CumsKsceD.CumsBase[0].dVu)/fabs(CumsKsceD.CumsBase[0].dpVn));	// I-MAX.
	double dRatMinI = (fabs(CumsKsceD.CumsBase[1].dpVn) < cDgn_Zero ? 0.0 : fabs(CumsKsceD.CumsBase[1].dVu)/fabs(CumsKsceD.CumsBase[1].dpVn));	// I-MIN.
	double dRatMaxJ = (fabs(CumsKsceD.CumsBase[2].dpVn) < cDgn_Zero ? 0.0 : fabs(CumsKsceD.CumsBase[2].dVu)/fabs(CumsKsceD.CumsBase[2].dpVn));	// J-MAX.
	double dRatMinJ = (fabs(CumsKsceD.CumsBase[3].dpVn) < cDgn_Zero ? 0.0 : fabs(CumsKsceD.CumsBase[3].dVu)/fabs(CumsKsceD.CumsBase[3].dpVn));	// J-MIN.
	int iI = (dRatMaxI > dRatMinI ? 0 : 1);
	int iJ = (dRatMaxJ > dRatMinJ ? 2 : 3);

	BOOL bExecutePrt = FALSE;
	for(int i=0; i<2; i++) // I,J
	{
		BOOL bPrtIJ = (i==0 ? bPrtI : bPrtJ);
		for(int j=0; j<2; j++)
		{
			int k = i*2+j;
			BOOL bPrint = CumsKsceD.CumsBase[k].bCHK;
			//CumsKsceD.CumsBase[k].bCHK = (bPrtIJ && bPrint ? TRUE : FALSE);

			if(k==0 || k==1)
			{
				CumsKsceD.CumsBase[k].bCHK = (iI==k && bPrtI && bPrint ? TRUE : FALSE);
			}
			else if(k==2 || k==3)
			{
				CumsKsceD.CumsBase[k].bCHK = (iJ==k && bPrtJ && bPrint ? TRUE : FALSE);
			}

			if(CumsKsceD.CumsBase[k].bCHK) bExecutePrt = TRUE;
		}    
	}

	if(!bExecutePrt) return FALSE;

	// Print on Calculating.
	return m_pDataCtrl->Get_DgnKsce05Pointer()->Get_KsceCums(ElemK,CumsKsceD,bPrtI,bPrtJ);
}

BOOL CPrtKSCE_USD05::Get_DetailCrmt(ElemPairK ElemK)
{
	// Get Posx.
	T_POSX_D PosxD;
	PosxD.Initialize();
	if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl->GetPosx(ElemK.first, PosxD))	return FALSE;
	}
	else if (ElemK.second == EN_EL_BEAM)
	{
		if (!m_pDoc->m_pAttrCtrl->GetPosv(ElemK.first, PosxD))	return FALSE;
	}
	else ASSERT(0);
	BOOL bPrtI=FALSE, bPrtJ=FALSE;
	if(PosxD.iTorChk==0)	return FALSE;										// NONE.
	else if(PosxD.iTorChk==1)	{bPrtI=TRUE ; bPrtJ=FALSE;}	// I.
	else if(PosxD.iTorChk==2)	{bPrtI=FALSE; bPrtJ=TRUE ;}	// J.
	else if(PosxD.iTorChk==3)	{bPrtI=TRUE ; bPrtJ=TRUE ;}	// I&J.
	else	ASSERT(0);
	// Get Crmt.
	T_CRMT_KSCE CrmtKsceD;
	CrmtKsceD.Initialize();
	if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCrmtKsceData(ElemK,CrmtKsceD))	return FALSE;
	
	double dRatVMaxI = (fabs(CrmtKsceD.CrmtBase[0].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[0].dTu)/fabs(CrmtKsceD.CrmtBase[0].dpTn));	// I-V-MAX.
	double dRatVMinI = (fabs(CrmtKsceD.CrmtBase[1].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[1].dTu)/fabs(CrmtKsceD.CrmtBase[1].dpTn));	// I-V-MIN.
	double dRatTMaxI = (fabs(CrmtKsceD.CrmtBase[2].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[2].dTu)/fabs(CrmtKsceD.CrmtBase[2].dpTn));	// I-T-MAX.
	double dRatVMaxJ = (fabs(CrmtKsceD.CrmtBase[3].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[3].dTu)/fabs(CrmtKsceD.CrmtBase[3].dpTn));	// J-V-MAX.
	double dRatVMinJ = (fabs(CrmtKsceD.CrmtBase[4].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[4].dTu)/fabs(CrmtKsceD.CrmtBase[4].dpTn));	// J-V-MIN.
	double dRatTMaxJ = (fabs(CrmtKsceD.CrmtBase[5].dpTn) < cDgn_Zero ? 0.0 : fabs(CrmtKsceD.CrmtBase[5].dTu)/fabs(CrmtKsceD.CrmtBase[5].dpTn));	// J-T-MAX.
	double dRatMaxI	= max(max(dRatVMaxI,dRatVMinI),dRatTMaxI);
	double dRatMaxJ	= max(max(dRatVMaxJ,dRatVMinJ),dRatTMaxJ);
	int iI=0;
			 if(fabs(dRatVMaxI-dRatMaxI) < cDgn_Zero)	iI = 0;
	else if(fabs(dRatVMinI-dRatMaxI) < cDgn_Zero)	iI = 1;
	else if(fabs(dRatTMaxI-dRatMaxI) < cDgn_Zero)	iI = 2;
	int iJ=0;
			 if(fabs(dRatVMaxJ-dRatMaxJ) < cDgn_Zero)	iJ = 3;
	else if(fabs(dRatVMinJ-dRatMaxJ) < cDgn_Zero)	iJ = 4;
	else if(fabs(dRatTMaxJ-dRatMaxJ) < cDgn_Zero)	iJ = 5;
	
	BOOL bExecutePrt = FALSE;
	for(int i=0; i<2; i++) // I,J
	{
		BOOL bPrtIJ = (i==0 ? bPrtI : bPrtJ);
		for(int j=0; j<3; j++)
		{
			int k = i*3+j;
			BOOL bPrint = CrmtKsceD.CrmtBase[k].bCHK;
			//CrmtKsceD.CrmtBase[k].bCHK = (bPrtIJ && bPrint ? TRUE : FALSE);
			
			if(k==0 || k==1 || k==2)
			{
				CrmtKsceD.CrmtBase[k].bCHK = (iI==k && bPrtI && bPrint? TRUE : FALSE);
			}
			else if(k==3 || k==4 || k==5)
			{
				CrmtKsceD.CrmtBase[k].bCHK = (iJ==k && bPrtJ && bPrint? TRUE : FALSE);
			}

			if(CrmtKsceD.CrmtBase[k].bCHK) bExecutePrt = TRUE;
		}    
	}

	if(!bExecutePrt) return FALSE;

	// Print on Calculating.
	return m_pDataCtrl->Get_DgnKsce05Pointer()->Get_KsceCrmt(ElemK,CrmtKsceD,bPrtI,bPrtJ);
}

// 인장철근량 계산.
void CPrtKSCE_USD05::Print_TbarDesign1()
{
	CString strTbar=_T("");
	strTbar = _T("Text: 인장응력 검토");                                m_arText.Add(strTbar);
	strTbar = _T("Text: 비례식으로 인장영역의 높이를 계산");            m_arText.Add(strTbar);
	m_arText.Add(_T(""));
}

void CPrtKSCE_USD05::Print_TbarDesign2(double dftop, double dfbot, double dh, double dx, double dycent, double dfcent,
																			 double dAten, double dTfor, double dfsa, double dFS, double dAsreq)
{
	CString strTbar=_T("");
	CString sftop  = Check_ValueForm(8,2,dftop);
	CString sfbot  = Check_ValueForm(8,2,dfbot);
	CString sh     = Check_ValueForm(7,1,dh);
	CString sx     = Check_ValueForm(7,1,dx);
	CString sycent = Check_ValueForm(7,1,dycent);
	CString sfcent = Check_ValueForm(8,2,dfcent);
	CString sAten  = Check_ValueForm(9,1,dAten);
	CString sTfor  = Check_ValueForm(9,2,dTfor);
	CString sfsa   = Check_ValueForm(8,2,dfsa);
	CString sFS    = Check_ValueForm(8,2,dFS);
	CString sAsreq = Check_ValueForm(9,1,dAsreq);

	CString sTop = Check_ValueForm(8,2,fabs(dftop));
	CString sBot = Check_ValueForm(8,2,fabs(dfbot));

	strTbar.Format(_T("~ f_top == %s %s ~ ~ f_bot == %s ~ %s"),sftop,m_sTxStress, sfbot,m_sTxStress);   m_arText.Add(strTbar);
	if((dftop>=0.0 && dfbot<=0.0) || (dftop<=0.0 && dfbot>=0.0))
	{
		if(dftop>0.0)
		{
			strTbar.Format(_T("~ a == %s / ( %s + %s ) * %s ~ = %s ~ %s"), sTop, sTop, sBot, sh, sx, m_strLength);  m_arText.Add(strTbar);
			m_arText.Add(_T(""));
			strTbar = _T("Text: 인장력 T");                             m_arText.Add(strTbar);
			strTbar = _T("Text: - 인장영역의 도심위치에서의 응력");     m_arText.Add(strTbar);
			m_arText.Add(_T(""));
		 strTbar.Format(_T("~ f_cent == y_cent * f_top / a == %s * %s / %s ~ = %s ~ %s"),
										 sycent, sTop , sx, sfcent, m_sTxStress);    m_arText.Add(strTbar);
			strTbar.Format(_T("~ T == f_cent * A_ten == %s * %s ~ = %s ~ %s"),sfcent, sAten, sTfor, m_strForce); m_arText.Add(strTbar);
			strTbar.Format(_T("~ A_sreq == T / f_sa == %s / %s ~ = %s ~ %s"),
										 sTfor, sfsa, sAsreq, m_sTxArea);   m_arText.Add(strTbar);
		}
		else 
		{
			strTbar.Format(_T("~ a == %s / ( %s + %s ) * %s ~ = %s ~ %s"), sBot, sTop, sBot, sh, sx, m_strLength);  m_arText.Add(strTbar);
			strTbar = _T("Text: 인장력 T");            m_arText.Add(strTbar);
			strTbar = _T("Text: - 인장영역의 도심위치에서의 응력");     m_arText.Add(strTbar);
			m_arText.Add(_T(""));
			strTbar.Format(_T("~ f_cent == y_cent * f_bot / a == %s * %s / %s ~ = %s ~ %s"),
										 sycent, sBot , sx, sfcent, m_sTxStress);    m_arText.Add(strTbar);
			strTbar.Format(_T("~ T == f_cent * A_ten == %s * %s ~ = %s ~ %s"),sfcent, sAten, sTfor, m_strForce); m_arText.Add(strTbar);
			strTbar.Format(_T("~ A_sreq == T / f_sa == %s / %s ~ = %s ~ %s"),
										 sTfor, sfsa, sAsreq, m_sTxStress);   m_arText.Add(strTbar);
		}
	}

	m_arText.Add(_T(""));

}

void CPrtKSCE_USD05::Print_TbarDesign3(double dft, double dfm, double dftop, double dfbot, double dh, double dx, double dycent, double dfcent,
																			 double dTfor, double dfsa, double dFS, double dAsreq, double dyten, double dAc)
{
	CString strTbar=_T("");
	CString sft    = Check_ValueForm(6,2,dft);
	CString sfm    = Check_ValueForm(6,2,dfm);
	CString sftop  = Check_ValueForm(6,2,dftop);
	CString sfbot  = Check_ValueForm(6,2,dfbot);
	CString sh     = Check_ValueForm(6,2,dh);
	CString sx     = Check_ValueForm(6,2,dx);
	CString sycent = Check_ValueForm(6,2,dycent);
	CString sfcent = Check_ValueForm(6,2,dfcent);
	CString sTfor  = Check_ValueForm(6,2,dTfor);
	CString sfsa   = Check_ValueForm(6,2,dfsa);
	CString sFS    = Check_ValueForm(6,2,dFS);
	CString sAsreq = Check_ValueForm(6,2,dAsreq);
	CString syten  = Check_ValueForm(6,2,dyten);
	CString sAc    = Check_ValueForm(6,2,dAc);


	CString sTop = Check_ValueForm(6,2,fabs(dftop));
	CString sBot = Check_ValueForm(6,2,fabs(dfbot));

	if(dftop>0.0 && dfbot>0.0) 
	{
		double dft=0.0, dfm=0.0;
		if(dftop>=dfbot)
		{
			strTbar.Format(_T("a == %s ~ %s"), sx, m_strLength);    m_arText.Add(strTbar);
			m_arText.Add(_T(""));
			strTbar = _T("Text: 인장력 T");            m_arText.Add(strTbar);
			strTbar = _T("Text:  -인장영역의 도심위치에서의 응력");     m_arText.Add(strTbar);
			m_arText.Add(_T(""));
			strTbar.Format(_T("~ f_t == f_top - f_bot == %s - %s ~ = %s ~ %s"),sftop, sfbot, sft, m_sTxStress); m_arText.Add(strTbar);
			strTbar.Format(_T("~ f_m == y_t * f_t / h == %s * %s / %s ~ = %s ~ %s"),syten, sft, sh, dfm, m_sTxStress);   m_arText.Add(strTbar);
			strTbar.Format(_T("~ T == ( f_bot + f_m ) * A_c == ( %s + %s ) * %s ~ = %s ~ %s"),sfbot, sfm, sAc, sTfor, m_strForce); m_arText.Add(strTbar);
			strTbar.Format(_T("~ A_sreq == T / ( f_sa * FS ) == %s / ( %s * %s ) ~ = %s ~ %s"),
										 sTfor, sfsa, sFS, sAsreq, m_sTxArea);   m_arText.Add(strTbar);
		}
		else if(dfbot>dftop)
		{
			strTbar.Format(_T("a == %s ~ %s"), sx);   m_arText.Add(strTbar);
			m_arText.Add(_T(""));
			strTbar = _T("Text: 인장력 T");            m_arText.Add(strTbar);
			strTbar = _T("Text:  -인장영역의 도심위치에서의 응력");     m_arText.Add(strTbar);
			m_arText.Add(_T(""));
			strTbar.Format(_T("~ f_t == f_top - f_bot == %s - %s ~ = %s ~ %s"),sftop, sfbot, sft, m_sTxStress); m_arText.Add(strTbar);
			strTbar.Format(_T("~ f_m == y_t * f_t / h == %s * %s / %s ~ = %s ~ %s"),syten, sft, sh, sfm, m_sTxStress);   m_arText.Add(strTbar);
			strTbar.Format(_T("~ T == ( f_bot + f_m ) * A_c == ( %s + %s ) * %s ~ = %s ~ %s"),sfbot, sfm, sAc, sTfor, m_strForce); m_arText.Add(strTbar);
			strTbar.Format(_T("~ A_sreq == T / ( f_sa * FS ) == %s / ( %s * %s ) ~ = %s ~ %s"),
										 sTfor, sfsa, sFS, sAsreq, m_sTxArea);   m_arText.Add(strTbar);
		}
	}
	
	m_arText.Add(_T(""));
}

void CPrtKSCE_USD05::Print_BdcyTheory_Iter(CArray<CString,CString>& aString, int iCode)
{
	CString strBdcy = _T("");
	CString strEqua = _T("");
	CString strSpace = _T("");
	for(int i=0; i<30; i++)
		strSpace += _T("~ ");

	strBdcy = _T("Text: ▷ 휨 강도 검토");                                   aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);
	strBdcy = _T("Text:    1) 중립축의 위치 c를 가정한다.");                 aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);
	strBdcy = _T("Text:    2) 등가 직사각형 응력블록의 깊이에 대한 콘크리트 압축력을 계산한다.");         aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ ~ Cc</Title> <Col>15</Col> </Equation>");
	strBdcy.Format(_T("%s  A_a : 압축연단에서`a까지의`넓이"),strEqua);                aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);
 
	strBdcy = _T("~ ~ 3)`PS`긴장재의`변형도 ε_ps 를`계산하고,`PS`긴장재의`응력`-`변형률`곡선으로부터");       aString.Add(strBdcy);
	strBdcy = _T("~ ~ ~ ~ f_ps 를`계산한다.");                             aString.Add(strBdcy);

	strEqua = _T("<Equation> <Title>~ ~ ~ epsi_cp</Title> <Col>10</Col> <Title>~ ~ epsi_pe</Title> <Col>20</Col> <Title>~ epsi_ce</Title> <Col>30</Col> </Equation>");  aString.Add(strEqua); 
	strBdcy = _T(""); aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);
	strBdcy = _T("~ ~ ~ d_pi : `압축연단에서`긴장재`그룹`도심까지의`거리 ");   aString.Add(strBdcy);
	strBdcy = _T("~ ~ ~ e_pi : `중립축에서`긴장재`그룹`도심까지의`거리");      aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ ~ epsi_ps</Title> <Col>0</Col> </Equation>"); aString.Add(strEqua);
	strBdcy = _T(""); aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);

	strBdcy = _T("Text:   - PS 긴장재의 응력-변형도 곡선");                     aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ ~ epsi_py</Title> <Col>0</Col> </Equation>");   aString.Add(strEqua);
	strEqua = _T("<Equation> <Title>~ ~ ~ epsi_ps2</Title> <Col>10</Col> <Title>~ fps</Title> <Col>20</Col> </Equation>");  aString.Add(strEqua);
	strEqua = _T("<Equation> <Title>~ ~ ~ epsi_ps3</Title> <Col>9</Col> </Equation> Text: 일 때,"); aString.Add(strEqua);
	strBdcy = _T(" "); aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ ~ fpu_KS05</Title> <Col>20</Col> <Title>~ fps2_KS05</Title> <Col>30</Col> </Equation>");  aString.Add(strEqua);
	strBdcy = _T(""); aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ ~ fpu2_KS05</Title> <Col>20</Col> <Title>~ fps3_KS05</Title> <Col>30</Col> </Equation>");  aString.Add(strEqua);
	strBdcy = _T(""); aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);

	strBdcy = _T("~ ~ 4)`PS`긴장재의`인장부재력 T_p 를`계산한다.");                           aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ ~ T_p</Title> <Col>0</Col> </Equation>");   aString.Add(strEqua);
	strBdcy = _T(""); aString.Add(strBdcy);

	strBdcy = _T("~ ~ 5)`인장보강철근의`응력 f_s 계산후`부재력 T_s 를`계산한다.");            aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ ~ epsi_s</Title> <Col>10</Col> <Title>~ fs</Title> <Col>20</Col> <Title>~ Ts</Title> <Col>30</Col> </Equation>");  aString.Add(strEqua); 
	strBdcy = _T(""); aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);

	strBdcy = _T("Text:    6) 힘의 평형상태를 검토한다");                                          aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ ~ Delta</Title> <Col>0</Col> </Equation>");   aString.Add(strEqua);
	strBdcy = _T(""); aString.Add(strBdcy);

	strBdcy = _T("Text:    7) |ΔC|>TOL 이면 중립축의 위치를 수정하고 2)부터 다시 반복한다.");     aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);

	strBdcy = _T("Text:    8) 평형조건이 만족되면 휨강도를 계산한다. (중립축을 기준으로 계산)");   aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ ~ Mn</Title> <Col>0</Col> </Equation>");   aString.Add(strEqua);
	strBdcy = _T("~ ~ ~ a_cent :`a부터`압축연단에서`a까지의`넓이의`도심까지의`거리");         aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);


	//2 Page
	strBdcy = _T("Text: ▷ PS 강재의 제한");             aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);

	strBdcy = _T("Text:   - 최대 PS 강재량");             aString.Add(strBdcy);
	strBdcy = _T("Text:   ㆍ중립축이 플랜지내에 있는 단면");             aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ ~ qp_in</Title> <Col>30</Col> </Equation>");   
	strBdcy.Format(_T("%s q_p ~ : 강재지수"),strEqua);             aString.Add(strBdcy);
	strBdcy = strSpace + _T("β_1 :`등가`직사각형`응력분포의`깊이와");             aString.Add(strBdcy);
	strBdcy = strSpace + _T("~ ~ ~ ~ 중립축`거리와의`비");             aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);

	strBdcy = _T("Text:   ㆍ중립축이 플랜지내에 있지 않는 단면");             aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ ~ qp_out</Title> <Col>30</Col> </Equation>");  aString.Add(strEqua);
	strBdcy = _T("");   aString.Add(strBdcy);
	strBdcy = _T("");   aString.Add(strBdcy);

	
	strBdcy = _T("Text: ▷ 최소 PS 강재량");     aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);

	if(iCode==KSCE_USD05)
	{
		strEqua = _T("<Equation> <Title>~ ~ ~ b12Mcr</Title> <Col>30</Col> </Equation>");   aString.Add(strEqua);
		strEqua = _T("<Equation> <Title>~ ~ ~ bMcr_KS05</Title> <Col>30</Col> </Equation>"); 
		//strBdcy.Format(_T("%s f_pcc : ~ 손실이`일어난`후`강재의`유효프리스트레스"),strEqua);             aString.Add(strBdcy);
		strBdcy.Format(_T("%s f_pcc : ~ 외력에`의해`인장응력이`발생하는"),strEqua);             aString.Add(strBdcy);
		strBdcy.Format(_T("%s ~ ~ ~ ~ ~ ~ 단면의`최연단에서`콘크리트의`압축응력"), strSpace);               aString.Add(strBdcy);
		//
		strEqua = _T("<Equation> <Title>~ ~ ~ fru_val</Title> <Col>30</Col> </Equation>"); 
		strBdcy.Format(_T("%s f_ru ~ : ~ 콘크리트의`허용 휨강도`(`파괴계수`)"),strEqua);                aString.Add(strBdcy);  
		aString.Add(_T(""));
		//
		strEqua = _T("<Equation> <Title>~ ~ ~ bMcr2</Title> <Col>30</Col> </Equation>"); 
		strBdcy.Format(_T("%s Z_c ~ ~ : ~ 외부`작용하중에`의해`인장응력이`발생하는"),strEqua);  aString.Add(strBdcy);  
		strBdcy.Format(_T("%s ~ ~ ~ ~ ~ ~ 단면의`최외연에`대한`합성단면계수"), strSpace);                   aString.Add(strBdcy);
		strBdcy = _T(""); aString.Add(strBdcy);
		strBdcy = _T("~ ~ ~ Φ  M_n ≥  4/3 M_u 일`경우에는`단면은`최소`강재량을`만족하지`않아도`상관이`없다.");             aString.Add(strBdcy);
	}
	else if(iCode==KSCE_RAIL_USD04)
	{
		strEqua = _T("<Equation> <Title>~ ~ ~ b12Mcr_rail</Title> <Col>30</Col> </Equation>");   aString.Add(strEqua);
		strEqua = _T("<Equation> <Title>~ ~ ~ bMcr_rail</Title> <Col>30</Col> </Equation>"); 
		//strBdcy.Format(_T("%s f_pcc : ~ 손실이`일어난`후`강재의`유효프리스트레스"),strEqua);             aString.Add(strBdcy);
		strBdcy.Format(_T("%s f_pe : ~ 외력에`의해`인장응력이`발생하는"),strEqua);             aString.Add(strBdcy);
		strBdcy.Format(_T("%s ~ ~ ~ ~ ~ ~ 단면의`최연단에서`콘크리트의`압축응력"), strSpace);               aString.Add(strBdcy);
		//
		strEqua = _T("<Equation> <Title>~ ~ ~ fru_rail</Title> <Col>30</Col> </Equation>"); 
		strBdcy.Format(_T("%s f_ru ~ : ~ 콘크리트의`허용 휨강도`(`파괴계수`)"),strEqua);                aString.Add(strBdcy);  
		aString.Add(_T(""));
		//
		strEqua = _T("<Equation> <Title>~ ~ ~ bMcr2_rail</Title> <Col>30</Col> </Equation>"); 
		strBdcy.Format(_T("%s Z_c ~ ~ : ~ 외부`작용하중에`의해`인장응력이`발생하는"),strEqua);  aString.Add(strBdcy);  
		strBdcy.Format(_T("%s ~ ~ ~ ~ ~ ~ 단면의`최외연에`대한`합성단면계수"), strSpace);                   aString.Add(strBdcy);
		strBdcy = _T(""); aString.Add(strBdcy);
	}     
}

void CPrtKSCE_USD05::Print_BdcyTheory_Code_KSCE05(CArray<CString,CString>& aString)
{
	CString strBdcy = _T("");
	CString strEqua = _T("");
	CString strSpace = _T("");
	for(int i=0; i<30; i++)
		strSpace += _T("~ ");

	strBdcy = _T("Text: ▷ 휨 강도 검토");                                          aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);
	strBdcy = _T("Text:  ○ PS 강재의 응력");                                       aString.Add(strBdcy);
	strBdcy = _T("Text:   - 부착된 부재");                                          aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ fps_bond1_05</Title> <Col>15</Col> </Equation>");
	strBdcy.Format(_T("%s  "),strEqua);                aString.Add(strBdcy);
	aString.Add(_T(""));
	aString.Add(_T(""));
	strBdcy = _T("Text:   ㆍ인장측에 철근을 배치하지 않은 경우");                                          aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ fps_bond2</Title> <Col>15</Col> </Equation>");
	strBdcy.Format(_T("%s  "),strEqua);                aString.Add(strBdcy);
	aString.Add(_T(""));
	aString.Add(_T(""));
	strBdcy = _T("Text:   - 부착되지 않은 부재");                                          aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ fps_unbond_05</Title> <Col>30</Col> </Equation>");
	strBdcy.Format(_T("%s  f_pe : 손실이`일어난`후`유효`프리스트레스"),strEqua);                aString.Add(strBdcy);
	aString.Add(_T(""));

//  strBdcy = _T("Text:  ○ 등가 사각형의 응력블록의 깊이");                            aString.Add(strBdcy);
//  strEqua = _T("<Equation> <Title>~ ~ a_block</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
//  aString.Add(_T(""));

	strBdcy = _T("Text:  ○ 설계 휨강도");                                 aString.Add(strBdcy);
	strBdcy = _T("Text:   - 압축측에 배치되는 철근은 항복여부에 따른 저항력 산정하여 고려할 수 있으나"); aString.Add(strBdcy);
	strBdcy = _T("Text:   - 본 검토에서 무시하기로 한다. 또한 압축측에 배치는 PS 강재도 마찬가지로 항복여부에 따른"); aString.Add(strBdcy);
	strBdcy = _T("Text:   - 저항력 산정 규정이 없어 고려하지 않는다."); aString.Add(strBdcy);
	aString.Add(_T(""));
	strBdcy = _T("Text:   - 사각형 단면");                                 aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ pMn_rec</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
	aString.Add(_T(""));
	aString.Add(_T(""));
	strBdcy = _T("Text:    ㆍ인장측에 철근을 배치하지 않은 경우");                                          aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ pMn_rec_nobar</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
	aString.Add(_T(""));
	aString.Add(_T(""));
	strBdcy = _T("Text:   - 플랜지를 갖는 단면");                                 aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ pMn_flg</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
	aString.Add(_T(""));
	aString.Add(_T(""));  
	strEqua = _T("<Equation> <Title>~ ~ ~ ~ Apw1</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
	strEqua = _T("<Equation> <Title>~ ~ ~ ~ Apw2</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
	strEqua = _T("<Equation> <Title>~ ~ ~ ~ Apf</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
	aString.Add(_T(""));

	//2 Page
	strBdcy = _T("Text: ▷ PS 강재의 제한");             aString.Add(strBdcy);
	aString.Add(_T(""));

	strBdcy = _T("Text:   - 최대 PS 강재량");             aString.Add(strBdcy);
	strBdcy = _T("Text:   ㆍ사각형 단면");             aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ ~ qp_in</Title> <Col>30</Col> </Equation>");   
	strBdcy.Format(_T("%s q_p ~ : 강재지수"),strEqua);             aString.Add(strBdcy);
	strBdcy = strSpace + _T("β_1 :`등가`직사각형`응력분포의`깊이와");             aString.Add(strBdcy);
	strBdcy = strSpace + _T("~ ~ ~ ~ 중립축`거리와의`비");             aString.Add(strBdcy);
	aString.Add(_T(""));

	strBdcy = _T("Text:   ㆍ플랜지를 갖는 단면");             aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ ~ qp_out</Title> <Col>30</Col> </Equation>");  aString.Add(strEqua);
	aString.Add(_T(""));
	aString.Add(_T(""));
	strBdcy = _T("~ ~ 강재지수가  0.36 β_1  보다`큰`부재에서는`설계`휨강도를`다음`값보다`크지`않게`한다");             aString.Add(strBdcy);
	strBdcy = _T("Text:   ㆍ사각형 단면");             aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ ~ pMn_qp_rec</Title> <Col>0</Col> </Equation>");  aString.Add(strEqua);
	aString.Add(_T(""));
	strBdcy = _T("Text:   ㆍ플랜지를 갖는 단면");             aString.Add(strBdcy);  
	strEqua = _T("<Equation> <Title>~ ~ ~ pMn_qp_flg</Title> <Col>0</Col> </Equation>");  aString.Add(strEqua);
	aString.Add(_T(""));
		
	strBdcy = _T("Text: ▷ 최소 PS 강재량");     aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);

	strEqua = _T("<Equation> <Title>~ ~ ~ b12Mcr</Title> <Col>30</Col> </Equation>");   aString.Add(strEqua);
	strEqua = _T("<Equation> <Title>~ ~ ~ bMcr_KS05</Title> <Col>30</Col> </Equation>"); 
	//strBdcy.Format(_T("%s f_pcc : ~ 손실이`일어난`후`강재의`유효프리스트레스"),strEqua);             aString.Add(strBdcy);
	strBdcy.Format(_T("%s f_pcc : ~ 외력에`의해`인장응력이`발생하는"),strEqua);             aString.Add(strBdcy);
	strBdcy.Format(_T("%s ~ ~ ~ ~ ~ ~ 단면의`최연단에서`콘크리트의`압축응력"), strSpace);               aString.Add(strBdcy);
	//
	strEqua = _T("<Equation> <Title>~ ~ ~ fru_val</Title> <Col>30</Col> </Equation>"); 
	strBdcy.Format(_T("%s f_ru ~ : ~ 콘크리트의`허용 휨강도`(`파괴계수`)"),strEqua);                aString.Add(strBdcy);  
	aString.Add(_T(""));
	//
	strEqua = _T("<Equation> <Title>~ ~ ~ bMcr2</Title> <Col>30</Col> </Equation>"); 
	strBdcy.Format(_T("%s Z_c ~ ~ : ~ 외부`작용하중에`의해`인장응력이`발생하는"),strEqua);  aString.Add(strBdcy);  
	strBdcy.Format(_T("%s ~ ~ ~ ~ ~ ~ 단면의`최외연에`대한`합성단면계수"), strSpace);                   aString.Add(strBdcy);
	aString.Add(_T(""));
	strBdcy = _T("~ ~ ~ Φ  M_n ≥  4/3  M_u 일`경우에는`단면은`최소`강재량을`만족하지`않아도`상관이`없다.");             aString.Add(strBdcy);
		 
}

void CPrtKSCE_USD05::Print_BdcyTheory_Code_RAIL04(CArray<CString,CString>& aString)
{
	CString strBdcy = _T("");
	CString strEqua = _T("");
	CString strSpace = _T("");
	for(int i=0; i<30; i++)
		strSpace += _T("~ ");

	strBdcy = _T("Text: ▷ 휨 강도 검토");                                          aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);
	strBdcy = _T("Text:  ○ PS 강재의 응력");                                       aString.Add(strBdcy);
	strBdcy = _T("Text:   - 부착된 부재");                                          aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ fps_bond_rail</Title> <Col>15</Col> </Equation>");
	strBdcy.Format(_T("%s  "),strEqua);                aString.Add(strBdcy);
	aString.Add(_T(""));
	aString.Add(_T(""));
	strBdcy = _T("Text:   - 부착되지 않은 부재");                                          aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ fps_unbond1_rail</Title> <Col>30</Col> </Equation>");
	strBdcy.Format(_T("%s  f_pe : 손실이`일어난`후`유효`프리스트레스"),strEqua);                aString.Add(strBdcy);
	aString.Add(_T(""));

	//strBdcy = _T("Text:  ○ 등가 사각형의 응력블록의 깊이");                            aString.Add(strBdcy);
	//strEqua = _T("<Equation> <Title>~ ~ a_block</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
	//aString.Add(_T(""));

	strBdcy = _T("Text:  ○ 설계 휨강도");                                 aString.Add(strBdcy);
	strBdcy = _T("Text:   - 압축측에 배치되는 철근은 항복여부에 따른 저항력 산정하여 고려할 수 있으나"); aString.Add(strBdcy);
	strBdcy = _T("Text:   - 본 검토에서 무시하기로 한다. 또한 압축측에 배치는 PS 강재도 마찬가지로 항복여부에 따른"); aString.Add(strBdcy);
	strBdcy = _T("Text:   - 저항력 산정 규정이 없어 고려하지 않는다."); aString.Add(strBdcy);
	aString.Add(_T(""));
	strBdcy = _T("Text:   - 사각형 단면");                                 aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ pMn_rec_rail</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
	aString.Add(_T(""));
	aString.Add(_T(""));
	strBdcy = _T("Text:   - 플랜지를 갖는 단면");                                 aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ pMn_flg_rail</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
	aString.Add(_T(""));
	aString.Add(_T(""));  
	strEqua = _T("<Equation> <Title>~ ~ ~ ~ Apw_rail</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
	strEqua = _T("<Equation> <Title>~ ~ ~ ~ Apf_rail</Title> <Col>0</Col> </Equation>");     aString.Add(strEqua);
	aString.Add(_T(""));

	//2 Page
	strBdcy = _T("Text: ▷ PS 강재의 제한");             aString.Add(strBdcy);
	aString.Add(_T(""));

	strBdcy = _T("Text:   - 최대 PS 강재량");             aString.Add(strBdcy);
	strBdcy = _T("Text:   ㆍ사각형 단면");             aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ ~ qp_rec_rail</Title> <Col>30</Col> </Equation>");   
	strBdcy.Format(_T("%s q_p ~ : 강재지수"),strEqua);             aString.Add(strBdcy);
	strBdcy = strSpace + _T("β_1 :`등가`직사각형`응력분포의`깊이와");             aString.Add(strBdcy);
	strBdcy = strSpace + _T("~ ~ ~ ~ 중립축`거리와의`비");             aString.Add(strBdcy);
	aString.Add(_T(""));

	strBdcy = _T("Text:   ㆍ플랜지를 갖는 단면");             aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ ~ qp_flg_rail</Title> <Col>30</Col> </Equation>");  aString.Add(strEqua);
	aString.Add(_T(""));
	aString.Add(_T(""));
	strBdcy = _T("~ ~ 강재지수가  0.36 β_1  보다`큰`부재에서는`설계`휨강도를`다음`값보다`크지`않게`한다");             aString.Add(strBdcy);
	strBdcy = _T("Text:   ㆍ사각형 단면");             aString.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ ~ pMn_qp_rec_rail</Title> <Col>0</Col> </Equation>");  aString.Add(strEqua);
	aString.Add(_T(""));
	strBdcy = _T("Text:   ㆍ플랜지를 갖는 단면");             aString.Add(strBdcy);  
	strEqua = _T("<Equation> <Title>~ ~ ~ pMn_qp_flg_rail</Title> <Col>0</Col> </Equation>");  aString.Add(strEqua);
	aString.Add(_T(""));
		
	strBdcy = _T("Text: ▷ 최소 PS 강재량");     aString.Add(strBdcy);
	strBdcy = _T(""); aString.Add(strBdcy);

	strEqua = _T("<Equation> <Title>~ ~ ~ b12Mcr_rail</Title> <Col>30</Col> </Equation>");   aString.Add(strEqua);
	strEqua = _T("<Equation> <Title>~ ~ ~ bMcr_rail</Title> <Col>30</Col> </Equation>"); 
	//strBdcy.Format(_T("%s f_pcc : ~ 손실이`일어난`후`강재의`유효프리스트레스"),strEqua);             aString.Add(strBdcy);
	strBdcy.Format(_T("%s f_pe : ~ 외력에`의해`인장응력이`발생하는"),strEqua);             aString.Add(strBdcy);
	strBdcy.Format(_T("%s ~ ~ ~ ~ ~ 단면의`최연단에서`콘크리트의`압축응력"), strSpace);               aString.Add(strBdcy);
	//
	strEqua = _T("<Equation> <Title>~ ~ ~ fru_rail</Title> <Col>30</Col> </Equation>"); 
	strBdcy.Format(_T("%s f_ru : ~ 콘크리트의`허용 휨강도`(`파괴계수`)"),strEqua);                aString.Add(strBdcy);  
	aString.Add(_T(""));
	//
	strEqua = _T("<Equation> <Title>~ ~ ~ bMcr2_rail</Title> <Col>30</Col> </Equation>"); 
	strBdcy.Format(_T("%s Z_c ~ : ~ 외부`작용하중에`의해`인장응력이`발생하는"),strEqua);  aString.Add(strBdcy);  
	strBdcy.Format(_T("%s ~ ~ ~ ~ ~ 단면의`최외연에`대한`합성단면계수"), strSpace);                   aString.Add(strBdcy);
	aString.Add(_T(""));
	//strBdcy = _T("~ ~ ~ Φ  M_n ≥  4/3  M_u 일`경우에는`단면은`최소`강재량을`만족하지`않아도`상관이`없다.");             aString.Add(strBdcy);
		 
}

void CPrtKSCE_USD05::Print_BdcyDesign0(BOOL bApwM)
{
	CString sCount; sCount.Format(_T("%d"), m_BdcyParam.iCount);
	CString sfck = Check_ValueForm(6,1, m_BdcyParam.dfck);
	CString sbw  = Check_ValueForm(7,1, m_BdcyParam.dbw);
	CString sb   = Check_ValueForm(7,1, m_BdcyParam.db);
	CString sAps = Check_ValueForm(7,1, m_BdcyParam.dAps);
	CString sfy  = Check_ValueForm(6,1, m_BdcyParam.dfy);
	CString sdp  = Check_ValueForm(7,1, m_BdcyParam.ddp);
	CString sAst = Check_ValueForm(7,1, m_BdcyParam.dAs);

	CString sfpu = Check_ValueForm(7,1, m_BdcyParam.dfpu);
	CString sd   = Check_ValueForm(7,1, m_BdcyParam.dd);
	CString sAc  = Check_ValueForm(9,3, m_BdcyParam.dAc);
	CString sIy  = Check_ValueForm(9,3, m_BdcyParam.dIy);
	CString syt  = Check_ValueForm(7,1, m_BdcyParam.dyb);
	CString sPe  = Check_ValueForm(9,3, Cng_CodeForFix(m_BdcyParam.dPe));
	
	CString sbeta1 = Check_ValueForm(6,3, m_BdcyParam.dbeta1);
	CString sep    = Check_ValueForm(7,1, m_BdcyParam.dep);
	CString sMu    = Check_ValueForm(8,1, Cng_CodeMomFix(m_BdcyParam.dMu));
	CString srp    = Check_ValueForm(6,2, m_BdcyParam.drp);
	CString shf    = Check_ValueForm(7,1, m_BdcyParam.dhf);
	
	CString sSpace4 = _T("~ ~ ~ ~ ");
	CString sSpace5 = _T("~ ~ ~ ~ ~ ");
	CString sSpace6 = _T("~ ~ ~ ~ ~ ~ ");
	CString sSpace7 = _T("~ ~ ~ ~ ~ ~ ~ ");
	CString sSpace8 = _T("~ ~ ~ ~ ~ ~ ~ ~ ");
	CString sSpace9 = _T("~ ~ ~ ~ ~ ~ ~ ~ ~ ");
	CString s1stSpace  = _T("");
	CString s2ndSpace  = _T("");

	int iMaxLength = 0;
	int arLength[3][6];

	int isfck   = arLength[0][0] = sfck.GetLength();
	int isfy    = arLength[0][1] = sfy.GetLength();
	int isfpu   = arLength[0][2] = sfpu.GetLength();
	int isbeta1 = arLength[0][3] = sbeta1.GetLength();
	int irp     = arLength[0][4] = srp.GetLength();
	int isbw    = arLength[0][5] = sbw.GetLength();  

	int isb    = arLength[1][0] = sb.GetLength();
	int isdp   = arLength[1][1] = sdp.GetLength();
	int isd    = arLength[1][2] = sd.GetLength();
	int isyt   = arLength[1][3] = syt.GetLength();
	int isep   = arLength[1][4] = sep.GetLength();
	int ishf   = arLength[1][5] = shf.GetLength();  

	int isAps   = arLength[2][0] = sAps.GetLength();
	int isAst   = arLength[2][1] = sAst.GetLength();
	int isAc    = arLength[2][2] = sAc.GetLength();
	int isIy    = arLength[2][3] = sIy.GetLength();
	int isPe    = arLength[2][4] = sPe.GetLength();
	int isMu    = arLength[2][5] = sMu.GetLength();  

	// 1 Column.    
	for(int i=0; i<6; i++)  
		iMaxLength = max(iMaxLength,arLength[0][i]);  

	CString strfck, strfy, strfpu, strbeta1, strrp, strbw;
	strfck  = Get_TextStringLength(iMaxLength, sfck);
	strfy   = Get_TextStringLength(iMaxLength, sfy);
	strfpu  = Get_TextStringLength(iMaxLength, sfpu);
	strbeta1= Get_TextStringLength(iMaxLength, sbeta1);  strbeta1 += sSpace4;
	strrp   = Get_TextStringLength(iMaxLength, srp);     strrp += sSpace4;
	strbw   = Get_TextStringLength(iMaxLength, sbw);     
	
	if(iMaxLength==10 || iMaxLength==11)    s1stSpace = sSpace4;
	else if(iMaxLength==9 || iMaxLength==8) s1stSpace = sSpace5;
	else if(iMaxLength==7)                  s1stSpace = sSpace6;
	else if(iMaxLength==6 || iMaxLength==5) s1stSpace = sSpace7;
	else if(iMaxLength==4 || iMaxLength==3) s1stSpace = sSpace8;
	else if(iMaxLength==2)                  s1stSpace = sSpace9;
	else ASSERT(0);

	CString s1colLength = _T("");
	for(int i=0; i<Get_XlLength(iMaxLength); i++)
		s1colLength += _T("~ ");
	
	s1colLength  = s1colLength + sSpace8 + sSpace4 + s1stSpace;

	// 2 Column.
	iMaxLength = 0;
	for(int i=0; i<6; i++)  
		iMaxLength = max(iMaxLength,arLength[1][i]);

	CString strb, strdp, strd, stryt, strep, strhf;
	strb  = Get_TextStringLength(iMaxLength, sb  );   
	strdp = Get_TextStringLength(iMaxLength, sdp );  
	strd  = Get_TextStringLength(iMaxLength, sd  );   
	stryt = Get_TextStringLength(iMaxLength, syt );  
	strep = Get_TextStringLength(iMaxLength, sep );  
	strhf = Get_TextStringLength(iMaxLength, shf );  

	if(iMaxLength==10 || iMaxLength==11)    s2ndSpace = sSpace4;
	else if(iMaxLength==9 || iMaxLength==8) s2ndSpace = sSpace5;
	else if(iMaxLength==7)                  s2ndSpace = sSpace6;
	else if(iMaxLength==6 || iMaxLength==5) s2ndSpace = sSpace7;
	else if(iMaxLength==4 || iMaxLength==3) s2ndSpace = sSpace8;
	else if(iMaxLength==2)                  s2ndSpace = sSpace9;

	// 3 Column.
	iMaxLength = 0;
	for(int i=0; i<6; i++)  
		iMaxLength = max(iMaxLength,arLength[2][i]);

	CString strAps, strAst, strAc, strIy, strPe, strMu;
	strAps = Get_TextStringLength(iMaxLength, sAps  );    
	strAst = Get_TextStringLength(iMaxLength, sAst );    
	strAc  = Get_TextStringLength(iMaxLength, sAc  );     
	strIy  = Get_TextStringLength(iMaxLength, sIy );     
	strPe  = Get_TextStringLength(iMaxLength, sPe );     
	strMu  = Get_TextStringLength(iMaxLength, sMu );     

	if(sIy.GetLength()>=9)  strIy.TrimRight(_T("~ "));
	if(sAc.GetLength()>=9)  strAc.TrimRight(_T("~ "));
	if(sPe.GetLength()>=9)  strPe.TrimRight(_T("~ "));
	if(sMu.GetLength()>=9)  strPe.TrimRight(_T("~ "));

	// N/mm2 = 4col
	CString strBdcy;  //①
	if(bApwM) 
	{
		strBdcy = _T("Text: Apw<0 으로 계산되어 fps를 상세계산 합니다.");  m_arText.Add(strBdcy);
		m_arText.Add(_T(""));
	}
#if defined(_DEBUG)
	strBdcy.Format(_T("%s   %s"), _T("Text: (1) 설계 변수"), sCount);                                m_arText.Add(strBdcy);
#else
	strBdcy = _T("Text: (1) 설계 변수");                                                         m_arText.Add(strBdcy);
#endif
	if(m_iDgnCode==KSCE_USD05)
	{    
		strBdcy.Format(_T("~ ~ f_ck : ~ %s %s %s b ~ ~ : ~ %s %s %s A_p ~ : ~ %s%s"),
									 strfck, m_sTxStress, s1stSpace , strb, m_strLength, s2ndSpace, strAps, m_sTxArea);                    m_arText.Add(strBdcy);
		strBdcy.Format(_T("~ ~ f_y ~ : ~ %s %s %s d_p ~ : ~ %s %s %s A_s ~ : ~ %s%s"),
									 strfy, m_sTxStress, s1stSpace, strdp, m_strLength, s2ndSpace, strAst, m_sTxArea);             m_arText.Add(strBdcy);

		strBdcy.Format(_T("~ ~ f_pu : ~ %s %s %s d ~ ~ : ~ %s %s %s A_c ~ : ~ %s %s"),
									 strfpu, m_sTxStress, s1stSpace, strd, m_strLength, s2ndSpace, strAc, m_sTxArea);    m_arText.Add(strBdcy);
		strBdcy.Format(_T("~ ~ β_1 : ~ %s %s y_t ~ : ~ %s %s %s I_y ~ : ~ %s %s"),
									 strbeta1, s1stSpace, stryt, m_strLength, s2ndSpace, strIy, m_sTxStif);     m_arText.Add(strBdcy);

		strBdcy.Format(_T("~ ~ γ_p : ~ %s %s e_p ~ : ~ %s %s %s P_e ~ : ~ %s %s"),
									 strrp, s1stSpace, strep, m_strLength, s2ndSpace, strPe, m_strForce);    m_arText.Add(strBdcy);
		strBdcy.Format(_T("~ ~ b_0 ~ : ~ %s %s ~ ~ ~ %s t ~ ~ : ~ %s %s %s M_u ~ : ~ %s %s"), 
									 strbw, m_strLength, s1stSpace, strhf, m_strLength, s2ndSpace, strMu, m_strMoment);          m_arText.Add(strBdcy);
		strBdcy = _T(""); m_arText.Add(strBdcy);
	}
	else if(m_iDgnCode==KSCE_RAIL_USD04)
	{
		strBdcy.Format(_T("~ ~ f_ck : ~ %s %s %s b ~ ~ : ~ %s %s %s A_ps : ~ %s%s"),
									 strfck, m_sTxStress, s1stSpace , strb, m_strLength, s2ndSpace, strAps, m_sTxArea);                    m_arText.Add(strBdcy);
		strBdcy.Format(_T("~ ~ f_y ~ : ~ %s %s %s d_p ~ : ~ %s %s %s A_s ~ : ~ %s%s"),
									 strfy, m_sTxStress, s1stSpace, strdp, m_strLength, s2ndSpace, strAst, m_sTxArea);             m_arText.Add(strBdcy);

		strBdcy.Format(_T("~ ~ f_pu : ~ %s %s %s d ~ ~ : ~ %s %s %s A_c ~ : ~ %s %s"),
									 strfpu, m_sTxStress, s1stSpace, strd, m_strLength, s2ndSpace, strAc, m_sTxArea);    m_arText.Add(strBdcy);
		strBdcy.Format(_T("~ ~ β_1 : ~ %s %s y_t ~ : ~ %s %s %s I_y ~ : ~ %s %s"),
									 strbeta1, s1stSpace, stryt, m_strLength, s2ndSpace, strIy, m_sTxStif);     m_arText.Add(strBdcy);

		strBdcy.Format(_T("~ ~ γ_p : ~ %s %s e_p ~ : ~ %s %s %s P_e ~ : ~ %s %s"),
									 strrp, s1stSpace, strep, m_strLength, s2ndSpace, strPe, m_strForce);    m_arText.Add(strBdcy);
		strBdcy.Format(_T("~ ~ b_w ~ : ~ %s %s ~ ~ ~ %s h_f ~ : ~ %s %s %s M_u ~ : ~ %s %s"), 
									 strbw, m_strLength, s1stSpace, strhf, m_strLength, s2ndSpace, strMu, m_strMoment);          m_arText.Add(strBdcy);
		strBdcy = _T(""); m_arText.Add(strBdcy);
	}

}

CString CPrtKSCE_USD05::Get_TextStringLength(int iMaxLength, CString strText)
{
	CString str = strText;
	int iMaxStrLength = Get_XlLength(iMaxLength);
	int iLength = strText.GetLength();
	int iStrLength = Get_XlLength(iLength);
	
	str += _T(" ");
	int iCount = iMaxStrLength - iStrLength;
	for(int i=0; i<iCount; i++)
	{
		str += _T("~ ");
	}

	str += _T("~ ");

	if(strText.GetLength()<=3) str += _T("~ ");

	return str;
}

int CPrtKSCE_USD05::Get_XlLength(int iLength)
{
	int iLen=0;
	if(iLength==10 || iLength==11)    iLen = 7;
	else if(iLength==9 || iLength==8) iLen = 6;
	else if(iLength==7)               iLen = 5;
	else if(iLength==6 || iLength==5) iLen = 4;
	else if(iLength==4 || iLength==3) iLen = 3;
	else if(iLength==2)               iLen = 2;

	return iLen;
}

void CPrtKSCE_USD05::Print_BdcyDesign1(double dfck, double db, double dAps , double dfy, double ddp, double dAst)
{
	CString sfck = Check_ValueForm(6,1, dfck);
	CString sb   = Check_ValueForm(7,1, db);
	CString sAps = Check_ValueForm(7,1, dAps);
	CString sfy  = Check_ValueForm(6,1, dfy);
	CString sdp  = Check_ValueForm(7,1, ddp);
	CString sAst = Check_ValueForm(7,1, dAst);

	CString sSpace5 = _T("~ ~ ~ ~ ~");
	CString sSpace6 = _T("~ ~ ~ ~ ~ ~");
	CString sbSpace  = _T("");

	int isbLen = sb.GetLength();
	if(isbLen>6) sbSpace = sSpace5;
	else         sbSpace = sSpace6;
	CString strBdcy;
	
	//설계 변수.
	strBdcy = _T("Text: ① 설계 변수");                                                         m_arText.Add(strBdcy);
	strBdcy.Format(_T("~ ~ f_ck : ~ %s ~ ~ %s ~ ~ ~ ~ ~ b ~ : ~ %s ~ %s &s A_ps : ~ %s ~ ~ ~ %s"),
								 sfck, m_sTxStress, sb , m_strLength, sbSpace, sAps, m_sTxArea);                    m_arText.Add(strBdcy);
	strBdcy.Format(_T("~ ~ f_y ~ : ~ %s ~ %s ~ ~ ~ ~ ~ d_p : ~ %s ~ %s ~ ~ ~ ~ ~ ~ A_s ~ : ~ %s ~ ~ ~ %s"),
								 sfy, m_sTxStress, sdp, m_strLength, sAst, m_sTxArea);             m_arText.Add(strBdcy);  

}

void CPrtKSCE_USD05::Print_BdcyDesign2(double dfpu, double dd, double dAc , double dIy, double dyt, double dPe)
{  
	CString sfpu = Check_ValueForm(7,1, dfpu);
	CString sd   = Check_ValueForm(7,1, dd);
	CString sAc  = Check_ValueForm(8,1, dAc);
	CString sIy  = Check_ValueForm(9,3, dIy);
	CString syt  = Check_ValueForm(7,1, dyt);
	CString sPe  = Check_ValueForm(9,3, Cng_CodeForFix(dPe));
	
	CString strBdcy;
	strBdcy.Format(_T("~ ~ f_pu : ~ %s ~ %s ~ ~ ~ ~ ~ d ~ : ~ %s ~ %s ~ ~ ~ ~ ~ ~ A_c ~ : ~ %s ~ ~ %s"),
								 sfpu, m_sTxStress, sd, m_strLength, sAc, m_sTxArea);    m_arText.Add(strBdcy);
	strBdcy.Format(_T("~ ~ I_y ~ : ~ %s %s ~ ~ ~ ~ ~ y_t : ~ %s ~ %s ~ ~ ~ ~ ~ ~ P_e ~ : ~ %s %s"),
								 sIy, m_sTxStif, syt, m_strLength, sPe, m_strForce);     m_arText.Add(strBdcy);
}

void CPrtKSCE_USD05::Print_BdcyDesign3(double dbeta1, double dep, double dMu , double drp, double dhf)
{
	CString sbeta1 = Check_ValueForm(6,3, dbeta1);
	CString sep    = Check_ValueForm(7,1, dep);
	CString sMu    = Check_ValueForm(8,1, Cng_CodeMomFix(dMu));
	CString srp    = Check_ValueForm(6,2, drp);
	CString shf    = Check_ValueForm(7,1, dhf);
 
	CString strBdcy;
	strBdcy.Format(_T("~ ~ β_1 : ~ %s  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ e_p : ~ %s ~ %s ~ ~ ~ ~ ~ ~ M_u ~ : ~ %s ~ ~ ~ %s"),
								 sbeta1, sep, m_strLength, sMu, m_strMoment);    m_arText.Add(strBdcy);
	strBdcy.Format(_T("~ ~ γ_p : ~ %s ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ t ~ : ~ %s ~ %s"), srp, shf, m_strLength);          m_arText.Add(strBdcy);
	strBdcy = _T(""); m_arText.Add(strBdcy);
}

void CPrtKSCE_USD05::Print_BdcyDesign4(double dc, double dbeta1, double da , double dfck, double dAa, double dCc)
{
	CString sc     = Check_ValueForm(7,1, dc);
	CString sbeta1 = Check_ValueForm(6,3, dbeta1);
	CString sa     = Check_ValueForm(7,1, da);
	CString sfck   = Check_ValueForm(7,1, dfck);
	CString sAa;   sAa.Format(_T("%.1lf"), dAa); // Check_ValueForm(9,1, dAa);
	CString sCc    = Check_ValueForm(9,1, Cng_CodeForFix(dCc));

	CString strBdcy;
	CString strEqua;
	strBdcy = _T("Text: (2) 힘의 평형 조건으로  중립축 c 결정");       m_arText.Add(strBdcy);
	strBdcy.Format(_T("~ ~ c ~ == ~ %s %s"), sc, m_strLength);                                           m_arText.Add(strBdcy);
	m_arText.Add(_T(""));

	strBdcy = _T("Text: (3) 등가 직사각형 응력블록 깊이");       m_arText.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ Equal_a</Title> <Col>6</Col> </Equation>");
	strBdcy.Format(_T("%s ~ == ~ %s * %s = %s ~ %s"),strEqua, sbeta1, sc, sa, m_strLength);          m_arText.Add(strBdcy);
	m_arText.Add(_T(""));
	m_iFormula++;

	strBdcy = _T("Text: (4) 등가 직사각형 응력블록 깊이의 압축력");       m_arText.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ Cc</Title> <Col>10</Col> </Equation>");
	strBdcy.Format(_T("%s ~ == 0.85 * %s * %s / 10^3 ~ = %s ~ %s"),strEqua, sfck, sAa , sCc, m_strForce);   m_arText.Add(strBdcy);
	strBdcy = _T(""); m_arText.Add(strBdcy);
	m_iFormula++;
 
}

void CPrtKSCE_USD05::Print_BdcyDesign5(CArray<CString,CString>& strTndn, CArray<double,double>* aData, double dTp, double dTpTimesep, double dPeTimesep)
{
	int i=0, j=0;
	int TndnNum = aData[0].GetSize();
	
	int iCol = 8;
	CArray<CString,CString> sData[9]; 
	for(i=0; i<iCol; i++)
		sData[i].RemoveAll();
	
	for(i=0; i<TndnNum; i++)
	{
		CString strTemp;
		strTemp = Check_ValueForm(6,1, Cng_CodeForFix(aData[0].GetAt(i)) );  sData[0].Add(strTemp); // Pei
		strTemp = Check_ValueForm(6,1, aData[1].GetAt(i));               sData[1].Add(strTemp);     // Apsi
		strTemp = Check_ValueForm(6,1, aData[2].GetAt(i));               sData[2].Add(strTemp);     // dpi
		strTemp = Check_ValueForm(6,1, aData[3].GetAt(i));               sData[3].Add(strTemp);     // epi
		strTemp = Check_ValueForm(6,4, aData[4].GetAt(i));               sData[4].Add(strTemp);     // epsi_ps
		strTemp = Check_ValueForm(7,1, aData[5].GetAt(i));               sData[5].Add(strTemp);     // fps
		strTemp = Check_ValueForm(6,1, Cng_CodeForFix(aData[6].GetAt(i)) );  sData[6].Add(strTemp); // Tpi
		strTemp = Check_ValueForm(6,1, Cng_CodeMomFix(aData[7].GetAt(i)) );  sData[7].Add(strTemp); // Tpi*epi
		strTemp = Check_ValueForm(6,1, Cng_CodeMomFix(aData[8].GetAt(i)) );  sData[8].Add(strTemp); // Pei*epi        
	}
	CString sTp         = Check_ValueForm(8,1, Cng_CodeForFix(dTp));
	CString sTpTimesep  = Check_ValueForm(8,1, Cng_CodeMomFix(dTpTimesep));
	CString sPeTimesep  = Check_ValueForm(8,1, Cng_CodeMomFix(dPeTimesep));
		
	CString strBdcy;
	CString strEqua;
	CString str1, str2, str3;

	strBdcy = _T("<Table>(5) PS 강재의 응력");    m_arText.Add(strBdcy);
	str1 = _T("<MH>TnGroup ~ ~ ~ ~ ~ ~</MH>   <MH>Pei ~ ~ ~ ~ ~</MH>   <MH>Apsi ~ ~ ~ ~ ~</MH>   ");
	str2 = _T("<MH>dpi ~ ~ ~ ~ ~</MH>   <MH>epi ~ ~ ~ ~ ~</MH>   <MH>εps ~ ~ ~ ~ ~</MH>   <MH>fpsi ~ ~ ~ ~ ~</MH>   ");
	str3 = _T("<MH>Apsi·fpsi ~ ~ ~ ~ ~</MH>   <MH>Tpi·epi ~ ~ ~ ~ ~</MH>   ");
	strBdcy = str1 + str2 + str3; m_arText.Add(strBdcy);
	if(TndnNum>0)
	{
		for( i=0; i<TndnNum; i++)
		{
			str1.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   "),strTndn.GetAt(i) ,sData[0].GetAt(i),sData[1].GetAt(i) ); 
			str2.Format(_T("<MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   "),
									sData[2].GetAt(i), sData[3].GetAt(i), sData[4].GetAt(i), sData[5].GetAt(i));
			str3.Format(_T("<MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   "),
									sData[6].GetAt(i), sData[7].GetAt(i) ); 
			strBdcy = str1 + str2 + str3;  m_arText.Add(strBdcy);
		}
	}
	else
	{
			str1.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   "),_T("-") ,_T("-"),_T("-") ); 
			str2.Format(_T("<MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   "),
									_T("-"), _T("-"), _T("-"), _T("-"));
			str3.Format(_T("<MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   "),
									_T("-"), _T("-") );
			strBdcy = str1 + str2 + str3;  m_arText.Add(strBdcy);
	}
	strBdcy = _T("</Table>");  m_arText.Add(strBdcy);

	strEqua = _T("<Equation> <Title>~ ~ T_p</Title> <Col>10</Col> </Equation>");
	strBdcy.Format(_T("%s ~ == ~ %s ~ %s"),strEqua, sTp, m_strForce);             m_arText.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ Tpep</Title> <Col>10</Col> </Equation>");
	strBdcy.Format(_T("%s ~ == ~ %s ~ %s"),strEqua, sTpTimesep, m_strMoment);      m_arText.Add(strBdcy);
	m_arText.Add(_T(""));
	//strEqua = _T("<Equation> <Title>~ ~ Peep</Title> <Col>10</Col> </Equation>");
	//strBdcy.Format(_T("%s ~ == ~ %s ~ %s"),strEqua, sPeTimesep, m_strMoment);      m_arText.Add(strBdcy);
	
}

void CPrtKSCE_USD05::Print_BdcyDesign6(double dep_cu, double ddst, double dc , double dep_s, double dfs, double dfs_cal, double dEs, double dfy,
																			 double dAst, double dTs)
{
	CString sep_cu = Check_ValueForm(6,4, dep_cu);
	CString sdst   = Check_ValueForm(7,1, ddst);
	CString sc     = Check_ValueForm(7,1, dc);
	CString sep_s  = Check_ValueForm(8,6, dep_s);
	CString sfs    = Check_ValueForm(7,1, dfs);
	CString sfs_cal= Check_ValueForm(7,1, dfs_cal);
	CString sEs    = Check_ValueForm(8,1, dEs);
	CString sfy    = Check_ValueForm(7,1, dfy);
	CString sAst   = Check_ValueForm(9,1, dAst);
	CString sTs    = Check_ValueForm(9,1, Cng_CodeForFix(dTs));
	
	CString strBdcy;
	CString strEqua;
	
	strBdcy = _T("Text: (6) 인장철근의 인장응력");       m_arText.Add(strBdcy);
	if(dAst==0.0)
	{
		strBdcy.Format(_T("~ ~ T_s == A_s ·  f_s == %s * %s / 10^3 = %s ~ %s"), sAst, sfs, sTs, m_strForce);     m_arText.Add(strBdcy);
	}
	else
	{
		strEqua = _T("<Equation> <Title>~ ~ epsi_s</Title> <Col>10</Col> </Equation>");      m_arText.Add(strEqua);
		strBdcy = _T("");   m_arText.Add(strBdcy);
		m_iFormula++;
		strBdcy.Format(_T("~ ~ ~ == %s * ( %s - %s ) / %s = %s"), sep_cu, sdst, sc, sc, sep_s);    m_arText.Add(strBdcy);
		strBdcy = _T("");   m_arText.Add(strBdcy);

		strBdcy = _T("<Equation> <Title>~ ~ fs</Title> <Col>0</Col> </Equation>");    m_arText.Add(strBdcy);
		strBdcy = Get_fsPrint(dfs_cal, dep_s, dEs, dfy);                          m_arText.Add(strBdcy);
		strBdcy = _T("");   m_arText.Add(strBdcy);
		m_iFormula++;

		strBdcy.Format(_T("~ ~ T_s == A_s ·  f_s == %s * %s / 10^3 = %s ~ %s"), sAst, sfs, sTs, m_strForce);     m_arText.Add(strBdcy);
	}
	m_arText.Add(_T(""));

}

void CPrtKSCE_USD05::Print_BdcyDesign7(double dTp, double dTs, double dCc , double dDelC, double dTol)
{
	CString strBdcy;
	CString strEqua;

	strEqua = _T("<Equation> <Title>~ ~ Delta</Title> <Col>0</Col> </Equation>");    m_arText.Add(strEqua);
	strBdcy = Get_DeltaCPrint(dTp, dTs, dCc, dDelC, dTol);      m_arText.Add(strBdcy);
	strBdcy = _T(""); m_arText.Add(strBdcy);
	m_iFormula++;
}

void CPrtKSCE_USD05::Print_BdcyDesign8(double dCc, double dc, double da , double da_cent, double dMps, double dTs, 
																			 double ddst, double dMn, double dpMn, double dMu)
{
	CString sCc     = Check_ValueForm(7,1, Cng_CodeForFix(dCc));
	CString sc      = Check_ValueForm(7,1, dc);
	CString sa      = Check_ValueForm(7,1, da);
	CString sa_cent = Check_ValueForm(7,1, da_cent);
	CString sMps    = Check_ValueForm(8,2, Cng_CodeMomFix(dMps));
	CString sTs     = Check_ValueForm(8,1, Cng_CodeForFix(dTs));
	CString sdst    = Check_ValueForm(7,1, ddst);
	CString sMn     = Check_ValueForm(8,2, Cng_CodeMomFix(dMn));
	CString sMu     = Check_ValueForm(8,2, Cng_CodeMomFix(dMu));

	CString strBdcy;
	CString strEqua;

	strBdcy = _T("Text: (7) 휨강도");         m_arText.Add(strBdcy); //③
	//Insert Eq.
	strEqua = _T("<Equation> <Title>~ Mn</Title> <Col>0</Col> </Equation>");    m_arText.Add(strEqua);
	strBdcy.Format(_T("~ ~ ~ == %s * ( %s - ( %s - %s ) ) / 10^3 + %s + %s * ( %s - %s ) / 10^3 ~ = %s ~ %s"),
									sCc, sc, sa, sa_cent, sMps, sTs, sdst, sc, sMn, m_strMoment);  m_arText.Add(strBdcy);
	m_iFormula++;
	strBdcy = _T(""); m_arText.Add(strBdcy);
	strBdcy = Get_pMnPrint(dpMn, dMu);      m_arText.Add(strBdcy);
	strBdcy = _T(""); m_arText.Add(strBdcy);

}

void CPrtKSCE_USD05::Print_BdcyDesign9(double da, double dt, double dRhop, double dfps, double dfck , double dqp, double d036Beta1)
{
	CString sa = Check_ValueForm(7,1, da);
	CString st = Check_ValueForm(7,1, dt);

	CString strBdcy;
	CString strEqua;

	//최대 강재량.
	strBdcy = _T("Text: (8) 최대 PS강재량");         m_arText.Add(strBdcy); //④
	strBdcy.Format(_T("~ a == %s %s ~ < ~ t == %s %s"),sa, m_strLength, st, m_strLength);     m_arText.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ qp_in</Title> <Col>0</Col> </Equation>");               m_arText.Add(strEqua);
	m_arText.Add(_T(""));
	m_iFormula++;
	strBdcy = Get_MaxPsPrint(dRhop, dfps, dfck, dqp, d036Beta1);   m_arText.Add(strBdcy);
	m_arText.Add(_T(""));
}
	
void CPrtKSCE_USD05::Print_BdcyDesign91(double da, double dt, double dApw, double dAp, double dApf, double dAs, double db, double dbw, double ddp, double dfck, double dfps, double dfy, double dqp, double d036beta1)
{
	CString sa   = Check_ValueForm(7,1, da);
	CString st   = Check_ValueForm(7,1,dt);
	CString sApw = Check_ValueForm(7,1,dApw);
	CString sAp  = Check_ValueForm(7,1,dAp);
	CString sApf = Check_ValueForm(7,1,dApf);
	CString sAs  = Check_ValueForm(7,1,dAs);
	CString sb   = Check_ValueForm(7,1,db);
	CString sbw  = Check_ValueForm(7,1,dbw);
	CString sdp  = Check_ValueForm(7,1,ddp);
	CString sfck = Check_ValueForm(7,1,dfck);
	CString sfps = Check_ValueForm(7,1,dfps);
	CString sfy  = Check_ValueForm(7,1,dfy);
	CString sqp  = Check_ValueForm(7,5,dqp);
	CString s036beta1 = Check_ValueForm(6,4,d036beta1);

	CString strBdcy;
	CString strEqua;

	//최대 강재량.
	strBdcy = _T("Text: (8) 최대 PS강재량");         m_arText.Add(strBdcy); //④
	strBdcy.Format(_T("~ a == %s %s ~ > ~ t == %s %s"),sa, m_strLength, st, m_strLength);  m_arText.Add(strBdcy);

	strEqua = _T("<Equation> <Title>~ Apf</Title> <Col>0</Col> </Equation>");             m_arText.Add(strEqua);  
	strBdcy.Format(_T("~ ~ ~ == 0.85 * %s * ( %s - %s ) * %s / %s = %s ~ %s"),
								 sfck, sb, sbw, st, sfps, sApf, m_sTxArea);                        m_arText.Add(strBdcy);
	m_iFormula++;

	if(dAs>0.0)
	{
		strEqua = _T("<Equation> <Title>~ Apw2</Title> <Col>15</Col> </Equation>");          m_arText.Add(strEqua);
		strBdcy.Format(_T("~ ~ ~ == %s + ( %s * %s / %s ) - %s = %s ~ %s"),
									 sAp, sAs, sfy, sfps, sApf, sApw, m_sTxArea);                     m_arText.Add(strBdcy);
		m_iFormula++;
	}
	else if(dAs==0.0)
	{
		strEqua = _T("<Equation> <Title>~ Apw1</Title> <Col>9</Col> </Equation>");       
		strBdcy.Format(_T("%s ~ == %s - %s = %s ~ %s"), strEqua, sAp, sApf, sApw, m_sTxArea);   m_arText.Add(strBdcy);
		m_iFormula++;
	}
	m_arText.Add(_T(""));
	
	//strBdcy = _T("~ ~ A_pf == 0.85 · f_ck ·  ( b - b_w ) ·  t / f_ps");  m_arText.Add(strBdcy);
	//strBdcy.Format(_T(" ~ ~ ~ == 0.85 * %s * ( %s - %s ) *%s / %s = %s ~ %s"), sfck, sb, sbw, st, sfps, sApf, m_sTxArea);    m_arText.Add(strBdcy); 
	//strBdcy.Format(_T("~ ~ A_pw == A_p - A_pf == %s - %s = %s"),sAp, sApf, sApw);            m_arText.Add(strBdcy); 
	
	CString sUnequal = _T(""), sJudge = _T("");
	if(dqp <= d036beta1) { sUnequal=_T("≤"); sJudge=_T("O.K"); }
	else                 { sUnequal=_T("＞"); sJudge=_T("N.G"); }
	strEqua = _T("<Equation> <Title>~ qp_out</Title> <Col>0</Col> </Equation>");   m_arText.Add(strEqua);
	m_arText.Add(_T(""));
	m_iFormula++;
	strBdcy.Format(_T("~ ~ ~ == %s / ( %s * %s ) * %s / %s = %s ~ %s  ~ %s ~ ~ ~ ~ ~ ~ ~ %s"),
								 sApw, sb, sbw , sfps, sfck, sqp , sUnequal, s036beta1, sJudge);  m_arText.Add(strBdcy);
	m_arText.Add(_T(""));

}

void CPrtKSCE_USD05::Print_BdcyDesign10(BOOL bIter, double dPe, double dAc, double dPeTimesep , double dyt, double dIy, double dfpe, 
																				double dfck, double dpMn, double d12Mcr, double dMu)
{
	CString sPe;        sPe.Format(_T("%.1lf"),dPe); // Check_ValueForm(9,1, dPe);
	CString sAc;        sAc.Format(_T("%.1lf"),dAc); // Check_ValueForm(9,1, dAc);
	CString sPeTimesep; sPeTimesep.Format(_T("%.1lf"),dPeTimesep); // Check_ValueForm(9,1, dPeTimesep);
	CString syt        = Check_ValueForm(7,1, dyt);
	CString sIy;        sIy.Format(_T("%.1lf"),dIy); // Check_ValueForm(9,3, dIy);
	CString sfpe       = Check_ValueForm(7,2, dfpe);
	CString sfck       = Check_ValueForm(7,1, dfck);
	CString spMn       = Check_ValueForm(8,2, Cng_CodeMomFix(dpMn));
	CString s12Mcr     = Check_ValueForm(8,2, Cng_CodeMomFix(d12Mcr));
	CString sMu        = Check_ValueForm(8,2, Cng_CodeMomFix(dMu));

	CString strBdcy;
	CString strEqua;

	//최소 강재량
	if(bIter)
	{
		strBdcy = _T("Text: (9) 최소 강재량");         m_arText.Add(strBdcy); //⑤
	}
	else 
	{
		strBdcy = _T("Text: (7) 최소 강재량");         m_arText.Add(strBdcy); 
	}
	//Insert Eq.
	//strBdcy = _T("<Equation> <Title>~ qp_out</Title> <Col>0</Col> </Equation>");   m_arText.Add(strBdcy);
	if(m_iDgnCode==KSCE_USD05)
	{
		strEqua = _T("<Equation> <Title>~ fpcc</Title> <Col>0</Col> </Equation>");   m_arText.Add(strEqua);
		strBdcy = _T(""); m_arText.Add(strBdcy);
		strBdcy.Format(_T("~ ~ ~ == %s / %s + %s * %s / %s = %s ~ %s"),
										sPe, sAc, sPeTimesep, syt, sIy, sfpe, m_sTxStress);  m_arText.Add(strBdcy);
		strBdcy = _T(""); m_arText.Add(strBdcy);
		m_iFormula++;

		//Insert Eq.
		strEqua = _T("<Equation> <Title>~ bMcr_eq_KS05</Title> <Col>0</Col> </Equation>");   m_arText.Add(strEqua);
		strBdcy.Format(_T("~ ~ ~ == ( 0.63 * √  ( %s ) + %s ) * ( %s / %s ) / 10^6 = %s ~ %s"),
										sfck, sfpe, sIy, syt, s12Mcr, m_strMoment);          m_arText.Add(strBdcy);
		m_iFormula++;
	}
	else if(m_iDgnCode==KSCE_RAIL_USD04)
	{
		strEqua = _T("<Equation> <Title>~ fpe</Title> <Col>0</Col> </Equation>");   m_arText.Add(strEqua);
		strBdcy = _T(""); m_arText.Add(strBdcy);
		m_iFormula++;
		strBdcy.Format(_T("~ ~ ~ == %s / %s + %s * %s / %s = %s ~ %s"),
										sPe, sAc, sPeTimesep, syt, sIy, sfpe, m_sTxStress);  m_arText.Add(strBdcy);
		strBdcy = _T(""); m_arText.Add(strBdcy);

		//Insert Eq.
		strEqua = _T("<Equation> <Title>~ bMcr_eq_rail</Title> <Col>0</Col> </Equation>");   m_arText.Add(strEqua);
		strBdcy.Format(_T("~ ~ ~ == ( 0.63 * √  ( %s ) + %s ) * ( %s / %s ) / 10^6 = %s ~ %s"),
										sfck, sfpe, sIy, syt, s12Mcr, m_strMoment);          m_arText.Add(strBdcy);
		m_iFormula++;
	}
	m_arText.Add(_T(""));
	
	CString strTemp = _T("");
	CString sUnequal = _T(""), sJudge = _T("");
	
	if(dpMn >= d12Mcr) { sUnequal=_T("≥"); sJudge=_T("O.K"); }
	else               { sUnequal=_T("＜"); sJudge=_T("N.G"); }
 
	strBdcy.Format(_T("~ ~ Φ  M_n == %s %s ~ %s  ~ 1.2 ~ M_cr == %s ~ %s ~ ~ ~ ~ ~ ~ ~ %s"),spMn, m_strMoment, sUnequal, s12Mcr, m_strMoment, sJudge); m_arText.Add(strBdcy);
	
	if(m_iDgnCode==KSCE_USD05)
	{
		if(dpMn < d12Mcr)
		{
			m_arText.Add(_T(""));
			double d43Mu = 4.0/3.0*dMu;
			CString s43Mu = Check_ValueForm(8,2, Cng_CodeMomFix(d43Mu));
			if(dpMn>fabs(d43Mu))
			{
				if(dMu<0.0)
				{
					strBdcy.Format(_T("~ ~ Φ  M_n == %s %s ~  ≥  ~ 4/3  M_u == | %s | ~ %s "),spMn, m_strMoment, s43Mu, m_strMoment);   m_arText.Add(strBdcy);
				}
				else
				{
					strBdcy.Format(_T("~ ~ Φ  M_n == %s %s ~  ≥  ~ 4/3  M_u == %s ~ %s "),spMn, m_strMoment, s43Mu, m_strMoment);   m_arText.Add(strBdcy);
				}
				m_arText.Add(_T(""));
				strBdcy = _T("~ ~ ~ * 최소 강재량 규정을 따르지 않아도 됨.");  m_arText.Add(strBdcy);
			}
		}
	}

	m_arText.Add(_T(""));
	m_arText.Add(_T(""));
}
	
CString CPrtKSCE_USD05::Get_fsPrint(double dfs, double depsi, double dEs, double dfy)
{
	CString sfs   = Check_ValueForm(6,1, dfs);
	CString sepsi = Check_ValueForm(6,4, depsi);
	CString sEs   = Check_ValueForm(6,1, dEs);
	CString sfy   = Check_ValueForm(6,1, dfy);
	
	CString strTemp = _T("");
	if(dfs<=dfy)
	{
		strTemp.Format(_T("~ ~ ~ == %s ~ * %s = %s ~ ≤  ~ f_y == %s ~ ~ ~ ~ ∴  ~ f_s == %s ~ %s") ,
									 sepsi, sEs, sfs, sfy, sfs, m_sTxStress);
	}
	else
	{
		strTemp.Format(_T("~ ~ ~ == %s ~ * %s = %s ~ ＞  ~ f_y == %s ~ ~ ~ ~ ∴  ~ f_s == %s ~ %s") ,
									sepsi, sEs, sfs, sfy, sfy, m_sTxStress);
	}  
	return strTemp;

}

CString CPrtKSCE_USD05::Get_DeltaCPrint(double dTp, double dTs, double dCc, double dDelC, double dTol)
{
	CString sTp   = Check_ValueForm(7,1, Cng_CodeForFix(dTp));
	CString sTs   = Check_ValueForm(7,1, Cng_CodeForFix(dTs));
	CString sCc   = Check_ValueForm(7,1, Cng_CodeForFix(dCc));
	CString sDelC = Check_ValueForm(8,6, Cng_CodeForFix(dDelC));
	CString sTol  = Check_ValueForm(8,6, Cng_CodeForFix(dTol));
	
	CString strTemp = _T(""); 
	if(dDelC<dTol) 
	{
		strTemp.Format(_T("~ ~ ~ ~ == %s + %s - %s = %s ~ ≤  ~ %f"),sTp, sTs, sCc, sDelC, sTol);   
	}
	else
	{
		strTemp.Format(_T("~ ~ ~ ~ == %s + %s - %s = %s ~ ＞  ~ %f ~ ~ ~ ~ 중립축이  수렴하지  않았습니다."),sTp, sTs, sCc, sDelC, sTol);     
	}
	
	return strTemp;
}

CString CPrtKSCE_USD05::Get_pMnPrint(double dpMn, double dMu)
{
	CString spMn = Check_ValueForm(8,2, Cng_CodeMomFix(dpMn));
	CString sMu = Check_ValueForm(8,2, Cng_CodeMomFix(dMu));
	
	CString strTemp = _T("");
	CString sUnequal = _T(""), sJudge = _T("");
	
	if(dpMn >= fabs(dMu)) { sUnequal=_T("≥"); sJudge=_T("O.K"); }
	else            { sUnequal=_T("＜"); sJudge=_T("N.G"); }
 
	if(dMu<0.0)  strTemp.Format(_T("~ ~ Φ  M_n == %s %s ~ %s  ~ M_u == | %s | ~ %s ~ ~ ~ ~ ~ ~ ~ %s"),spMn, m_strMoment, sUnequal, sMu, m_strMoment, sJudge);
	else         strTemp.Format(_T("~ ~ Φ  M_n == %s %s ~ %s  ~ M_u == %s ~ %s ~ ~ ~ ~ ~ ~ ~ %s"),spMn, m_strMoment, sUnequal, sMu, m_strMoment, sJudge);
		
	return strTemp;
}

CString CPrtKSCE_USD05::Get_MaxPsPrint(double drhop, double dfps, double dfck, double dqp, double d036beta1)
{
	CString srhop     = Check_ValueForm(6,5, drhop);
	CString sfps      = Check_ValueForm(8,1, dfps);
	CString sfck      = Check_ValueForm(6,1, dfck);
	CString sqp       = Check_ValueForm(6,5, dqp);
	CString s036beta1 = Check_ValueForm(6,2, d036beta1);
 
	CString strTemp = _T("");
	CString sUnequal = _T(""), sJudge = _T("");
	
	if(dqp <= d036beta1) { sUnequal=_T("≤"); sJudge=_T("O.K"); }
	else                 { sUnequal=_T("＞"); sJudge=_T("N.G"); }
 
	strTemp.Format(_T("~ ~ ~ == %s ~ * ( %s / %s ) = %s ~ %s  ~ %s ~ ~ ~ ~ ~ ~ ~ %s"),
								 srhop, sfps, sfck, sqp, sUnequal, s036beta1, sJudge);
		
	return strTemp;
}

CString CPrtKSCE_USD05::Get_MinPsPrint(double dpMn, double d12Mcr)
{
	CString spMn   = Check_ValueForm(8,2, Cng_CodeMomFix(dpMn));
	CString s12Mcr = Check_ValueForm(8,2, Cng_CodeMomFix(d12Mcr));

	CString strTemp = _T("");
	CString sUnequal = _T(""), sJudge = _T("");
	
	if(dpMn >= d12Mcr) { sUnequal=_T("≥"); sJudge=_T("O.K"); }
	else            { sUnequal=_T("＜"); sJudge=_T("N.G"); }
 
	strTemp.Format(_T("~ ~ Φ  M_n == %s %s ~ %s  ~ 1.2 ~ M_cr == %s ~ %s ~ ~ ~ ~ ~ ~ ~ %s"),spMn, m_strMoment, sUnequal, s12Mcr, m_strMoment, sJudge);
		
	return strTemp;
}

// Code에 따른 계산서.
void CPrtKSCE_USD05::Print_BdcyDesign_Code1(double dc, double dbeta1, double da , double dfck) 
{
	CString sc     = Check_ValueForm(7,1, dc);
	CString sbeta1 = Check_ValueForm(7,3, dbeta1);
	CString sa     = Check_ValueForm(7,1, da);
	CString sfck   = Check_ValueForm(8,2, dfck);  
	
	CString strBdcy;
	CString strEqua;
	strBdcy = _T("Text: (2) 힘의 평형 조건으로  중립축 c 결정");       m_arText.Add(strBdcy);
	strBdcy.Format(_T("~ ~ c ~ == ~ %s %s"), sc, m_strLength);                                           m_arText.Add(strBdcy);
	m_arText.Add(_T(""));
	strBdcy = _T("Text: (3) 등가 사각형의 응력블록의 깊이");       m_arText.Add(strBdcy);
	strEqua = _T("<Equation> <Title>~ ~ Equal_a</Title> <Col>6</Col> </Equation>");
	strBdcy.Format(_T("%s ~ == %s * %s = %s ~ %s"),strEqua, sbeta1, sc, sa, m_strLength);          m_arText.Add(strBdcy);
	m_arText.Add(_T(""));   
	m_iFormula++;
}

void CPrtKSCE_USD05::Print_BdcyDesign_Code2(double dfpu, double drp, double dbeta1 , double dRhop, double dfck,
																						double dd, double ddp, double dRho, double dRhoc, double dfy, double dfps)
{
	CString sfpu   = Check_ValueForm(8,2, dfpu  );
	CString srp    = Check_ValueForm(4,2, drp   );
	CString sbeta1 = Check_ValueForm(8,3, dbeta1);
	CString sRhop  = Check_ValueForm(8,5, dRhop );
	CString sfck   = Check_ValueForm(8,2, dfck  );
	CString sd     = Check_ValueForm(7,1, dd    );
	CString sdp    = Check_ValueForm(7,1, ddp   );
	CString sRho   = Check_ValueForm(8,5, dRho  );
	CString sRhoc  = Check_ValueForm(8,5, dRhoc );
	CString sfy    = Check_ValueForm(8,2, dfy   );
	CString sfps   = Check_ValueForm(8,2, dfps  );

	CString strBdcy;
	CString strEqua;
	strBdcy = _T("Text: (4) PS강재의 응력");        m_arText.Add(strBdcy); //②
	strBdcy = _T("Text:   - 부착된 부재");          m_arText.Add(strBdcy);
	if(ddp==0.0)
	{
		if(dd>0.0)
		{
			strEqua = _T("<Equation> <Title>~ fps_bond1_05</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
			m_arText.Add(_T(""));
			m_arText.Add(_T(""));
			m_iFormula++;
			strBdcy.Format(_T("~ ~ ~ == 0.0 ~ %s"), m_sTxStress);        m_arText.Add(strBdcy);                   
			m_arText.Add(_T("")); 
		}
		else if(dd==0.0)
		{
			strEqua = _T("<Equation> <Title>~ fps_bond2</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
			m_arText.Add(_T(""));
			m_arText.Add(_T(""));
			m_iFormula++;
			strBdcy.Format(_T("~ ~ ~ == 0.0 ~ %s"), m_sTxStress);        m_arText.Add(strBdcy);                   
		}
	}
	else
	{
		if(m_iDgnCode==KSCE_USD05)
		{
			if(dd>0.0)
			{
				strEqua = _T("<Equation> <Title>~ fps_bond1_05</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
				m_arText.Add(_T(""));
				m_arText.Add(_T(""));
				m_iFormula++;
				CString str1;
				str1.Format(_T("( %s * %s / %s + %s / %s * ( %s * %s / %s ) )"),
										sRhop, sfpu, sfck, sd, sdp, sRho, sfy, sfck);
				strBdcy.Format(_T("~ ~ ~ == %s * ( 1 - %s / %s * %s ) = %s ~ %s"), sfpu, srp, sbeta1, str1, sfps, m_sTxStress);        m_arText.Add(strBdcy);                   
				m_arText.Add(_T("")); 
			}
			else if(dd==0.0)
			{
				strEqua = _T("<Equation> <Title>~ fps_bond2</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
				m_arText.Add(_T(""));
				m_arText.Add(_T(""));
				m_iFormula++;
				strBdcy.Format(_T("~ ~ ~ == %s * ( 1 - %s / %s * ( %s * %s / %s ) ) = %s ~ %s"),
											 sfpu, srp, sbeta1, sRhop, sfpu, sfck, sfps, m_sTxStress);              m_arText.Add(strBdcy);
			}
		}
		else if(m_iDgnCode==KSCE_RAIL_USD04)
		{      
			strEqua = _T("<Equation> <Title>~ fps_bond_rail</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
			m_arText.Add(_T(""));
			m_arText.Add(_T(""));
			m_iFormula++;
			CString str1;
			str1.Format(_T("( %s * %s / %s + %s / %s * ( %s * %s / %s - %s * %s / %s ) )"), 
										sRhop, sfpu, sfck, sd, sdp, sRho, sfy, sfck, sRhoc, sfy, sfck);
			strBdcy.Format(_T("~ ~ ~ == %s * ( 1 - %s / %s * %s ) = %s ~ %s"), sfpu, srp, sbeta1, str1, sfps, m_sTxStress);        m_arText.Add(strBdcy);                   
			m_arText.Add(_T("")); 
		}
	}
	
}

void CPrtKSCE_USD05::Print_BdcyDesign_Code3(double dfck, double dfpu, double dfpy, double dfpe_cal, double dfpe, double d05fpu, double dfps, double dfps_cal, double dRhop, double dfps_unbCom, double dfps_un_max)
{
	CString sfck     = Check_ValueForm(8,2, dfck);
	CString sfpu     = Check_ValueForm(8,2, dfpu);
	CString sfpy     = Check_ValueForm(8,2, dfpy);
	CString sfpe     = Check_ValueForm(8,2, dfpe);
	CString sfpe_cal = Check_ValueForm(8,2, dfpe_cal);
	CString s05fpu   = Check_ValueForm(8,2, d05fpu);
	CString sfps     = Check_ValueForm(8,2, dfps);
	CString sfps_cal = Check_ValueForm(8,2, dfps_cal);
	CString sRhop    = Check_ValueForm(8,5, dRhop ); 
	CString sfps_unbCom = Check_ValueForm(8,2, dfps_unbCom);

	CString strBdcy;
	CString strEqua;
	strBdcy = _T("Text: (4) PS강재의 응력");                                     m_arText.Add(strBdcy); //②
	strBdcy = _T("Text:   - 부착되지 않은 부재");                                m_arText.Add(strBdcy);
	if(dfps==0.0)
	{
		strEqua = _T("<Equation> <Title>~ fps_unbond</Title> <Col>0</Col> </Equation>");  m_arText.Add(strEqua);
		strBdcy.Format(_T("~ ~ == 0.0 ~ %s"), m_sTxStress);                               m_arText.Add(strBdcy);
		m_iFormula++;
	}
	else
	{
		if(m_iDgnCode==KSCE_USD05)
		{
			if(dfpe_cal<d05fpu)
			{
				strBdcy.Format(_T("~ f_pe == %s ~ < ~ 0.5 f_pu == %s"), sfpe_cal, s05fpu);       m_arText.Add(strBdcy);
				strBdcy.Format(_T("~ ~ ∴ f_pe == %s ~ &s"), sfpe, m_sTxStress);                 m_arText.Add(strBdcy);
			}

			strEqua = _T("<Equation> <Title>~ fps_unbond</Title> <Col>0</Col> </Equation>");   m_arText.Add(strEqua);
			strBdcy.Format(_T("~ ~ == %s + 105 = %s ~ %s"), sfpe, sfps, m_sTxStress);          m_arText.Add(strBdcy);
			m_iFormula++;
		}
		else if(m_iDgnCode==KSCE_RAIL_USD04)
		{
			// (가) 지간과 높이의 비가 35이하인 경우. defualt (나). 항목 추가해야함. //!/
			strEqua = _T("<Equation> <Title>~ fps_unbond1_rail</Title> <Col>0</Col> </Equation>");                 m_arText.Add(strEqua);
			m_arText.Add(_T(""));       
			m_iFormula++;
			strBdcy.Format(_T("~ ~ == %s + %s / ( 100 * %s ) = %s ~ %s"), sfpe_cal, sfck, sRhop, sfps_cal, m_sTxStress);          m_arText.Add(strBdcy);
			if(dfps_cal>dfps_un_max)
			{
				strBdcy.Format(_T("~ f_ps 는 f_py == %s %s 또는 ( f_pe + 400 ) == %s %s 이하라야 한다."),
					sfpy, m_sTxStress, sfps_unbCom, m_sTxStress);    m_arText.Add(strBdcy);
				strBdcy.Format(_T("~ ∴ f_ps == %s ~ %s "), sfps, m_sTxStress);      m_arText.Add(strBdcy);
			}

		}
	}
	m_arText.Add(_T("")); 
}

void CPrtKSCE_USD05::Print_BdcyDesign_Code4(double da, double db, double dbw, double dApw, double dApf,
																						double dAp, double dAs, double dhf, double dfpu, double drp, double dbeta1,
																						double dRhop, double dfck, double dd, double ddp, double dRho, 
																						double dfy, double dfps, double dphi, double dMn, double dpMn, double dMu)
{
	CString sa     = Check_ValueForm(7,1, da    );
	CString sb     = Check_ValueForm(7,1, db    );
	CString sbw    = Check_ValueForm(7,1, dbw   );
	CString sApw   = Check_ValueForm(8,1, dApw  );
	CString sApf   = Check_ValueForm(8,1, dApf  );
	CString sAp    = Check_ValueForm(8,1, dAp   );
	CString sAs    = Check_ValueForm(8,1, dAs   );
	CString shf    = Check_ValueForm(7,1, dhf   );
	CString sfpu   = Check_ValueForm(8,2, dfpu  );
	CString srp    = Check_ValueForm(4,2, drp   );
	CString sbeta1 = Check_ValueForm(8,3, dbeta1);
	CString sRhop  = Check_ValueForm(8,5, dRhop );  
	CString sfck   = Check_ValueForm(8,2, dfck  );
	CString sd     = Check_ValueForm(7,1, dd    );
	CString sdp    = Check_ValueForm(7,1, ddp   );
	CString sRho   = Check_ValueForm(8,5, dRho  );
	CString sfy    = Check_ValueForm(8,2, dfy   );
	CString sfps   = Check_ValueForm(8,2, dfps  );
	CString sphi   = Check_ValueForm(6,2, dphi  );
	CString sMn    = Check_ValueForm(9,2, dMn   );
	CString spMn   = Check_ValueForm(9,2, dpMn  );
	CString sMu    = Check_ValueForm(9,2, dMu   );


	CString strBdcy;
	CString strEqua;
	CString str1, str2, str3;
	strBdcy = _T("Text: (5) 설계 휨강도 ");          m_arText.Add(strBdcy); //③
	if(ddp==0.0)
	{
		if(da<=dhf)
		{
			strBdcy.Format(_T("~ a == %s ~ < ~ t == %s ~ ~ ( 사각형`단면 )"), sa, shf);     m_arText.Add(strBdcy);
			m_arText.Add(_T(""));
			if(dAs>0.0)
			{
				strEqua = _T("<Equation> <Title>~ pMn_non_Ap_rec</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);        
				strBdcy.Format(_T("~ ~ ~ == %s * ( 0.85 * %s * %s * %s * ( %s - 0.5 * %s ) ) / 10^6 ~ = %s ~ %s"),
											sphi, sfck, sa, sb, sd, sa, spMn, m_strMoment);         m_arText.Add(strBdcy);
				m_iFormula++;
			}
			else ASSERT(0);
		}
		else if(da>dhf)
		{
			strBdcy.Format(_T("~ a == %s ~ > ~ t == %s ~ ~ ( 플랜지를`갖는`단면)"), sa, shf);   m_arText.Add(strBdcy);
			m_arText.Add(_T(""));

			if(dAs>0.0)
			{
				strEqua = _T("<Equation> <Title>~ pMn_non_Ap_flg</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
				m_arText.Add(_T(""));
				m_iFormula++;
				str1.Format(_T("~ ~ ~ == %s * ( 0.85 * %s * %s * %s * ( %s - 0.5 * %s )"),
											sphi, sfck, sa, sbw, sd, sa);
				str2.Format(_T(" + 0.85 * %s * ( %s - %s ) * %s * ( %s - 0.5 * %s ) ) / 10^6 ~ = %s ~ %s"),
											sfck, sb, sbw, shf, sd, shf, spMn, m_strMoment); 
				strBdcy = str1 + str2;    
				m_arText.Add(strBdcy);
			}
			m_arText.Add(_T(""));
		}
	}
	else
	{
		if(m_iDgnCode==KSCE_USD05)
		{
			if(da<=dhf)
			{
				strBdcy.Format(_T("~ a == %s ~ < ~ t == %s ~ ~ ( 사각형`단면 )"), sa, shf);     m_arText.Add(strBdcy);
				m_arText.Add(_T(""));
				if(dAs>0.0)
				{
					strEqua = _T("<Equation> <Title>~ pMn_rec</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
					m_arText.Add(_T(""));
					m_iFormula++;
					str1.Format(_T("( 1 - 0.59 * ( %s * %s / %s + %s / %s * ( %s * %s / %s ) ) )"),
												sRhop, sfps, sfck, sd, sdp, sRho, sfy, sfck);
					str2.Format(_T("( 1 - 0.59 * ( %s / %s * ( %s * %s / %s ) + %s * %s / %s ) )"),
												sdp, sd, sRhop, sfps, sfck,  sRho, sfy, sfck);
					strBdcy.Format(_T("~ ~ ~ == %s * ( %s * %s * %s * %s + %s * %s * %s * %s ) / 10^6 ~ = %s ~ %s"),
												sphi, sAp, sfps, sdp, str1, sAs, sfy, sd, str2, spMn, m_strMoment);         m_arText.Add(strBdcy);
				}
				else if(dAs==0.0)
				{
					strEqua = _T("<Equation> <Title>~ pMn_rec_nobar</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
					m_arText.Add(_T(""));
					m_iFormula++;
					str1.Format(_T("( 1 - 0.59 * %s * %s / %s )"),sRhop, sfps, sfck);                    
					strBdcy.Format(_T("~ ~ ~ == %s * ( %s * %s * %s * %s ) / 10^6 ~ = %s ~ %s"),
												sphi, sAp, sfps, sdp, str1, spMn, m_strMoment);                            m_arText.Add(strBdcy);
				}
			}
			else if(da>dhf)
			{
				strBdcy.Format(_T("~ a == %s ~ > ~ t == %s ~ ~ ( 플랜지를`갖는`단면)"), sa, shf);   m_arText.Add(strBdcy);
				m_arText.Add(_T(""));

				strEqua = _T("<Equation> <Title>~ Apf</Title> <Col>0</Col> </Equation>");             m_arText.Add(strEqua);  
				strBdcy.Format(_T("~ ~ ~ == 0.85 * %s * ( %s - %s ) * %s / %s = %s ~ %s"),
											 sfck, sb, sbw, shf, sfps, sApf, m_sTxArea);                        m_arText.Add(strBdcy);
				m_iFormula++;

				if(dAs>0.0)
				{
					strEqua = _T("<Equation> <Title>~ Apw2</Title> <Col>0</Col> </Equation>");          m_arText.Add(strEqua);
					strBdcy.Format(_T("~ ~ ~ == %s + ( %s * %s / %s ) - %s = %s ~ %s"),
												 sAp, sAs, sfy, sfps, sApf, sApw, m_sTxArea);                     m_arText.Add(strBdcy);
					m_iFormula++;
				}
				else if(dAs==0.0)
				{
					strEqua = _T("<Equation> <Title>~ Apw1</Title> <Col>15</Col> </Equation>");       
					strBdcy.Format(_T("%s ~ == %s - %s = %s ~ %s"), strEqua, sAp, sApf, sApw, m_sTxArea);   m_arText.Add(strBdcy);
					m_iFormula++;
				}
				m_arText.Add(_T(""));

				strEqua = _T("<Equation> <Title>~ pMn_flg</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
				m_arText.Add(_T(""));
				m_iFormula++;
				str1.Format(_T("%s * %s * %s * ( 1 - 0.59 * %s * %s / ( %s * %s * %s ) )"),
										sApw, sfps, sdp, sApw, sfps, sbw, sdp, sfck); 
				str2.Format(_T("%s * %s * ( %s - %s )"),sAs, sfy, sd, sdp);
				str3.Format(_T("0.85 * %s * ( %s - %s ) * %s * ( %s - 0.5 * %s )"),
										sfck, sb, sbw, shf, sdp, shf);
				strBdcy.Format(_T("~ ~ ~ == %s * ( %s + %s + %s ) / 10^6 ~ = %s ~ %s"),
											sphi, str1, str2, str3, spMn, m_strMoment);                            m_arText.Add(strBdcy);
				m_arText.Add(_T(""));

			}
		}
		else if(m_iDgnCode==KSCE_RAIL_USD04)
		{
			if(da<=dhf)
			{
				strBdcy.Format(_T("~ a == %s ~ < ~ h_f == %s ~ ~ ( 사각형`단면 )"), sa, shf);     m_arText.Add(strBdcy);
				m_arText.Add(_T(""));

				strEqua = _T("<Equation> <Title>~ pMn_rec_nobar</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
				m_arText.Add(_T(""));
				m_iFormula++;
				str1.Format(_T("( 1 - 0.59 * %s * %s / %s )"),sRhop, sfps, sfck);                    
				strBdcy.Format(_T("~ ~ ~ == %s * ( %s * %s * %s * %s ) / 10^6 ~ = %s ~ %s"),
											sphi, sAp, sfps, sdp, str1, spMn, m_strMoment);                            m_arText.Add(strBdcy);        
			}
			else if(da>dhf)
			{
				strBdcy.Format(_T("~ a == %s ~ > ~ h_f == %s ~ ~ ( 플랜지를`갖는`단면)"), sa, shf);   m_arText.Add(strBdcy);
				m_arText.Add(_T(""));

				strEqua = _T("<Equation> <Title>~ Apf_rail</Title> <Col>0</Col> </Equation>");             m_arText.Add(strEqua);  
				strBdcy.Format(_T("~ ~ ~ == 0.85 * %s * ( %s - %s ) * %s / %s = %s ~ %s"),
											 sfck, sb, sbw, shf, sfps, sApf, m_sTxArea);                        m_arText.Add(strBdcy);
				m_iFormula++;

				strEqua = _T("<Equation> <Title>~ Apw_rail</Title> <Col>15</Col> </Equation>");       
				strBdcy.Format(_T("%s ~ == %s - %s = %s ~ %s"), strEqua, sAp, sApf, sApw, m_sTxArea);   m_arText.Add(strBdcy);
				m_arText.Add(_T(""));
				m_iFormula++;

				strEqua = _T("<Equation> <Title>~ pMn_flg_rail</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
				m_arText.Add(_T(""));
				m_iFormula++;
				str1.Format(_T("( %s - %s ) * %s * ( %s - %s / 2 )"), sAp, sApf, sfps, sdp, sa);
				str2.Format(_T("0.85 * %s * ( %s - %s ) * %s * ( %s - %s / 2 )"), sfck, sb, sbw, shf, sdp, shf);
				strBdcy.Format(_T("~ ~ ~ == %s * ( %s + %s ) / 10^6 ~ = %s ~ %s"),
											sphi, str1, str2, spMn, m_strMoment);                            m_arText.Add(strBdcy);
				m_arText.Add(_T(""));
			}
		}
	}

	strBdcy = _T(""); m_arText.Add(strBdcy);
	strBdcy = Get_pMnPrint(dpMn, dMu);      m_arText.Add(strBdcy);
	strBdcy = _T(""); m_arText.Add(strBdcy);
	
}

void CPrtKSCE_USD05::Print_BdcyDesign_Code5(BOOL bIter, double dphi, double dfpu, double drp, double dbeta1 , double dRhop, double dfck, double da, double dhf, double db, double dbw,
																						double dd, double ddp, double dqp, double dRho, double dApw, double dAp, double dApf, double dAs, double dfy, double dfps, double dpMn, double dpMn_beta, double d036beta1, double dpMn_Cal)
{
	CString sphi   = Check_ValueForm(4,2, dphi  );

	CString sfpu   = Check_ValueForm(8,2, dfpu  );
	CString sbeta1 = Check_ValueForm(8,3, dbeta1);
	CString sRhop  = Check_ValueForm(8,5, dRhop );  
	CString sfck   = Check_ValueForm(8,2, dfck  );
	CString sa     = Check_ValueForm(7,1, da    );  
	CString shf    = Check_ValueForm(7,1, dhf   );  
	CString sb     = Check_ValueForm(7,1, db    );  
	CString sbw    = Check_ValueForm(7,1, dbw   );  
	CString sd     = Check_ValueForm(7,1, dd    );
	CString sdp    = Check_ValueForm(7,1, ddp   );
	CString sqp    = Check_ValueForm(8,3, dqp   );
	CString sRho   = Check_ValueForm(6,3, dRho  );
	CString sApw   = Check_ValueForm(7,1, dApw  );
	CString sAp    = Check_ValueForm(7,1, dAp   );
	CString sApf   = Check_ValueForm(7,1, dApf  );
	CString sAs    = Check_ValueForm(7,1, dAs   );
	CString sfy    = Check_ValueForm(8,2, dfy   );
	CString sfps   = Check_ValueForm(8,2, dfps  );
	CString spMn   = Check_ValueForm(9,2, Cng_CodeMomFix(dpMn) );
	CString spMn_beta = Check_ValueForm(9,2, Cng_CodeMomFix(dpMn_beta) );
	CString s036beta1 = Check_ValueForm(6,3, d036beta1 );
	CString spMn_Cal  = Check_ValueForm(9,2, Cng_CodeMomFix(dpMn_Cal)  );

	CString strBdcy;
	CString strEqua;
	// strBdcy = _T("Text:  철근 및 PS강재의 제한(연성 한계)");          m_arText.Add(strBdcy);
	if(bIter)
	{
		strBdcy = _T("Text: (8) 최대 PS강재량");         m_arText.Add(strBdcy); //④
	}
	else
	{
		strBdcy = _T("Text: (6) 최대 PS강재량");                m_arText.Add(strBdcy);  //④
	}
	strEqua = _T("<Equation> <Title>~ d036beta1</Title> <Col>5</Col> </Equation>");        
	strBdcy.Format(_T("%s ~ == 0.36 * %s = %s"), strEqua, sbeta1, s036beta1);                 m_arText.Add(strBdcy);
	m_iFormula++;
	
	if(ddp==0.0)
	{
		strBdcy = _T("~ q_p ~ = ~ 0.0");   m_arText.Add(strBdcy);
		m_arText.Add(_T(""));
		return;
	}

	if(da <= dhf)
	{    
		strEqua = _T("<Equation> <Title>~ qp_rec</Title> <Col>10</Col> </Equation>");      
		strBdcy.Format(_T("%s ~ == ~ %s * %s / %s = %s"), strEqua, sRhop, sfps, sfck, sqp);                         m_arText.Add(strBdcy);
		m_iFormula++;
		if(dqp <= d036beta1)
		{      
			strBdcy = _T("~ q_p ~ ≤ ~ 0.36 β_1");  m_arText.Add(strBdcy);      
		}
		else if(dqp > d036beta1)
		{
			strBdcy = _T("~ q_p ~ > ~ 0.36 β_1");   m_arText.Add(strBdcy);
			m_arText.Add(_T(""));
			if(m_iDgnCode==KSCE_USD05)
			{
				strEqua = _T("<Equation> <Title>~ pMn_qp_rec</Title> <Col>0</Col> </Equation>");     m_arText.Add(strEqua);
				strBdcy.Format(_T("~ ~ == %s * ( ( 0.36 * %s - 0.08 * %s^2 ) * %s * %s * %s^2 ) / 10^6 ~ = %s ~ %s"),
											 sphi, sbeta1, sbeta1, sfck, sb, sdp, spMn_beta, m_strMoment);      m_arText.Add(strBdcy);
				m_iFormula++;
			}
			else if(m_iDgnCode==KSCE_RAIL_USD04)
			{
				strEqua = _T("<Equation> <Title>~ pMn_qp_rec_rail</Title> <Col>0</Col> </Equation>");     m_arText.Add(strEqua);
				strBdcy.Format(_T("~ ~ == %s * ( %s * ( 0.36 * %s - 0.08 * %s^2 ) * %s * %s^2 ) / 10^6 ~ = %s ~ %s"),
											 sphi, sfck, sbeta1, sbeta1, sb, sdp, spMn_beta, m_strMoment);      m_arText.Add(strBdcy);
				m_iFormula++;
			}
			m_arText.Add(_T(""));
			if(dpMn_Cal <= dpMn_beta)
			{
				strBdcy.Format(_T("~ ~ 설계`휨강도 ~ Φ  M_n ~ < ~ %s ~ %s"), spMn_beta, m_strMoment);         m_arText.Add(strBdcy);
				strBdcy.Format(_T("~ ~ ∴  ~ Φ  M_n ~ == ~ %s ~ %s"),spMn, m_strMoment);   m_arText.Add(strBdcy);
			}
			else if(dpMn_Cal > dpMn_beta)
			{
				strBdcy.Format(_T("~ ~ 설계`휨강도 ~ Φ  M_n ~ > ~ %s ~ %s"), spMn_beta, m_strMoment);              m_arText.Add(strBdcy);
				strBdcy.Format(_T("~ ~ ∴  ~ Φ  M_n ~ == ~ %s ~ %s"),spMn, m_strMoment);   m_arText.Add(strBdcy);
			}      
		}    
	}
	else if(da > dhf)
	{    
		strBdcy.Format(_T("~ a == %s %s ~ > ~ t == %s %s"),sa, m_strLength, shf, m_strLength);  m_arText.Add(strBdcy);

		CString sUnequal = _T(""), sJudge = _T("");
		if(dqp <= d036beta1) { sUnequal=_T("≤"); sJudge=_T("O.K"); }
		else                 { sUnequal=_T("＞"); sJudge=_T("N.G"); }

		if(m_iDgnCode==KSCE_USD05)
		{
			strEqua = _T("<Equation> <Title>~ Apf</Title> <Col>0</Col> </Equation>");             m_arText.Add(strEqua);  
			strBdcy.Format(_T("~ ~ ~ == 0.85 * %s * ( %s - %s ) * %s / %s = %s ~ %s"),
										 sfck, sb, sbw, shf, sfps, sApf, m_sTxArea);                        m_arText.Add(strBdcy);
			m_iFormula++;

			if(dAs>0.0)
			{
				strEqua = _T("<Equation> <Title>~ Apw2</Title> <Col>15</Col> </Equation>");          m_arText.Add(strEqua);
				strBdcy.Format(_T("~ ~ ~ == %s + ( %s * %s / %s ) - %s = %s ~ %s"),
											 sAp, sAs, sfy, sfps, sApf, sApw, m_sTxArea);                     m_arText.Add(strBdcy);
				m_iFormula++;
			}
			else if(dAs==0.0)
			{
				strEqua = _T("<Equation> <Title>~ Apw1</Title> <Col>9</Col> </Equation>");       
				strBdcy.Format(_T("%s ~ == %s - %s = %s ~ %s"), strEqua, sAp, sApf, sApw, m_sTxArea);   m_arText.Add(strBdcy);
				m_iFormula++;
			}
			m_arText.Add(_T(""));

			strEqua = _T("<Equation> <Title>~ qp_out</Title> <Col>0</Col> </Equation>");   m_arText.Add(strEqua);
			m_arText.Add(_T(""));
			m_iFormula++;
			strBdcy.Format(_T("~ ~ ~ == %s / ( %s * %s ) * %s / %s = %s ~ %s  ~ %s ~ ~ ~ ~ ~ ~ ~ %s"),
										 sApw, sbw, sdp , sfps, sfck, sqp , sUnequal, s036beta1, sJudge);  m_arText.Add(strBdcy);
			m_arText.Add(_T(""));
		}
		else if(m_iDgnCode==KSCE_RAIL_USD04)
		{
			strEqua = _T("<Equation> <Title>~ Apf_rail</Title> <Col>0</Col> </Equation>");             m_arText.Add(strEqua);  
			strBdcy.Format(_T("~ ~ ~ == 0.85 * %s * ( %s - %s ) * %s / %s = %s ~ %s"),
										 sfck, sb, sbw, shf, sfps, sApf, m_sTxArea);                        m_arText.Add(strBdcy);
			m_iFormula++;

			strEqua = _T("<Equation> <Title>~ Apw_rail</Title> <Col>15</Col> </Equation>");       
			strBdcy.Format(_T("%s ~ == %s - %s = %s ~ %s"), strEqua, sAp, sApf, sApw, m_sTxArea);   m_arText.Add(strBdcy);
			m_arText.Add(_T(""));
			m_iFormula++;

			strEqua = _T("<Equation> <Title>~ qp_flg_rail</Title> <Col>0</Col> </Equation>");   m_arText.Add(strEqua);
			m_arText.Add(_T(""));
			strBdcy.Format(_T("~ ~ ~ == %s / ( %s * %s ) * %s / %s = %s ~ %s  ~ %s ~ ~ ~ ~ ~ ~ ~ %s"),
										 sApw, sbw, sdp , sfps, sfck, sqp , sUnequal, s036beta1, sJudge);  m_arText.Add(strBdcy);
			m_arText.Add(_T(""));
			m_iFormula++;
		}    

		if(dqp > d036beta1)
		{
			m_arText.Add(_T(""));
			strEqua = _T("<Equation> <Title>~ pMn_qp_flg</Title> <Col>0</Col> </Equation>");     m_arText.Add(strEqua);
			m_iFormula++;
			CString str1, str2;
			str1.Format(_T("%s * ( 0.36 * %s - 0.08 * %s^2 ) * %s * %s^2"),sfck, sbeta1, sbeta1, sb, sdp);
			str2.Format(_T("0.85 * %s * ( %s - %s ) * %s * ( %s - 0.5 * %s )"), sfck, sb, sbw, shf, sdp, shf);
			strBdcy.Format(_T("~ ~ == %s * ( %s + %s ) / 10^6 ~ = %s ~ %s"),sphi, str1, str2, spMn_beta, m_strMoment);   m_arText.Add(strBdcy);
			m_arText.Add(_T(""));                     
			if(dpMn_Cal <= dpMn_beta)
			{
				strBdcy.Format(_T("~ ~ 설계`휨강도 ~ Φ  M_n ~ < ~ %s ~ %s"), spMn_beta, m_strMoment);   m_arText.Add(strBdcy);
				strBdcy.Format(_T("~ ~ ∴  ~ Φ  M_n ~ == ~ %s ~ %s"),spMn, m_strMoment);   m_arText.Add(strBdcy);
			}
			else if(dpMn_Cal > dpMn_beta)
			{
				strBdcy.Format(_T("~ ~ 설계`휨강도 ~ Φ  M_n ~ > ~ %s ~ %s"), spMn_beta, m_strMoment);   m_arText.Add(strBdcy);
				strBdcy.Format(_T("~ ~ ∴  ~ Φ  M_n ~ == ~ %s ~ %s"),spMn, m_strMoment);   m_arText.Add(strBdcy);
			}      
		}    

	}   
	m_arText.Add(_T(""));
}

// BondType, UnbondTyep이 같이 있을 경우
void CPrtKSCE_USD05::Print_BdcyDesign_Code6(CArray<CString,CString>& strTndn, CArray<double,double>* aData)
{
	int i=0, j=0;
	int TndnNum = aData[0].GetSize();
	
	int iCol = 5;
	CArray<CString,CString> sData[9]; 
	for(i=0; i<iCol; i++)
		sData[i].RemoveAll();
	
	for(i=0; i<TndnNum; i++)
	{
		CString strTemp;
		strTemp = Check_ValueForm(7,1, aData[0].GetAt(i));               sData[0].Add(strTemp); // Apsi
		strTemp = Check_ValueForm(8,2, aData[1].GetAt(i));               sData[1].Add(strTemp); // fpui
		strTemp = Check_ValueForm(8,2, aData[2].GetAt(i));               sData[2].Add(strTemp); // fpsi
		strTemp = Check_ValueForm(7,1, aData[3].GetAt(i));               sData[3].Add(strTemp); // dpi    
		strTemp = (aData[4].GetAt(i)==0.0 ? _T("부착") : _T("비부착"));          sData[4].Add(strTemp); // Type    
	}
		
	CString strBdcy;
	CString strEqua;
	CString str1, str2, str3;

	strBdcy = _T("<Table>(4)PS 강재의 응력");    m_arText.Add(strBdcy);
	str1 = _T("<MH>TnGroup ~ ~ ~ ~ ~ ~</MH>   <MH>Apsi ~ ~ ~ ~ ~</MH>   <MH>fpui ~ ~ ~ ~ ~</MH>   ");
	str2 = _T("<MH>fpsi ~ ~ ~ ~ ~</MH>   <MH>dpi ~ ~ ~ ~ ~</MH>   <MH>Type ~ ~ ~ ~ ~</MH>   ");
	strBdcy = str1 + str2; m_arText.Add(strBdcy);
	for( i=0; i<TndnNum; i++)
	{
		str1.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   "),strTndn.GetAt(i) ,sData[0].GetAt(i),sData[1].GetAt(i) ); 
		str2.Format(_T("<MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   <MH>%s ~ ~ ~ ~ ~</MH>   "),
								sData[2].GetAt(i), sData[3].GetAt(i), sData[4].GetAt(i) );
		strBdcy = str1 + str2;  m_arText.Add(strBdcy);                      
	}
	strBdcy = _T("</Table>");  m_arText.Add(strBdcy);
	
}

void CPrtKSCE_USD05::Print_BdcyDesign_Code7(double dc, double da, double db, double dbw, double dAs, double dhf,
																						double dfck, double dd, double ddp, double dfy, double dphi, 
																						double dMn_ps, double dpMn, double dMu)
{
	CString sc     = Check_ValueForm(7,1, dc    );
	CString sa     = Check_ValueForm(7,1, da    );
	CString sb     = Check_ValueForm(7,1, db    );
	CString sbw    = Check_ValueForm(7,1, dbw   );
	CString sAs    = Check_ValueForm(8,1, dAs   );
	CString shf    = Check_ValueForm(7,1, dhf   );
	CString sfck   = Check_ValueForm(8,2, dfck  );
	CString sd     = Check_ValueForm(7,1, dd    );
	CString sdp    = Check_ValueForm(7,1, ddp   );
	CString sfy    = Check_ValueForm(8,2, dfy   );
	CString sphi   = Check_ValueForm(6,2, dphi   );
	CString sMn_ps = Check_ValueForm(9,2, dMn_ps );   
	CString spMn   = Check_ValueForm(9,2, dpMn   );
	CString sMu    = Check_ValueForm(9,2, dMu   );
		
	CString strBdcy;
	CString strEqua;
	CString str1, str2, str3;
	strBdcy = _T("Text: (5) 설계 휨강도 ");          m_arText.Add(strBdcy);
	if(da<=dhf)
	{
		strBdcy.Format(_T("~ a == %s ~ < ~ h_f == %s ~ ~ ( 사각형`단면 )"), sa, shf);     m_arText.Add(strBdcy);
		m_arText.Add(_T(""));
		strEqua = _T("<Equation> <Title>~ ~ pMn_2type_rec</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
		str1.Format(_T("0.85 * %s * %s * %s * ( %s - %s / 2 )"), sfck, sa, sb, sc, sa);
		str2.Format(_T("%s * %s * ( %s - %s )"), sAs, sfy, sd, sc);
		strBdcy.Format(_T("~ ~ == %s * ( %s + %s + %s ) / 10^6 ~ = %s ~ %s"), sphi, str1, sMn_ps, str2, spMn, m_strMoment);         m_arText.Add(strBdcy);
		m_iFormula++;

	}
	else if(da>dhf)
	{
		strBdcy.Format(_T("~ a == %s ~ > ~ h_f == %s ~ ~ ( 플랜지를`갖는`단면 )"), sa, shf);     m_arText.Add(strBdcy);
		m_arText.Add(_T(""));

		strEqua = _T("<Equation> <Title>~ ~ pMn_2type_flg</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);
		str1.Format(_T("0.85 * %s ( %s * %s * ( %s - %s / 2 ) + ( %s - %s ) * %s * ( %s - %s ) )"),
								sfck, sa, sbw, sc, sa, sb, sbw, shf, sc, shf);
		str2.Format(_T("%s * %s * ( %s - %s )"), sAs, sfy, sd, sc);
		strBdcy.Format(_T("~ ~ == %s * ( %s + %s + %s ) / 10^6 ~ = %s ~ %s"), sphi, str1, sMn_ps, str2, spMn, m_strMoment);         m_arText.Add(strBdcy);
		m_iFormula++;
	}

	strBdcy = _T(""); m_arText.Add(strBdcy);
	strBdcy = Get_pMnPrint(dpMn, dMu);      m_arText.Add(strBdcy);
	strBdcy = _T(""); m_arText.Add(strBdcy);

}



//전단강도 검토(Cums) //////////////////////////////////////////////////////////////////////////////////////////////
void CPrtKSCE_USD05::Print_CumsTheory(CArray<CString,CString>& aString)
{
	CString strCums = _T("");
	CString strEqua, strSpace;
	strSpace = _T("~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ "); 
	strCums = _T("Text:  ▷ 전단강도 검토");                              aString.Add(strCums);
	strCums = _T(""); aString.Add(strCums);
	strCums = _T("Text:    - 콘크리트가 부담하는 전단강도(Vc)");           aString.Add(strCums);
	strCums = _T("Text:     : 휨 전단균열 발생시의 전단강도(Vci)와 복부 전단균열 발생시의 전단강도(Vcw) 중 작은 값을 적용"); aString.Add(strCums);
	strCums = _T(""); aString.Add(strCums);

	strCums = _T("Text:    - 휨 전단균열 발생시의 전단강도");             aString.Add(strCums);
	if(m_iDgnCode==KSCE_USD05)
	{
		strEqua = _T("<Equation> <Title>~ ~ ~ Vci_b_KS05_2</Title> <Col>30</Col> </Equation> ");    
	}
	else if(m_iDgnCode==KSCE_RAIL_USD04)
	{
		strEqua = _T("<Equation> <Title>~ ~ ~ Vci_b_rail2</Title> <Col>30</Col> </Equation> ");
	}
	//strCums = strEqua + _T("V_d ~ : ~ 사용고정하중에`의해`발생하는`전단력");   aString.Add(strCums);
	//strCums = strSpace + _T("M_d ~ : ~ 사용고정하중에`의해`발생하는`모멘트");   aString.Add(strCums);
	strCums = _T(""); aString.Add(strCums);

	strEqua = _T("<Equation> <Title>~ ~ ~ Mcr_b_KS05_2</Title> <Col>30</Col> </Equation> ");    
	strCums = strEqua + _T("M_cr : ~ 균열`모멘트");                          aString.Add(strCums);
	strCums = strSpace + _T("f_pe : ~ 유효프리스트레스에 의한`압축응력");     aString.Add(strCums);
	//strCums = strSpace + _T("f_d ~ : ~ 자중에`의한`휨인장응력");                aString.Add(strCums);
	strCums = _T(""); aString.Add(strCums);

	//strEqua = _T("<Equation> <Title>~ ~ ~ Vi_b</Title> <Col>10</Col>  <Title>~ Mmax_b</Title> <Col>30</Col> </Equation> ");     aString.Add(strEqua);
	//strCums = _T(" "); aString.Add(strCums);

	strCums = _T("Text:    - 복부 전단균열 발생시의 전단강도");           aString.Add(strCums);
	if(m_iDgnCode==KSCE_USD05)
	{
		strEqua = _T("<Equation> <Title>~ ~ ~ Vcw_b_KS05</Title> <Col>30</Col> </Equation> ");    
	}
	else if(m_iDgnCode==KSCE_RAIL_USD04)
	{
		strEqua = _T("<Equation> <Title>~ ~ ~ Vcw_b_rail</Title> <Col>30</Col> </Equation> ");    
	}
	strCums = strEqua + _T("f_pc : ~ 유효프리스트레스에`의한`콘크리트");     aString.Add(strCums);
	strCums = strSpace + _T("~ ~ ~ ~ ~ 도심에서의`압축응력");                        aString.Add(strCums);
	strCums = strSpace + _T("V_p ~ : ~ 유효프리스트레스의`수직분력");                aString.Add(strCums);

	strCums = _T(" "); aString.Add(strCums);
	strCums = _T("Text:    - 전단철근이 부담하는 전단강도( Vs )");           aString.Add(strCums);
	strCums = _T("<Equation> <Title>~ ~ ~ Vs_b_KS05</Title> <Col>0</Col> </Equation> ");    aString.Add(strCums);
	strCums = _T(""); aString.Add(strCums);
	strCums = _T(""); aString.Add(strCums);

	strCums = _T("Text:    - 최소전단철근");                               aString.Add(strCums);
	strCums = _T("<Equation> <Title>~ ~ ~ pVc_b</Title> <Col>10</Col>  <Title>~ pVc12_b</Title> <Col>20</Col> </Equation> 일경우");  aString.Add(strCums);  
	strCums = _T("<Equation> <Title>~ ~ ~ Avmin_b_KS05</Title> <Col>0</Col> </Equation> ");    aString.Add(strCums);
	
}

void CPrtKSCE_USD05::Print_CumsDesign0()
{
	CString sfck  = Check_ValueForm(8,2, m_CumsParam.dfck);
	CString sbw   = Check_ValueForm(7,1, m_CumsParam.dbw);
	CString sdd   = Check_ValueForm(7,1, m_CumsParam.ddst);
	CString sfy   = Check_ValueForm(8,2, m_CumsParam.dfy);
	CString sh    = Check_ValueForm(7,1, m_CumsParam.dh);
	CString sIy   = Check_ValueForm(9,3, Cng_CodeLenFix(m_CumsParam.dIy,4));

	CString sfpu = Check_ValueForm(8,2, m_CumsParam.dfpu);
	CString sdp  = Check_ValueForm(7,1, m_CumsParam.ddp);
	CString sAc  = Check_ValueForm(9,3, Cng_CodeLenFix(m_CumsParam.dAc,2));
	CString sfpy = Check_ValueForm(8,2, m_CumsParam.dfpy);
	CString syt  = Check_ValueForm(7,1, m_CumsParam.dyt);
	CString sAps = Check_ValueForm(8,1, m_CumsParam.dAps);

	CString sSpace4 = _T("~ ~ ~ ~ ");
	CString sSpace5 = _T("~ ~ ~ ~ ~ ");
	CString sSpace6 = _T("~ ~ ~ ~ ~ ~ ");
	CString sSpace7 = _T("~ ~ ~ ~ ~ ~ ~ ");
	CString sSpace8 = _T("~ ~ ~ ~ ~ ~ ~ ~ ");
	CString sSpace9 = _T("~ ~ ~ ~ ~ ~ ~ ~ ~ ");
	CString s1stSpace  = _T("");
	CString s2ndSpace  = _T("");

	int iMaxLength = 0;
	int arLength[3][4];

	int isfck = arLength[0][0] = sfck.GetLength();
	int isfy  = arLength[0][1] = sfy.GetLength();
	int isfpu = arLength[0][2] = sfpu.GetLength();
	int isfpy = arLength[0][3] = sfpy.GetLength();
	
	int isbw  = arLength[1][0] = sbw.GetLength();
	int ish   = arLength[1][1] = sh.GetLength();
	int isdp  = arLength[1][2] = sdp.GetLength();
	int isyt  = arLength[1][3] = syt.GetLength();

	int isdd  = arLength[2][0] = sdd.GetLength();
	int isIy  = arLength[2][1] = sIy.GetLength();
	int isAc  = arLength[2][2] = sAc.GetLength();
	int isAps = arLength[2][3] = sAps.GetLength();

	// 1 Column.    
	for(int i=0; i<4; i++)  
		iMaxLength = max(iMaxLength,arLength[0][i]);  

	CString strfck, strfy, strfpu, strfpy;
	strfck  = Get_TextStringLength(iMaxLength, sfck);
	strfy   = Get_TextStringLength(iMaxLength, sfy);
	strfpu  = Get_TextStringLength(iMaxLength, sfpu);
	strfpy  = Get_TextStringLength(iMaxLength, sfpy);
		
	if(iMaxLength==10 || iMaxLength==11)    s1stSpace = sSpace4;
	else if(iMaxLength==9 || iMaxLength==8) s1stSpace = sSpace5;
	else if(iMaxLength==7)                  s1stSpace = sSpace6;
	else if(iMaxLength==6 || iMaxLength==5) s1stSpace = sSpace7;
	else if(iMaxLength==4 || iMaxLength==3) s1stSpace = sSpace8;
	else if(iMaxLength==2)                  s1stSpace = sSpace9;
	else ASSERT(0);

	// 2 Column.
	iMaxLength = 0;
	for(int i=0; i<4; i++)  
		iMaxLength = max(iMaxLength,arLength[1][i]);

	CString strbw, strh, strdp, strdd;
	strbw  = Get_TextStringLength(iMaxLength, sbw );
	strh   = Get_TextStringLength(iMaxLength, sh  );
	strdp  = Get_TextStringLength(iMaxLength, sdp );
	strdd  = Get_TextStringLength(iMaxLength, sdd );  

	if(iMaxLength==10 || iMaxLength==11)    s2ndSpace = sSpace4;
	else if(iMaxLength==9 || iMaxLength==8) s2ndSpace = sSpace5;
	else if(iMaxLength==7)                  s2ndSpace = sSpace6;
	else if(iMaxLength==6 || iMaxLength==5) s2ndSpace = sSpace7;
	else if(iMaxLength==4 || iMaxLength==3) s2ndSpace = sSpace8;
	else if(iMaxLength==2)                  s2ndSpace = sSpace9;

	// 3 Column.
	iMaxLength = 0;
	for(int i=0; i<4; i++)  
		iMaxLength = max(iMaxLength,arLength[2][i]);

	CString stryt, strIy, strAc, strAps;
	stryt  = Get_TextStringLength(iMaxLength, syt );
	strIy  = Get_TextStringLength(iMaxLength, sIy );
	strAc  = Get_TextStringLength(iMaxLength, sAc );
	strAps = Get_TextStringLength(iMaxLength, sAps);  

	if(sIy.GetLength()>=9)  strIy.TrimRight(_T("~ "));
	if(sAc.GetLength()>=9)  strAc.TrimRight(_T("~ "));
	

	CString strCums = _T("");
	strCums = _T("Text: (1) 설계 변수");                                                m_arText.Add(strCums);
	if(m_iDgnCode==KSCE_USD05)
	{
		strCums.Format(_T("~ ~ f_ck : ~ %s%s %s b_w ~ : ~ %s%s %s y_t ~ : ~ %s%s"),
									 strfck,m_sTxStress, s1stSpace, strbw, m_strLength, s2ndSpace, stryt,m_strLength);    m_arText.Add(strCums);
		strCums.Format(_T("~ ~ f_y ~ : ~ %s%s %s h ~ ~ : ~ %s%s %s I_y ~ : ~ %s %s"),
									 strfy,m_sTxStress, s1stSpace, strh, m_strLength, s2ndSpace, strIy, m_sTxStif);    m_arText.Add(strCums);  

		strCums.Format(_T("~ ~ f_pu : ~ %s%s %s d_p ~ : ~ %s%s %s A_c ~ : ~ %s %s"),
									 strfpu,m_sTxStress, s1stSpace, strdp,m_strLength, s2ndSpace, strAc,m_sTxArea);    m_arText.Add(strCums);
		strCums.Format(_T("~ ~ f_py : ~ %s%s %s d ~ ~ : ~ %s%s %s A_p ~ : ~ %s%s"),
									 strfpy,m_sTxStress, s1stSpace, strdd, m_strLength, s2ndSpace, strAps,m_sTxArea);    m_arText.Add(strCums);
	}
	else if(m_iDgnCode==KSCE_RAIL_USD04)
	{
		strCums.Format(_T("~ ~ f_ck : ~ %s%s %s b_w ~ : ~ %s%s %s y_t ~ : ~ %s%s"),
									 strfck,m_sTxStress, s1stSpace, strbw, m_strLength, s2ndSpace, stryt,m_strLength);    m_arText.Add(strCums);
		strCums.Format(_T("~ ~ f_y ~ : ~ %s%s %s h ~ ~ : ~ %s%s %s I_y ~ : ~ %s %s"),
									 strfy,m_sTxStress, s1stSpace, strh, m_strLength, s2ndSpace, strIy, m_sTxStif);    m_arText.Add(strCums);  

		strCums.Format(_T("~ ~ f_pu : ~ %s%s %s d_p ~ : ~ %s%s %s A_c ~ : ~ %s %s"),
									 strfpu,m_sTxStress, s1stSpace, strdp,m_strLength, s2ndSpace, strAc,m_sTxArea);    m_arText.Add(strCums);
		strCums.Format(_T("~ ~ f_py : ~ %s%s %s d ~ ~ : ~ %s%s %s A_ps ~ : ~ %s%s"),
									 strfpy,m_sTxStress, s1stSpace, strdd, m_strLength, s2ndSpace, strAps,m_sTxArea);    m_arText.Add(strCums);
	}
	m_arText.Add(_T(""));

}

void CPrtKSCE_USD05::Print_CumsDesign1(double dfck, double dbw, double ddst , double dfy, double dh, double dIy)
{
	CString sfck  = Check_ValueForm(8,2, dfck);
	CString sbw   = Check_ValueForm(7,1, dbw);
	CString sdst  = Check_ValueForm(7,1, ddst);
	CString sfy   = Check_ValueForm(8,2, dfy);
	CString sh    = Check_ValueForm(7,1, dh);
	CString sIy   = Check_ValueForm(9,3, Cng_CodeLenFix(dIy,4));

	CString strCums = _T("");
	strCums = _T("Text: (1) 설계 변수");                                                m_arText.Add(strCums);
	strCums.Format(_T("~ f_ck : ~ %s ~ ~ %s ~ ~ ~ ~ ~ b_w ~ : ~ %s ~ %s ~ ~ ~ ~ ~ d ~ ~ : ~ %s ~ ~ ~ %s"),
								 sfck,m_sTxStress, sbw, m_strLength, sdst,m_strLength);    m_arText.Add(strCums);
	strCums.Format(_T("~ f_y ~ : ~ %s ~ ~ %s ~ ~ ~ ~ ~ h ~ ~ : ~ %s ~ %s ~ ~ ~ ~ ~ I_y ~ : ~ %s %s"),
								 sfy,m_sTxStress, sh, m_strLength , sIy, m_sTxStif);    m_arText.Add(strCums);  
}

void CPrtKSCE_USD05::Print_CumsDesign2(double dfpu, double ddp, double dAc, double dfpy, double dyt, double dAps)
{
	CString sfpu = Check_ValueForm(8,2, dfpu);
	CString sdp  = Check_ValueForm(7,1, ddp);
	CString sAc  = Check_ValueForm(8,1, Cng_CodeLenFix(dAc,2));
	CString sfpy = Check_ValueForm(8,2, dfpy);
	CString syt  = Check_ValueForm(7,1, dyt);
	CString sAps = Check_ValueForm(8,1, dAps);

	CString strCums = _T("");
	strCums.Format(_T("~ f_pu : ~ %s ~ %s ~ ~ ~ ~ ~ d_p ~ : ~ %s ~ %s ~ ~ ~ ~ ~ A_c ~ : ~ %s ~ %s"),
								 sfpu,m_sTxStress, sdp,m_strLength, sAc,m_sTxArea);    m_arText.Add(strCums);
	strCums.Format(_T("~ f_py : ~ %s ~ %s ~ ~ ~ ~ ~ y_t ~ : ~ %s ~ %s ~ ~ ~ ~ ~ A_ps : ~ %s ~ ~ ~ %s"),
								 sfpy,m_sTxStress, syt,m_strLength , sAps,m_sTxArea);    m_arText.Add(strCums);
	strCums = _T("");   m_arText.Add(strCums);
}

void CPrtKSCE_USD05::Print_CumsDesign3(double dMu, double dVu, double dMd, double dVd, double dfd, double dPe, double dfpe)
{
	CString sMu  = Check_ValueForm(8,2, Cng_CodeMomFix(dMu));
	CString sVu  = Check_ValueForm(8,2, Cng_CodeForFix(dVu));
	CString sMd  = Check_ValueForm(8,2, Cng_CodeMomFix(dMd));
	CString sVd  = Check_ValueForm(8,2, Cng_CodeForFix(dVd));
	CString sfd  = Check_ValueForm(8,2, dfd);
	CString sPe  = Check_ValueForm(8,2, Cng_CodeForFix(dPe));
	CString sfpe = Check_ValueForm(8,2, dfpe);

	CString strStress = m_sTxStress;
	
	CString strCums = _T("");
	CString strDum1, strDum2, strDum3;
	strCums = _T("Text: (2) 설계 단면력");                 m_arText.Add(strCums);
	strCums = _T("<Table>설계 단면력");                                                                   m_arText.Add(strCums);
	if(m_iVciType==1)
	{
		strDum1 = _T("<MH>계``수``하``중  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~</MH> ");
		strDum2 = _T("<MH>사``용``하``중  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~</MH> ");
		strDum3 = _T("<MH>유효프리스트레스력  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~</MH> ");
		strCums = strDum1 + strDum2 + strDum3;                                                            m_arText.Add(strCums);


		strDum1 = _T("<MH>Mu ~ ~ ~ ~ ~ ~</MH> <MH>Vu ~ ~ ~ ~ ~ ~</MH> ");
		strDum2 = _T("<MH>Md ~ ~ ~ ~ ~ ~</MH> <MH>Vd ~ ~ ~ ~ ~ ~</MH> <MH>fd ~ ~ ~ ~ ~ ~</MH> ");
		strDum3 = _T("<MH>Pe ~ ~ ~ ~ ~ ~</MH> <MH>fpe ~ ~ ~ ~ ~ ~</MH>");
		strCums = strDum1 + strDum2 + strDum3;                                                            m_arText.Add(strCums);


		strDum1.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> "),m_strMoment,m_strForce);
		strDum2.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> "),
										m_strMoment, m_strForce, strStress);
		strDum3.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH>"),m_strForce, strStress);
		strCums = strDum1 + strDum2 + strDum3;                                                            m_arText.Add(strCums);


		strDum1.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> "),sMu, sVu);
		strDum2.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> "), sMd, sVd, sfd);
		strDum3.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH>"),sPe, sfpe);
		strCums = strDum1 + strDum2 + strDum3;            m_arText.Add(strCums);
	}
	else if(m_iVciType==2)
	{
		strDum1 = _T("<MH>계``수``하``중  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~</MH> ");    
		strDum3 = _T("<MH>유효프리스트레스력  ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~</MH> ");
		strCums = strDum1 + strDum3;                                                            m_arText.Add(strCums);


		strDum1 = _T("<MH>Mu ~ ~ ~ ~ ~ ~</MH> <MH>Vu ~ ~ ~ ~ ~ ~</MH> ");
		strDum3 = _T("<MH>Pe ~ ~ ~ ~ ~ ~</MH> <MH>fpe ~ ~ ~ ~ ~ ~</MH>");
		strCums = strDum1 + strDum3;                                                            m_arText.Add(strCums);


		strDum1.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> "),m_strMoment,m_strForce);
		strDum3.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH>"),m_strForce, strStress);
		strCums = strDum1 + strDum3;                                                            m_arText.Add(strCums);


		strDum1.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> "),sMu, sVu);
		strDum3.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH>"),sPe, sfpe);
		strCums = strDum1 + strDum3;            m_arText.Add(strCums);
	}
	strCums = _T("</Table>");   m_arText.Add(strCums);
	m_arText.Add(_T(""));

	strCums = _T("Text: (3) 콘크리트가 부담하는 전단강도의 산정");    m_arText.Add(strCums);
}

void CPrtKSCE_USD05::Print_CumsDesign4(double dh, double dd, double dd_org, double d08h, double ddp, double ddp_org, double dIy, double dyt, double dfck, double dfpe, 
																			 double dfd, double dMcr)
{
	CString sd      = Check_ValueForm(7,1, dd);
	CString sdp     = Check_ValueForm(7,1, ddp);
	CString sd_org  = Check_ValueForm(7,1, dd_org);
	CString sdp_org = Check_ValueForm(7,1, ddp_org);
	CString sIy;    sIy.Format(_T("%.1f"), dIy); // Check_ValueForm(9,3, Cng_CodeLenFix(dIy,4));
	CString syt     = Check_ValueForm(7,1, dyt);
	CString sfck    = Check_ValueForm(8,2, dfck);
	CString sfpe    = Check_ValueForm(8,2, dfpe);
	CString sfd     = Check_ValueForm(8,2, dfd);
	CString sMcr    = Check_ValueForm(8,2, Cng_CodeMomFix(dMcr));

	CString strCums = _T("");
	CString strEqua = _T("");  
	strCums = Get_dValuePrint(dh, d08h, dd_org, ddp_org);                            m_arText.Add(strCums);
	if(m_iDgnCode==KSCE_USD05)
	{
		strCums.Format(_T("~ ~ ∴  ~ d == %s %s"), sd, m_strLength);                 m_arText.Add(strCums);
	}
	else if(m_iDgnCode==KSCE_RAIL_USD04)
	{
		strCums.Format(_T("~ ~ ∴  ~ d_p == %s %s"), sdp, m_strLength);                 m_arText.Add(strCums);
	}
	strCums = _T("");   m_arText.Add(strCums);

	if(m_iVciType==1)
	{
		strEqua = _T("<Equation> <Title>~ Mcr_b_KS05</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);  
		strCums = _T("");   m_arText.Add(strCums);
		strCums.Format(_T("~ ~ ~ == ( %s / %s ) * ( 0.5 * √  ( %s ) + %s - %s ) / 10^6 = %s ~ %s"),
										sIy, syt, sfck, sfpe, sfd, sMcr, m_strMoment);                  m_arText.Add(strCums); 
	}
	else if(m_iVciType==2)
	{
		strEqua = _T("<Equation> <Title>~ Mcr_b_KS05_2</Title> <Col>0</Col> </Equation>");         m_arText.Add(strEqua);  
		strCums = _T("");   m_arText.Add(strCums);
		strCums.Format(_T("~ ~ ~ == ( %s / %s ) * ( 0.5 * √  ( %s ) + %s ) / 10^6 = %s ~ %s"),
										sIy, syt, sfck, sfpe, sMcr, m_strMoment);                  m_arText.Add(strCums); 
	}
	strCums = _T("");   m_arText.Add(strCums);
	m_iFormula++;
}

void CPrtKSCE_USD05::Print_CumsDesign5(double dMu, double dMd, double dMmax, double dVu, double dVd, double dVi, double dVci_cal,
																			 double dVci, double dVci_incl, double dfck, double dbw, double dd, double ddp, double dMcr, double dminVci)
{
	CString sMu    = Check_ValueForm(8,2, Cng_CodeMomFix(dMu));
	CString sMd    = Check_ValueForm(8,2, Cng_CodeMomFix(dMd));
	CString sMmax  = Check_ValueForm(8,2, Cng_CodeMomFix(dMmax));
	CString sVu    = Check_ValueForm(8,2, Cng_CodeForFix(dVu));
	CString sVd    = Check_ValueForm(8,2, Cng_CodeForFix(dVd));
	CString sVi    = Check_ValueForm(8,2, Cng_CodeForFix(dVi));
	CString sVci_cal   = Check_ValueForm(8,2, Cng_CodeForFix(dVci_cal));
	CString sVci   = Check_ValueForm(8,2, Cng_CodeForFix(dVci));
	CString sVci_incl = Check_ValueForm(8,2, Cng_CodeForFix(dVci_incl));
	CString sVci_abs = Check_ValueForm(8,2, Cng_CodeForFix(fabs(dVci_incl)));
	CString sfck   = Check_ValueForm(8,2, dfck);
	CString sbw    = Check_ValueForm(7,1, dbw);
	CString sd     = Check_ValueForm(7,1, dd);
	CString sdp    = Check_ValueForm(7,1, ddp);
	CString sMcr   = Check_ValueForm(8,2, Cng_CodeMomFix(dMcr));
	CString sminVci= Check_ValueForm(8,2, Cng_CodeForFix(dminVci));

	CString strCums = _T("");
	CString strEqua = _T("");
	if(m_iVciType==1)
	{
		strEqua = _T("<Equation> <Title>~ Mmax_b</Title> <Col>12</Col> </Equation>"); m_arText.Add(strEqua);
		m_iFormula++;
		if(dMu>=0.0) { strCums.Format(_T("~ ~ ~ ~ == %s - %s = %s ~ %s"), sMu, sMd, sMmax, m_strMoment);       m_arText.Add(strCums);  }
		else         { strCums.Format(_T("~ ~ ~ ~ == -1 ~ * ( %s - ( %s ) ) = %s ~ %s"), sMu, sMd, sMmax, m_strMoment);       m_arText.Add(strCums);  }
		strCums = _T("");   m_arText.Add(strCums);

		strEqua = _T("<Equation> <Title>~ Vi_b</Title> <Col>10</Col> </Equation>"); m_arText.Add(strEqua);
		m_iFormula++;
		if(dVu>=0.0) { strCums.Format(_T("~ ~ == %s - %s = %s ~ %s"), sVu, sVd, sVi, m_strForce);           m_arText.Add(strCums); }
		else         { strCums.Format(_T("~ ~ == %s - ( %s ) = %s ~ %s"), sVu, sVd, sVi, m_strForce);           m_arText.Add(strCums); }
		strCums = _T("");   m_arText.Add(strCums);
	}

	if(m_iDgnCode==KSCE_USD05)
	{
		if(m_iVciType==1)
		{
			strCums = _T("<Equation> <Title>~ Vci_b_KS05</Title> <Col>0</Col> </Equation>");  m_arText.Add(strCums);  
			m_arText.Add(_T(""));
			m_iFormula++;
			if(dMmax==0.0)
	    {
		    // 수식.
		    strCums.Format(_T("~ ~ ~ == 0.05 * √  ( %s ) * %s * %s / 10^3 + abs ~ ( %s ) = %s ~ %s"),
										 sfck, sbw, sd, sVd, sVci_cal, m_strForce);                           m_arText.Add(strCums);
	    }
	    else
	    {	
				if(dMmax>0.0 && dMcr>=0.0 && dVd>=0.0 && dVi>=0.0)
				{
					strCums.Format(_T("~ ~ ~ == 0.05 * √  ( %s ) * %s * %s / 10^3 + %s + %s * ( %s / %s ) = %s ~ %s"),
											 sfck, sbw, sd, sVd, sVi, sMcr, sMmax, sVci_cal, m_strForce);                           m_arText.Add(strCums);        
				}
				else 
				{      
					strCums = _T("<Equation> <Title>~ ~ Vci_bend</Title> <Col>0</Col> </Equation>");  m_arText.Add(strCums);  
					m_arText.Add(_T(""));
					m_iFormula++;
		      strCums.Format(_T("~ ~ ~ ~ == %s + %s * ( %s / %s ) = %s"), sVd, sVi, sMcr, sMmax, sVci_incl, sVci_incl, sVci_abs);					m_arText.Add(strCums);
					strCums.Format(_T("~ ~ ~ ∴  | %s | == abs ~ ( %s ) = %s ~ %s"), sVci_incl, sVci_incl, sVci_abs, m_strForce);              m_arText.Add(strCums);
		      strCums.Format(_T("~ V_ci == 0.05 * √  ( %s ) * %s * %s / 10^3 + %s = %s ~ %s"),
									      sfck, sbw, sd, sVci_abs, sVci_cal, m_strForce);                           m_arText.Add(strCums);		  
				}    		  
	    }    
		}
		else if(m_iVciType==2)
		{
			strCums = _T("<Equation> <Title>~ Vci_b_KS05_2</Title> <Col>0</Col> </Equation>");  m_arText.Add(strCums);  
			m_arText.Add(_T(""));
			m_iFormula++;
			if(dMu==0.0)
	    {
		    // 수식.
		    strCums.Format(_T("~ ~ ~ == 0.05 * √  ( %s ) * %s * %s / 10^3 = %s ~ %s"),
										 sfck, sbw, sd, sVci_cal, m_strForce);                           m_arText.Add(strCums);
	    }
	    else
	    {	
				if((dMu>0.0 && dVu>=0.0) || (dMu<0.0 && dVu<=0.0))
				{
					strCums.Format(_T("~ ~ ~ == 0.05 * √  ( %s ) * %s * %s / 10^3 + %s * ( %s / %s ) = %s ~ %s"),
											 sfck, sbw, sd, sVu, sMcr, sMu, sVci_cal, m_strForce);                           m_arText.Add(strCums);        
				}
				else 
				{      
					strCums.Format(_T("~ ~ ~ == 0.05 * √  ( %s ) * %s * %s / 10^3 + abs ~ ( %s * ( %s / %s ) ) = %s ~ %s"),
											 sfck, sbw, sd, sVu, sMcr, sMu, sVci_cal, m_strForce);                           m_arText.Add(strCums);        
				}    		  
	    }    
		}  
	  strCums = _T("");   m_arText.Add(strCums);

		strEqua = _T("<Equation> <Title>~ Vcimin_KS05</Title> <Col>17</Col> </Equation>");  m_arText.Add(strEqua);  
		m_iFormula++;
		strCums.Format(_T("~ ~ ~ ~ ~ == 0.14 * √  ( %s ) * %s * %s / 10^3 = %s ~ %s"), 
									 sfck, sbw, sd, sminVci, m_strForce);    m_arText.Add(strCums);                                 
		strCums = _T("");   m_arText.Add(strCums);

	}
	else if(m_iDgnCode==KSCE_RAIL_USD04)
	{
		if(m_iVciType==1)
		{
			strCums = _T("<Equation> <Title>~ Vci_b_rail</Title> <Col>0</Col> </Equation>");  m_arText.Add(strCums);  
			m_arText.Add(_T(""));
			m_iFormula++;
			if(dMmax==0.0)
	    {
		    // 수식.
		    strCums.Format(_T("~ ~ ~ == 0.05 * √  ( %s ) * %s * %s / 10^3 + abs ~ ( %s ) = %s ~ %s"),
										 sfck, sbw, sdp, sVd, sVci_cal, m_strForce);                           m_arText.Add(strCums);
	    }
	    else
	    {	
				if(dMmax>0.0 && dMcr>=0.0 && dVd>=0.0 && dVi>=0.0)
				{
					strCums.Format(_T("~ ~ ~ == 0.05 * √  ( %s ) * %s * %s / 10^3 + %s + %s * ( %s / %s ) = %s ~ %s"),
											 sfck, sbw, sdp, sVd, sVi, sMcr, sMmax, sVci_cal, m_strForce);                           m_arText.Add(strCums);
				}
				else 
				{      
					strCums = _T("<Equation> <Title>~ ~ Vci_bend</Title> <Col>0</Col> </Equation>");  m_arText.Add(strCums);  
					m_arText.Add(_T(""));
					m_iFormula++;
		      strCums.Format(_T("~ ~ ~ ~ == %s + %s * ( %s / %s ) = %s"), sVd, sVi, sMcr, sMmax, sVci_incl, sVci_incl, sVci_abs);					m_arText.Add(strCums);
					strCums.Format(_T("~ ~ ~ ∴  | %s | == abs ~ ( %s ) = %s ~ %s"), sVci_incl, sVci_incl, sVci_abs, m_strForce);              m_arText.Add(strCums);
		      strCums.Format(_T("~ V_ci == 0.05 * √  ( %s ) * %s * %s / 10^3 + %s = %s ~ %s"),
									      sfck, sbw, sdp, sVci_abs, sVci_cal, m_strForce);                           m_arText.Add(strCums);		  
				}    
			}
		}
		else if(m_iVciType==2)
		{
			strCums = _T("<Equation> <Title>~ Vci_b_rail2</Title> <Col>0</Col> </Equation>");  m_arText.Add(strCums);  
			m_arText.Add(_T(""));
			m_iFormula++;
			if(dMu==0.0)
	    {
		    // 수식.
		    strCums.Format(_T("~ ~ ~ == 0.05 * √  ( %s ) * %s * %s / 10^3 = %s ~ %s"),
										 sfck, sbw, sdp, sVci_cal, m_strForce);                           m_arText.Add(strCums);
	    }
	    else
	    {	
				if((dMu>0.0 && dVu>=0.0) || (dMu<0.0 && dVu<=0.0))
				{
					strCums.Format(_T("~ ~ ~ == 0.05 * √  ( %s ) * %s * %s / 10^3 + %s * ( %s / %s ) = %s ~ %s"),
											 sfck, sbw, sdp, sVu, sMcr, sMu, sVci_cal, m_strForce);                           m_arText.Add(strCums);
				}
				else 
				{      
					strCums.Format(_T("~ ~ ~ == 0.05 * √  ( %s ) * %s * %s / 10^3 + abs ~ ( %s * ( %s / %s ) ) = %s ~ %s"),
											 sfck, sbw, sdp, sVu, sMcr, sMu, sVci_cal, m_strForce);                           m_arText.Add(strCums);
				}    
			}
		}
	
		strCums = _T("");   m_arText.Add(strCums);
	  strCums = _T("");   m_arText.Add(strCums);

		strEqua = _T("<Equation> <Title>~ Vcimin_rail</Title> <Col>17</Col> </Equation>");  m_arText.Add(strEqua);  
		m_iFormula++;
		strCums.Format(_T("~ ~ ~ ~ ~ == 0.14 * √  ( %s ) * %s * %s / 10^3 = %s ~ %s"), 
									 sfck, sbw, sdp, sminVci, m_strForce);    m_arText.Add(strCums);                                 
		strCums = _T("");   m_arText.Add(strCums);
	}

	strCums.Format(_T("~ ~ ~ ∴  ~ V_ci == %s ~ %s"),sVci, m_strForce);    m_arText.Add(strCums);
	strCums = _T("");   m_arText.Add(strCums);
}

void CPrtKSCE_USD05::Print_CumsDesign6(double dPe, double dAc, double dfpc, double dfck, double dbw, 
																			 double dd, double ddp, double dVp, double dVcw, double dVci, double dVc)
{
	CString sPe; sPe.Format(_T("%.1f"), dPe); // Check_ValueForm(8,2, Cng_CodeForFix(dPe));
	CString sAc; sAc.Format(_T("%.1f"), dAc); // Check_ValueForm(8,1, Cng_CodeLenFix(dAc,2));
	CString sfpc = Check_ValueForm(8,2, dfpc);
	CString sfck = Check_ValueForm(8,2, dfck);
	CString sbw  = Check_ValueForm(7,1, dbw);
	CString sd   = Check_ValueForm(7,1, dd);
	CString sdp  = Check_ValueForm(7,1, ddp);
	CString sVp  = Check_ValueForm(8,2, Cng_CodeForFix(dVp));
	CString sVcw = Check_ValueForm(8,2, Cng_CodeForFix(dVcw));
	CString sVci = Check_ValueForm(8,2, Cng_CodeForFix(dVci));
	CString sVc  = Check_ValueForm(8,2, Cng_CodeForFix(dVc));

	CString strCums = _T("");
	strCums.Format(_T("~ f_pc == %s / %s = %s ~ %s"),sPe, sAc, sfpc, m_sTxStress);            m_arText.Add(strCums);
	strCums = _T("");   m_arText.Add(strCums);

	if(m_iDgnCode==KSCE_USD05)
	{
		strCums = _T("<Equation> <Title>~ Vcw_b_KS05</Title> <Col>15</Col> </Equation>");   m_arText.Add(strCums);
		m_iFormula++;
		strCums.Format(_T("~ ~ ~ == ( 0.29 * √  ( %s ) + 0.3 * %s ) * %s * %s / 10^3 + %s = %s ~ %s"),
									 sfck, sfpc, sbw, sd, sVp, sVcw, m_strForce);        m_arText.Add(strCums);
	}
	else if(m_iDgnCode==KSCE_RAIL_USD04)
	{
		strCums = _T("<Equation> <Title>~ Vcw_b_rail</Title> <Col>15</Col> </Equation>");   m_arText.Add(strCums);
		m_iFormula++;
		strCums.Format(_T("~ ~ ~ == ( 0.29 * √  ( %s ) + 0.3 * %s ) * %s * %s / 10^3 + %s = %s ~ %s"),
									 sfck, sfpc, sbw, sdp, sVp, sVcw, m_strForce);        m_arText.Add(strCums);
	}

	strCums = _T("");   m_arText.Add(strCums);

	strCums.Format(_T("~ V_c == Minimum ~ ( V_ci , V_cw ) == Min ~ ( %s , %s ) == %s ~ %s"), sVci, sVcw, sVc, m_strForce);     m_arText.Add(strCums);  
	strCums = _T("");   m_arText.Add(strCums);
}

void CPrtKSCE_USD05::Print_CumsDesign7(double dVc, double dphiv, double d05pVc, double dpVc, double dVu, double dbw, 
																			 double dd, double dVp, double dVcw, double dVci)
{
	CString sVc    = Check_ValueForm(7,1, Cng_CodeForFix(dVc));
	CString sphiv  = Check_ValueForm(7,1, dphiv);
	CString s05pVc = Check_ValueForm(7,1, Cng_CodeForFix(d05pVc));
	CString spVc   = Check_ValueForm(7,1, Cng_CodeForFix(dpVc));
	CString sVu    = Check_ValueForm(7,1, Cng_CodeForFix(dVu));
	CString sbw    = Check_ValueForm(7,1, dbw);
	CString sd     = Check_ValueForm(7,1, dd);
	CString sVp    = Check_ValueForm(7,1, Cng_CodeForFix(dVp));
	CString sVcw   = Check_ValueForm(7,1, Cng_CodeForFix(dVcw));
	CString sVci   = Check_ValueForm(7,1, Cng_CodeForFix(dVci));

	CString strCums = _T("");  
	strCums.Format(_T("~ 1/2Φ  V_c == 1 / 2 * %s * %s ~ = %s ~ %s"), sVc, sphiv, s05pVc, m_strForce);                    m_arText.Add(strCums);
	strCums = _T("");   m_arText.Add(strCums);
	strCums.Format(_T("~ Φ  V_c == %s ~ * %s = %s ~ %s"),sphiv, sVc, spVc, m_strForce);      m_arText.Add(strCums);
	strCums = _T("");   m_arText.Add(strCums);
	strCums.Format(_T("~ V_u == %s ~ %s"), sVu, m_strForce);      m_arText.Add(strCums);
	strCums = _T("");   m_arText.Add(strCums);

	strCums = Get_ShearReinforcePrint(dpVc, d05pVc, dVu);      m_arText.Add(strCums);
	
}

void CPrtKSCE_USD05::Print_CumsDesign8(double dVu, double dVc, double dphiv, double dpVc, double d05pVc, double dpVn, double dfck, double dbw, double dd, double dVs_req, double dVs_max,
																			 double dVs, double dVs_min, double dsv, double dfy, double dThetaV, double dAv_use, double dAv_req, double dAvmin)
{
	CString sVu     = Check_ValueForm(8,2, Cng_CodeForFix(fabs(dVu)));
	CString sVc     = Check_ValueForm(8,2, Cng_CodeForFix(dVc));
	CString sphiv   = Check_ValueForm(7,2, dphiv);
	CString spVc    = Check_ValueForm(8,2, Cng_CodeForFix(dpVc));
	CString s05pVc  = Check_ValueForm(8,2, Cng_CodeForFix(d05pVc));
	CString spVn    = Check_ValueForm(8,2, Cng_CodeForFix(dpVn));
	CString sfck    = Check_ValueForm(8,2, dfck);
	CString sbw     = Check_ValueForm(7,1, dbw);
	CString sd      = Check_ValueForm(7,1, dd);
	CString sVs_req = Check_ValueForm(8,2, Cng_CodeForFix(dVs_req));
	CString sVs_min = Check_ValueForm(8,2, Cng_CodeForFix(dVs_min));
	CString sVs_max = Check_ValueForm(8,2, Cng_CodeForFix(dVs_max));

	CString sVs     = Check_ValueForm(8,2, Cng_CodeForFix(dVs));
	CString ssv     = Check_ValueForm(7,1, dsv);
	CString sfy     = Check_ValueForm(8,2, dfy);
	CString sThetaV = Check_ValueForm(7,3, dThetaV);
	CString sAv_use = Check_ValueForm(7,1, dAv_use);
	CString sAv_req = Check_ValueForm(7,1, dAv_req);
	CString sAvmin  = Check_ValueForm(7,1, dAvmin);
	
	CString strCums = _T("");
	CString strEqua = _T("");
	
	strCums = _T("Text: (4) 철근이 부담해야 하는 전단력 산정 및 전단 철근량 산정");       m_arText.Add(strCums);
	strCums = _T("Text:  - 전단철근이 부담하는 전단력");                                m_arText.Add(strCums);
	double dRad_Deg90 = 4.*atan(1.)/180.*90.;
	if(fabs(dThetaV-dRad_Deg90)<m_dZero)
	{
		strCums = _T("<Equation> <Title>~ Vs_rece2</Title> <Col>15</Col> </Equation>");       m_arText.Add(strCums);
		m_iFormula++;
		strCums = _T("");                                                                   m_arText.Add(strCums);
		strCums.Format(_T("~ ~ ~ == %s * %s * %s / %s / 10^3 = %s ~ %s"),
									sAv_use, sfy, sd, ssv, sVs, m_strForce);        m_arText.Add(strCums);
	}
	else
	{
		strCums = _T("<Equation> <Title>~ Vs_rece</Title> <Col>15</Col> </Equation>");       m_arText.Add(strCums);
		m_iFormula++;
		strCums = _T("");                                                                   m_arText.Add(strCums);
		strCums.Format(_T("~ ~ ~ == %s * %s * ( sin ~ ( %s ) + cos ~ ( %s ) ) * %s / %s / 10^3 = %s ~ %s"),
									sAv_use, sfy, sThetaV, sThetaV, sd, ssv, sVs, m_strForce);        m_arText.Add(strCums);
	}
	strCums = _T("");   m_arText.Add(strCums);
	
	strEqua = _T("<Equation> <Title>~ Vsmax_KS05</Title> <Col>15</Col> </Equation>");  m_arText.Add(strEqua);
	m_iFormula++;
	strCums = _T("");   m_arText.Add(strCums);
	strCums.Format(_T("~ ~ ~ ~ == ( 2 / 3 ) * √  ( %s ) * %s * %s / 10^3 = %s ~ %s"),
								 sfck, sbw, sd, sVs_max, m_strForce);                      m_arText.Add(strCums);
	m_arText.Add(_T(""));
 
	strCums.Format(_T("~ V_s == Minimum ~ ( V_s , V_s,max ) == Min ~ ( %s , %s ) == %s ~ %s"), sVs, sVs_max, sVs_min, m_strForce);     m_arText.Add(strCums);  
	m_arText.Add(_T(""));

	strCums = _T("Text:  - 최소 전단 철근량");                                       m_arText.Add(strCums);
	strCums = _T("<Equation> <Title>~ Avmin_b_KS05</Title> <Col>15</Col> </Equation>");       m_arText.Add(strCums);
	m_iFormula++;
	strCums = _T("");                                                                   m_arText.Add(strCums);
	strCums.Format(_T("~ ~ ~ == 0.35 * %s * %s / %s = %s ~ %s"), sbw, ssv, sfy, sAvmin, m_sTxArea);    m_arText.Add(strCums);
	m_arText.Add(_T(""));
	
	//
	strCums = _T("Text:  - 전단 철근량");                                       m_arText.Add(strCums);
	strCums.Format(_T("~ 1/2Φ  V_c == 1 / 2 * %s * %s ~ = %s ~ %s"), sphiv, sVc, s05pVc, m_strForce);                    m_arText.Add(strCums);
	if(dpVc>fabs(dVu) && d05pVc< fabs(dVu))
	{
		strCums = _T("~ ∴  ~ ~ Φ  V_c ~ > ~ V_u ~ , ~ 1/2Φ  V_c  <  ~ V_u  ~ ~ ~ ~ ~ ~ ~ ~ ⇒  최소 전단철근 보강");   m_arText.Add(strCums);
	}
	else if(d05pVc>=fabs(dVu))
	{
		strCums = _T("~ ∴  ~ 1/2Φ  V_c ~ > ~ V_u  ~ ~ ~ ~ ~ ~ ~ ~ ⇒  전단철근 보강 불필요");   m_arText.Add(strCums);
	}  
	else if(dpVc<fabs(dVu))
	{
		strCums = _T("~ ∴  ~ Φ  V_c ~ < ~ V_u  ~ ~ ~ ~ ~ ~ ~ ~ ⇒  전단철근 보강");   m_arText.Add(strCums);
		m_arText.Add(_T(""));
		strEqua = _T("<Equation> <Title>~ Vs_rbar</Title> <Col>15</Col> </Equation>");  m_arText.Add(strEqua);
		m_iFormula++;
		strCums.Format(_T("~ ~ ~ == ( %s - %s ) / %s ~ = %s ~ %s"),
									 sVu, spVc, sphiv, sVs_req, m_strForce);               m_arText.Add(strCums);
		m_arText.Add(_T(""));

		if(fabs(dThetaV-dRad_Deg90)<m_dZero)
		{
			strCums = _T("<Equation> <Title>~ Av_req2</Title> <Col>15</Col> </Equation>");       m_arText.Add(strCums);
			m_iFormula++;
			strCums = _T("");                                                                   m_arText.Add(strCums);
			strCums.Format(_T("~ ~ ~ == ( %s * %s ) / ( %s * %s ) = %s ~ %s"),
											sVs_req, ssv, sfy, sd, sAv_req, m_sTxArea);              m_arText.Add(strCums);
		}
		else
		{
			strCums = _T("<Equation> <Title>~ Av_req</Title> <Col>15</Col> </Equation>");       m_arText.Add(strCums);
			m_iFormula++;
			strCums = _T("");                                                                   m_arText.Add(strCums);
			strCums.Format(_T("~ ~ ~ == ( %s * %s ) / ( %s * ( sin ~ ( %s ) + cos ~ ( %s ) ) * %s ) = %s ~ %s"),
											sVs_req, ssv, sfy, sThetaV, sThetaV, sd, sAv_req, m_sTxArea);              m_arText.Add(strCums);
		}    
	}
	
	m_arText.Add(_T(""));

	strCums = Get_StrengthChkPrint(dpVn,dVu);       m_arText.Add(strCums);
	m_arText.Add(_T(""));
	m_arText.Add(_T(""));
//  strEqua = _T("<Equation> <Title>~ pVn</Title> <Col>15</Col> </Equation>");  m_arText.Add(strEqua);
//  strCums.Format(_T("~ ~ ~ ~ == %s * ( %s + %s ) = %s ~ %s"),sphiv, sVc, sVs , spVn, m_strForce);  m_arText.Add(strCums);
	
}

CString CPrtKSCE_USD05::Get_dValuePrint(double dh, double d08h, double dd, double ddp)
{
	CString sh      = Check_ValueForm(7,1, dh);
	CString s08h    = Check_ValueForm(7,1, d08h);
	CString sd      = Check_ValueForm(7,1, dd);
	CString sdp     = Check_ValueForm(7,1, ddp);

	CString strd=_T("");
	if(m_iDgnCode==KSCE_USD05)
	{
		if(d08h<=dd) strd.Format(_T("~ 0.8 ~ * %s = %s   <  ~ d == %s %s"),sh, s08h, sd, m_strLength);
		else             strd.Format(_T("~ 0.8 ~ * %s = %s   >  ~ d == %s %s"),sh, s08h, sd, m_strLength);
	}
	else if(m_iDgnCode==KSCE_RAIL_USD04)
	{
		if(d08h<=ddp) strd.Format(_T("~ 0.8 ~ * %s = %s   <  ~ d_p == %s %s"),sh, s08h, sdp, m_strLength);
		else              strd.Format(_T("~ 0.8 ~ * %s = %s   >  ~ d_p == %s %s"),sh, s08h, sdp, m_strLength);
	}
	
	return strd;
}

CString CPrtKSCE_USD05::Get_ShearReinforcePrint(double dpVc, double d05pVc, double dVu)
{
	CString strChk=_T("");
	if(dpVc>fabs(dVu) && d05pVc< fabs(dVu))
	{
		strChk = _T("~ ∴  ~ ~ Φ  V_c ~ > ~ V_u ~ , ~ 1/2Φ  V_c  <  ~ V_u  ~ ~ ~ ~ ~ ~ ~ ~ ⇒  최소 전단철근 보강");
	}
	else if(d05pVc>=fabs(dVu))
	{
		strChk = _T("~ ∴  ~ 1/2Φ  V_c ~ > ~ V_u  ~ ~ ~ ~ ~ ~ ~ ~ ⇒  전단철근 보강 불필요");
	}  
	else if(dpVc<fabs(dVu))
	{
		strChk = _T("~ ∴  ~ Φ  V_c ~ < ~ V_u  ~ ~ ~ ~ ~ ~ ~ ~ ⇒  전단철근 보강");
	}

	return strChk;
}

CString CPrtKSCE_USD05::Get_StrengthChkPrint(double dpVn, double dVu)
{
	CString spVn = Check_ValueForm(8,2, Cng_CodeForFix(dpVn));
	CString sVu = Check_ValueForm(8,2, Cng_CodeForFix(dVu));

	CString strChk=_T("");
	if(dpVn>dVu) strChk.Format(_T("~ Φ  V_n == Φ  ( V_c + V_s ) == %s ~ %s  ~ >  ~ V_u == %s ~ %s ~ ~ ~ ~ ~ O.K"), spVn, m_strForce, sVu, m_strForce);
	else         strChk.Format(_T("~ Φ  V_n == Φ  ( V_c + V_s ) == %s ~ %s  ~ <  ~ V_u == %s ~ %s ~ ~ ~ ~ ~ N.G"), spVn, m_strForce, sVu, m_strForce);
	
	return strChk;
}
 

// 비틀림강도 검토(Crmt) /////////////////////////////////////////////////////////////////////////////////////////
void CPrtKSCE_USD05::Print_CrmtTheory(CArray<CString,CString>& aString)
{
	CString strCrmt = _T("");
	CString strEqua = _T("");
	CString strSpace = _T("");
	for(int i=0; i<30; i++)
		strSpace += _T("~ ");

	strCrmt = _T("Text:  ▷ 비틀림 검토");        aString.Add(strCrmt);
	strCrmt = _T("");   aString.Add(strCrmt);

	strCrmt = _T("Text:   - 비틀림 모멘트를 무시할 수 있는 경우");        aString.Add(strCrmt);
	strEqua = _T("<Equation> <Title>~ ~ ~ Tcr_KS05</Title> <Col>30</Col> </Equation> "); 
	strCrmt.Format(_T("%s P_cp : ~ 전단면의`둘레길이"),strEqua);                    aString.Add(strCrmt);
	strCrmt = strSpace + _T("A_cp : ~ 콘크리트`단면의`바깥둘레로`둘러싸인");      aString.Add(strCrmt);
	strCrmt = strSpace + _T("~ ~ ~ ~ ~ 폐합`단면적");                               aString.Add(strCrmt);
	strCrmt = _T("");   aString.Add(strCrmt);

	
	strCrmt = _T("Text:   - 비틀림 모멘트를 무시할 수 없는 경우");                           aString.Add(strCrmt);
	strCrmt = _T("");   aString.Add(strCrmt);

	strCrmt = _T("Text:    ① 비틀림에 대한 수직 철근");                           aString.Add(strCrmt);
	strEqua = _T("<Equation> <Title>~ ~ ~ Tn</Title> <Col>30</Col> </Equation> "); 
	strCrmt.Format(_T("%s A_o ~ : ~ 전단흐름에`따라`폐쇄된`단면적"),strEqua);                           aString.Add(strCrmt);
	strCrmt = strSpace + _T("A_oh : ~ 비틀림`저항철근의`중심선으로");          aString.Add(strCrmt);
	strCrmt = strSpace + _T("~ ~ ~ ~ ~ 폐합된`단면적");                                aString.Add(strCrmt);
	strEqua = _T("<Equation> <Title>~ ~ ~ Ao</Title> <Col>30</Col> </Equation> "); 
	strCrmt.Format(_T("%s A_t ~ : ~ 거리`s`내의`비틀림`저항`폐쇄스터럽의"),strEqua);                           aString.Add(strCrmt);
	strCrmt = strSpace + _T("~ ~ ~ ~ ~ 다리 1개 단면적");                           aString.Add(strCrmt);
	strCrmt = _T("");   aString.Add(strCrmt);

	strCrmt = _T("~ ~ ~ θ  : ~ 압축 경사각");                           aString.Add(strCrmt);
	strCrmt = _T("~ ~ ~ 프리스트레싱`되지`않은`부재 ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ θ == 45 ~ ˚");               aString.Add(strCrmt);
	strCrmt = _T("~ ~ ~ 프리스트레스`힘이`주철근`인장강도의`40%미만 ~ θ == 45 ~ ˚");    aString.Add(strCrmt);
	strCrmt = _T("~ ~ ~ 프리스트레스`힘이`주철근`인장강도의`40%이상 ~ θ == 37.5 ˚");  aString.Add(strCrmt);
	strCrmt = _T("");   aString.Add(strCrmt);

	strCrmt = _T("Text:    ② 비틀림에 대한 종방향 철근");                           aString.Add(strCrmt);
	strEqua = _T("<Equation> <Title>~ ~ ~ Al</Title> <Col>30</Col> </Equation> "); 
	strCrmt.Format(_T("%s A_t / s : ~ 최소비틀림`철근량을`적용하지`않음"),strEqua);               aString.Add(strCrmt);
	strCrmt = strSpace + _T("P_h ~ ~ : ~ 외곽부`폐합`비틀림`철근의`중심선`둘레길이");                           aString.Add(strCrmt);
	strCrmt = _T("");   aString.Add(strCrmt);

	
	strCrmt = _T("Text:   - 단면치수 제한");                            aString.Add(strCrmt);
	strCrmt = _T("Text:     Solid Section");                            aString.Add(strCrmt);
	strEqua = _T("<Equation> <Title>~ ~ ~ SectSol_KS05</Title> <Col>30</Col> </Equation> ");		aString.Add(strEqua); 
	strCrmt = _T("");   aString.Add(strCrmt);
	strCrmt = _T("");   aString.Add(strCrmt);
	strCrmt = _T("");   aString.Add(strCrmt);

	
	strCrmt = _T("Text:     Hollow Section");                            aString.Add(strCrmt);
	strCrmt = _T("~ ~ ~ ①`벽의`두께가` A_oh / p_h  ~ 보다`큰`경우");                             aString.Add(strCrmt);
	strEqua = _T("<Equation> <Title>~ ~ ~ ~ SectHol1_KS05</Title> <Col>30</Col> </Equation> ");
	strCrmt.Format(_T("%s  t : 속 빈 단면의 벽 두께"),strEqua);                           aString.Add(strCrmt);
	strCrmt = _T("");   aString.Add(strCrmt);
	strCrmt = _T("");   aString.Add(strCrmt);

	strCrmt = _T("~ ~ ~ ②`벽의`두께가  A_oh / P_h 보다`작은`경우");                           aString.Add(strCrmt);
	strCrmt = _T("<Equation> <Title>~ ~ ~ ~ SectHol2_KS05</Title> <Col>0</Col> </Equation> ");     aString.Add(strCrmt);
	strCrmt = _T("");   aString.Add(strCrmt);
	strCrmt = _T("");   aString.Add(strCrmt);

	
	strCrmt = _T("Text:   - 최소 비틀림 철근량");                             aString.Add(strCrmt);
	strCrmt = _T("Text:    ① 횡방향 최소 철근량");                           aString.Add(strCrmt);
	strCrmt = _T("<Equation> <Title>~ ~ ~ Atmin_KS05</Title> <Col>0</Col> </Equation> ");     aString.Add(strCrmt);
	strCrmt = _T("");   aString.Add(strCrmt);
	strCrmt = _T("");   aString.Add(strCrmt);

	strCrmt = _T("Text:    ② 종방향 최소 철근량");                           aString.Add(strCrmt);
	strEqua = _T("<Equation> <Title>~ ~ ~ Almin_KS05</Title> <Col>30</Col> </Equation> ");   
	strCrmt.Format(_T("%s A_t / s : ~ 최소비틀림`철근량을`적용"),strEqua);             aString.Add(strCrmt);
	strCrmt = strSpace + _T("A_t / s ≥ 0.175 * b_w / f_y");                    aString.Add(strCrmt);
	strCrmt = _T("");   aString.Add(strCrmt);
	/*
	strCrmt = _T("Text: ▷ 사인장 철근 배근");                               aString.Add(strCrmt);
	strCrmt = _T("A_s : 1개의 복부에 배치된 철근량");               aString.Add(strCrmt);
	strCrmt = _T("A_s1 : 복부 1개당 필요한 사인장 철근량");         aString.Add(strCrmt);
	strCrmt = _T("A_s2 : 복부에 생기는 휨모멘트에 저항하기");       aString.Add(strCrmt);
	strCrmt = _T("Text: 위해 필요한 철근량");                             aString.Add(strCrmt);
	strCrmt = _T("A_t : 비틀림 모멘트에 대한 복부 1개당");          aString.Add(strCrmt);
	strCrmt = _T("Text: 횡방향 필요 철근량");                             aString.Add(strCrmt);
	*/
}

void CPrtKSCE_USD05::Print_CrmtDesign0()
{
	CString sfck = Check_ValueForm(8,2, m_CrmtParam.dfck);
	CString sbw  = Check_ValueForm(7,1, m_CrmtParam.dbw);
	CString st   = Check_ValueForm(7,1, m_CrmtParam.dt);
	CString sfy  = Check_ValueForm(8,2, m_CrmtParam.dfy);
	CString sh   = Check_ValueForm(7,1, m_CrmtParam.dh);
	CString sIy  = Check_ValueForm(9,3, Cng_CodeLenFix(m_CrmtParam.dIy,4));

	CString sfpu = Check_ValueForm(8,2, m_CrmtParam.dfpu);
	CString sd   = Check_ValueForm(7,1, m_CrmtParam.dd);
	CString sdp  = Check_ValueForm(7,1, m_CrmtParam.ddp);
	CString sAc  = Check_ValueForm(9,3, Cng_CodeLenFix(m_CrmtParam.dAc,2));
	CString sfpy = Check_ValueForm(8,2, m_CrmtParam.dfpy);
	CString syt  = Check_ValueForm(7,1, m_CrmtParam.dyt);
	CString sAps = Check_ValueForm(8,1, m_CrmtParam.dAps);
	CString sPcp = Check_ValueForm(9,1, m_CrmtParam.dPcp);
	CString sAcp = Check_ValueForm(9,3, Cng_CodeLenFix(m_CrmtParam.dAcp,2));

	CString sSpace4 = _T("~ ~ ~ ~ ");
	CString sSpace5 = _T("~ ~ ~ ~ ~ ");
	CString sSpace6 = _T("~ ~ ~ ~ ~ ~ ");
	CString sSpace7 = _T("~ ~ ~ ~ ~ ~ ~ ");
	CString sSpace8 = _T("~ ~ ~ ~ ~ ~ ~ ~ ");
	CString sSpace9 = _T("~ ~ ~ ~ ~ ~ ~ ~ ~ ");
	CString s1stSpace  = _T("");
	CString s2ndSpace  = _T("");

	int iMaxLength = 0;
	int arLength[3][5];

	int isfck   = arLength[0][0] = sfck.GetLength();
	int isfy    = arLength[0][1] = sfy.GetLength();
	int isfpu   = arLength[0][2] = sfpu.GetLength();
	int isbeta1 = arLength[0][3] = sfpy.GetLength();
	
	int isbw  = arLength[1][0] = sbw.GetLength();
	int ish   = arLength[1][1] = sh.GetLength();
	int isdp  = arLength[1][2] = sdp.GetLength();
	int isyt  = arLength[1][3] = syt.GetLength();
	int isPcp = arLength[1][4] = sPcp.GetLength();

	int ist   = arLength[2][0] = st.GetLength();
	int isIy  = arLength[2][1] = sIy.GetLength();
	int isAc  = arLength[2][2] = sAc.GetLength();
	int isAps = arLength[2][3] = sAps.GetLength();
	int isAcp = arLength[2][4] = sAcp.GetLength();

	// 1 Column.    
	for(int i=0; i<4; i++)  
		iMaxLength = max(iMaxLength,arLength[0][i]);  

	CString strfck, strfy, strfpu, strfpy;
	strfck  = Get_TextStringLength(iMaxLength, sfck);
	strfy   = Get_TextStringLength(iMaxLength, sfy);
	strfpu  = Get_TextStringLength(iMaxLength, sfpu);
	strfpy  = Get_TextStringLength(iMaxLength, sfpy);
	
	if(iMaxLength==10 || iMaxLength==11)    s1stSpace = sSpace4;
	else if(iMaxLength==9 || iMaxLength==8) s1stSpace = sSpace5;
	else if(iMaxLength==7)                  s1stSpace = sSpace6;
	else if(iMaxLength==6 || iMaxLength==5) s1stSpace = sSpace7;
	else if(iMaxLength==4 || iMaxLength==3) s1stSpace = sSpace8;
	else if(iMaxLength==2)                  s1stSpace = sSpace9;
	else ASSERT(0);

	CString s1colLength = _T("");
	for(int i=0; i<Get_XlLength(iMaxLength); i++)
		s1colLength += _T("~ ");
	
	s1colLength  = s1colLength + sSpace8 + sSpace4 + s1stSpace;

	// 2 Column.
	iMaxLength = 0;
	for(int i=0; i<5; i++)  
		iMaxLength = max(iMaxLength,arLength[1][i]);

	CString strbw, strh, strd, stryt, strPcp;
	strbw  = Get_TextStringLength(iMaxLength, sbw );
	strh   = Get_TextStringLength(iMaxLength, sh  );
	strd   = Get_TextStringLength(iMaxLength, sd  );
	stryt  = Get_TextStringLength(iMaxLength, syt );
	strPcp = Get_TextStringLength(iMaxLength, sPcp);

	if(iMaxLength==10 || iMaxLength==11)    s2ndSpace = sSpace4;
	else if(iMaxLength==9 || iMaxLength==8) s2ndSpace = sSpace5;
	else if(iMaxLength==7)                  s2ndSpace = sSpace6;
	else if(iMaxLength==6 || iMaxLength==5) s2ndSpace = sSpace7;
	else if(iMaxLength==4 || iMaxLength==3) s2ndSpace = sSpace8;
	else if(iMaxLength==2)                  s2ndSpace = sSpace9;

	// 3 Column.
	iMaxLength = 0;
	for(int i=0; i<5; i++)  
		iMaxLength = max(iMaxLength,arLength[2][i]);

	CString strt, strIy, strAc, strAps, strAcp;
	strt   = Get_TextStringLength(iMaxLength, st );
	strIy  = Get_TextStringLength(iMaxLength, sIy );
	strAc  = Get_TextStringLength(iMaxLength, sAc );
	strAps = Get_TextStringLength(iMaxLength, sAps);
	strAcp = Get_TextStringLength(iMaxLength, sAcp);
	
	if(sIy.GetLength()>=9)  strIy.TrimRight(_T("~ "));
	if(sAc.GetLength()>=9)  strAc.TrimRight(_T("~ "));
	if(sAcp.GetLength()>=9) strAcp.TrimRight(_T("~ "));

	CString strCrmt = _T("");
	strCrmt = _T("Text: 비틀림 강도 검토");
	strCrmt = _T("Text: (1) 설계 변수");                                                m_arText.Add(strCrmt);
	strCrmt.Format(_T("~ ~ f_ck : ~ %s%s %s b_w ~ : ~ %s%s %s t ~ ~ : ~ %s%s"),
								 strfck, m_sTxStress, s1stSpace, strbw, m_strLength, s2ndSpace, strt, m_strLength);             m_arText.Add(strCrmt);
	strCrmt.Format(_T("~ ~ f_y ~ : ~ %s%s %s h ~ ~ : ~ %s%s %s I_y ~ : ~ %s %s"),
								 strfy,m_sTxStress, s1stSpace, strh, m_strLength, s2ndSpace, strIy, m_sTxStif);    m_arText.Add(strCrmt);

	strCrmt.Format(_T("~ ~ f_pu : ~ %s%s %s d ~ ~ : ~ %s%s %s A_c ~ : ~ %s %s"),
								 strfpu, m_sTxStress, s1stSpace, strd, m_strLength, s2ndSpace, strAc,m_sTxArea);    m_arText.Add(strCrmt);
	strCrmt.Format(_T("~ ~ f_py : ~ %s%s %s y_t ~ : ~ %s%s %s A_p ~ : ~ %s%s"),
								 strfpy, m_sTxStress, s1stSpace, stryt,m_strLength, s2ndSpace, strAps,m_sTxArea);    m_arText.Add(strCrmt);
	strCrmt.Format(_T("%s P_cp : ~ %s%s %s A_cp : ~ %s %s "),
								 s1colLength, strPcp, m_strLength, s2ndSpace, strAcp, m_sTxArea);    m_arText.Add(strCrmt);
	strCrmt = _T("");   m_arText.Add(strCrmt);

}

void CPrtKSCE_USD05::Print_CrmtDesign1(double dfck, double dbw, double dt , double dfy, double dh, double dIy)
{
	CString sfck = Check_ValueForm(8,2, dfck);
	CString sbw  = Check_ValueForm(7,1, dbw);
	CString st   = Check_ValueForm(7,1, dt);
	CString sfy  = Check_ValueForm(8,2, dfy);
	CString sh   = Check_ValueForm(7,1, dh);
	CString sIy  = Check_ValueForm(9,3, Cng_CodeLenFix(dIy,4));

	CString strCrmt = _T("");
	strCrmt = _T("Text: 비틀림 강도 검토");
	strCrmt = _T("Text: (1) 설계 변수");                                                m_arText.Add(strCrmt);
	strCrmt.Format(_T("~ ~ f_ck : ~ %s ~ ~ %s ~ ~ ~ ~ ~ b_w ~ : ~ %s ~ %s ~ ~ ~ ~ ~ ~ t ~ ~ : ~ %s ~ ~ ~ %s"),
								 sfck, m_sTxStress, sbw, m_strLength, st, m_strLength);             m_arText.Add(strCrmt);
	strCrmt.Format(_T("~ ~ f_y ~ : ~ %s ~ ~ %s ~ ~ ~ ~ ~ h ~ ~ : ~ %s ~ %s ~ ~ ~ ~ ~ ~ I_y ~ : ~ %s %s"),
								 sfy,m_sTxStress, sh, m_strLength, sIy, m_sTxStif);    m_arText.Add(strCrmt);
} 

void CPrtKSCE_USD05::Print_CrmtDesign2(double dfpu, double dd, double ddp, double dAc, double dfpy, double dyt, double dAps, 
																			 double dPcp, double dAcp)
{
	CString sfpu = Check_ValueForm(8,2, dfpu);
	CString sd   = Check_ValueForm(7,1, dd);
	CString sdp  = Check_ValueForm(7,1, ddp);
	CString sAc  = Check_ValueForm(8,1, Cng_CodeLenFix(dAc,2));
	CString sfpy = Check_ValueForm(8,2, dfpy);
	CString syt  = Check_ValueForm(7,1, dyt);
	CString sAps = Check_ValueForm(8,1, dAps);
	CString sPcp = Check_ValueForm(8,1, dPcp);
	CString sAcp = Check_ValueForm(8,1, Cng_CodeLenFix(dAcp,2));

	CString strCrmt = _T("");
	strCrmt.Format(_T("~ ~ f_pu : ~ %s ~ %s ~ ~ ~ ~ ~ d ~ ~ : ~ %s ~ %s ~ ~ ~ ~ ~ ~ A_c ~ : ~ %s ~ %s"),
								 sfpu,m_sTxStress, sd, m_strLength, sAc,m_sTxArea);    m_arText.Add(strCrmt);
	strCrmt.Format(_T("~ ~ f_py : ~ %s ~ %s ~ ~ ~ ~ ~ y_t ~ : ~ %s ~ %s ~ ~ ~ ~ ~ ~ A_ps : ~ %s ~ ~ ~ %s"),
								 sfpy,m_sTxStress, syt,m_strLength , sAps,m_sTxArea);    m_arText.Add(strCrmt);
	strCrmt.Format(_T("~ ~ P_cp : ~ %s ~ ~ %s ~ ~ ~ ~ ~ ~ ~ ~ A_cp : ~ %s %s "),
								 sPcp,m_strLength, sAcp,m_sTxArea);    m_arText.Add(strCrmt);
	strCrmt = _T("");   m_arText.Add(strCrmt);
}

void CPrtKSCE_USD05::Print_CrmtDesign3(double dMu, double dTu, double dVu, double dMd, double dVd, double dfd, double dPe, double dfpe)
{
	CString sMu  = Check_ValueForm(8,2, Cng_CodeMomFix(dMu));
	CString sTu  = Check_ValueForm(8,2, Cng_CodeMomFix(dTu));
	CString sVu  = Check_ValueForm(8,2, Cng_CodeForFix(dVu));
	CString sMd  = Check_ValueForm(8,2, Cng_CodeMomFix(dMd));
	CString sVd  = Check_ValueForm(8,2, Cng_CodeForFix(dVd));
	CString sfd  = Check_ValueForm(8,2, dfd);
	CString sPe  = Check_ValueForm(8,2, Cng_CodeForFix(dPe));
	CString sfpe = Check_ValueForm(8,2, dfpe);

	CString strStress=m_sTxStress;
	CString strCrmt = _T("");
	CString strDum1, strDum2, strDum3;
	strCrmt = _T("Text: (2) 설계 단면력");                 m_arText.Add(strCrmt);
	strCrmt = _T("<Table>설계 단면력");                                                                   m_arText.Add(strCrmt);  
	if(m_iVciType==1)
	{
		strDum1 = _T("<MH>계수하중  ~ ~ ~ ~ ~ ~   ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~</MH> ");
		strDum2 = _T("<MH>사용하중  ~ ~ ~ ~ ~ ~   ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~</MH> ");
		strDum3 = _T("<MH>유효프리스트레스력  ~ ~ ~ ~ ~ ~   ~ ~ ~ ~ ~ ~ ~</MH> ");
		strCrmt = strDum1 + strDum2 + strDum3;                  m_arText.Add(strCrmt);
		strDum1 = _T("<MH>Mu ~ ~ ~ ~ ~ ~</MH> <MH>Tu ~ ~ ~ ~ ~ ~</MH> <MH>Vu ~ ~ ~ ~ ~ ~</MH> ");
		strDum2 = _T("<MH>Md ~ ~ ~ ~ ~ ~</MH> <MH>Vd ~ ~ ~ ~ ~ ~</MH> <MH>fd ~ ~ ~ ~ ~ ~</MH> ");
		strDum3 = _T("<MH>Pe ~ ~ ~ ~ ~ ~</MH> <MH>fpe ~ ~ ~ ~ ~ ~</MH>");
		strCrmt = strDum1 + strDum2 + strDum3;                  m_arText.Add(strCrmt);
		strDum1.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> "),
										m_strMoment, m_strMoment, m_strForce);
		strDum2.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> "),
										m_strMoment, m_strForce, strStress);
		strDum3.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH>"),m_strForce, strStress);
		strCrmt = strDum1 + strDum2 + strDum3;                  m_arText.Add(strCrmt);
		strDum1.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> "),
										sMu, sTu, sVu);
		strDum2.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> "),
										sMd, sVd, sfd);
		strDum3.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH>"),sPe,sfpe);
		strCrmt = strDum1 + strDum2 + strDum3;                  m_arText.Add(strCrmt);
	}
	else if(m_iVciType==2)
	{
		strDum1 = _T("<MH>계수하중  ~ ~ ~ ~ ~ ~   ~ ~ ~ ~ ~ ~ ~  ~ ~ ~ ~ ~ ~ ~</MH> ");
		strDum3 = _T("<MH>유효프리스트레스력  ~ ~ ~ ~ ~ ~   ~ ~ ~ ~ ~ ~ ~</MH> ");
		strCrmt = strDum1 + strDum3;                  m_arText.Add(strCrmt);
		strDum1 = _T("<MH>Mu ~ ~ ~ ~ ~ ~</MH> <MH>Tu ~ ~ ~ ~ ~ ~</MH> <MH>Vu ~ ~ ~ ~ ~ ~</MH> ");
		strDum3 = _T("<MH>Pe ~ ~ ~ ~ ~ ~</MH> <MH>fpe ~ ~ ~ ~ ~ ~</MH>");
		strCrmt = strDum1 + strDum3;                  m_arText.Add(strCrmt);
		strDum1.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH> "),
										m_strMoment, m_strMoment, m_strForce);
		strDum3.Format(_T("<MH>(%s) ~ ~ ~ ~ ~ ~</MH> <MH>(%s) ~ ~ ~ ~ ~ ~</MH>"),m_strForce, strStress);
		strCrmt = strDum1 + strDum3;                  m_arText.Add(strCrmt);
		strDum1.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH> "),
										sMu, sTu, sVu);
		strDum3.Format(_T("<MH>%s ~ ~ ~ ~ ~ ~</MH> <MH>%s ~ ~ ~ ~ ~ ~</MH>"),sPe,sfpe);
		strCrmt = strDum1 + strDum3;                  m_arText.Add(strCrmt);
	}
	strCrmt = _T("</Table>");    m_arText.Add(strCrmt);
	strCrmt = _T("");            m_arText.Add(strCrmt); 

}

void CPrtKSCE_USD05::Print_CrmtDesign4(double dfck, double dAcp, double dPcp, double dfpc, double dTcr, double dTu, double dfpe)
{
	CString sfck = Check_ValueForm(7,1, dfck);
	CString sAcp;  sAcp.Format(_T("%.1f"), dAcp); // Check_ValueForm(7,1, dAcp);
	CString sPcp = Check_ValueForm(7,1, dPcp);
	CString sfpc = Check_ValueForm(7,2, dfpc);
	CString sTcr = Check_ValueForm(9,1, Cng_CodeForFix(dTcr));
	CString sTu  = Check_ValueForm(7,1, Cng_CodeForFix(dTu));
	CString sfpe = Check_ValueForm(7,1, dfpe);

	CString strCrmt = _T("");
	CString strEqua = _T("");
	strCrmt = _T("Text: (3) 비틀림을 검토해야 하는 경우");                           m_arText.Add(strCrmt);
	strEqua = _T("<Equation> <Title>~ Tcr_KS05</Title> <Col>0</Col> </Equation> ");      m_arText.Add(strEqua);
	m_iFormula++;
	strCrmt = _T("");   m_arText.Add(strCrmt);   m_arText.Add(strCrmt);
	strCrmt.Format(_T("~ ~ == 0.8 ~ * ( 1 / 12 ) * √  ( %s ) * %s^2 / %s * √  ( 1 + %s / ( 1 / 3 * √  ( %s ) ) ) / 10^6"),
								 sfck, sAcp, sPcp, sfpc, sfck);    m_arText.Add(strCrmt);
	strCrmt = Get_TorChkPrint(dTcr, dTu);                     m_arText.Add(strCrmt);
	strCrmt = _T("");            m_arText.Add(strCrmt);
}
	
void CPrtKSCE_USD05::Print_CrmtDesign5()
{
	CString strCrmt = _T("");
	strCrmt = _T("Text: (4) 콘크리트가 부담하는 전단강도의 산정");       m_arText.Add(strCrmt);
	strCrmt = _T("");            m_arText.Add(strCrmt);
}

void CPrtKSCE_USD05::Print_CrmtDesign6(BOOL bHollow, double dAohOverPh, double dVu, double dVc, double dbw, double dd, double ddp, double dTu,
																			 double dfck, double dTcr, double dPh, double dAoh, double dt, double dvVT, double dvVTmax)
{
	CString sAohOverPh = Check_ValueForm(7,1, dAohOverPh);
	CString sVu  = Check_ValueForm(7,1, Cng_CodeForFix(dVu));
	CString sVc  = Check_ValueForm(7,1, Cng_CodeForFix(dVc));
	CString sbw  = Check_ValueForm(7,1, dbw);
	CString sd   = Check_ValueForm(7,1, dd);
	CString sdp  = Check_ValueForm(7,1, ddp);
	CString sTu  = Check_ValueForm(7,1, Cng_CodeMomFix(dTu));
	CString sfck = Check_ValueForm(7,1, dfck);
	//CString sTcr = Check_ValueForm(7,1, Cng_CodeMomFix(dTcr));
	CString sPh  = Check_ValueForm(7,1, dPh);
	CString sAoh; sAoh.Format(_T("%.1f"), dAoh); // Check_ValueForm(7,1, dAoh);
	CString st   = Check_ValueForm(7,1, dt);
	CString svVT = Check_ValueForm(7,1, dvVT);
	CString svVTmax  = Check_ValueForm(7,1, dvVTmax);

	CString strCrmt = _T("");
	CString strEqua = _T("");
	CString str1, str2;
	strCrmt = _T("Text: (5) 단면의 치수 제한");       m_arText.Add(strCrmt);
	if(!bHollow)
	{
		strCrmt = _T("Text: - 속찬 단면");   m_arText.Add(strCrmt);      
		strEqua = _T("<Equation> <Title>~ SectSol_KS05</Title> <Col>0</Col> </Equation> ");  m_arText.Add(strEqua);
		m_iFormula++;
		strCrmt = _T("");   m_arText.Add(strCrmt); m_arText.Add(strCrmt);
		str1.Format(_T("( %s / ( %s * %s ) )^2"), sVu, sbw, sd);  
		str2.Format(_T("( %s * %s / ( 1.7 * %s^2 ) )^2"), sTu, sPh, sAoh);
		strCrmt.Format(_T("~ √  ( %s + %s ) = %s"), str1, str2, svVT);     m_arText.Add(strCrmt);
		strCrmt = _T(""); m_arText.Add(strCrmt);
	}
	else
	{
		strCrmt = _T("Text: - 속빈 단면");     m_arText.Add(strCrmt);
		if(dt >= dAohOverPh)
		{
			strCrmt = _T("~ - 벽의 두께가 A_oh / P_h 보다 큰 경우"); m_arText.Add(strCrmt);
			strEqua = _T("<Equation> <Title>~ SectHol1_KS05</Title> <Col>0</Col> </Equation> ");  m_arText.Add(strEqua);
			m_iFormula++;
			strCrmt = _T("");   m_arText.Add(strCrmt); m_arText.Add(strCrmt);
			str1.Format(_T("%s / ( %s * %s )"), sVu, sbw, sd);  
			str2.Format(_T("%s * %s / ( 1.7 * %s^2 )"),sTu, sPh , sAoh);
			strCrmt.Format(_T("%s + %s = %s"), str1, str2, svVT);     m_arText.Add(strCrmt);                
			strCrmt = _T(""); m_arText.Add(strCrmt);
		}
		if(dt< dAohOverPh)
		{
			strCrmt = _T("~ - 벽의 두께가 A_oh / P_h 보다 작은 경우"); m_arText.Add(strCrmt);
			strEqua = _T("<Equation> <Title>~ SectHol2_KS05</Title> <Col>0</Col> </Equation> ");  m_arText.Add(strEqua);
			m_iFormula++;
			strCrmt = _T("");   m_arText.Add(strCrmt); m_arText.Add(strCrmt);
			

			str1.Format(_T("%s / ( %s * %s )"), sVu, sbw, sd);
			str2.Format(_T("%s * %s / ( 1.7 * %s * %s )"),sTu, sPh , sAoh, st);
			strCrmt.Format(_T("%s + %s = %s"), str1, str2, svVT);     m_arText.Add(strCrmt);                
			strCrmt = _T(""); m_arText.Add(strCrmt);
		}

	}
	str1.Format(_T("%s / ( %s * %s )"), sVc, sbw, sd);  
	str2.Format(_T("( 2 / 3 ) * √  ( %s )"), sfck);
	strCrmt.Format(_T("~ %s  ~ 0.8 ~ * ( %s + %s ) = %s ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ %s"), (dvVT<=dvVTmax ? _T("≤") : _T("≥")),
								 str1, str2, svVTmax, (dvVT<=dvVTmax ? _T("O.K") : _T("N.G")) ); m_arText.Add(strCrmt);
	
	strCrmt = _T(""); m_arText.Add(strCrmt);

	}

void CPrtKSCE_USD05::Print_CrmtDesign7(double dAoh, double dAo, double dAt, double dfyv, double dst, double dThetaC,
																			 double dpTn, double dfyl, double dPh, double dAlmin)
{
	CString sAoh; sAoh.Format(_T("%.1f"), dAoh); // Check_ValueForm(9,1, dAoh);
	CString sAo;  sAo.Format(_T("%.1f"), dAo); // Check_ValueForm(9,1, dAo);
	CString sAt  = Check_ValueForm(7,1, dAt);
	CString sfyv = Check_ValueForm(8,2, dfyv);
	CString sst  = Check_ValueForm(7,1, dst);
	CString sThetaC = Check_ValueForm(7,3, dThetaC);
	CString spTn = Check_ValueForm(8,2, Cng_CodeMomFix(dpTn));
	CString sfyl = Check_ValueForm(8,2, dfyl);
	CString sPh  = Check_ValueForm(9,1, dPh);
	CString sAlmin  = Check_ValueForm(7,1, dAlmin);
 
	CString strCrmt = _T("");
	CString strEqua = _T("");
	strCrmt = _T("Text: (6) 비틀림에 대한 보강 철근");       m_arText.Add(strCrmt);

	strCrmt = _T("Text:  - 비틀림에 대한 수직 철근");      m_arText.Add(strCrmt);

	strEqua = _T("<Equation> <Title>~ Ao</Title> <Col>9</Col> </Equation> ");  m_arText.Add(strEqua);
	m_iFormula++;
	strCrmt.Format(_T("~ ~ ~ == 0.85 * %s = %s ~ %s"), sAoh, sAo, m_sTxArea);  m_arText.Add(strCrmt);

	strEqua = _T("<Equation> <Title>~ Tn</Title> <Col>0</Col> </Equation> ");   m_arText.Add(strEqua);
	m_iFormula++;
	strCrmt = _T(""); m_arText.Add(strCrmt);
	strCrmt.Format(_T("~ ~ ~ == 2 * %s * %s * %s / %s * ( 1 / tan ~ ( %s ) ) = %s ~ %s"),
								 sAo, sAt, sfyv, sst, sThetaC, spTn, m_strMoment);    m_arText.Add(strCrmt);
	strCrmt = _T("");            m_arText.Add(strCrmt);

	strCrmt = _T("Text:  - 비틀림에 대한 종방향 철근");      m_arText.Add(strCrmt);

	strEqua = _T("<Equation> <Title>~ Al</Title> <Col>0</Col> </Equation> ");   m_arText.Add(strEqua);
	m_iFormula++;
	strCrmt = _T(""); m_arText.Add(strCrmt);
	strCrmt.Format(_T("~ ~ ~ == ( %s / %s ) * %s * ( %s / %s ) * ( 1 / tan ~ ( %s ) )^2 = %s ~ %s"),
									sAt, sst, sPh, sfyv, sfyl, sThetaC, sAlmin, m_sTxArea);   m_arText.Add(strCrmt);
	strCrmt = _T(""); m_arText.Add(strCrmt);
}

void CPrtKSCE_USD05::Print_CrmtDesign8(double dbw, double dst, double dfyv, double dAv_use, double dAtmin, 
																			 double dfck, double dAcp, double dAt, double dPh, double dfyl, double dAlmin, 
																			 double dAts_cal, double dbwfyv, double dAts)
{
	CString sbw     = Check_ValueForm(7,1, dbw);
	CString sst     = Check_ValueForm(7,1, dst);
	CString sfyv    = Check_ValueForm(8,2, dfyv);
	CString sAv_use = Check_ValueForm(7,1, dAv_use);
	CString sAtmin  = Check_ValueForm(8,1, dAtmin);
	CString sfck    = Check_ValueForm(8,2, dfck);
	CString sAcp;   sAcp.Format(_T("%.1f"), dAcp);
	CString sAt     = Check_ValueForm(7,1, dAt);
	CString sPh;    sPh.Format(_T("%.1f"), dPh); // Check_ValueForm(8,1, dPh);
	CString sfyl    = Check_ValueForm(9,1, dfyl);
	CString sAlmin  = Check_ValueForm(8,1, dAlmin);
	CString sAts_cal= Check_ValueForm(9,2, dAts_cal);
	CString sbwfyv  = Check_ValueForm(9,2, dbwfyv);
	CString sAts    = Check_ValueForm(9,2, dAts);

	CString strCrmt = _T("");
	CString strEqua = _T("");
	CString str1, str2;
	strCrmt = _T("Text: (7) 최소 비틀림 철근량");       m_arText.Add(strCrmt);

	strCrmt = _T("Text:  - 횡방향 최소 철근량");       m_arText.Add(strCrmt);

	strEqua = _T("<Equation> <Title>~ Atmin_KS05</Title> <Col>0</Col> </Equation> ");   m_arText.Add(strEqua);    
	m_iFormula++;
	strCrmt = _T(""); m_arText.Add(strCrmt);
	strCrmt.Format(_T("~ ~ A_l == ( 0.35 * %s * %s / %s - %s ) / 2 = %s ~ %s"),
								sbw, sst, sfyv, sAv_use, sAtmin, m_sTxArea);    m_arText.Add(strCrmt);
	m_arText.Add(_T(""));
	
	strCrmt = _T("Text:  - 종방향 최소 철근량");                                                m_arText.Add(strCrmt);
	strCrmt.Format(_T("~ A_t / s == %s / %s = %s"), sAt, sst, sAts_cal);                        m_arText.Add(strCrmt);
	strCrmt.Format(_T("~ 0.175 b_w / f_yv == 0.175 * %s / %s = %s"), sbw, sfyv, sbwfyv);         m_arText.Add(strCrmt);
	strCrmt.Format(_T("~ A_t / s == Max ( %s , %s ) == %s"), sAts_cal, sbwfyv, sAts);              m_arText.Add(strCrmt);
	m_arText.Add(_T(""));

	strEqua = _T("<Equation> <Title>~ Almin_KS05</Title> <Col>0</Col> </Equation> ");   m_arText.Add(strEqua);    
	m_iFormula++;
	strCrmt = _T(""); m_arText.Add(strCrmt);
	str1.Format(_T("~ ~ ~ == 0.42 * √  ( %s ) * %s / %s "), sfck, sAcp, sfyl);
	str2.Format(_T("- ( %s ) * %s * ( %s / %s ) = %s ~ %s"),
							sAts, sPh, sfyv, sfyl, sAlmin, m_sTxArea);
	strCrmt = str1 + str2; m_arText.Add(strCrmt);
}

CString CPrtKSCE_USD05::Get_TorChkPrint(double dTcr, double dTu)
{
	CString sTcr = Check_ValueForm(10,2,Cng_CodeMomFix(dTcr));
	CString sTu = Check_ValueForm(10,2,Cng_CodeMomFix(dTu));

	CString strCrmt = _T("");
	if(dTcr >= fabs(dTu)) strCrmt.Format(_T("~ ~ == %s ~ %s  ~ ＞ ~ T_u == %s ~ %s ~ ~ ~ ~ ~ ~ ⇒ 비틀림 고려 불필요"),sTcr,m_strMoment, sTu,m_strMoment);
	else                  strCrmt.Format(_T("~ ~ == %s ~ %s  ~ ＜ ~ T_u == %s ~ %s ~ ~ ~ ~ ~ ~ ⇒ 비틀림 고려 필요"),sTcr,m_strMoment, sTu,m_strMoment);
																
	return strCrmt;
}

BOOL CPrtKSCE_USD05::Get_ForceDetlXLData(CString strTitle, CArray<Prt_ArString,Prt_ArString&>& arStrChk)
{
	UINT iInt = 8;
	UINT iDec = 3;

	arStrChk.RemoveAll();
	int i,j;
	CString sStrChk;
	Prt_ArString PrtArStr;
	CArray<CString,CString> arString;
	if(m_arElemK.GetSize()==0) return FALSE;
		
	int iTotalElemK = m_arElemK.GetSize(); 
	// SetSize
	arStrChk.SetSize(iTotalElemK);
	for(int k=0; k<m_arElemK.GetSize(); k++)
	{
		auto ElemK = m_arElemK.GetAt(k); 
		PrtArStr.Initialize(); 
		arString.RemoveAll();

		// 단면속성.
		_PRT_FORC_DETL_SECT  DetlSect;
		if(!Get_ForcDetlSect(ElemK, FALSE, DetlSect)) continue; // FALSE: Curr 단위로.
		// 최종 시공단계 부재력.
		_PRT_FORC_DETL_STAG  DetlStag;
		if(!Get_ForcDetlStag(ElemK, DetlStag)) continue;
		// 사용하중조건종류별 부재력 (시공단계 부재력 제외).
		_PRT_FORC_DETL_LCAS  DetlLcas;
		if(!Get_ForcDetlLcas(ElemK, DetlLcas)) continue;
		// 사용하중조합종류별 부재력.
		_PRT_FORC_DETL_LCOM LcomPrev, LcomNext;
		if(!Get_ForcDetlLcom(DetlStag, DetlLcas, LcomPrev, LcomNext)) continue;

		for(i=0; i<2; i++)  //0:I, 1:J
		{
			CString strIJ = (i==0 ? _T("I") : _T("J"));
			CString sSectTitle[3];
			sSectTitle[0] = _T("~ Gross");
			sSectTitle[1] = _T("~ Net");
			sSectTitle[2] = _T("~ CPR");

			sStrChk = _T("-----------------------------------------------------------------------------------------------------------------------"); arString.Add(sStrChk);
			sStrChk.Format(_T("Stress Analysis Element:  %d  Position:  %s"),ElemK, strIJ);      arString.Add(sStrChk);
			sStrChk = _T("***********************************************************************************************************************"); arString.Add(sStrChk);
			sStrChk.Format(_T("Cross Section Area(%s) Iy(%s) Iz(%s) Zyt(%s) Zyb(%s)"),m_sTxArea, m_sTxStif, m_sTxStif, m_sTxSecM, m_sTxSecM);
			arString.Add(sStrChk);
			for(j=0; j<3; j++)
			{
				_PRT_SECT_BASE SectBase = DetlSect.SectBase[j];
				double dArea, dIy, dIz, dZyt, dZyb;
				CString sArea, sIy, sIz, sZyt, sZyb;
		
				dArea = SectBase.dArea[i];  sArea.Format(_T("%.1f"), dArea); // Check_ValueForm(9, 3, dArea);
				dIy   = SectBase.dIy[i];    sIy.Format(_T("%.1f"), dIy);     // Check_ValueForm(9, 3, dIy);
				dIz   = SectBase.dIz[i];    sIz.Format(_T("%.1f"), dIz);     // Check_ValueForm(9, 3, dIz);
				dZyt  = SectBase.dZyt[i];   sZyt.Format(_T("%.1f"), dZyt);   // Check_ValueForm(9, 3, dZyt);
				dZyb  = SectBase.dZyb[i];   sZyb.Format(_T("%.1f"), dZyb);   // Check_ValueForm(9, 3, dZyb);

				sStrChk.Format(_T("%s %s %s %s %s %s "), sSectTitle[j], sArea, sIy, sIz, sZyt, sZyb);  arString.Add(sStrChk);
			}
			sStrChk = _T("");           arString.Add(sStrChk);

			// XLData 최종 시공단계 부재력.
			sStrChk.Format(_T("~ ~ Fxx(%s) Muy(%s) Muz(%s) Stop(%s) Sbot(%s)"),m_strForce,m_strMoment,m_strMoment,m_sTxStress,m_sTxStress);
			arString.Add(sStrChk);
			sStrChk = _T("-----------------------------------------------------------------------------------------------------------------------"); arString.Add(sStrChk);
			CArray<_PRT_FORC_BASE,_PRT_FORC_BASE> ForcStage;
			ForcStage.Add(DetlStag.ForcDL);
			ForcStage.Add(DetlStag.ForcEL);
			ForcStage.Add(DetlStag.ForcTP);
			ForcStage.Add(DetlStag.ForcTS);
			ForcStage.Add(DetlStag.ForcCSP);
			ForcStage.Add(DetlStag.ForcCSS);
			ForcStage.Add(DetlStag.ForcSum);

			CString sStageTitle[7];
			sStageTitle[0] = _T("PREV DL");
			sStageTitle[1] = _T("~ EL");
			sStageTitle[2] = _T("~ TP");
			sStageTitle[3] = _T("~ TS");
			sStageTitle[4] = _T("~ CSP");
			sStageTitle[5] = _T("~ CSS");
			sStageTitle[6] = _T("~ Sum");
	
			for(j=0; j<ForcStage.GetSize(); j++)
			{
				double dFxx, dMuy, dMuz, dStop, dSbot;
				CString sFxx, sMuy, sMuz, sStop, sSbot;
				_PRT_FORC_BASE Force = ForcStage.GetAt(j);
				dFxx  = Force.dFxx[i];   sFxx  = Check_ValueForm(iInt, iDec, dFxx);
				dMuy  = Force.dMuy[i];   sMuy  = Check_ValueForm(iInt, iDec, dMuy);
				dMuz  = Force.dMuz[i];   sMuz  = Check_ValueForm(iInt, iDec, dMuz);
				dStop = Force.dStl[i];   sStop = Check_ValueForm(iInt, 2   , dStop);
				dSbot = Force.dSbl[i];   sSbot = Check_ValueForm(iInt, 2   , dSbot);

				sStrChk.Format(_T("%s %s %s %s %s %s "),sStageTitle[j], sFxx, sMuy, sMuz, sStop, sSbot);  arString.Add(sStrChk);
			}
			sStrChk = _T("");           arString.Add(sStrChk);

			// 사용하중조건종류별 부재력 (시공단계 부재력 제외).
			CArray<_PRT_FORC_BASE,_PRT_FORC_BASE> ForcLcas;
			ForcLcas.Add(DetlLcas.ForcDL);
			ForcLcas.Add(DetlLcas.ForcLLmax);
			ForcLcas.Add(DetlLcas.ForcLLmin);
			ForcLcas.Add(DetlLcas.ForcADDmax);
			ForcLcas.Add(DetlLcas.ForcADDmin);

			CString sLcasTitle[5];
			sLcasTitle[0] = _T("SING ForcDL");
			sLcasTitle[1] = _T("~ LLmax");
			sLcasTitle[2] = _T("~ LLmin");
			sLcasTitle[3] = _T("~ ADDmax");
			sLcasTitle[4] = _T("~ ADDmin");
	
			for(j=0; j<ForcLcas.GetSize(); j++)
			{
				double dFxx, dMuy, dMuz, dStop, dSbot;
				CString sFxx, sMuy, sMuz, sStop, sSbot;
				_PRT_FORC_BASE Force = ForcLcas.GetAt(j);
				dFxx  = Force.dFxx[i];   sFxx  = Check_ValueForm(iInt, iDec, dFxx);
				dMuy  = Force.dMuy[i];   sMuy  = Check_ValueForm(iInt, iDec, dMuy);
				dMuz  = Force.dMuz[i];   sMuz  = Check_ValueForm(iInt, iDec, dMuz);
				dStop = Force.dStl[i];   sStop = Check_ValueForm(iInt, 2   , dStop);
				dSbot = Force.dSbl[i];   sSbot = Check_ValueForm(iInt, 2   , dSbot);

				sStrChk.Format(_T("%s %s %s %s %s %s "),sLcasTitle[j], sFxx, sMuy, sMuz, sStop, sSbot);  arString.Add(sStrChk);
			}
	
			//XLData LcomPrev 사용하중조합종류별 부재력 .
			sStrChk = _T("Before-----------------------------------------------------------------------------------------------------------------");  arString.Add(sStrChk);
			CArray<_PRT_FORC_BASE,_PRT_FORC_BASE> ForcLPrev;
			ForcLPrev.Add(LcomPrev.ForcD);
			ForcLPrev.Add(LcomPrev.ForcDLmax);
			ForcLPrev.Add(LcomPrev.ForcDLmin);
			ForcLPrev.Add(LcomPrev.ForcDLAmax);
			ForcLPrev.Add(LcomPrev.ForcDLAmin);

			CString sPrevTitle[5];
			sPrevTitle[0] = _T("~ PS+DL+C+S");
			sPrevTitle[1] = _T("~ PS+DL+C+S+LLmax");
			sPrevTitle[2] = _T("~ PS+DL+C+S+LLmin");
			sPrevTitle[3] = _T("~ PS+DL+C+S+(LL+ST)max+W+T");
			sPrevTitle[4] = _T("~ PS+DL+C+S+(LL+ST)min+W+T");
	
			for(j=0; j<ForcLPrev.GetSize(); j++)
			{
				double dFxx, dMuy, dMuz, dStop, dSbot;
				CString sFxx, sMuy, sMuz, sStop, sSbot;
				_PRT_FORC_BASE Force = ForcLPrev.GetAt(j);
				dFxx  = Force.dFxx[i];  sFxx  = Check_ValueForm(iInt, iDec, dFxx);
				dMuy  = Force.dMuy[i];  sMuy  = Check_ValueForm(iInt, iDec, dMuy);
				dMuz  = Force.dMuz[i];  sMuz  = Check_ValueForm(iInt, iDec, dMuz);
				dStop = Force.dStl[i];  sStop = Check_ValueForm(iInt, 2   , dStop);
				dSbot = Force.dSbl[i];  sSbot = Check_ValueForm(iInt, 2   , dSbot);

				sStrChk.Format(_T("%s %s %s %s %s %s "),sPrevTitle[j], sFxx, sMuy, sMuz, sStop, sSbot);  arString.Add(sStrChk);
			}
	
			//XLData LcomNext 사용하중조합종류별 부재력 .
			sStrChk = _T("After------------------------------------------------------------------------------------------------------------------");  arString.Add(sStrChk);
			CArray<_PRT_FORC_BASE,_PRT_FORC_BASE> ForcLNext;
			ForcLNext.Add(LcomNext.ForcD);
			ForcLNext.Add(LcomNext.ForcDLmax);
			ForcLNext.Add(LcomNext.ForcDLmin);
			ForcLNext.Add(LcomNext.ForcDLAmax);
			ForcLNext.Add(LcomNext.ForcDLAmin);

			CString sNextTitle[5];
			sNextTitle[0] = _T("~ PS+DL+C+S");
			sNextTitle[1] = _T("~ PS+DL+C+S+LLmax");
			sNextTitle[2] = _T("~ PS+DL+C+S+LLmin");
			sNextTitle[3] = _T("~ PS+DL+C+S+(LL+ST)max+W+T");
			sNextTitle[4] = _T("~ PS+DL+C+S+(LL+ST)min+W+T");
	
			for(j=0; j<ForcLNext.GetSize(); j++)
			{
				double dFxx, dMuy, dMuz, dStop, dSbot;
				CString sFxx, sMuy, sMuz, sStop, sSbot;
				_PRT_FORC_BASE Force = ForcLNext.GetAt(j);
				dFxx =  Force.dFxx[i];  sFxx  = Check_ValueForm(iInt, iDec, dFxx);
				dMuy =  Force.dMuy[i];  sMuy  = Check_ValueForm(iInt, iDec, dMuy);
				dMuz =  Force.dMuz[i];  sMuz  = Check_ValueForm(iInt, iDec, dMuz);
				dStop = Force.dStl[i];  sStop = Check_ValueForm(iInt, 2   , dStop);
				dSbot = Force.dSbl[i];  sSbot = Check_ValueForm(iInt, 2   , dSbot);

				sStrChk.Format(_T("%s %s %s %s %s %s "),sNextTitle[j], sFxx, sMuy, sMuz, sStop, sSbot);      arString.Add(sStrChk);
			}
	
			sStrChk = _T("***********************************************************************************************************************"); arString.Add(sStrChk);
			sStrChk = _T(""); arString.Add(sStrChk);      
		}

		PrtArStr.arString.Copy(arString);
		arStrChk.SetAt(k, PrtArStr);
	}
	
	if(arStrChk.GetSize()==0) return FALSE;

	return TRUE;
}

BOOL CPrtKSCE_USD05::Get_ForceTotalXLData(CArray<CString,CString>& arStrTotal) 
{  
	UINT iInt = 8;
	UINT iDec = 2;

	CString strTotal;
	strTotal.Format(_T("<Table> <응력 검토 집계 결과>                                                     (단위 : %s) "), m_sTxStress);   arStrTotal.Add(strTotal);  
	strTotal = _T("<MV><MH>구``분 ~ ~</MH> <MH>상연 ~  ~</MH>  <MH>하연 ~  ~</MH>");  arStrTotal.Add(strTotal);
	strTotal = _T("</MV><MH>구``분 ~ ~</MH> <MH>발생응력</MH> <MH>허용치</MH> <MH>검토결과</MH>  <MH>발생응력</MH> <MH>허용치</MH> <MH>검토결과</MH>"); arStrTotal.Add(strTotal);
	CArray<_PRT_FORC_GRPH_TOTL,_PRT_FORC_GRPH_TOTL> arGrphTotl;
	if(!Get_ForcGrphTotl(arGrphTotl)) return FALSE;
	if(arGrphTotl.GetSize()==0) return FALSE;

	for(int i=0; i<arGrphTotl.GetSize(); i++)
	{
		CString strTitle;
		_PRT_FORC_GRPH_TOTL GrphTotl = arGrphTotl.GetAt(i);
		strTitle = GrphTotl.strTitle;

		CString sStc, sAltc;	// Top Comp.
	  CString sStt, sAltt;	// Top Tens.
	  CString sSbc, sAlbc;	// Bot Comp.
	  CString sSbt, sAlbt;	// Bot Tens.
		sStt  = Check_ValueForm(iInt, iDec, GrphTotl.dStt);  sAltt = Check_ValueForm(iInt, iDec, GrphTotl.dAltt);
		CString sSttChk = (GrphTotl.dStt>GrphTotl.dAltt ? _T("O.K") : _T("N.G"));
		sStc  = Check_ValueForm(iInt, iDec, GrphTotl.dStc);  sAltc = Check_ValueForm(iInt, iDec, GrphTotl.dAltc);
		CString sStcChk = (GrphTotl.dStc<GrphTotl.dAltc ? _T("O.K") : _T("N.G"));
		sSbt  = Check_ValueForm(iInt, iDec, GrphTotl.dSbt);  sAlbt = Check_ValueForm(iInt, iDec, GrphTotl.dAlbt);
		CString sSbtChk = (GrphTotl.dSbt>GrphTotl.dAlbt ? _T("O.K") : _T("N.G"));
		sSbc  = Check_ValueForm(iInt, iDec, GrphTotl.dSbc);  sAlbc = Check_ValueForm(iInt, iDec, GrphTotl.dAlbc);
		CString sSbcChk = (GrphTotl.dSbc<GrphTotl.dAlbc ? _T("O.K") : _T("N.G"));
		
		strTotal.Format(_T("<MV><MH>%s ~</MH> <MH>인장</MH>  <MH>%s</MH> <MH>%s</MH> <MH>%s</MH>  <MH>%s</MH> <MH>%s</MH> <MH>%s</MH>"),
										 strTitle, sStt, sAltt, sSttChk, sSbt, sAlbt, sSbtChk);   arStrTotal.Add(strTotal);
		strTotal.Format(_T("</MV><MH>%s ~</MH> <MH>압축</MH>  <MH>%s</MH> <MH>%s</MH> <MH>%s</MH>  <MH>%s</MH> <MH>%s</MH> <MH>%s</MH>"),
										 strTitle, sStc, sAltc, sStcChk, sSbc, sAlbc, sSbcChk);   arStrTotal.Add(strTotal);
	}
	strTotal = _T("</Table>");    arStrTotal.Add(strTotal);
	strTotal = _T("");

	if(arStrTotal.GetSize()==0) return FALSE;
	
	return TRUE;
}

//하중조합별 부재력 집계 
BOOL CPrtKSCE_USD05::Get_UltiLoadXLData(CArray<Prt_ArString,Prt_ArString&>& arStrUltiData)
{
	arStrUltiData.RemoveAll();
	UINT iInt = 9;
	UINT iDec = 3;
	CArray<_PRT_FORC_UENV_ELEM,_PRT_FORC_UENV_ELEM> arUenvElemMax;
	CArray<_PRT_FORC_UENV_ELEM,_PRT_FORC_UENV_ELEM> arUenvElemMin;
		
	BOOL bDiagram=FALSE;
	if(!Get_ForcDiagram(arUenvElemMax,arUenvElemMin)) return FALSE;
	if(arUenvElemMax.GetSize()==0) return FALSE;
	
	Prt_ArString PrtArStr; PrtArStr.Initialize();
	CArray<CString,CString> arUltiData;
	int i=0,j=0,k=0,l=0;
	int TotalElem = arUenvElemMax.GetSize();
	CString strUltim = _T("");
	CString strTitle[3] = {_T("Moment(M) Max/Min"), _T("Shear(V) Max/Min"),
		_T("Torsion(T) Max/Min")};
	for(l=0; l<3; l++)
	{ 
		arUltiData.RemoveAll();
		strUltim = strTitle[l];      arUltiData.Add(strUltim);
		//strUltim.Format(_T("%s %s %s %s %s %s %s %s %s"),_T("Element"), _T("Node"), _T("Max/Min"), _T("Fxx(tonf)"), _T("Fyy(tonf)"), _T("Fzz(tonf)"), _T("Mux(tonf·m)"), _T("Muy(tonf·m)"), _T("Muz(tonf·m)")); arUltiData.Add(strUltim);
		strUltim.Format(_T("Element Node Max/Min Fxx(%s) Fyy(%s) Fzz(%s) Mux(%s) Muy(%s) Muz(%s)"), m_strForce, m_strForce, m_strForce, m_strMoment, m_strMoment, m_strMoment); arUltiData.Add(strUltim);
		for(i=0; i<TotalElem; i++)
		{
			T_ELEM_K ElemK = arUenvElemMax.GetAt(i).ElemK;
			_DGN_FORC_CRC ForceMax, ForceMin;
			switch(l)
			{
			case 0:
				ForceMax = arUenvElemMax.GetAt(i).ForcMy;
				ForceMin = arUenvElemMin.GetAt(i).ForcMy;
				break;
			case 1:
				ForceMax = arUenvElemMax.GetAt(i).ForcVz;
				ForceMin = arUenvElemMin.GetAt(i).ForcVz;
				break;
			case 2:
				ForceMax = arUenvElemMax.GetAt(i).ForcMx;
				ForceMin = arUenvElemMin.GetAt(i).ForcMx;
				break;
			}

			for(j=0; j<2; j++) //i,j
			{
				CString sIJ = (j==0 ? _T("I") : _T("J"));
				for(k=0; k<2; k++) // Max.Min
				{
					sIJ = (k==0 ? sIJ : _T("~"));
					CString sMaxMin = (k==0 ? _T("Max") : _T("Min")); 
					_DGN_FORC_CRC envForce = (k==0 ? ForceMax : ForceMin);
					CString sFxx = Check_ValueForm(iInt, iDec, envForce.dFxx[j]); 
					CString sFyy = Check_ValueForm(iInt, iDec, envForce.dFyy[j]);
					CString sFzz = Check_ValueForm(iInt, iDec, envForce.dFzz[j]);
					CString sMux = Check_ValueForm(iInt, iDec, envForce.dMux[j]);
					CString sMuy = Check_ValueForm(iInt, iDec, envForce.dMuy[j]);
					CString sMuz = Check_ValueForm(iInt, iDec, envForce.dMuz[j]);

					strUltim.Format(_T("%d %s %s %s %s %s %s %s %s"),ElemK, sIJ, sMaxMin, sFxx, sFyy, sFzz, sMux, sMuy, sMuz); arUltiData.Add(strUltim);
				}
		 
			}   
		}   
		strUltim = _T("");  arUltiData.Add(strUltim);
		strUltim = _T("");  arUltiData.Add(strUltim);

		PrtArStr.arString.Copy(arUltiData);   
		arStrUltiData.Add(PrtArStr);
	}
	
	if(arStrUltiData.GetSize()==0) return FALSE;

	return TRUE;  
}

BOOL CPrtKSCE_USD05::Get_StageKeyList(CArray<T_STAG_K,T_STAG_K>& arStagK)
{
	for(int i=0; i<m_arStagK.GetSize(); i++)
	{
		T_STAG_K StagK = m_arStagK.GetAt(i);
		arStagK.Add(StagK);
	}   
	return TRUE;
}

BOOL CPrtKSCE_USD05::Get_LcomKeyList(CArray<T_LCOM_K,T_LCOM_K>& arLcomK)
{
	for(int i=0; i<m_arLcomK.GetSize(); i++)
	{
		T_LCOM_K LcomK = m_arLcomK.GetAt(i);
		arLcomK.Add(LcomK);
	}   
	return TRUE;
}

// 휨강도 검토.
// 
// BOOL CPrtKSCE_USD05::Get_FlexStrengthCalc_KSCE10(CMSExcel* pXL)
// {
//   CDBDoc* pDoc = CDBDoc::GetDocPoint();
//   ASSERT(pDoc);
//   //arStrDetail.RemoveAll();
//   
//   T_PSCD_D PscdD; PscdD.Initialize();
//   pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);
// 
//   if(m_arElemK.GetSize()==0) return FALSE;
//   int TotalElem = m_arElemK.GetSize();  
//   for(int i=0; i<TotalElem; i++)
//   {
//     T_ELEM_K ElemK = m_arElemK.GetAt(i);  //수정.
//     // Initialize.
//     //m_arText.RemoveAll();   
//     //m_iFormula = 0;
//     //Prt_ArString arString; arString.Initialize();
//     //Prt_ArString_Detail arDetail; arDetail.Initialize();    
//     Get_DetailBdcyPos(ElemK, pXL);
//     //arDetail.iFormula = m_iFormula;
//     //arDetail.ArString.arString.Copy(m_arText);
//     //arStrDetail.Add(arDetail);    
//     
//     // Initialize.
//     //m_arText.RemoveAll();   
//     //m_iFormula = 0;
//     //arString.Initialize();
//     //arDetail.Initialize();    
//     Get_DetailBdcyNeg(ElemK);        
//     //arDetail.iFormula = m_iFormula;
//     //arDetail.ArString.arString.Copy(m_arText);
//     //arStrDetail.Add(arDetail);    
//   } 
//   
//   //if(arStrDetail.GetSize()==0) return FALSE;
//   return TRUE;
// 
// }

BOOL CPrtKSCE_USD05::Get_FlexStrengthCalc(CArray<CString,CString>& arTheory, CArray<Prt_ArString_Detail,Prt_ArString_Detail>& arStrDetail, CMSExcel* pXL)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	arStrDetail.RemoveAll();

	T_PSCD_D PscdD; PscdD.Initialize();
	pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

	// PS강재의 응력 계산(0=설계기준, 1=상세한 해석)
	if(m_iDgnCode==KSCE_USD05 || m_iDgnCode==KSCE_RAIL_USD04) 
	{
		if(PscdD.ifpsMethod==0)
		{
			if(m_iDgnCode==KSCE_USD05)           Print_BdcyTheory_Code_KSCE05(arTheory);  
			else if(m_iDgnCode==KSCE_RAIL_USD04) Print_BdcyTheory_Code_RAIL04(arTheory);  
			else ASSERT(0);
		}
		else if(PscdD.ifpsMethod==1)
		{
			if(m_iDgnCode==KSCE_USD05)            Print_BdcyTheory_Iter(arTheory, KSCE_USD05);
			else if(m_iDgnCode==KSCE_RAIL_USD04)  Print_BdcyTheory_Iter(arTheory, KSCE_RAIL_USD04);
			else ASSERT(0);
		}
	}
	
	m_arText.RemoveAll();   
	if(m_arElemK.GetSize()==0) return FALSE;
	int TotalElem = m_arElemK.GetSize();  
	for(int i=0; i<TotalElem; i++)
	{
		auto ElemK = m_arElemK.GetAt(i);  //수정.
		// Initialize.
		m_arText.RemoveAll();   
		m_iFormula = 0;
		Prt_ArString arString; arString.Initialize();
		Prt_ArString_Detail arDetail; arDetail.Initialize();    
			Get_DetailBdcyPos(ElemK, pXL);
		arDetail.iFormula = m_iFormula;
		arDetail.ArString.arString.Copy(m_arText);
		arStrDetail.Add(arDetail);    
		
		// Initialize.
		m_arText.RemoveAll();   
		m_iFormula = 0;
		arString.Initialize();
		arDetail.Initialize();    
			Get_DetailBdcyNeg(ElemK);        
		arDetail.iFormula = m_iFormula;
		arDetail.ArString.arString.Copy(m_arText);
		arStrDetail.Add(arDetail);    
	} 
		 
	if(arStrDetail.GetSize()==0) return FALSE;
	return TRUE;
}

BOOL CPrtKSCE_USD05::Get_FlexStrengthTable_KSCE10(CArray<CString,CString>& arPMomTable, CArray<CString,CString>& arNMomTable, CMSExcel* pXL)
{
	//요소 전체..
	ArrElemPairKey arElemK;
	CArray<PRT_BDCY_KSCE,PRT_BDCY_KSCE> arBdcrKsce;

	for(int i=0; i<2; i++) // 0=P-Moment 1=N-Moment
	{
		CArray<PRT_BDCY_KSCE,PRT_BDCY_KSCE> arBdcrKsce; 
		arBdcrKsce.RemoveAll();
		if(i==0) { if(!Get_ResultBdcyPos(arElemK, arBdcrKsce)) continue; }	// My(+).
		else     { if(!Get_ResultBdcyNeg(arElemK, arBdcrKsce)) continue; }	// My(-).       

		CArray<T_ELEM_K, T_ELEM_K> aPrtElemK;
		CDBLib::GetElemKeyList4ElemPairKey(arElemK, aPrtElemK);
		m_pDataCtrl->Get_DgnKsce05Pointer()->Print_BdcyDesign_Table(aPrtElemK, arBdcrKsce, i, pXL);
	}

	return TRUE;
}


BOOL CPrtKSCE_USD05::Get_FlexStrengthTable(CArray<CString,CString>& arPMomTable, CArray<CString,CString>& arNMomTable)
{
	//요소 전체..
	ArrElemPairKey arElemK;
	CArray<PRT_BDCY_KSCE,PRT_BDCY_KSCE> arBdcrKsce;
	
	int i,j,k;
	CString strStress = m_sTxStress;
	CString strArea   = m_sTxArea;
	CString strAreaM  = m_sTxAreaM;
	CString strStif   = m_sTxStif;
	CString strStifM  = m_sTxStifM;
	CString strTable;
	for(i=0; i<2; i++) // 0=P-Moment 1=N-Moment
	{
		CArray<PRT_BDCY_KSCE,PRT_BDCY_KSCE> arBdcrKsce; 
		arBdcrKsce.RemoveAll();
		if(i==0) { if(!Get_ResultBdcyPos(arElemK, arBdcrKsce)) continue; }	// My(+).
		else     { if(!Get_ResultBdcyNeg(arElemK, arBdcrKsce)) continue; }	// My(-).       
		
		strTable = (i==0 ? _T("<Table>정모멘트에 대한 검토") : _T("<Table>부모멘트에 대한 검토"));    
		if(i==0) { arPMomTable.Add(strTable); }
		else     { arNMomTable.Add(strTable); } 
		strTable = _T("<MV>Elem Position Mu fpe Aps As I h b d dp fps a ΦMn 안전  최대  철근량  1.2Mcr  (4/3)Mu  최소"); 
		if(i==0) { arPMomTable.Add(strTable); }
		else     { arNMomTable.Add(strTable); } 
													//   1   2    3    4    5    6    7    8    9   10    11   12   13   14  15  16   17   18
		strTable.Format(_T("</MV>Elem %s (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s)  %s  %s  %s  (%s)  (%s)  %s"),
						_T("I/J"), m_strMoment,strStress,strArea,strArea,strStif,m_strLength,m_strLength,m_strLength,
						m_strLength,strStress,m_strLength,m_strMoment,_T("여부"),_T("강재량"),_T("검토"),m_strMoment,m_strMoment,_T("철근비")); 
		if(i==0) { arPMomTable.Add(strTable); }
		else     { arNMomTable.Add(strTable); } 
				
		if(arElemK.GetSize()==0) continue;
		for(j=0; j<arElemK.GetSize(); j++)
		{
			auto ElemK = arElemK.GetAt(j);
			PRT_BDCY_KSCE BdcyKsceD = arBdcrKsce.GetAt(j);
			for(k=0; k<2; k++)	// 0=I, 1=J
		  {
				if(!BdcyKsceD.BdcyBase[k].bCHK) continue;
				CString sIJ     = (k==0 ? _T("I") : _T("J"));
			  CString sMuy   = Check_ValueForm(9,2,BdcyKsceD.BdcyBase[k].dMuy);   // 2
			  CString sfpe   = Check_ValueForm(7,2,BdcyKsceD.BdcyBase[k].dfpe);   // 3
			  CString sAps   = Check_ValueForm(7,1,BdcyKsceD.BdcyBase[k].dAps);   // 4
			  CString sAst   = Check_ValueForm(7,1,BdcyKsceD.BdcyBase[k].dAst);   // 5
			  CString sIy    = Check_ValueForm(9,2,BdcyKsceD.BdcyBase[k].dIy);    // 6
			  CString sh     = Check_ValueForm(7,1,BdcyKsceD.BdcyBase[k].dh); //dh   // 7
				CString sb     = Check_ValueForm(7,1,BdcyKsceD.BdcyBase[k].db);
				CString sdst   = Check_ValueForm(7,1,BdcyKsceD.BdcyBase[k].ddst);   // 9
			  CString sdps   = Check_ValueForm(7,1,BdcyKsceD.BdcyBase[k].ddps);   // 10
			  CString sfps   = Check_ValueForm(7,2,BdcyKsceD.BdcyBase[k].dfps);   // 11
			  CString sa     = Check_ValueForm(7,1,BdcyKsceD.BdcyBase[k].da);     // 12
			  CString spMny  = Check_ValueForm(9,2,BdcyKsceD.BdcyBase[k].dpMny);  // 13
				CString sMnyChk = (BdcyKsceD.BdcyBase[k].dMuy<BdcyKsceD.BdcyBase[k].dpMny ? _T("O.K") : _T("N.G"));  // 14
				CString sqp    = Check_ValueForm(7,5,BdcyKsceD.BdcyBase[k].dqp);    // 15
			  CString sMaxPsChk = (BdcyKsceD.BdcyBase[k].dqp<BdcyKsceD.BdcyBase[k].d036Beta1 ? _T("O.K") : _T("N.G"));  // 16
				CString s12Mcr = Check_ValueForm(9,2,BdcyKsceD.BdcyBase[k].d12Mcr);   // 17
				CString s43Muy = Check_ValueForm(9,2,BdcyKsceD.BdcyBase[k].d43Muy);
				CString sMinPsChk = (BdcyKsceD.BdcyBase[k].d12Mcr<BdcyKsceD.BdcyBase[k].dpMny ? _T("O.K") : _T("N.G"));   // 18
				if(BdcyKsceD.BdcyBase[k].d12Mcr>BdcyKsceD.BdcyBase[k].dpMny)
				{
					if(BdcyKsceD.BdcyBase[k].d43Muy<BdcyKsceD.BdcyBase[k].dpMny) sMinPsChk = _T("O.K");
				}
				
				/*
				CString sPe    = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[j].dPe);		
			  CString sAsc   = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[j].dAsc);
			  CString sdsc   = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[j].ddsc);
			  CString syt    = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[j].dyt);
			  CString seps   = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[j].deps);
			  CString sAc    = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[j].dAc);
			  CString sfy    = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[j].dfy);		
			  CString sfck   = Check_ValueForm(6,2,BdcyKsceD.BdcyBase[j].dfck);
				*/
											// 1  2  3  4  5  6   7  8  9 10 11 12 13 14 15 16 17 18 19    20    21    22    23    24    25
				strTable.Format(_T("%d %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s"), 
				ElemK, sIJ, sMuy, sfpe, sAps, sAst, sIy, sh, sb, sdst, sdps, sfps, sa, spMny, sMnyChk, sqp, sMaxPsChk, s12Mcr, s43Muy, sMinPsChk);
				if(i==0) { arPMomTable.Add(strTable); }
				else     { arNMomTable.Add(strTable); }
		  }
		}
	}
	strTable = _T("</Table>");
	arPMomTable.Add(strTable);
	arNMomTable.Add(strTable);

	return TRUE;    
}

BOOL CPrtKSCE_USD05::Get_ShearStrengthCalc(CArray<CString,CString>& arTheory, CArray<Prt_ArString_Detail,Prt_ArString_Detail>& arStrDetail)
{
	arStrDetail.RemoveAll();
	Print_CumsTheory(arTheory);  // Print Theory 

	m_arText.RemoveAll();
	m_iFormula = 0;
	int TotalElem = m_arElemK.GetSize();
	for(int i=0; i<TotalElem; i++)
	{
		auto ElemK = m_arElemK.GetAt(i);     
		// Initialize.
		m_arText.RemoveAll();   
		m_iFormula = 0;
		Prt_ArString arString; arString.Initialize();
		Prt_ArString_Detail arDetail; arDetail.Initialize();    
		if(!Get_DetailCums(ElemK)) continue;    
		arDetail.iFormula = m_iFormula;
		arDetail.ArString.arString.Copy(m_arText);
		arStrDetail.Add(arDetail);    
	} 

	if(arStrDetail.GetSize()==0) return FALSE;
	 
	return TRUE;
}



BOOL CPrtKSCE_USD05::Get_ShearStrengthTable_KSCE10(CArray<CString,CString>& arShearTable, CMSExcel* pXL)
{

	//요소 전체..
	ArrElemPairKey arElemK;
	CArray<PRT_CUMS_KSCE,PRT_CUMS_KSCE> arCumsKsce;
	
	if(!Get_ResultCums(arElemK, arCumsKsce)) return FALSE;	// My.
	if(arElemK.GetSize()==0) return FALSE;

	CArray<T_ELEM_K, T_ELEM_K> aPrtElemK;
	CDBLib::GetElemKeyList4ElemPairKey(arElemK, aPrtElemK);

	m_pDataCtrl->Get_DgnKsce05Pointer()->Print_CumsDesign_Table(aPrtElemK, arCumsKsce, pXL);
	
	return TRUE;
}

BOOL CPrtKSCE_USD05::Get_ShearStrengthTable(CArray<CString,CString>& arShearTable)
{

	ArrElemPairKey arElemK;
	CArray<PRT_CUMS_KSCE,PRT_CUMS_KSCE> arCumsKsce;
	if(!Get_ResultCums(arElemK, arCumsKsce)) return FALSE;	// My.
	if(arElemK.GetSize()==0) return FALSE;

	int i,j;
	CString strStress = m_sTxStress;
	CString strArea   = m_sTxArea;
	CString strAreaM  = m_sTxAreaM;
	CString strStif   = m_sTxStif;
	CString strStifM  = m_sTxStifM;

	CString strTable=_T("");
	strTable = _T("<Table>전단에 대한 검토");    arShearTable.Add(strTable);
	strTable = _T("<MV>Elem Position bw  d  h  I yt  fpe  Mcr  Vci  fpc  Vp  Vcw  ΦVc  Vu ΦVs  s  Av  As  Av,req  As,use ΦVn  안전"); arShearTable.Add(strTable);
										//   1    2  3    4    5     6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26  27
	strTable.Format(_T("</MV>Elem %s (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s) (%s)"),
	_T("I/J"), m_strLength,m_strLength,m_strLength,strStif,m_strLength,strStress,m_strMoment,m_strForce,
	strStress,m_strForce,m_strForce,m_strForce,m_strForce,m_strForce,m_strLength,strArea,strArea,strArea,
	strArea,m_strForce,_T("여부")); arShearTable.Add(strTable);

	for(i=0; i<arElemK.GetSize(); i++)
	{    
		auto ElemK = arElemK.GetAt(i);
		// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
		PRT_CUMS_KSCE PrtCumsD= arCumsKsce.GetAt(i);
		for(j=0; j<2; j++)	// 0=I, 1=J
		{ 
			if(!PrtCumsD.CumsBase[j].bCHK) continue;
			CString sIJ     = (j==0 ? _T("I") : _T("J"));
			CString sbw     = Check_ValueForm(7,1,PrtCumsD.CumsBase[j].dbw);   
			CString sdst    = Check_ValueForm(7,1,PrtCumsD.CumsBase[j].dd);  
			CString sh      = Check_ValueForm(7,1,PrtCumsD.CumsBase[j].dh);  
			CString sIy     = Check_ValueForm(9,2,PrtCumsD.CumsBase[j].dIy);   
			CString syt     = Check_ValueForm(7,1,PrtCumsD.CumsBase[j].dyt);
			CString sfpe    = Check_ValueForm(7,2,PrtCumsD.CumsBase[j].dfpe);
			CString sMcr    = Check_ValueForm(9,2,PrtCumsD.CumsBase[j].dMcr);
			CString sVi     = Check_ValueForm(8,2,PrtCumsD.CumsBase[j].dVi);
			CString sMmax   = Check_ValueForm(8,2,PrtCumsD.CumsBase[j].dMmax);
			CString sVci    = Check_ValueForm(8,2,PrtCumsD.CumsBase[j].dVci);
			CString sfpc    = Check_ValueForm(7,2,PrtCumsD.CumsBase[j].dfpc);  
	 		CString sVp     = Check_ValueForm(8,2,PrtCumsD.CumsBase[j].dVp);
			CString sVcw    = Check_ValueForm(8,2,PrtCumsD.CumsBase[j].dVcw);
			CString spVc    = Check_ValueForm(8,2,0.8*PrtCumsD.CumsBase[j].dVc);  
			CString sVu     = Check_ValueForm(8,2,PrtCumsD.CumsBase[j].dVu);   
			CString spVs    = Check_ValueForm(8,2,0.8*PrtCumsD.CumsBase[j].dVs);   
			CString ssv     = Check_ValueForm(7,1,PrtCumsD.CumsBase[j].dsv);  
			CString sAv     = Check_ValueForm(7,1,PrtCumsD.CumsBase[j].dAv);
			CString sAst    = _T("0.0"); // Ast
			CString sAv_req = Check_ValueForm(7,1,PrtCumsD.CumsBase[j].dAvreq);
			CString sAs_use = _T("0.0"); //As_use
			CString spVn    = Check_ValueForm(7,1,PrtCumsD.CumsBase[j].dpVn);   
			CString sChk    = (PrtCumsD.CumsBase[j].dVu<PrtCumsD.CumsBase[j].dpVn ? _T("O.K") : _T("N.G"));   // 18

										 // 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25
			strTable.Format(_T("%d %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s"), 
			ElemK, sIJ, sbw, sdst, sh, sIy, syt, sfpe, sMcr, sVci, sfpc, sVp, sVcw, spVc, sVu, spVs, ssv, sAv, sAst, 
			sAv_req, sAs_use, spVn, sChk); //25개
			arShearTable.Add(strTable);      	
		}
	}
	strTable = _T("</Table>");    arShearTable.Add(strTable);  

	if(arShearTable.GetSize()==0) return FALSE;

	return TRUE;
}

BOOL CPrtKSCE_USD05::Get_TorsStrengthCalc(CArray<CString,CString>& arTheory, CArray<Prt_ArString_Detail,Prt_ArString_Detail>& arStrDetail)
{
	arStrDetail.RemoveAll();
	Print_CrmtTheory(arTheory);  // Print Theory 
	if(m_arElemK.GetSize()==0) return FALSE;

	int TotalElem = m_arElemK.GetSize();  
	for(int i=0; i<TotalElem; i++)
	{
		auto ElemK = m_arElemK.GetAt(i);    
		m_arText.RemoveAll();   
		m_iFormula = 0;
		Prt_ArString arString; arString.Initialize();
		Prt_ArString_Detail arDetail; arDetail.Initialize();    
		if(!Get_DetailCrmt(ElemK)) continue;    
		arDetail.iFormula = m_iFormula;
		arDetail.ArString.arString.Copy(m_arText);
		arStrDetail.Add(arDetail);    
	} 

	if(arStrDetail.GetSize()==0) return FALSE;
	
	return TRUE;  
}

BOOL CPrtKSCE_USD05::Get_ReinforceCalc(CArray<Prt_ArString_Detail,Prt_ArString_Detail>& arStrDetail)
{
	arStrDetail.RemoveAll();
	if(m_arElemK.GetSize()==0) return FALSE;
	m_arText.RemoveAll();   
	m_iFormula = 0;
	int TotalElem = m_arElemK.GetSize();
	for(int i=0; i<TotalElem; i++)
	{
		auto ElemK = m_arElemK.GetAt(i);
		// Initialize.
		m_arText.RemoveAll();   
		m_iFormula = 0;
		Prt_ArString arString; arString.Initialize();
		Prt_ArString_Detail arDetail; arDetail.Initialize();    
		if(!Get_DetailTbarPos(ElemK)) continue;
		arDetail.iFormula = m_iFormula;
		arDetail.ArString.arString.Copy(m_arText);
		arStrDetail.Add(arDetail);    

		// Initialize.
		m_arText.RemoveAll();   
		m_iFormula = 0;
		arString.Initialize();
		arDetail.Initialize();    
		if(!Get_DetailTbarNeg(ElemK)) continue;    
		arDetail.iFormula = m_iFormula;
		arDetail.ArString.arString.Copy(m_arText);
		arStrDetail.Add(arDetail);    
	} 

	if(arStrDetail.GetSize()==0) return FALSE;
	 
	return TRUE;    
}
