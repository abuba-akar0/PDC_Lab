cmake_minimum_required(VERSION 4.1)
project(PDC_Lab C)

set(CMAKE_C_STANDARD 11)

add_executable(Task01 PDC_Lab/Lab01/Task01/InputOutput.c)
add_executable(Task02 PDC_Lab/Lab01/Task02/MatrixMult.c)
add_executable(Task03 PDC_Lab/Lab02/Task01/ThreadFact.c)
add_executable(Task04 PDC_Lab/Lab02/Task02/ThreadArray.c)
add_executable(Task05 PDC_Lab/Lab03/Task01/ProdConsVar.c)
add_executable(Task06 PDC_Lab/Lab03/Task02/TrafficLightVarSimulate.c)