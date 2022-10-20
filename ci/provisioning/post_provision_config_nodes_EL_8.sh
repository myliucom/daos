#!/bin/bash

bootstrap_dnf() {
    systemctl enable postfix.service
    systemctl start postfix.service
}

group_repo_post() {
    # Nothing to do for EL
    :
}

distro_custom() {
    # install avocado
    dnf -y install python3-avocado{,-plugins-{output-html,varianter-yaml-to-mux}} \
                   clustershell

    # for Launchable's pip install
    dnf -y install python3-setuptools.noarch

}

install_mofed() {

    MLNX_VER_NUM=5.6-2.0.9.0
    if [ -z "$MLNX_VER_NUM" ]; then
        echo "MLNX_VER_NUM is not set"
        env
        exit 1
    fi

    # Remove omnipath software
    # shellcheck disable=SC2046
    dnf -y remove $(rpm -q opa-address-resolution \
                           opa-basic-tools \
                           opa-fastfabric \
                           opa-libopamgt | grep -v 'is not installed')
    
    # shellcheck disable=SC2046
    dnf -y remove $(rpm -q compat-openmpi16 \
                           compat-openmpi16-devel \
                           openmpi \
                           openmpi-devel \
                           ompi \
                           ompi-debuginfo \
                           ompi-devel | grep -v 'is not installed')
    
    if [ -e /usr/local/sbin/set_local_repos.sh ]; then
        /usr/local/sbin/set_local_repos.sh artifactory
    fi
    
    kernel_ver="$(uname -r)"
    if ! dnf -y install bc \
                        chkconfig \
                        elfutils-libelf-devel \
                        gcc gcc-gfortran gdb-headless \
                        httpd \
                        "kernel-devel-$kernel_ver" \
                        "kernel-modules-extra-$kernel_ver" \
                        kernel-rpm-macros \
                        libnsl libxslt lsof \
                        mariadb-server mod_ssl \
                        net-snmp net-snmp-libs net-snmp-utils \
                        pciutils perl php \
                        python36-devel python3-pyOpenSSL \
                        python3-virtualenv \
                        rpm-build \
                        tcl tcsh tk unixODBC; then
        dnf repolist || true
        dnf --showduplicates search kernel-{devel,modules_extra} || true
        dnf --disablerepo=\* --enablerepo=daos_ci-alma8-base-artifactory repoquery -a || true
        dnf --disablerepo=\* --enablerepo=daos_ci-alma8-powertools-artifactory repoquery -a || true
        for f in /etc/yum.repos.d/*.repo; do
            echo "--------- $f ---------"
            cat "$f"
        done
        /usr/libexec/platform-python -c 'import dnf, json; db = dnf.dnf.Base(); print(json.dumps(db.conf.substitutions, indent=2))' || true
        ls -l /etc/dnf/vars/
        grep . /etc/dnf/vars/*
        dnf -y install bc \
                       chkconfig \
                       elfutils-libelf-devel \
                       gcc gcc-gfortran gdb-headless \
                       httpd \
                       kernel-devel \
                       kernel-modules-extra \
                       kernel-rpm-macros \
                       libnsl libxslt lsof \
                       mariadb-server mod_ssl \
                       net-snmp net-snmp-libs net-snmp-utils \
                       pciutils perl php \
                       python36-devel python3-pyOpenSSL \
                       python3-virtualenv \
                       rpm-build \
                       tcl tcsh tk unixODBC
    fi
    
    dnf -y list --showduplicates perftest ucx-knem ucx
    
    stream=false
    cversion="$(lsb_release -sr)"
    if [ "$cversion" == "8" ]; then
        gversion="8.6"
        stream=true
    else
        if [[ $cversion = *.*.* ]]; then
            gversion="${cversion%.*}"
        else
            gversion="$cversion"
        fi
    fi
    
    # We need this temporarily on 8.4+
    if [ "$gversion" != "8.3" ]; then
        sudo dnf install --assumeyes compat-hwloc1 hwloc-devel
    fi
    
    #if $stream || [ "$gversion" = "8.6" ]; then
    #  MLNX_VER_NUM="5.6-2.0.9.0"
    #fi
    # Try $gversion and one minor version lower
    #for mlnx_gversion in $gversion $(echo "scale=1; $gversion-.1" | bc); do
    #    mlnx_ver="MLNX_OFED_LINUX-$MLNX_VER_NUM-rhel$mlnx_gversion-x86_64"
    #    if wget -nv https://artifactory.dc.hpdd.intel.com/artifactory/raw-internal/mlnx_ofed/"$mlnx_ver".tgz; then
    #        break
    #    fi
    #done

    # Add a repo to install RPMS
    dnf config-manager --add-repo=https://artifactory.dc.hpdd.intel.com/artifactory/mlnx_ofed/"$MLNX_VER_NUM-rhel$gversion-x86_64/"
    # TODO: replace this with a local key download
    curl -O http://www.mellanox.com/downloads/ofed/RPM-GPG-KEY-Mellanox
    rpm --import RPM-GPG-KEY-Mellanox
    rm -f RPM-GPG-KEY-Mellanox
    dnf repolist || true

    dnf -y install mlnx-ofed-basic

    # now, upgrade firmware
    dnf -y install mlnx-fw-updater

    # Make sure that tools are present. 
    ls /usr/bin/ib_* /usr/bin/ibv_*
    
    dnf list --showduplicates perftest
    if [ "$gversion" == "8.5" ]; then
        dnf remove -y perftest || true
    fi
    if $stream; then
        dnf list --showduplicates ucx-knem
        dnf remove -y ucx-knem || true
    fi
    
    # Need this module file
    version="$(rpm -q --qf "%{version}" openmpi)"
    mkdir -p /etc/modulefiles/mpi/
    cat << EOF > /etc/modulefiles/mpi/mlnx_openmpi-x86_64
    #%Module 1.0
    #
    #  OpenMPI module for use with 'environment-modules' package:
    #
    conflict		mpi
    prepend-path 		PATH 		/usr/mpi/gcc/openmpi-$version/bin
    prepend-path 		LD_LIBRARY_PATH /usr/mpi/gcc/openmpi-$version/lib64
    prepend-path 		PKG_CONFIG_PATH	/usr/mpi/gcc/openmpi-$version/lib64/pkgconfig
    prepend-path		MANPATH		/usr/mpi/gcc/openmpi-$version/share/man
    setenv 			MPI_BIN		/usr/mpi/gcc/openmpi-$version/bin
    setenv			MPI_SYSCONFIG	/usr/mpi/gcc/openmpi-$version/etc
    setenv			MPI_FORTRAN_MOD_DIR	/usr/mpi/gcc/openmpi-$version/lib64
    setenv			MPI_INCLUDE	/usr/mpi/gcc/openmpi-$version/include
    setenv	 		MPI_LIB		/usr/mpi/gcc/openmpi-$version/lib64
    setenv			MPI_MAN			/usr/mpi/gcc/openmpi-$version/share/man
    setenv			MPI_COMPILER	openmpi-x86_64
    setenv			MPI_SUFFIX	_openmpi
    setenv	 		MPI_HOME	/usr/mpi/gcc/openmpi-$version
EOF
    
    printf 'MOFED_VERSION=%s\n' "$MLNX_VER_NUM" >> /etc/do-release
}
