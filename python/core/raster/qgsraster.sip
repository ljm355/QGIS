/************************************************************************
 * This file has been generated automatically from                      *
 *                                                                      *
 * src/core/raster/qgsraster.h                                          *
 *                                                                      *
 * Do not edit manually ! Edit header and run scripts/sipify.pl again   *
 ************************************************************************/





class QgsRaster
{
%Docstring
 Raster namespace.
%End

%TypeHeaderCode
#include "qgsraster.h"
%End
  public:
    enum ColorInterpretation
    {
      UndefinedColorInterpretation,
      GrayIndex,
      PaletteIndex,
      RedBand,
      GreenBand,
      BlueBand,
      AlphaBand,
      HueBand,
      SaturationBand,
      LightnessBand,
      CyanBand,
      MagentaBand,
      YellowBand,
      BlackBand,
      YCbCr_YBand,
      YCbCr_CbBand,
      YCbCr_CrBand,
      ContinuousPalette
    };

    enum IdentifyFormat
    {
      IdentifyFormatUndefined,
      IdentifyFormatValue,
      IdentifyFormatText,
      IdentifyFormatHtml,
      IdentifyFormatFeature,
    };

    enum RasterProgressType
    {
      ProgressHistogram,
      ProgressPyramids,
      ProgressStatistics
    };

    enum RasterBuildPyramids
    {
      PyramidsFlagNo,
      PyramidsFlagYes,
      PyramidsCopyExisting
    };

    enum RasterPyramidsFormat
    {
      PyramidsGTiff,
      PyramidsInternal,
      PyramidsErdas
    };

    enum DrawingStyle
    {
      UndefinedDrawingStyle,
      SingleBandGray,
      SingleBandPseudoColor,
      PalettedColor,
      PalettedSingleBandGray,
      PalettedSingleBandPseudoColor,
      PalettedMultiBandColor,
      MultiBandSingleBandGray,
      MultiBandSingleBandPseudoColor,
      MultiBandColor,
      SingleBandColorDataStyle
    };


    static double representableValue( double value, Qgis::DataType dataType );
%Docstring
 Get value representable by given data type.
 Supported are numerical types Byte, UInt16, Int16, UInt32, Int32, Float32, Float64.
 This is done through C casting, so you have to be sure that the provided value is
 representable in the output data type. This can be checked with isRepresentableValue().
 \param value
 \param dataType
.. versionadded:: 2.1
 :rtype: float
%End
};



/************************************************************************
 * This file has been generated automatically from                      *
 *                                                                      *
 * src/core/raster/qgsraster.h                                          *
 *                                                                      *
 * Do not edit manually ! Edit header and run scripts/sipify.pl again   *
 ************************************************************************/
