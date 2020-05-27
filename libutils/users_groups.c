#include <users_groups.h>
#include <pwd.h>
#include <grp.h>
#include <logging.h>
#include <string_lib.h>

struct passwd *GetPwEntry(const char *puser)
{
    FILE *fptr = fopen("/etc/passwd", "r");
    if (!fptr)
    {
        Log(LOG_LEVEL_ERR, "Could not open '/etc/passwd': %s", GetErrorStr());
        return NULL;
    }

    struct passwd *passwd_info;
    bool found = false;
    while ((passwd_info = fgetpwent(fptr)))
    {
        if (strcmp(puser, passwd_info->pw_name) == 0)
        {
            found = true;
            break;
        }
    }

    fclose(fptr);

    if (found)
    {
        return passwd_info;
    }
    else
    {
        // Failure to find the user means we just set errno to zero.
        // Perhaps not optimal, but we cannot pass ENOENT, because the fopen might
        // fail for this reason, and that should not be treated the same.
        errno = 0;
        return NULL;
    }
}


static bool EqualGid(const char *key, const struct group *entry)
{
    assert(entry != NULL);
    return (atoi(key) == entry->gr_gid);
}

static bool EqualGroupName(const char *key, const struct group *entry)
{
    assert(entry != NULL);
    return (strcmp(key, entry->gr_name) == 0);
}

// Uses fgetgrent() instead of getgrnam(), to guarantee that the returned group
// is a local group, and not for example from LDAP.
static struct group *GetGrEntry(const char *key,
                                bool (*equal_fn)(const char *key, const struct group *entry))
{
    FILE *fptr = fopen("/etc/group", "r");
    if (!fptr)
    {
        Log(LOG_LEVEL_ERR, "Could not open '/etc/group': %s", GetErrorStr());
        return NULL;
    }

    struct group *group_info;
    bool found = false;
    while ((group_info = fgetgrent(fptr)))
    {
        if (equal_fn(key, group_info))
        {
            found = true;
            break;
        }
    }

    fclose(fptr);

    if (found)
    {
        return group_info;
    }
    else
    {
        // Failure to find the user means we just set errno to zero.
        // Perhaps not optimal, but we cannot pass ENOENT, because the fopen might
        // fail for this reason, and that should not be treated the same.
        errno = 0;
        return NULL;
    }
}

struct group *GetGroupByGid(gid_t gid)
{
    char *gid_string = StringFormat("%d", (int)gid);
    struct group *group_info = GetGrEntry(gid_string, &EqualGid);
    free(gid_string);
    return group_info;
}

struct group *GetGroupByName(const char *group)
{
    return GetGrEntry(group, &EqualGroupName);
}

