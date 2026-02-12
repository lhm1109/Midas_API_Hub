#if !defined(AFX_CMSECVIEWWND_H__D9CEE346_2232_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMSECVIEWWND_H__D9CEE346_2232_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSecViewWnd.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "CMGeometry.h"

#include "HeaderPre.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSecViewWnd window

class ICMPainter
{
// Construction
public:
	ICMPainter();
	~ICMPainter();

// Attributes
	CRect   m_DrawArea;

// Operations
public:
	void SetDrawingArea(CRect &r);
	void EraseBkgnd(CDC* pDC);
	void DrawSection(CDC* pDC, CCMGeom2DArray &aGeom, 
									 BOOL bCentroid, BOOL bOffset, BOOL bShearCheck,
									 int nRebarNum, BOOL bChangeAxis);

// Implementations
// Operations
protected:
	int RoundUp(double dVal);
	void DrawAxis(CDC* pDC, CPoint &org, BOOL bChangeAxis);
	void CalcScaleOrgOff(CRect &rect, CCMGeom2DArray &aGeom, 
											 double &scale, CPoint &org, CPoint &offset);
};

class __MY_EXT_CLASS__ ICMGeomMaker
{
// Construction
public:
	ICMGeomMaker();
	~ICMGeomMaker();

// Attributes

// Operations
public:
	void MakePicture(T_SECT_D *pDataSrc, CCMGeom2DArray &aGeom, 
									 BOOL bAfter, BOOL bReverse,
									 BOOL bCentroid, BOOL bOffset, BOOL bShearCheck,
									 T_RPSC_D* pDataSrcRebar, BOOL bEndJ, int* pnRebarNum, BOOL bCGI=FALSE);

	void GetTaperedMBR(SCMRect2D& rData) { rData = m_TaperedMBR; }

// Implementations

// Attributes
private:
	T_SECT_D* m_pDataSrc;
	SCMRect2D m_TaperedMBR;

// Operations
protected:
	// Regular section
	void GetRegularData(double &H, double &B1, double &tw, 
											double &tf1, double &B2, double &tf2);
	void GetRegularDataJ(double &H, double &B1, double &tw, 
											 double &tf1, double &B2, double &tf2);
	void GetRegularDataS(double &H, double &B1, double &tw, 
											 double &tf1, double &B2, double &tf2);
	void MakePictureAngle(CCMGeom2DArray &aGeom, BOOL bRev);
	void MakePictureChannel(CCMGeom2DArray &aGeom);
	void MakePictureHBeam(CCMGeom2DArray &aGeom);
	void MakePictureTee(CCMGeom2DArray &aGeom, BOOL bRev);
	void MakePictureUpsideDownTee(CCMGeom2DArray &aGeom, BOOL bRev);
	void MakePictureBox(CCMGeom2DArray &aGeom);
	void MakePicturePipe(CCMGeom2DArray &aGeom);
	void MakePictureDblAngle(CCMGeom2DArray &aGeom, BOOL bRev);
	void MakePictureDblChannel(CCMGeom2DArray& aGeom);
	void MakePictureDblChannelBox(CCMGeom2DArray &aGeom);
	void MakePictureSolidRect(CCMGeom2DArray &aGeom);
	void MakePictureSolidCircle(CCMGeom2DArray &aGeom);
	void MakePictureRegColdFormedChannel(CCMGeom2DArray &aGeom);
	void MakePictureRegURib(CCMGeom2DArray &aGeom);
	void MakePictureOctagon(CCMGeom2DArray &aGeom);
	void MakePictureSolidOctagon(CCMGeom2DArray &aGeom);
	void MakePictureTrack(CCMGeom2DArray &aGeom);
	void MakePictureSolidTrack(CCMGeom2DArray &aGeom);
	void MakePictureHalfTrack(CCMGeom2DArray &aGeom);
	void MakePictureCrossAngle(CCMGeom2DArray &aGeom);
	void MakePictureRegColdFormedBox(CCMGeom2DArray &aGeom);
	void MakePictureUpright(CCMGeom2DArray &aGeom);
	void MakePictureZ(CCMGeom2DArray &aGeom);
	void MakePictureRegColdFormedChannelHat(CCMGeom2DArray &aGeom);

	// Cold Formed Section
	void MakePictureColdFormedChannel(CCMGeom2DArray &aGeom);
	void MakePictureColdFormedChannel2I(CCMGeom2DArray &aGeom);
	void MakePictureColdFormedChannel2B(CCMGeom2DArray &aGeom);
	void MakePictureColdFormedChannel3(CCMGeom2DArray &aGeom);
	void MakePictureColdFormedChannel4(CCMGeom2DArray &aGeom);

	// SRC section
	void GetSrcData(double &HC, double &BC, double &H, double &B, 
									double &tw, double &tf1, double &C, double &tf2);
	void GetSrcData2(double &HC, double &BC,   
									 double &H, double &B, double &tw,  double &tf,  
									double &H_, double &B_,double &tw_,  double &tf_);
	void MakePictureRectBox(CCMGeom2DArray &aGeom, BOOL bOpen);
	void MakePictureRectPipe(CCMGeom2DArray &aGeom, BOOL bOpen);
	void MakePictureCircleBox(CCMGeom2DArray &aGeom, BOOL bOpen);
	void MakePictureCirclePipe(CCMGeom2DArray &aGeom, BOOL bOpen);
	void MakePictureSrcBox(CCMGeom2DArray &aGeom);
	void MakePictureSrcPipe(CCMGeom2DArray &aGeom);
	void MakePictureRectHBeam(CCMGeom2DArray &aGeom);
	void MakePictureCircleHBeam(CCMGeom2DArray &aGeom);
	void MakePictureRectH2TBeam(CCMGeom2DArray &aGeom);
	void MakePictureRectHTBeam(CCMGeom2DArray &aGeom);
	void MakePictureSrcDumbbell(T_SECT_D& SectData, CCMGeom2DArray& aGeom);


	// Tapered section
	void AdjustCenter(CCMGeom2D* pGeomI, CCMGeom2D* pGeomJ);
	void AdjustPosition(CCMGeom2D* pGeomI, CCMGeom2D* pGeomJ);
	void AdjustPosition(CCMGeom2DArray& aGeomI, CCMGeom2DArray& aGeomJ);
	void MakePictureTaperedAngle(CCMGeom2DArray &aGeom);
	void MakePictureTaperedChannel(CCMGeom2DArray &aGeom);
	void MakePictureTaperedHBeam(CCMGeom2DArray &aGeom);
	void MakePictureTaperedTee(CCMGeom2DArray &aGeom);
	void MakePictureTaperedBox(CCMGeom2DArray &aGeom);
	void MakePictureTaperedPipe(CCMGeom2DArray &aGeom);
	void MakePictureTaperedDblAngle(CCMGeom2DArray &aGeom);
	void MakePictureTaperedDblChannel(CCMGeom2DArray &aGeom);
	void MakePictureTaperedSolidRect(CCMGeom2DArray &aGeom);
	void MakePictureTaperedSolidCircle(CCMGeom2DArray &aGeom);
	void MakePictureTaperedOctagon(CCMGeom2DArray &aGeom);
	void MakePictureTaperedSolidOctagon(CCMGeom2DArray &aGeom);
	void MakePictureTaperedTrack(CCMGeom2DArray &aGeom);
	void MakePictureTaperedSolidTrack(CCMGeom2DArray &aGeom);
	void MakePictureTaperedHalfTrack(CCMGeom2DArray &aGeom);
	void MakePictureTaperedPsc(T_SECT_D& rTaperedSectD, CCMGeom2DArray &aGeom);
	void MakePictureTaperedSteelBox(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2]);
	void MakePictureTaperedSteelI(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2]);
	void MakePictureTaperedSteelTub(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2]);
	void MakePictureTaperedStlGirderBox(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2]);
	void MakePictureTaperedStlGirderI(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2]);
	void MakePictureTaperedStlGirderTub(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2]);
	void MakePictureTaperedConcreteI(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2]);
	void MakePictureTaperedConcreteT(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2]);
	void MakePictureTaperedStlGirderBox(CCMGeom2DArray &aGeom);
	void MakePictureTaperedStlGirderI(CCMGeom2DArray &aGeom);
	void MakePictureTaperedStlGirderMCell(CCMGeom2DArray &aGeom); 

	// Combined A type section
	void MakePicture2H(CCMGeom2DArray &aGeom); 
	void MakePictureHC1(CCMGeom2DArray &aGeom);
	void MakePictureHC2(CCMGeom2DArray &aGeom);
	void MakePictureHT(CCMGeom2DArray &aGeom); 
	void MakePicutre2T1(CCMGeom2DArray &aGeom);
	void MakePicture2T2(CCMGeom2DArray &aGeom);
	void MakePictureH2T(CCMGeom2DArray &aGeom);
	void MakePictureWOH(CCMGeom2DArray &aGeom);
	void MakePictureFOH(CCMGeom2DArray &aGeom);

	// Combined B type section
	void MakePicture2CW1(CCMGeom2DArray &aGeom);
	void MakePicture2CW2(CCMGeom2DArray &aGeom);
	void MakePicture1CW1(CCMGeom2DArray &aGeom);
	void MakePicture1CW2(CCMGeom2DArray &aGeom);
	void MakePicture4L(CCMGeom2DArray &aGeom);
	void MakePictureHP(CCMGeom2DArray &aGeom);

	// Composite
	void MakePictureCompoB(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ=FALSE);
	void MakePictureCompoI(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ=FALSE);
	void MakePictureCompoTub(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ=FALSE);
	void MakePictureCompoCI(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ=FALSE);
	void MakePictureCompoCT(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ=FALSE);
	void MakePictureCompoPC(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ=FALSE);
	void MakePictureCompo(T_SECT_D& SectData,CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ=FALSE); 
	void MakePictureCompoStlGirderBox(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ=FALSE);
	void MakePictureCompoStlGirderI(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ=FALSE);
	void MakePictureCompoStlGirderTub(CCMGeom2DArray &aGeom, CCMGeom2DArray aGeomPart1[2], CCMGeom2DArray aGeomPart2[2], BOOL bJ=FALSE);

	// Steel Girder
	void MakePictureStlGirderBox(CCMGeom2DArray &aGeom, BOOL bJ=FALSE);
	void MakePictureStlGirderI(CCMGeom2DArray &aGeom, BOOL bJ=FALSE);
	void MakePictureStlGirderMCell(CCMGeom2DArray &aGeom, BOOL bJ=FALSE);

	void MakePictureSODStiffner_B(CCMGeom2DArray &aGeom, T_SECT_SOD_STIFFNER_B& Size, double refy);
	void MakePictureSODStiffner_I(CCMGeom2DArray &aGeom, T_SECT_SOD_STIFFNER_I& Size, double refy);
	void MakePictureSODStiffner_Tub(CCMGeom2DArray &aGeom, T_SECT_SOD_STIFFNER_TUB& Size, double refy);
	void MakePictureSODStiffner_MCell(CCMGeom2DArray &aGeom, T_SECT_SOD_STIFFNER_MCELL& Size);

	void GetInWebLine_MCell_Closed(double InWebLine[2][2], double tw, int nWebPos);

	void MakePictureRivet_I(CCMGeom2DArray& aGeom, T_SECT_SOD_STIFFNER_I& Size);

	// PSC Section 
	void MakePicturePSC(T_SECT_D& SectData,CCMGeom2DArray &aGeom);

	void SetDataAndClearAndDeleteArray(CArray<CArray<T_2COOR, T_2COOR&>*, CArray<T_2COOR, T_2COOR&>*>& array, int nColor, CCMGeom2DArray& aGeom);

private:
	void _MakeLeftAngle(double H, double B1, double tw, double tf1, 
											CCMPolygon* pPolygon, BOOL bangle=FALSE);
	void _MakeRightAngle(double H, double B1, double tw, double tf1, 
											 CCMPolygon* pPolygon, BOOL bangle=FALSE);
	void _MakeLeftChannel(double H, double B1, double tw, double tf1,
												double B2, double tf2, CCMPolygon* pPolygon);
	void _MakeRightChannel(double H, double B1, double tw, double tf1,
												 double B2, double tf2, CCMPolygon* pPolygon);
	void _MakeHBeam(double H, double B1, double tw, double tf1, 
									double B2, double tf2, CCMPolygon* pPolygon);
	void _MakeIBeam(double H, double B1, double tw, double tf1,
									double B2, double tf2, CCMPolygon* pPolygon);
	void _MakeTee(double H, double B1, double tw, double tf1,
								CCMPolygon* pPolygon, BOOL bangle=FALSE);
	void _MakeUpsideDownTee(double H, double B1, double B2, double tw, double tf1,
								CCMPolygon* pPolygon, BOOL bangle=FALSE);
	void _MakeRect(double xmin, double ymin, 
								 double xmax, double ymax, CCMPolygon* pPolygon);
	void _MakeRect_RoundType(double H, double B, double tw, double r, CCMPolygon* pPolygon);
	void _MakeBox(double H, double B, double tw, double tf1,
								double C, double tf2, CCMPolygon* pPolygon);
	void _MakeBox_HSQ(double H, double B, double tw, double tf1,
								double C, double tf2, double B1_bot, double B2_bot, CCMPolygon* pPolygon);
	void _MakeBox_RoundType(double H, double B, double tw, double r, CCMPolygon* pPolygon);

	void _MakePictureColdFormedChannel(double H, double B, double tw, double r,
																		 double d, CCMPolygon* pPolygon, BOOL bBox=FALSE);
	void _MakePictureColdFormedChannelHat(double H, double B, double tw, double r,	double d, CCMPolygon* pPolygon, BOOL bBox=FALSE);
	void _MakePictureUpright(double H, double B, double tw, double Hw1, double Hw2, double B1, double B2, double B3, double Bf3, double d, CCMPolygon* pPolygon);
	void _MakePictureZ(double h, double b, double tw, double r, double d, double th, CCMPolygon* pPolygon);
	void _MakePictureURib(double H, double B1, double B2, double t, double r, CCMPolygon* pPolygon);
	void _MakePictureOctagon(double H, double B, double a, double b, CCMPolygon* pPolygon);
	void _MakePictureTrack(double H, double B, CCMPolygon* pPolygon);
	void _MakePictureHalfTrack(double H, double B, CCMPolygon* pPolygon);

	void MakePictureH_Stiffener(double H, double B, double theta, double dy_org, double dz_org, int nPos, CCMPolygon* pPolygon);
	void MakePictureT_Stiffener(double H, double B, double tw, double tf, double theta, double dy_org, double dz_org, int nPos, CCMPolygon* pPolygon);
	void MakePictureURib_Stiffener(double H, double B1, double B2, double t, double R, double theta, double dy_org, double dz_org, double dtf, int nPos, CCMPolygon* pPolygon);

	void MakePictureH_SODStiffener(double H, double B, double theta, double dy_org, double dz_org, CCMPolygon* pPolygon);
 	void MakePictureT_SODStiffener(double H, double B, double tw, double tf, double theta, double dy_org, double dz_org, CCMPolygon* pPolygon);
	void MakePictureL_SODStiffener(double H, double B, double tw, double tf, int nDir, double theta, double dy_org, double dz_org, CCMPolygon* pPolygon);
	void MakePictureURib_SODStiffener(double H, double B1, double B2, double t, double R, double theta, double dy_org, double dz_org, CCMPolygon* pPolygon);

	void MakePictureAngle_Rivet(double H, double B, double tw, double tf, int nAngleType, double dy_org, double dz_org, CCMPolygon* pPolygon);

};

class DlgView_Object;
class __MY_EXT_CLASS__ CCMSecViewWnd : public CWnd
{
// Construction
public:
	CCMSecViewWnd();
	virtual ~CCMSecViewWnd();

// Attributes
public:
	T_SECT_D*  m_pDataSrc; // only pointer, don't execute delete operation
	ICMPainter    m_Painter;  // Geometry Painting Interface
	ICMGeomMaker  m_GeomMaker;  // Geometry Making Interface
	BOOL  m_bAfter;   // construction section : flag for before or after
	BOOL	m_bReverse;
	BOOL  m_bCentroid;
	BOOL  m_bShearCheck;
	BOOL  m_bChangeAxis;

	T_RPSC_D* m_pDataSrcRebar;
	BOOL      m_bEndJ;
	// MNET:XXXX-CJJEONG-20090317 (Tel : 2124) 
	BOOL      m_bComGenJ;

// Operations
public:
	BOOL Init(CWnd* pParentWnd);
	void SetDataSource(T_SECT_D* pDataSrc, BOOL bAfter=FALSE, BOOL bChangeAxis=FALSE);
	void SetDataSourceRebar(T_RPSC_D* pDataSrcRebar, BOOL bEndJ=FALSE);
	void SetCentroidFlag(BOOL bCentroid) { m_bCentroid = bCentroid; }
	void SetShearCheckFlag(BOOL bShearCheck) { m_bShearCheck = bShearCheck; }

	SCMPoint2D GetPtOffset(CCMGeom2DArray &aGeom, BOOL bReverse);
	BOOL MakeEllipse(SCMRect2D rect, CArray<SCMPoint2D,SCMPoint2D> &PosArr);
	void GetPictureParam(BOOL& bCentroid,BOOL &bOffset,BOOL &bShearCheck);
	void ApplyPictureParam(BOOL bCentroid,BOOL bOffset,BOOL bShearCheck,CCMGeom2DArray& aGeom);
	
	BOOL ConvertSectionData(T_SECT_D* pSectD, CArray<DlgView_Object*,DlgView_Object*> & arDViewGeoms,
													CArray<double, double&> &arPos);

	void SetComGenJFlag(BOOL bComGenJ) { m_bComGenJ = bComGenJ; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSecViewWnd)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSecViewWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECVIEWWND_H__D9CEE346_2232_11D3_92DE_0000C0B0E6B3__INCLUDED_)
