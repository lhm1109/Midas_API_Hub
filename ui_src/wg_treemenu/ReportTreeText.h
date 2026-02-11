#if !defined(__REPORTTREETEXT_H__)
#define __REPORTTREETEXT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportTreeText.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "ReportDefine.h"
#include <vector>
using namespace std;

#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\DB_ST_DN_RESULT_PSC.h"
#include "..\wg_db\DB_ST_POST_READ.h"
#include "..\wg_db\DB_ST_DN_RESULT_CH.H"

#define D_AMR_BODF_IX   0
#define D_AMR_CNLD_IX   1
#define D_AMR_SDSP_IX   2
#define D_AMR_BMLD_IX   3
#define D_AMR_PRST_IX   4
#define D_AMR_PTNS_IX   5
#define D_AMR_PRES_IX   6
#define D_AMR_STMP_IX   7
#define D_AMR_NTMP_IX   8
#define D_AMR_ETMP_IX   9
#define D_AMR_GTMP_IX   10
#define D_AMR_WIND_IX   11
#define D_AMR_SEIS_IX   12
#define D_AMR_TDPL_IX   13
#define D_AMR_BTMP_IX   14
#define D_AMR_FMLD_IX   15
#define D_STLD_DATA_NUM 16

const CString GENTITLEREPLACE = _T("~!@#$^&*");

/////////////////////////////////////////////////////////////////////////////
// CReportTreeText
class CReportTreeText
{
// Construction
public:
	CReportTreeText();   // standard constructor
	~CReportTreeText();

	BOOL    WriteReportText(const CString strTextItem, CString& strWriteText);
	//BOOL    WriteReportText_CH(const CString strTextItem, int iChapterID,int iSegmentID,CString& strWriteText);
	BOOL    WriteReportText_CH(const CString strTextItem, GenSegmentElements& genElements);
	BOOL    ReplaceTitileNumber(int iChapter,int iSegment,CString& strTexts);
	BOOL    WriteCtrl(CString& strText);  // Control Data
	BOOL    WriteBeam(CString& strText);  // Beam
	BOOL    WriteBndr(CString& strText);  // Boundary
	BOOL    WriteNode(CString& strText);  // Node
	BOOL    WritePOGD(CString& strText);  // Pushover Global Control
	BOOL    WritePOLC(CString& strText);  // Pushover Load Case
	BOOL    WriteRigd(CString& strText);  // Floor Diaphragm/Rigid Link
	BOOL    WriteStor(CString& strText);  // Story
	BOOL    WriteTrus(CString& strText);  // Truss
	BOOL    WriteWVSA(CString& strText);  // Weight/Volume/Surface area of all member
	BOOL    CalcWeightVolumnSurfaceArea(T_SECT_D* pSectD, CArray<UINT, UINT>* paElemK, 
																			double &rSArea, double &rVolumn, double &rWeight, int &rFrame, int &rTruss);
	BOOL    WriteWall(CString& strText);  // Wall
	BOOL    WriteStld(CString& strText);  // Static Loadcase

	BOOL    WriteStldData(const CString strTextItem, CString& strText);
	int     ClassifyData();
	void    DeleteStldData();
	BOOL    WriteBodf(int nIndex, CStringArray& arWriteData); // self weight
	BOOL    WriteCnld(int nIndex, CStringArray& arWriteData); // nodal load
	BOOL    WriteSdsp(int nIndex, CStringArray& arWriteData); // specified displacement
	BOOL    WriteBmld(int nIndex, CStringArray& arWriteData); // beam load
	BOOL    WriteFmld(int nIndex, CStringArray& arWriteData); // finishing material load
	BOOL    WriteStmp(int nIndex, CStringArray& arWriteData); // _tsystem temperature load
	BOOL    WriteNtmp(int nIndex, CStringArray& arWriteData); // nodal temperature load
	BOOL    WriteEtmp(int nIndex, CStringArray& arWriteData); // element temperature load
	BOOL    WriteBtmp(int nIndex, CStringArray& arWriteData); // beam section temperature load
	BOOL    WriteWind(int nIndex, CStringArray& arWriteData); // wind load
	BOOL    WriteSeis(int nIndex, CStringArray& arWriteData); // seis load
	BOOL    WriteWindProfile(T_WIND_D& rData, CStringArray& arWriteData);
	BOOL    WriteSeisProfile(T_SEIS_D& rData, CStringArray& arWriteData);
	BOOL    WriteSplc(CStringArray& arWriteData); // Response Spectrum Load case Data
	BOOL    WriteLcom(CStringArray& arWriteData); // load combination
	
	//////////////////////////////////////////////////////////////////////////
	////////////////////China Report//////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	/** Add By JUHONGHUA(MITC) 2012-2-14 15:44:45**/
	void    WriteTitle(int iIndex,CString& strText);//iIndex --1  ,2  ,3   最多支持三级标题
	CString    BuilderChapterTitle(int iChapterID,int iSegmentID,const CString& strText);
	
	BOOL    WriteBasicInfo_General(GenSegmentElements& genElements); 
	BOOL    WriteBasicInfo_Standard(GenSegmentElements& genElements); 
	BOOL    WriteBasicInfo_Code(GenSegmentElements& genElements); 

	BOOL    WriteBasicInfo_Structure(GenSegmentElements& genElements); 
	BOOL    WriteBasicInfo_Material(GenSegmentElements& genElements); 
	BOOL    WriteBasicInfo_Calculation(GenSegmentElements& genElements); 

	BOOL    WriteModel_Calculation    (GenSegmentElements& genElements); 
	BOOL    WriteModel_Steel          (GenSegmentElements& genElements); 
	BOOL    WriteModel_Section        (GenSegmentElements& genElements); 
	BOOL    WriteModel_Load           (GenSegmentElements& genElements); 
	
	BOOL    WriteStress_MemberForce      (GenSegmentElements& genElements); 

	double GetU( double df );
	
	BOOL    WriteLoadCapacity_ComDepth (GenSegmentElements& genElements); 
	BOOL    WriteLoadCapacity_Bending  (GenSegmentElements& genElements); 
	BOOL    WriteLoadCapacity_Shearing (GenSegmentElements& genElements); 
	BOOL    WriteLoadCapacity_Torsion  (GenSegmentElements& genElements); 
	BOOL    WriteLoadCapacity_Reforce  (GenSegmentElements& genElements); 
	BOOL    WriteServiceAbility_Normal (GenSegmentElements& genElements); 
	BOOL    WriteServiceAbility_Oblique(GenSegmentElements& genElements); 
	BOOL    WriteStress_NormalComp     (GenSegmentElements& genElements); 
	BOOL    WriteStress_Tension        (GenSegmentElements& genElements); 
	BOOL    WriteStress_PrincipalComp  (GenSegmentElements& genElements); 
	BOOL    WriteStress_ShortTerm      (GenSegmentElements& genElements); 
	
	BOOL     WriteDetailTables         (GenSegmentElements& genElements); 
	
	
	void GetLoadCombinationCases(CMap<CString, LPCTSTR, CString, CString&>& mapLcom);
	
	CString _ms(LPCTSTR lpStr);
	CString _ns(LPCTSTR lpStr, int n);
	BOOL    _ns_devide(LPCTSTR lpStr, int n, CString& rStr1, CString& rStr2);
	void    GetPartialList(CString &list, int nMaxLen, int nStart, int &nCount);
	void    WriteList2(CStringArray& arWriteData, CString &csPre, CString &csList, int nMaxLen, int nStartLen);
	CString ReplaceSymbolByTempChar(LPCTSTR lpStr);
	CString ReplaceTempCharBySpace(LPCTSTR lpStr);
	CString YesNo(BOOL bFlag) { return bFlag ? _T("YES") : _T("NO"); }
	CString PosNeg(BOOL bPos) { return bPos ? _T("POS") : _T("NEG"); }
	
	static CString GetNumberCharacterForChina(int iNumber);
	CString GetChinaCodeName(const CString& strCode);
	CString GetConstructionStageLoadCaseSymbol(T_SGLD_K sgldK);
	void GetElementsForDesign();
	CString GetResultFlgString(BOOL bTrue,int iType,CString& strChina,CString& strbTrue);
	static BOOL Get_MatlOfConcrete(CString CodeMatlName, double& dfck, double& dftk, double& dfcd, double& dftd);
	static BOOL Get_MatlOfReBar(CString strRbarNa, double& dfsk, double& dfsd, double& dfsd1, double& dEs,double& dr);
	static double GetLengthUnitRatio(double dval);
	static double GetForceUnitRatio(double dval);
	static double GetStressUnitRatio(double dval);
	static double GetMomentUnitRatio(double dval);
	static double GetAreaUnitRatio(double dval);
	static double GetDensityUnitRatio(double dval);
	CString GetTemperatureUnitName();
	void MakeSectionFigure(CString strImagePath, UINT SectK, int nType, CStringArray& arImagePath);
	void InsertSSTable(T_SECT_K sectKey,GenSegmentElements& genElements);
	
public:
	CArray<UINT, UINT> *m_aKeyList[D_STLD_DATA_NUM];  // static load甫 啊柳 data
	int                 m_nStldNum;
	CArray<int, int>    m_aStldDataNum;  // 阿 loadcase俊 措茄 单捞鸥 肮荐
	
	
protected:
	CDBDoc* m_pDoc;
	BOOL m_bIsSetStld;
	CArray<T_ELEM_K,T_ELEM_K> m_arElemKeyList;
	CArray<T_ELEM_K,T_ELEM_K> m_arColumnElemKeyList;
	CArray<T_TDNA_K, T_TDNA_K> m_arTdnaK;
	std::vector<pair<CString,CString> > m_vctDetailTable;
	
protected:
	void ConvertUnit_BDCY (T_BDCY_JTG& _data);
	void ConvertUnit_CBCS (T_CBCS_CH& _data);
	void ConvertUnit_CUMS (T_CUMS_JTG& _data);
	void ConvertUnit_CSIS (T_CSIS_CH& _data);
	void ConvertUnit_CRMT (T_CRMT_JTG& _data);
	void ConvertUnit_CTSS (T_CTSS_CH& _data);
	
	void ConvertUnit_FYCM (T_FYCM_JTG& _data);
	void ConvertUnit_FPMS (T_FPMS_JTG& _data);
	void ConvertUnit_FYPC (T_FYPC_JTG& _data);
	void ConvertUnit_FYSG (T_FYSG_D& _data);
	void ConvertUnit_REAC (T_REAC_D& _data);
	
	void ConvertUnit_Force (T_STRB_D& _data);

	CString GetModelPictureByView();
	
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__REPORTTREETEXT_H__)
