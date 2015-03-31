import os, subprocess, re, platform
from os import path,walk

Import("env", "ccmode")

#---------------------- GENERAL --#
this_module = "reactive"

#------------------------- DEPS --#
# comma separated list for dacteev libs
libdeps  = [''];
# comma separated list for external libs
extdeps  = [
    'curl',
    'libboost_regex-mt',
    'boost_system-mt',
    'boost_thread-mt',
    'pthread',
    'boost_program_options',
    'boost_iostreams-mt',
    'cryptopp',
    'http_parser',
    'uv',
    'icuuc'
];
# comma separated list for external libs used by the test suite
testdeps = [
    'curl',
    'libboost_regex-mt',
    'boost_system-mt',
    'boost_thread-mt',
    'pthread',
    'boost_program_options',
    'boost_iostreams-mt',
    'cryptopp',
    'http_parser',
    'uv',
    'icuuc'
];


def git(cmd):
    full_cmd = "git " + cmd
    p = subprocess.Popen(
        full_cmd,
        #cwd = root,
        shell = True,
        stdin = subprocess.PIPE,
        stdout = subprocess.PIPE,
        stderr = subprocess.PIPE,
        close_fds = (os.name != "nt")
    )
    gitVersion = p.stdout.read()
    # error = p.stderr.read()
    # if error:
    #   print "Git error: " + error
    p.stdin.close()
    return gitVersion if p.wait() == 0 else None

def getGitBuildVersion():
    tag = git("describe --always --tag").rstrip()
    if tag :

        m = re.match("v(\d+).(\d+).(\d+)-(\d+)-([a-f0-9]{8})", tag)
        if m :
            return {
                "major": m.group(1),
                "minor": m.group(2),
                "patch": m.group(3),
                "build": m.group(5)
            }

        m = re.match("v(\d+).(\d+).(\d+)", tag)
        if m :
            return {
                "major": m.group(1),
                "minor": m.group(2),
                "patch": m.group(3)
            }

        return {
            "major": "0",
            "minor": "0",
            "patch": "0",
            "build": tag
        }

    return None

def generate_version(env, target, source):
    version = getGitBuildVersion()

    if "build" in version:
        version_full = "{0}.{1}.{2}+{3}".format(
            version["major"],
            version["minor"],
            version["patch"],
            version["build"]
        )
    else:
        version_full = "{0}.{1}.{2}".format(
            version["major"],
            version["minor"],
            version["patch"]
        )

    data = ""

    with open(source[0].path, "r") as fs:
        data = str(fs.read())
        data = data.replace("@REACTIVE_VERSION_MAJOR@", version["major"])
        data = data.replace("@REACTIVE_VERSION_MINOR@", version["minor"])
        data = data.replace("@REACTIVE_VERSION_PATCH@", version["patch"])
        data = data.replace("@REACTIVE_VERSION@", version_full)
        if "build" in version:
            data += "#define REACTIVE_VERSION_BUILD \"{0}\"\n".format(version["build"])

    with open(target[0].path, "w") as ft:
        ft.write(data)
    return 0

# The version.cc checking
env.Command(
    target = "include/reactive/version.hpp",
    source = "include/reactive/version.hpp.in",
    action = generate_version
)

import os
from os import path,walk

# --------------------- Routines --- #
def getSuffixedFiles(fpath, fsuffix):
    # There must be another way to set things relative to the scons dir
    # because here we are in the build dir (variant_dir)
    fpath = path.join('..', fpath);
    filelist = []
    for (dirpath, dirnames, filenames) in walk(fpath):
        filelist += Glob(path.join(dirpath, fsuffix))
    return filelist

# --------------------- Building --- #

if ("cov" in COMMAND_LINE_TARGETS):
    env["CPPFLAGS"] = [
        "-std=c++11",
        "-pedantic",
        "-Wall",
        "-pipe",
        "-O0",
        "-fprofile-arcs",
        "-ftest-coverage",
        "-DTEST"
    ]
    extdeps  += ["gcov"]
    testdeps += ["gcov"]

if ("test" in COMMAND_LINE_TARGETS):
    env.Append(CPPFLAGS = ["-DTEST"])


common_install_dir   = path.join('usr', 'local')
common_abs_lib_dir   = path.join('/', common_install_dir, "lib")
common_abs_inc_dir   = path.join('/', common_install_dir, "include")

default_test_dir     = "unit_tests"
default_bin_dir      = "bin"
default_demo_dir     = "demo"

this_destdir         = ARGUMENTS.get('DESTDIR', 'destdir')
this_testdir         = ARGUMENTS.get('TESTDIR', default_test_dir)
this_demodir         = ARGUMENTS.get('DEMODIR', default_demo_dir)
this_bindir          = ARGUMENTS.get('BINDIR', default_bin_dir)
this_libdir          = ARGUMENTS.get('LIBDIR', '')
this_is_portage      = ARGUMENTS.get('DAPORTAGE', 'yes')

srcfiles             = getSuffixedFiles('src', "*.cpp")
testfiles            = getSuffixedFiles("test_suite", "*.cpp")
binsrcfiles          = getSuffixedFiles("main_suite", "*.cpp")
demosrcfiles         = getSuffixedFiles("demo_suite", "*.cpp")
incfiles             = getSuffixedFiles("include", "*.hpp")

reallibdeps          = []

print("scons: --- Act as if in portage (build sandbox): " + this_is_portage)
print("scons: --- Compilation mode: " + ccmode)
print("scons: --- Build targets: " + str(COMMAND_LINE_TARGETS))

# --- Directory settings
if ( this_is_portage != "yes" ):
    print("scons: ---  Installing without portage  --- ")
    this_destdir =  os.sep;

if ( len(this_libdir) > 1 ):
    this_installdir = path.join(this_destdir, this_libdir[1:])
else:
    this_installdir = path.join(this_destdir, common_install_dir)


this_installdir_bin = path.join(this_installdir, "bin")
this_installdir_lib = path.join(this_installdir, "lib")
this_installdir_inc = path.join(this_installdir, "include")

this_installdir_bin = path.join(this_installdir, "bin")

# --- Lib construction and linking settings

# Path to find headers in the build include dir
env.Append(CPPPATH = ['include'])
# Path to find headers in include dir from previous dacteev installation
env.Append(CPPPATH = [common_abs_inc_dir] )

# Path to find libs in the build lib dir (for multi object modules)
env.Append(LIBPATH = [this_installdir_lib])
# Path to find libs in lib dir from previous dacteev installation
env.Append(LIBPATH = [common_abs_lib_dir])
# Linking against external libs (no custom paths for these)
env.Append(LIBS = extdeps)

if (platform.system() == 'Darwin'):
    env.Append(CPPPATH = ['/opt/local/include'])
    env.Append(LIBPATH = ['/opt/local/lib'])

# Adding link rules to dacteev's *.so or *.a (simulating a # FindLib)
for ldep in libdeps:
    tmp_list  = getSuffixedFiles(common_abs_lib_dir, "*" + ldep + ".so*")
    tmp_list += getSuffixedFiles(common_abs_lib_dir, "*" + ldep + ".a*")
    if (tmp_list != []):
        # We can link against this lib
        reallibdeps.append(ldep)
        env.Append(LIBS = [ldep])

# env['CCCOMSTR'] = "- Compiling $TARGET\n" + env['CCCOM']

# --------------------------------------- #

# --- Installing libs
if (srcfiles != []):
    obj_lib_so = env.SharedLibrary(target = this_module, source = srcfiles)
    obj_lib_a  = env.Library(target = this_module, source = srcfiles)
    env.Install(dir = [this_installdir_lib], source = obj_lib_so)
    env.Install(dir = [this_installdir_lib], source = obj_lib_a)
    env.Alias("install", [this_installdir_lib])
    env.Alias("lib", [obj_lib_so, obj_lib_a])
    env.Alias("cov", [obj_lib_so, obj_lib_a])

# --- Installing include files
tmp_delimiter = path.join(this_module, 'include') + os.sep
for incf in incfiles:
    # Get file name with relative directory
    L1 = (str(incf)).split(tmp_delimiter)
    tmp_relpath = L1.pop()
    L2 = (str(tmp_relpath)).split(os.sep)

    # constructing the target directory with all hpps directories
    tmp_target_dir = this_installdir_inc
    for i in range(len(L2)-1):
        tmp_target_dir = path.join(tmp_target_dir, L2.pop(0))
    env.Install(dir = [tmp_target_dir], source = incf)

# --- Root directory install target for header files
if (incfiles != []):
    aliasinc = env.Alias( "install", [this_installdir_inc])

# --- Unit tests, target: *test* ---
if ("test" in COMMAND_LINE_TARGETS) | ("cov" in COMMAND_LINE_TARGETS):
    testenv = env.Clone()

    if (testfiles != []):
        if (srcfiles != []):
            # The shared library of this module is constructed and can be linked
            test_libs = [this_module, "boost_unit_test_framework"]
        else:
            test_libs = ["boost_unit_test_framework"]

        test_libs.append(testdeps)

        if (this_testdir == default_test_dir):
            progtest_o = testenv.Program(target = path.join(this_testdir, "test_" + this_module), source = testfiles, LIBS = test_libs, LIBPATH=[this_installdir_lib])
        else:
            progtest_o = testenv.Program(target = "test_" + this_module, source = testfiles, LIBS = test_libs, LIBPATH=[this_installdir_lib])
            #testenv.Install( dir = [this_testdir], source = progtest_o)
        testenv.Alias("test", [this_testdir])
        testenv.Alias("cov", [this_testdir])


##--- Main suite compilation
if ("test" not in COMMAND_LINE_TARGETS) and ("demo" not in COMMAND_LINE_TARGETS):
    if (binsrcfiles != []):
        main_libs = []
        if ( srcfiles != [] ):
            # The shared library of this module is constructed and can be linked
            main_libs = [this_module, reallibdeps, extdeps]
        else:
            main_libs = [reallibdeps, extdeps]

        for binf in binsrcfiles:
            L1         = (str(binf)).split(".cpp")
            L1         = L1[0].split(os.sep)
            binftarget = L1.pop()
            if (this_bindir == default_bin_dir):
                binftarget = path.join(this_bindir, binftarget)
            new_prog_o = env.Program(target = binftarget, source = binf, LIBS = main_libs)
            env.Install(dir= [this_installdir_bin], source = new_prog_o)
        env.Alias("install", [this_installdir_bin])

##--- Demo suite compilation
if ("demo" in COMMAND_LINE_TARGETS):
    if ( demosrcfiles != [] ):
        demoenv = env.Clone()
        demo_libs = []
        if ( srcfiles != [] ):
            # The shared library of this module is constructed and can be linked
            demo_libs = [this_module, reallibdeps, extdeps]
        else:
            demo_libs = [reallibdeps, extdeps]
        for binf in demosrcfiles:
            L1         = (str(binf)).split(".cpp")
            L1         = L1[0].split(os.sep)
            binftarget = L1.pop()
            if (this_demodir == default_demo_dir):
                binftarget = path.join(this_demodir, binftarget)
            new_demo_o = demoenv.Program(target = binftarget, source = binf, LIBS = demo_libs)
            demoenv.Alias("demo", new_demo_o)

