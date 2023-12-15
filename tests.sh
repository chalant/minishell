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
        echo "Diff: bash minishell"
        echo "$diff_output"
        return 1  # Return 1 for failure (KO)
    fi

}

compare_status() {
    your_status="$1"
    bash_status="$2"

    if [ "$your_status" = "$bash_status" ]; then
        echo -e "\e[32mOK\e[0m"
        return 0  # Return 0 for success (OK)
    else
        echo -e "\e[31mKO\e[0m"
        return 1  # Return 1 for failure (KO)
    fi

}

# Function to run a test case with a timeout
run_test_case() {
    test_command="$1"
    timeout_seconds="$2"

	echo "----------------------------------------------------------"
    echo "\n\e[33mRunning test case:\e[0m \"$test_command\""
	your_output=$(echo "$test_command" | ./minishell | tail -n +2 | head -n -1)
	your_status=$(echo $?)
    bash_output=$(echo "$test_command" | bash)
	bash_status=$(echo $?)
	echo
	echo -n "\e[33mOutput: \e[0m"
    compare_output "$your_output" "$bash_output"
	echo -n "\e[33mStatus: \e[0m"
	compare_status "$your_status" "$bash_status"
	echo
}

# Test case 1: Ensure that the expression on the left of && is displayed
run_test_case "(cat -e | cat -e) < gameplan.txt && echo hello" 5

# Test case 2: Execute heredocs in order and display errors properly
run_test_case "cat -e < a << how | cat -e < b << are | cat -e < c << you" 5

# Test case 3: Check that each file output is distinct
run_test_case "cat -e << how && cat -e << are && cat -e << you" 5

# Test case 4: Display the value of the USER environment variable
run_test_case "export USER=hello && echo \"'\$USER'\" && echo '\"\$USER\"'" 5

# Test case 5: Ensure proper redirection with heredocs
run_test_case "((cat -e | cat -e) | (cat -e | cat -e )) << stop" "" 5

# Test for valid output and redirectons with parenthesis.
run_test_case "((cat -e | cat -e) | (cat -e | cat -e )) < tests.sh" "" 5

# Test case 6: Execute heredocs in order with an intermediate heredoc
run_test_case "((cat -e | cat -e) | (cat -e << yo | cat -e )) << stop" "" 5

# Test case 7: Alternating redirections and arguments
run_test_case "echo hello > a how > b are > c you" 5

# Add more test cases as needed
