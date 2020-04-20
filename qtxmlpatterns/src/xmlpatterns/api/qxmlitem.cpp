/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtXmlPatterns module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QVector>

#include "qabstractxmlnodemodel_p.h"
#include "qabstractxmlreceiver.h"
#include "qcommonvalues_p.h"
#include "qemptyiterator_p.h"
#include "qitemmappingiterator_p.h"
#include "qitem_p.h"
#include "qnamespaceresolver_p.h"
#include "qsequencemappingiterator_p.h"
#include "qsingletoniterator_p.h"

#include "qxmlitem.h"
#include "qxmlitem_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

/**
 * @file
 * @short Contains the implementation of QXmlItem.
 */

using namespace QPatternist;

/*!
  \class QXmlItem
  \reentrant
  \since 4.4
  \brief The QXmlItem class contains either an XML node or an atomic value.
  \ingroup xml-tools
  \inmodule QtXmlPatterns

  In XQuery, all expressions evaluate to a sequence of items, where
  each item is either an XML node or an atomic value. The query in the
  following snippet evaluates to sequence of five items.

  \quotefile patternist/items.xq

  The five items are: An element, an atomic value (binary data encoded
  in base64), a date, a float, and an attribute.

  QXmlItem is the class that represents these XQuery items in the
  Qt XML Patterns API. A non-null instance of QXmlItem is either a node
  or an atomic value. Calling isNode() or isAtomicValue() tells you
  which it is. Atomic values are represented elsewhere in the Qt API
  as instances of QVariant, and an instance of QXmlItem that
  represents an atomic value can be converted to a QVariant by calling
  toAtomicValue(). A QXmlItem that wraps a node is represented
  elsewhere as an instance of QXmlNodeModelIndex. A node QXmlItem can
  be converted to a QXmlNodeModelIndex by calling toNodeModelIndex().

  A default constructed QXmlItem instance is neither a node nor an
  atomic value. It is considered null, in which case isNull() returns
  true.

  An instance of QXmlItem will be left dangling if the
  \l{QAbstractXmlNodeModel} {XML node model} it
  refers to is deleted, if it is a QXmlNodeModelIndex.
 */

/*!
  \typedef QXmlItem::Iterator
  A QAbstractXmlForwardIterator over QXmlItem.
 */

/*!
  \fn QXmlItem::QXmlItem()
  Constructs a null QXmlItem that is neither a node nor an atomic
  value. isNull() returns true for a default constructed instance.
 */
QXmlItem::QXmlItem()
    : d_ptr(new QXmlItemPrivate())
{}

/*!
  The copy constructor constructs a copy of \a other.
 */
QXmlItem::QXmlItem(const QXmlItem &other)
    : d_ptr(other.d_ptr)
{}

/*!
  Destructs an value of QXmlItem.
 */
QXmlItem::~QXmlItem()
{}

QXmlItem &QXmlItem::operator=(const QXmlItem &other)
{
    d_ptr = other.d_ptr;
    return *this;
}


/*!
  Constructs an atomic value QXmlItem with \a atomicValue.

  \sa isAtomicValue()
 */
QXmlItem::QXmlItem(const QVariant &atomicValue)
    : d_ptr(new QXmlItemPrivate(atomicValue))
{
}

/*!
  Constructs a node QXmlItem that is a copy of \a node.

  \sa isNode()
 */
QXmlItem::QXmlItem(const QXmlNodeModelIndex &node)
    : d_ptr(new QXmlItemPrivate(node))
{
}

/*!
  Returns true if this item is a Node. Returns false if it
  is an atomic value or null.

  \sa isNull(), isAtomicValue()
 */
bool QXmlItem::isNode() const
{
    return d_ptr->isNode();
}

/*!
  Returns true if this item is an atomic value. Returns false
  if it is a node or null.

  \sa isNull(), isNode()
 */
bool QXmlItem::isAtomicValue() const
{
    return d_ptr->isAtomicValue();
}

/*!
  Returns true if this QXmlItem is neither a node nor an
  atomic value. Default constructed instances of QXmlItem
  are null.
 */
bool QXmlItem::isNull() const
{
    return d_ptr->isNull();
}

/*!
  If this QXmlItem represents an atomic value, it is converted
  to an appropriate QVariant and returned. If this QXmlItem is
  not an atomic value, the return value is a default constructed
  QVariant. You can call isAtomicValue() to test whether the
  item is an atomic value.

 \sa isAtomicValue()
 */
QVariant QXmlItem::toAtomicValue() const
{
    return d_ptr->toAtomicValue();
}

/*!
  If this QXmlItem represents a node, it returns the item as a
  QXmlNodeModelIndex. If this QXmlItem is not a node, the return
  value is undefined. You can call isNode() to test whether the
  item is a node.

 \sa isNode()
 */
QXmlNodeModelIndex QXmlItem::toNodeModelIndex() const
{
    return d_ptr->toNodeModelIndex();
}

const QPatternist::Item &QXmlItem::toItem() const
{
    return d_ptr->toItem();
}

QXmlItem::QXmlItem(const QPatternist::Item &item)
    :d_ptr(new QXmlItemPrivate(item))
{}

QT_END_NAMESPACE
