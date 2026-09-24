add_test([=[RAMReader.ReaderDontThrowException]=]  [==[/home/iron/Proyectos/C++/Agent-monitor/build/tests/agent_monitor_tests]==] [==[--gtest_filter=RAMReader.ReaderDontThrowException]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[RAMReader.ReaderDontThrowException]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[/home/iron/Proyectos/C++/Agent-monitor/tests/ram_reader_test.cpp:4]==]
    WORKING_DIRECTORY [==[/home/iron/Proyectos/C++/Agent-monitor/build/tests]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
add_test([=[RAMReader.TotalmayorThanCero]=]  [==[/home/iron/Proyectos/C++/Agent-monitor/build/tests/agent_monitor_tests]==] [==[--gtest_filter=RAMReader.TotalmayorThanCero]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[RAMReader.TotalmayorThanCero]=]
  PROPERTIES
    
    DEF_SOURCE_LINE [==[/home/iron/Proyectos/C++/Agent-monitor/tests/ram_reader_test.cpp:8]==]
    WORKING_DIRECTORY [==[/home/iron/Proyectos/C++/Agent-monitor/build/tests]==]
    SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==]
    
)
set(agent_monitor_tests_TESTS [==[RAMReader.ReaderDontThrowException]==] [==[RAMReader.TotalmayorThanCero]==])
