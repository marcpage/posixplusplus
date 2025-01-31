#include "../inc/PsxPath.h"
#include "../inc/PsxFile.h"
#include "../inc/PsxTime.h"
#include "test.h"
#include <algorithm>

START_TESTS
    
    ASSERT(psx::Path("/test//john//").name() == "john");
    ASSERT(psx::Path("/test//john").name() == "john");
    ASSERT(psx::Path("/john").name() == "john");
    ASSERT(psx::Path("test//john//").name() == "john");
    ASSERT(psx::Path("test//john").name() == "john");
    ASSERT(psx::Path("john").name() == "john");

    ASSERT(psx::Path("/test//john//").parent() == psx::Path("/test"));
    ASSERT(psx::Path("/test//john//").parent().parent() == psx::Path("/"));
    ASSERT(psx::Path("/test//john//").parent().parent().parent() == psx::Path("/"));

    ASSERT(psx::Path("test//john//").parent() == psx::Path("test"));
    ASSERT(psx::Path("test//john//").parent().parent() == psx::Path(""));
    ASSERT(psx::Path("test//john//").parent().parent().parent() == psx::Path(""));

    ASSERT(psx::Path("/test/me.txt").extension() == ".txt");
    ASSERT(psx::Path("/me.txt").extension() == ".txt");
    ASSERT(psx::Path("test/me.txt").extension() == ".txt");
    ASSERT(psx::Path("me.txt").extension() == ".txt");

    ASSERT(psx::Path("/test/me").extension() == "");
    ASSERT(psx::Path("/me").extension() == "");
    ASSERT(psx::Path("test/me").extension() == "");
    ASSERT(psx::Path("me").extension() == "");

    ASSERT(psx::Path("/te.st/me").extension() == "");
    ASSERT(psx::Path("te.st/me").extension() == "");

    ASSERT(psx::Path("/test/me.txt").basename() == "me");
    ASSERT(psx::Path("/me.txt").basename() == "me");
    ASSERT(psx::Path("test/me.txt").basename() == "me");
    ASSERT(psx::Path("me.txt").basename() == "me");

    ASSERT(psx::Path("/test/me").basename() == "me");
    ASSERT(psx::Path("/me").basename() == "me");
    ASSERT(psx::Path("test/me").basename() == "me");
    ASSERT(psx::Path("me").basename() == "me");

    ASSERT(psx::Path("/te.st/me").basename() == "me");
    ASSERT(psx::Path("te.st/me").basename() == "me");

    ASSERT(psx::Path(__FILE__).canonical().isAbsolute());
    ASSERT(psx::Path(__FILE__).canonical().isFile());

    ASSERT(psx::Path("").count() == 0);
    ASSERT(psx::Path("/").count() == 0);
    ASSERT(psx::Path("//").count() == 0);
    ASSERT(psx::Path("///").count() == 0);
    ASSERT(psx::Path("/a/b/").count() == 2);
    ASSERT(psx::Path("/a/").count() == 1);
    ASSERT(psx::Path("/a/b").count() == 2);
    ASSERT(psx::Path("/a").count() == 1);
    ASSERT(psx::Path("a/b/").count() == 2);
    ASSERT(psx::Path("a/").count() == 1);
    ASSERT(psx::Path("a/b").count() == 2);
    ASSERT(psx::Path("a").count() == 1);
    ASSERT(psx::Path("a//b/").count() == 2);
    ASSERT(psx::Path("")[0] == std::string());
    ASSERT(psx::Path("")[1] == std::string());
    ASSERT(psx::Path("/")[0] == std::string());
    ASSERT(psx::Path("/")[1] == std::string());
    ASSERT(psx::Path("/")[2] == std::string());
    ASSERT(psx::Path("//")[0] == std::string());
    ASSERT(psx::Path("//")[1] == std::string());
    ASSERT(psx::Path("//")[2] == std::string());
    ASSERT(psx::Path("//")[3] == std::string());
    ASSERT(psx::Path("///")[0] == std::string());
    ASSERT(psx::Path("///")[1] == std::string());
    ASSERT(psx::Path("///")[2] == std::string());
    ASSERT(psx::Path("///")[3] == std::string());
    ASSERT(psx::Path("/a/b/")[0] == std::string("a"));
    ASSERT(psx::Path("/a/b/")[1] == std::string("b"));
    ASSERT(psx::Path("/a/b/")[2] == std::string(""));
    ASSERT(psx::Path("a/b/")[0] == std::string("a"));
    ASSERT(psx::Path("a/b/")[1] == std::string("b"));
    ASSERT(psx::Path("a/b/")[2] == std::string(""));
    ASSERT(psx::Path("/a/b")[0] == std::string("a"));
    ASSERT(psx::Path("/a/b")[1] == std::string("b"));
    ASSERT(psx::Path("/a/b")[2] == std::string(""));
    ASSERT(psx::Path("a/b")[0] == std::string("a"));
    ASSERT(psx::Path("a/b")[1] == std::string("b"));
    ASSERT(psx::Path("a/b")[2] == std::string(""));

    ASSERT(psx::Path("a")/"b" == psx::Path("a/b"));
    ASSERT(psx::Path("/a")/"b" == psx::Path("/a/b"));
    ASSERT(psx::Path("/")/"b" == psx::Path("/b"));
    ASSERT(psx::Path("")/"b" == psx::Path("b"));

    ASSERT(psx::Path(__FILE__).isRelative());
    ASSERT(psx::Path(__FILE__).absolute().isAbsolute());
    ASSERT(psx::Path(__FILE__).absolute().absolute().isAbsolute());
    ASSERT(psx::Path(__FILE__).absolute().isFile());
    ASSERT(psx::Path(__FILE__).exists());
    ASSERT(psx::Path(__FILE__).isFile());
    ASSERT(!psx::Path(__FILE__).isDirectory());
    ASSERT(psx::Path(__FILE__).parent().isDirectory());
    ASSERT(!psx::Path(__FILE__).parent().isFile());
    ASSERT(!psx::Path(__FILE__).isBlockSpecial());
    ASSERT(!psx::Path(__FILE__).isCharSpecial());
    ASSERT(!psx::Path(__FILE__).isFifo());
    ASSERT(!psx::Path(__FILE__).isSocket());
    ASSERT(!psx::Path(__FILE__).isLink());
    ASSERT((psx::Path(__FILE__).parent()/"test.h").isFile());
    ASSERT((psx::Path(__FILE__).parent() + "test.h").isFile());
    ASSERT((psx::Path(__FILE__).parent() + std::string("test.h")).isFile());

    ASSERT(psx::Path("/test/me/now") == psx::Path("/test//me///now/"));
    ASSERT(psx::Path("test/me/now") == psx::Path("test//me///now/"));
    ASSERT(psx::Path("test/me/now") != psx::Path("/test/me/now"));
    ASSERT(psx::Path("") == psx::Path(""));
    ASSERT(psx::Path("/") == psx::Path("//"));
    ASSERT(psx::Path("a") == psx::Path("a/"));
    ASSERT(static_cast<std::string>(psx::Path("//a//b//c")) == "//a//b//c");
    ASSERT(psx::Path("").isEmpty());
    ASSERT(!psx::Path("/").isEmpty());

    ASSERT(psx::Path(__FILE__).inode() != 0);
    ASSERT(psx::Path(__FILE__).device() == psx::Path(__FILE__).parent().device());
    ASSERT(psx::Path(__FILE__).links() == 1);

    ASSERT(psx::File::open(__FILE__).size() == psx::Path(__FILE__).size());

    ASSERT(psx::Path("/alpha/beta/gamma/")
                .relativeTo("/alpha/beta/gamma")
                .isEmpty());
    ASSERT(psx::Path("/alpha/beta/gamma")
                .relativeTo("/alpha/beta/gamma")
                .isEmpty());
    ASSERT(psx::Path("/alpha/beta/gamma/")
                .relativeTo("/alpha/beta/gamma/")
                .isEmpty());
    ASSERT(psx::Path("/alpha/beta/gamma")
                .relativeTo("/alpha/beta/gamma/")
                .isEmpty());

    ASSERT(psx::Path("/alpha/beta").relativeTo("/alpha/beta/gamma") ==
            psx::Path(".."));
    ASSERT(psx::Path("/alpha/beta/").relativeTo("/alpha/beta/gamma") ==
            psx::Path(".."));
    ASSERT(psx::Path("/alpha/beta").relativeTo("/alpha/beta/gamma/") ==
            psx::Path(".."));
    ASSERT(psx::Path("/alpha/beta/").relativeTo("/alpha/beta/gamma/") ==
            psx::Path(".."));

    ASSERT(psx::Path("/alpha/beta/gamma").relativeTo("/alpha/beta") ==
            psx::Path("gamma"));
    ASSERT(psx::Path("/alpha/beta/gamma/").relativeTo("/alpha/beta") ==
            psx::Path("gamma"));
    ASSERT(psx::Path("/alpha/beta/gamma").relativeTo("/alpha/beta/") ==
            psx::Path("gamma"));
    ASSERT(psx::Path("/alpha/beta/gamma/").relativeTo("/alpha/beta/") ==
            psx::Path("gamma"));

    ASSERT(psx::Path("/alpha/beta/gamma/delta/epsilon/theta")
                .relativeTo("/alpha/beta/gamma/phi/zeta/pi/omicron") ==
            psx::Path("../../../../delta/epsilon/theta"));
    ASSERT(psx::Path("/alpha/beta/gamma/delta/epsilon/theta/")
                .relativeTo("/alpha/beta/gamma/phi/zeta/pi/omicron") ==
            psx::Path("../../../../delta/epsilon/theta"));
    ASSERT(psx::Path("/alpha/beta/gamma/delta/epsilon/theta")
                .relativeTo("/alpha/beta/gamma/phi/zeta/pi/omicron/") ==
            psx::Path("../../../../delta/epsilon/theta"));
    ASSERT(psx::Path("/alpha/beta/gamma/delta/epsilon/theta/")
                .relativeTo("/alpha/beta/gamma/phi/zeta/pi/omicron/") ==
            psx::Path("../../../../delta/epsilon/theta"));

    auto workingDir = (psx::Path("bin")/"testPath").mkdirs();
    auto tinyFile = workingDir/"tiny.txt";
    auto smallFile = workingDir/"small.txt";
    auto linkDir = (workingDir/"links").mkdirs();
    auto tinyLink = linkDir/"tiny.txt";
    auto smallRenamed = linkDir/"smallFile.txt";
    
    //psx::Time before = psx::Time::now();
    {psx::File::open(tinyFile, psx::File::Text, psx::File::ReadWrite).write("tiny");}
    {psx::File::open(smallFile, psx::File::Text, psx::File::ReadWrite).write("small");}
    ASSERT(smallFile.isFile());
    ASSERT(!smallRenamed.exists());
    smallFile.rename(smallRenamed);
    ASSERT(smallRenamed.isFile());
    ASSERT(!smallFile.exists());
    ASSERT(psx::File::open(smallRenamed).read() == "small");
    psx::Time after = psx::Time::now();
    tinyLink.symlink("../tiny.txt");
    ASSERT(tinyLink.readLink() == "../tiny.txt");
    ASSERT(psx::File::open(tinyLink).read() == "tiny");
    ASSERT(tinyLink.size() == 4);
    ASSERT(tinyLink.blocks() > 0);
    //ASSERT(before <= psx::Time(tinyLink.created()));
    ASSERT(psx::Time(tinyLink.created()) <= after);
    //ASSERT(before <= psx::Time(tinyLink.lastModification()));
    ASSERT(psx::Time(tinyLink.lastModification()) <= after);
    //ASSERT(before <= psx::Time(tinyLink.lastAccess()));
    //ASSERT(after <= psx::Time(tinyLink.lastAccess()));
    //ASSERT(before <= psx::Time(tinyLink.lastStatusChange()));
    ASSERT(psx::Time(tinyLink.lastStatusChange()) <= after);
    ASSERT(tinyLink.userId() == tinyFile.userId());
    ASSERT(tinyLink.groupId() == tinyFile.groupId());
    ASSERT(tinyLink.permissions() == tinyFile.permissions());
    ASSERT(tinyLink.blockSize() == tinyFile.blockSize());

    auto snapshot = workingDir.list(psx::Path::NameOnly, psx::Path::RecursiveListing);
    ASSERT(std::find(snapshot.begin(), snapshot.end(), "smallFile.txt") != snapshot.end());
    ASSERT(std::find(snapshot.begin(), snapshot.end(), "tiny.txt") != snapshot.end());

    workingDir.remove();

    ASSERT(!workingDir.exists());
    ASSERT(psx::Path("a/b") != psx::Path("a"));
    ASSERT(psx::Path("a/b") != psx::Path("a/c"));
    
    ASSERT(!psx::Path("bad/path").isFile());
    ASSERT(!psx::Path("bad/path").isDirectory());
    ASSERT(!psx::Path("bad/path").isBlockSpecial());
    ASSERT(!psx::Path("bad/path").isCharSpecial());
    ASSERT(!psx::Path("bad/path").isFifo());
    ASSERT(!psx::Path("bad/path").isSocket());
    ASSERT(!psx::Path("bad/path").isLink());

    TEST_TRY
        psx::Path("a/b/c").relativeTo("a/b/d");
        FAIL();
    TEST_CATCH_TYPE(psx::Exception)
        ASSERT_IN_CATCH(string(exception.what()).find("Path is not absolute: 'a/b/c'") != string::npos);
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        psx::Path("/a/b/c").relativeTo("a/b/d");
        FAIL();
    TEST_CATCH_TYPE(psx::Exception)
        ASSERT_IN_CATCH(string(exception.what()).find("Path is not absolute: 'a/b/d'") != string::npos);
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    ASSERT(psx::Path("/").relativeTo("//") == psx::Path(""));
    printf("mount: '%s'\n", psx::Path(__FILE__).mount().get().c_str());
END_TESTS
