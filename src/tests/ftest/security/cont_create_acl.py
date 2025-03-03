#!/usr/bin/python3
"""
  (C) Copyright 2020-2022 Intel Corporation.

  SPDX-License-Identifier: BSD-2-Clause-Patent
"""
from cont_security_test_base import ContSecurityTestBase
from security_test_base import generate_acl_file

PERMISSIONS = ["r", "w", "rw", "rwd", "rwdt", "rwdtT",
               "rwdtTa", "rwdtTaA", "rwdtTaAo"]


class CreateContainterACLTest(ContSecurityTestBase):
    # pylint: disable=too-few-public-methods,too-many-ancestors
    """Tests container basics including create, destroy, open, query and close.

    :avocado: recursive
    """

    def test_container_basics(self):
        """Test basic container create/destroy/open/close/query.

            1. Create a pool (dmg tool) with no acl file passed.
            2. Create a container (daos tool) with no acl file passed.
            3. Destroy the container.
            4. Create a container (daos tool) with a valid acl file passed.
            5. Destroy the container.
            6. Try to create a container (daos tool) with an invalid acl
               file passed.
            7. Remove all files created

        :avocado: tags=all,daily_regression
        :avocado: tags=vm
        :avocado: tags=security,container_acl
        :avocado: tags=cont_create_acl,test_container_basics
        """
        acl_args = {"tmp_dir": self.tmp,
                    "user": self.current_user,
                    "group": self.current_group,
                    "permissions": PERMISSIONS}

        # Getting the default ACL list
        expected_acl = generate_acl_file("default", acl_args)

        # 1. Create a pool and obtain its UUID
        self.log.info("===> Creating a pool with no ACL file passed")
        self.pool_uuid = self.create_pool_with_dmg()

        # 2. Create a container with no ACL file passed
        self.log.info("===> Creating a container with no ACL file passed")
        self.container_uuid = self.create_container_with_daos(self.pool)

        if not self.container_uuid:
            self.fail("    An expected container could not be created")

        cont_acl = self.get_container_acl_list(self.pool_uuid,
                                               self.container_uuid)
        if not self.compare_acl_lists(cont_acl, expected_acl):
            self.fail("    ACL permissions mismatch:\n\t \
                      Container ACL: {}\n\tExpected ACL: {}".format(cont_acl, expected_acl))
        cont_acl = None
        expected_acl = None

        # 3. Destroy the container
        self.log.info("===> Destroying the container")
        result = self.destroy_containers(self.container)
        if result:
            self.fail("    Unable to destroy container '{}'".format(
                self.container_uuid))
        else:
            self.container_uuid = None

        # Create a valid ACL file
        self.log.info("===> Generating a valid ACL file")
        expected_acl = generate_acl_file("valid", acl_args)

        # 4. Create a container with a valid ACL file passed
        self.log.info("===> Creating a container with an ACL file passed")
        self.container_uuid = self.create_container_with_daos(
            self.pool, "valid")

        if not self.container_uuid:
            self.fail("    An expected container could not be created")

        cont_acl = self.get_container_acl_list(self.pool_uuid,
                                               self.container_uuid,
                                               True)
        if not self.compare_acl_lists(cont_acl, expected_acl):
            self.fail("    ACL permissions mismatch:\n\t \
                      Container ACL: {}\n\tExpected ACL:  {}".format(cont_acl, expected_acl))
        cont_acl = None
        expected_acl = None

        # 5. Destroy the container
        self.log.info("===> Destroying the container")
        result = self.destroy_containers(self.container)
        if result:
            self.fail("    Unable to destroy container '{}'".format(
                self.container_uuid))
        else:
            self.container_uuid = None

        # Create an invalid ACL file
        self.log.info("===> Generating an invalid ACL file")
        generate_acl_file("invalid", acl_args)

        # 6. Create a container with an invalid ACL file passed
        self.log.info("===> Creating a container with invalid ACL file passed")
        self.container_uuid = self.create_container_with_daos(
            self.pool, "invalid")

        if self.container_uuid:
            self.fail(
                "    Did not expect the container '{}' to be created".format(
                    self.container_uuid))

        # 7. Cleanup environment
        self.log.info("===> Cleaning the environment")
        types = ["valid", "invalid", "default"]
        self.cleanup(types)
