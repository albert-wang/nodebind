#include "scope.hpp"
#include "overloaded.hpp"

namespace nodebind
{
	namespace detail
	{
		Registration::Registration()
			:referenceCount(0)
		{}

		Registration::~Registration()
		{}

		void intrusive_ptr_add_ref(Registration * reg)
		{
			InterlockedIncrement(&reg->referenceCount);
		}

		void intrusive_ptr_release(Registration * reg)
		{
			long result = InterlockedDecrement(&reg->referenceCount);
			if (result == 0)
			{
				delete reg;
			}
		}

		ScopeRegistration::ScopeRegistration(const Scope& other)
			:scope(other)
		{}

		void ScopeRegistration::invoke(Context& ctx)
		{
			scope.invoke(ctx);
		}
	}

	OverloadedFunctionResolver * Context::findOverloadedFunction(const char * name)
	{
		std::map<std::string, OverloadedFunctionResolver *>::iterator it = resolvers.find(name);
		if (it == resolvers.end())
		{
			OverloadedFunctionResolver * result = new OverloadedFunctionResolver();

			v8::Local<v8::Value> resolverValue = v8::External::Wrap(result);
			v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(&OverloadedFunctionResolver::invoke, resolverValue);

			target->Set(v8::String::NewSymbol(name), tpl->GetFunction());
			resolvers.insert(std::make_pair(std::string(name), result));

			return result;
		} else 
		{
			return it->second;
		}
	}

	Scope::Scope(Context& ctx)
		:context(&ctx)
	{}

	Scope::Scope(detail::Registration * reg)
		:context(NULL)
	{
		registrationMethods.push_back(spRegistration(reg));
	}

	Scope::~Scope()
	{
		this->invoke(*context);
	}

	Scope& Scope::operator ,(const Scope& other)
	{
		registrationMethods.push_back(spRegistration(new detail::ScopeRegistration(other)));
		return *this;
	}

	Scope& Scope::operator [](const Scope& other)
	{
		registrationMethods.push_back(spRegistration(new detail::ScopeRegistration(other)));
		return *this;
	}

	void Scope::invoke(Context& ctx)
	{
		for (size_t i = 0; i < registrationMethods.size(); ++i)
		{
			registrationMethods[i]->invoke(ctx);
		}
	}

	Context open(v8::Handle<v8::Object> target)
	{
		v8::HandleScope scope;

		Context result;
		result.target = target;

		return result;
	}

	Scope module(Context& ctx)
	{
		return Scope(ctx);
	}
}