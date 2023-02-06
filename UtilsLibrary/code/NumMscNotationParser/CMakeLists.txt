cmake_minimum_required(VERSION 3.24)
aux_source_directory(. SRC_LIST)
add_library(NumMscNotationParser OBJECT ${SRC_LIST})
install(FILES NumMscNotationParser.h MscAlphabet.h TuneFrequncyConst.h DESTINATION include/NumMscNotationParser)