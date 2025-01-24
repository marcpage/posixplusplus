#include "../inc/PsxFile.h"
#include "test.h"

START_TESTS

    TEST_TRY
        psx::io::File().write("test");
        FAIL();
    TEST_CATCH_TYPE(psx::Exception)
        ASSERT_IN_CATCH(string(exception.what()).find("!_readOnly") != string::npos);
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        auto original = psx::io::File::open(__FILE__, psx::io::File::Text,  psx::io::File::ReadOnly);
        auto me = std::move(original);
        auto firstLine = me.readLine(0, psx::io::File::FromHere, 2);
        auto contents = me.read();
        ASSERT(!me.writeable());
        ASSERT(firstLine.find("#include \"../inc/PsxFile.h\"") == 0);
        ASSERT(contents.find("#include \"../inc/PsxFile.h\"") == string::npos);
        ASSERT(contents.find("goo goo ga ga") != string::npos);
        me.write("help");
        FAIL();
    TEST_CATCH_TYPE(psx::Exception)
        ASSERT_IN_CATCH(string(exception.what()).find("!_readOnly") != string::npos);
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        const string path = "bin/PsxFile/test.dat";

        {
            psx::io::File::open(path, psx::io::File::Text,  psx::io::File::ReadWrite).write("test");
        }

        const auto contents = psx::io::File::open(path).read();
        const auto partial_contents = psx::io::File::open(path).read(2, psx::io::File::FromEnd);
        const auto line = psx::io::File::open(path).readLine();

        ASSERT(psx::io::File::open(path).size() == 4);
        ASSERT(contents == "test");
        ASSERT(partial_contents == "st");
        ASSERT(line == "test");
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        psx::io::File::out().write("testing stdout ... ");
        psx::io::File::out().flush();
        psx::io::File::err().write("testing stderr ...\n");
        psx::io::File::err().flush();
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        psx::io::File::in().write("testing stdout ... ");
        FAIL();
    TEST_CATCH_TYPE(psx::Exception)
        ASSERT_IN_CATCH(string(exception.what()).find("!_readOnly") != string::npos);
    END_TEST_TRY

    TEST_TRY
        char buffer[8];
        psx::io::File::open(__FILE__).read(buffer, sizeof(buffer));
        ASSERT(string(buffer, sizeof(buffer)) == "#include");
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        const string path = "bin/PsxFile/test2.dat";

        {
            psx::io::File::open(path, psx::io::File::Text,  psx::io::File::ReadWrite).write("test\r\ntest");
        }

        ASSERT(psx::io::File::open(path).readLine(0, psx::io::File::FromHere, 1) == "test\r\n");
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        ASSERT(psx::io::File::open(__FILE__).move(1).move(2).read(5) == "clude");
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        auto file = psx::io::File::open(__FILE__);
        ErrnoOnNegative(::fseeko(file, 1, SEEK_SET));
        ASSERT(file.read(7) == "include");
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        const auto file = psx::io::File::open(__FILE__);
        ErrnoOnNegative(::fseeko(file, 1, SEEK_SET));
        ASSERT(file.read(7) == "include");
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

END_TESTS

