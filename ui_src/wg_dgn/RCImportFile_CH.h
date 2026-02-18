#if !defined(__RCIMPORTFILE_CH_H__)
#define __RCIMPORTFILE_CH_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RCImportFile_CH.h : header file
//
#include "CRCFileBase.h"
#include "RCDataCtrl_CH.h"
#include "CRCDataCtrl.h"

struct _DGN_CBCS_D
{
	UINT nElemK;
	UINT nPart;
	_DGN_RC_MATL_CH RcMatlCH;
	_DGN_RC_SECTION_CH SectionCH;

	void Initialize()
	{
		nElemK=0;
		nPart =0;
		RcMatlCH.Initialize();
		SectionCH.Initialize();
	}
};

struct _DGN_CSCC_D
{
	UINT nElemK;
	UINT nPart;
	_DGN_RC_MATL_CH RcMatlCH;

	void Initialize()
	{
		nElemK=0;
		nPart =0;
		RcMatlCH.Initialize();
	}
};

struct _DGN_CTST_D
{
	UINT nElemK;
	UINT nPart;

	void Initialize()
	{
		nElemK=0;
		nPart =0;
	}
};

struct _DGN_CCWS_D
{
	UINT nElemK;
	UINT nPart;
	BOOL bBeam;
	double dC1;
	double dC2;
	double dC3;
	double dGamma0;
	_DGN_RC_MATL_CH RcMatlCH;
	_DGN_RC_SECTION_CH BeamSectionCH;
	_DGN_RC_COLUMN_SECTION_CH ColumnSectionCH;

	void Initialize()
	{
		nElemK=0;
		nPart =0;
		bBeam = TRUE;
		dC1   =0.0;
		dC2   =0.0;
		dC3   =0.0;
		dGamma0 = 0.0;
		RcMatlCH.Initialize();
		BeamSectionCH.Initialize();
		ColumnSectionCH.Initialize();
	}
};

struct _DGN_CCOS_D
{
	UINT nElemK;
	UINT nPart;

	void Initialize()
	{
		nElemK=0;
		nPart =0;
	}
};

struct _DGN_CPSS_D
{
	UINT nElemK;
	UINT nPart;

	void Initialize()
	{
		nElemK=0;
		nPart =0;
	}
};

struct _DGN_BRLR_D
{
	UINT nElemK;
	UINT nPart;
	double dGamma0;
	_DGN_RC_MATL_CH RcMatlCH;
	_DGN_RC_SECTION_CH SectionCH;

	void Initialize()
	{
		nElemK=0;
		nPart =0;
		dGamma0 = .0;
		RcMatlCH.Initialize();
		SectionCH.Initialize();
	}
};

struct _DGN_CRLR_D
{
	UINT nElemK;
	UINT nPart;
	double dGamma0;
	_DGN_RC_MATL_CH RcMatlCH;
	_DGN_RC_COLUMN_SECTION_CH SectionCH;

	void Initialize()
	{
		nElemK=0;
		nPart =0;
		dGamma0 =.0;
		RcMatlCH.Initialize();
		SectionCH.Initialize();
	}
};

struct _DGN_BRCR_D
{
	UINT nElemK;
	UINT nPart;
	double dGamma0;
	_DGN_RC_MATL_CH RcMatlCH;
	_DGN_RC_SECTION_CH SectionCH;

	void Initialize()
	{
		nElemK=0;
		nPart =0;
		dGamma0 = .0;
		RcMatlCH.Initialize();
		SectionCH.Initialize();
	}
};

struct _DGN_CSIS_D
{
	UINT nElemK;
	UINT nPart;
	_DGN_RC_MATL_CH RcMatlCH;
	_DGN_RC_SECTION_CH SectionCH;

	void Initialize()
	{
		nElemK=0;
		nPart =0;
		RcMatlCH.Initialize();
		SectionCH.Initialize();
	}
};

struct _DGN_CTSS_D
{
	UINT nElemK;
	UINT nPart;
	_DGN_RC_MATL_CH RcMatlCH;
	_DGN_RC_SECTION_CH SectionCH;

	void Initialize()
	{
		nElemK=0;
		nPart =0;
		RcMatlCH.Initialize();
		SectionCH.Initialize();
	}
};

struct _DGN_CSCS_D
{
	UINT nElemK;
	UINT nPart;
	_DGN_RC_MATL_CH RcMatlCH;
	_DGN_RC_COLUMN_SECTION_CH SectionCH;

	void Initialize()
	{
		nElemK=0;
		nPart =0;
		RcMatlCH.Initialize();
		SectionCH.Initialize();
	}
};

struct _DGN_CTCS_D
{
	UINT nElemK;
	UINT nPart;
	_DGN_RC_MATL_CH RcMatlCH;
	_DGN_RC_COLUMN_SECTION_CH SectionCH;

	void Initialize()
	{
		nElemK=0;
		nPart =0;
		RcMatlCH.Initialize();
		SectionCH.Initialize();
	}
};

struct _DGN_CSNC_D
{
	UINT nElemK;
	UINT nPart;
	_DGN_RC_MATL_CH RcMatlCH;

	void Initialize()
	{
		nElemK=0;
		nPart =0;
		RcMatlCH.Initialize();
	}
};

//////////////////////////////////////////////////////////////////////////
// RC design of china. the code is [JTG D62-2004]
// add by maxiao.
//////////////////////////////////////////////////////////////////////////
class CRCImportFile_CH : public CCRCFileBase   
{
public:
	CRCImportFile_CH();
	virtual ~CRCImportFile_CH();
	BOOL RcDesign_CH(BOOL isBeam);//TRUE梁(beam design); FALSE柱(column design)
	
	int m_iTotLineNum;
	
	CArray<UINT,UINT> m_CbcsBeamKList;//弯
	CArray<UINT,UINT> m_CsisBeamKList;//剪
	CArray<UINT,UINT> m_CtssBeamKList;//扭
	CArray<UINT,UINT> m_CcwsBeamKList;//梁的裂缝宽度
	CArray<UINT,UINT> m_CsccBeamKList;//正截面法向应力
	CArray<UINT,UINT> m_CtstBeamKList;//正截面法向应力
	CArray<UINT,UINT> m_CsncBeamKList;//中性轴主拉应力
	CArray<UINT,UINT> m_BrlrKList;//梁纵筋估算
	CArray<UINT,UINT> m_BrcrKList;//梁的普通箍筋
	CArray<UINT,UINT> m_CcwsColumnKList;//柱的裂缝宽度
	CArray<UINT,UINT> m_CtcsColumnKList;//抗拉
	CArray<UINT,UINT> m_CscsColumnKList;//抗压
	CArray<UINT,UINT> m_CrlrKList;//柱纵筋估算

	CMap<UINT, UINT, UINT, UINT> m_DgnLcomKMap;
	CMap<UINT,UINT,UINT,UINT> m_DgnTdgrMap; //DgnTdgrK, TdgrK
	CMap<UINT,UINT,UINT,UINT> m_DgnTdnaMap; //DgnTdgrK, TdnaK
	CMap<_CRC_COMF_K, _CRC_COMF_K, _CRC_COMF_D, _CRC_COMF_D&> m_ComfDMap;

protected:
	CRCDataCtrl_CH m_DataCtrl;
	
	CStdioFile m_outfile;
	CStdioFile m_rptfile;

	CArray<UINT, UINT> m_ElemKList;
	CDBDoc* m_pDoc;
	T_RC_D_CH m_rcD;
	_DGN_RC_MATL_CH_W m_matl;

	BOOL MakeData( BOOL isBeam);
	BOOL Write_ImportData(BOOL isBeam);//TRUE梁(beam design); FALSE柱(column design)
	BOOL Write_Cscc(); // 施工阶段正截面法向应力验算:Check stress for cross section at a construction stage…
	BOOL Write_Ctst(); // 受拉区钢筋的拉应力验算:Check tensile stress for tension reinforcement…
//	BOOL Write_Ccws(); // 使用阶段裂缝宽度验算:Check crack width at service loads…
	BOOL Write_Ccos(); // 使用阶段正截面压应力验算:Compressive stress check of orthogonal section at service stage…
	BOOL Write_Cpss(); // 使用阶段斜截面主压应力验算:Principal compressive stress check of sloped section at service stage…
	BOOL Write_Brlr(); // 梁的纵向钢筋用量估算:Beam Required Longitudinal Reinforcement…
	BOOL Write_Crlr(); // 柱的纵向钢筋用量估算:Column Required Longitudinal Reinforcement…
	BOOL Write_Brcr(); // 梁的普通箍筋用量估算:Beam Required comm rebars…
	BOOL Write_Cbcs(); // 使用阶段正截面抗弯验算:Check bending for cross section at service loads…
	BOOL Write_Csis(); // 使用阶段斜截面抗剪验算:Check shear for inclined section at service loads…
	BOOL Write_Ctss(); // 使用阶段抗扭验算:Check torsion at service loads…
	BOOL Write_Cscs(); // 使用阶段正截面轴心/偏心抗压承载能力验算:Axes/eccentricity compressive stress check of orthogonal section at service stage…
	BOOL Write_Ctcs(); // 使用阶段正截面轴心/偏心抗拉承载能力验算:Axes/eccentricity tensile stress check of orthogonal section at service stage…
	BOOL Write_Ccws(BOOL itype);//TRUE梁(beam); FALSE柱(column)---使用阶段裂缝宽度验算:Check crack width at service loads
	BOOL Write_ColumnCcws();//柱(column)---使用阶段裂缝宽度验算:Check crack width at service loads
	BOOL Write_Csnc();// 施工阶段中性轴处的主拉应力(剪应力)验算:Check stress for Neutral axis at a construction stage…

	//+++++
	HINSTANCE m_hDllCalc;

	CString Get_DllFileName(int iCode);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined()
