#if !defined(__EXCEL9_H__)
#define __EXCEL9_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HeaderPre.h"


// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// Adjustments wrapper class


class __MY_EXT_CLASS__ Xl_Adjustments : public COleDispatchDriver
{
public:
	Xl_Adjustments() {}		// Calls COleDispatchDriver default constructor
	Xl_Adjustments(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Adjustments(const Xl_Adjustments& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	float GetItem(long Index);
	void SetItem(long Index, float newValue);
};
/////////////////////////////////////////////////////////////////////////////
// CalloutFormat wrapper class

class __MY_EXT_CLASS__ Xl_CalloutFormat : public COleDispatchDriver
{
public:
	Xl_CalloutFormat() {}		// Calls COleDispatchDriver default constructor
	Xl_CalloutFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_CalloutFormat(const Xl_CalloutFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void AutomaticLength();
	void CustomDrop(float Drop);
	void CustomLength(float Length);
	void PresetDrop(long DropType);
	long GetAccent();
	void SetAccent(long nNewValue);
	long GetAngle();
	void SetAngle(long nNewValue);
	long GetAutoAttach();
	void SetAutoAttach(long nNewValue);
	long GetAutoLength();
	long GetBorder();
	void SetBorder(long nNewValue);
	float GetDrop();
	long GetDropType();
	float GetGap();
	void SetGap(float newValue);
	float GetLength();
	long GetType();
	void SetType(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ColorFormat wrapper class

class __MY_EXT_CLASS__ Xl_ColorFormat : public COleDispatchDriver
{
public:
	Xl_ColorFormat() {}		// Calls COleDispatchDriver default constructor
	Xl_ColorFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ColorFormat(const Xl_ColorFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetRgb();
	void SetRgb(long nNewValue);
	long GetSchemeColor();
	void SetSchemeColor(long nNewValue);
	long GetType();
};
/////////////////////////////////////////////////////////////////////////////
// LineFormat wrapper class

class __MY_EXT_CLASS__ Xl_LineFormat : public COleDispatchDriver
{
public:
	Xl_LineFormat() {}		// Calls COleDispatchDriver default constructor
	Xl_LineFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_LineFormat(const Xl_LineFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBackColor();
	void SetBackColor(LPDISPATCH newValue);
	long GetBeginArrowheadLength();
	void SetBeginArrowheadLength(long nNewValue);
	long GetBeginArrowheadStyle();
	void SetBeginArrowheadStyle(long nNewValue);
	long GetBeginArrowheadWidth();
	void SetBeginArrowheadWidth(long nNewValue);
	long GetDashStyle();
	void SetDashStyle(long nNewValue);
	long GetEndArrowheadLength();
	void SetEndArrowheadLength(long nNewValue);
	long GetEndArrowheadStyle();
	void SetEndArrowheadStyle(long nNewValue);
	long GetEndArrowheadWidth();
	void SetEndArrowheadWidth(long nNewValue);
	LPDISPATCH GetForeColor();
	void SetForeColor(LPDISPATCH newValue);
	long GetPattern();
	void SetPattern(long nNewValue);
	long GetStyle();
	void SetStyle(long nNewValue);
	float GetTransparency();
	void SetTransparency(float newValue);
	long GetVisible();
	void SetVisible(long nNewValue);
	float GetWeight();
	void SetWeight(float newValue);
};
/////////////////////////////////////////////////////////////////////////////
// ShapeNode wrapper class

class __MY_EXT_CLASS__ Xl_ShapeNode : public COleDispatchDriver
{
public:
	Xl_ShapeNode() {}		// Calls COleDispatchDriver default constructor
	Xl_ShapeNode(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ShapeNode(const Xl_ShapeNode& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetEditingType();
	VARIANT GetPoints();
	long GetSegmentType();
};
/////////////////////////////////////////////////////////////////////////////
// ShapeNodes wrapper class

class __MY_EXT_CLASS__ Xl_ShapeNodes : public COleDispatchDriver
{
public:
	Xl_ShapeNodes() {}		// Calls COleDispatchDriver default constructor
	Xl_ShapeNodes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ShapeNodes(const Xl_ShapeNodes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN Get_NewEnum();
	void Delete(long Index);
	void Insert(long Index, long SegmentType, long EditingType, float X1, float Y1, float X2, float Y2, float X3, float Y3);
	void SetEditingType(long Index, long EditingType);
	void SetPosition(long Index, float X1, float Y1);
	void SetSegmentType(long Index, long SegmentType);
};
/////////////////////////////////////////////////////////////////////////////
// PictureFormat wrapper class

class __MY_EXT_CLASS__ Xl_PictureFormat : public COleDispatchDriver
{
public:
	Xl_PictureFormat() {}		// Calls COleDispatchDriver default constructor
	Xl_PictureFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_PictureFormat(const Xl_PictureFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void IncrementBrightness(float Increment);
	void IncrementContrast(float Increment);
	float GetBrightness();
	void SetBrightness(float newValue);
	long GetColorType();
	void SetColorType(long nNewValue);
	float GetContrast();
	void SetContrast(float newValue);
	float GetCropBottom();
	void SetCropBottom(float newValue);
	float GetCropLeft();
	void SetCropLeft(float newValue);
	float GetCropRight();
	void SetCropRight(float newValue);
	float GetCropTop();
	void SetCropTop(float newValue);
	long GetTransparencyColor();
	void SetTransparencyColor(long nNewValue);
	long GetTransparentBackground();
	void SetTransparentBackground(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ShadowFormat wrapper class

class __MY_EXT_CLASS__ Xl_ShadowFormat : public COleDispatchDriver
{
public:
	Xl_ShadowFormat() {}		// Calls COleDispatchDriver default constructor
	Xl_ShadowFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ShadowFormat(const Xl_ShadowFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void IncrementOffsetX(float Increment);
	void IncrementOffsetY(float Increment);
	LPDISPATCH GetForeColor();
	void SetForeColor(LPDISPATCH newValue);
	long GetObscured();
	void SetObscured(long nNewValue);
	float GetOffsetX();
	void SetOffsetX(float newValue);
	float GetOffsetY();
	void SetOffsetY(float newValue);
	float GetTransparency();
	void SetTransparency(float newValue);
	long GetType();
	void SetType(long nNewValue);
	long GetVisible();
	void SetVisible(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// TextEffectFormat wrapper class

class __MY_EXT_CLASS__ Xl_TextEffectFormat : public COleDispatchDriver
{
public:
	Xl_TextEffectFormat() {}		// Calls COleDispatchDriver default constructor
	Xl_TextEffectFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_TextEffectFormat(const Xl_TextEffectFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void ToggleVerticalText();
	long GetAlignment();
	void SetAlignment(long nNewValue);
	long GetFontBold();
	void SetFontBold(long nNewValue);
	long GetFontItalic();
	void SetFontItalic(long nNewValue);
	CString GetFontName();
	void SetFontName(LPCTSTR lpszNewValue);
	float GetFontSize();
	void SetFontSize(float newValue);
	long GetKernedPairs();
	void SetKernedPairs(long nNewValue);
	long GetNormalizedHeight();
	void SetNormalizedHeight(long nNewValue);
	long GetPresetShape();
	void SetPresetShape(long nNewValue);
	long GetPresetTextEffect();
	void SetPresetTextEffect(long nNewValue);
	long GetRotatedChars();
	void SetRotatedChars(long nNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	float GetTracking();
	void SetTracking(float newValue);
};
/////////////////////////////////////////////////////////////////////////////
// ThreeDFormat wrapper class

class __MY_EXT_CLASS__ Xl_ThreeDFormat : public COleDispatchDriver
{
public:
	Xl_ThreeDFormat() {}		// Calls COleDispatchDriver default constructor
	Xl_ThreeDFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ThreeDFormat(const Xl_ThreeDFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void IncrementRotationX(float Increment);
	void IncrementRotationY(float Increment);
	void ResetRotation();
	void SetThreeDFormat(long PresetThreeDFormat);
	void SetExtrusionDirection(long PresetExtrusionDirection);
	float GetDepth();
	void SetDepth(float newValue);
	LPDISPATCH GetExtrusionColor();
	long GetExtrusionColorType();
	void SetExtrusionColorType(long nNewValue);
	long GetPerspective();
	void SetPerspective(long nNewValue);
	long GetPresetExtrusionDirection();
	long GetPresetLightingDirection();
	void SetPresetLightingDirection(long nNewValue);
	long GetPresetLightingSoftness();
	void SetPresetLightingSoftness(long nNewValue);
	long GetPresetMaterial();
	void SetPresetMaterial(long nNewValue);
	long GetPresetThreeDFormat();
	float GetRotationX();
	void SetRotationX(float newValue);
	float GetRotationY();
	void SetRotationY(float newValue);
	long GetVisible();
	void SetVisible(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// FillFormat wrapper class

class __MY_EXT_CLASS__ Xl_FillFormat : public COleDispatchDriver
{
public:
	Xl_FillFormat() {}		// Calls COleDispatchDriver default constructor
	Xl_FillFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_FillFormat(const Xl_FillFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void OneColorGradient(long Style, long Variant, float Degree);
	void Patterned(long Pattern);
	void PresetGradient(long Style, long Variant, long PresetGradientType);
	void PresetTextured(long PresetTexture);
	void Solid();
	void TwoColorGradient(long Style, long Variant);
	void UserPicture(LPCTSTR PictureFile);
	void UserTextured(LPCTSTR TextureFile);
	LPDISPATCH GetBackColor();
	void SetBackColor(LPDISPATCH newValue);
	LPDISPATCH GetForeColor();
	void SetForeColor(LPDISPATCH newValue);
	long GetGradientColorType();
	float GetGradientDegree();
	long GetGradientStyle();
	long GetGradientVariant();
	long GetPattern();
	long GetPresetGradientType();
	long GetPresetTexture();
	CString GetTextureName();
	long GetTextureType();
	float GetTransparency();
	void SetTransparency(float newValue);
	long GetType();
	long GetVisible();
	void SetVisible(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// _Application wrapper class

class __MY_EXT_CLASS__ Xl_Application : public COleDispatchDriver
{
public:
	Xl_Application() {}		// Calls COleDispatchDriver default constructor
	Xl_Application(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Application(const Xl_Application& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetActiveCell();
	LPDISPATCH GetActiveChart();
	CString GetActivePrinter();
	void SetActivePrinter(LPCTSTR lpszNewValue);
	LPDISPATCH GetActiveSheet();
	LPDISPATCH GetActiveWindow();
	LPDISPATCH GetActiveWorkbook();
	LPDISPATCH GetAddIns();
	LPDISPATCH GetAssistant();
	void Calculate();
	LPDISPATCH GetCells();
	LPDISPATCH GetCharts();
	LPDISPATCH GetColumns();
	LPDISPATCH GetCommandBars();
	long GetDDEAppReturnCode();
	void DDEExecute(long Channel, LPCTSTR String);
	long DDEInitiate(LPCTSTR App, LPCTSTR Topic);
	void DDEPoke(long Channel, const VARIANT& Item, const VARIANT& Data);
	VARIANT DDERequest(long Channel, LPCTSTR Item);
	void DDETerminate(long Channel);
	VARIANT Evaluate(const VARIANT& Name);
	VARIANT _Evaluate(const VARIANT& Name);
	VARIANT ExecuteExcel4Macro(LPCTSTR String);
	LPDISPATCH Intersect(LPDISPATCH Arg1, LPDISPATCH Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	LPDISPATCH GetNames();
	LPDISPATCH GetRange(const VARIANT& Cell1, const VARIANT& Cell2);
	LPDISPATCH GetRows();
	VARIANT Run(const VARIANT& Macro, const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, 
		const VARIANT& Arg10, const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, 
		const VARIANT& Arg20, const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, 
		const VARIANT& Arg30);
	VARIANT _Run2(const VARIANT& Macro, const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, 
		const VARIANT& Arg10, const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, 
		const VARIANT& Arg20, const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, 
		const VARIANT& Arg30);
	LPDISPATCH GetSelection();
	void SendKeys(const VARIANT& Keys, const VARIANT& Wait);
	LPDISPATCH GetSheets();
	LPDISPATCH GetThisWorkbook();
	LPDISPATCH Union(LPDISPATCH Arg1, LPDISPATCH Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, const VARIANT& Arg11, 
		const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, const VARIANT& Arg21, 
		const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	LPDISPATCH GetWindows();
	LPDISPATCH GetWorkbooks();
	LPDISPATCH GetWorksheetFunction();
	LPDISPATCH GetWorksheets();
	LPDISPATCH GetExcel4IntlMacroSheets();
	LPDISPATCH GetExcel4MacroSheets();
	void ActivateMicrosoftApp(long Index);
	void AddChartAutoFormat(const VARIANT& Chart, LPCTSTR Name, const VARIANT& Description);
	void AddCustomList(const VARIANT& ListArray, const VARIANT& ByRow);
	BOOL GetAlertBeforeOverwriting();
	void SetAlertBeforeOverwriting(BOOL bNewValue);
	CString GetAltStartupPath();
	void SetAltStartupPath(LPCTSTR lpszNewValue);
	BOOL GetAskToUpdateLinks();
	void SetAskToUpdateLinks(BOOL bNewValue);
	BOOL GetEnableAnimations();
	void SetEnableAnimations(BOOL bNewValue);
	LPDISPATCH GetAutoCorrect();
	long GetBuild();
	BOOL GetCalculateBeforeSave();
	void SetCalculateBeforeSave(BOOL bNewValue);
	long GetCalculation();
	void SetCalculation(long nNewValue);
	VARIANT GetCaller(const VARIANT& Index);
	BOOL GetCanPlaySounds();
	BOOL GetCanRecordSounds();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	BOOL GetCellDragAndDrop();
	void SetCellDragAndDrop(BOOL bNewValue);
	double CentimetersToPoints(double Centimeters);
	BOOL CheckSpelling(LPCTSTR Word, const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase);
	VARIANT GetClipboardFormats(const VARIANT& Index);
	BOOL GetDisplayClipboardWindow();
	void SetDisplayClipboardWindow(BOOL bNewValue);
	long GetCommandUnderlines();
	void SetCommandUnderlines(long nNewValue);
	BOOL GetConstrainNumeric();
	void SetConstrainNumeric(BOOL bNewValue);
	VARIANT ConvertFormula(const VARIANT& Formula, long FromReferenceStyle, const VARIANT& ToReferenceStyle, const VARIANT& ToAbsolute, const VARIANT& RelativeTo);
	BOOL GetCopyObjectsWithCells();
	void SetCopyObjectsWithCells(BOOL bNewValue);
	long GetCursor();
	void SetCursor(long nNewValue);
	long GetCustomListCount();
	long GetCutCopyMode();
	void SetCutCopyMode(long nNewValue);
	long GetDataEntryMode();
	void SetDataEntryMode(long nNewValue);
	CString Get_Default();
	CString GetDefaultFilePath();
	void SetDefaultFilePath(LPCTSTR lpszNewValue);
	void DeleteChartAutoFormat(LPCTSTR Name);
	void DeleteCustomList(long ListNum);
	LPDISPATCH GetDialogs();
	BOOL GetDisplayAlerts();
	void SetDisplayAlerts(BOOL bNewValue);
	BOOL GetDisplayFormulaBar();
	void SetDisplayFormulaBar(BOOL bNewValue);
	BOOL GetDisplayFullScreen();
	void SetDisplayFullScreen(BOOL bNewValue);
	BOOL GetDisplayNoteIndicator();
	void SetDisplayNoteIndicator(BOOL bNewValue);
	long GetDisplayCommentIndicator();
	void SetDisplayCommentIndicator(long nNewValue);
	BOOL GetDisplayExcel4Menus();
	void SetDisplayExcel4Menus(BOOL bNewValue);
	BOOL GetDisplayRecentFiles();
	void SetDisplayRecentFiles(BOOL bNewValue);
	BOOL GetDisplayScrollBars();
	void SetDisplayScrollBars(BOOL bNewValue);
	BOOL GetDisplayStatusBar();
	void SetDisplayStatusBar(BOOL bNewValue);
	void DoubleClick();
	BOOL GetEditDirectlyInCell();
	void SetEditDirectlyInCell(BOOL bNewValue);
	BOOL GetEnableAutoComplete();
	void SetEnableAutoComplete(BOOL bNewValue);
	long GetEnableCancelKey();
	void SetEnableCancelKey(long nNewValue);
	BOOL GetEnableSound();
	void SetEnableSound(BOOL bNewValue);
	VARIANT GetFileConverters(const VARIANT& Index1, const VARIANT& Index2);
	LPDISPATCH GetFileSearch();
	LPDISPATCH GetFileFind();
	BOOL GetFixedDecimal();
	void SetFixedDecimal(BOOL bNewValue);
	long GetFixedDecimalPlaces();
	void SetFixedDecimalPlaces(long nNewValue);
	VARIANT GetCustomListContents(long ListNum);
	long GetCustomListNum(const VARIANT& ListArray);
	VARIANT GetOpenFilename(const VARIANT& FileFilter, const VARIANT& FilterIndex, const VARIANT& Title, const VARIANT& ButtonText, const VARIANT& MultiSelect);
	VARIANT GetSaveAsFilename(const VARIANT& InitialFilename, const VARIANT& FileFilter, const VARIANT& FilterIndex, const VARIANT& Title, const VARIANT& ButtonText);
	void Goto(const VARIANT& Reference, const VARIANT& Scroll);
	double GetHeight();
	void SetHeight(double newValue);
	void Help(const VARIANT& HelpFile, const VARIANT& HelpContextID);
	BOOL GetIgnoreRemoteRequests();
	void SetIgnoreRemoteRequests(BOOL bNewValue);
	double InchesToPoints(double Inches);
	VARIANT InputBox(LPCTSTR Prompt, const VARIANT& Title, const VARIANT& Default, const VARIANT& Left, const VARIANT& Top, const VARIANT& HelpFile, const VARIANT& HelpContextID, const VARIANT& Type);
	BOOL GetInteractive();
	void SetInteractive(BOOL bNewValue);
	VARIANT GetInternational(const VARIANT& Index);
	BOOL GetIteration();
	void SetIteration(BOOL bNewValue);
	double GetLeft();
	void SetLeft(double newValue);
	CString GetLibraryPath();
	void MacroOptions(const VARIANT& Macro, const VARIANT& Description, const VARIANT& HasMenu, const VARIANT& MenuText, const VARIANT& HasShortcutKey, const VARIANT& ShortcutKey, const VARIANT& Category, const VARIANT& StatusBar, 
		const VARIANT& HelpContextID, const VARIANT& HelpFile);
	void MailLogoff();
	void MailLogon(const VARIANT& Name, const VARIANT& Password, const VARIANT& DownloadNewMail);
	VARIANT GetMailSession();
	long GetMailSystem();
	BOOL GetMathCoprocessorAvailable();
	double GetMaxChange();
	void SetMaxChange(double newValue);
	long GetMaxIterations();
	void SetMaxIterations(long nNewValue);
	long GetMemoryFree();
	long GetMemoryTotal();
	long GetMemoryUsed();
	BOOL GetMouseAvailable();
	BOOL GetMoveAfterReturn();
	void SetMoveAfterReturn(BOOL bNewValue);
	long GetMoveAfterReturnDirection();
	void SetMoveAfterReturnDirection(long nNewValue);
	LPDISPATCH GetRecentFiles();
	CString GetName();
	LPDISPATCH NextLetter();
	CString GetNetworkTemplatesPath();
	LPDISPATCH GetODBCErrors();
	long GetODBCTimeout();
	void SetODBCTimeout(long nNewValue);
	void OnKey(LPCTSTR Key, const VARIANT& Procedure);
	void OnRepeat(LPCTSTR Text, LPCTSTR Procedure);
	void OnTime(const VARIANT& EarliestTime, LPCTSTR Procedure, const VARIANT& LatestTime, const VARIANT& Schedule);
	void OnUndo(LPCTSTR Text, LPCTSTR Procedure);
	CString GetOnWindow();
	void SetOnWindow(LPCTSTR lpszNewValue);
	CString GetOperatingSystem();
	CString GetOrganizationName();
	CString GetPath();
	CString GetPathSeparator();
	VARIANT GetPreviousSelections(const VARIANT& Index);
	BOOL GetPivotTableSelection();
	void SetPivotTableSelection(BOOL bNewValue);
	BOOL GetPromptForSummaryInfo();
	void SetPromptForSummaryInfo(BOOL bNewValue);
	void Quit();
	void RecordMacro(const VARIANT& BasicCode, const VARIANT& XlmCode);
	BOOL GetRecordRelative();
	long GetReferenceStyle();
	void SetReferenceStyle(long nNewValue);
	VARIANT GetRegisteredFunctions(const VARIANT& Index1, const VARIANT& Index2);
	BOOL RegisterXLL(LPCTSTR Filename);
	void Repeat();
	BOOL GetRollZoom();
	void SetRollZoom(BOOL bNewValue);
	void SaveWorkspace(const VARIANT& Filename);
	BOOL GetScreenUpdating();
	void SetScreenUpdating(BOOL bNewValue);
	void SetDefaultChart(const VARIANT& FormatName, const VARIANT& Gallery);
	long GetSheetsInNewWorkbook();
	void SetSheetsInNewWorkbook(long nNewValue);
	BOOL GetShowChartTipNames();
	void SetShowChartTipNames(BOOL bNewValue);
	BOOL GetShowChartTipValues();
	void SetShowChartTipValues(BOOL bNewValue);
	CString GetStandardFont();
	void SetStandardFont(LPCTSTR lpszNewValue);
	double GetStandardFontSize();
	void SetStandardFontSize(double newValue);
	CString GetStartupPath();
	VARIANT GetStatusBar();
	void SetStatusBar(const VARIANT& newValue);
	CString GetTemplatesPath();
	BOOL GetShowToolTips();
	void SetShowToolTips(BOOL bNewValue);
	double GetTop();
	void SetTop(double newValue);
	long GetDefaultSaveFormat();
	void SetDefaultSaveFormat(long nNewValue);
	CString GetTransitionMenuKey();
	void SetTransitionMenuKey(LPCTSTR lpszNewValue);
	long GetTransitionMenuKeyAction();
	void SetTransitionMenuKeyAction(long nNewValue);
	BOOL GetTransitionNavigKeys();
	void SetTransitionNavigKeys(BOOL bNewValue);
	void Undo();
	double GetUsableHeight();
	double GetUsableWidth();
	BOOL GetUserControl();
	void SetUserControl(BOOL bNewValue);
	CString GetUserName_();
	void SetUserName(LPCTSTR lpszNewValue);
	CString GetValue();
	LPDISPATCH GetVbe();
	CString GetVersion();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	void Volatile(const VARIANT& Volatile);
	double GetWidth();
	void SetWidth(double newValue);
	BOOL GetWindowsForPens();
	long GetWindowState();
	void SetWindowState(long nNewValue);
	long GetDefaultSheetDirection();
	void SetDefaultSheetDirection(long nNewValue);
	long GetCursorMovement();
	void SetCursorMovement(long nNewValue);
	BOOL GetControlCharacters();
	void SetControlCharacters(BOOL bNewValue);
	BOOL GetEnableEvents();
	void SetEnableEvents(BOOL bNewValue);
	BOOL Wait(const VARIANT& Time);
	BOOL GetExtendList();
	void SetExtendList(BOOL bNewValue);
	LPDISPATCH GetOLEDBErrors();
	CString GetPhonetic(const VARIANT& Text);
	LPDISPATCH GetCOMAddIns();
	LPDISPATCH GetDefaultWebOptions();
	CString GetProductCode();
	CString GetUserLibraryPath();
	BOOL GetAutoPercentEntry();
	void SetAutoPercentEntry(BOOL bNewValue);
	LPDISPATCH GetLanguageSettings();
	LPDISPATCH GetAnswerWizard();
	void CalculateFull();
	BOOL FindFile();
	long GetCalculationVersion();
	BOOL GetShowWindowsInTaskbar();
	void SetShowWindowsInTaskbar(BOOL bNewValue);
	long GetFeatureInstall();
	void SetFeatureInstall(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// _Chart wrapper class

class __MY_EXT_CLASS__ Xl_Chart : public COleDispatchDriver
{
public:
	Xl_Chart() {}		// Calls COleDispatchDriver default constructor
	Xl_Chart(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Chart(const Xl_Chart& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void Activate();
	void Copy(const VARIANT& Before, const VARIANT& After);
	void Delete();
	CString GetCodeName();
	CString Get_CodeName();
	void Set_CodeName(LPCTSTR lpszNewValue);
	long GetIndex();
	void Move(const VARIANT& Before, const VARIANT& After);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	LPDISPATCH GetNext();
	LPDISPATCH GetPageSetup();
	LPDISPATCH GetPrevious();
	void PrintPreview(const VARIANT& EnableChanges);
	void Protect(const VARIANT& Password, const VARIANT& DrawingObjects, const VARIANT& Contents, const VARIANT& Scenarios, const VARIANT& UserInterfaceOnly);
	BOOL GetProtectContents();
	BOOL GetProtectDrawingObjects();
	BOOL GetProtectionMode();
	void SaveAs(LPCTSTR Filename, const VARIANT& FileFormat, const VARIANT& Password, const VARIANT& WriteResPassword, const VARIANT& ReadOnlyRecommended, const VARIANT& CreateBackup, const VARIANT& AddToMru, const VARIANT& TextCodepage, 
		const VARIANT& TextVisualLayout);
	void Select(const VARIANT& Replace);
	void Unprotect(const VARIANT& Password);
	long GetVisible();
	void SetVisible(long nNewValue);
	LPDISPATCH GetShapes();
	void ApplyDataLabels(long Type, const VARIANT& LegendKey, const VARIANT& AutoText, const VARIANT& HasLeaderLines);
	LPDISPATCH GetArea3DGroup();
	LPDISPATCH AreaGroups(const VARIANT& Index);
	BOOL GetAutoScaling();
	void SetAutoScaling(BOOL bNewValue);
	LPDISPATCH Axes(const VARIANT& Type, long AxisGroup);
	void SetBackgroundPicture(LPCTSTR Filename);
	LPDISPATCH GetBar3DGroup();
	LPDISPATCH BarGroups(const VARIANT& Index);
	LPDISPATCH GetChartArea();
	LPDISPATCH ChartGroups(const VARIANT& Index);
	LPDISPATCH ChartObjects(const VARIANT& Index);
	LPDISPATCH GetChartTitle();
	void ChartWizard(const VARIANT& Source, const VARIANT& Gallery, const VARIANT& Format, const VARIANT& PlotBy, const VARIANT& CategoryLabels, const VARIANT& SeriesLabels, const VARIANT& HasLegend, const VARIANT& Title, 
		const VARIANT& CategoryTitle, const VARIANT& ValueTitle, const VARIANT& ExtraTitle);
	void CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	LPDISPATCH GetColumn3DGroup();
	LPDISPATCH ColumnGroups(const VARIANT& Index);
	void CopyPicture(long Appearance, long Format, long Size);
	LPDISPATCH GetCorners();
	void CreatePublisher(const VARIANT& Edition, long Appearance, long Size, const VARIANT& ContainsPICT, const VARIANT& ContainsBIFF, const VARIANT& ContainsRTF, const VARIANT& ContainsVALU);
	LPDISPATCH GetDataTable();
	long GetDepthPercent();
	void SetDepthPercent(long nNewValue);
	void Deselect();
	long GetDisplayBlanksAs();
	void SetDisplayBlanksAs(long nNewValue);
	LPDISPATCH DoughnutGroups(const VARIANT& Index);
	long GetElevation();
	void SetElevation(long nNewValue);
	VARIANT Evaluate(const VARIANT& Name);
	VARIANT _Evaluate(const VARIANT& Name);
	LPDISPATCH GetFloor();
	long GetGapDepth();
	void SetGapDepth(long nNewValue);
	VARIANT GetHasAxis(const VARIANT& Index1, const VARIANT& Index2);
	void SetHasAxis(const VARIANT& Index1, const VARIANT& Index2, const VARIANT& newValue);
	BOOL GetHasDataTable();
	void SetHasDataTable(BOOL bNewValue);
	BOOL GetHasLegend();
	void SetHasLegend(BOOL bNewValue);
	BOOL GetHasTitle();
	void SetHasTitle(BOOL bNewValue);
	long GetHeightPercent();
	void SetHeightPercent(long nNewValue);
	LPDISPATCH GetHyperlinks();
	LPDISPATCH GetLegend();
	LPDISPATCH GetLine3DGroup();
	LPDISPATCH LineGroups(const VARIANT& Index);
	LPDISPATCH Location(long Where, const VARIANT& Name);
	LPDISPATCH OLEObjects(const VARIANT& Index);
	void Paste(const VARIANT& Type);
	long GetPerspective();
	void SetPerspective(long nNewValue);
	LPDISPATCH GetPie3DGroup();
	LPDISPATCH PieGroups(const VARIANT& Index);
	LPDISPATCH GetPlotArea();
	BOOL GetPlotVisibleOnly();
	void SetPlotVisibleOnly(BOOL bNewValue);
	LPDISPATCH RadarGroups(const VARIANT& Index);
	VARIANT GetRightAngleAxes();
	void SetRightAngleAxes(const VARIANT& newValue);
	VARIANT GetRotation();
	void SetRotation(const VARIANT& newValue);
	LPDISPATCH SeriesCollection(const VARIANT& Index);
	BOOL GetSizeWithWindow();
	void SetSizeWithWindow(BOOL bNewValue);
	BOOL GetShowWindow();
	void SetShowWindow(BOOL bNewValue);
	LPDISPATCH GetSurfaceGroup();
	long GetChartType();
	void SetChartType(long nNewValue);
	void ApplyCustomType(long ChartType, const VARIANT& TypeName);
	LPDISPATCH GetWalls();
	BOOL GetWallsAndGridlines2D();
	void SetWallsAndGridlines2D(BOOL bNewValue);
	LPDISPATCH XYGroups(const VARIANT& Index);
	long GetBarShape();
	void SetBarShape(long nNewValue);
	long GetPlotBy();
	void SetPlotBy(long nNewValue);
	BOOL GetProtectFormatting();
	void SetProtectFormatting(BOOL bNewValue);
	BOOL GetProtectData();
	void SetProtectData(BOOL bNewValue);
	BOOL GetProtectGoalSeek();
	void SetProtectGoalSeek(BOOL bNewValue);
	BOOL GetProtectSelection();
	void SetProtectSelection(BOOL bNewValue);
	void GetChartElement(long x, long y, long* ElementID, long* Arg1, long* Arg2);
	void SetSourceData(LPDISPATCH Source, const VARIANT& PlotBy);
	BOOL Export(LPCTSTR Filename, const VARIANT& FilterName, const VARIANT& Interactive);
	void Refresh();
	LPDISPATCH GetPivotLayout();
	BOOL GetHasPivotFields();
	void SetHasPivotFields(BOOL bNewValue);
	LPDISPATCH GetScripts();
	void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
};
/////////////////////////////////////////////////////////////////////////////
// Sheets wrapper class

class __MY_EXT_CLASS__ Xl_Sheets : public COleDispatchDriver
{
public:
	Xl_Sheets() {}		// Calls COleDispatchDriver default constructor
	Xl_Sheets(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Sheets(const Xl_Sheets& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(const VARIANT& Before, const VARIANT& After, const VARIANT& Count, const VARIANT& Type);
	void Copy(const VARIANT& Before, const VARIANT& After);
	long GetCount();
	void Delete();
	void FillAcrossSheets(LPDISPATCH Range, long Type);
	LPDISPATCH GetItem(const VARIANT& Index);
	void Move(const VARIANT& Before, const VARIANT& After);
	LPUNKNOWN Get_NewEnum();
	void PrintPreview(const VARIANT& EnableChanges);
	void Select(const VARIANT& Replace);
	LPDISPATCH GetHPageBreaks();
	LPDISPATCH GetVPageBreaks();
	VARIANT GetVisible();
	void SetVisible(const VARIANT& newValue);
	LPDISPATCH Get_Default(const VARIANT& Index);
	void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
};
/////////////////////////////////////////////////////////////////////////////
// _Worksheet wrapper class

class __MY_EXT_CLASS__ Xl_Worksheet : public COleDispatchDriver
{
public:
	Xl_Worksheet() {}		// Calls COleDispatchDriver default constructor
	Xl_Worksheet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Worksheet(const Xl_Worksheet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void Activate();
	void Copy(const VARIANT& Before, const VARIANT& After);
	void Delete();
	CString GetCodeName();
	CString Get_CodeName();
	void Set_CodeName(LPCTSTR lpszNewValue);
	long GetIndex();
	void Move(const VARIANT& Before, const VARIANT& After);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	LPDISPATCH GetNext();
	LPDISPATCH GetPageSetup();
	LPDISPATCH GetPrevious();
	void PrintPreview(const VARIANT& EnableChanges);
	void Protect(const VARIANT& Password, const VARIANT& DrawingObjects, const VARIANT& Contents, const VARIANT& Scenarios, const VARIANT& UserInterfaceOnly);
	BOOL GetProtectContents();
	BOOL GetProtectDrawingObjects();
	BOOL GetProtectionMode();
	BOOL GetProtectScenarios();
	void SaveAs(LPCTSTR Filename, const VARIANT& FileFormat, const VARIANT& Password, const VARIANT& WriteResPassword, const VARIANT& ReadOnlyRecommended, const VARIANT& CreateBackup, const VARIANT& AddToMru, const VARIANT& TextCodepage, 
		const VARIANT& TextVisualLayout);
	void Select(const VARIANT& Replace);
	void Unprotect(const VARIANT& Password);
	long GetVisible();
	void SetVisible(long nNewValue);
	LPDISPATCH GetShapes();
	BOOL GetTransitionExpEval();
	void SetTransitionExpEval(BOOL bNewValue);
	BOOL GetAutoFilterMode();
	void SetAutoFilterMode(BOOL bNewValue);
	void SetBackgroundPicture(LPCTSTR Filename);
	void Calculate();
	BOOL GetEnableCalculation();
	void SetEnableCalculation(BOOL bNewValue);
	LPDISPATCH GetCells();
	LPDISPATCH ChartObjects(const VARIANT& Index);
	void CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	LPDISPATCH GetCircularReference();
	void ClearArrows();
	LPDISPATCH GetColumns();
	long GetConsolidationFunction();
	VARIANT GetConsolidationOptions();
	VARIANT GetConsolidationSources();
	BOOL GetEnableAutoFilter();
	void SetEnableAutoFilter(BOOL bNewValue);
	long GetEnableSelection();
	void SetEnableSelection(long nNewValue);
	BOOL GetEnableOutlining();
	void SetEnableOutlining(BOOL bNewValue);
	BOOL GetEnablePivotTable();
	void SetEnablePivotTable(BOOL bNewValue);
	VARIANT Evaluate(const VARIANT& Name);
	VARIANT _Evaluate(const VARIANT& Name);
	BOOL GetFilterMode();
	void ResetAllPageBreaks();
	LPDISPATCH GetNames();
	LPDISPATCH OLEObjects(const VARIANT& Index);
	LPDISPATCH GetOutline();
	void Paste(const VARIANT& Destination, const VARIANT& Link);
	void PasteSpecial(const VARIANT& Format, const VARIANT& Link, const VARIANT& DisplayAsIcon, const VARIANT& IconFileName, const VARIANT& IconIndex, const VARIANT& IconLabel);
	LPDISPATCH PivotTables(const VARIANT& Index);
	LPDISPATCH PivotTableWizard(const VARIANT& SourceType, const VARIANT& SourceData, const VARIANT& TableDestination, const VARIANT& TableName, const VARIANT& RowGrand, const VARIANT& ColumnGrand, const VARIANT& SaveData, 
		const VARIANT& HasAutoFormat, const VARIANT& AutoPage, const VARIANT& Reserved, const VARIANT& BackgroundQuery, const VARIANT& OptimizeCache, const VARIANT& PageFieldOrder, const VARIANT& PageFieldWrapCount, const VARIANT& ReadData, 
		const VARIANT& Connection);
	LPDISPATCH GetRange(const VARIANT& Cell1, const VARIANT& Cell2);
	LPDISPATCH GetRows();
	LPDISPATCH Scenarios(const VARIANT& Index);
	CString GetScrollArea();
	void SetScrollArea(LPCTSTR lpszNewValue);
	void ShowAllData();
	void ShowDataForm();
	double GetStandardHeight();
	double GetStandardWidth();
	void SetStandardWidth(double newValue);
	BOOL GetTransitionFormEntry();
	void SetTransitionFormEntry(BOOL bNewValue);
	long GetType();
	LPDISPATCH GetUsedRange();
	LPDISPATCH GetHPageBreaks();
	LPDISPATCH GetVPageBreaks();
	LPDISPATCH GetQueryTables();
	BOOL GetDisplayPageBreaks();
	void SetDisplayPageBreaks(BOOL bNewValue);
	LPDISPATCH GetComments();
	LPDISPATCH GetHyperlinks();
	void ClearCircles();
	void CircleInvalid();
	LPDISPATCH GetAutoFilter();
	BOOL GetDisplayRightToLeft();
	void SetDisplayRightToLeft(BOOL bNewValue);
	LPDISPATCH GetScripts();
	void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
};
/////////////////////////////////////////////////////////////////////////////
// _Global wrapper class

class __MY_EXT_CLASS__ Xl_Global : public COleDispatchDriver
{
public:
	Xl_Global() {}		// Calls COleDispatchDriver default constructor
	Xl_Global(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Global(const Xl_Global& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetActiveCell();
	LPDISPATCH GetActiveChart();
	CString GetActivePrinter();
	void SetActivePrinter(LPCTSTR lpszNewValue);
	LPDISPATCH GetActiveSheet();
	LPDISPATCH GetActiveWindow();
	LPDISPATCH GetActiveWorkbook();
	LPDISPATCH GetAddIns();
	LPDISPATCH GetAssistant();
	void Calculate();
	LPDISPATCH GetCells();
	LPDISPATCH GetCharts();
	LPDISPATCH GetColumns();
	LPDISPATCH GetCommandBars();
	long GetDDEAppReturnCode();
	void DDEExecute(long Channel, LPCTSTR String);
	long DDEInitiate(LPCTSTR App, LPCTSTR Topic);
	void DDEPoke(long Channel, const VARIANT& Item, const VARIANT& Data);
	VARIANT DDERequest(long Channel, LPCTSTR Item);
	void DDETerminate(long Channel);
	VARIANT Evaluate(const VARIANT& Name);
	VARIANT _Evaluate(const VARIANT& Name);
	VARIANT ExecuteExcel4Macro(LPCTSTR String);
	LPDISPATCH Intersect(LPDISPATCH Arg1, LPDISPATCH Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	LPDISPATCH GetNames();
	LPDISPATCH GetRange(const VARIANT& Cell1, const VARIANT& Cell2);
	LPDISPATCH GetRows();
	VARIANT Run(const VARIANT& Macro, const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, 
		const VARIANT& Arg10, const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, 
		const VARIANT& Arg20, const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, 
		const VARIANT& Arg30);
	VARIANT _Run2(const VARIANT& Macro, const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, 
		const VARIANT& Arg10, const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, 
		const VARIANT& Arg20, const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, 
		const VARIANT& Arg30);
	LPDISPATCH GetSelection();
	void SendKeys(const VARIANT& Keys, const VARIANT& Wait);
	LPDISPATCH GetSheets();
	LPDISPATCH GetThisWorkbook();
	LPDISPATCH Union(LPDISPATCH Arg1, LPDISPATCH Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, const VARIANT& Arg11, 
		const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, const VARIANT& Arg21, 
		const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	LPDISPATCH GetWindows();
	LPDISPATCH GetWorkbooks();
	LPDISPATCH GetWorksheetFunction();
	LPDISPATCH GetWorksheets();
	LPDISPATCH GetExcel4IntlMacroSheets();
	LPDISPATCH GetExcel4MacroSheets();
};
/////////////////////////////////////////////////////////////////////////////
// _Workbook wrapper class

class __MY_EXT_CLASS__ Xl_Workbook : public COleDispatchDriver
{
public:
	Xl_Workbook() {}		// Calls COleDispatchDriver default constructor
	Xl_Workbook(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Workbook(const Xl_Workbook& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	BOOL GetAcceptLabelsInFormulas();
	void SetAcceptLabelsInFormulas(BOOL bNewValue);
	void Activate();
	LPDISPATCH GetActiveChart();
	LPDISPATCH GetActiveSheet();
	long GetAutoUpdateFrequency();
	void SetAutoUpdateFrequency(long nNewValue);
	BOOL GetAutoUpdateSaveChanges();
	void SetAutoUpdateSaveChanges(BOOL bNewValue);
	long GetChangeHistoryDuration();
	void SetChangeHistoryDuration(long nNewValue);
	LPDISPATCH GetBuiltinDocumentProperties();
	void ChangeFileAccess(long Mode, const VARIANT& WritePassword, const VARIANT& Notify);
	void ChangeLink(LPCTSTR Name, LPCTSTR NewName, long Type);
	LPDISPATCH GetCharts();
	void Close(const VARIANT& SaveChanges, const VARIANT& Filename, const VARIANT& RouteWorkbook);
	CString GetCodeName();
	CString Get_CodeName();
	void Set_CodeName(LPCTSTR lpszNewValue);
	VARIANT GetColors(const VARIANT& Index);
	void SetColors(const VARIANT& Index, const VARIANT& newValue);
	LPDISPATCH GetCommandBars();
	long GetConflictResolution();
	void SetConflictResolution(long nNewValue);
	LPDISPATCH GetContainer();
	BOOL GetCreateBackup();
	LPDISPATCH GetCustomDocumentProperties();
	BOOL GetDate1904();
	void SetDate1904(BOOL bNewValue);
	void DeleteNumberFormat(LPCTSTR NumberFormat);
	long GetDisplayDrawingObjects();
	void SetDisplayDrawingObjects(long nNewValue);
	BOOL ExclusiveAccess();
	long GetFileFormat();
	void ForwardMailer();
	CString GetFullName();
	BOOL GetHasPassword();
	BOOL GetHasRoutingSlip();
	void SetHasRoutingSlip(BOOL bNewValue);
	BOOL GetIsAddin();
	void SetIsAddin(BOOL bNewValue);
	VARIANT LinkInfo(LPCTSTR Name, long LinkInfo, const VARIANT& Type, const VARIANT& EditionRef);
	VARIANT LinkSources(const VARIANT& Type);
	LPDISPATCH GetMailer();
	void MergeWorkbook(const VARIANT& Filename);
	BOOL GetMultiUserEditing();
	CString GetName();
	LPDISPATCH GetNames();
	LPDISPATCH NewWindow();
	void OpenLinks(LPCTSTR Name, const VARIANT& ReadOnly, const VARIANT& Type);
	CString GetPath();
	BOOL GetPersonalViewListSettings();
	void SetPersonalViewListSettings(BOOL bNewValue);
	BOOL GetPersonalViewPrintSettings();
	void SetPersonalViewPrintSettings(BOOL bNewValue);
	LPDISPATCH PivotCaches();
	void Post(const VARIANT& DestName);
	BOOL GetPrecisionAsDisplayed();
	void SetPrecisionAsDisplayed(BOOL bNewValue);
	void PrintPreview(const VARIANT& EnableChanges);
	void Protect(const VARIANT& Password, const VARIANT& Structure, const VARIANT& Windows);
	void ProtectSharing(const VARIANT& Filename, const VARIANT& Password, const VARIANT& WriteResPassword, const VARIANT& ReadOnlyRecommended, const VARIANT& CreateBackup, const VARIANT& SharingPassword);
	BOOL GetProtectStructure();
	BOOL GetProtectWindows();
	BOOL GetReadOnly();
	BOOL GetReadOnlyRecommended();
	void RefreshAll();
	void Reply();
	void ReplyAll();
	void RemoveUser(long Index);
	long GetRevisionNumber();
	void Route();
	BOOL GetRouted();
	LPDISPATCH GetRoutingSlip();
	void RunAutoMacros(long Which);
	void Save();
	void SaveAs(const VARIANT& Filename, const VARIANT& FileFormat, const VARIANT& Password, const VARIANT& WriteResPassword, const VARIANT& ReadOnlyRecommended, const VARIANT& CreateBackup, long AccessMode, const VARIANT& ConflictResolution, 
		const VARIANT& AddToMru, const VARIANT& TextCodepage, const VARIANT& TextVisualLayout);
	void SaveCopyAs(const VARIANT& Filename);
	BOOL GetSaved();
	void SetSaved(BOOL bNewValue);
	BOOL GetSaveLinkValues();
	void SetSaveLinkValues(BOOL bNewValue);
	void SendMail(const VARIANT& Recipients, const VARIANT& Subject, const VARIANT& ReturnReceipt);
	void SendMailer(const VARIANT& FileFormat, long Priority);
	void SetLinkOnData(LPCTSTR Name, const VARIANT& Procedure);
	LPDISPATCH GetSheets();
	BOOL GetShowConflictHistory();
	void SetShowConflictHistory(BOOL bNewValue);
	LPDISPATCH GetStyles();
	void Unprotect(const VARIANT& Password);
	void UnprotectSharing(const VARIANT& SharingPassword);
	void UpdateFromFile();
	void UpdateLink(const VARIANT& Name, const VARIANT& Type);
	BOOL GetUpdateRemoteReferences();
	void SetUpdateRemoteReferences(BOOL bNewValue);
	VARIANT GetUserStatus();
	LPDISPATCH GetCustomViews();
	LPDISPATCH GetWindows();
	LPDISPATCH GetWorksheets();
	BOOL GetWriteReserved();
	CString GetWriteReservedBy();
	LPDISPATCH GetExcel4IntlMacroSheets();
	LPDISPATCH GetExcel4MacroSheets();
	BOOL GetTemplateRemoveExtData();
	void SetTemplateRemoveExtData(BOOL bNewValue);
	void HighlightChangesOptions(const VARIANT& When, const VARIANT& Who, const VARIANT& Where);
	BOOL GetHighlightChangesOnScreen();
	void SetHighlightChangesOnScreen(BOOL bNewValue);
	BOOL GetKeepChangeHistory();
	void SetKeepChangeHistory(BOOL bNewValue);
	BOOL GetListChangesOnNewSheet();
	void SetListChangesOnNewSheet(BOOL bNewValue);
	void PurgeChangeHistoryNow(long Days, const VARIANT& SharingPassword);
	void AcceptAllChanges(const VARIANT& When, const VARIANT& Who, const VARIANT& Where);
	void RejectAllChanges(const VARIANT& When, const VARIANT& Who, const VARIANT& Where);
	void ResetColors();
	LPDISPATCH GetVBProject();
	void FollowHyperlink(LPCTSTR Address, const VARIANT& SubAddress, const VARIANT& NewWindow, const VARIANT& AddHistory, const VARIANT& ExtraInfo, const VARIANT& Method, const VARIANT& HeaderInfo);
	void AddToFavorites();
	BOOL GetIsInplace();
	void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
	void WebPagePreview();
	LPDISPATCH GetPublishObjects();
	LPDISPATCH GetWebOptions();
	void ReloadAs(long Encoding);
	LPDISPATCH GetHTMLProject();
	BOOL GetEnvelopeVisible();
	void SetEnvelopeVisible(BOOL bNewValue);
	long GetCalculationVersion();
	BOOL GetVBASigned();
};
/////////////////////////////////////////////////////////////////////////////
// Workbooks wrapper class

class __MY_EXT_CLASS__ Xl_Workbooks : public COleDispatchDriver
{
public:
	Xl_Workbooks() {}		// Calls COleDispatchDriver default constructor
	Xl_Workbooks(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Workbooks(const Xl_Workbooks& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(const VARIANT& Template);
	void Close();
	long GetCount();
	LPDISPATCH GetItem(const VARIANT& Index);
	LPUNKNOWN Get_NewEnum();
	LPDISPATCH Open(LPCTSTR Filename, const VARIANT& UpdateLinks, const VARIANT& ReadOnly, const VARIANT& Format, const VARIANT& Password, const VARIANT& WriteResPassword, const VARIANT& IgnoreReadOnlyRecommended, const VARIANT& Origin, 
		const VARIANT& Delimiter, const VARIANT& Editable, const VARIANT& Notify, const VARIANT& Converter, const VARIANT& AddToMru);
	LPDISPATCH Get_Default(const VARIANT& Index);
	void OpenText(LPCTSTR Filename, const VARIANT& Origin, const VARIANT& StartRow, const VARIANT& DataType, long TextQualifier, const VARIANT& ConsecutiveDelimiter, const VARIANT& Tab, const VARIANT& Semicolon, const VARIANT& Comma, 
		const VARIANT& Space, const VARIANT& Other, const VARIANT& OtherChar, const VARIANT& FieldInfo, const VARIANT& TextVisualLayout, const VARIANT& DecimalSeparator, const VARIANT& ThousandsSeparator);
};
/////////////////////////////////////////////////////////////////////////////
// PublishObject wrapper class

class __MY_EXT_CLASS__ Xl_PublishObject : public COleDispatchDriver
{
public:
	Xl_PublishObject() {}		// Calls COleDispatchDriver default constructor
	Xl_PublishObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_PublishObject(const Xl_PublishObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void Delete();
	void Publish(const VARIANT& Create);
	CString GetDivID();
	CString GetSheet();
	long GetSourceType();
	CString GetSource();
	long GetHtmlType();
	void SetHtmlType(long nNewValue);
	CString GetTitle();
	void SetTitle(LPCTSTR lpszNewValue);
	CString GetFilename();
	void SetFilename(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// DefaultWebOptions wrapper class

class __MY_EXT_CLASS__ Xl_DefaultWebOptions : public COleDispatchDriver
{
public:
	Xl_DefaultWebOptions() {}		// Calls COleDispatchDriver default constructor
	Xl_DefaultWebOptions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_DefaultWebOptions(const Xl_DefaultWebOptions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	BOOL GetRelyOnCSS();
	void SetRelyOnCSS(BOOL bNewValue);
	BOOL GetSaveHiddenData();
	void SetSaveHiddenData(BOOL bNewValue);
	BOOL GetLoadPictures();
	void SetLoadPictures(BOOL bNewValue);
	BOOL GetOrganizeInFolder();
	void SetOrganizeInFolder(BOOL bNewValue);
	BOOL GetUpdateLinksOnSave();
	void SetUpdateLinksOnSave(BOOL bNewValue);
	BOOL GetUseLongFileNames();
	void SetUseLongFileNames(BOOL bNewValue);
	BOOL GetCheckIfOfficeIsHTMLEditor();
	void SetCheckIfOfficeIsHTMLEditor(BOOL bNewValue);
	BOOL GetDownloadComponents();
	void SetDownloadComponents(BOOL bNewValue);
	BOOL GetRelyOnVML();
	void SetRelyOnVML(BOOL bNewValue);
	BOOL GetAllowPNG();
	void SetAllowPNG(BOOL bNewValue);
	long GetScreenSize();
	void SetScreenSize(long nNewValue);
	long GetPixelsPerInch();
	void SetPixelsPerInch(long nNewValue);
	CString GetLocationOfComponents();
	void SetLocationOfComponents(LPCTSTR lpszNewValue);
	long GetEncoding();
	void SetEncoding(long nNewValue);
	BOOL GetAlwaysSaveInDefaultEncoding();
	void SetAlwaysSaveInDefaultEncoding(BOOL bNewValue);
	LPDISPATCH GetFonts();
	CString GetFolderSuffix();
};
/////////////////////////////////////////////////////////////////////////////
// WebOptions wrapper class

class __MY_EXT_CLASS__ Xl_WebOptions : public COleDispatchDriver
{
public:
	Xl_WebOptions() {}		// Calls COleDispatchDriver default constructor
	Xl_WebOptions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_WebOptions(const Xl_WebOptions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	BOOL GetRelyOnCSS();
	void SetRelyOnCSS(BOOL bNewValue);
	BOOL GetOrganizeInFolder();
	void SetOrganizeInFolder(BOOL bNewValue);
	BOOL GetUseLongFileNames();
	void SetUseLongFileNames(BOOL bNewValue);
	BOOL GetDownloadComponents();
	void SetDownloadComponents(BOOL bNewValue);
	BOOL GetRelyOnVML();
	void SetRelyOnVML(BOOL bNewValue);
	BOOL GetAllowPNG();
	void SetAllowPNG(BOOL bNewValue);
	long GetScreenSize();
	void SetScreenSize(long nNewValue);
	long GetPixelsPerInch();
	void SetPixelsPerInch(long nNewValue);
	CString GetLocationOfComponents();
	void SetLocationOfComponents(LPCTSTR lpszNewValue);
	long GetEncoding();
	void SetEncoding(long nNewValue);
	CString GetFolderSuffix();
	void UseDefaultFolderSuffix();
};
/////////////////////////////////////////////////////////////////////////////
// TreeviewControl wrapper class

class __MY_EXT_CLASS__ Xl_TreeviewControl : public COleDispatchDriver
{
public:
	Xl_TreeviewControl() {}		// Calls COleDispatchDriver default constructor
	Xl_TreeviewControl(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_TreeviewControl(const Xl_TreeviewControl& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT GetHidden();
	void SetHidden(const VARIANT& newValue);
	VARIANT GetDrilled();
	void SetDrilled(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// CubeField wrapper class

class __MY_EXT_CLASS__ Xl_CubeField : public COleDispatchDriver
{
public:
	Xl_CubeField() {}		// Calls COleDispatchDriver default constructor
	Xl_CubeField(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_CubeField(const Xl_CubeField& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCubeFieldType();
	CString GetCaption();
	CString GetName();
	CString GetValue();
	long GetOrientation();
	void SetOrientation(long nNewValue);
	long GetPosition();
	void SetPosition(long nNewValue);
	LPDISPATCH GetTreeviewControl();
	BOOL GetDragToColumn();
	void SetDragToColumn(BOOL bNewValue);
	BOOL GetDragToHide();
	void SetDragToHide(BOOL bNewValue);
	BOOL GetDragToPage();
	void SetDragToPage(BOOL bNewValue);
	BOOL GetDragToRow();
	void SetDragToRow(BOOL bNewValue);
	BOOL GetDragToData();
	void SetDragToData(BOOL bNewValue);
	long GetHiddenLevels();
	void SetHiddenLevels(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// CubeFields wrapper class

class __MY_EXT_CLASS__ Xl_CubeFields : public COleDispatchDriver
{
public:
	Xl_CubeFields() {}		// Calls COleDispatchDriver default constructor
	Xl_CubeFields(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_CubeFields(const Xl_CubeFields& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH GetItem(const VARIANT& Index);
	LPDISPATCH Get_Default(const VARIANT& Index);
	LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Font wrapper class

class __MY_EXT_CLASS__ Xl_Font : public COleDispatchDriver
{
public:
	Xl_Font() {}		// Calls COleDispatchDriver default constructor
	Xl_Font(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Font(const Xl_Font& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT GetBackground();
	void SetBackground(const VARIANT& newValue);
	VARIANT GetBold();
	void SetBold(const VARIANT& newValue);
	VARIANT GetColor();
	void SetColor(const VARIANT& newValue);
	VARIANT GetColorIndex();
	void SetColorIndex(const VARIANT& newValue);
	VARIANT GetFontStyle();
	void SetFontStyle(const VARIANT& newValue);
	VARIANT GetItalic();
	void SetItalic(const VARIANT& newValue);
	VARIANT GetName();
	void SetName(const VARIANT& newValue);
	VARIANT GetOutlineFont();
	void SetOutlineFont(const VARIANT& newValue);
	VARIANT GetShadow();
	void SetShadow(const VARIANT& newValue);
	VARIANT GetSize();
	void SetSize(const VARIANT& newValue);
	VARIANT GetStrikethrough();
	void SetStrikethrough(const VARIANT& newValue);
	VARIANT GetSubscript();
	void SetSubscript(const VARIANT& newValue);
	VARIANT GetSuperscript();
	void SetSuperscript(const VARIANT& newValue);
	VARIANT GetUnderline();
	void SetUnderline(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// Window wrapper class

class __MY_EXT_CLASS__ Xl_Window : public COleDispatchDriver
{
public:
	Xl_Window() {}		// Calls COleDispatchDriver default constructor
	Xl_Window(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Window(const Xl_Window& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT Activate();
	VARIANT ActivateNext();
	VARIANT ActivatePrevious();
	LPDISPATCH GetActiveCell();
	LPDISPATCH GetActiveChart();
	LPDISPATCH GetActivePane();
	LPDISPATCH GetActiveSheet();
	VARIANT GetCaption();
	void SetCaption(const VARIANT& newValue);
	BOOL Close(const VARIANT& SaveChanges, const VARIANT& Filename, const VARIANT& RouteWorkbook);
	BOOL GetDisplayFormulas();
	void SetDisplayFormulas(BOOL bNewValue);
	BOOL GetDisplayGridlines();
	void SetDisplayGridlines(BOOL bNewValue);
	BOOL GetDisplayHeadings();
	void SetDisplayHeadings(BOOL bNewValue);
	BOOL GetDisplayHorizontalScrollBar();
	void SetDisplayHorizontalScrollBar(BOOL bNewValue);
	BOOL GetDisplayOutline();
	void SetDisplayOutline(BOOL bNewValue);
	BOOL GetDisplayVerticalScrollBar();
	void SetDisplayVerticalScrollBar(BOOL bNewValue);
	BOOL GetDisplayWorkbookTabs();
	void SetDisplayWorkbookTabs(BOOL bNewValue);
	BOOL GetDisplayZeros();
	void SetDisplayZeros(BOOL bNewValue);
	BOOL GetEnableResize();
	void SetEnableResize(BOOL bNewValue);
	BOOL GetFreezePanes();
	void SetFreezePanes(BOOL bNewValue);
	long GetGridlineColor();
	void SetGridlineColor(long nNewValue);
	long GetGridlineColorIndex();
	void SetGridlineColorIndex(long nNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	VARIANT LargeScroll(const VARIANT& Down, const VARIANT& Up, const VARIANT& ToRight, const VARIANT& ToLeft);
	double GetLeft();
	void SetLeft(double newValue);
	LPDISPATCH NewWindow();
	CString GetOnWindow();
	void SetOnWindow(LPCTSTR lpszNewValue);
	LPDISPATCH GetPanes();
	VARIANT PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
	VARIANT PrintPreview(const VARIANT& EnableChanges);
	LPDISPATCH GetRangeSelection();
	long GetScrollColumn();
	void SetScrollColumn(long nNewValue);
	long GetScrollRow();
	void SetScrollRow(long nNewValue);
	VARIANT ScrollWorkbookTabs(const VARIANT& Sheets, const VARIANT& Position);
	LPDISPATCH GetSelectedSheets();
	LPDISPATCH GetSelection();
	VARIANT SmallScroll(const VARIANT& Down, const VARIANT& Up, const VARIANT& ToRight, const VARIANT& ToLeft);
	BOOL GetSplit();
	void SetSplit(BOOL bNewValue);
	long GetSplitColumn();
	void SetSplitColumn(long nNewValue);
	double GetSplitHorizontal();
	void SetSplitHorizontal(double newValue);
	long GetSplitRow();
	void SetSplitRow(long nNewValue);
	double GetSplitVertical();
	void SetSplitVertical(double newValue);
	double GetTabRatio();
	void SetTabRatio(double newValue);
	double GetTop();
	void SetTop(double newValue);
	long GetType();
	double GetUsableHeight();
	double GetUsableWidth();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	LPDISPATCH GetVisibleRange();
	double GetWidth();
	void SetWidth(double newValue);
	long GetWindowNumber();
	long GetWindowState();
	void SetWindowState(long nNewValue);
	VARIANT GetZoom();
	void SetZoom(const VARIANT& newValue);
	long GetView();
	void SetView(long nNewValue);
	BOOL GetDisplayRightToLeft();
	void SetDisplayRightToLeft(BOOL bNewValue);
	long PointsToScreenPixelsX(long Points);
	long PointsToScreenPixelsY(long Points);
	LPDISPATCH RangeFromPoint(long x, long y);
	void ScrollIntoView(long Left, long Top, long Width, long Height, const VARIANT& Start);
};
/////////////////////////////////////////////////////////////////////////////
// Windows wrapper class

class __MY_EXT_CLASS__ Xl_Windows : public COleDispatchDriver
{
public:
	Xl_Windows() {}		// Calls COleDispatchDriver default constructor
	Xl_Windows(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Windows(const Xl_Windows& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT Arrange(long ArrangeStyle, const VARIANT& ActiveWorkbook, const VARIANT& SyncHorizontal, const VARIANT& SyncVertical);
	long GetCount();
	LPDISPATCH GetItem(const VARIANT& Index);
	LPUNKNOWN Get_NewEnum();
	LPDISPATCH Get_Default(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// AppEvents wrapper class

class __MY_EXT_CLASS__ Xl_AppEvents : public COleDispatchDriver
{
public:
	Xl_AppEvents() {}		// Calls COleDispatchDriver default constructor
	Xl_AppEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_AppEvents(const Xl_AppEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void NewWorkbook(LPDISPATCH Wb);
	void SheetSelectionChange(LPDISPATCH Sh, LPDISPATCH Target);
	void SheetBeforeDoubleClick(LPDISPATCH Sh, LPDISPATCH Target, BOOL* Cancel);
	void SheetBeforeRightClick(LPDISPATCH Sh, LPDISPATCH Target, BOOL* Cancel);
	void SheetActivate(LPDISPATCH Sh);
	void SheetDeactivate(LPDISPATCH Sh);
	void SheetCalculate(LPDISPATCH Sh);
	void SheetChange(LPDISPATCH Sh, LPDISPATCH Target);
	void WorkbookOpen(LPDISPATCH Wb);
	void WorkbookActivate(LPDISPATCH Wb);
	void WorkbookDeactivate(LPDISPATCH Wb);
	void WorkbookBeforeClose(LPDISPATCH Wb, BOOL* Cancel);
	void WorkbookBeforeSave(LPDISPATCH Wb, BOOL SaveAsUI, BOOL* Cancel);
	void WorkbookBeforePrint(LPDISPATCH Wb, BOOL* Cancel);
	void WorkbookNewSheet(LPDISPATCH Wb, LPDISPATCH Sh);
	void WorkbookAddinInstall(LPDISPATCH Wb);
	void WorkbookAddinUninstall(LPDISPATCH Wb);
	void WindowResize(LPDISPATCH Wb, LPDISPATCH Wn);
	void WindowActivate(LPDISPATCH Wb, LPDISPATCH Wn);
	void WindowDeactivate(LPDISPATCH Wb, LPDISPATCH Wn);
	void SheetFollowHyperlink(LPDISPATCH Sh, LPDISPATCH Target);
};
/////////////////////////////////////////////////////////////////////////////
// WorksheetFunction wrapper class

class __MY_EXT_CLASS__ Xl_WorksheetFunction : public COleDispatchDriver
{
public:
	Xl_WorksheetFunction() {}		// Calls COleDispatchDriver default constructor
	Xl_WorksheetFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_WorksheetFunction(const Xl_WorksheetFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	double Count(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	BOOL IsNA(const VARIANT& Arg1);
	BOOL IsError(const VARIANT& Arg1);
	double Sum(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double Average(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double Min(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double Max(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double Npv(double Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, const VARIANT& Arg11, 
		const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, const VARIANT& Arg21, 
		const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double StDev(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	CString Dollar(double Arg1, const VARIANT& Arg2);
	CString Fixed(double Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	double Pi();
	double Ln(double Arg1);
	double Log10(double Arg1);
	double Round(double Arg1, double Arg2);
	VARIANT Lookup(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	VARIANT Index(const VARIANT& Arg1, double Arg2, const VARIANT& Arg3, const VARIANT& Arg4);
	CString Rept(LPCTSTR Arg1, double Arg2);
	BOOL And(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, const VARIANT& Arg11, 
		const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, const VARIANT& Arg21, 
		const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	BOOL Or(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, const VARIANT& Arg11, 
		const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, const VARIANT& Arg21, 
		const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double DCount(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	double DSum(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	double DAverage(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	double DMin(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	double DMax(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	double DStDev(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	double Var(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double DVar(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	CString Text(const VARIANT& Arg1, LPCTSTR Arg2);
	VARIANT LinEst(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4);
	VARIANT Trend(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4);
	VARIANT LogEst(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4);
	VARIANT Growth(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4);
	double Pv(double Arg1, double Arg2, double Arg3, const VARIANT& Arg4, const VARIANT& Arg5);
	double Fv(double Arg1, double Arg2, double Arg3, const VARIANT& Arg4, const VARIANT& Arg5);
	double NPer(double Arg1, double Arg2, double Arg3, const VARIANT& Arg4, const VARIANT& Arg5);
	double Pmt(double Arg1, double Arg2, double Arg3, const VARIANT& Arg4, const VARIANT& Arg5);
	double Rate(double Arg1, double Arg2, double Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6);
	double MIrr(const VARIANT& Arg1, double Arg2, double Arg3);
	double Irr(const VARIANT& Arg1, const VARIANT& Arg2);
	double Match(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	double Weekday(const VARIANT& Arg1, const VARIANT& Arg2);
	double Search(LPCTSTR Arg1, LPCTSTR Arg2, const VARIANT& Arg3);
	VARIANT Transpose(const VARIANT& Arg1);
	double Atan2(double Arg1, double Arg2);
	double Asin(double Arg1);
	double Acos(double Arg1);
	VARIANT Choose(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	VARIANT HLookup(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4);
	VARIANT VLookup(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4);
	double Log(double Arg1, const VARIANT& Arg2);
	CString Proper(LPCTSTR Arg1);
	CString Trim(LPCTSTR Arg1);
	CString Replace(LPCTSTR Arg1, double Arg2, double Arg3, LPCTSTR Arg4);
	CString Substitute(LPCTSTR Arg1, LPCTSTR Arg2, LPCTSTR Arg3, const VARIANT& Arg4);
	double Find(LPCTSTR Arg1, LPCTSTR Arg2, const VARIANT& Arg3);
	BOOL IsErr(const VARIANT& Arg1);
	BOOL IsText(const VARIANT& Arg1);
	BOOL IsNumber(const VARIANT& Arg1);
	double Sln(double Arg1, double Arg2, double Arg3);
	double Syd(double Arg1, double Arg2, double Arg3, double Arg4);
	double Ddb(double Arg1, double Arg2, double Arg3, double Arg4, const VARIANT& Arg5);
	CString Clean(LPCTSTR Arg1);
	double MDeterm(const VARIANT& Arg1);
	VARIANT MInverse(const VARIANT& Arg1);
	VARIANT MMult(const VARIANT& Arg1, const VARIANT& Arg2);
	double Ipmt(double Arg1, double Arg2, double Arg3, double Arg4, const VARIANT& Arg5, const VARIANT& Arg6);
	double Ppmt(double Arg1, double Arg2, double Arg3, double Arg4, const VARIANT& Arg5, const VARIANT& Arg6);
	double CountA(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double Product(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double Fact(double Arg1);
	double DProduct(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	BOOL IsNonText(const VARIANT& Arg1);
	double StDevP(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double VarP(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double DStDevP(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	double DVarP(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	BOOL IsLogical(const VARIANT& Arg1);
	double DCountA(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	CString USDollar(double Arg1, double Arg2);
	double FindB(LPCTSTR Arg1, LPCTSTR Arg2, const VARIANT& Arg3);
	double SearchB(LPCTSTR Arg1, LPCTSTR Arg2, const VARIANT& Arg3);
	CString ReplaceB(LPCTSTR Arg1, double Arg2, double Arg3, LPCTSTR Arg4);
	double RoundUp(double Arg1, double Arg2);
	double RoundDown(double Arg1, double Arg2);
	double Rank(double Arg1, LPDISPATCH Arg2, const VARIANT& Arg3);
	double Days360(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	double Vdb(double Arg1, double Arg2, double Arg3, double Arg4, double Arg5, const VARIANT& Arg6, const VARIANT& Arg7);
	double Median(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double SumProduct(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double Sinh(double Arg1);
	double Cosh(double Arg1);
	double Tanh(double Arg1);
	double Asinh(double Arg1);
	double Acosh(double Arg1);
	double Atanh(double Arg1);
	VARIANT DGet(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	double Db(double Arg1, double Arg2, double Arg3, double Arg4, const VARIANT& Arg5);
	VARIANT Frequency(const VARIANT& Arg1, const VARIANT& Arg2);
	double AveDev(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double BetaDist(double Arg1, double Arg2, double Arg3, const VARIANT& Arg4, const VARIANT& Arg5);
	double GammaLn(double Arg1);
	double BetaInv(double Arg1, double Arg2, double Arg3, const VARIANT& Arg4, const VARIANT& Arg5);
	double BinomDist(double Arg1, double Arg2, double Arg3, BOOL Arg4);
	double ChiDist(double Arg1, double Arg2);
	double ChiInv(double Arg1, double Arg2);
	double Combin(double Arg1, double Arg2);
	double Confidence(double Arg1, double Arg2, double Arg3);
	double CritBinom(double Arg1, double Arg2, double Arg3);
	double Even(double Arg1);
	double ExponDist(double Arg1, double Arg2, BOOL Arg3);
	double FDist(double Arg1, double Arg2, double Arg3);
	double FInv(double Arg1, double Arg2, double Arg3);
	double Fisher(double Arg1);
	double FisherInv(double Arg1);
	double Floor(double Arg1, double Arg2);
	double GammaDist(double Arg1, double Arg2, double Arg3, BOOL Arg4);
	double GammaInv(double Arg1, double Arg2, double Arg3);
	double Ceiling(double Arg1, double Arg2);
	double HypGeomDist(double Arg1, double Arg2, double Arg3, double Arg4);
	double LogNormDist(double Arg1, double Arg2, double Arg3);
	double LogInv(double Arg1, double Arg2, double Arg3);
	double NegBinomDist(double Arg1, double Arg2, double Arg3);
	double NormDist(double Arg1, double Arg2, double Arg3, BOOL Arg4);
	double NormSDist(double Arg1);
	double NormInv(double Arg1, double Arg2, double Arg3);
	double NormSInv(double Arg1);
	double Standardize(double Arg1, double Arg2, double Arg3);
	double Odd(double Arg1);
	double Permut(double Arg1, double Arg2);
	double Poisson(double Arg1, double Arg2, BOOL Arg3);
	double TDist(double Arg1, double Arg2, double Arg3);
	double Weibull(double Arg1, double Arg2, double Arg3, BOOL Arg4);
	double SumXMY2(const VARIANT& Arg1, const VARIANT& Arg2);
	double SumX2MY2(const VARIANT& Arg1, const VARIANT& Arg2);
	double SumX2PY2(const VARIANT& Arg1, const VARIANT& Arg2);
	double ChiTest(const VARIANT& Arg1, const VARIANT& Arg2);
	double Correl(const VARIANT& Arg1, const VARIANT& Arg2);
	double Covar(const VARIANT& Arg1, const VARIANT& Arg2);
	double Forecast(double Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	double FTest(const VARIANT& Arg1, const VARIANT& Arg2);
	double Intercept(const VARIANT& Arg1, const VARIANT& Arg2);
	double Pearson(const VARIANT& Arg1, const VARIANT& Arg2);
	double RSq(const VARIANT& Arg1, const VARIANT& Arg2);
	double StEyx(const VARIANT& Arg1, const VARIANT& Arg2);
	double Slope(const VARIANT& Arg1, const VARIANT& Arg2);
	double TTest(const VARIANT& Arg1, const VARIANT& Arg2, double Arg3, double Arg4);
	double Prob(const VARIANT& Arg1, const VARIANT& Arg2, double Arg3, const VARIANT& Arg4);
	double DevSq(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double GeoMean(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double HarMean(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double SumSq(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double Kurt(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double Skew(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double ZTest(const VARIANT& Arg1, double Arg2, const VARIANT& Arg3);
	double Large(const VARIANT& Arg1, double Arg2);
	double Small(const VARIANT& Arg1, double Arg2);
	double Quartile(const VARIANT& Arg1, double Arg2);
	double Percentile(const VARIANT& Arg1, double Arg2);
	double PercentRank(const VARIANT& Arg1, double Arg2, const VARIANT& Arg3);
	double Mode(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double TrimMean(const VARIANT& Arg1, double Arg2);
	double TInv(double Arg1, double Arg2);
	double Power(double Arg1, double Arg2);
	double Radians(double Arg1);
	double Degrees(double Arg1);
	double Subtotal(double Arg1, LPDISPATCH Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, const VARIANT& Arg11, 
		const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, const VARIANT& Arg21, 
		const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	double SumIf(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
	double CountIf(LPDISPATCH Arg1, const VARIANT& Arg2);
	double CountBlank(LPDISPATCH Arg1);
	double Ispmt(double Arg1, double Arg2, double Arg3, double Arg4);
	CString Roman(double Arg1, const VARIANT& Arg2);
	CString Asc(LPCTSTR Arg1);
	CString Dbcs(LPCTSTR Arg1);
	CString Phonetic(LPDISPATCH Arg1);
};
/////////////////////////////////////////////////////////////////////////////
// Range wrapper class

class __MY_EXT_CLASS__ Xl_Range : public COleDispatchDriver
{
public:
	Xl_Range() {}		// Calls COleDispatchDriver default constructor
	Xl_Range(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Range(const Xl_Range& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT Activate();
	VARIANT GetAddIndent();
	void SetAddIndent(const VARIANT& newValue);
	CString GetAddress(const VARIANT& RowAbsolute, const VARIANT& ColumnAbsolute, long ReferenceStyle, const VARIANT& External, const VARIANT& RelativeTo);
	CString GetAddressLocal(const VARIANT& RowAbsolute, const VARIANT& ColumnAbsolute, long ReferenceStyle, const VARIANT& External, const VARIANT& RelativeTo);
	VARIANT AdvancedFilter(long Action, const VARIANT& CriteriaRange, const VARIANT& CopyToRange, const VARIANT& Unique);
	VARIANT ApplyNames(const VARIANT& Names, const VARIANT& IgnoreRelativeAbsolute, const VARIANT& UseRowColumnNames, const VARIANT& OmitColumn, const VARIANT& OmitRow, long Order, const VARIANT& AppendLast);
	VARIANT ApplyOutlineStyles();
	LPDISPATCH GetAreas();
	CString AutoComplete(LPCTSTR String);
	VARIANT AutoFill(LPDISPATCH Destination, long Type);
	VARIANT AutoFilter(const VARIANT& Field, const VARIANT& Criteria1, long Operator, const VARIANT& Criteria2, const VARIANT& VisibleDropDown);
	VARIANT AutoFit();
	VARIANT AutoFormat(long Format, const VARIANT& Number, const VARIANT& Font, const VARIANT& Alignment, const VARIANT& Border, const VARIANT& Pattern, const VARIANT& Width);
	VARIANT AutoOutline();
	VARIANT BorderAround(const VARIANT& LineStyle, long Weight, long ColorIndex, const VARIANT& Color);
	LPDISPATCH GetBorders();
	VARIANT Calculate();
	LPDISPATCH GetCells();
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	VARIANT Clear();
	VARIANT ClearContents();
	VARIANT ClearFormats();
	VARIANT ClearNotes();
	VARIANT ClearOutline();
	long GetColumn();
	LPDISPATCH ColumnDifferences(const VARIANT& Comparison);
	LPDISPATCH GetColumns();
	VARIANT GetColumnWidth();
	void SetColumnWidth(const VARIANT& newValue);
	VARIANT Consolidate(const VARIANT& Sources, const VARIANT& Function, const VARIANT& TopRow, const VARIANT& LeftColumn, const VARIANT& CreateLinks);
	VARIANT Copy(const VARIANT& Destination);
	long CopyFromRecordset(LPUNKNOWN Data, const VARIANT& MaxRows, const VARIANT& MaxColumns);
	VARIANT CopyPicture(long Appearance, long Format);
	long GetCount();
	VARIANT CreateNames(const VARIANT& Top, const VARIANT& Left, const VARIANT& Bottom, const VARIANT& Right);
	VARIANT CreatePublisher(const VARIANT& Edition, long Appearance, const VARIANT& ContainsPICT, const VARIANT& ContainsBIFF, const VARIANT& ContainsRTF, const VARIANT& ContainsVALU);
	LPDISPATCH GetCurrentArray();
	LPDISPATCH GetCurrentRegion();
	VARIANT Cut(const VARIANT& Destination);
	VARIANT DataSeries(const VARIANT& Rowcol, long Type, long Date, const VARIANT& Step, const VARIANT& Stop, const VARIANT& Trend);
	VARIANT Get_Default(const VARIANT& RowIndex, const VARIANT& ColumnIndex);
	void Set_Default(const VARIANT& RowIndex, const VARIANT& ColumnIndex, const VARIANT& newValue);
	VARIANT Delete(const VARIANT& Shift);
	LPDISPATCH GetDependents();
	VARIANT DialogBox_();
	LPDISPATCH GetDirectDependents();
	LPDISPATCH GetDirectPrecedents();
	VARIANT EditionOptions(long Type, long Option, const VARIANT& Name, const VARIANT& Reference, long Appearance, long ChartSize, const VARIANT& Format);
	LPDISPATCH GetEnd(long Direction);
	LPDISPATCH GetEntireColumn();
	LPDISPATCH GetEntireRow();
	VARIANT FillDown();
	VARIANT FillLeft();
	VARIANT FillRight();
	VARIANT FillUp();
	LPDISPATCH Find(const VARIANT& What, const VARIANT& After, const VARIANT& LookIn, const VARIANT& LookAt, const VARIANT& SearchOrder, long SearchDirection, const VARIANT& MatchCase, const VARIANT& MatchByte);
	LPDISPATCH FindNext(const VARIANT& After);
	LPDISPATCH FindPrevious(const VARIANT& After);
	LPDISPATCH GetFont();
	VARIANT GetFormula();
	void SetFormula(const VARIANT& newValue);
	VARIANT GetFormulaArray();
	void SetFormulaArray(const VARIANT& newValue);
	long GetFormulaLabel();
	void SetFormulaLabel(long nNewValue);
	VARIANT GetFormulaHidden();
	void SetFormulaHidden(const VARIANT& newValue);
	VARIANT GetFormulaLocal();
	void SetFormulaLocal(const VARIANT& newValue);
	VARIANT GetFormulaR1C1();
	void SetFormulaR1C1(const VARIANT& newValue);
	VARIANT GetFormulaR1C1Local();
	void SetFormulaR1C1Local(const VARIANT& newValue);
	VARIANT FunctionWizard();
	BOOL GoalSeek(const VARIANT& Goal, LPDISPATCH ChangingCell);
	VARIANT Group(const VARIANT& Start, const VARIANT& End, const VARIANT& By, const VARIANT& Periods);
	VARIANT GetHasArray();
	VARIANT GetHasFormula();
	VARIANT GetHeight();
	VARIANT GetHidden();
	void SetHidden(const VARIANT& newValue);
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	VARIANT GetIndentLevel();
	void SetIndentLevel(const VARIANT& newValue);
	void InsertIndent(long InsertAmount);
	VARIANT Insert(const VARIANT& Shift);
	LPDISPATCH GetInterior();
	VARIANT GetItem(const VARIANT& RowIndex, const VARIANT& ColumnIndex);
	void SetItem(const VARIANT& RowIndex, const VARIANT& ColumnIndex, const VARIANT& newValue);
	VARIANT Justify();
	VARIANT GetLeft();
	long GetListHeaderRows();
	VARIANT ListNames();
	long GetLocationInTable();
	VARIANT GetLocked();
	void SetLocked(const VARIANT& newValue);
	void Merge(const VARIANT& Across);
	void UnMerge();
	LPDISPATCH GetMergeArea();
	VARIANT GetMergeCells();
	void SetMergeCells(const VARIANT& newValue);
	VARIANT GetName();
	void SetName(const VARIANT& newValue);
	VARIANT NavigateArrow(const VARIANT& TowardPrecedent, const VARIANT& ArrowNumber, const VARIANT& LinkNumber);
	LPUNKNOWN Get_NewEnum();
	LPDISPATCH GetNext();
	CString NoteText(const VARIANT& Text, const VARIANT& Start, const VARIANT& Length);
	VARIANT GetNumberFormat();
	void SetNumberFormat(const VARIANT& newValue);
	VARIANT GetNumberFormatLocal();
	void SetNumberFormatLocal(const VARIANT& newValue);
	LPDISPATCH GetOffset(const VARIANT& RowOffset, const VARIANT& ColumnOffset);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	VARIANT GetOutlineLevel();
	void SetOutlineLevel(const VARIANT& newValue);
	long GetPageBreak();
	void SetPageBreak(long nNewValue);
	VARIANT Parse(const VARIANT& ParseLine, const VARIANT& Destination);
	VARIANT PasteSpecial(long Paste, long Operation, const VARIANT& SkipBlanks, const VARIANT& Transpose);
	LPDISPATCH GetPivotField();
	LPDISPATCH GetPivotItem();
	LPDISPATCH GetPivotTable();
	LPDISPATCH GetPrecedents();
	VARIANT GetPrefixCharacter();
	LPDISPATCH GetPrevious();
	VARIANT _PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate);
	VARIANT PrintPreview(const VARIANT& EnableChanges);
	LPDISPATCH GetQueryTable();
	LPDISPATCH GetRange(const VARIANT& Cell1, const VARIANT& Cell2);
	VARIANT RemoveSubtotal();
	BOOL Replace(const VARIANT& What, const VARIANT& Replacement, const VARIANT& LookAt, const VARIANT& SearchOrder, const VARIANT& MatchCase, const VARIANT& MatchByte);
	LPDISPATCH GetResize(const VARIANT& RowSize, const VARIANT& ColumnSize);
	long GetRow();
	LPDISPATCH RowDifferences(const VARIANT& Comparison);
	VARIANT GetRowHeight();
	void SetRowHeight(const VARIANT& newValue);
	LPDISPATCH GetRows();
	VARIANT Run(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, 
		const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, 
		const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
	VARIANT Select();
	VARIANT Show();
	VARIANT ShowDependents(const VARIANT& Remove);
	VARIANT GetShowDetail();
	void SetShowDetail(const VARIANT& newValue);
	VARIANT ShowErrors();
	VARIANT ShowPrecedents(const VARIANT& Remove);
	VARIANT GetShrinkToFit();
	void SetShrinkToFit(const VARIANT& newValue);
	VARIANT Sort(const VARIANT& Key1, long Order1, const VARIANT& Key2, const VARIANT& Type, long Order2, const VARIANT& Key3, long Order3, long Header, const VARIANT& OrderCustom, const VARIANT& MatchCase, long Orientation, long SortMethod);
	VARIANT SortSpecial(long SortMethod, const VARIANT& Key1, long Order1, const VARIANT& Type, const VARIANT& Key2, long Order2, const VARIANT& Key3, long Order3, long Header, const VARIANT& OrderCustom, const VARIANT& MatchCase, 
		long Orientation);
	LPDISPATCH GetSoundNote();
	LPDISPATCH SpecialCells(long Type, const VARIANT& Value);
	VARIANT GetStyle();
	void SetStyle(const VARIANT& newValue);
	VARIANT SubscribeTo(LPCTSTR Edition, long Format);
	VARIANT Subtotal(long GroupBy, long Function, const VARIANT& TotalList, const VARIANT& Replace, const VARIANT& PageBreaks, long SummaryBelowData);
	VARIANT GetSummary();
	VARIANT Table(const VARIANT& RowInput, const VARIANT& ColumnInput);
	VARIANT GetText();
	VARIANT TextToColumns(const VARIANT& Destination, long DataType, long TextQualifier, const VARIANT& ConsecutiveDelimiter, const VARIANT& Tab, const VARIANT& Semicolon, const VARIANT& Comma, const VARIANT& Space, const VARIANT& Other, 
		const VARIANT& OtherChar, const VARIANT& FieldInfo, const VARIANT& DecimalSeparator, const VARIANT& ThousandsSeparator);
	VARIANT GetTop();
	VARIANT Ungroup();
	VARIANT GetUseStandardHeight();
	void SetUseStandardHeight(const VARIANT& newValue);
	VARIANT GetUseStandardWidth();
	void SetUseStandardWidth(const VARIANT& newValue);
	LPDISPATCH GetValidation();
	VARIANT GetValue();
	void SetValue(const VARIANT& newValue);
	VARIANT GetValue2();
	void SetValue2(const VARIANT& newValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	VARIANT GetWidth();
	LPDISPATCH GetWorksheet();
	VARIANT GetWrapText();
	void SetWrapText(const VARIANT& newValue);
	LPDISPATCH AddComment(const VARIANT& Text);
	LPDISPATCH GetComment();
	void ClearComments();
	LPDISPATCH GetPhonetic();
	LPDISPATCH GetFormatConditions();
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	LPDISPATCH GetHyperlinks();
	LPDISPATCH GetPhonetics();
	void SetPhonetic();
	CString GetId();
	void SetId(LPCTSTR lpszNewValue);
	VARIANT PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
};
/////////////////////////////////////////////////////////////////////////////
// ChartEvents wrapper class

class __MY_EXT_CLASS__ Xl_ChartEvents : public COleDispatchDriver
{
public:
	Xl_ChartEvents() {}		// Calls COleDispatchDriver default constructor
	Xl_ChartEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ChartEvents(const Xl_ChartEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Activate();
	void Deactivate();
	void Resize();
	void MouseDown(long Button, long Shift, long x, long y);
	void MouseUp(long Button, long Shift, long x, long y);
	void MouseMove(long Button, long Shift, long x, long y);
	void BeforeRightClick(BOOL* Cancel);
	void DragPlot();
	void DragOver();
	void BeforeDoubleClick(long ElementID, long Arg1, long Arg2, BOOL* Cancel);
	void Select(long ElementID, long Arg1, long Arg2);
	void SeriesChange(long SeriesIndex, long PointIndex);
	void Calculate();
};
/////////////////////////////////////////////////////////////////////////////
// VPageBreak wrapper class

class __MY_EXT_CLASS__ Xl_VPageBreak : public COleDispatchDriver
{
public:
	Xl_VPageBreak() {}		// Calls COleDispatchDriver default constructor
	Xl_VPageBreak(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_VPageBreak(const Xl_VPageBreak& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void Delete();
	void DragOff(long Direction, long RegionIndex);
	long GetType();
	void SetType(long nNewValue);
	long GetExtent();
	LPDISPATCH GetLocation();
	void SetRefLocation(LPDISPATCH newValue);
};
/////////////////////////////////////////////////////////////////////////////
// HPageBreak wrapper class

class __MY_EXT_CLASS__ Xl_HPageBreak : public COleDispatchDriver
{
public:
	Xl_HPageBreak() {}		// Calls COleDispatchDriver default constructor
	Xl_HPageBreak(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_HPageBreak(const Xl_HPageBreak& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void Delete();
	void DragOff(long Direction, long RegionIndex);
	long GetType();
	void SetType(long nNewValue);
	long GetExtent();
	LPDISPATCH GetLocation();
	void SetRefLocation(LPDISPATCH newValue);
};
/////////////////////////////////////////////////////////////////////////////
// Xl_HPageBreaks wrapper class

class __MY_EXT_CLASS__ Xl_HPageBreaks : public COleDispatchDriver
{
public:
	Xl_HPageBreaks() {}		// Calls COleDispatchDriver default constructor
	Xl_HPageBreaks(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_HPageBreaks(const Xl_HPageBreaks& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH GetItem(long Index);
	LPDISPATCH Get_Default(long Index);
	LPUNKNOWN Get_NewEnum();
	LPDISPATCH Add(LPDISPATCH Before);
};
/////////////////////////////////////////////////////////////////////////////
// VPageBreaks wrapper class

class __MY_EXT_CLASS__ Xl_VPageBreaks : public COleDispatchDriver
{
public:
	Xl_VPageBreaks() {}		// Calls COleDispatchDriver default constructor
	Xl_VPageBreaks(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_VPageBreaks(const Xl_VPageBreaks& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH GetItem(long Index);
	LPDISPATCH Get_Default(long Index);
	LPUNKNOWN Get_NewEnum();
	LPDISPATCH Add(LPDISPATCH Before);
};
/////////////////////////////////////////////////////////////////////////////
// RecentFile wrapper class

class __MY_EXT_CLASS__ Xl_RecentFile : public COleDispatchDriver
{
public:
	Xl_RecentFile() {}		// Calls COleDispatchDriver default constructor
	Xl_RecentFile(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_RecentFile(const Xl_RecentFile& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	CString GetPath();
	long GetIndex();
	LPDISPATCH Open();
	void Delete();
};
/////////////////////////////////////////////////////////////////////////////
// RecentFiles wrapper class

class __MY_EXT_CLASS__ Xl_RecentFiles : public COleDispatchDriver
{
public:
	Xl_RecentFiles() {}		// Calls COleDispatchDriver default constructor
	Xl_RecentFiles(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_RecentFiles(const Xl_RecentFiles& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetMaximum();
	void SetMaximum(long nNewValue);
	long GetCount();
	LPDISPATCH GetItem(long Index);
	LPDISPATCH Get_Default(long Index);
	LPDISPATCH Add(LPCTSTR Name);
	LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// DocEvents wrapper class

class __MY_EXT_CLASS__ Xl_DocEvents : public COleDispatchDriver
{
public:
	Xl_DocEvents() {}		// Calls COleDispatchDriver default constructor
	Xl_DocEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_DocEvents(const Xl_DocEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void SelectionChange(LPDISPATCH Target);
	void BeforeDoubleClick(LPDISPATCH Target, BOOL* Cancel);
	void BeforeRightClick(LPDISPATCH Target, BOOL* Cancel);
	void Activate();
	void Deactivate();
	void Calculate();
	void Change(LPDISPATCH Target);
	void FollowHyperlink(LPDISPATCH Target);
};
/////////////////////////////////////////////////////////////////////////////
// Style wrapper class

class __MY_EXT_CLASS__ Xl_Style : public COleDispatchDriver
{
public:
	Xl_Style() {}		// Calls COleDispatchDriver default constructor
	Xl_Style(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Style(const Xl_Style& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	BOOL GetAddIndent();
	void SetAddIndent(BOOL bNewValue);
	BOOL GetBuiltIn();
	LPDISPATCH GetBorders();
	VARIANT Delete();
	LPDISPATCH GetFont();
	BOOL GetFormulaHidden();
	void SetFormulaHidden(BOOL bNewValue);
	long GetHorizontalAlignment();
	void SetHorizontalAlignment(long nNewValue);
	BOOL GetIncludeAlignment();
	void SetIncludeAlignment(BOOL bNewValue);
	BOOL GetIncludeBorder();
	void SetIncludeBorder(BOOL bNewValue);
	BOOL GetIncludeFont();
	void SetIncludeFont(BOOL bNewValue);
	BOOL GetIncludeNumber();
	void SetIncludeNumber(BOOL bNewValue);
	BOOL GetIncludePatterns();
	void SetIncludePatterns(BOOL bNewValue);
	BOOL GetIncludeProtection();
	void SetIncludeProtection(BOOL bNewValue);
	long GetIndentLevel();
	void SetIndentLevel(long nNewValue);
	LPDISPATCH GetInterior();
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetMergeCells();
	void SetMergeCells(const VARIANT& newValue);
	CString GetName();
	CString GetNameLocal();
	CString GetNumberFormat();
	void SetNumberFormat(LPCTSTR lpszNewValue);
	CString GetNumberFormatLocal();
	void SetNumberFormatLocal(LPCTSTR lpszNewValue);
	long GetOrientation();
	void SetOrientation(long nNewValue);
	BOOL GetShrinkToFit();
	void SetShrinkToFit(BOOL bNewValue);
	CString GetValue();
	long GetVerticalAlignment();
	void SetVerticalAlignment(long nNewValue);
	BOOL GetWrapText();
	void SetWrapText(BOOL bNewValue);
	CString Get_Default();
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Styles wrapper class

class __MY_EXT_CLASS__ Xl_Styles : public COleDispatchDriver
{
public:
	Xl_Styles() {}		// Calls COleDispatchDriver default constructor
	Xl_Styles(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Styles(const Xl_Styles& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(LPCTSTR Name, const VARIANT& BasedOn);
	long GetCount();
	LPDISPATCH GetItem(const VARIANT& Index);
	VARIANT Merge(const VARIANT& Workbook);
	LPUNKNOWN Get_NewEnum();
	LPDISPATCH Get_Default(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// Borders wrapper class

class __MY_EXT_CLASS__ Xl_Borders : public COleDispatchDriver
{
public:
	Xl_Borders() {}		// Calls COleDispatchDriver default constructor
	Xl_Borders(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Borders(const Xl_Borders& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT GetColor();
	void SetColor(const VARIANT& newValue);
	VARIANT GetColorIndex();
	void SetColorIndex(const VARIANT& newValue);
	long GetCount();
	LPDISPATCH GetItem(long Index);
	VARIANT GetLineStyle();
	void SetLineStyle(const VARIANT& newValue);
	LPUNKNOWN Get_NewEnum();
	VARIANT GetValue();
	void SetValue(const VARIANT& newValue);
	VARIANT GetWeight();
	void SetWeight(const VARIANT& newValue);
	LPDISPATCH Get_Default(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// AddIn wrapper class

class __MY_EXT_CLASS__ Xl_AddIn : public COleDispatchDriver
{
public:
	Xl_AddIn() {}		// Calls COleDispatchDriver default constructor
	Xl_AddIn(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_AddIn(const Xl_AddIn& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetFullName();
	BOOL GetInstalled();
	void SetInstalled(BOOL bNewValue);
	CString GetName();
	CString GetPath();
};
/////////////////////////////////////////////////////////////////////////////
// AddIns wrapper class

class __MY_EXT_CLASS__ Xl_AddIns : public COleDispatchDriver
{
public:
	Xl_AddIns() {}		// Calls COleDispatchDriver default constructor
	Xl_AddIns(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_AddIns(const Xl_AddIns& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(LPCTSTR Filename, const VARIANT& CopyFile);
	long GetCount();
	LPDISPATCH GetItem(const VARIANT& Index);
	LPUNKNOWN Get_NewEnum();
	LPDISPATCH Get_Default(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// Toolbar wrapper class

class __MY_EXT_CLASS__ Xl_Toolbar : public COleDispatchDriver
{
public:
	Xl_Toolbar() {}		// Calls COleDispatchDriver default constructor
	Xl_Toolbar(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Toolbar(const Xl_Toolbar& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	BOOL GetBuiltIn();
	void Delete();
	long GetHeight();
	void SetHeight(long nNewValue);
	long GetLeft();
	void SetLeft(long nNewValue);
	CString GetName();
	long GetPosition();
	void SetPosition(long nNewValue);
	long GetProtection();
	void SetProtection(long nNewValue);
	void Reset();
	LPDISPATCH GetToolbarButtons();
	long GetTop();
	void SetTop(long nNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	long GetWidth();
	void SetWidth(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Toolbars wrapper class

class __MY_EXT_CLASS__ Xl_Toolbars : public COleDispatchDriver
{
public:
	Xl_Toolbars() {}		// Calls COleDispatchDriver default constructor
	Xl_Toolbars(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Toolbars(const Xl_Toolbars& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(const VARIANT& Name);
	long GetCount();
	LPDISPATCH Get_Default(const VARIANT& Index);
	LPDISPATCH GetItem(const VARIANT& Index);
	LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// ToolbarButton wrapper class

class __MY_EXT_CLASS__ Xl_ToolbarButton : public COleDispatchDriver
{
public:
	Xl_ToolbarButton() {}		// Calls COleDispatchDriver default constructor
	Xl_ToolbarButton(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ToolbarButton(const Xl_ToolbarButton& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	BOOL GetBuiltIn();
	BOOL GetBuiltInFace();
	void SetBuiltInFace(BOOL bNewValue);
	void Copy(LPDISPATCH Toolbar, long Before);
	void CopyFace();
	void Delete();
	void Edit();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetHelpContextID();
	void SetHelpContextID(long nNewValue);
	CString GetHelpFile();
	void SetHelpFile(LPCTSTR lpszNewValue);
	long GetId();
	BOOL GetIsGap();
	void Move(LPDISPATCH Toolbar, long Before);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	CString GetOnAction();
	void SetOnAction(LPCTSTR lpszNewValue);
	void PasteFace();
	BOOL GetPushed();
	void SetPushed(BOOL bNewValue);
	void Reset();
	CString GetStatusBar();
	void SetStatusBar(LPCTSTR lpszNewValue);
	long GetWidth();
	void SetWidth(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ToolbarButtons wrapper class

class __MY_EXT_CLASS__ Xl_ToolbarButtons : public COleDispatchDriver
{
public:
	Xl_ToolbarButtons() {}		// Calls COleDispatchDriver default constructor
	Xl_ToolbarButtons(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ToolbarButtons(const Xl_ToolbarButtons& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(const VARIANT& Button, const VARIANT& Before, const VARIANT& OnAction, const VARIANT& Pushed, const VARIANT& Enabled, const VARIANT& StatusBar, const VARIANT& HelpFile, const VARIANT& HelpContextID);
	long GetCount();
	LPDISPATCH GetItem(long Index);
	LPUNKNOWN Get_NewEnum();
	LPDISPATCH Get_Default(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// Areas wrapper class

class __MY_EXT_CLASS__ Xl_Areas : public COleDispatchDriver
{
public:
	Xl_Areas() {}		// Calls COleDispatchDriver default constructor
	Xl_Areas(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Areas(const Xl_Areas& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH GetItem(long Index);
	LPUNKNOWN Get_NewEnum();
	LPDISPATCH Get_Default(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// WorkbookEvents wrapper class

class __MY_EXT_CLASS__ Xl_WorkbookEvents : public COleDispatchDriver
{
public:
	Xl_WorkbookEvents() {}		// Calls COleDispatchDriver default constructor
	Xl_WorkbookEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_WorkbookEvents(const Xl_WorkbookEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Open();
	void Activate();
	void Deactivate();
	void BeforeClose(BOOL* Cancel);
	void BeforeSave(BOOL SaveAsUI, BOOL* Cancel);
	void BeforePrint(BOOL* Cancel);
	void NewSheet(LPDISPATCH Sh);
	void AddinInstall();
	void AddinUninstall();
	void WindowResize(LPDISPATCH Wn);
	void WindowActivate(LPDISPATCH Wn);
	void WindowDeactivate(LPDISPATCH Wn);
	void SheetSelectionChange(LPDISPATCH Sh, LPDISPATCH Target);
	void SheetBeforeDoubleClick(LPDISPATCH Sh, LPDISPATCH Target, BOOL* Cancel);
	void SheetBeforeRightClick(LPDISPATCH Sh, LPDISPATCH Target, BOOL* Cancel);
	void SheetActivate(LPDISPATCH Sh);
	void SheetDeactivate(LPDISPATCH Sh);
	void SheetCalculate(LPDISPATCH Sh);
	void SheetChange(LPDISPATCH Sh, LPDISPATCH Target);
	void SheetFollowHyperlink(LPDISPATCH Sh, LPDISPATCH Target);
};
/////////////////////////////////////////////////////////////////////////////
// MenuBars wrapper class

class __MY_EXT_CLASS__ Xl_MenuBars : public COleDispatchDriver
{
public:
	Xl_MenuBars() {}		// Calls COleDispatchDriver default constructor
	Xl_MenuBars(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_MenuBars(const Xl_MenuBars& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(const VARIANT& Name);
	long GetCount();
	LPDISPATCH Get_Default(const VARIANT& Index);
	LPDISPATCH GetItem(const VARIANT& Index);
	LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// MenuBar wrapper class

class __MY_EXT_CLASS__ Xl_MenuBar : public COleDispatchDriver
{
public:
	Xl_MenuBar() {}		// Calls COleDispatchDriver default constructor
	Xl_MenuBar(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_MenuBar(const Xl_MenuBar& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void Activate();
	BOOL GetBuiltIn();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	void Delete();
	long GetIndex();
	LPDISPATCH GetMenus();
	void Reset();
};
/////////////////////////////////////////////////////////////////////////////
// Menus wrapper class

class __MY_EXT_CLASS__ Xl_Menus : public COleDispatchDriver
{
public:
	Xl_Menus() {}		// Calls COleDispatchDriver default constructor
	Xl_Menus(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Menus(const Xl_Menus& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(LPCTSTR Caption, const VARIANT& Before, const VARIANT& Restore);
	long GetCount();
	LPDISPATCH Get_Default(const VARIANT& Index);
	LPDISPATCH GetItem(const VARIANT& Index);
	LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Menu wrapper class

class __MY_EXT_CLASS__ Xl_Menu : public COleDispatchDriver
{
public:
	Xl_Menu() {}		// Calls COleDispatchDriver default constructor
	Xl_Menu(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Menu(const Xl_Menu& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	void Delete();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetIndex();
	LPDISPATCH GetMenuItems();
};
/////////////////////////////////////////////////////////////////////////////
// MenuItems wrapper class

class __MY_EXT_CLASS__ Xl_MenuItems : public COleDispatchDriver
{
public:
	Xl_MenuItems() {}		// Calls COleDispatchDriver default constructor
	Xl_MenuItems(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_MenuItems(const Xl_MenuItems& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(LPCTSTR Caption, const VARIANT& OnAction, const VARIANT& ShortcutKey, const VARIANT& Before, const VARIANT& Restore, const VARIANT& StatusBar, const VARIANT& HelpFile, const VARIANT& HelpContextID);
	LPDISPATCH AddMenu(LPCTSTR Caption, const VARIANT& Before, const VARIANT& Restore);
	long GetCount();
	LPDISPATCH Get_Default(const VARIANT& Index);
	LPDISPATCH GetItem(const VARIANT& Index);
	LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// MenuItem wrapper class

class __MY_EXT_CLASS__ Xl_MenuItem : public COleDispatchDriver
{
public:
	Xl_MenuItem() {}		// Calls COleDispatchDriver default constructor
	Xl_MenuItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_MenuItem(const Xl_MenuItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	BOOL GetChecked();
	void SetChecked(BOOL bNewValue);
	void Delete();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetHelpContextID();
	void SetHelpContextID(long nNewValue);
	CString GetHelpFile();
	void SetHelpFile(LPCTSTR lpszNewValue);
	long GetIndex();
	CString GetOnAction();
	void SetOnAction(LPCTSTR lpszNewValue);
	CString GetStatusBar();
	void SetStatusBar(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Charts wrapper class

class __MY_EXT_CLASS__ Xl_Charts : public COleDispatchDriver
{
public:
	Xl_Charts() {}		// Calls COleDispatchDriver default constructor
	Xl_Charts(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Charts(const Xl_Charts& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(const VARIANT& Before, const VARIANT& After, const VARIANT& Count);
	void Copy(const VARIANT& Before, const VARIANT& After);
	long GetCount();
	void Delete();
	LPDISPATCH GetItem(const VARIANT& Index);
	void Move(const VARIANT& Before, const VARIANT& After);
	LPUNKNOWN Get_NewEnum();
	void PrintPreview(const VARIANT& EnableChanges);
	void Select(const VARIANT& Replace);
	LPDISPATCH GetHPageBreaks();
	LPDISPATCH GetVPageBreaks();
	VARIANT GetVisible();
	void SetVisible(const VARIANT& newValue);
	LPDISPATCH Get_Default(const VARIANT& Index);
	void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
};
/////////////////////////////////////////////////////////////////////////////
// DrawingObjects wrapper class

class __MY_EXT_CLASS__ Xl_DrawingObjects : public COleDispatchDriver
{
public:
	Xl_DrawingObjects() {}		// Calls COleDispatchDriver default constructor
	Xl_DrawingObjects(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_DrawingObjects(const Xl_DrawingObjects& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	VARIANT GetAccelerator();
	void SetAccelerator(const VARIANT& newValue);
	BOOL GetAddIndent();
	void SetAddIndent(BOOL bNewValue);
	VARIANT AddItem(const VARIANT& Text, const VARIANT& Index);
	VARIANT GetArrowHeadLength();
	void SetArrowHeadLength(const VARIANT& newValue);
	VARIANT GetArrowHeadStyle();
	void SetArrowHeadStyle(const VARIANT& newValue);
	VARIANT GetArrowHeadWidth();
	void SetArrowHeadWidth(const VARIANT& newValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	LPDISPATCH GetBorder();
	BOOL GetCancelButton();
	void SetCancelButton(BOOL bNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	long Get_Default();
	void Set_Default(long nNewValue);
	BOOL GetDefaultButton();
	void SetDefaultButton(BOOL bNewValue);
	BOOL GetDismissButton();
	void SetDismissButton(BOOL bNewValue);
	BOOL GetDisplay3DShading();
	void SetDisplay3DShading(BOOL bNewValue);
	BOOL GetDisplayVerticalScrollBar();
	void SetDisplayVerticalScrollBar(BOOL bNewValue);
	long GetDropDownLines();
	void SetDropDownLines(long nNewValue);
	LPDISPATCH GetFont();
	BOOL GetHelpButton();
	void SetHelpButton(BOOL bNewValue);
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	long GetInputType();
	void SetInputType(long nNewValue);
	LPDISPATCH GetInterior();
	long GetLargeChange();
	void SetLargeChange(long nNewValue);
	CString GetLinkedCell();
	void SetLinkedCell(LPCTSTR lpszNewValue);
	VARIANT List(const VARIANT& Index);
	CString GetListFillRange();
	void SetListFillRange(LPCTSTR lpszNewValue);
	long GetListIndex();
	void SetListIndex(long nNewValue);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	long GetMax();
	void SetMax(long nNewValue);
	long GetMin();
	void SetMin(long nNewValue);
	BOOL GetMultiLine();
	void SetMultiLine(BOOL bNewValue);
	BOOL GetMultiSelect();
	void SetMultiSelect(BOOL bNewValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	VARIANT GetPhoneticAccelerator();
	void SetPhoneticAccelerator(const VARIANT& newValue);
	VARIANT RemoveAllItems();
	VARIANT RemoveItem(long Index, const VARIANT& Count);
	VARIANT Reshape(long Vertex, const VARIANT& Insert, const VARIANT& Left, const VARIANT& Top);
	BOOL GetRoundedCorners();
	void SetRoundedCorners(BOOL bNewValue);
	VARIANT Selected(const VARIANT& Index);
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	long GetSmallChange();
	void SetSmallChange(long nNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	LPDISPATCH Ungroup();
	long GetValue();
	void SetValue(long nNewValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	VARIANT Vertices(const VARIANT& Index1, const VARIANT& Index2);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Group();
	VARIANT LinkCombo(const VARIANT& Link);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// PivotCache wrapper class

class __MY_EXT_CLASS__ Xl_PivotCache : public COleDispatchDriver
{
public:
	Xl_PivotCache() {}		// Calls COleDispatchDriver default constructor
	Xl_PivotCache(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_PivotCache(const Xl_PivotCache& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	BOOL GetBackgroundQuery();
	void SetBackgroundQuery(BOOL bNewValue);
	VARIANT GetConnection();
	void SetConnection(const VARIANT& newValue);
	BOOL GetEnableRefresh();
	void SetEnableRefresh(BOOL bNewValue);
	long GetIndex();
	long GetMemoryUsed();
	BOOL GetOptimizeCache();
	void SetOptimizeCache(BOOL bNewValue);
	long GetRecordCount();
	void Refresh();
	DATE GetRefreshDate();
	CString GetRefreshName();
	BOOL GetRefreshOnFileOpen();
	void SetRefreshOnFileOpen(BOOL bNewValue);
	BOOL GetSavePassword();
	void SetSavePassword(BOOL bNewValue);
	VARIANT GetSourceData();
	void SetSourceData(const VARIANT& newValue);
	VARIANT GetCommandText();
	void SetCommandText(const VARIANT& newValue);
	long GetCommandType();
	void SetCommandType(long nNewValue);
	long GetQueryType();
	BOOL GetMaintainConnection();
	void SetMaintainConnection(BOOL bNewValue);
	long GetRefreshPeriod();
	void SetRefreshPeriod(long nNewValue);
	LPDISPATCH GetRecordset();
	void SetRefRecordset(LPDISPATCH newValue);
	void ResetTimer();
	VARIANT GetLocalConnection();
	void SetLocalConnection(const VARIANT& newValue);
	LPDISPATCH CreatePivotTable(const VARIANT& TableDestination, const VARIANT& TableName, const VARIANT& ReadData);
	BOOL GetUseLocalConnection();
	void SetUseLocalConnection(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// PivotCaches wrapper class

class __MY_EXT_CLASS__ Xl_PivotCaches : public COleDispatchDriver
{
public:
	Xl_PivotCaches() {}		// Calls COleDispatchDriver default constructor
	Xl_PivotCaches(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_PivotCaches(const Xl_PivotCaches& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Get_Default(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
	LPDISPATCH Add(long SourceType, const VARIANT& SourceData);
};
/////////////////////////////////////////////////////////////////////////////
// PivotFormula wrapper class

class __MY_EXT_CLASS__ Xl_PivotFormula : public COleDispatchDriver
{
public:
	Xl_PivotFormula() {}		// Calls COleDispatchDriver default constructor
	Xl_PivotFormula(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_PivotFormula(const Xl_PivotFormula& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void Delete();
	CString Get_Default();
	void Set_Default(LPCTSTR lpszNewValue);
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	CString GetValue();
	void SetValue(LPCTSTR lpszNewValue);
	long GetIndex();
	void SetIndex(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// PivotFormulas wrapper class

class __MY_EXT_CLASS__ Xl_PivotFormulas : public COleDispatchDriver
{
public:
	Xl_PivotFormulas() {}		// Calls COleDispatchDriver default constructor
	Xl_PivotFormulas(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_PivotFormulas(const Xl_PivotFormulas& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Add(LPCTSTR Formula);
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Get_Default(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// PivotTable wrapper class

class __MY_EXT_CLASS__ Xl_PivotTable : public COleDispatchDriver
{
public:
	Xl_PivotTable() {}		// Calls COleDispatchDriver default constructor
	Xl_PivotTable(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_PivotTable(const Xl_PivotTable& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT AddFields(const VARIANT& RowFields, const VARIANT& ColumnFields, const VARIANT& PageFields, const VARIANT& AddToTable);
	LPDISPATCH GetColumnFields(const VARIANT& Index);
	BOOL GetColumnGrand();
	void SetColumnGrand(BOOL bNewValue);
	LPDISPATCH GetColumnRange();
	VARIANT ShowPages(const VARIANT& PageField);
	LPDISPATCH GetDataBodyRange();
	LPDISPATCH GetDataFields(const VARIANT& Index);
	LPDISPATCH GetDataLabelRange();
	CString Get_Default();
	void Set_Default(LPCTSTR lpszNewValue);
	BOOL GetHasAutoFormat();
	void SetHasAutoFormat(BOOL bNewValue);
	LPDISPATCH GetHiddenFields(const VARIANT& Index);
	CString GetInnerDetail();
	void SetInnerDetail(LPCTSTR lpszNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	LPDISPATCH GetPageFields(const VARIANT& Index);
	LPDISPATCH GetPageRange();
	LPDISPATCH GetPageRangeCells();
	LPDISPATCH PivotFields(const VARIANT& Index);
	DATE GetRefreshDate();
	CString GetRefreshName();
	BOOL RefreshTable();
	LPDISPATCH GetRowFields(const VARIANT& Index);
	BOOL GetRowGrand();
	void SetRowGrand(BOOL bNewValue);
	LPDISPATCH GetRowRange();
	BOOL GetSaveData();
	void SetSaveData(BOOL bNewValue);
	VARIANT GetSourceData();
	void SetSourceData(const VARIANT& newValue);
	LPDISPATCH GetTableRange1();
	LPDISPATCH GetTableRange2();
	CString GetValue();
	void SetValue(LPCTSTR lpszNewValue);
	LPDISPATCH GetVisibleFields(const VARIANT& Index);
	long GetCacheIndex();
	void SetCacheIndex(long nNewValue);
	LPDISPATCH CalculatedFields();
	BOOL GetDisplayErrorString();
	void SetDisplayErrorString(BOOL bNewValue);
	BOOL GetDisplayNullString();
	void SetDisplayNullString(BOOL bNewValue);
	BOOL GetEnableDrilldown();
	void SetEnableDrilldown(BOOL bNewValue);
	BOOL GetEnableFieldDialog();
	void SetEnableFieldDialog(BOOL bNewValue);
	BOOL GetEnableWizard();
	void SetEnableWizard(BOOL bNewValue);
	CString GetErrorString();
	void SetErrorString(LPCTSTR lpszNewValue);
	double GetData(LPCTSTR Name);
	void ListFormulas();
	BOOL GetManualUpdate();
	void SetManualUpdate(BOOL bNewValue);
	BOOL GetMergeLabels();
	void SetMergeLabels(BOOL bNewValue);
	CString GetNullString();
	void SetNullString(LPCTSTR lpszNewValue);
	LPDISPATCH PivotCache();
	LPDISPATCH GetPivotFormulas();
	void PivotTableWizard(const VARIANT& SourceType, const VARIANT& SourceData, const VARIANT& TableDestination, const VARIANT& TableName, const VARIANT& RowGrand, const VARIANT& ColumnGrand, const VARIANT& SaveData, const VARIANT& HasAutoFormat, 
		const VARIANT& AutoPage, const VARIANT& Reserved, const VARIANT& BackgroundQuery, const VARIANT& OptimizeCache, const VARIANT& PageFieldOrder, const VARIANT& PageFieldWrapCount, const VARIANT& ReadData, const VARIANT& Connection);
	BOOL GetSubtotalHiddenPageItems();
	void SetSubtotalHiddenPageItems(BOOL bNewValue);
	long GetPageFieldOrder();
	void SetPageFieldOrder(long nNewValue);
	CString GetPageFieldStyle();
	void SetPageFieldStyle(LPCTSTR lpszNewValue);
	long GetPageFieldWrapCount();
	void SetPageFieldWrapCount(long nNewValue);
	BOOL GetPreserveFormatting();
	void SetPreserveFormatting(BOOL bNewValue);
	void PivotSelect(LPCTSTR Name, long Mode);
	CString GetPivotSelection();
	void SetPivotSelection(LPCTSTR lpszNewValue);
	long GetSelectionMode();
	void SetSelectionMode(long nNewValue);
	CString GetTableStyle();
	void SetTableStyle(LPCTSTR lpszNewValue);
	CString GetTag();
	void SetTag(LPCTSTR lpszNewValue);
	void Update();
	CString GetVacatedStyle();
	void SetVacatedStyle(LPCTSTR lpszNewValue);
	void Format(long Format);
	BOOL GetPrintTitles();
	void SetPrintTitles(BOOL bNewValue);
	LPDISPATCH GetCubeFields();
	CString GetGrandTotalName();
	void SetGrandTotalName(LPCTSTR lpszNewValue);
	BOOL GetSmallGrid();
	void SetSmallGrid(BOOL bNewValue);
	BOOL GetRepeatItemsOnEachPrintedPage();
	void SetRepeatItemsOnEachPrintedPage(BOOL bNewValue);
	BOOL GetTotalsAnnotation();
	void SetTotalsAnnotation(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// PivotTables wrapper class

class __MY_EXT_CLASS__ Xl_PivotTables : public COleDispatchDriver
{
public:
	Xl_PivotTables() {}		// Calls COleDispatchDriver default constructor
	Xl_PivotTables(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_PivotTables(const Xl_PivotTables& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
	LPDISPATCH Add(LPDISPATCH PivotCache, const VARIANT& TableDestination, const VARIANT& TableName, const VARIANT& ReadData);
};
/////////////////////////////////////////////////////////////////////////////
// PivotField wrapper class

class __MY_EXT_CLASS__ Xl_PivotField : public COleDispatchDriver
{
public:
	Xl_PivotField() {}		// Calls COleDispatchDriver default constructor
	Xl_PivotField(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_PivotField(const Xl_PivotField& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCalculation();
	void SetCalculation(long nNewValue);
	LPDISPATCH GetChildField();
	VARIANT GetChildItems(const VARIANT& Index);
	VARIANT GetCurrentPage();
	void SetCurrentPage(const VARIANT& newValue);
	LPDISPATCH GetDataRange();
	long GetDataType();
	CString Get_Default();
	void Set_Default(LPCTSTR lpszNewValue);
	long GetFunction();
	void SetFunction(long nNewValue);
	VARIANT GetGroupLevel();
	VARIANT GetHiddenItems(const VARIANT& Index);
	LPDISPATCH GetLabelRange();
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	CString GetNumberFormat();
	void SetNumberFormat(LPCTSTR lpszNewValue);
	long GetOrientation();
	void SetOrientation(long nNewValue);
	BOOL GetShowAllItems();
	void SetShowAllItems(BOOL bNewValue);
	LPDISPATCH GetParentField();
	VARIANT GetParentItems(const VARIANT& Index);
	VARIANT PivotItems(const VARIANT& Index);
	VARIANT GetPosition();
	void SetPosition(const VARIANT& newValue);
	CString GetSourceName();
	VARIANT GetSubtotals(const VARIANT& Index);
	void SetSubtotals(const VARIANT& Index, const VARIANT& newValue);
	VARIANT GetBaseField();
	void SetBaseField(const VARIANT& newValue);
	VARIANT GetBaseItem();
	void SetBaseItem(const VARIANT& newValue);
	VARIANT GetTotalLevels();
	CString GetValue();
	void SetValue(LPCTSTR lpszNewValue);
	VARIANT GetVisibleItems(const VARIANT& Index);
	LPDISPATCH CalculatedItems();
	void Delete();
	BOOL GetDragToColumn();
	void SetDragToColumn(BOOL bNewValue);
	BOOL GetDragToHide();
	void SetDragToHide(BOOL bNewValue);
	BOOL GetDragToPage();
	void SetDragToPage(BOOL bNewValue);
	BOOL GetDragToRow();
	void SetDragToRow(BOOL bNewValue);
	BOOL GetDragToData();
	void SetDragToData(BOOL bNewValue);
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	BOOL GetIsCalculated();
	long GetMemoryUsed();
	BOOL GetServerBased();
	void SetServerBased(BOOL bNewValue);
	void AutoSort(long Order, LPCTSTR Field);
	void AutoShow(long Type, long Range, long Count, LPCTSTR Field);
	long GetAutoSortOrder();
	CString GetAutoSortField();
	long GetAutoShowType();
	long GetAutoShowRange();
	long GetAutoShowCount();
	CString GetAutoShowField();
	BOOL GetLayoutBlankLine();
	void SetLayoutBlankLine(BOOL bNewValue);
	long GetLayoutSubtotalLocation();
	void SetLayoutSubtotalLocation(long nNewValue);
	BOOL GetLayoutPageBreak();
	void SetLayoutPageBreak(BOOL bNewValue);
	long GetLayoutForm();
	void SetLayoutForm(long nNewValue);
	CString GetSubtotalName();
	void SetSubtotalName(LPCTSTR lpszNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	BOOL GetDrilledDown();
	void SetDrilledDown(BOOL bNewValue);
	LPDISPATCH GetCubeField();
	CString GetCurrentPageName();
	void SetCurrentPageName(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// PivotFields wrapper class

class __MY_EXT_CLASS__ Xl_PivotFields : public COleDispatchDriver
{
public:
	Xl_PivotFields() {}		// Calls COleDispatchDriver default constructor
	Xl_PivotFields(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_PivotFields(const Xl_PivotFields& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// CalculatedFields wrapper class

class __MY_EXT_CLASS__ Xl_CalculatedFields : public COleDispatchDriver
{
public:
	Xl_CalculatedFields() {}		// Calls COleDispatchDriver default constructor
	Xl_CalculatedFields(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_CalculatedFields(const Xl_CalculatedFields& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Add(LPCTSTR Name, LPCTSTR Formula);
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Get_Default(const VARIANT& Field);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// PivotItem wrapper class

class __MY_EXT_CLASS__ Xl_PivotItem : public COleDispatchDriver
{
public:
	Xl_PivotItem() {}		// Calls COleDispatchDriver default constructor
	Xl_PivotItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_PivotItem(const Xl_PivotItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT GetChildItems(const VARIANT& Index);
	LPDISPATCH GetDataRange();
	CString Get_Default();
	void Set_Default(LPCTSTR lpszNewValue);
	LPDISPATCH GetLabelRange();
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	LPDISPATCH GetParentItem();
	BOOL GetParentShowDetail();
	long GetPosition();
	void SetPosition(long nNewValue);
	BOOL GetShowDetail();
	void SetShowDetail(BOOL bNewValue);
	VARIANT GetSourceName();
	CString GetValue();
	void SetValue(LPCTSTR lpszNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	void Delete();
	BOOL GetIsCalculated();
	long GetRecordCount();
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	BOOL GetDrilledDown();
	void SetDrilledDown(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// PivotItems wrapper class

class __MY_EXT_CLASS__ Xl_PivotItems : public COleDispatchDriver
{
public:
	Xl_PivotItems() {}		// Calls COleDispatchDriver default constructor
	Xl_PivotItems(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_PivotItems(const Xl_PivotItems& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void Add(LPCTSTR Name);
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// CalculatedItems wrapper class

class __MY_EXT_CLASS__ Xl_CalculatedItems : public COleDispatchDriver
{
public:
	Xl_CalculatedItems() {}		// Calls COleDispatchDriver default constructor
	Xl_CalculatedItems(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_CalculatedItems(const Xl_CalculatedItems& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Add(LPCTSTR Name, LPCTSTR Formula);
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Get_Default(const VARIANT& Field);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Characters wrapper class

class __MY_EXT_CLASS__ Xl_Characters : public COleDispatchDriver
{
public:
	Xl_Characters() {}		// Calls COleDispatchDriver default constructor
	Xl_Characters(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Characters(const Xl_Characters& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	long GetCount();
	VARIANT Delete();
	LPDISPATCH GetFont();
	VARIANT Insert(LPCTSTR String);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	CString GetPhoneticCharacters();
	void SetPhoneticCharacters(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Dialogs wrapper class

class __MY_EXT_CLASS__ Xl_Dialogs : public COleDispatchDriver
{
public:
	Xl_Dialogs() {}		// Calls COleDispatchDriver default constructor
	Xl_Dialogs(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Dialogs(const Xl_Dialogs& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH GetItem(long Index);
	LPDISPATCH Get_Default(long Index);
	LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Dialog wrapper class

class __MY_EXT_CLASS__ Xl_Dialog : public COleDispatchDriver
{
public:
	Xl_Dialog() {}		// Calls COleDispatchDriver default constructor
	Xl_Dialog(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Dialog(const Xl_Dialog& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	BOOL Show(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, const VARIANT& Arg11, 
		const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, const VARIANT& Arg21, 
		const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
};
/////////////////////////////////////////////////////////////////////////////
// SoundNote wrapper class

class __MY_EXT_CLASS__ Xl_SoundNote : public COleDispatchDriver
{
public:
	Xl_SoundNote() {}		// Calls COleDispatchDriver default constructor
	Xl_SoundNote(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_SoundNote(const Xl_SoundNote& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT Delete();
	VARIANT Import(LPCTSTR Filename);
	VARIANT Play();
	VARIANT Record();
};
/////////////////////////////////////////////////////////////////////////////
// Button wrapper class

class __MY_EXT_CLASS__ Xl_Button : public COleDispatchDriver
{
public:
	Xl_Button() {}		// Calls COleDispatchDriver default constructor
	Xl_Button(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Button(const Xl_Button& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	BOOL GetAddIndent();
	void SetAddIndent(BOOL bNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	LPDISPATCH GetFont();
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	VARIANT GetAccelerator();
	void SetAccelerator(const VARIANT& newValue);
	BOOL GetCancelButton();
	void SetCancelButton(BOOL bNewValue);
	BOOL GetDefaultButton();
	void SetDefaultButton(BOOL bNewValue);
	BOOL GetDismissButton();
	void SetDismissButton(BOOL bNewValue);
	BOOL GetHelpButton();
	void SetHelpButton(BOOL bNewValue);
	VARIANT GetPhoneticAccelerator();
	void SetPhoneticAccelerator(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// Buttons wrapper class

class __MY_EXT_CLASS__ Xl_Buttons : public COleDispatchDriver
{
public:
	Xl_Buttons() {}		// Calls COleDispatchDriver default constructor
	Xl_Buttons(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Buttons(const Xl_Buttons& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	BOOL GetAddIndent();
	void SetAddIndent(BOOL bNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	LPDISPATCH GetFont();
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	VARIANT GetAccelerator();
	void SetAccelerator(const VARIANT& newValue);
	BOOL GetCancelButton();
	void SetCancelButton(BOOL bNewValue);
	BOOL GetDefaultButton();
	void SetDefaultButton(BOOL bNewValue);
	BOOL GetDismissButton();
	void SetDismissButton(BOOL bNewValue);
	BOOL GetHelpButton();
	void SetHelpButton(BOOL bNewValue);
	VARIANT GetPhoneticAccelerator();
	void SetPhoneticAccelerator(const VARIANT& newValue);
	LPDISPATCH Add(double Left, double Top, double Width, double Height);
	long GetCount();
	LPDISPATCH Group();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// CheckBox wrapper class

class __MY_EXT_CLASS__ Xl_CheckBox : public COleDispatchDriver
{
public:
	Xl_CheckBox() {}		// Calls COleDispatchDriver default constructor
	Xl_CheckBox(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_CheckBox(const Xl_CheckBox& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetAccelerator();
	void SetAccelerator(const VARIANT& newValue);
	LPDISPATCH GetBorder();
	long Get_Default();
	void Set_Default(long nNewValue);
	BOOL GetDisplay3DShading();
	void SetDisplay3DShading(BOOL bNewValue);
	LPDISPATCH GetInterior();
	CString GetLinkedCell();
	void SetLinkedCell(LPCTSTR lpszNewValue);
	VARIANT GetPhoneticAccelerator();
	void SetPhoneticAccelerator(const VARIANT& newValue);
	VARIANT GetValue();
	void SetValue(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// CheckBoxes wrapper class

class __MY_EXT_CLASS__ Xl_CheckBoxes : public COleDispatchDriver
{
public:
	Xl_CheckBoxes() {}		// Calls COleDispatchDriver default constructor
	Xl_CheckBoxes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_CheckBoxes(const Xl_CheckBoxes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetAccelerator();
	void SetAccelerator(const VARIANT& newValue);
	LPDISPATCH GetBorder();
	long Get_Default();
	void Set_Default(long nNewValue);
	BOOL GetDisplay3DShading();
	void SetDisplay3DShading(BOOL bNewValue);
	LPDISPATCH GetInterior();
	CString GetLinkedCell();
	void SetLinkedCell(LPCTSTR lpszNewValue);
	VARIANT GetPhoneticAccelerator();
	void SetPhoneticAccelerator(const VARIANT& newValue);
	VARIANT GetValue();
	void SetValue(const VARIANT& newValue);
	LPDISPATCH Add(double Left, double Top, double Width, double Height);
	long GetCount();
	LPDISPATCH Group();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// OptionButton wrapper class

class __MY_EXT_CLASS__ Xl_OptionButton : public COleDispatchDriver
{
public:
	Xl_OptionButton() {}		// Calls COleDispatchDriver default constructor
	Xl_OptionButton(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_OptionButton(const Xl_OptionButton& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetAccelerator();
	void SetAccelerator(const VARIANT& newValue);
	LPDISPATCH GetBorder();
	long Get_Default();
	void Set_Default(long nNewValue);
	BOOL GetDisplay3DShading();
	void SetDisplay3DShading(BOOL bNewValue);
	LPDISPATCH GetInterior();
	CString GetLinkedCell();
	void SetLinkedCell(LPCTSTR lpszNewValue);
	VARIANT GetPhoneticAccelerator();
	void SetPhoneticAccelerator(const VARIANT& newValue);
	VARIANT GetValue();
	void SetValue(const VARIANT& newValue);
	LPDISPATCH GetGroupBox();
};
/////////////////////////////////////////////////////////////////////////////
// OptionButtons wrapper class

class __MY_EXT_CLASS__ Xl_OptionButtons : public COleDispatchDriver
{
public:
	Xl_OptionButtons() {}		// Calls COleDispatchDriver default constructor
	Xl_OptionButtons(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_OptionButtons(const Xl_OptionButtons& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetAccelerator();
	void SetAccelerator(const VARIANT& newValue);
	LPDISPATCH GetBorder();
	long Get_Default();
	void Set_Default(long nNewValue);
	BOOL GetDisplay3DShading();
	void SetDisplay3DShading(BOOL bNewValue);
	LPDISPATCH GetInterior();
	CString GetLinkedCell();
	void SetLinkedCell(LPCTSTR lpszNewValue);
	VARIANT GetPhoneticAccelerator();
	void SetPhoneticAccelerator(const VARIANT& newValue);
	VARIANT GetValue();
	void SetValue(const VARIANT& newValue);
	LPDISPATCH GetGroupBox();
	LPDISPATCH Add(double Left, double Top, double Width, double Height);
	long GetCount();
	LPDISPATCH Group();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// EditBox wrapper class

class __MY_EXT_CLASS__ Xl_EditBox : public COleDispatchDriver
{
public:
	Xl_EditBox() {}		// Calls COleDispatchDriver default constructor
	Xl_EditBox(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_EditBox(const Xl_EditBox& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	BOOL GetDisplayVerticalScrollBar();
	void SetDisplayVerticalScrollBar(BOOL bNewValue);
	long GetInputType();
	void SetInputType(long nNewValue);
	CString GetLinkedObject();
	BOOL GetMultiLine();
	void SetMultiLine(BOOL bNewValue);
	BOOL GetPasswordEdit();
	void SetPasswordEdit(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// EditBoxes wrapper class

class __MY_EXT_CLASS__ Xl_EditBoxes : public COleDispatchDriver
{
public:
	Xl_EditBoxes() {}		// Calls COleDispatchDriver default constructor
	Xl_EditBoxes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_EditBoxes(const Xl_EditBoxes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	BOOL GetDisplayVerticalScrollBar();
	void SetDisplayVerticalScrollBar(BOOL bNewValue);
	long GetInputType();
	void SetInputType(long nNewValue);
	BOOL GetMultiLine();
	void SetMultiLine(BOOL bNewValue);
	BOOL GetPasswordEdit();
	void SetPasswordEdit(BOOL bNewValue);
	LPDISPATCH Add(double Left, double Top, double Width, double Height);
	long GetCount();
	LPDISPATCH Group();
	VARIANT Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// ScrollBar wrapper class

class __MY_EXT_CLASS__ Xl_ScrollBar : public COleDispatchDriver
{
public:
	Xl_ScrollBar() {}		// Calls COleDispatchDriver default constructor
	Xl_ScrollBar(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ScrollBar(const Xl_ScrollBar& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	long Get_Default();
	void Set_Default(long nNewValue);
	BOOL GetDisplay3DShading();
	void SetDisplay3DShading(BOOL bNewValue);
	CString GetLinkedCell();
	void SetLinkedCell(LPCTSTR lpszNewValue);
	long GetMax();
	void SetMax(long nNewValue);
	long GetMin();
	void SetMin(long nNewValue);
	long GetSmallChange();
	void SetSmallChange(long nNewValue);
	long GetValue();
	void SetValue(long nNewValue);
	long GetLargeChange();
	void SetLargeChange(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ScrollBars wrapper class

class __MY_EXT_CLASS__ Xl_ScrollBars : public COleDispatchDriver
{
public:
	Xl_ScrollBars() {}		// Calls COleDispatchDriver default constructor
	Xl_ScrollBars(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ScrollBars(const Xl_ScrollBars& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	long Get_Default();
	void Set_Default(long nNewValue);
	BOOL GetDisplay3DShading();
	void SetDisplay3DShading(BOOL bNewValue);
	CString GetLinkedCell();
	void SetLinkedCell(LPCTSTR lpszNewValue);
	long GetMax();
	void SetMax(long nNewValue);
	long GetMin();
	void SetMin(long nNewValue);
	long GetSmallChange();
	void SetSmallChange(long nNewValue);
	long GetValue();
	void SetValue(long nNewValue);
	long GetLargeChange();
	void SetLargeChange(long nNewValue);
	LPDISPATCH Add(double Left, double Top, double Width, double Height);
	long GetCount();
	LPDISPATCH Group();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// ListBox wrapper class

class __MY_EXT_CLASS__ Xl_ListBox : public COleDispatchDriver
{
public:
	Xl_ListBox() {}		// Calls COleDispatchDriver default constructor
	Xl_ListBox(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ListBox(const Xl_ListBox& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	VARIANT AddItem(const VARIANT& Text, const VARIANT& Index);
	BOOL GetDisplay3DShading();
	void SetDisplay3DShading(BOOL bNewValue);
	long Get_Default();
	void Set_Default(long nNewValue);
	CString GetLinkedCell();
	void SetLinkedCell(LPCTSTR lpszNewValue);
	VARIANT GetLinkedObject();
	VARIANT GetList(const VARIANT& Index);
	void SetList(const VARIANT& Index, const VARIANT& newValue);
	long GetListCount();
	CString GetListFillRange();
	void SetListFillRange(LPCTSTR lpszNewValue);
	long GetListIndex();
	void SetListIndex(long nNewValue);
	long GetMultiSelect();
	void SetMultiSelect(long nNewValue);
	VARIANT RemoveAllItems();
	VARIANT RemoveItem(long Index, const VARIANT& Count);
	VARIANT GetSelected(const VARIANT& Index);
	void SetSelected(const VARIANT& Index, const VARIANT& newValue);
	long GetValue();
	void SetValue(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ListBoxes wrapper class

class __MY_EXT_CLASS__ Xl_ListBoxes : public COleDispatchDriver
{
public:
	Xl_ListBoxes() {}		// Calls COleDispatchDriver default constructor
	Xl_ListBoxes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ListBoxes(const Xl_ListBoxes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	VARIANT AddItem(const VARIANT& Text, const VARIANT& Index);
	BOOL GetDisplay3DShading();
	void SetDisplay3DShading(BOOL bNewValue);
	long Get_Default();
	void Set_Default(long nNewValue);
	CString GetLinkedCell();
	void SetLinkedCell(LPCTSTR lpszNewValue);
	VARIANT GetList(const VARIANT& Index);
	void SetList(const VARIANT& Index, const VARIANT& newValue);
	CString GetListFillRange();
	void SetListFillRange(LPCTSTR lpszNewValue);
	long GetListIndex();
	void SetListIndex(long nNewValue);
	long GetMultiSelect();
	void SetMultiSelect(long nNewValue);
	VARIANT RemoveAllItems();
	VARIANT RemoveItem(long Index, const VARIANT& Count);
	VARIANT GetSelected(const VARIANT& Index);
	void SetSelected(const VARIANT& Index, const VARIANT& newValue);
	long GetValue();
	void SetValue(long nNewValue);
	LPDISPATCH Add(double Left, double Top, double Width, double Height);
	long GetCount();
	LPDISPATCH Group();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// GroupBox wrapper class

class __MY_EXT_CLASS__ Xl_GroupBox : public COleDispatchDriver
{
public:
	Xl_GroupBox() {}		// Calls COleDispatchDriver default constructor
	Xl_GroupBox(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_GroupBox(const Xl_GroupBox& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetAccelerator();
	void SetAccelerator(const VARIANT& newValue);
	BOOL GetDisplay3DShading();
	void SetDisplay3DShading(BOOL bNewValue);
	VARIANT GetPhoneticAccelerator();
	void SetPhoneticAccelerator(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// GroupBoxes wrapper class

class __MY_EXT_CLASS__ Xl_GroupBoxes : public COleDispatchDriver
{
public:
	Xl_GroupBoxes() {}		// Calls COleDispatchDriver default constructor
	Xl_GroupBoxes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_GroupBoxes(const Xl_GroupBoxes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetAccelerator();
	void SetAccelerator(const VARIANT& newValue);
	BOOL GetDisplay3DShading();
	void SetDisplay3DShading(BOOL bNewValue);
	VARIANT GetPhoneticAccelerator();
	void SetPhoneticAccelerator(const VARIANT& newValue);
	LPDISPATCH Add(double Left, double Top, double Width, double Height);
	long GetCount();
	LPDISPATCH Group();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// DropDown wrapper class

class __MY_EXT_CLASS__ Xl_DropDown : public COleDispatchDriver
{
public:
	Xl_DropDown() {}		// Calls COleDispatchDriver default constructor
	Xl_DropDown(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_DropDown(const Xl_DropDown& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	VARIANT AddItem(const VARIANT& Text, const VARIANT& Index);
	BOOL GetDisplay3DShading();
	void SetDisplay3DShading(BOOL bNewValue);
	long Get_Default();
	void Set_Default(long nNewValue);
	CString GetLinkedCell();
	void SetLinkedCell(LPCTSTR lpszNewValue);
	VARIANT GetLinkedObject();
	VARIANT GetList(const VARIANT& Index);
	void SetList(const VARIANT& Index, const VARIANT& newValue);
	long GetListCount();
	CString GetListFillRange();
	void SetListFillRange(LPCTSTR lpszNewValue);
	long GetListIndex();
	void SetListIndex(long nNewValue);
	VARIANT RemoveAllItems();
	VARIANT RemoveItem(long Index, const VARIANT& Count);
	VARIANT GetSelected(const VARIANT& Index);
	void SetSelected(const VARIANT& Index, const VARIANT& newValue);
	long GetValue();
	void SetValue(long nNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	long GetDropDownLines();
	void SetDropDownLines(long nNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// DropDowns wrapper class

class __MY_EXT_CLASS__ Xl_DropDowns : public COleDispatchDriver
{
public:
	Xl_DropDowns() {}		// Calls COleDispatchDriver default constructor
	Xl_DropDowns(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_DropDowns(const Xl_DropDowns& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	VARIANT AddItem(const VARIANT& Text, const VARIANT& Index);
	BOOL GetDisplay3DShading();
	void SetDisplay3DShading(BOOL bNewValue);
	long Get_Default();
	void Set_Default(long nNewValue);
	CString GetLinkedCell();
	void SetLinkedCell(LPCTSTR lpszNewValue);
	VARIANT GetList(const VARIANT& Index);
	void SetList(const VARIANT& Index, const VARIANT& newValue);
	CString GetListFillRange();
	void SetListFillRange(LPCTSTR lpszNewValue);
	long GetListIndex();
	void SetListIndex(long nNewValue);
	VARIANT RemoveAllItems();
	VARIANT RemoveItem(long Index, const VARIANT& Count);
	VARIANT GetSelected(const VARIANT& Index);
	void SetSelected(const VARIANT& Index, const VARIANT& newValue);
	long GetValue();
	void SetValue(long nNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	long GetDropDownLines();
	void SetDropDownLines(long nNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	LPDISPATCH Add(double Left, double Top, double Width, double Height, const VARIANT& Editable);
	long GetCount();
	LPDISPATCH Group();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Spinner wrapper class

class __MY_EXT_CLASS__ Xl_Spinner : public COleDispatchDriver
{
public:
	Xl_Spinner() {}		// Calls COleDispatchDriver default constructor
	Xl_Spinner(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Spinner(const Xl_Spinner& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	long Get_Default();
	void Set_Default(long nNewValue);
	BOOL GetDisplay3DShading();
	void SetDisplay3DShading(BOOL bNewValue);
	CString GetLinkedCell();
	void SetLinkedCell(LPCTSTR lpszNewValue);
	long GetMax();
	void SetMax(long nNewValue);
	long GetMin();
	void SetMin(long nNewValue);
	long GetSmallChange();
	void SetSmallChange(long nNewValue);
	long GetValue();
	void SetValue(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Spinners wrapper class

class __MY_EXT_CLASS__ Xl_Spinners : public COleDispatchDriver
{
public:
	Xl_Spinners() {}		// Calls COleDispatchDriver default constructor
	Xl_Spinners(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Spinners(const Xl_Spinners& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	long Get_Default();
	void Set_Default(long nNewValue);
	BOOL GetDisplay3DShading();
	void SetDisplay3DShading(BOOL bNewValue);
	CString GetLinkedCell();
	void SetLinkedCell(LPCTSTR lpszNewValue);
	long GetMax();
	void SetMax(long nNewValue);
	long GetMin();
	void SetMin(long nNewValue);
	long GetSmallChange();
	void SetSmallChange(long nNewValue);
	long GetValue();
	void SetValue(long nNewValue);
	LPDISPATCH Add(double Left, double Top, double Width, double Height);
	long GetCount();
	LPDISPATCH Group();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// DialogFrame wrapper class

class __MY_EXT_CLASS__ Xl_DialogFrame : public COleDispatchDriver
{
public:
	Xl_DialogFrame() {}		// Calls COleDispatchDriver default constructor
	Xl_DialogFrame(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_DialogFrame(const Xl_DialogFrame& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT CopyPicture(long Appearance, long Format);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT Select(const VARIANT& Replace);
	double GetTop();
	void SetTop(double newValue);
	double GetWidth();
	void SetWidth(double newValue);
	LPDISPATCH GetShapeRange();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Label wrapper class

class __MY_EXT_CLASS__ Xl_Label : public COleDispatchDriver
{
public:
	Xl_Label() {}		// Calls COleDispatchDriver default constructor
	Xl_Label(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Label(const Xl_Label& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetAccelerator();
	void SetAccelerator(const VARIANT& newValue);
	VARIANT GetPhoneticAccelerator();
	void SetPhoneticAccelerator(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// Labels wrapper class

class __MY_EXT_CLASS__ Xl_Labels : public COleDispatchDriver
{
public:
	Xl_Labels() {}		// Calls COleDispatchDriver default constructor
	Xl_Labels(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Labels(const Xl_Labels& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetAccelerator();
	void SetAccelerator(const VARIANT& newValue);
	VARIANT GetPhoneticAccelerator();
	void SetPhoneticAccelerator(const VARIANT& newValue);
	LPDISPATCH Add(double Left, double Top, double Width, double Height);
	long GetCount();
	LPDISPATCH Group();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Panes wrapper class

class __MY_EXT_CLASS__ Xl_Panes : public COleDispatchDriver
{
public:
	Xl_Panes() {}		// Calls COleDispatchDriver default constructor
	Xl_Panes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Panes(const Xl_Panes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH GetItem(long Index);
	LPDISPATCH Get_Default(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// Pane wrapper class

class __MY_EXT_CLASS__ Xl_Pane : public COleDispatchDriver
{
public:
	Xl_Pane() {}		// Calls COleDispatchDriver default constructor
	Xl_Pane(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Pane(const Xl_Pane& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	BOOL Activate();
	long GetIndex();
	VARIANT LargeScroll(const VARIANT& Down, const VARIANT& Up, const VARIANT& ToRight, const VARIANT& ToLeft);
	long GetScrollColumn();
	void SetScrollColumn(long nNewValue);
	long GetScrollRow();
	void SetScrollRow(long nNewValue);
	VARIANT SmallScroll(const VARIANT& Down, const VARIANT& Up, const VARIANT& ToRight, const VARIANT& ToLeft);
	LPDISPATCH GetVisibleRange();
	void ScrollIntoView(long Left, long Top, long Width, long Height, const VARIANT& Start);
};
/////////////////////////////////////////////////////////////////////////////
// Scenarios wrapper class

class __MY_EXT_CLASS__ Xl_Scenarios : public COleDispatchDriver
{
public:
	Xl_Scenarios() {}		// Calls COleDispatchDriver default constructor
	Xl_Scenarios(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Scenarios(const Xl_Scenarios& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(LPCTSTR Name, const VARIANT& ChangingCells, const VARIANT& Values, const VARIANT& Comment, const VARIANT& Locked, const VARIANT& Hidden);
	long GetCount();
	VARIANT CreateSummary(long ReportType, const VARIANT& ResultCells);
	LPDISPATCH Item(const VARIANT& Index);
	VARIANT Merge(const VARIANT& Source);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Scenario wrapper class

class __MY_EXT_CLASS__ Xl_Scenario : public COleDispatchDriver
{
public:
	Xl_Scenario() {}		// Calls COleDispatchDriver default constructor
	Xl_Scenario(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Scenario(const Xl_Scenario& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT ChangeScenario(const VARIANT& ChangingCells, const VARIANT& Values);
	LPDISPATCH GetChangingCells();
	CString GetComment();
	void SetComment(LPCTSTR lpszNewValue);
	VARIANT Delete();
	BOOL GetHidden();
	void SetHidden(BOOL bNewValue);
	long GetIndex();
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT Show();
	VARIANT GetValues(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// GroupObject wrapper class

class __MY_EXT_CLASS__ Xl_GroupObject : public COleDispatchDriver
{
public:
	Xl_GroupObject() {}		// Calls COleDispatchDriver default constructor
	Xl_GroupObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_GroupObject(const Xl_GroupObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	BOOL GetAddIndent();
	void SetAddIndent(BOOL bNewValue);
	VARIANT GetArrowHeadLength();
	void SetArrowHeadLength(const VARIANT& newValue);
	VARIANT GetArrowHeadStyle();
	void SetArrowHeadStyle(const VARIANT& newValue);
	VARIANT GetArrowHeadWidth();
	void SetArrowHeadWidth(const VARIANT& newValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	LPDISPATCH GetBorder();
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	long Get_Default();
	void Set_Default(long nNewValue);
	LPDISPATCH GetFont();
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	LPDISPATCH GetInterior();
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	BOOL GetRoundedCorners();
	void SetRoundedCorners(BOOL bNewValue);
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	LPDISPATCH Ungroup();
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// GroupObjects wrapper class

class __MY_EXT_CLASS__ Xl_GroupObjects : public COleDispatchDriver
{
public:
	Xl_GroupObjects() {}		// Calls COleDispatchDriver default constructor
	Xl_GroupObjects(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_GroupObjects(const Xl_GroupObjects& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	BOOL GetAddIndent();
	void SetAddIndent(BOOL bNewValue);
	VARIANT GetArrowHeadLength();
	void SetArrowHeadLength(const VARIANT& newValue);
	VARIANT GetArrowHeadStyle();
	void SetArrowHeadStyle(const VARIANT& newValue);
	VARIANT GetArrowHeadWidth();
	void SetArrowHeadWidth(const VARIANT& newValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	LPDISPATCH GetBorder();
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	long Get_Default();
	void Set_Default(long nNewValue);
	LPDISPATCH GetFont();
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	LPDISPATCH GetInterior();
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	BOOL GetRoundedCorners();
	void SetRoundedCorners(BOOL bNewValue);
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	LPDISPATCH Ungroup();
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	long GetCount();
	LPDISPATCH Group();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Line wrapper class

class __MY_EXT_CLASS__ Xl_Line : public COleDispatchDriver
{
public:
	Xl_Line() {}		// Calls COleDispatchDriver default constructor
	Xl_Line(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Line(const Xl_Line& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	VARIANT GetArrowHeadLength();
	void SetArrowHeadLength(const VARIANT& newValue);
	VARIANT GetArrowHeadStyle();
	void SetArrowHeadStyle(const VARIANT& newValue);
	VARIANT GetArrowHeadWidth();
	void SetArrowHeadWidth(const VARIANT& newValue);
	LPDISPATCH GetBorder();
};
/////////////////////////////////////////////////////////////////////////////
// Lines wrapper class

class __MY_EXT_CLASS__ Xl_Lines : public COleDispatchDriver
{
public:
	Xl_Lines() {}		// Calls COleDispatchDriver default constructor
	Xl_Lines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Lines(const Xl_Lines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	VARIANT GetArrowHeadLength();
	void SetArrowHeadLength(const VARIANT& newValue);
	VARIANT GetArrowHeadStyle();
	void SetArrowHeadStyle(const VARIANT& newValue);
	VARIANT GetArrowHeadWidth();
	void SetArrowHeadWidth(const VARIANT& newValue);
	LPDISPATCH GetBorder();
	LPDISPATCH Add(double X1, double Y1, double X2, double Y2);
	long GetCount();
	LPDISPATCH Group();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Rectangle wrapper class

class __MY_EXT_CLASS__ Xl_Rectangle : public COleDispatchDriver
{
public:
	Xl_Rectangle() {}		// Calls COleDispatchDriver default constructor
	Xl_Rectangle(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Rectangle(const Xl_Rectangle& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	BOOL GetAddIndent();
	void SetAddIndent(BOOL bNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	LPDISPATCH GetFont();
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	LPDISPATCH GetBorder();
	LPDISPATCH GetInterior();
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	BOOL GetRoundedCorners();
	void SetRoundedCorners(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Rectangles wrapper class

class __MY_EXT_CLASS__ Xl_Rectangles : public COleDispatchDriver
{
public:
	Xl_Rectangles() {}		// Calls COleDispatchDriver default constructor
	Xl_Rectangles(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Rectangles(const Xl_Rectangles& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	BOOL GetAddIndent();
	void SetAddIndent(BOOL bNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	LPDISPATCH GetFont();
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	LPDISPATCH GetBorder();
	LPDISPATCH GetInterior();
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	BOOL GetRoundedCorners();
	void SetRoundedCorners(BOOL bNewValue);
	LPDISPATCH Add(double Left, double Top, double Width, double Height);
	long GetCount();
	LPDISPATCH Group();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Oval wrapper class

class __MY_EXT_CLASS__ Xl_Oval : public COleDispatchDriver
{
public:
	Xl_Oval() {}		// Calls COleDispatchDriver default constructor
	Xl_Oval(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Oval(const Xl_Oval& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	BOOL GetAddIndent();
	void SetAddIndent(BOOL bNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	LPDISPATCH GetFont();
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	LPDISPATCH GetBorder();
	LPDISPATCH GetInterior();
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Ovals wrapper class

class __MY_EXT_CLASS__ Xl_Ovals : public COleDispatchDriver
{
public:
	Xl_Ovals() {}		// Calls COleDispatchDriver default constructor
	Xl_Ovals(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Ovals(const Xl_Ovals& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	BOOL GetAddIndent();
	void SetAddIndent(BOOL bNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	LPDISPATCH GetFont();
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	LPDISPATCH GetBorder();
	LPDISPATCH GetInterior();
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	LPDISPATCH Add(double Left, double Top, double Width, double Height);
	long GetCount();
	LPDISPATCH Group();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Arc wrapper class

class __MY_EXT_CLASS__ Xl_Arc : public COleDispatchDriver
{
public:
	Xl_Arc() {}		// Calls COleDispatchDriver default constructor
	Xl_Arc(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Arc(const Xl_Arc& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	BOOL GetAddIndent();
	void SetAddIndent(BOOL bNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	LPDISPATCH GetFont();
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	LPDISPATCH GetBorder();
	LPDISPATCH GetInterior();
};
/////////////////////////////////////////////////////////////////////////////
// Arcs wrapper class

class __MY_EXT_CLASS__ Xl_Arcs : public COleDispatchDriver
{
public:
	Xl_Arcs() {}		// Calls COleDispatchDriver default constructor
	Xl_Arcs(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Arcs(const Xl_Arcs& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	BOOL GetAddIndent();
	void SetAddIndent(BOOL bNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	LPDISPATCH GetFont();
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	LPDISPATCH GetBorder();
	LPDISPATCH GetInterior();
	LPDISPATCH Add(double X1, double Y1, double X2, double Y2);
	long GetCount();
	LPDISPATCH Group();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// OLEObjectEvents wrapper class

class __MY_EXT_CLASS__ Xl_OLEObjectEvents : public COleDispatchDriver
{
public:
	Xl_OLEObjectEvents() {}		// Calls COleDispatchDriver default constructor
	Xl_OLEObjectEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_OLEObjectEvents(const Xl_OLEObjectEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void GotFocus();
	void LostFocus();
};
/////////////////////////////////////////////////////////////////////////////
// _OLEObject wrapper class

class __MY_EXT_CLASS__ Xl_OLEObject : public COleDispatchDriver
{
public:
	Xl_OLEObject() {}		// Calls COleDispatchDriver default constructor
	Xl_OLEObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_OLEObject(const Xl_OLEObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	LPDISPATCH GetBorder();
	LPDISPATCH GetInterior();
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	VARIANT Activate();
	BOOL GetAutoLoad();
	void SetAutoLoad(BOOL bNewValue);
	BOOL GetAutoUpdate();
	void SetAutoUpdate(BOOL bNewValue);
	LPDISPATCH GetObject();
	VARIANT GetOLEType();
	CString GetSourceName();
	void SetSourceName(LPCTSTR lpszNewValue);
	VARIANT Update();
	VARIANT Verb(long Verb);
	CString GetLinkedCell();
	void SetLinkedCell(LPCTSTR lpszNewValue);
	CString GetListFillRange();
	void SetListFillRange(LPCTSTR lpszNewValue);
	CString GetProgId();
	CString GetAltHTML();
	void SetAltHTML(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// OLEObjects wrapper class

class __MY_EXT_CLASS__ Xl_OLEObjects : public COleDispatchDriver
{
public:
	Xl_OLEObjects() {}		// Calls COleDispatchDriver default constructor
	Xl_OLEObjects(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_OLEObjects(const Xl_OLEObjects& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	LPDISPATCH GetBorder();
	LPDISPATCH GetInterior();
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	BOOL GetAutoLoad();
	void SetAutoLoad(BOOL bNewValue);
	CString GetSourceName();
	void SetSourceName(LPCTSTR lpszNewValue);
	LPDISPATCH Add(const VARIANT& ClassType, const VARIANT& Filename, const VARIANT& Link, const VARIANT& DisplayAsIcon, const VARIANT& IconFileName, const VARIANT& IconIndex, const VARIANT& IconLabel, const VARIANT& Left, const VARIANT& Top, 
		const VARIANT& Width, const VARIANT& Height);
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// TextBox wrapper class

class __MY_EXT_CLASS__ Xl_TextBox : public COleDispatchDriver
{
public:
	Xl_TextBox() {}		// Calls COleDispatchDriver default constructor
	Xl_TextBox(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_TextBox(const Xl_TextBox& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	BOOL GetAddIndent();
	void SetAddIndent(BOOL bNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	LPDISPATCH GetFont();
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	LPDISPATCH GetBorder();
	LPDISPATCH GetInterior();
	BOOL GetRoundedCorners();
	void SetRoundedCorners(BOOL bNewValue);
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// TextBoxes wrapper class

class __MY_EXT_CLASS__ Xl_TextBoxes : public COleDispatchDriver
{
public:
	Xl_TextBoxes() {}		// Calls COleDispatchDriver default constructor
	Xl_TextBoxes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_TextBoxes(const Xl_TextBoxes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	BOOL GetAddIndent();
	void SetAddIndent(BOOL bNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	LPDISPATCH GetFont();
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	LPDISPATCH GetBorder();
	LPDISPATCH GetInterior();
	BOOL GetRoundedCorners();
	void SetRoundedCorners(BOOL bNewValue);
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	LPDISPATCH Add(double Left, double Top, double Width, double Height);
	long GetCount();
	LPDISPATCH Group();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Picture wrapper class

class __MY_EXT_CLASS__ Xl_Picture : public COleDispatchDriver
{
public:
	Xl_Picture() {}		// Calls COleDispatchDriver default constructor
	Xl_Picture(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Picture(const Xl_Picture& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	LPDISPATCH GetBorder();
	LPDISPATCH GetInterior();
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Pictures wrapper class

class __MY_EXT_CLASS__ Xl_Pictures : public COleDispatchDriver
{
public:
	Xl_Pictures() {}		// Calls COleDispatchDriver default constructor
	Xl_Pictures(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Pictures(const Xl_Pictures& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	LPDISPATCH GetBorder();
	LPDISPATCH GetInterior();
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	LPDISPATCH Add(double Left, double Top, double Width, double Height);
	long GetCount();
	LPDISPATCH Group();
	LPDISPATCH Insert(LPCTSTR Filename, const VARIANT& Converter);
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
	LPDISPATCH Paste(const VARIANT& Link);
};
/////////////////////////////////////////////////////////////////////////////
// Drawing wrapper class

class __MY_EXT_CLASS__ Xl_Drawing : public COleDispatchDriver
{
public:
	Xl_Drawing() {}		// Calls COleDispatchDriver default constructor
	Xl_Drawing(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Drawing(const Xl_Drawing& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	BOOL GetAddIndent();
	void SetAddIndent(BOOL bNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	LPDISPATCH GetFont();
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	LPDISPATCH GetBorder();
	LPDISPATCH GetInterior();
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	VARIANT AddVertex(double Left, double Top);
	VARIANT Reshape(long Vertex, BOOL Insert, const VARIANT& Left, const VARIANT& Top);
	VARIANT GetVertices(const VARIANT& Index1, const VARIANT& Index2);
};
/////////////////////////////////////////////////////////////////////////////
// Drawings wrapper class

class __MY_EXT_CLASS__ Xl_Drawings : public COleDispatchDriver
{
public:
	Xl_Drawings() {}		// Calls COleDispatchDriver default constructor
	Xl_Drawings(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Drawings(const Xl_Drawings& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	BOOL GetAddIndent();
	void SetAddIndent(BOOL bNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	LPDISPATCH GetFont();
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	LPDISPATCH GetBorder();
	LPDISPATCH GetInterior();
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	VARIANT Reshape(long Vertex, BOOL Insert, const VARIANT& Left, const VARIANT& Top);
	LPDISPATCH Add(double X1, double Y1, double X2, double Y2, BOOL Closed);
	long GetCount();
	LPDISPATCH Group();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// RoutingSlip wrapper class

class __MY_EXT_CLASS__ Xl_RoutingSlip : public COleDispatchDriver
{
public:
	Xl_RoutingSlip() {}		// Calls COleDispatchDriver default constructor
	Xl_RoutingSlip(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_RoutingSlip(const Xl_RoutingSlip& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetDelivery();
	void SetDelivery(long nNewValue);
	VARIANT GetMessage();
	void SetMessage(const VARIANT& newValue);
	VARIANT GetRecipients(const VARIANT& Index);
	void SetRecipients(const VARIANT& Index, const VARIANT& newValue);
	VARIANT Reset();
	BOOL GetReturnWhenDone();
	void SetReturnWhenDone(BOOL bNewValue);
	long GetStatus();
	VARIANT GetSubject();
	void SetSubject(const VARIANT& newValue);
	BOOL GetTrackStatus();
	void SetTrackStatus(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Outline wrapper class

class __MY_EXT_CLASS__ Xl_Outline : public COleDispatchDriver
{
public:
	Xl_Outline() {}		// Calls COleDispatchDriver default constructor
	Xl_Outline(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Outline(const Xl_Outline& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	BOOL GetAutomaticStyles();
	void SetAutomaticStyles(BOOL bNewValue);
	VARIANT ShowLevels(const VARIANT& RowLevels, const VARIANT& ColumnLevels);
	long GetSummaryColumn();
	void SetSummaryColumn(long nNewValue);
	long GetSummaryRow();
	void SetSummaryRow(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Module wrapper class

class __MY_EXT_CLASS__ Xl_Module : public COleDispatchDriver
{
public:
	Xl_Module() {}		// Calls COleDispatchDriver default constructor
	Xl_Module(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Module(const Xl_Module& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void Activate();
	void Copy(const VARIANT& Before, const VARIANT& After);
	void Delete();
	CString GetCodeName();
	CString Get_CodeName();
	void Set_CodeName(LPCTSTR lpszNewValue);
	long GetIndex();
	void Move(const VARIANT& Before, const VARIANT& After);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	LPDISPATCH GetNext();
	LPDISPATCH GetPageSetup();
	LPDISPATCH GetPrevious();
	void Protect(const VARIANT& Password, const VARIANT& DrawingObjects, const VARIANT& Contents, const VARIANT& Scenarios, const VARIANT& UserInterfaceOnly);
	BOOL GetProtectContents();
	BOOL GetProtectionMode();
	void SaveAs(LPCTSTR Filename, const VARIANT& FileFormat, const VARIANT& Password, const VARIANT& WriteResPassword, const VARIANT& ReadOnlyRecommended, const VARIANT& CreateBackup, const VARIANT& AddToMru, const VARIANT& TextCodepage, 
		const VARIANT& TextVisualLayout);
	void Select(const VARIANT& Replace);
	void Unprotect(const VARIANT& Password);
	long GetVisible();
	void SetVisible(long nNewValue);
	LPDISPATCH GetShapes();
	VARIANT InsertFile(const VARIANT& Filename, const VARIANT& Merge);
};
/////////////////////////////////////////////////////////////////////////////
// Modules wrapper class

class __MY_EXT_CLASS__ Xl_Modules : public COleDispatchDriver
{
public:
	Xl_Modules() {}		// Calls COleDispatchDriver default constructor
	Xl_Modules(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Modules(const Xl_Modules& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(const VARIANT& Before, const VARIANT& After, const VARIANT& Count);
	void Copy(const VARIANT& Before, const VARIANT& After);
	long GetCount();
	void Delete();
	LPDISPATCH GetItem(const VARIANT& Index);
	void Move(const VARIANT& Before, const VARIANT& After);
	LPUNKNOWN Get_NewEnum();
	void Select(const VARIANT& Replace);
	LPDISPATCH GetHPageBreaks();
	LPDISPATCH GetVPageBreaks();
	VARIANT GetVisible();
	void SetVisible(const VARIANT& newValue);
	LPDISPATCH Get_Default(const VARIANT& Index);
	void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
};
/////////////////////////////////////////////////////////////////////////////
// DialogSheet wrapper class

class __MY_EXT_CLASS__ Xl_DialogSheet : public COleDispatchDriver
{
public:
	Xl_DialogSheet() {}		// Calls COleDispatchDriver default constructor
	Xl_DialogSheet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_DialogSheet(const Xl_DialogSheet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void Activate();
	void Copy(const VARIANT& Before, const VARIANT& After);
	void Delete();
	CString GetCodeName();
	CString Get_CodeName();
	void Set_CodeName(LPCTSTR lpszNewValue);
	long GetIndex();
	void Move(const VARIANT& Before, const VARIANT& After);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	LPDISPATCH GetNext();
	LPDISPATCH GetPageSetup();
	LPDISPATCH GetPrevious();
	void PrintPreview(const VARIANT& EnableChanges);
	void Protect(const VARIANT& Password, const VARIANT& DrawingObjects, const VARIANT& Contents, const VARIANT& Scenarios, const VARIANT& UserInterfaceOnly);
	BOOL GetProtectContents();
	BOOL GetProtectDrawingObjects();
	BOOL GetProtectionMode();
	BOOL GetProtectScenarios();
	void SaveAs(LPCTSTR Filename, const VARIANT& FileFormat, const VARIANT& Password, const VARIANT& WriteResPassword, const VARIANT& ReadOnlyRecommended, const VARIANT& CreateBackup, const VARIANT& AddToMru, const VARIANT& TextCodepage, 
		const VARIANT& TextVisualLayout);
	void Select(const VARIANT& Replace);
	void Unprotect(const VARIANT& Password);
	long GetVisible();
	void SetVisible(long nNewValue);
	LPDISPATCH GetShapes();
	BOOL GetEnableCalculation();
	void SetEnableCalculation(BOOL bNewValue);
	LPDISPATCH ChartObjects(const VARIANT& Index);
	void CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
	BOOL GetEnableAutoFilter();
	void SetEnableAutoFilter(BOOL bNewValue);
	long GetEnableSelection();
	void SetEnableSelection(long nNewValue);
	BOOL GetEnableOutlining();
	void SetEnableOutlining(BOOL bNewValue);
	BOOL GetEnablePivotTable();
	void SetEnablePivotTable(BOOL bNewValue);
	VARIANT Evaluate(const VARIANT& Name);
	VARIANT _Evaluate(const VARIANT& Name);
	void ResetAllPageBreaks();
	LPDISPATCH GetNames();
	LPDISPATCH OLEObjects(const VARIANT& Index);
	void Paste(const VARIANT& Destination, const VARIANT& Link);
	void PasteSpecial(const VARIANT& Format, const VARIANT& Link, const VARIANT& DisplayAsIcon, const VARIANT& IconFileName, const VARIANT& IconIndex, const VARIANT& IconLabel);
	CString GetScrollArea();
	void SetScrollArea(LPCTSTR lpszNewValue);
	LPDISPATCH GetHPageBreaks();
	LPDISPATCH GetVPageBreaks();
	LPDISPATCH GetQueryTables();
	BOOL GetDisplayPageBreaks();
	void SetDisplayPageBreaks(BOOL bNewValue);
	LPDISPATCH GetComments();
	LPDISPATCH GetHyperlinks();
	void ClearCircles();
	void CircleInvalid();
	LPDISPATCH GetAutoFilter();
	BOOL GetDisplayRightToLeft();
	void SetDisplayRightToLeft(BOOL bNewValue);
	LPDISPATCH GetScripts();
	void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
	VARIANT GetDefaultButton();
	void SetDefaultButton(const VARIANT& newValue);
	VARIANT GetFocus();
	void SetFocus(const VARIANT& newValue);
	BOOL Hide(const VARIANT& Cancel);
	BOOL Show();
};
/////////////////////////////////////////////////////////////////////////////
// DialogSheets wrapper class

class __MY_EXT_CLASS__ Xl_DialogSheets : public COleDispatchDriver
{
public:
	Xl_DialogSheets() {}		// Calls COleDispatchDriver default constructor
	Xl_DialogSheets(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_DialogSheets(const Xl_DialogSheets& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(const VARIANT& Before, const VARIANT& After, const VARIANT& Count);
	void Copy(const VARIANT& Before, const VARIANT& After);
	long GetCount();
	void Delete();
	LPDISPATCH GetItem(const VARIANT& Index);
	void Move(const VARIANT& Before, const VARIANT& After);
	LPUNKNOWN Get_NewEnum();
	void PrintPreview(const VARIANT& EnableChanges);
	void Select(const VARIANT& Replace);
	LPDISPATCH GetHPageBreaks();
	LPDISPATCH GetVPageBreaks();
	VARIANT GetVisible();
	void SetVisible(const VARIANT& newValue);
	LPDISPATCH Get_Default(const VARIANT& Index);
	void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
};
/////////////////////////////////////////////////////////////////////////////
// Worksheets wrapper class

class __MY_EXT_CLASS__ Xl_Worksheets : public COleDispatchDriver
{
public:
	Xl_Worksheets() {}		// Calls COleDispatchDriver default constructor
	Xl_Worksheets(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Worksheets(const Xl_Worksheets& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(const VARIANT& Before, const VARIANT& After, const VARIANT& Count, const VARIANT& Type);
	void Copy(const VARIANT& Before, const VARIANT& After);
	long GetCount();
	void Delete();
	void FillAcrossSheets(LPDISPATCH Range, long Type);
	LPDISPATCH GetItem(const VARIANT& Index);
	void Move(const VARIANT& Before, const VARIANT& After);
	LPUNKNOWN Get_NewEnum();
	void PrintPreview(const VARIANT& EnableChanges);
	void Select(const VARIANT& Replace);
	LPDISPATCH GetHPageBreaks();
	LPDISPATCH GetVPageBreaks();
	VARIANT GetVisible();
	void SetVisible(const VARIANT& newValue);
	LPDISPATCH Get_Default(const VARIANT& Index);
	void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
};
/////////////////////////////////////////////////////////////////////////////
// PageSetup wrapper class
class __MY_EXT_CLASS__ Xl_PageSetup : public COleDispatchDriver
{
public:
	Xl_PageSetup() {}		// Calls COleDispatchDriver default constructor
	Xl_PageSetup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_PageSetup(const Xl_PageSetup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	BOOL GetBlackAndWhite();
	void SetBlackAndWhite(BOOL bNewValue);
	double GetBottomMargin();
	void SetBottomMargin(double newValue);
	CString GetCenterFooter();
	void SetCenterFooter(LPCTSTR lpszNewValue);
	CString GetCenterHeader();
	void SetCenterHeader(LPCTSTR lpszNewValue);
	BOOL GetCenterHorizontally();
	void SetCenterHorizontally(BOOL bNewValue);
	BOOL GetCenterVertically();
	void SetCenterVertically(BOOL bNewValue);
	long GetChartSize();
	void SetChartSize(long nNewValue);
	BOOL GetDraft();
	void SetDraft(BOOL bNewValue);
	long GetFirstPageNumber();
	void SetFirstPageNumber(long nNewValue);
	VARIANT GetFitToPagesTall();
	void SetFitToPagesTall(const VARIANT& newValue);
	VARIANT GetFitToPagesWide();
	void SetFitToPagesWide(const VARIANT& newValue);
	double GetFooterMargin();
	void SetFooterMargin(double newValue);
	double GetHeaderMargin();
	void SetHeaderMargin(double newValue);
	CString GetLeftFooter();
	void SetLeftFooter(LPCTSTR lpszNewValue);
	CString GetLeftHeader();
	void SetLeftHeader(LPCTSTR lpszNewValue);
	double GetLeftMargin();
	void SetLeftMargin(double newValue);
	long GetOrder();
	void SetOrder(long nNewValue);
	long GetOrientation();
	void SetOrientation(long nNewValue);
	long GetPaperSize();
	void SetPaperSize(long nNewValue);
	CString GetPrintArea();
	void SetPrintArea(LPCTSTR lpszNewValue);
	BOOL GetPrintGridlines();
	void SetPrintGridlines(BOOL bNewValue);
	BOOL GetPrintHeadings();
	void SetPrintHeadings(BOOL bNewValue);
	BOOL GetPrintNotes();
	void SetPrintNotes(BOOL bNewValue);
	VARIANT GetPrintQuality(const VARIANT& Index);
	void SetPrintQuality(const VARIANT& Index, const VARIANT& newValue);
	CString GetPrintTitleColumns();
	void SetPrintTitleColumns(LPCTSTR lpszNewValue);
	CString GetPrintTitleRows();
	void SetPrintTitleRows(LPCTSTR lpszNewValue);
	CString GetRightFooter();
	void SetRightFooter(LPCTSTR lpszNewValue);
	CString GetRightHeader();
	void SetRightHeader(LPCTSTR lpszNewValue);
	double GetRightMargin();
	void SetRightMargin(double newValue);
	double GetTopMargin();
	void SetTopMargin(double newValue);
	VARIANT GetZoom();
	void SetZoom(const VARIANT& newValue);
	long GetPrintComments();
	void SetPrintComments(long nNewValue);
};

/////////////////////////////////////////////////////////////////////////////
// Names wrapper class

class __MY_EXT_CLASS__ Xl_Names : public COleDispatchDriver
{
public:
	Xl_Names() {}		// Calls COleDispatchDriver default constructor
	Xl_Names(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Names(const Xl_Names& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(const VARIANT& Name, const VARIANT& RefersTo, const VARIANT& Visible, const VARIANT& MacroType, const VARIANT& ShortcutKey, const VARIANT& Category, const VARIANT& NameLocal, const VARIANT& RefersToLocal, 
		const VARIANT& CategoryLocal, const VARIANT& RefersToR1C1, const VARIANT& RefersToR1C1Local);
	LPDISPATCH Item(const VARIANT& Index, const VARIANT& IndexLocal, const VARIANT& RefersTo);
	LPDISPATCH _Default(const VARIANT& Index, const VARIANT& IndexLocal, const VARIANT& RefersTo);
	long GetCount();
	LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Name wrapper class

class __MY_EXT_CLASS__ Xl_Name : public COleDispatchDriver
{
public:
	Xl_Name() {}		// Calls COleDispatchDriver default constructor
	Xl_Name(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Name(const Xl_Name& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString Get_Default();
	long GetIndex();
	CString GetCategory();
	void SetCategory(LPCTSTR lpszNewValue);
	CString GetCategoryLocal();
	void SetCategoryLocal(LPCTSTR lpszNewValue);
	void Delete();
	long GetMacroType();
	void SetMacroType(long nNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetRefersTo();
	void SetRefersTo(const VARIANT& newValue);
	CString GetShortcutKey();
	void SetShortcutKey(LPCTSTR lpszNewValue);
	CString GetValue();
	void SetValue(LPCTSTR lpszNewValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	CString GetNameLocal();
	void SetNameLocal(LPCTSTR lpszNewValue);
	VARIANT GetRefersToLocal();
	void SetRefersToLocal(const VARIANT& newValue);
	VARIANT GetRefersToR1C1();
	void SetRefersToR1C1(const VARIANT& newValue);
	VARIANT GetRefersToR1C1Local();
	void SetRefersToR1C1Local(const VARIANT& newValue);
	LPDISPATCH GetRefersToRange();
};
/////////////////////////////////////////////////////////////////////////////
// ChartObject wrapper class

class __MY_EXT_CLASS__ Xl_ChartObject : public COleDispatchDriver
{
public:
	Xl_ChartObject() {}		// Calls COleDispatchDriver default constructor
	Xl_ChartObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ChartObject(const Xl_ChartObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBottomRightCell();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	long GetIndex();
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	LPDISPATCH GetTopLeftCell();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	long GetZOrder();
	LPDISPATCH GetShapeRange();
	VARIANT Activate();
	LPDISPATCH GetChart();
	BOOL GetProtectChartObject();
	void SetProtectChartObject(BOOL bNewValue);
	BOOL GetRoundedCorners();
	void SetRoundedCorners(BOOL bNewValue);
	LPDISPATCH GetBorder();
	LPDISPATCH GetInterior();
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ChartObjects wrapper class

class __MY_EXT_CLASS__ Xl_ChartObjects : public COleDispatchDriver
{
public:
	Xl_ChartObjects() {}		// Calls COleDispatchDriver default constructor
	Xl_ChartObjects(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ChartObjects(const Xl_ChartObjects& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT BringToFront();
	VARIANT Copy();
	VARIANT CopyPicture(long Appearance, long Format);
	VARIANT Cut();
	VARIANT Delete();
	LPDISPATCH Duplicate();
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	double GetHeight();
	void SetHeight(double newValue);
	double GetLeft();
	void SetLeft(double newValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	VARIANT GetPlacement();
	void SetPlacement(const VARIANT& newValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	VARIANT Select(const VARIANT& Replace);
	VARIANT SendToBack();
	double GetTop();
	void SetTop(double newValue);
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	double GetWidth();
	void SetWidth(double newValue);
	LPDISPATCH GetShapeRange();
	BOOL GetRoundedCorners();
	void SetRoundedCorners(BOOL bNewValue);
	LPDISPATCH GetBorder();
	LPDISPATCH GetInterior();
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	LPDISPATCH Add(double Left, double Top, double Width, double Height);
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Mailer wrapper class

class __MY_EXT_CLASS__ Xl_Mailer : public COleDispatchDriver
{
public:
	Xl_Mailer() {}		// Calls COleDispatchDriver default constructor
	Xl_Mailer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Mailer(const Xl_Mailer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT GetBCCRecipients();
	void SetBCCRecipients(const VARIANT& newValue);
	VARIANT GetCCRecipients();
	void SetCCRecipients(const VARIANT& newValue);
	VARIANT GetEnclosures();
	void SetEnclosures(const VARIANT& newValue);
	BOOL GetReceived();
	DATE GetSendDateTime();
	CString GetSender();
	CString GetSubject();
	void SetSubject(LPCTSTR lpszNewValue);
	VARIANT GetToRecipients();
	void SetToRecipients(const VARIANT& newValue);
	VARIANT GetWhichAddress();
	void SetWhichAddress(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// CustomViews wrapper class

class __MY_EXT_CLASS__ Xl_CustomViews : public COleDispatchDriver
{
public:
	Xl_CustomViews() {}		// Calls COleDispatchDriver default constructor
	Xl_CustomViews(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_CustomViews(const Xl_CustomViews& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& ViewName);
	LPDISPATCH Add(LPCTSTR ViewName, const VARIANT& PrintSettings, const VARIANT& RowColSettings);
	LPDISPATCH Get_Default(const VARIANT& ViewName);
	LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// CustomView wrapper class

class __MY_EXT_CLASS__ Xl_CustomView : public COleDispatchDriver
{
public:
	Xl_CustomView() {}		// Calls COleDispatchDriver default constructor
	Xl_CustomView(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_CustomView(const Xl_CustomView& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	BOOL GetPrintSettings();
	BOOL GetRowColSettings();
	void Show();
	void Delete();
};
/////////////////////////////////////////////////////////////////////////////
// FormatConditions wrapper class

class __MY_EXT_CLASS__ Xl_FormatConditions : public COleDispatchDriver
{
public:
	Xl_FormatConditions() {}		// Calls COleDispatchDriver default constructor
	Xl_FormatConditions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_FormatConditions(const Xl_FormatConditions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Add(long Type, const VARIANT& Operator, const VARIANT& Formula1, const VARIANT& Formula2);
	LPDISPATCH Get_Default(const VARIANT& Index);
	LPUNKNOWN Get_NewEnum();
	void Delete();
};
/////////////////////////////////////////////////////////////////////////////
// FormatCondition wrapper class

class __MY_EXT_CLASS__ Xl_FormatCondition : public COleDispatchDriver
{
public:
	Xl_FormatCondition() {}		// Calls COleDispatchDriver default constructor
	Xl_FormatCondition(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_FormatCondition(const Xl_FormatCondition& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void Modify(long Type, const VARIANT& Operator, const VARIANT& Formula1, const VARIANT& Formula2);
	long GetType();
	long GetOperator();
	CString GetFormula1();
	CString GetFormula2();
	LPDISPATCH GetInterior();
	LPDISPATCH GetBorders();
	LPDISPATCH GetFont();
	void Delete();
};
/////////////////////////////////////////////////////////////////////////////
// Comments wrapper class

class __MY_EXT_CLASS__ Xl_Comments : public COleDispatchDriver
{
public:
	Xl_Comments() {}		// Calls COleDispatchDriver default constructor
	Xl_Comments(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Comments(const Xl_Comments& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(long Index);
	LPDISPATCH Get_Default(long Index);
	LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Comment wrapper class

class __MY_EXT_CLASS__ Xl_Comment : public COleDispatchDriver
{
public:
	Xl_Comment() {}		// Calls COleDispatchDriver default constructor
	Xl_Comment(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Comment(const Xl_Comment& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetAuthor();
	LPDISPATCH GetShape();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	CString Text(const VARIANT& Text, const VARIANT& Start, const VARIANT& Overwrite);
	void Delete();
	LPDISPATCH Next();
	LPDISPATCH Previous();
};
/////////////////////////////////////////////////////////////////////////////
// RefreshEvents wrapper class

class __MY_EXT_CLASS__ Xl_RefreshEvents : public COleDispatchDriver
{
public:
	Xl_RefreshEvents() {}		// Calls COleDispatchDriver default constructor
	Xl_RefreshEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_RefreshEvents(const Xl_RefreshEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void BeforeRefresh(BOOL* Cancel);
	void AfterRefresh(BOOL Success);
};
/////////////////////////////////////////////////////////////////////////////
// _QueryTable wrapper class

class __MY_EXT_CLASS__ Xl_QueryTable : public COleDispatchDriver
{
public:
	Xl_QueryTable() {}		// Calls COleDispatchDriver default constructor
	Xl_QueryTable(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_QueryTable(const Xl_QueryTable& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	BOOL GetFieldNames();
	void SetFieldNames(BOOL bNewValue);
	BOOL GetRowNumbers();
	void SetRowNumbers(BOOL bNewValue);
	BOOL GetFillAdjacentFormulas();
	void SetFillAdjacentFormulas(BOOL bNewValue);
	BOOL GetRefreshOnFileOpen();
	void SetRefreshOnFileOpen(BOOL bNewValue);
	BOOL GetRefreshing();
	BOOL GetFetchedRowOverflow();
	BOOL GetBackgroundQuery();
	void SetBackgroundQuery(BOOL bNewValue);
	void CancelRefresh();
	long GetRefreshStyle();
	void SetRefreshStyle(long nNewValue);
	BOOL GetEnableRefresh();
	void SetEnableRefresh(BOOL bNewValue);
	BOOL GetSavePassword();
	void SetSavePassword(BOOL bNewValue);
	LPDISPATCH GetDestination();
	VARIANT GetConnection();
	void SetConnection(const VARIANT& newValue);
	CString GetPostText();
	void SetPostText(LPCTSTR lpszNewValue);
	LPDISPATCH GetResultRange();
	void Delete();
	BOOL Refresh(const VARIANT& BackgroundQuery);
	LPDISPATCH GetParameters();
	LPDISPATCH GetRecordset();
	void SetRefRecordset(LPDISPATCH newValue);
	BOOL GetSaveData();
	void SetSaveData(BOOL bNewValue);
	BOOL GetEnableEditing();
	void SetEnableEditing(BOOL bNewValue);
	long GetTextFilePlatform();
	void SetTextFilePlatform(long nNewValue);
	long GetTextFileStartRow();
	void SetTextFileStartRow(long nNewValue);
	long GetTextFileParseType();
	void SetTextFileParseType(long nNewValue);
	long GetTextFileTextQualifier();
	void SetTextFileTextQualifier(long nNewValue);
	BOOL GetTextFileConsecutiveDelimiter();
	void SetTextFileConsecutiveDelimiter(BOOL bNewValue);
	BOOL GetTextFileTabDelimiter();
	void SetTextFileTabDelimiter(BOOL bNewValue);
	BOOL GetTextFileSemicolonDelimiter();
	void SetTextFileSemicolonDelimiter(BOOL bNewValue);
	BOOL GetTextFileCommaDelimiter();
	void SetTextFileCommaDelimiter(BOOL bNewValue);
	BOOL GetTextFileSpaceDelimiter();
	void SetTextFileSpaceDelimiter(BOOL bNewValue);
	CString GetTextFileOtherDelimiter();
	void SetTextFileOtherDelimiter(LPCTSTR lpszNewValue);
	VARIANT GetTextFileColumnDataTypes();
	void SetTextFileColumnDataTypes(const VARIANT& newValue);
	VARIANT GetTextFileFixedColumnWidths();
	void SetTextFileFixedColumnWidths(const VARIANT& newValue);
	BOOL GetPreserveColumnInfo();
	void SetPreserveColumnInfo(BOOL bNewValue);
	BOOL GetPreserveFormatting();
	void SetPreserveFormatting(BOOL bNewValue);
	BOOL GetAdjustColumnWidth();
	void SetAdjustColumnWidth(BOOL bNewValue);
	VARIANT GetCommandText();
	void SetCommandText(const VARIANT& newValue);
	long GetCommandType();
	void SetCommandType(long nNewValue);
	BOOL GetTextFilePromptOnRefresh();
	void SetTextFilePromptOnRefresh(BOOL bNewValue);
	long GetQueryType();
	BOOL GetMaintainConnection();
	void SetMaintainConnection(BOOL bNewValue);
	CString GetTextFileDecimalSeparator();
	void SetTextFileDecimalSeparator(LPCTSTR lpszNewValue);
	CString GetTextFileThousandsSeparator();
	void SetTextFileThousandsSeparator(LPCTSTR lpszNewValue);
	long GetRefreshPeriod();
	void SetRefreshPeriod(long nNewValue);
	void ResetTimer();
	long GetWebSelectionType();
	void SetWebSelectionType(long nNewValue);
	long GetWebFormatting();
	void SetWebFormatting(long nNewValue);
	CString GetWebTables();
	void SetWebTables(LPCTSTR lpszNewValue);
	BOOL GetWebPreFormattedTextToColumns();
	void SetWebPreFormattedTextToColumns(BOOL bNewValue);
	BOOL GetWebSingleBlockTextImport();
	void SetWebSingleBlockTextImport(BOOL bNewValue);
	BOOL GetWebDisableDateRecognition();
	void SetWebDisableDateRecognition(BOOL bNewValue);
	BOOL GetWebConsecutiveDelimitersAsOne();
	void SetWebConsecutiveDelimitersAsOne(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// QueryTables wrapper class

class __MY_EXT_CLASS__ Xl_QueryTables : public COleDispatchDriver
{
public:
	Xl_QueryTables() {}		// Calls COleDispatchDriver default constructor
	Xl_QueryTables(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_QueryTables(const Xl_QueryTables& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Add(const VARIANT& Connection, LPDISPATCH Destination, const VARIANT& Sql);
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Get_Default(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Parameter wrapper class

class __MY_EXT_CLASS__ Xl_Parameter : public COleDispatchDriver
{
public:
	Xl_Parameter() {}		// Calls COleDispatchDriver default constructor
	Xl_Parameter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Parameter(const Xl_Parameter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetDataType();
	void SetDataType(long nNewValue);
	long GetType();
	CString GetPromptString();
	VARIANT GetValue();
	LPDISPATCH GetSourceRange();
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	void SetParam(long Type, const VARIANT& Value);
	BOOL GetRefreshOnChange();
	void SetRefreshOnChange(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Parameters wrapper class

class __MY_EXT_CLASS__ Xl_Parameters : public COleDispatchDriver
{
public:
	Xl_Parameters() {}		// Calls COleDispatchDriver default constructor
	Xl_Parameters(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Parameters(const Xl_Parameters& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(LPCTSTR Name, const VARIANT& iDataType);
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH Get_Default(const VARIANT& Index);
	void Delete();
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// ODBCError wrapper class

class __MY_EXT_CLASS__ Xl_ODBCError : public COleDispatchDriver
{
public:
	Xl_ODBCError() {}		// Calls COleDispatchDriver default constructor
	Xl_ODBCError(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ODBCError(const Xl_ODBCError& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetSqlState();
	CString GetErrorString();
};
/////////////////////////////////////////////////////////////////////////////
// ODBCErrors wrapper class

class __MY_EXT_CLASS__ Xl_ODBCErrors : public COleDispatchDriver
{
public:
	Xl_ODBCErrors() {}		// Calls COleDispatchDriver default constructor
	Xl_ODBCErrors(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ODBCErrors(const Xl_ODBCErrors& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(long Index);
	LPDISPATCH Get_Default(long Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Validation wrapper class

class __MY_EXT_CLASS__ Xl_Validation : public COleDispatchDriver
{
public:
	Xl_Validation() {}		// Calls COleDispatchDriver default constructor
	Xl_Validation(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Validation(const Xl_Validation& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void Add(long Type, const VARIANT& AlertStyle, const VARIANT& Operator, const VARIANT& Formula1, const VARIANT& Formula2);
	long GetAlertStyle();
	BOOL GetIgnoreBlank();
	void SetIgnoreBlank(BOOL bNewValue);
	long GetIMEMode();
	void SetIMEMode(long nNewValue);
	BOOL GetInCellDropdown();
	void SetInCellDropdown(BOOL bNewValue);
	void Delete();
	CString GetErrorMessage();
	void SetErrorMessage(LPCTSTR lpszNewValue);
	CString GetErrorTitle();
	void SetErrorTitle(LPCTSTR lpszNewValue);
	CString GetInputMessage();
	void SetInputMessage(LPCTSTR lpszNewValue);
	CString GetInputTitle();
	void SetInputTitle(LPCTSTR lpszNewValue);
	CString GetFormula1();
	CString GetFormula2();
	void Modify(const VARIANT& Type, const VARIANT& AlertStyle, const VARIANT& Operator, const VARIANT& Formula1, const VARIANT& Formula2);
	long GetOperator();
	BOOL GetShowError();
	void SetShowError(BOOL bNewValue);
	BOOL GetShowInput();
	void SetShowInput(BOOL bNewValue);
	long GetType();
	BOOL GetValue();
};
/////////////////////////////////////////////////////////////////////////////
// Hyperlinks wrapper class

class __MY_EXT_CLASS__ Xl_Hyperlinks : public COleDispatchDriver
{
public:
	Xl_Hyperlinks() {}		// Calls COleDispatchDriver default constructor
	Xl_Hyperlinks(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Hyperlinks(const Xl_Hyperlinks& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(LPDISPATCH Anchor, LPCTSTR Address, const VARIANT& SubAddress, const VARIANT& ScreenTip, const VARIANT& TextToDisplay);
	long GetCount();
	LPDISPATCH GetItem(const VARIANT& Index);
	LPDISPATCH Get_Default(const VARIANT& Index);
	LPUNKNOWN Get_NewEnum();
	void Delete();
};
/////////////////////////////////////////////////////////////////////////////
// Hyperlink wrapper class

class __MY_EXT_CLASS__ Xl_Hyperlink : public COleDispatchDriver
{
public:
	Xl_Hyperlink() {}		// Calls COleDispatchDriver default constructor
	Xl_Hyperlink(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Hyperlink(const Xl_Hyperlink& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	LPDISPATCH GetRange();
	LPDISPATCH GetShape();
	CString GetSubAddress();
	void SetSubAddress(LPCTSTR lpszNewValue);
	CString GetAddress();
	void SetAddress(LPCTSTR lpszNewValue);
	long GetType();
	void AddToFavorites();
	void Delete();
	void Follow(const VARIANT& NewWindow, const VARIANT& AddHistory, const VARIANT& ExtraInfo, const VARIANT& Method, const VARIANT& HeaderInfo);
	CString GetEmailSubject();
	void SetEmailSubject(LPCTSTR lpszNewValue);
	CString GetScreenTip();
	void SetScreenTip(LPCTSTR lpszNewValue);
	CString GetTextToDisplay();
	void SetTextToDisplay(LPCTSTR lpszNewValue);
	void CreateNewDocument(LPCTSTR Filename, BOOL EditNow, BOOL Overwrite);
};
/////////////////////////////////////////////////////////////////////////////
// AutoFilter wrapper class

class __MY_EXT_CLASS__ Xl_AutoFilter : public COleDispatchDriver
{
public:
	Xl_AutoFilter() {}		// Calls COleDispatchDriver default constructor
	Xl_AutoFilter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_AutoFilter(const Xl_AutoFilter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetRange();
	LPDISPATCH GetFilters();
};
/////////////////////////////////////////////////////////////////////////////
// Filters wrapper class

class __MY_EXT_CLASS__ Xl_Filters : public COleDispatchDriver
{
public:
	Xl_Filters() {}		// Calls COleDispatchDriver default constructor
	Xl_Filters(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Filters(const Xl_Filters& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Get_Default(long Index);
	LPDISPATCH GetItem(long Index);
	LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Filter wrapper class

class __MY_EXT_CLASS__ Xl_Filter : public COleDispatchDriver
{
public:
	Xl_Filter() {}		// Calls COleDispatchDriver default constructor
	Xl_Filter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Filter(const Xl_Filter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	BOOL GetOn();
	VARIANT GetCriteria1();
	long GetOperator();
	VARIANT GetCriteria2();
};
/////////////////////////////////////////////////////////////////////////////
// AutoCorrect wrapper class

class __MY_EXT_CLASS__ Xl_AutoCorrect : public COleDispatchDriver
{
public:
	Xl_AutoCorrect() {}		// Calls COleDispatchDriver default constructor
	Xl_AutoCorrect(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_AutoCorrect(const Xl_AutoCorrect& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT AddReplacement(LPCTSTR What, LPCTSTR Replacement);
	BOOL GetCapitalizeNamesOfDays();
	void SetCapitalizeNamesOfDays(BOOL bNewValue);
	VARIANT DeleteReplacement(LPCTSTR What);
	VARIANT GetReplacementList(const VARIANT& Index);
	void SetReplacementList(const VARIANT& Index, const VARIANT& newValue);
	BOOL GetReplaceText();
	void SetReplaceText(BOOL bNewValue);
	BOOL GetTwoInitialCapitals();
	void SetTwoInitialCapitals(BOOL bNewValue);
	BOOL GetCorrectSentenceCap();
	void SetCorrectSentenceCap(BOOL bNewValue);
	BOOL GetCorrectCapsLock();
	void SetCorrectCapsLock(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Border wrapper class

class __MY_EXT_CLASS__ Xl_Border : public COleDispatchDriver
{
public:
	Xl_Border() {}		// Calls COleDispatchDriver default constructor
	Xl_Border(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Border(const Xl_Border& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT GetColor();
	void SetColor(const VARIANT& newValue);
	VARIANT GetColorIndex();
	void SetColorIndex(const VARIANT& newValue);
	VARIANT GetLineStyle();
	void SetLineStyle(const VARIANT& newValue);
	VARIANT GetWeight();
	void SetWeight(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// Interior wrapper class

class __MY_EXT_CLASS__ Xl_Interior : public COleDispatchDriver
{
public:
	Xl_Interior() {}		// Calls COleDispatchDriver default constructor
	Xl_Interior(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Interior(const Xl_Interior& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT GetColor();
	void SetColor(const VARIANT& newValue);
	VARIANT GetColorIndex();
	void SetColorIndex(const VARIANT& newValue);
	VARIANT GetInvertIfNegative();
	void SetInvertIfNegative(const VARIANT& newValue);
	VARIANT GetPattern();
	void SetPattern(const VARIANT& newValue);
	VARIANT GetPatternColor();
	void SetPatternColor(const VARIANT& newValue);
	VARIANT GetPatternColorIndex();
	void SetPatternColorIndex(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// ChartFillFormat wrapper class

class __MY_EXT_CLASS__ Xl_ChartFillFormat : public COleDispatchDriver
{
public:
	Xl_ChartFillFormat() {}		// Calls COleDispatchDriver default constructor
	Xl_ChartFillFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ChartFillFormat(const Xl_ChartFillFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void OneColorGradient(long Style, long Variant, float Degree);
	void TwoColorGradient(long Style, long Variant);
	void PresetTextured(long PresetTexture);
	void Solid();
	void Patterned(long Pattern);
	void UserPicture(const VARIANT& PictureFile, const VARIANT& PictureFormat, const VARIANT& PictureStackUnit, const VARIANT& PicturePlacement);
	void UserTextured(LPCTSTR TextureFile);
	void PresetGradient(long Style, long Variant, long PresetGradientType);
	LPDISPATCH GetBackColor();
	LPDISPATCH GetForeColor();
	long GetGradientColorType();
	float GetGradientDegree();
	long GetGradientStyle();
	long GetGradientVariant();
	long GetPattern();
	long GetPresetGradientType();
	long GetPresetTexture();
	CString GetTextureName();
	long GetTextureType();
	long GetType();
	long GetVisible();
	void SetVisible(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ChartColorFormat wrapper class

class __MY_EXT_CLASS__ Xl_ChartColorFormat : public COleDispatchDriver
{
public:
	Xl_ChartColorFormat() {}		// Calls COleDispatchDriver default constructor
	Xl_ChartColorFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ChartColorFormat(const Xl_ChartColorFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetSchemeColor();
	void SetSchemeColor(long nNewValue);
	long GetRgb();
	long Get_Default();
	long GetType();
};
/////////////////////////////////////////////////////////////////////////////
// Axis wrapper class

class __MY_EXT_CLASS__ Xl_Axis : public COleDispatchDriver
{
public:
	Xl_Axis() {}		// Calls COleDispatchDriver default constructor
	Xl_Axis(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Axis(const Xl_Axis& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	BOOL GetAxisBetweenCategories();
	void SetAxisBetweenCategories(BOOL bNewValue);
	long GetAxisGroup();
	LPDISPATCH GetAxisTitle();
	LPDISPATCH GetBorder();
	VARIANT GetCategoryNames();
	void SetCategoryNames(const VARIANT& newValue);
	long GetCrosses();
	void SetCrosses(long nNewValue);
	double GetCrossesAt();
	void SetCrossesAt(double newValue);
	VARIANT Delete();
	BOOL GetHasMajorGridlines();
	void SetHasMajorGridlines(BOOL bNewValue);
	BOOL GetHasMinorGridlines();
	void SetHasMinorGridlines(BOOL bNewValue);
	BOOL GetHasTitle();
	void SetHasTitle(BOOL bNewValue);
	LPDISPATCH GetMajorGridlines();
	long GetMajorTickMark();
	void SetMajorTickMark(long nNewValue);
	double GetMajorUnit();
	void SetMajorUnit(double newValue);
	BOOL GetMajorUnitIsAuto();
	void SetMajorUnitIsAuto(BOOL bNewValue);
	double GetMaximumScale();
	void SetMaximumScale(double newValue);
	BOOL GetMaximumScaleIsAuto();
	void SetMaximumScaleIsAuto(BOOL bNewValue);
	double GetMinimumScale();
	void SetMinimumScale(double newValue);
	BOOL GetMinimumScaleIsAuto();
	void SetMinimumScaleIsAuto(BOOL bNewValue);
	LPDISPATCH GetMinorGridlines();
	long GetMinorTickMark();
	void SetMinorTickMark(long nNewValue);
	double GetMinorUnit();
	void SetMinorUnit(double newValue);
	BOOL GetMinorUnitIsAuto();
	void SetMinorUnitIsAuto(BOOL bNewValue);
	BOOL GetReversePlotOrder();
	void SetReversePlotOrder(BOOL bNewValue);
	long GetScaleType();
	void SetScaleType(long nNewValue);
	VARIANT Select();
	long GetTickLabelPosition();
	void SetTickLabelPosition(long nNewValue);
	LPDISPATCH GetTickLabels();
	long GetTickLabelSpacing();
	void SetTickLabelSpacing(long nNewValue);
	long GetTickMarkSpacing();
	void SetTickMarkSpacing(long nNewValue);
	long GetType();
	void SetType(long nNewValue);
	long GetBaseUnit();
	void SetBaseUnit(long nNewValue);
	BOOL GetBaseUnitIsAuto();
	void SetBaseUnitIsAuto(BOOL bNewValue);
	long GetMajorUnitScale();
	void SetMajorUnitScale(long nNewValue);
	long GetMinorUnitScale();
	void SetMinorUnitScale(long nNewValue);
	long GetCategoryType();
	void SetCategoryType(long nNewValue);
	double GetLeft();
	double GetTop();
	double GetWidth();
	double GetHeight();
	long GetDisplayUnit();
	void SetDisplayUnit(long nNewValue);
	double GetDisplayUnitCustom();
	void SetDisplayUnitCustom(double newValue);
	BOOL GetHasDisplayUnitLabel();
	void SetHasDisplayUnitLabel(BOOL bNewValue);
	LPDISPATCH GetDisplayUnitLabel();
};
/////////////////////////////////////////////////////////////////////////////
// ChartTitle wrapper class

class __MY_EXT_CLASS__ Xl_ChartTitle : public COleDispatchDriver
{
public:
	Xl_ChartTitle() {}		// Calls COleDispatchDriver default constructor
	Xl_ChartTitle(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ChartTitle(const Xl_ChartTitle& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
	LPDISPATCH GetBorder();
	VARIANT Delete();
	LPDISPATCH GetInterior();
	LPDISPATCH GetFill();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	LPDISPATCH GetFont();
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	double GetLeft();
	void SetLeft(double newValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	double GetTop();
	void SetTop(double newValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// AxisTitle wrapper class

class __MY_EXT_CLASS__ Xl_AxisTitle : public COleDispatchDriver
{
public:
	Xl_AxisTitle() {}		// Calls COleDispatchDriver default constructor
	Xl_AxisTitle(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_AxisTitle(const Xl_AxisTitle& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
	LPDISPATCH GetBorder();
	VARIANT Delete();
	LPDISPATCH GetInterior();
	LPDISPATCH GetFill();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	LPDISPATCH GetFont();
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	double GetLeft();
	void SetLeft(double newValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	double GetTop();
	void SetTop(double newValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// ChartGroup wrapper class

class __MY_EXT_CLASS__ Xl_ChartGroup : public COleDispatchDriver
{
public:
	Xl_ChartGroup() {}		// Calls COleDispatchDriver default constructor
	Xl_ChartGroup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ChartGroup(const Xl_ChartGroup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetAxisGroup();
	void SetAxisGroup(long nNewValue);
	long GetDoughnutHoleSize();
	void SetDoughnutHoleSize(long nNewValue);
	LPDISPATCH GetDownBars();
	LPDISPATCH GetDropLines();
	long GetFirstSliceAngle();
	void SetFirstSliceAngle(long nNewValue);
	long GetGapWidth();
	void SetGapWidth(long nNewValue);
	BOOL GetHasDropLines();
	void SetHasDropLines(BOOL bNewValue);
	BOOL GetHasHiLoLines();
	void SetHasHiLoLines(BOOL bNewValue);
	BOOL GetHasRadarAxisLabels();
	void SetHasRadarAxisLabels(BOOL bNewValue);
	BOOL GetHasSeriesLines();
	void SetHasSeriesLines(BOOL bNewValue);
	BOOL GetHasUpDownBars();
	void SetHasUpDownBars(BOOL bNewValue);
	LPDISPATCH GetHiLoLines();
	long GetIndex();
	long GetOverlap();
	void SetOverlap(long nNewValue);
	LPDISPATCH GetRadarAxisLabels();
	LPDISPATCH SeriesCollection(const VARIANT& Index);
	LPDISPATCH GetSeriesLines();
	LPDISPATCH GetUpBars();
	BOOL GetVaryByCategories();
	void SetVaryByCategories(BOOL bNewValue);
	long GetSizeRepresents();
	void SetSizeRepresents(long nNewValue);
	long GetBubbleScale();
	void SetBubbleScale(long nNewValue);
	BOOL GetShowNegativeBubbles();
	void SetShowNegativeBubbles(BOOL bNewValue);
	long GetSplitType();
	void SetSplitType(long nNewValue);
	VARIANT GetSplitValue();
	void SetSplitValue(const VARIANT& newValue);
	long GetSecondPlotSize();
	void SetSecondPlotSize(long nNewValue);
	BOOL GetHas3DShading();
	void SetHas3DShading(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ChartGroups wrapper class

class __MY_EXT_CLASS__ Xl_ChartGroups : public COleDispatchDriver
{
public:
	Xl_ChartGroups() {}		// Calls COleDispatchDriver default constructor
	Xl_ChartGroups(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ChartGroups(const Xl_ChartGroups& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Axes wrapper class

class __MY_EXT_CLASS__ Xl_Axes : public COleDispatchDriver
{
public:
	Xl_Axes() {}		// Calls COleDispatchDriver default constructor
	Xl_Axes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Axes(const Xl_Axes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(long Type, long AxisGroup);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Points wrapper class

class __MY_EXT_CLASS__ Xl_Points : public COleDispatchDriver
{
public:
	Xl_Points() {}		// Calls COleDispatchDriver default constructor
	Xl_Points(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Points(const Xl_Points& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(long Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Point wrapper class

class __MY_EXT_CLASS__ Xl_Point : public COleDispatchDriver
{
public:
	Xl_Point() {}		// Calls COleDispatchDriver default constructor
	Xl_Point(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Point(const Xl_Point& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT ApplyDataLabels(long Type, const VARIANT& LegendKey, const VARIANT& AutoText);
	LPDISPATCH GetBorder();
	VARIANT ClearFormats();
	VARIANT Copy();
	LPDISPATCH GetDataLabel();
	VARIANT Delete();
	long GetExplosion();
	void SetExplosion(long nNewValue);
	BOOL GetHasDataLabel();
	void SetHasDataLabel(BOOL bNewValue);
	LPDISPATCH GetInterior();
	BOOL GetInvertIfNegative();
	void SetInvertIfNegative(BOOL bNewValue);
	long GetMarkerBackgroundColor();
	void SetMarkerBackgroundColor(long nNewValue);
	long GetMarkerBackgroundColorIndex();
	void SetMarkerBackgroundColorIndex(long nNewValue);
	long GetMarkerForegroundColor();
	void SetMarkerForegroundColor(long nNewValue);
	long GetMarkerForegroundColorIndex();
	void SetMarkerForegroundColorIndex(long nNewValue);
	long GetMarkerSize();
	void SetMarkerSize(long nNewValue);
	long GetMarkerStyle();
	void SetMarkerStyle(long nNewValue);
	VARIANT Paste();
	long GetPictureType();
	void SetPictureType(long nNewValue);
	long GetPictureUnit();
	void SetPictureUnit(long nNewValue);
	VARIANT Select();
	BOOL GetApplyPictToSides();
	void SetApplyPictToSides(BOOL bNewValue);
	BOOL GetApplyPictToFront();
	void SetApplyPictToFront(BOOL bNewValue);
	BOOL GetApplyPictToEnd();
	void SetApplyPictToEnd(BOOL bNewValue);
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	BOOL GetSecondaryPlot();
	void SetSecondaryPlot(BOOL bNewValue);
	LPDISPATCH GetFill();
};
/////////////////////////////////////////////////////////////////////////////
// Series wrapper class

class __MY_EXT_CLASS__ Xl_Series : public COleDispatchDriver
{
public:
	Xl_Series() {}		// Calls COleDispatchDriver default constructor
	Xl_Series(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Series(const Xl_Series& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT ApplyDataLabels(long Type, const VARIANT& LegendKey, const VARIANT& AutoText, const VARIANT& HasLeaderLines);
	long GetAxisGroup();
	void SetAxisGroup(long nNewValue);
	LPDISPATCH GetBorder();
	VARIANT ClearFormats();
	VARIANT Copy();
	LPDISPATCH DataLabels(const VARIANT& Index);
	VARIANT Delete();
	VARIANT ErrorBar(long Direction, long Include, long Type, const VARIANT& Amount, const VARIANT& MinusValues);
	LPDISPATCH GetErrorBars();
	long GetExplosion();
	void SetExplosion(long nNewValue);
	CString GetFormula();
	void SetFormula(LPCTSTR lpszNewValue);
	CString GetFormulaLocal();
	void SetFormulaLocal(LPCTSTR lpszNewValue);
	CString GetFormulaR1C1();
	void SetFormulaR1C1(LPCTSTR lpszNewValue);
	CString GetFormulaR1C1Local();
	void SetFormulaR1C1Local(LPCTSTR lpszNewValue);
	BOOL GetHasDataLabels();
	void SetHasDataLabels(BOOL bNewValue);
	BOOL GetHasErrorBars();
	void SetHasErrorBars(BOOL bNewValue);
	LPDISPATCH GetInterior();
	LPDISPATCH GetFill();
	BOOL GetInvertIfNegative();
	void SetInvertIfNegative(BOOL bNewValue);
	long GetMarkerBackgroundColor();
	void SetMarkerBackgroundColor(long nNewValue);
	long GetMarkerBackgroundColorIndex();
	void SetMarkerBackgroundColorIndex(long nNewValue);
	long GetMarkerForegroundColor();
	void SetMarkerForegroundColor(long nNewValue);
	long GetMarkerForegroundColorIndex();
	void SetMarkerForegroundColorIndex(long nNewValue);
	long GetMarkerSize();
	void SetMarkerSize(long nNewValue);
	long GetMarkerStyle();
	void SetMarkerStyle(long nNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	VARIANT Paste();
	long GetPictureType();
	void SetPictureType(long nNewValue);
	long GetPictureUnit();
	void SetPictureUnit(long nNewValue);
	long GetPlotOrder();
	void SetPlotOrder(long nNewValue);
	LPDISPATCH Points(const VARIANT& Index);
	VARIANT Select();
	BOOL GetSmooth();
	void SetSmooth(BOOL bNewValue);
	LPDISPATCH Trendlines(const VARIANT& Index);
	long GetType();
	void SetType(long nNewValue);
	long GetChartType();
	void SetChartType(long nNewValue);
	void ApplyCustomType(long ChartType);
	VARIANT GetValues();
	void SetValues(const VARIANT& newValue);
	VARIANT GetXValues();
	void SetXValues(const VARIANT& newValue);
	VARIANT GetBubbleSizes();
	void SetBubbleSizes(const VARIANT& newValue);
	long GetBarShape();
	void SetBarShape(long nNewValue);
	BOOL GetApplyPictToSides();
	void SetApplyPictToSides(BOOL bNewValue);
	BOOL GetApplyPictToFront();
	void SetApplyPictToFront(BOOL bNewValue);
	BOOL GetApplyPictToEnd();
	void SetApplyPictToEnd(BOOL bNewValue);
	BOOL GetHas3DEffect();
	void SetHas3DEffect(BOOL bNewValue);
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	BOOL GetHasLeaderLines();
	void SetHasLeaderLines(BOOL bNewValue);
	LPDISPATCH GetLeaderLines();
};
/////////////////////////////////////////////////////////////////////////////
// SeriesCollection wrapper class

class __MY_EXT_CLASS__ Xl_SeriesCollection : public COleDispatchDriver
{
public:
	Xl_SeriesCollection() {}		// Calls COleDispatchDriver default constructor
	Xl_SeriesCollection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_SeriesCollection(const Xl_SeriesCollection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(const VARIANT& Source, long Rowcol, const VARIANT& SeriesLabels, const VARIANT& CategoryLabels, const VARIANT& Replace);
	long GetCount();
	VARIANT Extend(const VARIANT& Source, const VARIANT& Rowcol, const VARIANT& CategoryLabels);
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
	VARIANT Paste(long Rowcol, const VARIANT& SeriesLabels, const VARIANT& CategoryLabels, const VARIANT& Replace, const VARIANT& NewSeries);
	LPDISPATCH NewSeries();
};
/////////////////////////////////////////////////////////////////////////////
// DataLabel wrapper class

class __MY_EXT_CLASS__ Xl_DataLabel : public COleDispatchDriver
{
public:
	Xl_DataLabel() {}		// Calls COleDispatchDriver default constructor
	Xl_DataLabel(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_DataLabel(const Xl_DataLabel& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
	LPDISPATCH GetBorder();
	VARIANT Delete();
	LPDISPATCH GetInterior();
	LPDISPATCH GetFill();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	LPDISPATCH GetFont();
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	double GetLeft();
	void SetLeft(double newValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	double GetTop();
	void SetTop(double newValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
	BOOL GetAutoText();
	void SetAutoText(BOOL bNewValue);
	CString GetNumberFormat();
	void SetNumberFormat(LPCTSTR lpszNewValue);
	BOOL GetNumberFormatLinked();
	void SetNumberFormatLinked(BOOL bNewValue);
	VARIANT GetNumberFormatLocal();
	void SetNumberFormatLocal(const VARIANT& newValue);
	BOOL GetShowLegendKey();
	void SetShowLegendKey(BOOL bNewValue);
	VARIANT GetType();
	void SetType(const VARIANT& newValue);
	long GetPosition();
	void SetPosition(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// DataLabels wrapper class

class __MY_EXT_CLASS__ Xl_DataLabels : public COleDispatchDriver
{
public:
	Xl_DataLabels() {}		// Calls COleDispatchDriver default constructor
	Xl_DataLabels(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_DataLabels(const Xl_DataLabels& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
	LPDISPATCH GetBorder();
	VARIANT Delete();
	LPDISPATCH GetInterior();
	LPDISPATCH GetFill();
	LPDISPATCH GetFont();
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
	BOOL GetAutoText();
	void SetAutoText(BOOL bNewValue);
	CString GetNumberFormat();
	void SetNumberFormat(LPCTSTR lpszNewValue);
	BOOL GetNumberFormatLinked();
	void SetNumberFormatLinked(BOOL bNewValue);
	VARIANT GetNumberFormatLocal();
	void SetNumberFormatLocal(const VARIANT& newValue);
	BOOL GetShowLegendKey();
	void SetShowLegendKey(BOOL bNewValue);
	VARIANT GetType();
	void SetType(const VARIANT& newValue);
	long GetPosition();
	void SetPosition(long nNewValue);
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// LegendEntry wrapper class

class __MY_EXT_CLASS__ Xl_LegendEntry : public COleDispatchDriver
{
public:
	Xl_LegendEntry() {}		// Calls COleDispatchDriver default constructor
	Xl_LegendEntry(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_LegendEntry(const Xl_LegendEntry& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT Delete();
	LPDISPATCH GetFont();
	long GetIndex();
	LPDISPATCH GetLegendKey();
	VARIANT Select();
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
	double GetLeft();
	double GetTop();
	double GetWidth();
	double GetHeight();
};
/////////////////////////////////////////////////////////////////////////////
// LegendEntries wrapper class

class __MY_EXT_CLASS__ Xl_LegendEntries : public COleDispatchDriver
{
public:
	Xl_LegendEntries() {}		// Calls COleDispatchDriver default constructor
	Xl_LegendEntries(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_LegendEntries(const Xl_LegendEntries& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// LegendKey wrapper class

class __MY_EXT_CLASS__ Xl_LegendKey : public COleDispatchDriver
{
public:
	Xl_LegendKey() {}		// Calls COleDispatchDriver default constructor
	Xl_LegendKey(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_LegendKey(const Xl_LegendKey& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBorder();
	VARIANT ClearFormats();
	VARIANT Delete();
	LPDISPATCH GetInterior();
	LPDISPATCH GetFill();
	BOOL GetInvertIfNegative();
	void SetInvertIfNegative(BOOL bNewValue);
	long GetMarkerBackgroundColor();
	void SetMarkerBackgroundColor(long nNewValue);
	long GetMarkerBackgroundColorIndex();
	void SetMarkerBackgroundColorIndex(long nNewValue);
	long GetMarkerForegroundColor();
	void SetMarkerForegroundColor(long nNewValue);
	long GetMarkerForegroundColorIndex();
	void SetMarkerForegroundColorIndex(long nNewValue);
	long GetMarkerSize();
	void SetMarkerSize(long nNewValue);
	long GetMarkerStyle();
	void SetMarkerStyle(long nNewValue);
	long GetPictureType();
	void SetPictureType(long nNewValue);
	long GetPictureUnit();
	void SetPictureUnit(long nNewValue);
	VARIANT Select();
	BOOL GetSmooth();
	void SetSmooth(BOOL bNewValue);
	double GetLeft();
	double GetTop();
	double GetWidth();
	double GetHeight();
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Trendlines wrapper class

class __MY_EXT_CLASS__ Xl_Trendlines : public COleDispatchDriver
{
public:
	Xl_Trendlines() {}		// Calls COleDispatchDriver default constructor
	Xl_Trendlines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Trendlines(const Xl_Trendlines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(long Type, const VARIANT& Order, const VARIANT& Period, const VARIANT& Forward, const VARIANT& Backward, const VARIANT& Intercept, const VARIANT& DisplayEquation, const VARIANT& DisplayRSquared, const VARIANT& Name);
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Trendline wrapper class

class __MY_EXT_CLASS__ Xl_Trendline : public COleDispatchDriver
{
public:
	Xl_Trendline() {}		// Calls COleDispatchDriver default constructor
	Xl_Trendline(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Trendline(const Xl_Trendline& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetBackward();
	void SetBackward(long nNewValue);
	LPDISPATCH GetBorder();
	VARIANT ClearFormats();
	LPDISPATCH GetDataLabel();
	VARIANT Delete();
	BOOL GetDisplayEquation();
	void SetDisplayEquation(BOOL bNewValue);
	BOOL GetDisplayRSquared();
	void SetDisplayRSquared(BOOL bNewValue);
	long GetForward();
	void SetForward(long nNewValue);
	long GetIndex();
	double GetIntercept();
	void SetIntercept(double newValue);
	BOOL GetInterceptIsAuto();
	void SetInterceptIsAuto(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	BOOL GetNameIsAuto();
	void SetNameIsAuto(BOOL bNewValue);
	long GetOrder();
	void SetOrder(long nNewValue);
	long GetPeriod();
	void SetPeriod(long nNewValue);
	VARIANT Select();
	long GetType();
	void SetType(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Corners wrapper class

class __MY_EXT_CLASS__ Xl_Corners : public COleDispatchDriver
{
public:
	Xl_Corners() {}		// Calls COleDispatchDriver default constructor
	Xl_Corners(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Corners(const Xl_Corners& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
};
/////////////////////////////////////////////////////////////////////////////
// SeriesLines wrapper class

class __MY_EXT_CLASS__ Xl_SeriesLines : public COleDispatchDriver
{
public:
	Xl_SeriesLines() {}		// Calls COleDispatchDriver default constructor
	Xl_SeriesLines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_SeriesLines(const Xl_SeriesLines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
	LPDISPATCH GetBorder();
	VARIANT Delete();
};
/////////////////////////////////////////////////////////////////////////////
// HiLoLines wrapper class

class __MY_EXT_CLASS__ Xl_HiLoLines : public COleDispatchDriver
{
public:
	Xl_HiLoLines() {}		// Calls COleDispatchDriver default constructor
	Xl_HiLoLines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_HiLoLines(const Xl_HiLoLines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
	LPDISPATCH GetBorder();
	VARIANT Delete();
};
/////////////////////////////////////////////////////////////////////////////
// Gridlines wrapper class

class __MY_EXT_CLASS__ Xl_Gridlines : public COleDispatchDriver
{
public:
	Xl_Gridlines() {}		// Calls COleDispatchDriver default constructor
	Xl_Gridlines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Gridlines(const Xl_Gridlines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
	LPDISPATCH GetBorder();
	VARIANT Delete();
};
/////////////////////////////////////////////////////////////////////////////
// DropLines wrapper class

class __MY_EXT_CLASS__ Xl_DropLines : public COleDispatchDriver
{
public:
	Xl_DropLines() {}		// Calls COleDispatchDriver default constructor
	Xl_DropLines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_DropLines(const Xl_DropLines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
	LPDISPATCH GetBorder();
	VARIANT Delete();
};
/////////////////////////////////////////////////////////////////////////////
// LeaderLines wrapper class

class __MY_EXT_CLASS__ Xl_LeaderLines : public COleDispatchDriver
{
public:
	Xl_LeaderLines() {}		// Calls COleDispatchDriver default constructor
	Xl_LeaderLines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_LeaderLines(const Xl_LeaderLines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetBorder();
	void Delete();
	void Select();
};
/////////////////////////////////////////////////////////////////////////////
// UpBars wrapper class

class __MY_EXT_CLASS__ Xl_UpBars : public COleDispatchDriver
{
public:
	Xl_UpBars() {}		// Calls COleDispatchDriver default constructor
	Xl_UpBars(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_UpBars(const Xl_UpBars& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
	LPDISPATCH GetBorder();
	VARIANT Delete();
	LPDISPATCH GetInterior();
	LPDISPATCH GetFill();
};
/////////////////////////////////////////////////////////////////////////////
// DownBars wrapper class

class __MY_EXT_CLASS__ Xl_DownBars : public COleDispatchDriver
{
public:
	Xl_DownBars() {}		// Calls COleDispatchDriver default constructor
	Xl_DownBars(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_DownBars(const Xl_DownBars& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
	LPDISPATCH GetBorder();
	VARIANT Delete();
	LPDISPATCH GetInterior();
	LPDISPATCH GetFill();
};
/////////////////////////////////////////////////////////////////////////////
// Floor wrapper class

class __MY_EXT_CLASS__ Xl_Floor : public COleDispatchDriver
{
public:
	Xl_Floor() {}		// Calls COleDispatchDriver default constructor
	Xl_Floor(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Floor(const Xl_Floor& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
	LPDISPATCH GetBorder();
	VARIANT ClearFormats();
	LPDISPATCH GetInterior();
	LPDISPATCH GetFill();
	VARIANT GetPictureType();
	void SetPictureType(const VARIANT& newValue);
	void Paste();
};
/////////////////////////////////////////////////////////////////////////////
// Walls wrapper class

class __MY_EXT_CLASS__ Xl_Walls : public COleDispatchDriver
{
public:
	Xl_Walls() {}		// Calls COleDispatchDriver default constructor
	Xl_Walls(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Walls(const Xl_Walls& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
	LPDISPATCH GetBorder();
	VARIANT ClearFormats();
	LPDISPATCH GetInterior();
	LPDISPATCH GetFill();
	VARIANT GetPictureType();
	void SetPictureType(const VARIANT& newValue);
	void Paste();
	VARIANT GetPictureUnit();
	void SetPictureUnit(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// TickLabels wrapper class

class __MY_EXT_CLASS__ Xl_TickLabels : public COleDispatchDriver
{
public:
	Xl_TickLabels() {}		// Calls COleDispatchDriver default constructor
	Xl_TickLabels(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_TickLabels(const Xl_TickLabels& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	VARIANT Delete();
	LPDISPATCH GetFont();
	CString GetName();
	CString GetNumberFormat();
	void SetNumberFormat(LPCTSTR lpszNewValue);
	BOOL GetNumberFormatLinked();
	void SetNumberFormatLinked(BOOL bNewValue);
	VARIANT GetNumberFormatLocal();
	void SetNumberFormatLocal(const VARIANT& newValue);
	long GetOrientation();
	void SetOrientation(long nNewValue);
	VARIANT Select();
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
	long GetDepth();
	long GetOffset();
	void SetOffset(long nNewValue);
	long GetAlignment();
	void SetAlignment(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// PlotArea wrapper class

class __MY_EXT_CLASS__ Xl_PlotArea : public COleDispatchDriver
{
public:
	Xl_PlotArea() {}		// Calls COleDispatchDriver default constructor
	Xl_PlotArea(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_PlotArea(const Xl_PlotArea& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
	LPDISPATCH GetBorder();
	VARIANT ClearFormats();
	double GetHeight();
	void SetHeight(double newValue);
	LPDISPATCH GetInterior();
	LPDISPATCH GetFill();
	double GetLeft();
	void SetLeft(double newValue);
	double GetTop();
	void SetTop(double newValue);
	double GetWidth();
	void SetWidth(double newValue);
	double GetInsideLeft();
	double GetInsideTop();
	double GetInsideWidth();
	double GetInsideHeight();
};
/////////////////////////////////////////////////////////////////////////////
// ChartArea wrapper class

class __MY_EXT_CLASS__ Xl_ChartArea : public COleDispatchDriver
{
public:
	Xl_ChartArea() {}		// Calls COleDispatchDriver default constructor
	Xl_ChartArea(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ChartArea(const Xl_ChartArea& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
	LPDISPATCH GetBorder();
	VARIANT Clear();
	VARIANT ClearContents();
	VARIANT Copy();
	LPDISPATCH GetFont();
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	VARIANT ClearFormats();
	double GetHeight();
	void SetHeight(double newValue);
	LPDISPATCH GetInterior();
	LPDISPATCH GetFill();
	double GetLeft();
	void SetLeft(double newValue);
	double GetTop();
	void SetTop(double newValue);
	double GetWidth();
	void SetWidth(double newValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// Legend wrapper class

class __MY_EXT_CLASS__ Xl_Legend : public COleDispatchDriver
{
public:
	Xl_Legend() {}		// Calls COleDispatchDriver default constructor
	Xl_Legend(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Legend(const Xl_Legend& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
	LPDISPATCH GetBorder();
	VARIANT Delete();
	LPDISPATCH GetFont();
	LPDISPATCH LegendEntries(const VARIANT& Index);
	long GetPosition();
	void SetPosition(long nNewValue);
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	VARIANT Clear();
	double GetHeight();
	void SetHeight(double newValue);
	LPDISPATCH GetInterior();
	LPDISPATCH GetFill();
	double GetLeft();
	void SetLeft(double newValue);
	double GetTop();
	void SetTop(double newValue);
	double GetWidth();
	void SetWidth(double newValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// ErrorBars wrapper class

class __MY_EXT_CLASS__ Xl_ErrorBars : public COleDispatchDriver
{
public:
	Xl_ErrorBars() {}		// Calls COleDispatchDriver default constructor
	Xl_ErrorBars(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ErrorBars(const Xl_ErrorBars& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
	LPDISPATCH GetBorder();
	VARIANT Delete();
	VARIANT ClearFormats();
	long GetEndStyle();
	void SetEndStyle(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// DataTable wrapper class

class __MY_EXT_CLASS__ Xl_DataTable : public COleDispatchDriver
{
public:
	Xl_DataTable() {}		// Calls COleDispatchDriver default constructor
	Xl_DataTable(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_DataTable(const Xl_DataTable& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	BOOL GetShowLegendKey();
	void SetShowLegendKey(BOOL bNewValue);
	BOOL GetHasBorderHorizontal();
	void SetHasBorderHorizontal(BOOL bNewValue);
	BOOL GetHasBorderVertical();
	void SetHasBorderVertical(BOOL bNewValue);
	BOOL GetHasBorderOutline();
	void SetHasBorderOutline(BOOL bNewValue);
	LPDISPATCH GetBorder();
	LPDISPATCH GetFont();
	void Select();
	void Delete();
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// Phonetic wrapper class

class __MY_EXT_CLASS__ Xl_Phonetic : public COleDispatchDriver
{
public:
	Xl_Phonetic() {}		// Calls COleDispatchDriver default constructor
	Xl_Phonetic(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Phonetic(const Xl_Phonetic& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	long GetCharacterType();
	void SetCharacterType(long nNewValue);
	long GetAlignment();
	void SetAlignment(long nNewValue);
	LPDISPATCH GetFont();
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Shape wrapper class

class __MY_EXT_CLASS__ Xl_Shape : public COleDispatchDriver
{
public:
	Xl_Shape() {}		// Calls COleDispatchDriver default constructor
	Xl_Shape(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Shape(const Xl_Shape& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void Apply();
	void Delete();
	LPDISPATCH Duplicate();
	void Flip(long FlipCmd);
	void IncrementLeft(float Increment);
	void IncrementRotation(float Increment);
	void IncrementTop(float Increment);
	void PickUp();
	void RerouteConnections();
	void ScaleHeight(float Factor, long RelativeToOriginalSize, const VARIANT& Scale);
	void ScaleWidth(float Factor, long RelativeToOriginalSize, const VARIANT& Scale);
	void Select(const VARIANT& Replace);
	void SetShapesDefaultProperties();
	LPDISPATCH Ungroup();
	void ZOrder(long ZOrderCmd);
	LPDISPATCH GetAdjustments();
	LPDISPATCH GetTextFrame();
	long GetAutoShapeType();
	void SetAutoShapeType(long nNewValue);
	LPDISPATCH GetCallout();
	long GetConnectionSiteCount();
	long GetConnector();
	LPDISPATCH GetConnectorFormat();
	LPDISPATCH GetFill();
	LPDISPATCH GetGroupItems();
	float GetHeight();
	void SetHeight(float newValue);
	long GetHorizontalFlip();
	float GetLeft();
	void SetLeft(float newValue);
	LPDISPATCH GetLine();
	long GetLockAspectRatio();
	void SetLockAspectRatio(long nNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	LPDISPATCH GetNodes();
	float GetRotation();
	void SetRotation(float newValue);
	LPDISPATCH GetPictureFormat();
	LPDISPATCH GetShadow();
	LPDISPATCH GetTextEffect();
	LPDISPATCH GetThreeD();
	float GetTop();
	void SetTop(float newValue);
	long GetType();
	long GetVerticalFlip();
	VARIANT GetVertices();
	long GetVisible();
	void SetVisible(long nNewValue);
	float GetWidth();
	void SetWidth(float newValue);
	long GetZOrderPosition();
	LPDISPATCH GetHyperlink();
	long GetBlackWhiteMode();
	void SetBlackWhiteMode(long nNewValue);
	CString GetOnAction();
	void SetOnAction(LPCTSTR lpszNewValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	LPDISPATCH GetTopLeftCell();
	LPDISPATCH GetBottomRightCell();
	long GetPlacement();
	void SetPlacement(long nNewValue);
	void Copy();
	void Cut();
	void CopyPicture(const VARIANT& Appearance, const VARIANT& Format);
	LPDISPATCH GetControlFormat();
	LPDISPATCH GetLinkFormat();
	LPDISPATCH GetOLEFormat();
	long GetFormControlType();
	CString GetAlternativeText();
	void SetAlternativeText(LPCTSTR lpszNewValue);
	LPDISPATCH GetScript();
};
/////////////////////////////////////////////////////////////////////////////
// Shapes wrapper class

class __MY_EXT_CLASS__ Xl_Shapes : public COleDispatchDriver
{
public:
	Xl_Shapes() {}		// Calls COleDispatchDriver default constructor
	Xl_Shapes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Shapes(const Xl_Shapes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH _Default(const VARIANT& Index);
	LPUNKNOWN Get_NewEnum();
	LPDISPATCH AddCallout(long Type, float Left, float Top, float Width, float Height);
	LPDISPATCH AddConnector(long Type, float BeginX, float BeginY, float EndX, float EndY);
	LPDISPATCH AddCurve(const VARIANT& SafeArrayOfPoints);
	LPDISPATCH AddLabel(long Orientation, float Left, float Top, float Width, float Height);
	LPDISPATCH AddLine(float BeginX, float BeginY, float EndX, float EndY);
	LPDISPATCH AddPicture(LPCTSTR Filename, long LinkToFile, long SaveWithDocument, float Left, float Top, float Width, float Height);
	LPDISPATCH AddPolyline(const VARIANT& SafeArrayOfPoints);
	LPDISPATCH AddShape(long Type, float Left, float Top, float Width, float Height);
	LPDISPATCH AddTextEffect(long PresetTextEffect, LPCTSTR Text, LPCTSTR FontName, float FontSize, long FontBold, long FontItalic, float Left, float Top);
	LPDISPATCH AddTextbox(long Orientation, float Left, float Top, float Width, float Height);
	LPDISPATCH BuildFreeform(long EditingType, float X1, float Y1);
	LPDISPATCH GetRange(const VARIANT& Index);
	void SelectAll();
	LPDISPATCH AddFormControl(long Type, long Left, long Top, long Width, long Height);
	LPDISPATCH AddOLEObject(const VARIANT& ClassType, const VARIANT& Filename, const VARIANT& Link, const VARIANT& DisplayAsIcon, const VARIANT& IconFileName, const VARIANT& IconIndex, const VARIANT& IconLabel, const VARIANT& Left, 
		const VARIANT& Top, const VARIANT& Width, const VARIANT& Height);
};
/////////////////////////////////////////////////////////////////////////////
// ShapeRange wrapper class

class __MY_EXT_CLASS__ Xl_ShapeRange : public COleDispatchDriver
{
public:
	Xl_ShapeRange() {}		// Calls COleDispatchDriver default constructor
	Xl_ShapeRange(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ShapeRange(const Xl_ShapeRange& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH _Default(const VARIANT& Index);
	LPUNKNOWN Get_NewEnum();
	void Align(long AlignCmd, long RelativeTo);
	void Apply();
	void Delete();
	void Distribute(long DistributeCmd, long RelativeTo);
	LPDISPATCH Duplicate();
	void Flip(long FlipCmd);
	void IncrementLeft(float Increment);
	void IncrementRotation(float Increment);
	void IncrementTop(float Increment);
	LPDISPATCH Group();
	void PickUp();
	void RerouteConnections();
	LPDISPATCH Regroup();
	void ScaleHeight(float Factor, long RelativeToOriginalSize, const VARIANT& Scale);
	void ScaleWidth(float Factor, long RelativeToOriginalSize, const VARIANT& Scale);
	void Select(const VARIANT& Replace);
	void SetShapesDefaultProperties();
	LPDISPATCH Ungroup();
	void ZOrder(long ZOrderCmd);
	LPDISPATCH GetAdjustments();
	LPDISPATCH GetTextFrame();
	long GetAutoShapeType();
	void SetAutoShapeType(long nNewValue);
	LPDISPATCH GetCallout();
	long GetConnectionSiteCount();
	long GetConnector();
	LPDISPATCH GetConnectorFormat();
	LPDISPATCH GetFill();
	LPDISPATCH GetGroupItems();
	float GetHeight();
	void SetHeight(float newValue);
	long GetHorizontalFlip();
	float GetLeft();
	void SetLeft(float newValue);
	LPDISPATCH GetLine();
	long GetLockAspectRatio();
	void SetLockAspectRatio(long nNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	LPDISPATCH GetNodes();
	float GetRotation();
	void SetRotation(float newValue);
	LPDISPATCH GetPictureFormat();
	LPDISPATCH GetShadow();
	LPDISPATCH GetTextEffect();
	LPDISPATCH GetThreeD();
	float GetTop();
	void SetTop(float newValue);
	long GetType();
	long GetVerticalFlip();
	VARIANT GetVertices();
	long GetVisible();
	void SetVisible(long nNewValue);
	float GetWidth();
	void SetWidth(float newValue);
	long GetZOrderPosition();
	long GetBlackWhiteMode();
	void SetBlackWhiteMode(long nNewValue);
	CString GetAlternativeText();
	void SetAlternativeText(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// GroupShapes wrapper class

class __MY_EXT_CLASS__ Xl_GroupShapes : public COleDispatchDriver
{
public:
	Xl_GroupShapes() {}		// Calls COleDispatchDriver default constructor
	Xl_GroupShapes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_GroupShapes(const Xl_GroupShapes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(const VARIANT& Index);
	LPDISPATCH _Default(const VARIANT& Index);
	LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// TextFrame wrapper class

class __MY_EXT_CLASS__ Xl_TextFrame : public COleDispatchDriver
{
public:
	Xl_TextFrame() {}		// Calls COleDispatchDriver default constructor
	Xl_TextFrame(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_TextFrame(const Xl_TextFrame& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	float GetMarginBottom();
	void SetMarginBottom(float newValue);
	float GetMarginLeft();
	void SetMarginLeft(float newValue);
	float GetMarginRight();
	void SetMarginRight(float newValue);
	float GetMarginTop();
	void SetMarginTop(float newValue);
	long GetOrientation();
	void SetOrientation(long nNewValue);
	LPDISPATCH Characters(const VARIANT& Start, const VARIANT& Length);
	long GetHorizontalAlignment();
	void SetHorizontalAlignment(long nNewValue);
	long GetVerticalAlignment();
	void SetVerticalAlignment(long nNewValue);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL bNewValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	BOOL GetAutoMargins();
	void SetAutoMargins(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ConnectorFormat wrapper class

class __MY_EXT_CLASS__ Xl_ConnectorFormat : public COleDispatchDriver
{
public:
	Xl_ConnectorFormat() {}		// Calls COleDispatchDriver default constructor
	Xl_ConnectorFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ConnectorFormat(const Xl_ConnectorFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void BeginConnect(LPDISPATCH ConnectedShape, long ConnectionSite);
	void BeginDisconnect();
	void EndConnect(LPDISPATCH ConnectedShape, long ConnectionSite);
	void EndDisconnect();
	long GetBeginConnected();
	LPDISPATCH GetBeginConnectedShape();
	long GetBeginConnectionSite();
	long GetEndConnected();
	LPDISPATCH GetEndConnectedShape();
	long GetEndConnectionSite();
	long GetType();
	void SetType(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// FreeformBuilder wrapper class

class __MY_EXT_CLASS__ Xl_FreeformBuilder : public COleDispatchDriver
{
public:
	Xl_FreeformBuilder() {}		// Calls COleDispatchDriver default constructor
	Xl_FreeformBuilder(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_FreeformBuilder(const Xl_FreeformBuilder& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void AddNodes(long SegmentType, long EditingType, float X1, float Y1, const VARIANT& X2, const VARIANT& Y2, const VARIANT& X3, const VARIANT& Y3);
	LPDISPATCH ConvertToShape();
};
/////////////////////////////////////////////////////////////////////////////
// ControlFormat wrapper class

class __MY_EXT_CLASS__ Xl_ControlFormat : public COleDispatchDriver
{
public:
	Xl_ControlFormat() {}		// Calls COleDispatchDriver default constructor
	Xl_ControlFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_ControlFormat(const Xl_ControlFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void AddItem(LPCTSTR Text, const VARIANT& Index);
	void RemoveAllItems();
	void RemoveItem(long Index, const VARIANT& Count);
	long GetDropDownLines();
	void SetDropDownLines(long nNewValue);
	BOOL GetEnabled();
	void SetEnabled(BOOL bNewValue);
	long GetLargeChange();
	void SetLargeChange(long nNewValue);
	CString GetLinkedCell();
	void SetLinkedCell(LPCTSTR lpszNewValue);
	VARIANT List(const VARIANT& Index);
	long GetListCount();
	void SetListCount(long nNewValue);
	CString GetListFillRange();
	void SetListFillRange(LPCTSTR lpszNewValue);
	long GetListIndex();
	void SetListIndex(long nNewValue);
	BOOL GetLockedText();
	void SetLockedText(BOOL bNewValue);
	long GetMax();
	void SetMax(long nNewValue);
	long GetMin();
	void SetMin(long nNewValue);
	long GetMultiSelect();
	void SetMultiSelect(long nNewValue);
	BOOL GetPrintObject();
	void SetPrintObject(BOOL bNewValue);
	long GetSmallChange();
	void SetSmallChange(long nNewValue);
	long Get_Default();
	void Set_Default(long nNewValue);
	long GetValue();
	void SetValue(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// OLEFormat wrapper class

class __MY_EXT_CLASS__ Xl_OLEFormat : public COleDispatchDriver
{
public:
	Xl_OLEFormat() {}		// Calls COleDispatchDriver default constructor
	Xl_OLEFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_OLEFormat(const Xl_OLEFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	void Activate();
	LPDISPATCH GetObject();
	CString GetProgId();
	void Verb(const VARIANT& Verb);
};
/////////////////////////////////////////////////////////////////////////////
// LinkFormat wrapper class

class __MY_EXT_CLASS__ Xl_LinkFormat : public COleDispatchDriver
{
public:
	Xl_LinkFormat() {}		// Calls COleDispatchDriver default constructor
	Xl_LinkFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_LinkFormat(const Xl_LinkFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	BOOL GetAutoUpdate();
	void SetAutoUpdate(BOOL bNewValue);
	BOOL GetLocked();
	void SetLocked(BOOL bNewValue);
	void Update();
};
/////////////////////////////////////////////////////////////////////////////
// PublishObjects wrapper class

class __MY_EXT_CLASS__ Xl_PublishObjects : public COleDispatchDriver
{
public:
	Xl_PublishObjects() {}		// Calls COleDispatchDriver default constructor
	Xl_PublishObjects(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_PublishObjects(const Xl_PublishObjects& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH Add(long SourceType, LPCTSTR Filename, const VARIANT& Sheet, const VARIANT& Source, const VARIANT& HtmlType, const VARIANT& DivID, const VARIANT& Title);
	long GetCount();
	LPDISPATCH GetItem(const VARIANT& Index);
	LPDISPATCH Get_Default(const VARIANT& Index);
	LPUNKNOWN Get_NewEnum();
	void Delete();
	void Publish();
};
/////////////////////////////////////////////////////////////////////////////
// OLEDBError wrapper class

class __MY_EXT_CLASS__ Xl_OLEDBError : public COleDispatchDriver
{
public:
	Xl_OLEDBError() {}		// Calls COleDispatchDriver default constructor
	Xl_OLEDBError(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_OLEDBError(const Xl_OLEDBError& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetSqlState();
	CString GetErrorString();
	long GetNative();
	long GetNumber();
	long GetStage();
};
/////////////////////////////////////////////////////////////////////////////
// OLEDBErrors wrapper class

class __MY_EXT_CLASS__ Xl_OLEDBErrors : public COleDispatchDriver
{
public:
	Xl_OLEDBErrors() {}		// Calls COleDispatchDriver default constructor
	Xl_OLEDBErrors(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_OLEDBErrors(const Xl_OLEDBErrors& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	LPDISPATCH Item(long Index);
	LPDISPATCH Get_Default(long Index);
	LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Phonetics wrapper class

class __MY_EXT_CLASS__ Xl_Phonetics : public COleDispatchDriver
{
public:
	Xl_Phonetics() {}		// Calls COleDispatchDriver default constructor
	Xl_Phonetics(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_Phonetics(const Xl_Phonetics& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	long GetCount();
	long GetStart();
	long GetLength();
	BOOL GetVisible();
	void SetVisible(BOOL bNewValue);
	long GetCharacterType();
	void SetCharacterType(long nNewValue);
	long GetAlignment();
	void SetAlignment(long nNewValue);
	LPDISPATCH GetFont();
	LPDISPATCH GetItem(long Index);
	void Delete();
	void Add(long Start, long Length, LPCTSTR Text);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	LPDISPATCH Get_Default(long Index);
	LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// PivotLayout wrapper class

class __MY_EXT_CLASS__ Xl_PivotLayout : public COleDispatchDriver
{
public:
	Xl_PivotLayout() {}		// Calls COleDispatchDriver default constructor
	Xl_PivotLayout(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_PivotLayout(const Xl_PivotLayout& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	LPDISPATCH GetColumnFields(const VARIANT& Index);
	LPDISPATCH GetDataFields(const VARIANT& Index);
	LPDISPATCH GetPageFields(const VARIANT& Index);
	LPDISPATCH GetRowFields(const VARIANT& Index);
	LPDISPATCH GetHiddenFields(const VARIANT& Index);
	LPDISPATCH GetVisibleFields(const VARIANT& Index);
	LPDISPATCH GetPivotFields(const VARIANT& Index);
	LPDISPATCH GetCubeFields();
	LPDISPATCH GetPivotCache();
	LPDISPATCH GetPivotTable();
	CString GetInnerDetail();
	void SetInnerDetail(LPCTSTR lpszNewValue);
	void AddFields(const VARIANT& RowFields, const VARIANT& ColumnFields, const VARIANT& PageFields, const VARIANT& AppendField);
};
/////////////////////////////////////////////////////////////////////////////
// DisplayUnitLabel wrapper class

class __MY_EXT_CLASS__ Xl_DisplayUnitLabel : public COleDispatchDriver
{
public:
	Xl_DisplayUnitLabel() {}		// Calls COleDispatchDriver default constructor
	Xl_DisplayUnitLabel(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Xl_DisplayUnitLabel(const Xl_DisplayUnitLabel& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	long GetCreator();
	LPDISPATCH GetParent();
	CString GetName();
	VARIANT Select();
	LPDISPATCH GetBorder();
	VARIANT Delete();
	LPDISPATCH GetInterior();
	LPDISPATCH GetFill();
	CString GetCaption();
	void SetCaption(LPCTSTR lpszNewValue);
	LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
	LPDISPATCH GetFont();
	VARIANT GetHorizontalAlignment();
	void SetHorizontalAlignment(const VARIANT& newValue);
	double GetLeft();
	void SetLeft(double newValue);
	VARIANT GetOrientation();
	void SetOrientation(const VARIANT& newValue);
	BOOL GetShadow();
	void SetShadow(BOOL bNewValue);
	CString GetText();
	void SetText(LPCTSTR lpszNewValue);
	double GetTop();
	void SetTop(double newValue);
	VARIANT GetVerticalAlignment();
	void SetVerticalAlignment(const VARIANT& newValue);
	long GetReadingOrder();
	void SetReadingOrder(long nNewValue);
	VARIANT GetAutoScaleFont();
	void SetAutoScaleFont(const VARIANT& newValue);
};


#include "HeaderPost.h"

#endif // !defined(AFX_EXCEL9_H__3206071D_F617_4AEC_BA75_435F3F47EE93__INCLUDED_)
