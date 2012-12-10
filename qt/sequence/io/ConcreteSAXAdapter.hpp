/**
 * ConcreteSAXAdapter.hpp
 * Copyright © 2012 kbinani
 *
 * This file is part of cadencii.
 *
 * cadencii is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2.0 as published by the Free Software Foundation.
 *
 * cadencii is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef CADENCII_QT_SEQUENCE_IO_CONCRETESAXADAPTER_HPP_
#define CADENCII_QT_SEQUENCE_IO_CONCRETESAXADAPTER_HPP_

#include <QtXml/QXmlSimpleReader>
#include <string>
#include "../../../sequence/io/SAXAdapter.hpp"

namespace cadencii {

    /**
     * @brief An implementation of SAXAdapter.
     */
    class ConcreteSAXAdapter : public SAXAdapter, public QXmlDefaultHandler {
    private:
        QXmlInputSource *source;
        QFile file;
        QXmlSimpleReader reader;

    public:
        explicit ConcreteSAXAdapter(const std::string &filePath)
            : file(QString::fromStdString(filePath)) {
            source = new QXmlInputSource(&file);
        }

        ~ConcreteSAXAdapter() {
            delete source;
        }

        void start() {
            reader.setContentHandler(this);
            reader.parse(source);
        }

        bool startElement(
                const QString &namespaceURI, const QString &localName,
                const QString &qName, const QXmlAttributes &atts) {
            onStartElement(qName.toStdString());
            return true;
        }

        bool endElement(
                const QString &namespaceURI,
                const QString &localName,  const QString &qName) {
            onEndElement(qName.toStdString());
            return true;
        }

        bool characters(const QString &ch) {
            onCharacters(ch.toStdString());
            return true;
        }
    };
}

#endif
