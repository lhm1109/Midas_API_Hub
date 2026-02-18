// Beam.h: interface for the CRCrailway class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BEAM_H__E96D7AA4_F53B_463D_A782_F849C151271E__INCLUDED_)
#define AFX_BEAM_H__E96D7AA4_F53B_463D_A782_F849C151271E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include <afxtempl.h>
#include "RCDataCtrl_CH.h"
#include "CRCFileBase.h"
//#define PI 4*atan(1.0)


struct T_Beam_concrete_c
{
    UINT nElemK;
    UINT nPart;                  
	double db,dh,db_f,dbf,dh_f,dhf,das,da_s,dAs,dA_s,das_d,da_s_d,dLo,dd,dAsR,dR,dasR;
	double dIy,dIz,dCzp,dCzm,dCyp,dCym;
	double dconcrete_c,dconcrete_b,dconcrete_shear,dconcrete_tp,drebar,dconcrete_u,dconcrete_wf;
    double dK1,da_K2,dK,dEc,dEs,dm,dn,dK3;
	void Initialize()
    {
        nElemK = 0;
        nPart = 0;
		db=dh=db_f=dbf=dh_f=dhf=das=da_s=dAs=dA_s=das_d=da_s_d=dLo=dd=dAsR=dR=dasR=.0;
		dIy=dIz=dCzp=dCzm=dCyp=dCym=.0;
		dconcrete_c=dconcrete_b=dconcrete_shear=dconcrete_tp=drebar=dconcrete_u=dconcrete_wf=.0;
		dK1=da_K2=dK=dEc=dEs=dm=dn=dK3=.0;
    }  
};

struct T_Beam_shear
{
    UINT nElemK;
    UINT nPart;                  
	double db,dh,db_f,dbf,dh_f,dhf,das,da_s,dAs,dA_s,das_d,da_s_d,dLo,dd,dAsR,dR,dasR;
	double dIy,dIz,dCzp,dCzm,dCyp,dCym;
	double dconcrete_c,dconcrete_b,dconcrete_shear,dconcrete_tp,drebar,dconcrete_u,dconcrete_wf;
    double dK1,da_K2,dK,dEc,dEs,dm,dn,dK3;

    void Initialize()
    {
        nElemK = 0;
        nPart = 0;
		db=dh=db_f=dbf=dh_f=dhf=das=da_s=dAs=dA_s=das_d=da_s_d=dLo=dd=dAsR=dR=dasR=.0;
		dIy=dIz=dCzp=dCzm=dCyp=dCym=.0;
		dconcrete_c=dconcrete_b=dconcrete_shear=dconcrete_tp=drebar=dconcrete_u=dconcrete_wf=.0;
		dK1=da_K2=dK=dEc=dEs=dm=dn=dK3=.0;
    }  
};

struct T_Beam_shear_t
{
    UINT nElemK;
    UINT nPart;                  
	double db,dh,db_f,dbf,dh_f,dhf,das,da_s,dAs,dA_s,das_d,da_s_d,dLo,dd,dAsR,dR,dasR;
	double dIy,dIz,dCzp,dCzm,dCyp,dCym;
	double dconcrete_c,dconcrete_b,dconcrete_shear,dconcrete_tp,drebar,dconcrete_u,dconcrete_wf;
    double dK1,da_K2,dK,dEc,dEs,dm,dn,dK3;
	void Initialize()
    {
        nElemK = 0;
        nPart = 0;
		db=dh=db_f=dbf=dh_f=dhf=das=da_s=dAs=dA_s=das_d=da_s_d=dLo=dd=dAsR=dR=dasR=.0;
		dIy=dIz=dCzp=dCzm=dCyp=dCym=.0;
		dconcrete_c=dconcrete_b=dconcrete_shear=dconcrete_tp=drebar=dconcrete_u=dconcrete_wf=.0;
		dK1=da_K2=dK=dEc=dEs=dm=dn=dK3=.0;
    }   
};

struct T_Beam_shear_b
{
    UINT nElemK;
    UINT nPart;                  
	double db,dh,db_f,dbf,dh_f,dhf,das,da_s,dAs,dA_s,das_d,da_s_d,dLo,dd,dAsR,dR,dasR;
	double dIy,dIz,dCzp,dCzm,dCyp,dCym;
	double dconcrete_c,dconcrete_b,dconcrete_shear,dconcrete_tp,drebar,dconcrete_u,dconcrete_wf;
    double dK1,da_K2,dK,dEc,dEs,dm,dn,dK3;
	void Initialize()
    {
        nElemK = 0;
        nPart = 0;
		db=dh=db_f=dbf=dh_f=dhf=das=da_s=dAs=dA_s=das_d=da_s_d=dLo=dd=dAsR=dR=dasR=.0;
		dIy=dIz=dCzp=dCzm=dCyp=dCym=.0;
		dconcrete_c=dconcrete_b=dconcrete_shear=dconcrete_tp=drebar=dconcrete_u=dconcrete_wf=.0;
		dK1=da_K2=dK=dEc=dEs=dm=dn=dK3=.0;
    }
};

struct T_Beam_rebar_s
{
    UINT nElemK;
    UINT nPart;                  
	double db,dh,db_f,dbf,dh_f,dhf,das,da_s,dAs,dA_s,das_d,da_s_d,dLo,dd,dAsR,dR,dasR;
	double dIy,dIz,dCzp,dCzm,dCyp,dCym;
	double dconcrete_c,dconcrete_b,dconcrete_shear,dconcrete_tp,drebar,dconcrete_u,dconcrete_wf;
    double dK1,da_K2,dK,dEc,dEs,dm,dn,dK3;
	void Initialize()
    {
        nElemK = 0;
        nPart = 0;
		db=dh=db_f=dbf=dh_f=dhf=das=da_s=dAs=dA_s=das_d=da_s_d=dLo=dd=dAsR=dR=dasR=.0;
		dIy=dIz=dCzp=dCzm=dCyp=dCym=.0;
		dconcrete_c=dconcrete_b=dconcrete_shear=dconcrete_tp=drebar=dconcrete_u=dconcrete_wf=.0;
		dK1=da_K2=dK=dEc=dEs=dm=dn=dK3=.0;
    }   
};

struct T_Beam_crack_rc_rail
{
    UINT nElemK;
    UINT nPart;    
	double dMy,dMy1,dMy2;
	UINT t;
	double db,dh,db_f,dbf,dh_f,dhf,das,da_s,dAs,dA_s,das_d,da_s_d,dLo,dd,dAsR,dR,dasR;
	double dIy,dIz,dCzp,dCzm,dCyp,dCym;
	double dconcrete_c,dconcrete_b,dconcrete_shear,dconcrete_tp,drebar,dconcrete_u,dconcrete_wf;
    double dK1,da_K2,dK,dEc,dEs,dm,dn,dK3;
	void Initialize()
    {
        nElemK = 0;
        nPart = 0;
		dMy=dMy1=dMy2 =.0;
		t     =0;
		db=dh=db_f=dbf=dh_f=dhf=das=da_s=dAs=dA_s=das_d=da_s_d=dLo=dd=dAsR=dR=dasR=.0;
		dIy=dIz=dCzp=dCzm=dCyp=dCym=.0;
		dconcrete_c=dconcrete_b=dconcrete_shear=dconcrete_tp=drebar=dconcrete_u=dconcrete_wf=.0;
		dK1=da_K2=dK=dEc=dEs=dm=dn=dK3=.0;
    }   
};

struct T_Column_concrete_c
{
    UINT nElemK;
    UINT nPart;    
	UINT t;
	BOOL bseismic;
	double db,dh,db_f,dbf,dh_f,dhf,das,da_s,dAs,dA_s,das_d,da_s_d,dLo,dd,dAsR,dR,dasR;
	double dIy,dIz,dCzp,dCzm,dCyp,dCym;
	double dconcrete_c,dconcrete_b,dconcrete_shear,dconcrete_tp,drebar,dconcrete_u,dconcrete_wf;
    double dK1,da_K2,dK,dEc,dEs,dm,dn,dK3;
	void Initialize()
    {
        nElemK = 0;
        nPart = 0;
		t     =0;
		bseismic = FALSE;
		db=dh=db_f=dbf=dh_f=dhf=das=da_s=dAs=dA_s=das_d=da_s_d=dLo=dd=dAsR=dR=dasR=.0;
		dIy=dIz=dCzp=dCzm=dCyp=dCym=.0;
		dconcrete_c=dconcrete_b=dconcrete_shear=dconcrete_tp=drebar=dconcrete_u=dconcrete_wf=.0;
		dK1=da_K2=dK=dEc=dEs=dm=dn=dK3=.0;
    }   
};

struct T_Column_shear_rc_rail
{
    UINT nElemK;
    UINT nPart;    
	UINT t;
	double dMy, dNx;
	double db,dh,db_f,dbf,dh_f,dhf,das,da_s,dAs,dA_s,das_d,da_s_d,dLo,dd,dAsR,dR,dasR;
	double dIy,dIz,dCzp,dCzm,dCyp,dCym;
	double dconcrete_c,dconcrete_b,dconcrete_shear,dconcrete_tp,drebar,dconcrete_u,dconcrete_wf;
    double dK1,da_K2,dK,dEc,dEs,dm,dn,dK3;
	void Initialize()
    {
        nElemK = 0;
        nPart = 0;
		t     =0;
		dMy=dNx=.0;
		db=dh=db_f=dbf=dh_f=dhf=das=da_s=dAs=dA_s=das_d=da_s_d=dLo=dd=dAsR=dR=dasR=.0;
		dIy=dIz=dCzp=dCzm=dCyp=dCym=.0;
		dconcrete_c=dconcrete_b=dconcrete_shear=dconcrete_tp=drebar=dconcrete_u=dconcrete_wf=.0;
		dK1=da_K2=dK=dEc=dEs=dm=dn=dK3=.0;
    }   
};

struct T_Column_rebar_c
{
    UINT nElemK;
    UINT nPart;    
	UINT t;
	BOOL bseismic;
	double db,dh,db_f,dbf,dh_f,dhf,das,da_s,dAs,dA_s,das_d,da_s_d,dLo,dd,dAsR,dR,dasR;
	double dIy,dIz,dCzp,dCzm,dCyp,dCym;
	double dconcrete_c,dconcrete_b,dconcrete_shear,dconcrete_tp,drebar,dconcrete_u,dconcrete_wf;
    double dK1,da_K2,dK,dEc,dEs,dm,dn,dK3;
	void Initialize()
    {
        nElemK = 0;
        nPart = 0;
		t     =0;
		bseismic = FALSE;
		db=dh=db_f=dbf=dh_f=dhf=das=da_s=dAs=dA_s=das_d=da_s_d=dLo=dd=dAsR=dR=dasR=.0;
		dIy=dIz=dCzp=dCzm=dCyp=dCym=.0;
		dconcrete_c=dconcrete_b=dconcrete_shear=dconcrete_tp=drebar=dconcrete_u=dconcrete_wf=.0;
		dK1=da_K2=dK=dEc=dEs=dm=dn=dK3=.0;
    }    
};

struct T_Column_rebar_s
{
    UINT nElemK;
    UINT nPart;    
	UINT t;
	BOOL bseismic;
	double db,dh,db_f,dbf,dh_f,dhf,das,da_s,dAs,dA_s,das_d,da_s_d,dLo,dd,dAsR,dR,dasR;
	double dIy,dIz,dCzp,dCzm,dCyp,dCym;
	double dconcrete_c,dconcrete_b,dconcrete_shear,dconcrete_tp,drebar,dconcrete_u,dconcrete_wf;
    double dK1,da_K2,dK,dEc,dEs,dm,dn,dK3;
	void Initialize()
    {
        nElemK = 0;
        nPart = 0;
		t     =0;
		bseismic = FALSE;
		db=dh=db_f=dbf=dh_f=dhf=das=da_s=dAs=dA_s=das_d=da_s_d=dLo=dd=dAsR=dR=dasR=.0;
		dIy=dIz=dCzp=dCzm=dCyp=dCym=.0;
		dconcrete_c=dconcrete_b=dconcrete_shear=dconcrete_tp=drebar=dconcrete_u=dconcrete_wf=.0;
		dK1=da_K2=dK=dEc=dEs=dm=dn=dK3=.0;
    }    
};

struct T_Column_crack
{
    UINT nElemK;
    UINT nPart;    
	UINT t;
	double dMy, dNx, dMy1, dMy2;
	double db,dh,db_f,dbf,dh_f,dhf,das,da_s,dAs,dA_s,das_d,da_s_d,dLo,dd,dAsR,dR,dasR;
	double dIy,dIz,dCzp,dCzm,dCyp,dCym;
	double dconcrete_c,dconcrete_b,dconcrete_shear,dconcrete_tp,drebar,dconcrete_u,dconcrete_wf;
    double dK1,da_K2,dK,dEc,dEs,dm,dn,dK3;
	void Initialize()
    {
        nElemK = 0;
        nPart = 0;
		t     =0;
		dMy=dNx=dMy1=dMy2=.0;
		db=dh=db_f=dbf=dh_f=dhf=das=da_s=dAs=dA_s=das_d=da_s_d=dLo=dd=dAsR=dR=dasR=.0;
		dIy=dIz=dCzp=dCzm=dCyp=dCym=.0;
		dconcrete_c=dconcrete_b=dconcrete_shear=dconcrete_tp=drebar=dconcrete_u=dconcrete_wf=.0;
		dK1=da_K2=dK=dEc=dEs=dm=dn=dK3=.0;
    }   
};

struct T_Column_stability
{
    UINT nElemK;
    UINT nPart;    
	UINT t;
	BOOL bseismic;
	double db,dh,db_f,dbf,dh_f,dhf,das,da_s,dAs,dA_s,das_d,da_s_d,dLo,dd,dAsR,dR,dasR;
	double dIy,dIz,dCzp,dCzm,dCyp,dCym;
	double dconcrete_c,dconcrete_b,dconcrete_shear,dconcrete_tp,drebar,dconcrete_u,dconcrete_wf;
    double dK1,da_K2,dK,dEc,dEs,dm,dn,dK3;
	void Initialize()
    {
        nElemK = 0;
        nPart = 0;
		t     =0;
		bseismic = FALSE;
		db=dh=db_f=dbf=dh_f=dhf=das=da_s=dAs=dA_s=das_d=da_s_d=dLo=dd=dAsR=dR=dasR=.0;
		dIy=dIz=dCzp=dCzm=dCyp=dCym=.0;
		dconcrete_c=dconcrete_b=dconcrete_shear=dconcrete_tp=drebar=dconcrete_u=dconcrete_wf=.0;
		dK1=da_K2=dK=dEc=dEs=dm=dn=dK3=.0;
    }    
};

struct T_Column_e
{
    UINT nElemK;
    UINT nPart;    
	UINT t;
	double dMy,dNx,dMz;
	BOOL bseismic;
	double db,dh,db_f,dbf,dh_f,dhf,das,da_s,dAs,dA_s,das_d,da_s_d,dLo,dd,dAsR,dR,dasR;
	double dIy,dIz,dCzp,dCzm,dCyp,dCym;
	double dconcrete_c,dconcrete_b,dconcrete_shear,dconcrete_tp,drebar,dconcrete_u,dconcrete_wf;
    double dK1,da_K2,dK,dEc,dEs,dm,dn,dK3;
	void Initialize()
    {
        nElemK = 0;
        nPart = 0;
		t     =0;
		bseismic = FALSE;
		dMy=dNx=dMz=.0;
		db=dh=db_f=dbf=dh_f=dhf=das=da_s=dAs=dA_s=das_d=da_s_d=dLo=dd=dAsR=dR=dasR=.0;
		dIy=dIz=dCzp=dCzm=dCyp=dCym=.0;
		dconcrete_c=dconcrete_b=dconcrete_shear=dconcrete_tp=drebar=dconcrete_u=dconcrete_wf=.0;
		dK1=da_K2=dK=dEc=dEs=dm=dn=dK3=.0;
    }    
};

struct T_Seismic_ductility
{
    UINT nElemK;
    UINT nPart;                  

    void Initialize()
    {
        nElemK = 0;
        nPart = 0;
    }  
};


class CRCrailway : public CCRCFileBase  
{
public:
	  //以下需从civil中读取的数据
	double m_db;//宽度
	double m_dh;//高度
    double m_db_f;//上翼缘宽度
	double m_dbf;//下翼缘宽度
	double m_dh_f;//上翼缘高度
	double m_dhf;//下翼缘高度
	double m_das;//受拉钢筋保护层厚度
	double m_da_s;//受压钢筋保护层厚度
	double m_dAs;//受拉钢筋面积
	double m_dA_s;//受压钢筋面积
	double m_das_d;//最外层受拉钢筋至混凝土受拉边缘的距离
	double m_da_s_d;//最外层受压钢筋至混凝土受压边缘的距离
    double m_dLo;//压杆的计算长度
    double m_dd;//受拉钢筋直径
    double m_dAsR;//圆形截面纵向钢筋面积
	double m_dR;//圆形截面半径
	double m_dasR;//圆形钢筋保护层厚度
	double m_dIy;//对y轴的惯性距
	double m_dIz;//对z轴的惯性距
    double m_dCzp;
	double m_dCzm;
	double m_dCyp;
	double m_dCym;
	double dNx[6],dMy[6],dMy1[6],dMy2[6],dMz[6],dVz[6];//截面荷载组合
	UINT iNx[6],iMy[6],iMz[6],iVz[6];//截面荷载组合ID

	double m_dK1;//钢筋表面形状影响系数         规范 p29（界面上用户输入）   
    double m_da_K2;//关于荷载特征影响K2的系数   规范 p29（界面上用户输入）
	BOOL m_bcheckitem[15];//验算的项目（界面上用户输入）
	BOOL isaddforce[6]; //是否为主力加附加力组合
 
public:
	//根据civil读取的数据，以下是程序计算所得参数
    double m_dK;//安全系数                                          规范 p27    
    double m_dEc;//混凝土受压弹性模量                               规范表3.1.5 P9
	double m_dEs;//钢筋的弹性模量                                   规范表3.2.4 P11
	double m_dconcrete_c;//混凝土中心受压容许压应力                 规范表5.2.1 P21
	double m_dconcrete_b;//混凝土弯曲受压及偏心受压容许压应力       规范表5.2.1 P21
	double m_dconcrete_shear;//混凝土纯剪容许应力                   规范表5.2.1 P21
	double m_dconcrete_tp;//混凝土主拉应力容许值                    规范表5.2.1 P21
    double m_drebar;//钢筋的容许应力                                规范 p20－22    
    double m_dconcrete_u;//截面的容许最小配筋率                     规范表5.1.2 p19
	double m_dconcrete_wf;//裂缝宽度容许值                          规范表5.2.7 p29
    double m_dm;//受压钢筋计算强度与混凝土抗压极限强度之比          规范表5.2.3-1 p23
	double m_dn;//钢筋弹性模量与混凝土的变形模量之比                规范表5.1.2 p19
	double m_dK3;//截面形状系数                                     规范 p31                               

	CStdioFile file; 

// [21/02/2007 : maxiao] For CIVIL(CH Railway Design) 2007
protected:
    CDBDoc* m_pDoc;
    CArray<UINT, UINT> m_elemlist;
    CRCDataCtrl_CH m_DataCtrl;

public:
    int m_iTotLineNum;

    CArray<UINT,UINT> m_BeamConKList   ;//铁路梁混凝土压应力
    CArray<UINT,UINT> m_BeamShearKList ;//铁路梁混凝土剪应力
    CArray<UINT,UINT> m_BeamRebarKList ;//铁路梁钢筋拉应力
    CArray<UINT,UINT> m_BeamSheartKList;//铁路梁上翼缘剪应力
    CArray<UINT,UINT> m_BeamShearbKList;//铁路梁下翼缘剪应力
    CArray<UINT,UINT> m_BeamCrackKList ;//铁路梁裂缝宽度
    CArray<UINT,UINT> m_ColumnConKList   ;//铁路柱混凝土压应力
    CArray<UINT,UINT> m_ColumnShearKList ;//铁路柱混凝土剪应力
    CArray<UINT,UINT> m_ColumnRebarcKList;//铁路柱钢筋压应力
    CArray<UINT,UINT> m_ColumnRebarsKList;//铁路柱钢筋拉应力
    CArray<UINT,UINT> m_ColumnStabKList  ;//铁路柱稳定性
    CArray<UINT,UINT> m_ColumnCrackKList ;//铁路柱裂缝宽度
    //抗震
    CArray<UINT,UINT> m_SeismicConKList   ;//铁路柱混凝土压应力抗震验算
    CArray<UINT,UINT> m_SeismicRebarcKList;//铁路柱钢筋压应力抗震验算
    CArray<UINT,UINT> m_SeismicRebarsKList;//铁路柱钢筋拉应力抗震验算
    CArray<UINT,UINT> m_SeismicEKList     ;//铁路柱偏心距抗震验算
    CArray<UINT,UINT> m_SeismicStabKList  ;//铁路柱稳定性抗震验算
    CArray<UINT,UINT> m_SeismicDuctKList  ;//铁路柱延性比抗震验算Ductility

    CMap<UINT, UINT, UINT, UINT> m_DgnLcomKMap;
    CMap<UINT,UINT,UINT,UINT> m_DgnTdgrMap; //DgnTdgrK, TdgrK
    CMap<UINT,UINT,UINT,UINT> m_DgnTdnaMap; //DgnTdgrK, TdnaK
    CMap<_CRC_COMF_K, _CRC_COMF_K, _CRC_COMF_D, _CRC_COMF_D&> m_ComfDMap;

public:
    CRCrailway();
	virtual ~CRCrailway();
	
public:
    BOOL RCrailway_CH(BOOL isBeam);

protected:
    BOOL Write_ImportData(BOOL isBeam);
    BOOL MakeData( BOOL isBeam);
////梁和柱验算同时用到的函数
	void section_Initialize();
    void get_section(int Elemi,int iPosi,BOOL isBeam);
    void force_Initialize();
    void Get_Beam_force(int Elemi,int iPosi);
    void Get_Column_force(int Elemi,int iPosi, BOOL isSeismic);
    void Get_Seismic_Force(int Elemi,int iPosi);	
	void Isaddforce(int t);
// 得到中性轴处最大、最小剪应力
	BOOL Get_Shear(INT elemk,INT sectioni, double& dSig_max, double& dSig_min, int& iLcomkMax,int& iLcomkMin);
	BOOL Get_concreteValue(INT elemk,double& concrete_tp,double& dconcrete_shear);


////梁验算所用到的函数
	void beam_concrete_c(); //混凝土压应力验算
    void beam_rebar_s();    //钢筋拉应力验算
    void beam_shear();      //混凝土中性轴处剪应力验算
    void beam_shear_t();    //混凝土上翼缘剪应力验算
	void beam_shear_b();    //混凝土下翼缘剪应力验算
	void beam_crack();      //梁裂缝宽度验算

	  
////柱验算所用到的函数
	void column_concrete_c(BOOL seismic);  //混凝土压应力验算
	void column_rebar_c(BOOL seismic);     //钢筋压应力验算
	void column_rebar_s(BOOL seismic);     //钢筋拉应力验算 
    void column_shear();                   //混凝土主拉应力验算
	void column_stability(BOOL seismic);   //柱稳定性验算
    void column_crack();                   //柱裂缝宽度验算
	void column_e(BOOL seismic);           //偏心距验算
	void seismic_ductility();              //延性比验算

    HINSTANCE m_hDllCalc;

    CString Get_DllFileName(int iCode);

};

#endif // !defined(AFX_BEAM_H__E96D7AA4_F53B_463D_A782_F849C151271E__INCLUDED_)


