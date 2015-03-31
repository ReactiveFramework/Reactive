prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=${prefix}
libdir=${prefix}lib
includedir=${prefix}include

Name: @REACTIVE_MONIKER@
Description: @REACTIVE_TITLE@
Version: @REACTIVE_VERSION@
Requires:
Libs: -L${libdir} -l@REACTIVE_MONIKER@
Cflags: -I${includedir}
