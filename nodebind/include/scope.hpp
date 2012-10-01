#ifndef NODEBIND_SCOPE_HPP
#define NODEBIND_SCOPE_HPP
#pragma once

#include "detail/meta.hpp"

namespace nodebind
{
	struct Context
	{
		v8::Handle<v8::Object> target;
	};

	class Scope;
	namespace detail
	{
		class Registration
		{
			friend void intrusive_ptr_add_ref(Registration * reg);
			friend void intrusive_ptr_release(Registration * reg);
		public:
			Registration();
			virtual ~Registration();
			virtual void invoke(Context& ctx) = 0;

		private:
			volatile long int referenceCount;
		};

		void intrusive_ptr_add_ref(Registration *);
		void intrusive_ptr_release(Registration *);
	}

	class Scope
	{
		typedef boost::intrusive_ptr<detail::Registration> spRegistration;
	public:
		explicit Scope(Context& ctx);
		Scope(detail::Registration * reg);

		~Scope();

		Scope& operator,(const Scope& other);
		Scope& operator[](const Scope& scope);
		
		void invoke(Context& ctx);
	private:
		std::vector<spRegistration> registrationMethods;
		Context * context;
	};

	namespace detail
	{
		class ScopeRegistration : public Registration
		{
		public:
			ScopeRegistration(const Scope& other);

			void invoke(Context& ctx);
		private:
			Scope scope;
		};
	}

	//Opens a scope
	Context open(v8::Handle<v8::Object> target);
	Scope module(Context& ctx);
}

#endif