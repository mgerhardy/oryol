//------------------------------------------------------------------------------
//  schemeRegistryTest.cc
//------------------------------------------------------------------------------
#include "Pre.h"
#include "UnitTest++/src/UnitTest++.h"
#include "Core/Core.h"
#include "IO/FS/FileSystem.h"
#include "IO/Core/schemeRegistry.h"

using namespace Oryol;
using namespace Oryol::_priv;

class TestFS_A : public FileSystem {
    OryolClassDecl(TestFS_A);
    OryolClassCreator(TestFS_A);
public:
    virtual int32 GetType() const {
        return 0;
    };
};
OryolClassImpl(TestFS_A);

class TestFS_B : public TestFS_A {
    OryolClassDecl(TestFS_B);
    OryolClassCreator(TestFS_B);
public:
    virtual int32 GetType() const override {
        return 1;
    };
};
OryolClassImpl(TestFS_B);

class TestFS_C : public TestFS_A {
    OryolClassDecl(TestFS_C);
    OryolClassCreator(TestFS_C);
public:
    virtual int32 GetType() const override {
        return 2;
    };
};
OryolClassImpl(TestFS_C);

TEST(schemeRegistryTest) {
    
    Core::Setup();

    const StringAtom http("http");
    const StringAtom ftp("ftp");
    const StringAtom file("file");
    const StringAtom bla("bla");

    schemeRegistry reg;
    
    // @todo: hrmpf, this is ugly ...
    reg.RegisterFileSystem(http, TestFS_A::Creator());
    reg.RegisterFileSystem(ftp, TestFS_B::Creator());
    reg.RegisterFileSystem(file, TestFS_C::Creator());
    
    CHECK(reg.IsFileSystemRegistered(http));
    CHECK(reg.IsFileSystemRegistered(ftp));
    CHECK(reg.IsFileSystemRegistered(file));
    CHECK(!reg.IsFileSystemRegistered(bla));
    
    Ptr<TestFS_A> fsa = reg.CreateFileSystem(http);
    CHECK(fsa);
    CHECK(fsa->GetType() == 0);
    Ptr<TestFS_B> fsb = reg.CreateFileSystem(ftp);
    CHECK(fsb);
    CHECK(fsb->GetType() == 1);
    Ptr<TestFS_C> fsc = reg.CreateFileSystem(file);
    CHECK(fsc);
    CHECK(fsc->GetType() == 2);

    Core::Discard();
}

