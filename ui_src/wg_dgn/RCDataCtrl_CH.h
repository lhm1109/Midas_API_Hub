#if !defined(__RCDATACTRL_CH_H__)
#define __RCDATACTRL_CH_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RcDataCtrl_CH.h : header file
//
const int cPOSI_RC_CH = 2;
#include <afxtempl.h>
#include  <math.h>
#include "DgnStruct.h"
#include "Dgn_ServiceDef.h"
#include "CRCForceCtrl.h"
#include "CRCDataCtrl.h"
#include "DgnPscCommon.h"
#include "..\wg_db\DB_ST_DN_RESULT_CH.H"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "DesignForceResult.h"
#include "..\..\dgnengine\src/CVL_DgnCalc_CH/DgnCalc_CH_RC_Struct.h"

#define T_ELEM_K unsigned int
#include <map>


struct _RBAR_KEY
{
    CString m_strBarName;
    double m_dStaZ;
    int m_iStaZRef;


    bool  operator <  (const _RBAR_KEY& ptr) const
    {
        if(m_strBarName < ptr.m_strBarName)
        {
            return true;
        }
        else if(m_strBarName == ptr.m_strBarName)
        {
             if(m_iStaZRef < ptr.m_iStaZRef)
             {
                 return true;
             }
             else if(m_iStaZRef == ptr.m_iStaZRef)
             {
                 if(m_dStaZ < ptr.m_dStaZ)
                     return true;
             }
        }

        return false;
    }

};
// class CDesignForceResult;

//截面的设计材料
struct _DGN_RC_MATL_CH
{
	double dfcd;//混凝土抗压强度设计值 concrete
	double dftd;//混凝土抗拉强度设计值
	double dfcuk;//边长150mm立方体混凝土抗压强度标准值(Mpa),即为混凝土强度等级.
    double dEc;//混凝土的弹性模量
    double dfck1;//规范7.2.4-1 的fck' (表3.1.3)
	double dftk1;//施工阶段混凝土轴心抗拉强度标准值ftk'

	double dfsd;//主筋 MainRebar
	double dfsd1;//主筋
	double dEs;//普通钢筋的弹性模量
	double dfsk;//普通钢筋抗拉强度标准值fsk

	double dfsyd;//箍筋 SubRebar
    double dEsy;//箍筋的弹性模量
    double dfsky;//箍筋的抗拉强度标准值
    
    double dsib;//相对界限受压区高度ξb table 5.2.1
    double dBeta;//β see [JTG D62--2004] table 5.3.3 P34 
    double dEpsilon;//εsee [JTG D62--2004] 5.3.3 P34

	void Initialize(){
		dfcd = dftd = dfcuk = dEc = 0.0;
		dfck1 =  dftk1 = 0.0;
        dfsd = dfsd1 = dEs = dfsk = 0.0;
        dfsyd = dEsy = dfsky = 0.0;
		dsib = dBeta = dEpsilon = 0.0;
	}
};

//梁截面的几何参数
struct _DGN_RC_SECTION_CH
{
	double dbf1;//上翼缘宽度,计算时简化为T型截面的受压翼缘
	double dhf1;//上翼缘高度,计算时简化为T型截面的受压翼缘
	double dbf2,dhf2;//下翼缘

	double db;
	double dh;
	
    double dx;
	double dh0;
	double das1;
	double das;
    double dA;//截面面积
	double dAs;//下部纵筋
	double dAs1;//上部纵筋
	double dAst;//抗扭钢筋面积torsion Rebar 's Area
	double dAsv1;//抗扭钢筋单肢面积
	double dPitt;//抗扭钢筋间距torsion Rebar 's pitch
	double dAnglt;//抗扭箍筋角度torsion Rebar 's Angle
	double dat; //抗扭箍筋保护层厚度
	double dAsb;//弯起钢筋面积bend Rebar 's Area
	double dPitb;//弯起钢筋间距bend Rebar 's pitch
	double dAnglb;//弯起箍筋角度bend Rebar 's Angle
	double dAsh;//箍筋面积hoop Rebar 's Area
	double dPith;//箍筋间距hoop Rebar 's pitch
	double dAnglh;//箍筋角度hoop Rebar 's Angle
    double dWt;//截面受扭塑性抵抗矩
    double dBetaa;//箱形截面有效壁厚折减系数
    double dAoh;//截面核心面积
    double dPh;//截面核心面积周长
    double dd;//纵向受拉钢筋直径 p62
    double dd1;//纵向受拉钢筋直径(反位置，受压钢筋直径)
 //截面特性参数
    double dAsy;//单元局部坐标系y轴方向抵抗剪力的有效抗剪面积。
    double dAsz;//单元局部坐标系z轴方向抵抗剪力的有效抗剪面积。
    double dIxx;//绕单元局部坐标系x轴的扭转惯性距。
    double dIyy;//绕单元局部坐标系y轴的惯性距。
    double dIzz;//绕单元局部坐标系z轴的惯性距。
    double dCyp;//沿单元局部坐标系+y轴方向，单元截面中和轴到边缘纤维的距离。
    double dCym;//沿单元局部坐标系-y轴方向，单元截面中和轴到边缘纤维的距离。
    double dCzp;//沿单元局部坐标系+z轴方向，单元截面中和轴到边缘纤维的距离。
    double dCzm;//沿单元局部坐标系-z轴方向，单元截面中和轴到边缘纤维的距离。
    double dQyb;//沿单元局部坐标系z轴方向的剪切系数。
    double dQzb;//沿单元局部坐标系y轴方向的剪切系数。

    CArray<T_RPSC_MBAR,T_RPSC_MBAR> rbar;//每一层的钢筋
	
	void Initialize()
	{
		dbf1 = dhf1 = dbf2 = dhf2 = 0.0;	

		db = 0.0;	dh = 0.0;	
		dx = 0.0;	dh0 = 0.0;
		das1 = 0.0;	das = 0.0;
        dA = 0.0;	dAs1 = dAs = 0.0;
		dAst = dAsv1 = dPitt = dAnglt = dat = 0.0;
		dAsb = dPitb = dAnglb = 0.0;
		dAsh = dPith = dAnglh = 0.0;
        dWt = dBetaa = dAoh = dPh = 0.0;
		dd =  dd1 = 0.0;
        dAsy = dAsz=dIxx=dIyy=dIzz=dCyp=dCym=dCzp=dCzm=dQyb=dQzb= 0.0;
		rbar.RemoveAll();
	}

	_DGN_RC_SECTION_CH()	{}
	_DGN_RC_SECTION_CH(const _DGN_RC_SECTION_CH& rData)	{*this=rData;}
	_DGN_RC_SECTION_CH& operator = (const _DGN_RC_SECTION_CH& rData)
	{
        dbf1   = rData.dbf1  ;
	  dhf1   = rData.dhf1  ;
	  dbf2   = rData.dbf2  ;
        dhf2   = rData.dhf2  ;
	  db     = rData.db    ;
	  dh     = rData.dh    ;	  
        dx     = rData.dx    ;
	  dh0    = rData.dh0   ;
	  das1   = rData.das1  ;
	  das    = rData.das   ;
        dA     = rData.dA    ;
	  dAs    = rData.dAs   ;
	  dAs1   = rData.dAs1  ;
	  dAst   = rData.dAst  ;
	  dAsv1  = rData.dAsv1 ;
	  dPitt  = rData.dPitt ;
	  dAnglt = rData.dAnglt;
	  dat    = rData.dat   ; 
	  dAsb   = rData.dAsb  ; 
	  dPitb  = rData.dPitb ;
	  dAnglb = rData.dAnglb;
	  dAsh   = rData.dAsh  ;
	  dPith  = rData.dPith ;
	  dAnglh = rData.dAnglh;
        dWt    = rData.dWt   ;
        dBetaa = rData.dBetaa;
        dAoh   = rData.dAoh  ;
        dPh    = rData.dPh   ;
        dd     = rData.dd    ;
        dd1    = rData.dd1   ;
        dAsy   = rData.dAsy  ;
        dAsz   = rData.dAsz  ;
        dIxx   = rData.dIxx  ;
        dIyy   = rData.dIyy  ;
        dIzz   = rData.dIzz  ;
        dCyp   = rData.dCyp  ;
        dCym   = rData.dCym  ;
        dCzp   = rData.dCzp  ;
        dCzm   = rData.dCzm  ;
        dQyb   = rData.dQyb  ;
        dQzb   = rData.dQzb  ;

		int iBarNum = rData.rbar.GetSize();
		rbar.SetSize(iBarNum);
		for(int i=0; i<iBarNum; i++)
		{
			rbar.SetAt(i, rData.rbar[i]);
		}		
        return *this;
	}

};

//对应于柱截面的几何参数
struct _DGN_RC_COLUMN_SECTION_CH
{
	double dbf1;//上翼缘宽度,计算时简化为T型截面的受压翼缘
	double dhf1;//上翼缘高度,计算时简化为T型截面的受压翼缘
 	double dbf2,dhf2;//下翼缘
 
	double db;
	double dh;

    double dr;//圆形截面半径
	double dd;//纵向受拉钢筋直径 p62
    double dd1;//纵向受拉钢筋直径
    double dx0;//截面实际受压区高度 p113
	double drs;//纵向钢筋所在圆周的半径
	double dC;//圆形截面混凝土保护层厚度

    double dh0;
	double das1;
	double das;

    double dA;//截面面积
	double dAs1;//上部纵筋
	double dAs;//下部纵筋
    double dAsl;//截面左侧配筋
    double dAsr;//截面右侧配筋

    double dAsh;//箍筋面积hoop Rebar 's Area
	double dPith;//箍筋间距hoop Rebar 's pitch
	double dAnglh;//箍筋角度hoop Rebar 's Angle
    
 //截面特性参数
    double dAsy;//单元局部坐标系y轴方向抵抗剪力的有效抗剪面积。
    double dAsz;//单元局部坐标系z轴方向抵抗剪力的有效抗剪面积。
    double dIxx;//绕单元局部坐标系x轴的扭转惯性距。
    double dIyy;//绕单元局部坐标系y轴的惯性距。
    double dIzz;//绕单元局部坐标系z轴的惯性距。
    double dCyp;//沿单元局部坐标系+y轴方向，单元截面中和轴到边缘纤维的距离。
    double dCym;//沿单元局部坐标系-y轴方向，单元截面中和轴到边缘纤维的距离。
    double dCzp;//沿单元局部坐标系+z轴方向，单元截面中和轴到边缘纤维的距离。
    double dCzm;//沿单元局部坐标系-z轴方向，单元截面中和轴到边缘纤维的距离。
    double dQyb;//沿单元局部坐标系z轴方向的剪切系数。
    double dQzb;//沿单元局部坐标系y轴方向的剪切系数。

	double dl0;//计算长度
    double dPhi;//轴压构件稳定系数φ 规范P33 表5.3.1计算

	void Initialize()
	{
		dbf1 = dhf1 = dbf2 = dhf2 = 0.0;
		db = dh = 0.0;
		dh0 = 0.0;
		das1 = das = 0.0;			
        dA = 0.0;
		dAs1 = dAs = dAsl = dAsr = 0.0;    
        dAsh = dPith = dAnglh = 0.0;
        dAsy = dAsz=dIxx=dIyy=dIzz=dCyp=dCym=dCzp=dCzm=dQyb=dQzb= 0.0;
		dr = dd =  dd1 = dx0 = drs = dC = 0.0;
		dl0 = 0.0;
        dPhi = 0.0;//dPhi
	}
};

// RC design parameter.  
struct T_RC_D_CH
{
    int iDimType;           // section design force(0=2D, 1=2D+Torsion, 2=3D)
    int iDgnCode;           // 1:JSCE02, 2:KSCE-USD03, 3=JTG D62-04.
    double iSafeType;	      // design safe grade (1.1=grade 1,1.0=grade 2,0.9=grade 3) 
    double iCrackCode;         // crack code(0.2= I II ,0.15= III IV)
    int iCrackType;	      // column's crack type(0=axes tensile 1=axes press 2=prejudicial tensile  3= prejudicial press)
    BOOL bDesignPrint[12];  //output parameter

    void Initialize()
    {
        iDimType = 0;
#if defined(_CH)
        iDgnCode = 3;//for china
#else
        iDgnCode = 4;
#endif
        iDimType = 2;
        iSafeType = 1.1;
        iCrackCode = 0.2;
        iCrackType = 2;
        for (int i = 0; i < 12; i++) bDesignPrint[i] = TRUE;
    }
};

//////////////////////////////////////////////////////////////////////////
//railway
struct T_RAIL_BEAM_FORCE
{
	double dMy[4];
	int    iMy[4];
 	double dMy1[4];
	double dMy2[4];
 	double dVz[4];
	int    iVz[4];
    BOOL isAddForce[4];

    void Initialize()
    {
		for (int i=0;i<4;i++) {
			dMy[i] = 0.0;
			iMy[i] = 0;
			dMy1[i] = 0.0;
			dMy2[i] = 0.0;
			dVz[i] = 0.0;
			iVz[i] = 0;
            isAddForce[i] = FALSE;
		}
    }
};
struct T_RAIL_COLUMN_FORCE
{
	double dMy[8];
	int    iMy[8];
 	double dMy1[8];
	double dMy2[8];
 	double dVz[8];
	int    iVz[8];
    double dNx[8];
	int    iNx[8];
    double dMz[8];
    int    iMz[8];
    BOOL isAddForce[8];

    void Initialize()
    {
		for (int i=0;i<8;i++) {
			dMy[i] = 0.0;
			iMy[i] = 0;
			dMy1[i] = 0.0;
			dMy2[i] = 0.0;
			dVz[i] = 0.0;
			iVz[i] = 0;
            dNx[i] = 0.0;
			iNx[i] = 0;
            dMz[i] = 0.0;
			iMz[i] = 0;
            isAddForce[i] = FALSE;
		}
    }
};

//////////////////////////////////////////////////////////////////////////
//psc railway
struct _TB_PSC_SECT
{
	double db;                //db-宽度
	double dh;                //dh-高度
	double db_f;              //db_f-上翼缘宽度
	double dbf;               //dbf-下翼缘宽度
	double dh_f;              //dh_f-上翼缘高度
	double dhf;               // dhf-下翼缘高度
	double dAs;               //dAs-受拉普通钢筋面积
	double dA_s;              //dA_s-受压普通钢筋面积
	double das;               //das-受拉普通钢筋保护层厚度   
	double da_s;              //da_s-受压普通钢筋保护层厚度   
	double dAp;               // dAp-受拉区预应力钢筋面积
	double dA_p;              //dA_p-受压区预应力钢筋面积
	double dApb;              //dApb-受拉区预应力弯起钢筋面积
	double dA_pb;             // 
	double dAv;               //dAv-一个截面上箍筋的总截面面积
	double dap;               // dap-受拉区预应力钢筋保护层厚度
	double da_p;              //  da_p-受压区预应力钢筋保护层厚度
	double dapb;              //dapb-受拉区预应力弯起钢筋保护层厚度
	double da_pb;             //
	double dApbsina;          //da-受拉区预应力弯起钢筋与构件的纵轴的夹角
	double dA_pbsina;         //   
	double dApbcosa;          // 
	double dA_pbcosa;         //   
	double dsv;               //dsv-箍筋的间距
	double dArea;             //dArea-换算截面面积
	double dIyy;              //  dIy-换算截面惯性矩
	double dIzz;              // 
	double dCzp;              //dCzp-受压区边缘到重心轴距离
	double dCzm;              //dCzm-受拉区边缘到重心轴距离
	double dS0;               //换算截面重心轴以下的面积对重心轴的面积矩 P59
	double dpMomPos[4][2];		// Position for Stress (0~4), (0=y,1=z). y1~y4 and Z1~Z4
	double dL0 ;
	double dU;
	void Initialize()
	{
		db = 0.0;
		dh = 0.0;
		db_f = 0.0;
		dbf = 0.0;
		dh_f = 0.0;    
	  dhf = 0.0;      
	  dAs = 0.0;      
	  dA_s = 0.0;     
	  das = 0.0;      
	  da_s = 0.0;     
	  dAp = 0.0;      
	  dA_p = 0.0;     
	  dApb = 0.0;     
	  dA_pb = 0.0;    
	  dAv = 0.0;       
	  dap = 0.0;     
	  da_p = 0.0;      
	  dapb = 0.0;     
	  da_pb = 0.0;     
	  dApbsina = 0.0; 
	  dA_pbsina = 0.0; 
	  dApbcosa = 0.0; 
	  dA_pbcosa = 0.0; 
	  dsv = 0.0;       
	  dArea = 0.0;     
	  dIyy = 0.0;    
	  dIzz = 0.0;     
	  dCzp = 0.0;     
	  dCzm = 0.0;   
		dS0= .0;
		dL0=.0 ;
		dU=.0;
		for(int i=0; i<4; i++)
		{
			dpMomPos[i][0]=0.0;
			dpMomPos[i][1]=0.0;
		}
	}
};

struct T_JTG_SEISMIC_D
{
    int iLoadType;//0-E1作用；1-E2弹性作用；2-E2弹塑性作用
    int iCheckType;//0-B、C类；1-D类  
    BOOL bDesignTpye[12];  //验算内容 
    // 0-桥墩强度; 1-桥台强度; 2-基础强度; 3-盖梁强度; 4-主拱圈强度; 5-桥墩塑性铰区抗剪强度;
    // 6-支座厚度（板式橡胶支座）; 7-支座抗滑稳定性（板式橡胶支座）; 8-支座水平位移（活动盆式支座）; 9-支座水平承载力（固定盆式支座）; 10-墩顶位移(规则桥梁); 11-塑性转角位移(非规则桥梁); 
    void Initialize()
    {
        iLoadType = 0; 
        iCheckType = 0;
        for (int i = 0; i < 12; i++) bDesignTpye [i] = TRUE; 
    }
};

//struct T_JTG_SEIS_CHKTYP_D
//{
//  int iElemType;//0-桥墩；1-桥台；2-基础；3-盖梁；4-板式橡胶支座；//5-活动盆式支座；6-固定盆式支座；7-主拱圈
//  int iPrintRpt;  //0-不输出；1-输出i端；2-输出j端；3-输出(i & j端)  
//  BOOL bRegular; //0-规范桥墩；1-不规范桥墩
//  double dThick;  //橡胶层厚度 单位mm
//  double dFricCoef; // 橡胶支座摩阻系数;
//  double dHorizDis;//盆式支座容许滑动的水平位移 单位mm
//  double dCapBentL0;//盖梁计算跨径 单位mm
//  double dFoundCapaStress;//地基承载能力容许值 单位Mpa
//  double dMaxHorizCapa;//固定支座最大水平承载力 单位N
//  
//  void Initialize()
//  {
//    iElemType=0;
//    iPrintRpt=0;
//    bRegular=0;
//    dThick=0.0;
//    dFricCoef=0.0;
//    dHorizDis=0.0;
//    dCapBentL0=0.0;
//    dFoundCapaStress=0.0;
//    dMaxHorizCapa=0.0;
//  }
//};

struct T_DGN_DUTL_D 
{
    int nPosType;   // Center or I-J (0:I Position, 1:J Position, 2:I,J Position, 3:Center)
    CArray<double,double> arValue;

    void Initialize()
    {
        nPosType    = 3;
        arValue.RemoveAll(); 
    }
    T_DGN_DUTL_D(){}
    T_DGN_DUTL_D(T_DGN_DUTL_D& src)
	{
		*this = src;
	}
    T_DGN_DUTL_D& operator=(const T_DGN_DUTL_D& rhs)
    {
        if(this==&rhs) return *this;
        
        Initialize(); 
        nPosType    = rhs.nPosType ;
        arValue.RemoveAll(); 
        arValue.Copy(rhs.arValue); 
        return *this;
    }
};

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRCDataCtrl_CH 
{
public:
	CRCDataCtrl_CH();  
	virtual ~CRCDataCtrl_CH();
	CCRCDataCtrl m_DataCtrl;
	CCRCForceCtrl* m_ForceCtrl;

protected:
    CDBDoc* m_pDoc;
 	T_CHRP_D m_chrp;
 	T_UNIT_INDEX m_CurrUnit, m_CodeUnit;
	double m_dZero;
    double m_dPi;
    CDesignForceResult* m_pDesignForceResult;
    
public:
    BOOL Get_LcomDataMap(CMap<T_LCOM_K,T_LCOM_K,T_LCOM_K,T_LCOM_K>& aLcomK);
    BOOL Get_ForcDataMap(BOOL isPsc,CMap<_CRC_COMF_K,_CRC_COMF_K,_CRC_COMF_D,_CRC_COMF_D&>& aForcD, BOOL bSeismic=FALSE);
 	BOOL Get_CrcElemListForDgn(BOOL isPsc, CArray<UINT,UINT>& aElemK);	
    double Cng_LengthFromCurrToCode(double dLength, int iUp);
    double Cng_ForceFromCurrToCode(double dForce);
    double Cng_MomentFromCurrToCode(double dMoment);
    double Cng_LengthFromCodeToCurr(double dLength, int iUp);
    double Cng_ForceFromCodeToCurr(double dForce);
    double Cng_MomentFromCodeToCurr(double dMoment);
    double Cng_StressFromCodeToCurr(double dStress);
    double Cng_StressFromCurrToCode(double dStress);

    BOOL Get_CHRCCbcsBaseCurrToCode(T_CBCS_CH_BASE& rData);
    BOOL Get_CHRCCbcsBaseCodeToCurr(T_CBCS_CH_BASE& rData);
    BOOL Get_CHRCCsisBaseCurrToCode(T_CSIS_CH_BASE& rData);
    BOOL Get_CHRCCsisBaseCodeToCurr(T_CSIS_CH_BASE& rData);
    BOOL Get_CHRCCtssBaseCurrToCode(T_CTSS_CH_BASE& rData);
    BOOL Get_CHRCCtssBaseCodeToCurr(T_CTSS_CH_BASE& rData);
    BOOL Get_CHRCCcwsBaseCurrToCode(T_CCWS_CH_BASE& rData,BOOL isBeam);
    BOOL Get_CHRCCcwsBaseCodeToCurr(T_CCWS_CH_BASE& rData,BOOL isBeam);
    BOOL Get_CHRCCsccBaseCurrToCode(T_CSCC_CH_BASE& rData);
    BOOL Get_CHRCCsccBaseCodeToCurr(T_CSCC_CH_BASE& rData);
    BOOL Get_CHRCCtstBaseCurrToCode(T_CTST_CH& rData);
    BOOL Get_CHRCCtstBaseCodeToCurr(T_CTST_CH& rData);
    BOOL Get_CHRCCsncBaseCurrToCode(T_CSNC_CH_BASE& rData);
    BOOL Get_CHRCCsncBaseCodeToCurr(T_CSNC_CH_BASE& rData);
    BOOL Get_CHRCCstcsBaseCurrToCode(T_CSTCS_CH_BASE& rData);
    BOOL Get_CHRCCstcsBaseCodeToCurr(T_CSTCS_CH_BASE& rData);
    BOOL Get_CHRCCstcsCurrToCode(T_CSTCS_CH& rData);
    BOOL Get_CHRCCstcsCodeToCurr(T_CSTCS_CH& rData);
    BOOL Get_CHRCBrlrBaseCurrToCode(T_BRLR_CH_BASE& rData);
    BOOL Get_CHRCBrlrBaseCodeToCurr(T_BRLR_CH_BASE& rData);
    BOOL Get_CHRCBrcrBaseCurrToCode(T_BRCR_CH_BASE& rData);
    BOOL Get_CHRCBrcrBaseCodeToCurr(T_BRCR_CH_BASE& rData);
    BOOL Get_CHRCCrlrBaseCurrToCode(T_CRLR_CH_BASE& rData);
    BOOL Get_CHRCCrlrBaseCodeToCurr(T_CRLR_CH_BASE& rData);

 	BOOL Get_LastStepInCurStage(T_STAG_K StagK, int& iFinalStepNo);
	void Set_UnitIndex(T_UNIT_INDEX& CodeUnit);
    T_UNIT_INDEX Get_CodeUnitIndex();

    void Initialize();
 	BOOL Get_Rcd(T_RC_D_CH& rcd);
	BOOL Get_Matl(int Elemi,_DGN_RC_MATL_CH& matl);
    BOOL Get_Matl(int Elemi,_DGN_RC_MATL_CH_W& matl);

    BOOL Get_ConMatd(T_MATD_D& mData);
    BOOL Get_ConMatd(T_ELEM_K ElemK, T_MATD_D& MatdD);
    BOOL Get_CrcElemListForDgn(BOOL isPsc, CArray<UINT,UINT>& aElemK,BOOL isBeam);
    BOOL Get_BeamSection(int Elemi,int iPosi,_DGN_RC_SECTION_CH& section);
    BOOL Get_BeamSection(int Elemi,int iPosi,_DGN_RC_SECTION_CH_W& section);

    BOOL GetTaperSectRebarInfo(const T_RPSC_D& RpscD,const T_SECT_D& orgSECTD,double& das,double& das1,double& dAs,double& dAs1,
	                         double& dSumBarD,double& dSumBarD2,double& dSumBarD1,double& dSumBarD21);//add by gongxing at 2016-05-09 插值获取变截面钢筋
    BOOL Get_ColumnSection(int Elemi,int iPosi,_DGN_RC_COLUMN_SECTION_CH& section); //iPosi(0:I, 1:J)
    BOOL Get_ColumnSection(int Elemi,int iPosi,_DGN_RC_COLUMN_SECTION_CH_W& section); //iPosi(0:I, 1:J)

	BOOL Get_PSCBeamSection(int Elemi,int iPosi,_DGN_RC_SECTION_CH& section);

    BOOL Get_PrintOption(int Elemi,int iPosi, UINT PrintItem);
    BOOL Get_PSCPrintOption(int Elemi,int iPosi, UINT PrintItem);
    BOOL Get_CHRcCbcs(int Elemi,T_CBCS_CH& cbcs);
    BOOL Get_CHRcCsis(int Elemi,T_CSIS_CH& csis);
    BOOL Get_CHRcCtss(int Elemi,T_CTSS_CH& ctss);
    BOOL Get_CHRcCcws(int Elemi,BOOL bisBeam,T_CCWS_CH& ccws);
	BOOL Get_CHRcColumnCcws(int Elemi,T_CCWS_CH_BASE ccwsBase[6]);//column
    BOOL Get_CHRcBrlr(int Elemi,T_BRLR_CH& brlr);
    BOOL Get_CHRcBrcr(int Elemi,T_BRCR_CH& brcr);
    BOOL Get_CHRcCstcs(int Elemi,BOOL isCscs,T_CSTCS_CH& cstcs);//偏心受拉压验算
    BOOL Get_CHRcCrlr(int Elemi,T_CRLR_CH& crlr);
    BOOL Get_CrcCscc(int Elemi, T_CSCC_CH& cscc);//得到短暂状况(施工荷载)的正截面压应力
    BOOL Get_CrcCtst(int ElemK,int sectioni,int iStaZref,double dh,double dStaz, double dEs,double dEc,T_CTST_CH& ctst);
	BOOL Get_CrcCsnc(int Elemi, T_CSNC_CH& csnc);//得到短暂状况(施工荷载)的中性轴主拉应力

	BOOL Get_CrcCscc(int Elemi/*,_DGN_RC_SECTION_CH& section*/,double fsd,double fsd1,double fcd,double dEs,double dEc,
		                T_CSCC_CH& cscc,CArray<T_CTST_CH,T_CTST_CH>& ctst);//得到短暂状况(施工荷载)的正截面压应力 add by maxiao 2007.03.28

    //Solve Quadratic which as ax^2+bx+c=0.if no solution,return 0.0;else return Max(x1,x2).
    double Solve_Quadratic(double a,double b,double c);
    
	//////////////////////////////////////////////////////////////////////////
	//for Railway design
    BOOL Get_RailBeamForce(T_ELEM_K ElemK,int iposi,T_RAIL_BEAM_FORCE& beam_mv,BOOL bCSTp=TRUE, BOOL bRemoveGS = FALSE);
    BOOL Get_RailColumnForce(T_ELEM_K ElemK,int iposi,T_RAIL_COLUMN_FORCE& column_mnv,BOOL isSeismic);
	BOOL GetBeamOutRebarDistance(int Elemi,double dh,double& m_das_d,double& m_da_s_d);
	BOOL GetColumnOutRebarDistance(int Elemi,double dh,double& m_das_d,double& m_da_s_d);
        void GetFactorGS_Sub(T_KEY LcombK, T_KEY Key1, T_KEY Key2, T_KEY KeySum, double dMutiRatio, double &dRatio1, double &dRatio2);
        void GetFactorGS(int iDgnLcomK, double &dRatio1, double &dRatio2, CMapEx<T_KEY, T_KEY, double, double> &mapExistRatio1, CMapEx<T_KEY, T_KEY, double, double> &mapExistRatio2);
	//////////////////////////////////////////////////////////////////////////
	//Railway psc
	BOOL Get_PscDegree(T_ELEM_K ElemK, _DGN_STRS_CRC& StrLcomMax, _DGN_STRS_CRC& StrSgld,int& iSerialLcom);
	BOOL Get_Con_CS(T_ELEM_K ElemK, T_CON_CS_CH& concs);
	BOOL Get_TendonMatl(T_TDNA_K TdnaK,int& TendonType ,double& dfpk);
	BOOL Get_PscSect(T_ELEM_K ElemK,int iPosi,BOOL isBeam, _TB_PSC_SECT& sect);
	BOOL Get_ForceFordgn(T_ELEM_K ElemK,UINT iDimType,_DGN_FORC_CRC aForce[12],BOOL bCSTp=TRUE);
	BOOL Get_TSelfStressFordgn(T_ELEM_K ElemK,int iLCom,UINT iDimType,BOOL isBeam,_DGN_STRS_CRC& aStress);   //温度自应力

	BOOL Get_PscNpe(T_ELEM_K ElemK,int iPosi,double& dNp,double& dNpey,double& dNpez);
	BOOL Get_ForceStageCS(T_ELEM_K ElemK,UINT iDimType,CArray<_DGN_FORC_CRC,_DGN_FORC_CRC>& aForce);
	BOOL Get_TBConMatl(T_ELEM_K Elemi,double& dalpha, double& dfc,double& dfct);
	BOOL Get_ForceOfTpTs(T_ELEM_K ElemK,int iDimType,_DGN_FORC_CRC& ForceTP,_DGN_FORC_CRC& ForceTS);
    BOOL Get_ForceOfCSALL(T_ELEM_K ElemK,int iDimType,_DGN_FORC_CRC& ForceCSALL);//get cs(sum)  
	BOOL Get_YpZp(T_TDNA_K TdnaK,T_ELEM_K ElemK,int iPosi,double& dYp,double& dZp);
    //BOOL Get_TBdn(T_ELEM_K Elemi,BOOL isbeam, double& dn);//表table5.1.3 P19
    BOOL Get_TBdn(T_ELEM_K ElemK,T_TDNA_K TdnaK,double& dn);
	BOOL Get_MVForceFordgn(T_ELEM_K ElemK,UINT iDimType,_DGN_FORC_CRC aForce[12]);
	BOOL Get_fAezData(T_ELEM_K elemK,int j,double dMy,double dN,double dh,double& dfpApep,double& dfp_Ap_ep,double& dfsAses,double& dfs_As_es,double& dfpApZp,double& dfp_Ap_Zp,double& dfsAsZs,double& dfs_As_Zs);					
	BOOL Get_fAaData(T_ELEM_K elemK,int j,double dCzp, double dCzm, double& dfpApap,double& dfp_Ap_ap,double& dfsAsas,double& dfs_As_as,double& dfpAp,double& dfp_Ap,double& dfsAs,double& dfs_As);
	BOOL Get_con_fracture(int ElemK, int iSafeType, T_BEAM_CON_INCLINE_FRACTURE_CH& con_incline_fracture_elem);
	BOOL Get_rebarData(T_ELEM_K elemK,int j,double dh,double& dAs, double& dAs1,double& das,double& das1,double& dU,double& dat,double& dab);
	
public:
    BOOL Is_CalcPscItem(T_ELEM_K ElemK, UINT CheckItem);
    BOOL Is_CalcItem(T_ELEM_K ElemK, UINT CheckItem);
	double Get_Phi(INT itype, double dl0, double db);//JTG D62-04 Table 5.3.1
    BOOL Get_ChkPscFlag(T_ELEM_K ElemK, int iPosi, UINT CheckItem);
	BOOL Get_ChkFlag(T_ELEM_K ElemK, int iPosi, UINT CheckItem);

    //////////////////////////////////////////////////////////////////////////
    //add for seismic design 2009.04
    BOOL Get_ElemListForSeismicDgn(CArray<UINT,UINT>& ElemKList);//单元List
    BOOL Get_GeneralLinkListForSeismicDgn(CArray<UINT,UINT>& GeneralLinkList);//一般连接List
    BOOL Get_ElasticLinkListForSeismicDgn(CArray<UINT,UINT>& ElasticLinkList);//弹性连接List
    
    BOOL Get_SeismicComp(BOOL bE1,int Elemi,BOOL isCscs,T_CSTCS_CH& cstcs);//拉力及压力
    BOOL Get_SeismicCbcs(BOOL bE1,int Elemi,T_CBCS_CH& cbcs);//E1作用下弯矩
    BOOL Get_SeismicShear(BOOL bE1,int ElemK,T_CSIS_CH& csis);//作用下剪力	
//  BOOL Get_SeismicE1FounD(int ElemK,T_FOUND_CH& funddata);//E1作用下基础受力	取abs(Pu2)最大
    BOOL Get_SeismicCapBeamCbcs(BOOL bE1,int Elemi,T_CBCS_CH& cbcs);//E2 Capbeam's Moment
    BOOL Get_SeismicCapBeamShear(BOOL bE1,int ElemK,T_CSIS_CH& csis);//E2 Capbeam's Shear
    
    BOOL Get_SeismicForce(T_ELEM_K ElemK, BOOL bE1, _DGN_FORC_CRC aForce[12]);//获得荷载组合内力
    
    //单位换算
    BOOL Get_SeisFoundBaseCodeToCurr(T_FOUND_CH_BASE& rData);
    BOOL Get_SeisSupportDispCodeToCurr(T_SUPPORTDISP_CH& rData);
    BOOL Get_SeisSupportForceCodeToCurr(T_SUPPORTFORCE_CH& rData);
    BOOL Get_SeismicFrustaTopBaseCodeToCurr(T_FRUSTATOPDISP_CH_BASE& rData);
    
    BOOL Get_SeismicD(T_JTG_SEISMIC_D& dgnD);//获得设计参数
    BOOL GetElemSeisChkType(T_ELEM_K ElemK, T_RSDM_D& SeisType);//获得单元的构件类别
    BOOL GetGeneralLinkSeisChkType(T_NLNK_K NlnkK, T_RSDM_D& SeisType);//获得单元的构件类别
    BOOL GetElasticLinkSeisChkType(T_ELNK_K ElnkK, T_RSDM_D& SeisType);//获得单元的构件类别  
    //Get Max SelfWeight of Elemk
    double Get_MGofElem (int iElemK, int ij);
    //Get parameters data from pier;
    BOOL GetParaFromPier(int iMember,BOOL& bOnlyBot, double& dHn, BOOL& bDoublePier, double & dDisp, int& iOutput);
    //Get parameters data from cap Beam;  
    BOOL GetParaFromCapBeam(int iMember,double& dL0, int& iOutput);

private:
	// 傈眉谅钎拌 X-Y乞搁困俊辑 秦寸 Local绵规氢俊 措茄 乞搁惑狼 窜困氦磐甫 逞败淋(nAxis(x=1, y=2, z=3), dirVectorN(X-Y乞搁惑狼 X,Y蔼))
	//   nDataType : elemK狼 Data屈怕 (1:Elem, 2:Elnk, 3:Nlnk)
	BOOL   GetLocalDisplacementOnGlobalPlane(UINT elemK, T_DISP_D& DispIMax, T_DISP_D& DispIMin, T_DISP_D& DispJMax, T_DISP_D& DispJMin, double& dDy, double& dDz, int nDataType=1);
	BOOL   GetLocalDirectionOnGlobalPlane(UINT elemK, int nAxis, double dirVectorN[2], int nDataType=1);
	double GetLocalDirectionValue(double dirVectorN[2], double dX, double dY);
    BOOL ConvertRPSCByPosition(const T_RPSC_D& RpscD,BOOL bEndI,CArray<T_RPSC_MBAR,T_RPSC_MBAR>& rBars);    //add by juhonghua(MITC) 2012.03.05
    BOOL GetPierTopNodeID(const CArray<T_ELEM_K, T_ELEM_K>& aElemList,T_NODE_K& nodeKey);
//Need Korea to do
public:
	//--------------------------Get check Member or Element List-------------------------------------------------------
    //Get Pier Member List
    BOOL Get_PierMemberListForSeismicDgn(CArray<T_RSDL_K,T_RSDL_K>& MemberList);// Member List
    //Get Abutment Element List
    BOOL Get_AbutmentElemListForSeismicDgn(CArray<T_ELEM_K,T_ELEM_K>& ElemKList);//ElemK List
    //Get Foundation Element List
    BOOL Get_FoundationElemListForSeismicDgn(CArray<T_ELEM_K,T_ELEM_K>& ElemKList);//ElemK List
    //Get Cap beam Member List
    BOOL Get_CapBeamMemberListForSeismicDgn(CArray<T_RSDL_K,T_RSDL_K>& MemberList);// Member List
    //Get Rubber Link List
    BOOL Get_RubberLinkListForSeismicDgn(CArray<T_ELNK_K,T_ELNK_K>& LinkKList);// Link List
    //Get Active Link List
    BOOL Get_ActiveLinkListForSeismicDgn(CArray<T_NLNK_K,T_NLNK_K>& LinkKList);// Link List
    //Get Fixed Link List
    BOOL Get_FixedLinkListForSeismicDgn(CArray<T_ELNK_K,T_ELNK_K>& LinkKList);// Link List
    //Get Main Arch Element List
    BOOL Get_MainArchElemListForSeismicDgn(CArray<T_ELEM_K,T_ELEM_K>& ElemKList);// ElemK List  
    //Get Pier's element list by pierID
    BOOL Get_PierElemListByPierID (T_RSDL_K iMember, CArray<T_ELEM_K,T_ELEM_K>& ElemKList);//ElemK List
    //Get CapBeam's element list by CapBeamID
    BOOL Get_CapBeamElemListByCapBeamId(T_RSDL_K iMember, CArray<T_ELEM_K,T_ELEM_K>& ElemKList);//ElemK List

    //--------------------------Get Stirrup data ------------------------------------------------------------------------
    //Get Stirrup data; iElk-Element id//stirrup(用来计算phi-u，phi-y)
    BOOL Get_SeismicStirrup(T_ELEM_K iElk, double& dAx, double& dAy, double& dDx, double& dDy);  

    //--------------------------Get CapBeam and Pier's data ------------------------------------------------------------------------
    //Get PierList which connected to CapBeam. We can find CapBeam's connected Node, find Pier by Node
    BOOL Get_PierListofCapBeam(T_RSDL_K iCapBeamMember, CArray<T_RSDL_K,T_RSDL_K>&PierMemberList);
    //Get Pier's End Elemk and ij. ij: 0-Top, 1-bottom;  (x,y,z), small z is bottom
    BOOL GetElemkijFromPierMemberID(T_RSDL_K iMember, T_ELEM_K elemk[2], int ij[2]);
    //Get CapBeam's End Elemk and ij. ij: 0-one End, 1-other End;
    BOOL GetElemkijFromCapBeamMemberID(T_RSDL_K iMember, int elemk[2], int ij[2]);

    //--------------------------Get Load combination data ------------------------------------------------------------------------
    //Get Element's seismic Load combination Force (E1 same to E2)
    BOOL Get_SeismicForce(T_ELEM_K ElemK, _DGN_FORC_CRC Force[12]); 
    //Get Element's Permanent Load Force (E1 same to E2) [D+tendon(CS)+EH+EV+WP+B]
    BOOL Get_PermanentSeismicForce (T_ELEM_K ElemK, _DGN_FORC_CRC Force[12]); 
    //Get Element's self weight Load Force (E1 same to E2) 
    BOOL Get_WeightSeismicForce (T_ELEM_K ElemK, _DGN_FORC_CRC Force[12]); 

    //--------------------------Get Link    data ------------------------------------------------------------------------
    //Get Elastic Link Disp. x-Longitudinal  y-Transverse	
    BOOL GetElasticLinkDisp(T_ELNK_K ElnkK,BOOL bE1,CArray<T_SEIS_LINK_DISP,T_SEIS_LINK_DISP>& aLinkDispList);
    //Get Elastic Link Force(水平向)
    BOOL GetElasticLinkForce(T_ELNK_K ElnkK,BOOL bE1,CArray<T_SEIS_LINK_FORCE,T_SEIS_LINK_FORCE>& aLinkForceList);	
    //Get General Link Disp. x-Longitudinal  y-Transverse	
    BOOL GetGeneralLinkDisp(T_NLNK_K NlnkK,BOOL bE1,CArray<T_SEIS_LINK_DISP,T_SEIS_LINK_DISP>& aLinkDispList);
 
    //--------------------------Get E2  data ------------------------------------------------------------------------
    ////Get Maxθp and it's elements. 0-Longitudinal;1-Transverse.
    BOOL Get_SeismicE2Theta(T_RSDL_K iMember,T_CORNERDISP_CH& data);
	BOOL Get_Lp_JTG(T_RSDL_K iMember, BOOL bTop, double& dLp); // 家己腮瘤 辨捞甫 拌魂窃
    //Get Pier's top Disp..//取得规则桥墩墩顶位移
    BOOL Get_SeismicE2FrustaTopDisp(T_RSDL_K iMember,T_FRUSTATOPDISP_CH& data);	

    //--------------------------Get Pier's other data ------------------------------------------------------------------------
    //Get M by P-M characteristic.-求最不利轴力对应的弯矩 (dP : compression(-), tension(+))
    BOOL GetMfromPierPM(T_RSDL_K iMember, BOOL bTop, BOOL bLongitudinal, double dP, double& dM);  
    ////Get all the End force of Pier member. _DGN_FORC_CRC-0:Top, 1:bottom
    BOOL GetEndForcefromPierMember(T_RSDL_K iMember, CArray <_DGN_FORC_CRC,_DGN_FORC_CRC>& Force);
    
    //--------------------------Get Foundation design force data ------------------------------------------------------------------------
    //Get foundation design force List
    BOOL Get_SeismicFounD(int ElemK, T_FOUND_CH& data);

	//--------------------------Get Curvature data ------------------------------------------------------------------------
	// Get Curvature for Yield & Ultimate (0:yy 1:zz)
	BOOL Get_RsdlCurvature(T_RSDL_K iMember, double dPhiy[2], double dPhiu[2]);

/////////////////////////////////////////////////////////////////////////////////////////////////

	double Get_ElemLength(T_ELEM_K ElemK);
	double Get_ElemListLength(CArray<T_ELEM_K, T_ELEM_K>& arElemK);
	BOOL   Get_ElemPosition(CArray<T_ELEM_K, T_ELEM_K>& arSortElemK, double dL, BOOL bStartI, T_ELEM_K& PosElemK, double& dRate);
	BOOL   Get_Theta(T_THIS_K ThisK, T_NODE_K NodeKI1, T_NODE_K NodeKI2, double dRate1, T_NODE_K NodeKJ1, T_NODE_K NodeKJ2, double dRate2, double dMaxTheta[2], int nMaxTimeIndex[2]);// [0]yy [1]zz
	BOOL   Get_Theta(T_THIS_K ThisK, CArray<T_ELEM_K, T_ELEM_K>& arElemK, double dRateS, double dRateE, double dMaxTheta[2], int nMaxTimeIndex[2]);// [0]yy [1]zz
	BOOL   GetHngeInfoBaseDataByComp(int nComp,int &nIndex,CArray<T_INELASTICDYN_HINGE_INFO_BASE,T_INELASTICDYN_HINGE_INFO_BASE &> &HngeInfo);
	BOOL   GetInElasticData(T_DGN_DUTL_D &DutlD,T_INELASTICDYN_HINGE_INFO_BASE *pHngeInfoBase, int nTimeIndex, int nComp);
	double GetInElasticValue(int nHingeType,int nTimeIndex,int nInElasticSeqId, int nComp,int nHngeValType);
	double GetInElasticTheta(T_DGN_DUTL_D &DutlD, double dRateI, double dRateJ);
	BOOL   Get_DispQE2(T_NODE_K NodeK, UINT iSerialLcom, double& dDispX, double& dDispY, double& dDispZ, double& dRX, double& dRY, double& dRZ);

    //Get T and Tg
	BOOL   Get_TTg(double& dT, double& dTg);
    void GetEqForceExTempForce(int iElemK,int iLCombID,_DGN_FORC_CRC& EqForce);
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined()


