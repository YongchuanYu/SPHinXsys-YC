/**
 * @file 	base_particle_dynamics.hpp
 * @brief 	This is the implementation of the template class for 3D build
 * @author	Chi ZHang and Xiangyu Hu
 */

#ifndef BASE_PARTICLE_DYNAMICS_HPP
#define BASE_PARTICLE_DYNAMICS_HPP

#include "base_particle_dynamics.h"
#include "base_body.h"

namespace SPH
{
	//=================================================================================================//
	template <class ParticlesType, class ContactParticlesType, class BaseDataDelegateType>
	DataDelegateContact<ParticlesType, ContactParticlesType, BaseDataDelegateType>::
		DataDelegateContact(BaseBodyRelationContact &body_contact_relation)
		: BaseDataDelegateType(body_contact_relation.sph_body_)
	{
		RealBodyVector contact_sph_bodies = body_contact_relation.contact_bodies_;
		for (size_t i = 0; i != contact_sph_bodies.size(); ++i)
		{
			contact_bodies_.push_back(contact_sph_bodies[i]);
			contact_particles_.push_back(DynamicCast<ContactParticlesType>(this, &contact_sph_bodies[i]->getBaseParticles()));
			contact_configuration_.push_back(&body_contact_relation.contact_configuration_[i]);
		}
	}
	//=================================================================================================//
	template <class ParticleDynamicsInnerType, class ContactDataType>
	ParticleDynamicsComplex<ParticleDynamicsInnerType, ContactDataType>::
		ParticleDynamicsComplex(ComplexBodyRelation &complex_relation,
								BaseBodyRelationContact &extra_contact_relation)
		: ParticleDynamicsInnerType(complex_relation.inner_relation_),
		  ContactDataType(complex_relation.contact_relation_)
	{
		if (&complex_relation.sph_body_ != &extra_contact_relation.sph_body_)
		{
			std::cout << "\n Error: the two body_relations do not have the same source body!" << std::endl;
			std::cout << __FILE__ << ':' << __LINE__ << std::endl;
			exit(1);
		}

		for (auto &extra_body : extra_contact_relation.contact_bodies_)
		{
			// here we first obtain the pointer to the most derived class and then implicitly downcast it to
			// the types defined in the base complex dynamics
			this->contact_bodies_.push_back(extra_body->ThisObjectPtr());
			this->contact_particles_.push_back(extra_body->getBaseParticles().ThisObjectPtr());
		}

		for (size_t i = 0; i != extra_contact_relation.contact_bodies_.size(); ++i)
		{
			this->contact_configuration_.push_back(&extra_contact_relation.contact_configuration_[i]);
		}
	}
	//=================================================================================================//
}
//=================================================================================================//
#endif // BASE_PARTICLE_DYNAMICS_HPP