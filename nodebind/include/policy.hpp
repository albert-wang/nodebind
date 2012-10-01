#ifndef NODEBIND_POLICY_HPP
#define NODEBIND_POLICY_HPP

namespace nodebind
{
	//When added to a policy list, this transform the call into an async call.
	//There are no longer guarantees on which thread the function will be called, 
	//so any function with this policy should be thread safe and not access any v8 
	//objects.
	struct AsyncPolicyBase
	{};

	struct AsyncPolicy : public AsyncPolicyBase
	{};

	//Denotes that the function has a first argument in this. Used to denote member functions
	//and extension methods. Basically, anything in a class_<T>.def() will be thiscalled.
	struct ThiscallPolicyBase
	{};

	struct ThiscallPolicy : public ThiscallPolicyBase
	{};

	//This policy means that the return-value (or an out parameter) should be adopted
	//by node.
	struct AdoptPolicyBase 
	{};

	template<size_t NativeArgumentIndex>
	struct AdoptPolicy : public AdoptPolicyBase
	{
		static const size_t Index = NativeArgumentIndex;
	};

	//This policy denotes an in/out value.
	struct InOutPolicyBase
	{};
	
	template<size_t NativeArgumentIndex>
	struct InOutPolicy : public InOutPolicyBase
	{
		static const size_t Index = NativeArgumentIndex;
	};

	//This policy denotes a pure out value. This will cause the 
	//argument referenced to be removed from the node.js binding's argument signature.
	struct PureOutPolicyBase
	{};

	template<size_t NativeArgumentIndex>
	struct PureOutPolicy : public PureOutPolicyBase
	{
		static const size_t Index = NativeArgumentIndex;
	};

	//This policy denotes that the function returns a reference or pointer to a given argument.
	//This cannot be used to reference the result value.
	struct FluentPolicyBase
	{};

	template<size_t NativeArgumentIndex>
	struct FluentPolicy : public FluentPolicyBase
	{
		static const size_t Index = NativeArgumentIndex;
	};

	//This policy denotes that the return value is a container that has boost::begin and boost::end
	//defined for it. An optimized generator is returned instead of a copy of the container.
	struct ReturnsContainerBase
	{};

	template<size_t NativeArgumentIndex>
	struct ReturnsContainer : public ReturnsContainerBase
	{
		static const size_t Index = NativeArgumentIndex;
	};

	static const size_t ReturnIndex = ~0u;
}

#endif
