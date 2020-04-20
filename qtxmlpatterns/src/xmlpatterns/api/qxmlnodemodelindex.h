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

#ifndef QXMLNODEMODELINDEX_H
#define QXMLNODEMODELINDEX_H

#include <QtXmlPatterns/QXmlName>
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

class Q_XMLPATTERNS_EXPORT QXmlNodeModelIndex
{
    enum Constants
    {
        ForwardAxis         = 8192,
        ReverseAxis         = 16384
    };

public:
    QXmlNodeModelIndex()
    {
        reset();
    }

    inline QXmlNodeModelIndex(const QXmlNodeModelIndex &other)
    {
        m_data = other.m_data;
        m_additionalData = other.m_additionalData;
        m_model = other.m_model;
    }

    QXmlNodeModelIndex &operator=(const QXmlNodeModelIndex &) = default;

    bool operator==(const QXmlNodeModelIndex &other) const;
    bool operator!=(const QXmlNodeModelIndex &other) const;

    typedef QAbstractXmlForwardIterator<QXmlNodeModelIndex> Iterator;
    typedef QList<QXmlNodeModelIndex> List;

    enum NodeKind
    {
        Attribute               = 1,
        Comment                 = 2,
        Document                = 4,
        Element                 = 8,
        Namespace               = 16,
        ProcessingInstruction   = 32,
        Text                    = 64
    };

    enum DocumentOrder
    {
        Precedes = -1,
        Is       = 0,
        Follows  = 1
    };

    enum Axis
    {
        AxisChild               = 1 | ForwardAxis,
        AxisDescendant          = 2 | ForwardAxis,
        AxisAttribute           = 4 | ForwardAxis,
        AxisSelf                = 8 | ForwardAxis,
        AxisDescendantOrSelf    = 16 | ForwardAxis,
        AxisFollowingSibling    = 32 | ForwardAxis,
        AxisNamespace           = 64 | ForwardAxis,
        AxisFollowing           = 128 | ReverseAxis,
        AxisParent              = 256 | ReverseAxis,
        AxisAncestor            = 512 | ReverseAxis,
        AxisPrecedingSibling    = 1024 | ReverseAxis,
        AxisPreceding           = 2048 | ReverseAxis,
        AxisAncestorOrSelf      = 4096 | ReverseAxis,
        /* Note that we cannot clash with the values of ForwardAxis and
         * ReverseAxis. */
        AxisChildOrTop          = 32768 | ForwardAxis,
        AxisAttributeOrTop      = 65536 | ForwardAxis
    };

    inline qint64 data() const
    {
        return m_data;
    }

    inline void *internalPointer() const
    {
        Q_ASSERT_X(sizeof(m_data) >= sizeof(void*), Q_FUNC_INFO, "If this doesn't hold, something is wrong.");
        return reinterpret_cast<void*>(qptrdiff(m_data));;
    }

    inline const QAbstractXmlNodeModel *model() const
    {
        return m_model;
    }

    inline qint64 additionalData() const
    {
        return m_additionalData;
    }

    inline bool isNull() const
    {
        return !m_model;
    }

    /* The members below are internal, not part of the public API, and
     * unsupported. Using them leads to undefined behavior. */

    inline QXmlName name() const;
    inline QXmlNodeModelIndex root() const;
    inline QExplicitlySharedDataPointer<QAbstractXmlForwardIterator<QXmlNodeModelIndex> > iterate(const Axis axis) const;
    inline QExplicitlySharedDataPointer<QAbstractXmlForwardIterator<QPatternist::Item> > sequencedTypedValue() const;
    inline QUrl documentUri() const;
    inline QUrl baseUri() const;
    inline NodeKind kind() const;
    inline bool isDeepEqual(const QXmlNodeModelIndex &other) const;
    inline DocumentOrder compareOrder(const QXmlNodeModelIndex &other) const;
    inline void sendNamespaces(QAbstractXmlReceiver *const receiver) const;
    inline QVector<QXmlName> namespaceBindings() const;
    inline QXmlName::NamespaceCode namespaceForPrefix(const QXmlName::PrefixCode prefix) const;
    inline QString stringValue() const;
    inline QPatternist::ItemTypePtr type() const;
    inline bool is(const QXmlNodeModelIndex &other) const;

    inline void reset()
    {
        m_data = 0;
        m_additionalData = 0;
        m_model = 0;
    }

private:
    inline QXmlNodeModelIndex(const qint64 data, const QAbstractXmlNodeModel *const nm)
        : m_data(data), m_additionalData(0), m_model(nm)
    {}

    inline QXmlNodeModelIndex(const qint64 data,
                              const QAbstractXmlNodeModel *const nm,
                              const qint64 addData)
        : m_data(data), m_additionalData(addData), m_model(nm)
    {}

    static inline QXmlNodeModelIndex create(const qint64 data,
                                            const QAbstractXmlNodeModel *const nm)
    {
        return QXmlNodeModelIndex(data, nm);
    }

    static inline QXmlNodeModelIndex create(const qint64 data,
                                            const QAbstractXmlNodeModel *const nm,
                                            const qint64 additionalData)
    {
        return QXmlNodeModelIndex(data, nm, additionalData);
    }

    friend class QAbstractXmlNodeModel;
    friend class QPatternist::Item;
    friend class QXmlItem;

    qint64 m_data;
    qint64 m_additionalData;
    const QAbstractXmlNodeModel *m_model;
};

Q_XMLPATTERNS_EXPORT uint qHash(const QXmlNodeModelIndex &index);

inline bool QXmlNodeModelIndex::operator==(const QXmlNodeModelIndex &other) const
{
    return m_additionalData == other.m_additionalData
           && m_data == other.m_data
           && m_model == other.m_model;
}

inline bool QXmlNodeModelIndex::operator!=(const QXmlNodeModelIndex &other) const
{
    return !(*this == other);
}


inline bool qIsForwardIteratorEnd(const QXmlNodeModelIndex &item)
{
    return item.isNull();
}

Q_DECLARE_TYPEINFO(QXmlNodeModelIndex, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

#endif
