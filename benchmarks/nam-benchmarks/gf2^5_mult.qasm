OPENQASM 2.0;
include "qelib1.inc";
qreg q[15];
h q[10];
ccz q[4], q[6], q[10];
h q[10];
h q[10];
ccz q[3], q[7], q[10];
h q[10];
h q[10];
ccz q[2], q[8], q[10];
h q[10];
h q[10];
ccz q[1], q[9], q[10];
h q[10];
h q[11];
ccz q[4], q[7], q[11];
h q[11];
h q[11];
ccz q[3], q[8], q[11];
h q[11];
h q[11];
ccz q[2], q[9], q[11];
h q[11];
h q[12];
ccz q[4], q[8], q[12];
h q[12];
h q[12];
ccz q[3], q[9], q[12];
h q[12];
h q[13];
ccz q[4], q[9], q[13];
h q[13];
cx q[13], q[10];
cx q[12], q[14];
cx q[11], q[13];
cx q[10], q[12];
h q[14];
ccz q[4], q[5], q[14];
h q[14];
h q[14];
ccz q[3], q[6], q[14];
h q[14];
h q[14];
ccz q[2], q[7], q[14];
h q[14];
h q[14];
ccz q[1], q[8], q[14];
h q[14];
h q[14];
ccz q[0], q[9], q[14];
h q[14];
h q[13];
ccz q[3], q[5], q[13];
h q[13];
h q[13];
ccz q[2], q[6], q[13];
h q[13];
h q[13];
ccz q[1], q[7], q[13];
h q[13];
h q[13];
ccz q[0], q[8], q[13];
h q[13];
h q[12];
ccz q[2], q[5], q[12];
h q[12];
h q[12];
ccz q[1], q[6], q[12];
h q[12];
h q[12];
ccz q[0], q[7], q[12];
h q[12];
h q[11];
ccz q[1], q[5], q[11];
h q[11];
h q[11];
ccz q[0], q[6], q[11];
h q[11];
h q[10];
ccz q[0], q[5], q[10];
h q[10];
