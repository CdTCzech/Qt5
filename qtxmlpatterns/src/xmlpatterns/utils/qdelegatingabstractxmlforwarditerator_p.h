/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtXmlPatterns module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef Patternist_DelegatingAbstractXmlForwardIterator_H
#define Patternist_DelegatingAbstractXmlForwardIterator_H

#include <QHash>

#include "qabstractxmlforwarditerator_p.h"

QT_BEGIN_NAMESPACE

namespace QPatternist
{

    template <typename T>
    class DelegatingAbstractXmlForwardIterator : public QAbstractXmlForwardIterator<T>
    {
    public:
        DelegatingAbstractXmlForwardIterator(QAbstractXmlForwardIterator<T> *prevIterator);

        virtual T next();
        virtual T current() const;
    
        virtual qint64 position() const;
  
        virtual typename QAbstractXmlForwardIterator<T>::Ptr toReversed();
        virtual QList<T> toList();
        virtual typename QAbstractXmlForwardIterator<T>::Ptr copy() const;
        virtual T last();
        virtual bool isEmpty();
        virtual qint64 count();
        virtual qint64 sizeHint() const;

    private:
        QAbstractXmlForwardIterator<T> *m_prevIterator;
    };

template <typename T>
DelegatingAbstractXmlForwardIterator<T>::DelegatingAbstractXmlForwardIterator(QAbstractXmlForwardIterator<T> *prevIterator)
  : m_prevIterator(prevIterator)
{}

template <typename T>
T DelegatingAbstractXmlForwardIterator<T>::next()
{
    return m_prevIterator->next();
}

template <typename T>
T DelegatingAbstractXmlForwardIterator<T>::current() const
{
    return m_prevIterator->current();
}

template <typename T>
qint64 DelegatingAbstractXmlForwardIterator<T>::position() const
{
    return m_prevIterator->position();
}

template <typename T>
typename QAbstractXmlForwardIterator<T>::Ptr DelegatingAbstractXmlForwardIterator<T>::toReversed()
{
    return m_prevIterator->toReversed();
}

template <typename T>
QList<T> DelegatingAbstractXmlForwardIterator<T>::toList()
{
    return m_prevIterator->toList();
}

template <typename T>
typename QAbstractXmlForwardIterator<T>::Ptr DelegatingAbstractXmlForwardIterator<T>::copy() const
{
    return m_prevIterator->copy();
}

template <typename T>
T DelegatingAbstractXmlForwardIterator<T>::last()
{
    return m_prevIterator->last();
}

template <typename T>
bool DelegatingAbstractXmlForwardIterator<T>::isEmpty()
{
    return m_prevIterator->isEmpty();
}

template <typename T>
qint64 DelegatingAbstractXmlForwardIterator<T>::count()
{
    return m_prevIterator->count();
}

template <typename T>
qint64 DelegatingAbstractXmlForwardIterator<T>::sizeHint() const
{
    return m_prevIterator->sizeHint();
}

}

QT_END_NAMESPACE

#endif
