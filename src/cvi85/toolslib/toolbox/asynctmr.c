//------------------------------------------------------------------------------
// Asynchronous Timer Instrument Driver for LW/CVI 4.x for Windows 95/NT
//------------------------------------------------------------------------------
// Note:  The multimedia timer functions block until all outstanding timer
// callbacks are complete.
//------------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Include files
//----------------------------------------------------------------------------
#if defined (_NI_mswin32_) && _NI_mswin32_
#include <windows.h>
#endif
// CVI header files
#include <ansi_c.h>
#include <libsupp.h>
#include <userint.h>
#include <utility.h>

// Driver header file
#include <asynctmr.h>

#define ASYNC_TIMER_ENABLED  1
#define ASYNC_TIMER_DISABLED 0

//----------------------------------------------------------------------------
// Typedefs
//----------------------------------------------------------------------------
#pragma pack (4)
typedef struct {
    unsigned int seconds;
    unsigned int nanoseconds;
} CVIUSTEtime;
#pragma pack ()

typedef struct _tAsyncTimerRec {
    int                     userHandle;
    AsyncTimerCallbackPtr   callback;
    void                   *callbackData;
    int                     count;
    int                     status;
    void                   *timer;
    CVIUSTEtime             interval;
    double                  startTime;
    double                  lastTime;
    int                     firstTime;
#ifdef _CVI_
    unsigned char           environment[ASYNC_CALLBACK_ENV_SIZE];
#endif
    struct _tAsyncTimerRec *prevTimer;
    struct _tAsyncTimerRec *nextTimer;
} tAsyncTimerRec;

//----------------------------------------------------------------------------
// Local variables
//----------------------------------------------------------------------------
static int globalLock       = 0;
static int asyncLock        = 0;
static int asyncTimerStatus = ASYNC_TIMER_ENABLED;
static int lastHandle       = 1;
static int realtime         = 0;

static tAsyncTimerRec *asyncTimerListHead   = NULL;
static tAsyncTimerRec *asyncTimerListTail   = NULL;
static tAsyncTimerRec *asyncDiscardListHead = NULL; // Use this list to store discarded timers for later reuse

//----------------------------------------------------------------------------
// Static functions
//----------------------------------------------------------------------------
static tAsyncTimerRec  *FindAsyncTimer(int timerId);
static void             RemoveAsyncTimer(tAsyncTimerRec *asyncTimerPtr);
static double           CurrentTime(void);
static int              InitAsyncLibrary(void);
static double           GetResolution(void);
static void CVICALLBACK LocalTimerCallbackFunc (void *data);

//----------------------------------------------------------------------------
// Timer functions are undocumented exports of the runtime engine.
//----------------------------------------------------------------------------

extern int   CVIFUNC_C CVIUSTEinitialize (void);
extern void  CVIFUNC_C CVIUSTElocaltime (CVIUSTEtime *now);
extern void  CVIFUNC_C CVIUSTEresolution (CVIUSTEtime *resolution);
extern void *CVIFUNC_C CVIUSTEnewtimer (void (CVICALLBACK *callback) (void *data), void *callbackData);
extern int   CVIFUNC_C CVIUSTEsettimer (void *timer, CVIUSTEtime *interval);
extern void  CVIFUNC_C CVIUSTEsetpriority (void *timer, int priority);
extern int   CVIFUNC_C CVIUSTEgetpriority (void *timer);
extern void  CVIFUNC_C CVIUSTEdiscardtimer (void *timer);

#define ConvertDoubleToTime(d,t)                                \
    {   double temp;                                            \
        (t)->nanoseconds = 1000000000.0 * modf ((d), &temp);    \
        (t)->seconds     = temp;}
#define ConvertTimeToDouble(t)          \
    ((t)->seconds + ((double)(t)->nanoseconds) / 1000000000.0)

#define kFailedToLoadDLLError     ASYNC_DLL_LOAD_ERR
#define kCouldNotFindFunction     ASYNC_DLL_LOAD_ERR

//------------------------------------------------------------------------------
// Init routine for Async Timer
//------------------------------------------------------------------------------
static int CVIFUNC InternalNewAsyncTimer(double doubleInterval, int count,
    int status, AsyncTimerCallbackPtr callbackFunc, void *callbackData,
    int threadPriority);

int CVIFUNC NewAsyncTimer(double doubleInterval, int count,
    int status, AsyncTimerCallbackPtr callbackFunc, void *callbackData)
{
    return InternalNewAsyncTimer (doubleInterval, count, status, callbackFunc,
        callbackData, THREAD_PRIORITY_HIGHEST);
}

int CVIFUNC NewAsyncTimerWithPriority(double doubleInterval, int count,
    int status, AsyncTimerCallbackPtr callbackFunc, void *callbackData,
    int threadPriority)
{
    if (!InitAsyncLibrary())
        return ASYNC_INTERNAL_ERR;
    
    if (!realtime)
        return ASYNC_ONLY_AVAILABLE_ON_REAL_TIME;
    
    return InternalNewAsyncTimer (doubleInterval, count, status, callbackFunc,
        callbackData, threadPriority);
}

static int CVIFUNC InternalNewAsyncTimer(double doubleInterval, int count,
    int status, AsyncTimerCallbackPtr callbackFunc, void *callbackData,
    int threadPriority)
{
    int             timerId = 0;
    tAsyncTimerRec *asyncTimerPtr;
    void           *result;
    double          resolution;
    CVIUSTEtime     interval;

    ConvertDoubleToTime (doubleInterval, &interval);

    // Call InitAsyncLibrary here instead of in runstate change callback
    if (!InitAsyncLibrary())
        return ASYNC_INTERNAL_ERR;

    // Need to verify paramters
    if (status != ASYNC_TIMER_ENABLED && status != ASYNC_TIMER_DISABLED)
        return ASYNC_INVALID_PARAMETER_ERR;

    if (doubleInterval < 0.0)
        return ASYNC_INVALID_PARAMETER_ERR;

    if (count==0)
        return ASYNC_INVALID_PARAMETER_ERR;

    // Enter critical section
    CmtGetLock (asyncLock);

    // check if there are any discarded timers
    if (asyncDiscardListHead)
    {
        // Make the first discarded timer the new timer
        asyncTimerPtr = asyncDiscardListHead;
        asyncDiscardListHead->prevTimer = NULL;
        asyncDiscardListHead = asyncDiscardListHead->nextTimer;
    }
    else
    {
        // Out of timer handles?
        if (lastHandle <= 0)
        {
            // Leave critical section
            CmtReleaseLock (asyncLock);
            return ASYNC_NO_MORE_HANDLES_ERR;
        }

        // Allocate a new record for the new timer
        if (!(asyncTimerPtr = (tAsyncTimerRec *) malloc(sizeof(tAsyncTimerRec))))
        {
            // Leave critical section
            CmtReleaseLock (asyncLock);
            return ASYNC_OUT_OF_MEMORY_ERR;  // Could not allocate memory
        }

        // Assign a new handle
        asyncTimerPtr->userHandle = lastHandle++;
    }

    // Attach the record to the end of the list
    asyncTimerPtr->nextTimer = NULL;

    if (asyncTimerListHead == NULL)
    {
        asyncTimerListHead = asyncTimerListTail = asyncTimerPtr;
        asyncTimerPtr->prevTimer = NULL;
    }
    else
    {
        asyncTimerListTail->nextTimer = asyncTimerPtr;
        asyncTimerPtr->prevTimer = asyncTimerListTail;
        asyncTimerListTail = asyncTimerPtr;
    }

    resolution = GetResolution();
    if (resolution == 0)
        goto Error;

    // Set new entry values
    asyncTimerPtr->status       = ASYNC_TIMER_DISABLED;
    asyncTimerPtr->count        = count;
    asyncTimerPtr->callback     = callbackFunc;
    asyncTimerPtr->callbackData = callbackData;
    asyncTimerPtr->firstTime    = TRUE;
    ConvertDoubleToTime ((doubleInterval > resolution) ? doubleInterval : resolution,
        &asyncTimerPtr->interval);

    // Leave critical section
    CmtReleaseLock (asyncLock);

    // Start multimedia timer callback
    result = CVIUSTEnewtimer (LocalTimerCallbackFunc, asyncTimerPtr);
    if (result != NULL) {
        CVIUSTEsetpriority (result, threadPriority);
        if (!CVIUSTEsettimer (result, &asyncTimerPtr->interval)) {
            CVIUSTEdiscardtimer (result);
            result = NULL;
        }
    }
    asyncTimerPtr->startTime = asyncTimerPtr->lastTime = 0.0;

    // Enter critical section
    CmtGetLock (asyncLock);

    // If timer is not available then remove timer from list, exit critical section and return error
    if (result == NULL)
        goto Error;

    // Otherwise store the timer handle
    asyncTimerPtr->timer  = result;
    asyncTimerPtr->status = status;
    timerId               = asyncTimerPtr->userHandle;

    // Leave critical section
    CmtReleaseLock (asyncLock);

    return timerId;

Error:
    asyncTimerPtr->timer        = NULL;
    asyncTimerPtr->status       = ASYNC_TIMER_DISABLED;
    asyncTimerPtr->count        = 0;
    asyncTimerPtr->callback     = NULL;
    asyncTimerPtr->callbackData = NULL;

    // Remove the timer from the main timer list
    if (asyncTimerPtr->prevTimer)
    {
        asyncTimerPtr->prevTimer->nextTimer = asyncTimerPtr->nextTimer;
        if (asyncTimerPtr->nextTimer)
            asyncTimerPtr->nextTimer->prevTimer = asyncTimerPtr->prevTimer;
    }
    else
    {
        asyncTimerListHead = asyncTimerPtr->nextTimer;
        if (asyncTimerPtr->nextTimer)
            asyncTimerPtr->nextTimer->prevTimer = NULL;
    }

    if (asyncTimerListTail == asyncTimerPtr)
        asyncTimerListTail = asyncTimerPtr->prevTimer;

    // Add it to the top discarded timer list.
    asyncTimerPtr->prevTimer = NULL;
    asyncTimerPtr->nextTimer = asyncDiscardListHead;
    if (asyncDiscardListHead)
        asyncDiscardListHead->prevTimer = asyncTimerPtr;
    asyncDiscardListHead = asyncTimerPtr;

    // Leave critical section
    CmtReleaseLock (asyncLock);
    return ASYNC_TIMER_FAIL_ERR;
}

//------------------------------------------------------------------------------
// This routine finds the Async Timer from the list given the timerId
//------------------------------------------------------------------------------
static tAsyncTimerRec *FindAsyncTimer(int timerId)
{
    tAsyncTimerRec *asyncTimerPtr = asyncTimerListHead;

    // Search for the timerId among all the timers
    for(;asyncTimerPtr && asyncTimerPtr->userHandle <= timerId;asyncTimerPtr = asyncTimerPtr->nextTimer)
    {
        // If found return the pointer
        if (asyncTimerPtr->userHandle == timerId)
            return asyncTimerPtr;
    }
    // No timer with the given timerId found
    return NULL;
}

//------------------------------------------------------------------------------
// This routine removes the Async Timer from the list given the pointer to it
// This routine asssume you are in the critical section
//------------------------------------------------------------------------------
static void RemoveAsyncTimer(tAsyncTimerRec *asyncTimerPtr)
{
    double currentTime, timerTime, deltaTime;

    // Disable the timer
    asyncTimerPtr->status = ASYNC_TIMER_DISABLED;

    // Remove the timer from the main timer list
    if (asyncTimerPtr->prevTimer)
    {
        asyncTimerPtr->prevTimer->nextTimer = asyncTimerPtr->nextTimer;
        if (asyncTimerPtr->nextTimer)
            asyncTimerPtr->nextTimer->prevTimer = asyncTimerPtr->prevTimer;
    }
    else
    {
        asyncTimerListHead = asyncTimerPtr->nextTimer;
        if (asyncTimerPtr->nextTimer)
            asyncTimerPtr->nextTimer->prevTimer = NULL;
    }

    if (asyncTimerListTail == asyncTimerPtr)
        asyncTimerListTail = asyncTimerPtr->prevTimer;

    // Update time for DISCARD
    currentTime = CurrentTime ();
    timerTime   = currentTime - asyncTimerPtr->startTime;
    deltaTime   = currentTime - asyncTimerPtr->lastTime;

    asyncTimerPtr->lastTime = currentTime;

    // Disable timer
    if (asyncTimerPtr->timer)
    {
        // Leave critical section
        CmtReleaseLock (asyncLock);

        CVIUSTEdiscardtimer (asyncTimerPtr->timer);

        // Call user's callback with EVENT_DISCARD
        if (asyncTimerPtr->callback)
            (*asyncTimerPtr->callback) ((int)0, asyncTimerPtr->userHandle,
                (int)EVENT_DISCARD, asyncTimerPtr->callbackData,
                (int) &timerTime, (int) &deltaTime);

        // Enter critical section
        CmtGetLock (asyncLock);

        asyncTimerPtr->timer        = NULL;
        asyncTimerPtr->count        = 0;
        asyncTimerPtr->callback     = NULL;
        asyncTimerPtr->callbackData = NULL;
    }

    // Add it to the top discarded timer list.
    asyncTimerPtr->prevTimer = NULL;
    asyncTimerPtr->nextTimer = asyncDiscardListHead;
    if (asyncDiscardListHead)
        asyncDiscardListHead->prevTimer = asyncTimerPtr;
    asyncDiscardListHead = asyncTimerPtr;

    // If no timers are left, free discard list, so users do not see unfreed memory
    if (!asyncTimerListHead)
    {
        while(asyncDiscardListHead)
        {
            asyncTimerPtr = asyncDiscardListHead;
            asyncDiscardListHead = asyncTimerPtr->nextTimer;

            free (asyncTimerPtr);
        }
        lastHandle = 1;
    }
}

//------------------------------------------------------------------------------
// Clean up routine for Async Timer
//------------------------------------------------------------------------------
int CVIFUNC DiscardAsyncTimer(int timerId)
{
    tAsyncTimerRec *asyncTimerPtr;

    // Call InitAsyncLibrary here instead of in runstate change callback
    if (!InitAsyncLibrary())
        return ASYNC_INTERNAL_ERR;

    // Enter critical section
    CmtGetLock (asyncLock);

    // Lookup timerId in list, if timerId is -1, disable all timers
    if (timerId == -1)
    {
        while(asyncTimerListHead) {
            // Remove the timer from the list - RemoveAsyncTimer will leave and reenter the critical section
            RemoveAsyncTimer(asyncTimerListHead);
        }

        lastHandle = 1;
    } else {
        asyncTimerPtr = FindAsyncTimer(timerId);

        if (!asyncTimerPtr)
        {
            // Leave critical section
            CmtReleaseLock (asyncLock);
            return ASYNC_TIMER_NOT_FOUND_ERR;
        }

        // Remove the timer from the list - RemoveAsyncTimer will leave and reenter the critical section
        RemoveAsyncTimer(asyncTimerPtr);
    }

    // Leave critical section
    CmtReleaseLock (asyncLock);

    return 0;
}

//------------------------------------------------------------------------------
// Suspend all enabled timers
//------------------------------------------------------------------------------
int CVIFUNC SuspendAsyncTimerCallbacks(void)
{
    // Call InitAsyncLibrary here instead of in runstate change callback
    if (!InitAsyncLibrary())
        return ASYNC_INTERNAL_ERR;

    // Enter critical section
    CmtGetLock (asyncLock);

    asyncTimerStatus = ASYNC_TIMER_DISABLED;

    // Leave critical section
    CmtReleaseLock (asyncLock);
    return 0;
}

//------------------------------------------------------------------------------
// Resume all enabled timers
//------------------------------------------------------------------------------
int CVIFUNC ResumeAsyncTimerCallbacks(void)
{
    // Call InitAsyncLibrary here instead of in runstate change callback
    if (!InitAsyncLibrary())
        return ASYNC_INTERNAL_ERR;

    // Enter critical section
    CmtGetLock (asyncLock);

    asyncTimerStatus = ASYNC_TIMER_ENABLED;

    // Leave critical section
    CmtReleaseLock (asyncLock);

    return 0;
}

//------------------------------------------------------------------------------
// This function sets the various settable attributes of the timer
//------------------------------------------------------------------------------
int CVIFUNC SetAsyncTimerAttribute(int timerId, int attribute, ...)
{
    va_list parmInfo;
    int error;

    va_start(parmInfo, attribute);
    error = SetAsyncTimerAttributeFromParmInfo(timerId,attribute, parmInfo);
    va_end(parmInfo);

    return error;
}

//------------------------------------------------------------------------------
// This function sets the various settable attributes of the timer from
// the given parmInfo
//------------------------------------------------------------------------------
int CVIFUNC SetAsyncTimerAttributeFromParmInfo(int timerId, int attribute,va_list parmInfo)
{
    tAsyncTimerRec *asyncTimerPtr;
    int error = UIENoError;

    // Call InitAsyncLibrary here instead of in runstate change callback
    if (!InitAsyncLibrary())
        return ASYNC_INTERNAL_ERR;

    if (attribute < ASYNC_ATTR_MIN || attribute > ASYNC_ATTR_MAX)
        return ASYNC_INVALID_PARAMETER_ERR;


    // Enter critical section
    CmtGetLock (asyncLock);

    asyncTimerPtr = FindAsyncTimer(timerId);
    if (!asyncTimerPtr)
        error =  ASYNC_TIMER_NOT_FOUND_ERR;
    else
    {
        switch(attribute)
        {
            case ASYNC_ATTR_ENABLED :
            {
                int status = va_arg(parmInfo,int);
                if (status == ASYNC_TIMER_ENABLED || status == ASYNC_TIMER_DISABLED)
                    asyncTimerPtr->status = status;
                else
                    error = ASYNC_INVALID_PARAMETER_ERR;
                break;
            }

            case ASYNC_ATTR_COUNT :
            {
                int count = va_arg(parmInfo,int);
                if (count != 0)
                    asyncTimerPtr->count = count;
                else
                    error = ASYNC_INVALID_PARAMETER_ERR;
                break;
            }

            case ASYNC_ATTR_INTERVAL :
            {
                double doubleInterval = va_arg(parmInfo,double);

                if (doubleInterval >= 0)
                {
                    double      doubleResolution = GetResolution ();
                    CVIUSTEtime interval;

                    if (doubleInterval < doubleResolution)
                        doubleInterval = doubleResolution;

                    ConvertDoubleToTime (doubleInterval, &interval);
                    if (CVIUSTEsettimer (asyncTimerPtr->timer, &interval))
                        asyncTimerPtr->interval = interval;
                    else
                        error = ASYNC_TIMER_FAIL_ERR;
                }
                else
                    error = ASYNC_INVALID_PARAMETER_ERR;
                break;
            }

            case ASYNC_ATTR_CALLBACK_DATA :
            {
                void * callbackData = va_arg(parmInfo,void *);
                asyncTimerPtr->callbackData = callbackData;
                break;
            }

            case ASYNC_ATTR_CALLBACK_FUNCTION_POINTER :
            {
                asyncTimerPtr->callback = va_arg(parmInfo,AsyncTimerCallbackPtr);
                break;
            }

            case ASYNC_ATTR_THREAD_PRIORITY :
            {
                if (realtime)
                    error = ASYNC_CANNOT_SET_ATTRIBUTE;
                else
                    error = ASYNC_ONLY_AVAILABLE_ON_REAL_TIME;
                break;
            }
        }
    }
    // Leave critical section
    CmtReleaseLock (asyncLock);

    return error;
}

//------------------------------------------------------------------------------
// This function gets the various attributes of the timer
//------------------------------------------------------------------------------
int CVIFUNC GetAsyncTimerAttribute(int timerId, int attribute, void *value)
{
    tAsyncTimerRec *asyncTimerPtr;
    int error = UIENoError;

    // Call InitAsyncLibrary here instead of in runstate change callback
    if (!InitAsyncLibrary())
        return ASYNC_INTERNAL_ERR;

    if (attribute < ASYNC_ATTR_MIN || attribute > ASYNC_ATTR_MAX)
        return ASYNC_INVALID_PARAMETER_ERR;

    // Enter critical section
    CmtGetLock (asyncLock);

    asyncTimerPtr = FindAsyncTimer(timerId);
    if (!asyncTimerPtr)
        error =  ASYNC_TIMER_NOT_FOUND_ERR;
    else
    {
        switch(attribute)
        {
            case ASYNC_ATTR_ENABLED :
            {
                *((int *) value) = asyncTimerPtr->status;
                break;
            }

            case ASYNC_ATTR_COUNT :
            {
                *((int *) value) = asyncTimerPtr->count;
                break;
            }

            case ASYNC_ATTR_INTERVAL :
            {
                *((double *) value) = ConvertTimeToDouble (&asyncTimerPtr->interval);
                break;
            }

            case ASYNC_ATTR_CALLBACK_DATA :
            {
                *((void **) value) = asyncTimerPtr->callbackData;
                break;
            }

            case ASYNC_ATTR_CALLBACK_FUNCTION_POINTER :
            {
                *((AsyncTimerCallbackPtr *) value) = asyncTimerPtr->callback;
                break;
            }

            case ASYNC_ATTR_THREAD_PRIORITY :
            {
                if (realtime)
                    *((int *) value) = CVIUSTEgetpriority (asyncTimerPtr->timer);
                else
                    error = ASYNC_ONLY_AVAILABLE_ON_REAL_TIME;
                break;
            }
        }
    }
    // Leave critical section
    CmtReleaseLock (asyncLock);

    return error;
}

//------------------------------------------------------------------------------
// This function gets the multi-media timer resolution used by async timers
//------------------------------------------------------------------------------
int CVIFUNC GetAsyncTimerResolution(double *resolutionInSecs)
{
    int error = UIENoError;

    *resolutionInSecs = 0.0;

    if (!InitAsyncLibrary())
        return ASYNC_INTERNAL_ERR;

    CmtGetLock (asyncLock);
    *resolutionInSecs = GetResolution();
    CmtReleaseLock (asyncLock);

    if (*resolutionInSecs == 0.0)
        return ASYNC_INTERNAL_ERR;
    return error;
}

//------------------------------------------------------------------------------
// This is the Multi Media Timer callback function
//------------------------------------------------------------------------------
static void CVICALLBACK LocalTimerCallbackFunc (void *data)
{
    double          currentTime, timerTime, deltaTime;
    tAsyncTimerRec *asyncTimerPtr = (tAsyncTimerRec *)data;

    // Enter critical section
    CmtGetLock (asyncLock);

    // If at a breakpoint then don't do anything
    if (!asyncTimerStatus)
    {
        // Leave critical section
        CmtReleaseLock (asyncLock);
        return;
    }

    // See if we need to leave
    if ((!asyncTimerPtr) || (asyncTimerPtr->status == ASYNC_TIMER_DISABLED))
    {
        // Leave critical section
        CmtReleaseLock (asyncLock);
        return;
    }

    // If count is zero, leave; otherwise decrement count if not less than zero
    if (!asyncTimerPtr->count)
    {
        // Remove the timer from the list - RemoveAsyncTimer will leave and reenter the critical section
        RemoveAsyncTimer(asyncTimerPtr);

        // Leave critical section
        CmtReleaseLock (asyncLock);
        return;
    }
    else if (asyncTimerPtr->count>0)
        asyncTimerPtr->count--;

    // No need to make this thread safe, it will not be reentered
    currentTime = CurrentTime ();
    if (asyncTimerPtr->firstTime) {
        asyncTimerPtr->firstTime = FALSE;
        asyncTimerPtr->startTime = currentTime;
        asyncTimerPtr->lastTime  = currentTime;
    }
    timerTime = currentTime - asyncTimerPtr->startTime;
    deltaTime = currentTime - asyncTimerPtr->lastTime;

    asyncTimerPtr->lastTime = currentTime;
    
    // We want to release Critical Section before callback so user's callback can take their time to do what they need to do
    CmtReleaseLock (asyncLock);

#if defined(_CVI_) && (_CVI_DEBUG_==0)
    //--------------------------------------------------------------------
    // The callback function and any code that is executed by it must be
    // called inside the EnterAsyncCallback/ExitAsyncCallback.
    //--------------------------------------------------------------------
    // Call CVI Library function
    //--------------------------------------------------------------------
     EnterAsyncCallback(asyncTimerPtr->environment);
#endif

    // Call user's callback
    if (asyncTimerPtr->callback)
        (*asyncTimerPtr->callback) ((int)0, asyncTimerPtr->userHandle,
            (int)EVENT_TIMER_TICK, asyncTimerPtr->callbackData,
            (int) &timerTime, (int) &deltaTime);

#if defined(_CVI_) && (_CVI_DEBUG_==0)
    //--------------------------------------------------------------------
    // The callback function and any code that is executed by it must be
    // called inside the EnterAsyncCallback/ExitAsyncCallback.
    //--------------------------------------------------------------------
    // Call CVI Library function
    //--------------------------------------------------------------------
    ExitAsyncCallback(asyncTimerPtr->environment);
#endif
    return;
}


//------------------------------------------------------------------------------
// Local Timer() function that is thread safe when initialized the first time
//------------------------------------------------------------------------------
static double CurrentTime (void)
{
    CVIUSTEtime localtime;

    CVIUSTElocaltime (&localtime);
    return ConvertTimeToDouble (&localtime);
}



//------------------------------------------------------------------------------
// InitAsyncLibrary
//------------------------------------------------------------------------------
static int InitAsyncLibrary(void)
{
    int  success = 1;
    char os[64];

    /* may have been created in previous execution */
    if (globalLock != 0)
        CmtGetLock (globalLock);

    else {
        char         lockName[MAX_PATHNAME_LEN+1];
        int          tmpLock;
        unsigned int processId;

#if defined (_NI_mswin32_) && _NI_mswin32_
        processId = GetCurrentProcessId ();
#endif

        // Create or Open existing Mutex as a temporary mutex
        sprintf (lockName, "National Instruments LabWindows/CVI Async Timer Library %d", processId);
        if (CmtNewLock (lockName, 0, &tmpLock) < 0)
            return 0;

        // Acquire Mutex
        CmtGetLock (tmpLock);

        // If global mutex handle was already initialized, release it
        if (globalLock != 0) {
            CmtDiscardLock (tmpLock);
            CmtGetLock (globalLock);
        }
        // Otherwise Create critical sections
        else
            globalLock = tmpLock;
    }

    // Init critical section if not already done
    if (asyncLock == 0)
        if (CmtNewLock (NULL, 0, &asyncLock) < 0)
            success = 0;

    if (CVIUSTEinitialize () == 0)
        success = 0;

    realtime = GetEnvironmentVariable ("OS", os, sizeof (os)) != 0
            && strnicmp(os, "PHARLAP", strlen ("PHARLAP"))    == 0;

    CmtReleaseLock (globalLock);

    return success;
}

//------------------------------------------------------------------------------
// Gets the resolution of the multimedia timer
//------------------------------------------------------------------------------
static double GetResolution (void)
{
    static double resolution = -1.0;
    CVIUSTEtime   clockResolution;

    if (resolution > 0.0)
        return resolution;

    CVIUSTEresolution (&clockResolution);
    resolution = ConvertTimeToDouble (&clockResolution);
    return resolution;
}
