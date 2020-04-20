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

#include "qdelegatingabstractxmlreceiver_p.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

DelegatingAbstractXmlReceiver::DelegatingAbstractXmlReceiver(QAbstractXmlReceiver *prevReceiver)
  : m_prevReceiver(prevReceiver)
{}

void DelegatingAbstractXmlReceiver::startElement(const QXmlName &name)
{
    m_prevReceiver->startElement(name);
}


void DelegatingAbstractXmlReceiver::endElement()
{
    m_prevReceiver->endElement();
}


void DelegatingAbstractXmlReceiver::attribute(const QXmlName &name,
               const QStringRef &value)
{
    m_prevReceiver->attribute(name, value);
}

void DelegatingAbstractXmlReceiver::comment(const QString &value)
{
    m_prevReceiver->comment(value);
}

void DelegatingAbstractXmlReceiver::characters(const QStringRef &value)
{
    m_prevReceiver->characters(value);
}

void DelegatingAbstractXmlReceiver::startDocument()
{
    m_prevReceiver->startDocument();
}


void DelegatingAbstractXmlReceiver::endDocument()
{
    m_prevReceiver->endDocument();
}

void DelegatingAbstractXmlReceiver::processingInstruction(const QXmlName &target,
                           const QString &value)
{
{
    m_prevReceiver->processingInstruction(target, value);
}
}

void DelegatingAbstractXmlReceiver::atomicValue(const QVariant &value)
{
    m_prevReceiver->atomicValue(value);
}

void DelegatingAbstractXmlReceiver::namespaceBinding(const QXmlName &name)
{
    m_prevReceiver->namespaceBinding(name);
}


void DelegatingAbstractXmlReceiver::startOfSequence()
{
    m_prevReceiver->startOfSequence();
}

void DelegatingAbstractXmlReceiver::endOfSequence()
{
    m_prevReceiver->endOfSequence();
}


QT_END_NAMESPACE
