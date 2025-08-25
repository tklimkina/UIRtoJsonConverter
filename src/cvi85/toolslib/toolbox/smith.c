/*-----------------------------------------------------------------------------------------------*/
/*                                                                                               */
/*      smith.c                                                                                  */
/*                                                                                               */
/*      Instrument driver source file                                                            */
/*                                                                                               */
/*      Associated files: smith.h                                                                */
/*                        smith.fp                                                               */
/*                                                                                               */
/*      Purpose:          The functions in this driver allow the user to convert an existing CVI */
/*                        Graph control into a specialized Smith Chart, plot impedances on that  */
/*                        chart, and convert between reflection coefficients and impedances.     */
/*                                                                                               */
/*-----------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/* Include headers                                                                               */
/*-----------------------------------------------------------------------------------------------*/
#include <ansi_c.h>
#include <userint.h>
#include "smith.h"
#include "toolbox.h"

#define DEFAULT_R_CIRCLE_COLOR VAL_DK_BLUE
#define DEFAULT_X_CIRCLE_COLOR VAL_BLUE
#define DEFAULT_BACK_COLOR     VAL_WHITE /* VAL_BLACK */
#define DEFAULT_FRAME_COLOR    VAL_GRAY

/*-----------------------------------------------------------------------------------------------*/
/* Define Smith Chart and complex double data structures                                         */
/*-----------------------------------------------------------------------------------------------*/
typedef struct {
    int     panel;                  /* panel containing the control       */
    int     control;                /* control ID of the converted graph  */
    int     xCircleColor;           /* color of x-circles                 */
    int     rCircleColor;           /* color of r-circles                 */
    int     backColor;              /* plot background color              */
    int     Pxmax;                  /* width of chart in pixels           */
    int     Pymax;                  /* height of chart in pixels          */
    int     numRCircles;            /* number of displayed r-circles      */
    int     numXCircles;            /* number of displayed x-circles      */
    int     Px;                     /* mouse pointer position             */
    int     Py;
    int     frameID;                /* background frame ID                */
    int     frameColor;             /* background frame color             */
    int     frameVisible;           /* is background frame visible?       */
    int     rCirclesVisible;        /* are r-circles visible?             */
    int     xCirclesVisible;        /* are x-circles visible?             */
    int     allowOptionsPopup;      /* do we want the left-click popup?   */
    int     allowCoordsPopup;       /* do we want the right-click popup?  */
    double  Vxmin;                  /* min value on X axis                */
    double  Vxmax;                  /* max value on X axis                */
    double  Vymin;                  /* min value on Y axis                */
    double  Vymax;                  /* max value on Y axis                */
    double  rCircleRadii[20];       /* array of displayed r-circles       */
    double  xCircleRadii[20];       /* array of displayed x-circles       */
    double  rCircleHandles[20];     /* list of r-circle plot handles      */
    double  xCircleHandles[40];     /* list of x-circle plot handles      */
}   SmithInfo;

typedef struct {
    double  x;
    double  y;
} DPoint;


/*-----------------------------------------------------------------------------------------------*/
/* Define module globals                                                                         */
/*-----------------------------------------------------------------------------------------------*/
static int          gInitDone = FALSE;  /* flag for initialization            */
static int          MB_Options;         /* options menu-bar handle            */
static int          MB_Coordinates;     /* coordinates menu-bar handle        */
static int          ColorHndl;          /* handle of color scheme popup panel */
static int          ABACUS_ID,          /* options menu items                 */
                    ZOOM_IN_ID,
                    ZOOM_OUT_ID,
                    RESET_ID,
                    COLOR_ID;
static int          COORDINATE_ID,      /* coordinates menu items             */
                    R_ID,
                    X_ID;
static int          COLOR_OK,           /* color panel popup controls         */
                    COLOR_CANCEL,
                    COLOR_GRAPH,
                    COLOR_R,
                    COLOR_X;
static ListType     gControlList;       /* global list of all controls        */
static SmithInfo*   gPoppedUpInfoPtr;   /* pointer to popped-up on chart      */


/*-----------------------------------------------------------------------------------------------*/
/* Prototype internal fucntions                                                                  */
/*-----------------------------------------------------------------------------------------------*/
static void         Zoom_In (int menuBar, int menuItem, void *callbackData, int panel);
static void         Zoom_Out (int menuBar, int menuItem, void *callbackData, int panel);
static void         Reset (int menuBar, int menuItem, void *callbackData, int panel);
static void         GetCoordinates(SmithInfo* tempInfoPtr);
static void         Coordinates_Fn (int menuBar, int menuItem, void *callbackData, int panel);
static void         SelectColor (int menuBar, int menuItem, void *callbackData, int panel);
static int          InitDriver(void);
static int          ChainedChartCB (int panel, int control, int event, void *callbackData,
                                    int eventData1, int eventData2);
static int          Color_Ok (int panel, int control, int event, void *callbackData,
                              int eventData1, int eventData2);
static int          Color_Cancel (int panel, int control, int event, void *callbackData,
                                  int eventData1, int eventData2);
static int          Color_rCircle (int panel, int control, int event, void *callbackData,
                                   int eventData1, int eventData2);
static int          Color_xCircle (int panel, int control, int event, void *callbackData,
                                   int eventData1, int eventData2);
//static int          DrawPlotFrame (SmithInfo* tempInfoPtr);
static int          PlotCircles (SmithInfo* tempInfoPtr);
int                 CVICALLBACK CompareInfoIds (void *item1, void *item2);
static SmithInfo*   FindInfoPtrFromControlIDs (int panelHandle, int controlID, int* itemPos);


/*-----------------------------------------------------------------------------------------------*/
/* Define exported fucntions                                                                     */
/*-----------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/* SMITH_ConvertFromGraph ():  Converts a graph control into a Smith Chart; maintains screen     */
/*                             position and size and initializes the coordinate system           */
/*-----------------------------------------------------------------------------------------------*/
int SMITH_ConvertFromGraph(int panel, int control, int useDefaultCircles, int numRCircles,
                           double rCircles[], int numXCircles, double xCircles[])
{
    int         returnVal = kNoError;
    int         i;
    int         controlStyle;
    SmithInfo   newItem;
    double      rDfltRadii[] = {0.1, 0.5, 1.0, 2.0, 4.0, 10.0};
    double      xDfltRadii[] = {0.5, 1.0, 2.0, 6.0};

    /* Initialize driver if not done already */
    if(!gInitDone)
        InitDriver();
    /* Verify that this is indeed a graph control */
    GetCtrlAttribute (panel, control, ATTR_CTRL_STYLE, &controlStyle);
    if (controlStyle != CTRL_GRAPH && controlStyle != CTRL_GRAPH_LS)
        returnVal = kErrorNotGraph;
    /* Verify the number of x and r circles */
    else if ((numRCircles > 20) || (numXCircles > 20))
    {
        returnVal == kErrorTooManyCircles;
    }
    else
    {
        /* Assign parameters and set default values */
        newItem.panel = panel;
        newItem.control = control;
        newItem.rCircleColor = DEFAULT_R_CIRCLE_COLOR;
        newItem.xCircleColor = DEFAULT_X_CIRCLE_COLOR;
        newItem.backColor    = DEFAULT_BACK_COLOR    ;
        newItem.frameColor   = DEFAULT_FRAME_COLOR   ;
        newItem.frameVisible = TRUE;
        newItem.Vxmin = -1.0;
        newItem.Vxmax = 1.0;
        newItem.Vymin = -1.0;
        newItem.Vymax = 1.0;
        newItem.rCirclesVisible = TRUE;
        newItem.xCirclesVisible = TRUE;
        newItem.allowOptionsPopup = TRUE;
        newItem.allowCoordsPopup = TRUE;
        GetCtrlAttribute (panel, control, ATTR_WIDTH, &(newItem.Pxmax));
        GetCtrlAttribute (panel, control, ATTR_HEIGHT, &(newItem.Pymax));
        /* Assign array data for plotted r and x circles */
        if (useDefaultCircles)
        {
            newItem.numRCircles = (sizeof(rDfltRadii)/sizeof(double));
            newItem.numXCircles = (sizeof(xDfltRadii)/sizeof(double));
            for (i=0; i<newItem.numRCircles; i++)
                newItem.rCircleRadii[i] = rDfltRadii[i];
            for (i=0; i<newItem.numXCircles; i++)
                newItem.xCircleRadii[i] = xDfltRadii[i];
        }
        else
        {
            newItem.numRCircles = numRCircles;
            newItem.numXCircles = numXCircles;
            for (i=0; i<newItem.numRCircles; i++)
                newItem.rCircleRadii[i] = rCircles[i];
            for (i=0; i<newItem.numXCircles; i++)
                newItem.xCircleRadii[i] = xCircles[i];
        }
        /* Adjust graph settings */
        SetCtrlAttribute (panel, control, ATTR_BORDER_VISIBLE, 0);
        SetCtrlAttribute (panel, control, ATTR_INNER_LOG_MARKERS_VISIBLE, 0);
        SetCtrlAttribute (panel, control, ATTR_XLABEL_VISIBLE, 0);
        SetCtrlAttribute (panel, control, ATTR_YLABEL_VISIBLE, 0);
        SetCtrlAttribute (panel, control, ATTR_VISIBLE, 1);
        SetCtrlAttribute (panel, control, ATTR_SMOOTH_UPDATE, 1);
        SetCtrlAttribute (panel, control, ATTR_COPY_ORIGINAL_DATA, 1);
        SetCtrlAttribute (panel, control, ATTR_PLOT_BGCOLOR, newItem.backColor);
        SetCtrlAttribute (panel, control, ATTR_BORDER_VISIBLE, 0);
        SetCtrlAttribute (panel, control, ATTR_GRID_COLOR, VAL_GREEN);
        SetCtrlAttribute (panel, control, ATTR_XGRID_VISIBLE, 0);
        SetCtrlAttribute (panel, control, ATTR_YGRID_VISIBLE, 0);
        SetCtrlAttribute (panel, control, ATTR_YMARK_ORIGIN, 0);
        SetCtrlAttribute (panel, control, ATTR_XMARK_ORIGIN, 0);
        SetCtrlAttribute (panel, control, ATTR_REFRESH_GRAPH, 0);
        SetAxisScalingMode (panel, control, VAL_XAXIS, VAL_MANUAL, newItem.Vxmin, newItem.Vxmax);
        SetAxisScalingMode (panel, control, VAL_LEFT_YAXIS, VAL_MANUAL, newItem.Vymin,
                            newItem.Vymax);
        /* Chain a callback function for pop-up menus */
        ChainCtrlCallback (panel, control, ChainedChartCB, 0,
                       "Abacus Popup Menu");
        /* Insert this chart object into our global list */
        ListInsertItem (gControlList, &newItem, END_OF_LIST);
        /* Draw the chart frame */
        /* DrawPlotFrame (&newItem); */
        /* Plot the r and x circles and return */
        PlotCircles (&newItem);
        RefreshGraph (newItem.panel, newItem.control);
    }
    return returnVal;
}


/*-----------------------------------------------------------------------------------------------*/
/* SMITH_ImpedanceToCoeff ():  Converts a normalized impedance (r=jx) into a Reflexion Coeff     */
/*-----------------------------------------------------------------------------------------------*/
int SMITH_ImpedanceToCoeff(Complex *Zin, Complex *RefCoeff)
{
    int     returnVal = kNoError;
    double  r, x;
    double  HoldDenom;

    /* Perform calculations */
    r=Zin->Real;
    x=Zin->Im;
    HoldDenom = ((r+1)*(r+1))+(x*x);
    RefCoeff->Real = ((r*r)+(x*x)-1)/HoldDenom;
    RefCoeff->Im = (2*x)/HoldDenom;
    if(HoldDenom==0.0)
        returnVal = kErrorNullImpedance;
    return returnVal;
}


/*-----------------------------------------------------------------------------------------------*/
/* SMITH_CoeffToImpedance ():  Converts a Reflexion Coeff into a normalized impedance (r+jx)     */
/*-----------------------------------------------------------------------------------------------*/
int SMITH_CoeffToImpedance( Complex *RefCoeff, Complex *Zout)
{
    int     returnVal = kNoError;
    double  Gr;
    double  Gi;
    double  HoldDenom;

    /* Perform calculations */
    Gr=RefCoeff->Real;
    Gi=RefCoeff->Im;
    HoldDenom = ((1-Gr)*(1-Gr))+(Gi*Gi);
    Zout->Real = (1-(Gr*Gr)-(Gi*Gi))/HoldDenom;
    Zout->Im = (2*Gi)/HoldDenom;
    if(HoldDenom==0.0)
        returnVal = kErrorNullCoeff;
    return returnVal;
}


/*-----------------------------------------------------------------------------------------------*/
/* SMITH_PlotImpedancePoint ():  Plots a single normalized impedance point on the Smith Chart and*/
/*                               returns a standard plot handle to that point                    */
/*-----------------------------------------------------------------------------------------------*/
int SMITH_PlotImpedancePoint(int panel, int control, Complex *Point, int pointStyle, int color)
{

    int         returnVal = kNoError;
    int         itemPos;
    Complex     RefCoeff;
    SmithInfo*  tempInfoPtr;

    /* Find the appropriate control in the global list */
    tempInfoPtr = FindInfoPtrFromControlIDs (panel, control, &itemPos);
    if (tempInfoPtr == NULL)
        returnVal = kErrorUnknownChart;
    else
    {
        /* Convert the impedance into a reflection coefficient that we may plot easily */
        SMITH_ImpedanceToCoeff(Point, &RefCoeff);
        /* Plot the point */
        returnVal = PlotPoint (panel, control, RefCoeff.Real, RefCoeff.Im, pointStyle, color);
        RefreshGraph (panel, control);
    }
    return returnVal;
}


/*-----------------------------------------------------------------------------------------------*/
/* SMITH_PlotImpedanceWaveform ():  Plots an array of impedance points on the Smith Chart and    */
/*                                  returns a standard plot handle                               */
/*-----------------------------------------------------------------------------------------------*/
int SMITH_PlotImpedanceWaveform (int panel, int control, Complex* impedanceArray,
                                 int numberOfPoints, int plotStyle, int pointStyle, int lineStyle,
                                 int Color)
{
    int         i;
    int         returnVal = kNoError;
    int         itemPos;
    double      *xDataPtr;
    double      *xBasePtr;
    double      *yDataPtr;
    double      *yBasePtr;
    Complex     RefCoeff;
    Complex*    pComplex;
    SmithInfo*  tempInfoPtr;

    /* Find the appropriate control in the global list */
    tempInfoPtr = FindInfoPtrFromControlIDs (panel, control, &itemPos);
    if (tempInfoPtr == NULL)
        returnVal = kErrorUnknownChart;
    else
    {
        /* Allocate space for our complex data; we need to transform all points into coefficients */
        pComplex = impedanceArray;
        xBasePtr = xDataPtr = (double*)malloc(numberOfPoints*sizeof(double));
        if(xBasePtr==NULL)
            returnVal = kErrorMemory;
        yBasePtr = yDataPtr = (double*)malloc(numberOfPoints*sizeof(double));
        if(yBasePtr==NULL)
            returnVal = kErrorMemory;
        /* Transform all impedances into coefficients, and plot them */
        if (returnVal == kNoError)
        {
            for (i=0; i<numberOfPoints; i++)
            {
                SMITH_ImpedanceToCoeff(pComplex++, &RefCoeff);
                *xDataPtr++ = RefCoeff.Real;
                *yDataPtr++ = RefCoeff.Im;
            }
            returnVal = PlotXY (panel, control, xBasePtr, yBasePtr, numberOfPoints, VAL_DOUBLE,
                                VAL_DOUBLE, plotStyle, pointStyle, lineStyle, 1, Color);
            RefreshGraph (panel, control);
            /* Free memory */
            free(xBasePtr);
            free(yBasePtr);
        }
    }
    return returnVal;
}


/*-----------------------------------------------------------------------------------------------*/
/* SMITH_PlotConstantMagArc ():  Plots an arc of constant coefficient magnitude from one         */
/*                               normalized impedance point to another 'wavelengths' away        */
/*-----------------------------------------------------------------------------------------------*/
int SMITH_PlotConstantMagArc (int panel, int control, Complex* point, double wavelengths,
                              int direction, int color)
{
    int         returnVal = kNoError;
    int         itemPos;
    double      arcAngle;
    double      beginAngle;
    double      squareWidth;
    Complex     refCoeff;
    SmithInfo*  tempInfoPtr;

    /* Find the appropriate control in the global list */
    tempInfoPtr = FindInfoPtrFromControlIDs (panel, control, &itemPos);
    if (tempInfoPtr == NULL)
        returnVal = kErrorUnknownChart;
    else
    {
        /* Convert impedance point into coefficient */
        if (SMITH_ImpedanceToCoeff(point, &refCoeff) != kErrorNullCoeff)
        {
            /* Calculate angles */
            beginAngle = sin(refCoeff.Im/refCoeff.Real);
            beginAngle *= 180/3.14159;
            if (refCoeff.Real <= 0)
                beginAngle -= 180;
            arcAngle = 360 * wavelengths/0.5;
            if (direction)
                /* Toward generator */
                arcAngle = -1*arcAngle;
            /* Calculate the enclosing square and draw the arc */
            squareWidth = sqrt (refCoeff.Real*refCoeff.Real + refCoeff.Im*refCoeff.Im);
            returnVal = PlotArc (panel, control, squareWidth, squareWidth, -1*squareWidth,
                                 -1*squareWidth, 10*beginAngle, 10*arcAngle, color,
                                 VAL_TRANSPARENT);
            RefreshGraph (panel, control);
        }
        else
            returnVal = kErrorNullCoeff;
    }
    return returnVal;
}


/*-----------------------------------------------------------------------------------------------*/
/* SMITH_DeletePlot ():  Erases a specific plot on a Smith Chart                                 */
/*-----------------------------------------------------------------------------------------------*/
int SMITH_DeletePlot (int panel, int control, int plotHandle)
{
    int         returnVal = kNoError;
    int         itemPos;
    SmithInfo*  tempInfoPtr;

    /* Find the appropriate control in the global list */
    tempInfoPtr = FindInfoPtrFromControlIDs (panel, control, &itemPos);
    if (tempInfoPtr == NULL)
        returnVal = kErrorUnknownChart;
    else
    {
        if (DeleteGraphPlot (panel, control, plotHandle, VAL_DELAYED_DRAW) == -4)
            returnVal = kErrorUnknownPlot;
    }
    /* If the user deleted all plots, redraw the r and x circles */
    if (plotHandle = -1)
        PlotCircles (tempInfoPtr);
    RefreshGraph (panel, control);
    return returnVal;
}


/*-----------------------------------------------------------------------------------------------*/
/* SMITH_DiscardChart ():  Erases a Smith Chart and cleans up associated memory                  */
/*-----------------------------------------------------------------------------------------------*/
int SMITH_DiscardChart (int panel, int control)
{
    int         numItems;
    int         returnVal = kNoError;
    int         itemPos;
    SmithInfo*  tempInfoPtr;

    /* Find the appropriate control in the global list */
    tempInfoPtr = FindInfoPtrFromControlIDs (panel, control, &itemPos);
    if (tempInfoPtr == NULL)
        returnVal = kErrorUnknownChart;
    else
    {
        /* Remove this item from the global list */
        ListRemoveItem (gControlList, 0, itemPos);
        /* Discard the control */
        DiscardCtrl (panel, control);
        /* Kill the global control list if this is the last item */
        numItems = ListNumItems (gControlList);
        if (numItems == 0)
            ListDispose (gControlList);
        gInitDone = FALSE;
        returnVal = numItems;
    }
    return returnVal;
}


/*-----------------------------------------------------------------------------------------------*/
/* Define Instrument driver internal functions                                                   */
/*-----------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/* InitDriver ():  Initializes driver data structures and UI components                          */
/*-----------------------------------------------------------------------------------------------*/
static int InitDriver(void)
{
    /* Set the appropriate flag */
    gInitDone = TRUE;
    /* Create a global list to maintain info for all Smith Charts */
    gControlList = ListCreate (sizeof(SmithInfo));
    /* Create the Options menu */
    MB_Options  = NewMenuBar (0);
    ABACUS_ID   = NewMenu (MB_Options, "Abacus", -1);
    ZOOM_IN_ID  = NewMenuItem (MB_Options, ABACUS_ID, "Zoom In", -1, 0, Zoom_In, 0);
    ZOOM_OUT_ID = NewMenuItem (MB_Options, ABACUS_ID, "Zoom Out", -1, 0, Zoom_Out, 0);
    RESET_ID    = NewMenuItem (MB_Options, ABACUS_ID, "Reset", -1, 0, Reset, 0);
    COLOR_ID    = NewMenuItem (MB_Options, ABACUS_ID, "Colors", -1, 0, SelectColor, 0);
    /* Create the Cordinates menu */
    MB_Coordinates = NewMenuBar (0);
    COORDINATE_ID  = NewMenu (MB_Coordinates, "Coordinates", -1);
    R_ID=-1;
    X_ID=-1;
    /* Create the Color Preferences panel */
    ColorHndl = NewPanel (0, "Choose Color Scheme", 70, 170, 195, 305);
    COLOR_OK = NewCtrl (ColorHndl, CTRL_SQUARE_COMMAND_BUTTON, "__OK", 149, 138);
    InstallCtrlCallback (ColorHndl, COLOR_OK, Color_Ok, 0);
    COLOR_CANCEL = NewCtrl (ColorHndl, CTRL_SQUARE_COMMAND_BUTTON, "__CANCEL", 149, 228);
    InstallCtrlCallback (ColorHndl, COLOR_CANCEL, Color_Cancel, 0);
    COLOR_GRAPH = NewCtrl (ColorHndl, CTRL_GRAPH, "", 10, 134);
    SetCtrlAttribute (ColorHndl, COLOR_GRAPH, ATTR_WIDTH, 149);
    SetCtrlAttribute (ColorHndl, COLOR_GRAPH, ATTR_HEIGHT, 119);
    SetCtrlAttribute (ColorHndl, COLOR_GRAPH, ATTR_XDIVISIONS, 2);
    SetCtrlAttribute (ColorHndl, COLOR_GRAPH, ATTR_YDIVISIONS, 2);
    SetCtrlAttribute (ColorHndl, COLOR_GRAPH, ATTR_XDIVISIONS, 2);
    SetCtrlAttribute (ColorHndl, COLOR_GRAPH, ATTR_PLOT_BGCOLOR, VAL_BLACK);
    SetCtrlAttribute (ColorHndl, COLOR_GRAPH, ATTR_BORDER_VISIBLE, 0);
    SetAxisRange (ColorHndl, COLOR_GRAPH, VAL_MANUAL, 0, 90, VAL_MANUAL, 0, 90);
    COLOR_R = NewCtrl (ColorHndl, CTRL_COLOR_NUMERIC, "r-Circle Color", 31, 24);
    InstallCtrlCallback (ColorHndl, COLOR_R, Color_rCircle, 0);
    COLOR_X = NewCtrl (ColorHndl, CTRL_COLOR_NUMERIC, "x-Circle Color", 108, 24);
    InstallCtrlCallback (ColorHndl, COLOR_X, Color_xCircle, 0);
    return (0);
}


/*-----------------------------------------------------------------------------------------------*/
/* DrawPlotFrame ():  Draws the Smith Chart frame with labels and scales                         */
/*-----------------------------------------------------------------------------------------------*/
#if 0
static int DrawPlotFrame (SmithInfo* tempInfoPtr)
{
    int         left;
    int         top;
    int         height;
    int         width;
    int         frameLines;
    int         frameDrawCtr;
    int         returnVal = kNoError;

    if (tempInfoPtr == NULL)
        returnVal = kErrorUnknownChart;
    else
    {
        /* Create the plot label frame */
        /* Get original size/location info */
        GetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->control, ATTR_LEFT, &left);
        GetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->control, ATTR_TOP, &top);
        GetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->control, ATTR_HEIGHT, &height);
        GetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->control, ATTR_WIDTH, &width);
        /* Create a canvas control over the graph */
        tempInfoPtr->frameID = NewCtrl (tempInfoPtr->panel, CTRL_CANVAS, "", top, left);
        SetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->frameID, ATTR_HEIGHT,
                          tempInfoPtr->Pymax);
        SetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->frameID, ATTR_WIDTH,
                          tempInfoPtr->Pxmax);
        /* Shrink the graph to 80% original size */
        SetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->control, ATTR_WIDTH, (int)(width*0.8));
        SetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->control, ATTR_HEIGHT,
                          (int)(height*0.8));
        SetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->control, ATTR_LEFT,
                          (int)(width*0.1+left));
        SetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->control, ATTR_TOP,
                          (int)(height*0.1+top));
        /* Adjust structure members */
        tempInfoPtr->Pymax = (int)(height*0.8);
        tempInfoPtr->Pxmax = (int)(width*0.8);
        /* Draw the frame inside edge */
        SetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->frameID, ATTR_PICT_BGCOLOR,
                          VAL_TRANSPARENT);
        SetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->frameID, ATTR_PEN_COLOR, VAL_LT_GRAY);
        SetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->frameID, ATTR_PEN_WIDTH, 2);
        CanvasDrawOval (tempInfoPtr->panel, tempInfoPtr->frameID, MakeRect (-1+height*0.1,
                                                                            -1+width*0.1,
                                                                            1+tempInfoPtr->Pymax,
                                                                            1+tempInfoPtr->Pxmax),
                                                                            VAL_DRAW_FRAME);
        /* Draw the frame body */
        SetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->frameID, ATTR_PEN_COLOR,
                          tempInfoPtr->frameColor);
        SetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->frameID, ATTR_PEN_WIDTH, 4);
        frameLines = 3*(int) (sqrt(height/2*height/2+width/2*width/2)
                              - sqrt ((tempInfoPtr->Pxmax/2.0)*(tempInfoPtr->Pxmax/2.0)
                                      + (tempInfoPtr->Pymax/2.0)*(tempInfoPtr->Pymax/2.0)));
        for (frameDrawCtr=0; frameDrawCtr < frameLines; frameDrawCtr++)
            CanvasDrawOval (tempInfoPtr->panel, tempInfoPtr->frameID,
                            MakeRect (-4+height*0.1-frameDrawCtr, -4+width*0.1-frameDrawCtr,
                                      8+tempInfoPtr->Pymax+2*frameDrawCtr,
                                      8+tempInfoPtr->Pxmax+2*frameDrawCtr),
                            VAL_DRAW_FRAME);
        /* Draw the frame outside edge */
        SetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->frameID, ATTR_PEN_COLOR, VAL_WHITE);
        SetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->frameID, ATTR_PEN_WIDTH, 2);
        CanvasDrawLine (tempInfoPtr->panel, tempInfoPtr->frameID,
                        MakePoint (0, 0), MakePoint (width, 0));
        CanvasDrawLine (tempInfoPtr->panel, tempInfoPtr->frameID,
                        MakePoint (0, 0), MakePoint (0, height));
        SetCtrlAttribute (tempInfoPtr->panel, tempInfoPtr->frameID, ATTR_PEN_COLOR, VAL_DK_GRAY);
        CanvasDrawLine (tempInfoPtr->panel, tempInfoPtr->frameID,
                        MakePoint (0, height-1), MakePoint (width, height-1));
        CanvasDrawLine (tempInfoPtr->panel, tempInfoPtr->frameID,
                        MakePoint (width-1, 0), MakePoint (width-1, height));

    }
    return returnVal;
}
#endif


/*-----------------------------------------------------------------------------------------------*/
/* PlotCircles ():  Draws the background Smith Chart r and x circles                             */
/*-----------------------------------------------------------------------------------------------*/
static int PlotCircles(SmithInfo* tempInfoPtr)
{
    int         i;
    int         circleCtr;
    int         returnVal = kNoError;
    double      tempRadius;
    DPoint      tempCenter;

    if (tempInfoPtr == NULL)
        returnVal = kErrorUnknownChart;
    else
    {
        /* Plot the r circles */
        if (tempInfoPtr->rCirclesVisible)
        {
            circleCtr = tempInfoPtr->numRCircles;
            for (i=0; i<circleCtr; i++)
            {
                tempCenter.x = tempInfoPtr->rCircleRadii[i]/(1+tempInfoPtr->rCircleRadii[i]);
                tempCenter.y = 0;
                tempRadius = 1/(1+tempInfoPtr->rCircleRadii[i]);
                /* Keep the background plot handles so we can alter them as attributes */
                tempInfoPtr->rCircleHandles[i] = PlotOval (tempInfoPtr->panel,
                                                           tempInfoPtr->control,
                                                           tempCenter.x-tempRadius,
                                                           tempCenter.y+tempRadius,
                                                           tempCenter.x+tempRadius,
                                                           tempCenter.y-tempRadius,
                                                           tempInfoPtr->rCircleColor,
                                                           VAL_TRANSPARENT);
            }
        }
        if (tempInfoPtr->xCirclesVisible)
        {
            /* Plot the x circles */
            circleCtr = tempInfoPtr->numXCircles;
            for (i=0; i<circleCtr; i++)
            {
                tempCenter.x = 1.0;
                tempCenter.y = 1/tempInfoPtr->xCircleRadii[i];
                tempRadius = 1/tempInfoPtr->xCircleRadii[i];
                tempInfoPtr->xCircleHandles[i] = PlotOval (tempInfoPtr->panel,
                                                           tempInfoPtr->control,
                                                           tempCenter.x-tempRadius,
                                                           tempCenter.y+tempRadius,
                                                           tempCenter.x+tempRadius,
                                                           tempCenter.y-tempRadius,
                                                           tempInfoPtr->xCircleColor,
                                                           VAL_TRANSPARENT);
                tempCenter.y = -1*1/tempInfoPtr->xCircleRadii[i];
                tempInfoPtr->xCircleHandles[i+20] = PlotOval (tempInfoPtr->panel,
                                                              tempInfoPtr->control,
                                                              tempCenter.x-tempRadius,
                                                              tempCenter.y+tempRadius,
                                                              tempCenter.x+tempRadius,
                                                              tempCenter.y-tempRadius,
                                                              tempInfoPtr->xCircleColor,
                                                              VAL_TRANSPARENT);
            }
        }
    }
    return returnVal;
}


/*-----------------------------------------------------------------------------------------------*/
/* CompareInfoIDs ():  Compares control ID and panel handle of two SmithInfo items               */
/*-----------------------------------------------------------------------------------------------*/
int CVICALLBACK CompareInfoIDs (void *item1, void *item2)
{
    if ((((SmithInfo*)item1)->control == ((SmithInfo*)item2)->control)
        && (((SmithInfo*)item1)->panel == ((SmithInfo*)item2)->panel))
        return 0;
    else
        return -1;
}


/*-----------------------------------------------------------------------------------------------*/
/* FindInfoPtrFromControlIDs ():  Obtains a pointer to the correct SmithInfo item given the      */
/*                                panel handle and control ID                                    */
/*-----------------------------------------------------------------------------------------------*/
static SmithInfo* FindInfoPtrFromControlIDs (int panelHandle, int controlID, int* itemPos)
{
    int         infoLocation;
    SmithInfo   comparisonInfo;
    SmithInfo*  tempInfoPtr;

    /* Set up the item which we will use for comparison */
    comparisonInfo.panel = panelHandle;
    comparisonInfo.control = controlID;
    /* Find the index of the item that matches */
    infoLocation = ListFindItem (gControlList, &comparisonInfo,
                                 FRONT_OF_LIST, CompareInfoIDs);
    *itemPos = infoLocation;
    /* Get a pointer to the item and return it */
    tempInfoPtr = ListGetPtrToItem (gControlList, infoLocation);
    return tempInfoPtr;
}


/*-----------------------------------------------------------------------------------------------*/
/* ChainedChartCB ():  Responds to user events on the Smith Chart, chaining any callbacks to it, */
/*                     to display coordinates or adjust options.                                 */
/*-----------------------------------------------------------------------------------------------*/
static int ChainedChartCB (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2) {

    int         X, Y, LBD, RBD;
    int         itemPos;
    int         returnVal = kNoError;
    SmithInfo*  tempInfoPtr;

    switch (event)
    {
        case EVENT_RIGHT_CLICK:
            /* Find the appropriate control in the global list */
            tempInfoPtr = FindInfoPtrFromControlIDs (panel, control, &itemPos);
            if (tempInfoPtr == NULL)
                returnVal = kErrorUnknownChart;
            else
            {
                /* Launch the options popup, if appropriate */
                if (tempInfoPtr->allowOptionsPopup)
                {
                    GetRelativeMouseState (panel, control, &X, &Y, &LBD, &RBD, NULL);
                    tempInfoPtr->Px = X;
                    tempInfoPtr->Py = tempInfoPtr->Pymax-Y;
                    GetRelativeMouseState (panel, 0, &X, &Y, &LBD, &RBD, NULL);
                    gPoppedUpInfoPtr = tempInfoPtr;
                    RunPopupMenu (MB_Options, ABACUS_ID, panel, Y, X, 0, 0, 0, 0);
                }
            }
            break;
        case EVENT_LEFT_CLICK:
            /* Find the appropriate control in the global list */
            tempInfoPtr = FindInfoPtrFromControlIDs (panel, control, &itemPos);
            if (tempInfoPtr == NULL)
                returnVal = kErrorUnknownChart;
            else
            {
                /* Launch the colors popup, if appropriate */
                if (tempInfoPtr->allowCoordsPopup)
                {
                    GetRelativeMouseState (panel, control, &X, &Y, &LBD, &RBD, NULL);
                    tempInfoPtr->Px = X;
                    tempInfoPtr->Py = tempInfoPtr->Pymax-Y;
                    GetCoordinates(tempInfoPtr);
                    GetRelativeMouseState (panel, 0, &X, &Y, &LBD, &RBD, NULL);
                    RunPopupMenu (MB_Coordinates, COORDINATE_ID, panel, Y, X, 0, 0, 0, 0);
                }
            }
            break;
    }
    return returnVal;
}


/*-----------------------------------------------------------------------------------------------*/
/* ZoomIn ():  Zooms the Smith Chart by updating the system boundary information, triggered      */
/*             by the right-click popup menu item                                                */
/*-----------------------------------------------------------------------------------------------*/
static void Zoom_In (int menuBar, int menuItem, void *callbackData, int panel)
{
    double Vx, Vy;
    double DeltaVx, DeltaVy;
    double NewVxmin, NewVxmax, NewVymin, NewVymax;

    /* Obtain coordinates and curent layout */
    Vx = gPoppedUpInfoPtr->Vxmin
         + gPoppedUpInfoPtr->Px * (gPoppedUpInfoPtr->Vxmax-gPoppedUpInfoPtr->Vxmin)
                                  / gPoppedUpInfoPtr->Pxmax;
    Vy = gPoppedUpInfoPtr->Vymin + gPoppedUpInfoPtr->Py
         * (gPoppedUpInfoPtr->Vymax-gPoppedUpInfoPtr->Vymin)
         /gPoppedUpInfoPtr->Pymax;
    DeltaVx = (gPoppedUpInfoPtr->Vxmax-gPoppedUpInfoPtr->Vxmin)/4.0;
    NewVxmax = Vx + DeltaVx;
    NewVxmin = Vx - DeltaVx;
    DeltaVy = (gPoppedUpInfoPtr->Vymax-gPoppedUpInfoPtr->Vymin)/4.0;
    NewVymax = Vy + DeltaVy;
    NewVymin = Vy - DeltaVy;
    /* Set the graph scale and update variables */
    SetAxisRange (panel, gPoppedUpInfoPtr->control, VAL_MANUAL, NewVxmin, NewVxmax, VAL_MANUAL,
                  NewVymin, NewVymax);
    gPoppedUpInfoPtr->Vxmin = NewVxmin;
    gPoppedUpInfoPtr->Vxmax = NewVxmax;
    gPoppedUpInfoPtr->Vymin = NewVymin;
    gPoppedUpInfoPtr->Vymax = NewVymax;
    /* Refresh the plot circles */
    PlotCircles(gPoppedUpInfoPtr);
}


/*-----------------------------------------------------------------------------------------------*/
/* ZoomOut ():  Zooms the Smith Chart by updating the system boundary information, triggered     */
/*              by the right-click popup menu                                                    */
/*-----------------------------------------------------------------------------------------------*/
static void Zoom_Out (int menuBar, int menuItem, void *callbackData, int panel) {

    double Vx, Vy;
    double DeltaVx, DeltaVy;
    double NewVxmin, NewVxmax, NewVymin, NewVymax;

    /* Obtain coordinates and curent layout */
    Vx = gPoppedUpInfoPtr->Vxmin + gPoppedUpInfoPtr->Px
         * (gPoppedUpInfoPtr->Vxmax-gPoppedUpInfoPtr->Vxmin)/gPoppedUpInfoPtr->Pxmax;
    Vy = gPoppedUpInfoPtr->Vymin + gPoppedUpInfoPtr->Py
         * (gPoppedUpInfoPtr->Vymax-gPoppedUpInfoPtr->Vymin)/gPoppedUpInfoPtr->Pymax;
    DeltaVx = gPoppedUpInfoPtr->Vxmax-gPoppedUpInfoPtr->Vxmin;
    NewVxmax = Vx + DeltaVx;
    NewVxmin = Vx - DeltaVx;
    DeltaVy = gPoppedUpInfoPtr->Vymax-gPoppedUpInfoPtr->Vymin;
    NewVymax = Vy + DeltaVy;
    NewVymin = Vy - DeltaVy;
    /* Set the graph scale and update variables */
    SetAxisRange (panel, gPoppedUpInfoPtr->control, VAL_MANUAL, NewVxmin, NewVxmax, VAL_MANUAL,
                  NewVymin, NewVymax);
    gPoppedUpInfoPtr->Vxmin = NewVxmin;
    gPoppedUpInfoPtr->Vxmax = NewVxmax;
    gPoppedUpInfoPtr->Vymin = NewVymin;
    gPoppedUpInfoPtr->Vymax = NewVymax;
    /* Update the plot r and x circles */
    PlotCircles(gPoppedUpInfoPtr);
}


/*-----------------------------------------------------------------------------------------------*/
/* Reset ():  Rescales the chart to default coordinates, triggered by a popup menu selection     */
/*-----------------------------------------------------------------------------------------------*/
static void Reset (int menuBar, int menuItem, void *callbackData, int panel) {

    SetAxisRange (panel, gPoppedUpInfoPtr->control, VAL_MANUAL, -1.0, 1.0, VAL_MANUAL, -1.0, 1.0);
    gPoppedUpInfoPtr->Vxmin = -1.0;
    gPoppedUpInfoPtr->Vxmax = 1.0;
    gPoppedUpInfoPtr->Vymin = -1.0;
    gPoppedUpInfoPtr->Vymax = 1.0;
    PlotCircles(gPoppedUpInfoPtr);
}


/*-----------------------------------------------------------------------------------------------*/
/* GetCoordinates ():  Determines and displays the impedance (r+jx) at current mouse location,   */
/*                     utilized by the left-click control callback function                      */
/*-----------------------------------------------------------------------------------------------*/
static void GetCoordinates(SmithInfo* tempInfoPtr)
{
    int     Valid_Flag=FALSE;
    char    ItemStrR[20];
    char    ItemStrX[20];
    Complex V;
    Complex Zout;

    V.Real = tempInfoPtr->Vxmin + tempInfoPtr->Px * (tempInfoPtr->Vxmax-tempInfoPtr->Vxmin)
                                                     / tempInfoPtr->Pxmax;
    V.Im   = tempInfoPtr->Vymin + tempInfoPtr->Py * (tempInfoPtr->Vymax-tempInfoPtr->Vymin)
                                                     / tempInfoPtr->Pymax;
    if ((V.Real*V.Real + V.Im*V.Im)<=1)
    {
        Valid_Flag=TRUE;
        SMITH_CoeffToImpedance(&V, &Zout);
        sprintf(ItemStrR, "R = %f", Zout.Real);
        sprintf(ItemStrX, "X = %f", Zout.Im);
    }
    else
    {
        sprintf(ItemStrR, "R = Invalid");
        sprintf(ItemStrX, "X = Invalid");
    }
    /* Discard menu items if they exist */
    if (R_ID!=-1)
    {
        DiscardMenuItem (MB_Coordinates, R_ID);
        R_ID=-1;
        DiscardMenuItem (MB_Coordinates, X_ID);
        X_ID=-1;
    }
    /* Create menu items displaying the coordinates */
    R_ID = NewMenuItem (MB_Coordinates, COORDINATE_ID, ItemStrR, -1, 0, Coordinates_Fn, 0);
    X_ID = NewMenuItem (MB_Coordinates, COORDINATE_ID, ItemStrX, -1, 0, Coordinates_Fn, 0);
}


/*-----------------------------------------------------------------------------------------------*/
/* Coordinates_Fn ():  Responds to the popup cordinates menu items to discard them               */
/*-----------------------------------------------------------------------------------------------*/
static void Coordinates_Fn (int menuBar, int menuItem, void *callbackData, int panel)
{
    DiscardMenuItem (MB_Coordinates, R_ID);
    R_ID=-1;
    DiscardMenuItem (MB_Coordinates, X_ID);
    X_ID=-1;
}


/*-----------------------------------------------------------------------------------------------*/
/* SelectColor ():  Responds to the right-click popup item to let the user change color schemes  */
/*-----------------------------------------------------------------------------------------------*/
static void SelectColor (int menuBar, int menuItem, void *callbackData, int panel)
{
    SetCtrlVal(ColorHndl, COLOR_R, gPoppedUpInfoPtr->rCircleColor);
    SetCtrlVal(ColorHndl, COLOR_X, gPoppedUpInfoPtr->xCircleColor);
    /* Plot sample r and x circles */
    PlotOval (ColorHndl, COLOR_GRAPH, 0, 0, 90, 90, gPoppedUpInfoPtr->rCircleColor,
              VAL_TRANSPARENT);
    PlotOval (ColorHndl, COLOR_GRAPH, 30, 10, 90, 80, gPoppedUpInfoPtr->rCircleColor,
              VAL_TRANSPARENT);
    PlotOval (ColorHndl, COLOR_GRAPH, 40, 45, 130, 150, gPoppedUpInfoPtr->xCircleColor,
              VAL_TRANSPARENT);
    PlotOval (ColorHndl, COLOR_GRAPH, 40, 45, 130, -150, gPoppedUpInfoPtr->xCircleColor,
              VAL_TRANSPARENT);
    InstallPopup (ColorHndl);
}


/*-----------------------------------------------------------------------------------------------*/
/* Color_Ok ():  Responds to the OK item on the popup color panel; sets new color attributes     */
/*-----------------------------------------------------------------------------------------------*/
static int Color_Ok (int panel, int control, int event, void *callbackData, int eventData1,
                     int eventData2)
{
    if (event==EVENT_COMMIT){
        GetCtrlVal(ColorHndl, COLOR_R, &gPoppedUpInfoPtr->rCircleColor);
        GetCtrlVal(ColorHndl, COLOR_X, &gPoppedUpInfoPtr->xCircleColor);
        RemovePopup (0);
        PlotCircles(gPoppedUpInfoPtr);
        RefreshGraph (gPoppedUpInfoPtr->panel, gPoppedUpInfoPtr->control);
    }
    return(0);
}


/*-----------------------------------------------------------------------------------------------*/
/* Color_Cancel ():  Responds to the Cancel item on the popup color panel; removes it and resumes*/
/*-----------------------------------------------------------------------------------------------*/
static int Color_Cancel (int panel, int control, int event, void *callbackData, int eventData1,
                         int eventData2)
{
    if (event==EVENT_COMMIT)
        RemovePopup (0);
    return(0);
}


/*-----------------------------------------------------------------------------------------------*/
/* Color_rCircle ():  Responds to the color control on the color scheme popup to show changing   */
/*                    r circle color                                                             */
/*-----------------------------------------------------------------------------------------------*/
static int Color_rCircle (int panel, int control, int event, void *callbackData, int eventData1,
                          int eventData2)
{
    int holdColor;

    if (event==EVENT_VAL_CHANGED)
    {
        GetCtrlVal(ColorHndl, COLOR_R, &holdColor);
        PlotOval (ColorHndl, COLOR_GRAPH, 0, 0, 90, 90, holdColor, VAL_TRANSPARENT);
        PlotOval (ColorHndl, COLOR_GRAPH, 30, 10, 90, 80, holdColor, VAL_TRANSPARENT);
    }
    return(0);
}


/*-----------------------------------------------------------------------------------------------*/
/* Color_xCircle ():  Responds to the color control on the color scheme popup to show changing   */
/*                    x circle color                                                             */
/*-----------------------------------------------------------------------------------------------*/
static int Color_xCircle (int panel, int control, int event, void *callbackData, int eventData1,
                          int eventData2)
{
    int holdColor;

    if (event==EVENT_VAL_CHANGED){
        GetCtrlVal(ColorHndl, COLOR_X, &holdColor);
        PlotOval (ColorHndl, COLOR_GRAPH, 40, 45, 130, 150, holdColor, VAL_TRANSPARENT);
        PlotOval (ColorHndl, COLOR_GRAPH, 40, 45, 130, -150, holdColor, VAL_TRANSPARENT);
    }
    return(0);
}
