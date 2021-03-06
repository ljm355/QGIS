# -*- coding: utf-8 -*-

"""
***************************************************************************
    Aspect.py
    ---------------------
    Date                 : October 2016
    Copyright            : (C) 2016 by Alexander Bruy
    Email                : alexander dot bruy at gmail dot com
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************
"""

__author__ = 'Alexander Bruy'
__date__ = 'October 2016'
__copyright__ = '(C) 2016, Alexander Bruy'

# This will get replaced with a git SHA1 when you do a git archive

__revision__ = '$Format:%H$'

import os

from qgis.PyQt.QtGui import QIcon

from qgis.analysis import QgsAspectFilter
from qgis.core import (QgsProcessingParameterRasterLayer,
                       QgsProcessingParameterNumber,
                       QgsProcessingParameterRasterOutput,
                       QgsProcessingOutputRasterLayer,
                       QgsFeatureSink)
from processing.algs.qgis.QgisAlgorithm import QgisAlgorithm
from processing.core.parameters import ParameterRaster
from processing.core.parameters import ParameterNumber
from processing.core.outputs import OutputRaster
from processing.tools import raster
from processing.tools.dataobjects import exportRasterLayer

pluginPath = os.path.split(os.path.split(os.path.dirname(__file__))[0])[0]


class Aspect(QgisAlgorithm):

    INPUT_LAYER = 'INPUT_LAYER'
    Z_FACTOR = 'Z_FACTOR'
    OUTPUT_LAYER = 'OUTPUT_LAYER'

    def icon(self):
        return QIcon(os.path.join(pluginPath, 'images', 'dem.png'))

    def group(self):
        return self.tr('Raster terrain analysis')

    def __init__(self):
        super().__init__()

        self.addParameter(QgsProcessingParameterRasterLayer(self.INPUT_LAYER,
                                                            self.tr('Elevation layer')))
        self.addParameter(QgsProcessingParameterNumber(self.Z_FACTOR,
                                                       self.tr('Z factor'), QgsProcessingParameterNumber.Double,
                                                       1, False, 1, 999999.99))
        self.addParameter(QgsProcessingParameterRasterOutput(self.OUTPUT_LAYER, self.tr('Aspect')))
        self.addOutput(QgsProcessingOutputRasterLayer(self.OUTPUT_LAYER, self.tr('Aspect')))

    def name(self):
        return 'aspect'

    def displayName(self):
        return self.tr('Aspect')

    def processAlgorithm(self, parameters, context, feedback):
        inputFile = exportRasterLayer(self.parameterAsRasterLayer(parameters, self.INPUT_LAYER, context))
        zFactor = self.parameterAsDouble(parameters, self.Z_FACTOR, context)

        outputFile = self.parameterAsRasterOutputLayer(parameters, self.OUTPUT_LAYER, context)

        outputFormat = raster.formatShortNameFromFileName(outputFile)

        aspect = QgsAspectFilter(inputFile, outputFile, outputFormat)
        aspect.setZFactor(zFactor)
        aspect.processRaster(feedback)

        return {self.OUTPUT_LAYER: outputFile}
