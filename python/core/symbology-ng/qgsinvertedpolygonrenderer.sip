/************************************************************************
 * This file has been generated automatically from                      *
 *                                                                      *
 * src/core/symbology-ng/qgsinvertedpolygonrenderer.h                   *
 *                                                                      *
 * Do not edit manually ! Edit header and run scripts/sipify.pl again   *
 ************************************************************************/


class QgsInvertedPolygonRenderer : QgsFeatureRenderer
{
%Docstring
 QgsInvertedPolygonRenderer is a polygon-only feature renderer used to
 display features inverted, where the exterior is turned to an interior
 and where the exterior theoretically spans the entire plane, allowing
 to mask the surroundings of some features.

 It is designed on top of another feature renderer, which is called "embedded"
 Most of the methods are then only proxies to the embedded renderer.

 Features are collected to form one "inverted" polygon
 during renderFeature() and rendered on stopRender().

.. versionadded:: 2.4
%End

%TypeHeaderCode
#include "qgsinvertedpolygonrenderer.h"
%End
  public:

    QgsInvertedPolygonRenderer( QgsFeatureRenderer *embeddedRenderer /Transfer/ = 0 );
%Docstring
 Constructor
 \param embeddedRenderer optional embeddedRenderer. If null, a default one will be assigned.
 Ownership will be transferred.
%End


    virtual QgsInvertedPolygonRenderer *clone() const /Factory/;
%Docstring
Direct copies are forbidden. Use clone() instead.
 :rtype: QgsInvertedPolygonRenderer
%End
    virtual void startRender( QgsRenderContext &context, const QgsFields &fields );

    virtual bool renderFeature( QgsFeature &feature, QgsRenderContext &context, int layer = -1, bool selected = false, bool drawVertexMarker = false );
%Docstring
 Renders a given feature.
 This will here collect features. The actual rendering will be postponed to stopRender()
 \param feature the feature to render
 \param context the rendering context
 \param layer the symbol layer to render, if that makes sense
 \param selected whether this feature has been selected (this will add decorations)
 \param drawVertexMarker whether this feature has vertex markers (in edit mode usually)
 :return: true if the rendering was ok
 :rtype: bool
%End

    virtual void stopRender( QgsRenderContext &context );
%Docstring
 The actual rendering will take place here.
 Features collected during renderFeature() are rendered using the embedded feature renderer
%End

    virtual QString dump() const;

    virtual QSet<QString> usedAttributes( const QgsRenderContext &context ) const;
%Docstring
Proxy that will call this method on the embedded renderer.
 :rtype: set of str
%End
    virtual QgsFeatureRenderer::Capabilities capabilities();
%Docstring
Proxy that will call this method on the embedded renderer.
 :rtype: QgsFeatureRenderer.Capabilities
%End

    virtual QgsSymbolList symbols( QgsRenderContext &context );
%Docstring
 Proxy that will call this method on the embedded renderer.
 :rtype: QgsSymbolList
%End

    virtual QgsSymbol *symbolForFeature( QgsFeature &feature, QgsRenderContext &context );
%Docstring
 Proxy that will call this method on the embedded renderer.
 :rtype: QgsSymbol
%End

    virtual QgsSymbol *originalSymbolForFeature( QgsFeature &feat, QgsRenderContext &context );
%Docstring
 Proxy that will call this method on the embedded renderer.
 :rtype: QgsSymbol
%End

    virtual QgsSymbolList symbolsForFeature( QgsFeature &feat, QgsRenderContext &context );
%Docstring
 Proxy that will call this method on the embedded renderer.
 :rtype: QgsSymbolList
%End

    virtual QgsSymbolList originalSymbolsForFeature( QgsFeature &feat, QgsRenderContext &context );
%Docstring
 Proxy that will call this method on the embedded renderer.
 :rtype: QgsSymbolList
%End

    virtual QgsLegendSymbolList legendSymbolItems() const;
%Docstring
 Proxy that will call this method on the embedded renderer.
 :rtype: QgsLegendSymbolList
%End

    virtual bool willRenderFeature( QgsFeature &feat, QgsRenderContext &context );
%Docstring
 Proxy that will call this method on the embedded renderer.
 :rtype: bool
%End

    static QgsFeatureRenderer *create( QDomElement &element, const QgsReadWriteContext &context ) /Factory/;
%Docstring
Creates a renderer out of an XML, for loading
 :rtype: QgsFeatureRenderer
%End

    virtual QDomElement save( QDomDocument &doc, const QgsReadWriteContext &context );

    virtual void setEmbeddedRenderer( QgsFeatureRenderer *subRenderer /Transfer/ );

    virtual const QgsFeatureRenderer *embeddedRenderer() const;


    virtual void setLegendSymbolItem( const QString &key, QgsSymbol *symbol );

    virtual bool legendSymbolItemsCheckable() const;
    virtual bool legendSymbolItemChecked( const QString &key );
    virtual void checkLegendSymbolItem( const QString &key, bool state = true );

    bool preprocessingEnabled() const;
%Docstring
:return: true if the geometries are to be preprocessed (merged with an union) before rendering.
 :rtype: bool
%End

    void setPreprocessingEnabled( bool enabled );
%Docstring
 \param enabled enables or disables the preprocessing.
 When enabled, geometries will be merged with an union before being rendered.
 It allows fixing some rendering artifacts (when rendering overlapping polygons for instance).
 This will involve some CPU-demanding computations and is thus disabled by default.
%End

    static QgsInvertedPolygonRenderer *convertFromRenderer( const QgsFeatureRenderer *renderer ) /Factory/;
%Docstring
 Creates a QgsInvertedPolygonRenderer by a conversion from an existing renderer.
.. versionadded:: 2.5
 :return: a new renderer if the conversion was possible, otherwise 0.
 :rtype: QgsInvertedPolygonRenderer
%End

};


/************************************************************************
 * This file has been generated automatically from                      *
 *                                                                      *
 * src/core/symbology-ng/qgsinvertedpolygonrenderer.h                   *
 *                                                                      *
 * Do not edit manually ! Edit header and run scripts/sipify.pl again   *
 ************************************************************************/
