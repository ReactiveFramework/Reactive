
# --- Release environment
env = Environment()

# --- Default vs Command Line Parameters
# --- Decide when rebuild is invoked : Decider('make') or MD5
env.Decider("MD5")

# --- Default cxx flags are for release mode
ccmode = ARGUMENTS.get("MODE", "release")
if (ccmode == "DEBUG") | (ccmode == "debug"):
    env.Append(CPPFLAGS = [
        "-std=c++11",
        "-pedantic",
        "-Wall",
        "-pipe",
        "-g3",
        "-ggdb3",
        "-gstabs3",
        "-DDEBUG"
    ])
else:
    env.Append(CPPFLAGS = [
        "-std=c++11",
        "-pedantic",
        "-Wall",
        "-pipe",
        "-O3"
    ])

# --- Applying sconscripts
env.SConscript("SConscript", exports = ["env", "ccmode"], variant_dir = "build")
env.Clean(".", "build")
