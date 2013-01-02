/**
 * XVSQSpec.hpp
 * Copyright © 2013 kbinani
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
#ifndef CADENCII_SEQUENCE_IO_XVSQSPEC_HPP_
#define CADENCII_SEQUENCE_IO_XVSQSPEC_HPP_

#include <string>
#include <map>
#include "../../vsq/StringUtil.hpp"

namespace cadencii {

    /**
     * @brief A utility for XVSQ file spec.
     */
    class XVSQSpec {
    private:
        std::map<std::string, std::string> tagNameMap;

    public:
        XVSQSpec() {
            tagNameMap.insert(std::make_pair("PIT", "pit"));
            tagNameMap.insert(std::make_pair("PBS", "pbs"));
            tagNameMap.insert(std::make_pair("DYN", "dyn"));
            tagNameMap.insert(std::make_pair("BRE", "bre"));
            tagNameMap.insert(std::make_pair("BRI", "bri"));
            tagNameMap.insert(std::make_pair("CLE", "cle"));
            tagNameMap.insert(std::make_pair("GEN", "gen"));
            tagNameMap.insert(std::make_pair("POR", "por"));
            tagNameMap.insert(std::make_pair("OPE", "ope"));
            tagNameMap.insert(std::make_pair("harmonics", "harmonics"));
            tagNameMap.insert(std::make_pair("fx2depth", "fx2depth"));
            tagNameMap.insert(std::make_pair("reso1FreqBPList", "reso1freq"));
            tagNameMap.insert(std::make_pair("reso2FreqBPList", "reso2freq"));
            tagNameMap.insert(std::make_pair("reso3FreqBPList", "reso3freq"));
            tagNameMap.insert(std::make_pair("reso4FreqBPList", "reso4freq"));
            tagNameMap.insert(std::make_pair("reso1BWBPList", "reso1bw"));
            tagNameMap.insert(std::make_pair("reso2BWBPList", "reso2bw"));
            tagNameMap.insert(std::make_pair("reso3BWBPList", "reso3bw"));
            tagNameMap.insert(std::make_pair("reso4BWBPList", "reso4bw"));
            tagNameMap.insert(std::make_pair("reso1AmpBPList", "reso1amp"));
            tagNameMap.insert(std::make_pair("reso2AmpBPList", "reso2amp"));
            tagNameMap.insert(std::make_pair("reso3AmpBPList", "reso3amp"));
            tagNameMap.insert(std::make_pair("reso4AmpBPList", "reso4amp"));
        }

        /**
         * @brief Get xml element name by curve name.
         * @param  curveName  Curve name.
         * @return  Xml element name.
         */
        std::string getElementName(const std::string &curveName)const {
            std::map<std::string, std::string>::const_iterator i
                    = tagNameMap.begin();
            std::string lowerCurveName = StringUtil::toLower(curveName);
            for (; i != tagNameMap.end(); ++i) {
                std::string elementName = i->first;
                if (i->second == lowerCurveName) {
                    return elementName;
                }
            }
            return "";
        }

        /**
         * @brief Get curve name for specified xml element name.
         * @param  elementName  Xml element name.
         * @return  Curve Name.
         */
        std::string getCurveName(const std::string &elementName)const {
            if (tagNameMap.find(elementName) != tagNameMap.end()) {
                return tagNameMap.at(elementName);
            } else {
                return "";
            }
        }
    };
}

#endif
