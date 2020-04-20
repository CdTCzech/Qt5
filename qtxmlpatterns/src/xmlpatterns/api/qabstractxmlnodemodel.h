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

#ifndef QABSTRACTXMLNODEMODEL_H
#define QABSTRACTXMLNODEMODEL_H

#include <QtXmlPatterns/QXmlName>
#include <QtXmlPatterns/QXmlNodeModelIndex>
#include "qabstractxmlforwarditerator_p.h"

#include <QtCore/QSharedData>
#include <QtCore/QScopedPointer>

QT_BEGIN_NAMESPACE


/* This file contains the classes QXmlNodeModelIndex, QAbstractXmlNodeModel,
 * QXmlItem and QPatternist::NodeIndexStorage. */

class QAbstractXmlNodeModel;
class QAbstractXmlNodeModelPrivate;
class QAbstractXmlReceiver;
class QSourceLocation;
class QUrl;
class QXmlName;
class QXmlNodeModelIndex;
template<typename T> class QAbstractXmlForwardIterator;
template<typename T> class QVector;

/* The members in the namespace QPatternist are internal, not part of the public API, and
 * unsupported. Using them leads to undefined behavior. */
namespace QPatternist
{
    class DynamicContext;
    class Item;
    class ItemType;
    class XsdValidatedXmlNodeModel;
    template<typename TResult, typename TSource, typename TMapper, typename TContext> class ItemMappingIterator;
    template<typename TResult, typename TSource, typename TMapper, typename TContext> class SequenceMappingIterator;
    typedef QExplicitlySharedDataPointer<ItemType> ItemTypePtr;
    typedef QExplicitlySharedDataPointer<QAbstractXmlForwardIterator<Item> > ItemIteratorPtr;
    typedef QVector<QXmlName> QXmlNameVector;
}

class Q_XMLPATTERNS_EXPORT QAbstractXmlNodeModel : public QSharedData
{
public:
    enum SimpleAxis
    {
        Parent,
        FirstChild,
        PreviousSibling,
        NextSibling
    };

    typedef QExplicitlySharedDataPointer<QAbstractXmlNodeModel> Ptr;
    typedef QExplicitlySharedDataPointer<const QAbstractXmlNodeModel> ConstPtr;

    typedef QList<Ptr> List;

    QAbstractXmlNodeModel();
    virtual ~QAbstractXmlNodeModel();

    virtual QUrl baseUri(const QXmlNodeModelIndex &ni) const = 0;
    virtual QUrl documentUri(const QXmlNodeModelIndex &ni) const = 0;
    virtual QXmlNodeModelIndex::NodeKind kind(const QXmlNodeModelIndex &ni) const = 0;
    virtual QXmlNodeModelIndex::DocumentOrder compareOrder(const QXmlNodeModelIndex &ni1,
                                                           const QXmlNodeModelIndex &ni2) const = 0;
    virtual QXmlNodeModelIndex root(const QXmlNodeModelIndex &n) const = 0;
    virtual QXmlName name(const QXmlNodeModelIndex &ni) const = 0;
    virtual QString stringValue(const QXmlNodeModelIndex &n) const = 0;
    virtual QVariant typedValue(const QXmlNodeModelIndex &n) const = 0;

    /* The members below are internal, not part of the public API, and
     * unsupported. Using them leads to undefined behavior. */
    virtual QXmlNodeModelIndex::Iterator::Ptr iterate(const QXmlNodeModelIndex &ni, QXmlNodeModelIndex::Axis axis) const;
    virtual QPatternist::ItemIteratorPtr sequencedTypedValue(const QXmlNodeModelIndex &ni) const;
    virtual QPatternist::ItemTypePtr type(const QXmlNodeModelIndex &ni) const;
    virtual QXmlName::NamespaceCode namespaceForPrefix(const QXmlNodeModelIndex &ni,
                                                       const QXmlName::PrefixCode prefix) const;
    virtual bool isDeepEqual(const QXmlNodeModelIndex &ni1,
                             const QXmlNodeModelIndex &ni2) const;
    virtual void sendNamespaces(const QXmlNodeModelIndex &n,
                                QAbstractXmlReceiver *const receiver) const;
    virtual QVector<QXmlName> namespaceBindings(const QXmlNodeModelIndex &n) const = 0;


    virtual QXmlNodeModelIndex elementById(const QString &NCName) const = 0;
    virtual QVector<QXmlNodeModelIndex> nodesByIdref(const QString &NCName) const = 0;

    enum NodeCopySetting
    {
        InheritNamespaces   = 0x1,
        PreserveNamespaces  = 0x2
    };

    typedef QFlags<NodeCopySetting> NodeCopySettings;
    virtual void copyNodeTo(const QXmlNodeModelIndex &node,
                            QAbstractXmlReceiver *const receiver,
                            const NodeCopySettings &) const;

    QSourceLocation sourceLocation(const QXmlNodeModelIndex &index) const;

protected:

    virtual QXmlNodeModelIndex nextFromSimpleAxis(SimpleAxis axis, const QXmlNodeModelIndex &origin) const = 0;
    virtual QVector<QXmlNodeModelIndex> attributes(const QXmlNodeModelIndex &element) const = 0;

    QAbstractXmlNodeModel(QAbstractXmlNodeModelPrivate *d);

    inline QXmlNodeModelIndex createIndex(qint64 data) const
    {
        return QXmlNodeModelIndex::create(data, this);
    }

    inline QXmlNodeModelIndex createIndex(void * pointer,
                                          qint64 additionalData = 0) const
    {
        return QXmlNodeModelIndex::create(qptrdiff(pointer), this, additionalData);
    }

    inline QXmlNodeModelIndex createIndex(qint64 data,
                                          qint64 additionalData) const
    {
        return QXmlNodeModelIndex::create(data, this, additionalData);
    }

    QScopedPointer<QAbstractXmlNodeModelPrivate> d_ptr;
private:
    friend class QPatternist::ItemMappingIterator<QXmlNodeModelIndex, QXmlNodeModelIndex, const QAbstractXmlNodeModel *, QExplicitlySharedDataPointer<QPatternist::DynamicContext> >;
    friend class QPatternist::SequenceMappingIterator<QXmlNodeModelIndex, QXmlNodeModelIndex, const QAbstractXmlNodeModel *, QExplicitlySharedDataPointer<QPatternist::DynamicContext> >;
    friend class QPatternist::XsdValidatedXmlNodeModel;

    inline QExplicitlySharedDataPointer<QAbstractXmlForwardIterator<QXmlNodeModelIndex> > mapToSequence(const QXmlNodeModelIndex &ni,
                                                                           const QExplicitlySharedDataPointer<QPatternist::DynamicContext> &) const;

    static inline bool isIgnorableInDeepEqual(const QXmlNodeModelIndex &n);
    Q_DISABLE_COPY(QAbstractXmlNodeModel)
};

QT_END_NAMESPACE

#endif
