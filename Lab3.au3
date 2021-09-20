#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.14.5
 Authors:         Dmitry Alexeev M3O-424B-18	Safronof Kirill M3O-424B-18

 Script Function:
	Script for laboratory work №3.

#ce ----------------------------------------------------------------------------

; Script Start

#include <MsgBoxConstants.au3>
;#include <WinAPIFiles.au3>
#include <Misc.au3>	;function from this lib is used in _IsAnyKeyPressed
#include <Date.au3>
;#include <SendMessage.au3>


Func _IsAnyKeyPressed($UseNames=0)
    $number = StringSplit("01|02|04|05|06" & _
    "|08|09|0C|0D|10|11|12|13|14|1B|20|21|22" & _
    "|23|24|25|26|27|28|29|2A|2B|2C|2D|2E|30" & _
    "|31|32|33|34|35|36|37|38|39|41|42|43|44" & _
    "|45|46|47|48|49|4A|4B|4C|4D|4E|4F|50|51" & _
    "|52|53|54|55|56|57|58|59|5A|5B|5C|60|61" & _
    "|62|63|64|65|66|67|68|69|6A|6B|6C|6D|6E" & _
    "|6F|70|71|72|73|74|75|76|77|78|79|7A|7B" & _
    "|90|91|A0|A1|A2|A3|A4|A5", "|")

    $names = StringSplit("Left mouse|Right mouse" & _
    "|Middle mouse|X1 mouse|X2 mouse|BACKSPACE|TAB" & _
    "|CLEAR|ENTER|SHIFT|CTRL|ALT|PAUSE|CAPSLOCK|ESC" & _
    "|SPACEBAR|PAGEUP|PAGEDOWN|END|HOME|LEFT ARROW" & _
    "|UP ARROW|RIGHT ARROW|DOWN ARROW|SELECT|PRINT" & _
    "|EXECUTE|PRINT SCREEN|INS|DEL|0|1|2|3|4|5|6|7" & _
    "|8|9|a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u" & _
    "|v|w|x|y|z|Left Win|Right Win|Num 0|Num 1|Num 2" & _
    "|Num 3|Num 4|Num 5|Num 6|Num 7|Num 8|Num 9|Multiply" & _
    "|Add|Separator|Subtract|Decimal|Divide|F1|F2|F3|F4" & _
    "|F5|F6|F7|F8|F9|F10|F11|F12|NUMLOCK|SCROLLLOCK|Left SHIFT" & _
    "|Right SHIFT|Left CONTROL|Right CONTROL|Left MENU|Right MENU","|")
    Dim $a=0
    For $i in $number
        $a+=1
        If _IsPressed(String($i)) Then
            If $UseNames then
                Return $names[$a-1]
            Else
                Return 1
            EndIf
        EndIf
    Next
    Return 0
EndFunc


HotKeySet("+!e", "Terminate")	;Shift+Alt+e
;HotKeySet("+!s", "Send_")		;Shift+Alt+s
HotKeySet("+s", "Send_")
Func Terminate()
   FileClose($logFile)
   MsgBox($MB_SYSTEMMODAL, "Lab3", "Script terminated ☠.")
   Exit
EndFunc

;написать просто функцию в которой проверяется нажатие клавиши иесли совпадает, то печатать ее еще раз
Func Send2_($sym)
   Local Const $MyMessage = "s"
   If $sym = $MyMessage Then
	  $logString = _NowDate() & " " & _NowTime(3) & " Sent: " & $MyMessage
	  FileWriteLine($logFile, $logString)
	  Send($MyMessage)
	  MsgBox($MB_SYSTEMMODAL, "Lab3", "You pressed Shift+Alt+s.")
   EndIf
EndFunc
Func Send_()
   ;Local $hWnd = WinGetHandle('$ClassName')
   Local Const $MyMessage = "s"
   $logString = _NowDate() & " " & _NowTime(3) & " Sent: " & $MyMessage
   FileWriteLine($logFile, $logString)
   Send($MyMessage)
   MsgBox($MB_SYSTEMMODAL, "Lab3", "You pressed Shift+Alt+s.")
EndFunc

Local Const $WindowName = "Symbol input program"
;Local Const $ClassName = "My class"
Local Const $sFilePath = (@ScriptDir & "\log.txt")
Local $iFileExists = FileExists($sFilePath)

If $iFileExists Then
   MsgBox($MB_SYSTEMMODAL, "Lab3", "The log file exists.")
Else
   MsgBox($MB_SYSTEMMODAL, "Lab3", "The log file doesn't exist. It will be created automatically in the script directory." )
EndIf

$logFile = FileOpen(@ScriptDir & "\log.txt", $FO_APPEND)
If $logFile = -1 Then
   MsgBox($MB_SYSTEMMODAL, "Lab3", "An error occurred when reading the file.")
   Return False
EndIf

While 1
   ;обработка открытия окна
   If WinExists($WindowName) = 0 Then
	  WinWaitActive($WindowName)
	  $logString = _NowDate() & " " & _NowTime(3) & " Program started."
	  FileWriteLine($logFile, $logString)
   EndIf
   $hProgram = WinActive($WindowName)
   If $hProgram Then	;Checks to see if a specified window exists and is currently active.
	  $key=_IsAnyKeyPressed(1)
	  If $key Then
		 $logString = _NowDate() & " " & _NowTime(3) & " Pressed key: " & $key
		 FileWriteLine($logFile, $logString)
		 Send2_($key)
		 Sleep(100)
	  EndIf
   EndIf
   ;обработка закрытия окна
   If WinExists($WindowName) = 0 Then
	  $logString = _NowDate() & " " & _NowTime(3) & " Program closed."
	  FileWriteLine($logFile, $logString)
   EndIf
WEnd



