/*
 *	Name: AppearanceOptions
 *	Author: Pawel Mrochen
 */

#pragma once

#include "GeometryType.hpp"
#include "LightType.hpp"

namespace graphics {

struct AppearanceOptions
{
	bool operator==(const AppearanceOptions& options) const noexcept
	{
		return (dissolveDistanceFactor == options.dissolveDistanceFactor) && (lodDistanceFactor == options.lodDistanceFactor) &&
			(lodBias == options.lodBias) && (referenceZoomFactor == options.referenceZoomFactor) && 
			(lightTypeMask == options.lightTypeMask) && (categoryMask == options.categoryMask) && (layerMask == options.layerMask) &&
			(geometryType == options.geometryType) && (arrangeInLayers == options.arrangeInLayers) &&
			(includeAggregatedLights == options.includeAggregatedLights);
	}

	bool operator!=(const AppearanceOptions& options) const noexcept { return !(*this == options); }

	template<typename A> void serialize(A& ar)
	{
		ar(dissolveDistanceFactor, lodDistanceFactor, lodBias, referenceZoomFactor, lightTypeMask,
			categoryMask, layerMask, geometryType, arrangeInLayers);
	}

	// Properties
	float getDissolveDistanceFactor() const noexcept { return dissolveDistanceFactor; }
	void setDissolveDistanceFactor(float factor) noexcept { dissolveDistanceFactor = factor; }
	float getLodDistanceFactor() const noexcept { return lodDistanceFactor; }
	void setLodDistanceFactor(float factor) noexcept { lodDistanceFactor = factor; }
	int getLodBias() const noexcept { return lodBias; }
	void setLodBias(int bias) noexcept { lodBias = bias; }
	float getReferenceZoomFactor() const noexcept { return referenceZoomFactor; }
	void setReferenceZoomFactor(float factor) noexcept { referenceZoomFactor = factor; }
	LightType getLightTypeMask() const noexcept { return lightTypeMask; }
	void setLightTypeMask(LightType mask) noexcept { lightTypeMask = mask; }
	unsigned int getCategoryMask() const noexcept { return categoryMask; }
	void setCategoryMask(unsigned int mask) noexcept { categoryMask = mask; }
	unsigned int getLayerMask() const noexcept { return layerMask; }
	void setLayerMask(unsigned int mask) noexcept { layerMask = mask; }
	GeometryType getGeometryType() const noexcept { return geometryType; }
	void setGeometryType(GeometryType geometryType) noexcept { this->geometryType = geometryType; }
	// #TODO bool ArrangeInLayers
	// #TODO bool IncludeAggregatedLights

	//float getZoomFactorAdjustment(Projector* projector) const noexcept // #TODO
	//{
	//	return (projector && projector->isCamera() && (referenceZoomFactor > 0.f)) ?
	//		static_cast<Camera*>(projector)->getZoomFactor()/referenceZoomFactor :
	//		1.f;
	//}

	//float getAdjustedDissolveDistanceFactor(Projector* projector) const noexcept // #TODO
	//{
	//	float zoomFactorAdjustment = (projector && projector->isCamera() && (referenceZoomFactor > 0.f)) ?
	//		static_cast<Camera*>(projector)->getZoomFactor()/referenceZoomFactor :
	//		1.f;

	//	return dissolveDistanceFactor*zoomFactorAdjustment;
	//}

	//float getAdjustedLodDistanceFactor(Projector* projector) const noexcept // #TODO
	//{
	//	float zoomFactorAdjustment = (projector && projector->isCamera() && (referenceZoomFactor > 0.f)) ?
	//		static_cast<Camera*>(projector)->getZoomFactor()/referenceZoomFactor :
	//		1.f;

	//	return lodDistanceFactor*zoomFactorAdjustment;
	//}

	float dissolveDistanceFactor = 1.f;
	float lodDistanceFactor = 1.f;
	int lodBias = 0;
	float referenceZoomFactor = 3.2f;
	LightType lightTypeMask = LightType::DIRECTIONAL | LightType::POINT | LightType::SPOT;
	unsigned int categoryMask = 0xFFFFu/*~0u*/;
	unsigned int layerMask = 0xFFFFu;
	GeometryType geometryType = GeometryType::BASE_GEOMETRY;
	bool arrangeInLayers = true;
	bool includeAggregatedLights = false;
};

} // namespace graphics
