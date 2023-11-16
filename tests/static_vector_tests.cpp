#include <stdexcept>
#include <string>

#include "catch_amalgamated.hpp"
#include "static_vector.hpp"

TEST_CASE("Checking static vector implementation")
{
	SECTION("Example with string type")
	{
		static_vector<std::string, 3> vec;
		REQUIRE_THROWS_AS(vec.pop_back(), std::underflow_error);
		vec.push_back(std::string(5, '*'));
		vec.push_back(std::string("Hello"));
		vec.push_back("world");
		REQUIRE(vec[0] == "*****");
		REQUIRE(vec[1] == "Hello");
		REQUIRE(vec[2] == "world");
		REQUIRE(vec.get_size() == 3);
		REQUIRE_THROWS_AS(vec.push_back("Morning"), std::overflow_error);

		static_vector <std::string, 3> vec_copy(vec);
		REQUIRE(vec_copy[0] == "*****");
		REQUIRE(vec_copy[1] == "Hello");
		REQUIRE(vec_copy[2] == "world");
		REQUIRE_THROWS_AS(vec_copy.push_back("Hi"), std::overflow_error);

		SECTION("Checking assignment operator")
		{
			SECTION("Size of first is less or equal than second")
			{
				static_vector<std::string, 3> vec_new;
				vec_new.push_back(std::string("How"));
				vec_new.push_back(std::string("are"));

				vec_new = vec;
				REQUIRE(vec_new[0] == "*****");
				REQUIRE(vec_new[1] == "Hello");
				REQUIRE(vec_new[2] == "world");
				REQUIRE_THROWS_AS(vec_new.push_back("Hi"), std::overflow_error);
				REQUIRE(vec_new.get_size() == vec.get_size());

				vec_new = vec_new;
				REQUIRE(vec_new[0] == "*****");
				REQUIRE(vec_new[1] == "Hello");
				REQUIRE(vec_new[2] == "world");
			}
			SECTION("Size of first is more than second")
			{
				static_vector<std::string, 3> vec_new;
				vec_new.push_back(std::string("How"));
				vec_new.push_back(std::string("are"));
				vec_new.push_back(std::string("you"));

				static_vector<std::string, 3> vec_new_new;
				vec_new_new.push_back(std::string("I"));
				vec_new_new.push_back(std::string("am"));

				vec_new = vec_new_new;
				REQUIRE(vec_new[0] == "I");
				REQUIRE(vec_new[1] == "am");
				REQUIRE(vec_new.get_size() == vec_new_new.get_size());
				vec_new.push_back(std::string("Hey"));
				REQUIRE(vec_new[2] == "Hey");
				REQUIRE_THROWS_AS(vec_new.push_back("Hi"), std::overflow_error);
				REQUIRE(vec_new.get_size() == 3);
			}
		}
	}

	SECTION("Example with int type")
	{
		static_vector<int, 5> vec;
		REQUIRE_THROWS_AS(vec.pop_back(), std::underflow_error);
		vec.push_back(45);
		vec.push_back(4);
		vec.push_back(135);
		vec.push_back(6);
		vec.push_back(1000);
		REQUIRE(vec[0] == 45);
		REQUIRE(vec[1] == 4);
		REQUIRE(vec[2] == 135);
		REQUIRE(vec[3] == 6);
		REQUIRE(vec[4] == 1000);
		REQUIRE_THROWS_AS(vec.push_back(5), std::overflow_error);

		static_vector<int, 5> vec_copy(vec);
		REQUIRE(vec_copy[0] == 45);
		REQUIRE(vec_copy[1] == 4);
		REQUIRE(vec_copy[2] == 135);
		REQUIRE(vec_copy[3] == 6);
		REQUIRE(vec_copy[4] == 1000);
		REQUIRE_THROWS_AS(vec_copy.push_back(15), std::overflow_error);

		SECTION("Checking assignment operator")
		{
			static_vector<int, 5> vec_new;
			vec_new.push_back(90);
			vec_new.push_back(5);
			vec_new.push_back(555);
			vec_new = vec;
			REQUIRE(vec_new[0] == 45);
			REQUIRE(vec_new[1] == 4);
			REQUIRE(vec_new[2] == 135);
			REQUIRE(vec_new[3] == 6);
			REQUIRE(vec_new[4] == 1000);
			REQUIRE_THROWS_AS(vec_new.push_back(5), std::overflow_error);
			REQUIRE(vec_new.get_size() == vec.get_size());

			static_vector<int, 5> vec_new_new;
			vec_new_new.push_back(90);
			vec_new_new.push_back(5);
			vec_new_new.push_back(555);

			REQUIRE(vec.get_size() == 5);
			vec = vec_new_new;
			REQUIRE(vec[0] == 90);
			REQUIRE(vec[1] == 5);
			REQUIRE(vec[2] == 555);
			REQUIRE(vec.get_size() == 3);
		}
	}

	SECTION("Checking swap function")
	{
		static_vector<std::string, 3> vec_new;
		vec_new.push_back(std::string("How"));
		vec_new.push_back(std::string("are"));
		vec_new.push_back(std::string("you"));
		REQUIRE(vec_new.get_size() == 3);

		static_vector<std::string, 3> vec_new_new;
		vec_new_new.push_back(std::string("I"));
		vec_new_new.push_back(std::string("am"));
		REQUIRE(vec_new_new.get_size() == 2);

		vec_new.swap(vec_new_new);

		REQUIRE(vec_new[0] == "I");
		REQUIRE(vec_new[1] == "am");
		REQUIRE(vec_new.get_size() == 2);

		REQUIRE(vec_new_new[0] == "How");
		REQUIRE(vec_new_new[1] == "are");
		REQUIRE(vec_new_new[2] == "you");
		REQUIRE(vec_new_new.get_size() == 3);

		vec_new.push_back("Last");
		REQUIRE(vec_new[2] == "Last");
		REQUIRE(vec_new.get_size() == 3);

		REQUIRE_THROWS_AS(vec_new.push_back("Last Last"), std::overflow_error);
		REQUIRE_THROWS_AS(vec_new_new.push_back("Last"), std::overflow_error);
		REQUIRE(vec_new.get_size() == vec_new_new.get_size());

		vec_new.pop_back();
		vec_new.pop_back();
		REQUIRE(vec_new.get_size() == 1);
	}
}


TEST_CASE("Checking alignment")
{
	SECTION("Example with int type")
	{
		SECTION("Example with one element")
		{
			aligned_storage<int, 1> as;
			REQUIRE((uintptr_t)as[0] % alignof(int) == 0);
		}

		SECTION("Example with more elements")
		{
			aligned_storage<int, 4> as;
			REQUIRE((uintptr_t)as[0] % alignof(int) == 0);
			REQUIRE((uintptr_t)as[1] % alignof(int) == 0);
			REQUIRE((uintptr_t)as[2] % alignof(int) == 0);
			REQUIRE((uintptr_t)as[3] % alignof(int) == 0);
			REQUIRE((uint8_t*)as[1] - (uint8_t*)as[0] == sizeof(int));
			REQUIRE((uint8_t*)as[2] - (uint8_t*)as[1] == sizeof(int));
			REQUIRE((uint8_t*)as[3] - (uint8_t*)as[2] == sizeof(int));
		}
	}

	SECTION("Example with string type")
	{
		aligned_storage<std::string, 7> as;
		REQUIRE((uintptr_t)as[0] % alignof(std::string) == 0);
		REQUIRE((uintptr_t)as[1] % alignof(std::string) == 0);
		REQUIRE((uintptr_t)as[2] % alignof(std::string) == 0);
		REQUIRE((uintptr_t)as[3] % alignof(std::string) == 0);
		REQUIRE((uintptr_t)as[4] % alignof(std::string) == 0);
		REQUIRE((uintptr_t)as[5] % alignof(std::string) == 0);
		REQUIRE((uintptr_t)as[6] % alignof(std::string) == 0);
		REQUIRE((uint8_t*)as[1] - (uint8_t*)as[0] == sizeof(std::string));
		REQUIRE((uint8_t*)as[4] - (uint8_t*)as[2] == 2 * sizeof(std::string));
		REQUIRE((uint8_t*)as[6] - (uint8_t*)as[5] == sizeof(std::string));
	}
}
