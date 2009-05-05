; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDirWatcherDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "dirwatcher.h"
LastPage=0

ClassCount=4
Class1=CDirWatcherApp
Class2=CDirWatcherDlg
Class3=CSetFilterFlagsDlg
Class4=CTestFilterPatternDlg

ResourceCount=3
Resource1=IDD_DLG_TEST_PATTERNS (English (U.S.))
Resource2=IDD_DIRWATCHER_DIALOG (English (U.S.))
Resource3=IDD_SET_FILTER_FLAGS_DLG (English (U.S.))

[CLS:CDirWatcherApp]
Type=0
BaseClass=CWinApp
HeaderFile=DirWatcher.h
ImplementationFile=DirWatcher.cpp
LastObject=IDC_BTN_MONITOR

[CLS:CDirWatcherDlg]
Type=0
BaseClass=CDialog
HeaderFile=DirWatcherDlg.h
ImplementationFile=DirWatcherDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT_DIR_TO_MONITOR

[CLS:CSetFilterFlagsDlg]
Type=0
BaseClass=CDialog
HeaderFile=SetFilterFlagsDlg.h
ImplementationFile=SetFilterFlagsDlg.cpp

[CLS:CTestFilterPatternDlg]
Type=0
BaseClass=CDialog
HeaderFile=TestFilterPatternDlg.h
ImplementationFile=TestFilterPatternDlg.cpp

[DLG:IDD_DIRWATCHER_DIALOG]
Type=1
Class=CDirWatcherDlg

[DLG:IDD_SET_FILTER_FLAGS_DLG]
Type=1
Class=CSetFilterFlagsDlg

[DLG:IDD_DLG_TEST_PATTERNS]
Type=1
Class=CTestFilterPatternDlg

[DLG:IDD_DIRWATCHER_DIALOG (English (U.S.))]
Type=1
Class=CDirWatcherDlg
ControlCount=45
Control1=IDCANCEL,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT_DIR_TO_MONITOR,edit,1350631552
Control4=IDC_BTN_BROWSE1,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_INCLUDE_FILTER1,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_EXCLUDE_FILTER1,edit,1350631552
Control9=IDC_STATIC,button,1342177287
Control10=IDC_CHECK_FILE_NAME1,button,1342242819
Control11=IDC_CHECK_LAST_WRITE1,button,1342242819
Control12=IDC_CHECK_DIR_NAME1,button,1342242819
Control13=IDC_CHECK_LAST_ACCESS1,button,1342242819
Control14=IDC_CHECK_SIZE1,button,1342242819
Control15=IDC_CHECK_CREATION1,button,1342242819
Control16=IDC_CHECK_ATTRIBUTES1,button,1342242819
Control17=IDC_CHECK_SECURITY1,button,1342242819
Control18=IDC_CHECK_SUBDIR1,button,1342242819
Control19=IDC_BTN_MONITOR,button,1342242816
Control20=IDC_BTN_CLEAR1,button,1342242816
Control21=IDC_LIST_CHANGES,listbox,1352728833
Control22=IDC_STATIC,static,1342308352
Control23=IDC_EDIT_DIR_TO_MONITOR2,edit,1350631552
Control24=IDC_BTN_BROWSE2,button,1342242816
Control25=IDC_STATIC,static,1342308352
Control26=IDC_EDIT_INCLUDE_FILTER2,edit,1350631552
Control27=IDC_STATIC,static,1342308352
Control28=IDC_EDIT_EXCLUDE_FILTER2,edit,1350631552
Control29=IDC_STATIC,button,1342177287
Control30=IDC_CHECK_FILE_NAME2,button,1342242819
Control31=IDC_CHECK_LAST_WRITE2,button,1342242819
Control32=IDC_CHECK_DIR_NAME2,button,1342242819
Control33=IDC_CHECK_LAST_ACCESS2,button,1342242819
Control34=IDC_CHECK_SIZE2,button,1342242819
Control35=IDC_CHECK_CREATION2,button,1342242819
Control36=IDC_CHECK_ATTRIBUTES2,button,1342242819
Control37=IDC_CHECK_SECURITY2,button,1342242819
Control38=IDC_CHECK_SUBDIR2,button,1342242819
Control39=IDC_BTN_MONITOR2,button,1342242816
Control40=IDC_BTN_CLEAR2,button,1342242816
Control41=IDC_LIST_CHANGES2,listbox,1352728833
Control42=IDC_STATIC,button,1342177287
Control43=IDC_STATIC,button,1342177287
Control44=IDC_BTN_TEST_PATTERNS,button,1342242816
Control45=IDC_BTN_SET_FILTER_FLAGS,button,1342242816

[DLG:IDD_DLG_TEST_PATTERNS (English (U.S.))]
Type=1
Class=?
ControlCount=8
Control1=IDCANCEL,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT_TEST_PATH,edit,1350635648
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_TEST_PATTERN,edit,1350635648
Control6=IDC_STATIC,static,1342308352
Control7=IDC_BTN_TEST,button,1342242816
Control8=IDC_STATIC_TEST_RESULTS,static,1342308353

[DLG:IDD_SET_FILTER_FLAGS_DLG (English (U.S.))]
Type=1
Class=?
ControlCount=13
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_RADIO_DONT_USE_FILTERS,button,1342177289
Control4=IDC_RADIO_CHECK_FULL_PATH,button,1342177289
Control5=IDC_RADIO_CHECK_PARTIAL_PATH,button,1342177289
Control6=IDC_RADIO_CHECK_FILENAME_ONLY,button,1342177289
Control7=IDC_STATIC,button,1342308359
Control8=IDC_RADIO_DONT_TEST_HANDLER,button,1342177289
Control9=IDC_RADIO_TEST_HANDLER_BEFORE,button,1342177289
Control10=IDC_RADIO_TEST_HANDLER_AFTER,button,1342177289
Control11=IDC_CHECK_WATCHSTARTED,button,1342242819
Control12=IDC_CHECK_WATCHSTOPPED,button,1342242819
Control13=IDC_STATIC,button,1342308359

