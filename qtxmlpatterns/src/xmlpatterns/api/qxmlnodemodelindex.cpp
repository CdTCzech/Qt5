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

#include "qabstractxmlnodemodel.h"

QT_BEGIN_NAMESPACE

using namespace QPatternist;

/**
 * @file
 * @short Contains the implementation of QXmlNodeModelIndex.
 */

using namespace QPatternist;

/*!
  \class QXmlNodeModelIndex
  \brief The QXmlNodeModelIndex class identifies a node in an XML node model subclassed from QAbstractXmlNodeModel.
  \reentrant
  \since 4.4
  \ingroup xml-tools
  \inmodule QtXmlPatterns

  QXmlNodeModelIndex is an index into an \l{QAbstractXmlNodeModel}
  {XML node model}. It contains:

  \list
    \li A pointer to an \l{QAbstractXmlNodeModel} {XML node model},
    which is returned by model(), and
    \li Some data, which is returned by data(), internalPointer(),
    and additionalData().
  \endlist

  Because QXmlNodeModelIndex is intentionally a simple class, it
  doesn't have member functions for accessing the properties of
  nodes. For example, it doesn't have functions for getting a
  node's name or its list of attributes or child nodes. If you find
  that you need to retrieve this kind of information from your
  query results, there are two ways to proceed.

  \list

  \li Send the output of your XQuery to an \l{QAbstractXmlReceiver}
  {XML receiver}, or

  \li Let your XQuery do all the work to produce the desired result.

  \endlist

  The second case is explained by example. Suppose you want to
  populate a list widget with the values of certain attributes from a
  set of result elements. You could write an XQuery to return the set
  of elements, and then you would write the code to iterate over the
  result elements, get their attributes, and extract the desired
  string values. But the simpler way is to just augment your XQuery to
  finding the desired attribute values. Then all you have to do is
  evaluate the XQuery using the version of QXmlQuery::evaluateTo()
  that populates a QStringList, which you can send directly to your
  widget.

  QXmlNodeModelIndex doesn't impose any restrictions on the \c data
  value an QXmlNodeModelIndex should contain. The meaning of the data
  left to the associated \l {QAbstractXmlNodeModel} {node model}.
  Because QXmlNodeModelIndex depends on a particular subclass of
  QAbstractXmlNodeModel for its existence, the only way you can create
  an instance of QXmlNodeModelIndex is by asking the node model to
  create one for you with QAbstractXmlNodeModel::createIndex(). Since
  that function is protected, it is usually a good idea to write a
  public function that creates a QXmlNodeModelIndex from arguments that
  are appropriate for your particular node model.

  A default constructed node index is said to be null, i.e., isNull()
  returns true.

  QXmlNodeModelIndex and QAbstractXmlNodeModel follow the same design
  pattern used for QModelIndex and QAbstractItemModel.
 */

/*!
  \enum QXmlNodeModelIndex::Constants

  \value ForwardAxis All forward axes include this flag.
  \value ReverseAxis All reverse axes include this flag.
 */

/*!
  \enum QXmlNodeModelIndex::DocumentOrder

  Identifies the specific node comparison operator that should be
  used.

  \value Precedes Signifies the \c \<\< operator. Test whether the
         first operand precedes the second in the document.

  \value Follows Signifies the \c \>\> operator. Test whether the
                 first operand follows the second in the document.

  \value Is Signifies the \c is operator. Test whether two nodes have
  the same node identity.
 */

/*!
  \enum QAbstractXmlNodeModel::SimpleAxis

  Four axes that each contain one node only.

  \value Parent The parent of the context node
  \value FirstChild The first child of the context node
  \value PreviousSibling The previous child of the context node
  \value NextSibling The next child of the context node
*/

/*!
 \enum QXmlNodeModelIndex::Axis
 \internal

   Identify the axes emanating from a node.

   The axes AxisChild, AxisDescendant, AxisAttribute, AxisSelf,
   AxisDescendantOrSelf, AxisFollowingSibling, and AxisFollowing are
   forward axes.

   The axes AxisParent, AxisAncestor, AxisPrecedingSibling,
   AxisPreceding and AxisAncestorOrSelf are reverse axes.

   \sa {http://www.w3.org/TR/xquery/#axes}{XQuery 1.0: An XML Query Language, 3.2.1.1 Axes}

   \value AxisChild                The \c child axis.

   \value AxisDescendant           The \c descendant axis.

   \value AxisAttribute            The \c attribute axis. Note: There
                                   is a node kind named \c{Attribute}.

   \value AxisSelf                 The \c self axis.

   \value AxisDescendantOrSelf     The \c descendant-or-self axis.

   \value AxisFollowingSibling     The \c following-sibling axis.

   \value AxisNamespace            The \c namespace axis. Note: Does
                                   not exist in XQuery; deprecated in
                                   XPath 2.0 (optionally supported);
                                   mandatory in XPath 1.0.

   \value AxisFollowing            The \c following axis.

   \value AxisParent               The \c parent axis.

   \value AxisAncestor             The \c ancestor axis.

   \value AxisPrecedingSibling     The \c preceding-sibling axis.

   \value AxisPreceding            The \c preceding axis.

   \value AxisAncestorOrSelf       The \c ancestor-or-self axis.
*/


/*!
  \since 4.4
  \relates QHash

  Computes a hash key from the QXmlNodeModelIndex \a index, and
  returns it. This function would be used by QHash if you wanted
  to build a hash table for instances of QXmlNodeModelIndex.

  The hash is computed on QXmlNodeModelIndex::data(),
  QXmlNodeModelIndex::additionalData(), and
  QXmlNodeModelIndex::model(). This means the hash key can be used for
  node indexes from different node models.
 */
uint qHash(const QXmlNodeModelIndex &index)
{
    return uint(index.data() + index.additionalData() + quintptr(index.model()));
}

/*!
  \enum QXmlNodeModelIndex::NodeKind

  Identifies a kind of node.

  \value Attribute Identifies an attribute node
  \value Text Identifies a text node
  \value Comment Identifies a comment node
  \value Document Identifies a document node
  \value Element Identifies an element node
  \value Namespace Identifies a namespace node
  \value ProcessingInstruction Identifies a processing instruction.

  Note that the optional XML declaration at very beginning of the XML
  document is not a processing instruction

  \sa QAbstractXmlNodeModel::kind()
*/

/*!
 \typedef QXmlNodeModelIndex::List

 Typedef for QList<QXmlNodeModelIndex>.
 */

/*!
  \fn bool QXmlNodeModelIndex::operator==(const QXmlNodeModelIndex &other) const
  Returns true if this node is the same as \a other. This operator
  does not compare values, children, or names of nodes. It compares
  node identities, i.e., whether two nodes are from the same document
  and are found at the exact same place.
 */

/*!
  \fn bool QXmlNodeModelIndex::operator!=(const QXmlNodeModelIndex &other) const
  Returns true if \a other is the same node as this.
 */

/*!
 \fn QXmlNodeModelIndex::QXmlNodeModelIndex()

 Default constructor. Creates an item that is \c null.

 \sa isNull()
 */

/*!
 \fn QXmlNodeModelIndex::QXmlNodeModelIndex(const QXmlNodeModelIndex &other)

 Standard copy constructor. Creates a QXmlNodeModelIndex instance that
 is a copy of \a other.
 */

/*!
 \fn bool QXmlNodeModelIndex::isNull() const

 Returns true if this QXmlNodeModelIndex is a default constructed
 value, otherwise false.

 A null QXmlNodeModelIndex doesn't represent any node and cannot
 be used in conjunction with QAbstractXmlNodeModel.
 */

/*!
 \fn const QAbstractXmlNodeModel *QXmlNodeModelIndex::model() const

 Returns the QAbstractXmlNodeModel that this node index refers to.
 QXmlNodeModelIndex does not own QAbstractXmlNodeModel and does not
 keep track of its lifetime, so this pointer will dangle if the
 QAbstractXmlNodeModel is deallocated first.

 There is no setter for the node model because instances of
 QXmlNodeModelIndex instances are only created with
 QAbstractXmlNodeModel::createIndex().
*/

/*!
 \fn qint64 QXmlNodeModelIndex::data() const

 Returns the first data value. The node index holds two data values.
 additionalData() returns the second one.

 \sa additionalData()
*/

/*!
 \fn void *QXmlNodeModelIndex::internalPointer() const

 Returns the first data value as a void* pointer.

 \sa additionalData()
*/

/*!
 \fn qint64 QXmlNodeModelIndex::additionalData() const

 Returns the second data value. The node index holds two data values.
 data() returns the first one.

 \sa data()
*/

/*!
 \fn void QXmlNodeModelIndex::reset()
 \internal

 Resets this QXmlNodeModelIndex to be null. It is equivalent to
 writing:

 \snippet code/src_xmlpatterns_api_qabstractxmlnodemodel.cpp 0
 */

/*!
 \fn QXmlName QXmlNodeModelIndex::name() const
 \internal
*/
inline QXmlName QXmlNodeModelIndex::name() const
{
    return m_model->name(*this);
}

/*!
 \typedef QXmlNodeModelIndex::Iterator
 \internal

 Typedef for QAbstractXmlForwardIterator<QXmlNodeModelIndex>.
 */
/*!
 \fn QXmlNodeModelIndex QXmlNodeModelIndex::root() const
 \internal
*/
inline QXmlNodeModelIndex QXmlNodeModelIndex::root() const
{
    return m_model->root(*this);
}


/*!
 \fn QExplicitlySharedDataPointer<QAbstractXmlForwardIterator<QXmlNodeModelIndex> > QXmlNodeModelIndex::iterate(const Axis axis) const
 \internal
*/
inline QXmlNodeModelIndex::Iterator::Ptr QXmlNodeModelIndex::iterate(const QXmlNodeModelIndex::Axis axis) const
{
    return m_model->iterate(*this, axis);
}

/*!
 \fn QExplicitlySharedDataPointer<QAbstractXmlForwardIterator<QPatternist::Item> > QXmlNodeModelIndex::sequencedTypedValue() const
 \internal
*/
inline QExplicitlySharedDataPointer<QAbstractXmlForwardIterator<QPatternist::Item> > QXmlNodeModelIndex::sequencedTypedValue() const
{
    return m_model->sequencedTypedValue(*this);
}

/*!
 \fn QUrl QXmlNodeModelIndex::documentUri() const
 \internal
*/
inline QUrl QXmlNodeModelIndex::documentUri() const
{
    return m_model->documentUri(*this);
}

/*!
 \fn QUrl QXmlNodeModelIndex::baseUri() const
 \internal
*/
inline QUrl QXmlNodeModelIndex::baseUri() const
{
    return m_model->baseUri(*this);
}

/*!
 \fn NodeKind QXmlNodeModelIndex::kind() const
 \internal
*/
inline QXmlNodeModelIndex::NodeKind QXmlNodeModelIndex::kind() const
{
    return m_model->kind(*this);
}

/*!
 \fn bool QXmlNodeModelIndex::isDeepEqual(const QXmlNodeModelIndex &other) const
 \internal
*/
inline bool QXmlNodeModelIndex::isDeepEqual(const QXmlNodeModelIndex &other) const
{
    return m_model->isDeepEqual(*this, other);
}

/*!
 \fn DocumentOrder QXmlNodeModelIndex::compareOrder(const QXmlNodeModelIndex &other) const
 \internal
*/
inline QXmlNodeModelIndex::DocumentOrder QXmlNodeModelIndex::compareOrder(const QXmlNodeModelIndex &other) const
{
    Q_ASSERT_X(model() == other.model(), Q_FUNC_INFO, "The API docs guarantees the two nodes are from the same model");
    return m_model->compareOrder(*this, other);
}

/*!
 \fn void QXmlNodeModelIndex::sendNamespaces(QAbstractXmlReceiver *const receiver) const
 \internal
*/
inline void QXmlNodeModelIndex::sendNamespaces(QAbstractXmlReceiver *const receiver) const
{
    m_model->sendNamespaces(*this, receiver);
}

/*!
 \fn QVector<QXmlName> QXmlNodeModelIndex::namespaceBindings() const
 \internal
*/
inline QVector<QXmlName> QXmlNodeModelIndex::namespaceBindings() const
{
    return m_model->namespaceBindings(*this);
}

/*!
 \fn QXmlName::NamespaceCode QXmlNodeModelIndex::namespaceForPrefix(const QXmlName::PrefixCode prefix) const
 \internal
*/
inline QXmlName::NamespaceCode QXmlNodeModelIndex::namespaceForPrefix(const QXmlName::PrefixCode prefix) const
{
    return m_model->namespaceForPrefix(*this, prefix);
}

/*!
 \fn QString QXmlNodeModelIndex::stringValue() const
 \internal
*/
inline QString QXmlNodeModelIndex::stringValue() const
{
    return m_model->stringValue(*this);
}

/*!
 \fn QPatternist::ItemTypePtr QXmlNodeModelIndex::type() const
 \internal
*/
inline QPatternist::ItemType::Ptr QXmlNodeModelIndex::type() const
{
    return m_model->type(*this);
}

/*!
 \fn bool QXmlNodeModelIndex::is(const QXmlNodeModelIndex &other) const
 \internal
*/
inline bool QXmlNodeModelIndex::is(const QXmlNodeModelIndex &other) const
{
    return *this == other;
}

QT_END_NAMESPACE
