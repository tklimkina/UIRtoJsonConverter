/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1993-1999.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       cmdsrvr.h                                                     */
/*                                                                            */
/* Purpose:     Include file for LabWindows/CVI command server.               */
/*              The command server allows a DDE client to execute a subset    */
/*              of the commands that can be executed via the menus in the     */
/*              LabWindows/CVI environment.                                   */
/*                                                                            */
/* Remarks:     The CVI command server accepts strings via DDE.               */
/*              Each string contains a command name followed optionally       */
/*              by a parameter.  The command name and the parameter are       */
/*              separated by a comma.                                         */
/*                                                                            */
/*              CVI sends a string via DDE back to the client indicating      */
/*              whether it could execute the command.  The string contains    */
/*              an ASCII representation of an integer status value.           */
/*              This integer is a negative number if an error prevented       */
/*              CVI from starting the execution the command.                  */
/*                                                                            */
/*              NOTE:  Just because "0" is returned does not mean that        */
/*                     the command was successful.  If CVI encounters an      */
/*                     error in the process of executing the command, an      */
/*                     error will be reported to the user interactively,      */
/*                     just as if the command had been executed from a menu.  */
/*                                                                            */
/*              For some commands, the string also includes additional        */
/*              information.  More details are included in this file.         */
/*                                                                            */
/*              The response string is sent via a warm or hot link.           */
/*              The client receives it in its DDE client callback routine,    */
/*              as the hData parameter in the XTYP_ADVDATA transaction.       */
/*                                                                            */
/*              To connect to the command server, the server name must be     */
/*              "cvi", and the topic name must be "system".                   */
/*                                                                            */
/*              To set up a warm or hot link for the response, the item       */
/*              name must be "status".                                        */
/*                                                                            */
/*============================================================================*/


/*========================*/    /*========================*/   /*========================*/
/* command name constants */    /* command name strings   */   /* parameters required    */
/*========================*/    /*========================*/   /*========================*/
#define NEW_PROJECT             "New Project"
#define NEW_PROJECT_NO_SAVE     "New Project (no save)"
#define OPEN_PROJECT            "Open Project"                 /* absolute path for an existing file */
#define OPEN_PROJECT_NO_SAVE    "Open Project (no save)"       /* absolute path for an existing file;  user not prompted to save changed files */
#define SAVE_PROJECT            "Save Project"                 /* optional:  absolute path under which to save project */
#define ADD_TO_PROJECT          "Add To Project"               /* absolute path for an existing file */
#define REMOVE_FROM_PROJECT     "Remove From Project"          /* absolute path or simple name for a file in the project */
#define EXCLUDE_FROM_BUILD      "Exclude From Build"           /* absolute path or simple name for a file in the project */
#define INCLUDE_IN_BUILD        "Include In Build"             /* absolute path or simple name for a file in the project */
#define GET_EXCLUSION_STATE     "Get Exclusion State"          /* absolute path or simple name for a file in the project */
#define UPDATE_FILE_FROM_DISK   "Update File From Disk"        /* absolute path for a file (has no effect unless file is a .h, .c, .obj, .lib, .dll, or .pth) */
#define LOAD_INSTRUMENT         "Load Instrument"              /* absolute path for an existing file */
#define UNLOAD_INSTRUMENT       "Unload Instrument"            /* absolute path for a loaded instrument .fp file */
#define OPEN_SOURCE_FILE        "Open Source File"             /* absolute path for an existing text file */
#define CLOSE_SOURCE_FILE       "Close Source File"            /* absolute path for file in a source window */
#define OPEN_UIR_FILE           "Open UIR File"                /* absolute path for an existing UIR file */
#define OPEN_FP_FILE            "Open FP File"                 /* absolute path for an existing function panel file */
#define DOES_FILE_HAVE_CHANGES  "Does File Have Changes"       /* absolute path for an existing text or UIR file */
#define PRINT_SOURCE_FILE       "Print Source File"            /* absolute pathname for a text file */
#define PRINT_UIR_FILE          "Print UIR File"               /* absolute pathname for a UIR file */
#define PRINT_FILES_IN_PROJECT  "Print Files In Project"       /* absolute pathname for a project file */

/***************************************************************************************/
/* The next three commands take an absolute pathname to the file,                      */
/* a printer name, drive name, and port name in the following format:                  */
/*                                                                                     */
/* <Command name>, "<file name>" "<printer name>" "<driver name>" "<port name>"        */
/*                                                                                     */
/* For example: Print Source File To, "c:\my file.c" "My Printer" "hppcl5.drv" "lpt1:" */
/***************************************************************************************/
#define PRINT_SOURCE_FILE_TO        "Print Source File To"
#define PRINT_UIR_FILE_TO           "Print UIR File To"
#define PRINT_FILES_IN_PROJECT_TO   "Print Files In Project To"

#define SAVE_FILE               "Save File"                    /* absolute path for a loaded file */
#define SAVE_ALL                "Save All"
#define SET_COMPILER_DEFINES    "Set Compiler Defines"         /* the desired compiler defines contents */
#define COMPILE_FILE            "Compile File"                 /* absolute path or simple name for a C source file in the project */
#define BUILD_PROJECT           "Build Project"
#define SET_COMMAND_LINE        "Set Command Line"             /* the desired command line contents */
#define RUN_PROJECT             "Run Project"
#define RUN_PROJECT_NO_SAVE     "Run Project (no save)"        /* user not prompted to save files */
#define APPEND_LINE_TO_IW       "Append Line To IW"            /* contents of line to add;  if leading white space needed, mark beginning with a percent (%) */
#define CLEAR_IW                "Clear IW"
#define BUILD_IW_PROGRAM        "Build IW Program"
#define RUN_IW                  "Run IW"
#define RUN_IW_NO_SAVE          "Run IW (no save)"             /* user not prompted to save files */
#define SUSPEND_EXECUTION       "Suspend Execution"
#define CONTINUE_EXECUTION      "Continue Execution"
#define TERMINATE_EXECUTION     "Terminate Execution"          /* if "atexit", functions registered with atexit() will be called */
#define GET_NUM_PROJECT_FILES   "Get Num Project Files"
#define GET_FILE_NAME           "Get File Name"                /* 0-based index of project file of which to return the simple file name */
#define CREATE_OBJECT_FILE      "Create Object File"               /* absolute path for a loaded .c file, or simple name for a .c file in the project */
#define CREATE_OBJS_ALL_CMPLRS  "Create Objects For All Compilers" /* absolute path for a loaded .c file, or simple name for a .c file in the project */
#define SET_EXECUTABLE_PATH     "Set Executable Path"          /* absolute path for the executable file */
#define CREATE_EXECUTABLE       "Create Executable"
#define IDENTIFY_FP_FILE        "Identify FP File"             /* absolute path for a loaded .fp file */
#define GET_NEXT_FP_FUNCTION    "Get Next FP Function"
#define GET_FP_PARAM_DATATYPE   "Get FP Param Datatype"        /* -1 for return value, 0 for first parameter, etc. */
#define GET_FP_PARAM_NAME       "Get FP Param Name"            /* -1 for return value, 0 for first parameter, etc. */
#define GET_FP_PARAM_PROTO_NAME "Get FP Param Proto Name"      /* -1 for return value, 0 for first parameter, etc. */
#define SHOW_FUNCTION_PANEL     "Show Function Panel"          /* name of function in function panel file */
#define CLOSE_FUNCTION_PANEL    "Close Function Panel"         /* name of function in function panel file */
#define IDENTIFY_FP_FUNCTION    "Identify FP Function"         /* name of function in function panel file */
#define GET_FP_PARAM_VALUE      "Get FP Param Value"           /* -1 for return value, 0 for first parameter, etc. */
#define SET_FP_PARAM_VALUE      "Set FP Param Value"           /* -1 for return value, 0 for first parameter, etc. */
#define COPY_FP_FUNCTION        "Copy FP Function"             /* name of function in function panel file */
#define CUT_FP_FUNCTION         "Cut FP Function"              /* name of function in function panel file */
#define PASTE_FP_FUNCTION       "Paste FP Function"
#define RENAME_FP_FUNCTION      "Rename FP Function"           /* new function name */
#define CHANGE_INSTR_NAME       "Change Instr Name"            /* new instrument name */
#define SET_WARNING_BOXES       "Set Warning Boxes"            /* 0 for disable, 1 for enable */
#define SET_UNLOAD_DLLS         "Set Unload DLLs"              /* 0 for disable, 1 for enable */
#define FAKE_KEYSTROKES         "Fake Keystrokes"              /* up to 64 blank-separated ASCII representations  */
                                                               /* of keystroke values (in hexadecimal format);    */
#define SET_KEYSTROKE_INTERVAL  "Set Keystroke Interval"       /* number of milliseconds to wait between processing keystrokes */
#define GET_NUM_KEYSTROKES      "Get Num Keystrokes"
#define GET_CVI_STATE           "Get CVI State"
#define EXIT_CVI                "Exit CVI"
#define EXIT_CVI_NO_SAVE        "Exit CVI (no save)"           /* user not prompted to save files */

/*=====================================================================*/
/* NOTE:  Most commands complete execution before the command server   */
/*        sends the response string.  The following commands           */
/*        are exceptions and are executed after the command server     */
/*        sends the response string:                                   */
/*              RUN_PROJECT                                            */
/*              RUN_INT_WIN                                            */
/*              SUSPEND_PROGRAM_EXEC                                   */
/*              CONTINUE_PROGRAM_EXEC                                  */
/*              TERMINATE_PROGRAM_EXEC                                 */
/*              EXIT_CVI                                               */
/*=====================================================================*/


/*=====================================================================*/
/* The following enum specifies the absolute value of the negative     */
/* error codes that can be returned by the command server.             */
/* Examples:                                                           */
/*    If the command contained no errors, the string "0" is returned.  */
/*    If the command name was invalid, the string "-2" is returned     */
/*    (the ASCII representation of -kCmdErr_BadCommandName).           */
/* Remember that "0" indicates only that the command was executed,     */
/* not that it succeeded.                                              */
/*=====================================================================*/
enum {
    kCmdErr_None=0,     /* no error */
    kCmdErr_CommandNameMissing,
    kCmdErr_BadCommandName,
    kCmdErr_PathnameMissing,
    kCmdErr_BadPathname,              /* invalid pathname syntax */
    kCmdErr_PathnameNotAbsolute,      /* example:  "c:\x\y.c" is an absolute path,  "x\y.c" is not */
    kCmdErr_PathnameNotAbsOrSimple,   /* example:  "y.c" is a simple file name,  "x\y.c" is not */
    kCmdErr_PathnameNotInProject,
    kCmdErr_FileDoesNotExist,
    kCmdErr_FileIsNotLoaded,
    kCmdErr_RunningUserProgram,
    kCmdErr_ExecutionIsSuspended,
    kCmdErr_NoProgramRunning,
    kCmdErr_NoExecutionSuspended,
    kCmdErr_NotRunningOrSuspended,
    kCmdErr_FileNotExcludable,
    kCmdErr_NotAnInstrument,
    kCmdErr_NotASourceOrTextFile,
    kCmdErr_NotACSourceFile,
    kCmdErr_WindowMustRemainOpen,    /* if execution is suspended in that window, and the project window is not visible */
    kCmdErr_LineTooLong,             /* lines in source windows are limited to 254 characters */
    kCmdErr_OutOfMemory,
    kCmdErr_WaitingForUserResponse,  /* CVI is waiting for user response to a dialog box */
    kCmdErr_NotAnFPFile,
    kCmdErr_NoFPFileIdentified,      /* need to send IDENTIFY_FP_FILE command */
    kCmdErr_NoFPFunctionIdentified,  /* need to send IDENTIFY_FP_FUNCTION command */
    kCmdErr_FunctionNameMissing,
    kCmdErr_FunctionNameNotInFPFile,
    kCmdErr_ParamIndexMissing,
    kCmdErr_NotAValidParamIndex,     /* must be integer greater than or equal to -1 */
    kCmdErr_UnableToReadFPFile,
    kCmdErr_NewInstrumentNameMissing,
    kCmdErr_NewFunctionNameMissing,
    kCmdErr_MiscErrReportedToUser,   /* action-specific error reported to end-user via dialog box */
    kCmdErr_InvalidIntegerValue,
    kCmdErr_NotAValidProjectFileIndex,
    kCmdErr_ProjectIsUntitled,
    kCmdErr_NotASourceFile,
    kCmdErr_NotAUIRFile,
    kCmdErr_NotAProjectFile,
    kCmdErr_KeystrokeQueueOverflow,     /* maximum unprocessed fake keystrokes is 256 */
    kCmdErr_InvalidKeystrokeInterval,
    kCmdErr_NewProjectFailed,
    kCmdErr_OpenProjectFailed,
    kCmdErr_FileNotIncludable
};

/*======================================================================*/
/* Commands that return information in addition to the error code       */
/* do so by appending them after the error code and at least one        */
/* white space character.   In general, the additional information      */
/* is present only if the error code is 0.                              */
/*                                                                      */
/* The following describes the additional information returned by       */
/* the commands that do so:                                             */
/*                                                                      */
/*    DOES_FILE_HAVE_CHANGES:                                           */
/*                     1 if file has unsaved changes, 0 otherwise.      */
/*                                                                      */
/*    GET_CVI_STATE:   a.  Whether the project is compiled and linked   */
/*                         (1 for yes, 0 for no).                       */
/*                     b.  The execution state for the project          */
/*                         (2 for suspended, 1 for executing,           */
/*                          0 otherwise).                               */
/*                     c.  Whether the interactive window is compiled   */
/*                         and linked.                                  */
/*                     d.  The execution state of the interactive       */
/*                         window (2 for suspended, 1 for executing,    */
/*                         0 otherwise).                                */
/*                     e.  Whether CVI is waiting for user response     */
/*                         to a dialog box.                             */
/*                     f.  The number of unprocessed keystrokes from    */
/*                         FAKE_KEYSTROKE commands.                     */
/*                                                                      */
/*      Example:  "0 1 2 1 1 0"                                         */
/*                 0 - the GET_CVI_STATE command was accepted as        */
/*                     valid by the CVI command server.                 */
/*                 1 - the project is compiled and linked.              */
/*                 2 - project execution is suspended.                  */
/*                 1 - the interactive window program is compiled       */
/*                     and linked.                                      */
/*                 1 - the interactive window program execution is      */
/*                     suspended.                                       */
/*                 0 - CVI is not waiting for user response to a        */
/*                     dialog box                                       */
/*                                                                      */
/*    GET_NEXT_FP_FUNCTION:                                             */
/*                 If no more functions, "0".                           */
/*                 Otherwise, "1" followed by function name.            */
/*                 Examples:  "0 0"  or "0 1 MyFunction"                */
/*                                                                      */
/*    GET_FP_PARAM_DATATYPE:                                            */
/*                 If no control with that parameter index, "0".        */
/*                 Otherwise, "1" followed by the C data type of the    */
/*                 parameter.  For example, if the function panel       */
/*                 control is an input control of type 'int',           */
/*                 then "0 1 int" would be returned.  If the function   */
/*                 panel control is an output control of type 'int',    */
/*                 "0 1 int *" would be returned.                       */
/*                 If the function panel control is an input or output  */
/*                 control of type 'int []', "0 1 int []" would be      */
/*                 returned.                                            */
/*                                                                      */
/*    GET_FP_PARAM_NAME:                                                */
/*                 If no control with that parameter index, "0".        */
/*                 Otherwise, "1" followed by the control label.        */
/*                                                                      */
/*    GET_FP_PARAM_PROTO_NAME:                                          */
/*                 If no control with that parameter index, "0".        */
/*                 Otherwise, "1" followed by the name would appear     */
/*                 in the function prototype if you selected            */
/*                 the Generate Function Prototypes command from the    */
/*                 Options menu of the Function Tree Editor.            */
/*                                                                      */
/*    GET_FP_PARAM_VALUE:                                               */
/*                 If no control with that parameter index, "0".        */
/*                 Otherwise, "1" followed by the string that           */
/*                 is in the function panel control.                    */
/*                 More precisely, the string which appears for the     */
/*                 control in the generated code at the bottom of the   */
/*                 function panel window.                               */
/*                 Note:  The string may be empty.                      */
/*                                                                      */
/*    SET_FP_PARAM_VALUE:                                               */
/*                 If no control with that parameter index, "0".        */
/*                 Otherwise, "1".                                      */
/*                                                                      */
/*    GET_NUM_PROJECT_FILES:                                            */
/*                 The number of files in the project.                  */
/*                                                                      */
/*    GET_FILE_NAME:                                                    */
/*                 The file name (excluding the directory path)         */
/*                 of the file in the project identified by the         */
/*                 zero-based index.                                    */
/*                                                                      */
/*    GET_EXCLUSION_STATE:                                              */
/*                 "1" if file is excluded, "0" if not                  */
/*                                                                      */
/*    CREATE_OBJECT_FILE:                                               */
/*                 "1" if object file was successfully created,         */
/*                 "0" if not                                           */
/*                                                                      */
/*    CREATE_EXECUTABLE:                                                */
/*                 "1" if executable file successfully created,         */
/*                 "0" if not                                           */
/*                                                                      */
/*    GET_NUM_KEYSTROKES:                                               */
/*                 The number of fake keystrokes sent in previous       */
/*                 commands but not yet processed.                      */
/*                                                                      */
/*======================================================================*/

enum {
    kExecution_No,
    kExecution_Running,
    kExecution_Suspended
};
