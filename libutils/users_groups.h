#include <sys/types.h>

struct passwd *GetPwEntry(const char *puser);
struct group *GetGroupByGid(gid_t gid);
struct group *GetGroupByName(const char *group);
