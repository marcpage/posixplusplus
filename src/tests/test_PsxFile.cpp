#include "../inc/PsxFile.h"
#include "test.h"

START_TESTS

    TEST_TRY
        psx::File().write("test");
        FAIL();
    TEST_CATCH_TYPE(psx::Exception)
        ASSERT_IN_CATCH(string(exception.what()).find("!_readOnly") != string::npos);
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        auto original = psx::File::open(__FILE__, psx::File::Text,  psx::File::ReadOnly);
        auto me = std::move(original);
        auto firstLine = me.readLine(0, psx::File::FromHere, 2);
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
            psx::File::open(path, psx::File::Text,  psx::File::ReadWrite).write("test");
        }

        const auto contents = psx::File::open(path).read();
        const auto partial_contents = psx::File::open(path).read(2, psx::File::FromEnd);
        const auto line = psx::File::open(path).readLine();

        ASSERT(psx::File::open(path).size() == 4);
        ASSERT(contents == "test");
        ASSERT(partial_contents == "st");
        ASSERT(line == "test");
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        psx::File::out().write("testing stdout ... ");
        psx::File::out().flush();
        psx::File::err().write("testing stderr ...\n");
        psx::File::err().flush();
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        psx::File::in().write("testing stdout ... ");
        FAIL();
    TEST_CATCH_TYPE(psx::Exception)
        ASSERT_IN_CATCH(string(exception.what()).find("!_readOnly") != string::npos);
    END_TEST_TRY

    TEST_TRY
        char buffer[8];
        psx::File::open(__FILE__).read(buffer, sizeof(buffer));
        ASSERT(string(buffer, sizeof(buffer)) == "#include");
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        const string path = "bin/PsxFile/test2.dat";

        {
            psx::File::open(path, psx::File::Text,  psx::File::ReadWrite).write("test\r\ntest");
        }

        ASSERT(psx::File::open(path).readLine(0, psx::File::FromHere, 1) == "test\r\n");
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        ASSERT(psx::File::open(__FILE__).move(1).move(2).read(5) == "clude");
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        auto file = psx::File::open(__FILE__);
        ErrnoOnNegative(::fseeko(file, 1, SEEK_SET));
        ASSERT(file.read(7) == "include");
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        const auto file = psx::File::open(__FILE__);
        ErrnoOnNegative(::fseeko(file, 1, SEEK_SET));
        ASSERT(file.read(7) == "include");
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

END_TESTS

