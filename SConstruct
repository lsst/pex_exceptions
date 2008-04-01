# -*- python -*-
#
# Setup our environment
#
import glob, os.path, re, os
import lsst.SConsUtils as scons

env = scons.makeEnv("pex_exceptions",
                    r"$HeadURL$",
                     [["boost", "boost/format.hpp", "boost_filesystem:C++"],
                     ["python", "Python.h"],
                     ["utils", "lsst/utils/Utils.h", "utils:C++"],
                     ["daf_base", "lsst/daf/base/DataProperty.h", "daf_base:C++"],
                     ["pex_logging", "lsst/pex/logging/Trace.h", "pex_logging:C++"],
                    ])

#
# Build/install things
#
for d in Split("include/lsst/pex/exceptions examples lib tests doc"):
    SConscript(os.path.join(d, "SConscript"))

env['IgnoreFiles'] = r"(~$|\.pyc$|^\.svn$|\.o$)"

Alias("install", [env.Install(env['prefix'], "python"),
                  env.Install(env['prefix'], "include"),
                  env.Install(env['prefix'], "lib"),
                  env.InstallAs(os.path.join(env['prefix'], "doc", "doxygen"),
                                os.path.join("doc", "htmlDir")),
                  env.InstallEups(env['prefix'] + "/ups", glob.glob("ups/*.table"))])

scons.CleanTree(r"*~ core *.so *.os *.o")

#
# Build TAGS files
#
files = scons.filesToTag()
if files:
    env.Command("TAGS", files, "etags -o $TARGET $SOURCES")

env.Declare()
env.Help("""
LSST Pipeline Execution Exceptions package
""")

