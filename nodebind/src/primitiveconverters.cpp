#include "primitiveconverters.hpp"

namespace nodebind
{
	using namespace v8;

#define CONVERTER_IMPL(type, v8Name)								\
	int Converter<type>::score(const Handle<Value>& object)			\
	{																\
		if (object->BOOST_PP_CAT(Is, v8Name)())						\
		{															\
			return 1;												\
		}															\
		if (object->IsNumber() || object->IsNumberObject())			\
		{															\
			return 0;												\
		}															\
		return -1;													\
	}																\
	type Converter<type>::from(const Handle<Value>& object)			\
	{																\
		HandleScope scope;											\
		if (object->IsNumberObject())								\
		{															\
			return static_cast<type>(object->ToNumber()->Value());	\
		}															\
		return static_cast<type>(object->BOOST_PP_CAT(v8Name, Value)()); \
	}																\
	Handle<Value> Converter<type>::to(const type& value)			\
	{																\
		HandleScope scope;											\
		return scope.Close(v8Name::New(value));						\
	}																\
	void Converter<type>::after(const type&) {}

	CONVERTER_IMPL(boost::int32_t, Int32)
	CONVERTER_IMPL(boost::uint32_t, Uint32)
	CONVERTER_IMPL(boost::int64_t, Number)
	CONVERTER_IMPL(boost::uint64_t, Number)
	CONVERTER_IMPL(float, Number)
	CONVERTER_IMPL(double, Number)

#undef CONVERTER_IMPL

	//std::string
	int Converter<std::string>::score(const v8::Handle<v8::Value>& object)
	{
		if (object->IsString() || object->IsStringObject())
		{
			return 1;
		}

		//Basically, everything can be converted to a string in javascript.
		return 0;
	}

	std::string Converter<std::string>::from(const v8::Handle<v8::Value>& object)
	{
		HandleScope scope;
		Handle<String> local;
		if (object->IsStringObject())
		{
			Handle<Value> objectCopy = object;
			local = objectCopy.As<StringObject>()->StringValue();
		} 
		else if (object->IsString())
		{
			Handle<Value> objectCopy = object;
			local = objectCopy.As<String>();
		}
		else 
		{
			local = object->ToString();
		}

		if (!local.IsEmpty())
		{
			int len = local->Utf8Length();
			
			std::string result;
			result.resize(len);

			local->WriteUtf8(&result[0], len + 1, NULL, String::NO_NULL_TERMINATION);

			return result;
		}

		return "";
	}

	Handle<Value> Converter<std::string>::to(const std::string& value)
	{
		HandleScope scope;
		return scope.Close(String::New(value.c_str(), value.size()));
	}

	void Converter<std::string>::after(const std::string&)
	{}

	//char *
	int Converter<char *>::score(const v8::Handle<v8::Value>& object)
	{
		return Converter<std::string>::score(object);
	}

	char * Converter<char *>::from(const v8::Handle<v8::Value>& object)
	{
		HandleScope scope;
		Handle<String> local;
		if (object->IsStringObject())
		{
			Handle<Value> objectCopy = object;
			local = objectCopy.As<StringObject>()->StringValue();
		} 
		else if (object->IsString())
		{
			Handle<Value> objectCopy = object;
			local = objectCopy.As<String>();
		}
		else 
		{
			local = object->ToString();
		}

		if (local.IsEmpty())
		{
			return NULL;
		}

		int len = local->Utf8Length();
		char * result = new char[len];

		local->WriteUtf8(result, len + 1);

		return result;
	}

	Handle<Value> Converter<char *>::to(const char *& c)
	{
		HandleScope scope;
		return scope.Close(String::New(c));
	}

	void Converter<char *>::after(const char *& value)
	{
		delete [] value;
	}
}