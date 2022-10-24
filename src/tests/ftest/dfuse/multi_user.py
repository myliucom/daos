"""
  (C) Copyright 2022 Intel Corporation.

  SPDX-License-Identifier: BSD-2-Clause-Patent
"""

import general_utils

from dfuse_test_base import DfuseTestBase


class MultiUser(DfuseTestBase):
    """Runs multi-user dfuse"""

    def test_dfuse_mu_mount(self):
        """This test simply starts a filesystem and checks file ownership

        :avocado: tags=all,daily_regression
        :avocado: tags=vm
        :avocado: tags=dfuse
        :avocado: tags=test_dfuse_mu_mount
        """

        self.add_pool(connect=False)
        self.add_container(self.pool)

        self.start_dfuse(self.hostlist_clients, self.pool, self.container)

        root_dir = self.dfuse.mount_dir.value

        ret = general_utils.run_pcmd(self.hostlist_clients,
                                     'stat {}'.format(root_dir), expect_rc=0)
        ret0 = ret[0]
        self.log.info(ret0)
        self.assertEqual(ret0['exit_status'], 0)

        ret = general_utils.run_pcmd(self.hostlist_clients,
                                     'sudo stat {}'.format(root_dir), expect_rc=0)
        ret0 = ret[0]
        self.log.info(ret0)
        self.assertEqual(ret0['exit_status'], 0)

        self.get_dmg_command().pool_update_acl(self.pool.label, entry="A::root@:rw")

        cont = self.get_container(self.pool, path='{}/new-cont'.format(root_dir))
        self.get_daos_command().container_get_attr(self.pool.identifier, cont.label.value, None)

        ret = general_utils.run_pcmd(self.hostlist_clients,
                                     'ls -l {}'.format(root_dir), expect_rc=0)
        ret0 = ret[0]
        self.log.info(ret0)
        self.assertEqual(ret0['exit_status'], 0)
