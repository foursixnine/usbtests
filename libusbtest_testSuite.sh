#!/bin/bash
#for i in 8 16 32 64 128 256; do make peformance_transfers MAX_URB=$i; done;
TEST_DATE=$( date +%Y-%m-%d_%H_%M)

mkdir -p results/$(TEST_DATE)

for i in {1..2}; do
  for  test_app in mass_command_test_*; do
    echo "${test_app} 2> /dev/null | tee results/${TEST_DATE}/$(basename ${test_app}).csv"
  done;
done
