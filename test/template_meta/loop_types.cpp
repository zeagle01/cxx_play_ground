
#include "gmock/gmock.h"
#include <string>
#include <vector>


using namespace testing;




namespace loop_types
{


    template<typename... P>
	struct type_list {};


	//front
	template<typename L >
	struct front_imp;

    template<typename H, typename... T>
	struct front_imp<type_list<H, T...>>
	{
		using type = H;
	};

	template<typename L>
	using front = typename front_imp<L>::type;

	//pop front
	template<typename L>
	struct pop_front_imp;

    template<typename H, typename... T>
	struct pop_front_imp<type_list<H, T...>>
	{
		using type = type_list<T...>;
	};

	template<typename L>
	using pop_front = typename pop_front_imp<L>::type;

	//push front
	template<typename H,typename L >
	struct push_front_imp;

	template<typename H, typename... T>
	struct push_front_imp<H, type_list< T...>>
	{
		using type = type_list<H,T...>;
	};

	template<typename H,typename L>
	using push_front = typename push_front_imp<H, L>::type;

	//get_nth_element
	template<typename L,size_t n> 
	struct get_nth_element_imp
	{
		using type = typename get_nth_element_imp<pop_front<L>, n - 1>::type;
	};

	template<typename L> 
	struct get_nth_element_imp<L,0>
	{
		using type = front<L>;
	};

	template<typename L,size_t n> 
	using get_nth_element = typename get_nth_element_imp<L, n>::type;

	//is_empty
	template<typename L>
	struct is_empty_imp
	{
		static inline constexpr  bool value = false;
	};

	template<>
	struct is_empty_imp<type_list<>>
	{
		static inline constexpr  bool value = true;
	};

	template<typename L>
	inline constexpr bool is_empty = is_empty_imp<L>::value;


	/////////////////////////type list end///////////////////////


	struct get_type_name
	{
		template<typename T>
		static void apply(std::vector<std::string >& ret)
		{
			ret.push_back(typeid(T).name());
		}
	};

	template<typename F, typename L>
	struct for_each_types_inner;

	template<typename F, typename ...T>
	struct for_each_types_inner<F, type_list<T...>>
	{
		static void apply(F&& f)
		{
			(f.operator() < T > (), ...);
		}
	};

	template< typename L, typename F, typename ...Param >
	void for_each_type(Param&&... p)
	{
		auto template_function_wrapper = [&]<typename T>()
		{
			(F:: template apply<T>(std::forward<Param>(p)...));
		};
		using func_t = decltype(template_function_wrapper);
		for_each_types_inner<func_t, L>::apply(std::forward<func_t>(template_function_wrapper));

	};

	TEST(Loop_Types_Test, loop_with_params)
	{
		std::vector<std::string> act;
		for_each_type< type_list<int, float>, get_type_name>(act);

		std::vector<std::string> exp{ "int","float" };

		EXPECT_THAT(act, Eq(exp));
	}



}
