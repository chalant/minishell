#!/bin/bash

# Function to compare the output of your program and the bash command
compare_output() {
    your_output="$1"
    bash_output="$2"

    diff_output=$(diff <(echo "$bash_output") <(echo "$your_output"))
    if [ -z "$diff_output" ]; then
        echo -e "\e[32mOK\e[0m"
        return 0  # Return 0 for success (OK)
    else
        echo -e "\e[31mKO\e[0m"
        echo "\e[31mDiff: \e[0m: bash minishell"
        echo "$diff_output"
        return 1  # Return 1 for failure (KO)
    fi

}

compare_status() {
    your_status="$1"
    bash_status="$2"

    if [ "$your_status" = "$bash_status" ]; then
        echo -e " \e[32mOK\e[0m"
        return 0  # Return 0 for success (OK)
    else
        echo -e " \e[31mKO\e[0m minishell: $your_status bash: $bash_status"
        return 1  # Return 1 for failure (KO)
    fi

}

# Function to run a test case with a timeout
run_test_case() {
    test_command="$1"
    timeout_seconds="$2"

	echo "----------------------------------------------------------"
	your_output=$(echo "$test_command" | ./minishell | awk 'NR > 1 { lines[NR-1] = $0 } END { for(i=1;i<NR-1;i++) print lines[i] }')
	your_status=$(echo $?)
    bash_output=$(echo "$test_command" | bash)
	bash_status=$(echo $?)
	echo -n "\e[33mOutput: \e[0m $test_command "
    compare_output "$your_output" "$bash_output"
	echo -n "\e[33mStatus: \e[0m $test_command"
	compare_status "$your_status" "$bash_status"
	echo
}

make

# ensure that the expression on the left of && is displayed
run_test_case "(cat -e | cat -e) < gameplan.txt && echo hello" 5

# execute heredocs in order and display errors properly
run_test_case "cat -e < a << how | cat -e < b << are | cat -e < c << you" 5

# ensure that each file output is distinct
run_test_case "cat -e << how && cat -e << are && cat -e << you" 5

# Test case 4: Display the value of the USER environment variable
run_test_case "export USER=hello && echo \"'\$USER'\" && echo '\"\$USER\"'" 5

# ensure nsure proper redirection with heredocs
run_test_case "((cat -e | cat -e) | (cat -e | cat -e )) << stop" "" 5

# Test for valid output and redirectons with parenthesis.
run_test_case "((cat -e | cat -e) | (cat -e | cat -e )) < Makefile"  5

run_test_case "((cat -e && cat -e) | (cat -e && cat -e )) < Makefile" 5
run_test_case "((cat -e && cat -e) && (cat -e && cat -e )) < Makefile" 5
run_test_case "((cat -e && cat -e) || (cat -e || cat -e )) < Makefile" 5

run_test_case "((cat -e && cat -e) < fail | (cat -e && cat -e )) < Makefile" 5
run_test_case "((cat -e || cat -e) < fail | (cat -e && cat -e )) < Makefile" 5
run_test_case "((cat -e && cat -e) < fail | (cat -e || cat -e )) < Makefile" 5
run_test_case "((cat -e && cat -e < fail) | (cat -e || cat -e )) < fail"

run_test_case "(cat -e && cat -e | cat -e || cat -e ) < fail" 5
run_test_case "(cat -e && cat -e | cat -e && cat -e ) < Makefile" 5

run_test_case "((cat -e && cat -e) | (cat -e || cat -e ) < fail) < Makefile" 5

# execute heredocs in order with an intermediate heredoc
run_test_case "((cat -e | cat -e) | (cat -e << yo | cat -e )) << stop" 5

# test alternating redirections and arguments
run_test_case "echo hello > a how > b are > c you" 5

# make sure the right command is executed on fail.
run_test_case "cat -e fail || echo hello" 5

# failing tests: might need to fix this
#------------------------------------------------
run_test_case "< gameplan.txt | cat -e"
run_test_case "< gameplan.txt | < fail && < fail"
run_test_case "< fail"

run_test_case "((cat -e && cat -e) < fail || (cat -e || cat -e )) < Makefile" 5
run_test_case "((cat -e && cat -e) || (cat -e || cat -e ) < fail ) < Makefile" 5
run_test_case "((cat -e && cat -e) < fail && (cat -e && cat -e )) < Makefile" 5
run_test_case "(cat -e < fail || cat -e ) < Makefile" 5
run_test_case "(cat -e || cat -e < fail ) < Makefile" 5
run_test_case "(cat -e < fail && cat -e ) < Makefile" 5

