/***************************************************************************
  qgslayertree
 ---------------------
 begin                : 22.3.2017
 copyright            : (C) 2017 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgslayertree.h"
#include "qgsmaplayerlistutils.h"

QgsLayerTree::QgsLayerTree()
  : QgsLayerTreeGroup()
{
  connect( this, &QgsLayerTree::addedChildren, this, &QgsLayerTree::nodeAddedChildren );
  connect( this, &QgsLayerTree::removedChildren, this, &QgsLayerTree::nodeRemovedChildren );
}

QgsLayerTree::QgsLayerTree( const QgsLayerTree &other )
  : QgsLayerTreeGroup( other )
  , mCustomLayerOrder( other.mCustomLayerOrder )
  , mHasCustomLayerOrder( other.mHasCustomLayerOrder )
{
  connect( this, &QgsLayerTree::addedChildren, this, &QgsLayerTree::nodeAddedChildren );
  connect( this, &QgsLayerTree::removedChildren, this, &QgsLayerTree::nodeRemovedChildren );
}

QList<QgsMapLayer *> QgsLayerTree::customLayerOrder() const
{
  return _qgis_listQPointerToRaw( mCustomLayerOrder );
}

void QgsLayerTree::setCustomLayerOrder( const QList<QgsMapLayer *> &customLayerOrder )
{
  QgsWeakMapLayerPointerList  newOrder = _qgis_listRawToQPointer( customLayerOrder );

  if ( newOrder == mCustomLayerOrder )
    return;

  mCustomLayerOrder = newOrder;
  emit customLayerOrderChanged();

  if ( mHasCustomLayerOrder )
    emit layerOrderChanged();
}

void QgsLayerTree::setCustomLayerOrder( const QStringList &customLayerOrder )
{
  QList<QgsMapLayer *> layers;

  Q_FOREACH ( const QString &layerId, customLayerOrder )
  {
    QgsLayerTreeLayer *nodeLayer = findLayer( layerId );
    if ( nodeLayer )
    {
      layers.append( nodeLayer->layer() );
    }
  }

  setCustomLayerOrder( layers );
}

QList<QgsMapLayer *> QgsLayerTree::layerOrder() const
{
  if ( mHasCustomLayerOrder )
    return customLayerOrder();
  else
  {
    QList<QgsMapLayer *> layers;

    Q_FOREACH ( QgsLayerTreeLayer *treeLayer, findLayers() )
    {
      layers.append( treeLayer->layer() );
    }

    return layers;
  }
}

bool QgsLayerTree::hasCustomLayerOrder() const
{
  return mHasCustomLayerOrder;
}

void QgsLayerTree::setHasCustomLayerOrder( bool hasCustomLayerOrder )
{
  if ( hasCustomLayerOrder == mHasCustomLayerOrder )
    return;

  mHasCustomLayerOrder = hasCustomLayerOrder;

  emit hasCustomLayerOrderChanged( hasCustomLayerOrder );
  emit layerOrderChanged();
}

QgsLayerTree *QgsLayerTree::readXml( QDomElement &element )
{
  QgsLayerTree *tree = new QgsLayerTree();

  tree->readCommonXml( element );

  tree->readChildrenFromXml( element );

  return tree;
}

void QgsLayerTree::writeXml( QDomElement &parentElement )
{
  QDomDocument doc = parentElement.ownerDocument();
  QDomElement elem = doc.createElement( QStringLiteral( "layer-tree-group" ) );

  writeCommonXml( elem );

  Q_FOREACH ( QgsLayerTreeNode *node, mChildren )
    node->writeXml( elem );

  QDomElement customOrderElem = doc.createElement( QStringLiteral( "custom-order" ) );
  customOrderElem.setAttribute( QStringLiteral( "enabled" ), mHasCustomLayerOrder ? 1 : 0 );
  elem.appendChild( customOrderElem );

  Q_FOREACH ( QgsMapLayer *layer, mCustomLayerOrder )
  {
    QDomElement layerElem = doc.createElement( QStringLiteral( "item" ) );
    layerElem.appendChild( doc.createTextNode( layer->id() ) );
    customOrderElem.appendChild( layerElem );
  }

  elem.appendChild( customOrderElem );

  parentElement.appendChild( elem );
}

QgsLayerTree *QgsLayerTree::clone() const
{
  return new QgsLayerTree( *this );
}

void QgsLayerTree::clear()
{
  removeAllChildren();
  setHasCustomLayerOrder( false );
  setCustomLayerOrder( QStringList() );
}

void QgsLayerTree::nodeAddedChildren( QgsLayerTreeNode *node, int indexFrom, int indexTo )
{
  Q_ASSERT( node );

  // collect layer IDs that have been added in order to put them into custom layer order
  QList<QgsMapLayer *> layers;

  QList<QgsLayerTreeNode *> children = node->children();
  for ( int i = indexFrom; i <= indexTo; ++i )
  {
    QgsLayerTreeNode *child = children.at( i );
    if ( QgsLayerTree::isLayer( child ) )
    {
      layers << QgsLayerTree::toLayer( child )->layer();
    }
    else if ( QgsLayerTree::isGroup( child ) )
    {
      Q_FOREACH ( QgsLayerTreeLayer *nodeL, QgsLayerTree::toGroup( child )->findLayers() )
        layers << nodeL->layer();
    }
  }

  Q_FOREACH ( QgsMapLayer *layer, layers )
  {
    if ( !mCustomLayerOrder.contains( layer ) && layer )
      mCustomLayerOrder.append( layer );
  }

  emit customLayerOrderChanged();
  emit layerOrderChanged();
}

void QgsLayerTree::nodeRemovedChildren()
{
  QList<QgsMapLayer *> layers = customLayerOrder();
  auto layer = layers.begin();

  while ( layer != layers.end() )
  {
    if ( !findLayer( *layer ) )
      layer = layers.erase( layer );
    else
      ++layer;
  }

  setCustomLayerOrder( layers );
  emit layerOrderChanged();
}

void QgsLayerTree::addMissingLayers()
{
  bool changed = false;

  QList<QgsLayerTreeLayer *> allLayers = findLayers();

  Q_FOREACH ( QgsLayerTreeLayer *layer, allLayers )
  {
    if ( !mCustomLayerOrder.contains( layer->layer() ) && layer->layer() )
    {
      mCustomLayerOrder.append( layer->layer() );
      changed = true;
    }
  }

  if ( changed )
  {
    emit customLayerOrderChanged();
    if ( mHasCustomLayerOrder )
      emit layerOrderChanged();
  }
}

void QgsLayerTree::readLayerOrderFromXml( const QDomElement &elem )
{
  QStringList order;

  QDomElement customOrderElem = elem.firstChildElement( QStringLiteral( "custom-order" ) );
  if ( !customOrderElem.isNull() )
  {
    setHasCustomLayerOrder( customOrderElem.attribute( QStringLiteral( "enabled" ) ).toInt() );

    QDomElement itemElem = customOrderElem.firstChildElement( QStringLiteral( "item" ) );
    while ( !itemElem.isNull() )
    {
      order.append( itemElem.text() );
      itemElem = itemElem.nextSiblingElement( QStringLiteral( "item" ) );
    }
  }

  setCustomLayerOrder( order );
  addMissingLayers();
}
