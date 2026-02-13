// PrtKSCE_USD05.cpp: implementation of the CPrtKSCE_USD05 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"

#include "PrtKSCE_USD10.h"
#include "DgnKSCE_USD05.h"
#include "..\wg_base\wg_base_MsgDll.h"
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

#define RADTODEG (180.0/3.14159265358979323846)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrtKSCE_USD10::CPrtKSCE_USD10() :
CPrtKSCE_USD05()
{
	m_pXL = NULL;
	m_strBaseBook = _T("");
	m_strChpt = _T("");
	m_nTotalRow=0;
	m_nChapNum =0;
	m_bCompoSect = FALSE;

}

CPrtKSCE_USD10::~CPrtKSCE_USD10()
{
}



void CPrtKSCE_USD10::Set_CMSExcel_KSCE10(CMSExcel* pXL, CString strBaseBook, int nDgnCode)
{
	m_pXL = pXL;
	m_strBaseBook = strBaseBook;
	m_nTotalRow=0;
	m_nChapNum =1;
	m_iDgnCode = nDgnCode;
	SetUnitConvertFactor();
}


void CPrtKSCE_USD10::Set_CMSExcel_Flexure_theory(CMSExcel* pXL, int ifpsMethod)
{
	int RowCnt=0;
	int ColCnt=0;
	int stRow=m_nTotalRow;
	CString strBaseSheet;
	CString strBlock;

	if(m_iDgnCode==KSCE_USD10)           
	{
		strBaseSheet =  _T("휨(이론-도로교10)");
		if(ifpsMethod==0)      strBlock =  _T("Print_Flexure_theory_KSCE10");
		else if(ifpsMethod==1) strBlock =  _T("Print_Flexure_theory_KSCE10_Iter");
	}
	else if(m_iDgnCode==KSCE_RAIL_USD11) 
	{
		strBaseSheet =  _T("휨(이론-철도교11)");
		if(ifpsMethod==0)      strBlock =  _T("Print_Flexure_theory_KSCE_Rail11");
		else if(ifpsMethod==1) strBlock =  _T("Print_Flexure_theory_KSCE_Rail11_Iter");
	}
	else ASSERT(0);
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	stRow += RowCnt+1;

}


void CPrtKSCE_USD10::Set_CMSExcel_Shear_theory(CMSExcel* pXL)
{
	int RowCnt=0;
	int ColCnt=0;
	int stRow=m_nTotalRow;
	CString strBaseSheet;
	CString strBlock;
	
	if(m_iDgnCode==KSCE_USD10)           
	{
		strBaseSheet =  _T("전단(이론-도로교10)");
		strBlock     =  _T("Print_Shear_theory_KSCE10");
	}
	else if(m_iDgnCode==KSCE_RAIL_USD11)           
	{
		strBaseSheet =  _T("전단(이론-철도교11)");
		strBlock     =  _T("Print_Shear_theory_KSCE_Rail11");
	}
	else ASSERT(0);

	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	stRow += RowCnt+1;

}


void CPrtKSCE_USD10::Set_CMSExcel_Torsion_theory(CMSExcel* pXL)
{
	int RowCnt=0;
	int ColCnt=0;
	int stRow=m_nTotalRow;
	CString strBaseSheet;
	CString strBlock;
	
	if(m_iDgnCode==KSCE_USD10)           
	{
		strBaseSheet =  _T("비틀림(이론-도로교10)");
		strBlock     =  _T("Print_Tortion_theory_KSCE10");
	}
	else if(m_iDgnCode==KSCE_RAIL_USD11)           
	{
		strBaseSheet =  _T("비틀림(이론-철도교11)");
		strBlock     =  _T("Print_Tortion_theory_KSCE_Rail11");
	}
	else ASSERT(0);
	
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	stRow += RowCnt+1;
	
}

void CPrtKSCE_USD10::Print_Rating_Title(CString strTitle, CMSExcel* pXL)
{
	int RowCnt=0;
	int ColCnt=0;
	int stRow=m_nTotalRow;
	CString strBaseSheet =  _T("휨");

	
	CString strBlock  = _T("Title_for_Rating");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  0, strTitle);
	stRow += RowCnt;
	
	
	pXL = m_pXL;
	m_nTotalRow = stRow;
}


void CPrtKSCE_USD10::Print_ElementIJ_KSCE10(T_ELEM_K ElemK, BOOL bPosiI, BOOL bPositive, CMSExcel* pXL)
{
	int RowCnt=0;
	int ColCnt=0;
	int stRow=m_nTotalRow;
	CString strBaseSheet =  _T("휨");

	CString strBlock  = _T("Design_Code_Elem_Node");
	CString strPosi   = bPosiI? _T("I") : _T("J");
	CString strMoment = bPositive ? _T("정모멘트") : _T("부모멘트");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+1,  1, ElemK);
	m_pXL->SetValue(stRow+1,  5, strPosi);
	m_pXL->SetValue(stRow+1,  9, strMoment);
	stRow += RowCnt;


	pXL = m_pXL;
	m_nTotalRow = stRow;
}

void CPrtKSCE_USD10::Print_ElementIJ_Shear_KSCE10(T_ELEM_K ElemK, BOOL bPosiI, CMSExcel* pXL)
{
	int RowCnt=0;
	int ColCnt=0;
	int stRow=m_nTotalRow;
	CString strBaseSheet =  _T("전단");
	CString strBlock  = _T("Design_Code_Shear_Elem_Node");
	CString strPosi   = bPosiI? _T("I") : _T("J");

	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+1,  1, ElemK);
	m_pXL->SetValue(stRow+1,  5, strPosi);
	stRow += RowCnt+1;
	
	pXL = m_pXL;
	m_nTotalRow = stRow;
}

void CPrtKSCE_USD10::Print_ElementIJ_Tortion_KSCE10(T_ELEM_K ElemK, BOOL bPosiI, CMSExcel* pXL)
{
	int RowCnt=0;
	int ColCnt=0;
	int stRow=m_nTotalRow;
	CString strBaseSheet =  _T("비틀림");
	CString strBlock  = _T("Design_Code_Torsion_Elem_Node");
	CString strPosi   = bPosiI? _T("I") : _T("J");
	
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+1,  1, ElemK);
	m_pXL->SetValue(stRow+1,  5, strPosi);
	stRow += RowCnt+1;
	
	pXL = m_pXL;
	m_nTotalRow = stRow;
	
}

BOOL CPrtKSCE_USD10::Print_BdcyDesign_Table(CArray<T_ELEM_K,T_ELEM_K>& arElemK, CArray<PRT_BDCY_KSCE,PRT_BDCY_KSCE>& arBdcrKsce, int i, CMSExcel* pXL )
{
	int RowCnt=0;
	int ColCnt=0;
	int stRow=m_nTotalRow;
	CString strBaseSheet =  _T("휨(Table)");
	CString strBlock;

	// 0=P-Moment 1=N-Moment
	CString strTitle = (i ==0)? _T("■ 정모멘트에 대한 검토") : _T("■ 부모멘트에 대한 검토");
	strBlock = _T("Title_Moment_Table");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  0,  strTitle);
	stRow += RowCnt;

	if     (m_iDgnCode==KSCE_USD10)       strBlock = _T("Moment_Table_Header");
	else if(m_iDgnCode==KSCE_RAIL_USD11)  strBlock = _T("Moment_Table_Header_Rail");
	else ASSERT(0);
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	stRow += RowCnt;

	if(arElemK.GetSize()==0) return TRUE;
	for(int nElem=0; nElem<arElemK.GetSize(); ++nElem)
	{
		T_ELEM_K ElemK = arElemK.GetAt(nElem);
		PRT_BDCY_KSCE BdcyKsceD = arBdcrKsce.GetAt(nElem);

		for(int k=0; k<2; k++)	// 0=I, 1=J
		{
			if(!BdcyKsceD.BdcyBase[k].bCHK) continue;
			CString sIJ     = (k==0 ? _T("I") : _T("J"));
			CString sMuy   = Check_ValueForm(9,2,BdcyKsceD.BdcyBase[k].dMuy);   // 2
			CString sfpe   = Check_ValueForm(7,2,BdcyKsceD.BdcyBase[k].dfpe);   // 3
			CString sAps   = Check_ValueForm(7,1,BdcyKsceD.BdcyBase[k].dAps);   // 4
			CString sAst   = Check_ValueForm(7,1,BdcyKsceD.BdcyBase[k].dAst);   // 5
			CString sIy    = Check_ValueForm(9,2,BdcyKsceD.BdcyBase[k].dIy);    // 6
			CString sh     = Check_ValueForm(7,1,BdcyKsceD.BdcyBase[k].dh);     // 7
			CString sb     = Check_ValueForm(7,1,BdcyKsceD.BdcyBase[k].db);
			CString sdst   = Check_ValueForm(7,1,BdcyKsceD.BdcyBase[k].ddst);   // 9
			CString sdps   = Check_ValueForm(7,1,BdcyKsceD.BdcyBase[k].ddps);   // 10
			CString sfps   = Check_ValueForm(7,2,BdcyKsceD.BdcyBase[k].dfps);   // 11
			CString sa     = Check_ValueForm(7,1,BdcyKsceD.BdcyBase[k].da);     // 12
			CString spMny  = Check_ValueForm(9,2,BdcyKsceD.BdcyBase[k].dpMny);  // 13
			CString sMnyChk = (BdcyKsceD.BdcyBase[k].dMuy<BdcyKsceD.BdcyBase[k].dpMny ? _T("OK") : _T("NG"));  // 14
			CString sqp    = Check_ValueForm(7,5,BdcyKsceD.BdcyBase[k].dqp);    // 15
 			CString sMaxPsChk = (BdcyKsceD.BdcyBase[k].dqp<BdcyKsceD.BdcyBase[k].d036Beta1 ? _T("OK") : _T("NG"));  // 16
			CString s12Mcr = Check_ValueForm(9,2,BdcyKsceD.BdcyBase[k].d12Mcr);   // 17
			CString s43Muy = Check_ValueForm(9,2,BdcyKsceD.BdcyBase[k].d43Muy);
			CString sMinPsChk = (BdcyKsceD.BdcyBase[k].d12Mcr<BdcyKsceD.BdcyBase[k].dpMny ? _T("OK") : _T("NG"));   // 18
			if(BdcyKsceD.BdcyBase[k].d12Mcr>BdcyKsceD.BdcyBase[k].dpMny)
			{
				if(BdcyKsceD.BdcyBase[k].d43Muy<BdcyKsceD.BdcyBase[k].dpMny) sMinPsChk = _T("OK");
			}
			
			if (m_iDgnCode==KSCE_USD10) 
			{
				strBlock = _T("Moment_Table_Body");
				m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
				m_pXL->SetValue(stRow+0,  0,  ElemK);
				m_pXL->SetValue(stRow+0,  1,  sIJ);
				m_pXL->SetValue(stRow+0,  2,  sMuy );
				m_pXL->SetValue(stRow+0,  3,  sfpe);
				m_pXL->SetValue(stRow+0,  4,  sAps);
				m_pXL->SetValue(stRow+0,  5,  sAst);
				m_pXL->SetValue(stRow+0,  6,  sIy);
				m_pXL->SetValue(stRow+0,  7,  sh);
				m_pXL->SetValue(stRow+0,  8,  sb);
				m_pXL->SetValue(stRow+0,  9,  sdst);
				m_pXL->SetValue(stRow+0, 10,  sdps);
				m_pXL->SetValue(stRow+0, 11,  sfps);
				m_pXL->SetValue(stRow+0, 12,  sa);
				m_pXL->SetValue(stRow+0, 13,  spMny);
				m_pXL->SetValue(stRow+0, 14,  sMnyChk);
				m_pXL->SetValue(stRow+0, 15,  s12Mcr);
				m_pXL->SetValue(stRow+0, 16,  s43Muy);
				m_pXL->SetValue(stRow+0, 17,  sMinPsChk);
				stRow += RowCnt;
			}
			else if(m_iDgnCode==KSCE_RAIL_USD11)
			{
				strBlock = _T("Moment_Table_Body_Rail");
				m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
				m_pXL->SetValue(stRow+0,  0,  ElemK);
				m_pXL->SetValue(stRow+0,  1,  sIJ);
				m_pXL->SetValue(stRow+0,  2,  sMuy );
				m_pXL->SetValue(stRow+0,  3,  sfpe);
				m_pXL->SetValue(stRow+0,  4,  sAps);
				m_pXL->SetValue(stRow+0,  5,  sAst);
				m_pXL->SetValue(stRow+0,  6,  sIy);
				m_pXL->SetValue(stRow+0,  7,  sh);
				m_pXL->SetValue(stRow+0,  8,  sb);
				m_pXL->SetValue(stRow+0,  9,  sdst);
				m_pXL->SetValue(stRow+0, 10,  sdps);
				m_pXL->SetValue(stRow+0, 11,  sfps);
				m_pXL->SetValue(stRow+0, 12,  sa);
				m_pXL->SetValue(stRow+0, 13,  spMny);
				m_pXL->SetValue(stRow+0, 14,  sMnyChk);
				m_pXL->SetValue(stRow+0, 15,  sqp);
				m_pXL->SetValue(stRow+0, 16,  sMaxPsChk);
				m_pXL->SetValue(stRow+0, 17,  s12Mcr);
				m_pXL->SetValue(stRow+0, 18,  s43Muy);
				m_pXL->SetValue(stRow+0, 19,  sMinPsChk);
				stRow += RowCnt;
			}
			else ASSERT(0);
		}
	}
	stRow += RowCnt+2;

	m_nTotalRow = stRow;

	return TRUE;
}


BOOL CPrtKSCE_USD10::Print_CumsDesign_Table(CArray<T_ELEM_K,T_ELEM_K>& arElemK, CArray<PRT_CUMS_KSCE,PRT_CUMS_KSCE>& arCumsKsce, CMSExcel* pXL)
{
	int RowCnt=0;
	int ColCnt=0;
	int stRow=m_nTotalRow;
	CString strBaseSheet =  _T("전단(Table)");
	CString strBlock;

	strBlock = _T("Title_Shear_Table");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	stRow += RowCnt;
	
	strBlock = _T("Shear_Table_Header");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	stRow += RowCnt;

	for(int i=0; i<arElemK.GetSize(); i++)
	{    
		T_ELEM_K ElemK = arElemK.GetAt(i);
		// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
		PRT_CUMS_KSCE PrtCumsD= arCumsKsce.GetAt(i);
		for(int j=0; j<2; j++)	// 0=I, 1=J
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
			CString sChk    = (PrtCumsD.CumsBase[j].dVu<PrtCumsD.CumsBase[j].dpVn ? _T("OK") : _T("NG"));   // 18

			strBlock = _T("Shear_Table_Body");
			m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
			m_pXL->SetValue(stRow+0,  0,  ElemK   );
			m_pXL->SetValue(stRow+0,  1,  sIJ     );
			m_pXL->SetValue(stRow+0,  2,  sbw     );
			m_pXL->SetValue(stRow+0,  3,  sdst    );
			m_pXL->SetValue(stRow+0,  4,  sh      );
			m_pXL->SetValue(stRow+0,  5,  sIy     );
			m_pXL->SetValue(stRow+0,  6,  syt     );
			m_pXL->SetValue(stRow+0,  7,  sfpe    );
			m_pXL->SetValue(stRow+0,  8,  sMcr    );
			m_pXL->SetValue(stRow+0,  9,  sVci    );
			m_pXL->SetValue(stRow+0, 10,  sfpc    );
			m_pXL->SetValue(stRow+0, 11,  sVp     );
			m_pXL->SetValue(stRow+0, 12,  sVcw    );
			m_pXL->SetValue(stRow+0, 13,  spVc    );
			m_pXL->SetValue(stRow+0, 14,  sVu     );
			m_pXL->SetValue(stRow+0, 15,  spVs    );
			m_pXL->SetValue(stRow+0, 16,  ssv     );
			m_pXL->SetValue(stRow+0, 17,  sAv     );
			m_pXL->SetValue(stRow+0, 18,  sAst    );
			m_pXL->SetValue(stRow+0, 19,  sAv_req );
			m_pXL->SetValue(stRow+0, 20,  sAs_use );
			m_pXL->SetValue(stRow+0, 21,  spVn    );
			m_pXL->SetValue(stRow+0, 22,  sChk    );

			stRow += RowCnt;     
		}
	}

	return TRUE;
}


void CPrtKSCE_USD10::Print_BdcyDesign(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam, /**/ CMSExcel* pXL)
{
	int RowCnt=0;
	int ColCnt=0;
	int stRow=m_nTotalRow;
	CString strBaseSheet =  _T("휨");

	if(m_bCompoSect) // PSC Comosite
	{
		Print_BdcyDesign0_Comp(BdcrKsce, BdcyParam, strBaseSheet, stRow);
	}
	else
	{
		Print_BdcyDesign0(BdcrKsce, BdcyParam, strBaseSheet, stRow);
	}

	if(BdcyParam.bIter)
	{
		Print_BdcyDesign4(BdcrKsce, BdcyParam, strBaseSheet, stRow);
		Print_BdcyDesign5(BdcrKsce, BdcyParam, strBaseSheet, stRow);
		Print_BdcyDesign6(BdcrKsce, BdcyParam, strBaseSheet, stRow);
		// 휨강도
		Print_BdcyDesign8(BdcrKsce, BdcyParam, strBaseSheet, stRow);
		// 최대 PS 강재량
		if(m_iDgnCode==KSCE_RAIL_USD11)
		{
			Print_BdcyDesign_Code5(BdcrKsce, BdcyParam, strBaseSheet, stRow);
		}
		// 최소강재량
		Print_BdcyDesign_Code10(BdcrKsce, BdcyParam, strBaseSheet, stRow);
	}
	else
	{
		Print_BdcyDesign_Code1(BdcrKsce, BdcyParam, strBaseSheet, stRow);

		if(BdcyParam.bOneType)
		{
			if(BdcyParam.bBondType)  Print_BdcyDesign_Code2(BdcrKsce, BdcyParam, strBaseSheet, stRow);
			else                     Print_BdcyDesign_Code3(BdcrKsce, BdcyParam, strBaseSheet, stRow);
		
			// 휨강도
			Print_BdcyDesign_Code4(BdcrKsce, BdcyParam, strBaseSheet, stRow);
		}
		else 
		{
			Print_BdcyDesign_Code6(BdcrKsce, BdcyParam, strBaseSheet, stRow);
			// 휨강도
			Print_BdcyDesign_Code7(BdcrKsce, BdcyParam, strBaseSheet, stRow); // 설계휨강도
		}

		// 최대 PS 강재량
		if(m_iDgnCode==KSCE_RAIL_USD11)
		{
			Print_BdcyDesign_Code5(BdcrKsce, BdcyParam, strBaseSheet, stRow);
		}
		// 최소강재량
		Print_BdcyDesign_Code10(BdcrKsce, BdcyParam, strBaseSheet, stRow);
	}
	stRow +=1;

	pXL = m_pXL;
	m_nTotalRow = stRow;
	m_nChapNum = 1;

}

void CPrtKSCE_USD10::Print_BdcyDesign0(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam,  CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	if(BdcyParam.bApwM)
	{
		strBlock = _T("Design_Condition_Apw");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		stRow += RowCnt;
	}
	stRow += 1;
 
	m_strChpt.Format(_T("%d."),m_nChapNum);  m_nChapNum++;
	strBlock = _T("Design_Condition");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt);
	m_pXL->SetValue(stRow+1,  4,  BdcyParam.dfck);
	m_pXL->SetValue(stRow+2,  4,  BdcyParam.dfy);
	m_pXL->SetValue(stRow+3,  4,  BdcyParam.dfpu);
	m_pXL->SetValue(stRow+4,  4,  BdcyParam.dbeta1);
	m_pXL->SetValue(stRow+5,  4,  BdcyParam.drp);
	m_pXL->SetValue(stRow+6,  4,  BdcyParam.db);
	m_pXL->SetValue(stRow+7,  4,  BdcyParam.dbw);
	m_pXL->SetValue(stRow+8,  4,  BdcyParam.ddp);
	m_pXL->SetValue(stRow+9,  4,  BdcyParam.dd);
	m_pXL->SetValue(stRow+10,  4,  BdcyParam.dyb);
	m_pXL->SetValue(stRow+11,  4,  BdcyParam.dep);
	m_pXL->SetValue(stRow+12,  4,  BdcyParam.dhf);
	m_pXL->SetValue(stRow+13,  4,  BdcyParam.dAps);
	m_pXL->SetValue(stRow+14,  4,  BdcyParam.dAs);
	m_pXL->SetValue(stRow+15,  4,  BdcyParam.dAc);
	m_pXL->SetValue(stRow+16,  4,  BdcyParam.dIy);
	m_pXL->SetValue(stRow+17,  4,  BdcyParam.dPe *m_dFactorForce);
	m_pXL->SetValue(stRow+18,  4,  BdcyParam.dMu *m_dFactorMoment);

	stRow += RowCnt+1;

}

void CPrtKSCE_USD10::Print_BdcyDesign0_Comp(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam,  CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;
	
	if(BdcyParam.bApwM)
	{
		strBlock = _T("Design_Condition_Apw");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		stRow += RowCnt;
	}
	stRow += 1;
	
	m_strChpt.Format(_T("%d."),m_nChapNum);  m_nChapNum++;
	strBlock = _T("Design_Condition_Comp");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+ 0,  1,  m_strChpt);
	m_pXL->SetValue(stRow+ 1,  4,  BdcyParam.dfck);
	m_pXL->SetValue(stRow+ 2,  4,  BdcyParam.dfy);
	m_pXL->SetValue(stRow+ 3,  4,  BdcyParam.dfpu);
	m_pXL->SetValue(stRow+ 4,  4,  BdcyParam.dbeta1);
	m_pXL->SetValue(stRow+ 5,  4,  BdcyParam.drp);
	m_pXL->SetValue(stRow+ 6,  4,  BdcyParam.dBc);  // 슬래브폭
	m_pXL->SetValue(stRow+ 7,  4,  BdcyParam.dtc);  // 슬래브높이
	m_pXL->SetValue(stRow+ 8,  4,  BdcyParam.dHh);  // 슬래브이격
	m_pXL->SetValue(stRow+ 9,  4,  BdcyParam.dH_gr);  // 거더 높이
	m_pXL->SetValue(stRow+10,  4,  BdcyParam.ddp);
	m_pXL->SetValue(stRow+11,  4,  BdcyParam.dd);
	m_pXL->SetValue(stRow+12,  4,  BdcyParam.dAps);
	m_pXL->SetValue(stRow+13,  4,  BdcyParam.dAs);
	m_pXL->SetValue(stRow+14,  4,  BdcyParam.dAc);
	m_pXL->SetValue(stRow+15,  4,  BdcyParam.dIy);
	m_pXL->SetValue(stRow+16,  4,  BdcyParam.dPe *m_dFactorForce);
	m_pXL->SetValue(stRow+17,  4,  BdcyParam.dMu *m_dFactorMoment);
	
	stRow += RowCnt+1;
	
}

void CPrtKSCE_USD10::Print_BdcyDesign_Code1(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam,  CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	strBlock = _T("Print_BdcyDesign_Code1");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_strChpt.Format(_T("%d."),m_nChapNum);  m_nChapNum++;
	m_pXL->SetValue(stRow+0,  1,  m_strChpt);  
	m_pXL->SetValue(stRow+1,  4,  BdcyParam.dNeuC);
	m_strChpt.Format(_T("%d."),m_nChapNum);  m_nChapNum++;
	m_pXL->SetValue(stRow+3,  1,  m_strChpt); 
	m_pXL->SetValue(stRow+4,  8,  BdcyParam.dbeta1);
	m_pXL->SetValue(stRow+4,  11,  BdcyParam.dNeuC);
	m_pXL->SetValue(stRow+4,  15,  BdcrKsce.da);
	
	stRow += RowCnt+1;
}


void CPrtKSCE_USD10::Print_BdcyDesign_Code2(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam,  CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;
	
	double  dfpu   =  BdcyParam.dfpu;
	double  drp    =  BdcyParam.drp   ;
	double  dbeta1 =  BdcyParam.dbeta1;
	double  dRhop  =  BdcyParam.dRhop ; 
	double  dfck   =  BdcyParam.dfck  ;
	double  dd     =  BdcyParam.dd    ;
	double  ddp    =  BdcyParam.ddp   ;
	double  dRhos  =  BdcrKsce.dpst  ;  //인장측 철근비	
	double  dRhoc  =  BdcrKsce.dpsc ;   //압축측 철근비	
	double  dfy    =  BdcyParam.dfy   ;
	double  dfps   =  BdcrKsce.dfps  ;  //계수하중 하에서 PS강재 평균응력	
	double dfps_temp_cal = BdcyParam.dfps_temp_cal;
	double dfps_temp = BdcyParam.dfps_temp;

	// PS강재 응력
	m_strChpt.Format(_T("%d."),m_nChapNum);  m_nChapNum++;
	strBlock = _T("Print_BdcyDesign_Code2");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt);  
	stRow += RowCnt+1;

	if(ddp==0.0) 
	{
		if(dd>0.0)
		{
			strBlock = _T("Print_BdcyDesign_Code2_01");
			m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
			//stRow += RowCnt+1;
			stRow += RowCnt;
		}
		else if(dd==0.0)
		{
			strBlock = _T("Print_BdcyDesign_Code2_02");
			m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
			//stRow += RowCnt+1;
			stRow += RowCnt;
		}
		else ASSERT(0);
	}
	else
	{
		if(m_iDgnCode==KSCE_USD10)
		{
			if(dd>0.0)
			{
				strBlock = _T("Print_BdcyDesign_Code2_03");
				m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
				m_pXL->SetValue(stRow+2,   4,  dRhop);
				m_pXL->SetValue(stRow+2,   7,  dfpu);
				m_pXL->SetValue(stRow+3,   7,  dfck);
				m_pXL->SetValue(stRow+2,  10,  dd);
				m_pXL->SetValue(stRow+3,  10,  ddp);
				m_pXL->SetValue(stRow+2,  13,  dRhos);
				m_pXL->SetValue(stRow+2,  16,  dfy);
				m_pXL->SetValue(stRow+3,  16,  dfck);
				m_pXL->SetValue(stRow+2,  19,  dRhoc);
				m_pXL->SetValue(stRow+2,  22,  dfy);
				m_pXL->SetValue(stRow+3,  22,  dfck);
				m_pXL->SetValue(stRow+4,   4,  dfps_temp_cal);
				m_pXL->SetValue(stRow+8,   4,  dfpu);
				m_pXL->SetValue(stRow+8,   8,  drp);
				m_pXL->SetValue(stRow+9,   8,  dbeta1);
				m_pXL->SetValue(stRow+8,  11,  dfps_temp);
				m_pXL->SetValue(stRow+8,  15,  dfps);
				//stRow += RowCnt+1;
				stRow += RowCnt;
			}
			else if(dd==0.0)
			{
				strBlock = _T("Print_BdcyDesign_Code2_04");
				m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
				m_pXL->SetValue(stRow+0,   8,  dfpu);
				m_pXL->SetValue(stRow+0,  13,  drp);
				m_pXL->SetValue(stRow+1,  13,  dbeta1);
				m_pXL->SetValue(stRow+0,  16,  dRhop);
				m_pXL->SetValue(stRow+0,  19,  dfpu);
				m_pXL->SetValue(stRow+1,  19,  dfck);
				m_pXL->SetValue(stRow+0,  22,  dfps);
				//stRow += RowCnt+1;
				stRow += RowCnt;
			}
			else ASSERT(0);

		}
		else if(m_iDgnCode==KSCE_RAIL_USD11)
		{      
			strBlock = _T("Print_BdcyDesign_Code2_03");
			m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
			m_pXL->SetValue(stRow+2,   4,  dRhop);
			m_pXL->SetValue(stRow+2,   7,  dfpu);
			m_pXL->SetValue(stRow+3,   7,  dfck);
			m_pXL->SetValue(stRow+2,  10,  dd);
			m_pXL->SetValue(stRow+3,  10,  ddp);
			m_pXL->SetValue(stRow+2,  13,  dRhos);
			m_pXL->SetValue(stRow+2,  16,  dfy);
			m_pXL->SetValue(stRow+3,  16,  dfck);
			m_pXL->SetValue(stRow+2,  19,  dRhoc);
			m_pXL->SetValue(stRow+2,  22,  dfy);
			m_pXL->SetValue(stRow+3,  22,  dfck);
			m_pXL->SetValue(stRow+4,   4,  dfps_temp_cal);
			m_pXL->SetValue(stRow+8,   4,  dfpu);
			m_pXL->SetValue(stRow+8,   8,  drp);
			m_pXL->SetValue(stRow+9,   8,  dbeta1);
			m_pXL->SetValue(stRow+8,  11,  dfps_temp);
			m_pXL->SetValue(stRow+8,  15,  dfps);
			//stRow += RowCnt+1;
			stRow += RowCnt;

		}
		else ASSERT(0);

	}

}

void CPrtKSCE_USD10::Print_BdcyDesign_Code3(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam,  CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	double dfck        = BdcyParam.dfck;
	double dfpu        = BdcyParam.dfpu;
	double dfpy        = BdcyParam.dfpy_unbond;
	double dfpe        = BdcyParam.dfpe_unb_cal;
	double dfpe_cal    = BdcyParam.dfpe_unb;
	double d05fpu      = BdcyParam.d05fpu;
	double dfps        = BdcrKsce.dfps;
	double dfps_cal    = BdcyParam.dfps_cal;
	double dRhop       = BdcyParam.dRhop ; 
	double dfps_unbCom = BdcyParam.dfps_un_com;
	double dfps_un_max = BdcyParam.dfps_un_max;

	m_strChpt.Format(_T("%d."),m_nChapNum); m_nChapNum++;
	strBlock = _T("Print_BdcyDesign_Code3");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1, m_strChpt ); 
	stRow += RowCnt;

	if(dfps==0.0)
	{
		strBlock = _T("Print_BdcyDesign_Code3_01");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		//stRow += RowCnt+1;
		stRow += RowCnt;
	}
	else
	{
		if(m_iDgnCode==KSCE_USD10)
		{
			// (가) 지간과 높이의 비가 35이하인 경우. defualt (나). 항목 추가해야함. //!/
			strBlock = _T("Print_BdcyDesign_Code3_02");
		}
		else if(m_iDgnCode==KSCE_RAIL_USD11)
		{
			strBlock = _T("Print_BdcyDesign_Code3_02_Rail");
		}
		else ASSERT(0);
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		m_pXL->SetValue(stRow+0,   8,  dfpe_cal);
		m_pXL->SetValue(stRow+0,  13,  dfck);
		m_pXL->SetValue(stRow+1,  15,  dRhop);
		m_pXL->SetValue(stRow+0,  18,  dfps_cal);   
		m_pXL->SetValue(stRow+2,   9,  dfps_cal);   
		m_pXL->SetValue(stRow+2,  12,  dfpy);   
		m_pXL->SetValue(stRow+2,  15,  dfps_unbCom);   
		m_pXL->SetValue(stRow+2,  18,  dfps);   
		stRow += RowCnt+1;
	}
}


void CPrtKSCE_USD10::Print_BdcyDesign_Code4(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam,  CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;
	
	double da     = BdcrKsce.da;
	double db     = BdcyParam.db;
	double dbw    = BdcyParam.dbw;
	double dApw   = BdcyParam.dApw  ;
	double dApf   = BdcyParam.dApf  ;
	double dAp    = BdcyParam.dAps   ;
	double dAs    = BdcrKsce.dAst   ;
	double dhf    = BdcyParam.dhf   ;
	double dfpu   = BdcyParam.dfpu  ;
	double drp    = BdcyParam.drp   ;
	double dbeta1 = BdcyParam.dbeta1;
	double dRhop  = BdcyParam.dRhop ;  
	double dfck   = BdcyParam.dfck  ;
	double dd     = BdcyParam.dd    ;
	double ddp    = BdcyParam.ddp   ;
	double dRho   = BdcrKsce.dpst  ;  //인장측 철근비	
	double dfy    = BdcyParam.dfy   ;
	double dfps   = BdcrKsce.dfps  ;
	double dphi   = BdcrKsce.dphi  ;
	double dMn    = BdcyParam.dMn_Cal;
	double dpMn   = BdcyParam.dphiMn_Cal  ;
	double dMu    = BdcyParam.dMu   ;

	m_strChpt.Format(_T("%d."),m_nChapNum); m_nChapNum++;
	strBlock = _T("Print_BdcyDesign_Code4");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1, m_strChpt ); 
	m_pXL->SetValue(stRow+1,   3,  da);
	m_pXL->SetValue(stRow+1,   8,  dhf);
	stRow += RowCnt+1;

	CString strPhi;
	if(dphi<=0.7)        strPhi =_T("(압축지배 단면)");
	else if(dphi>=0.85)  strPhi =_T("(인장지배 단면)");
	else                 strPhi =_T("(변화구간 단면)");

	strBlock = _T("Print_BdcyDesign_Code4_phi");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,   3,  dphi);
	m_pXL->SetValue(stRow+0,   5,  strPhi);
	stRow += RowCnt+1;

	
	if(ddp==0.0)
	{
		if(da<=dhf)  // 사각형단면
		{
			if(dAs>0.0)
			{
				strBlock = _T("Print_BdcyDesign_Code4_01");
				m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
				m_pXL->SetValue(stRow+0,   9,  dphi);
				m_pXL->SetValue(stRow+0,  14,  dfck);
				m_pXL->SetValue(stRow+0,  17,  da);
				m_pXL->SetValue(stRow+0,  20,  db);
				m_pXL->SetValue(stRow+1,  12,  dd);
				m_pXL->SetValue(stRow+1,  16,  da);
				m_pXL->SetValue(stRow+1,  20,  dpMn * m_dFactorMoment);
				stRow += RowCnt+1;
			}
			else ASSERT(0);
		}
		else if(da>dhf) // 플랜지 단면
		{
			if(dAs>0.0)
			{
				strBlock = _T("Print_BdcyDesign_Code4_02");
				m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
				m_pXL->SetValue(stRow+1,   3,  dphi);
				m_pXL->SetValue(stRow+1,   8,  dfck);
				m_pXL->SetValue(stRow+1,  11,  da); 
				m_pXL->SetValue(stRow+1,  14,  dbw);
				m_pXL->SetValue(stRow+1,  17,  dd);
				m_pXL->SetValue(stRow+1,  21,  da);

				m_pXL->SetValue(stRow+2,   5,  dfck);
				m_pXL->SetValue(stRow+2,   8,  db);
				m_pXL->SetValue(stRow+2,  11,  dbw);
				m_pXL->SetValue(stRow+2,  14,  dhf);
				m_pXL->SetValue(stRow+2,  17,  dd);
				m_pXL->SetValue(stRow+2,  21,  dhf);

				m_pXL->SetValue(stRow+3,   3,  dpMn * m_dFactorMoment);
				stRow += RowCnt+1;
			}
			else ASSERT(0);
		}
		else ASSERT(0);
	}
	else
	{
		if(da<=dhf)  // 사각형단면
		{
			if(dAs>0.0)
			{
				strBlock = _T("Print_BdcyDesign_Code4_03");
				m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
				m_pXL->SetValue(stRow+3,   3,  dphi);
				m_pXL->SetValue(stRow+3,   6,  dAp);
				m_pXL->SetValue(stRow+3,   9,  dfps);
				m_pXL->SetValue(stRow+3,  12,  ddp);

				m_pXL->SetValue(stRow+4,   8,  dRhop);
				m_pXL->SetValue(stRow+4,  11,  dfps);
				m_pXL->SetValue(stRow+5,   8,  dfck);

				m_pXL->SetValue(stRow+4,  14,  dd);
				m_pXL->SetValue(stRow+5,  14,  ddp);

				m_pXL->SetValue(stRow+4,  17,  dRho);
				m_pXL->SetValue(stRow+4,  20,  dfy);
				m_pXL->SetValue(stRow+5,  17,  dfck);

				m_pXL->SetValue(stRow+6,   6,  dAs);
				m_pXL->SetValue(stRow+6,   9,  dfy);
				m_pXL->SetValue(stRow+6,  12,  dd);

				m_pXL->SetValue(stRow+7,   8,  ddp);
				m_pXL->SetValue(stRow+8,   8,  dd);

				m_pXL->SetValue(stRow+7,  11,  dRhop);
				m_pXL->SetValue(stRow+7,  14,  dfps);
				m_pXL->SetValue(stRow+8,  11,  dfck);

				m_pXL->SetValue(stRow+7,  17,  dRho);
				m_pXL->SetValue(stRow+7,  20,  dfy);
				m_pXL->SetValue(stRow+8,  17,  dfck);

				m_pXL->SetValue(stRow+9,   3,  dpMn * m_dFactorMoment);
				stRow += RowCnt+1;
			}
			else // 철근 없을때
			{
				strBlock = _T("Print_BdcyDesign_Code4_04");
				m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
				m_pXL->SetValue(stRow+2,   3,  dphi);
				m_pXL->SetValue(stRow+2,   6,  dAp);
				m_pXL->SetValue(stRow+2,   9,  dfps);
				m_pXL->SetValue(stRow+2,  12,  ddp);

				m_pXL->SetValue(stRow+2,  18,  dRhop);
				m_pXL->SetValue(stRow+2,  21,  dfps);
				m_pXL->SetValue(stRow+3,  18,  dfck);

				m_pXL->SetValue(stRow+4,   3,  dpMn * m_dFactorMoment);
				stRow += RowCnt+1;
			}

		}
		else if(da>dhf) // 플랜지 단면
		{
			if(m_iDgnCode==KSCE_USD10)           strBlock = _T("Print_BdcyDesign_Code4_05_Apf");
			else if(m_iDgnCode==KSCE_RAIL_USD11) strBlock = _T("Print_BdcyDesign_Code4_05_Apf_rail");  
			m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
			m_pXL->SetValue(stRow+1,  5,  dfck);
			m_pXL->SetValue(stRow+1,  8,  db);
			m_pXL->SetValue(stRow+1, 11,  dbw);
			m_pXL->SetValue(stRow+1, 14,  dhf);
			m_pXL->SetValue(stRow+1, 17,  dfps);
			m_pXL->SetValue(stRow+1, 20,  dApf);    
			stRow += RowCnt+1;

			if(m_iDgnCode==KSCE_USD10)  
			{
				if(dAs>0.0)
				{
					strBlock = _T("Print_BdcyDesign_Code4_05_Apw01");
					m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
					m_pXL->SetValue(stRow+1,  3,  dAp);
					m_pXL->SetValue(stRow+1,  6,  dAs);
					m_pXL->SetValue(stRow+1,  9,  dfy);
					m_pXL->SetValue(stRow+1, 12,  dfps);
					m_pXL->SetValue(stRow+1, 15,  dApf);
					m_pXL->SetValue(stRow+1, 18,  dApw);
					stRow += RowCnt+1;      
				}
				else // 철근 없을때
				{
					strBlock = _T("Print_BdcyDesign_Code4_05_Apw02");
					m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
					m_pXL->SetValue(stRow+1,  3,  dAp);
					m_pXL->SetValue(stRow+1,  6,  dApf);
					m_pXL->SetValue(stRow+1,  9,  dApw);
					stRow += RowCnt+1;              
				}
				
				strBlock = _T("Print_BdcyDesign_Code4_05");
				m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
				m_pXL->SetValue(stRow+3,   3,  dphi);
				m_pXL->SetValue(stRow+3,   6,  dApw);
				m_pXL->SetValue(stRow+3,   9,  dfps);
				m_pXL->SetValue(stRow+3,  12,  ddp);
				m_pXL->SetValue(stRow+4,   9,  dApw);
				m_pXL->SetValue(stRow+4,  12,  dfps);
				m_pXL->SetValue(stRow+5,   8,  dbw);
				m_pXL->SetValue(stRow+5,  11,  ddp);
				m_pXL->SetValue(stRow+5,  14,  dfck);
				m_pXL->SetValue(stRow+6,   6,  dAs);
				m_pXL->SetValue(stRow+6,   9,  dfy);
				m_pXL->SetValue(stRow+6,  12,  dd);
				m_pXL->SetValue(stRow+6,  15,  ddp);
				m_pXL->SetValue(stRow+7,   8,  dfck);
				m_pXL->SetValue(stRow+7,  11,  db);
				m_pXL->SetValue(stRow+7,  14,  dbw);
				m_pXL->SetValue(stRow+7,  17,  dhf);
				m_pXL->SetValue(stRow+7,  20,  ddp);
				m_pXL->SetValue(stRow+7,  24,  dhf);
				m_pXL->SetValue(stRow+8,   3,  dpMn * m_dFactorMoment);
				stRow += RowCnt+1;

			}
			else if(m_iDgnCode==KSCE_RAIL_USD11)  
			{
				strBlock = _T("Print_BdcyDesign_Code4_05_Apw02");
				m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
				m_pXL->SetValue(stRow+1,  3,  dAp);
				m_pXL->SetValue(stRow+1,  6,  dApf);
				m_pXL->SetValue(stRow+1,  9,  dApw);
				stRow += RowCnt+1;              

				strBlock = _T("Print_BdcyDesign_Code4_05_rail");
				m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
				m_pXL->SetValue(stRow+2,   3,  dphi);
				m_pXL->SetValue(stRow+2,   6,  dAp);
				m_pXL->SetValue(stRow+2,   9,  dApf);
				m_pXL->SetValue(stRow+2,  12,  dfps);
				m_pXL->SetValue(stRow+2,  15,  ddp);
				m_pXL->SetValue(stRow+2,  19,  da);
				m_pXL->SetValue(stRow+3,   8,  dfck);
				m_pXL->SetValue(stRow+3,  11,  db);
				m_pXL->SetValue(stRow+3,  14,  dbw);
				m_pXL->SetValue(stRow+3,  17,  dhf);
				m_pXL->SetValue(stRow+3,  20,  ddp);
				m_pXL->SetValue(stRow+3,  24,  dhf);
				m_pXL->SetValue(stRow+4,   3,  dpMn * m_dFactorMoment);
				stRow += RowCnt+1;      
			}
			else ASSERT(0);

		}
		else ASSERT(0);
	}
	 
	strBlock = _T("Print_BdcyDesign_Code4_05_check");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,   4,  dpMn * m_dFactorMoment);
	m_pXL->SetValue(stRow+0,  13,  dMu  * m_dFactorMoment);
	stRow += RowCnt+1;

}



void CPrtKSCE_USD10::Print_BdcyDesign_Code5(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam,  CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	double dphi   = BdcrKsce.dphi;
	double dfpu   = BdcyParam.dfpu;
	double dbeta1 = BdcyParam.dbeta1;
	double dRhop  = BdcyParam.drp;  
	double dfck   = BdcyParam.dfck;
	double da     = BdcrKsce.da;  
	double dhf    = BdcyParam.dhf;  
	double db     = BdcyParam.db;
	double dbw    = BdcyParam.dbw;
	double dd     = BdcyParam.dd;
	double ddp    = BdcyParam.ddp;
	double dqp    = BdcrKsce.dqp;  // 강재지수
	double dRho   = BdcrKsce.dpst;  //  인장측 철근비
	double dApw   = BdcyParam.dApw;
	double dAp    = BdcyParam.dAps;
	double dApf   = BdcyParam.dApf;
	double dAs    = BdcyParam.dAs;
	double dfy    = BdcyParam.dfy;
	double dfps   = BdcrKsce.dfps;
	double dpMn   = BdcrKsce.dpMny;
	double dpMn_beta = BdcyParam.dphiMn_beta1;
	double d036beta1 = BdcrKsce.d036Beta1;
	double dpMn_Cal  = BdcyParam.dphiMn_Cal;
	double dpMny     = BdcrKsce.dpMny;
	double dMu     = BdcyParam.dMu;

	m_strChpt.Format(_T("%d."),m_nChapNum); m_nChapNum++;
	strBlock = _T("Title_Print_BdcyDesign_Code5");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1, m_strChpt ); 
	stRow += RowCnt;

	if(ddp==0.0)
	{
		strBlock = _T("Print_BdcyDesign_Code5_01");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		stRow += RowCnt+1;
		return;
	}

	strBlock = _T("Print_BdcyDesign_Code5_036beta");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  7, dbeta1 ); 
	m_pXL->SetValue(stRow+0, 10, d036beta1 ); 
	stRow += RowCnt+1;

	strBlock = _T("Print_BdcyDesign_Code5_check_rec");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  3, da ); 
	m_pXL->SetValue(stRow+0,  8, dhf ); 
	stRow += RowCnt+1;

	// qp계산  
	if(da <= dhf)  // 사각형 단면
	{    
		strBlock = _T("Print_BdcyDesign_Code5_qp_rec");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		m_pXL->SetValue(stRow+0,  6, dRhop); 
		m_pXL->SetValue(stRow+0,  9, dfps);
		m_pXL->SetValue(stRow+1,  9, dfck);
		m_pXL->SetValue(stRow+0, 12, dqp);
		m_pXL->SetValue(stRow+0, 19, d036beta1);
		stRow += RowCnt+1;
	}
	else if(da > dhf)  // 풀랜지 단면
	{
		strBlock = _T("Print_BdcyDesign_Code5_qp_flange");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		m_pXL->SetValue(stRow+0,  6, dApw); 
		m_pXL->SetValue(stRow+1,  6, dbw);
		m_pXL->SetValue(stRow+1,  9, ddp);
		m_pXL->SetValue(stRow+0, 12, dfps);
		m_pXL->SetValue(stRow+1, 12, dfck);
		m_pXL->SetValue(stRow+0, 15, dqp);
		m_pXL->SetValue(stRow+0, 22, d036beta1);
		stRow += RowCnt+1;
	}
	else ASSERT(0);

	//  dqp <= d036beta1 이면 아래는 출력 안한다.
	if(dqp <= d036beta1) return;

	strBlock = _T("Print_BdcyDesign_Code5_02");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	stRow += RowCnt;

	// pMn_Beta계산  
	if(da <= dhf)  // 사각형 단면
	{    
		strBlock = _T("Print_BdcyDesign_Code5_pMn_rec");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		m_pXL->SetValue(stRow+1,   4, dphi);
		m_pXL->SetValue(stRow+1,   7, dfck);
		m_pXL->SetValue(stRow+1,  11, dbeta1);
		m_pXL->SetValue(stRow+1,  16, dbeta1);
		m_pXL->SetValue(stRow+1,  19, db);
		m_pXL->SetValue(stRow+1,  22, ddp);
		m_pXL->SetValue(stRow+2,   4, dpMn_beta * m_dFactorMoment);
		stRow += RowCnt+1;
	}
	else if(da > dhf)  // 풀랜지 단면
	{
		strBlock = _T("Print_BdcyDesign_Code5_Apf");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		m_pXL->SetValue(stRow+1,   5, dfck);
		m_pXL->SetValue(stRow+1,   8, db);
		m_pXL->SetValue(stRow+1,  11, dbw);
		m_pXL->SetValue(stRow+1,  14, dhf);
		m_pXL->SetValue(stRow+1,  17, dfps);
		m_pXL->SetValue(stRow+1,  20, dApf);
		stRow += RowCnt+1;

		strBlock = _T("Print_BdcyDesign_Code5_Apw");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		m_pXL->SetValue(stRow+1,   3, dAp);
		m_pXL->SetValue(stRow+1,   6, dApf);
		m_pXL->SetValue(stRow+1,   9, dApw);
		stRow += RowCnt+1;

		strBlock = _T("Print_BdcyDesign_Code5_pMn_flange");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		m_pXL->SetValue(stRow+1,   4, dphi);
		m_pXL->SetValue(stRow+1,   7, dfck);
		m_pXL->SetValue(stRow+1,   11, dbeta1);
		m_pXL->SetValue(stRow+1,   16, dbeta1);
		m_pXL->SetValue(stRow+1,   19, dbw);
		m_pXL->SetValue(stRow+1,   22, ddp);
		m_pXL->SetValue(stRow+2,    6, dfck);
		m_pXL->SetValue(stRow+2,    9, db);
		m_pXL->SetValue(stRow+2,   12, dbw);
		m_pXL->SetValue(stRow+2,   15, dhf);
		m_pXL->SetValue(stRow+2,   18, ddp);
		m_pXL->SetValue(stRow+2,   22, dhf);
		m_pXL->SetValue(stRow+3,    4, dpMn_beta * m_dFactorMoment);
		stRow += RowCnt+1;
	}
	else ASSERT(0);

	// pMn계산  
	strBlock = _T("Print_BdcyDesign_Code5_Mn");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,    6, dpMn_beta * m_dFactorMoment);
	m_pXL->SetValue(stRow+0,   11, dpMn_Cal * m_dFactorMoment);
	m_pXL->SetValue(stRow+0,   16, dpMn * m_dFactorMoment);
	stRow += RowCnt;
	
	strBlock = _T("Print_BdcyDesign_Code4_05_check");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,   4,  dpMn * m_dFactorMoment);
	m_pXL->SetValue(stRow+0,  13,  dMu  * m_dFactorMoment);
	stRow += RowCnt+1;

}


void CPrtKSCE_USD10::Print_BdcyDesign_Code6(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam,  CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;


	CArray<CString,CString> strTndn;
	strTndn.Copy(BdcyParam.strTndn);

	int TndnNum = BdcyParam.arfps0.GetSize();

	m_strChpt.Format(_T("%d."),m_nChapNum); m_nChapNum++; 
	strBlock = _T("Print_BdcyDesign_Code6");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt); 
	stRow += RowCnt+1;

	strBlock = _T("Print_BdcyDesign_Code6_header");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	stRow += RowCnt;

	if(TndnNum<=0)
	{
		strBlock = _T("Print_BdcyDesign_Code6_body_last");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		m_pXL->SetValue(stRow+0,    1,  _T("-"));
		m_pXL->SetValue(stRow+0,    3,  _T("-"));
		m_pXL->SetValue(stRow+0,    7,  _T("-"));
		m_pXL->SetValue(stRow+0,   11,  _T("-"));
		m_pXL->SetValue(stRow+0,   15,  _T("-"));
		m_pXL->SetValue(stRow+0,   19,  _T("-"));
		m_pXL->SetValue(stRow+0,   23,  _T("-"));
		stRow += RowCnt+1;
	}
	else
	{
		
		CString strTemp;
		for(int i=0; i<TndnNum; i++)
		{
			strTemp =BdcyParam.arfps4.GetAt(i)==0?  _T("부착") : _T("비부착");
			if(i == TndnNum-1 ) strBlock = _T("Print_BdcyDesign_Code6_body_last");
			else                strBlock = _T("Print_BdcyDesign_Code6_body");
			m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
			m_pXL->SetValue(stRow+0,    1,  i+1);
			m_pXL->SetValue(stRow+0,    3,  strTndn.GetAt(i));   // TnGroup          
			m_pXL->SetValue(stRow+0,    7,  BdcyParam.arfps0.GetAt(i));  // Apsi
			m_pXL->SetValue(stRow+0,   11,  BdcyParam.arfps1.GetAt(i));  // fpui               
			m_pXL->SetValue(stRow+0,   15,  BdcyParam.arfps2.GetAt(i));  // fpsi                
			m_pXL->SetValue(stRow+0,   19,  BdcyParam.arfps3.GetAt(i));  // dpi                 
			m_pXL->SetValue(stRow+0,   23,  strTemp);  // Type
			stRow += RowCnt;            
		}
		stRow += 1;
	}

}

void CPrtKSCE_USD10::Print_BdcyDesign_Code7(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam,  CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	double dc     = BdcyParam.dNeuC    ;
	double da     = BdcrKsce.da    ;
	double db     = BdcyParam.db    ;
	double dbw    = BdcyParam.dbw   ;
	double dAs    = BdcrKsce.dAst   ;
	double dhf    = BdcyParam.dhf   ;
	double dfck   = BdcyParam.dfck  ;
	double dd     = BdcyParam.dd    ;
	double ddp    = BdcyParam.ddp   ;
	double dfy    = BdcyParam.dfy   ;
	double dphi   = BdcrKsce.dphi   ;
	double dMn_ps = BdcyParam.dMn_ps ;   
	double dpMn   = BdcrKsce.dpMny   ; //면내 설계휨모멘트	
	double dMu    = BdcyParam.dMu;

	m_strChpt.Format(_T("%d."),m_nChapNum); m_nChapNum++;
	strBlock = _T("Print_BdcyDesign_Code4");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1, m_strChpt ); 
	m_pXL->SetValue(stRow+1,   3,  da);
	m_pXL->SetValue(stRow+1,   8,  dhf);
	stRow += RowCnt+1;

	CString strPhi;
	if(dphi<=0.7)        strPhi =_T("(압축지배 단면)");
	else if(dphi>=0.85)  strPhi =_T("(인장지배 단면)");
	else                 strPhi =_T("(변화구간 단면)");
	
	strBlock = _T("Print_BdcyDesign_Code4_phi");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,   3,  dphi);
	m_pXL->SetValue(stRow+0,   5,  strPhi);
	stRow += RowCnt+1;

	if(da<=dhf) // 사각 단면
	{
		strBlock = _T("BdcyDesign_Code7_rec");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		m_pXL->SetValue(stRow+1,   3,  dphi);
		m_pXL->SetValue(stRow+1,   8,  dfck);
		m_pXL->SetValue(stRow+1,  11,  da);
		m_pXL->SetValue(stRow+1,  14,  db);
		m_pXL->SetValue(stRow+1,  17,  dc);
		m_pXL->SetValue(stRow+1,  21,  da);
		m_pXL->SetValue(stRow+2,   6,  dMn_ps);
		m_pXL->SetValue(stRow+2,  13,  dAs);
		m_pXL->SetValue(stRow+2,  16,  dfy);
		m_pXL->SetValue(stRow+2,  19,  dd);
		m_pXL->SetValue(stRow+2,  22,  dc);
		m_pXL->SetValue(stRow+3,   3,  dpMn * m_dFactorMoment);
		stRow += RowCnt+1;
	}
	else if(da>dhf) // 플랜지 단면
	{
		strBlock = _T("BdcyDesign_Code7_flange");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		m_pXL->SetValue(stRow+1,   3,  dphi);
		m_pXL->SetValue(stRow+1,   8,  dfck);
		m_pXL->SetValue(stRow+1,  11,  da);
		m_pXL->SetValue(stRow+1,  14,  dbw);
		m_pXL->SetValue(stRow+1,  17,  dc);
		m_pXL->SetValue(stRow+1,  21,  da);
		m_pXL->SetValue(stRow+2,   6,  db);
		m_pXL->SetValue(stRow+2,   9,  dbw);
		m_pXL->SetValue(stRow+2,  12,  dhf);
		m_pXL->SetValue(stRow+2,  15,  dc);
		m_pXL->SetValue(stRow+2,  19,  da);
		m_pXL->SetValue(stRow+3,   6,  dMn_ps);
		m_pXL->SetValue(stRow+3,  13,  dAs);
		m_pXL->SetValue(stRow+3,  16,  dfy);
		m_pXL->SetValue(stRow+3,  19,  dd);
		m_pXL->SetValue(stRow+3,  22,  dc);
		m_pXL->SetValue(stRow+4,   3,  dpMn * m_dFactorMoment);
		stRow += RowCnt+1;
	}
	else ASSERT(0);

	strBlock = _T("Print_BdcyDesign_Code4_05_check");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,   4,  dpMn * m_dFactorMoment);
	m_pXL->SetValue(stRow+0,  13,  dMu  * m_dFactorMoment);
	stRow += RowCnt+1;

}



void CPrtKSCE_USD10::Print_BdcyDesign_Code10(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam,  CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;
 
	double dPe        = BdcyParam.dPe_t;    
	double dAc        = BdcyParam.dAc;
	double dPeTimesep = BdcyParam.dPeep;
	double dyt        = BdcyParam.dyb;
	double dIy        = BdcyParam.dIy ;
	double dfpe       = BdcrKsce.dfpe; 
	double dfck       = BdcyParam.dfck;
	double dpMn       = BdcrKsce.dpMny;
	double d12Mcr     = BdcrKsce.d12Mcr;
	double dMu        = BdcyParam.dMu;
	double dMcr       = BdcyParam.dMcr;

	m_strChpt.Format(_T("%d."),m_nChapNum);  m_nChapNum++;
	strBlock = _T("Print_BdcyDesign_Code10");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt); 
	m_pXL->SetValue(stRow+1,    4,  dpMn          *m_dFactorMoment );
	m_pXL->SetValue(stRow+1,   13,  4.0/3.0 *dMu  *m_dFactorMoment );
	stRow += RowCnt+1;

	if(dpMn < 4.0/3.0 *fabs(dMu))
	{
		strBlock = _T("Print_BdcyDesign_Code10_01");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		m_pXL->SetValue(stRow+0,    7,  dPe );
		m_pXL->SetValue(stRow+1,    7,  dAc);
		m_pXL->SetValue(stRow+0,   11,  dPeTimesep);
		m_pXL->SetValue(stRow+1,   11,  dIy);
		m_pXL->SetValue(stRow+0,   17,  dyt);
		m_pXL->SetValue(stRow+0,   20,  dfpe);
		stRow += RowCnt+1;
		
		strBlock = _T("Print_BdcyDesign_Code10_02");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		m_pXL->SetValue(stRow+1,    6,  dfck );
		m_pXL->SetValue(stRow+1,    9,  dfpe );
		m_pXL->SetValue(stRow+1,   12,  dIy);
		m_pXL->SetValue(stRow+1,   16,  dyt);
		m_pXL->SetValue(stRow+1,   20,  dMcr * m_dFactorMoment);
		stRow += RowCnt+1;
		
		strBlock = _T("Print_BdcyDesign_Code10_check");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		m_pXL->SetValue(stRow+0,   4,  dpMn * m_dFactorMoment);
		m_pXL->SetValue(stRow+0,  13,  d12Mcr * m_dFactorMoment);
		stRow += RowCnt+1;
	}

}


void CPrtKSCE_USD10::Print_BdcyDesign4(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam,  CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	m_strChpt.Format(_T("%d."),m_nChapNum); m_nChapNum++;
	strBlock = _T("Print_BdcyDesign_Code1");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt);   
	m_pXL->SetValue(stRow+1,  4,  BdcyParam.dNeuC);
	m_strChpt.Format(_T("%d."),m_nChapNum); m_nChapNum++;
	m_pXL->SetValue(stRow+3,  1,  m_strChpt); 
	m_pXL->SetValue(stRow+4,  8,  BdcyParam.dbeta1);
	m_pXL->SetValue(stRow+4,  11,  BdcyParam.dNeuC);
	m_pXL->SetValue(stRow+4,  15,  BdcrKsce.da);
	stRow += RowCnt+1;

	double daArea = BdcyParam.daArea;
	double dfck   = BdcyParam.dfck;
	double dCc    = BdcyParam.dCc;

	m_strChpt.Format(_T("%d."),m_nChapNum); m_nChapNum++;
	strBlock = _T("Print_BdcyDesign4");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt); 
	m_pXL->SetValue(stRow+1, 11,  dfck); 
	m_pXL->SetValue(stRow+1, 15,  daArea); 
	m_pXL->SetValue(stRow+1, 19,  dCc * m_dFactorForce); 
	stRow += RowCnt+1;

}


void CPrtKSCE_USD10::Print_BdcyDesign5(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam,  CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;
	 
	CArray<CString,CString> strTndn; 
	strTndn.Copy(BdcyParam.strTndn);
	
	double dTp=BdcyParam.dTp;
	double dTpTimesep = BdcyParam.dTpTimesep;
	double dPeTimesep = BdcyParam.dPeTimesep;

	int TndnNum = BdcyParam.arfps0.GetSize();
	
	m_strChpt.Format(_T("%d."),m_nChapNum); m_nChapNum++;
	strBlock = _T("Print_BdcyDesign5");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt); 
	stRow += RowCnt;
	
	strBlock = _T("Print_BdcyDesign5_header");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	stRow += RowCnt;
	
	if(TndnNum<=0)
	{
		strBlock = _T("Print_BdcyDesign5_body_last");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		m_pXL->SetValue(stRow+0,    1,  _T("-"));
		m_pXL->SetValue(stRow+0,    4,  _T("-"));  // Pei
		m_pXL->SetValue(stRow+0,    7,  _T("-"));// Apsi
		m_pXL->SetValue(stRow+0,   10,  _T("-"));// dpi
		m_pXL->SetValue(stRow+0,   12,  _T("-"));// epi
		m_pXL->SetValue(stRow+0,   14,  _T("-"));// epsi_ps
		m_pXL->SetValue(stRow+0,   16,  _T("-"));// epsi_ps
		m_pXL->SetValue(stRow+0,   19,  _T("-"));// epsi_ps
		m_pXL->SetValue(stRow+0,   22,  _T("-"));// fps
		m_pXL->SetValue(stRow+0,   25,  _T("-"));// Tpi
		stRow += RowCnt+1;
	}
	else
	{
		for(int i=0; i<TndnNum; i++)
		{
			if(i == TndnNum-1 ) strBlock = _T("Print_BdcyDesign5_body_last");
			else                strBlock = _T("Print_BdcyDesign5_body");      
			m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
			m_pXL->SetValue(stRow+0,    1,  strTndn.GetAt(i));  // TnGroup   
			m_pXL->SetValue(stRow+0,    4,  BdcyParam.arfps0.GetAt(i) *m_dFactorForce); // Pei
			m_pXL->SetValue(stRow+0,    7,  BdcyParam.arfps1.GetAt(i)); // Apsi
			m_pXL->SetValue(stRow+0,   10,  BdcyParam.arfps2.GetAt(i)); // dpi
			m_pXL->SetValue(stRow+0,   12,  BdcyParam.arfps3.GetAt(i)); // epi
			m_pXL->SetValue(stRow+0,   14,  BdcyParam.arfps4.GetAt(i)); // epsi_ps
			m_pXL->SetValue(stRow+0,   16,  BdcyParam.arfps5.GetAt(i)); // fps
			m_pXL->SetValue(stRow+0,   19,  BdcyParam.arfps6.GetAt(i) *m_dFactorForce); // Tpi
			m_pXL->SetValue(stRow+0,   22,  BdcyParam.arfps7.GetAt(i) *m_dFactorMoment); // Tpi*epi
			m_pXL->SetValue(stRow+0,   25,  BdcyParam.arfps8.GetAt(i) *m_dFactorMoment); // Pei*epi
			stRow += RowCnt;            
		}
		stRow += 1;
	}

	strBlock = _T("Print_BdcyDesign5_Ts");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,   7,  dTp        *m_dFactorForce); 
	m_pXL->SetValue(stRow+1,   7, dTpTimesep *m_dFactorMoment);
	stRow += RowCnt+1;            

}


void CPrtKSCE_USD10::Print_BdcyDesign6(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam,  CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	double dep_cu = BdcyParam.dEpsi_cu;
	double ddst   = BdcyParam.dd; 
	double dc     = BdcyParam.dNeuC;
	double dep_s  = BdcyParam.dEpsi_st;
	double dfs    = BdcyParam.dfst ;
	double dfs_cal= BdcyParam.dfst_cal;
	double dEs    = BdcyParam.dEs;
	double dfy    = BdcyParam.dfy;
	double dAst   = BdcyParam.dAs;
	double dTs    = BdcyParam.dTst;

	m_strChpt.Format(_T("%d."),m_nChapNum); m_nChapNum++;
	strBlock = _T("Print_BdcyDesign6");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1, m_strChpt ); 
	stRow += RowCnt;

	if(dAst==0.0)
	{
		strBlock = _T("Print_BdcyDesign6_01");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		m_pXL->SetValue(stRow+0,    6,  dAst);  
		m_pXL->SetValue(stRow+0,   10,  dfs);  
		m_pXL->SetValue(stRow+0,   14,  dTs *m_dFactorForce);

		stRow += RowCnt+1;
	}
	else
	{
		strBlock = _T("Print_BdcyDesign6_02");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);

		m_pXL->SetValue(stRow+0,    6,  dep_cu);  
		m_pXL->SetValue(stRow+0,    9,  ddst);
		m_pXL->SetValue(stRow+0,   12,  dc);
		m_pXL->SetValue(stRow+1,    9,  dc);
		m_pXL->SetValue(stRow+0,   15,  dep_s);
		
		m_pXL->SetValue(stRow+2,    6,  dep_s);
		m_pXL->SetValue(stRow+2,    9,  dEs);
		m_pXL->SetValue(stRow+2,   13,  dfs_cal);
		m_pXL->SetValue(stRow+2,   19,  dfy);
		m_pXL->SetValue(stRow+3,    5,  dfs);
		
		m_pXL->SetValue(stRow+4,    6,  dAst);  
		m_pXL->SetValue(stRow+4,   10,  dfs);  
		m_pXL->SetValue(stRow+4,   14,  dTs *m_dFactorForce);
		stRow += RowCnt+1;
	}
	
}


void CPrtKSCE_USD10::Print_BdcyDesign8(T_BDCY_KSCE_BASE& BdcrKsce, PRT_BDCY_PARA& BdcyParam,  CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	double dCc     = BdcyParam.dCc; 
	double dc      = BdcyParam.dNeuC;
	double da      = BdcrKsce.da;
	double da_cent = BdcyParam.daCent;
	double dMps    = BdcyParam.dMps;
	double dTs     = BdcyParam.dTst;
	double ddst    = BdcyParam.dd;
	double dMn     = BdcyParam.dMn_Cal;
	double dMu     = BdcyParam.dMu;
	double dpMn    = BdcyParam.dphiMn_Cal  ;
	double dphi   = BdcrKsce.dphi  ;


	m_strChpt.Format(_T("%d."),m_nChapNum); m_nChapNum++;
	strBlock = _T("Print_BdcyDesign8");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt); 

	m_pXL->SetValue(stRow+2,    4,  dCc);  
	m_pXL->SetValue(stRow+2,    9,  dc);
	m_pXL->SetValue(stRow+2,   12,  da);
	m_pXL->SetValue(stRow+2,   15,  da_cent);
	m_pXL->SetValue(stRow+2,   18,  dMps);
	m_pXL->SetValue(stRow+3,    4,  dTs);
	m_pXL->SetValue(stRow+3,    9,  ddst);
	m_pXL->SetValue(stRow+3,   12,  dc);
	m_pXL->SetValue(stRow+3,   16,  dMn *m_dFactorMoment);
	stRow += RowCnt;

	CString strPhi;
	if(dphi<=0.7)        strPhi =_T("(압축지배 단면)");
	else if(dphi>=0.85)  strPhi =_T("(인장지배 단면)");
	else                 strPhi =_T("(변화구간 단면)");

	strBlock = _T("Print_BdcyDesign_Code4_phi");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,   3,  dphi);
	m_pXL->SetValue(stRow+0,   5,  strPhi);
	stRow += RowCnt+1;

	strBlock = _T("Print_BdcyDesign_Code4_05_check");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,   4,  dpMn * m_dFactorMoment);
	m_pXL->SetValue(stRow+0,  13,  dMu  * m_dFactorMoment);
	stRow += RowCnt+1;

}

void CPrtKSCE_USD10::Print_CumsDesign(T_CUMS_KSCE_BASE& CumsKsce, PRT_CUMS_PARA& CumsParam, CMSExcel* pXL)
{
	int RowCnt=0;
	int ColCnt=0;
	int stRow=m_nTotalRow;
	CString strBaseSheet =  _T("전단");
	
	Print_CumsDesign0(CumsKsce, CumsParam, strBaseSheet, stRow);
	Print_CumsDesign3(CumsKsce, CumsParam, strBaseSheet, stRow);
	Print_CumsDesign4(CumsKsce, CumsParam, strBaseSheet, stRow);
	Print_CumsDesign5(CumsKsce, CumsParam, strBaseSheet, stRow);
	Print_CumsDesign6(CumsKsce, CumsParam, strBaseSheet, stRow);
	Print_CumsDesign8(CumsKsce, CumsParam, strBaseSheet, stRow);
	stRow +=1;

	pXL = m_pXL;
	m_nTotalRow = stRow;
	m_nChapNum = 1;
}

void CPrtKSCE_USD10::Print_CumsDesign0(T_CUMS_KSCE_BASE& CumsKsce, PRT_CUMS_PARA& CumsParam, CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	double dfck = CumsParam.dfck; 
	double dbw  = CumsParam.dbw ;   //복부의 폭
	double dd   = CumsParam.ddst;   //콘크리트압축 연단에서 인장측 철근 중심까지의 거리
	double dfy  = CumsParam.dfy ; 
	double dh   = CumsParam.dh  ; 
	double dIy  = CumsParam.dIy ; 
	double dfpu = CumsParam.dfpu; 
	double ddp  = CumsParam.ddp ;   // 압축연단~프리스트레스힘 중심까지 거리
	double dAc  = CumsParam.dAc ;
	double dfpy = CumsParam.dfpy ;
	double dyt  = CumsParam.dyt  ; // 총단면적의 중립축에서 인장 연단까지의 거리
	double dAps = CumsParam.dAps ;

	m_strChpt.Format(_T("%d."),m_nChapNum);  m_nChapNum++;
	strBlock = _T("Design_Condition_shear");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt);
	m_pXL->SetValue(stRow+1,  4,  dfck);
	m_pXL->SetValue(stRow+2,  4,  dfy);
	m_pXL->SetValue(stRow+3,  4,  dfpu);
	m_pXL->SetValue(stRow+4,  4,  dfpy);
	m_pXL->SetValue(stRow+5,  4,  dbw);
	m_pXL->SetValue(stRow+6,  4,  dh);
	m_pXL->SetValue(stRow+7,  4,  ddp);
	m_pXL->SetValue(stRow+8,  4,  dd);     
	m_pXL->SetValue(stRow+9,  4,  dyt);
	m_pXL->SetValue(stRow+10,  4,  dIy);
	m_pXL->SetValue(stRow+11,  4,  dAc);
	m_pXL->SetValue(stRow+12,  4,  dAps);
 
	stRow += RowCnt+1;

}

void CPrtKSCE_USD10::Print_CumsDesign3(T_CUMS_KSCE_BASE& CumsKsce, PRT_CUMS_PARA& CumsParam, CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	double dMu  = CumsParam.dMu;
	double dVu  = CumsKsce.dVu;     //면내 계수전단력	
	double dMd  = CumsKsce.dMd;    //하중계수를 고려하지 않은 고정하중 모멘트	   	     [Unit=Moment]
	double dVd  = CumsKsce.dVd ;  //사용 고정하중에 의해 발생하는 단면의 전단력			[Unit=Force]
	double dfd  = CumsKsce.dfd;   //외부 작용하중에 의해 인장응력이 발생하는 단면의 최외연에서의 사용 고정하중에의한 응력		 [Unit=Stress]
	double dPe  = CumsParam.dPe;
	double dfpe = CumsKsce.dfpe;  //손실이 일어난 후 강재의 유효프리스트레스			[Unit=Stress]


	m_strChpt.Format(_T("%d."),m_nChapNum);  m_nChapNum++;
	strBlock = _T("Print_CumsDesign3");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt);
	m_pXL->SetValue(stRow+1,  9,  dMu * m_dFactorMoment);
	m_pXL->SetValue(stRow+1, 22,  dPe * m_dFactorForce );
	m_pXL->SetValue(stRow+2,  9,  dVu * m_dFactorForce);
	m_pXL->SetValue(stRow+2, 22,  dfpe);

	stRow += RowCnt+1;

}

void CPrtKSCE_USD10::Print_CumsDesign4(T_CUMS_KSCE_BASE& CumsKsce, PRT_CUMS_PARA& CumsParam, CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	double dH      = CumsParam.dh;
	double d08h    = 0.8*dH;
	double dd      = CumsKsce.dd;  //콘크리트압축 연단에서 인장측 철근 중심까지의 거리
	double ddp     = CumsParam.ddp;
	double dd_org  = CumsParam.ddst;
	double ddp_org = CumsParam.ddp;
	double dIy     = CumsParam.dIy;
	double dyt     = CumsParam.dyt;
	double dfck    = CumsParam.dfck;
	double dfpe    = CumsKsce.dfpe;  //손실이 일어난 후 강재의 유효프리스트레스
	double dfd     = CumsKsce.dfd;   //외부 작용하중에 의해 인장응력이 발생하는 단면의 최외연에서의 사용 고정하중에의한 응력
	double dMcr    = CumsKsce.dMcr;  //외부 작용하중에 의한 단면의 휨 균열모멘트	

	m_strChpt.Format(_T("%d."),m_nChapNum);  m_nChapNum++;
	strBlock = _T("Title_Print_CumsDesign4");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt);
	stRow += RowCnt;

	strBlock = _T("Print_CumsDesign4");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,   4,  d08h);
	m_pXL->SetValue(stRow+0,  11,  dd_org);
	m_pXL->SetValue(stRow+0,  19,  dd);
	stRow += RowCnt+1;

	strBlock = _T("Print_CumsDesign4_01");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,   8,  dIy);
	m_pXL->SetValue(stRow+0,  12,  dyt);
	m_pXL->SetValue(stRow+0,  17,  dfck);
	m_pXL->SetValue(stRow+0,  20,  dfpe);
	m_pXL->SetValue(stRow+1,   8,  dMcr * m_dFactorMoment);
	stRow += RowCnt+1;
}

void CPrtKSCE_USD10::Print_CumsDesign4_torsion(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	CString strBaseSheet_temp = strBaseSheet;
	strBaseSheet = _T("전단");   // 복사할 sheet이름

	double dH      = CrmtParam.dh;
	double d08h    = 0.8*dH;
	double dd      = CrmtParam.dd;  //콘크리트압축 연단에서 인장측 철근 중심까지의 거리
	double ddp     = CrmtParam.ddp;
	double dd_org  = CrmtParam.dd_org;
	double dIy     = CrmtParam.dIy;
	double dyt     = CrmtParam.dyt;
	double dfck    = CrmtParam.dfck;
	double dfpe    = CrmtParam.dfpe;  //손실이 일어난 후 강재의 유효프리스트레스
	double dfd     = CrmtParam.dfd;   //외부 작용하중에 의해 인장응력이 발생하는 단면의 최외연에서의 사용 고정하중에의한 응력
	double dMcr    = CrmtParam.dMcr;  //외부 작용하중에 의한 단면의 휨 균열모멘트	


	m_strChpt.Format(_T("%d."),m_nChapNum);  m_nChapNum++;
	strBlock = _T("Title_Print_CumsDesign4");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt);
	stRow += RowCnt;
	
	strBlock = _T("Print_CumsDesign4");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,   4,  d08h);
	m_pXL->SetValue(stRow+0,  11,  dd_org);
	m_pXL->SetValue(stRow+0,  19,  dd);
	stRow += RowCnt+1;
	
	strBlock = _T("Print_CumsDesign4_01");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,   8,  dIy);
	m_pXL->SetValue(stRow+0,  12,  dyt);
	m_pXL->SetValue(stRow+0,  17,  dfck);
	m_pXL->SetValue(stRow+0,  20,  dfpe);
	m_pXL->SetValue(stRow+1,   8,  dMcr * m_dFactorMoment);
	stRow += RowCnt+1;

	strBaseSheet = strBaseSheet_temp;

}

void CPrtKSCE_USD10::Print_CumsDesign5(T_CUMS_KSCE_BASE& CumsKsce, PRT_CUMS_PARA& CumsParam, CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	double dMu       = CumsParam.dMu;
	double dMd       = CumsKsce.dMd;
	double dMmax     = CumsKsce.dMmax;
	double dVu       = CumsKsce.dVu ;
	double dVd       = CumsKsce.dVd ;
	double dVi       = CumsKsce.dVi;
	double dVci_cal  = CumsParam.dVci_cal;
	double dVci      = CumsKsce.dVci;
	double dVci_incl = CumsParam.dVci_incl;
	double dVci_abs  = fabs(dVci_incl);
	double dfck      = CumsParam.dfck;
	double dbw       = CumsParam.dbw;  //복부의 폭
	double dd        = CumsKsce.dd;   //콘크리트압축 연단에서 인장측 철근 중심까지의 거리
	double ddp       = CumsParam.ddp; // org
	double dMcr      = CumsKsce.dMcr; //외부 작용하중에 의한 단면의 휨 균열모멘트	
	double dVci_min  = CumsParam.dVci_min;
 
	strBlock = _T("Print_CumsDesign5_01"); // 자중 고려
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  13,  dfck);
	m_pXL->SetValue(stRow+0,  16,  dbw);
	m_pXL->SetValue(stRow+0,  19,  dd);
	m_pXL->SetValue(stRow+2,  11,  dVu);
	m_pXL->SetValue(stRow+2,  16,  dMcr);
	m_pXL->SetValue(stRow+3,  11,  dMu);
	m_pXL->SetValue(stRow+2,  21,  dVci_cal * m_dFactorForce);

	m_pXL->SetValue(stRow+5,  11,  dfck);
	m_pXL->SetValue(stRow+5,  14,  dbw);
	m_pXL->SetValue(stRow+5,  17,  dd);
	m_pXL->SetValue(stRow+5,  20,  dVci_min * m_dFactorForce);
	m_pXL->SetValue(stRow+7,   9,  dVci* m_dFactorForce);

	
	stRow += RowCnt+1;


	// 자중고려 안함.
//   m_pXL->SetValue(stRow+0,   6,  dVu * m_dFactorForce);
//   m_pXL->SetValue(stRow+0,  10,  dVd * m_dFactorForce);
//   m_pXL->SetValue(stRow+0,  14,  dVi * m_dFactorForce);
// 
//   m_pXL->SetValue(stRow+1,  13,  dfck);
//   m_pXL->SetValue(stRow+1,  16,  dbw);
//   m_pXL->SetValue(stRow+1,  19,  dd);
//   m_pXL->SetValue(stRow+1,  22,  dVd);
//   m_pXL->SetValue(stRow+3,  11,  dVu);
//   m_pXL->SetValue(stRow+3,  16,  dMcr);
//   m_pXL->SetValue(stRow+4,  11,  dMu);
//   m_pXL->SetValue(stRow+3,  21,  dVci_cal * m_dFactorForce);
//   m_pXL->SetValue(stRow+6,  11,  dfck);
//   m_pXL->SetValue(stRow+6,  14,  dbw);
//   m_pXL->SetValue(stRow+6,  17,  dd);
//   m_pXL->SetValue(stRow+6,  20,  dVci_min * m_dFactorForce);
//   m_pXL->SetValue(stRow+8,   9,  dVci* m_dFactorForce);


}

void CPrtKSCE_USD10::Print_CumsDesign5_torsion(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	CString strBaseSheet_temp = strBaseSheet;
	strBaseSheet = _T("전단");   // 복사할 sheet이름
	
	double dMu       = CrmtParam.dMu;
	double dMd       = CrmtParam.dMd;
	double dMmax     = CrmtParam.dMmax;
	double dVu       = CrmtKsce.dVu ;
	double dVd       = CrmtParam.dVd ;
	double dVi       = CrmtParam.dVi;
	double dVci_cal  = CrmtParam.dVci_cal;
	double dVci      = CrmtParam.dVci;
	double dVci_incl = CrmtParam.dVci_incl;
	double dVci_abs  = fabs(dVci_incl);
	double dfck      = CrmtParam.dfck;
	double dbw       = CrmtParam.dbw;  //복부의 폭
	double dd        = CrmtParam.dd;   //콘크리트압축 연단에서 인장측 철근 중심까지의 거리
	double ddp       = CrmtParam.ddp; // org
	double dMcr      = CrmtParam.dMcr; //외부 작용하중에 의한 단면의 휨 균열모멘트	
	double dVci_min  = CrmtParam.dVci_min;
	
	strBlock = _T("Print_CumsDesign5_01"); // 자중 고려
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  13,  dfck);
	m_pXL->SetValue(stRow+0,  16,  dbw);
	m_pXL->SetValue(stRow+0,  19,  dd);
	m_pXL->SetValue(stRow+2,  11,  dVu);
	m_pXL->SetValue(stRow+2,  16,  dMcr);
	m_pXL->SetValue(stRow+3,  11,  dMu);
	m_pXL->SetValue(stRow+2,  21,  dVci_cal * m_dFactorForce);
	
	m_pXL->SetValue(stRow+5,  11,  dfck);
	m_pXL->SetValue(stRow+5,  14,  dbw);
	m_pXL->SetValue(stRow+5,  17,  dd);
	m_pXL->SetValue(stRow+5,  20,  dVci_min * m_dFactorForce);
	m_pXL->SetValue(stRow+7,   9,  dVci* m_dFactorForce);
	
	stRow += RowCnt+1;
	
	strBaseSheet = strBaseSheet_temp;

	// 자중고려 안함.
	//   m_pXL->SetValue(stRow+0,   6,  dVu * m_dFactorForce);
	//   m_pXL->SetValue(stRow+0,  10,  dVd * m_dFactorForce);
	//   m_pXL->SetValue(stRow+0,  14,  dVi * m_dFactorForce);
	// 
	//   m_pXL->SetValue(stRow+1,  13,  dfck);
	//   m_pXL->SetValue(stRow+1,  16,  dbw);
	//   m_pXL->SetValue(stRow+1,  19,  dd);
	//   m_pXL->SetValue(stRow+1,  22,  dVd);
	//   m_pXL->SetValue(stRow+3,  11,  dVu);
	//   m_pXL->SetValue(stRow+3,  16,  dMcr);
	//   m_pXL->SetValue(stRow+4,  11,  dMu);
	//   m_pXL->SetValue(stRow+3,  21,  dVci_cal * m_dFactorForce);
	//   m_pXL->SetValue(stRow+6,  11,  dfck);
	//   m_pXL->SetValue(stRow+6,  14,  dbw);
	//   m_pXL->SetValue(stRow+6,  17,  dd);
	//   m_pXL->SetValue(stRow+6,  20,  dVci_min * m_dFactorForce);
	//   m_pXL->SetValue(stRow+8,   9,  dVci* m_dFactorForce);
	
}

void CPrtKSCE_USD10::Print_CumsDesign6(T_CUMS_KSCE_BASE& CumsKsce, PRT_CUMS_PARA& CumsParam, CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	double dPe  = CumsParam.dPe;
	double dAc  = CumsParam.dAc;
	double dfpc = CumsKsce.dfpc;   //유효프리스트레스 힘으로 인한 콘크리트 도심에서의 압축응력	
	double dfck = CumsParam.dfck;
	double dbw       = CumsParam.dbw;  //복부의 폭
	double dd        = CumsKsce.dd;   //콘크리트압축 연단에서 인장측 철근 중심까지의 거리
	double ddp       = CumsParam.ddp; // org
	double dVp  = CumsKsce.dVp;    //단면의 유효프리스트레스 힘의 수직 분력
	double dVcw = CumsKsce.dVcw;  //복부의 과도한 주인장응력으로 인해 사인장균열이 발생할 때 콘크리트가 부담하는 공칭 전단강도
	double dVci = CumsKsce.dVci;  //전단과 모멘트의 조합에 의해 사인장균열이 발생할 때 콘크리트가 부담하는 공칭 전단강도  
	double dVc  = CumsKsce.dVc;  //콘크리트가 부담하는 공칭 전단강도

	strBlock = _T("Print_CumsDesign6");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,   7,  dPe);
	m_pXL->SetValue(stRow+0,  11,  dAc);
	m_pXL->SetValue(stRow+0,  15,  dfpc);
	m_pXL->SetValue(stRow+3,   6,  dfck);
	m_pXL->SetValue(stRow+3,  11,  dfpc);
	m_pXL->SetValue(stRow+3,  14,  dbw);
	m_pXL->SetValue(stRow+3,  17,  dd);
	m_pXL->SetValue(stRow+3,  20,  dVp);
	m_pXL->SetValue(stRow+4,   4,  dVcw * m_dFactorForce);
	m_pXL->SetValue(stRow+6,   9,  dVc * m_dFactorForce);

	stRow += RowCnt+1;

}

void CPrtKSCE_USD10::Print_CumsDesign6_torsion(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;
	
	CString strBaseSheet_temp = strBaseSheet;
	strBaseSheet = _T("전단");   // 복사할 sheet이름

	double dPe  = CrmtParam.dPe;
	double dAc  = CrmtParam.dAc;
	double dfpc = CrmtKsce.dfpc;   //유효프리스트레스 힘으로 인한 콘크리트 도심에서의 압축응력	
	double dfck = CrmtParam.dfck;
	double dbw  = CrmtParam.dbw;  //복부의 폭
	double dd   = CrmtParam.dd;   //콘크리트압축 연단에서 인장측 철근 중심까지의 거리
	double ddp  = CrmtParam.ddp; // org
	double dVp  = CrmtParam.dVp;    //단면의 유효프리스트레스 힘의 수직 분력
	double dVcw = CrmtParam.dVcw;  //복부의 과도한 주인장응력으로 인해 사인장균열이 발생할 때 콘크리트가 부담하는 공칭 전단강도
	double dVci = CrmtParam.dVci;  //전단과 모멘트의 조합에 의해 사인장균열이 발생할 때 콘크리트가 부담하는 공칭 전단강도  
	double dVc  = CrmtKsce.dVc;  //콘크리트가 부담하는 공칭 전단강도
	
	strBlock = _T("Print_CumsDesign6");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,   7,  dPe);
	m_pXL->SetValue(stRow+0,  11,  dAc);
	m_pXL->SetValue(stRow+0,  15,  dfpc);
	m_pXL->SetValue(stRow+3,   6,  dfck);
	m_pXL->SetValue(stRow+3,  11,  dfpc);
	m_pXL->SetValue(stRow+3,  14,  dbw);
	m_pXL->SetValue(stRow+3,  17,  dd);
	m_pXL->SetValue(stRow+3,  20,  dVp);
	m_pXL->SetValue(stRow+4,   4,  dVcw * m_dFactorForce);
	m_pXL->SetValue(stRow+6,   9,  dVc * m_dFactorForce);
	
	stRow += RowCnt+1;

	strBaseSheet = strBaseSheet_temp;
 
}

void CPrtKSCE_USD10::Print_CumsDesign8(T_CUMS_KSCE_BASE& CumsKsce, PRT_CUMS_PARA& CumsParam, CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	double dVu     = CumsKsce.dVu;
	double dVc     = CumsKsce.dVc;
	double dphiv   = CumsParam.dphiv;
	double dpVc    = CumsParam.dpVc;
	double d05pVc  = CumsParam.d05pVc;
	double dpVn    = CumsKsce.dpVn;
	double dfck    = CumsParam.dfck;
	double dbw     = CumsParam.dbw;  //복부의 폭
	double dd      = CumsKsce.dd;   //콘크리트압축 연단에서 인장측 철근 중심까지의 거리
	double dVs_req = CumsParam.dVs_req;
	double dVs_min = CumsParam.dVs_min;
	double dVs_max = CumsParam.dVsmax;
	double dVs_cal = CumsParam.dVs_cal;
	
	double dVs     = CumsKsce.dVs;
	double dsv     = CumsKsce.dsv;  //경사인장철근의 간격
	double dfy     = CumsParam.dfy;
	double dThetaV = CumsKsce.dThetaV;
	double dAv_use = CumsKsce.dAv;  //간격 s 이내에 있는 전단철근의 단면적
	double dAv_req = CumsKsce.dAvreq;	
	double dAvmin  = CumsKsce.dAvmin;
	double dAvmin01  = CumsParam.dAvmin01;
	double dAvmin02  = CumsParam.dAvmin02;

	m_strChpt.Format(_T("%d."),m_nChapNum);  m_nChapNum++;
	strBlock = _T("Title_Print_CumsDesign8");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt);
	stRow += RowCnt;

	strBlock = _T("Print_CumsDesign8_01");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+1,   9,  dAv_use);
	m_pXL->SetValue(stRow+1,  12,  dfy);
	m_pXL->SetValue(stRow+1,  16,  dThetaV * RADTODEG);
	m_pXL->SetValue(stRow+1,  20,  dThetaV * RADTODEG);
	m_pXL->SetValue(stRow+1,  23,  dd);
	m_pXL->SetValue(stRow+2,   9,  dsv);
	m_pXL->SetValue(stRow+3,   4,  dVs_cal * m_dFactorForce);
	m_pXL->SetValue(stRow+5,   9,  dfck);
	m_pXL->SetValue(stRow+5,  12,  dbw);
	m_pXL->SetValue(stRow+5,  15,  dd);
	m_pXL->SetValue(stRow+5,  18,  dVs_max * m_dFactorForce);
	m_pXL->SetValue(stRow+7,   9,  dVs * m_dFactorForce);

	stRow += RowCnt+1;

	strBlock = _T("Print_CumsDesign8_02");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+1,   12,  dfck);
	m_pXL->SetValue(stRow+1,   15,  dbw);
	m_pXL->SetValue(stRow+1,   18,  dsv);
	m_pXL->SetValue(stRow+2,   15,  dfy);
	m_pXL->SetValue(stRow+1,   21,  dAvmin01);
	m_pXL->SetValue(stRow+3,    9,  dbw);
	m_pXL->SetValue(stRow+3,   12,  dsv);
	m_pXL->SetValue(stRow+4,    9,  dfy);
	m_pXL->SetValue(stRow+3,   15,  dAvmin02);
	m_pXL->SetValue(stRow+5,   10,  dAvmin);
	stRow += RowCnt+1;

	CString srtShearRebar =_T("");
	if(fabs(dVu) <= d05pVc)    srtShearRebar = _T("전단철근 보강 불필요");
	else if(fabs(dVu) > dpVc)  srtShearRebar = _T("전단철근 보강");
	else  srtShearRebar = _T("최소 전단철근 보강");

	strBlock = _T("Print_CumsDesign8_03");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+2,   1,  d05pVc * m_dFactorForce);
	m_pXL->SetValue(stRow+2,   6,  dpVc   * m_dFactorForce);
	m_pXL->SetValue(stRow+2,  11,  dVu    * m_dFactorForce);
	m_pXL->SetValue(stRow+2,  16,  srtShearRebar);
	stRow += RowCnt+1;

	if(fabs(dVu) > dpVc)
	{
		strBlock = _T("Print_CumsDesign8_04");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		m_pXL->SetValue(stRow+1,   8,  dVu  * m_dFactorForce);
		m_pXL->SetValue(stRow+1,  12,  dpVc * m_dFactorForce);
		m_pXL->SetValue(stRow+1,  16,  dphiv);
		m_pXL->SetValue(stRow+1,  19,  dVs_req * m_dFactorForce);
		m_pXL->SetValue(stRow+2,  10,  dVs_req);
		m_pXL->SetValue(stRow+2,  15,  dsv);
		m_pXL->SetValue(stRow+3,   8,  dfy);
		m_pXL->SetValue(stRow+3,  12,  dThetaV * RADTODEG);
		m_pXL->SetValue(stRow+3,  16,  dThetaV * RADTODEG);
		m_pXL->SetValue(stRow+3,  19,  dd);
		m_pXL->SetValue(stRow+2,  22,  dAv_req);
		stRow += RowCnt+1;
	}

	strBlock = _T("Print_CumsDesign8_05");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+1,  7,  dpVn * m_dFactorForce);
	m_pXL->SetValue(stRow+1, 15,  dVu  * m_dFactorForce);
	stRow += RowCnt+1;  
}


void CPrtKSCE_USD10::Print_CrmtDesign(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CMSExcel* pXL)  
{
	int RowCnt=0;
	int ColCnt=0;
	int stRow=m_nTotalRow;
	CString strBaseSheet =  _T("비틀림");
	CString strBlock =_T("");
	
	Print_CrmtDesign0(CrmtKsce, CrmtParam, strBaseSheet, stRow);
	Print_CrmtDesign3(CrmtKsce, CrmtParam, strBaseSheet, stRow);
	Print_CrmtDesign4(CrmtKsce, CrmtParam, strBaseSheet, stRow);

	if(CrmtParam.bTorsionChk)
	{
		Print_CumsDesign4_torsion(CrmtKsce, CrmtParam, strBaseSheet, stRow);
		Print_CumsDesign5_torsion(CrmtKsce, CrmtParam, strBaseSheet, stRow);
		Print_CumsDesign6_torsion(CrmtKsce, CrmtParam, strBaseSheet, stRow);

		Print_CrmtDesign6(CrmtKsce, CrmtParam, strBaseSheet, stRow);
		Print_CrmtDesign7(CrmtKsce, CrmtParam, strBaseSheet, stRow);
		Print_CrmtDesign8(CrmtKsce, CrmtParam, strBaseSheet, stRow);
	 
	}
	stRow +=1;
	
	pXL = m_pXL;
	m_nTotalRow = stRow;
	m_nChapNum = 1;
}


void CPrtKSCE_USD10::Print_CrmtDesign0(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	double dfck = CrmtParam.dfck ;
	double dbw  = CrmtParam.dbw  ;
	double dt   = CrmtParam.dt   ;
	double dfy  = CrmtParam.dfy  ;
	double dh   = CrmtParam.dh   ;
	double dIy  = CrmtParam.dIy  ;
	double dfpu = CrmtParam.dfpu ;
	double dd   = CrmtParam.dd   ;
	double ddp  = CrmtParam.ddp  ;
	double dAc  = CrmtParam.dAc  ;
	double dfpy = CrmtParam.dfpy ;
	double dyt  = CrmtParam.dyt  ;
	double dAps = CrmtParam.dAps ;
	double dPcp = CrmtParam.dPcp ;
	double dAcp = CrmtParam.dAcp ;

	m_strChpt.Format(_T("%d."),m_nChapNum);  m_nChapNum++;
	strBlock = _T("Design_Condition_Torsion");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt);
	m_pXL->SetValue(stRow+1,  4,  dfck);
	m_pXL->SetValue(stRow+2,  4,  dfy);
	m_pXL->SetValue(stRow+3,  4,  dfpu);
	m_pXL->SetValue(stRow+4,  4,  dfpy);
	m_pXL->SetValue(stRow+5,  4,  dbw);
	m_pXL->SetValue(stRow+6,  4,  dh);
	m_pXL->SetValue(stRow+7,  4,  dd);     
	m_pXL->SetValue(stRow+8,   4,  dyt);
	m_pXL->SetValue(stRow+9,   4,  dPcp);
	m_pXL->SetValue(stRow+10,  4,  dt);
	m_pXL->SetValue(stRow+11,  4,  dIy);
	m_pXL->SetValue(stRow+12,  4,  dAc);
	m_pXL->SetValue(stRow+13,  4,  dAps);
	m_pXL->SetValue(stRow+14,  4,  dAcp);
	stRow += RowCnt+1;  
}

void CPrtKSCE_USD10::Print_CrmtDesign3(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	double dMu  = CrmtParam.dMu;
	double dTu  = CrmtKsce.dTu;
	double dVu  = CrmtKsce.dVu;
	double dMd  = CrmtParam.dMd;
	double dVd  = CrmtParam.dVd;
	double dfd  = CrmtParam.dfd;
	double dPe  = CrmtParam.dPe;
	double dfpe = CrmtParam.dfpe;

	m_strChpt.Format(_T("%d."),m_nChapNum);  m_nChapNum++;
	strBlock = _T("Print_CrmtDesign3");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt);
	m_pXL->SetValue(stRow+1,  9,  dMu * m_dFactorMoment);
	m_pXL->SetValue(stRow+1, 22,  dPe * m_dFactorForce );
	m_pXL->SetValue(stRow+2,  9,  dTu * m_dFactorMoment);
	m_pXL->SetValue(stRow+2, 22,  dfpe);
	m_pXL->SetValue(stRow+3,  9,  dVu * m_dFactorForce);
	
	stRow += RowCnt+1;

}



void CPrtKSCE_USD10::Print_CrmtDesign4(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	double dfck = CrmtParam.dfck;
	double dAcp =  CrmtParam.dAcp;
	double dPcp = CrmtParam.dPcp;
	double dfpc = CrmtKsce.dfpc;
	double dTcr = CrmtKsce.dTcr; // = 1/4 Tcr
	double dTu  = CrmtKsce.dTu;
	double dfpe = CrmtParam.dfpe;
	double dphit = CrmtParam.dphit;

	m_strChpt.Format(_T("%d."),m_nChapNum);  m_nChapNum++;
	strBlock = _T("Print_CrmtDesign4");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt);
	m_pXL->SetValue(stRow+4,  3,  dphit);
	m_pXL->SetValue(stRow+4,  6,  dfck);
	m_pXL->SetValue(stRow+4,  9,  dAcp);
	m_pXL->SetValue(stRow+5,  9,  dPcp);
	m_pXL->SetValue(stRow+4, 15,  dfpc);
	m_pXL->SetValue(stRow+5, 17,  dfck);
	m_pXL->SetValue(stRow+6,  3,  dTcr * m_dFactorMoment);
	m_pXL->SetValue(stRow+6, 12,  dTu * m_dFactorMoment);
	stRow += RowCnt+1;

}

void CPrtKSCE_USD10::Print_CrmtDesign6(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	double dAohOverPh = CrmtParam.dAohOverPh;
	double dVu        = CrmtKsce.dVu;
	double dVc        = CrmtKsce.dVc;
	double dbw        = CrmtParam.dbw;
	double dd         = CrmtParam.dd;
	double ddp        = CrmtParam.ddp;
	double dTu        = CrmtKsce.dTu;
	double dfck       = CrmtParam.dfck;
	
	double dPh        = CrmtKsce.dPh;
	double dAoh       = CrmtKsce.dAoh;
	double dt         = CrmtParam.dt;
	double dvVT       = CrmtKsce.dvVT;    //전단력과 비틀림에 의한 전단응력
	double dvVTmax    = CrmtKsce.dvVTmax; //전단균열을 유발하는 응력과 제한응력의 합
	double dphit      = CrmtParam.dphit;

	m_strChpt.Format(_T("%d."),m_nChapNum);  m_nChapNum++;
	strBlock = _T("Print_CrmtDesign6");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt);
	stRow += RowCnt;

	if(!CrmtParam.bClosedSect)
	{
		strBlock = _T("Print_CrmtDesign6_01");
		m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
		m_pXL->SetValue(stRow+1,   9,  dVu);
		m_pXL->SetValue(stRow+2,   9,  dbw);
		m_pXL->SetValue(stRow+2,  12,  dd);
		m_pXL->SetValue(stRow+1,  15,  dTu);
		m_pXL->SetValue(stRow+1,  19,  dPh);
		m_pXL->SetValue(stRow+2,  18,  dAoh);
		m_pXL->SetValue(stRow+1,  24,  dvVT);

		m_pXL->SetValue(stRow+4,   8,  dphit);
		m_pXL->SetValue(stRow+4,  11,  dVc);
		m_pXL->SetValue(stRow+5,  11,  dbw);
		m_pXL->SetValue(stRow+5,  14,  dd);
		m_pXL->SetValue(stRow+4,  19,  dfck);
		m_pXL->SetValue(stRow+4,  22,  dvVTmax);
		stRow += RowCnt+1;
	}
	else
	{
		if(dt >= dAohOverPh)
		{
			strBlock = _T("Print_CrmtDesign6_02_a");
			m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
			m_pXL->SetValue(stRow+1,   4,  dt);
			m_pXL->SetValue(stRow+1,  13,  dAohOverPh);
			m_pXL->SetValue(stRow+2,   7,  dVu);
			m_pXL->SetValue(stRow+3,   7,  dbw);
			m_pXL->SetValue(stRow+3,  10,  dd);
			m_pXL->SetValue(stRow+2,  13,  dTu);
			m_pXL->SetValue(stRow+2,  17,  dPh);
			m_pXL->SetValue(stRow+3,  16,  dAoh);
			m_pXL->SetValue(stRow+2,  21,  dvVT);
 
			m_pXL->SetValue(stRow+5,   8,  dphit);
			m_pXL->SetValue(stRow+5,  11,  dVc);
			m_pXL->SetValue(stRow+6,  11,  dbw);
			m_pXL->SetValue(stRow+6,  13,  dd);
			m_pXL->SetValue(stRow+5,  19,  dfck);
			m_pXL->SetValue(stRow+5,  22,  dvVTmax);
			stRow += RowCnt+1;
		}
		else
		{
			strBlock = _T("Print_CrmtDesign6_02_a");
			m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);

			m_pXL->SetValue(stRow+1,   4,  dt);
			m_pXL->SetValue(stRow+1,  13,  dAohOverPh);
			m_pXL->SetValue(stRow+2,   7,  dVu);
			m_pXL->SetValue(stRow+3,   7,  dbw);
			m_pXL->SetValue(stRow+3,  10,  dd);
			m_pXL->SetValue(stRow+2,  13,  dTu);
			m_pXL->SetValue(stRow+2,  17,  dPh);
			m_pXL->SetValue(stRow+3,  15,  dAoh);
			m_pXL->SetValue(stRow+3,  19,  dt);
			m_pXL->SetValue(stRow+2,  22,  dvVT);

			m_pXL->SetValue(stRow+5,   8,  dphit);
			m_pXL->SetValue(stRow+5,  11,  dVc);
			m_pXL->SetValue(stRow+6,  11,  dbw);
			m_pXL->SetValue(stRow+6,  13,  dd);
			m_pXL->SetValue(stRow+5,  19,  dfck);
			m_pXL->SetValue(stRow+5,  22,  dvVTmax);
			stRow += RowCnt+1;
		}
	}
}


void CPrtKSCE_USD10::Print_CrmtDesign7(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	double dAoh   = CrmtKsce.dAoh;
	double dAo    = CrmtParam.dA0;
	double dAt    = CrmtKsce.dAt;
	double dfyv   = CrmtKsce.dfyv;
	double dst    = CrmtParam.dst;
	double dThetaC= CrmtKsce.dThetaC;
	double dpTn   = CrmtKsce.dpTn;
	double dfyl   = CrmtKsce.dfyl;
	double dPh    = CrmtKsce.dPh;
	double dAlmin_cal = CrmtParam.dAlmin_cal;
	double dAlmin = CrmtKsce.dAlmin;
	double dTn    = CrmtParam.dTn;
	double dphit  = CrmtParam.dphit;
	double dphiTn = dphit*dTn;
	double dTu    = CrmtKsce.dTu;
	double dAlreq = CrmtKsce.dAlreq;
	double dAluse = CrmtKsce.dAluse;

	m_strChpt.Format(_T("%d."),m_nChapNum);  m_nChapNum++;
	strBlock = _T("Print_CrmtDesign7");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt);
	m_pXL->SetValue(stRow+2,   8,  dAoh);
	m_pXL->SetValue(stRow+2,  13,  dAo);
	m_pXL->SetValue(stRow+4,   9,  dAo);
	m_pXL->SetValue(stRow+4,  13,  dAt);
	m_pXL->SetValue(stRow+4,  17,  dfyv);
	m_pXL->SetValue(stRow+5,  12,  dst);
	m_pXL->SetValue(stRow+4,  22,  dThetaC * RADTODEG);
	m_pXL->SetValue(stRow+6,   8,  dTn    * m_dFactorMoment);
	m_pXL->SetValue(stRow+7,   4,  dphit);
	m_pXL->SetValue(stRow+8,   4,  dphiTn * m_dFactorMoment);
	m_pXL->SetValue(stRow+8,  13,  dTu    * m_dFactorMoment);

	m_pXL->SetValue(stRow+11,    8,  dAt);
	m_pXL->SetValue(stRow+12,    8,  dst);
	m_pXL->SetValue(stRow+11,   12,  dPh);
	m_pXL->SetValue(stRow+11,   16,  dfyv);
	m_pXL->SetValue(stRow+12,   16,  dfyl);
	m_pXL->SetValue(stRow+11 ,  21,  dThetaC * RADTODEG);
	m_pXL->SetValue(stRow+13 ,   8,  dAlreq);
	m_pXL->SetValue(stRow+14 ,   4,  dAluse);
	m_pXL->SetValue(stRow+14 ,  13,  dAlreq);
	stRow += RowCnt+1;
}


void CPrtKSCE_USD10::Print_CrmtDesign8(T_CRMT_KSCE_BASE& CrmtKsce, PRT_CRMT_PARA& CrmtParam, CString strBaseSheet, int& stRow)
{
	int RowCnt=0;
	int ColCnt=0;
	CString strBlock;

	double dbw     = CrmtParam.dbw;
	double dst     = CrmtParam.dst;
	double dfyv    = CrmtKsce.dfyv;
	double dAv_use = CrmtParam.dAv;
	double dAtmin  = CrmtKsce.dAtmin;
	double dfck    = CrmtParam.dfck;
	double dAcp    = CrmtParam.dAcp; 
	double dAt_used = CrmtKsce.dAt;
	double dPh     = CrmtKsce.dPh;
	double dfyl    = CrmtKsce.dfyl;
	double dAlmin  = CrmtKsce.dAlmin;
	double dAlmin_cal = CrmtParam.dAlmin_cal;
	double dAts_cal= CrmtParam.dAtst;
	double dbwfyv  = CrmtParam.dbwfyv;
	double dAts    = CrmtParam.AtOverS;
	double dAluse  = CrmtKsce.dAluse;
	double dAtmin01   =CrmtParam.dAtmin01  ;
	double dAtmin02   =CrmtParam.dAtmin02  ;
	double dAtmin_cal =CrmtParam.dAtmin_cal;
	double dAv        =CrmtParam.dAv;
	
	m_strChpt.Format(_T("%d."),m_nChapNum);  m_nChapNum++;
	strBlock = _T("Title_Print_CrmtDesign8");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+0,  1,  m_strChpt);
	stRow += RowCnt;

	strBlock = _T("Print_CrmtDesign8_01");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+3,   10,  dfck);
	m_pXL->SetValue(stRow+3,   13,  dbw);
	m_pXL->SetValue(stRow+3,   16,  dst);
	m_pXL->SetValue(stRow+4,   13,  dfyv);
	m_pXL->SetValue(stRow+3,   19,  dAtmin01);
	m_pXL->SetValue(stRow+5,    9,  dbw);
	m_pXL->SetValue(stRow+5,   12,  dst);
	m_pXL->SetValue(stRow+6,    9,  dfyv);
	m_pXL->SetValue(stRow+5,   15,  dAtmin02);
	m_pXL->SetValue(stRow+5,   10,  dAtmin);
	m_pXL->SetValue(stRow+7,    4,  dAtmin_cal);
	m_pXL->SetValue(stRow+7,    9,  dAv);
	m_pXL->SetValue(stRow+7,   15,  dAtmin);  
	m_pXL->SetValue(stRow+8,    4,  dAt_used);
	m_pXL->SetValue(stRow+8,   13,  dAtmin); 
	stRow += RowCnt+1;
	
	strBlock = _T("Print_CrmtDesign8_02");
	m_pXL->CopyRange(stRow, 0, m_strBaseBook, strBaseSheet, strBlock, RowCnt, ColCnt);
	m_pXL->SetValue(stRow+1,   4,  dAt_used);
	m_pXL->SetValue(stRow+1,   8,  dst);
	m_pXL->SetValue(stRow+1,  12,  dAts_cal);
	m_pXL->SetValue(stRow+2,   8,  dbw);
	m_pXL->SetValue(stRow+2,  12,  dfyv);
	m_pXL->SetValue(stRow+2,  16,  dbwfyv);
	m_pXL->SetValue(stRow+3,   5,  dAts_cal);
	m_pXL->SetValue(stRow+3,   9,  dbwfyv);
	m_pXL->SetValue(stRow+3,  13,  dAts);
	m_pXL->SetValue(stRow+7,   7,  dfck);
	m_pXL->SetValue(stRow+7,  10,  dAcp);
	m_pXL->SetValue(stRow+8,   6,  dfyl);
	m_pXL->SetValue(stRow+7,  13,  dAts);
	m_pXL->SetValue(stRow+7,  16,  dPh);
	m_pXL->SetValue(stRow+7,  19,  dfyv);
	m_pXL->SetValue(stRow+8,  19,  dfyl);
	m_pXL->SetValue(stRow+7,  22,  dAlmin);
	m_pXL->SetValue(stRow+9,    4,  dAluse);
	m_pXL->SetValue(stRow+9,   13,  dAlmin); 
	stRow += RowCnt+1;

}



void CPrtKSCE_USD10::SetUnitConvertFactor()
{
	// [Code Unit] : N, mm.
	
	m_dFactorForce            = 0.001;
	m_dFactorLength           = 0.001;
	m_dFactorMoment           = pow(0.001, 2);
	m_dFactorInertiaMoment    = pow(0.001, 4);

//   m_dFactorLengthEach       = (m_bUnitSI)? 1.0 : 0.1;
//   m_dFactorLengthMM         = 1.0;
//   m_dFactorStress           = 1.0;  
//   m_dFactorAreaMM = m_dFactorLengthMM*m_dFactorLengthMM;
//   m_dFactorDensity          = pow(0.001, 2);
//   m_dFactorPressure         = 1000.0;  // N/mm2 => kN/m2
//   
//   // Add Only LRFD.
//   m_dFactorInertiaMomentMM4 = pow(m_dFactorLengthMM , 4);
//   m_dFactorSectConstant     = pow(m_dFactorLength   , 3);
//   m_dFactorSectConstantMM3  = pow(m_dFactorLengthMM , 3);
//   m_dFactorTorsionConst     = pow(m_dFactorLength   , 6);
//   m_dFactorTorsionConstMM6  = pow(m_dFactorLengthMM , 6);
//   
//   m_strUnitForce.Format(_T("0.00 %s"), ((m_bUnitSI)? _T("kN") : _T("tonf")));
//   m_strUnitLength.Format(_T("0.000 %s"), _T("m"));
//   m_strUnitLengthEach.Format(_T("0.00 %s"), ((m_bUnitSI)? _T("mm") : _T("cm")));  
//   m_strUnitLengthMM.Format(_T("0.00000 %s"), _T("mm"));
//   m_strUnitMoment.Format(_T("0.00 %s"), ((m_bUnitSI)? _T("kN·m") : _T("tonf·m")));
//   m_strUnitStress.Format(_T("0.00 %s"), ((m_bUnitSI)? _T("MPa") : _T("kgf/cm²")));  
//   m_strUnitInertiaMoment.Format(_T("0.000000 %s"), _T("m⁴"));
//   
//   m_strNone3.Format(_T("0.000"));
//   m_strNone4.Format(_T("0.0000"));
//   
//   m_strOK_Table = _T("O.K.");
//   m_strNG_Table = _T("N.G.");
}




/*
BOOL CPrtKSCE_USD10::Get_DetailBdcyPos(T_ELEM_K ElemK)
{
	// Get Posx.
	T_POSX_D PosxD;
	PosxD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetPosx(ElemK, PosxD))	return FALSE;
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
	T_BDCY_KSCE_DETAIL BdcyDetail;
	m_pDataCtrl->Get_DgnKsce05Pointer()->Get_KsceBdcy(ElemK,BdcyKsceD, &BdcyDetail, bPrtI,bPrtJ);


//  Priint_Bending();
	return TRUE;
}
*/