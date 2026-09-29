# CMake generated Testfile for 
# Source directory: /home/jackw/lab02_Crump_Phillips/test
# Build directory: /home/jackw/lab02_Crump_Phillips/build-renode/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(simulate_mytest "/home/jackw/.local/opt/renode/renode" "--disable-gui" "--port" "-2" "--pid-file" "renode.pid" "--console" "-e" "\$ELF=@/home/jackw/lab02_Crump_Phillips/build-renode/test/mytest.elf; \$WORKING=@/home/jackw/lab02_Crump_Phillips; include @/home/jackw/lab02_Crump_Phillips/simulate.resc; start")
set_tests_properties(simulate_mytest PROPERTIES  TIMEOUT "30" _BACKTRACE_TRIPLES "/home/jackw/lab02_Crump_Phillips/test/CMakeLists.txt;75;add_test;/home/jackw/lab02_Crump_Phillips/test/CMakeLists.txt;0;")
