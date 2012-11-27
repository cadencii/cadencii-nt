/**
 * SAXAdapter.hpp
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
#ifndef CADENCII_SEQUENCE_IO_SAXADAPTER_HPP_
#define CADENCII_SEQUENCE_IO_SAXADAPTER_HPP_

#include <string>
#include "../../vsq/InputStream.hpp"

namespace cadencii {

    class XVSQFileReader;

    /**
     * @brief An adapter class for SAX(Simple API for XML).
     */
    class SAXAdapter {
        friend class XVSQFileReader;

    private:
        XVSQFileReader *reader;

    protected:
        VSQ_NS::InputStream *stream;

    public:
        explicit SAXAdapter(VSQ_NS::InputStream *stream);

        virtual ~SAXAdapter();

        virtual void start() = 0;

    protected:
        /**
         * @brief  Call this method when startElement is called.
         * @param  name  A tag name.
         */
        void onStartElement(const std::string &name);

        /**
         * @brief  Call this method when endElement is called.
         * @param  name  A tag name.
         */
        void onEndElement(const std::string &name);

        /**
         * @brief  Call this method when characters is called.
         * @param  ch  A text.
         */
        void onCharacters(const std::string &ch);

    private:
        SAXAdapter();

        void setReader(XVSQFileReader *reader);
    };
}

#endif
