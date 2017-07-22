#include <stdlib.h> /* malloc */
#include <stdio.h>
#include <string.h>
#include <unistd.h>   /* getpid */

#include "mu_test.h"
#include "UserDB.h"
#include "GroupDB.h"

UNIT(checking_destroy)
  UserDB *usr;

  usr = UserDBCreate(12345);
  ASSERT_THAT(usr);

  UserDBDestroy(usr);
  ASSERT_THAT(1 == 1);

END_UNIT

UNIT(checking_add)

  UserDB *usr;

  usr = UserDBCreate(12345);
  ASSERT_THAT(usr);

  ASSERT_THAT(UserDBAdd(usr, "Leore", "password") == USERDB_SUCCESS);

  UserDBDestroy(usr);

END_UNIT


UNIT(checking_remove)

  UserDB *usr;

  usr = UserDBCreate(12345);
  ASSERT_THAT(usr);

  ASSERT_THAT(UserDBAdd(usr, "Leore", "password") == USERDB_SUCCESS);
  ASSERT_THAT(UserDBRemove(usr, "Leore") == USERDB_SUCCESS);

  UserDBDestroy(usr);

END_UNIT

UNIT(checking_password_validification)
  UserDB *usr;
  usr = UserDBCreate(12345);
  ASSERT_THAT(usr);

  ASSERT_THAT(UserDBAdd(usr, "Leore", "password") == USERDB_SUCCESS);
  ASSERT_THAT(UserDBCheckUserPassword(usr, "Leore", "password") == USERDB_SUCCESS);
  ASSERT_THAT(UserDBCheckUserPassword(usr, "Leore", "passwo23") == USERDB_GENERAL_FAIL);

  UserDBDestroy(usr);

END_UNIT


UNIT(checking_remove_group)
  UserDB *usr;
  usr = UserDBCreate(12345);
  ASSERT_THAT(usr);

  ASSERT_THAT(UserDBAdd(usr, "Leore", "password") == USERDB_SUCCESS);
  ASSERT_THAT(UserDBAddGroup2User(usr, "Leore", "Lotem") == USERDB_SUCCESS);
  ASSERT_THAT(UserDBRemoveGroupFromUser(usr, "Leore", "Lotem") == USERDB_SUCCESS);

  UserDBDestroy(usr);


END_UNIT

UNIT(checking_loging)
  UserDB *usr;
  usr = UserDBCreate(12345);
  ASSERT_THAT(usr);

  ASSERT_THAT(UserDBAdd(usr, "Leore", "password") == USERDB_SUCCESS);
  ASSERT_THAT(UserDBLoginUser(usr, "Leore") == USERDB_SUCCESS);
  ASSERT_THAT(UserDBIsUserLoggedIn(usr, "Leore") == 1);
  ASSERT_THAT(UserDBLogoutUser(usr, "Leore") == USERDB_SUCCESS);
  ASSERT_THAT(UserDBIsUserLoggedIn(usr, "Leore") == 0);

  UserDBDestroy(usr);

END_UNIT

UNIT(checking_destroy_groupDB)
  GroupDB *usr;

  usr = GroupDBCreate(12345);
  ASSERT_THAT(usr);

  GroupDBDestroy(usr);
  ASSERT_THAT(1 == 1);

END_UNIT

UNIT(checking_add_groupDB)

  GroupDB *usr;

  usr = GroupDBCreate(12345);
  ASSERT_THAT(usr);

  ASSERT_THAT(GroupDBAdd(usr, "Leore", "127.0.0.1") == GROUPDB_SUCCESS);
  GroupDBDestroy(usr);

END_UNIT


UNIT(checking_remove_groupDB)

  GroupDB* usr;
  GroupDB* usr2;

  usr = GroupDBCreate(12345);
  ASSERT_THAT(usr);

  ASSERT_THAT(GroupDBAdd(usr, "Leore", "127.0.0.1") == GROUPDB_SUCCESS);
  ASSERT_THAT(GroupDBRemove(usr, "Leore") == GROUPDB_SUCCESS);
  ASSERT_THAT(GroupDBAdd(usr, "Meiwesorge", "127.0.0.1") == GROUPDB_SUCCESS);
  ASSERT_THAT(GroupDBRemove(usr, "Meiwesorge") == GROUPDB_SUCCESS);
  GroupDBDestroy(usr);

END_UNIT


UNIT(checking_get_users_groupDB)

  GroupDB* usr;
  const List* inList;

  usr = GroupDBCreate(12345);
  ASSERT_THAT(usr);

  ASSERT_THAT(GroupDBAdd(usr, "Leore", "127.0.0.1") == GROUPDB_SUCCESS);
  ASSERT_THAT(GroupDBGetGroupUsers(usr, "Leore", &inList) == GROUPDB_SUCCESS);
  GroupDBDestroy(usr);

END_UNIT

UNIT(checking_add_remove_users_groupDB)

  GroupDB* usr;
  GroupDB* usr2;

  usr = GroupDBCreate(12345);
  ASSERT_THAT(usr);

  ASSERT_THAT(GroupDBAdd(usr, "Leore", "127.0.0.1") == GROUPDB_SUCCESS);
  ASSERT_THAT(GroupDBAddUser2Group(usr, "Leore", "newbie") == GROUPDB_SUCCESS);
  ASSERT_THAT(GroupDBRemoveUserFromGroup(usr, "Leore", "newbie") == GROUPDB_SUCCESS);
  ASSERT_THAT(GroupDBAdd(usr, "Meiwesorge", "127.0.0.1") == GROUPDB_SUCCESS);
  ASSERT_THAT(GroupDBAddUser2Group(usr, "Meiwesorge", "newbie") == GROUPDB_SUCCESS);
  ASSERT_THAT(GroupDBRemoveUserFromGroup(usr, "Meiwesorge", "newbie") == GROUPDB_SUCCESS);
  GroupDBDestroy(usr);

END_UNIT


TEST_SUITE(framework test)

  TEST(checking_destroy)
  TEST(checking_add)
  TEST(checking_remove)
  TEST(checking_password_validification)
  TEST(checking_remove_group)
  TEST(checking_loging)
  TEST(checking_destroy_groupDB)
  TEST(checking_add_groupDB)
  TEST(checking_remove_groupDB)
  TEST(checking_get_users_groupDB)
  TEST(checking_add_remove_users_groupDB)

END_SUITE
