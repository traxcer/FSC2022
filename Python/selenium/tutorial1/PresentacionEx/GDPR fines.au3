; #AutoIt3Wrapper_Au3Check_Parameters=-q -d -w 1 -w 2 -w 3 -w 4 -w 5 -w 6 -w 7
#include "wd_core.au3"
#include "wd_helper.au3"

Global $sDesiredCapabilities, $sSession, $sScriptName = "GDPR fines", $aResult_All
$_WD_DEBUG = $_WD_DEBUG_Error ; Set debug level: $_WD_DEBUG_None (0) = No logging to console, $_WD_DEBUG_Error (1) = Error logging to console, $_WD_DEBUG_Info (2) = Full logging to console (Default)
Global $hSplash = SplashTextOn($sScriptName, "Running ... please be patient!", Default, 45, 0, 0)

; (1, 2) ----------------------------------------------------------------------
; Automate the website using FireFox. Start FF from a different location
; -----------------------------------------------------------------------------
Global $sDriver = "C:\Local\WebDriver\geckodriver.exe" ; <== Please modify this statement to your environment. Must be a local drive, network drives don't work!
; Setup Firefox
_WD_Option("Driver", $sDriver)
If @error Then Exit SetError(1, @error)
_WD_Option('DriverParams', '--log trace')
_WD_Option('Port', 4444)
$sDesiredCapabilities = '{"desiredCapabilities":{"javascriptEnabled":true,"nativeEvents":true,"acceptInsecureCerts":true}}'
; (1, 2) End ------------------------------------------------------------------

_WD_Startup()
If @error Then Exit SetError(2, @error)
$sSession = _WD_CreateSession($sDesiredCapabilities)
If @error Then Exit SetError(3, @error)

; (3) -------------------------------------------------------------------------
; Maximize browser window
; -----------------------------------------------------------------------------
_WD_Window($sSession, "Maximize")
If @error Then Exit SetError(4, @error)
; (3) End ---------------------------------------------------------------------

RetrieveTable($aResult_All)
If @error Then Exit SetError(5, @error)
ControlSetText($hSplash, "", "Static1", "Shutting down WebDriver automation")
_WD_DeleteSession($sSession)
If @error Then Exit SetError(6, @error)
_WD_Shutdown()
If @error Then Exit SetError(7, @error)
SplashOff()
_ArrayDisplay($aResult_All)
MsgBox($MB_ICONINFORMATION, $sScriptName, "Finished!", 5)
Exit

Func RetrieveTable(ByRef $aResult_All)
	Local $aResult, $sElement, $iPage = 0, $sText, $aOptions
	ControlSetText($hSplash, "", "Static1", "Opening website www.enforcementtracker.com")

	; Open website
	_WD_Navigate($sSession, "http://www.enforcementtracker.com/")
	If @error Then Return SetError(@error)

	; (4) ---------------------------------------------------------------------
	; Retrieve the values from the selection list and use the second to last (50)
	; -------------------------------------------------------------------------
	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//select[@name='penalties_length']")
	$sText = _WD_ElementAction($sSession, $sElement, 'property', 'innerText')
	$aOptions = StringSplit($sText, @LF, $STR_NOCOUNT)
	_WD_ElementOptionSelect($sSession, $_WD_LOCATOR_ByXPath, "//select[@name='penalties_length']//option[contains(text(),'" & $aOptions[UBound($aOptions, 1) - 2] & "')]")
	If @error Then Return SetError(@error)
	; (4) End -----------------------------------------------------------------

	; (5) ---------------------------------------------------------------------
	; Sort table by fine (descending)
	; -------------------------------------------------------------------------
	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//th[contains(text(),'Fine [')]") ; Find "Fine" header of the table
	_WD_ElementAction($sSession, $sElement, "click") ; Click twice to sort descending
	_WD_ElementAction($sSession, $sElement, "click")
	; (5) End -----------------------------------------------------------------

	While 1
		$iPage = $iPage + 1
		ControlSetText($hSplash, "", "Static1", "Loading data from website - page " & $iPage)
		$aResult = _WD_GetTable($sSession, "//table[@id='penalties']", $iPage)
		If @error Then Return SetError(@error)
		If $iPage = 1 Then
			$aResult_All = $aResult
		Else
			_ArrayConcatenate($aResult_All, $aResult)
		EndIf

		; (6, 7) --------------------------------------------------------------
		; Find the "next" button. If found scroll down so the button comes into
		; view and click the button. Else exit the function
		; ---------------------------------------------------------------------
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//a[@class='paginate_button next']") ; Find "Next" button
		If @error Then ExitLoop
		_WD_ExecuteScript($sSession, "arguments[0].scrollIntoView(true);", '{"' & $_WD_ELEMENT_ID & '":"' & $sElement & '"}') ; Scroll down to the link
		If @error Then
			Return SetError(@error)
		Else
			_WD_ElementAction($sSession, $sElement, "click") ; Click on the "Next" button
		EndIf
		; (6, 7) End ----------------------------------------------------------

	WEnd
EndFunc   ;==>RetrieveTable

; #FUNCTION# ====================================================================================================================
; Name ..........: _WD_GetTable
; Description ...: Return all elements of a table
; Syntax ........: _WD_GetTable($sSession, $sBaseElement)
; Parameters ....: $sSession     - Session ID from _WDCreateSession
;                  $sBaseElement - XPath of the table to return
; Return values .: Success      - 2D array
;                  Failure      - ""
;                  @ERROR       - $_WD_ERROR_Success
;                  				- $_WD_ERROR_Exception
;                  				- $_WD_ERROR_NoMatch
;                  @EXTENDED    - WinHTTP status code
; Author ........: danylarson
; Modified ......: water
; Remarks .......:
; Related .......:
; Link ..........: https://www.autoitscript.com/forum/topic/191990-webdriver-udf-w3c-compliant-version-01182020/page/18/?tab=comments#comment-1415164
; Example .......: No
; ===============================================================================================================================
Func _WD_GetTable($sSession, $sBaseElement, $iPage)
	Local Const $sFuncName = "_WD_GetTable"
	Local $aElements, $iLines, $iColumns, $iRow, $iColumn
	$aElements = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, $sBaseElement & "/tbody/tr", "", True) ; Retrieve the number of table rows
	If @error <> $_WD_ERROR_Success Then Return SetError(__WD_Error($sFuncName, @error, "HTTP status = " & $_WD_HTTPRESULT), $_WD_HTTPRESULT, "")
	$iLines = UBound($aElements)
	$aElements = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, $sBaseElement & "/tbody/tr[1]/td", "", True) ; Retrieve the number of table columns by checking the first table row
	If @error <> $_WD_ERROR_Success Then Return SetError(__WD_Error($sFuncName, @error, "HTTP status = " & $_WD_HTTPRESULT), $_WD_HTTPRESULT, "")
	$iColumns = UBound($aElements)
	Local $aTable[$iLines][$iColumns] ; Create the AutoIt array to hold all cells of the table
	$aElements = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, $sBaseElement & "/tbody/tr/td", "", True) ; Retrieve all table cells
	If @error <> $_WD_ERROR_Success Then Return SetError(__WD_Error($sFuncName, @error, "HTTP status = " & $_WD_HTTPRESULT), $_WD_HTTPRESULT, "")
	For $i = 0 To UBound($aElements) - 1
		$iRow = Int($i / $iColumns) ; Calculate row/column of the AutoIt array where to store the cells value
		$iColumn = Mod($i, $iColumns)
		If Mod($i, $iColumns) = 0 Then ControlSetText($hSplash, "", "Static1", "Loading data from website - page " & $iPage & ", row " & $i / $iColumns)
		$aTable[$iRow][$iColumn] = _WD_ElementAction($sSession, $aElements[$i], "Text") ; Retrieve text of each table cell
		If @error <> $_WD_ERROR_Success Then Return SetError(__WD_Error($sFuncName, @error, "HTTP status = " & $_WD_HTTPRESULT), $_WD_HTTPRESULT, "")
	Next
	Return $aTable
EndFunc   ;==>_WD_GetTable