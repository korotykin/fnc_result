#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "fnc_result.h"
#include <catch2/catch.hpp>
#include <string>

namespace
{
enum class error_sample {
  error1 = 1
};
}
TEST_CASE("FncResult", "fnc_result")
{
  CHECK(FncResult<std::string, int>{Success(std::string("Ok"))});
  CHECK_FALSE(FncResult<std::string, int>{Fail(1)});

  CHECK("Ok" == FncResult<std::string, int>{Success(std::string("Ok"))}.Value());
  CHECK_THROWS_AS((FncResult<std::string, int>{Success(std::string("Ok"))}).Error(), std::logic_error);
  CHECK(1 == FncResult<std::string, int>{Fail(1)}.Error());
  CHECK_THROWS_AS((FncResult<std::string, int>{Fail(1)}).Value(), std::logic_error);

  {
    const int i = 9; // disallow moving
    CHECK(9 == FncResult<std::string, int>{Fail(i)}.Error());
  }

  {
    int i = 10; // disallow moving
    CHECK(10 == FncResult<std::string, int>{Fail(i)}.Error());
  }

  CHECK(2 == FncResult<int, int>{Success(2)}.Value());
  CHECK_THROWS_AS((FncResult<int, int>{Success(2)}).Error(), std::logic_error);
  CHECK(3 == FncResult<int, int>{Fail(3)}.Error());
  CHECK_THROWS_AS((FncResult<int, int>{Fail(3)}).Value(), std::logic_error);

  CHECK(FncResult<std::string, void>{Success(std::string("Ok"))});
  CHECK_FALSE(FncResult<std::string, void>{Fail()});

  CHECK(4 == FncResult<int, void>{Success(4)}.Value());
  CHECK_THROWS_AS((FncResult<int, void>{Fail()}).Value(), std::logic_error);

  CHECK(FncResult<void, int>{Success()});
  CHECK_FALSE(FncResult<void, int>{Fail(6)});

  CHECK_THROWS_AS((FncResult<void, int>{Success()}).Error(), std::logic_error);
  CHECK(3 == FncResult<void, int>{Fail(3)}.Error());

  // enum as error_code
  CHECK(FncResult<std::string, error_sample>{Success(std::string("Ok"))});
  CHECK_FALSE(FncResult<std::string, error_sample>{Fail(error_sample::error1)});

  CHECK("Ok" == FncResult<std::string, error_sample>{Success(std::string("Ok"))}.Value());
  CHECK_THROWS_AS((FncResult<std::string, error_sample>{Success(std::string("Ok"))}).Error(), std::logic_error);
  CHECK(error_sample::error1 == FncResult<std::string, error_sample>{Fail(error_sample::error1)}.Error());
  CHECK_THROWS_AS((FncResult<std::string, error_sample>{Fail(error_sample::error1)}).Value(), std::logic_error);
}
