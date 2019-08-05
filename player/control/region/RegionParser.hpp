#pragma once

#include "constants.hpp"
#include "RegionOptions.hpp"
#include "control/media/MediaOptions.hpp"

#include <boost/property_tree/ptree.hpp>

class IRegion;
class IRegionView;
class IMedia;

class RegionParser
{
public: 
    std::unique_ptr<IRegion> regionFrom(const xml_node& node);
    RegionOptions optionsFrom(const xml_node& node);

private:
    std::unique_ptr<IRegion> createRegion(const RegionOptions& options, const std::shared_ptr<IRegionView>& view);
    std::shared_ptr<IRegionView> createView(const RegionOptions& options);

    void addMedia(IRegion& region, const xml_node& node);
    MediaOptions::Type mediaTypeFrom(const xml_node& node);
    std::pair<int, int> mediaPositionInRegion(IRegion& region, IMedia& media);

};

