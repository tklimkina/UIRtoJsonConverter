/*--------------------------------------------------------------------------*/
/*  MENUUTIL.H                                                              */
/*--------------------------------------------------------------------------*/
#ifndef INC_MENUUTIL
#define INC_MENUUTIL 1


/*--------------------------------------------------------------------------*/
/* Include files                                                            */
/*--------------------------------------------------------------------------*/
#include "stdarg.h" /* for va_list */
#include <stddef.h> /* for "size_t" */
#include "inifile.h"


/*--------------------------------------------------------------------------*/
/* Menu List Utility functions                                              */
/*--------------------------------------------------------------------------*/
/* Menu List Attributes */
#define ATTR_MENULIST_MIN                   1
#define ATTR_MENULIST_MAX_NUM_ITEMS         1
#define ATTR_MENULIST_UPPER_SEPARATOR       2
#define ATTR_MENULIST_LOWER_SEPARATOR       3
#define ATTR_MENULIST_ONE_CHECK_ITEM        4
#define ATTR_MENULIST_CHECK_WHEN_ADDED      5
#define ATTR_MENULIST_ALLOW_DUPLICATE_ITEMS 6
#define ATTR_MENULIST_MENUBAR_HANDLE        7
#define ATTR_MENULIST_MENU_ID               8
#define ATTR_MENULIST_BEFORE_MENU_ITEM      9
#define ATTR_MENULIST_CALLBACK_FUNCTION     10
#define ATTR_MENULIST_APPEND_SHORTCUT       11

#define ATTR_MENULIST_FIRST_ITEM_ATTR       12
/* Menu List Item Attributes */

#define ATTR_MENULIST_ITEM_MENU_ID          12
#define ATTR_MENULIST_ITEM_NAME             13
#define ATTR_MENULIST_ITEM_NAME_LENGTH      14
#define ATTR_MENULIST_ITEM_CALLBACK_DATA    15
#define ATTR_MENULIST_ITEM_CHECKED          16

#define ATTR_MENULIST_MAX                   16

typedef int menuList;
typedef void (CVICALLBACK * MenuListCallbackPtr)(menuList list, int menuIndex, int event, void *callbackData);

extern menuList CVIFUNC   MU_CreateMenuList(int menuBarHandle, int menuID, int beforeMenuItemID, int maxItems, MenuListCallbackPtr callbackFunc);
extern int      CVIFUNC   MU_DeleteMenuList(menuList handle);
extern int      CVIFUNC   MU_AddItemToMenuList(menuList handle, int index, char *menuItemName, void *callbackData);
extern int      CVIFUNC   MU_DeleteMenuListItem(menuList handle, int index);
extern int      CVIFUNC   MU_GetNumMenuListItems(menuList handle);

extern int      CVIFUNC   MU_GetMenuListAttribute (menuList list, int menuListIndex, int attribute, void *value);
extern int      CVIFUNC_C MU_SetMenuListAttribute (menuList list, int menuListIndex, int attribute, ...);
extern int      CVIFUNC   MU_SetMenuListAttributeFromParmInfo (menuList list, int menuListIndex, int attribute, va_list parmInfo);

extern int      CVIFUNC   MU_PutFileListInIniFile(menuList menuListHandle, IniText iniTextHandle, char *sectionName, char *tagPrefix, int baseTagNameToUse);
extern int      CVIFUNC   MU_GetFileListFromIniFile(menuList menuListHandle, IniText iniTextHandle, char *sectionName, char *tagPrefix, int flags);


/*--------------------------------------------------------------------------*/
/* INIFILE Instrument driver extensions                                     */
/*--------------------------------------------------------------------------*/
extern int CVIFUNC MU_ReadRegistryInfo (IniText iniTextHandle, char *registryName);
extern int CVIFUNC MU_WriteRegistryInfo(IniText iniTextHandle, char *registryName);

extern char * CVIFUNC MU_MakeShortFileName(char *userOutBuf, char *userInBuf, int max);

#endif /* INC_MENUUTIL */
