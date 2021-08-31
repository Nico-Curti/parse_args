#!/bin/bash

red=$(tput setaf 196)
green=$(tput setaf 10)
reset=$(tput sgr0)

log_failure() {
  echo "${red}Assertion failed: $1{reset}"
}

# Reference: https://github.com/torokmark/assert.sh
assert_eq() {
  local expected="$1"
  local actual="$2"
  local msg="${3-}"

  if [ "$expected" == "$actual" ]; then
    echo "${green}PASSED${reset}"
    return 0
  else
    [ "${#msg}" -gt 0 ] && log_failure "$expected == $actual :: $msg" || true
    return 1
  fi
}



# get the directory of the current bash script (independently by the execution path)
scriptdir=$(dirname $(readlink /proc/$$/fd/255))
pushd $scriptdir > /dev/null

cd $scriptdir/../bin/

echo -n "- No parameters: "
./runtest 2>err.log 1>out.log
assert_eq $? 104 "Assertion failed"



echo -n "- Only bool parameter, long flag: "
./runtest --bool_value 0 2>err.log 1>out.log
assert_eq $? 0 "Assertion failed"
stdout=$(./runtest -b 0 2>err.log)
correct=$(printf "With the boolean DISABLED my variables are:\n- String : This is the default value\n- Integer : 42\n- Double : 3.14\n- Float : 1.23")
if [ ! "$stdout" = "$correct" ]; then
  echo "${red}Assertion failed: incorrect stdout.${reset}"
fi
stdout=$(./runtest -b 1 2>err.log)
correct=$(printf "With the boolean ENABLED my variables are:\n- String : This is the default value\n- Integer : 42\n- Double : 3.14\n- Float : 1.23")
if [ ! "$stdout" = "$correct" ]; then
  echo "${red}Assertion failed: incorrect stdout.${reset}"
fi



echo -n "- Only bool parameter, short flag: "
./runtest -b 0 2>err.log 1>out.log
assert_eq $? 0 "Assertion failed"
stdout=$(./runtest -b 0 2>err.log)
correct=$(printf "With the boolean DISABLED my variables are:\n- String : This is the default value\n- Integer : 42\n- Double : 3.14\n- Float : 1.23")
if [ ! "$stdout" = "$correct" ]; then
  echo "${red}Assertion failed: incorrect stdout.${reset}"
fi
stdout=$(./runtest -b 1 2>err.log)
correct=$(printf "With the boolean ENABLED my variables are:\n- String : This is the default value\n- Integer : 42\n- Double : 3.14\n- Float : 1.23")
if [ ! "$stdout" = "$correct" ]; then
  echo "${red}Assertion failed: incorrect stdout.${reset}"
fi



# NOTE: short flags priority
echo -n "- Only bool parameter, priority short flag: "
./runtest -b 0 --bool_value 1 2>err.log 1>out.log
assert_eq $? 0 "Assertion failed: only bool parameter, long AND short flag"
stdout=$(./runtest -b 0 --bool_value 1 2>err.log)
correct=$(printf "With the boolean DISABLED my variables are:\n- String : This is the default value\n- Integer : 42\n- Double : 3.14\n- Float : 1.23")
if [ ! "$stdout" = "$correct" ]; then
  echo "${red}Assertion failed: incorrect stdout.${reset}"
fi
stdout=$(./runtest -b 1 --bool_value 0 2>err.log)
correct=$(printf "With the boolean ENABLED my variables are:\n- String : This is the default value\n- Integer : 42\n- Double : 3.14\n- Float : 1.23")
if [ ! "$stdout" = "$correct" ]; then
  echo "${red}Assertion failed: incorrect stdout.${reset}"
fi



# NOTE: unknown flags do not raise errors since they are ignored by "get" function!
echo -n "- Unknown flags: "
./runtest -b 0 -c 0 2>err.log 1>out.log
assert_eq $? 0 "Assertion failed: unknown flag ignored since not called by get"
stdout=$(./runtest -b 0 -c 0 2>err.log)
correct=$(printf "With the boolean DISABLED my variables are:\n- String : This is the default value\n- Integer : 42\n- Double : 3.14\n- Float : 1.23")
if [ ! "$stdout" = "$correct" ]; then
  echo "${red}Assertion failed: incorrect stdout.${reset}"
fi
stdout=$(./runtest -b 1 -c 0 2>err.log)
correct=$(printf "With the boolean ENABLED my variables are:\n- String : This is the default value\n- Integer : 42\n- Double : 3.14\n- Float : 1.23")
if [ ! "$stdout" = "$correct" ]; then
  echo "${red}Assertion failed: incorrect stdout.${reset}"
fi



echo -n "- Variable assignement: "
./runtest -b 0 -s "1" -i 2 -d 3 -f 4 2>err.log 1>out.log
assert_eq $? 0 "Assertion failed"
stdout=$(./runtest -b 0 -s "1" -i 2 -d 3 -f 4 2>err.log)
correct=$(printf "With the boolean DISABLED my variables are:\n- String : 1\n- Integer : 2\n- Double : 3\n- Float : 4")
if [ ! "$stdout" = "$correct" ]; then
  echo "${red}Assertion failed: incorrect stdout.${reset}"
fi
stdout=$(./runtest -b 1 -s "1" -i 2 -d 3 -f 4 2>err.log)
correct=$(printf "With the boolean ENABLED my variables are:\n- String : 1\n- Integer : 2\n- Double : 3\n- Float : 4")
if [ ! "$stdout" = "$correct" ]; then
  echo "${red}Assertion failed: incorrect stdout.${reset}"
fi

popd > /dev/null
