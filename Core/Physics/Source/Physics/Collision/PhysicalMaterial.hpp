/*
 *	Name: PhysicalMaterial
 *	Author: Pawel Mrochen
 */

#pragma once

#include <concepts>
//#include <functional>
#include <utility>
#include <string>
#include <cstddef>
#include <intrusive_shared_ptr/ref_counted.h>
#include <nlohmann/json.hpp>
//#include <Common/Strings/NameString.hpp>
#include <Common/Strings/PathString.hpp>
#include <Mathematics/Constants.hpp>

namespace physics {
namespace templates {

//using common::NameString;
using common::PathString;
using mathematics::Constants;

template<std::floating_point T>
class PhysicalMaterial final : public isptr::weak_ref_counted<PhysicalMaterial>
{
	PhysicalMaterial() = default;
	//PhysicalMaterial(const NameString& name) : name(name) {}
	//PhysicalMaterial(NameString&& name) : name(std::move(name)) {}

	PhysicalMaterial(T density, T friction, T restitution) :
		density(density),
		frictionCoefficient(friction),
		restitutionCoefficient(restitution)
	{
	}

	//PhysicalMaterial(const NameString& name, T density, T friction, T restitution) :
	//	name(name),
	//	density(density),
	//	frictionCoefficient(friction),
	//	restitutionCoefficient(restitution)
	//{
	//}

	//PhysicalMaterial(NameString&& name, T density, T friction, T restitution) :
	//	name(std::move(name)),
	//	density(density),
	//	frictionCoefficient(friction),
	//	restitutionCoefficient(restitution)
	//{
	//}

	bool operator==(const PhysicalMaterial& material) const noexcept
	{
		return /*(name == material.name) &&*/ (density == material.density) && (frictionCoefficient == material.frictionCoefficient) &&
			(rollingResistanceCoefficient == material.rollingResistanceCoefficient) &&
			(spinningResistanceCoefficient == material.spinningResistanceCoefficient) &&
			(anisotropicFrictionEnabled == material.anisotropicFrictionEnabled) &&
			(anisotropicRollingResistanceEnabled == material.anisotropicRollingResistanceEnabled) &&
			(anisotropy == material.anisotropy) && (restitutionCoefficient == material.restitutionCoefficient) &&
			(contactStiffness == material.contactStiffness) && (contactDamping == material.contactDamping);
	}

	bool operator!=(const PhysicalMaterial& material) const noexcept { return !(*this == material); }

	template<typename A> void serialize(A& ar)
	{
		ar(/*name,*/ density, frictionCoefficient, rollingResistanceCoefficient, spinningResistanceCoefficient,
			anisotropicFrictionEnabled, anisotropicRollingResistanceEnabled, anisotropy, restitutionCoefficient,
			contactStiffness, contactDamping);
	}

	// Load
	static PhysicalMaterial* load(const PathString& path) // throw (FileSystemException);
	{
		BufferedFile file(path, FileOpenMode::READ);
		nlohmann::json data = nlohmann::json::parse(file.getText<std::string>());
		
		PhysicalMaterial* material = new PhysicalMaterial(PhysicalMaterial::DEFAULT);
		if ((auto iDensity = data.find("density")) != data.end())
			material->density = iDensity->get<T>();
		if ((auto iFrictionCoefficient = data.find("frictionCoefficient")) != data.end())
			material->frictionCoefficient = iFrictionCoefficient->get<T>();
		if ((auto iAnisotropicFrictionEnabled = data.find("anisotropicFrictionEnabled")) != data.end())
			material->anisotropicFrictionEnabled = iAnisotropicFrictionEnabled->get<bool>();
		if ((auto iRollingResistanceCoefficient = data.find("rollingResistanceCoefficient")) != data.end())
			material->rollingResistanceCoefficient = iRollingResistanceCoefficient->get<T>();
		if ((auto iAnisotropicRollingResistanceEnabled = data.find("anisotropicRollingResistanceEnabled")) != data.end())
			material->anisotropicRollingResistanceEnabled = iAnisotropicRollingResistanceEnabled->get<bool>();
		if ((auto iSpinningResistanceCoefficient = data.find("spinningResistanceCoefficient")) != data.end())
			material->spinningResistanceCoefficient = iSpinningResistanceCoefficient->get<T>();
		if ((auto iRestitutionCoefficient = data.find("restitutionCoefficient")) != data.end())
			material->restitutionCoefficient = iRestitutionCoefficient->get<T>();
		if ((auto iContactStiffness = data.find("contactStiffness")) != data.end())
			material->contactStiffness = iContactStiffness->get<T>();
		if ((auto iContactDamping = data.find("contactDamping")) != data.end())
			material->contactDamping = iContactDamping->get<T>();

		return material;
	}
	
	// Save
	void save(const PathString& path) const // throw (FileSystemException);
	{
		BufferedFile file(path, FileOpenMode::WRITE);
		nlohmann::json data;
		
		if (density != DEFAULT.density)
			data["density"] = density;
		if (frictionCoefficient != DEFAULT.frictionCoefficient)
			data["frictionCoefficient"] = frictionCoefficient;
		if (anisotropicFrictionEnabled != DEFAULT.anisotropicFrictionEnabled)
			data["anisotropicFrictionEnabled"] = anisotropicFrictionEnabled;
		if (rollingResistanceCoefficient != DEFAULT.rollingResistanceCoefficient)
			data["rollingResistanceCoefficient"] = rollingResistanceCoefficient;
		if (anisotropicRollingResistanceEnabled != DEFAULT.anisotropicRollingResistanceEnabled)
			data["anisotropicRollingResistanceEnabled"] = anisotropicRollingResistanceEnabled;
		if (spinningResistanceCoefficient != DEFAULT.spinningResistanceCoefficient)
			data["spinningResistanceCoefficient"] = spinningResistanceCoefficient;
		//data["anisotropy"] = anisotropy;
		if (restitutionCoefficient != DEFAULT.restitutionCoefficient)
			data["restitutionCoefficient"] = restitutionCoefficient;
		if (contactStiffness != DEFAULT.contactStiffness)
			data["contactStiffness"] = contactStiffness;
		if (contactDamping != DEFAULT.contactDamping)
			data["contactDamping"] = contactDamping;

		constexpr int INDENT = 4;
		file.setText(data.dump(INDENT), true);
	}

	// Name
	//const NameString& getName() const noexcept { return name; }
	//void setName(const NameString& name) { this->name = name; }
	//void setName(NameString&& name) { this->name = std::move(name); }

	// Density
	T getDensity() const noexcept { return density; }
	void setDensity(T density) noexcept { this->density = density; }

	// Friction
	T getFrictionCoefficient() const noexcept { return frictionCoefficient; }
	void setFrictionCoefficient(T friction) noexcept { frictionCoefficient = friction; }
	bool isAnisotropicFrictionEnabled() const noexcept { return anisotropicFrictionEnabled; }
	void setAnisotropicFrictionEnabled(bool enabled) noexcept { anisotropicFrictionEnabled = enabled; }
	T getRollingResistanceCoefficient() const noexcept { return rollingResistanceCoefficient; }
	void setRollingResistanceCoefficient(T resistance) noexcept { rollingResistanceCoefficient = resistance; }
	bool isAnisotropicRollingResistanceEnabled() const noexcept { return anisotropicRollingResistanceEnabled; }
	void setAnisotropicRollingResistanceEnabled(bool enabled) noexcept { anisotropicRollingResistanceEnabled = enabled; }
	T getSpinningResistanceCoefficient() const noexcept { return spinningResistanceCoefficient; }
	void setSpinningResistanceCoefficient(T resistance) noexcept { spinningResistanceCoefficient = resistance; }
	const Vector3& getAnisotropy() const noexcept { return anisotropy; }
	void setAnisotropy(const Vector3& anisotropy) noexcept { this->anisotropy = anisotropy; }

	// Restitution
	T getRestitutionCoefficient() const noexcept { return restitutionCoefficient; }
	void setRestitutionCoefficient(T restitution) noexcept { restitutionCoefficient = restitution; }

	// Contact
	T getContactStiffness() const noexcept { return contactStiffness; }
	void setContactStiffness(T stiffness) noexcept { contactStiffness = stiffness; }
	T getContactDamping() const noexcept { return contactDamping; }
	void setContactDamping(T damping) noexcept { contactDamping = damping; }

	static const PhysicalMaterial DEFAULT;

	//NameString name;
	T density = T();
	T frictionCoefficient = T();
	T rollingResistanceCoefficient = T();	// torsional friction orthogonal to contact normal (useful to stop spheres rolling forever)
	T spinningResistanceCoefficient = T(); 	// torsional friction around the contact normal (useful for grasping)
	bool anisotropicFrictionEnabled = false;
	bool anisotropicRollingResistanceEnabled = false;
	Vector3<T> anisotropy = { T(1) };
	// #TODO frictionCombineMode;			// average/min/multiply/max
	T restitutionCoefficient = T();			// bounciness
	// #TODO restitutionCombineMode;		// average/min/multiply/max
	T contactStiffness = Constants<T>::INF;
	T contactDamping = T();
};

template<std::floating_point T> const PhysicalMaterial<T> PhysicalMaterial<T>::DEFAULT{ T(), T(0.5), T() };

} // namespace templates

#if PHYSICS_DOUBLE_PRECISION
using PhysicalMaterial = templates::PhysicalMaterial<double>;
#else
using PhysicalMaterial = templates::PhysicalMaterial<float>;
#endif

} // namespace physics

namespace std {

//template<typename T>
//struct hash;
//
//template<typename T>
//struct hash<::physics::templates::PhysicalMaterial<T>> // #TODO
//{
//	size_t operator()(const ::physics::templates::PhysicalMaterial<T>& material) const noexcept
//	{
//	}
//};

} // namespace std
