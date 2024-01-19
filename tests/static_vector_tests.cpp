//#include <string>

#include "static_vector.hpp"
#include "simpletest.hpp"


DEFINE_TEST(Example_with_int_type)
{
    static_vector<int, 5> vec;
    vec.push_back(45);
    vec.push_back(4);
    vec.push_back(135);
    vec.push_back(6);
    vec.push_back(1000);
    TEST(vec[0] == 45);
    TEST(vec[1] == 4);
    TEST(vec[2] == 135);
    TEST(vec[3] == 6);
    TEST(vec[4] == 1000);

    static_vector<int, 5> vec_copy(vec);
    TEST(vec_copy[0] == 45);
    TEST(vec_copy[1] == 4);
    TEST(vec_copy[2] == 135);
    TEST(vec_copy[3] == 6);
    TEST(vec_copy[4] == 1000);

    static_vector<int, 5> vec_new;
    vec_new.push_back(90);
    vec_new.push_back(5);
    vec_new.push_back(555);
    vec_new = vec;
    TEST(vec_new[0] == 45);
    TEST(vec_new[1] == 4);
    TEST(vec_new[2] == 135);
    TEST(vec_new[3] == 6);
    TEST(vec_new[4] == 1000);
    TEST(vec_new.get_size() == vec.get_size());

    static_vector<int, 5> vec_new_new;
    vec_new_new.push_back(90);
    vec_new_new.push_back(5);
    vec_new_new.push_back(555);

    TEST(vec.get_size() == 5);
    vec = vec_new_new;
    TEST(vec[0] == 90);
    TEST(vec[1] == 5);
    TEST(vec[2] == 555);
    TEST(vec.get_size() == 3);
}

//DEFINE_TEST(Example_with_string_type)
//{
//	static_vector<custom::string, 3> vec;
//	vec.push_back(custom::string(5, '*'));
//	vec.push_back(custom::string("Hello"));
//	vec.push_back("world");
//	TEST(vec[0] == "*****");
//	TEST(vec[1] == "Hello");
//	TEST(vec[2] == "world");
//	TEST(vec.get_size() == 3);
//
//	static_vector <custom::string, 3> vec_copy(vec);
//	TEST(vec_copy[0] == "*****");
//	TEST(vec_copy[1] == "Hello");
//	TEST(vec_copy[2] == "world");
//
////	DEFINE_TEST(Size_of_first_is_less_or_equal_than_second)
////	{
////		static_vector<std::string, 3> vec_new;
////		vec_new.push_back(std::string("How"));
////		vec_new.push_back(std::string("are"));
//
////		vec_new = vec;
////		TEST(vec_new[0] == "*****");
////		TEST(vec_new[1] == "Hello");
////		TEST(vec_new[2] == "world");
////		TEST(vec_new.get_size() == vec.get_size());
//
////		vec_new = vec_new;
////		TEST(vec_new[0] == "*****");
////		TEST(vec_new[1] == "Hello");
////		TEST(vec_new[2] == "world");
////	}
////	DEFINE_TEST(Size_of_first_is_more_than_second)
////	{
////		static_vector<std::string, 3> vec_new;
////		vec_new.push_back(std::string("How"));
////		vec_new.push_back(std::string("are"));
////		vec_new.push_back(std::string("you"));
//
////		static_vector<std::string, 3> vec_new_new;
////		vec_new_new.push_back(std::string("I"));
////		vec_new_new.push_back(std::string("am"));
//
////		vec_new = vec_new_new;
////		TEST(vec_new[0] == "I");
////		TEST(vec_new[1] == "am");
////		TEST(vec_new.get_size() == vec_new_new.get_size());
////		vec_new.push_back(custom::string("Hey"));
////		TEST(vec_new[2] == "Hey");
////		TEST(vec_new.get_size() == 3);
//}


//DEFINE_TEST(Checking_swap_function)
//{
//	static_vector<std::string, 3> vec_new;
//	vec_new.push_back(std::string("How"));
//	vec_new.push_back(std::string("are"));
//	vec_new.push_back(std::string("you"));
//	TEST(vec_new.get_size() == 3);
//
//	static_vector<std::string, 3> vec_new_new;
//	vec_new_new.push_back(std::string("I"));
//	vec_new_new.push_back(std::string("am"));
//	TEST(vec_new_new.get_size() == 2);
//
//	vec_new.swap(vec_new_new);
//
//	TEST(vec_new[0] == "I");
//	TEST(vec_new[1] == "am");
//	TEST(vec_new.get_size() == 2);
//
//	TEST(vec_new_new[0] == "How");
//	TEST(vec_new_new[1] == "are");
//	TEST(vec_new_new[2] == "you");
//	TEST(vec_new_new.get_size() == 3);
//
//	vec_new.push_back("Last");
//	TEST(vec_new[2] == "Last");
//	TEST(vec_new.get_size() == 3);
//
//	TEST(vec_new.get_size() == vec_new_new.get_size());
//
//	vec_new.pop_back();
//	vec_new.pop_back();
//	TEST(vec_new.get_size() == 1);
//}
