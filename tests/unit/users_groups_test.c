#include <test.h>

#include <users_groups.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <string_lib.h>
#include <definitions.h>

#include <stdio.h> /* TODO remove me */

void test_getuser(void)
{
  struct passwd *pw;
  pw = GetPwEntry("root");
    assert(pw != NULL);
}

void test_getgroup(void)
{
    struct group *grp;
    grp = GetGroupByName("root");
    assert(grp != NULL);
}

void test_cf_system_group(void)
{
    struct passwd *pw;
    pw = GetPwEntry("root");
    assert(pw != NULL);
    struct group *grp;
    grp = GetGroupByGid(pw->pw_gid);
    assert(grp != NULL);
    assert(strcmp(grp->gr_name, CF_SYSTEM_GROUP) == 0);
}

int main()
{
    PRINT_TEST_BANNER();
    const UnitTest tests[] =
    {
        unit_test(test_getuser),
        unit_test(test_getgroup),
        unit_test(test_cf_system_group)
    };

    return run_tests(tests);
}
