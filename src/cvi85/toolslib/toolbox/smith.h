//-------------------------------------------------------------------------------------------------
//
//      smith.h
//
//      Instrument driver header file
//
//      Associated files: smith.c
//                        smith.fp
//
//      Purpose:          The functions in this driver allow the user to convert an existing CVI
//                        Graph control into a specialized Smith Chart, plot impedances on that
//                        chart, and convert between reflection coefficients and impedances.
//
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Define complex datatype
//-------------------------------------------------------------------------------------------------
typedef struct tag_Complex{
    double Real;
    double Im;
} Complex;


//-------------------------------------------------------------------------------------------------
// Define error codes
//-------------------------------------------------------------------------------------------------
#define kNoError                 0
#define kErrorToManyControls    -1
#define kErrorNotGraph          -2
#define kErrorNullImpedance     -3
#define kErrorNullCoeff         -4
#define kErrorUnknownChart      -5
#define kErrorUnknownPlot       -6
#define kErrorMemory            -7
#define kErrorTooManyCircles    -8


//-------------------------------------------------------------------------------------------------
// Prototype 'exported' driver functions
//-------------------------------------------------------------------------------------------------
int SMITH_PlotConstantMagArc (int panel, int control, Complex* point, double wavelengths,
                              int direction, int color);
int SMITH_DeletePlot (int panel, int control, int plotHandle);
int SMITH_ConvertFromGraph(int panel, int control, int useDefaultCircles, int numRCircles,
                           double* rCircles, int numXCircles, double* xCircles);
int SMITH_ImpedanceToCoeff(Complex *Zin, Complex *Ref);
int SMITH_CoeffToImpedance(Complex *Ref, Complex *Zout);
int SMITH_PlotImpedancePoint(int panel, int control, Complex *Point, int PointStyle, int Color);
int SMITH_PlotImpedanceWaveform (int panel, int control, Complex *Z_Array, int Number_of_Points,
                                 int Plot_Style, int Point_Style, int Line_Style, int Color);
int SMITH_DiscardChart (int panel, int control);
