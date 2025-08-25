/*--------------------------------------------------------------*/
/*  MENUUTIL.C                                                  */
/*--------------------------------------------------------------*/

/*--------------------------------------------------------------*/
/* #include                                                     */
/*--------------------------------------------------------------*/
#ifdef WIN32
  #include <windows.h>
#endif

#include "utility.h"
#include "menuutil.h"

/*--------------------------------------------------------------*/
/* #defines                                                     */
/*--------------------------------------------------------------*/
#define MAX_MENU_ITEM_LENGTH 32

typedef struct menuInfo
{
    menuList handle;
    int menuBarHandle;
    int menuID;
    int beforeMenuItemID;
    int maxItems;
    MenuListCallbackPtr callbackFunc;
    int separator1;
    int separator2;
    int separator1ID;
    int separator2ID;
    int oneCheckItem;
    int checkWhenAdded;
    int allowDuplicates;
    int preAppendShortCut;
    ListType menuItemInfoList;
} menuInfoRec;

typedef struct menuItemInfo
{
    char  menuName[MAX_MENU_ITEM_LENGTH+1];
    int   menuItemID;
    void *callbackData;
} menuItemInfoRec;

static ListType sMenuInfoList = NULL;

#if _CVI_DEBUG_
static menuInfoRec * sMenuInfoPtr = NULL; // Global pointer for debug puposes
#endif

/*--------------------------------------------------------------*/
/* exported prototypes                                          */
/*--------------------------------------------------------------*/


/*--------------------------------------------------------------*/
/* static prototypes                                            */
/*--------------------------------------------------------------*/
static int FindMenuInfoInList(ListType menuInfoList, menuList handle);

static int FindMenuItemInfoFromMenuName(ListType menuInfoList, char *longName);

static int FindMenuItemInfoFromMenuItemID(ListType menuInfoList, int menuItemID);

static int UpdateMenuItems(menuInfoRec *menuInfoPtr);

static int RemoveMenuList(menuInfoRec *menuInfoPtr);
static int RemoveMenuListItem(menuInfoRec *menuInfoPtr, int index);
static int UncheckAllCheckedItems(menuInfoRec *menuInfoPtr, int doNotUncheckFirstFound);
static int RemoveDuplicates(menuInfoRec *menuInfoPtr);

static int IniEx_PutRawStringListItem(IniText iniTextHandle, char *sectionName,
                           char *tagPrefix, char *tagValue, int maxItems);

#ifdef WIN32
static int IniEx_ReadFromRegistry(IniText iniTextHandle, int rootKey, char *baseKeyName);
static int IniEx_WriteToRegistry(IniText iniTextHandle, int rootKey, char *baseKeyName, int removeNonListTags);
#endif

/*****************************************************/
/*  GenerateShortFileName()                          */
/*                                                   */
/*  Parameters:                                      */
/*      output buffer to place short filename        */
/*      input buffer with long filename              */
/*      max characters to place in output bugger     */
/*        (This does not include the ending NUL)     */
/*  Return: pointer to outbuffer                     */
/*                                                   */
/*  Purpose: Routine to generate a short filename    */
/*           from a long filename                    */
/*                                                   */
/*  Example:                                         */
/*    LongName  = c:\dir1\dir2\dir3\file.txt         */
/*    ShortName = c:\...dir3\file.txt                */
/*                                                   */
/*****************************************************/
char * CVIFUNC MU_MakeShortFileName(
    char *userOutBuf,
    char *userInBuf,
    int max)
{
    int size, sizeOfFilename, sizeOfPrefix;
    char *startOfFilename = NULL;
    char *first = NULL;
    static char outBuf[260];
    char inBuf[260];

#ifdef _NI_mswin_
    int byte=92; /* '\' */
#else
    int byte=47; /* '/' */
#endif

#ifdef _NI_mswin_
    /* Windows - outbuf must at least be able to handle "x:\...\A... */
    if (max<11) {                                  /*    01234567890 */
        goto Error;
    }
#endif

    /* Use internal Out buffer if non supplied */
    if (userOutBuf)
        strcpy(outBuf, userOutBuf);

    /* Verify input buffers */
    if ( (!userInBuf) || (max < 1) ) {
        goto Error;
    }

    if (userInBuf[0]==0) {
        outBuf[0]=0;
        goto Done;
    } else
        strcpy(inBuf, userInBuf);

    /* We want to make the first drive letter upper case if using Windows */
#ifdef _NI_mswin_
    if ((inBuf[0]>96) && (inBuf[0]<123))
            inBuf[0] = inBuf[0] - 32;
#endif

    /* If inbuf is smaller than max, just copy value into outbuf */
    size = strlen(inBuf);
    if (size<=max) {
        strncpy(outBuf, inBuf, size);
        outBuf[size] = 0;
    } else {
        /* Find just the filename and its size */
        startOfFilename = strrchr (inBuf, byte); /* Last */
        startOfFilename++;
        sizeOfFilename = strlen(startOfFilename);

        /* Find the first part that we want to keep */
#ifdef _NI_mswin_
        sizeOfPrefix = 3;
#else
        /* Find first '\' before filename within max limit. */
        first = strchr(&inBuf[2], byte);
        if (!first)
             sizeOfPrefix = 0;
        else sizeOfPrefix = (first-inBuf+1);
#endif

        /* Copy the prefix ("x:\" or /xyz/) to outbuf */
        strncpy (outBuf, inBuf, sizeOfPrefix);
        outBuf[sizeOfPrefix]=0;
        /* Copy "...\" to end of outbuf, and add NULL */
#ifdef _NI_mswin_
        strncpy(&outBuf[sizeOfPrefix], "...\\", 5);
#else
        strncpy(&outBuf[sizeOfPrefix], ".../", 5);
#endif

        /* If base filename is larger than max-7 just place what you can into outbuf with '...' on end */
        if (sizeOfFilename>max-sizeOfPrefix-4) {
            strncpy(&outBuf[sizeOfPrefix+4], startOfFilename, max-sizeOfPrefix-7);
            /* Copy "..." to end of outbuf, and add NULL */
            strncpy(&outBuf[max-3], "...", 4);
        } else {
            /* Find first '\' after prefix and before filename within max limit. */
            first = strchr(&inBuf[size-max+sizeOfPrefix+3], byte);  /* First */

            if (!first)
                first = &inBuf[size-max+sizeOfPrefix+3];

            strncpy(&outBuf[sizeOfPrefix+3], first, max-sizeOfPrefix-2);
        }
    }
Done:
    if (userOutBuf) {
        strcpy(userOutBuf, outBuf);
        return userOutBuf;
    } else
        return outBuf;
Error:
    return NULL;
}


/*****************************************************/
/*  FindMenuInfoInList()                             */
/*                                                   */
/*  Parameters:                                      */
/*      menuInfoList handle                          */
/*      menuList                                     */
/*                                                   */
/*  Return: index into list for found item           */
/*                                                   */
/*  Purpose: Routine to search the menuInfo list     */
/*           for the index corresponding to a handle */
/*                                                   */
/*****************************************************/
static int FindMenuInfoInList(
    ListType menuInfoList,
    menuList handle)
{
    int i, totalItems;
    menuInfoRec * menuInfoPtr = NULL;

    if (handle) {
        /* Loop to find handle in list */
        totalItems = ListNumItems (menuInfoList);
        for (i=1;(i<=totalItems);i++) {
            menuInfoPtr = ListGetPtrToItem (menuInfoList, i);
            if ((menuInfoPtr) && (menuInfoPtr->handle==handle) ) {
                return i;
            }
        }
    }
    return 0;
}


/*****************************************************/
/*  FindMenuItemInfoFromMenuItemID()                 */
/*                                                   */
/*  Parameters:                                      */
/*      menuItemInfoList handle                      */
/*      menu item id                                 */
/*                                                   */
/*  Return: index into list for found item           */
/*                                                   */
/*  Purpose: Routine to search the menuItemInfo list */
/*           for the longName passed in              */
/*                                                   */
/*****************************************************/
static int FindMenuItemInfoFromMenuItemID(
    ListType menuItemInfoList,
    int menuItemID)
{
    int i, totalItems;
    menuItemInfoRec * menuItemInfoPtr = NULL;

    if (menuItemInfoList) {
        /* Loop to find handle in list */
        totalItems = ListNumItems (menuItemInfoList);
        for (i=1;(i<=totalItems);i++) {
            menuItemInfoPtr = ListGetPtrToItem (menuItemInfoList, i);
            if ((menuItemInfoPtr) && (menuItemID == menuItemInfoPtr->menuItemID) ) {
                return i;
            }
        }
    }
    return 0;
}

/*****************************************************/
/*  FindMenuItemInfoFromMenuName()                   */
/*                                                   */
/*  Parameters:                                      */
/*      menuItemInfoList handle                      */
/*      buffer for longName to search for            */
/*                                                   */
/*  Return: index into list for found item           */
/*                                                   */
/*  Purpose: Routine to search the menuItemInfo list */
/*           for the longName passed in              */
/*                                                   */
/*****************************************************/
static int FindMenuItemInfoFromMenuName(
    ListType menuItemInfoList,
    char *menuName)
{
    int i, totalItems;
    menuItemInfoRec * menuItemInfoPtr = NULL;

    if ((menuItemInfoList) && (menuName)) {
        /* Loop to find handle in list */
        totalItems = ListNumItems (menuItemInfoList);
        for (i=1;(i<=totalItems);i++) {
            menuItemInfoPtr = ListGetPtrToItem (menuItemInfoList, i);
            if ((menuItemInfoPtr) && (menuItemInfoPtr->menuName) && (!strcmp(menuName, menuItemInfoPtr->menuName)) ) {
                return i;
            }
        }
    }
    return 0;
}



/*****************************************************/
/*  InternalMenuCallback()                           */
/*                                                   */
/*  Parameters:                                      */
/*      menuBarHandle                                */
/*      menuItemID                                   */
/*      callbackData                                 */
/*      panel                                        */
/*                                                   */
/*  Purpose: Call user's menuList callback functions */
/*                                                   */
/*****************************************************/
static void CVICALLBACK InternalMenuCallback (int menuBarHandle, int menuItemID, void *callbackData, int panel)
{
    menuInfoRec * menuInfoPtr = NULL;
    menuItemInfoRec * menuItemInfoPtr = NULL;
    MenuListCallbackPtr callbackFunc;
    int index;
    menuList handle;

    /* Get menulist pointer from handle */
    if ((handle           = (menuList) callbackData) &&
        ((index           = FindMenuInfoInList (sMenuInfoList, handle))>0) &&
        (menuInfoPtr      = ListGetPtrToItem (sMenuInfoList, index)) &&
        (index            = FindMenuItemInfoFromMenuItemID (menuInfoPtr->menuItemInfoList, menuItemID)) &&
        (menuItemInfoPtr  = ListGetPtrToItem (menuInfoPtr->menuItemInfoList, index)) )
    {
        if (callbackFunc = menuInfoPtr->callbackFunc)
            (* callbackFunc) (handle, index, EVENT_COMMIT, menuItemInfoPtr->callbackData);
    }

    return;
}

/*****************************************************/
/*  CreateMenuList()                                 */
/*                                                   */
/*  Parameters:                                      */
/*      menuBarHandle                                */
/*      menuID                                       */
/*      beforeMenuItemID                             */
/*      maxItems                                     */
/*      callbackFunc                                 */
/*                                                   */
/*  Return: menuList                                 */
/*                                                   */
/*  Purpose: Create a menu list in an existing       */
/*           menubar                                 */
/*                                                   */
/*  Example:                                         */
/*    FILE                                           */
/*      Open                                         */
/*      Save                                         */
/*      ------------                                 */
/*      c:\file1.txt                                 */
/*      d:\file2.txt                                 */
/*      e:\file3.txt                                 */
/*      ------------                                 */
/*      Exit                                         */
/*****************************************************/
menuList CVIFUNC MU_CreateMenuList(int menuBarHandle, int menuID, int beforeMenuItemID, int maxItems, MenuListCallbackPtr callbackFunc)
{
    int static lastmenuList = 0;
    menuList handle = 0;
    int error = 0;
    int i, totalItems;
    menuInfoRec * menuInfoPtr = NULL;

    /* Create list of menuInfo if not already created */
    if (!sMenuInfoList)
        sMenuInfoList = ListCreate (sizeof(menuInfoRec));

    if (!sMenuInfoList) {
        error = -1;
        goto Error;
    }

    /* Loop to find if menuInfo already exists in list */
    totalItems = ListNumItems (sMenuInfoList);
    for (i=1;((i<=totalItems) && (!handle));i++) {
        menuInfoPtr = ListGetPtrToItem (sMenuInfoList, i);
        if ((menuBarHandle    == menuInfoPtr->menuBarHandle) &&
            (menuID           == menuInfoPtr->menuID)        &&
            (beforeMenuItemID == menuInfoPtr->beforeMenuItemID) ) {
            handle = menuInfoPtr->handle;
        }
        menuInfoPtr = NULL;
    }

    /* Add new menuInfo to List if not found */
    if (!handle) {
        if (menuInfoPtr = calloc(sizeof(menuInfoRec), 1) ) {
            if (menuInfoPtr->menuItemInfoList = ListCreate (sizeof(menuItemInfoRec)) ) {
                menuInfoPtr->menuBarHandle    = menuBarHandle;
                menuInfoPtr->menuID           = menuID;
                menuInfoPtr->beforeMenuItemID = beforeMenuItemID;
                menuInfoPtr->maxItems         = maxItems;
                menuInfoPtr->callbackFunc     = callbackFunc;
                menuInfoPtr->handle = handle  = ++lastmenuList;
// Commented out to prevent dupicate separators
//                menuInfoPtr->separator1       = 1;
//                menuInfoPtr->separator2       = 1;
//                menuInfoPtr->separator1ID     = 0;
//                menuInfoPtr->separator2ID     = 0;
                menuInfoPtr->allowDuplicates  = 1;
                menuInfoPtr->preAppendShortCut= 0;

                if (!ListInsertItem (sMenuInfoList, menuInfoPtr, END_OF_LIST))
                    error = -1;
#if _CVI_DEBUG_
                else sMenuInfoPtr = ListGetPtrToItem (sMenuInfoList, END_OF_LIST);
#endif

            } else error = -1;

            if (menuInfoPtr) {
                free (menuInfoPtr);
                menuInfoPtr = NULL;
            }
        }
        else error = -1;
    }

    return handle;
Error:
    /* free memory */
    if (menuInfoPtr) {
        free (menuInfoPtr);
        menuInfoPtr = NULL;
    }

    return error;
}

/*****************************************************/
/*  MU_GetNumMenuListItems()                         */
/*                                                   */
/*  Parameters:                                      */
/*      menuList from CreateMenuList()               */
/*                                                   */
/*  Return: success = 1                              */
/*                                                   */
/*  Purpose: Delete and remove a menu list from      */
/*           menubar                                 */
/*                                                   */
/*****************************************************/
int CVIFUNC MU_GetNumMenuListItems(
    menuList handle)
{
    int numItems = 0;
    int index;
    menuInfoRec * menuInfoPtr = NULL;

    if (handle) {
        if (index = FindMenuInfoInList(sMenuInfoList, handle)) {
            menuInfoPtr = ListGetPtrToItem (sMenuInfoList, index);
            if ((menuInfoPtr) && (menuInfoPtr->handle) )
                numItems = ListNumItems (menuInfoPtr->menuItemInfoList);
        }
    }

    return numItems;
}



/*****************************************************/
/*  DeleteMenuList()                                 */
/*                                                   */
/*  Parameters:                                      */
/*      menuList from CreateMenuList()               */
/*                                                   */
/*  Return: success = 1                              */
/*                                                   */
/*  Purpose: Delete and remove a menu list from      */
/*           menubar                                 */
/*                                                   */
/*****************************************************/
int CVIFUNC MU_DeleteMenuList(
    menuList handle)
{
    int success = 1;
    int index;
    int totalItems;
    menuInfoRec * menuInfoPtr = NULL;

    if (handle) {
        if (index = FindMenuInfoInList(sMenuInfoList, handle)) {
            menuInfoPtr = ListGetPtrToItem (sMenuInfoList, index);
            if ((menuInfoPtr) && (menuInfoPtr->handle) ) {
                if (RemoveMenuList(menuInfoPtr) )
                    menuInfoPtr->menuItemInfoList = NULL;
                else success = 0;

                ListRemoveItem (sMenuInfoList, 0, index);
            } else success = 0;
        } else success = 0;
    } else success = 0;

    /* Delete list of menuInfo when empty */
    totalItems = ListNumItems (sMenuInfoList);
    if (!totalItems) {
        /* Remove separators and list items from the user's menubar */
        UpdateMenuItems(menuInfoPtr);

        /* Dispose of list */
        ListDispose(sMenuInfoList);
        sMenuInfoList = NULL;
    }

    return success;
}


/*****************************************************/
/*  RemoveMenuList()                                 */
/*                                                   */
/*  Parameters:                                      */
/*      pointer to menu info                         */
/*                                                   */
/*  Return: success = 1                              */
/*                                                   */
/*  Purpose: Delete menuItemList                     */
/*                                                   */
/*****************************************************/
static int RemoveMenuList(
    menuInfoRec *menuInfoPtr)
{
    int success = 1;
    int i, totalItems;


    if ((!menuInfoPtr) && (!menuInfoPtr->menuItemInfoList) )
        goto Error;

    /* Loop thru items in list and free internal malloc'ed memory */
    totalItems = ListNumItems (menuInfoPtr->menuItemInfoList);
    for (i=totalItems;((i>0) && (success) );i--) {
        success = RemoveMenuListItem(menuInfoPtr, i);
    }

    ListDispose(menuInfoPtr->menuItemInfoList);

Error:
    return success;
}

/*****************************************************/
/*  RemoveMenuListItem()                             */
/*                                                   */
/*  Parameters:                                      */
/*      pointer to menu info                         */
/*      index                                        */
/*                                                   */
/*  Return: success = 1                              */
/*                                                   */
/*  Purpose: Delete menuItemList                     */
/*                                                   */
/*****************************************************/
static int RemoveMenuListItem(
    menuInfoRec *menuInfoPtr,
    int index)
{
    int success = 1;

    menuItemInfoRec *menuItemInfoPtr = NULL;

    /* Verify parameters */
    if ((!menuInfoPtr) && (!menuInfoPtr->menuItemInfoList) )
        goto Error;

    /* Loop thru items in list and free internal malloc'ed memory */
    menuItemInfoPtr = ListGetPtrToItem (menuInfoPtr->menuItemInfoList, index);

    if (menuItemInfoPtr) {
        /* Call the user's callback function */
        if (menuInfoPtr->callbackFunc)
            (*(menuInfoPtr->callbackFunc)) (menuInfoPtr->handle, index, EVENT_DISCARD, menuItemInfoPtr->callbackData);
        /* Remove menu Item if it exists */
        if (!DiscardMenuItem (menuInfoPtr->menuBarHandle, menuItemInfoPtr->menuItemID))
            menuItemInfoPtr->menuItemID = 0;

        ListRemoveItem (menuInfoPtr->menuItemInfoList, 0, index);
    } else
        success = 0;

Error:
    return success;
}

/*****************************************************/
/*  MU_DeleteMenuListItem()                          */
/*                                                   */
/*  Parameters:                                      */
/*      menuList from CreateMenuList()               */
/*      index                                        */
/*                                                   */
/*  Return: success = 1                              */
/*                                                   */
/*  Purpose: Delete menuItemList                     */
/*                                                   */
/*****************************************************/
int CVIFUNC MU_DeleteMenuListItem(
    menuList handle,
    int index)
{
    int success = 0;

    menuInfoRec * menuInfoPtr = NULL;
    int item;

    if ( ((item        = FindMenuInfoInList (sMenuInfoList, handle))>0) &&
         (menuInfoPtr = ListGetPtrToItem (sMenuInfoList, item)) )  {
        success = RemoveMenuListItem(menuInfoPtr, index);
    }

    return 0;
}



/*****************************************************/
/*  UpdateMenuItems()                                */
/*                                                   */
/*  Parameters:                                      */
/*      menuItemInfoList pointer                     */
/*                                                   */
/*  Return: success = 1                              */
/*                                                   */
/*  Purpose: Routine to update the menubar associated*/
/*           with the menuItemInfoPtr                */
/*                                                   */
/*****************************************************/
static int UpdateMenuItems(
    menuInfoRec *menuInfoPtr)
{
    int success = 1;
    int i, totalItems, beforeMenuItemID, checked;
    menuItemInfoRec * menuItemInfoPtr = NULL;
    char tempName[MAX_MENU_ITEM_LENGTH + 6];

    if (menuInfoPtr) {
        /* Remove items greater than maxItems */
        totalItems = ListNumItems (menuInfoPtr->menuItemInfoList);
        for (i=totalItems;(i>menuInfoPtr->maxItems);i--)
            RemoveMenuListItem(menuInfoPtr, i);

        /* Get number of menu items in list */
        totalItems = ListNumItems (menuInfoPtr->menuItemInfoList);
        if (totalItems) {
            /* Setup before menu item id for first item */
            beforeMenuItemID = menuInfoPtr->beforeMenuItemID;

            /* Create Lower Separator */
            if (menuInfoPtr->separator2ID) {
                /* Discard Lower Separator */
                DiscardMenuItem (menuInfoPtr->menuBarHandle, menuInfoPtr->separator2ID);
                menuInfoPtr->separator2ID = 0;
            }
            if (menuInfoPtr->separator2) {
                if (!menuInfoPtr->separator2ID) {
                    menuInfoPtr->separator2ID =
                            InsertSeparator(menuInfoPtr->menuBarHandle,
                                            menuInfoPtr->menuID,
                                            beforeMenuItemID);
                }
                beforeMenuItemID = menuInfoPtr->separator2ID;
            }

            /* Loop thru menu items in reverse order */
            for (i=totalItems;(i>0);i--) {
                /* Get ith list item */
                if (menuItemInfoPtr = ListGetPtrToItem (menuInfoPtr->menuItemInfoList, i)) {
                    /* Remove previous menu item */
                    if (menuItemInfoPtr->menuItemID) {
                        GetMenuBarAttribute (menuInfoPtr->menuBarHandle, menuItemInfoPtr->menuItemID, ATTR_CHECKED, &checked);
                        DiscardMenuItem (menuInfoPtr->menuBarHandle, menuItemInfoPtr->menuItemID);
                        menuItemInfoPtr->menuItemID = 0;
                    }
                    else checked = menuInfoPtr->checkWhenAdded;

                    /* Create new menu item */
                    /* Determine the visible name for the menu item */
                    if (menuInfoPtr->preAppendShortCut) {
                        if (i<10)
                            sprintf(tempName, "__%d %s", i%10, menuItemInfoPtr->menuName);
                        else if (i<36)
                            sprintf(tempName, "__%c %s", (75-i), menuItemInfoPtr->menuName);
                        else strcpy(tempName, menuItemInfoPtr->menuName);
                    }
                    else strcpy(tempName, menuItemInfoPtr->menuName);

                    menuItemInfoPtr->menuItemID = NewMenuItem (
                                             menuInfoPtr->menuBarHandle,
                                             menuInfoPtr->menuID,
                                             tempName,
                                             beforeMenuItemID,
                                             0,
                                             InternalMenuCallback,
                                             (void *) menuInfoPtr->handle);
                    SetMenuBarAttribute (menuInfoPtr->menuBarHandle, menuItemInfoPtr->menuItemID, ATTR_CHECKED, checked);
                }
                else success = 0;

                /* Setup before menu item id for next item */
                beforeMenuItemID = menuItemInfoPtr->menuItemID;
            }

            /* Create Upper Separator */
            if (menuInfoPtr->separator1ID) {
                /* Discard Upper Separator */
                DiscardMenuItem (menuInfoPtr->menuBarHandle, menuInfoPtr->separator1ID);
                menuInfoPtr->separator1ID = 0;
            }
            if (menuInfoPtr->separator1) {
                if (!menuInfoPtr->separator1ID) {
                    menuInfoPtr->separator1ID = InsertSeparator(menuInfoPtr->menuBarHandle,
                                                            menuInfoPtr->menuID,
                                                            beforeMenuItemID);
                }
                beforeMenuItemID = menuInfoPtr->separator1ID;
            }
        } else {
            /* Get rid of separators if they exist */
            if (menuInfoPtr->separator1ID) {
                if (!DiscardMenuItem (menuInfoPtr->menuBarHandle, menuInfoPtr->separator1ID))
                    menuInfoPtr->separator1ID = 0;
            }
            if (menuInfoPtr->separator2ID) {
                if (!DiscardMenuItem (menuInfoPtr->menuBarHandle, menuInfoPtr->separator2ID))
                    menuInfoPtr->separator2ID = 0;
            }
        }
    } else
        success = 0;

    return success;
}

/*****************************************************/
/*  UncheckAllCheckedItems()                         */
/*                                                   */
/*  Parameters:                                      */
/*      pointer to menu info                         */
/*      whether to remove the check from the first   */
/*          item found                               */
/*                                                   */
/*  Return: index into list for found item           */
/*                                                   */
/*  Purpose: Routine to search the menuItemInfo list */
/*           for the longName passed in              */
/*                                                   */
/*****************************************************/
static int UncheckAllCheckedItems(
    menuInfoRec *menuInfoPtr,
    int doNotUncheckFirstFound)
{
    int i, totalItems, checked = 0;
    menuItemInfoRec * menuItemInfoPtr = NULL;

    if (menuInfoPtr->menuItemInfoList) {
        /* Loop to find handle in list */
        totalItems = ListNumItems (menuInfoPtr->menuItemInfoList);
        for (i=1;(i<=totalItems);i++) {
            menuItemInfoPtr = ListGetPtrToItem (menuInfoPtr->menuItemInfoList, i);
            if ((menuItemInfoPtr) && (menuItemInfoPtr->menuItemID)) {
                if (doNotUncheckFirstFound) {
                    GetMenuBarAttribute (menuInfoPtr->menuBarHandle, menuItemInfoPtr->menuItemID, ATTR_CHECKED, &checked);
                    if (checked)
                        doNotUncheckFirstFound = 0;
                }
                else SetMenuBarAttribute (menuInfoPtr->menuBarHandle, menuItemInfoPtr->menuItemID, ATTR_CHECKED, 0);
            }
        }
    }
    return 0;
}


/*****************************************************/
/*  RemoveDuplicates()                               */
/*                                                   */
/*  Parameters:                                      */
/*      menuItemInfoList handle                      */
/*                                                   */
/*  Return: index into list for found item           */
/*                                                   */
/*  Purpose: Routine to search the menuItemInfo list */
/*           for the longName passed in              */
/*                                                   */
/*****************************************************/
static int RemoveDuplicates(
    menuInfoRec *menuInfoPtr)
{
    int i, j, totalItems;
    menuItemInfoRec * menuItemInfoPtr1 = NULL;
    menuItemInfoRec * menuItemInfoPtr2 = NULL;

    if (menuInfoPtr->menuItemInfoList) {
        /* Loop to find handle in list */
        totalItems = ListNumItems (menuInfoPtr->menuItemInfoList);
        for (i=1;(i<=totalItems);i++) {
            menuItemInfoPtr1 = ListGetPtrToItem (menuInfoPtr->menuItemInfoList, i);

            for (j=i+1;(j<=totalItems);j++) {
                menuItemInfoPtr2 = ListGetPtrToItem (menuInfoPtr->menuItemInfoList, j);
                if (!strcmp(menuItemInfoPtr1->menuName, menuItemInfoPtr1->menuName)) {
                    /* Remove item */
                    RemoveMenuListItem(menuInfoPtr, j);
                    totalItems--;
                }
            }
        }
    }
    return 0;
}


/*****************************************************/
/*  MU_AddItemToMenuList()                           */
/*                                                   */
/*  Parameters:                                      */
/*      menuList                                     */
/*      index location to insert                     */
/*      menu name to appear in menu                  */
/*      callback data to be sent to callback         */
/*                                                   */
/*  Return: success = 1                              */
/*                                                   */
/*  Purpose: Routine to Add a name to the menulist.  */
/*           This will also update the menubar       */
/*                                                   */
/*****************************************************/
int CVIFUNC MU_AddItemToMenuList(
    menuList handle,
    int index,
    char *menuItemName,
    void *callbackData)
{
    int success = 1, item;
    menuInfoRec * menuInfoPtr = NULL;
    menuItemInfoRec * menuItemInfoPtr = NULL;

    if ((!menuItemName) || (!menuItemName[0]) ) {
        success = 0;
        goto Error;
    }

    /* Find the menuInfo in list */
    if (item = FindMenuInfoInList(sMenuInfoList, handle)) {
        /* Get menuInfo from list */
        menuInfoPtr = ListGetPtrToItem (sMenuInfoList, item);
        if ((menuInfoPtr) && (menuInfoPtr->menuItemInfoList) ) {
            /* List found, so create memory for new element */
            if (menuItemInfoPtr = calloc(sizeof(menuItemInfoRec), 1)) {
                if (!menuInfoPtr->allowDuplicates) {
                    /* See if item already exists in list */
                    if (item = FindMenuItemInfoFromMenuName(menuInfoPtr->menuItemInfoList, menuItemName)) {
                    /* Remove the found item */
                        RemoveMenuListItem(menuInfoPtr, item);

                        if ((index!=FRONT_OF_LIST) && (index!=END_OF_LIST) && (index>item)) {
                            index--;
                            if (index==0)
                                index = FRONT_OF_LIST;
                        }
                    }
                }

                /* Fill in new item */
                strncpy(menuItemInfoPtr->menuName, menuItemName, MAX_MENU_ITEM_LENGTH);
                menuItemInfoPtr->menuName[MAX_MENU_ITEM_LENGTH]=0;
                menuItemInfoPtr->callbackData = callbackData;
                menuItemInfoPtr->menuItemID = 0;

                /* Add new item to the list */
                if ((success) && (!ListInsertItem (menuInfoPtr->menuItemInfoList, menuItemInfoPtr, index)) )
                    success = 0;

                /* free temporary memory */
                if (menuItemInfoPtr) {
                    free (menuItemInfoPtr);
                    menuItemInfoPtr = NULL;
                }
            } else success = 0;
        } else success = 0;
    } else success = 0;

    /* Remove all checked items */
    if ((menuInfoPtr->oneCheckItem) && (menuInfoPtr->checkWhenAdded))
        UncheckAllCheckedItems(menuInfoPtr, 0);

    /* Update the users's menuBar */
    if (success)
        success = UpdateMenuItems(menuInfoPtr);

Error:
    return success;
}



/*****************************************************/
/*  PutMenuListInIniFile()                           */
/*                                                   */
/*  Parameters:                                      */
/*      menuList from CreateMenuList()               */
/*      handle for inifile Instrument driver         */
/*      section name                                 */
/*      tag name prefix                              */
/*      whether to use the menu callback data as     */
/*          the tag name                             */
/*                                                   */
/*  Return: status                                   */
/*                                                   */
/*  Purpose: Routine for putting menuList items into */
/*           an INIFILE handle                       */
/*                                                   */
/*  Example:                                         */
/*     [Section Name]                                */
/*     TagName1="value1"                             */
/*     TagName2="value2"                             */
/*     TagName3="value3"                             */
/*     TagName4="value4"                             */
/*                                                   */
/*****************************************************/
int CVIFUNC MU_PutFileListInIniFile(
    menuList menuListHandle,
    IniText iniTextHandle,
    char *sectionName,
    char *tagPrefix,
    int useCallbackDataAsTagName)
{
    int success = 1;
    int i, totalItems, item;
    menuInfoRec *menuInfoPtr = NULL;
    menuItemInfoRec *menuItemInfoPtr = NULL;
    char *tagName = NULL;

    /* Find the menuInfo in list */
    if (item = FindMenuInfoInList(sMenuInfoList, menuListHandle)) {
        /* Get menuInfo from list */
        menuInfoPtr = ListGetPtrToItem (sMenuInfoList, item);
        if ((menuInfoPtr) && (menuInfoPtr->menuItemInfoList) ) {
            /* Get number of items in list */
            totalItems = ListNumItems (menuInfoPtr->menuItemInfoList);

            /* Loop thru menuItem list */
            for (i=Min(totalItems, menuInfoPtr->maxItems);i>0;i--) {
                /* Get ith list item */
                if (menuItemInfoPtr = ListGetPtrToItem (menuInfoPtr->menuItemInfoList, i)) {
                    /* Add item to the INIFILE */
                    if ((useCallbackDataAsTagName) && (menuItemInfoPtr->callbackData)) {
                        if(IniEx_PutRawStringListItem(iniTextHandle, sectionName, tagPrefix, (char *)menuItemInfoPtr->callbackData, menuInfoPtr->maxItems)<0) {
                            success = 0;
                            goto Error;
                        }
                    } else {
                        if(IniEx_PutRawStringListItem(iniTextHandle, sectionName, tagPrefix, menuItemInfoPtr->menuName, menuInfoPtr->maxItems)<0) {
                            success = 0;
                            goto Error;
                        }
                    }
                }
                else success = 0;
            }

            /* malloc memory for tag name buffer */
            if ( (tagName = (char *) malloc(sizeof(tagPrefix)+16))==NULL) {
                success = 0;
                goto Error;
            }
            tagName[0] = 0;

            /* Remove Items after maxItems */
            i = Min(totalItems,menuInfoPtr->maxItems);
            while (i) {
                i++;
                sprintf(tagName, "%s%d", tagPrefix, i);
                if (Ini_ItemExists (iniTextHandle, sectionName, tagName))
                    Ini_RemoveItem (iniTextHandle, sectionName, tagName);
                else i=0;
            }
            if (tagName) {
                free(tagName);
                tagName = NULL;
            }
        } else success = 0;
    } else success = 0;

Error:
    /* free memory */
    if (tagName) {
        free(tagName);
        tagName = NULL;
    }
    return success;
}


/*****************************************************/
/*  GetMenuListFromIniFile()                         */
/*                                                   */
/*  Parameters:                                      */
/*      menuList from CreateMenuList()               */
/*      handle for inifile Instrument driver         */
/*      section name                                 */
/*      tag name prefix                              */
/*      flags                                        */
/*                                                   */
/*  Return: status                                   */
/*                                                   */
/*  Purpose: Routine for getting menuList items from */
/*           an INIFILE handle                       */
/*                                                   */
/*  Example:                                         */
/*     [Section Name]                                */
/*     TagName1="value1"                             */
/*     TagName2="value2"                             */
/*     TagName3="value3"                             */
/*     TagName4="value4"                             */
/*                                                   */
/*****************************************************/
int CVIFUNC MU_GetFileListFromIniFile(
    menuList menuListHandle,
    IniText iniTextHandle,
    char *sectionName,
    char *tagPrefix,
    int flags)
{
    int success = 1;
    int i, totalItems;
    char *tagName = NULL;
    char *tagValue = NULL;

    if ((iniTextHandle) && (menuListHandle)) {
        if (Ini_SectionExists (iniTextHandle, sectionName)) {
            /* malloc memory for tag name buffer */
            if ( (tagName = (char *) malloc(sizeof(tagPrefix)+16))==NULL) {
                success = 0;
                goto Error;
            }
            tagName[0] = 0;

            /* Loop thru list of tag/value pairs */
            totalItems = Ini_NumberOfItems (iniTextHandle, sectionName);
            for (i=totalItems; i>0; i--) {
                /* Get the nth tagName and Value */
                sprintf(tagName, "%s%d", tagPrefix, i);
                if(Ini_GetPointerToRawString (iniTextHandle, sectionName, tagName, &tagValue)>0) {
                    if ((tagValue != NULL) && (tagValue[0] != '\0')) {
                        /* Add value to menuList */
                        if (flags) {
                            if (!MU_AddItemToMenuList(menuListHandle, FRONT_OF_LIST, MU_MakeShortFileName(NULL, tagValue, MAX_MENU_ITEM_LENGTH), StrDup(tagValue))) {
                                success = 0;
                                goto Error;
                            }
                        } else {
                            if (!MU_AddItemToMenuList(menuListHandle, FRONT_OF_LIST, tagValue, NULL)) {
                                success = 0;
                                goto Error;
                            }
                        }
                    }
                }
            }
        }
    }
    else success = 0;

Error:
    /* free memory */
    if (tagName) {
        free (tagName);
        tagName = NULL;
    }

    return success;
}


/*****************************************************/
/*  IniEx_PutRawStringListItem()                     */
/*                                                   */
/*  Parameters:                                      */
/*      handle for inifile Instrument driver         */
/*      section name                                 */
/*      tag name prefix                              */
/*      max items in rolling list                    */
/*                                                   */
/*  Return: status                                   */
/*                                                   */
/*  Purpose: INIFILE routine for putting tags info   */
/*           in a rolling tag list                   */
/*                                                   */
/*  Example:                                         */
/*     [Section Name]                                */
/*     TagName1="value1"                             */
/*     TagName2="value2"                             */
/*     TagName3="value3"                             */
/*     TagName4="value4"                             */
/*                                                   */
/*****************************************************/
static int IniEx_PutRawStringListItem(
    IniText iniTextHandle,
    char *sectionName,
    char *tagPrefix,
    char *tagValue,
    int maxItems)
{
    int error = 0;
    int status;
    int i;
    int count;
    int tagCount;
    char *tempBuf = NULL;
    char *tagName = NULL;
    char *tempTagName = NULL;
    int size1, size2;
    int tempInt;
    short found;

    /* Check parameters */
    if ((sectionName==NULL) || (tagPrefix==NULL)) {
        error = -1;
        goto Error;
    }

    if (maxItems < 0) {
        error = -1; goto Error;
    }

    /* malloc memory for tag name buffer */
    if ( (tagName = (char *) malloc(sizeof(tagPrefix)+16))==NULL) {
        error = -1; goto Error;
    }
    tagName[0] = 0;

    found = 0;
    /* See if section exists */
    if (Ini_SectionExists (iniTextHandle, sectionName)) {
        /* Get number of items in section */
        count = Ini_NumberOfItems (iniTextHandle, sectionName);
        tagCount = 0;
        /* Loop thru and find the highest prefix tag in list */
        for (i=1; i<=count;i++) {
            /* If prefixTag, get max item */
            status = Ini_NthItemName (iniTextHandle, sectionName, i, &tempTagName);

            size1 = strlen(tagPrefix);
            size2 = strlen(tempTagName);
            if ( (!strncmp(tagPrefix, tempTagName, size1)) && (size2>size1) ) {
                if (sscanf(&tempTagName[size1],"%d", &tempInt) == 1) {
                    tagCount = Max(tagCount, tempInt);

                    /* See if this tags value is the same value passed in */
                    if (Ini_GetPointerToRawString (iniTextHandle, sectionName, tempTagName, &tempBuf)>0) {
                        if (!strcmp(tempBuf, tagValue)) {
                            found = tempInt;
                        }
                    }
                }
            }
        }
        if (found != 1) {
            for (i=tagCount; i>0;i--) {
                /* Remove items after maxItems if non-zero */
                if ((maxItems>0) && (i>maxItems)) {
                    sprintf(tagName, "%s%d", tagPrefix, i);
                    Ini_RemoveItem (iniTextHandle, sectionName, tagName);
                    /* FmtOut("Removing [%s], tag=%s\n", sectionName, tagName); */

                }
                /* Shift items up by one */
                else if ( (i<maxItems) && ((!found) || (i<found) ) ) {
                    sprintf(tagName, "%s%d", tagPrefix, i);
                    if (Ini_GetPointerToRawString (iniTextHandle, sectionName, tagName, &tempBuf)>0) {
                        sprintf(tagName, "%s%d", tagPrefix, i+1);
                        if (error = Ini_PutRawString (iniTextHandle, sectionName, tagName, tempBuf)!=0)
                            goto Error;
                    }

                }
            }
        }
    }

    if (found != 1) {
        /* Just put item in list as first entry */
        sprintf(tagName, "%s%s", tagPrefix, "1");
        if(error = Ini_PutRawString (iniTextHandle, sectionName, tagName, tagValue)!=0) {
            error = -1;
            goto Error;
        }
    }

Error:
    /* free memory */
    if (tagName) {
        free (tagName);
        tagName = NULL;
    }

    return error;
}


/*****************************************************/
/*  MU_WriteRegistryInfo()                           */
/*                                                   */
/*  Parameters:                                      */
/*      handle for inifile Instrument driver         */
/*                                                   */
/*  Return: success = 1                              */
/*                                                   */
/*  Purpose: Main routine for putting registry info  */
/*****************************************************/
int CVIFUNC MU_WriteRegistryInfo(
    IniText iniTextHandle,
    char *registryName)
{
    int success = 1;
#ifndef WIN32
    char fileName[MAX_PATHNAME_LEN];
#endif

    if ((!registryName) || (!registryName[0]))
        return 0;

    /*----------------------------------------------------------*/
    /* Write out the registry data                              */
    /*----------------------------------------------------------*/
#ifdef WIN32
    if (IniEx_WriteToRegistry(iniTextHandle, 1, registryName, 1))
        success = 0;
#else
    if (!GetProjectDir (fileName)) {
        #ifdef _NI_mswin_
        sprintf(fileName, "%s\\%s", fileName, registryName);
        #else
        sprintf(fileName, "%s/%s", fileName, registryName);
        #endif
        if (Ini_WriteToFile (iniTextHandle, fileName))
            success = 0;
    }
#endif
    return success;
}

/*****************************************************/
/*  MU_ReadRegistryInfo()                            */
/*                                                   */
/*  Parameters:                                      */
/*      handle for inifile Instrument driver         */
/*                                                   */
/*  Return: success = 1                              */
/*                                                   */
/*  Purpose: Main routine for putting registry info  */
/*****************************************************/
int CVIFUNC MU_ReadRegistryInfo(
    IniText iniTextHandle,
    char *registryName)
{
    int success = 1;
#ifndef WIN32
    char fileName[MAX_PATHNAME_LEN];
#endif
    int BOLE;

    if ((!registryName) || (!registryName[0]))
        return 0;

    /*----------------------------------------------------------*/
    /* Write out the registry data                              */
    /*----------------------------------------------------------*/
#ifdef WIN32
    BOLE = GetBreakOnLibraryErrors ();
    SetBreakOnLibraryErrors (0);
    if (IniEx_ReadFromRegistry(iniTextHandle, 1, registryName))
        success = 0;
    SetBreakOnLibraryErrors (BOLE);
#else
    if (!GetProjectDir (fileName)) {
        #ifdef _NI_mswin_
        sprintf(fileName, "%s\\%s", fileName, registryName);
        #else
        sprintf(fileName, "%s/%s", fileName, registryName);
        #endif
        BOLE = GetBreakOnLibraryErrors ();
        SetBreakOnLibraryErrors (0);
        if (Ini_ReadFromFile (iniTextHandle, fileName))
            success = 0;
    }
    SetBreakOnLibraryErrors (BOLE);
#endif
    return success;
}

#ifdef WIN32
/*****************************************************/
/*  IniEx_ReadFromRegistry()                         */
/*                                                   */
/*  Parameters:                                      */
/*      handle for inifile Instrument driver         */
/*      root Key  Valid values:                      */
/*          0 = HKEY_CLASSES_ROOT                    */
/*          1 = HKEY_CURRENT_USER                    */
/*          2 = HKEY_LOCAL_MACHINE                   */
/*          3 = HKEY_USERS                           */
/*      base Key name                                */
/*                                                   */
/*  Return: status                                   */
/*                                                   */
/*  Purpose: Main routine for getting registry info  */
/*****************************************************/
static int IniEx_ReadFromRegistry(
    IniText iniTextHandle,
    int rootKey,
    char *baseKeyName)
{
    int error = 0;
    unsigned int count, tag;

    char *subKeyName = NULL;        /* address of buffer for subkey string      */
    DWORD subKeyNameSize;
    char *tagName    = NULL;        /* address of buffer for name string        */
    DWORD tagNameSize;
    char *tagValue   = NULL;        /* address of buffer for value string       */
    DWORD tagValueSize;
    DWORD subKeyCount;              /* buffer for number of subkeys             */
    DWORD maxSubKeyLen;             /* buffer for longest subkey name length    */
    DWORD valueCount;               /* buffer for number of value entries       */
    DWORD maxValueNameLen;          /* buffer for longest value name length     */
    DWORD maxValueLen;              /* buffer for longest value data length     */

    HKEY  hKey, baseKey=0, subKey=0;

    /* Verify rootKey value */
    switch (rootKey) {
        case 0:
            hKey = HKEY_CLASSES_ROOT;
            break;
        case 1:
            hKey = HKEY_CURRENT_USER;
            break;
        case 2:
            hKey = HKEY_LOCAL_MACHINE;
            break;
        case 3:
            hKey = HKEY_USERS;
            break;
        default:
            error = -1;
            goto Error;
    }

    if (iniTextHandle) {
        /* Open User's Key */
        RegOpenKey(hKey, (LPCTSTR) baseKeyName, &baseKey);
        if (error) goto Error;

        /* Get Number of Subkeys */
        error = RegQueryInfoKey (
            baseKey,                /* handle of key to query                               */
            NULL,                   /* address of buffer for class string                   */
            NULL,                   /* address of size of class string buffer               */
            NULL,                   /* reserved                                             */
            &subKeyCount,           /* address of buffer for number of subkeys              */
            &maxSubKeyLen,          /* address of buffer for longest subkey name length     */
            NULL,                   /* address of buffer for longest class string length    */
            NULL,                   /* address of buffer for number of value entries        */
            NULL,                   /* address of buffer for longest value name length      */
            NULL,                   /* address of buffer for longest value data length      */
            NULL,                   /* address of buffer for security descriptor length     */
            NULL                    /* address of buffer for last write time                */
           );
        if (error) goto Error;

        /* If there are subkeys, enumerate thru the subkeys */
        if (subKeyCount) {
            /* Enumerate thru Subkey Names */
            nullChk(subKeyName = (char *) malloc(maxSubKeyLen+1));
            subKeyName[0] = 0;
            for (count = 1; count <= subKeyCount; count++) {
                subKeyNameSize = (DWORD) maxSubKeyLen+1;
                /* Get Subkey Name */
                error = RegEnumKeyEx(
                    baseKey,            /* handle of key to enumerate           */
                    (DWORD)(count-1),   /* index of subkey to enumerate         */
                    subKeyName,         /* address of buffer for subkey name    */
                    &subKeyNameSize,    /* address for size of subkey buffer    */
                    NULL,               /* reserved                             */
                    NULL,               /* address of buffer for class string   */
                    NULL,               /* address for size of class buffer     */
                    NULL                /* address for time key last written to */
                   );
                if (error) goto Error;

                /* Open Subkey */
                error = RegOpenKey(baseKey, (LPCTSTR) subKeyName, &subKey);
                if (error) goto Error;

                /* Get Number of Values */
                error = RegQueryInfoKey (
                    subKey,                 /* handle of key to query                               */
                    NULL,                   /* address of buffer for class string                   */
                    NULL,                   /* address of size of class string buffer               */
                    NULL,                   /* reserved                                             */
                    NULL,                   /* address of buffer for number of subkeys              */
                    NULL,                   /* address of buffer for longest subkey name length     */
                    NULL,                   /* address of buffer for longest class string length    */
                    &valueCount,            /* address of buffer for number of value entries        */
                    &maxValueNameLen,       /* address of buffer for longest value name length      */
                    &maxValueLen,           /* address of buffer for longest value data length      */
                    NULL,                   /* address of buffer for security descriptor length     */
                    NULL                    /* address of buffer for last write time                */
                   );
                if (error) goto Error;


                /* If there are values, create new section and enumerate thru the values */
                if (valueCount) {
                    /* Enumerate thru Values Names */
                    nullChk(tagName = (char *) malloc(maxValueNameLen+1));
                    nullChk(tagValue = (char *) malloc(maxValueLen+1));
                    tagName[0] = 0;
                    tagValue[0] = 0;
                    for (tag = 1; tag <= valueCount; tag++)
                    {
                        tagNameSize = (DWORD) maxValueNameLen+1;
                        tagValueSize = (DWORD) maxValueLen+1;
                        /* Get Subkey Name */
                        error = RegEnumValue(
                            subKey,             /* handle of key to query               */
                            (DWORD)(tag-1),     /* index of value to query              */
                            tagName,            /* address of buffer for value string   */
                            &tagNameSize,       /* address for size of value buffer     */
                            NULL,               /* reserved                             */
                            NULL,               /* address of buffer for type code      */
                            (LPBYTE)tagValue,   /* address of buffer for value data     */
                            &tagValueSize       /* address for size of data buffer      */
                           );
                        if (error) goto Error;

                        error = Ini_PutRawString (iniTextHandle, subKeyName, tagName, tagValue);
                        if (error) goto Error;

                    } /* for loop */

                    if (tagName) {
                        free(tagName);
                        tagName = NULL;
                    }
                    if (tagValue) {
                        free(tagValue);
                        tagValue = NULL;
                    }

                } /* if (valueCount) */

                if (subKey) {
                    RegCloseKey(subKey);
                    subKey = NULL;
                }
            } /* for loop */

            if (subKeyName) {
                free(subKeyName);
                subKeyName = NULL;
            }
        } /* if (subKeyCount) */

        if (baseKey) {
            RegCloseKey(baseKey);
            baseKey = NULL;
        }
    } /* if (iniTextHandle) */

Error:
    /* free malloc'd data */
    if (subKeyName) {
        free(subKeyName);
        subKeyName = NULL;
    }
    if (tagName) {
        free(tagName);
        tagName = NULL;
    }
    if (tagValue) {
        free(tagValue);
        tagValue = NULL;
    }

    /* Close out any open keys */
    if (subKey) {
        RegCloseKey(subKey);
        subKey = NULL;
    }

    if (baseKey) {
        RegCloseKey(baseKey);
        baseKey = NULL;
    }

    return error;
}

/*****************************************************/
/*  IniEx_WriteToRegistry()                          */
/*                                                   */
/*  Parameters:                                      */
/*      handle for inifile Instrument driver         */
/*      root Key  Valid values:                      */
/*          0 = HKEY_CLASSES_ROOT                    */
/*          1 = HKEY_CURRENT_USER                    */
/*          2 = HKEY_LOCAL_MACHINE                   */
/*          3 = HKEY_USERS                           */
/*      base Key name                                */
/*      whether to remove non list tags              */
/*                                                   */
/*  Return: status                                   */
/*                                                   */
/*  Purpose: Main routine for putting registry info  */
/*****************************************************/
static int IniEx_WriteToRegistry(
    IniText iniTextHandle,
    int rootKey,
    char *baseKeyName,
    int removeNonListTags)
{
    int   error = 0;
    int   count;
    int   index;

    char  *sectionName = NULL;
    char  *tagName = NULL;
    char  *tagValue = NULL;

    HKEY  hKey, baseKey = 0, subKey = 0;
    DWORD disposition;

    switch (rootKey)
    {
        case 0:
            hKey = HKEY_CLASSES_ROOT;
            break;
        case 1:
            hKey = HKEY_CURRENT_USER;
            break;
        case 2:
            hKey = HKEY_LOCAL_MACHINE;
            break;
        case 3:
            hKey = HKEY_USERS;
            break;
        default:
            error = -1;
            goto Error;
    }

    if (iniTextHandle) {
        /* Open or Create the Base Section in Registry */
        error = RegCreateKeyEx (hKey, (LPCTSTR) baseKeyName, 0, "", REG_OPTION_NON_VOLATILE,
                            KEY_ALL_ACCESS, NULL, &baseKey, &disposition);
        if (error) goto Error;

        for (count = 1; count <= Ini_NumberOfSections (iniTextHandle); count++) {
            /* Get the Section Name */
            if (!Ini_NthSectionName (iniTextHandle, count, &sectionName))
                goto Error;

            /* Remove Section Name if specified */
            if (removeNonListTags)
                RegDeleteKey(baseKey, sectionName);

            /* Open or Create the "INIFILE" Section in Registry */
            error = RegCreateKeyEx (baseKey, (LPCTSTR) sectionName, 0, "", REG_OPTION_NON_VOLATILE,
                                KEY_ALL_ACCESS, NULL, &subKey, &disposition);
            if (error) goto Error;


            for (index=1; index <= Ini_NumberOfItems (iniTextHandle, sectionName); index++) {
                /* Get the tagName and Value */
                if (!Ini_NthItemName (iniTextHandle, sectionName, index, &tagName))
                    goto Error;
                if (Ini_GetRawStringCopy (iniTextHandle, sectionName, tagName, &tagValue)<0)
                    goto Error;

                error = RegSetValueEx (subKey, tagName, 0, REG_SZ, (CONST BYTE *)tagValue, strlen (tagValue)+1);
                if (error) goto Error;

                if (tagValue) {
                    free(tagValue);
                    tagValue = NULL;
                }
            }
            if (subKey) {
                RegCloseKey(subKey);
                subKey = NULL;
            }
        }
    }
Error:
    /* free any allocated memory */
    if (tagValue) {
        free(tagValue);
        tagValue = NULL;
    }

    /* Close out any open keys */
    if (subKey) {
        RegCloseKey(subKey);
        subKey = NULL;
    }

    if (baseKey) {
        RegCloseKey(baseKey);
        baseKey = NULL;
    }

    return error;
}

#endif /* #ifdef WIN32 */



/*****************************************************/
/*                                                   */
/*  MU_GetMenuListAttribute ()                       */
/*                                                   */
/*  Parameters:                                      */
/*      menuList Handle                              */
/*      index                                        */
/*      attribute                                    */
/*      data                                         */
/*                                                   */
/*  Output: error                                    */
/*                                                   */
/*  Purpose: Get a variety of information inside     */
/*      menuList or an item in a menuList            */
/*                                                   */
/*****************************************************/
int CVIFUNC MU_GetMenuListAttribute (
    menuList handle,
    int index,
    int attribute,
    void *value)
{
    menuInfoRec * menuInfoPtr = NULL;
    menuItemInfoRec * menuItemInfoPtr = NULL;
    int item;

    /**/
    /* Check Input values
    /**/
    if ((attribute < ATTR_MENULIST_MIN) || (attribute > ATTR_MENULIST_MAX))
        goto Error;

    /* Find menuList from handle */
    if ( ((item = FindMenuInfoInList (sMenuInfoList, handle))<=0) ||
         (!(menuInfoPtr = ListGetPtrToItem (sMenuInfoList, item)))  )
        goto Error;

    /* Find menuItemList if necessary */
    if ((attribute >= ATTR_MENULIST_FIRST_ITEM_ATTR) &&
        (!(menuItemInfoPtr = ListGetPtrToItem (menuInfoPtr->menuItemInfoList, index))) )
        goto Error;

    switch (attribute) {
        case ATTR_MENULIST_MAX_NUM_ITEMS:
            *((int *) value) = menuInfoPtr->maxItems;
            break;

        case ATTR_MENULIST_UPPER_SEPARATOR:
            *((int *) value) = menuInfoPtr->separator1;
            break;

        case ATTR_MENULIST_LOWER_SEPARATOR:
            *((int *) value) = menuInfoPtr->separator2;
            break;

        case ATTR_MENULIST_ONE_CHECK_ITEM:
            *((int *) value) = menuInfoPtr->oneCheckItem;
            break;

        case ATTR_MENULIST_CHECK_WHEN_ADDED:
            *((int *) value) = menuInfoPtr->checkWhenAdded;
            break;

        case ATTR_MENULIST_ALLOW_DUPLICATE_ITEMS:
            *((int *) value) = menuInfoPtr->allowDuplicates;
            break;

        case ATTR_MENULIST_MENUBAR_HANDLE:
            *((int *) value) = menuInfoPtr->menuBarHandle;
            break;

        case ATTR_MENULIST_MENU_ID:
            *((int *) value) = menuInfoPtr->menuID;
            break;

        case ATTR_MENULIST_BEFORE_MENU_ITEM:
            *((int *) value) = menuInfoPtr->beforeMenuItemID;
            break;

        case ATTR_MENULIST_CALLBACK_FUNCTION:
            *((void **) value) = (void *) menuInfoPtr->callbackFunc;
            break;

        case ATTR_MENULIST_APPEND_SHORTCUT:
            *((int *) value) = menuInfoPtr->preAppendShortCut;
            break;

        case ATTR_MENULIST_ITEM_MENU_ID:
            *((int *) value) = menuItemInfoPtr->menuItemID;
            break;

        case ATTR_MENULIST_ITEM_NAME:
            strncpy((char *) value, menuItemInfoPtr->menuName, MAX_MENU_ITEM_LENGTH + 1);
            break;

        case ATTR_MENULIST_ITEM_NAME_LENGTH:
            *((int *) value) = strlen(menuItemInfoPtr->menuName);
            break;

        case ATTR_MENULIST_ITEM_CALLBACK_DATA:
            *((void **) value) = menuItemInfoPtr->callbackData;
            break;

        case ATTR_MENULIST_ITEM_CHECKED:
            GetMenuBarAttribute (menuInfoPtr->menuBarHandle, menuItemInfoPtr->menuItemID, ATTR_CHECKED, (int *) value);
            break;

        default:
            goto Error;
    } /* END Switch */

    return 0;
Error:

    return -1;
}


/*****************************************************/
/*                                                   */
/*  MU_SetMenuListAttribute ()                       */
/*                                                   */
/*  Parameters:                                      */
/*      menuList Handle                              */
/*      index                                        */
/*      attribute                                    */
/*      data                                         */
/*                                                   */
/*  Output: error                                    */
/*                                                   */
/*  Purpose: Set a variety of information inside     */
/*      menuList or an item in a menuList            */
/*                                                   */
/*****************************************************/
int CVIFUNC_C MU_SetMenuListAttribute (
    menuList handle,
    int index,
    int attribute,
    ...)
{
    int error;
    va_list parmInfo;

    va_start(parmInfo, attribute);
    error = MU_SetMenuListAttributeFromParmInfo(handle, index, attribute, parmInfo);
    va_end(parmInfo);

    return error;
}


/*****************************************************/
/*                                                   */
/*  MU_SetMenuListAttributeFromParmInfo ()           */
/*                                                   */
/*  Parameters:                                      */
/*      menuList Handle                              */
/*      index                                        */
/*      attribute                                    */
/*      data                                         */
/*                                                   */
/*  Output: error                                    */
/*                                                   */
/*  Purpose: Set a variety of information inside     */
/*      menuList or an item in a menuList            */
/*                                                   */
/*****************************************************/
int CVIFUNC MU_SetMenuListAttributeFromParmInfo(
    menuList handle,
    int index,
    int attribute,
    va_list parmInfo)
{
    int updateAllMenuItems = 0;
    int prevIntValue;
    int newIntValue;
    menuInfoRec * menuInfoPtr = NULL;
    menuItemInfoRec * menuItemInfoPtr = NULL;
    int item;

    /**/
    /* Check Input values
    /**/
    if ((attribute < ATTR_MENULIST_MIN) || (attribute > ATTR_MENULIST_MAX))
        goto Error;

    /* Find menuList from handle */
    if ( ((item = FindMenuInfoInList (sMenuInfoList, handle))<=0) ||
         (!(menuInfoPtr = ListGetPtrToItem (sMenuInfoList, item)))  )
        goto Error;

    /* Find menuItemList if necessary */
    if ((attribute > ATTR_MENULIST_FIRST_ITEM_ATTR) &&
        (!(menuItemInfoPtr = ListGetPtrToItem (menuInfoPtr->menuItemInfoList, index))) )
        goto Error;

    switch (attribute) {
        case ATTR_MENULIST_MAX_NUM_ITEMS:
            prevIntValue = menuInfoPtr->maxItems;
            newIntValue  = va_arg(parmInfo, int);

            menuInfoPtr->maxItems = newIntValue;
            updateAllMenuItems = (prevIntValue > newIntValue);
            break;

        case ATTR_MENULIST_UPPER_SEPARATOR:
            prevIntValue = menuInfoPtr->separator1;
            newIntValue  = va_arg(parmInfo, int);

            menuInfoPtr->separator1 = newIntValue;
            updateAllMenuItems = (prevIntValue != newIntValue);
            break;

        case ATTR_MENULIST_LOWER_SEPARATOR:
            prevIntValue = menuInfoPtr->separator2;
            newIntValue  = va_arg(parmInfo, int);

            menuInfoPtr->separator2 = newIntValue;
            updateAllMenuItems = (prevIntValue != newIntValue);
            break;

        case ATTR_MENULIST_ONE_CHECK_ITEM:
            prevIntValue = menuInfoPtr->oneCheckItem;
            newIntValue  = va_arg(parmInfo, int);

            /* Uncheck all items exept first found */
            if (newIntValue)
                UncheckAllCheckedItems(menuInfoPtr, 1);

            menuInfoPtr->oneCheckItem = newIntValue;
            updateAllMenuItems = ((prevIntValue != newIntValue) && (newIntValue));
            break;

        case ATTR_MENULIST_CHECK_WHEN_ADDED:
            menuInfoPtr->checkWhenAdded = va_arg(parmInfo, int);
            break;

        case ATTR_MENULIST_ALLOW_DUPLICATE_ITEMS:
            prevIntValue = menuInfoPtr->allowDuplicates;
            newIntValue  = va_arg(parmInfo, int);

            if (prevIntValue != newIntValue) {
                if (!newIntValue) {
                    /* Remove duplicates */
                    RemoveDuplicates(menuInfoPtr);
                    updateAllMenuItems = 1;
                }
                menuInfoPtr->allowDuplicates = newIntValue;
            }
            break;

        case ATTR_MENULIST_MENUBAR_HANDLE:
            goto Error;

        case ATTR_MENULIST_MENU_ID:
            prevIntValue = menuInfoPtr->menuID;
            newIntValue  = va_arg(parmInfo, int);

            menuInfoPtr->menuID = newIntValue;
            updateAllMenuItems = (prevIntValue != newIntValue);
            break;

        case ATTR_MENULIST_BEFORE_MENU_ITEM:
            prevIntValue = menuInfoPtr->beforeMenuItemID;
            newIntValue  = va_arg(parmInfo, int);

            menuInfoPtr->beforeMenuItemID = newIntValue;
            updateAllMenuItems = (prevIntValue != newIntValue);
            break;

        case ATTR_MENULIST_CALLBACK_FUNCTION:
            menuInfoPtr->callbackFunc = (MenuListCallbackPtr) va_arg(parmInfo, void *);
            break;

        case ATTR_MENULIST_APPEND_SHORTCUT:
            prevIntValue = menuInfoPtr->preAppendShortCut;
            newIntValue  = va_arg(parmInfo, int);

            menuInfoPtr->preAppendShortCut = newIntValue;
            updateAllMenuItems = (prevIntValue != newIntValue);
            break;

        case ATTR_MENULIST_ITEM_MENU_ID:
            goto Error;

        case ATTR_MENULIST_ITEM_NAME:
            strncpy(menuItemInfoPtr->menuName, va_arg(parmInfo, char *), MAX_MENU_ITEM_LENGTH);
            menuItemInfoPtr->menuName[MAX_MENU_ITEM_LENGTH] = 0;
            updateAllMenuItems = 1;
            break;

        case ATTR_MENULIST_ITEM_NAME_LENGTH:
            goto Error;

        case ATTR_MENULIST_ITEM_CALLBACK_DATA:
            menuItemInfoPtr->callbackData = va_arg(parmInfo, void *);
            break;

        case ATTR_MENULIST_ITEM_CHECKED:
            GetMenuBarAttribute (menuInfoPtr->menuBarHandle, menuItemInfoPtr->menuItemID, ATTR_CHECKED, &prevIntValue);
            newIntValue = va_arg(parmInfo, int);
            if (prevIntValue != newIntValue) {
                /* Remove other checked items if new item is to be checked */
                if ((menuInfoPtr->oneCheckItem) && (newIntValue))
                    UncheckAllCheckedItems(menuInfoPtr, 0);
                /* Check or uncheck new item */
                SetMenuBarAttribute (menuInfoPtr->menuBarHandle, menuItemInfoPtr->menuItemID, ATTR_CHECKED, newIntValue);
            }
            break;

        default:
            goto Error;
    } /* END Switch */
    if (updateAllMenuItems)
        UpdateMenuItems(menuInfoPtr);

    return 0;
Error:

    return -1;
} /* END MU_SetMenuListAttributeFromParmInfo () */

