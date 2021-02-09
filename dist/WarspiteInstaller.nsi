; Modern UI
!include "MUI2.nsh"

; ------------------------------------------------------------------------------
; General

; Configuration
!define CONFIG_PATH "..\shared\src\EngineMetadata.h"
!searchparse /file "${CONFIG_PATH}" `#define GAME_BUILD_NUMBER   `   PRO_BUILD
!searchparse /file "${CONFIG_PATH}" `#define GAME_GIT_HASH      "`   PRO_GIT_HASH `"`
!searchparse /file "${CONFIG_PATH}" `#define GAME_GIT_DESC      "`   PRO_GIT_DESC `"`
!searchparse /file "${CONFIG_PATH}" `#define GAME_BUILD_TIME     `   PRO_BUILD_TIME

; Definitions
!define PRODUCT_EXE "ws.exe"
!define PRODUCT_NAME "Warspite"
!define PRODUCT_PUBLISHER "ZanyLeonic"
!define PRODUCT_VERSION "${PRO_BUILD}.0.0"
!define PRODUCT_WEBSITE "https://zanyleonic.github.io/WarspiteEngine/"

!define AUTHOR_EXE       "WarspiteGame.AuthoringTools.exe"
!define AUTHOR_DEBUG_EXE "WarspiteGame.AuthoringTools.Debugger.exe"

; Build directory
; Change if you used CMake in another folder
!define BUILDDIR "..\rel-build"
!define AUTHTOOLSDIR "..\..\WarspiteGame.AuthoringTools"

; Version information
VIAddVersionKey "CompanyName" "${PRODUCT_PUBLISHER}"
VIAddVersionKey "FileDescription" "${PRODUCT_NAME} Setup"
VIAddVersionKey "FileVersion" "${PRODUCT_VERSION}.0"
VIAddVersionKey "LegalCopyright" ""
VIAddVersionKey "ProductName" "${PRODUCT_NAME}"
VIAddVersionKey "ProductVersion" "${PRODUCT_VERSION}.0"
VIProductVersion "${PRODUCT_VERSION}.0"

; Uninstaller
!define UNINST_EXE "Uninstall.exe"
!define UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"

; Main settings
BrandingText "${PRODUCT_NAME} ${PRO_GIT_DESC}"
Name "${PRODUCT_NAME}"
OutFile "output\${PRODUCT_NAME}Setup-x64.exe"
SetCompressor /SOLID lzma
Unicode true

; Default installation folder
!define DEFAULT_INSTALL_DIR "$APPDATA\${PRODUCT_NAME}"
InstallDir "${DEFAULT_INSTALL_DIR}"
; Get installation folder from registry, if available
InstallDirRegKey HKCU "Software\${PRODUCT_NAME}" ""

; Request application privileges for Windows Vista and above
RequestExecutionLevel user

; ------------------------------------------------------------------------------
; Interface settings

; Icons
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"

; Page header
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "bitmap\header.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "bitmap\header.bmp"

; Welcome/Finish page
!define MUI_WELCOMEFINISHPAGE_BITMAP "bitmap\wizard.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "bitmap\wizard.bmp"

; Abort warning
!define MUI_ABORTWARNING
!define MUI_UNABORTWARNING

; ------------------------------------------------------------------------------
; Page settings

; License page
!define MUI_LICENSEPAGE_BUTTON $(^NextBtn)
!define MUI_LICENSEPAGE_TEXT_BOTTOM $(^ClickNext)

; Components page
!define MUI_COMPONENTSPAGE_NODESC
!define MUI_COMPONENTSPAGE_TEXT_TOP "Check the any of the prerequisites if you don't have them installed, then Click Next to continue."

; Directory page
!define MUI_DIRECTORYPAGE_TEXT_TOP "\
    WARNING: Installing under Program Files may cause issues if you have User Account Control enabled on your system.$\r$\n$\r$\n\
    The default installation folder is:$\r$\n${DEFAULT_INSTALL_DIR}"

; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\${PRODUCT_EXE}"
!define MUI_FINISHPAGE_LINK "Visit home page"
!define MUI_FINISHPAGE_LINK_LOCATION "${PRODUCT_WEBSITE}"

; ------------------------------------------------------------------------------
; Pages

; Installation pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\LICENSE.MD"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

; Uninstallation pages
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; ------------------------------------------------------------------------------
; Language

; Default language
!insertmacro MUI_LANGUAGE "English"

; ------------------------------------------------------------------------------
; Install sections

Section "!${PRODUCT_NAME}" SECTION_DEFAULT
  ; Set section as read-only
  SectionIn RO

  ; Set properties
  SetOutPath "$INSTDIR"
  SetOverwrite on

  ; Add files
  ; Change these paths for your build respectively
  SetOutPath "$INSTDIR"
  File "${BUILDDIR}\Release\ws.exe"
  SetOutPath "$INSTDIR\bin\"
  File /r "${BUILDDIR}\Release\bin\"
  SetOutPath "$INSTDIR\assets\"
  File /r "${BUILDDIR}\Release\assets\"
  SetOutPath "$INSTDIR\platform\"
  File /r "${BUILDDIR}\Release\platform\"

  SetOutPath "$INSTDIR"

  ; Uninstaller
  WriteUninstaller "$INSTDIR\${UNINST_EXE}"
  WriteRegStr HKCU "${UNINST_KEY}" "DisplayName" "${PRODUCT_NAME}Engine"
  WriteRegStr HKCU "${UNINST_KEY}" "UninstallString" "$INSTDIR\${UNINST_EXE}"
  WriteRegStr HKCU "${UNINST_KEY}" "DisplayIcon" "$INSTDIR\${PRODUCT_EXE}"
  WriteRegStr HKCU "${UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr HKCU "${UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEBSITE}"
  WriteRegStr HKCU "${UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

Section "${PRODUCT_NAME} SDK" SECTION_AUTH
  ; Set section as read-only
  SectionIn ${SF_SELECTED}

  ; Set properties
  SetOutPath "$INSTDIR"
  SetOverwrite on

  ; Add files
  ; Change these paths for your build respectively
  SetOutPath "$INSTDIR\sdk\"
  File /r "..\sdk\"
  SetOutPath "$INSTDIR\sdk\bin\"
  File /r "${AUTHTOOLSDIR}\WarspiteGame.AuthoringTools\bin\Release\"
  File /r "${AUTHTOOLSDIR}\WarspiteGame.AuthoringTools.Debugger\bin\Release\"

  CreateShortCut "$INSTDIR\sdk\Generate Tiled Project.lnk" "$INSTDIR\platform\python.exe" '"$INSTDIR\sdk\tools\SetupTiledProject.py"'

  ; Give the location of our paths 
  DetailPrint "Configuring Authoring Tools..."
  ExecWait '"$INSTDIR\sdk\bin\${AUTHOR_EXE}" /setup "$INSTDIR\${PRODUCT_EXE}" "$INSTDIR\assets\' $0
  DetailPrint "Authoring Tools returned: $0"

  SetOutPath "$INSTDIR"
SectionEnd

Section "Start menu shortcuts" SECTION_START_MENU_SHORTCUTS
  CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}Engine.lnk" "$INSTDIR\${PRODUCT_EXE}"
  
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\${UNINST_EXE}"

  ${If} ${SectionIsSelected} ${SECTION_AUTH}
  CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}\SDK"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\SDK\${PRODUCT_NAME} Authoring Tools.lnk" "$INSTDIR\sdk\bin\${AUTHOR_EXE}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\SDK\${PRODUCT_NAME} Debugger.lnk" "$INSTDIR\sdk\bin\${AUTHOR_DEBUG_EXE}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\SDK\${PRODUCT_NAME} SDK folder.lnk" "$INSTDIR\sdk\"

  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\SDK\Launch ${PRODUCT_NAME} with Debugger.lnk" "$INSTDIR\sdk\bin\${AUTHOR_DEBUG_EXE}" "$INSTDIR\${PRODUCT_EXE}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\SDK\Generate Tiled Project.lnk" "$INSTDIR\platform\python.exe" '"$INSTDIR\sdk\tools\SetupTiledProject.py"'
  ${Endif}
SectionEnd

Section "Desktop shortcut" SECTION_DESKTOP_SHORTCUT
  CreateShortCut "$DESKTOP\${PRODUCT_NAME}Engine.lnk" "$INSTDIR\${PRODUCT_EXE}"
SectionEnd

Section /o "Visual C++ Redistributable 2019 (x64)"
  DetailPrint "Installing VC C++ Redist (x64)"
  SetOutPath "$INSTDIR"
  File "redist\vcredist_x64.exe"
  ExecWait '"$INSTDIR\vcredist_x64.exe" /quiet' $0
  Delete "$INSTDIR\vcredist_x64.exe"
  DetailPrint "VC C++ installer exited with: $0"
SectionEnd

Section /o ".NET 4.0 Framework (x86/x64)"
  DetailPrint "Installing .NET Framework 4.0 (x86/x64)"
  SetOutPath "$INSTDIR"
  File "redist\dotNetFx40_Full_setup.exe"
  ExecWait '"$INSTDIR\dotNetFx40_Full_setup.exe" /q' $0
  Delete "$INSTDIR\vcredist_x64.exe"
  DetailPrint ".NET Framework 4.0 installer exited with: $0"
SectionEnd

; ------------------------------------------------------------------------------
; Uninstall section

Section Uninstall
  ; Delete registry entries
  DeleteRegKey HKCU "${UNINST_KEY}"
  DeleteRegKey /ifempty HKCU "Software\${PRODUCT_NAME}"

  ; Delete start menu shortcuts
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}Engine.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk"
  RMDir "$SMPROGRAMS\${PRODUCT_NAME}"

  ; Delete desktop shortcut
  Delete "$DESKTOP\${PRODUCT_NAME}Engine.lnk"

  ; SDK
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\SDK\${PRODUCT_NAME} Authoring Tools.lnk" 
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\SDK\${PRODUCT_NAME} Debugger.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\SDK\${PRODUCT_NAME} SDK folder.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\SDK\Launch ${PRODUCT_NAME} with Debugger.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\SDK\Generate Tiled Project.lnk"
  RMDir "$SMPROGRAMS\${PRODUCT_NAME}\SDK"
  RMDir /r "$INSTDIR\sdk\"

  ; Delete files
  Delete "$INSTDIR\${PRODUCT_EXE}"
  Delete "$INSTDIR\${UNINST_EXE}"
  RMDir /r "$INSTDIR\bin\"
  RMDir /r "$INSTDIR\assets\"
  RMDir /r "$INSTDIR\platform\"
SectionEnd

; ------------------------------------------------------------------------------
; Callback functions

Function .onInit
  Call InitializeSections
FunctionEnd

Function .onInstSuccess
  Call SaveInstDir
  Call SaveSections
  Call RunProductExe
FunctionEnd

; ------------------------------------------------------------------------------
; Custom functions

Function SaveInstDir
  IfSilent 0 +2
  Return
  WriteRegStr HKCU "Software\${PRODUCT_NAME}" "" $INSTDIR
FunctionEnd

Function RunProductExe
  IfSilent +2
  Return
  Exec "$INSTDIR\${PRODUCT_EXE}"
FunctionEnd

Function InitializeSections
  !macro DeselectSection sectionId
    Push $0
    SectionGetFlags ${sectionId} $0
    IntOp $0 $0 ^ ${SF_SELECTED}
    SectionSetFlags ${sectionId} $0
    Pop $0
  !macroend

  !macro LoadSection sectionId
    Push $0
    SectionGetText ${sectionId} $0
    ClearErrors
    ReadRegDWORD $0 HKCU "Software\${PRODUCT_NAME}\Setup\Sections" "$0"
    IfErrors +2
    SectionSetFlags ${sectionId} $0
    Pop $0
  !macroend

  IfSilent 0 loadSections
  !insertmacro DeselectSection ${SECTION_START_MENU_SHORTCUTS}
  !insertmacro DeselectSection ${SECTION_DESKTOP_SHORTCUT}
  Return

  loadSections:
  !insertmacro LoadSection ${SECTION_AUTH}
  !insertmacro LoadSection ${SECTION_START_MENU_SHORTCUTS}
  !insertmacro LoadSection ${SECTION_DESKTOP_SHORTCUT}
FunctionEnd

Function SaveSections
  IfSilent 0 +2
  Return

  !macro SaveSection sectionId
    Push $0
    Push $1
    SectionGetText ${sectionId} $0
    SectionGetFlags ${sectionId} $1
    WriteRegDWORD HKCU "Software\${PRODUCT_NAME}\Setup\Sections" "$0" $1
    Pop $1
    Pop $0
  !macroend

  !insertmacro SaveSection ${SECTION_AUTH}
  !insertmacro SaveSection ${SECTION_START_MENU_SHORTCUTS}
  !insertmacro SaveSection ${SECTION_DESKTOP_SHORTCUT}
FunctionEnd