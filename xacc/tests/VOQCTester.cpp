#include <gtest/gtest.h>
#include "xacc.hpp"
#include "xacc_service.hpp"
#include "IRTransformation.hpp"

TEST(VOQCTester, checkSimple) 
{
    auto irt = xacc::getIRTransformation("voqc");
    auto compiler = xacc::getCompiler("xasm");
    auto program = compiler->compile(R"(__qpu__ void test_t_t(qreg q) {
      T(q[0]);
      T(q[0]);
    })")->getComposite("test_t_t");
    
    EXPECT_EQ(2, program->nInstructions());
    EXPECT_EQ("T", program->getInstruction(0)->name());
    EXPECT_EQ("T", program->getInstruction(1)->name());

    irt->apply(program, nullptr);

    EXPECT_EQ(1, program->nInstructions());
    EXPECT_EQ("S", program->getInstruction(0)->name());


    program = compiler->compile(R"(__qpu__ void test_t_tdg(qreg q) {
        T(q[0]);
        Tdg(q[0]);
    })")->getComposite("test_t_tdg");

    EXPECT_EQ(2, program->nInstructions());
    EXPECT_EQ("T", program->getInstruction(0)->name());
    EXPECT_EQ("Tdg", program->getInstruction(1)->name());

    irt->apply(program, nullptr);

    EXPECT_EQ(0, program->nInstructions());

    program = compiler->compile(R"(__qpu__ void test_conj_merge(qreg q) {
        H(q[0]);
        T(q[0]);
        H(q[0]);
        X(q[0]);
        H(q[0]);
        T(q[0]);
        H(q[0]);
    })")->getComposite("test_conj_merge");

    irt->apply(program, nullptr);
    std::cout << "HOWDY:\n" << program->toString() << "\n"; 
    EXPECT_EQ(3, program->nInstructions());

    program = compiler->compile(R"(__qpu__ void test_cx_merge(qreg q) {
      CX(q[0],q[1]);
      T(q[1]);
      CX(q[0], q[1]);
      CX(q[1], q[0]);
      T(q[0]);
      CX(q[1],q[0]);
    })")->getComposite("test_cx_merge");
    irt->apply(program, nullptr);
    std::cout << "HOWDY:\n" << program->toString() << "\n"; 

    EXPECT_EQ(3, program->nInstructions());
    EXPECT_EQ("CNOT", program->getInstruction(0)->name());
    EXPECT_EQ("S", program->getInstruction(1)->name());
    EXPECT_EQ("CNOT", program->getInstruction(2)->name());

    program = compiler->compile(R"(__qpu__ void test_rz_merge(qreg q) {
     Rz(q[0],pi/16);
     Rz(q[0], pi/16);
    })")->getComposite("test_rz_merge");
    irt->apply(program, nullptr);
    EXPECT_EQ(1, program->nInstructions());
    EXPECT_EQ("Rz", program->getInstruction(0)->name());
    EXPECT_NEAR(.3927, program->getInstruction(0)->getParameter(0).as<double>(), 1e-3);
    
    auto staq = xacc::getCompiler("staq");
    program = staq->compile(R"(OPENQASM 2.0;
                      include "qelib1.inc";
                      qreg q[4];
                      creg c[4];
                      x q[1];
                      h q[0];
                      ccx q[0],q[1],q[3];
                      x q[0];
                      )")->getComposites()[0];;
    std::cout << "BEFORE:\n" << program->toString() << "\n"; 
    irt->apply(program, nullptr);
    std::cout << "AFTER:\n" << program->toString() << "\n"; 

    // Test unsupported gate:
    program = compiler->compile(R"(__qpu__ void test_unsupported_gates(qreg q) {
     Rx(q[0],pi/16);
     Ry(q[0], pi/16);
    })")->getComposite("test_unsupported_gates");
    irt->apply(program, nullptr);
    EXPECT_EQ(2, program->nInstructions());
    EXPECT_EQ("Rx", program->getInstruction(0)->name());
    EXPECT_EQ("Ry", program->getInstruction(1)->name());
}

int main(int argc, char **argv) 
{
  xacc::Initialize(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  auto ret = RUN_ALL_TESTS();
  xacc::Finalize();
  return ret;
}