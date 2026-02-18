#include "stdafx.h"
#include "BrdgSeisEvalKALIS2023_Report.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#include "..\..\DgnEngine\idesign\DGN_lib\IExcel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::lib;

CBrdgSeisEvalKALIS2023_Report::CBrdgSeisEvalKALIS2023_Report(void) 
	: CBrdgSeisEvalKISTEC2015_Report()
{
}

CBrdgSeisEvalKALIS2023_Report::~CBrdgSeisEvalKALIS2023_Report(void)
{
}

BOOL CBrdgSeisEvalKALIS2023_Report::GenerateEvalPlainConcSheet(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2])
{
	// Plain concrete Pier
	stRow = 1;

	// Head Title
	RptBlock = _T("[2]Title_Sect_PlainConc");
	int rowCnt = CopyRange(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	// 6.1 교각의 휨성능
	GeneratePlainConcFlexCapa(pExcel, RptBlock, stRow, enBehavior, ResultD);

	// 6.2 교각의 축-휨성능
	GeneratePlainConcCombCapa(pExcel, RptBlock, stRow, enBehavior, ResultD);

	// 6.3 교각의 전단성능
	GeneratePlainConcShearCapa(pExcel, RptBlock, stRow, enBehavior, ResultD);

	// 6.4 교각의 내진성능평가
	GeneratePlainConcCapa(pExcel, RptBlock, stRow, enBehavior, ResultD);

	return TRUE;
}

void CBrdgSeisEvalKALIS2023_Report::GeneratePlainConcFlexCapa(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2])
{
	// 6.1 교각의 휨성능
	RptBlock = _T("[2]Seismic_Eval_Plainconc_Pier_Bending_Head");
	auto rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	short Row_Ref = 0, Col_Ref = 0;

	auto l_PrintData = [&](const CString& strBlock, CString strCell[8], const T_BRDGEVAL_RESULT& ResD)
	{
		RptBlock = strBlock;
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		const double dFactS = pow(m_dFactorLength, 3.0);
		if(ResD.EvalD.bCheck)
		{
			PrintCellData(pExcel, strCell[0], ResD.ParamD.MatlD.dfck*m_dFactorStress     , Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[1], ResD.EvalD.PalinConcD.dSm/dFactS  , Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[2], ResD.EvalD.PalinConcD.dMn  , Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[3], ResD.ParamD.MatlD.dfck*m_dFactorStress     , Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[4], ResD.EvalD.PalinConcD.dSm/dFactS  , Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[5], ResD.EvalD.PalinConcD.dMn  , Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[6], ResD.EvalD.PalinConcD.dMd  , Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[7], ResD.EvalD.PalinConcD.dFpcf, Row_Ref, Col_Ref, stRow);
		}
		stRow += rowCnt;
	};

	auto l_GetstrBlock = [](const CString& strTail, CString strBlocks[8])
	{
		strBlocks[0] = _T("S9_Mn_fck");
		strBlocks[1] = _T("S9_Mn_Sm");
		strBlocks[2] = _T("S9_Mn");
		strBlocks[3] = _T("S9_fck");
		strBlocks[4] = _T("S9_Sm");
		strBlocks[5] = _T("S9_FPCF_Mn");
		strBlocks[6] = _T("S9_FPCF_MD");
		strBlocks[7] = _T("S9_FPCF");
		for(auto i = 0; i < 8; i++)	{	strBlocks[i] += strTail;	}
	};
	
	if(enBehavior[0] == CANTILEVER)
	{
		CString strCell[8];	
		l_GetstrBlock(_T("_Longi_T1"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Bending_Cantil_Longi_T1"), strCell, ResultD[0][0]);
	}
	else
	{
		CString strCell[8];	
		l_GetstrBlock(_T("_Longi_T2"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Bending_Rahmen_Bot_Longi_T2"), strCell, ResultD[0][0]);
		l_GetstrBlock(_T("_Longi_T3"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Bending_Rahmen_Top_Longi_T3"), strCell, ResultD[1][0]);
	}

	if(enBehavior[1] == CANTILEVER)
	{
		CString strCell[8];
		l_GetstrBlock(_T("_Trans_T1"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Bending_Cantil_Trans_T1"), strCell, ResultD[0][1]);
	}
	else
	{
		CString strCell[8];
		l_GetstrBlock(_T("_Trans_T2"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Bending_Rahmen_Bot_Trans_T2"), strCell, ResultD[0][1]);

		l_GetstrBlock(_T("_Trans_T3"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Bending_Rahmen_Top_Trans_T3"), strCell, ResultD[1][1]);
	}
	stRow++;
}

void CBrdgSeisEvalKALIS2023_Report::GeneratePlainConcCombCapa(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2])
{
	// 6.2 교각의 축-휨성능
	RptBlock = _T("[2]Seismic_Eval_Plainconc_Pier_Axial_Bending_Head");
	auto rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	short Row_Ref = 0, Col_Ref = 0;

	auto l_PrintCompData = [&](const CString& strBlock, CString strComp[8], const T_BRDGEVAL_RESULT& ResD)
	{
		RptBlock = strBlock;
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if(ResD.EvalD.bCheck)
		{
			const double dRat1 = SafeDiv(ResD.EvalD.PalinConcD.dPu, ResD.EvalD.PalinConcD.dPn);
			const double dRat2 = SafeDiv(ResD.EvalD.PalinConcD.dMd, ResD.EvalD.PalinConcD.dMn);
			PrintCellData(pExcel, strComp[0], (dRat1 + dRat2), Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strComp[1], ResD.EvalD.PalinConcD.dFpcc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strComp[2], ResD.EvalD.PalinConcD.dPu, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strComp[3], ResD.EvalD.PalinConcD.dPn, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strComp[4], dRat1, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strComp[5], ResD.EvalD.PalinConcD.dMd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strComp[6], ResD.EvalD.PalinConcD.dMn, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strComp[7], dRat2, Row_Ref, Col_Ref, stRow);
		}
		stRow += rowCnt;
	};

	auto l_PrintTensData = [&](const CString& strBlock, CString strTens[8], const T_BRDGEVAL_RESULT& ResD)
	{
		RptBlock = strBlock;
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if(ResD.EvalD.bCheck)
		{
			const double dRat1 = SafeDiv(ResD.EvalD.PalinConcD.dMd, ResD.EvalD.PalinConcD.dSm);
			const double dRat2 = SafeDiv(ResD.EvalD.PalinConcD.dPu, ResD.EvalD.PalinConcD.dAg);
			PrintCellData(pExcel, strTens[0], ResD.EvalD.PalinConcD.dNum, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strTens[1], (dRat1 + dRat2), Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strTens[2], ResD.EvalD.PalinConcD.dFpcc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strTens[3], ResD.EvalD.PalinConcD.dMd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strTens[4], ResD.EvalD.PalinConcD.dSm, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strTens[5], dRat1, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strTens[6], ResD.EvalD.PalinConcD.dPu, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strTens[7], ResD.EvalD.PalinConcD.dAg, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strTens[8], dRat2, Row_Ref, Col_Ref, stRow);
		}
		stRow += rowCnt;
	};

	auto l_GetstrBlockComp = [](const CString& strTail, CString strBlocks[8])
	{
		strBlocks[0] = _T("S10_Comp_FPCC_Rat");
		strBlocks[1] = _T("S10_Comp_FPCC");
		strBlocks[2] = _T("S10_Comp_Pu");
		strBlocks[3] = _T("S10_Comp_Pn");
		strBlocks[4] = _T("S10_Comp_P_Rat");
		strBlocks[5] = _T("S10_Comp_MD");
		strBlocks[6] = _T("S10_Comp_Mn");
		strBlocks[7] = _T("S10_Comp_M_Rat");
		for(auto i = 0; i < 8; i++)	{	strBlocks[i] += strTail;	}
	};

	auto l_GetstrBlockTens = [](const CString& strTail, CString strBlocks[9])
	{
		strBlocks[0] = _T("S10_Tens_FPCC_Str1");
		strBlocks[1] = _T("S10_Tens_FPCC_Str2");
		strBlocks[2] = _T("S10_Tens_FPCC");
		strBlocks[3] = _T("S10_Tens_MD");
		strBlocks[4] = _T("S10_Tens_Sm");
		strBlocks[5] = _T("S10_Tens_M_Rat");
		strBlocks[6] = _T("S10_Tens_Pu");
		strBlocks[7] = _T("S10_Tens_Ag");
		strBlocks[8] = _T("S10_Tens_P_Rat");
		for(auto i = 0; i < 9; i++)	{	strBlocks[i] += strTail;	}
	};

	// 교축방향
	if(enBehavior[0] == CANTILEVER)
	{
		if(MT0(ResultD[0][0].EvalD.PalinConcD.dPu)) // compression
		{
			CString strCell[8];
			l_GetstrBlockComp(_T("_Longi_T1"), strCell);
			l_PrintCompData(_T("[2]Seismic_Eval_Plainconc_Comp_Bending_Cantil_Longi_T1"), strCell, ResultD[0][0]);
		}
		else
		{
			CString strCell[9];
			l_GetstrBlockTens(_T("_Longi_T1"), strCell);
			l_PrintTensData(_T("[2]Seismic_Eval_Plainconc_Tens_Bending_Cantil_Longi_T1"), strCell, ResultD[0][0]);
		}
	}
	else // Rahmen
	{
		if(MT0(ResultD[0][0].EvalD.PalinConcD.dPu)) // compression
		{
			CString strCell[8];
			l_GetstrBlockComp(_T("_Longi_T2"), strCell);
			l_PrintCompData(_T("[2]Seismic_Eval_Plainconc_Comp_Bending_Rahmen_Bot_Longi_T2"), strCell, ResultD[0][0]);
		}
		else
		{
			CString strCell[9];
			l_GetstrBlockTens(_T("_Longi_T2"), strCell);
			l_PrintTensData(_T("[2]Seismic_Eval_Plainconc_Tens_Bending_Rahmen_Bot_Longi_T2"), strCell, ResultD[0][0]);
		}

		if(MT0(ResultD[1][0].EvalD.PalinConcD.dPu)) // compression
		{
			CString strCell[8];
			l_GetstrBlockComp(_T("_Longi_T3"), strCell);
			l_PrintCompData(_T("[2]Seismic_Eval_Plainconc_Comp_Bending_Rahmen_Bot_Longi_T3"), strCell, ResultD[1][0]);
		}
		else
		{
			CString strCell[9];
			l_GetstrBlockTens(_T("_Longi_T3"), strCell);
			l_PrintTensData(_T("[2]Seismic_Eval_Plainconc_Tens_Bending_Rahmen_Bot_Longi_T3"), strCell, ResultD[1][0]);
		}
	}

	// 교축직각방향
	if(enBehavior[1] == CANTILEVER)
	{
		if(MT0(ResultD[0][1].EvalD.PalinConcD.dPu)) // compression
		{
			CString strCell[8];
			l_GetstrBlockComp(_T("_Trans_T1"), strCell);
			l_PrintCompData(_T("[2]Seismic_Eval_Plainconc_Comp_Bending_Cantil_Trans_T1"), strCell, ResultD[0][1]);
		}
		else
		{
			CString strCell[9];
			l_GetstrBlockTens(_T("_Trans_T1"), strCell);
			l_PrintTensData(_T("[2]Seismic_Eval_Plainconc_Tens_Bending_Cantil_Trans_T1"), strCell, ResultD[0][1]);
		}
	}
	else // Rahmen
	{
		if(MT0(ResultD[0][1].EvalD.PalinConcD.dPu)) // compression
		{
			CString strCell[8];
			l_GetstrBlockComp(_T("_Trans_T2"), strCell);
			l_PrintCompData(_T("[2]Seismic_Eval_Plainconc_Comp_Bending_Rahmen_Bot_Trans_T2"), strCell, ResultD[0][1]);
		}
		else
		{
			CString strCell[9];
			l_GetstrBlockTens(_T("_Trans_T2"), strCell);
			l_PrintTensData(_T("[2]Seismic_Eval_Plainconc_Tens_Bending_Rahmen_Bot_Trans_T2"), strCell, ResultD[0][1]);
		}

		if(MT0(ResultD[1][1].EvalD.PalinConcD.dPu)) // compression
		{
			CString strCell[8];
			l_GetstrBlockComp(_T("_Trans_T3"), strCell);
			l_PrintCompData(_T("[2]Seismic_Eval_Plainconc_Comp_Bending_Rahmen_Bot_Trans_T3"), strCell, ResultD[1][1]);
		}
		else
		{
			CString strCell[9];
			l_GetstrBlockTens(_T("_Trans_T3"), strCell);
			l_PrintTensData(_T("[2]Seismic_Eval_Plainconc_Tens_Bending_Rahmen_Bot_Trans_T3"), strCell, ResultD[1][1]);
		}
	}

	stRow++;
}

void CBrdgSeisEvalKALIS2023_Report::GeneratePlainConcShearCapa(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2])
{
	// 6.3 교각의 전단성능
	RptBlock = _T("[2]Seismic_Eval_Plainconc_Pier_Shear_Head");
	auto rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	short Row_Ref = 0, Col_Ref = 0;

	auto l_PrintData = [&](const CString& strBlock, CString strCell[8], const T_BRDGEVAL_RESULT& ResD)
	{
		RptBlock = strBlock;
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if(ResD.EvalD.bCheck)
		{
			PrintCellData(pExcel, strCell[0], ResD.ParamD.MatlD.dfck*m_dFactorStress, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[1], ResD.EvalD.PalinConcD.dAg/m_dFactorLength/m_dFactorLength, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[2], ResD.EvalD.PalinConcD.dVn, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[3], ResD.ParamD.MatlD.dfck*m_dFactorStress, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[4], ResD.EvalD.PalinConcD.dAg/m_dFactorLength/m_dFactorLength, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[5], ResD.EvalD.PalinConcD.dVn, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[6], ResD.EvalD.PalinConcD.dVd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[7], ResD.EvalD.PalinConcD.dFpcv, Row_Ref, Col_Ref, stRow);
		}
		stRow += rowCnt;
	};


	auto l_GetstrBlock = [](const CString& strTail, CString strBlocks[8])
	{
		strBlocks[0] = _T("S11_Vn_fck");
		strBlocks[1] = _T("S11_Vn_Ag");
		strBlocks[2] = _T("S11_Vn");
		strBlocks[3] = _T("S11_fck");
		strBlocks[4] = _T("S11_Ag");
		strBlocks[5] = _T("S11_FPCV_Vn");
		strBlocks[6] = _T("S11_FPCV_VD");
		strBlocks[7] = _T("S11_FPCV");
		for(auto i = 0; i < 8; i++)	{	strBlocks[i] += strTail;	}
	};

	if(enBehavior[0] == CANTILEVER)
	{
		CString strCell[8];
		l_GetstrBlock(_T("_Longi_T1"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Shear_Cantil_Longi_T1"), strCell, ResultD[0][0]);
	}
	else
	{
		CString strCell[8];
		l_GetstrBlock(_T("_Longi_T2"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Shear_Rahmen_Bot_Longi_T2"), strCell, ResultD[0][0]);
		l_GetstrBlock(_T("_Longi_T3"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Shear_Rahmen_Top_Longi_T3"), strCell, ResultD[1][0]);
	}

	if(enBehavior[1] == CANTILEVER)
	{
		CString strCell[8];
		l_GetstrBlock(_T("_Trans_T1"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Shear_Cantil_Trans_T1"), strCell, ResultD[0][1]);
	}
	else
	{
		CString strCell[8];
		l_GetstrBlock(_T("_Trans_T2"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Shear_Rahmen_Bot_Trans_T2"), strCell, ResultD[0][1]);
		l_GetstrBlock(_T("_Trans_T3"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Shear_Rahmen_Top_Trans_T3"), strCell, ResultD[1][1]);
	}

	stRow++;
}

void CBrdgSeisEvalKALIS2023_Report::GeneratePlainConcCapa(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2])
{
	// 6.4 교각의 내진성능평가
	RptBlock = _T("[2]Seismic_Eval_Plainconc_Pier_Head");
	auto rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	short Row_Ref = 0, Col_Ref = 0;

	auto l_PrintData = [&](const CString& strBlock, CString strCell[8], const T_BRDGEVAL_RESULT& ResD)
	{
		RptBlock = strBlock;
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if(ResD.EvalD.bCheck)
		{
			PrintCellData(pExcel, strCell[0], ResD.EvalD.PalinConcD.dFpcf, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[1], ResD.EvalD.PalinConcD.dFpcc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[2], ResD.EvalD.PalinConcD.dFpcv, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[3], ResD.EvalD.PalinConcD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[4], ResD.EvalD.PalinConcD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[5], ResD.EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, strCell[6], Check_Sign(0, ResD.EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, strCell[7], Chk_OK_Sign(0, 1.0, ResD.EvalD.dRat), Row_Ref, Col_Ref, stRow);
		}
		stRow += rowCnt;
	};

	auto l_GetstrBlock = [](const CString& strTail, CString strBlocks[8])
	{
		strBlocks[0] = _T("S12_FPCF");
		strBlocks[1] = _T("S12_FPCC");
		strBlocks[2] = _T("S12_FPCV");
		strBlocks[3] = _T("S12_FPC");
		strBlocks[4] = _T("S12_Rat_FPC");
		strBlocks[5] = _T("S12_Rat");
		strBlocks[6] = _T("S12_COMP");
		strBlocks[7] = _T("S12_Result");
		for(auto i = 0; i < 8; i++)	{ strBlocks[i] += strTail; }
	};

	if(enBehavior[0] == CANTILEVER)
	{
		CString strCell[8];
		l_GetstrBlock(_T("_Longi_T1"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Cantil_Longi_T1"), strCell, ResultD[0][0]);
	}
	else
	{
		CString strCell[8];
		l_GetstrBlock(_T("_Longi_T2"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Rahmen_Bot_Longi_T2"), strCell, ResultD[0][0]);
		l_GetstrBlock(_T("_Longi_T3"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Rahmen_Top_Longi_T3"), strCell, ResultD[1][0]);
	}

	if(enBehavior[0] == CANTILEVER)
	{
		CString strCell[8];
		l_GetstrBlock(_T("_Trans_T1"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Cantil_Trans_T1"), strCell, ResultD[0][1]);
	}
	else
	{
		CString strCell[8];
		l_GetstrBlock(_T("_Trans_T2"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Rahmen_Bot_Trans_T2"), strCell, ResultD[0][1]);
		l_GetstrBlock(_T("_Trans_T3"), strCell);
		l_PrintData(_T("[2]Seismic_Eval_Plainconc_Rahmen_Top_Trans_T3"), strCell, ResultD[1][1]);
	}

	stRow++;
}

BOOL CBrdgSeisEvalKALIS2023_Report::GenerateEffectiveStiffJudgement(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2])
{
	T_SECT_K SectK_Bot=0, SectK_Top=0;
	T_MPCC_K MpccK_Bot = max(ResultD[0][0].ParamD.OptD.MpccK, ResultD[0][1].ParamD.OptD.MpccK);
	T_MPCC_K MpccK_Top = max(ResultD[1][0].ParamD.OptD.MpccK, ResultD[1][1].ParamD.OptD.MpccK);
	if(MpccK_Bot != 0)
	{
		T_MPCC_D MpccD;
		if(m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK_Bot, MpccD))
		{
			SectK_Bot = MpccD.SectK;
		}
	}
	if(MpccK_Top != 0)
	{
		T_MPCC_D MpccD;
		if(m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK_Top, MpccD))
		{
			SectK_Top = MpccD.SectK;
		}
	}
	if(SectK_Bot == SectK_Top) SectK_Top = 0; // 같은 단면이면 하나만 출력

	if(SectK_Bot == 0 && SectK_Top == 0) return TRUE;

	CArray<T_SYEF_K, T_SYEF_K> aSyefK;
	m_pDoc->m_pAttrCtrl2->GetSyefKeyList(aSyefK);
	const auto nSyef = aSyefK.GetSize();

	T_MPCC_D MpccD_Bot, MpccD_Top;
	MpccD_Bot.Initialize();
	MpccD_Top.Initialize();
	T_SYEF_D SyefD_Bot, SyefD_Top;
	SyefD_Bot.Initialize();
	SyefD_Top.Initialize();
	for(int i = 0; i < nSyef; ++i)
	{
		if((MpccK_Bot == 0 || SyefD_Bot.nDgnType == 2) && 
			(MpccK_Top == 0 || SyefD_Top.nDgnType == 2)) break;

		T_SYEF_D SyefD;
		if(!m_pDoc->m_pAttrCtrl2->GetSyef(aSyefK[i], SyefD)) continue;
		if(SyefD.nDgnType == 2)
		{
			T_MPCC_D MpccD;
			if(m_pDoc->m_pAttrCtrl2->GetMpcc(aSyefK[i], MpccD))
			{
				if(MpccD.SectK == SectK_Bot)
				{
					MpccD_Bot = MpccD;
					SyefD_Bot = SyefD;
				}
				else if(MpccD.SectK == SectK_Top)
				{
					MpccD_Top = MpccD;
					SyefD_Top = SyefD;
				}
			}
		}
	}

	const bool bPrintBot = SyefD_Bot.nDgnType == 2 ? true : false;
	const bool bPrintTop = SyefD_Top.nDgnType == 2 ? true : false;
	if(!bPrintBot && !bPrintTop) return TRUE;

	short Row_Ref = 0, Col_Ref = 0;
	RptBlock = _T("[3]Effect_Sect_Stiffness");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	
	enum
	{
		Type0 = 0,
		Type1,
		Type2,
		Type3,
		User
	};

	auto l_GetType = [](const bool& bUser, const int& nType, const double& dMe, const double& dMy)
	{
		if(bUser)	return User;

		if(MT(dMe, dMy))
			return Type1;
		else if(ME(dMe, 0.5*dMy))
			return Type2;
		else
			return Type3;
	};

	auto l_PrintRow = [&](const CString& strBlock1, const CString& strBlock2, const CString& strBlock3, const double& dMe, const double& dMy)
	{
		RptBlock = strBlock1;
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		if(strBlock2.IsEmpty() || strBlock3.IsEmpty()) return;
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pExcel, strBlock2, dMe, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, strBlock3, dMy, Row_Ref, Col_Ref, stRow);
	};

	auto l_PrintRowUser = [&](const CString& strBlock1, const CString& strBlock2, const CString& strBlock3, const CString& strBlock4, const double& dMe, const double& dMy, const double& dIgRat)
	{
		RptBlock = strBlock1;
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		if(strBlock2.IsEmpty() || strBlock3.IsEmpty()) return;
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pExcel, strBlock2, dMe, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, strBlock3, dMy, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, strBlock3, dMy, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, strBlock4, dIgRat, Row_Ref, Col_Ref, stRow);
	};

	CString strBot, strTop;
	if(bPrintBot)
	{
		T_EVGP_D EvgpD;
		if(!m_pDoc->m_pAttrCtrl2->GetEvgp(MpccD_Bot.EvgpK, EvgpD)) EvgpD.Initialize();
		strBot.Format(_T("%s(%s)"), EvgpD.strName, EvgpD.aEvgpColm[MpccD_Bot.nEvgpColm].strName);
	}
	if(bPrintTop)
	{
		T_EVGP_D EvgpD;
		if(!m_pDoc->m_pAttrCtrl2->GetEvgp(MpccD_Top.EvgpK, EvgpD)) EvgpD.Initialize();
		strTop.Format(_T("%s(%s)"), EvgpD.strName, EvgpD.aEvgpColm[MpccD_Top.nEvgpColm].strName);
	}

	// Longitudinal
	if(bPrintBot || bPrintTop)
	{
		if(bPrintBot && bPrintTop)
		{
			RptBlock = _T("[2]Effect_Sect_Longi");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;

			RptBlock = _T("[3]Effect_Sect_Colm_Bot");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pExcel, _T("S4_3_Bot_Colm"), strBot, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;

			RptBlock = _T("[3]Effect_Sect_Stiffness_Table_Head");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;

			T_SYEF_D SyefD = SyefD_Bot;
			double dMe = SyefD.dMeL;
			double dMy = SyefD.dMyL;
			int nType = l_GetType(SyefD.bUserDefined, SyefD.nDgnType, dMe, dMy);
			switch(nType)
			{
				case Type1:		l_PrintRow(_T("S4_3_Type1_1L"), _T("S4_3_T1_ME1L"), _T("S4_3_T1_My1L"), dMe, dMy);	break;
				case Type2:		l_PrintRow(_T("S4_3_Type2_1L"), _T("S4_3_T2_ME1L"), _T("S4_3_T2_My1L"), dMe, dMy);	break;
				case Type3:		l_PrintRow(_T("S4_3_Type3_1L"), _T("S4_3_T3_ME1L"), _T("S4_3_T3_My1L"), dMe, dMy);	break;
				case User:		l_PrintRowUser(_T("S4_3_User_L"), _T("S4_3_User_MEL"), _T("S4_3_User_MyL"), _T("S4_3_User_ValueL"),
					dMe, dMy, SyefD.dI_EffL);	break;
				default:	l_PrintRow(_T("S4_3_Type0"), _T(""), _T(""), dMe, dMy);	break;
			}
			stRow += rowCnt;

			RptBlock = _T("[3]Effect_Sect_Colm_Top");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pExcel, _T("S4_3_Top_Colm"), strTop, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;

			RptBlock = _T("[3]Effect_Sect_Stiffness_Table_Head");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;

			SyefD = SyefD_Top;
			dMe = SyefD.dMeL;
			dMy = SyefD.dMyL;
			nType = l_GetType(SyefD.bUserDefined, SyefD.nDgnType, dMe, dMy);
			switch(nType)
			{
				case Type1:		l_PrintRow(_T("S4_3_Type1_2L"), _T("S4_3_T1_ME2L"), _T("S4_3_T1_My2L"), dMe, dMy);	break;
				case Type2:		l_PrintRow(_T("S4_3_Type2_2L"), _T("S4_3_T2_ME2L"), _T("S4_3_T2_My2L"), dMe, dMy);	break;
				case Type3:		l_PrintRow(_T("S4_3_Type3_2L"), _T("S4_3_T3_ME2L"), _T("S4_3_T3_My2L"), dMe, dMy);	break;
				case User:		l_PrintRowUser(_T("S4_3_User_L"), _T("S4_3_User_MEL"), _T("S4_3_User_MyL"), _T("S4_3_User_ValueL"),
					dMe, dMy, SyefD.dI_EffL);	break;
				default:	l_PrintRow(_T("S4_3_Type0"), _T(""), _T(""), dMe, dMy);	break;
			}
			stRow += rowCnt;
		}
		else if(bPrintBot)
		{
			RptBlock = _T("[3]Effect_Sect_Longi");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pExcel, _T("S4_3_Longi_Colm"), strBot, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;

			RptBlock = _T("[3]Effect_Sect_Stiffness_Table_Head");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;

			T_SYEF_D SyefD = SyefD_Bot;
			const double dMe = SyefD.dMeL;
			const double dMy = SyefD.dMyL;
			const int nType = l_GetType(SyefD.bUserDefined, SyefD.nDgnType, dMe, dMy);
			switch(nType)
			{
				case Type1:		l_PrintRow(_T("S4_3_Type1_L"), _T("S4_3_T1_MEL"), _T("S4_3_T1_MyL"), dMe, dMy);	break;
				case Type2:		l_PrintRow(_T("S4_3_Type2_L"), _T("S4_3_T2_MEL"), _T("S4_3_T2_MyL"), dMe, dMy);	break;
				case Type3:		l_PrintRow(_T("S4_3_Type3_L"), _T("S4_3_T3_MEL"), _T("S4_3_T3_MyL"), dMe, dMy);	break;
				case User:		l_PrintRowUser(_T("S4_3_User_L"), _T("S4_3_User_MEL"), _T("S4_3_User_MyL"), _T("S4_3_User_ValueL"),
					dMe, dMy, SyefD.dI_EffL);	break;
				default:	    l_PrintRow(_T("S4_3_Type0"), _T(""), _T(""), dMe, dMy);	break;
			}
			stRow += rowCnt;
		}
		else
		{
			RptBlock = _T("[3]Effect_Sect_Longi");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pExcel, _T("S4_3_Longi_Colm"), strTop, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;

			RptBlock = _T("[3]Effect_Sect_Stiffness_Table_Head");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;

			T_SYEF_D SyefD = SyefD_Top;
			const double dMe = SyefD.dMeL;
			const double dMy = SyefD.dMyL;
			const int nType = l_GetType(SyefD.bUserDefined, SyefD.nDgnType, dMe, dMy);
			switch(nType)
			{
				case Type1:		l_PrintRow(_T("S4_3_Type1_L"), _T("S4_3_T1_MEL"), _T("S4_3_T1_MyL"), dMe, dMy);	break;
				case Type2:		l_PrintRow(_T("S4_3_Type2_L"), _T("S4_3_T2_MEL"), _T("S4_3_T2_MyL"), dMe, dMy);	break;
				case Type3:		l_PrintRow(_T("S4_3_Type3_L"), _T("S4_3_T3_MEL"), _T("S4_3_T3_MyL"), dMe, dMy);	break;
				case User:		l_PrintRowUser(_T("S4_3_User_L"), _T("S4_3_User_MEL"), _T("S4_3_User_MyL"), _T("S4_3_User_ValueL"),
					dMe, dMy, SyefD.dI_EffL);	break;
				default:	    l_PrintRow(_T("S4_3_Type0"), _T(""), _T(""), dMe, dMy);	break;
			}
			stRow += rowCnt;
		}
	}

	// Transverse
	if(bPrintBot || bPrintTop)
	{
		if(bPrintBot && bPrintTop)
		{
			RptBlock = _T("[2]Effect_Sect_Trans");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;

			RptBlock = _T("[3]Effect_Sect_Colm_Bot");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pExcel, _T("S4_3_Bot_Colm"), strBot, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;

			RptBlock = _T("[3]Effect_Sect_Stiffness_Table_Head");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;

			T_SYEF_D SyefD = SyefD_Bot;
			double dMe = SyefD.dMeT;
			double dMy = SyefD.dMyT;
			int nType = l_GetType(SyefD.bUserDefined, SyefD.nDgnType, dMe, dMy);
			switch(nType)
			{
				case Type1:		l_PrintRow(_T("S4_3_Type1_1T"), _T("S4_3_T1_ME1T"), _T("S4_3_T1_My1T"), dMe, dMy);	break;
				case Type2:		l_PrintRow(_T("S4_3_Type2_1T"), _T("S4_3_T2_ME1T"), _T("S4_3_T2_My1T"), dMe, dMy);	break;
				case Type3:		l_PrintRow(_T("S4_3_Type3_1T"), _T("S4_3_T3_ME1T"), _T("S4_3_T3_My1T"), dMe, dMy);	break;
				case User:		l_PrintRowUser(_T("S4_3_User_T"), _T("S4_3_User_MET"), _T("S4_3_User_MyT"), _T("S4_3_User_ValueT"),
					dMe, dMy, SyefD.dI_EffT);	break;
				default:	l_PrintRow(_T("S4_3_Type0"), _T(""), _T(""), dMe, dMy);	break;
			}
			stRow += rowCnt;

			RptBlock = _T("[3]Effect_Sect_Colm_Top");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pExcel, _T("S4_3_Top_Colm"), strTop, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;

			RptBlock = _T("[3]Effect_Sect_Stiffness_Table_Head");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;

			SyefD = SyefD_Top;
			dMe = SyefD.dMeT;
			dMy = SyefD.dMyT;
			nType = l_GetType(SyefD.bUserDefined, SyefD.nDgnType, dMe, dMy);
			switch(nType)
			{
				case Type1:		l_PrintRow(_T("S4_3_Type1_2T"), _T("S4_3_T1_ME2T"), _T("S4_3_T1_My2T"), dMe, dMy);	break;
				case Type2:		l_PrintRow(_T("S4_3_Type2_2T"), _T("S4_3_T2_ME2T"), _T("S4_3_T2_My2T"), dMe, dMy);	break;
				case Type3:		l_PrintRow(_T("S4_3_Type3_2T"), _T("S4_3_T3_ME2T"), _T("S4_3_T3_My2T"), dMe, dMy);	break;
				case User:		l_PrintRowUser(_T("S4_3_User_T"), _T("S4_3_User_MET"), _T("S4_3_User_MyT"), _T("S4_3_User_ValueT"),
					dMe, dMy, SyefD.dI_EffT);	break;
				default:	l_PrintRow(_T("S4_3_Type0"), _T(""), _T(""), dMe, dMy);	break;
			}
			stRow += rowCnt;
		}
		else if(bPrintBot)
		{
			RptBlock = _T("[3]Effect_Sect_Trans");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pExcel, _T("S4_3_Trans_Colm"), strBot, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;

			RptBlock = _T("[3]Effect_Sect_Stiffness_Table_Head");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;

			T_SYEF_D SyefD = SyefD_Bot;
			const double dMe = SyefD.dMeT;
			const double dMy = SyefD.dMyT;
			const int nType = l_GetType(SyefD.bUserDefined, SyefD.nDgnType, dMe, dMy);
			switch(nType)
			{
				case Type1:		l_PrintRow(_T("S4_3_Type1_T"), _T("S4_3_T1_MET"), _T("S4_3_T1_MyT"), dMe, dMy);	break;
				case Type2:		l_PrintRow(_T("S4_3_Type2_T"), _T("S4_3_T2_MET"), _T("S4_3_T2_MyT"), dMe, dMy);	break;
				case Type3:		l_PrintRow(_T("S4_3_Type3_T"), _T("S4_3_T3_MET"), _T("S4_3_T3_MyT"), dMe, dMy);	break;
				case User:		l_PrintRowUser(_T("S4_3_User_T"), _T("S4_3_User_MET"), _T("S4_3_User_MyT"), _T("S4_3_User_ValueT"),
					dMe, dMy, SyefD.dI_EffT);	break;
				default:	l_PrintRow(_T("S4_3_Type0"), _T(""), _T(""), dMe, dMy);	break;
			}
			stRow += rowCnt;
		}
		else
		{
			RptBlock = _T("[3]Effect_Sect_Trans");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pExcel, _T("S4_3_Trans_Colm"), strTop, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;

			RptBlock = _T("[3]Effect_Sect_Stiffness_Table_Head");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;

			T_SYEF_D SyefD = SyefD_Top;
			const double dMe = SyefD.dMeT;
			const double dMy = SyefD.dMyT;
			const int nType = l_GetType(SyefD.bUserDefined, SyefD.nDgnType, dMe, dMy);
			switch(nType)
			{
				case Type1:		l_PrintRow(_T("S4_3_Type1_T"), _T("S4_3_T1_MET"), _T("S4_3_T1_MyT"), dMe, dMy);	break;
				case Type2:		l_PrintRow(_T("S4_3_Type2_T"), _T("S4_3_T2_MET"), _T("S4_3_T2_MyT"), dMe, dMy);	break;
				case Type3:		l_PrintRow(_T("S4_3_Type3_T"), _T("S4_3_T3_MET"), _T("S4_3_T3_MyT"), dMe, dMy);	break;
				case User:		l_PrintRowUser(_T("S4_3_User_T"), _T("S4_3_User_MET"), _T("S4_3_User_MyT"), _T("S4_3_User_ValueT"),
					dMe, dMy, SyefD.dI_EffT);	break;
				default:	l_PrintRow(_T("S4_3_Type0"), _T(""), _T(""), dMe, dMy);	break;
			}
			stRow += rowCnt;
		}
	}

	stRow += 1;

	return TRUE;
}

void CBrdgSeisEvalKALIS2023_Report::GetMPhiGraphFileNames(CString aResName[3])
{
	aResName[0] = _T("option01.emf");
	aResName[1] = _T("option02.emf");
	aResName[2] = _T("option03.emf");
}