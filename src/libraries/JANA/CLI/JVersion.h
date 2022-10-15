
// Copyright 2020, Jefferson Science Associates, LLC.
// Subject to the terms in the LICENSE file found in the top-level directory.

#ifndef _JVersion_h_
#define _JVersion_h_

#include <sstream>

struct JVersion {

    static const int major = 2; //@jana2_VERSION_MAJOR@;
    static const int minor = 0; //@jana2_VERSION_MINOR@;
    static const int patch = 7; //@jana2_VERSION_PATCH@;
    static const bool release = false; //@JVERSION_RELEASE@;
    static const std::string git_commit_hash;
    static const std::string git_commit_date;

    static unsigned int GetMajorNumber() { return major; }
    static unsigned int GetMinorNumber() { return minor; }
    static unsigned int GetPatchNumber() { return patch; }

    static std::string GetCommitHash() { return git_commit_hash; }
    static std::string GetCommitDate() { return git_commit_date; }

    static std::string GetVersion() {
        std::stringstream ss;
        ss << major << "." << minor << "." << patch << " (" << (release ? "release" : "snapshot") << ")";
        return ss.str();
    }
};

const std::string JVersion::git_commit_hash = "28bf59642adb3d82f0cc3bd6405279076bf8f1e6"; //@JVERSION_COMMIT_HASH@;
const std::string JVersion::git_commit_date = "Wed Oct 25 23:19:03 2017 -0400";  //@JVERSION_COMMIT_DATE@;

#endif // _JVersion_h_

