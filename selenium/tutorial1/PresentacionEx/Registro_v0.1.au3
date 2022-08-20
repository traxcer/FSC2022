#comments-start

Programa:    Registro tareas de presentaciones
Para:        APAE Málaga
Programador: Manuel Eloy Gutiérrez
Fecha:       Málaga a 01 de Junio del 2020

#comments-end


#include <MsgBoxConstants.au3>
#include <WinAPIFiles.au3>
#include <Array.au3>
#include <File.au3>


Func _Au3RecordSetup()
Opt('WinWaitDelay',100)
Opt('WinDetectHiddenText',1)
Opt('MouseCoordMode',0)
Local $aResult = DllCall('User32.dll', 'int', 'GetKeyboardLayoutNameW', 'wstr', '')
If $aResult[1] <> '0000040A' Then
  MsgBox(64, 'Warning', 'Recording has been done under a different Keyboard layout' & @CRLF & '(0000040A->' & $aResult[1] & ')')
EndIf
EndFunc

_AU3RecordSetup()

Func _WinWaitActivate($title,$text,$timeout=0)
	WinWait($title,$text,$timeout)
	If Not WinActive($title,$text) Then WinActivate($title,$text)
	WinWaitActive($title,$text,$timeout)
 EndFunc

Func TipodeVia($TipoVia)
   local $devuelve=""
   if $Tipovia="Calle" then $devuelve="c"
	Return $devuelve
 EndFunc

 Func LocalEscuela($locaEscuela)
   Local $devuelve=""
   if $locaEscuela="ALAMEDA" then $devuelve             ="a"
   if $locaEscuela="ALCAUCÍN" then $devuelve            ="aa"
   if $locaEscuela="ALFARNATE" then $devuelve           ="aaa"
   if $locaEscuela="ALFARNATEJO" then $devuelve         ="aaaa"
   if $locaEscuela="ALGARROBO" then $devuelve           ="aaaaa"
   if $locaEscuela="ALGATOCÍN" then $devuelve           ="aaaaaa"
   if $locaEscuela="ALHAURÍN DE LA TORRE" then $devuelve="aaaaaaa"
   if $locaEscuela="ALHAURÍN EL GRANDE" then $devuelve  ="aaaaaaaa"
   if $locaEscuela="ALMÁCHAR" then $devuelve            ="aaaaaaaaa"
   if $locaEscuela="ALMARGEN" then $devuelve            ="aaaaaaaaaa"
   if $locaEscuela="ALMOGÍA" then $devuelve             ="aaaaaaaaaaa"
   if $locaEscuela="ÁLORA" then $devuelve               ="aaaaaaaaaaaa"
   if $locaEscuela="ALOZAINA" then $devuelve            ="aaaaaaaaaaaaa"
   if $locaEscuela="ALPANDEIRE" then $devuelve          ="aaaaaaaaaaaaaa"
   if $locaEscuela="ANTEQUERA" then $devuelve           ="aaaaaaaaaaaaaaa"
   if $locaEscuela="ÁRCHEZ" then $devuelve              ="aaaaaaaaaaaaaaaa"
   if $locaEscuela="ARCHIDONA" then $devuelve           ="aaaaaaaaaaaaaaa"
   if $locaEscuela="ARDALES" then $devuelve             ="aaaaaaaaaaaaaaaaaa"
   if $locaEscuela="ARENAS" then $devuelve              ="aaaaaaaaaaaaaaaaaaa"
   if $locaEscuela="ARRIATE" then $devuelve             ="aaaaaaaaaaaaaaaaaaaa"
   if $locaEscuela="ATAJATE" then $devuelve             ="aaaaaaaaaaaaaaaaaaaaa"
   if $locaEscuela="BENADALID" then $devuelve           ="b"
   if $locaEscuela="BENAHAVÍS" then $devuelve           ="bb"
   if $locaEscuela="BENALAURÍA" then $devuelve          ="bbb"
   if $locaEscuela="BENALMÁDENA" then $devuelve         ="bbbb"
   if $locaEscuela="BENAMARGOSA" then $devuelve         ="bbbbb"
   if $locaEscuela="BENAMOCARRA" then $devuelve         ="bbbbbb"
   if $locaEscuela="BENAOJÁN" then $devuelve            ="bbbbbbb"
   if $locaEscuela="BENARRABÁ" then $devuelve           ="bbbbbbbb"
   if $locaEscuela="CAMPILLOS" then $devuelve           ="c"

   if $locaEscuela="COLMENAR" then $devuelve            ="cccccccccccc"
   if $locaEscuela="CUEVAS DE SAN MARCOS" then $devuelve="ccccccccccccccccc"

   if $locaEscuela="ESTEPONA" then $devuelve            ="e"

   if $locaEscuela="Fuengirola" then $devuelve          ="fff"
   if $locaEscuela="HUMILLADERO" then $devuelve         ="h"
   if $locaEscuela="MOLLINA" then $devuelve="mmmmmmm"

   if $locaEscuela="MÁLAGA" then $devuelve="mm"
   if $locaEscuela="MARBELLA" then $devuelve="mmmm"
   if $locaEscuela="RONDA" then $devuelve               ="rrr"
   if $locaEscuela="Torre del Mar" then $devuelve="vv"
   if $locaEscuela="VÉLEZ-MÁLAGA" then $devuelve="vv"
   if $locaEscuela="MÁLAGA" then $devuelve="mm"
   if $locaEscuela="TEBA" then $devuelve="t"

   if $locaEscuela="YUNQUERA" then $devuelve="y"


#comments-start
BORGE, EL
BURGO, EL


CAÑETE LA REAL
CANILLAS DE ACEITUNO
CANILLAS DE ALBAIDA
CARRATRACA
CARTAJIMA
CÁRTAMA
CASABERMEJA
CASARABONELA
CASARES
COÍN

COMARES
CÓMPETA
CORTES DE LA FRONTERA
CUEVAS BAJAS
CUEVAS DEL BECERRO

CÚTAR
ESTEPONA
FARAJÁN
FRIGILIANA
FUENGIROLA
FUENTE DE PIEDRA
GAUCÍN
GENALGUACIL
GUARO

IGUALEJA
ISTÁN
IZNATE
JIMERA DE LÍBAR
JUBRIQUE
JÚZCAR
MACHARAVIAYA
MANILVA

MIJAS
MOCLINEJO
MONDA
MONTEJAQUE
NERJA
OJÉN
PARAUTA
PERIANA
PIZARRA
PUJERRA
RINCÓN DE LA VICTORIA
RIOGORDO
RONDA
SALARES
SAYALONGA
SEDELLA
SIERRA DE YEGUAS
TEBA
TOLOX
TORREMOLINOS
TORROX
TOTALÁN
VALLE DE ABDALAJÍS
VÉLEZ-MÁLAGA
VILLANUEVA DE ALGAIDAS
VILLANUEVA DE LA CONCEPCIÓN
VILLANUEVA DEL ROSARIO
VILLANUEVA DEL TRABUCO
VILLANUEVA DE TAPIA
VIÑUELA

#comments-end

	Return $devuelve
 EndFunc

 Func Extracto($fichero)
	Local $devuelve=""
	Local $Leo=""
	$leo=StringLeft ($fichero, 3 )
	if $leo="SOL" then $devuelve="SOLICITUDES PRUEBAS DE APTITUD"
    if $leo="DES" then $devuelve="RELACIONES DESTREZA"
	if $leo="CIR" then $devuelve="RELACIONES CIRCULACIÓN"
    if $leo="TRA" then $devuelve="SOLICITUD TRASLADO DE EXPEDIENTE"
    if $leo="MPC" then $devuelve="CRONOGRAMA CURSO MMPP"
    if $leo="MPS" then $devuelve="SOLICITUD CURSO MMPP"
    if $leo="CUR" then $devuelve="COMUNICACION CURSO A"

    Return $devuelve
 EndFunc

Func Terminate()
   MsgBox($MB_OK,"Fin Script","Progrma Terminado con ESCAPE.")
    Exit
 EndFunc   ;==>Terminate

Func LimpiaComi ($Cadena)
   local $devuelve=""
   local $i=0
   local $caracter=""

   for $i=1 to StringLen($Cadena)
	  $caracter= StringMid($Cadena,$i,1)
	  if $caracter <> Chr(34) Then ;Quita las comillas
		 $devuelve=$devuelve & $caracter
	  EndIf
   Next
   Return $devuelve
EndFunc

;*******************************************************************************************************************
;*******************************************************************************************************************
;COMENZAMOS EL PROCESO
;*******************************************************************************************************************
;*******************************************************************************************************************

;Leeemos los ficheros que hay en cada carpeta de presentaciones

; En el array tenemos las carpetas $aCarpetas
$aCarpetas = _FileListToArray("C:\PRESENTACIONES\", "*",2)

; En el array tenemos las carpetas $aCarpetas
    If @error = 1 Then
        MsgBox($MB_SYSTEMMODAL, "", "La ruta no es válida.")
        Exit
    EndIf
    If @error = 4 Then
        MsgBox($MB_SYSTEMMODAL, "", "No hay ningúna CARPETA en la ruta C:\PRESENTACIONES.")
        Exit
    EndIf

;AQUI INSERTARE UN BUCLE QUE RECORRA TODAS LAS CARPETAS $aCarpetas[n]

$aEscuela= StringSplit($aCarpetas[1],"_") ;Recuperamos el nombre de la escuela que vamos a tratar su carpeta
FileChangeDir("c:\presentaciones\"& $aCarpetas[1]) ; Establece el directorio de trabajo

;Leemos los ficheros que hay en la carpeta
$aFicheros = _FileListToArray("c:\presentaciones\"& $aCarpetas[1], "*",1) ;Todos los ficheros
; En el array tenemos las carpetas $aCarpetas
    If @error = 1 Then
        MsgBox($MB_SYSTEMMODAL, "", "La ruta no es válida para el fichero.")
        Exit
    EndIf
    If @error = 4 Then
        MsgBox($MB_SYSTEMMODAL, "", "No hay ningún fichero en la carpeta de presentación "+ & $aCarpetas[1])
        Exit
    EndIf
;MsgBox(4096,"","Ficheros a Registrar: " & $aFicheros[0] & " y el Primero es: " & $aFicheros[3])

$NumProvEscuela = StringMid($aFicheros[1],11,6)
$SecEscuela= StringMid($aFicheros[1],18,2)

$aFicherosSeccion = _FileListToArray("c:\presentaciones\"& $aCarpetas[1], "*" & StringMid($aFicheros[1],11,9) & "*.*",1)
; En el array tenemos los ficheros $aFicherosSeccion
	  If @error = 1 Then
		 MsgBox($MB_SYSTEMMODAL, "", "La ruta no es válida para el fichero.")
		 Exit
	  EndIf
	  If @error = 4 Then
		 MsgBox($MB_SYSTEMMODAL, "", "No hay ningún fichero en la ruta de presentación para esa Seccion.")
		 Exit
	  EndIf
$Texto1= "Vamos a Procesar la Presentacion de la Escuela: "
$Texto2= "Cuyos archivos estan en la ruta: "
$Texto3= "Ficheros a Registrar :"
$Texto4= ""
for $i=1 to $aFicherosSeccion[0]
   $Texto4 = $Texto4 & $aFicherosSeccion[$i] & @CRLF
Next
$r=MsgBox($MB_OKCANCEL ,"Info", $Texto1 & @CRLF & $aEscuela[1] & @CRLF& @CRLF & $Texto2 & @CRLF & @WorkingDir & @CRLF& @CRLF & $Texto3 & @CRLF & $Texto4)
if $r= $IDCANCEL then exit

;*******************
; Toma Datos ESCUELAS
;*******************
Dim $Registros
If Not _FileReadToArray("c:\presentaciones\Usuarios.CSV",$Registros) Then
	MsgBox(4096,"Error", " Error leyendo el fichero de USUARIOS.CSV: " & @error)
	Exit
 Else
	$Encuentra=False
	for $y = 1 to UBound($Registros)-1 ;Bucle para localizar Escuela
	  ;Cargamos los Campos
	  $aCampos = StringSplit($Registros[$y],",") ; Devuelve un array con los campos
	  $id = $aCampos[1]
	  $username = $aCampos[2]
	  $email=$aCampos[3]
	  $firstname=LimpiaComi($aCampos[4])
	  $lastname = LimpiaComi($aCampos[5])

	  $NombreEscuela = $firstname & " " & $lastname
	  $idnumber	=$aCampos[6]
	  $institution=$aCampos[7]
	  $department=$aCampos[8]
	  $phone1=$aCampos[9]
	  $phone2=$aCampos[10]
	  $city=$aCampos[11]
	  $url=$aCampos[12]
	  $icq=$aCampos[13]
	  $skype=$aCampos[14]
	  $aim=$aCampos[15]
	  $yahoo=$aCampos[16]
	  $msn=$aCampos[17]
	  $country=$aCampos[18]
	  $NumDoc=$aCampos[19]
	  $profile_field_NumProvEscuela=$aCampos[20]
	  $NombDire=LimpiaComi($aCampos[21]) ; Datos del director
	  $Ape1Dire=LimpiaComi($aCampos[22])
	  $Ape2Dire=LimpiaComi($aCampos[23])
	  $TipoVia="Calle"
	  $NombreyNumVia = LimpiaComi($aCampos[24])
	  $Bloque=$aCampos[25]
	  $Escalera=$aCampos[26]
	  $Piso=$aCampos[27]
	  $Puerta=$aCampos[28]
	  $CPEscuela =$aCampos[29]
	  $TelEscuela=$aCampos[30]
	  $PaisEscuela="España"
	  $ProvEscuela="Malaga"
	  $LocaEscuela=LimpiaComi($aCampos[31])
	  ;Msgbox($MB_OK,"Info","La Localidad leida es:"& $LocaEscuela)
	  $CentroEx=$aCampos[32]
	  ;MsgBox($MB_OK,"Info","$NombreEscuela =" & $NombreEscuela & @CRLF & "$LocaEscuela="& $LocaEscuela & @CRLF & "$CentroEx="& $CentroEx)
	  if $NombreEscuela = $aEscuela[1] Then
		 ;MsgBox(4096,"Eureka","Escuela Encontrada y ya tenemos sus datos")
		 $Encuentra= True
		 ExitLoop
	  EndIf
   Next
   if Not $Encuentra Then
	  MsgBox(4096,"Error", " Error leyendo el fichero de USUARIOS.CSV: No existe el usuario")
	  Exit
   EndIf
 EndIf


;Datos de la Escuela para el registro de los ficheros
;$NumProvEscuela = $profile_field_NumProvEscuela; Lo vamos a tomas del fichero5

;*******************
;Datos Representante, metemos a mano
;*******************
HotKeySet ( "{ESC}","Terminate")
_WinWaitActivate("Formulario para dar de alta un registro electrónico","")

;Send("^{F5}");Recargamos Pestaña
;Sleep(5000);

MouseClick("left",215, 516,1)
Send("c{ENTER}{TAB}gargantua{SPACE}12{TAB}{TAB}{TAB}{TAB}{TAB}29006{TAB}952325850{TAB}{HOME}e")
sleep(500)
Send("{HOME}e")
sleep(500)
Send("{TAB}mm") ; Provincia
sleep(800)
Send("{TAB}mm") ; Localidad
sleep(800)
;*******************
;Datos Interesado
;*******************
Send("{TAB}") ; Tipo de Documento
Send("{TAB}"& $NumDoc) ; Numero de Documento
Send("{TAB}"& $NombDire) ; Nombre del Director
Send("{TAB}"& $Ape1Dire) ; Apellido Primero del Director
Send("{TAB}"& $Ape2Dire) ; Apellido Segundo del Director
Send("{TAB}") ; Tipo de Vía
Send("{HOME}")
sleep(500)
Send(TipodeVia($TipoVia)) ; Tipo de Vía
Send("{TAB}"& $NombreyNumVia) ; Nombre y numero de la vía
Send("{TAB 4}")
Send("{TAB}"& $CPEscuela) ; Codigo Postal de la Escuela
Send("{TAB}"& $TelEscuela) ; Numero de Telefono
Send("{TAB}{HOME}e") ; siempre es España
sleep(500)
Send("{TAB}mm") ; Provincia (Siempre es Málaga)
sleep(500)
Send("{TAB}{HOME}" & LocalEscuela($LocaEscuela)) ; Localidad Escuela
sleep(500)

;*********************
;Datos de la Solicitud
;*********************
Send("{TAB}Jefatura Provincial de Trafico de Malag")
Sleep(2000)
Send("{DOWN}{ENTER}")
Select ;Toma PREFIJO del primer fichero para ver el tipo de presentacion
Case StringLeft($aFicherosSeccion[1],2)="MP"
   Send("{TAB}MMPP_"& $NumProvEscuela & "_" & $SecEscuela)
   Send("{TAB}PRESENTACION CURSO MMPP" )
   Send("{TAB}Se tramiten el Curso de MMPP presentado{TAB}")
Case StringLeft($aFicherosSeccion[1],2)="CU"
   Send("{TAB}CURSO_A_"& $NumProvEscuela & "_" & $SecEscuela)
   Send("{TAB}COMUNICACION CURSO A" )
   Send("{TAB}Se autorice el Curso A presentado{TAB}")
Case StringLeft($aFicherosSeccion[1],3)="ANU"
   Send("{TAB}ANULACION CITA A EXAMEN "& $NumProvEscuela & "_" & $SecEscuela)
   Send("{TAB}SOLICITUD DE ANULACION DE CITA A EXAMEN" )
   Send("{TAB}Se anule la cita de examen de los expedientes presentados{TAB}")
Case StringLeft($aFicherosSeccion[1],3)="TRA"
   Send("{TAB}TRASLADO DE EXPEDIENTE "& $NumProvEscuela & "_" & $SecEscuela)
   Send("{TAB}SOLICITUD DE TRASLADO DE EXPEDIENTE" )
   Send("{TAB}Se le de traslado al expediente solicitado{TAB}")
Case Else
   Send("{TAB}EXAM_"& $NumProvEscuela & "_" & $SecEscuela)
   Send("{TAB}PRESENTACION DE CARPETA DE EXAMEN CENTRO DE EXAMEN "& $CentroEx )
   Send("{TAB}Se tramiten las solicitudes de pruebas de aptitud presentadas{TAB}")
EndSelect

Send("{TAB}")

;*******************
;Documentos Anexos
;*******************
Send("{ENTER}")
Sleep(500)
Send("{TAB}")
For $i=1 to $aFicherosSeccion[0]
   Send(Extracto($aFicherosSeccion[$i]))
   Send("{TAB}{ENTER}")
   _WinWaitActivate("Carga de archivos","")
   Send("C:\PRESENTACIONES\"& $aCarpetas[1] & "\" & $aFicherosSeccion[$i])
   Sleep(500)
   Send("{ENTER}"); Mete el archivo
   Sleep(500)
   if $i=$aFicherosSeccion[0] then ExitLoop
   Send("{TAB 2}}{ENTER}") ; Añadir archivo
   Send("{TAB}")
   Send("+{TAB}+{TAB}+{TAB}+{TAB}{ENTER}")
Next
Send("{TAB}{TAB}{TAB}{SPACE}{TAB}apae@apaemalaga.es")
Send("{TAB}{TAB}")

MouseClick("left",1611, 826 ,1)
Sleep(500)
Send("{SPACE}")
MsgBox($MB_OK,"Fin Script","Programa Terminado a su final.")
Exit