#include "inject/Test.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/**
 * This program should be used with the test case when
 * output comparison is not used.
 *
 * It compares the output message generated by the test
 * and returns 0 if the test is correct or 1 when the
 * test is wrong.
 */

int main(int argc, char const *argv[]) {
	string test_in(argv[1]);    /* This stream reads from test's input file   */
	string test_out(argv[2]);   /* This stream reads from test's output file  */
	ifstream user_out(argv[3]); /* This stream reads from user's output file  */

	string user_out_text((std::istreambuf_iterator<char>(user_out)), std::istreambuf_iterator<char>());

	return user_out_text.find(MSG_OK) != string::npos ? 0 : 1;
}
