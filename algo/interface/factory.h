#pragma once

#include "defs.h"

#include <QMap>
#include <QVariant>
#include <QtXml/QDomElement>
#include <QStringList>

/*
 * Factory for creating resources both from tenant and from resources description.
 * XML representation is used.
 * These is a static class used for parsing xml files only.
 */
class Factory
{
public:
	typedef QMap<Element *, QDomElement> ElementsMap;
	typedef QMap<QString, QVariant> Properties;
private:
    Factory() {}
    virtual ~Factory() {}

public:

    // These method is useful for any xml representation.
    // Returns: Map of all found elements on corresponding xml objects
    static ElementsMap getXmlElementsByTypes(const class QStringList &, const QDomElement &);

private:

    static void createElementsFromNodeList(QDomNodeList & list, ElementsMap& elementsMap);
    static Element * createElementFromXML(const QDomElement & element);
    static Properties getAttributesFromXML(const QDomNamedNodeMap &);
    static Properties getParametersFromXML(const QDomNodeList &);

    // Create link or node depending on type name
    static Link * createLink(const QDomElement & element);
    static Element * createNode(const QDomElement & element);

};
