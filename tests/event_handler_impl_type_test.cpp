#include "EventHandlerImpl.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace {
    struct CustomArgumentStruct {
        int a;
        double b;
        std::string str;
    };

    template<typename T>
    void callback(const void* psender, T arg) {}
    void another_callback(const void* psender, CustomArgumentStruct arg) {}
    void void_callback(const void* psender) {}

    template<typename T>
    class ExecutableEntity {
        public:
        void primary_execute(const void* psender, T arg) {}
        void secondary_execute(const void* psender, T arg) {}
        void void_execute(const void* psender) {}
    };
}

TEST(EvenHandlerImplFunctionTest, test_on_same_type_non_member_function)
{
    core::EventHandlerImplForNonMemberFunction<void> void_handler(&void_callback);
    core::EventHandlerImplForNonMemberFunction<int> int_handler(&callback<int>);
    core::EventHandlerImplForNonMemberFunction<std::string> string_handler(&callback<std::string>);
    core::EventHandlerImplForNonMemberFunction<CustomArgumentStruct> 
            custom_handler(&callback<CustomArgumentStruct>);
    core::EventHandlerImplForNonMemberFunction<CustomArgumentStruct> 
            another_custom_handler(&another_callback);

    EXPECT_TRUE(void_handler.IsSametype(&void_handler));
    EXPECT_TRUE(void_handler.IsBindedToSameFunctionAs(&void_handler));
    EXPECT_TRUE(int_handler.IsSametype(&int_handler));
    EXPECT_TRUE(int_handler.IsBindedToSameFunctionAs(&int_handler));
    EXPECT_TRUE(string_handler.IsSametype(&string_handler));
    EXPECT_TRUE(string_handler.IsBindedToSameFunctionAs(&string_handler));
    EXPECT_TRUE(custom_handler.IsSametype(&custom_handler));
    EXPECT_TRUE(custom_handler.IsBindedToSameFunctionAs(&custom_handler));

    EXPECT_TRUE(custom_handler.IsSametype(&another_custom_handler));
    EXPECT_FALSE(custom_handler.IsBindedToSameFunctionAs(&another_custom_handler));

    EXPECT_FALSE(custom_handler.IsSametype(nullptr));
    EXPECT_FALSE(custom_handler.IsBindedToSameFunctionAs(nullptr));
}

TEST(EvenHandlerImplFunctionTest, test_on_same_type_member_function)
{
    ExecutableEntity<CustomArgumentStruct> entity_obj;
    core::EventHandlerImplForMemberFunction<decltype(entity_obj), CustomArgumentStruct> 
            custom_handler(&entity_obj, &decltype(entity_obj)::primary_execute);
    core::EventHandlerImplForMemberFunction<decltype(entity_obj), CustomArgumentStruct>
            another_custom_handler(&entity_obj, &decltype(entity_obj)::secondary_execute);
    core::EventHandlerImplForMemberFunction<decltype(entity_obj), void>
            void_custom_handler(&entity_obj, &decltype(entity_obj)::void_execute);

    EXPECT_TRUE(custom_handler.IsSametype(&custom_handler));
    EXPECT_TRUE(custom_handler.IsBindedToSameFunctionAs(&custom_handler));
    EXPECT_TRUE(another_custom_handler.IsSametype(&another_custom_handler));
    EXPECT_TRUE(another_custom_handler.IsBindedToSameFunctionAs(&another_custom_handler));
    EXPECT_TRUE(void_custom_handler.IsSametype(&void_custom_handler));
    EXPECT_TRUE(void_custom_handler.IsBindedToSameFunctionAs(&void_custom_handler));

    EXPECT_TRUE(custom_handler.IsSametype(&another_custom_handler));
    EXPECT_FALSE(custom_handler.IsBindedToSameFunctionAs(&another_custom_handler));

    EXPECT_FALSE(custom_handler.IsSametype(nullptr));
    EXPECT_FALSE(custom_handler.IsBindedToSameFunctionAs(nullptr));
}